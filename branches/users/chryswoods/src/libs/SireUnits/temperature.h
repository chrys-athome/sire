/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2007  Christopher Woods
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

#ifndef SIREUNITS_TEMPERATURE
#define SIREUNITS_TEMPERATURE

#include "dimensions.h"

SIRE_BEGIN_HEADER

namespace SireUnits
{

class Celsius;
class Fahrenheit;

namespace Dimension
{

class TempBase
{
friend class Celsius;
friend class Fahrenheit;

public:
    TempBase(double value = 0) : val(value)
    {}

    TempBase(const TempBase &other) : val(other.val)
    {}

    virtual ~TempBase()
    {}

    operator Temperature() const
    {
        return Temperature( convertToInternal() );
    }

    operator double() const
    {
        return val;
    }

protected:
    TempBase& operator=(const TempBase &other)
    {
        val = other.val;
        return *this;
    }

    virtual double convertToInternal() const=0;
    virtual void convertFromInternal(double value)=0;

    double val;
};

} //end of namespace Dimension

class Celsius : public Dimension::TempBase
{
protected:
    double convertToInternal() const
    {
        return val + 273.15;
    }

    void convertFromInternal(double value)
    {
        val = value - 273.15;
    }

public:
    Celsius() : Dimension::TempBase(1)
    {}

    explicit Celsius(double value) : Dimension::TempBase(value)
    {}

    Celsius(const Dimension::Temperature &temp) : Dimension::TempBase()
    {
        convertFromInternal(temp);
    }

    Celsius(const Dimension::TempBase &other) : Dimension::TempBase()
    {
        convertFromInternal( other.convertToInternal() );
    }

    Celsius(const Celsius &other)
          : Dimension::TempBase(other)
    {}

    ~Celsius()
    {}

    Celsius& operator=(const Celsius &other)
    {
        Dimension::TempBase::operator=(other);
        return *this;
    }

    Celsius& operator=(const Dimension::Temperature &temp)
    {
        convertFromInternal(temp);
        return *this;
    }

    bool operator==(const Celsius &other) const
    {
        return val == other.val;
    }

    bool operator!=(const Celsius &other) const
    {
        return val != other.val;
    }

    Celsius operator+(const Celsius &other) const
    {
        return Celsius(val + other.val);
    }

    Celsius operator-(const Celsius &other) const
    {
        return Celsius(val - other.val);
    }

    Celsius& operator+=(const Celsius &other)
    {
        val += other.val;
        return *this;
    }

    Celsius& operator-=(const Celsius &other)
    {
        val -= other.val;
        return *this;
    }

    Celsius operator+(const Dimension::Temperature &other) const
    {
        return *this + Celsius(other);
    }

    Celsius operator-(const Dimension::Temperature &other) const
    {
        return *this - Celsius(other);
    }

    Celsius& operator+=(const Dimension::Temperature &other)
    {
        val += Celsius(other);
        return *this;
    }

    Celsius& operator-=(const Dimension::Temperature &other)
    {
        val -= Celsius(other);
        return *this;
    }

    Celsius operator*(double value) const
    {
        return Celsius(val * value);
    }

    Celsius operator/(double value) const
    {
        return Celsius(val / value);
    }

    Celsius operator*(int value) const
    {
        return Celsius(val * value);
    }

    Celsius operator/(int value) const
    {
        return Celsius(val / value);
    }
};

Celsius operator*(double value, const Celsius &temp)
{
    return Celsius(value * double(temp));
}

Dimension::PhysUnit<0,0,0,0,-1> operator/(double value, const Celsius &temp)
{
    return Dimension::PhysUnit<0,0,0,0,-1>(value / double(temp));
}

Celsius operator*(int value, const Celsius &temp)
{
    return double(value) * temp;
}

Dimension::PhysUnit<0,0,0,0,-1> operator/(int value, const Celsius &temp)
{
    return double(value) / temp;
}

class Fahrenheit : public Dimension::TempBase
{
protected:
    double convertToInternal() const
    {
        return (val + 459.67) / 1.8;
    }

    void convertFromInternal(double value)
    {
        val = (value * 1.8) - 459.67;
    }

public:
    Fahrenheit() : Dimension::TempBase(1)
    {}

    explicit Fahrenheit(double value) : Dimension::TempBase(value)
    {}

    Fahrenheit(const Dimension::Temperature &temp) : Dimension::TempBase()
    {
        convertFromInternal(temp);
    }

    Fahrenheit(const Dimension::TempBase &other) : Dimension::TempBase()
    {
        convertFromInternal( other.convertToInternal() );
    }

    Fahrenheit(const Fahrenheit &other)
          : Dimension::TempBase(other)
    {}

    ~Fahrenheit()
    {}

    Fahrenheit& operator=(const Fahrenheit &other)
    {
        Dimension::TempBase::operator=(other);
        return *this;
    }

    Fahrenheit& operator=(const Dimension::Temperature &temp)
    {
        convertFromInternal(temp);
        return *this;
    }

    bool operator==(const Fahrenheit &other) const
    {
        return val == other.val;
    }

    bool operator!=(const Fahrenheit &other) const
    {
        return val != other.val;
    }

    Fahrenheit operator+(const Fahrenheit &other) const
    {
        return Fahrenheit(val + other.val);
    }

    Fahrenheit operator-(const Fahrenheit &other) const
    {
        return Fahrenheit(val - other.val);
    }

    Fahrenheit& operator+=(const Fahrenheit &other)
    {
        val += other.val;
        return *this;
    }

    Fahrenheit& operator-=(const Fahrenheit &other)
    {
        val -= other.val;
        return *this;
    }

    Fahrenheit operator+(const Dimension::Temperature &other) const
    {
        return *this + Fahrenheit(other);
    }

    Fahrenheit operator-(const Dimension::Temperature &other) const
    {
        return *this - Fahrenheit(other);
    }

    Fahrenheit& operator+=(const Dimension::Temperature &other)
    {
        val += Fahrenheit(other);
        return *this;
    }

    Fahrenheit& operator-=(const Dimension::Temperature &other)
    {
        val -= Fahrenheit(other);
        return *this;
    }

    Fahrenheit operator*(double value) const
    {
        return Fahrenheit(val * value);
    }

    Fahrenheit operator/(double value) const
    {
        return Fahrenheit(val / value);
    }

    Fahrenheit operator*(int value) const
    {
        return Fahrenheit(val * value);
    }

    Fahrenheit operator/(int value) const
    {
        return Fahrenheit(val / value);
    }

    template<class T>
    T in() const
    {
        return T(*this);
    }
};

Fahrenheit operator*(double value, const Fahrenheit &temp)
{
    return Fahrenheit(value * double(temp));
}

Dimension::PhysUnit<0,0,0,0,-1> operator/(double value, const Fahrenheit &temp)
{
    return Dimension::PhysUnit<0,0,0,0,-1>( value / (1.8*double(temp)) );
}

Fahrenheit operator*(int value, const Fahrenheit &temp)
{
    return double(value) * temp;
}

Dimension::PhysUnit<0,0,0,0,-1> operator/(int value, const Fahrenheit &temp)
{
    return double(value) / temp;
}

const Celsius celsius(1);
const Fahrenheit fahrenheit(1);

}

SIRE_END_HEADER

#endif
