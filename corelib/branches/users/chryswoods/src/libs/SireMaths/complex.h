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

/**
The documentation for a lot of these functions is copied directly from the 
documentation of the gsl functions of the equivalent name. 

The copyright information for GSL is;

########################################################################
GSL

More information about GSL can be found at the project homepage, 
http://www.gnu.org/software/gsl/.

Printed copies of this manual can be purchased from Network Theory Ltd at
http://www.network-theory.co.uk/gsl/manual/. The money raised from sales of the 
manual helps support the development of GSL.

Copyright  1996, 1997, 1998, 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006 The GSL Team.

Permission is granted to copy, distribute and/or modify this document 
under the terms of the GNU Free Documentation License, Version 1.2 or any 
later version published by the Free Software Foundation; with the Invariant 
Sections being "GNU General Public License" and "Free Software Needs Free Documentation", 
the Front-Cover text being "A GNU Manual", and with the Back-Cover Text 
being (a) (see below). A copy of the license is included in the section 
entitled ?GNU Free Documentation License?.

(a) The Back-Cover Text is: "You have freedom to copy and modify this GNU Manual, 
like GNU software."

GSL Complex

The functions described in this chapter provide support for complex numbers. 
The algorithms take care to avoid unnecessary intermediate underflows and overflows, 
allowing the functions to be evaluated over as much of the complex plane as possible.

For multiple-valued functions the branch cuts have been chosen to follow the 
conventions of Abramowitz and Stegun in the Handbook of Mathematical Functions. 
The functions return principal values which are the same as those in GNU Calc, 
which in turn are the same as those in Common Lisp, The Language (Second Edition)1 
and the HP-28/48 series of calculators.

The complex types are defined in the header file gsl_complex.h, while the 
corresponding complex functions and arithmetic operations are defined 
in gsl_complex_math.h.

#######################################################################
*/

#include <gsl/gsl_complex.h>
#include <gsl/gsl_complex_math.h>

#include <QString>

#include <complex>

#include "SireMaths/maths.h"
#include "SireMaths/rational.h"

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireMaths
{
class Complex;
}

class QDataStream;
QDataStream& operator<<(QDataStream&, const SireMaths::Complex&);
QDataStream& operator>>(QDataStream&, SireMaths::Complex&);

namespace SireMaths
{

/**
This class represents a complex number to the same precision as 'double'. 
This is merely a thin wrapper around the gsl_complex struct, and the 
gsl_complex functions.

(indeed, this is publically derived from gsl_complex, so you can use 
this class whereever you would normally use a gsl_complex)

@author Christopher Woods
*/
class SIREMATHS_EXPORT Complex : public gsl_complex
{
public:

    /** Construct the complex number  real + imag i */
    Complex(double r=0.0, double i=0.0)
            : gsl_complex( gsl_complex_rect(r,i) )
    {}

    /** Construct from a gsl_complex struct */
    Complex(const gsl_complex &complex) : gsl_complex(complex)
    {}

    /** Construct from a std::complex */
    template<typename T>
    Complex(const std::complex<T> &stdcomplex)
            : gsl_complex( gsl_complex_rect(stdcomplex.real(),stdcomplex.imag()) )
    {}

    /** Copy constructor */
    Complex(const Complex &other) : gsl_complex(other)
    {}

    /** Destructor */
    ~Complex()
    {}

    static const char* typeName();
    
    const char* what() const
    {
        return Complex::typeName();
    }

    /** Return the real part of this number */
    double real() const
    {
        return GSL_REAL(*this);
    }

    /** Return the imaginary part of this number */
    double imag() const
    {
        return GSL_IMAG(*this);
    }

    /** Implicit conversion to a std::complex */
    template<typename T>
    operator std::complex<T>() const
    {
        return std::complex<T>(real(),imag());
    }

    /** Is this a real number (imag == 0) ? */
    bool isReal() const
    {
        return SireMaths::isZero(imag());
    }

    /** Is this zero? */
    bool isZero() const
    {
        return isReal() and SireMaths::isZero(real());
    }

