#ifndef SIRETEST_TESTUNITS_H
#define SIRETEST_TESTUNITS_H

#include "testbase.h"

SIRE_BEGIN_HEADER

namespace SireTest
{

/**
These tests ensure that the representation of physical constants and units within the program is consistent.
 
@author Christopher Woods
*/
class SIREUNITTEST_EXPORT TestUnits : public TestBase
{
public:
    TestUnits();
    ~TestUnits();

    virtual void initialise(test_suite *test);
    static void runTests();

};

}

SIRE_END_HEADER

#endif
