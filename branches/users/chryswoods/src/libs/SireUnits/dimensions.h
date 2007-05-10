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

template<int M, int L, int T,
         int C, int t>
class ConvertedUnit;

/** Construct a physical unit with the specified
    Mass, Length, Time, Charge and temperature
    dimensions

    @author Christopher Woods
*/
template<int M, int L, int T,
         int C, int t>
class PhysUnit : public Unit
{
public:
    PhysUnit() : Unit(1)
    {}

    explicit PhysUnit(double scale_factor)
               : Unit(scale_factor)
    {}

    PhysUnit(const PhysUnit<M,L,T,C,t> &other)
               : Unit(other)
    {}

    ~PhysUnit()
    {}

    PhysUnit<M,L,T,C,t>
    operator=(const PhysUnit<M,L,T,C,t> &other)
    {
        Unit::setScale(other.scaleFactor());
        return *this;
    }

    bool operator==(const PhysUnit<M,L,T,C,t> &other) const
    {
        return scaleFactor() == other.scaleFactor();
    }

    bool operator!=(const PhysUnit<M,L,T,C,t> &other) const
    {
        return scaleFactor() != other.scaleFactor();
    }

    PhysUnit<M,L,T,C,t>
    operator+(const PhysUnit<M,L,T,C,t> &other) const
    {
        return PhysUnit<M,L,T,C,t>(
                  scaleFactor() + other.scaleFactor());
    }

    PhysUnit<M,L,T,C,t>
    operator-(const PhysUnit<M,L,T,C,t> &other) const
    {
        return PhysUnit<M,L,T,C,t>(
                  scaleFactor() - other.scaleFactor());
    }

    PhysUnit<M,L,T,C,t>&
    operator+=(const PhysUnit<M,L,T,C,t> &other)
    {
        Unit::setScale( scaleFactor() + other.scaleFactor() );
    }

    PhysUnit<M,L,T,C,t>&
    operator-=(const PhysUnit<M,L,T,C,t> &other)
    {
        Unit::setScale( scaleFactor() - other.scaleFactor() );
    }

    PhysUnit<M,L,T,C,t> operator*(double val) const
    {
        return PhysUnit<M,L,T,C,t>(scaleFactor() * val);
    }

    PhysUnit<M,L,T,C,t> operator/(double val) const
    {
        return PhysUnit<M,L,T,C,t>(scaleFactor() / val);
    }

    PhysUnit<M,L,T,C,t> operator*(int val) const
    {
        return PhysUnit<M,L,T,C,t>(scaleFactor() * val);
    }

    PhysUnit<M,L,T,C,t> operator/(int val) const
    {
        return PhysUnit<M,L,T,C,t>(scaleFactor() / val);
    }

    template<int _M, int _L, int _T,
             int _C, int _t>
    PhysUnit<M+_M, L+_L, T+_T, C+_C, t+_t>
    operator*(const PhysUnit<_M,_L,_T,_C,_t> &other) const
    {
        return PhysUnit<M+_M,L+_L,T+_T,C+_C,t+_t>(
                  scaleFactor() * other.scaleFactor());
    }

    template<int _M, int _L, int _T,
             int _C, int _t>
    PhysUnit<M-_M, L-_L, T-_T, C-_C, t-_t>
    operator/(const PhysUnit<_M,_L,_T,_C,_t> &other) const
    {
        return PhysUnit<M-_M,L-_L,T-_T,C-_C,t-_t>(
                  scaleFactor() / other.scaleFactor());
    }

    ConvertedUnit<M,L,T,C,t>
    in(const PhysUnit<M,L,T,C,t> &units) const
    {
        return ConvertedUnit<M,L,T,C,t>(*this, units);
    }

    template<class U>
    U in(const U &units) const;
};

template<int M, int L, int T, int C, int t>
PhysUnit<M,L,T,C,t>
operator*(double val, const PhysUnit<M,L,T,C,t> &unit)
{
    return PhysUnit<M,L,T,C,t>( val * unit.scaleFactor() );
}

