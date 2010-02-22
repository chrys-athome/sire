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

#include <QThreadStorage>
#include <QHash>
#include <QUuid>

#include <boost/bind.hpp>

#include "crypt.h"  // CONDITIONAL_INCLUDE

#include "privatekey.h"
#include "publickey.h"
#include "pubprilock.h"
#include "signaturelock.h"
#include "signedpubprilock.h"

#include "SireSec/errors.h"

#include "Siren/mutex.h"
#include "Siren/errors.h"
#include "Siren/stream.h"

#include <QDebug>

using namespace SireSec;
using namespace Siren;

////
//// Implementation of PrivateKeyData
////

namespace SireSec 
{ 
    namespace detail
    {
        Q_GLOBAL_STATIC( Mutex, privateKeyMutex );
        
        typedef boost::shared_ptr<Crypt::KeyContext> ContextPtr;
        
        static QHash< QUuid,ContextPtr > *global_privkeys(0);
        static QThreadStorage< QHash<QUuid,ContextPtr>* > local_privkeys;

        ContextPtr get_privkey(const QUuid &uid, bool is_local)
        {
            if (is_local)
            {
                if (local_privkeys.hasLocalData())
                {
                    return local_privkeys.localData()->value(uid);
                }
            }
            else
            {
                MutexLocker lkr( privateKeyMutex() );
                
                if (global_privkeys)
                {
                    return global_privkeys->value(uid);
                }
            }
            
            return ContextPtr();
        }
    
        class PrivateKeyData : public boost::noncopyable
        {
        public:
            QUuid uid;
            bool is_local;
        
            PrivateKeyData(const ContextPtr &keycontext, bool thread_local)
                   : boost::noncopyable()
            {
                uid = QUuid::createUuid();
                is_local = thread_local;
            
                if (is_local)
                {
                    if (not local_privkeys.hasLocalData())
                    {
                        local_privkeys.setLocalData( new QHash<QUuid,ContextPtr>() );
                    }
                    
                    local_privkeys.localData()->insert(uid, keycontext);
                }
                else
                {
                    MutexLocker lkr( privateKeyMutex() );
                    
                    if (global_privkeys == 0)
                    {
                        global_privkeys = new QHash<QUuid,ContextPtr>();
                    }
                    
                    global_privkeys->insert(uid, keycontext);
                }
            }
            
            ~PrivateKeyData()
            {
                if (is_local)
                {
                    if (local_privkeys.hasLocalData())
                    {
                        if (local_privkeys.localData()->contains(uid))
                        {
                            local_privkeys.localData()->remove(uid);
                            
                            if (local_privkeys.localData()->isEmpty())
                            {
                                local_privkeys.setLocalData(0);
                            }
                        }
                    }
                }
                else 
                {
                    MutexLocker lkr( privateKeyMutex() );
                    
                    if (global_privkeys)
                    {
                        global_privkeys->remove(uid);
                        
                        if (global_privkeys->isEmpty())
                        {
                            delete global_privkeys;
                            global_privkeys = 0;
                        }
                    }
                }
            }
            
            bool canCopy() const
            {
                if (is_local)
                {
                    if (local_privkeys.hasLocalData())
                    {
                        if (local_privkeys.localData()->contains(uid))
                            return true;
                    }
                    
                    return false;
                }
                
                return true;
            }
            
            bool availableToThisThread() const
            {
                return canCopy();
            }
        };
    }
}

using namespace SireSec::detail;

////
//// Implementation of PrivateKey
////

static const RegisterObject<PrivateKey> r_private_key;

/** Null Constructor */
PrivateKey::PrivateKey() : Implements<PrivateKey,Key>( Key::NonStreamable )
{}

/** Internal constructor */
PrivateKey::PrivateKey(const boost::shared_ptr<Crypt::KeyContext> &context,
                       const Key::Options &options, const QDateTime &expiry)
           : Implements<PrivateKey,Key>(expiry, (options | Key::NonStreamable)),
             d( new PrivateKeyData(context, options.testFlag(Key::LockedToThread)) )
{}

/** Copy constructor */
PrivateKey::PrivateKey(const PrivateKey &other) 
           : Implements<PrivateKey,Key>(other), d(other.d)
{
    if (other.d.get() != 0)
    {
        if (other.d->canCopy())
            d = other.d;
        else
            this->operator=(PrivateKey());
    }
}

/** Destructor */
PrivateKey::~PrivateKey()
{}

