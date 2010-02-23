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

#include "complex.h"
#include "rational.h"

#include "Siren/errors.h"

#ifdef HAVE_BOOST_COMPLEX_HPP
    #include <boost/math/complex.hpp>
#endif

using namespace Siren;
using namespace SireMaths;

static const RegisterPrimitive<Complex> r_complex;

/** Construct the complex number  real + imag i */
Complex::Complex(double r, double i) : Primitive<Complex>(), z(r,i)
{}

/** Copy constructor */
Complex::Complex(const Complex &other) : Primitive<Complex>(), z(other.z)
{}

/** Destructor */
Complex::~Complex()
{}

uint Complex::hashCode() const
{
    return qHash( Complex::typeName() ) + 
           qHash(z.real()) + qHash(z.imag());
}

const std::complex<double>& Complex::toStdComplex() const
{
    return z;
}

void Complex::stream(Stream &s)
{
    s.assertVersion<Complex>(1);
    
    Schema schema = s.item<Complex>();
    
    if (s.isSaving())
    {
        schema.data("real") << z.real();
        schema.data("imaginary") << z.imag();
    }
    else 
    {
        double r,i;
    
        schema.data("real") >> r;
        schema.data("imaginary") >> i;
        
        z = std::complex<double>(r,i);
    }

}

/** Return the real part of this number */
double Complex::real() const
{
    return z.real();
}

/** Return the imaginary part of this number */
double Complex::imag() const
{
    return z.imag();
}

/** Is this a real number (imag == 0) ? */
bool Complex::isReal() const
{
    return SireMaths::isZero(imag());
}

/** Is this a pure complex number (real == 0) */
bool Complex::isPurelyComplex() const
{
    return SireMaths::isZero(real());
}

/** Is this zero? */
bool Complex::isZero() const
{
    return isReal() and SireMaths::isZero(real());
}

/** Return a string representation of this Complex number */
QString Complex::toString() const
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
Complex Complex::rect(double x, double y)
{
    return Complex(x,y);
}

/** This function returns the complex number
    z = r \exp(i \theta) = r (\cos(\theta) + i \sin(\theta))
    from the polar representation (r,theta). */
Complex Complex::polar(double r, double theta)
{
    return Complex( std::polar<double>(r,theta) );
}

/** This function sets the rectangular cartesian components (x,y) to
    the complex number z = x + i y. */
void Complex::setRectangular(double x, double y)
{
    z = std::complex<double>(x,y);
}

/** This function sets the complex number to
    z = r \exp(i \theta) = r (\cos(\theta) + i \sin(\theta))
    from the polar representation (r,theta). */
void Complex::setPolar(double r, double theta)
{
    z = std::polar<double>(r,theta);
}

/** This function sets the real part of the complex number */
void Complex::setReal(double x)
{
    z = std::complex<double>(x, imag());
}

/** This function set the imaginary part of the complex number */
void Complex::setImag(double y)
{
    z = std::complex<double>(real(), y);
}

/** Comparison operator */
bool Complex::operator==(const Complex &other) const
{
    return real() == other.real() and imag() == other.imag();
}

/** Comparison operator */
bool Complex::operator!=(const Complex &other) const
{
    return not operator==(other);
}

/** Assignment operator */
Complex& Complex::operator=(const Complex &other)
{
    z = other.z;
    return *this;
}

/** self-addition */
Complex& Complex::operator+=(const Complex &other)
{
    z += other.z;
    return *this;
}

/** self-subtraction */
Complex& Complex::operator-=(const Complex &other)
{
    z -= other.z;
    return *this;
}

/** self-multiplication */
Complex& Complex::operator*=(const Complex &other)
{
    z *= other.z;
    return *this;
}

/** self-division */
Complex& Complex::operator/=(const Complex &other)
{
    z /= other.z;
    return *this;
}

/** This function returns the negative of the complex
    number z, -z = (-x) + i(-y). */
Complex Complex::operator-() const
{
    return Complex( -z );
}

/** Comparison operator */
bool Complex::operator==(double r) const
{
    return isReal() and SireMaths::areEqual(r, real());
}

/** Comparison operator */
bool Complex::operator!=(double r) const
{
    return not operator==(r);
}

