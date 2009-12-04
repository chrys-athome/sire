/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
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

#ifndef SIREMATHS_RATIONAL_H
#define SIREMATHS_RATIONAL_H

#include <boost/rational.hpp>

#include <QString>

#include "Siren/primitive.h"

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireMaths
{

/** This class represents a rational number */
class SIREMATHS_EXPORT Rational : public Siren::Primitive<Rational>
{
public:
    Rational();

    Rational(int n);
    Rational(int n, int d);

    Rational(double value);

    Rational(const Rational &other);
    
    ~Rational();
    
    Rational& operator=(const Rational &other);
    
    bool operator==(const Rational &other) const;
    bool operator!=(const Rational &other) const;

    int numerator() const;
    int denominator() const;

    void stream(Siren::Stream &s);
    uint hashCode() const;
    QString toString() const;

    Rational& operator+=(const Rational &other);
    Rational& operator-=(const Rational &other);
    Rational& operator*=(const Rational &other);
    Rational& operator/=(const Rational &other);
    
    Rational& operator+=(int i);
    Rational& operator-=(int i);
    Rational& operator*=(int i);
    Rational& operator/=(int i);

    const Rational& operator++();
    const Rational& operator--();

    bool operator!() const;

    bool operator<(const Rational &other) const;

    bool operator<(int i) const;
    bool operator>(int i) const;
    bool operator==(int i) const;    

    operator double() const;

    static Rational toRational(double val, int maxdenom);
    static Rational toRational(double val);
    
    static bool isRational(double val, int maxdenom);
    static bool isRational(double val);

private:
    boost::rational<qint32> r;
};

qint32 gcd(qint32 n, qint32 m);
qint32 lcm(qint32 n, qint32 m);

qint64 gcd(qint64 n, qint64 m);
qint64 lcm(qint64 n, qint64 m);

double pow(double value, const Rational &power);

}

Q_DECLARE_METATYPE(SireMaths::Rational)

SIRE_EXPOSE_PRIMITIVE( SireMaths::Rational )

SIRE_END_HEADER

#endif
