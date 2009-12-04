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

#include <boost/static_assert.hpp>

#include "Siren/primitive.h"
#include "Siren/stream.h"

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireUnits
{

namespace Dimension
{

class GeneralUnit;
namespace detail{ class Temperature; }

/** This is the base class of all units - at its heart, this is
    just a scale factor - how many times the base unit is the
    current value.

    @author Christopher Woods
*/
class SIREUNITS_EXPORT Unit
{

public:
    ~Unit();

    operator double() const;

    double value() const;

    double scaleFactor() const;

    double convertToInternal(double value) const;

    double convertFromInternal(double value) const;

protected:
    Unit(double scale_factor);
    
    Unit(const detail::Temperature &temperature);

    void setScale(double scale_factor);

    uint hashCode(int M, int L, int T, int C, int t, int Q, int A) const;
    bool test(Siren::Logger &test, int M, int L, int T, int C, int t, int Q, int A) const;

private:
    double sclfac;
};

QString getUnitString(double value, int M, int L, int T, int C, int t, int Q, int A);

/** Construct a physical unit with the specified
    Mass, Length, Time, Charge, temperature,
    Quantity and Angle dimensions

    @author Christopher Woods
*/
template<int M, int L, int T, int C, int t, int Q, int A>
class SIREUNITS_EXPORT PhysUnit : public Unit, 
                                  public Siren::Primitive< PhysUnit<M,L,T,C,t,Q,A> >
{
public:
    PhysUnit();

    explicit PhysUnit(double scale_factor);

    PhysUnit(const detail::Temperature &temperature);
    PhysUnit(const GeneralUnit &unit);

    PhysUnit(const PhysUnit<M,L,T,C,t,Q,A> &other);

    ~PhysUnit();

    static QString typeName();

    PhysUnit<M,L,T,C,t,Q,A>
    operator=(const PhysUnit<M,L,T,C,t,Q,A> &other);

    bool operator==(const PhysUnit<M,L,T,C,t,Q,A> &other) const;

    bool operator!=(const PhysUnit<M,L,T,C,t,Q,A> &other) const;

    PhysUnit<M,L,T,C,t,Q,A> operator-() const;

    PhysUnit<M,L,T,C,t,Q,A>
    operator+(const PhysUnit<M,L,T,C,t,Q,A> &other) const;

    PhysUnit<M,L,T,C,t,Q,A>
    operator-(const PhysUnit<M,L,T,C,t,Q,A> &other) const;

    PhysUnit<M,L,T,C,t,Q,A>&
    operator+=(const PhysUnit<M,L,T,C,t,Q,A> &other);

    PhysUnit<M,L,T,C,t,Q,A>&
    operator-=(const PhysUnit<M,L,T,C,t,Q,A> &other);

    PhysUnit<M,L,T,C,t,Q,A> operator*(double val) const;

    PhysUnit<M,L,T,C,t,Q,A> operator/(double val) const;

    PhysUnit<M,L,T,C,t,Q,A> operator*(int val) const;

    PhysUnit<M,L,T,C,t,Q,A> operator/(int val) const;

    template<int _M, int _L, int _T,
             int _C, int _t, int _Q, int _A>
    PhysUnit<M+_M, L+_L, T+_T, C+_C, t+_t, Q+_Q, A+_A>
    operator*(const PhysUnit<_M,_L,_T,_C,_t,_Q,_A> &other) const;

    template<int _M, int _L, int _T,
             int _C, int _t, int _Q, int _A>
    PhysUnit<M-_M, L-_L, T-_T, C-_C, t-_t, Q-_Q, A-_A>
    operator/(const PhysUnit<_M,_L,_T,_C,_t,_Q,_A> &other) const;

    PhysUnit<-M,-L,-T,-C,-t,-Q,-A> invert() const;

    double in(const PhysUnit<M,L,T,C,t,Q,A> &units) const;
    
    double to(const PhysUnit<M,L,T,C,t,Q,A> &units) const;

    QString toString() const;

    uint hashCode() const;
    bool test(Siren::Logger &logger) const;

    void stream(Siren::Stream &s);

    static int MASS();
    static int LENGTH();
    static int TIME();
    static int CHARGE();
    static int TEMPERATURE();
    static int QUANTITY();
    static int ANGLE();
};

#ifndef SIRE_SKIP_INLINE_FUNCTIONS

//////////
////////// Implementation of Unit
//////////

inline Unit::Unit(double scale_factor) : sclfac(scale_factor)
{}

inline Unit::~Unit()
{}

inline void Unit::setScale(double scale_factor)
{
    sclfac = scale_factor;
}

inline Unit::operator double() const
{
    return sclfac;
}

inline double Unit::value() const
{
    return sclfac;
}

inline double Unit::scaleFactor() const
{
    return sclfac;
}

//////////
////////// Implementation of PhysUnit
//////////

template<int M, int L, int T, int C, int t, int Q, int A>
SIRE_OUTOFLINE_TEMPLATE
PhysUnit<M,L,T,C,t,Q,A>::PhysUnit() 
        : Unit(0), Siren::Primitive< PhysUnit<M,L,T,C,t,Q,A> >()
{}

template<int M, int L, int T, int C, int t, int Q, int A>
SIRE_OUTOFLINE_TEMPLATE
PhysUnit<M,L,T,C,t,Q,A>::PhysUnit(double scale_factor)
           : Unit(scale_factor), Siren::Primitive< PhysUnit<M,L,T,C,t,Q,A> >()
{}

template<int M, int L, int T, int C, int t, int Q, int A>
SIRE_OUTOFLINE_TEMPLATE
PhysUnit<M,L,T,C,t,Q,A>::PhysUnit(const detail::Temperature &temperature) 
           : Unit(temperature), Siren::Primitive< PhysUnit<M,L,T,C,t,Q,A> >()
{
    //this must be a Temperature!
    BOOST_STATIC_ASSERT( t == 1 and M == 0 and L == 0 and 
                         T == 0 and C == 0 and Q == 0 and A == 0);
}

template<int M, int L, int T, int C, int t, int Q, int A>
SIRE_OUTOFLINE_TEMPLATE
PhysUnit<M,L,T,C,t,Q,A>::PhysUnit(const PhysUnit<M,L,T,C,t,Q,A> &other)
           : Unit(other), Siren::Primitive< PhysUnit<M,L,T,C,t,Q,A> >()
{}

template<int M, int L, int T, int C, int t, int Q, int A>
SIRE_OUTOFLINE_TEMPLATE
PhysUnit<M,L,T,C,t,Q,A>::~PhysUnit()
{}

template<int M, int L, int T, int C, int t, int Q, int A>
SIRE_OUTOFLINE_TEMPLATE
QString PhysUnit<M,L,T,C,t,Q,A>::typeName()
{
    return QMetaType::typeName( qMetaTypeId< PhysUnit<M,L,T,C,t,Q,A> >() );
}

template<int M, int L, int T, int C, int t, int Q, int A>
SIRE_OUTOFLINE_TEMPLATE
PhysUnit<M,L,T,C,t,Q,A>
PhysUnit<M,L,T,C,t,Q,A>::operator=(const PhysUnit<M,L,T,C,t,Q,A> &other)
{
    Unit::setScale(other.scaleFactor());
    return *this;
}

template<int M, int L, int T, int C, int t, int Q, int A>
SIRE_OUTOFLINE_TEMPLATE
bool PhysUnit<M,L,T,C,t,Q,A>::operator==(const PhysUnit<M,L,T,C,t,Q,A> &other) const
{
    return scaleFactor() == other.scaleFactor();
}

template<int M, int L, int T, int C, int t, int Q, int A>
SIRE_OUTOFLINE_TEMPLATE
bool PhysUnit<M,L,T,C,t,Q,A>::operator!=(const PhysUnit<M,L,T,C,t,Q,A> &other) const
{
    return scaleFactor() != other.scaleFactor();
}

template<int M, int L, int T, int C, int t, int Q, int A>
SIRE_OUTOFLINE_TEMPLATE
PhysUnit<M,L,T,C,t,Q,A> PhysUnit<M,L,T,C,t,Q,A>::operator-() const
{
    return PhysUnit<M,L,T,C,t,Q,A>( -scaleFactor() );
}

template<int M, int L, int T, int C, int t, int Q, int A>
SIRE_OUTOFLINE_TEMPLATE
PhysUnit<M,L,T,C,t,Q,A>
PhysUnit<M,L,T,C,t,Q,A>::operator+(const PhysUnit<M,L,T,C,t,Q,A> &other) const
{
    return PhysUnit<M,L,T,C,t,Q,A>(
              scaleFactor() + other.scaleFactor());
}

template<int M, int L, int T, int C, int t, int Q, int A>
SIRE_OUTOFLINE_TEMPLATE
PhysUnit<M,L,T,C,t,Q,A>
PhysUnit<M,L,T,C,t,Q,A>::operator-(const PhysUnit<M,L,T,C,t,Q,A> &other) const
{
    return PhysUnit<M,L,T,C,t,Q,A>(
              scaleFactor() - other.scaleFactor());
}

template<int M, int L, int T, int C, int t, int Q, int A>
SIRE_OUTOFLINE_TEMPLATE
PhysUnit<M,L,T,C,t,Q,A>&
PhysUnit<M,L,T,C,t,Q,A>::operator+=(const PhysUnit<M,L,T,C,t,Q,A> &other)
{
    Unit::setScale( scaleFactor() + other.scaleFactor() );
    return *this;
}

template<int M, int L, int T, int C, int t, int Q, int A>
SIRE_OUTOFLINE_TEMPLATE
PhysUnit<M,L,T,C,t,Q,A>&
PhysUnit<M,L,T,C,t,Q,A>::operator-=(const PhysUnit<M,L,T,C,t,Q,A> &other)
{
    Unit::setScale( scaleFactor() - other.scaleFactor() );
    return *this;
}

template<int M, int L, int T, int C, int t, int Q, int A>
SIRE_OUTOFLINE_TEMPLATE
PhysUnit<M,L,T,C,t,Q,A> PhysUnit<M,L,T,C,t,Q,A>::operator*(double val) const
{
    return PhysUnit<M,L,T,C,t,Q,A>(scaleFactor() * val);
}

template<int M, int L, int T, int C, int t, int Q, int A>
SIRE_OUTOFLINE_TEMPLATE
PhysUnit<M,L,T,C,t,Q,A> PhysUnit<M,L,T,C,t,Q,A>::operator/(double val) const
{
    return PhysUnit<M,L,T,C,t,Q,A>(scaleFactor() / val);
}

template<int M, int L, int T, int C, int t, int Q, int A>
SIRE_OUTOFLINE_TEMPLATE
PhysUnit<M,L,T,C,t,Q,A> PhysUnit<M,L,T,C,t,Q,A>::operator*(int val) const
{
    return PhysUnit<M,L,T,C,t,Q,A>(scaleFactor() * val);
}

template<int M, int L, int T, int C, int t, int Q, int A>
SIRE_OUTOFLINE_TEMPLATE
PhysUnit<M,L,T,C,t,Q,A> PhysUnit<M,L,T,C,t,Q,A>::operator/(int val) const
{
    return PhysUnit<M,L,T,C,t,Q,A>(scaleFactor() / val);
}

template<int M, int L, int T, int C, int t, int Q, int A>
template<int _M, int _L, int _T,
         int _C, int _t, int _Q, int _A>
SIRE_OUTOFLINE_TEMPLATE
PhysUnit<M+_M, L+_L, T+_T, C+_C, t+_t, Q+_Q, A+_A>
PhysUnit<M,L,T,C,t,Q,A>::operator*(const PhysUnit<_M,_L,_T,_C,_t,_Q,_A> &other) const
{
    return PhysUnit<M+_M,L+_L,T+_T,C+_C,t+_t,Q+_Q,A+_A>(
              scaleFactor() * other.scaleFactor());
}

template<int M, int L, int T, int C, int t, int Q, int A>
template<int _M, int _L, int _T,
         int _C, int _t, int _Q, int _A>
SIRE_OUTOFLINE_TEMPLATE
PhysUnit<M-_M, L-_L, T-_T, C-_C, t-_t, Q-_Q, A-_A>
PhysUnit<M,L,T,C,t,Q,A>::operator/(const PhysUnit<_M,_L,_T,_C,_t,_Q,_A> &other) const
{
    return PhysUnit<M-_M,L-_L,T-_T,C-_C,t-_t,Q-_Q,A-_A>(
              scaleFactor() / other.scaleFactor());
}

template<int M, int L, int T, int C, int t, int Q, int A>
SIRE_OUTOFLINE_TEMPLATE
PhysUnit<-M,-L,-T,-C,-t,-Q,-A> PhysUnit<M,L,T,C,t,Q,A>::invert() const
{
    return PhysUnit<-M,-L,-T,-C,-t,-Q,-A>( 1.0 / scaleFactor() );
}

template<int M, int L, int T, int C, int t, int Q, int A>
SIRE_OUTOFLINE_TEMPLATE
double PhysUnit<M,L,T,C,t,Q,A>::in(const PhysUnit<M,L,T,C,t,Q,A> &units) const
{
    return units.convertFromInternal(*this);
}

template<int M, int L, int T, int C, int t, int Q, int A>
SIRE_OUTOFLINE_TEMPLATE
double PhysUnit<M,L,T,C,t,Q,A>::to(const PhysUnit<M,L,T,C,t,Q,A> &units) const
{
    return this->in(units);
}

template<int M, int L, int T, int C, int t, int Q, int A>
SIRE_OUTOFLINE_TEMPLATE
QString PhysUnit<M,L,T,C,t,Q,A>::toString() const
{
    return SireUnits::Dimension::getUnitString(this->scaleFactor(), M,L,T,C,t,Q,A);
}

template<int M, int L, int T, int C, int t, int Q, int A>
SIRE_OUTOFLINE_TEMPLATE
uint PhysUnit<M,L,T,C,t,Q,A>::hashCode() const
{
    return Unit::hashCode(M,L,T,C,t,Q,A);
}

template<int M, int L, int T, int C, int t, int Q, int A>
SIRE_OUTOFLINE_TEMPLATE
bool PhysUnit<M,L,T,C,t,Q,A>::test(Siren::Logger &logger) const
{
    return Unit::test(logger, M, L, T, C, t, Q, A);
}

/** Stream this unit */
template<int M, int L, int T, int C, int t, int Q, int A>
SIRE_OUTOFLINE_TEMPLATE
void PhysUnit<M,L,T,C,t,Q,A>::stream(Siren::Stream &s)
{
    s.assertVersion< PhysUnit<M,L,T,C,t,Q,A> >(1);
    
    Siren::Schema schema = s.item< PhysUnit<M,L,T,C,t,Q,A> >();
    
    double v = this->value();
    
    schema.data("value") & v;
    
    if (s.isLoading())
        this->setScale(v);
}

template<int M, int L, int T, int C, int t, int Q, int A>
SIRE_OUTOFLINE_TEMPLATE
int PhysUnit<M,L,T,C,t,Q,A>::MASS()
{
    return M;
}

template<int M, int L, int T, int C, int t, int Q, int A>
SIRE_OUTOFLINE_TEMPLATE
int PhysUnit<M,L,T,C,t,Q,A>::LENGTH()
{
    return L;
}

template<int M, int L, int T, int C, int t, int Q, int A>
SIRE_OUTOFLINE_TEMPLATE
int PhysUnit<M,L,T,C,t,Q,A>::TIME()
{
    return T;
}

template<int M, int L, int T, int C, int t, int Q, int A>
SIRE_OUTOFLINE_TEMPLATE
int PhysUnit<M,L,T,C,t,Q,A>::CHARGE()
{
    return C;
}

template<int M, int L, int T, int C, int t, int Q, int A>
SIRE_OUTOFLINE_TEMPLATE
int PhysUnit<M,L,T,C,t,Q,A>::TEMPERATURE()
{
    return t;
}

template<int M, int L, int T, int C, int t, int Q, int A>
SIRE_OUTOFLINE_TEMPLATE
int PhysUnit<M,L,T,C,t,Q,A>::QUANTITY()
{
    return Q;
}

template<int M, int L, int T, int C, int t, int Q, int A>
SIRE_OUTOFLINE_TEMPLATE
int PhysUnit<M,L,T,C,t,Q,A>::ANGLE()
{
    return A;
}

template<int M, int L, int T, int C, int t, int Q, int A>
SIRE_OUTOFLINE_TEMPLATE
PhysUnit<M,L,T,C,t,Q,A> operator*(double val, const PhysUnit<M,L,T,C,t,Q,A> &unit)
{
    return PhysUnit<M,L,T,C,t,Q,A>( val * unit.scaleFactor() );
}

template<int M, int L, int T, int C, int t, int Q, int A>
SIRE_OUTOFLINE_TEMPLATE
PhysUnit<M,L,T,C,t,Q,A> operator*(int val, const PhysUnit<M,L,T,C,t,Q,A> &unit)
{
    return PhysUnit<M,L,T,C,t,Q,A>( val * unit.scaleFactor() );
}

template<int M, int L, int T, int C, int t, int Q, int A>
SIRE_OUTOFLINE_TEMPLATE
PhysUnit<-M,-L,-T,-C,-t,-Q,-A>
operator/(double val, const PhysUnit<M,L,T,C,t,Q,A> &unit)
{
    return PhysUnit<-M,-L,-T,-C,-t,-Q,-A>( val / unit.scaleFactor() );
}

template<int M, int L, int T, int C, int t, int Q, int A>
SIRE_OUTOFLINE_TEMPLATE
PhysUnit<-M,-L,-T,-C,-t,-Q,-A>
operator/(int val, const PhysUnit<M,L,T,C,t,Q,A> &unit)
{
    return PhysUnit<-M,-L,-T,-C,-t,-Q,-A>( val / unit.scaleFactor() );
}

#endif // SIRE_SKIP_INLINE_FUNCTIONS

/** Typedef the various unit dimensions (including derived units) */
typedef PhysUnit<0,0,0,0,0,0,0> Dimensionless;
typedef PhysUnit<1,0,0,0,0,0,0> Mass;
typedef PhysUnit<1,0,0,0,0,-1,0> MolarMass;
typedef PhysUnit<0,1,0,0,0,0,0> Length;
typedef PhysUnit<0,0,1,0,0,0,0> Time;
typedef PhysUnit<0,0,0,1,0,0,0> Charge;
typedef PhysUnit<0,0,0,1,0,-1,0> MolarCharge;
typedef PhysUnit<0,0,0,0,1,0,0> Temperature;
typedef PhysUnit<0,0,0,0,0,1,0> Quantity;
typedef PhysUnit<0,0,0,0,0,0,1> Angle;
typedef PhysUnit<0,2,0,0,0,0,0> Area;
typedef PhysUnit<0,3,0,0,0,0,0> Volume;
typedef PhysUnit<0,3,0,0,0,-1,0> MolarVolume;
typedef PhysUnit<0,1,-1,0,0,0,0> Velocity;
typedef PhysUnit<0,0,-1,0,0,0,1> AngularVelocity;
typedef PhysUnit<0,1,-2,0,0,0,0> Acceleration;
typedef PhysUnit<0,0,-2,0,0,0,1> AngularAcceleration;
typedef PhysUnit<1,2,-2,0,0,0,0> Energy;
typedef PhysUnit<1,2,-2,0,0,-1,0> MolarEnergy;
typedef PhysUnit<1,2,-3,0,0,0,0> Power;
typedef PhysUnit<1,2,-3,0,0,-1,0> MolarPower;
typedef PhysUnit<1,-3,0,0,0,0,0> Density;
typedef PhysUnit<1,-3,0,0,0,-1,0> MolarDensity;
typedef PhysUnit<1,1,-2,0,0,0,0> Force;
typedef PhysUnit<1,-1,-2,0,0,0,0> Pressure;
typedef PhysUnit<0,0,-1,1,0,0,0> Current;
typedef PhysUnit<-1,-2,2,2,0,0,0> Capacitance;
typedef PhysUnit<1,2,-2,-1,0,0,0> Potential;

} // end of namespace Dimension

}

