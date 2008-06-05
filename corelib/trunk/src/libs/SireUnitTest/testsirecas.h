#ifndef SIRETEST_TESTSIRECAS_H
#define SIRETEST_TESTSIRECAS_H

#include "testbase.h"

SIRE_BEGIN_HEADER

namespace SireTest
{

/**
Tests the SireCAS classes.
 
@author Christopher Woods
*/
class SIREUNITTEST_EXPORT TestSireCAS : public TestBase
{
public:
    TestSireCAS();
    ~TestSireCAS();

    virtual void initialise(test_suite *test);
    
    static void runTests();
};

}

SIRE_END_HEADER

#endif
