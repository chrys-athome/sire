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

#ifndef SIRESEC_PUBPRILOCK_H
#define SIRESEC_PUBPRILOCK_H

#include "lock.h"
#include "publickey.h"

SIRE_BEGIN_HEADER

namespace SireSec
{

class PrivateKey;

/** This is a Lock which uses a public key (PublicKey) to 
    encrypt the data, and a private key (PrivateKey) to 
    decrypt the data
    
    @author Christopher Woods
*/
class SIRESEC_EXPORT PubPriLock : public Siren::Implements<PubPriLock,Lock>
{
public:
    PubPriLock();
    PubPriLock(const PublicKey &public_key, 
               Ciphers::Cipher cipher = Ciphers::DEFAULT);
    PubPriLock(const PublicKey &public_key, 
               const PrivateKey &private_key, 
               Ciphers::Cipher cipher = Ciphers::DEFAULT);
    
    PubPriLock(const PubPriLock &other);
    
    ~PubPriLock();
    
    PubPriLock& operator=(const PubPriLock &other);
    
    bool operator==(const PubPriLock &other) const;
    bool operator!=(const PubPriLock &other) const;
    
    bool hasPrivateKey() const;
    bool hasPublicKey() const;
    
    QString toString() const;
    uint hashCode() const;
    void stream(Siren::Stream &s);

    Ciphers::Cipher cipher() const;

protected:
    void encryptStream(QDataStream &in_stream, QDataStream &out_stream,
                       int nbytes) const;
                       
    void decryptStream(QDataStream &in_stream, QDataStream &out_stream,
                       int nbytes) const;

private:
    /** The public key - this can be freely copied,
        and is widely known as it is only used to encrypt the data.
        It is the private key that must remain secret! */
    PublicKey public_key;
    
    /** The cipher to use for the encryption (the public key
        is used to encrypt the session key - the data is encrypted
        using the specified cipher) */
    Ciphers::Cipher cphr;
};

}

Q_DECLARE_METATYPE( SireSec::PubPriLock )

SIRE_EXPOSE_CLASS( SireSec::PubPriLock )

SIRE_END_HEADER

#endif
