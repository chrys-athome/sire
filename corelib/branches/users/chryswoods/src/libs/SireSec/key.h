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

#ifndef SIRESEC_KEY_H
#define SIRESEC_KEY_H

#include <QDateTime>

#include "Siren/object.h"
#include "Siren/objptr.hpp"

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireSec
{

/** All of the (still secure, non-patented) Ciphers supported by 
    cryptlib, and hence supported in SireSec */
namespace Ciphers
{
    enum Cipher { AES = 1,
                  Blowfish = 2,
                  CAST_128 = 3,
                  Triple_DES = 4 };
}

/** All of the (still secure, non-patented) public/private
    key algorithms supported by cryptlib, and hence supported in SireSec */
namespace KeyType
{
    enum KeyType { RSA = 1,
                   Elgamal = 2
                 };
}

/** All of the (still secure, non-patented) certificate algorithms
    supported by cryptlib, and hence supported in SireSec */
namespace CertificateTypes
{
    enum CertificateType { RSA = 1,
                           Elgamal = 2,
                           DSA = 3 };
}

/** All of the (still secure, non-patented) message authentication
    algorithms supported by cryptlib, and hence supported in SireSec */
namespace MACTypes
{
    enum MACType { HMAC_MD5 = 1,
                   HMAC_SHA1 = 2,
                   HMAC_RIPEMD_160 = 3,
                   RIPEMD_160 = 4,
                   SHA = 5,
                   SHA2 = 6 }; 
}

/** This is the virtual base class of all Key objects.
    A Key, when used with a Lock, can be used to 
    encrypt or decrypt data
    
    @author Christopher Woods
*/
class SIRESEC_EXPORT Key : public Siren::Extends<Key,Siren::Object>
{
public:
    enum Option { 
            Unrestricted = 0x00,
            NonStreamable = 0x01,
            LockedToThread = 0x02
                };
         
    Q_DECLARE_FLAGS(Options, Option);

    Key();
    Key(const Key &other);
    
    ~Key();
    
    static QString typeName();
    
    void stream(Siren::Stream &s);

    bool isStreamable() const;
    bool isLockedToThread() const;
    bool isExpired() const;
    
    virtual bool availableToThisThread() const=0;
    
    bool isTemporal() const;
    QDateTime bestBefore() const;
    
    void assertIsStreamable() const;
    void assertAvailableToThisThread() const;
    
    template<class T>
    const T& extract() const
    {
        if (this->isA<T>())
            return this->asA<T>();
        else
            return this->extractKey(T::typeName()).asA<T>();
    }
    
protected:
    Key(const Key::Options &key_options);
    Key(const QDateTime &expiry_date, const Key::Options &key_options);

    Key& operator=(const Key &other);
    bool operator==(const Key &other) const;
    bool operator!=(const Key &other) const;

    void cleanOptions();

    virtual const Key& extractKey(const QString &type_name) const;

private:
    /** The time and date when this key will expire */
    QDateTime expiry_date;

    /** Options associated with the key */
    Options key_options;
};

Q_DECLARE_OPERATORS_FOR_FLAGS( Key::Options )

typedef Siren::ObjPtr<Key> KeyPtr;

}

SIRE_EXPOSE_CLASS( SireSec::Key )

SIRE_EXPOSE_OBJECT_PTR( SireSec::KeyPtr, SireSec::Key )

SIRE_END_HEADER

#endif
