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

#ifndef SIREUNITS_DIMENSIONS_H
#define SIREUNITS_DIMENSIONS_H

#include "SireMaths/constants.h"

SIRE_BEGIN_HEADER

namespace SireUnits
{

using SireMaths::pi;


namespace Dimension
{

/** This is the base class of all units - at its heart, this is
    just a scale factor - how many times the base unit is the
    current value.

    @author Christopher Woods
*/
class Unit
{
public:
    ~Unit()
    {}

    operator double() const
    {
        return sclfac;
    }

    double scaleFactor() const
    {
        return sclfac;
    }

    double convertToInternal(double value) const
    {
        return value * sclfac;
    }

    double convertFromInternal(double value) const
    {
        return value / sclfac;
    }

protected:
    Unit(double scale_factor) : sclfac(scale_factor)
    {}

    void setScale(double scale_factor)
    {
        sclfac = scale_factor;
    }

private:
    double sclfac;
};

/** Construct a physical unit with the specified
    Mass, Length, Time, Charge, temperature
    and Quantity dimensions

    @author Christopher Woods
*/
template<int M, int L, int T,
         int C, int t, int Q>
class PhysUnit : public Unit
{
public:
    PhysUnit() : Unit(1)
    {}

    explicit PhysUnit(double scale_factor)
               : Unit(scale_factor)
    {}

    PhysUnit(const PhysUnit<M,L,T,C,t,Q> &other)
               : Unit(other)
    {}

    ~PhysUnit()
    {}

    PhysUnit<M,L,T,C,t,Q>
    operator=(const PhysUnit<M,L,T,C,t,Q> &other)
    {
        Unit::setScale(other.scaleFactor());
        return *this;
    }

    bool operator==(const PhysUnit<M,L,T,C,t,Q> &other) const
    {
        return scaleFactor() == other.scaleFactor();
    }

    bool operator!=(const PhysUnit<M,L,T,C,t,Q> &other) const
    {
        return scaleFactor() != other.scaleFactor();
    }

    PhysUnit<M,L,T,C,t,Q> operator-() const
    {
        return PhysUnit<M,L,T,C,t,Q>( -scaleFactor() );
    }

    PhysUnit<M,L,T,C,t,Q>
    operator+(const PhysUnit<M,L,T,C,t,Q> &other) const
    {
        return PhysUnit<M,L,T,C,t,Q>(
                  scaleFactor() + other.scaleFactor());
    }

    PhysUnit<M,L,T,C,t,Q>
    operator-(const PhysUnit<M,L,T,C,t,Q> &other) const
    {
        return PhysUnit<M,L,T,C,t,Q>(
                  scaleFactor() - other.scaleFactor());
    }

    PhysUnit<M,L,T,C,t,Q>&
    operator+=(const PhysUnit<M,L,T,C,t,Q> &other)
    {
        Unit::setScale( scaleFactor() + other.scaleFactor() );
        return *this;
    }

    PhysUnit<M,L,T,C,t,Q>&
    operator-=(const PhysUnit<M,L,T,C,t,Q> &other)
    {
        Unit::setScale( scaleFactor() - other.scaleFactor() );
        return *this;
    }

    PhysUnit<M,L,T,C,t,Q> operator*(double val) const
    {
        return PhysUnit<M,L,T,C,t,Q>(scaleFactor() * val);
    }

    PhysUnit<M,L,T,C,t,Q> operator/(double val) const
    {
        return PhysUnit<M,L,T,C,t,Q>(scaleFactor() / val);
    }

    PhysUnit<M,L,T,C,t,Q> operator*(int val) const
    {
        return PhysUnit<M,L,T,C,t,Q>(scaleFactor() * val);
    }

    PhysUnit<M,L,T,C,t,Q> operator/(int val) const
    {
        return PhysUnit<M,L,T,C,t,Q>(scaleFactor() / val);
    }

    template<int _M, int _L, int _T,
             int _C, int _t, int _Q>
    PhysUnit<M+_M, L+_L, T+_T, C+_C, t+_t, Q+_Q>
    operator*(const PhysUnit<_M,_L,_T,_C,_t,_Q> &other) const
    {
        return PhysUnit<M+_M,L+_L,T+_T,C+_C,t+_t,Q+_Q>(
                  scaleFactor() * other.scaleFactor());
    }