template<int M, int L, int T, int C, int t>
PhysUnit<M,L,T,C,t>
operator*(int val, const PhysUnit<M,L,T,C,t> &unit)
{
    return PhysUnit<M,L,T,C,t>( val * unit.scaleFactor() );
}

/** This represents the result of a unit conversion -
    this holds the original value of the variable in the
    base units, as well as the current value in the
    converted units. This is so that this can be
    further converted into other units.

    @author Christopher Woods
*/
template<int M, int L, int T, int C, int t>
class ConvertedUnit : public PhysUnit<M,L,T,C,t>
{
public:
    ConvertedUnit() : PhysUnit<M,L,T,C,t>(), val(1)
    {}

    ConvertedUnit(const PhysUnit<M,L,T,C,t> &value,
                  const PhysUnit<M,L,T,C,t> &units)
           : PhysUnit<M,L,T,C,t>(value)
    {
        val = value.scaleFactor() / units.scaleFactor();
    }

    ConvertedUnit(const ConvertedUnit<M,L,T,C,t> &other)
           : PhysUnit<M,L,T,C,t>(other), val(other.val)
    {}

    ~ConvertedUnit()
    {}

    ConvertedUnit<M,L,T,C,t>&
    operator=(const ConvertedUnit<M,L,T,C,t> &other)
    {
        val = other.val;
        PhysUnit<M,L,T,C,t>::operator=(other);
        return *this;
    }

    ConvertedUnit<M,L,T,C,t>&
    operator=(const PhysUnit<M,L,T,C,t> &other)
    {
        val = 1;
        PhysUnit<M,L,T,C,t>::operator=(other);
        return *this;
    }

    operator double() const
    {
        return val;
    }

private:
    double val;
};

/** Typedef the various unit dimensions (including derived units) */
typedef PhysUnit<0,0,0,0,0> Quantity;
typedef ConvertedUnit<0,0,0,0,0> ConvertedQuantity;

typedef PhysUnit<1,0,0,0,0> Mass;
typedef ConvertedUnit<1,0,0,0,0> ConvertedMass;

typedef PhysUnit<0,1,0,0,0> Length;
typedef ConvertedUnit<0,1,0,0,0> ConvertedLength;

typedef PhysUnit<0,0,1,0,0> Time;
typedef ConvertedUnit<0,0,1,0,0> ConvertedTime;

typedef PhysUnit<0,0,0,1,0> Charge;
typedef ConvertedUnit<0,0,0,1,0> ConvertedCharge;

typedef PhysUnit<0,0,0,0,1> Temperature;
typedef ConvertedUnit<0,0,0,0,1> ConvertedTemperature;

typedef PhysUnit<0,2,0,0,0> Area;
typedef ConvertedUnit<0,2,0,0,0> ConvertedArea;

typedef PhysUnit<0,3,0,0,0> Volume;
typedef ConvertedUnit<0,3,0,0,0> ConvertedVolume;

typedef PhysUnit<0,1,-1,0,0> Velocity;
typedef ConvertedUnit<0,1,-1,0,0> ConvertedVelocity;

typedef PhysUnit<1,2,-2,0,0> Energy;
typedef ConvertedUnit<1,2,-2,0,0> ConvertedEnergy;

typedef PhysUnit<1,2,-3,0,0> Power;
typedef ConvertedUnit<1,2,-3,0,0> ConvertedPower;

typedef PhysUnit<1,-3,0,0,0> Density;
typedef ConvertedUnit<1,-3,0,0,0> ConvertedDensity;

typedef PhysUnit<1,1,-2,0,0> Force;
typedef ConvertedUnit<1,1,-2,0,0> ConvertedForce;

typedef PhysUnit<1,-1,-2,0,0> Pressure;
typedef ConvertedUnit<1,-1,-2,0,0> ConvertedPressure;

typedef PhysUnit<0,0,-1,1,0> ElecCurrent;
typedef ConvertedUnit<0,0,-1,1,0> ConvertedElecCurrent;

typedef PhysUnit<1,2,-2,-1,0> ElecPotential;
typedef ConvertedUnit<1,2,-2,-1,0> ConvertedElecPotential;

} // end of namespace Dimension

}

SIRE_END_HEADER

#endif