Q_DECLARE_METATYPE( SireUnits::Dimension::Dimensionless )
Q_DECLARE_METATYPE( SireUnits::Dimension::Mass )
Q_DECLARE_METATYPE( SireUnits::Dimension::MolarMass )
Q_DECLARE_METATYPE( SireUnits::Dimension::Length )
Q_DECLARE_METATYPE( SireUnits::Dimension::Time )
Q_DECLARE_METATYPE( SireUnits::Dimension::Charge )
Q_DECLARE_METATYPE( SireUnits::Dimension::MolarCharge )
Q_DECLARE_METATYPE( SireUnits::Dimension::Temperature )
Q_DECLARE_METATYPE( SireUnits::Dimension::Quantity )
Q_DECLARE_METATYPE( SireUnits::Dimension::Angle )
Q_DECLARE_METATYPE( SireUnits::Dimension::Area )
Q_DECLARE_METATYPE( SireUnits::Dimension::Volume )
Q_DECLARE_METATYPE( SireUnits::Dimension::MolarVolume )
Q_DECLARE_METATYPE( SireUnits::Dimension::Velocity )
Q_DECLARE_METATYPE( SireUnits::Dimension::AngularVelocity )
Q_DECLARE_METATYPE( SireUnits::Dimension::Acceleration )
Q_DECLARE_METATYPE( SireUnits::Dimension::AngularAcceleration )
Q_DECLARE_METATYPE( SireUnits::Dimension::Energy )
Q_DECLARE_METATYPE( SireUnits::Dimension::MolarEnergy )
Q_DECLARE_METATYPE( SireUnits::Dimension::Power )
Q_DECLARE_METATYPE( SireUnits::Dimension::MolarPower )
Q_DECLARE_METATYPE( SireUnits::Dimension::Density )
Q_DECLARE_METATYPE( SireUnits::Dimension::MolarDensity )
Q_DECLARE_METATYPE( SireUnits::Dimension::Force )
Q_DECLARE_METATYPE( SireUnits::Dimension::Pressure )
Q_DECLARE_METATYPE( SireUnits::Dimension::Capacitance )
Q_DECLARE_METATYPE( SireUnits::Dimension::Current )
Q_DECLARE_METATYPE( SireUnits::Dimension::Potential )

