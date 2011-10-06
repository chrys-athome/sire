/********************************************\
*
*  Siren - C++ metaobject library
*
*  Copyright (C) 2011  Christopher Woods
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

#include "Siren/number.h"
#include "Siren/string.h"
#include "Siren/exceptions.h"
#include "Siren/siren.hpp"

using namespace Siren;

REGISTER_SIREN_CLASS( Siren::Number )

/** Number constructor - this equals 0 */
Number::Number()
{
    i64 = 0;
    num_type = ZERO;
}

/** Construct from the passed logical boolean */
Number::Number(bool number)
{
    b1 = number;
    num_type = LOGICAL;
}

/** Construct from the passed 8-bit integer */
Number::Number(int8 number)
{
    if (number == 0)
    {
        num_type = ZERO;
        i64 = 0;
    }
    else
    {
        i8 = number;
        num_type = INT8;
    }
}

/** Construct from the passed 32-bit integer */
Number::Number(int32 number)
{
    if (number == 0)
    {
        num_type = ZERO;
        i64 = 0;
    }
    else
    {
        i32 = number;
        num_type = INT32;
    }
}

/** Construct from the passed 64-bit integer */
Number::Number(int64 number)
{
    if (number == 0)
    {
        num_type = ZERO;
        i64 = 0;
    }
    else
    {
        i64 = number;
        num_type = INT64;
    }
}

/** Construct from the passed 8-bit unsigned integer */
Number::Number(uint8 number)
{
    if (number == 0)
    {
        num_type = ZERO;
        i64 = 0;
    }
    else
    {
        u8 = number;
        num_type = UINT8;
    }
}

/** Construct from the pased 32-bit unsigned integer */
Number::Number(uint32 number)
{
    if (number == 0)
    {
        num_type = ZERO;
        i64 = 0;
    }
    else
    {
        u32 = number;
        num_type = UINT32;
    }
}

/** Construct from the passed 64-bit unsigned integer */
Number::Number(uint64 number)
{
    if (number == 0)
    {
        num_type = ZERO;
        i64 = 0;
    }
    else
    {
        u64 = number;
        num_type = UINT64;
    }
}

/** Construct from the passed 32-bit floating point number */
Number::Number(float32 number)
{
    if (number == 0)
    {
        num_type = ZERO;
        i64 = 0;
    }
    else
    {
        f32 = number;
        num_type = FLOAT32;
    }
}

/** Construct from the passed 64-bit floating point number */
Number::Number(float64 number)
{
    if (number == 0)
    {
        num_type = ZERO;
        i64 = 0;
    }
    else
    {
        f64 = number;
        num_type = FLOAT64;
    }
}

/** Copy constructor */
Number::Number(const Number &other)
{
    memcpy(this, &other, sizeof(Number));
}

/** Destructor */
Number::~Number()
{}

/** Copy assignment operator */
Number& Number::operator=(const Number &other)
{
    if (this != &other)
    {
        memcpy(this, &other, sizeof(Number));
    }
    
    return *this;
}

/** Comparison function */
bool Number::operator==(const Number &other) const
{
    if (num_type == other.num_type)
    {
        switch(num_type)
        {
        case ZERO:
            return true;

        case LOGICAL:
            return b1 == other.b1;

        case INT8:
            return i8 == other.i8;
        case INT32:
            return i32 == other.i32;
        case INT64:
            return i64 == other.i64;

        case UINT8:
            return u8 == other.u8;
        case UINT32:
            return u32 == other.u32;
        case UINT64:
            return u64 == other.u64;

        case FLOAT32:
            return f32 == other.f32;
        case FLOAT64:
            return f64 == other.f64;
        }
    }
    else
        return false;
}

/** Comparison function */
bool Number::operator!=(const Number &other) const
{
    return not operator==(other);
}

