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

#include "rational.h"
#include "maths.h"

#include "Siren/tostring.h"

#include "SireMaths/errors.h"

using namespace SireMaths;
using namespace Siren;

/** Private function used by 'pow' to calculate 'x' raised to the positive
    fractional power 'power' */
static double pow_pvt(double x, const Rational &power)
{
    if ( x > 0.0 or SireMaths::isEven(power.numerator()) or 
         SireMaths::isOdd(power.denominator()) )
    {
        switch(power.denominator())
        {
            case 2:
                return std::sqrt( SireMaths::pow(x, power.numerator()) );
            default:
                return std::exp( std::log( SireMaths::pow(x, power.numerator()) ) 
                                              / power.denominator() );
        }
    }
    else 
        throw SireMaths::domain_error(
            QObject::tr("Cannot raise the negative number '%1' to a fractional "
                        "power (%2)").arg(x).arg(toString(power)), CODELOC);
}

namespace SireMaths
{

/** Return x raised to the fractional power 'power' */
double SIREMATHS_EXPORT pow(double x, const Rational &power)
{
    if (power.denominator() == 1)
        return SireMaths::pow(x, power.numerator());
    else if (power > 0)
        return pow_pvt(x,power);
    else
        return double(1.0) / ::pow_pvt(x, -power);
}

} // end of namespace SireMaths

static const RegisterPrimitive<Rational> r_rational;

Rational::Rational() : Primitive<Rational>()
{}

Rational::Rational(int n) : Primitive<Rational>(), r(n)
{}

Rational::Rational(int n, int d) : Primitive<Rational>(), r(n,d)
{}

Rational::Rational(double value) : Primitive<Rational>()
{
    this->operator=( Rational::toRational(value) );
}

Rational::Rational(const Rational &other) : Primitive<Rational>(), r(other.r)
{}

Rational::~Rational()
{}

Rational& Rational::operator=(const Rational &other)
{
    r = other.r;
    return *this;
}

bool Rational::operator==(const Rational &other) const
{
    return r == other.r;
}

bool Rational::operator!=(const Rational &other) const
{
    return r != other.r;
}

QString Rational::toString() const
{
    return QString("%1 / %2").arg( numerator() ).arg( denominator() );
}

int Rational::numerator() const
{
    return r.numerator();
}

int Rational::denominator() const
{
    return r.denominator();
}

Rational& Rational::operator+=(const Rational &other)
{
    r += other.r;
    return *this;
}

Rational& Rational::operator-=(const Rational &other)
{
    r -= other.r;
    return *this;
}

Rational& Rational::operator*=(const Rational &other)
{
    r *= other.r;
    return *this;
}

Rational& Rational::operator/=(const Rational &other)
{
    r /= other.r;
    return *this;
}

Rational& Rational::operator+=(int i)
{
    r += i;
    return *this;
}

Rational& Rational::operator-=(int i)
{
    r -= i;
    return *this;
}

Rational& Rational::operator*=(int i)
{
    r *= i;
    return *this;
}

Rational& Rational::operator/=(int i)
{
    r /= i;
    return *this;
}

const Rational& Rational::operator++()
{
    r++;
    return *this;
}

const Rational& Rational::operator--()
{
    r--;
    return *this;
}

bool Rational::operator!() const
{
    return !r;
}

bool Rational::operator<(const Rational &other) const
{
    return r < other.r;
}

bool Rational::operator<(int i) const
{
    return r < i;
}

bool Rational::operator>(int i) const
{
    return r > i;
}

bool Rational::operator==(int i) const
{
    return r == i;
}

void Rational::stream(Stream &s)
{
    s.assertVersion<Rational>(1);
    
    Schema schema = s.item<Rational>();
    
    if (s.isSaving())
    {
        schema.data("numerator") << r.numerator();
        schema.data("denominator") << r.denominator();
    }
    else 
    {
        qint32 n, d;
        
        schema.data("numerator") >> n;
        schema.data("denominator") >> d;
        
        r.assign(n,d);
    }
}

uint Rational::hashCode() const
{
    return (r.numerator()<<16) | (r.denominator() & 0x0000FFFF);
}

/** Return the real number version of this rational */
Rational::operator double() const
{
    return double(r.numerator()) / double(r.denominator());
}

/** Return whether this is a rational number (with maximum denominator = 'maxdenom') */
bool Rational::isRational(double val, int maxdenom)
{
    for (int i=1; i<=maxdenom; ++i)
    {
        int ival = int( val * double(i) );
        
        double error = std::abs( val - (double(ival)/double(i)) );
        
        if (error < std::numeric_limits<double>::epsilon())
            return true;
    }
    
    return false;
}

/** Return 'val' converted into the best approximated rational number
    with maximum denominator 'maxdenom'. A perfect conversion will only 
    result if 'isRational(val,maxdenom)' returned true. */
Rational Rational::toRational(double val, int maxdenom)
{
    Rational best_rational;
    double lowest_error = 0.0;

    //note, would be better if only tested primes...
    for (int i=1; i<=maxdenom; ++i)
    {
        int ival = int( val * double(i) );
        
        double error = std::abs( val - (double(ival)/double(i)) );
        
        if (error < std::numeric_limits<double>::epsilon())
            return Rational(ival, i);
        else if (i == 1 or error < lowest_error)
        {
            lowest_error = error;
            best_rational = Rational(ival, i);
        }
    }
    
    return best_rational;
}

bool Rational::isRational(double val)
{
    return isRational(val,500);
}

Rational Rational::toRational(double val)
{
    return toRational(val,500);
}
