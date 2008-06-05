
#include "SireCAS/qhash_sirecas.h"

#include "testsirecas.h"

#include "SireCAS/expression.h"
#include "SireCAS/symbol.h"
#include "SireCAS/values.h"
#include "SireCAS/trigfuncs.h"
#include "SireCAS/exp.h"
#include "SireCAS/i.h"
#include "SireCAS/complexvalues.h"

#include "SireMaths/maths.h"

using namespace SireCAS;
using namespace SireMaths;
using namespace SireTest;

TestSireCAS::TestSireCAS() : TestBase()
{}

TestSireCAS::~TestSireCAS()
{}

void TestSireCAS::initialise(test_suite *test)
{
    test->add( BOOST_TEST_CASE( &TestSireCAS::runTests ) );
}

bool testEqual(const Complex &z0, const Complex &z1, double epsilon=1.0e-8)
{
    if (z0 != z1)
    {
        bool areequal = SireMaths::areEqual(z0.real(),z1.real(),epsilon) and 
                        SireMaths::areEqual(z0.imag(),z1.imag(),epsilon);
                        
        if (not areequal)
            qDebug() << QObject::tr(
                "Two values are not equal: %1 and %2")
                    .arg(z0.toString()).arg(z1.toString());
        
        return areequal;
    }
    else
        return true;
}

