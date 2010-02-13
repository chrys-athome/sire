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

#include "key.h"

#include "SireSec/errors.h"

#include "Siren/errors.h"
#include "Siren/stream.h"

using namespace SireSec;
using namespace Siren;

namespace SireSec
{
    QString cipherString(Ciphers::Cipher cipher)
    {
        switch (cipher)
        {
            case Ciphers::DEFAULT:
                return "DEFAULT";
            case Ciphers::AES:
                return "AES";
            case Ciphers::Blowfish:
                return "Blowfish";
            case Ciphers::CAST_128:
                return "CAST-128";
            case Ciphers::Triple_DES:
                return "Triple DES";
        }
        
        return "DEFAULT";
    }
    
    Ciphers::Cipher stringToCipher(const QString &string)
    {
        if (string == QLatin1String("DEFAULT"))
            return Ciphers::DEFAULT;

        else if (string == QLatin1String("AES"))
            return Ciphers::AES;

        else if (string == QLatin1String("Blowfish"))
            return Ciphers::Blowfish;

        else if (string == QLatin1String("CAST-128"))
            return Ciphers::CAST_128;

        else if (string == QLatin1String("Triple DES"))
            return Ciphers::Triple_DES;
        
        else
        {
            throw Siren::unsupported( QObject::tr(
                    "The cipher \"%1\" is not supported.").arg(string), CODELOC );

            return Ciphers::DEFAULT;
        }
    }

    QString keyTypeString(KeyTypes::KeyType keytype)
    {
        switch (keytype)
        {
            case KeyTypes::DEFAULT:
                return "DEFAULT";
            case KeyTypes::RSA:
                return "RSA";
            case KeyTypes::Elgamal:
                return "Elgamal";
        }
        
        return "DEFAULT";
    }
    
    KeyTypes::KeyType stringToKeyType(const QString &string)
    {
        if (string == QLatin1String("DEFAULT"))
            return KeyTypes::DEFAULT;

        else if (string == QLatin1String("RSA"))
            return KeyTypes::RSA;

        else if (string == QLatin1String("Elgamal"))
            return KeyTypes::Elgamal;
        
        else
        {
            throw Siren::unsupported( QObject::tr(
                    "The key type \"%1\" is not supported.").arg(string), CODELOC );

            return KeyTypes::DEFAULT;
        }
    }

    QString certTypeString(CertTypes::CertType certtype)
    {
        switch (certtype)
        {
            case CertTypes::DEFAULT:
                return "DEFAULT";
            case CertTypes::RSA:
                return "RSA";
            case CertTypes::Elgamal:
                return "Elgamal";
            case CertTypes::DSA:
                return "DSA";
        }
        
        return "DEFAULT";
    }

    CertTypes::CertType stringToCertType(const QString &string)
    {
        if (string == QLatin1String("DEFAULT"))
            return CertTypes::DEFAULT;

        else if (string == QLatin1String("RSA"))
            return CertTypes::RSA;

        else if (string == QLatin1String("Elgamal"))
            return CertTypes::Elgamal;

        else if (string == QLatin1String("DSA"))
            return CertTypes::DSA;
        
        else
        {
            throw Siren::unsupported( QObject::tr(
                    "The certificate type \"%1\" is not supported.")
                        .arg(string), CODELOC );

            return CertTypes::DEFAULT;
        }
    }

    QString macTypeString(MACTypes::MACType mactype)
    {
        switch (mactype)
        {
            case MACTypes::DEFAULT:
                return "DEFAULT";
            case MACTypes::HMAC_MD5:
                return "HMAC-MD5";
            case MACTypes::HMAC_SHA1:
                return "HMAC-SHA1";
            case MACTypes::HMAC_RIPEMD_160:
                return "HMAC-RIPEMD-160";
            case MACTypes::RIPEMD_160:
                return "RIPEMD-160";
            case MACTypes::SHA:
                return "SHA";
            case MACTypes::SHA2:
                return "SHA2";
        }
        
        return "DEFAULT";
    }

