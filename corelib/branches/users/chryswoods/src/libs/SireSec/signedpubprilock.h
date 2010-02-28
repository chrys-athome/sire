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

#ifndef SIRESEC_SIGNEDPUBPRILOCK_H
#define SIRESEC_SIGNEDPUBPRILOCK_H

#include "lock.h"
#include "publickey.h"
#include "privatekey.h"

SIRE_BEGIN_HEADER

namespace SireSec
{

/** This is a Lock which uses a public key (PublicKey) to 
    encrypt the data, and a private key (PrivateKey) to 
    decrypt the data. In addition, it uses another PrivateKey
    to sign the data, and another PublicKey to verify the
    data. This is useful, as a message can be encrypted,
    so that it can only be read by the intended recipient, 
    who can use the signature to ensure that the message
    has not been tampered with, and definitely comes from
    the specified sender
    
    @author Christopher Woods
*/
class SIRESEC_EXPORT SignedPubPriLock : public Siren::Implements<SignedPubPriLock,Lock>
{
public:
    SignedPubPriLock();
    SignedPubPriLock(const PublicKey &encryption_public_key,
                     const PrivateKey &signing_private_key,
                     Ciphers::Cipher cipher = Ciphers::DEFAULT,
                     MACTypes::MACType mactype = MACTypes::DEFAULT);
         
    SignedPubPriLock(const PrivateKey &encryption_private_key,
                     const PublicKey &signing_public_key);                     
                                             
    SignedPubPriLock(const PublicKey &encryption_public_key, 
                     const PrivateKey &signing_private_key,
                     const PrivateKey &encryption_private_key,
                     const PublicKey &signing_public_key, 
                     Ciphers::Cipher cipher = Ciphers::DEFAULT,
                     MACTypes::MACType mactype = MACTypes::DEFAULT);
    
    SignedPubPriLock(const SignedPubPriLock &other);
    
    ~SignedPubPriLock();
    
    SignedPubPriLock& operator=(const SignedPubPriLock &other);
    
    bool operator==(const SignedPubPriLock &other) const;
    bool operator!=(const SignedPubPriLock &other) const;
    
    bool hasEncryptionPrivateKey() const;
    bool hasEncryptionPublicKey() const;
    
    bool hasSigningPrivateKey() const;
    bool hasSigningPublicKey() const;
    
    bool canEncrypt() const;
    bool canDecrypt() const;
    
    QString toString() const;
    uint hashCode() const;
    void stream(Siren::Stream &s);

    Ciphers::Cipher cipher() const;
    MACTypes::MACType macType() const;

protected:
    void encryptStream(QDataStream &in_stream, QDataStream &out_stream,
                       int nbytes) const;
                       
    void decryptStream(QDataStream &in_stream, QDataStream &out_stream,
                       int nbytes) const;

private:
    /** The encryption public key - this can be freely copied,
        and is widely known as it is only used to encrypt the data.
        It is the private key that must remain secret! */
    PublicKey encrypt_public_key;
    
    /** The signing public key - this can be freely copied and
        is used to verify the signature */
    PublicKey sign_public_key;
    
    /** The signing private key - this is used to sign the message */
    PrivateKey sign_private_key;
    
    /** The cipher to use for the encryption (the public key
        is used to encrypt the session key - the data is encrypted
        using the specified cipher) */
    Ciphers::Cipher cphr;
    
    /** The algorithm used for message authentication (used for signing) */
    MACTypes::MACType mactype;
};

}

Q_DECLARE_METATYPE( SireSec::SignedPubPriLock )

SIRE_EXPOSE_CLASS( SireSec::SignedPubPriLock )

SIRE_END_HEADER

#endif