Q_DECLARE_METATYPE( Siren::PrimitiveObject<SireUnits::Dimension::Dimensionless> )
Q_DECLARE_METATYPE( Siren::PrimitiveObject<SireUnits::Dimension::Mass> )
Q_DECLARE_METATYPE( Siren::PrimitiveObject<SireUnits::Dimension::MolarMass> )
Q_DECLARE_METATYPE( Siren::PrimitiveObject<SireUnits::Dimension::Length> )
Q_DECLARE_METATYPE( Siren::PrimitiveObject<SireUnits::Dimension::Time> )
Q_DECLARE_METATYPE( Siren::PrimitiveObject<SireUnits::Dimension::Charge> )
Q_DECLARE_METATYPE( Siren::PrimitiveObject<SireUnits::Dimension::MolarCharge> )
Q_DECLARE_METATYPE( Siren::PrimitiveObject<SireUnits::Dimension::Temperature> )
Q_DECLARE_METATYPE( Siren::PrimitiveObject<SireUnits::Dimension::Quantity> )
Q_DECLARE_METATYPE( Siren::PrimitiveObject<SireUnits::Dimension::Angle> )
Q_DECLARE_METATYPE( Siren::PrimitiveObject<SireUnits::Dimension::Area> )
Q_DECLARE_METATYPE( Siren::PrimitiveObject<SireUnits::Dimension::Volume> )
Q_DECLARE_METATYPE( Siren::PrimitiveObject<SireUnits::Dimension::MolarVolume> )
Q_DECLARE_METATYPE( Siren::PrimitiveObject<SireUnits::Dimension::Velocity> )
Q_DECLARE_METATYPE( Siren::PrimitiveObject<SireUnits::Dimension::AngularVelocity> )
Q_DECLARE_METATYPE( Siren::PrimitiveObject<SireUnits::Dimension::Acceleration> )
Q_DECLARE_METATYPE( Siren::PrimitiveObject<SireUnits::Dimension::AngularAcceleration> )
Q_DECLARE_METATYPE( Siren::PrimitiveObject<SireUnits::Dimension::Energy> )
Q_DECLARE_METATYPE( Siren::PrimitiveObject<SireUnits::Dimension::MolarEnergy> )
Q_DECLARE_METATYPE( Siren::PrimitiveObject<SireUnits::Dimension::Power> )
Q_DECLARE_METATYPE( Siren::PrimitiveObject<SireUnits::Dimension::MolarPower> )
Q_DECLARE_METATYPE( Siren::PrimitiveObject<SireUnits::Dimension::Density> )
Q_DECLARE_METATYPE( Siren::PrimitiveObject<SireUnits::Dimension::MolarDensity> )
Q_DECLARE_METATYPE( Siren::PrimitiveObject<SireUnits::Dimension::Force> )
Q_DECLARE_METATYPE( Siren::PrimitiveObject<SireUnits::Dimension::Pressure> )
Q_DECLARE_METATYPE( Siren::PrimitiveObject<SireUnits::Dimension::Capacitance> )
Q_DECLARE_METATYPE( Siren::PrimitiveObject<SireUnits::Dimension::Current> )
Q_DECLARE_METATYPE( Siren::PrimitiveObject<SireUnits::Dimension::Potential> )

