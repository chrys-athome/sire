/**
  * @file
  *
  * C++ Interface: TestPeriodicBox
  *
  * Description:
  * Interface for TestPeriodicBox
  *
  * @author Christopher Woods, (C) 2006
  *
  * Copyright: See COPYING file that comes with this distribution
  *
  */
#ifndef SIRETEST_TESTPERIODICBOX_H
#define SIRETEST_TESTPERIODICBOX_H

#include "testbase.h"

SIRE_BEGIN_HEADER

namespace SireTest
{

/**
This class is used to unit test the SharedPolyPointer class.

@author Christopher Woods
*/
class SIREUNITTEST_EXPORT TestPeriodicBox : public TestBase
{
public:
    TestPeriodicBox();

    ~TestPeriodicBox();

    void initialise(test_suite *test);

    static void runTests();
};

}

SIRE_END_HEADER

#endif
