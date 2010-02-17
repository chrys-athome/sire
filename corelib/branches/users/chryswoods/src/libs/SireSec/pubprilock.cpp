/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2010  Christopher Woods
  *
  *  This program is free software; you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation; either version 2 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program; if not, write to the Free Software
  *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  *  For full details of the license please see the COPYING file
  *  that should have come with this distribution.
  *
  *  You can contact the authors via the developer's mailing list
  *  at http://siremol.org
  *
\*********************************************/

#include "crypt.h" // CONDITIONAL_INCLUDE

#include <boost/bind.hpp>

#include "pubprilock.h"
#include "publickey.h"
#include "privatekey.h"

#include "SireSec/errors.h"

#include "Siren/errors.h"
#include "Siren/stream.h"

#include <QDebug>

using namespace SireSec;
using namespace Siren;

static const RegisterObject<PubPriLock> r_pub_pri_lock;

/** Null constructor */
PubPriLock::PubPriLock() : Implements<PubPriLock,Lock>(), cphr( Ciphers::DEFAULT )
{}

/** Construct with just a public key (for encryption only) */
PubPriLock::PubPriLock(const PublicKey &key, Ciphers::Cipher cipher)
           : Implements<PubPriLock,Lock>(),
             public_key(key), cphr(cipher)
{}

/** Construct with both a public and private key (for encryption and 
    decryption - note that these don't have to match!) */
PubPriLock::PubPriLock(const PublicKey &pub_key, 
                       const PrivateKey &pri_key, 
                       Ciphers::Cipher cipher)
           : Implements<PubPriLock,Lock>(pri_key),
             public_key(pub_key), cphr(cipher)
{}

/** Copy constructor */
PubPriLock::PubPriLock(const PubPriLock &other)
           : Implements<PubPriLock,Lock>(other),
             public_key(other.public_key), cphr(other.cphr)
{}

/** Destructor */
PubPriLock::~PubPriLock()
{}

/** Copy assignment operator */
PubPriLock& PubPriLock::operator=(const PubPriLock &other)
{
    if (this != &other)
    {
        super::operator=(other);
        public_key = other.public_key;
        cphr = other.cphr;
    }
    
    return *this;
}

/** Comparison operator */
bool PubPriLock::operator==(const PubPriLock &other) const
{
    return public_key == other.public_key and
           cphr == other.cphr and
           super::operator==(other);
}

/** Comparison operator */
bool PubPriLock::operator!=(const PubPriLock &other) const
{
    return not PubPriLock::operator==(other);
}

/** Return whether or not this has a valid private key */
bool PubPriLock::hasPrivateKey() const
{
    return Lock::hasKey();
}

/** Return whether or not this has a valid public key */
bool PubPriLock::hasPublicKey() const
{
    return public_key.isValid();
}

QString PubPriLock::toString() const
{
    if (hasPublicKey())
    {
        if (hasPrivateKey())
            return QObject::tr("PubPriLock( open for encryption and decryption )");
        else
            return QObject::tr("PubPriLock( open for encryption only )");
    }
    else if (hasPrivateKey())
    {
        return QObject::tr("PubPriLock( open for decryption only )");
    }
    else
        return QObject::tr("PubPriLock::null");
}

uint PubPriLock::hashCode() const
{
    return qHash( PubPriLock::typeName() );
}

void PubPriLock::stream(Siren::Stream &s)
{
    s.assertVersion<PubPriLock>(1);
    
    Schema schema = s.item<PubPriLock>();
    
    schema.data("public_key") & public_key;
    
    QString cipher;
    
    if (s.isSaving())
        cipher = cipherString(cphr);
        
    schema.data("cipher") & cipher;
    
    if (s.isLoading())
        cphr = stringToCipher(cipher);
    
    super::stream( schema.base() );
}

/** Return the cipher used to encrypt the data (only the
    session key is encrypted using public/private keys, the
    rest of the data is encrypted data is encrypted using this 
    cipher */
Ciphers::Cipher PubPriLock::cipher() const
{
    return cphr;
}

