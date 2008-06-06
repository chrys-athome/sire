
#include <Python.h>
#include <boost/python.hpp>

#include <QStringList>

#include "generalunit.h"

#include "SireUnits/dimensions.h"
#include "SireUnits/temperature.h"

using namespace SireUnits;
using namespace SireUnits::Dimension;

GeneralUnit::GeneralUnit() : Unit(0)
{
    Mass = 0;
    Length = 0;
    Time = 0;
    Charge = 0;
    temperature = 0;
    Quantity = 0;
    Angle = 0;
}

GeneralUnit::GeneralUnit(const GeneralUnit &other) : Unit(other)
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
        throw "Unit conversion error!!!";
    }
}
    
static void appendString(int M, QString rep, QStringList &pos, QStringList &neg)
{
    if (M > 1)
    {
        pos.append( QString("%1^%2").arg(rep).arg(M) );
    }
    else if (M == 1)
    {
        pos.append(rep);
    }
    else if (M < 0)
    {
        neg.append( QString("%1%2").arg(rep).arg(M) );
    }
} 
    
QString GeneralUnit::toString() const
{
    QStringList pos;
    QStringList neg;
    
    appendString(Mass, "M", pos, neg);
    appendString(Length, "L", pos, neg);
    appendString(Time, "T", pos, neg);
    appendString(Charge, "C", pos, neg);
    appendString(temperature, "t", pos, neg);
    appendString(Quantity, "Q", pos, neg);
    appendString(Angle, "A", pos, neg);
    
    if (pos.isEmpty() and neg.isEmpty())
        return QString::number(value());
    else if (neg.isEmpty())
        return QString("%1 %2").arg(value()).arg(pos.join(" "));
    else if (pos.isEmpty())
        return QString("%1 %2").arg(value()).arg(neg.join(" "));
    else
        return QString("%1 %2 %3").arg(value())
                                  .arg(pos.join(" "), neg.join(" "));
}

double GeneralUnit::to(const GeneralUnit &units) const
{
    assertCompatible(units);
    return units.convertFromInternal(value());
}

double GeneralUnit::to(const TempBase &other) const
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
