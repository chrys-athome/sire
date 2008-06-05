#ifndef SIRETEST_TESTFFDB_H
#define SIRETEST_TESTFFDB_H

#include "testbase.h"

SIRE_BEGIN_HEADER

namespace SireTest
{

/**
These are tests of the SireFF parameter database classes.
 
@author Christopher Woods
*/
class SIREUNITTEST_EXPORT TestFFDB : public TestBase
{
public:
    TestFFDB();
    ~TestFFDB();

    virtual void initialise(test_suite *test);
    static void runTests();

};

}

SIRE_END_HEADER

#endif