    template<int _M, int _L, int _T,
             int _C, int _t, int _Q>
    PhysUnit<M-_M, L-_L, T-_T, C-_C, t-_t, Q-_Q>
    operator/(const PhysUnit<_M,_L,_T,_C,_t,_Q> &other) const
    {
        return PhysUnit<M-_M,L-_L,T-_T,C-_C,t-_t,Q-_Q>(
                  scaleFactor() / other.scaleFactor());
    }

    double in(const PhysUnit<M,L,T,C,t,Q> &units) const
    {
        return units.convertFromInternal(*this);
    }
};

template<int M, int L, int T, int C, int t, int Q>
PhysUnit<M,L,T,C,t,Q>
operator*(double val, const PhysUnit<M,L,T,C,t,Q> &unit)
{
    return PhysUnit<M,L,T,C,t,Q>( val * unit.scaleFactor() );
}

template<int M, int L, int T, int C, int t, int Q>
PhysUnit<M,L,T,C,t,Q>
operator*(int val, const PhysUnit<M,L,T,C,t,Q> &unit)
{
    return PhysUnit<M,L,T,C,t,Q>( val * unit.scaleFactor() );
}

template<int M, int L, int T, int C, int t, int Q>
PhysUnit<-M,-L,-T,-C,-t,-Q>
operator/(double val, const PhysUnit<M,L,T,C,t,Q> &unit)
{
    return PhysUnit<-M,-L,-T,-C,-t,-Q>( val / unit.scaleFactor() );
}

template<int M, int L, int T, int C, int t, int Q>
PhysUnit<-M,-L,-T,-C,-t,-Q>
operator/(int val, const PhysUnit<M,L,T,C,t,Q> &unit)
{
    return PhysUnit<-M,-L,-T,-C,-t,-Q>( val / unit.scaleFactor() );
}

/** Typedef the various unit dimensions (including derived units) */
#ifndef SKIP_BROKEN_GCCXML_PARTS
typedef PhysUnit<0,0,0,0,0,0> Dimensionless;

typedef PhysUnit<1,0,0,0,0,0> Mass;

typedef PhysUnit<1,0,0,0,0,-1> MolarMass;

typedef PhysUnit<0,1,0,0,0,0> Length;

typedef PhysUnit<0,0,1,0,0,0> Time;

typedef PhysUnit<0,0,0,1,0,0> Charge;

typedef PhysUnit<0,0,0,1,0,-1> MolarCharge;

typedef PhysUnit<0,0,0,0,1,0> Temperature;

typedef PhysUnit<0,0,0,0,0,1> Quantity;

typedef PhysUnit<0,2,0,0,0,0> Area;

typedef PhysUnit<0,3,0,0,0,0> Volume;

typedef PhysUnit<0,3,0,0,0,-1> MolarVolume;

typedef PhysUnit<0,1,-1,0,0,0> Velocity;

typedef PhysUnit<0,1,-2,0,0,0> Acceleration;

typedef PhysUnit<1,2,-2,0,0,0> Energy;

typedef PhysUnit<1,2,-2,0,0,-1> MolarEnergy;

typedef PhysUnit<1,2,-3,0,0,0> Power;

typedef PhysUnit<1,2,-3,0,0,-1> MolarPower;

typedef PhysUnit<1,-3,0,0,0,0> Density;

typedef PhysUnit<1,-3,0,0,0,-1> MolarDensity;

typedef PhysUnit<1,1,-2,0,0,0> Force;

typedef PhysUnit<1,-1,-2,0,0,0> Pressure;

typedef PhysUnit<0,0,-1,1,0,0> ElecCurrent;

typedef PhysUnit<1,2,-2,-1,0,0> ElecPotential;

#else // else with 'ifndef SKIP_BROKEN_GCCXML_PARTS'

class Dimensionless;
class Mass;
class MolarMass;
class Length;
class Time;
class Charge;
class MolarCharge;
class Temperature;
class Quantity;
class Area;
class Volume;
class MolarVolume;
class Velocity;
class Acceleration;
class Energy;
class MolarEnergy;
class Power;
class MolarPower;
class Density;
class MolarDensity;
class Force;
class Pressure;
class ElecCurrent;
class ElecPotential;

#endif // end of 'ifndef SKIP_BROKEN_GCCXML_PARTS'

} // end of namespace Dimension

}

SIRE_END_HEADER

#endif