/** Copy assignment operator */
PrivateKey& PrivateKey::operator=(const PrivateKey &other)
{
    if (this != &other)
    {
        if (d.get() != other.d.get())
        {
            if (other.d.get())
            {
                if (other.d->canCopy())
                    d = other.d;
                else
                    return this->operator=(PrivateKey());
            }
        }
        
        super::operator=(other);
    }
    
    return *this;
}

/** Comparison operator */
bool PrivateKey::operator==(const PrivateKey &other) const
{
    return d.get() == other.d.get() and super::operator==(other);
}

/** Comparison operator */
bool PrivateKey::operator!=(const PrivateKey &other) const
{
    return not PrivateKey::operator==(other);
}

bool PrivateKey::isValid() const
{
    return d.get() != 0;
}

/** Eventually I'll tie this down to a single thread... */
bool PrivateKey::availableToThisThread() const
{
    if (d.get() != 0)
    {
        return d->availableToThisThread();
    }
    else
        return true;
}

QString PrivateKey::toString() const
{
    if (d.get() == 0)
        return QObject::tr("PrivateKey::null");

    else if (availableToThisThread())
    {
        if (isTemporal())
            return QObject::tr("PrivateKey( available - expires %1 )")
                                    .arg(bestBefore().toString());
        else if (isStreamable())
            return QObject::tr("PrivateKey( available and streamable )");
        else
            return QObject::tr("PrivateKey( available, but not streamable )");
    }
    else
    {
        return QObject::tr("PrivateKey( unavailable to this thread )");
    }
}

uint PrivateKey::hashCode() const
{
    return qHash( PrivateKey::typeName() );
}

void PrivateKey::stream(Siren::Stream &s)
{
    this->assertIsStreamable();
}

/** Generate a public/private key pair, optionally supplying
    the key type (algorithm used by the pair) */
boost::tuple<PublicKey,PrivateKey> 
PrivateKey::generate(Key::Options keyoptions, const QDateTime &expiry,
                     QString label, KeyTypes::KeyType keytype,
                     int keylength)
{
    CRYPT_CONTEXT crypt_context(0);
    CRYPT_CERTIFICATE crypt_certificate(0);
    
    try
    {
        Crypt::SireSec_init();
    
        int status;
    
        //generate the encryption context (this sets the key type)
        switch (keytype)
        {
            case KeyTypes::DEFAULT:
            case KeyTypes::RSA:
                status = cryptCreateContext( &crypt_context, CRYPT_UNUSED, 
                                             CRYPT_ALGO_RSA );
                break;
            
            case KeyTypes::Elgamal:
                status = cryptCreateContext( &crypt_context, CRYPT_UNUSED,
                                             CRYPT_ALGO_ELGAMAL );
                                             
            default:
                throw Siren::unsupported( QObject::tr(
                        "An unsupported public/private key type was requested."),
                            CODELOC );
        }
        
        Crypt::assertValidStatus(status, QUICK_CODELOC);
        
        //now add a label to the key - this is necessary so that this
        //key can be retrieved from a keyset
        if (label.isEmpty())
            label = "unnamed";
        
        QByteArray utf8_label = label.toUtf8();
        
        status = cryptSetAttributeString( crypt_context, CRYPT_CTXINFO_LABEL,
                                          utf8_label.constData(), utf8_label.length() );
                                          
        Crypt::assertValidStatus(status, QUICK_CODELOC);
        
        //if specified, set the key length
        if (keylength > 0)
        {
            //don't allow keys less than 128 bytes
            if (keylength < 128)
                keylength = 128;
                
            status = cryptSetAttribute( crypt_context, CRYPT_CTXINFO_KEYSIZE, keylength );
            
            Crypt::assertValidStatus(status, QUICK_CODELOC);
        }
        
        //finally(!) generate the keys
        status = cryptGenerateKey( crypt_context );
        
        Crypt::assertValidStatus(status, QUICK_CODELOC);

        //create a certificate from the public key - this will
        //allow the certificate to be separated from the private
        //key, and will also allow it to be exported as text or streamed
        status = cryptCreateCert( &crypt_certificate, CRYPT_UNUSED, 
                                  CRYPT_CERTTYPE_CERTIFICATE );
        Crypt::assertValidStatus(status, QUICK_CODELOC);
                                
        //tell cryptlib to create a simple, self-signed certificate
        status = cryptSetAttribute( crypt_certificate, CRYPT_CERTINFO_XYZZY, 1 );
        Crypt::assertValidStatus(status, QUICK_CODELOC);
        
        status = cryptSetAttribute( crypt_certificate, 
                                    CRYPT_CERTINFO_SUBJECTPUBLICKEYINFO,
                                    crypt_context );
        Crypt::assertValidStatus(status, QUICK_CODELOC);

        status = cryptSetAttributeString( crypt_certificate, CRYPT_CERTINFO_COMMONNAME,
                                          utf8_label.constData(), utf8_label.length() );
        Crypt::assertValidStatus(status, QUICK_CODELOC);
        
        status = cryptSignCert( crypt_certificate, crypt_context );
        Crypt::assertValidStatus(status, QUICK_CODELOC);
        
        //create the handle for the key context, and pass it to the private key
        boost::shared_ptr<Crypt::KeyContext> priv_d( new Crypt::KeyContext(crypt_context) );

        //create the handle for the certificate and pass it to the public key
        boost::shared_ptr<Crypt::CertContext> pub_d( new Crypt::CertContext(
                                                                crypt_certificate) );
        
        return boost::tuple<PublicKey,PrivateKey>( PublicKey(pub_d), 
                                                   PrivateKey(priv_d, keyoptions,
                                                              expiry) );
    }
    catch(...)
    {
        cryptDestroyContext(crypt_context);
        cryptDestroyCert(crypt_certificate);
        throw;
    }
}

