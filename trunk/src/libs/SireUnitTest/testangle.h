#ifndef SIREUNITTEST_TESTANGLE_H
#define SIREUNITTEST_TESTANGLE_H

#include "testbase.h"

SIRE_BEGIN_HEADER

namespace SireTest
{

/**
This unit test is used to test the SireMaths::Angle class.
 
@author Christopher Woods
*/
class SIREUNITTEST_EXPORT TestAngle : public TestBase
{
public:
    TestAngle();
    ~TestAngle();

    virtual void initialise(test_suite *test);
    static void runTests();

};

}

SIRE_END_HEADER

#endif
