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

#include "runall.h"

#include "testangle.h"
#include "testatomselection.h"
#include "testcoordgroup.h"
#include "testcutgroup.h"
#include "testcomplex.h"
#include "testffdb.h"
#include "testmatch.h"
#include "testperiodicbox.h"
#include "testpropertyextractor.h"
#include "testsharedpolypointer.h"
#include "testsirecas.h"
#include "testunits.h"
#include "testvector.h"
#include "testversion.h"

using namespace SireTest;

void SireTest::runAll(test_suite *test)
{
    initialise(new TestAngle(), test);
    initialise(new TestAtomSelection(), test);
    initialise(new TestComplex(), test);
    initialise(new TestCoordGroup(), test);
    initialise(new TestCutGroup(), test);
    initialise(new TestFFDB(), test);
    initialise(new TestMatch(), test);
    initialise(new TestPeriodicBox(), test);
    initialise(new TestPropertyExtractor(), test);
    initialise(new TestSharedPolyPointer(), test);
    initialise(new TestSireCAS(), test);
    initialise(new TestVector(), test);
    initialise(new TestVersion(), test);
    initialise(new TestUnits(), test);
}