/** Assignment operator */
Complex& Complex::operator=(double r)
{
    z = std::complex<double>(r, 0);
    return *this;
}

/** self-addition */
Complex& Complex::operator+=(double r)
{
    z += std::complex<double>(r,0);
    return *this;
}

/** self-subtraction */
Complex& Complex::operator-=(double r)
{
    z -= std::complex<double>(r,0);
    return *this;
}

/** self-multiplication */
Complex& Complex::operator*=(double r)
{
    z *= std::complex<double>(r,0);
    return *this;
}

/** self-division */
Complex& Complex::operator/=(double r)
{
    z /= std::complex<double>(r,0);
    return *this;
}

/** This function returns the argument of the complex number z,
    \arg(z), where -\pi < \arg(z) <= \pi. */
double Complex::arg() const
{
    return std::arg(z);
}

/** This function returns the magnitude of the complex number z, |z|. */
double Complex::abs() const
{
    return std::abs(z);
}

/** This function returns the squared magnitude of the complex number z, |z|^2. */
double Complex::abs2() const
{
    return pow_2(real()) + pow_2(imag());
}

/** This function returns the natural logarithm of the magnitude of the
    complex number z, \log|z|. It allows an accurate evaluation of \log|z|
    when |z| is close to one. The direct evaluation of log(gsl_complex_abs(z))
    would lead to a loss of precision in this case. */
double Complex::logAbs() const
{
    return std::log( this->abs() );
}

/** This function returns the complex conjugate of the complex
    number z, z^* = x - i y. */
Complex Complex::conjugate() const
{
    return Complex( std::conj(z) );
}

/** This function returns the inverse, or reciprocal, of the
    complex number z, 1/z = (x - i y)/(x^2 + y^2). */
Complex Complex::inverse() const
{
    double inv_abs2 = 1.0 / this->abs2();

    return Complex( std::complex<double>( real() * inv_abs2,
                                          -imag() * inv_abs2 ) );
}

/** This function returns the negative of the complex
    number z, -z = (-x) + i(-y). */
Complex Complex::negative() const
{
    return std::complex<double>( -real(), -imag() );
}

namespace SireMaths
{
    /** Addition */
    Complex SIREMATHS_EXPORT operator+(const Complex &z0, const Complex &z1)
    {
        Complex r(z0);
        r += z1;
    
        return r;
    }

    /** Subtraction */
    Complex SIREMATHS_EXPORT operator-(const Complex &z0, const Complex &z1)
    {
        Complex r(z0);
        r -= z1;
        
        return r;
    }

    /** Multiplication */
    Complex SIREMATHS_EXPORT operator*(const Complex &z0, const Complex &z1)
    {
        Complex r(z0);
        r *= z1;
        
        return r;
    }

    /** Division */
    Complex SIREMATHS_EXPORT operator/(const Complex &z0, const Complex &z1)
    {
        Complex r(z0);
        r /= z1;
        
        return r;
    }

    /** Addition */
    Complex SIREMATHS_EXPORT operator+(const Complex &z, double x)
    {
        Complex r(z);
        r += x;
        
        return r;
    }

    /** Subtraction */
    Complex SIREMATHS_EXPORT operator-(const Complex &z, double x)
    {
        Complex r(z);
        r -= x;
        
        return r;
    }

    /** Multiplication */
    Complex SIREMATHS_EXPORT operator*(const Complex &z, double x)
    {
        Complex r(z);
        r *= x;
        
        return r;
    }

    /** Division */
    Complex SIREMATHS_EXPORT operator/(const Complex &z, double x)
    {
        Complex r(z);
        r /= x;
        
        return r;
    }

    /** Addition */
    Complex SIREMATHS_EXPORT operator+(double x, const Complex &z)
    {
        Complex r(z);
        r += x;
        
        return r;
    }

    /** Subtraction */
    Complex SIREMATHS_EXPORT operator-(double x, const Complex &z)
    {
        Complex r(-z);
        r += x;
        
        return r;
    }

    /** Multiplication */
    Complex SIREMATHS_EXPORT operator*(double x, const Complex &z)
    {
        Complex r(z);
        r *= x;
        
        return r;
    }

    /** Division */
    Complex SIREMATHS_EXPORT operator/(double x, const Complex &z)
    {
        return Complex(x) / z;
    }