void TestSireCAS::runTests()
{
    try
    {

    qDebug() << QObject::tr("Starting TestSireCAS::runTests()");
    
    Symbol x("x");
    Symbol y("y");
    Symbol z("z");
    
    //test basic function...
    Expression ex0 = x + y + z;
    Expression ex1 = x + y - z;
    Expression ex2 = x - y + z;
    Expression ex3 = x * y + z;
    Expression ex4 = x*y*z;
    
    Expression ex5 = 2*x + y + z;
    Expression ex6 = 2.5*x + y + z + 3.2;
    Expression ex7 = 2.5*x*y - z - 6.4;
    Expression ex8 = 6.8*x + y + z + 3.2*x;
    Expression ex9 = 9.2*x + 3.1*y + 5.4*z - 3.2*x;
    
    BOOST_CHECK( not ex0.isComplex() );
    BOOST_CHECK( not ex0.isConstant() );
    BOOST_CHECK( not ex0.isZero() );
    BOOST_CHECK( ex0.isFunction(x) );
    BOOST_CHECK( ex0.isFunction(y) );
    BOOST_CHECK( ex0.isFunction(z) );
    
    BOOST_CHECK( not ex1.isComplex() );
    BOOST_CHECK( not ex1.isConstant() );
    BOOST_CHECK( not ex1.isZero() );
    BOOST_CHECK( ex1.isFunction(x) );
    BOOST_CHECK( ex1.isFunction(y) );
    BOOST_CHECK( ex1.isFunction(z) );
    
    BOOST_CHECK( not ex2.isComplex() );
    BOOST_CHECK( not ex2.isConstant() );
    BOOST_CHECK( not ex2.isZero() );
    BOOST_CHECK( ex2.isFunction(x) );
    BOOST_CHECK( ex2.isFunction(y) );
    BOOST_CHECK( ex2.isFunction(z) );
    
    BOOST_CHECK( not ex3.isComplex() );
    BOOST_CHECK( not ex3.isConstant() );
    BOOST_CHECK( not ex3.isZero() );
    BOOST_CHECK( ex3.isFunction(x) );
    BOOST_CHECK( ex3.isFunction(y) );
    BOOST_CHECK( ex3.isFunction(z) );
    
    BOOST_CHECK( not ex4.isComplex() );
    BOOST_CHECK( not ex4.isConstant() );
    BOOST_CHECK( not ex4.isZero() );
    BOOST_CHECK( ex4.isFunction(x) );
    BOOST_CHECK( ex4.isFunction(y) );
    BOOST_CHECK( ex4.isFunction(z) );
    
    UniformRand rand = uniformRanGenerator(-100.0,100.0,48923);
    
    for (int i=0; i<500; ++i)
    {
        double xval = rand();
        double yval = rand();
        double zval = rand();
        
        Values vals( x==xval, y==yval, z==zval );
        
        BOOST_CHECK( testEqual( ex0(vals), xval + yval + zval ) );
        BOOST_CHECK( testEqual( ex1(vals), xval + yval - zval ) );
        BOOST_CHECK( testEqual( ex2(vals), xval - yval + zval ) );
        BOOST_CHECK( testEqual( ex3(vals), xval * yval + zval ) );
        BOOST_CHECK( testEqual( ex4(vals), xval * yval * zval ) );
    
        BOOST_CHECK( testEqual( ex5(vals), 2*xval + yval + zval ) );
        BOOST_CHECK( testEqual( ex6(vals), 2.5*xval + yval + zval + 3.2 ) );
        BOOST_CHECK( testEqual( ex7(vals), 2.5*xval*yval - zval - 6.4 ) );
        BOOST_CHECK( testEqual( ex8(vals), 6.8*xval + yval + zval + 3.2*xval ) );
        BOOST_CHECK( testEqual( ex9(vals), 9.2*xval + 3.1*yval + 5.4*zval - 3.2*xval ) );
    }
    
    //test trig functions
    for (int i=0; i<500; ++i)
    {
        double xval = rand();
        
        BOOST_CHECK( testEqual( Cos(x).evaluate(x==xval), std::cos(xval) ) );
        BOOST_CHECK( testEqual( Sin(x).evaluate(x==xval), std::sin(xval) ) );
        BOOST_CHECK( testEqual( Tan(x).evaluate(x==xval), std::tan(xval) ) );
        BOOST_CHECK( testEqual( Csc(x).evaluate(x==xval), 1.0 / std::sin(xval) ) );
        BOOST_CHECK( testEqual( Sec(x).evaluate(x==xval), 1.0 / std::cos(xval) ) );
        BOOST_CHECK( testEqual( Cot(x).evaluate(x==xval), 1.0 / std::tan(xval) ) );
/*        BOOST_CHECK( testEqual( Cosh(x).evaluate(x==xval), std::cosh(xval) ) );
        BOOST_CHECK( testEqual( Sinh(x).evaluate(x==xval), std::sinh(xval) ) );
        BOOST_CHECK( testEqual( Tanh(x).evaluate(x==xval), std::tanh(xval) ) );*/
    }
    
    //trig expressions
    ex0 = Cos(x) + Sin(x);
    ex1 = 3.5*Cos(x) - 2.4*Sin(x);
    ex2 = 5.4*Cos( Sin(x) );
    ex3 = 3.1*Cos( Sin( Tan(x) ) );
    ex4 = -Cos(x);
    
    for (int i=0; i<500; ++i)
    {
        double xval = rand();
        
        double cosx = std::cos(xval);
        double sinx = std::sin(xval);
        double tanx = std::tan(xval);
        
        BOOST_CHECK( testEqual( ex0(x==xval), cosx+sinx ) );
        BOOST_CHECK( testEqual( ex1(x==xval), 3.5*cosx - 2.4*sinx ) );
        BOOST_CHECK( testEqual( ex2(x==xval), 5.4*std::cos(sinx) ) );
        BOOST_CHECK( testEqual( ex3(x==xval), 3.1*std::cos(std::sin(tanx)) ) );
        BOOST_CHECK( testEqual( ex4(x==xval), -cosx ) );
    }
    
    //powers
    ex0 = pow(x,2);
    ex1 = pow(x,3);
    ex2 = 3.5*pow(x,3) + 2.1*pow(x,2) - 6.4*x + 0.3;
    ex3 = pow(x,y);
    ex4 = pow(x, Sin(x*y));
    ex5 = pow(2.4, x);
    ex6 = pow(SireMaths::e, x);
    ex7 = Ln(x);
    ex8 = Ln(Exp(x));
    ex9 = pow(x, 4.3);
    
    BOOST_CHECK( not ex0.isComplex() );
    BOOST_CHECK( not ex0.isConstant() );
    BOOST_CHECK( not ex0.isZero() );
    BOOST_CHECK( ex0.isFunction(x) );
    BOOST_CHECK( not ex0.isFunction(y) );
    BOOST_CHECK( not ex0.isFunction(z) );
    
    for (int i=0; i<500; ++i)
    {
        double xval = std::abs( rand() );
        double yval = std::abs( rand() );
	
        double x2 = xval*xval;
        double x3 = xval*xval*xval;
        
        Values vals(x==xval, y==yval);
        
        BOOST_CHECK( testEqual( ex0(vals), x2 ) );
        BOOST_CHECK( testEqual( ex1(vals), x3 ) );
        BOOST_CHECK( testEqual( ex2(vals), 3.5*x3 + 2.1*x2 - 6.4*xval + 0.3 ) );
        BOOST_CHECK( testEqual( ex3(vals), std::pow(xval,yval) ) );
        BOOST_CHECK( testEqual( ex4(vals), std::pow(xval, std::sin(xval*yval)) ) );
        BOOST_CHECK( testEqual( ex5(vals), std::pow(2.4, xval) ) );
        BOOST_CHECK( testEqual( ex6(vals), std::exp(xval) ) );
        BOOST_CHECK( testEqual( ex7(vals), std::log(xval) ) );
        BOOST_CHECK( testEqual( ex8(vals), std::log(std::exp(xval)) ) );
        BOOST_CHECK( testEqual( ex9(vals), std::pow(xval, 4.3) ) );
    }
    
    //complex maths
    ex0 = I();
    ex1 = 5.4 + 3.2*I();
    ex2 = 5.4*x + 3.2*y*I();
    ex3 = Sin(x*I());
    ex4 = Cos(x) + I()*Sin(y);
    ex5 = pow(x+3*I(), 3.2+y*I());
    ex6 = Exp(-1 - I()*x);
    ex7 = Ln(5.3*x + 3.2*y*I());
    ex8 = I()*I()*x;
    ex9 = pow(I(),2)*x;
    
    BOOST_CHECK( ex0.isComplex() );
    BOOST_CHECK( ex0.isConstant() );
    BOOST_CHECK( not ex0.isZero() );
    BOOST_CHECK( not ex0.isFunction(x) );
    BOOST_CHECK( not ex0.isFunction(y) );
    BOOST_CHECK( not ex0.isFunction(z) );
    
    BOOST_CHECK( ex1.isComplex() );
    BOOST_CHECK( ex1.isConstant() );
    
    BOOST_CHECK( ex4.isComplex() );
    BOOST_CHECK( not ex4.isConstant() );
    BOOST_CHECK( not ex4.isZero() );
    BOOST_CHECK( ex4.isFunction(x) );
    BOOST_CHECK( ex4.isFunction(y) );
    BOOST_CHECK( not ex4.isFunction(z) );
    
    BOOST_CHECK( not ex8.isComplex() );
    
    for (int i=0; i<500; ++i)
    {
        double xval = rand();
        double yval = rand();
        
        ComplexValues vals(x==xval, y==yval);
        
        BOOST_CHECK( testEqual( ex0(vals), Complex(0,1) ) );
        BOOST_CHECK( testEqual( ex1(vals), Complex(5.4,3.2) ) );
        BOOST_CHECK( testEqual( ex2(vals), Complex(5.4*xval,3.2*yval) ) );
        BOOST_CHECK( testEqual( ex3(vals), sin(Complex(0,xval)) ) );
        BOOST_CHECK( testEqual( ex4(vals), cos(xval) + Complex(0,1)*sin(yval) ) );
        BOOST_CHECK( testEqual( ex5(vals), pow(Complex(xval,3),Complex(3.2,yval)) ) );
        BOOST_CHECK( testEqual( ex6(vals), exp(Complex(-1,-xval)) ) );
        BOOST_CHECK( testEqual( ex7(vals), log(Complex(5.3*xval,3.2*yval)) ) );
        BOOST_CHECK( testEqual( ex8(vals), -xval ) );
        BOOST_CHECK( testEqual( ex9(vals), -xval ) );
    }
    
    //differentiation
    ex0 = pow(x,2);
    Expression dx0 = 2*x;
    
    ex1 = 5.6*pow(x,3) - 9.1*pow(x,2) + 5.3*x - 1.0;
    Expression dx1 = 5.6*3*pow(x,2) - 9.1*2*x + 5.3;
    
    ex2 = Sin(x);
    Expression dx2 = Cos(x);
    
    ex3 = Sin(pow(x,2));
    Expression dx3 = 2*x*Cos(pow(x,2));
    
    ex4 = Sin( Cos(x) );
    Expression dx4 = -Sin(x) * Cos( Expression(Cos(x)) );
    
    for (int i=0; i<500; ++i)
    {
        double xval = rand();
        Values vals(x==xval);
        
        BOOST_CHECK( testEqual( ex0.differentiate(x).evaluate(vals), dx0(vals) ) );
        BOOST_CHECK( testEqual( ex1.differentiate(x).evaluate(vals), dx1(vals) ) );
        BOOST_CHECK( testEqual( ex2.differentiate(x).evaluate(vals), dx2(vals) ) );
        BOOST_CHECK( testEqual( ex3.differentiate(x).evaluate(vals), dx3(vals) ) );
        BOOST_CHECK( testEqual( ex4.differentiate(x).evaluate(vals), dx4(vals) ) );
    }
    
    //integration
    ex0 = pow(x,2);
    Expression ix0 = (1.0/3.0)*pow(x,3);
    
    ex1 = Cos(x);
    Expression ix1 = Sin(x);
    
    ex2 = pow( Cos(x), Sin(y) );
    Expression ix2 = ( 1.0/(-Sin(x) * (Sin(y)+1)) ) * pow( Cos(x), Sin(y)+1 );
    
    for (int i=0; i<500; ++i)
    {
        double xval = rand();
        Values vals(x==xval);
        
        BOOST_CHECK( testEqual( ex0.integrate(x).evaluate(vals), ix0(vals) ) );
        BOOST_CHECK( testEqual( ex1.integrate(x).evaluate(vals), ix1(vals) ) );
        BOOST_CHECK( testEqual( ex2.integrate(x).evaluate(vals), ix2(vals) ) );
    }
    
    qDebug() << QObject::tr("TestSireCAS::runTests() complete");
    
    }
    catch(const SireError::exception &error)
    {
        qDebug() << error.toString();
        throw;
    }
}
