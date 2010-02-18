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

#ifndef SIRESEC_PRIVATEKEY_H
#define SIRESEC_PRIVATEKEY_H

#include <boost/tuple/tuple.hpp>
#include <boost/shared_ptr.hpp>

#include "key.h"

SIRE_BEGIN_HEADER

namespace SireSec
{

namespace Crypt{ class KeyContext; }
namespace detail{ class PrivateKeyData; }

class PubPriLock;
class PublicKey;

/** This is a private key, used to decrypt data using
    a PubPriLock. This lock uses a pubic, widely known
    key (PublicKey) to encrypt data, and a private,
    secret key (PrivateKey) to decrypt data
    
    @author Christopher Woods
*/
class SIRESEC_EXPORT PrivateKey : public Siren::Implements<PrivateKey,Key>
{
public:
    PrivateKey();
    
    PrivateKey(const PrivateKey &other);
    
    ~PrivateKey();
    
    PrivateKey& operator=(const PrivateKey &other);
    
    bool operator==(const PrivateKey &other) const;
    bool operator!=(const PrivateKey &other) const;
    
    bool isValid() const;
    
    static boost::tuple<PublicKey,PrivateKey>
    generate(QString label, KeyTypes::KeyType keytype = KeyTypes::DEFAULT,
             int keylength = 0);

    static boost::tuple<PublicKey,PrivateKey> 
    generate(KeyTypes::KeyType keytype = KeyTypes::DEFAULT,
             int keylength = 0);
    
    static boost::tuple<PublicKey,PrivateKey> 
    generate(const QDateTime &expiry, QString label,
             KeyTypes::KeyType keytype = KeyTypes::DEFAULT,
             int keylength = 0);
    
    static boost::tuple<PublicKey,PrivateKey> 
    generate(Key::Options keyoptions, QString label,
             KeyTypes::KeyType keytype = KeyTypes::DEFAULT,
             int keylength = 0);
    
    static boost::tuple<PublicKey,PrivateKey> 
    generate(Key::Options keyoptions, const QDateTime &expiry,
             QString label,
             KeyTypes::KeyType keytype = KeyTypes::DEFAULT,
             int keylength = 0);
    
    static boost::tuple<PublicKey,PrivateKey> 
    generate(const QDateTime &expiry, KeyTypes::KeyType keytype = KeyTypes::DEFAULT,
             int keylength = 0);
    
    static boost::tuple<PublicKey,PrivateKey> 
    generate(Key::Options keyoptions, KeyTypes::KeyType keytype = KeyTypes::DEFAULT,
             int keylength = 0);
    
    static boost::tuple<PublicKey,PrivateKey> 
    generate(Key::Options keyoptions, const QDateTime &expiry, 
             KeyTypes::KeyType keytype = KeyTypes::DEFAULT,
             int keylength = 0);
    
    QString toString() const;
    uint hashCode() const;
    void stream(Siren::Stream &s);
    
    bool availableToThisThread() const;

protected:
    friend class PubPriLock;
    friend class SignatureLock;
    PrivateKey(const boost::shared_ptr<Crypt::KeyContext> &d,
               const Key::Options &key_options,
               const QDateTime &expiry_data);
    
private:
    /** The shared pointer to the UID of the private key data
        (the private key data is not stored directly in the key) */
    //boost::shared_ptr<Crypt::KeyContext> d;
    boost::shared_ptr<detail::PrivateKeyData> d;
};

}

Q_DECLARE_METATYPE( SireSec::PrivateKey )

SIRE_EXPOSE_CLASS( SireSec::PrivateKey )

SIRE_END_HEADER

#endif