    /** This function returns the square root of the complex number z, \sqrt z. 
        The branch cut is the negative real axis. The result always lies in the 
        right half of the complex plane. */
    Complex SIREMATHS_EXPORT sqrt(const Complex &z)
    {
        return std::sqrt(z.toStdComplex());
    }

    /** This function returns the complex square root of the real number x, 
        where x may be negative. */
    Complex SIREMATHS_EXPORT sqrt_real(double x)
    {
        Complex r(x,0);
        return std::sqrt(r.toStdComplex());
    }

    /** The function returns the complex number z raised to the complex power a, 
        z^a. This is computed as \exp(\log(z)*a) using complex logarithms and complex 
        exponentials. */
    Complex SIREMATHS_EXPORT pow(const Complex &z, const Complex &a)
    {
        return exp( log(z) * a );
    }

    /** This function returns the complex number z raised to the real power x, z^x. */
    Complex SIREMATHS_EXPORT pow(const Complex &z, double x)
    {
        return pow(z, Complex(x,0));
    }

    /** This function returns the complex number z raised to an integer power n, z^n */
    Complex SIREMATHS_EXPORT pow(const Complex &z, int n)
    {
        if (n >= 0)
        {
            switch(n)
            {
                case 0:
                    return Complex(1);
                case 1:
                    return z.toStdComplex();
                case 2:
                    return z.toStdComplex()*z.toStdComplex();
                default:
                    return pow(z.toStdComplex(), double(n));
            }
        }
        else
            return Complex(1) / pow(z.toStdComplex(), -n);
    }

    /** This function returns the complex number z raised to a rational power r, z^r */
    Complex SIREMATHS_EXPORT pow(const Complex &z, const Rational &r)
    {
        if (r.denominator() == 1)
            return pow(z.toStdComplex(), r.numerator());
        else
            return pow(z.toStdComplex(), double(r));
    }

    /** This function returns the real number x raised to the complex power z, x^z. */
    Complex SIREMATHS_EXPORT pow(double x, const Complex &z)
    {
        return pow( Complex(x,0), z );
    }

    /** This function returns the complex exponential of the complex number z,
        \exp(z). */
    Complex SIREMATHS_EXPORT exp(const Complex &z)
    {
        return Complex( std::exp(z.toStdComplex()) );
    }

    /** This function returns the complex natural logarithm (base e) of the complex 
        number z, \log(z). The branch cut is the negative real axis. */
    Complex SIREMATHS_EXPORT log(const Complex &z)
    {
        return Complex( std::log(z.toStdComplex()) );
    }

    /** This function returns the complex base-10 logarithm of the complex number z, 
        \log_10 (z). */
    Complex SIREMATHS_EXPORT log10(const Complex &z)
    {
        return Complex( std::log10(z.toStdComplex()) );
    }

    /** This function returns the complex sine of the complex number 
        z, \sin(z) = (\exp(iz) - \exp(-iz))/(2i). */
    Complex SIREMATHS_EXPORT sin(const Complex &z)
    {
        if (z.isReal())
            return std::sin(z.real());
        else
            return Complex( std::sin(z.toStdComplex()) );
    }

    /** This function returns the complex cosine of the complex number 
        z, \cos(z) = (\exp(iz) + \exp(-iz))/2. */
    Complex SIREMATHS_EXPORT cos(const Complex &z)
    {
        if (z.isReal())
            return std::cos(z.real());
        else
            return std::cos(z.toStdComplex());
    }

    /** This function returns the complex tangent of the complex number z, 
        \tan(z) = \sin(z)/\cos(z). */
    Complex SIREMATHS_EXPORT tan(const Complex &z)
    {
        if (z.isReal())
            return std::tan(z.real());
        else
            return std::tan(z.toStdComplex());
    }

    /** This function returns the complex secant of the complex number z, 
        \sec(z) = 1/\cos(z). */
    Complex SIREMATHS_EXPORT sec(const Complex &z)
    {
        return 1.0 / cos(z);
    }

    /** This function returns the complex cosecant of the complex number z, 
        \csc(z) = 1/\sin(z). */
    Complex SIREMATHS_EXPORT csc(const Complex &z)
    {
        return 1.0 / sin(z);
    }

