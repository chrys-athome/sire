#ifndef SIRETEST_TESTATOMSELECTION_H
#define SIRETEST_TESTATOMSELECTION_H

#include "testbase.h"

SIRE_BEGIN_HEADER

namespace SireTest
{

/**
This tests the 'AtomSelection' class...
 
@author Christopher Woods
*/
class SIREUNITTEST_EXPORT TestAtomSelection : public TestBase
{
public:
    TestAtomSelection();
    ~TestAtomSelection();

    virtual void initialise(test_suite *test);
    
    static void runTests();
};

}

SIRE_END_HEADER

#endif
