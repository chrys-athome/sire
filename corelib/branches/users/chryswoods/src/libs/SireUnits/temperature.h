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

#include "Siren/object.h"

SIRE_BEGIN_HEADER

namespace SireUnits
{

class Celsius;
class Fahrenheit;

namespace Dimension
{

namespace detail
{

class SIREUNITS_EXPORT Temperature : public Siren::Extends<Temperature,Siren::Object>
{

friend class SireUnits::Celsius;
friend class SireUnits::Fahrenheit;

public:
    Temperature(double value=0);
    Temperature(const SireUnits::Dimension::Temperature &temperature);

    Temperature(const SireUnits::Dimension::detail::Temperature &other);

    virtual ~Temperature();

    uint hashCode() const;
    void stream(Siren::Stream &s);

    Temperature& operator=(const SireUnits::Dimension::Temperature &temperature);
    Temperature& operator=(const SireUnits::Dimension::detail::Temperature &other);

    bool operator==(const SireUnits::Dimension::Temperature &temp) const;
    bool operator!=(const SireUnits::Dimension::Temperature &temp) const;

    bool operator==(const SireUnits::Dimension::detail::Temperature &other) const;
    bool operator!=(const SireUnits::Dimension::detail::Temperature &other) const;

    operator SireUnits::Dimension::Temperature() const;

    operator double() const;

    double in(const SireUnits::Dimension::detail::Temperature &other) const;
    double in(const SireUnits::Dimension::Temperature &temp) const;

    double to(const SireUnits::Dimension::detail::Temperature &other) const;

    virtual double convertToInternal(double value) const=0;
    virtual double convertFromInternal(double value) const=0;

    double convertFromInternal() const;

protected:
    /** This holds the temperature in internal units (Kelvin) */
    double val;
};

} // end of namespace detail

} //end of namespace Dimension

class Celsius : public Siren::Implements<Celsius,Dimension::detail::Temperature>
{

public:
    Celsius();

    explicit Celsius(double value);

    Celsius(const SireUnits::Dimension::Temperature &temperature);

    Celsius(const SireUnits::Dimension::detail::Temperature &temperature);

    Celsius(const Celsius &other);

    ~Celsius();

    QString toString() const;
    void stream(Siren::Stream &s);

    double convertToInternal(double value) const;

    double convertFromInternal(double value) const;

    double convertFromInternal() const;

    Celsius& operator=(const Celsius &other);

    bool operator==(const Celsius &other) const;
    bool operator!=(const Celsius &other) const;

    Celsius operator-() const;

    Celsius operator+(const Celsius &other) const;

    Celsius operator-(const Celsius &other) const;

    Celsius& operator+=(const Celsius &other);

    Celsius& operator-=(const Celsius &other);

    Celsius operator*(double value) const;

    Celsius operator/(double value) const;

    Celsius operator*(int value) const;

    Celsius operator/(int value) const;
};

class SIREUNITS_EXPORT Fahrenheit 
        : public Siren::Implements<Fahrenheit,Dimension::detail::Temperature>
{

public:
    Fahrenheit();
    
    explicit Fahrenheit(double value);

    Fahrenheit(const SireUnits::Dimension::Temperature &temperature);

    Fahrenheit(const SireUnits::Dimension::detail::Temperature &temperature);

    Fahrenheit(const Fahrenheit &other);

    ~Fahrenheit();

    QString toString() const;
    void stream(Siren::Stream &s);

    Fahrenheit& operator=(const Fahrenheit &other);

    bool operator==(const Fahrenheit &other) const;
    bool operator!=(const Fahrenheit &other) const;

    double convertToInternal(double value) const;

    double convertFromInternal(double value) const;

    double convertFromInternal() const;

    Fahrenheit operator-() const;

    Fahrenheit operator+(const Fahrenheit &other) const;

    Fahrenheit operator-(const Fahrenheit &other) const;

    Fahrenheit& operator+=(const Fahrenheit &other);

    Fahrenheit& operator-=(const Fahrenheit &other);

    Fahrenheit operator*(double value) const;

    Fahrenheit operator/(double value) const;

    Fahrenheit operator*(int value) const;

    Fahrenheit operator/(int value) const;
};

Celsius operator*(double value, const Celsius &temperature);
Celsius operator*(int value, const Celsius &temperature);

Dimension::PhysUnit<0,0,0,0,-1,0,0> operator/(double value, const Celsius &temperature);
Dimension::PhysUnit<0,0,0,0,-1,0,0> operator/(int value, const Celsius &temperature);

Fahrenheit operator*(double value, const Fahrenheit &temperature);
Fahrenheit operator*(int value, const Fahrenheit &temperature);

Dimension::PhysUnit<0,0,0,0,-1,0,0> operator/(double value, const Fahrenheit &temperature);
Dimension::PhysUnit<0,0,0,0,-1,0,0> operator/(int value, const Fahrenheit &temperature);

const Celsius celsius(1);
const Fahrenheit fahrenheit(1);

}

Q_DECLARE_METATYPE( SireUnits::Celsius )
Q_DECLARE_METATYPE( SireUnits::Fahrenheit )

SIRE_EXPOSE_ALIAS( SireUnits::Dimension::detail::Temperature, detail_Temperature )
SIRE_EXPOSE_CLASS( SireUnits::Celsius )
SIRE_EXPOSE_CLASS( SireUnits::Fahrenheit )

SIRE_END_HEADER

#endif
