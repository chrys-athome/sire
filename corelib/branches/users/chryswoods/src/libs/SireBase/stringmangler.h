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

#include "Siren/object.h"
#include "Siren/objptr.hpp"

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireBase
{

/** This is the base class of all string mangling functions. 
    A string mangler is just a simple function that performs 
    some transformations on an input string, e.g. changing its
    case, trimming away extra spaces etc.
    
    @author Christopher Woods
*/
class SIREBASE_EXPORT StringMangler : public Siren::Extends<StringMangler,Siren::Object>
{
public:
    StringMangler();
    StringMangler(const StringMangler &other);
    
    virtual ~StringMangler();
    
    static QString typeName();

    void stream(Siren::Stream &s);
    
    QString operator()(const QString &input) const;
    
    virtual QString mangle(const QString &input) const=0;
};

/** This mangler does absolutely nothing to the string!
    
    @author Christopher Woods
*/
class SIREBASE_EXPORT NoMangling : public Siren::Implements<NoMangling,StringMangler>
{
public:
    NoMangling();
    NoMangling(const NoMangling &other);
    
    ~NoMangling();
    
    NoMangling& operator=(const NoMangling &other);
    
    bool operator==(const NoMangling &other) const;
    bool operator!=(const NoMangling &other) const;

    QString toString() const;
    uint hashCode() const;
    void stream(Siren::Stream &s);

    QString mangle(const QString &input) const;
};

/** This mangler just trims spaces away from the beginning
    and end of the string
    
    @author Christopher Woods
*/
class SIREBASE_EXPORT TrimString : public Siren::Implements<TrimString,StringMangler>
{
public:
    TrimString();
    TrimString(const TrimString &other);
    
    ~TrimString();
    
    TrimString& operator=(const TrimString &other);
    
    bool operator==(const TrimString &other) const;
    bool operator!=(const TrimString &other) const;

    QString toString() const;
    uint hashCode() const;
    void stream(Siren::Stream &s);

    QString mangle(const QString &input) const;
};

/** This mangler just trims spaces away from the beginning
    and end of the string and upper-cases the whole string
    
    @author Christopher Woods
*/
class SIREBASE_EXPORT UpperCaseString 
            : public Siren::Implements<UpperCaseString,StringMangler>
{
public:
    UpperCaseString();
    UpperCaseString(const UpperCaseString &other);
    
    ~UpperCaseString();
    
    UpperCaseString& operator=(const UpperCaseString &other);
    
    bool operator==(const UpperCaseString &other) const;
    bool operator!=(const UpperCaseString &other) const;

    QString toString() const;
    uint hashCode() const;
    void stream(Siren::Stream &s);

    QString mangle(const QString &input) const;
};

/** This mangler just trims spaces away from the beginning
    and end of the string and lower-cases the whole string
    
    @author Christopher Woods
*/
class SIREBASE_EXPORT LowerCaseString 
            : public Siren::Implements<LowerCaseString,StringMangler>
{
public:
    LowerCaseString();
    LowerCaseString(const LowerCaseString &other);
    
    ~LowerCaseString();
    
    LowerCaseString& operator=(const LowerCaseString &other);
    
    bool operator==(const LowerCaseString &other) const;
    bool operator!=(const LowerCaseString &other) const;

    QString toString() const;
    uint hashCode() const;
    void stream(Siren::Stream &s);

    QString mangle(const QString &input) const;
};

typedef Siren::ObjPtr<StringMangler> StringManglerPtr;

}

Q_DECLARE_METATYPE( SireBase::NoMangling );
Q_DECLARE_METATYPE( SireBase::TrimString );
Q_DECLARE_METATYPE( SireBase::UpperCaseString );
Q_DECLARE_METATYPE( SireBase::LowerCaseString );

SIRE_EXPOSE_CLASS( SireBase::StringMangler )
SIRE_EXPOSE_CLASS( SireBase::NoMangling )
SIRE_EXPOSE_CLASS( SireBase::TrimString )
SIRE_EXPOSE_CLASS( SireBase::UpperCaseString )
SIRE_EXPOSE_CLASS( SireBase::LowerCaseString )

SIRE_EXPOSE_OBJECT_PTR( SireBase::StringManglerPtr, SireBase::StringMangler )

SIRE_END_HEADER

#endif
