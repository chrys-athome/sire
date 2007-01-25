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
#include <QTime>

#include "testatomselection.h"

#include "SireMol/atomselection.h"
#include "SireMol/molecule.h"
#include "SireMol/moleculeinfo.h"
#include "SireMol/residueinfo.h"
#include "SireMol/resid.h"
#include "SireMol/resnum.h"
#include "SireMol/cgatomid.h"

#include "SireMaths/maths.h"

#include "SireIO/pdb.h"

using namespace SireTest;
using namespace SireMol;
using namespace SireMaths;
using namespace SireIO;

TestAtomSelection::TestAtomSelection()
{}

TestAtomSelection::~TestAtomSelection()
{}

void TestAtomSelection::initialise(test_suite *test)
{
    test->add( BOOST_TEST_CASE( &TestAtomSelection::runTests ) );
}

void TestAtomSelection::runTests()
{
    qDebug() << QObject::tr("Running AtomSelection tests...");

    QTime t;

    t.start();
    QList<Molecule> mols = PDB().read(":p38.pdb");

    int ms = t.elapsed();
    qDebug() << "Reading p38 took " << ms;

    BOOST_CHECK_EQUAL( mols.count(), 1 );

    Molecule p38 = mols[0];

    BOOST_CHECK( p38.nResidues() > 1 );
    BOOST_CHECK( p38.nCutGroups() > 1 );
    BOOST_CHECK( p38.nAtoms() > 1 );

    AtomSelection s(p38);

    BOOST_CHECK( s.selectedAll() );
    BOOST_CHECK( not s.isEmpty() );

    BOOST_CHECK_EQUAL( s.nSelected(), p38.nAtoms() );

    uint nres = p38.nResidues();

    for (ResID i(0); i<nres; ++i)
    {
        ResNum resnum = p38.info().residueNumber(i);

        BOOST_CHECK_EQUAL( s.nSelected(resnum), p38.nAtoms(resnum) );
    }

    uint ncg = p38.nCutGroups();

    for (CutGroupID i(0); i<ncg; ++i)
    {
        BOOST_CHECK_EQUAL( s.nSelected(i), p38.nAtoms(i) );
    }

    t.start();
    s.deselectAll();
    ms = t.elapsed();

    qDebug() << "Deselecting all took " << ms;

    BOOST_CHECK( not s.selectedAll() );
    BOOST_CHECK( s.isEmpty() );

    t.start();
    for (ResID i(0); i<nres; ++i)
    {
        ResNum resnum = p38.info().residueNumber(i);

        BOOST_CHECK_EQUAL( s.nSelected(resnum), 0 );
    }

    ms = t.elapsed();
    qDebug() << "Checking each residue took " << ms;

    t.start();
    for (CutGroupID i(0); i<ncg; ++i)
    {
        BOOST_CHECK_EQUAL( s.nSelected(i), 0 );
    }
    ms = t.elapsed();
    qDebug() << "Checking each CutGroup took " << ms;

    t.start();
    s.selectAll();
    ms = t.elapsed();
    qDebug() << "Selecting all took " << ms;

    BOOST_CHECK( s.selectedAll() );
    BOOST_CHECK( not s.isEmpty() );

    BOOST_CHECK_EQUAL( s.nSelected(), p38.nAtoms() );

    for (ResID i(0); i<nres; ++i)
    {
        ResNum resnum = p38.info().residueNumber(i);

        BOOST_CHECK_EQUAL( s.nSelected(resnum), p38.nAtoms(resnum) );
    }

    for (CutGroupID i(0); i<ncg; ++i)
    {
        BOOST_CHECK_EQUAL( s.nSelected(i), p38.nAtoms(i) );
    }

    //deselect one residue at a time and check that the selection works
    t.start();
    for (ResID i(0); i<nres; ++i)
    {
        const ResidueInfo &resinfo = p38.info().at(i);

        s.deselectAll(resinfo.number());

        BOOST_CHECK( not s.selectedAll() );
        BOOST_CHECK_EQUAL( s.nSelected(resinfo.number()), 0 );

        BOOST_CHECK_EQUAL( s.nSelected(), p38.nAtoms() - resinfo.nAtoms() );

        uint nats = resinfo.nAtoms();

        for (AtomID j(0); j<nats; ++j)
        {
            BOOST_CHECK( not s.selected(resinfo[j]) );
        }

        //reselect the residue
        s.selectAll(resinfo.number());

        BOOST_CHECK( s.selectedAll(resinfo.number()) );
        BOOST_CHECK_EQUAL( s.nSelected(resinfo.number()), resinfo.nAtoms() );

        BOOST_CHECK( not s.isEmpty() );
        BOOST_CHECK( s.selectedAll() );
    }
    ms = t.elapsed();
    qDebug() << "Deselecting/selecting each residue took " << ms;

    //deselect one CutGroup at a time and check that the selection works
    t.start();
    for (CutGroupID i(0); i<ncg; ++i)
    {
        s.deselectAll(i);

        BOOST_CHECK( not s.selectedAll() );
        BOOST_CHECK_EQUAL( s.nSelected(i), 0 );

        BOOST_CHECK_EQUAL( s.nSelected(), p38.nAtoms() - p38.nAtoms(i) );

        uint nats = p38.nAtoms(i);

        for (AtomID j(0); j<nats; ++j)
        {
            BOOST_CHECK( not s.selected(CGAtomID(i,j)) );
        }

        //reselect the CutGroup
        s.selectAll(i);

        BOOST_CHECK( s.selectedAll(i) );
        BOOST_CHECK_EQUAL( s.nSelected(i), p38.nAtoms(i) );

        BOOST_CHECK( not s.isEmpty() );
        BOOST_CHECK( s.selectedAll() );
    }
    ms = t.elapsed();
    qDebug() << "Deselecting/selecting each CutGroup took " << ms;

    //deselect each atom in turn and check that the selection works...
    uint nats = p38.nAtoms();

    //pick twenty random atoms in the molecule
    int nrandom = 20;

    QHash<AtomID, CGAtomID> random_atoms;
    random_atoms.reserve(nrandom);

    UniformRanUInt ranuint = uniformRanUIntGenerator(0, nats-1);

    for (int i=0; i<nrandom; ++i)
    {
        AtomID j( ranuint() );

        random_atoms.insert( j, p38.info().at(j) );
    }

    t.start();
    for (AtomID i(0); i<nats; ++i)
    {
        const CGAtomID &cgatomid = p38.info().at(i);

        BOOST_CHECK( s.selected(cgatomid) );

        s.deselect(cgatomid);

        BOOST_CHECK( not s.selected(cgatomid) );

        BOOST_CHECK( not s.selectedAll() );
        BOOST_CHECK_EQUAL( s.nSelected(), nats - i - 1 );

        //check the status of the random atoms
        for (QHash<AtomID,CGAtomID>::const_iterator it = random_atoms.constBegin();
             it != random_atoms.constEnd();
             ++it)
        {
            if (it.key() <= i)
            {
                //this is one of the deselected atoms!
                BOOST_CHECK( not s.selected(it.value()) );
            }
            else
            {
                //this is one of the selected atoms
                BOOST_CHECK( s.selected(it.value()) );
            }
        }
    }

    ms = t.elapsed();
    qDebug() << "Deselecting each atom took " << ms;

    //all atoms should now be deselected
    BOOST_CHECK_EQUAL( s.nSelected(), 0 );
    BOOST_CHECK( not s.selectedAll() );
    BOOST_CHECK( s.isEmpty() );

    //now select each atom, one by one
    t.start();
    for (AtomID i(0); i<nats; ++i)
    {
        const CGAtomID &cgatomid = p38.info().at(i);

        BOOST_CHECK( not s.selected(cgatomid) );

        s.select(cgatomid);

        BOOST_CHECK( s.selected(cgatomid) );

        BOOST_CHECK( not s.isEmpty() );
        BOOST_CHECK_EQUAL( s.nSelected(), i+1 );

        //check the status of the random atoms
        for (QHash<AtomID,CGAtomID>::const_iterator it = random_atoms.constBegin();
             it != random_atoms.constEnd();
             ++it)
        {
            if (it.key() <= i)
            {
                //this is one of the selected atoms!
                BOOST_CHECK( s.selected(it.value()) );
            }
            else
            {
                //this is one of the deselected atoms
                BOOST_CHECK( not s.selected(it.value()) );
            }
        }
    }

    ms = t.elapsed();
    qDebug() << "Selecting each atom took " << ms;

    //all of the atoms should now be selected
    BOOST_CHECK_EQUAL( s.nSelected(), p38.nAtoms() );
    BOOST_CHECK( s.selectedAll() );
    BOOST_CHECK( not s.isEmpty() );

    qDebug() << QObject::tr("Finished AtomSelection tests");
}