    /** This function returns the complex cotangent of the complex number z,  
        \cot(z) = 1/\tan(z). */
    Complex SIREMATHS_EXPORT cot(const Complex &z)
    {
        return 1.0 / tan(z);
    }

    #ifndef HAVE_BOOST_COMPLEX_HPP
        static void throwBoostComplexUnsupported(const QString &codeloc)
        {
            throw Siren::unsupported( QObject::tr(
                    "This copy of Sire is linked against a version of the boost::math libraries "
                    "that doesn't support boost::math::complex. Please update to a newer version "
                    "of the boost libraries and recompile Sire."), codeloc );
        }
    #endif

    /** This function returns the complex arcsine of the complex number z, 
        \arcsin(z). The branch cuts are on the real axis, less than -1 and greater 
        than 1. */
    Complex SIREMATHS_EXPORT arcsin(const Complex &z)
    {
        #ifdef HAVE_BOOST_COMPLEX_HPP
            return boost::math::asin(z.toStdComplex());
        #else
            throwBoostComplexUnsupported(CODELOC);
            return Complex();
        #endif
    }

    /** This function returns the complex arcsine of the real number z, 
        \arcsin(z). For z between -1 and 1, the function returns a real 
        value in the range [-\pi/2,\pi/2]. For z less than -1 the result 
        has a real part of -\pi/2 and a positive imaginary part. For z greater 
        than 1 the result has a real part of \pi/2 and a negative imaginary part. */
    Complex SIREMATHS_EXPORT arcsin_real(double z)
    {
        return arcsin( Complex(z,0) );
    }

    /** This function returns the complex arccosine of the complex number z, 
        \arccos(z). The branch cuts are on the real axis, less than -1 and
         greater than 1. */
    Complex SIREMATHS_EXPORT arccos(const Complex &z)
    {
        #ifdef HAVE_BOOST_COMPLEX_HPP
            return boost::math::acos(z.toStdComplex());
        #else
            throwBoostComplexUnsupported(CODELOC);
            return Complex();
        #endif
    }

    /** This function returns the complex arccosine of the real number z, 
        \arccos(z). For z between -1 and 1, the function returns a real value in 
        the range [0,\pi]. For z less than -1 the result has a real part of \pi 
        and a negative imaginary part. For z greater than 1 the result is purely 
        imaginary and positive. */
    Complex SIREMATHS_EXPORT arccos_real(double z)
    {
        return cos(Complex(z,0));
    }

    /** This function returns the complex arctangent of the complex number z, 
        \arctan(z). The branch cuts are on the imaginary axis, below -i and above i. */
    Complex SIREMATHS_EXPORT arctan(const Complex &z)
    {
        #ifdef HAVE_BOOST_COMPLEX_HPP
            return boost::math::atan(z.toStdComplex());
        #else
            throwBoostComplexUnsupported(CODELOC);
            return Complex();
        #endif
    }

    /** This function returns the complex arcsecant of the complex number z, 
        \arcsec(z) = \arccos(1/z). */
    Complex SIREMATHS_EXPORT arcsec(const Complex &z)
    {
        return arccos( 1.0 / z );
    }

    /** This function returns the complex arcsecant of the real number z, 
        \arcsec(z) = \arccos(1/z). */
    Complex SIREMATHS_EXPORT arcsec_real(double z)
    {
        return arccos( 1.0 / Complex(z,0) );
    }

    /** This function returns the complex arccosecant of the complex number z, 
        \arccsc(z) = \arcsin(1/z). */
    Complex SIREMATHS_EXPORT arccsc(const Complex &z)
    {
        return arcsin( 1.0 / z );
    }

    /** This function returns the complex arccosecant of the real number z, 
        \arccsc(z) = \arcsin(1/z). */
    Complex SIREMATHS_EXPORT arccsc_real(double z)
    {
        return arcsin( 1.0 / Complex(z,0) );
    }

    /** This function returns the complex arccotangent of the complex number z, 
        \arccot(z) = \arctan(1/z). */
    Complex SIREMATHS_EXPORT arccot(const Complex &z)
    {
        return arctan( 1.0 / z );
    }

    /** This function returns the complex hyperbolic sine of the complex number z, 
        \sinh(z) = (\exp(z) - \exp(-z))/2. */
    Complex SIREMATHS_EXPORT sinh(const Complex &z)
    {
        return std::sinh(z.toStdComplex());
    }

