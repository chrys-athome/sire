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

#include <QMutex>

#include "stringmangler.h"

#include "Siren/stream.h"

using namespace SireBase;
using namespace Siren;

////////
//////// Implementation of StringMangler
////////

static const RegisterObject<StringMangler> r_stringmangler( VIRTUAL_CLASS );

/** Constructor */
StringMangler::StringMangler() : Extends<StringMangler,Object>()
{}

/** Copy constructor */
StringMangler::StringMangler(const StringMangler &other)
              : Extends<StringMangler,Object>(other)
{}

/** Destructor */
StringMangler::~StringMangler()
{}

QString StringMangler::typeName()
{
    return "SireBase::StringMangler";
}

/** Mangle the input string */
QString StringMangler::operator()(const QString &input) const
{
    return this->mangle(input);
}

void StringMangler::stream(Stream &s)
{
    s.assertVersion<StringMangler>(1);
    
    Schema schema = s.item<StringMangler>();
    
    Object::stream( schema.base() );
}

////////
//////// Implementation of NoMangling
////////

static const RegisterObject<NoMangling> r_nomangle;

/** Constructor */
NoMangling::NoMangling() : Implements<NoMangling,StringMangler>()
{}

/** Copy constructor */
NoMangling::NoMangling(const NoMangling &other)
           : Implements<NoMangling,StringMangler>(other)
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

QString NoMangling::toString() const
{
    return QObject::tr("NoMangling()");
}

uint NoMangling::hashCode() const
{
    return qHash( NoMangling::typeName() );
}

void NoMangling::stream(Stream &s)
{
    s.assertVersion<NoMangling>(1);
    
    Schema schema = s.item<NoMangling>();
    
    StringMangler::stream( schema.base() );
}

////////
//////// Implementation of TrimString
////////

static const RegisterObject<TrimString> r_trimstring;

/** Constructor */
TrimString::TrimString() : Implements<TrimString,StringMangler>()
{}

/** Copy constructor */
TrimString::TrimString(const TrimString &other)
           : Implements<TrimString,StringMangler>(other)
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

QString TrimString::toString() const
{
    return QObject::tr("TrimString()");
}

uint TrimString::hashCode() const
{
    return qHash( TrimString::typeName() );
}

void TrimString::stream(Stream &s)
{
    s.assertVersion<TrimString>(1);
    
    Schema schema = s.item<TrimString>();
    
    StringMangler::stream( schema.base() );
}

////////
//////// Implementation of UpperCaseString
////////

static const RegisterObject<UpperCaseString> r_upperstring;

/** Constructor */
UpperCaseString::UpperCaseString() : Implements<UpperCaseString,StringMangler>()
{}

/** Copy constructor */
UpperCaseString::UpperCaseString(const UpperCaseString &other)
           : Implements<UpperCaseString,StringMangler>(other)
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

QString UpperCaseString::toString() const
{
    return QObject::tr("UpperCaseString()");
}

uint UpperCaseString::hashCode() const
{
    return qHash( UpperCaseString::typeName() );
}

void UpperCaseString::stream(Stream &s)
{
    s.assertVersion<UpperCaseString>(1);
    
    Schema schema = s.item<UpperCaseString>();
    
    StringMangler::stream( schema.base() );
}

////////
//////// Implementation of LowerCaseString
////////

static const RegisterObject<LowerCaseString> r_lowerstring;

/** Constructor */
LowerCaseString::LowerCaseString() : Implements<LowerCaseString,StringMangler>()
{}

/** Copy constructor */
LowerCaseString::LowerCaseString(const LowerCaseString &other)
           : Implements<LowerCaseString,StringMangler>(other)
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

QString LowerCaseString::toString() const
{
    return QObject::tr("LowerCaseString()");
}

uint LowerCaseString::hashCode() const
{
    return qHash( LowerCaseString::typeName() );
}

void LowerCaseString::stream(Stream &s)
{
    s.assertVersion<LowerCaseString>(1);
    
    Schema schema = s.item<LowerCaseString>();
    
    StringMangler::stream( schema.base() );
}