    /** Return a string representation of this Complex number */
    QString toString() const
    {
        if (isReal())
            return QString::number(real());
        else
        {
            double i = imag();
            double r = real();

            if (SireMaths::isZero(r))
            {
                if (SireMaths::areEqual(i,1.0))
                    return "i";
                else if (SireMaths::areEqual(i,-1.0))
                    return "-i";
                else
                    return QString("%1i").arg(i);
            }
            else if (i > 0)
                return QString("%1 + %2i").arg(r).arg(i);
            else
                return QString("%1 - %2i").arg(r).arg(-i);
        }
    }

    /** This function uses the rectangular cartesian components (x,y) to
        return the complex number z = x + i y. */
    static Complex rect(double x, double y)
    {
        return Complex( gsl_complex_rect(x,y) );
    }

    /** This function returns the complex number
        z = r \exp(i \theta) = r (\cos(\theta) + i \sin(\theta))
        from the polar representation (r,theta). */
    static Complex polar(double r, double theta)
    {
       return Complex( gsl_complex_polar(r,theta) );
    }

    /** This function sets the rectangular cartesian components (x,y) to
        the complex number z = x + i y. */
    void setRectangular(double x, double y)
    {
        GSL_SET_COMPLEX(this, x, y);
    }

    /** This function sets the complex number to
        z = r \exp(i \theta) = r (\cos(\theta) + i \sin(\theta))
        from the polar representation (r,theta). */
    void setPolar(double r, double theta)
    {
        *this = gsl_complex_polar(r,theta);
    }

    /** This function sets the real part of the complex number */
    void setReal(double x)
    {
        GSL_SET_REAL(this,x);
    }

    /** This function set the imaginary part of the complex number */
    void setImag(double y)
    {
        GSL_SET_IMAG(this,y);
    }

    /** Comparison operator */
    bool operator==(const Complex &other) const
    {
        return real() == other.real() and imag() == other.imag();
    }

    /** Comparison operator */
    bool operator!=(const Complex &other) const
    {
        return not operator==(other);
    }

    /** Comparison to std::complex */
    template<typename T>
    bool operator==(const std::complex<T> &stdcomplex) const
    {
        return SireMaths::areEqual(real(), stdcomplex.real()) and
               SireMaths::areEqual(imag(), stdcomplex.imag());
    }

    /** Comparison to std::complex */
    template<typename T>
    bool operator!=(const std::complex<T> &stdcomplex) const
    {
        return not operator==(stdcomplex);
    }

    /** Assignment operator */
    Complex& operator=(const Complex &other)
    {
        gsl_complex::operator=(other);
        return *this;
    }

    /** Assignment from a std::complex */
    template<typename T>
    Complex& operator=(const std::complex<T> &stdcomplex)
    {
        setReal( stdcomplex.real() );
        setImag( stdcomplex.imag() );
        return *this;
    }

    /** self-addition */
    Complex& operator+=(const Complex &other)
    {
        *this = gsl_complex_add(*this,other);
        return *this;
    }

    /** self-subtraction */
    Complex& operator-=(const Complex &other)
    {
        *this = gsl_complex_sub(*this,other);
        return *this;
    }

    /** self-multiplication */
    Complex& operator*=(const Complex &other)
    {
        *this = gsl_complex_mul(*this,other);
        return *this;
    }

    /** self-division */
    Complex& operator/=(const Complex &other)
    {
        *this = gsl_complex_div(*this,other);
        return *this;
    }

    /** This function returns the negative of the complex
        number z, -z = (-x) + i(-y). */
    Complex operator-() const
    {
        return gsl_complex_negative(*this);
    }

    /** Comparison operator */
    bool operator==(double r) const
    {
        return isReal() and SireMaths::areEqual(r, real());
    }

    /** Comparison operator */
    bool operator!=(double r) const
    {
        return not operator==(r);
    }

    /** Assignment operator */
    Complex& operator=(double r)
    {
        GSL_SET_REAL(this,r);
        return *this;
    }

    /** self-addition */
    Complex& operator+=(double r)
    {
        *this = gsl_complex_add_real(*this, r);
        return *this;
    }

    /** self-subtraction */
    Complex& operator-=(double r)
    {
        *this = gsl_complex_sub_real(*this, r);
        return *this;
    }

    /** self-multiplication */
    Complex& operator*=(double r)
    {
        *this = gsl_complex_mul_real(*this,r);
        return *this;
    }