boost::tuple<PublicKey,PrivateKey>
PrivateKey::generate(QString label, KeyTypes::KeyType keytype, int keylength)
{
    return generate(Key::Unrestricted, QDateTime(), label, keytype, keylength);
}

boost::tuple<PublicKey,PrivateKey> 
PrivateKey::generate(KeyTypes::KeyType keytype, int keylength)
{
    return generate(Key::Unrestricted, QDateTime(), QString::null, keytype, keylength);
}

boost::tuple<PublicKey,PrivateKey> 
PrivateKey::generate(const QDateTime &expiry, QString label, KeyTypes::KeyType keytype,
                     int keylength)
{
    return generate(Key::Unrestricted, expiry, label, keytype, keylength);
}

boost::tuple<PublicKey,PrivateKey> 
PrivateKey::generate(Key::Options keyoptions, QString label,
                     KeyTypes::KeyType keytype, int keylength)
{
    return generate(keyoptions, QDateTime(), label, keytype, keylength);
}

boost::tuple<PublicKey,PrivateKey> 
PrivateKey::generate(const QDateTime &expiry, KeyTypes::KeyType keytype,
                     int keylength)
{
    return generate(Key::Unrestricted, expiry, QString::null, keytype, keylength);
}

boost::tuple<PublicKey,PrivateKey> 
PrivateKey::generate(Key::Options keyoptions, KeyTypes::KeyType keytype,
                     int keylength)
{
    return generate(keyoptions, QDateTime(), QString::null, keytype, keylength);
}

