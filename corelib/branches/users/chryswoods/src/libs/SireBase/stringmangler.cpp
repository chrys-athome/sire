/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2008  Christopher Woods
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

#include "stringmangler.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireBase;
using namespace SireStream;

////////
//////// Implementation of StringManglerBase
////////

/** Constructor */
StringManglerBase::StringManglerBase() : PropertyBase()
{}

/** Copy constructor */
StringManglerBase::StringManglerBase(const StringManglerBase &other)
              : PropertyBase(other)
{}

/** Destructor */
StringManglerBase::~StringManglerBase()
{}

/** Mangle the input string */
QString StringManglerBase::operator()(const QString &input) const
{
    return this->mangle(input);
}

////////
//////// Implementation of NoMangling
////////

static const RegisterMetaType<NoMangling> r_nomangle;

/** Serialise to a binary datastream */
QDataStream SIREBASE_EXPORT &operator<<(QDataStream &ds,
                                        const NoMangling &nomangle)
{
    writeHeader(ds, r_nomangle, 1);
    
    ds << static_cast<const StringManglerBase&>(nomangle);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREBASE_EXPORT &operator>>(QDataStream &ds, NoMangling &nomangle)
{
    VersionID v = readHeader(ds, r_nomangle);
    
    if (v == 1)
    {
        ds >> static_cast<StringManglerBase&>(nomangle);
    }
    else
        throw version_error(v, "1", r_nomangle, CODELOC);
        
    return ds;
}

/** Constructor */
NoMangling::NoMangling() : ConcreteProperty<NoMangling,StringManglerBase>()
{}

static Property nomangle_property = NoMangling();

const Property& NoMangling::toProperty()
{
    return nomangle_property;
}

/** Copy constructor */
NoMangling::NoMangling(const NoMangling &other)
           : ConcreteProperty<NoMangling,StringManglerBase>(other)
{}

/** Destructor */
NoMangling::~NoMangling()
{}

/** Copy assignment operator */
NoMangling& NoMangling::operator=(const NoMangling&)
{
    return *this;
}

/** Comparison operator */
bool NoMangling::operator==(const NoMangling&) const
{
    return true;
}

/** Comparison operator */
bool NoMangling::operator!=(const NoMangling&) const
{
    return false;
}

/** Mangle the string - remove all initial and trailing spaces */
QString NoMangling::mangle(const QString &input) const
{
    return input;
}

////////
//////// Implementation of TrimString
////////

static const RegisterMetaType<TrimString> r_trimstring;

/** Serialise to a binary datastream */
QDataStream SIREBASE_EXPORT &operator<<(QDataStream &ds,
                                        const TrimString &trimstring)
{
    writeHeader(ds, r_trimstring, 1);
    
    ds << static_cast<const StringManglerBase&>(trimstring);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREBASE_EXPORT &operator>>(QDataStream &ds, TrimString &trimstring)
{
    VersionID v = readHeader(ds, r_trimstring);
    
    if (v == 1)
    {
        ds >> static_cast<StringManglerBase&>(trimstring);
    }
    else
        throw version_error(v, "1", r_trimstring, CODELOC);
        
    return ds;
}

/** Constructor */
TrimString::TrimString() : ConcreteProperty<TrimString,StringManglerBase>()
{}

static Property trimstring_property = TrimString();

const Property& TrimString::toProperty()
{
    return trimstring_property;
}

/** Copy constructor */
TrimString::TrimString(const TrimString &other)
           : ConcreteProperty<TrimString,StringManglerBase>(other)
{}

/** Destructor */
TrimString::~TrimString()
{}

/** Copy assignment operator */
TrimString& TrimString::operator=(const TrimString&)
{
    return *this;
}

/** Comparison operator */
bool TrimString::operator==(const TrimString&) const
{
    return true;
}

/** Comparison operator */
bool TrimString::operator!=(const TrimString&) const
{
    return false;
}

/** Mangle the string - remove all initial and trailing spaces */
QString TrimString::mangle(const QString &input) const
{
    return input.trimmed();
}

////////
//////// Implementation of UpperCaseString
////////

static const RegisterMetaType<UpperCaseString> r_upperstring;

/** Serialise to a binary datastream */
QDataStream SIREBASE_EXPORT &operator<<(QDataStream &ds,
                                        const UpperCaseString &upperstring)
{
    writeHeader(ds, r_upperstring, 1);
    
    ds << static_cast<const StringManglerBase&>(upperstring);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREBASE_EXPORT &operator>>(QDataStream &ds, UpperCaseString &upperstring)
{
    VersionID v = readHeader(ds, r_upperstring);
    
    if (v == 1)
    {
        ds >> static_cast<StringManglerBase&>(upperstring);
    }
    else
        throw version_error(v, "1", r_upperstring, CODELOC);
        
    return ds;
}

/** Constructor */
UpperCaseString::UpperCaseString() : ConcreteProperty<UpperCaseString,StringManglerBase>()
{}

static Property upperstring_property = UpperCaseString();

const Property& UpperCaseString::toProperty()
{
    return upperstring_property;
}

/** Copy constructor */
UpperCaseString::UpperCaseString(const UpperCaseString &other)
           : ConcreteProperty<UpperCaseString,StringManglerBase>(other)
{}

/** Destructor */
UpperCaseString::~UpperCaseString()
{}

/** Copy assignment operator */
UpperCaseString& UpperCaseString::operator=(const UpperCaseString&)
{
    return *this;
}

/** Comparison operator */
bool UpperCaseString::operator==(const UpperCaseString&) const
{
    return true;
}

/** Comparison operator */
bool UpperCaseString::operator!=(const UpperCaseString&) const
{
    return false;
}

/** Mangle the string - remove all initial and trailing spaces */
QString UpperCaseString::mangle(const QString &input) const
{
    return input.trimmed().toUpper();
}

////////
//////// Implementation of LowerCaseString
////////

static const RegisterMetaType<LowerCaseString> r_lowerstring;

/** Serialise to a binary datastream */
QDataStream SIREBASE_EXPORT &operator<<(QDataStream &ds,
                                        const LowerCaseString &lowerstring)
{
    writeHeader(ds, r_lowerstring, 1);
    
    ds << static_cast<const StringManglerBase&>(lowerstring);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREBASE_EXPORT &operator>>(QDataStream &ds, LowerCaseString &lowerstring)
{
    VersionID v = readHeader(ds, r_lowerstring);
    
    if (v == 1)
    {
        ds >> static_cast<StringManglerBase&>(lowerstring);
    }
    else
        throw version_error(v, "1", r_lowerstring, CODELOC);
        
    return ds;
}

/** Constructor */
LowerCaseString::LowerCaseString() : ConcreteProperty<LowerCaseString,StringManglerBase>()
{}

static Property lowerstring_property = LowerCaseString();

const Property& LowerCaseString::toProperty()
{
    return lowerstring_property;
}

/** Copy constructor */
LowerCaseString::LowerCaseString(const LowerCaseString &other)
           : ConcreteProperty<LowerCaseString,StringManglerBase>(other)
{}

/** Destructor */
LowerCaseString::~LowerCaseString()
{}

/** Copy assignment operator */
LowerCaseString& LowerCaseString::operator=(const LowerCaseString&)
{
    return *this;
}

/** Comparison operator */
bool LowerCaseString::operator==(const LowerCaseString&) const
{
    return true;
}

/** Comparison operator */
bool LowerCaseString::operator!=(const LowerCaseString&) const
{
    return false;
}

/** Mangle the string - remove all initial and trailing spaces */
QString LowerCaseString::mangle(const QString &input) const
{
    return input.trimmed().toLower();
}

//////////////
////////////// Implementation of StringMangler
//////////////

RegisterMetaType<StringMangler> r_space;

/** Serialise a StringMangler to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds,
                                       const StringMangler &space)
{
    writeHeader(ds, r_space, 1);
    
    SharedDataStream sds(ds);
    
    sds << static_cast<const Property&>(space);
    
    return ds;
}

/** Deserialise a StringMangler from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds,
                                       StringMangler &space)
{
    VersionID v = readHeader(ds, r_space);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        sds >> static_cast<Property&>(space);
    }
    else
        throw version_error(v, "1", r_space, CODELOC);
        
    return ds;
}

static StringMangler *_pvt_shared_null = 0;

const StringMangler& StringMangler::shared_null()
{
    if (_pvt_shared_null == 0)
        _pvt_shared_null = new StringMangler( NoMangling() );
        
    return *_pvt_shared_null;
}

/** Null constructor */
StringMangler::StringMangler() : Property(StringMangler::shared_null())
{}

/** Construct from a passed property

    \throw SireError::invalid_cast
*/
StringMangler::StringMangler(const PropertyBase &property)
              : Property(property.asA<StringManglerBase>())
{}

/** Construct from passed StringManglerBase */
StringMangler::StringMangler(const StringManglerBase &other)
              : Property(other)
{}

/** Copy constructor */
StringMangler::StringMangler(const StringMangler &other)
              : Property(other)
{}

/** Destructor */
StringMangler::~StringMangler()
{}

/** Copy assignment operator from a Property object

    \throw SireError::invalid_cast
*/
StringMangler& StringMangler::operator=(const PropertyBase &property)
{
    Property::operator=(property.asA<StringManglerBase>());
    return *this;
}

/** Copy assignment operator from another StringManglerBase */
StringMangler& StringMangler::operator=(const StringManglerBase &other)
{
    Property::operator=(other);
    return *this;
}

/** Dereference this pointer */
const StringManglerBase* StringMangler::operator->() const
{
    return static_cast<const StringManglerBase*>(&(d()));
}

/** Dereference this pointer */
const StringManglerBase& StringMangler::operator*() const
{
    return static_cast<const StringManglerBase&>(d());
}

/** Return a read-only reference to the contained object */
const StringManglerBase& StringMangler::read() const
{
    return static_cast<const StringManglerBase&>(d());
}

/** Return the mangled string */
QString StringMangler::operator()(const QString &input) const
{
    return this->read().mangle(input);
}

/** Return a modifiable reference to the contained object.
    This will trigger a copy of the object if more than
    one pointer is pointing to it. */
StringManglerBase& StringMangler::edit()
{
    return static_cast<StringManglerBase&>(d());
}
    
/** Return a raw pointer to the StringManglerBase object */
const StringManglerBase* StringMangler::data() const
{
    return static_cast<const StringManglerBase*>(&(d()));
}

/** Return a raw pointer to the StringManglerBase object */
const StringManglerBase* StringMangler::constData() const
{
    return static_cast<const StringManglerBase*>(&(d()));
}
    
/** Return a raw pointer to the StringManglerBase object */
StringManglerBase* StringMangler::data()
{
    return static_cast<StringManglerBase*>(&(d()));
}

/** Automatic casting operator */
StringMangler::operator const StringManglerBase&() const
{
    return static_cast<const StringManglerBase&>(d());
}
