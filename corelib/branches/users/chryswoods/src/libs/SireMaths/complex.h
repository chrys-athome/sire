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

#ifndef SIRECAS_COMPLEX_H
#define SIRECAS_COMPLEX_H

#include <QString>

#include <complex>

#include "SireMaths/maths.h"

#include "Siren/primitive.h"

SIRE_BEGIN_HEADER

namespace SireMaths
{

class Rational;

/** This class represents a complex number to the same precision as 'double'. 

    @author Christopher Woods
*/
class SIREMATHS_EXPORT Complex : public Siren::Primitive<Complex>
{
public:
    Complex(double r=0.0, double i=0.0);

    template<typename T>
    Complex(const std::complex<T> &stdcomplex);

    Complex(const Complex &other);

    ~Complex();

    Complex& operator=(const Complex &other);

    template<typename T>
    Complex& operator=(const std::complex<T> &stdcomplex);
    
    bool operator==(const Complex &other) const;
    bool operator!=(const Complex &other) const;

    template<typename T>
    bool operator==(const std::complex<T> &stdcomplex) const;

    template<typename T>
    bool operator!=(const std::complex<T> &stdcomplex) const;

    Complex& operator+=(const Complex &other);
    Complex& operator-=(const Complex &other);
    Complex& operator*=(const Complex &other);
    Complex& operator/=(const Complex &other);

    Complex operator-() const;

    bool operator==(double r) const;
    bool operator!=(double r) const;

    Complex& operator=(double r);

    Complex& operator+=(double r);
    Complex& operator-=(double r);
    Complex& operator*=(double r);
    Complex& operator/=(double r);

    QString toString() const;
    uint hashCode() const;
    void stream(Siren::Stream &s);

    double real() const;
    double imag() const;
    
    template<typename T>
    operator std::complex<T>() const;

    bool isReal() const;
    bool isPurelyComplex() const;
    
    bool isZero() const;

    static Complex rect(double x, double y);
    static Complex polar(double r, double theta);

    void setRectangular(double x, double y);
    void setPolar(double r, double theta);

    void setReal(double x);
    void setImag(double y);

    double arg() const;
    double abs() const;
    double abs2() const;

    double logAbs() const;

    Complex conjugate() const;

    Complex inverse() const;

    Complex negative() const;

    std::complex<double> toStdComplex() const;

private:
    std::complex<double> z;
};

Complex operator+(const Complex &z0, const Complex &z1);
Complex operator-(const Complex &z0, const Complex &z1);
Complex operator*(const Complex &z0, const Complex &z1);
Complex operator/(const Complex &z0, const Complex &z1);
Complex operator+(const Complex &z, double x);
Complex operator-(const Complex &z, double x);
Complex operator*(const Complex &z, double x);
Complex operator/(const Complex &z, double x);
Complex operator+(double x, const Complex &z);
Complex operator-(double x, const Complex &z);
Complex operator*(double x, const Complex &z);
Complex operator/(double x, const Complex &z);
Complex sqrt(const Complex &z);
Complex sqrt_real(double x);
Complex pow(const Complex &z, const Complex &a);
Complex pow(const Complex &z, double x);
Complex pow(const Complex &z, int n);
Complex pow(const Complex &z, const Rational &r);
Complex pow(double x, const Complex &z);
Complex exp(const Complex &z);
Complex log(const Complex &z);
Complex log10(const Complex &z);
Complex sin(const Complex &z);
Complex cos(const Complex &z);
Complex tan(const Complex &z);
Complex sec(const Complex &z);
Complex csc(const Complex &z);
Complex cot(const Complex &z);
Complex arcsin(const Complex &z);
Complex arcsin_real(double z);
Complex arccos(const Complex &z);
Complex arccos_real(double z);
Complex arctan(const Complex &z);
Complex arcsec(const Complex &z);
Complex arcsec_real(double z);
Complex arccsc(const Complex &z);
Complex arccsc_real(double z);
Complex arccot(const Complex &z);
Complex sinh(const Complex &z);
Complex cosh(const Complex &z);
Complex tanh(const Complex &z);
Complex sech(const Complex &z);
Complex csch(const Complex &z);
Complex coth(const Complex &z);
Complex arcsinh(const Complex &z);
Complex arccosh(const Complex &z);
Complex arccosh_real(double z);
Complex arctanh(const Complex &z);
Complex arctanh_real(double z);
Complex arcsech(const Complex &z);
Complex arccsch(const Complex &z);
Complex arccoth(const Complex &z);

#ifndef SIRE_SKIP_INLINE_FUNCTIONS

/** Construct from a std::complex */
template<typename T>
Complex::Complex(const std::complex<T> &stdcomplex)
        : Siren::Primitive<Complex>(), z(stdcomplex.real(), stdcomplex.imag())
{}

/** Assignment from a std::complex */
template<typename T>
Complex& Complex::operator=(const std::complex<T> &stdcomplex)
{
    setReal( stdcomplex.real() );
    setImag( stdcomplex.imag() );
    return *this;
}

/** Implicit conversion to a std::complex */
template<typename T>
Complex::operator std::complex<T>() const
{
    return std::complex<T>(real(),imag());
}

/** Comparison to std::complex */
template<typename T>
bool Complex::operator==(const std::complex<T> &stdcomplex) const
{
    return SireMaths::areEqual(real(), stdcomplex.real()) and
           SireMaths::areEqual(imag(), stdcomplex.imag());
}

/** Comparison to std::complex */
template<typename T>
bool Complex::operator!=(const std::complex<T> &stdcomplex) const
{
    return not operator==(stdcomplex);
}

/** Comparison with std::complex */
template<typename T>
SIRE_INLINE_TEMPLATE
bool operator==(const std::complex<T> &stdcomplex, const Complex &complex)
{
    return complex == stdcomplex;
}

/** Comparison with std::complex */
template<typename T>
SIRE_INLINE_TEMPLATE
bool operator!=(const std::complex<T> &stdcomplex, const Complex &complex)
{
    return complex != stdcomplex;
}

/** This is the std sqrt function. This helps resolve namespace issues... */
inline double sqrt(double x)
{
    return std::sqrt(x);
}

#endif //SIRE_SKIP_INLINE_FUNCTIONS

}

Q_DECLARE_METATYPE(SireMaths::Complex)
Q_DECLARE_TYPEINFO(SireMaths::Complex, Q_MOVABLE_TYPE);

SIRE_EXPOSE_PRIMITIVE( SireMaths::Complex )

SIRE_END_HEADER

#endif
