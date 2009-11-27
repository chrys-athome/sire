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

#include "temperature.h"

using namespace SireUnits;
using namespace SireUnits::Dimension;
using namespace Siren;

///////////
/////////// Implementation of detail::Temperature
///////////

namespace SireUnits { namespace Dimension { namespace detail {

Temperature::Temperature(double value) : Extends<Temperature,Object>(), val(value)
{}

Temperature::Temperature(const SireUnits::Dimension::Temperature &temp)
            : Extends<Temperature,Object>(), val(temp)
{}            

Temperature::Temperature(const SireUnits::Dimension::detail::Temperature &other)
            : Extends<Temperature,Object>(other), val(other.val)
{}

Temperature::~Temperature()
{}

SireUnits::Dimension::detail::Temperature& 
Temperature::operator=(const SireUnits::Dimension::detail::Temperature &other)
{
    val = other.val;
    return *this;
}

bool Temperature::operator==(const SireUnits::Dimension::detail::Temperature &other) const
{
    return val == other.val;
}

bool Temperature::operator!=(const SireUnits::Dimension::detail::Temperature &other) const
{
    return val != other.val;
}

/** Convert this into a temperature object */
Temperature::operator SireUnits::Dimension::Temperature() const
{
    return SireUnits::Dimension::Temperature(val);
}

Temperature::operator double() const
{
    return val;
}

double Temperature::in(const SireUnits::Dimension::detail::Temperature &other) const
{
    return other.convertFromInternal(val) / other.convertFromInternal();
}

double Temperature::in(const SireUnits::Dimension::Temperature &temp) const
{
    return val * temp;
}

double Temperature::to(const SireUnits::Dimension::detail::Temperature &other) const
{
    return this->in(other);
}

double Temperature::convertFromInternal() const
{
    return this->convertFromInternal(val);
}

} } } //end of namespace SireUnits::Dimension::detail

///////////
/////////// Implementation of Celsius
///////////

Celsius::Celsius() 
        : Implements<Celsius,SireUnits::Dimension::detail::Temperature>(1)
{}

Celsius::Celsius(double value)
        : Implements<Celsius,SireUnits::Dimension::detail::Temperature>(1)

{
    val = convertToInternal(value);
}

Celsius::Celsius(const SireUnits::Dimension::Temperature &temperature)
        : Implements<Celsius,SireUnits::Dimension::detail::Temperature>(temperature)
{}

Celsius::Celsius(const SireUnits::Dimension::detail::Temperature &temperature) 
        : Implements<Celsius,SireUnits::Dimension::detail::Temperature>(temperature)
{}

Celsius::Celsius(const Celsius &other)
        : Implements<Celsius,SireUnits::Dimension::detail::Temperature>(other)
{}

Celsius::~Celsius()
{}

QString Celsius::toString() const
{
    return QString("%1 °C").arg( convertFromInternal() );
}

void Celsius::stream(Siren::Stream &s)
{
    s.assertVersion<Celsius>(1);
    
    Schema schema = s.item<Celsius>();
    
    double v = convertFromInternal();
    
    schema.data("temperature") & v;
    
    if (s.isSaving())
        val = convertToInternal(v);
}

double Celsius::convertToInternal(double value) const
{
    return value + 273.15;
}

double Celsius::convertFromInternal(double value) const
{
    return value - 273.15;
}

double Celsius::convertFromInternal() const
{
    return Temperature::convertFromInternal();
}

Celsius& Celsius::operator=(const Celsius &other)
{
    Temperature::operator=(other);
    return *this;
}

bool Celsius::operator==(const Celsius &other) const
{
    return val == other.val;
}

bool Celsius::operator!=(const Celsius &other) const
{
    return not Celsius::operator==(other);
}

Celsius Celsius::operator-() const
{
    return Celsius(-convertFromInternal());
}

Celsius Celsius::operator+(const Celsius &other) const
{
    return Celsius(convertFromInternal() + other.convertFromInternal());
}

Celsius Celsius::operator-(const Celsius &other) const
{
    return Celsius(convertFromInternal() - other.convertFromInternal());
}

Celsius& Celsius::operator+=(const Celsius &other)
{
    convertToInternal( convertFromInternal() + other.convertFromInternal() );
    return *this;
}

Celsius& Celsius::operator-=(const Celsius &other)
{
    convertToInternal( convertFromInternal() - other.convertFromInternal() );
    return *this;
}

Celsius Celsius::operator*(double value) const
{
    return Celsius(value * convertFromInternal());
}

Celsius Celsius::operator/(double value) const
{
    return Celsius(value / convertFromInternal());
}

Celsius Celsius::operator*(int value) const
{
    return Celsius(value * convertFromInternal());
}

Celsius Celsius::operator/(int value) const
{
    return Celsius(value / convertFromInternal());
}

///////////
/////////// Implementation of Fahrenheit
///////////

Fahrenheit::Fahrenheit() 
        : Implements<Fahrenheit,SireUnits::Dimension::detail::Temperature>(1)
{}

