/********************************************\
  *
  *  Conspire
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

#include "Conspire/value.h"
#include "Conspire/values.h"
#include "Conspire/dimensions.h"
#include "Conspire/dimensions.hpp"
#include "Conspire/indexvalue.h"
#include "Conspire/tempvalue.h"
#include "Conspire/conspire.hpp"

#include "Conspire/exceptions.h"

using namespace Conspire;
using namespace Conspire::Qt;
using namespace Siren;

REGISTER_SIREN_VIRTUAL_CLASS( Conspire::Value )

/** Constructor */
Value::Value() : Object()
{}

/** Copy constructor */
Value::Value(const Value &other) : Object(other)
{}

/** Destructor */
Value::~Value()
{}

void Value::copy_object(const Value &other)
{
    super::copy_object(other);
}

bool Value::compare_object(const Value &other) const
{
    return super::compare_object(other);
}

bool Value::isContainer() const
{
    return false;
}

Value::operator String() const
{
    if (this->isContainer())
        throw Conspire::invalid_cast( Conspire::tr(
                "Automatic casting to a string only works for Value types. "
                "It does not work for %1.").arg(this->what()), CODELOC );
                
    return this->toValueString();
}

String Value::toString() const
{
    if (this->isContainer())
        return this->toXML();
    else
        return this->toValueString();
}

String Value::toValueString() const
{
    return String();
}

StringList Value::toConfigLines(bool) const
{
    if (this->isContainer())
        throw Siren::program_bug( QObject::tr(
                "Container type %1 should provide its own implementation of "
                "toConfigLines(bool)").arg(what()), CODELOC );
                
    StringList lines;
    lines.append( this->toValueString() );
    return lines;
}

Obj Value::fromValueString(QString value) const
{
    throw Conspire::program_bug( QObject::tr(
            "You should not be asking for 'fromValueString(value)' for a %1 object...")
                .arg(this->what()), CODELOC );
                
    return None();
}

Obj Value::fromValueString(String key, String value) const
{
    throw Conspire::program_bug( Conspire::tr(
            "You should not be asking for 'fromValueString(key,value)'"
            "for a %1 object...")
                .arg(this->what()), CODELOC );
                
    return None();
}

Obj Value::setNestedValue(String key, String value) const
{
    if (this->isContainer())
    {
        return this->fromValueString(key,value);
    }
    else
    {
        if (not key.isEmpty())
            throw Conspire::invalid_key( QObject::tr(
                "It should not be possible to set a %1 value with "
                "a non-empty key! \"%2\"")
                    .arg(this->what())
                    .arg(key), CODELOC );

        return this->fromValueString(value);
    }
}

Obj Value::setNestedValue(String key, double value) const
{
    return this->setNestedValue(key, String::number(value, 'g', 12));
}

Obj Value::setNestedValue(String key, int64 value) const
{
    return this->setNestedValue(key, String::number(value));
}

Obj Value::getNestedValue(String key) const
{
    if (not key.isEmpty())
        throw Conspire::invalid_key( QObject::tr(
                "It should not be possible to get a %1 value with "
                "a non-empty key! \"%2\"")
                    .arg(this->what())
                    .arg(key), CODELOC );

    return *this;
}

/** Set the value from the passed line */
Obj Value::fromConfig(detail::ParsedLine &line) const
{
    if (not line.key.key.isEmpty())
        throw Conspire::invalid_key( QObject::tr(
                "It should not be possible to set a %1 value with "
                "a non-empty key! \"%2\"")
                    .arg(this->what())
                    .arg(line.key.key), CODELOC );

    if (line.been_read)
        return *this;
    
    else
    {
        line.been_read = true;
        return this->fromValueString(line.value);
    }
}

/** Return the description of these option/value as XML. */
String Value::toXML() const
{
    DomDocument doc("OPTIONS");
    {
        DomProcessingInstruction xmlDeclaration 
                = doc.createProcessingInstruction("xml", 
                                    "version=\"1.0\" encoding=\"UTF-8\"");
        
        doc.appendChild(xmlDeclaration);
    }
    
    doc.appendChild( doc.createComment("Created using Conspire") );
    
    DomElement elem = this->toDomElement(doc);
    
    doc.appendChild(elem);
    
    return doc.toString(2);
}

String Value::toConfig(bool include_help) const
{
    //TODO should add code to automatically split long config lines into separate lines

    if (this->isContainer())
        return this->toConfigLines(include_help).join("\n");
    else
        return this->toValueString();
}

Obj Value::createFrom(DomElement elem)
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
    else if (elem.tagName() == "index_list")
    {
        return IndexList(elem);
    }
    else
    {
        sirenDebug() << "COULD NOT RECOGNISE VALUE" << elem.tagName();
        return StringValue(elem.text());
    }
}
