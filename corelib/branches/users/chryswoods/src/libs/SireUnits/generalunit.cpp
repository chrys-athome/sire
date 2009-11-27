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

#include <QStringList>
#include <QMutex>
#include <QHash>

#include "generalunit.h"
#include "dimensions.h"
#include "temperature.h"

#include "Siren/logger.h"
#include "Siren/tester.h"

#include "Siren/errors.h"

using namespace SireUnits;
using namespace SireUnits::Dimension;

using namespace Siren;

static const RegisterPrimitive<GeneralUnit> r_genunit;

GeneralUnit::GeneralUnit() : Unit(1), Primitive<GeneralUnit>()
{
    Mass = 0;
    Length = 0;
    Time = 0;
    Charge = 0;
    temperature = 0;
    Quantity = 0;
    Angle = 0;
}

GeneralUnit::GeneralUnit(int M, int L, int T, int C, int t, int Q, int A)
            : Unit(1), Primitive<GeneralUnit>()
{
    Mass = M;
    Length = L;
    Time = T;
    Charge = C;
    temperature = t;
    Quantity = Q;
    Angle = A;
}

GeneralUnit::GeneralUnit(const GeneralUnit &other) 
            : Unit(other), Primitive<GeneralUnit>()
{
    Mass = other.Mass;
    Length = other.Length;
    Time = other.Time;
    Charge = other.Charge;
    temperature = other.temperature;
    Quantity = other.Quantity;
    Angle = other.Angle;
}

GeneralUnit::~GeneralUnit()
{}

void GeneralUnit::assertCompatible(const GeneralUnit &other) const
{
    if (Mass != other.Mass or
        Length != other.Length or
        Time != other.Time or
        Charge != other.Charge or
        temperature != other.temperature or
        Quantity != other.Quantity or
        Angle != other.Angle)
    {
        throw Siren::incompatible_error( QObject::tr(
                "The unit %1 is incompatible with %2.")
                    .arg(this->toString(), other.toString()), CODELOC );
    }
}

qint8 GeneralUnit::checkExponent(const char *type, int exp) const
{
    if (exp > 125 or exp < -125)
        throw Siren::unsupported( QObject::tr(
            "General units which involve exponents of %1 equal to %2 "
            "are not supported. The range of supported exponents is "
            "-125 to 125 inclusive.")
                .arg(QObject::tr(type)).arg(exp), CODELOC );

    return qint8(exp);
}
          
QString GeneralUnit::toString() const
{
    return SireUnits::Dimension::getUnitString(value(), Mass, Length,
                                               Time, Charge,
                                               temperature, Quantity,
                                               Angle);
}

void GeneralUnit::stream(Stream &s)
{
    s.assertVersion<GeneralUnit>(1);
    
    Schema schema = s.item<GeneralUnit>();
    
    schema.data("mass") & Mass;
    schema.data("length") & Length;
    schema.data("time") & Time;
    schema.data("charge") & Charge;
    schema.data("temperature") & temperature;
    schema.data("quantity") & Quantity;
    schema.data("angle") & Angle;
    
    double v = this->value();
    
    schema.data("value") & v;
    
    if (s.isLoading())
        this->setScale(v);
}

uint GeneralUnit::hashCode() const
{
    double v = value();

    return Mass + Length + Time + Charge + temperature + Quantity + Angle 
             + *((uint*)(&v));
}

bool GeneralUnit::test(Logger &logger) const
{
    Tester tester(*this, logger);
    
    try
    {
        GeneralUnit unit;
    
        SireUnits::Dimension::Mass mass;
        
        unit = mass;
        
        tester.expect_equal( QObject::tr("Equals a Mass"), CODELOC,
                             unit.MASS(), 1 );
                             
        tester.expect_true( QObject::tr("Really equals a Mass"), CODELOC,
                            mass == unit);
        
        tester.expect_equal( QObject::tr("Equals a Mass^2"), CODELOC,
                             (unit*unit).MASS(), 2 );
                             
        tester.expect_true( QObject::tr("Really equals a Mass^2"), CODELOC,
                            (mass*mass) == (unit*unit) );
    }
    catch(const Siren::exception &e)
    {
        tester.unexpected_error(e);
    }
    
    return tester.allPassed();
}

double GeneralUnit::to(const GeneralUnit &units) const
{
    assertCompatible(units);
    return units.convertFromInternal(value());
}

double GeneralUnit::to(const detail::Temperature &other) const
{
    //this must be a temperature!
    GeneralUnit general_temp;
    general_temp.temperature = 1;
    general_temp.setScale(other);
    
    this->assertCompatible(general_temp);
    
    return other.convertFromInternal(value()) / other.convertFromInternal();
}

