/**
  * @file
  *
  * C++ Interface: TestSharedPolyPointer
  *
  * Description: 
  * Interface for TestSharedPolyPointer
  * 
  * @author Christopher Woods, (C) 2006
  *
  * Copyright: See COPYING file that comes with this distribution
  *
  */
#ifndef SIRETEST_TESTSHAREDPOLYPOINTER_H
#define SIRETEST_TESTSHAREDPOLYPOINTER_H

#include "testbase.h"

SIRE_BEGIN_HEADER

namespace SireTest
{

/**
This class is used to unit test the SharedPolyPointer class.

@author Christopher Woods
*/
class SIREUNITTEST_EXPORT TestSharedPolyPointer : public TestBase
{
public:
    TestSharedPolyPointer();

    ~TestSharedPolyPointer();

    void initialise(test_suite *test);
    
    static void runTests();
};

}

SIRE_END_HEADER

#endif
