/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2009  Christopher Woods
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

#include <cmath>
#include <limits>

#include "primitive.h"
#include "logger.h"
#include "tester.h"

#include "Siren/errors.h"

using namespace Siren;

namespace Siren { namespace detail {

    void testNotImplemented(Logger &logger, QString type_name)
    {
        logger.write( QObject::tr(
                "Testing of %1 failed as no unit tests have been written "
                "for this class. Please ask the author to provide some tests.")
                    .arg(type_name) );
    }

    void testNotImplemented(QString type_name)
    {
        Logger logger;
        testNotImplemented(logger, type_name);
    }

}}

//////////
////////// Implementation of String
//////////

static const RegisterPrimitive<String> r_string;

/** Null constructor */
String::String() : Primitive<String>()
{}

/** Construct the string equal to 'text' */
String::String(const QString &t) : Primitive<String>(), text(t)
{}

/** Copy constructor */
String::String(const String &other) : Primitive<String>(other), text(other.text)
{}

/** Destructor */
String::~String()
{}

/** Copy assignment operator */
String& String::operator=(const String &other)
{
    text = other.text;
    return *this;
}

/** Comparison operator */
bool String::operator==(const String &other) const
{
    return text == other.text;
}

/** Comparison operator */
bool String::operator!=(const String &other) const
{
    return text != other.text;
}

/** Allow automatic casting to a QString */
String::operator const QString&() const
{
    return text;
}

QString String::toString() const
{
    return text;
}

uint String::hashCode() const
{
    return qHash(text);
}

bool String::test() const
{
    return true;
}

bool String::test(Logger&) const
{
    return true;
}

void String::stream(Stream &s)
{
    s.assertVersion<String>(1);
    
    Schema schema = s.item<String>();
    
    schema.data("text") & text;
}

//////////
////////// Implementation of Number
//////////

static const RegisterPrimitive<Number> r_number;

/** Null constructor */
Number::Number() : Primitive<Number>()
{
    is_integer = true;
    number_data.integer_value = 0;
}

/** Construct to hold the integer 'value' */
Number::Number(qint64 value) : Primitive<Number>()
{
    is_integer = true;
    number_data.integer_value = value;
}

/** Construct to hold the floating point value 'value' */
Number::Number(double value) : Primitive<Number>()
{
    is_integer = false;
    number_data.float_value = value;
}

/** Copy constructor */
Number::Number(const Number &other) : Primitive<Number>()
{
    is_integer = other.is_integer;
    
    if (is_integer)
    {
        number_data.integer_value = other.number_data.integer_value;
    }
    else
    {
        number_data.float_value = other.number_data.float_value;
    }
}

/** Destructor */
Number::~Number()
{}

/** Copy assignment operator */
Number& Number::operator=(const Number &other)
{
    if (this != &other)
    {
        is_integer = other.is_integer;
        
        if (is_integer)
        {
            number_data.integer_value = other.number_data.integer_value;
        }
        else
        {
            number_data.float_value = other.number_data.float_value;
        }
    }
    
    return *this;
}

/** Comparison operator */
bool Number::operator==(const Number &other) const
{
    if (is_integer and other.is_integer)
    {
        return number_data.integer_value == other.number_data.integer_value;
    }
    else if ( (not is_integer) and (not other.is_integer) )
    {
        return number_data.float_value == other.number_data.float_value;
    }
    else if (is_integer)
    {
        return double(number_data.integer_value) == other.number_data.float_value;
    }
    else
    {
        return double(other.number_data.integer_value) == number_data.float_value;
    }
}

/** Comparison operator */
bool Number::operator!=(const Number &other) const
{
    return not Number::operator==(other);
}

QString Number::toString() const
{
    if (is_integer)
        return QString::number( number_data.integer_value );
    else
        return QString::number( number_data.float_value );
}

/** Hash this Number */
uint Number::hashCode() const
{
    if (is_integer)
        return number_data.integer_value;
    else
        return *( (qint64*)(&(number_data.float_value)) );
}

bool Number::test(Logger &logger) const
{
    Tester tester(*this, logger);
    
    try
    {
        tester.nextTest();
    
        tester.expect_equal(
            QObject::tr("Number.getClass() equals Class(\"Siren::Number\")"),
            CODELOC, this->getClass(), Class( Number::typeName() ) );
    }
    catch(const Siren::exception &e)
    {
        tester.unexpected_error(e);
    }

    return tester.allPassed();;
}

bool Number::test() const
{
    Logger logger;
    return test(logger);
}

/** Construct a number from the passed value */
Number Number::fromInteger(qint64 value)
{
    return Number(value);
}

/** Construct a number from the passed value */
Number Number::fromFloat(double value)
{
    return Number(value);
}

/** Return this number as an integer - it is an error
    to cast a non-integer value to an integer
    
    \throw Siren::invalid_cast
*/
qint64 Number::toInteger() const
{
    if (is_integer)
        return number_data.integer_value;
    else
    {
        double d;
        double v = std::modf( number_data.float_value, &d );
        
        if (d < 1e-8)
            throw Siren::invalid_cast( QObject::tr(
                    "Cannot cast from floating point number %1 to an integer.")
                        .arg(number_data.float_value), CODELOC);
        
        else if (v > double(std::numeric_limits<qint64>::max()))
            throw Siren::invalid_cast( QObject::tr(
                    "Cannot cast from floating point number %1 to an integer, "
                    "as it is too large (greater than %2).")
                        .arg(number_data.float_value)
                        .arg(std::numeric_limits<qint64>::max()), CODELOC );

        else if (v < double(std::numeric_limits<qint64>::min()))
            throw Siren::invalid_cast( QObject::tr(
                    "Cannot cast from floating point number %1 to an integer, "
                    "as it is too small (less than %2).")
                        .arg(number_data.float_value)
                        .arg(std::numeric_limits<qint64>::min()), CODELOC );
        
        else
            return qint64(v);
    }
}

/** Return this number as a float */
double Number::toFloat() const
{
    if (is_integer)
        return double( number_data.integer_value );
    else
        return number_data.float_value;
}

/** Allow automatic casting to an integer

    \throw Siren::invalid_cast
*/
Number::operator qint64() const
{
    return this->toInteger();
}

/** Allow automatic casting to a floating point number */
Number::operator double() const
{
    return this->toFloat();
}

void Number::stream(Stream &s)
{
    s.assertVersion<Number>(1);
    
    Schema schema = s.item<Number>();
    
    schema.data("is_integer") & is_integer;
    
    if (is_integer)
        schema.data("integer_value") & number_data.integer_value;
    else
        schema.data("float_value") & number_data.float_value;
}

//////////
////////// Instantiate templates
//////////

template class PrimitiveObject<String>;
template class PrimitiveObject<Number>;
