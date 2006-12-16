#ifndef SIRETEST_TESTBASE_H
#define SIRETEST_TESTBASE_H

#include <QDebug>

#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include <boost/test/floating_point_comparison.hpp>

using boost::unit_test_framework::test_suite;

#include "SireMaths/maths.h"

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireTest
{

/**
This is the base class of all unit test classes.

@author Christopher Woods
*/
class SIREUNITTEST_EXPORT TestBase
{
public:
    TestBase();
    virtual ~TestBase();

    /** Initialise this test class and add it to the suite of tests */
    virtual void initialise(test_suite *suite)=0;
};

inline void initialise(TestBase *testclass, test_suite *suite)
{
    testclass->initialise(suite);
}

}

SIRE_END_HEADER

#endif
