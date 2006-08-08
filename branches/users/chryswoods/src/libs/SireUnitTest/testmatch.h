#ifndef SIRETEST_TESTMATCH_H
#define SIRETEST_TESTMATCH_H

#include "testbase.h"

SIRE_BEGIN_HEADER

namespace SireTest
{

/**
Tests the SireDB Match classes.
 
@author Christopher Woods
*/
class SIREUNITTEST_EXPORT TestMatch : public TestBase
{
public:
    TestMatch();
    ~TestMatch();

    virtual void initialise(test_suite *test);
    
    static void runTests();
};

}

SIRE_END_HEADER

#endif