void PubPriLock::encryptStream(QDataStream &in_stream, QDataStream &out_stream,
                               int nbytes) const
{
    assertReadingStream(in_stream);
    assertWritingStream(out_stream);
    
    if (not public_key.isValid())
        throw SireSec::missing_key( QObject::tr(
                "Cannot encrypt data if there isn't a valid public key!"), 
                    CODELOC );
                    
    CRYPT_ENVELOPE crypt_envelope = Crypt::createDefaultEnvelope();
    CRYPT_ENVELOPE compress_envelope = Crypt::createDefaultEnvelope();
    
    try
    {
        //set the size of the data to be read
        if (nbytes > 0)
            cryptSetAttribute( compress_envelope, CRYPT_ENVINFO_DATASIZE, nbytes );

        cryptSetAttribute( compress_envelope, CRYPT_ENVINFO_COMPRESSION, CRYPT_UNUSED );
        
        //add integrity checking so that the data cannot be tampered with
        int status = cryptSetAttribute( crypt_envelope,
                                        CRYPT_ENVINFO_INTEGRITY,
                                        CRYPT_INTEGRITY_FULL );

        Crypt::assertValidStatus(status, QUICK_CODELOC);

        //set the public key
        status = cryptSetAttribute( crypt_envelope, CRYPT_ENVINFO_PUBLICKEY,
                                    public_key.d->crypt_certificate );
                                              
        Crypt::assertValidStatus(status, QUICK_CODELOC);
              
        //if necessary, set the cipher
        if (cphr != Ciphers::DEFAULT)
        {
            switch (cphr)
            {
                case Ciphers::AES:
                    status = cryptSetAttribute( crypt_envelope, 
                                                CRYPT_OPTION_ENCR_ALGO,
                                                CRYPT_ALGO_AES );
                    break;

                case Ciphers::Blowfish:
                    status = cryptSetAttribute( crypt_envelope,
                                                CRYPT_OPTION_ENCR_ALGO,
                                                CRYPT_ALGO_BLOWFISH );
                    break;
                    
                case Ciphers::CAST_128:
                    status = cryptSetAttribute( crypt_envelope,
                                                CRYPT_OPTION_ENCR_ALGO,
                                                CRYPT_ALGO_CAST );
                    break;

                case Ciphers::Triple_DES:
                default:
                    status = cryptSetAttribute( crypt_envelope,
                                                CRYPT_OPTION_ENCR_ALGO,
                                                CRYPT_ALGO_3DES );
                    break;
            }
            
            Crypt::assertValidStatus(status, QUICK_CODELOC);
        }
                                                           
        //process the data - compress then encrypt
        Crypt::processThroughEnvelopes(compress_envelope, crypt_envelope, 
                                       in_stream, out_stream);
        
        cryptDestroyEnvelope(compress_envelope);
        cryptDestroyEnvelope(crypt_envelope);
    }
    catch(...)
    {
        cryptDestroyEnvelope(compress_envelope);
        cryptDestroyEnvelope(crypt_envelope);
        throw;
    }
}

static void supply_key(CRYPT_CONTEXT crypt_context,
                       CRYPT_ENVELOPE crypt_envelope, int ntries)
{
    int status = cryptSetAttribute( crypt_envelope, CRYPT_ENVINFO_PRIVATEKEY,
                                    crypt_context );
        
    if (status == CRYPT_OK)
        return;

    else if (status == CRYPT_ERROR_WRONGKEY)
        throw SireSec::invalid_key( QObject::tr(
                "The supplied private key is incorrect. Cannot decrypt the data!"),
                     CODELOC );
               
    else if (ntries > 5)
        Crypt::assertValidStatus(status, QUICK_CODELOC);
}

void PubPriLock::decryptStream(QDataStream &in_stream, QDataStream &out_stream,
                               int nbytes) const
{
    assertReadingStream(in_stream);
    assertWritingStream(out_stream);
    
    if (not this->hasKey())
        throw SireSec::missing_key( QObject::tr(
                "It is not possible to decrypt this data without "
                "providing a private key!"), CODELOC );
    
    const PrivateKey &key = this->getKey().asA<PrivateKey>();
    
    if (not key.isValid())
        throw Siren::program_bug( QObject::tr(
                "How has an invalid private key been saved?"), CODELOC );
    
    //create an envelope to decrypt the data and decompress the data
    CRYPT_ENVELOPE crypt_envelope = Crypt::createAutoFormatEnvelope();
    CRYPT_ENVELOPE compress_envelope = Crypt::createAutoFormatEnvelope();

    try
    {
        //set the size of the data to be read
        if (nbytes > 0)
            cryptSetAttribute( compress_envelope, CRYPT_ENVINFO_DATASIZE, nbytes );

        //get the private key context
        CRYPT_CONTEXT crypt_context = key.d->crypt_context;

        //process the data - providing the password when required
        Crypt::processThroughEnvelopes(crypt_envelope, compress_envelope,
                                       in_stream, out_stream,
                                       boost::bind(supply_key, crypt_context, _1, _2));

        cryptDestroyEnvelope(crypt_envelope);
        cryptDestroyEnvelope(compress_envelope);
    }
    catch(...)
    {
        cryptDestroyEnvelope(crypt_envelope);
        cryptDestroyEnvelope(compress_envelope);
        throw;
    }
}
