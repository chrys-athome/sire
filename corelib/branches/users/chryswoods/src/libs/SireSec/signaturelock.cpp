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

#include "signaturelock.h"
#include "publickey.h"
#include "privatekey.h"

#include "SireSec/errors.h"

#include "Siren/errors.h"
#include "Siren/stream.h"

#include <QDebug>

using namespace SireSec;
using namespace Siren;

static const RegisterObject<SignatureLock> r_signature_lock;

/** Null constructor */
SignatureLock::SignatureLock() 
              : Implements<SignatureLock,Lock>(), mactype( MACTypes::DEFAULT )
{}

/** Construct with just a public key (for encryption only) */
SignatureLock::SignatureLock(const PublicKey &key, MACTypes::MACType mac)
           : Implements<SignatureLock,Lock>(),
             public_key(key), mactype(mac)
{}

/** Construct with both a public and private key (for encryption and 
    decryption - note that these don't have to match!) */
SignatureLock::SignatureLock(const PublicKey &pub_key, 
                             const PrivateKey &pri_key, 
                             MACTypes::MACType mac)
           : Implements<SignatureLock,Lock>(pri_key),
             public_key(pub_key), mactype(mac)
{}

/** Copy constructor */
SignatureLock::SignatureLock(const SignatureLock &other)
              : Implements<SignatureLock,Lock>(other),
                public_key(other.public_key), mactype(other.mactype)
{}

/** Destructor */
SignatureLock::~SignatureLock()
{}

/** Copy assignment operator */
SignatureLock& SignatureLock::operator=(const SignatureLock &other)
{
    if (this != &other)
    {
        super::operator=(other);
        public_key = other.public_key;
        mactype = other.mactype;
    }
    
    return *this;
}

/** Comparison operator */
bool SignatureLock::operator==(const SignatureLock &other) const
{
    return public_key == other.public_key and
           mactype == other.mactype and
           super::operator==(other);
}

/** Comparison operator */
bool SignatureLock::operator!=(const SignatureLock &other) const
{
    return not SignatureLock::operator==(other);
}

/** Return whether or not this has a valid private key */
bool SignatureLock::hasPrivateKey() const
{
    return Lock::hasKey();
}

/** Return whether or not this has a valid public key */
bool SignatureLock::hasPublicKey() const
{
    return public_key.isValid();
}

QString SignatureLock::toString() const
{
    if (hasPublicKey())
    {
        if (hasPrivateKey())
            return QObject::tr("SignatureLock( open for signing and verification )");
        else
            return QObject::tr("SignatureLock( open for verification only )");
    }
    else if (hasPrivateKey())
    {
        return QObject::tr("SignatureLock( open for signing only )");
    }
    else
        return QObject::tr("SignatureLock::null");
}

uint SignatureLock::hashCode() const
{
    return qHash( SignatureLock::typeName() );
}

void SignatureLock::stream(Siren::Stream &s)
{
    s.assertVersion<SignatureLock>(1);
    
    Schema schema = s.item<SignatureLock>();
    
    schema.data("public_key") & public_key;
    
    QString mac;
    
    if (s.isSaving())
        mac = macTypeString(mactype);
        
    schema.data("cipher") & mac;
    
    if (s.isLoading())
        mactype = stringToMACType(mac);
    
    super::stream( schema.base() );
}

/** Return the MAC algorithm used to hash the data. */
MACTypes::MACType SignatureLock::macType() const
{
    return mactype;
}

void SignatureLock::sign(QDataStream &in_stream, QDataStream &out_stream) const
{
    this->encrypt(in_stream, out_stream);
}

void SignatureLock::verify(QDataStream &in_stream, QDataStream &out_stream) const
{
    this->decrypt(in_stream, out_stream);
}

QByteArray SignatureLock::signString(const QString &string) const
{
    return this->encryptString(string);
}

QString SignatureLock::verifyString(const QByteArray &string) const
{
    return this->decryptString(string);
}

QByteArray SignatureLock::sign(const QByteArray &data) const
{
    return this->encrypt(data);
}

QByteArray SignatureLock::verify(const QByteArray &data) const
{
    return this->decrypt(data);
}

void SignatureLock::decryptStream(QDataStream &in_stream, QDataStream &out_stream,
                                  int nbytes) const
{
    assertReadingStream(in_stream);
    assertWritingStream(out_stream);
    
    if (public_key.d.get() == 0)
        throw SireSec::missing_key( QObject::tr(
                "It is not possible to verify the signature without "
                "providing a public key!"), CODELOC );
    
    public_key.assertAvailableToThisThread();
    
    //create an envelope to verify the signature
    CRYPT_ENVELOPE sign_envelope = Crypt::createAutoFormatEnvelope();

    try
    {
        //set the size of the data to be read
        if (nbytes > 0)
            cryptSetAttribute( sign_envelope, CRYPT_ENVINFO_DATASIZE, nbytes );

        //process the data - providing the public signature key when required
        Crypt::processThroughEnvelope(sign_envelope,
                                      in_stream, out_stream );

        int status = cryptSetAttribute( sign_envelope, CRYPT_ENVINFO_SIGNATURE, 
                                        public_key.d->crypt_certificate );

        Crypt::assertValidStatus(status, QUICK_CODELOC);

        int sig_correct(0);
        status = cryptGetAttribute( sign_envelope, CRYPT_ENVINFO_SIGNATURE_RESULT,
                                    &sig_correct );

        Crypt::assertValidStatus(status, QUICK_CODELOC);
        Crypt::assertValidStatus(sig_correct, QUICK_CODELOC);
                           
        cryptDestroyEnvelope(sign_envelope);
    }
    catch(...)
    {
        cryptDestroyEnvelope(sign_envelope);
        throw;
    }
}

