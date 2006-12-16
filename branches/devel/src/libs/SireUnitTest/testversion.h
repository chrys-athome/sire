/**
  * @file
  *
  * C++ Interface: TestVersion
  *
  * Description:
  * Interface for TestVersion
  *
  * @author Christopher Woods, (C) 2006
  *
  * Copyright: See COPYING file that comes with this distribution
  *
  */
#ifndef SIRETEST_TESTVERSION_H
#define SIRETEST_TESTVERSION_H

#include "testbase.h"

SIRE_BEGIN_HEADER

namespace SireTest
{

/**
This class is used to unit test the SharedPolyPointer class.

@author Christopher Woods
*/
class SIREUNITTEST_EXPORT TestVersion : public TestBase
{
public:
    TestVersion();

    ~TestVersion();

    void initialise(test_suite *test);

    static void runTests();
};

}

SIRE_END_HEADER

#endif