/** Return a string representation of this number */
String Number::toString() const
{
    switch(num_type)
    {
    case ZERO:
        return String("0");

    case LOGICAL:
        if (b1)
            return String::tr("true");
        else
            return String::tr("false");

    case INT8:
        return String::number(i8);
    case INT32:
        return String::number(i32);
    case INT64:
        return String::number(i64);

    case UINT8:
        return String::number(u8);
    case UINT32:
        return String::number(u32);
    case UINT64:
        return String::number(u64);
    
    case FLOAT32:
        return String::number(f32);
    case FLOAT64:
        return String::number(f64);
    }
}

/** Return whether or not this number is a float */
bool Number::isFloat() const
{
    return num_type == ZERO or
           num_type == FLOAT32 or
           num_type == FLOAT64;
}

/** Return whether or not the number is a logical (bool) */
bool Number::isBool() const
{
    return num_type == ZERO or num_type == LOGICAL;
}

/** Return whether or not this number is an integer */
bool Number::isInteger() const
{
    return num_type == ZERO or
           num_type == INT8 or
           num_type == INT32 or
           num_type == INT64 or
           num_type == UINT8 or
           num_type == UINT32 or
           num_type == UINT64;
}

/** Return whether or not the number has a sign */
bool Number::isSigned() const
{
    return num_type == ZERO or
           num_type == INT8 or
           num_type == INT32 or
           num_type == INT64 or
           num_type == FLOAT32 or
           num_type == FLOAT64;
}

/** Return the number of bits used to store the number */
int Number::numBits() const
{
    switch(num_type)
    {
    case ZERO:
        return 0;
    
    case LOGICAL:
        return 1;
        
    case INT8:
    case UINT8:
        return 8;
        
    case INT32:
    case UINT32:
    case FLOAT32:
        return 32;
        
    case INT64:
    case UINT64:
    case FLOAT64:
        return 64;
    }
}

template<class T>
T toNum(int64 num)
{
    if (num < std::numeric_limits<T>::min() or
        num > std::numeric_limits<T>::max())
    {
        throw Siren::numeric_overflow( String::tr(
                "Overflow of %1 into an integer with %2 bytes. "
                "The maximum range of this type is %3 to %4.")
                    .arg(num)
                    .arg(sizeof(T))
                    .arg(std::numeric_limits<T>::min())
                    .arg(std::numeric_limits<T>::max()), CODELOC );
    }
    
    return T(num);
}

template<class T>
T toUNum(int64 num)
{
    if (num < 0 or
        num > std::numeric_limits<T>::max())
    {
        throw Siren::numeric_overflow( String::tr(
                "Overflow of %1 into an unsigned integer with %2 bytes. "
                "The maximum range of this type is 0 to %3.")
                    .arg(num)
                    .arg(sizeof(T))
                    .arg(std::numeric_limits<T>::max()), CODELOC );
    }
    
    return T(num);
};

/** Return this number as a 64 bit integer. This will
    raise an overflow error if the number cannot be cast
    
    \throw Siren::numeric_overflow
*/
int64 Number::toInt64() const
{
}


/** Return this number as an 8 bit integer. This will 
    raise an overflow error if the number cannot be cast
    
    \throw Siren::numeric_overflow
*/
int8 Number::toInt8() const
{
    if (num_type == INT8)
        return i8;
        
    else
    {
        return toNum( toInt64() );
    }
}

int32 Number::toInt32() const;

uint8 Number::toUInt8() const;
uint32 Number::toUInt32() const;
uint64 Number::toUInt64() const;

float Number::toFloat() const;
double Number::toDouble() const;

static Number Number::fromInt8(int8 number);
static Number Number::fromInt32(int32 number);
static Number Number::fromInt64(int64 number);

static Number Number::fromUInt8(uint8 number);
static Number Number::fromUInt32(uint32 number);
static Number Number::fromUInt64(uint64 number);

static Number Number::fromFloat(float number);
static Number Number::fromDouble(double number);

Number::operator int() const;
Number::operator double() const;
*/