    /** self-division */
    Complex& operator/=(double r)
    {
        *this = gsl_complex_div_real(*this,r);
        return *this;
    }

    /** This function returns the argument of the complex number z,
        \arg(z), where -\pi < \arg(z) <= \pi. */
    double arg() const
    {
        return gsl_complex_arg(*this);
    }

    /** This function returns the magnitude of the complex number z, |z|. */
    double abs() const
    {
        return gsl_complex_abs(*this);
    }

    /** This function returns the squared magnitude of the complex number z, |z|^2. */
    double abs2() const
    {
        return gsl_complex_abs2(*this);
    }

    /** This function returns the natural logarithm of the magnitude of the
        complex number z, \log|z|. It allows an accurate evaluation of \log|z|
        when |z| is close to one. The direct evaluation of log(gsl_complex_abs(z))
        would lead to a loss of precision in this case. */
    double logAbs() const
    {
        return gsl_complex_logabs(*this);
    }

    /** This function returns the complex conjugate of the complex
        number z, z^* = x - i y. */
    Complex conjugate() const
    {
        return gsl_complex_conjugate(*this);
    }

    /** This function returns the inverse, or reciprocal, of the
        complex number z, 1/z = (x - i y)/(x^2 + y^2). */
    Complex inverse() const
    {
        return gsl_complex_inverse(*this);
    }

