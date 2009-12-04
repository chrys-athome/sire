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

#ifndef SIREUNITS_GENERALUNIT_H
#define SIREUNITS_GENERALUNIT_H

#include "SireUnits/dimensions.h"

SIRE_BEGIN_HEADER

namespace SireUnits
{

namespace Dimension
{

class GeneralUnit;

namespace detail 
{ 
    class Temperature; 
    void throwIncompatible(const GeneralUnit &unit,
                           int M, int L, int T, int C, int t, int Q, int A);
}

/** This class can hold a general unit - this is used when
    there isn't a template-specialised form of the unit, and
    also when we need non-template code to handle units */
class SIREUNITS_EXPORT GeneralUnit : public Unit, public Siren::Primitive<GeneralUnit>
{
public:
    GeneralUnit();

    template<int M, int L, int T, int C, int t, int Q, int A>
    GeneralUnit(const PhysUnit<M,L,T,C,t,Q,A> &unit);
    
    GeneralUnit(int M, int L, int T, int C, int t, int Q, int A);
    
    GeneralUnit(const GeneralUnit &other);
    
    ~GeneralUnit();
    
    QString toString() const;

    uint hashCode() const;
    bool test(Siren::Logger &logger) const;
    void stream(Siren::Stream &s);

    int MASS() const;
    int LENGTH() const;
    int TIME() const;
    int CHARGE() const;
    int TEMPERATURE() const;
    int QUANTITY() const;
    int ANGLE() const;

    GeneralUnit& operator=(const GeneralUnit &other);
    
    bool operator==(const GeneralUnit &other) const;
    bool operator!=(const GeneralUnit &other) const;
    
    GeneralUnit operator-() const;
    
    GeneralUnit& operator+=(const GeneralUnit &other);
    
    GeneralUnit& operator-=(const GeneralUnit &other);
    
    GeneralUnit operator+(const GeneralUnit &other) const;
    
    GeneralUnit operator-(const GeneralUnit &other) const;

    GeneralUnit operator*=(const GeneralUnit &other);

    GeneralUnit operator/=(const GeneralUnit &other);
    
    GeneralUnit operator*(const GeneralUnit &other) const;
    
    GeneralUnit operator/(const GeneralUnit &other) const;
    
    GeneralUnit& operator*=(double val);
    GeneralUnit& operator/=(double val);
    
    GeneralUnit& operator*=(int val);
    GeneralUnit& operator/=(int val);
    
    GeneralUnit operator*(double val) const;
    GeneralUnit operator/(double val) const;
    
    GeneralUnit operator*(int val) const;
    GeneralUnit operator/(int val) const;
    
    GeneralUnit invert() const;
    
    double to(const detail::Temperature &other) const;
    double to(const GeneralUnit &other) const;

    template<int M, int L, int T, int C, int t, int Q, int A>
    operator PhysUnit<M,L,T,C,t,Q,A>() const;

private:
    void assertCompatible(const GeneralUnit &other) const;

    qint8 checkExponent(const char *type, int value) const;
    qint8 Mass, Length, Time, Charge, temperature, Quantity, Angle;
};

GeneralUnit operator*(double val, const GeneralUnit &unit);
GeneralUnit operator*(int val, const GeneralUnit &unit);
GeneralUnit operator/(double val, const GeneralUnit &unit);
GeneralUnit operator/(int val, const GeneralUnit &unit);

template<int M, int L, int T, int C, int t, int Q, int A>
bool operator==(const PhysUnit<M,L,T,C,t,Q,A> &unit, const GeneralUnit &genunit);

template<int M, int L, int T, int C, int t, int Q, int A>
bool operator!=(const PhysUnit<M,L,T,C,t,Q,A> &unit, const GeneralUnit &genunit);

#ifndef SIRE_SKIP_INLINE_FUNCTIONS

template<int M, int L, int T, int C, int t, int Q, int A>
SIRE_OUTOFLINE_TEMPLATE
PhysUnit<M,L,T,C,t,Q,A>::PhysUnit(const GeneralUnit &unit)
                        : Unit(unit), Siren::Primitive< PhysUnit<M,L,T,C,t,Q,A> >()
{
    //must be of the right type!
    if ( unit.MASS() != M or
         unit.LENGTH() != L or
         unit.TIME() != T or
         unit.CHARGE() != C or
         unit.TEMPERATURE() != t or
         unit.QUANTITY() != Q or
         unit.ANGLE() != A )
    {
        detail::throwIncompatible(unit, M, L, T, C, t, Q, A);
    }
}

template<int M, int L, int T, int C, int t, int Q, int A>
SIRE_OUTOFLINE_TEMPLATE
GeneralUnit::GeneralUnit(const PhysUnit<M,L,T,C,t,Q,A> &unit) 
            : Unit(unit), Siren::Primitive<GeneralUnit>()
{
    Mass = checkExponent("mass", M);
    Length = checkExponent("length", L);
    Time = checkExponent("time", T);
    Charge = checkExponent("charge", C);
    temperature = checkExponent("temperature", t);
    Quantity = checkExponent("quantity", Q);
    Angle = checkExponent("angle", A);
}

template<int M, int L, int T, int C, int t, int Q, int A>
SIRE_OUTOFLINE_TEMPLATE
GeneralUnit::operator PhysUnit<M,L,T,C,t,Q,A>() const
{
    return PhysUnit<M,L,T,C,t,Q,A>( this->value() );
}

template<int M, int L, int T, int C, int t, int Q, int A>
SIRE_OUTOFLINE_TEMPLATE
bool operator==(const PhysUnit<M,L,T,C,t,Q,A> &unit, const GeneralUnit &genunit)
{
    return GeneralUnit(unit) == genunit;
}

template<int M, int L, int T, int C, int t, int Q, int A>
SIRE_OUTOFLINE_TEMPLATE
bool operator!=(const PhysUnit<M,L,T,C,t,Q,A> &unit, const GeneralUnit &genunit)
{
    return GeneralUnit(unit) != genunit;
}

inline GeneralUnit operator*(double val, const GeneralUnit &unit)
{
    return unit * val;
}

inline GeneralUnit operator*(int val, const GeneralUnit &unit)
{
    return unit * val;
}

inline GeneralUnit operator/(double val, const GeneralUnit &unit)
{
    return unit.invert() * val;
}

inline GeneralUnit operator/(int val, const GeneralUnit &unit)
{
    return unit.invert() * val;
}

#endif //SIRE_SKIP_INLINE_FUNCTIONS

} // end of namespace Dimension

} // end of namespace SireUnits

Q_DECLARE_METATYPE( SireUnits::Dimension::GeneralUnit )

SIRE_EXPOSE_PRIMITIVE( SireUnits::Dimension::GeneralUnit )

SIRE_END_HEADER

#endif