    /** This function returns the complex hyperbolic cosine of the complex number z, 
        \cosh(z) = (\exp(z) + \exp(-z))/2.  */
    Complex SIREMATHS_EXPORT cosh(const Complex &z)
    {
        return std::cosh(z.toStdComplex());
    }

    /** This function returns the complex hyperbolic tangent of the complex number z, 
        \tanh(z) = \sinh(z)/\cosh(z). */
    Complex SIREMATHS_EXPORT tanh(const Complex &z)
    {
        return std::tanh(z.toStdComplex());
    }

    /** This function returns the complex hyperbolic secant of the complex number z, 
        \sech(z) = 1/\cosh(z). */
    Complex SIREMATHS_EXPORT sech(const Complex &z)
    {
        return 1.0 / cosh(z);
    }

    /** This function returns the complex hyperbolic cosecant of the complex number z, 
        \csch(z) = 1/\sinh(z). */
    Complex SIREMATHS_EXPORT csch(const Complex &z)
    {
        return 1.0 / sinh(z);
    }

    /** This function returns the complex hyperbolic cotangent of the complex number z, 
        \coth(z) = 1/\tanh(z). */
    Complex SIREMATHS_EXPORT coth(const Complex &z)
    {
        return 1.0 / tanh(z);
    }

    /** This function returns the complex hyperbolic arcsine of the complex number z, 
        \arcsinh(z). The branch cuts are on the imaginary axis, below -i and above i. */
    Complex SIREMATHS_EXPORT arcsinh(const Complex &z)
    {
        #ifdef HAVE_BOOST_COMPLEX_HPP
            return boost::math::asinh(z.toStdComplex());
        #else
            throwBoostComplexUnsupported(CODELOC);
            return Complex();
        #endif
    }

    /** This function returns the complex hyperbolic arccosine of the real number z, 
        \arcsinh(z). */
    Complex SIREMATHS_EXPORT arcsinh_real(double z)
    {
        return arcsinh( Complex(z,0) );
    }

    /** This function returns the complex hyperbolic arccosine of the complex number z, 
        \arccosh(z). The branch cut is on the real axis, less than 1. */
    Complex SIREMATHS_EXPORT arccosh(const Complex &z)
    {
        #ifdef HAVE_BOOST_COMPLEX_HPP
            return boost::math::acosh(z.toStdComplex());
        #else
            throwBoostComplexUnsupported(CODELOC);
            return Complex();
        #endif
    }

    /** This function returns the complex hyperbolic arccosine of the real number z, 
        \arccosh(z). */
    Complex SIREMATHS_EXPORT arccosh_real(double z)
    {
        return arccosh( Complex(z,0) );
    }

    /** This function returns the complex hyperbolic arctangent of the complex number z, 
        \arctanh(z). The branch cuts are on the real axis, less than -1 and greater than 1. */
    Complex SIREMATHS_EXPORT arctanh(const Complex &z)
    {
        #ifdef HAVE_BOOST_COMPLEX_HPP
            return boost::math::atanh(z.toStdComplex());
        #else
            throwBoostComplexUnsupported(CODELOC);
            return Complex();
        #endif
    }

    /** This function returns the complex hyperbolic arctangent of the real number z, 
        \arctanh(z). */
    Complex SIREMATHS_EXPORT arctanh_real(double z)
    {
        return arctanh( Complex(z,0) );
    }

    /** This function returns the complex hyperbolic arcsecant of the complex number z, 
       \arcsech(z) = \arccosh(1/z). */
    Complex SIREMATHS_EXPORT arcsech(const Complex &z)
    {
        return arccosh( 1.0 / z );
    }

    /** This function returns the complex hyperbolic arccosecant of the complex number z,
        \arccsch(z) = \arcsin(1/z). */
    Complex SIREMATHS_EXPORT arccsch(const Complex &z)
    {
        return arcsinh( 1.0 / z );
    }

    /** This function returns the complex hyperbolic arccotangent of the complex number z, 
        \arccoth(z) = \arctanh(1/z). */
    Complex SIREMATHS_EXPORT arccoth(const Complex &z)
    {
        return arctanh( 1.0 / z );
    }
}
