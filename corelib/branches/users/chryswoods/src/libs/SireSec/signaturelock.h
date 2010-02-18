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

#ifndef SIRESEC_SIGNATURELOCK_H
#define SIRESEC_SIGNATURELOCK_H

#include "lock.h"
#include "publickey.h"

SIRE_BEGIN_HEADER

namespace SireSec
{

class PrivateKey;

/** This is a Lock that can be used to sign data
    (using a PrivateKey), or to check a signature
    (using the corresponding PublicKey). Note that
    this does not encrypt the data
    
    @author Christopher Woods
*/
class SIRESEC_EXPORT SignatureLock : public Siren::Implements<SignatureLock,Lock>
{
public:
    SignatureLock();
    SignatureLock(const PublicKey &public_key, 
                  MACTypes::MACType mactype = MACTypes::DEFAULT);
    SignatureLock(const PublicKey &public_key, 
                  const PrivateKey &private_key, 
                  MACTypes::MACType mactype = MACTypes::DEFAULT);
    
    SignatureLock(const SignatureLock &other);
    
    ~SignatureLock();
    
    SignatureLock& operator=(const SignatureLock &other);
    
    bool operator==(const SignatureLock &other) const;
    bool operator!=(const SignatureLock &other) const;
    
    bool hasPrivateKey() const;
    bool hasPublicKey() const;
    
    QString toString() const;
    uint hashCode() const;
    void stream(Siren::Stream &s);

    MACTypes::MACType macType() const;

protected:
    void encryptStream(QDataStream &in_stream, QDataStream &out_stream,
                       int nbytes) const;
                       
    void decryptStream(QDataStream &in_stream, QDataStream &out_stream,
                       int nbytes) const;

private:
    /** The public key - this can be freely copied,
        and is widely known as it is only used to verify 
        the signature - it is the private key, used to sign
        the data, that must be private  */
    PublicKey public_key;
    
    /** The MAC type used to hash the data */
    MACTypes::MACType mactype;
};

}

Q_DECLARE_METATYPE( SireSec::SignatureLock )

SIRE_EXPOSE_CLASS( SireSec::SignatureLock )

SIRE_END_HEADER

#endif