Fahrenheit::Fahrenheit(double value)
        : Implements<Fahrenheit,SireUnits::Dimension::detail::Temperature>(1)

{
    val = convertToInternal(value);
}

Fahrenheit::Fahrenheit(const SireUnits::Dimension::Temperature &temperature)
        : Implements<Fahrenheit,SireUnits::Dimension::detail::Temperature>(temperature)
{}

Fahrenheit::Fahrenheit(const SireUnits::Dimension::detail::Temperature &temperature) 
        : Implements<Fahrenheit,SireUnits::Dimension::detail::Temperature>(temperature)
{}

Fahrenheit::Fahrenheit(const Fahrenheit &other)
        : Implements<Fahrenheit,SireUnits::Dimension::detail::Temperature>(other)
{}

Fahrenheit::~Fahrenheit()
{}

QString Fahrenheit::toString() const
{
    return QString("%1 °F").arg( convertFromInternal() );
}

void Fahrenheit::stream(Siren::Stream &s)
{
    s.assertVersion<Fahrenheit>(1);
    
    Schema schema = s.item<Fahrenheit>();
    
    double v = convertFromInternal();
    
    schema.data("temperature") & v;
    
    if (s.isSaving())
        val = convertToInternal(v);
}

double Fahrenheit::convertToInternal(double value) const
{
    return (value + 459.67) / 1.8;
}

double Fahrenheit::convertFromInternal(double value) const
{
    return (value * 1.8) - 459.67;
}

double Fahrenheit::convertFromInternal() const
{
    return Temperature::convertFromInternal();
}

Fahrenheit& Fahrenheit::operator=(const Fahrenheit &other)
{
    Temperature::operator=(other);
    return *this;
}

bool Fahrenheit::operator==(const Fahrenheit &other) const
{
    return val == other.val;
}

bool Fahrenheit::operator!=(const Fahrenheit &other) const
{
    return not Fahrenheit::operator==(other);
}

Fahrenheit Fahrenheit::operator-() const
{
    return Fahrenheit(-convertFromInternal());
}

Fahrenheit Fahrenheit::operator+(const Fahrenheit &other) const
{
    return Fahrenheit(convertFromInternal() + other.convertFromInternal());
}

Fahrenheit Fahrenheit::operator-(const Fahrenheit &other) const
{
    return Fahrenheit(convertFromInternal() - other.convertFromInternal());
}

Fahrenheit& Fahrenheit::operator+=(const Fahrenheit &other)
{
    convertToInternal( convertFromInternal() + other.convertFromInternal() );
    return *this;
}

Fahrenheit& Fahrenheit::operator-=(const Fahrenheit &other)
{
    convertToInternal( convertFromInternal() - other.convertFromInternal() );
    return *this;
}

Fahrenheit Fahrenheit::operator*(double value) const
{
    return Fahrenheit(value * convertFromInternal());
}

Fahrenheit Fahrenheit::operator/(double value) const
{
    return Fahrenheit(value / convertFromInternal());
}

Fahrenheit Fahrenheit::operator*(int value) const
{
    return Fahrenheit(value * convertFromInternal());
}

Fahrenheit Fahrenheit::operator/(int value) const
{
    return Fahrenheit(value / convertFromInternal());
}

//////////
////////// Operators
//////////

namespace SireUnits
{

Celsius SIREUNITS_EXPORT operator*(double value, const Celsius &temperature)
{
    return temperature * value;
}

Celsius SIREUNITS_EXPORT operator*(int value, const Celsius &temperature)
{
    return temperature * value;
}

Dimension::PhysUnit<0,0,0,0,-1,0,0> 
SIREUNITS_EXPORT operator/(double value, const Celsius &temp)
{
    return Dimension::PhysUnit<0,0,0,0,-1,0,0>(value / temp.convertFromInternal());
}

Dimension::PhysUnit<0,0,0,0,-1,0,0> 
SIREUNITS_EXPORT operator/(int value, const Celsius &temp)
{
    return Dimension::PhysUnit<0,0,0,0,-1,0,0>(value / temp.convertFromInternal());
}

Fahrenheit SIREUNITS_EXPORT operator*(double value, const Fahrenheit &temperature)
{
    return temperature * value;
}

Fahrenheit SIREUNITS_EXPORT operator*(int value, const Fahrenheit &temperature)
{
    return temperature * value;
}

Dimension::PhysUnit<0,0,0,0,-1,0,0> 
SIREUNITS_EXPORT operator/(double value, const Fahrenheit &temp)
{
    return Dimension::PhysUnit<0,0,0,0,-1,0,0>(value / temp.convertFromInternal());
}

Dimension::PhysUnit<0,0,0,0,-1,0,0> 
SIREUNITS_EXPORT operator/(int value, const Fahrenheit &temp)
{
    return Dimension::PhysUnit<0,0,0,0,-1,0,0>(value / temp.convertFromInternal());
}

} // end of namespace SireUnits