int GeneralUnit::MASS() const
{
    return Mass;
}

int GeneralUnit::LENGTH() const
{
    return Length;
}

int GeneralUnit::TIME() const
{
    return Time;
}

int GeneralUnit::CHARGE() const
{
    return Charge;
}

int GeneralUnit::TEMPERATURE() const
{
    return temperature;
}

int GeneralUnit::QUANTITY() const
{
    return Quantity;
}

int GeneralUnit::ANGLE() const
{
    return Angle;
}

GeneralUnit& GeneralUnit::operator=(const GeneralUnit &other)
{
    setScale(other.value());
    
    Mass = other.MASS();
    Length = other.LENGTH();
    Time = other.TIME();
    Charge = other.CHARGE();
    temperature = other.TEMPERATURE();
    Quantity = other.QUANTITY();
    Angle = other.ANGLE();    

    return *this;
}

bool GeneralUnit::operator==(const GeneralUnit &other) const
{
    assertCompatible(other);
    return value() == other.value();
}

bool GeneralUnit::operator!=(const GeneralUnit &other) const
{
    assertCompatible(other);
    return value() != other.value();
}

GeneralUnit GeneralUnit::operator-() const
{
    GeneralUnit ret = *this;
    ret.setScale( -value() );
    return ret;
}

GeneralUnit& GeneralUnit::operator+=(const GeneralUnit &other)
{
    assertCompatible(other);
    setScale(value() + other.value());
    return *this;
}

GeneralUnit& GeneralUnit::operator-=(const GeneralUnit &other)
{
    assertCompatible(other);
    setScale(value() - other.value());
    return *this;
}

GeneralUnit GeneralUnit::operator+(const GeneralUnit &other) const
{
    GeneralUnit ret = *this;
    ret += other;
    return ret;
}

GeneralUnit GeneralUnit::operator-(const GeneralUnit &other) const
{
    GeneralUnit ret = *this;
    ret -= other;
    return ret;
}

GeneralUnit GeneralUnit::operator*=(const GeneralUnit &other)
{
     setScale(value() * other.value());
     Mass += other.Mass;
     Length += other.Length;
     Time += other.Time;
     Charge += other.Charge;
     temperature += other.temperature;
     Quantity += other.Quantity;
     Angle += other.Angle;
     
     return *this;
}

GeneralUnit GeneralUnit::operator/=(const GeneralUnit &other)
{
    setScale(value() / other.value());
    Mass -= other.Mass;
    Length -= other.Length;
    Time -= other.Time;
    Charge -= other.Charge;
    temperature -= other.temperature;
    Quantity -= other.Quantity;
    Angle -= other.Angle;
    
    return *this;
}

GeneralUnit GeneralUnit::operator*(const GeneralUnit &other) const
{
    GeneralUnit ret = *this;
    ret *= other;
    return ret;
}

GeneralUnit GeneralUnit::operator/(const GeneralUnit &other) const
{
    GeneralUnit ret = *this;
    ret /= other;
    return ret;
}

GeneralUnit& GeneralUnit::operator*=(double val)
{
    setScale(value() * val);
    return *this;
}

GeneralUnit& GeneralUnit::operator/=(double val)
{
    setScale(value() / val);
    return *this;
}

GeneralUnit& GeneralUnit::operator*=(int val)
{
    setScale(value() * val);
    return *this;
}

GeneralUnit& GeneralUnit::operator/=(int val)
{
    setScale(value() / val);
    return *this;
}

GeneralUnit GeneralUnit::operator*(double val) const
{
    GeneralUnit ret = *this;
    ret *= val;
    return ret;
}

GeneralUnit GeneralUnit::operator/(double val) const
{
    GeneralUnit ret = *this;
    ret /= val;
    return ret;
}

GeneralUnit GeneralUnit::operator*(int val) const
{
    GeneralUnit ret = *this;
    ret *= val;
    return ret;
}

GeneralUnit GeneralUnit::operator/(int val) const
{
    GeneralUnit ret = *this;
    ret /= val;
    return ret;
}

GeneralUnit GeneralUnit::invert() const
{
    GeneralUnit ret;
    
    ret.setScale( 1.0 / value() );
    
    ret.Mass = -Mass;
    ret.Length = -Length;
    ret.Time = -Time;
    ret.Charge = -Charge;
    ret.temperature = -temperature;
    ret.Quantity = -Quantity;
    ret.Angle = -Angle;
    
    return ret;
}
