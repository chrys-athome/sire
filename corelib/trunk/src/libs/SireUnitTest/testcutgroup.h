#ifndef SIRETEST_TESTCUTGROUP_H
#define SIRETEST_TESTCUTGROUP_H

#include "testbase.h"

SIRE_BEGIN_HEADER

namespace SireTest
{

/**
This file is used to test the creation and manipulation of CutGroups.
 
@author Christopher Woods
*/
class SIREUNITTEST_EXPORT TestCutGroup : public TestBase
{
public:
    TestCutGroup();
    ~TestCutGroup();

    virtual void initialise(test_suite *test);
    
    static void runTests();
};

}

SIRE_END_HEADER

#endif
