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

#ifndef SIRESEC_PUBLICKEY_H
#define SIRESEC_PUBLICKEY_H

#include <boost/shared_ptr.hpp>

#include "key.h"

SIRE_BEGIN_HEADER

namespace SireSec
{

namespace Crypt{ class CertContext; }

class PubPriLock;
class PrivateKey;

/** This is a public key, used to encrypt data in a PubPriLock
    (a lock that uses a public, widely known key to encrypt
     data, and a private, secret key to decrypt data)
     
    @author Christopher Woods
*/
class SIRESEC_EXPORT PublicKey : public Siren::Implements<PublicKey,Key>
{
public:
    PublicKey();
    
    PublicKey(const PublicKey &other);
    
    ~PublicKey();
    
    PublicKey& operator=(const PublicKey &other);
    
    bool operator==(const PublicKey &other) const;
    bool operator!=(const PublicKey &other) const;
    
    QString toString() const;
    uint hashCode() const;
    void stream(Siren::Stream &s);
    
    bool isValid() const;
    
    bool availableToThisThread() const;

protected:
    friend class PubPriLock;
    friend class PrivateKey;
    PublicKey(const boost::shared_ptr<Crypt::CertContext> &d);
    
private:
    boost::shared_ptr<Crypt::CertContext> d;
};

}

Q_DECLARE_METATYPE( SireSec::PublicKey )

SIRE_EXPOSE_CLASS( SireSec::PublicKey )

SIRE_END_HEADER

#endif
