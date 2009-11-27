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

#include "id.h"
#include "name.h"
#include "number.h"

#include "Siren/stream.h"

using namespace SireID;
using namespace Siren;

///////////
/////////// Implementation of ID
///////////

static const RegisterObject<ID> r_id( VIRTUAL_CLASS );

ID::ID() : Extends<ID,Object>()
{}

ID::ID(const ID &other) : Extends<ID,Object>(other)
{}

ID::~ID()
{}

QString ID::typeName()
{
    return "SirenID::ID";
}

ID& ID::operator=(const ID &other)
{
    Object::operator=(other);
    return *this;
}

void ID::stream(Stream &s)
{
    s.assertVersion<ID>(1);
    
    Schema schema = s.item<ID>();
    
    Object::stream( schema.base() );
}

///////////
/////////// Implementation of Name
///////////

static const RegisterObject<Name> r_name( VIRTUAL_CLASS );

Name::Name(const QString &name, CaseSensitivity case_sensitivity) 
     : Extends<Name,ID>(), _name(name)
{
    switch (case_sensitivity)
    {
        case CaseSensitive:
            case_sensitive = true;
            break;
        case CaseInsensitive:
            case_sensitive = false;
            break;
    }
}

Name::Name(const Name &other) 
     : Extends<Name,ID>(other), _name(other._name), case_sensitive(other.case_sensitive)
{}

Name::~Name()
{}

Name& Name::operator=(const Name &other)
{
    _name = other._name;
    case_sensitive = other.case_sensitive;
    
    ID::operator=(other);
    
    return *this;
}

bool Name::operator==(const Name &other) const
{
    return _name == other._name and case_sensitive == other.case_sensitive;
}

bool Name::operator!=(const Name &other) const
{
    return _name != other._name or case_sensitive != other.case_sensitive;
}

Name::operator QString() const
{
    return _name;
}

bool Name::isNull() const
{
    return _name.isNull();
}

bool Name::isCaseSensitive() const
{
    return case_sensitive;
}

uint Name::hashCode() const
{
    return ::qHash(_name);
}

bool Name::isEmpty() const
{
    return _name.isEmpty();
}

const QString& Name::value() const
{
    return _name;
}

void Name::stream(Stream &s)
{
    s.assertVersion<Name>(1);
    
    Schema schema = s.item<Name>();
    
    schema.data("name") & _name;
    
    ID::stream( schema.base() );
}

///////////
/////////// Implementation of Number
///////////

static const RegisterObject<Number> r_number( VIRTUAL_CLASS );

Number::Number(qint32 num) : Extends<Number,ID>(), _num(num)
{}

Number::Number(const Number &other) : Extends<Number,ID>(other), _num(other._num)
{}

Number::~Number()
{}

Number::operator qint32() const
{
    return _num;
}

qint32 Number::null()
{
    return std::numeric_limits<qint32>::min();
}

uint Number::hashCode() const
{
    return quint32(_num);
}

bool Number::isNull() const
{
    return _num == null();
}

qint32 Number::value() const
{
    return _num;
}

void Number::stream(Stream &s)
{
    s.assertVersion<Number>(1);
    
    Schema schema = s.item<Number>();
    
    schema.data("number") & _num;
    
    ID::stream( schema.base() );
}
