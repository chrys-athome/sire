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

#ifndef SIREFF_FFNAME_H
#define SIREFF_FFNAME_H

#include "SireID/name.h"

#include "ffid.h"

SIRE_BEGIN_HEADER

namespace SireFF
{

/** This class holds the name of a forcefield
    
    @author Christopher Woods
*/
class SIREFF_EXPORT FFName : public SireID::Name, public FFID
{

public:
    FFName();
    explicit FFName(const QString &name);
    
    FFName(const FFName &other);
    
    ~FFName();
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<FFName>() );
    }
    
    const char* what() const
    {
        return FFName::typeName();
    }
    
    FFName* clone() const
    {
        return new FFName(*this);
    }
    
    bool isNull() const
    {
        return SireID::Name::isNull();
    }
    
    uint hash() const
    {
        return qHash(_name);
    }
    
    QString toString() const
    {
        return QString("FFName('%1')").arg(_name);
    }
    
    FFName& operator=(const FFName &other)
    {
        SireID::Name::operator=(other);
        FFID::operator=(other);
        return *this;
    }
    
    bool operator==(const SireID::ID &other) const
    {
        return SireID::ID::compare<FFName>(*this, other);
    }
    
    bool operator==(const FFName &other) const
    {
        return _name == other._name;
    }
    
    bool operator!=(const FFName &other) const
    {
        return _name != other._name;
    }
    
    QList<FFIdx> map(const ForceFields &ffields) const;
};

}

Q_DECLARE_METATYPE(SireFF::FFName);

SIRE_EXPOSE_CLASS( SireFF::FFName )

SIRE_END_HEADER

#endif
