#ifndef SIRETEST_TESTCOMPLEX_H
#define SIRETEST_TESTCOMPLEX_H

#include "testbase.h"

SIRE_BEGIN_HEADER

namespace SireTest
{

/**
Tests the SireMaths::Complex class.
 
@author Christopher Woods
*/
class SIREUNITTEST_EXPORT TestComplex : public TestBase
{
public:
    TestComplex();
    ~TestComplex();

    virtual void initialise(test_suite *test);
    
    static void runTests();
};

}

SIRE_END_HEADER

#endif
