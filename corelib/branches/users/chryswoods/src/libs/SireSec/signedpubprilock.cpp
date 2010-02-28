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

#include "signedpubprilock.h"

#include "SireSec/errors.h"

#include "Siren/errors.h"
#include "Siren/stream.h"

#include <QDebug>

using namespace SireSec;
using namespace Siren;

static const RegisterObject<SignedPubPriLock> r_signed_pub_pri_lock;

/** Null constructor */
SignedPubPriLock::SignedPubPriLock() 
                 : Implements<SignedPubPriLock,Lock>(), 
                   cphr( Ciphers::DEFAULT ),
                   mactype( MACTypes::DEFAULT )
{}

/** Construct with just a public key (for encryption) and private
    key (for signing) - this lock can only be used to encrypt data */
SignedPubPriLock::SignedPubPriLock(const PublicKey &pubkey, 
                                   const PrivateKey &privkey,
                                   Ciphers::Cipher cipher, 
                                   MACTypes::MACType mac)
                 : Implements<SignedPubPriLock,Lock>(),
                   encrypt_public_key(pubkey), 
                   sign_private_key(privkey),
                   cphr(cipher), mactype(mac)
{}

/** Construct with just a private key (for decryption) and
    public key (for signature verification) - this lock can
    only be used to decrypt data */
SignedPubPriLock::SignedPubPriLock(const PrivateKey &privkey,
                                   const PublicKey &pubkey)
                 : Implements<SignedPubPriLock,Lock>(privkey),
                   sign_public_key(pubkey),
                   cphr( Ciphers::DEFAULT ),
                   mactype( MACTypes::DEFAULT )
{}

/** Construct with both a public and private key (for encryption and 
    decryption - note that these don't have to match!) */
SignedPubPriLock::SignedPubPriLock(const PublicKey &encrypt_pub_key, 
                                   const PrivateKey &sign_pri_key,
                                   const PrivateKey &encrypt_pri_key,
                                   const PublicKey &sign_pub_key, 
                                   Ciphers::Cipher cipher,
                                   MACTypes::MACType mac)
                 : Implements<SignedPubPriLock,Lock>(encrypt_pri_key),
                   encrypt_public_key(encrypt_pub_key),
                   sign_public_key(sign_pub_key),
                   sign_private_key(sign_pri_key), 
                   cphr(cipher), mactype(mac)
{}

/** Copy constructor */
SignedPubPriLock::SignedPubPriLock(const SignedPubPriLock &other)
                 : Implements<SignedPubPriLock,Lock>(other),
                   encrypt_public_key(other.encrypt_public_key), 
                   sign_public_key(other.sign_public_key),
                   sign_private_key(other.sign_private_key),
                   cphr(other.cphr), mactype(other.mactype)
{}

/** Destructor */
SignedPubPriLock::~SignedPubPriLock()
{}

/** Copy assignment operator */
SignedPubPriLock& SignedPubPriLock::operator=(const SignedPubPriLock &other)
{
    if (this != &other)
    {
        super::operator=(other);
        encrypt_public_key = other.encrypt_public_key;
        sign_public_key = other.sign_public_key;
        sign_private_key = other.sign_private_key;
        cphr = other.cphr;
        mactype = other.mactype;
    }
    
    return *this;
}

/** Comparison operator */
bool SignedPubPriLock::operator==(const SignedPubPriLock &other) const
{
    return encrypt_public_key == other.encrypt_public_key and
           sign_public_key == other.sign_public_key and
           sign_private_key == other.sign_private_key and
           cphr == other.cphr and mactype == other.mactype and
           super::operator==(other);
}

/** Comparison operator */
bool SignedPubPriLock::operator!=(const SignedPubPriLock &other) const
{
    return not SignedPubPriLock::operator==(other);
}

/** Return whether or not this has a valid private key for encryption */
bool SignedPubPriLock::hasEncryptionPrivateKey() const
{
    return Lock::hasKey() and Lock::getKey().asA<PrivateKey>().isValid();
}

/** Return whether or not this has a valid public key for encryption */
bool SignedPubPriLock::hasEncryptionPublicKey() const
{
    return encrypt_public_key.isValid();
}

/** Return whether or not this has a valid private key for signing */
bool SignedPubPriLock::hasSigningPrivateKey() const
{
    return sign_private_key.isValid();
}

/** Return whether or not this has a valid public key for signing */
bool SignedPubPriLock::hasSigningPublicKey() const
{
    return sign_public_key.isValid();
}
    
/** Return whether or not this lock can be used to encrypt data */
bool SignedPubPriLock::canEncrypt() const
{
    return hasEncryptionPublicKey() and hasSigningPrivateKey();
}

/** Return whether or not this lock can be used to decrypt data */
bool SignedPubPriLock::canDecrypt() const
{
    return hasEncryptionPrivateKey() and hasSigningPublicKey();
}

QString SignedPubPriLock::toString() const
{
    if (canDecrypt() and canEncrypt())
        return QObject::tr("SignedPubPriLock( open for encryption and decryption )");
    
    else if (canDecrypt())
        return QObject::tr("SignedPubPriLock( open for decryption only )");
        
    else if (canEncrypt())
        return QObject::tr("SignedPubPriLock( open for encryption only )");
        
    else
        return QObject::tr("SignedPubPriLock::null");
}

uint SignedPubPriLock::hashCode() const
{
    return qHash( SignedPubPriLock::typeName() );
}

void SignedPubPriLock::stream(Siren::Stream &s)
{
    s.assertVersion<SignedPubPriLock>(1);
    
    Schema schema = s.item<SignedPubPriLock>();
    
    schema.data("encryption_public_key") & encrypt_public_key;
    schema.data("signing_public_key") & sign_public_key;
    
    QString cipher, mac;
    
    if (s.isSaving())
    {
        cipher = cipherString(cphr);
        mac = macTypeString(mactype);
    }
        
    schema.data("cipher") & cipher;
    schema.data("mactype") & mac;
    
    if (s.isLoading())
    {
        cphr = stringToCipher(cipher);
        mactype = stringToMACType(mac);
    }
    
    super::stream( schema.base() );
}

/** Return the cipher used to encrypt the data (only the
    session key is encrypted using public/private keys, the
    rest of the data is encrypted data is encrypted using this 
    cipher */
Ciphers::Cipher SignedPubPriLock::cipher() const
{
    return cphr;
}

/** Return the MAC algorithm used to authenticate the data */
MACTypes::MACType SignedPubPriLock::macType() const
{
    return mactype;
}
