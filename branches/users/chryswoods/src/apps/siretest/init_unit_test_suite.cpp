
/**
This contains the 'main' function that is needed by the boost::testing framework to initialise the suite of unit tests.

@author Christopher Woods
*/

#include <QCoreApplication>

#include "SireUnitTest/runall.h"

using namespace SireTest;

test_suite SIRE_EXPORT *init_unit_test_suite( int argc, char **argv )
{
    test_suite* test = BOOST_TEST_SUITE( "Sire Unit Test Suite" );

    //first, set the ID of the main thread - to do this we need to
    //have a QCoreApplication object..
    //(we will just have to accept the memory loss...)
    new QCoreApplication(argc, argv);

    //populate the test suite with all of the tests..
    runAll(test);

    return test;
}
