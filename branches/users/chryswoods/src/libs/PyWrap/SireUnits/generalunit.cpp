
#include "generalunit.h"

using namespace SireUnits;
using namespace SireUnits::Dimension;

GeneralUnit::GeneralUnit()
{
    value = 0;
    Mass = 0;
    Length = 0;
    Time = 0;
    Charge = 0;
    temperature = 0;
    Quantity = 0;
}

GeneralUnit::GeneralUnit(const GeneralUnit &other)
{
    value = other.value;
    Mass = other.Mass;
    Length = other.Length;
    Time = other.Time;
    Charge = other.Charge;
    temperature = other.temperature;
    Quantity = other.Quantity;
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
        Quantity != other.Quantity)
    {
        throw "Unit conversion error!!!";
    }
}

int GeneralUnit::M() const
{
    return Mass;
}

int GeneralUnit::L() const
{
    return Length;
}

int GeneralUnit::T() const
{
    return Time;
}

int GeneralUnit::C() const
{
    return Charge;
}

int GeneralUnit::t() const
{
    return temperature;
}

int GeneralUnit::Q() const
{
    return Quantity;
}

GeneralUnit& GeneralUnit::operator=(const GeneralUnit &other)
{
    value = other.value;
    Mass = other.Mass;
    Length = other.Length;
    Time = other.Time;
    Charge = other.Charge;
    temperature = other.temperature;
    Quantity = other.Quantity;
}

bool GeneralUnit::operator==(const GeneralUnit &other) const
{
    assertCompatible(other);
    return value == other.value;
}

bool GeneralUnit::operator!=(const GeneralUnit &other) const
{
    assertCompatible(other);
    return value != other.value;
}

GeneralUnit GeneralUnit::operator-() const
{
    GeneralUnit ret = *this;
    ret.value *= -1
    return ret;
}

GeneralUnit& GeneralUnit::operator+=(const GeneralUnit &other)
{
    assertCompatible(other);
    value += other.value;
    return *this;
}

GeneralUnit& GeneralUnit::operator-=(const GeneralUnit &other)
{
    assertCompatible(other);
    value -= other.value;
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
     value *= other.value;
     Mass += other.Mass;
     Length += other.Length;
     Time += other.Time;
     Charge += other.Charge;
     temperature += other.temperature;
     Quantity += other.Quantity;
     
     return *this;
}

GeneralUnit GeneralUnit::operator/=(const GeneralUnit &other)
{
    value /= other.value;
    Mass -= other.Mass;
    Length -= other.Length;
    Time -= other.Time;
    Charge -= other.Charge;
    temperature -= other.temperature;
    Quantity -= other.Quantity;
    
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
    value *= val;
    return *this;
}

GeneralUnit& GeneralUnit::operator/=(double val)
{
    value /= val;
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

GeneralUnit::operator double() const
{
    return value;
}
