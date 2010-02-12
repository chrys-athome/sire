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

#include "Siren/stream.h"

using namespace SireSec;
using namespace Siren;

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
