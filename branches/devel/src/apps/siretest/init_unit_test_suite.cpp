
/**
This contains the 'main' function that is needed by the boost::testing framework to initialise the suite of unit tests.

@author Christopher Woods
*/

#include <boost/test/unit_test.hpp>
using boost::unit_test_framework::test_suite;

#include <QCoreApplication>

#include "SireUnitTest/testvector.h"
#include "SireUnitTest/testenergy.h"
#include "SireUnitTest/testcutgroup.h"
#include "SireUnitTest/testunits.h"
#include "SireUnitTest/testangle.h"
#include "SireUnitTest/testcomplex.h"
#include "SireUnitTest/testsirecas.h"
#include "SireUnitTest/testmatch.h"
#include "SireUnitTest/testffdb.h"
#include "SireUnitTest/testsharedpolypointer.h"
#include "SireUnitTest/testperiodicbox.h"
#include "SireUnitTest/testversion.h"

using namespace SireTest;

test_suite SIRE_EXPORT *init_unit_test_suite( int argc, char **argv )
{
    test_suite* test= BOOST_TEST_SUITE( "Sire Unit Test Suite" );

    //first, set the ID of the main thread - to do this we need to have a QCoreApplication object..
    //(we will just have to accept the memory loss...)
    new QCoreApplication(argc, argv);

    initialise(new TestVector(), test);
    initialise(new TestEnergy(), test);
    initialise(new TestAngle(), test);
    initialise(new TestCutGroup(), test);
    initialise(new TestUnits(), test);
    initialise(new TestComplex(), test);
    initialise(new TestSireCAS(), test);
    initialise(new TestMatch(), test);
    initialise(new TestFFDB(), test);
    initialise(new TestSharedPolyPointer(), test);
    initialise(new TestPeriodicBox(), test);
    initialise(new TestVersion(), test);

    return test;
}
