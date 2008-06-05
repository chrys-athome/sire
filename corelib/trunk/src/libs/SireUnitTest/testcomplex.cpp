
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
    BOOST_CHECK( testEqual(c.real(), 0.0) );
    BOOST_CHECK( testEqual(c.imag(), 0.0) );
    
    c = Complex(1.0);
    BOOST_CHECK( testEqual(c.real(), 1.0) );
    BOOST_CHECK( testEqual(c.imag(), 0.0) );
    
    c = Complex(1.5,2.5);
    BOOST_CHECK( testEqual(c.real(), 1.5) );
    BOOST_CHECK( testEqual(c.imag(), 2.5) );
    
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
        
        BOOST_CHECK( testEqual( c.real(), stdc.real() ) );
        BOOST_CHECK( testEqual( c.imag(), stdc.imag() ) );
        
        c = Complex(r0,i0);
        c += Complex(r1,i1);
        BOOST_CHECK( testEqual( c.real(), stdc.real() ) );
        BOOST_CHECK( testEqual( c.imag(), stdc.imag() ) );
        
        //test subtraction
        c = Complex(r0,i0) - Complex(r1,i1);
        stdc = std::complex<double>(r0,i0) - std::complex<double>(r1,i1);
        
        BOOST_CHECK( testEqual( c.real(), stdc.real() ) );
        BOOST_CHECK( testEqual( c.imag(), stdc.imag() ) );
        
        c = Complex(r0,i0);
        c -= Complex(r1,i1);
        BOOST_CHECK( testEqual( c.real(), stdc.real() ) );
        BOOST_CHECK( testEqual( c.imag(), stdc.imag() ) );
        
        //test multiplication
        c = Complex(r0,i0) * Complex(r1,i1);
        stdc = std::complex<double>(r0,i0) * std::complex<double>(r1,i1);
        
        BOOST_CHECK( testEqual( c.real(), stdc.real() ) );
        BOOST_CHECK( testEqual( c.imag(), stdc.imag() ) );
        
        c = Complex(r0,i0);
        c *= Complex(r1,i1);
        BOOST_CHECK( testEqual( c.real(), stdc.real() ) );
        BOOST_CHECK( testEqual( c.imag(), stdc.imag() ) );
        
        //test division
        if (not (SireMaths::isZero(r1) or SireMaths::isZero(i1) ) )
        {
            c = Complex(r0,i0) / Complex(r1,i1);
            stdc = std::complex<double>(r0,i0) / std::complex<double>(r1,i1);
        
            BOOST_CHECK( testEqual( c.real(), stdc.real() ) );
            BOOST_CHECK( testEqual( c.imag(), stdc.imag() ) );
        
            c = Complex(r0,i0);
            c /= Complex(r1,i1);
            BOOST_CHECK( testEqual( c.real(), stdc.real() ) );
            BOOST_CHECK( testEqual( c.imag(), stdc.imag() ) );
        
            c = Complex(r0,i0);
            c /= Complex(r1,i1);
            BOOST_CHECK( testEqual( c.real(), stdc.real() ) );
            BOOST_CHECK( testEqual( c.imag(), stdc.imag() ) );
        }
        
        //arg
        BOOST_CHECK( testEqual( c.arg(), std::arg(stdc) ) );
        
        //abs
        BOOST_CHECK( testEqual( c.abs(), std::abs(stdc) ) );
        
        //polar
        Complex p = Complex::polar(r0,i0);
        std::complex<double> stdp = std::polar(r0,i0);
        
        BOOST_CHECK( testEqual( p.real(), stdp.real() ) );
        BOOST_CHECK( testEqual( p.imag(), stdp.imag() ) );
        
        //conj
        Complex cj = c.conjugate();
        std::complex<double> stdcj = std::conj(stdc);
        BOOST_CHECK( testEqual( cj.real(), stdcj.real() ) );
        BOOST_CHECK( testEqual( cj.imag(), stdcj.imag() ) );
    
        //cos
        cj = cos(c);
        stdcj = std::cos(stdc);
        BOOST_CHECK( testEqual( cj.real(), stdcj.real() ) );
        BOOST_CHECK( testEqual( cj.imag(), stdcj.imag() ) );
        BOOST_CHECK( cj == stdcj );
        
        //sin
        cj = sin(c);
        stdcj = std::sin(stdc);
        BOOST_CHECK( testEqual( cj.real(), stdcj.real() ) );
        BOOST_CHECK( testEqual( cj.imag(), stdcj.imag() ) );
        BOOST_CHECK( cj == stdcj );
        
        //tan
        cj = tan(c);
        stdcj = std::tan(stdc);
        if ( cj != stdcj )
        {
            //precision difference - test equal to low precision
            BOOST_CHECK( testEqual( cj.real(), stdcj.real(), 1e-2 ) );
            BOOST_CHECK( testEqual( cj.imag(), stdcj.imag(), 1e-2 ) );
        }
        else
        {
            BOOST_CHECK( testEqual( cj.real(), stdcj.real() ) );
            BOOST_CHECK( testEqual( cj.imag(), stdcj.imag() ) );
            BOOST_CHECK( cj == stdcj );
        }

        //exp
        cj = exp(c);
        stdcj = std::exp(stdc);
        BOOST_CHECK( testEqual( cj.real(), stdcj.real() ) );
        BOOST_CHECK( testEqual( cj.imag(), stdcj.imag() ) );
        
        //log
        cj = log(c);
        stdcj = std::log(stdc);
        BOOST_CHECK( testEqual( cj.real(), stdcj.real() ) );
        BOOST_CHECK( testEqual( cj.imag(), stdcj.imag() ) );
        
        //sqrt
        cj = sqrt(c);
        stdcj = std::sqrt(stdc);
        BOOST_CHECK( testEqual( cj.real(), stdcj.real() ) );
        BOOST_CHECK( testEqual( cj.imag(), stdcj.imag() ) );
        
        //cosh
        cj = cosh(c);
        stdcj = std::cosh(stdc);
        BOOST_CHECK( testEqual( cj.real(), stdcj.real() ) );
        BOOST_CHECK( testEqual( cj.imag(), stdcj.imag() ) );
        
        //sinh
        cj = sinh(c);
        stdcj = std::sinh(stdc);
        BOOST_CHECK( testEqual( cj.real(), stdcj.real() ) );
        BOOST_CHECK( testEqual( cj.imag(), stdcj.imag() ) );
        
        //tanh
        cj = tanh(c);
        stdcj = std::tanh(stdc);
        BOOST_CHECK( testEqual( cj.real(), stdcj.real() ) );
        BOOST_CHECK( testEqual( cj.imag(), stdcj.imag() ) );
        
        //cos
        cj = cos(c);
        stdcj = std::cos(stdc);
        BOOST_CHECK( testEqual( cj.real(), stdcj.real() ) );
        BOOST_CHECK( testEqual( cj.imag(), stdcj.imag() ) );
        
        //sin
        cj = sin(c);
        stdcj = std::sin(stdc);
        BOOST_CHECK( testEqual( cj.real(), stdcj.real() ) );
        BOOST_CHECK( testEqual( cj.imag(), stdcj.imag() ) );
        
        //pow (int)
        cj = pow(c,2);
        stdcj = std::pow(stdc,2);
        BOOST_CHECK( testEqual( cj.real(), stdcj.real() ) );
        BOOST_CHECK( testEqual( cj.imag(), stdcj.imag() ) );
        
        //pow (double)
        cj = pow(c,2.1);
        stdcj = std::pow(stdc,2.1);
        BOOST_CHECK( testEqual( cj.real(), stdcj.real() ) );
        BOOST_CHECK( testEqual( cj.imag(), stdcj.imag() ) );
    
        //pow (complex)
        cj = pow(c,c);
        stdcj = std::pow(stdc,stdc);
        BOOST_CHECK( testEqual( cj.real(), stdcj.real() ) );
        BOOST_CHECK( testEqual( cj.imag(), stdcj.imag() ) );
        
        //comparison
        BOOST_CHECK( c == c );
        cj = c;
        BOOST_CHECK( c == cj );
        
        cj = 2 * c;
        BOOST_CHECK( c != cj );
    
        //interoperablility with std::complex
        c = stdc;
        BOOST_CHECK( testEqual( c.real(), stdc.real() ) );
        BOOST_CHECK( testEqual( c.imag(), stdc.imag() ) );
        BOOST_CHECK( c == stdc );
        
        stdc = c;
        BOOST_CHECK( testEqual( c.real(), stdc.real() ) );
        BOOST_CHECK( testEqual( c.imag(), stdc.imag() ) );
        BOOST_CHECK( stdc == c );
        
        c = Complex(stdc);
        BOOST_CHECK( testEqual( c.real(), stdc.real() ) );
        BOOST_CHECK( testEqual( c.imag(), stdc.imag() ) );
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
