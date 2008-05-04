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

#ifndef SIREBASE_STRINGMANGLER_H
#define SIREBASE_STRINGMANGLER_H

#include "property.h"

SIRE_BEGIN_HEADER

namespace SireBase
{
class TrimString;
}

QDataStream& operator<<(QDataStream&, const SireBase::TrimString&);
QDataStream& operator>>(QDataStream&, SireBase::TrimString&);

namespace SireBase
{

/** This is the base class of all string mangling functions. 
    A string mangler is just a simple function that performs 
    some transformations on an input string, e.g. changing its
    case, trimming away extra spaces etc.
    
    @author Christopher Woods
*/
class SIREBASE_EXPORT StringMangler : public PropertyBase
{
public:
    StringMangler();
    StringMangler(const StringMangler &other);
    
    virtual ~StringMangler();
    
    virtual StringMangler* clone() const=0;
    
    static const char* typeName()
    {
        return "SireBase::StringMangler";
    }
    
    virtual QString mangle(const QString &input) const=0;
};

/** This mangler just trims spaces away from the beginning
    and end of the string
    
    @author Christopher Woods
*/
class SIREBASE_EXPORT TrimString : public ConcreteProperty<TrimString,StringMangler>
{
public:
    TrimString();
    TrimString(const TrimString &other);
    
    ~TrimString();
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<TrimString>() );
    }
    
    TrimString& operator=(const TrimString &other);
    
    bool operator==(const TrimString &other) const;
    bool operator!=(const TrimString &other) const;

    QString mangle(const QString &input) const;
    
    static const Property& toProperty();
};

/** This mangler just trims spaces away from the beginning
    and end of the string and upper-cases the whole string
    
    @author Christopher Woods
*/
class SIREBASE_EXPORT UpperCaseString 
            : public ConcreteProperty<UpperCaseString,StringMangler>
{
public:
    UpperCaseString();
    UpperCaseString(const UpperCaseString &other);
    
    ~UpperCaseString();
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<UpperCaseString>() );
    }
    
    UpperCaseString& operator=(const UpperCaseString &other);
    
    bool operator==(const UpperCaseString &other) const;
    bool operator!=(const UpperCaseString &other) const;

    QString mangle(const QString &input) const;
    
    static const Property& toProperty();
};

/** This mangler just trims spaces away from the beginning
    and end of the string and lower-cases the whole string
    
    @author Christopher Woods
*/
class SIREBASE_EXPORT LowerCaseString 
            : public ConcreteProperty<LowerCaseString,StringMangler>
{
public:
    LowerCaseString();
    LowerCaseString(const LowerCaseString &other);
    
    ~LowerCaseString();
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<LowerCaseString>() );
    }
    
    LowerCaseString& operator=(const LowerCaseString &other);
    
    bool operator==(const LowerCaseString &other) const;
    bool operator!=(const LowerCaseString &other) const;

    QString mangle(const QString &input) const;
    
    static const Property& toProperty();
};

}

Q_DECLARE_METATYPE( SireBase::TrimString );
Q_DECLARE_METATYPE( SireBase::UpperCaseString );
Q_DECLARE_METATYPE( SireBase::LowerCaseString );

SIRE_END_HEADER

#endif
