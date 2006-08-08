#ifndef SIRETEST_TESTVECTOR_H
#define SIRETEST_TESTVECTOR_H

#include "testbase.h"

SIRE_BEGIN_HEADER

namespace SireTest
{

/**
Test the 'Vector' class.
 
@author Christopher Woods
*/
class SIREUNITTEST_EXPORT TestVector : public TestBase
{
public:
    TestVector();
    ~TestVector();

    virtual void initialise(test_suite *test);
    
    static void runTests();
};

}

SIRE_END_HEADER

#endif
