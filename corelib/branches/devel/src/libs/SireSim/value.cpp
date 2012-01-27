/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2012  Christopher Woods
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

#include <QDomElement>
#include <QDebug>

#include "SireSim/value.h"
#include "SireSim/values.h"

#include "SireError/errors.h"

using namespace SireSim;
using namespace SireSim::detail;

/** Constructor */
Value::Value()
{}

/** Destructor */
Value::~Value()
{}

/** Return a clone of this value */
ValuePtr Value::clone() const
{
    Value *c = this->ptr_clone();
    
    ValuePtr ptr(c);
    c->self_ptr = ptr;
    
    return ptr;
}

/** Return a point to self */
ValuePtr Value::self() const
{
    ValuePtr ptr = self_ptr.lock();
    
    if (ptr)
        return ptr;
    else
        return this->clone();
}

/** Return the value of the option with the passed key */
ValuePtr Value::operator[](QString key) const
{
    return this->getValue(key);
}

void Value::throwInvalidCast(const char* this_type, const char* other_type)
{
    throw SireError::invalid_cast( QObject::tr(
            "You cannot cast from an object of type %1 to an object of type %2.")
                .arg(other_type).arg(this_type), CODELOC );
}

bool Value::isContainer() const
{
    return false;
}

Value::operator ValuePtr() const
{
    return self();
}

QString Value::toConfig(bool include_help) const
{
    return this->toConfigLines(include_help).join("\n");
}

ValuePtr Value::createFrom(QDomElement elem)
{
    if (elem.tagName() == "string")
    {
        return StringValue(elem);
    }
    else if (elem.tagName() == "directory")
    {
        return DirValue(elem);
    }
    else if (elem.tagName() == "file")
    {
        return FileValue(elem);
    }
    else if (elem.tagName() == "integer")
    {
        return IntegerValue(elem);
    }
    else if (elem.tagName() == "float")
    {
        return FloatValue(elem);
    }
    else if (elem.tagName() == "bool")
    {
        return BoolValue(elem);
    }
    else
    {
        qDebug() << "COULD NOT RECOGNISE VALUE" << elem.tagName();
        return StringValue(elem.text());
    }
}
