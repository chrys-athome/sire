/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2007  Christopher Woods
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

#include "SireMol/qhash_siremol.h"

#include <QDebug>

#include "testcutgroup.h"

#include "SireMol/cutgroup.h"

using namespace SireTest;
using namespace SireMol;

TestCutGroup::TestCutGroup() : TestBase()
{}

TestCutGroup::~TestCutGroup()
{}

void TestCutGroup::initialise(test_suite *test)
{
    test->add( BOOST_TEST_CASE( &TestCutGroup::runTests ) );
}

void TestCutGroup::runTests()
{
    qDebug() << QObject::tr("*** THERE ARE NO CUTGROUP TESTS ***");
    qDebug() << QObject::tr("***   NEW TESTS ARE NECESSARY   ***");
}
