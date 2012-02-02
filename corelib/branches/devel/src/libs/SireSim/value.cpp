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
#include "SireSim/dimensions.h"
#include "SireSim/dimensions.hpp"
#include "SireSim/tempvalue.h"

#include "SireError/errors.h"

using namespace SireSim;
using namespace SireSim::detail;

/** Constructor */
Value::Value()
{}

/** Copy constructor */
Value::Value(const Value&)
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

Value::operator QString() const
{
    if (this->isContainer())
        throw SireError::invalid_arg( QObject::tr(
                "Automatic casting to a string only works for Value types. "
                "It does not work for %1.").arg(this->what()), CODELOC );
                
    return this->toValueString();
}

QString Value::toString() const
{
    if (this->isContainer())
        return this->toXML();
    else
        return this->toValueString();
}

QString Value::toValueString() const
{
    return QString();
}

QStringList Value::toConfigLines(bool) const
{
    if (this->isContainer())
        throw SireError::program_bug( QObject::tr(
                "Container type %1 should provide its own implementation of "
                "toConfigLines(bool)").arg(what()), CODELOC );
                
    QStringList lines;
    lines.append( this->toValueString() );
    return lines;
}

ValuePtr Value::fromValueString(QString value) const
{
    throw SireError::program_bug( QObject::tr(
            "You should not be asking for 'fromValueString(value)' for a %1 object...")
                .arg(this->what()), CODELOC );
                
    return ValuePtr();
}

ValuePtr Value::fromValueString(QString key, QString value) const
{
    throw SireError::program_bug( QObject::tr(
            "You should not be asking for 'fromValueString(key,value)'"
            "for a %1 object...")
                .arg(this->what()), CODELOC );
                
    return ValuePtr();
}

ValuePtr Value::setNestedValue(QString key, QString value) const
{
    if (this->isContainer())
    {
        return this->fromValueString(key,value);
    }
    else
    {
        if (not key.isEmpty())
            throw SireError::invalid_key( QObject::tr(
                "It should not be possible to set a %1 value with "
                "a non-empty key! \"%2\"")
                    .arg(this->what())
                    .arg(key), CODELOC );

        return this->fromValueString(value);
    }
}

ValuePtr Value::setNestedValue(QString key, double value) const
{
    return this->setNestedValue(key, QString::number(value, 'g', 12));
}

ValuePtr Value::setNestedValue(QString key, qint64 value) const
{
    return this->setNestedValue(key, QString::number(value));
}

ValuePtr Value::getNestedValue(QString key) const
{
    if (not key.isEmpty())
        throw SireError::invalid_key( QObject::tr(
                "It should not be possible to get a %1 value with "
                "a non-empty key! \"%2\"")
                    .arg(this->what())
                    .arg(key), CODELOC );

    return self();
}

/** Set the value from the passed line */
ValuePtr Value::fromConfig(detail::ParsedLine &line) const
{
    if (not line.key.key.isEmpty())
        throw SireError::invalid_key( QObject::tr(
                "It should not be possible to set a %1 value with "
                "a non-empty key! \"%2\"")
                    .arg(this->what())
                    .arg(line.key.key), CODELOC );

    if (line.been_read)
        return self();
    
    else
    {
        line.been_read = true;
        return this->fromValueString(line.value);
    }
}

/** Return the description of these option/value as XML. */
QString Value::toXML() const
{
    QDomDocument doc("OPTIONS");
    {
        QDomProcessingInstruction xmlDeclaration 
                = doc.createProcessingInstruction("xml", 
                                    "version=\"1.0\" encoding=\"UTF-8\"");
        
        doc.appendChild(xmlDeclaration);
    }
    
    doc.appendChild( doc.createComment("Created using SireSim/C++") );
    
    QDomElement elem = this->toDomElement(doc);
    
    doc.appendChild(elem);
    
    return doc.toString(2);
}

QString Value::toConfig(bool include_help) const
{
    //TODO should add code to automatically split long config lines into separate lines

    if (this->isContainer())
        return this->toConfigLines(include_help).join("\n");
    else
        return this->toValueString();
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
    else if (elem.tagName() == "enum")
    {
        return EnumValue(elem);
    }
    else if (elem.tagName() == "length")
    {
        return LengthValue(elem);
    }
    else if (elem.tagName() == "area")
    {
        return AreaValue(elem);
    }
    else if (elem.tagName() == "temperature")
    {
        return TempValue(elem);
    }
    else if (elem.tagName() == "pressure")
    {
        return PressureValue(elem);
    }
    else
    {
        qDebug() << "COULD NOT RECOGNISE VALUE" << elem.tagName();
        return StringValue(elem.text());
    }
}
