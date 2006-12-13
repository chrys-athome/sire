
#include <QDebug>

#include "testcomplex.h"

#include "SireMaths/complex.h"
#include "SireMaths/maths.h"

#include <complex>

using namespace SireMaths;
using namespace SireTest;

TestComplex::TestComplex() : TestBase()
{}

TestComplex::~TestComplex()
{}

void TestComplex::initialise(test_suite *test)
{
    test->add( BOOST_TEST_CASE( &TestComplex::runTests ) );
}

void TestComplex::runTests()
{
    qDebug() << QObject::tr("Starting TestComplex::runTests()");

    Complex c;
    BOOST_CHECK_CLOSE( c.real(), 0.0, 1e-6 );
    BOOST_CHECK_CLOSE( c.imag(), 0.0, 1e-6 );

    c = Complex(1.0);
    BOOST_CHECK_CLOSE( c.real(), 1.0, 1e-6 );
    BOOST_CHECK_CLOSE( c.imag(), 0.0, 1e-6 );

    c = Complex(1.5,2.5);
    BOOST_CHECK_CLOSE( c.real(), 1.5, 1e-6 );
    BOOST_CHECK_CLOSE( c.imag(), 2.5, 1e-6 );

    UniformRand rand = uniformRanGenerator(-100.0,100.0,8423529);

    for (int i=0; i<500; ++i)
    {
        double r0 = rand();
        double r1 = rand();
        double i0 = rand();
        double i1 = rand();

        //test addition
        c = Complex(r0,i0) + Complex(r1,i1);

        std::complex<double> stdc = std::complex<double>(r0,i0) + std::complex<double>(r1,i1);

        BOOST_CHECK_CLOSE(  c.real(), stdc.real(), 1e-6 );
        BOOST_CHECK_CLOSE(  c.imag(), stdc.imag(), 1e-6 );

        c = Complex(r0,i0);
        c += Complex(r1,i1);
        BOOST_CHECK_CLOSE(  c.real(), stdc.real(), 1e-6 );
        BOOST_CHECK_CLOSE(  c.imag(), stdc.imag(), 1e-6 );

        //test subtraction
        c = Complex(r0,i0) - Complex(r1,i1);
        stdc = std::complex<double>(r0,i0) - std::complex<double>(r1,i1);

        BOOST_CHECK_CLOSE(  c.real(), stdc.real(), 1e-6 );
        BOOST_CHECK_CLOSE(  c.imag(), stdc.imag(), 1e-6 );

        c = Complex(r0,i0);
        c -= Complex(r1,i1);
        BOOST_CHECK_CLOSE(  c.real(), stdc.real(), 1e-6 );
        BOOST_CHECK_CLOSE(  c.imag(), stdc.imag(), 1e-6 );

        //test multiplication
        c = Complex(r0,i0) * Complex(r1,i1);
        stdc = std::complex<double>(r0,i0) * std::complex<double>(r1,i1);

        BOOST_CHECK_CLOSE(  c.real(), stdc.real(), 1e-6 );
        BOOST_CHECK_CLOSE(  c.imag(), stdc.imag(), 1e-6 );

        c = Complex(r0,i0);
        c *= Complex(r1,i1);
        BOOST_CHECK_CLOSE(  c.real(), stdc.real(), 1e-6 );
        BOOST_CHECK_CLOSE(  c.imag(), stdc.imag(), 1e-6 );

        //test division
        if (not (SireMaths::isZero(r1) or SireMaths::isZero(i1) ) )
        {
            c = Complex(r0,i0) / Complex(r1,i1);
            stdc = std::complex<double>(r0,i0) / std::complex<double>(r1,i1);

            BOOST_CHECK_CLOSE(  c.real(), stdc.real(), 1e-6 );
            BOOST_CHECK_CLOSE(  c.imag(), stdc.imag(), 1e-6 );

            c = Complex(r0,i0);
            c /= Complex(r1,i1);
            BOOST_CHECK_CLOSE(  c.real(), stdc.real(), 1e-6 );
            BOOST_CHECK_CLOSE(  c.imag(), stdc.imag(), 1e-6 );

            c = Complex(r0,i0);
            c /= Complex(r1,i1);
            BOOST_CHECK_CLOSE(  c.real(), stdc.real(), 1e-6 );
            BOOST_CHECK_CLOSE(  c.imag(), stdc.imag(), 1e-6 );
        }

        //arg
        BOOST_CHECK_CLOSE(  c.arg(), std::arg(stdc), 1e-6 );

        //abs
        BOOST_CHECK_CLOSE(  c.abs(), std::abs(stdc), 1e-6 );

        //polar
        Complex p = Complex::polar(r0,i0);
        std::complex<double> stdp = std::polar(r0,i0);

        BOOST_CHECK_CLOSE(  p.real(), stdp.real(), 1e-6 );
        BOOST_CHECK_CLOSE(  p.imag(), stdp.imag(), 1e-6 );

        //conj
        Complex cj = c.conjugate();
        std::complex<double> stdcj = std::conj(stdc);
        BOOST_CHECK_CLOSE(  cj.real(), stdcj.real(), 1e-6 );
        BOOST_CHECK_CLOSE(  cj.imag(), stdcj.imag(), 1e-6 );

        //cos
        cj = cos(c);
        stdcj = std::cos(stdc);
        BOOST_CHECK_CLOSE(  cj.real(), stdcj.real(), 1e-6 );
        BOOST_CHECK_CLOSE(  cj.imag(), stdcj.imag(), 1e-6 );
        BOOST_CHECK( cj == stdcj );

        //sin
        cj = sin(c);
        stdcj = std::sin(stdc);
        BOOST_CHECK_CLOSE(  cj.real(), stdcj.real(), 1e-6 );
        BOOST_CHECK_CLOSE(  cj.imag(), stdcj.imag(), 1e-6 );
        BOOST_CHECK( cj == stdcj );

        //tan
        cj = tan(c);
        stdcj = std::tan(stdc);
        if ( cj != stdcj )
        {
            //my redhat box shows big differences for very small numbers (e.g. 1e-17)
            //skip tests for such small numbers...
            if ( std::abs(cj.real()) > 1e-10 )
                BOOST_CHECK_CLOSE(  cj.real(), stdcj.real(), 1e-6 );

            if ( std::abs(cj.imag()) > 1e-10 )
                BOOST_CHECK_CLOSE(  cj.imag(), stdcj.imag(), 1e-6 );
        }
        else
        {
            BOOST_CHECK_CLOSE(  cj.real(), stdcj.real(), 1e-6 );
            BOOST_CHECK_CLOSE(  cj.imag(), stdcj.imag(), 1e-6 );
            BOOST_CHECK( cj == stdcj );
        }

        //exp
        cj = exp(c);
        stdcj = std::exp(stdc);
        BOOST_CHECK_CLOSE(  cj.real(), stdcj.real(), 1e-6 );
        BOOST_CHECK_CLOSE(  cj.imag(), stdcj.imag(), 1e-6 );

        //log
        cj = log(c);
        stdcj = std::log(stdc);
        BOOST_CHECK_CLOSE(  cj.real(), stdcj.real(), 1e-6);
        BOOST_CHECK_CLOSE(  cj.imag(), stdcj.imag(), 1e-6 );

        //sqrt
        cj = sqrt(c);
        stdcj = std::sqrt(stdc);
        BOOST_CHECK_CLOSE(  cj.real(), stdcj.real(), 1e-6 );
        BOOST_CHECK_CLOSE(  cj.imag(), stdcj.imag(), 1e-6 );

        //cosh
        cj = cosh(c);
        stdcj = std::cosh(stdc);
        BOOST_CHECK_CLOSE(  cj.real(), stdcj.real(), 1e-6 );
        BOOST_CHECK_CLOSE(  cj.imag(), stdcj.imag(), 1e-6 );

        //sinh
        cj = sinh(c);
        stdcj = std::sinh(stdc);
        BOOST_CHECK_CLOSE(  cj.real(), stdcj.real(), 1e-6 );
        BOOST_CHECK_CLOSE(  cj.imag(), stdcj.imag(), 1e-6 );

        //tanh
        cj = tanh(c);
        stdcj = std::tanh(stdc);

        //these tests fail due to machine precision problems when the
        //output is small (~1e-15)
        if (std::abs(cj.real()) > 1e-15)
            BOOST_CHECK_CLOSE(  cj.real(), stdcj.real(), 1e-6 );

        //these tests fail due to machine precision problems when the
        //output is small (~1e-8)
        if (std::abs(cj.imag()) > 1e-8)
            BOOST_CHECK_CLOSE(  cj.imag(), stdcj.imag(), 1e-6 );
        else if (std::abs(cj.imag()) > 1e-15)
            BOOST_CHECK_CLOSE( cj.imag(), stdcj.imag(), 1e-2 );

        //cos
        cj = cos(c);
        stdcj = std::cos(stdc);
        BOOST_CHECK_CLOSE(  cj.real(), stdcj.real(), 1e-6 );
        BOOST_CHECK_CLOSE(  cj.imag(), stdcj.imag(), 1e-6 );

        //sin
        cj = sin(c);
        stdcj = std::sin(stdc);
        BOOST_CHECK_CLOSE(  cj.real(), stdcj.real(), 1e-6 );
        BOOST_CHECK_CLOSE(  cj.imag(), stdcj.imag(), 1e-6 );

        //pow (int)
        cj = pow(c,2);
        stdcj = std::pow(stdc,2);
        BOOST_CHECK_CLOSE(  cj.real(), stdcj.real(), 1e-6 );
        BOOST_CHECK_CLOSE(  cj.imag(), stdcj.imag(), 1e-6 );

        //pow (double)
        cj = pow(c,2.1);
        stdcj = std::pow(stdc,2.1);
        BOOST_CHECK_CLOSE(  cj.real(), stdcj.real(), 1e-6 );
        BOOST_CHECK_CLOSE(  cj.imag(), stdcj.imag(), 1e-6 );

        //pow (complex)
        cj = pow(c,c);
        stdcj = std::pow(stdc,stdc);
        BOOST_CHECK_CLOSE(  cj.real(), stdcj.real(), 1e-6 );
        BOOST_CHECK_CLOSE(  cj.imag(), stdcj.imag(), 1e-6 );

        //comparison
        BOOST_CHECK( c == c );
        cj = c;
        BOOST_CHECK( c == cj );

        cj = 2 * c + Complex(0,1.0);
        BOOST_CHECK( c != cj );

        //interoperablility with std::complex
        c = stdc;
        BOOST_CHECK_CLOSE(  c.real(), stdc.real(), 1e-6 );
        BOOST_CHECK_CLOSE(  c.imag(), stdc.imag(), 1e-6 );
        BOOST_CHECK( c == stdc );

        stdc = c;
        BOOST_CHECK_CLOSE(  c.real(), stdc.real(), 1e-6 );
        BOOST_CHECK_CLOSE(  c.imag(), stdc.imag(), 1e-6 );
        BOOST_CHECK( stdc == c );

        c = Complex(stdc);
        BOOST_CHECK_CLOSE(  c.real(), stdc.real(), 1e-6 );
        BOOST_CHECK_CLOSE(  c.imag(), stdc.imag(), 1e-6 );
        BOOST_CHECK( c == stdc );

    }

    c = Complex(1.0);
    BOOST_CHECK( c.isReal() );
    c = Complex(0.0);
    BOOST_CHECK( c.isZero() );
    c = Complex(0.0, 1.0);
    BOOST_CHECK( not c.isReal() and SireMaths::isZero(c.real()) );

    qDebug() << QObject::tr("TestComplex::runTests() complete");
}
