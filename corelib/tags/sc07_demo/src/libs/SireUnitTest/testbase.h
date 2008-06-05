/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
  *
  *  This program is free software; you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation; either version 2 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program; if not, write to the Free Software
  *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  *  For full details of the license please see the COPYING file
  *  that should have come with this distribution.
  *
  *  You can contact the authors via the developer's mailing list
  *  at http://siremol.org
  *
\*********************************************/

#ifndef SIRETEST_TESTBASE_H
#define SIRETEST_TESTBASE_H

#include <QDebug>

#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include <boost/test/floating_point_comparison.hpp>

using boost::unit_test_framework::test_suite;

#include "SireMaths/maths.h"

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireTest
{

/**
This is the base class of all unit test classes.

@author Christopher Woods
*/
class SIREUNITTEST_EXPORT TestBase
{
public:
    TestBase();
    virtual ~TestBase();

    /** Initialise this test class and add it to the suite of tests */
    virtual void initialise(test_suite *suite)=0;
};

inline void initialise(TestBase *testclass, test_suite *suite)
{
    testclass->initialise(suite);
}

}

SIRE_END_HEADER

#endif
