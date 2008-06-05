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

#include <QDebug>

#include "testharmonicff.h"

#include "SireMM/harmonicff.h"

#include "SireMol/molecules.h"
#include "SireMol/molecule.h"
#include "SireMol/partialmolecule.h"
#include "SireMol/propertyextractor.h"
#include "SireMol/moleculemover.h"

#include "SireVol/space.h"
#include "SireVol/periodicbox.h"

#include "SireIO/pdb.h"

using namespace SireTest;
using namespace SireMM;
using namespace SireIO;
using namespace SireVol;
using namespace SireMol;

TestHarmonicFF::TestHarmonicFF() : TestBase()
{}

TestHarmonicFF::~TestHarmonicFF()
{}

void TestHarmonicFF::initialise(test_suite *test)
{
    test->add( BOOST_TEST_CASE( &TestHarmonicFF::runTests ) );
}

void TestHarmonicFF::runTests()
{
    qDebug() << "Running TestHarmonicFF tests...";

    QList<Molecule> mols = PDB().read(":water.pdb");

    BOOST_CHECK_EQUAL( mols.count(), 1679 );

    Space periodic_box = PeriodicBox( Vector(-18.3854,-18.66855,-18.4445),
                                      Vector( 18.3854, 18.66855, 18.4445) );

    for (QList<Molecule>::iterator it = mols.begin();
         it != mols.end();
         ++it)
    {
        it->setProperty("r0", QVariant::fromValue(it->extract().geometricCenter()));
        it->setProperty("k", QVariant(10.0));
    }

    HarmonicFF hff(periodic_box);

    BOOST_CHECK_CLOSE( hff.energy(), 0.0, 1e-6 );

    hff.add( PartialMolecule(mols[0]), hff.parameters().r0() == "r0" and
                                       hff.parameters().k() == "k" );

    BOOST_CHECK_CLOSE( hff.energy(), 0.0, 1e-6 );

    mols[0] = mols[0].move().translate( Vector(1,0,0) );

    hff.change(mols);

    BOOST_CHECK_CLOSE( hff.energy(), 10.0, 1e-6 );

    hff.add( PartialMolecule(mols[1]), hff.parameters().r0() == "r0" and
                                       hff.parameters().k() == "k" );

    BOOST_CHECK_CLOSE( hff.energy(), 10.0, 1e-6 );

    mols[1] = mols[1].move().translate( Vector(1,0,0) );

    hff.change(mols);

    BOOST_CHECK_CLOSE( hff.energy(), 20.0, 1e-6 );

    mols[2] = mols[2].move().translate( Vector(2,2,2) );

    hff.add( PartialMolecule(mols[2]), hff.parameters().r0() == "r0" and
                                       hff.parameters().k() == "k" );

    BOOST_CHECK_CLOSE( hff.energy(), 140.0, 1e-6 );

    hff.remove( PartialMolecule(mols[2]) );

    BOOST_CHECK_CLOSE( hff.energy(), 20.0, 1e-6 );

    mols[0] = mols[0].move().translate( Vector(-1,0,0) );
    mols[1] = mols[1].move().translate( Vector(-1,0,0) );

    hff.change(mols);

    BOOST_CHECK_CLOSE( hff.energy(), 0.0, 1e-6 );

    qDebug() << "Finished TestHarmonicFF tests...";
}