    MACTypes::MACType stringToMACType(const QString &string)
    {
        if (string == QLatin1String("DEFAULT"))
            return MACTypes::DEFAULT;

        else if (string == QLatin1String("HMAC-MD5"))
            return MACTypes::HMAC_MD5;

        else if (string == QLatin1String("HMAC-SHA1"))
            return MACTypes::HMAC_SHA1;

        else if (string == QLatin1String("HMAC-RIPEMD-160"))
            return MACTypes::HMAC_RIPEMD_160;

        else if (string == QLatin1String("RIPEMD-160"))
            return MACTypes::RIPEMD_160;

        else if (string == QLatin1String("SHA"))
            return MACTypes::SHA;

        else if (string == QLatin1String("SHA2"))
            return MACTypes::SHA2;
        
        else
        {
            throw Siren::unsupported( QObject::tr(
                    "The MAC type \"%1\" is not supported.")
                        .arg(string), CODELOC );

            return MACTypes::DEFAULT;
        }
    }
}

static const RegisterObject<Key> r_key( VIRTUAL_CLASS );

/** Constructor */
Key::Key() : Extends<Key,Object>(), key_options( Key::Unrestricted )
{}

void Key::cleanOptions()
{
    if (key_options.testFlag( LockedToThread ) or expiry_date.isValid())
    {
        key_options |= NonStreamable;
    }
}

/** Internal constructor used to set the key options */
Key::Key(const Key::Options &options)
    : Extends<Key,Object>(), key_options(options)
{
    cleanOptions();
}

/** Internal constructor used to set the key options and expiry date */
Key::Key(const QDateTime &expires, const Key::Options &options)
    : Extends<Key,Object>(), expiry_date(expires), key_options(options)
{
    cleanOptions();
}

/** Copy constructor */
Key::Key(const Key &other) 
    : Extends<Key,Object>(other),
      expiry_date(other.expiry_date),
      key_options(other.key_options)
{}

/** Destructor */
Key::~Key()
{}

/** Copy assignment operator */
Key& Key::operator=(const Key &other)
{
    if (this != &other)
    {
        expiry_date = other.expiry_date;
        key_options = other.key_options;
        
        super::operator=(other);
    }
    
    return *this;
}

/** Comparison operator */
bool Key::operator==(const Key &other) const
{
    return expiry_date == other.expiry_date and
           key_options == other.key_options and
           super::operator==(other);
}

/** Comparison operator */
bool Key::operator!=(const Key &other) const
{
    return not Key::operator==(other);
}

QString Key::typeName()
{
    return "SireSec::Key";
}

const Key& Key::extractKey(const QString &type_name) const
{
    if (this->what() != type_name)
        throw SireSec::missing_key( QObject::tr(
                "A key of type %1 cannot be extracted from the key %2.")
                    .arg(type_name, this->toString()), CODELOC );
                    
    return *this;
}

/** Return whether or not this key is streamble.
    If it isn't, then an exception will be raised
    if any attempt is made to stream this key */
bool Key::isStreamable() const
{
    return not key_options.testFlag( Key::NonStreamable );
}

/** Assert that this key is streamable 

    \throw SireSec::key_is_private
*/
void Key::assertIsStreamable() const
{
    if (not isStreamable())
        throw SireSec::key_is_private( QObject::tr(
                "It is not possible to stream the key %1 as the "
                "key is private, thread-locked or may expire, "
                "and is thus, for security reasons, not streamable!")
                    .arg(this->toString()), CODELOC );
}

/** Assert that this key is available to this thread

    \throw SireSec::key_is_private
*/
void Key::assertAvailableToThisThread() const
{
    if (not this->availableToThisThread())
        throw SireSec::key_is_private( QObject::tr(
                "The key %1 is not available to this thread. It is locked "
                "to just a single thread, and this isn't it!")
                    .arg(this->toString()), CODELOC );
}

/** Return whether or not this key is locked to a thread.
    This means that the key can only be read by the thread
    that created the key. This also means that the key is
    not streamable */
bool Key::isLockedToThread() const
{
    return key_options.testFlag( Key::LockedToThread );
}

/** Return whether or not this key has expired */
bool Key::isExpired() const
{
    if (expiry_date.isValid())
        return expiry_date > QDateTime::currentDateTime();
    else
        return false;
}

/** Return whether or not this key is temporal - i.e. it
    has an expiry date and will expire at some point. 
    A temporal key is also one that can't be streamed */
bool Key::isTemporal() const
{
    return not expiry_date.isValid();
}

/** Return the expiry date of this key - this returns a null
    QDateTime if this is not a temporal key */
QDateTime Key::bestBefore() const
{
    return expiry_date;
}

void Key::stream(Siren::Stream &s)
{
    s.assertVersion<Key>(1);
    
    Schema schema = s.item<Key>();
        
    super::stream( schema.base() );
    
    if (s.isLoading())
    {
        expiry_date = QDateTime();
        key_options = Key::Unrestricted;
    }
}
