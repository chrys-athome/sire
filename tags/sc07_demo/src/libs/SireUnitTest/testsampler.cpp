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

#include "testsampler.h"

#include "SireMove/sampler.h"
#include "SireMove/uniformsampler.h"

#include "SireMol/molecule.h"
#include "SireMol/molecules.h"
#include "SireMol/partialmolecule.h"
#include "SireMol/moleculegroup.h"

#include "SireMaths/rangenerator.h"

#include "SireIO/pdb.h"

#include "SireSystem/checkpoint.h"
#include "SireSystem/system.h"
#include "SireSystem/localsimsystem.h"

#include "SireError/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireTest;
using namespace SireMove;
using namespace SireIO;
using namespace SireMol;
using namespace SireMaths;
using namespace SireSystem;
using namespace SireStream;

TestSampler::TestSampler() : TestBase()
{}

TestSampler::~TestSampler()
{}

void TestSampler::initialise(test_suite *test)
{
    test->add( BOOST_TEST_CASE( &TestSampler::runTests ) );
}

void TestSampler::runTests()
{
    try
    {

    qDebug() << "Running TestSampler tests...";

    QList<Molecule> mols = PDB().read(":water.pdb");

    BOOST_CHECK_EQUAL( mols.count(), 1679 );

    int nmols = mols.count();

    MoleculeGroup group;
    group.add(mols);

    //create the system that holds the molecules
    System system(group);
    LocalSimSystem simsystem( system.checkPoint() );

    RanGenerator rand;

    UniformSampler unisampler(group, rand);

    UniformSampler unisampler2 = unisampler;

    QByteArray pickle;
    QDataStream ds(&pickle, QIODevice::WriteOnly);

    ds << unisampler;

    ds.setDevice(0);

    BOOST_CHECK( unisampler.generator() == rand );

    QSet<MoleculeID> molids;

    for (int i=0; i<100; ++i)
    {
        tuple<PartialMolecule,double> mol = unisampler.sample(simsystem);

        BOOST_CHECK_EQUAL( mol.get<1>(), 1.0 / nmols );
        BOOST_CHECK( group.contains(mol.get<0>()) );

        molids.insert( mol.get<0>().ID() );

        BOOST_CHECK_EQUAL( unisampler.probabilityOf(mol.get<0>(), simsystem),
                           1.0 / nmols );
    }

    qDebug() << "Selected" << molids.count() << "molecules..." << molids;

    //this may fail ( 1 in 1679**100 chance... )
    BOOST_CHECK( molids.count() > 1 );

    //unpack the pickle and ensure that it duplicates the selected molecules
    QDataStream ds2(&pickle, QIODevice::ReadOnly);

    ds2 >> unisampler;

//     QSet<MoleculeID> same_mols;
//
//     for (int i=0; i<100; ++i)
//     {
//         tuple<PartialMolecule,double> mol = unisampler.sample(simsystem);
//
//         BOOST_CHECK_EQUAL( mol.get<1>(), 1.0 / nmols );
//         BOOST_CHECK( group.contains(mol.get<0>()) );
//
//         BOOST_CHECK( molids.contains(mol.get<0>().ID()) );
//
//         BOOST_CHECK_EQUAL( unisampler.probabilityOf(mol.get<0>(), simsystem),
//                            1.0 / nmols );
//
//         tuple<PartialMolecule,double> mol2 = unisampler2.sample(simsystem);
//
//         if (mol2.get<0>() == mol.get<0>())
//             same_mols.insert(mol2.get<0>().ID());
//     }
//
//     //we mustn't repeat the same sample order!
//     BOOST_CHECK( same_mols.count() != 1679 );

    qDebug() << "Finished TestSampler tests...";

    }
    catch(SireError::exception &e)
    {
        qDebug() << e.toString();
        throw;
    }
}
