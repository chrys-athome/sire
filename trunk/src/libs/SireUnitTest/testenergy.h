#ifndef SIRETEST_TESTENERGY_H
#define SIRETEST_TESTENERGY_H

#include "testbase.h"

SIRE_BEGIN_HEADER

namespace SireTest
{

/**
This tests the 'Energy' class...
 
@author Christopher Woods
*/
class SIREUNITTEST_EXPORT TestEnergy : public TestBase
{
public:
    TestEnergy();
    ~TestEnergy();

    virtual void initialise(test_suite *test);
    
    static void runTests();
};

}

SIRE_END_HEADER

#endif