Q_DECLARE_TYPEINFO( SireUnits::Dimension::Dimensionless, Q_MOVABLE_TYPE );
Q_DECLARE_TYPEINFO( SireUnits::Dimension::Mass, Q_MOVABLE_TYPE );
Q_DECLARE_TYPEINFO( SireUnits::Dimension::MolarMass, Q_MOVABLE_TYPE );
Q_DECLARE_TYPEINFO( SireUnits::Dimension::Length, Q_MOVABLE_TYPE );
Q_DECLARE_TYPEINFO( SireUnits::Dimension::Time, Q_MOVABLE_TYPE );
Q_DECLARE_TYPEINFO( SireUnits::Dimension::Charge, Q_MOVABLE_TYPE );
Q_DECLARE_TYPEINFO( SireUnits::Dimension::MolarCharge, Q_MOVABLE_TYPE );
Q_DECLARE_TYPEINFO( SireUnits::Dimension::Temperature, Q_MOVABLE_TYPE );
Q_DECLARE_TYPEINFO( SireUnits::Dimension::Quantity, Q_MOVABLE_TYPE );
Q_DECLARE_TYPEINFO( SireUnits::Dimension::Angle, Q_MOVABLE_TYPE );
Q_DECLARE_TYPEINFO( SireUnits::Dimension::Area, Q_MOVABLE_TYPE );
Q_DECLARE_TYPEINFO( SireUnits::Dimension::Volume, Q_MOVABLE_TYPE );
Q_DECLARE_TYPEINFO( SireUnits::Dimension::MolarVolume, Q_MOVABLE_TYPE );
Q_DECLARE_TYPEINFO( SireUnits::Dimension::Velocity, Q_MOVABLE_TYPE );
Q_DECLARE_TYPEINFO( SireUnits::Dimension::AngularVelocity, Q_MOVABLE_TYPE );
Q_DECLARE_TYPEINFO( SireUnits::Dimension::Acceleration, Q_MOVABLE_TYPE );
Q_DECLARE_TYPEINFO( SireUnits::Dimension::AngularAcceleration, Q_MOVABLE_TYPE );
Q_DECLARE_TYPEINFO( SireUnits::Dimension::Energy, Q_MOVABLE_TYPE );
Q_DECLARE_TYPEINFO( SireUnits::Dimension::MolarEnergy, Q_MOVABLE_TYPE );
Q_DECLARE_TYPEINFO( SireUnits::Dimension::Power, Q_MOVABLE_TYPE );
Q_DECLARE_TYPEINFO( SireUnits::Dimension::MolarPower, Q_MOVABLE_TYPE );
Q_DECLARE_TYPEINFO( SireUnits::Dimension::Density, Q_MOVABLE_TYPE );
Q_DECLARE_TYPEINFO( SireUnits::Dimension::MolarDensity, Q_MOVABLE_TYPE );
Q_DECLARE_TYPEINFO( SireUnits::Dimension::Force, Q_MOVABLE_TYPE );
Q_DECLARE_TYPEINFO( SireUnits::Dimension::Pressure, Q_MOVABLE_TYPE );
Q_DECLARE_TYPEINFO( SireUnits::Dimension::Capacitance, Q_MOVABLE_TYPE );
Q_DECLARE_TYPEINFO( SireUnits::Dimension::Current, Q_MOVABLE_TYPE );
Q_DECLARE_TYPEINFO( SireUnits::Dimension::Potential, Q_MOVABLE_TYPE );

SIRE_EXPOSE_CLASS( SireUnits::Dimension::Unit )

SIRE_END_HEADER

#endif