    /** This function returns the negative of the complex
        number z, -z = (-x) + i(-y). */
    Complex negative() const
    {
        return gsl_complex_negative(*this);
    }
};

#ifndef SIRE_SKIP_INLINE_FUNCTIONS

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

/** Addition */
inline Complex operator+(const Complex &z0, const Complex &z1)
{
    return gsl_complex_add(z0,z1);
}

/** Subtraction */
inline Complex operator-(const Complex &z0, const Complex &z1)
{
    return gsl_complex_sub(z0,z1);
}

/** Multiplication */
inline Complex operator*(const Complex &z0, const Complex &z1)
{
    return gsl_complex_mul(z0,z1);
}

/** Division */
inline Complex operator/(const Complex &z0, const Complex &z1)
{
    return gsl_complex_div(z0,z1);
}

/** Addition */
inline Complex operator+(const Complex &z, double x)
{
    return gsl_complex_add_real(z,x);
}

/** Subtraction */
inline Complex operator-(const Complex &z, double x)
{
    return gsl_complex_sub_real(z,x);
}

/** Multiplication */
inline Complex operator*(const Complex &z, double x)
{
    return gsl_complex_mul_real(z,x);
}

/** Division */
inline Complex operator/(const Complex &z, double x)
{
    return gsl_complex_div_real(z,x);
}

/** Addition */
inline Complex operator+(double x, const Complex &z)
{
    return z + x;
}

/** Subtraction */
inline Complex operator-(double x, const Complex &z)
{
    return z - x;
}

/** Multiplication */
inline Complex operator*(double x, const Complex &z)
{
    return z * x;
}

/** Division */
inline Complex operator/(double x, const Complex &z)
{
    return Complex(x) / z;
}

/** This function returns the square root of the complex number z, \sqrt z. The branch cut is the negative real axis. The result always lies in the right half of the complex plane. */
inline Complex sqrt(const Complex &z)
{
    return gsl_complex_sqrt(z);
}

/** This function returns the complex square root of the real number x, where x may be negative. */
inline Complex sqrt_real(double x)
{
    return gsl_complex_sqrt_real(x);
}

/** This is the std sqrt function. This helps resolve namespace issues... */
inline double sqrt(double x)
{
    return std::sqrt(x);
}

/** The function returns the complex number z raised to the complex power a, z^a. This is computed as \exp(\log(z)*a) using complex logarithms and complex exponentials. */
inline Complex pow(const Complex &z, const Complex &a)
{
    return gsl_complex_pow(z,a);
}

/** This function returns the complex number z raised to the real power x, z^x. */
inline Complex pow(const Complex &z, double x)
{
    return gsl_complex_pow_real(z,x);
}

/** This function returns the complex number z raised to an integer power n, z^n */
inline Complex pow(const Complex &z, int n)
{
    if (n >= 0)
    {
        switch(n)
        {
            case 0:
                return Complex(1);
            case 1:
                return z;
            case 2:
                return z*z;
            default:
                return pow(z, double(n));
        }
    }
    else
        return Complex(1) / pow(z, -n);
}

/** This function returns the complex number z raised to a rational power r, z^r */
inline Complex pow(const Complex &z, const Rational &r)
{
    if (r.denominator() == 1)
        return pow(z, r.numerator());
    else
        return pow(z, SireMaths::toDouble(r));
}

/** This function returns the real number x raised to the complex power z, x^z. */
inline Complex pow(double x, const Complex &z)
{
    return gsl_complex_pow( Complex(x), z );
}

/** This function returns the complex exponential of the complex number z, \exp(z). */
inline Complex exp(const Complex &z)
{
    return gsl_complex_exp(z);
}

/** This function returns the complex natural logarithm (base e) of the complex number z, \log(z). The branch cut is the negative real axis. */
inline Complex log(const Complex &z)
{
    return gsl_complex_log(z);
}

/** This function returns the complex base-10 logarithm of the complex number z, \log_10 (z). */
inline Complex log10(const Complex &z)
{
    return gsl_complex_log10(z);
}

/** This function returns the complex base-b logarithm of the complex number z, \log_b(z). This quantity is computed as the ratio \log(z)/\log(b). */
inline Complex log_b(const Complex &z, const Complex &b)
{
    return gsl_complex_log_b(z,b);
}

/** This function returns the complex sine of the complex number z, \sin(z) = (\exp(iz) - \exp(-iz))/(2i). */
inline Complex sin(const Complex &z)
{
    if (z.isReal())
        return std::sin(z.real());
    else
        return gsl_complex_sin(z);
}

/** This function returns the complex cosine of the complex number z, \cos(z) = (\exp(iz) + \exp(-iz))/2. */
inline Complex cos(const Complex &z)
{
    if (z.isReal())
        return std::cos(z.real());
    else
        return gsl_complex_cos(z);
}

/** This function returns the complex tangent of the complex number z, \tan(z) = \sin(z)/\cos(z). */
inline Complex tan(const Complex &z)
{
    if (z.isReal())
        return std::tan(z.real());
    else
        return gsl_complex_tan(z);
}

/** This function returns the complex secant of the complex number z, \sec(z) = 1/\cos(z). */
inline Complex sec(const Complex &z)
{
    return gsl_complex_sec(z);
}

/** This function returns the complex cosecant of the complex number z, \csc(z) = 1/\sin(z). */
inline Complex csc(const Complex &z)
{
    return gsl_complex_csc(z);
}

/** This function returns the complex cotangent of the complex number z, \cot(z) = 1/\tan(z). */
inline Complex cot(const Complex &z)
{
    return gsl_complex_cot(z);
}

/** This function returns the complex arcsine of the complex number z, \arcsin(z). The branch cuts are on the real axis, less than -1 and greater than 1. */
inline Complex arcsin(const Complex &z)
{
    return gsl_complex_arcsin(z);
}

/** This function returns the complex arcsine of the real number z, \arcsin(z). For z between -1 and 1, the function returns a real value in the range [-\pi/2,\pi/2]. For z less than -1 the result has a real part of -\pi/2 and a positive imaginary part. For z greater than 1 the result has a real part of \pi/2 and a negative imaginary part. */
inline Complex arcsin_real(double z)
{
    return gsl_complex_arcsin_real(z);
}

/** This function returns the complex arccosine of the complex number z, \arccos(z). The branch cuts are on the real axis, less than -1 and greater than 1. */
inline Complex arccos(const Complex &z)
{
    return gsl_complex_arccos(z);
}

/** This function returns the complex arccosine of the real number z, \arccos(z). For z between -1 and 1, the function returns a real value in the range [0,\pi]. For z less than -1 the result has a real part of \pi and a negative imaginary part. For z greater than 1 the result is purely imaginary and positive. */
inline Complex arccos_real(double z)
{
    return gsl_complex_arccos_real(z);
}

/** This function returns the complex arctangent of the complex number z, \arctan(z). The branch cuts are on the imaginary axis, below -i and above i. */
inline Complex arctan(const Complex &z)
{
    return gsl_complex_arctan(z);
}

/** This function returns the complex arcsecant of the complex number z, \arcsec(z) = \arccos(1/z). */
inline Complex arcsec(const Complex &z)
{
    return gsl_complex_arcsec(z);
}

/** This function returns the complex arcsecant of the real number z, \arcsec(z) = \arccos(1/z). */
inline Complex arcsec_real(double z)
{
    return gsl_complex_arcsec_real(z);
}

/** This function returns the complex arccosecant of the complex number z, \arccsc(z) = \arcsin(1/z). */
inline Complex arccsc(const Complex &z)
{
    return gsl_complex_arccsc(z);
}

/** This function returns the complex arccosecant of the real number z, \arccsc(z) = \arcsin(1/z). */
inline Complex arccsc_real(double z)
{
    return gsl_complex_arccsc_real(z);
}

/** This function returns the complex arccotangent of the complex number z, \arccot(z) = \arctan(1/z). */
inline Complex arccot(const Complex &z)
{
    return gsl_complex_arccot(z);
}

/** This function returns the complex hyperbolic sine of the complex number z, \sinh(z) = (\exp(z) - \exp(-z))/2. */
inline Complex sinh(const Complex &z)
{
    return gsl_complex_sinh(z);
}

/** This function returns the complex hyperbolic cosine of the complex number z, \cosh(z) = (\exp(z) + \exp(-z))/2.  */
inline Complex cosh(const Complex &z)
{
    return gsl_complex_cosh(z);
}

/** This function returns the complex hyperbolic tangent of the complex number z, \tanh(z) = \sinh(z)/\cosh(z). */
inline Complex tanh(const Complex &z)
{
    return gsl_complex_tanh(z);
}

/** This function returns the complex hyperbolic secant of the complex number z, \sech(z) = 1/\cosh(z). */
inline Complex sech(const Complex &z)
{
    return gsl_complex_sech(z);
}

/** This function returns the complex hyperbolic cosecant of the complex number z, \csch(z) = 1/\sinh(z). */
inline Complex csch(const Complex &z)
{
    return gsl_complex_csch(z);
}

/** This function returns the complex hyperbolic cotangent of the complex number z, \coth(z) = 1/\tanh(z). */
inline Complex coth(const Complex &z)
{
    return gsl_complex_coth(z);
}

/** This function returns the complex hyperbolic arcsine of the complex number z, \arcsinh(z). The branch cuts are on the imaginary axis, below -i and above i. */
inline Complex arcsinh(const Complex &z)
{
    return gsl_complex_arcsinh(z);
}

/** This function returns the complex hyperbolic arccosine of the complex number z, \arccosh(z). The branch cut is on the real axis, less than 1. */
inline Complex arccosh(const Complex &z)
{
    return gsl_complex_arccosh(z);
}

/** This function returns the complex hyperbolic arccosine of the real number z, \arccosh(z). */
inline Complex arccosh_real(double z)
{
    return gsl_complex_arccosh_real(z);
}

/** This function returns the complex hyperbolic arctangent of the complex number z, \arctanh(z). The branch cuts are on the real axis, less than -1 and greater than 1. */
inline Complex arctanh(const Complex &z)
{
    return gsl_complex_arctanh(z);
}

/** This function returns the complex hyperbolic arctangent of the real number z, \arctanh(z). */
inline Complex arctanh_real(double z)
{
    return gsl_complex_arctanh_real(z);
}

/** This function returns the complex hyperbolic arcsecant of the complex number z, \arcsech(z) = \arccosh(1/z). */
inline Complex arcsech(const Complex &z)
{
    return gsl_complex_arcsech(z);
}

/** This function returns the complex hyperbolic arccosecant of the complex number z, \arccsch(z) = \arcsin(1/z). */
inline Complex arccsch(const Complex &z)
{
    return gsl_complex_arccsch(z);
}

/** This function returns the complex hyperbolic arccotangent of the complex number z, \arccoth(z) = \arctanh(1/z). */
inline Complex arccoth(const Complex &z)
{
    return gsl_complex_arccoth(z);
}

#endif //SIRE_SKIP_INLINE_FUNCTIONS

}

Q_DECLARE_METATYPE(SireMaths::Complex)
Q_DECLARE_TYPEINFO(SireMaths::Complex, Q_MOVABLE_TYPE);

SIRE_EXPOSE_CLASS( SireMaths::Complex )

SIRE_END_HEADER

#endif