boost::tuple<PublicKey,PrivateKey> 
PrivateKey::generate(Key::Options keyoptions, const QDateTime &expiry, 
                     KeyTypes::KeyType keytype, int keylength)
{
    return generate(keyoptions, expiry, QString::null, keytype, keylength);
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
        
    key.assertAvailableToThisThread();

    ContextPtr context = get_privkey(key.d->uid, key.d->is_local); 

    if (context.get() == 0)
        throw SireSec::missing_key( QObject::tr(
                "It is not possible to decrypt private key protected data with "
                "a null private key!"), CODELOC );
    
    //create an envelope to decrypt the data and decompress the data
    CRYPT_ENVELOPE crypt_envelope = Crypt::createAutoFormatEnvelope();
    CRYPT_ENVELOPE compress_envelope = Crypt::createAutoFormatEnvelope();

    try
    {
        //set the size of the data to be read
        if (nbytes > 0)
            cryptSetAttribute( compress_envelope, CRYPT_ENVINFO_DATASIZE, nbytes );

        //get the private key context
        CRYPT_CONTEXT crypt_context = context->crypt_context;

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

void SignatureLock::encryptStream(QDataStream &in_stream, QDataStream &out_stream,
                                  int nbytes) const
{
    assertReadingStream(in_stream);
    assertWritingStream(out_stream);
    
    if (not this->hasKey())
        throw SireSec::missing_key( QObject::tr(
                "It is not possible to sign this data without "
                "providing a private key!"), CODELOC );
    
    const PrivateKey &key = this->getKey().asA<PrivateKey>();
    
    if (not key.isValid())
        throw Siren::program_bug( QObject::tr(
                "How has an invalid private key been saved?"), CODELOC );
        
    key.assertAvailableToThisThread();

    ContextPtr context = get_privkey(key.d->uid, key.d->is_local); 

    if (context.get() == 0)
        throw SireSec::missing_key( QObject::tr(
                "It is not possible to sign data with "
                "a null private key!"), CODELOC );
                    
    CRYPT_ENVELOPE sign_envelope = Crypt::createDefaultEnvelope();
    
    try
    {
        //set the size of the data to be read
        if (nbytes > 0)
            cryptSetAttribute( sign_envelope, CRYPT_ENVINFO_DATASIZE, nbytes );

        //set the public key
        int status = cryptSetAttribute( sign_envelope, CRYPT_ENVINFO_SIGNATURE,
                                        context->crypt_context );

        Crypt::assertValidStatus(status, QUICK_CODELOC);
              
        //if necessary, set the MAC algorithm
        if (mactype != MACTypes::DEFAULT)
        {
            switch (mactype)
            {
                case MACTypes::HMAC_MD5:
                    status = cryptSetAttribute( sign_envelope, 
                                                CRYPT_OPTION_ENCR_HASH,
                                                CRYPT_ALGO_HMAC_MD5 );
                    break;
                   
                case MACTypes::HMAC_SHA1:
                    status = cryptSetAttribute( sign_envelope, 
                                                CRYPT_OPTION_ENCR_HASH,
                                                CRYPT_ALGO_HMAC_SHA1 );
                    break;
                   
                case MACTypes::HMAC_RIPEMD_160:
                    status = cryptSetAttribute( sign_envelope, 
                                                CRYPT_OPTION_ENCR_HASH,
                                                CRYPT_ALGO_HMAC_RIPEMD160 );
                    break;
                   
                case MACTypes::SHA:
                    status = cryptSetAttribute( sign_envelope, 
                                                CRYPT_OPTION_ENCR_HASH,
                                                CRYPT_ALGO_SHA );
                    break;
                   
                case MACTypes::SHA2:
                    status = cryptSetAttribute( sign_envelope, 
                                                CRYPT_OPTION_ENCR_HASH,
                                                CRYPT_ALGO_SHA2 );
                    break;

                default:
                    status = cryptSetAttribute( sign_envelope,
                                                CRYPT_OPTION_ENCR_HASH,
                                                CRYPT_ALGO_HMAC_MD5 );
                    break;
            }
            
            Crypt::assertValidStatus(status, QUICK_CODELOC);
        }
                                                           
        //process the data
        Crypt::processThroughEnvelope(sign_envelope,
                                      in_stream, out_stream);
        
        cryptDestroyEnvelope(sign_envelope);
    }
    catch(...)
    {
        cryptDestroyEnvelope(sign_envelope);
        throw;
    }
}

void SignedPubPriLock::encryptStream(QDataStream &in_stream, QDataStream &out_stream,
                                     int nbytes) const
{
    assertReadingStream(in_stream);
    assertWritingStream(out_stream);
    
    if (not encrypt_public_key.isValid())
        throw SireSec::missing_key( QObject::tr(
                "Cannot encrypt data if there isn't a valid public key!"), 
                    CODELOC );

    if (not sign_private_key.isValid())
        throw SireSec::missing_key( QObject::tr(
                "Cannot sign the encrypted data if there isn't a valid private key!"),
                    CODELOC );
        
    sign_private_key.assertAvailableToThisThread();

    ContextPtr context = get_privkey(sign_private_key.d->uid, 
                                     sign_private_key.d->is_local); 

    if (context.get() == 0)
        throw SireSec::missing_key( QObject::tr(
                "It is not possible to sign data with "
                "a null private key!"), CODELOC );
                    
    CRYPT_ENVELOPE sign_envelope = Crypt::createDefaultEnvelope();
    CRYPT_ENVELOPE crypt_envelope = Crypt::createDefaultEnvelope();
    CRYPT_ENVELOPE compress_envelope = Crypt::createDefaultEnvelope();
    
    try
    {
        //set the size of the data to be read
        if (nbytes > 0)
            cryptSetAttribute( compress_envelope, CRYPT_ENVINFO_DATASIZE, nbytes );

        cryptSetAttribute( compress_envelope, CRYPT_ENVINFO_COMPRESSION, CRYPT_UNUSED );
        
        //set the public key
        int status = cryptSetAttribute( crypt_envelope, CRYPT_ENVINFO_PUBLICKEY,
                                        encrypt_public_key.d->crypt_certificate );
                                              
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

        //set the public key
        status = cryptSetAttribute( sign_envelope, CRYPT_ENVINFO_SIGNATURE,
                                    context->crypt_context );

        Crypt::assertValidStatus(status, QUICK_CODELOC);
              
        //if necessary, set the MAC algorithm
        if (mactype != MACTypes::DEFAULT)
        {
            switch (mactype)
            {
                case MACTypes::HMAC_MD5:
                    status = cryptSetAttribute( sign_envelope, 
                                                CRYPT_OPTION_ENCR_HASH,
                                                CRYPT_ALGO_HMAC_MD5 );
                    break;
                   
                case MACTypes::HMAC_SHA1:
                    status = cryptSetAttribute( sign_envelope, 
                                                CRYPT_OPTION_ENCR_HASH,
                                                CRYPT_ALGO_HMAC_SHA1 );
                    break;
                   
                case MACTypes::HMAC_RIPEMD_160:
                    status = cryptSetAttribute( sign_envelope, 
                                                CRYPT_OPTION_ENCR_HASH,
                                                CRYPT_ALGO_HMAC_RIPEMD160 );
                    break;
                   
                case MACTypes::SHA:
                    status = cryptSetAttribute( sign_envelope, 
                                                CRYPT_OPTION_ENCR_HASH,
                                                CRYPT_ALGO_SHA );
                    break;
                   
                case MACTypes::SHA2:
                    status = cryptSetAttribute( sign_envelope, 
                                                CRYPT_OPTION_ENCR_HASH,
                                                CRYPT_ALGO_SHA2 );
                    break;

                default:
                    status = cryptSetAttribute( sign_envelope,
                                                CRYPT_OPTION_ENCR_HASH,
                                                CRYPT_ALGO_HMAC_MD5 );
                    break;
            }
            
            Crypt::assertValidStatus(status, QUICK_CODELOC);
        }
                                                           
        //process the data - compress then encrypt
        Crypt::processThroughEnvelopes(compress_envelope, crypt_envelope,
                                       sign_envelope, in_stream, out_stream);
        
        cryptDestroyEnvelope(compress_envelope);
        cryptDestroyEnvelope(crypt_envelope);
        cryptDestroyEnvelope(sign_envelope);
    }
    catch(...)
    {
        cryptDestroyEnvelope(compress_envelope);
        cryptDestroyEnvelope(crypt_envelope);
        cryptDestroyEnvelope(sign_envelope);
        throw;
    }
}

void SignedPubPriLock::decryptStream(QDataStream &in_stream, QDataStream &out_stream,
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
        
    key.assertAvailableToThisThread();

    ContextPtr context = get_privkey(key.d->uid, key.d->is_local); 

    if (context.get() == 0)
        throw SireSec::missing_key( QObject::tr(
                "It is not possible to decrypt private key protected data with "
                "a null private key!"), CODELOC );
    
    if (not sign_public_key.isValid())
        throw SireSec::missing_key( QObject::tr(
                "It is not possible to verify the signature without a valid "
                "public key!"), CODELOC );
    
    //create an envelope to decrypt the data and decompress the data
    CRYPT_ENVELOPE crypt_envelope = Crypt::createAutoFormatEnvelope();
    CRYPT_ENVELOPE compress_envelope = Crypt::createAutoFormatEnvelope();
    CRYPT_ENVELOPE sign_envelope = Crypt::createAutoFormatEnvelope();

    try
    {
        //set the size of the data to be read
        if (nbytes > 0)
            cryptSetAttribute( sign_envelope, CRYPT_ENVINFO_DATASIZE, nbytes );

        //get the private key context
        CRYPT_CONTEXT crypt_context = context->crypt_context;

        //process the data - providing the password when required
        Crypt::processThroughEnvelopes(sign_envelope, crypt_envelope, compress_envelope,
                                       in_stream, out_stream,
                                       boost::bind(supply_key, crypt_context, _1, _2));

        int status = cryptSetAttribute( sign_envelope, CRYPT_ENVINFO_SIGNATURE, 
                                        sign_public_key.d->crypt_certificate );

        Crypt::assertValidStatus(status, QUICK_CODELOC);

        int sig_correct(0);
        status = cryptGetAttribute( sign_envelope, CRYPT_ENVINFO_SIGNATURE_RESULT,
                                    &sig_correct );

        Crypt::assertValidStatus(status, QUICK_CODELOC);
        Crypt::assertValidStatus(sig_correct, QUICK_CODELOC);
        
        cryptDestroyEnvelope(crypt_envelope);
        cryptDestroyEnvelope(compress_envelope);
        cryptDestroyEnvelope(sign_envelope);
    }
    catch(...)
    {
        cryptDestroyEnvelope(crypt_envelope);
        cryptDestroyEnvelope(compress_envelope);
        cryptDestroyEnvelope(sign_envelope);
        throw;
    }
}
