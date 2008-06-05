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

#include "testpropertyextractor.h"

#include "SireMol/propertyextractor.h"

#include "SireMol/atomselection.h"
#include "SireMol/molecule.h"
#include "SireMol/moleculeinfo.h"
#include "SireMol/residueinfo.h"
#include "SireMol/resid.h"
#include "SireMol/resnum.h"
#include "SireMol/cgatomid.h"
#include "SireMol/cutgroup.h"
#include "SireMol/atom.h"

#include "SireVol/coordgroup.h"

#include "SireMaths/maths.h"

#include "SireIO/pdb.h"

using namespace SireTest;
using namespace SireMol;
using namespace SireVol;
using namespace SireIO;

TestPropertyExtractor::TestPropertyExtractor() : TestBase()
{}

TestPropertyExtractor::~TestPropertyExtractor()
{}

void TestPropertyExtractor::initialise(test_suite *test)
{
    test->add( BOOST_TEST_CASE( &TestPropertyExtractor::runTests ) );
}

void TestPropertyExtractor::runTests()
{
    qDebug() << "Running TestPropertyExtractor tests...";

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

    qDebug() << "Testing the CutGroup index...";

    QHash<CutGroupID,quint32> index = p38.extract().cutGroupIndex();

    BOOST_CHECK_EQUAL( index.count(), p38.nCutGroups() );

    uint ncg = p38.nCutGroups();

    for (CutGroupID i(0); i<ncg; ++i)
    {
        BOOST_CHECK_EQUAL( i, index.value(i) );
    }

    qDebug() << "Testing extracting CoordGroups...";

    QVector<CoordGroup> coordgroups = p38.extract().coordGroups();

    BOOST_CHECK_EQUAL( coordgroups.count(), p38.nCutGroups() );

    qDebug() << "Testing extracting CutGroups...";

    QVector<CutGroup> cgroups = p38.extract().cutGroups();

    BOOST_CHECK_EQUAL( cgroups.count(), p38.nCutGroups() );

    qDebug() << "Testing extracting elements...";

    QVector< QVector<Element> > elements = p38.extract().elements();

    BOOST_CHECK_EQUAL( elements.count(), p38.nCutGroups() );

    qDebug() << "Comparing values...";

    int ngroups = p38.nCutGroups();

    for (int i=0; i<ngroups; ++i)
    {
        const CoordGroup &coordgroup = coordgroups.at(i);
        const CutGroup &cutgroup = cgroups.at(i);
        const QVector<Element> &element = elements.at(i);

        uint natms = cutgroup.nAtoms();

        BOOST_CHECK_EQUAL( coordgroup.count(), natms );
        BOOST_CHECK_EQUAL( element.count(), int(natms) );

        for (AtomID i(0); i<natms; ++i)
        {
            BOOST_CHECK( coordgroup.at(i) == cutgroup.at(i) );
            BOOST_CHECK( element.at(i) == cutgroup.at(i) );
        }
    }

    qDebug() << "Testing extracting a subset...";

    QSet<ResNum> selection;
    selection.insert( ResNum(3) );
    selection.insert( ResNum(30) );
    selection.insert( ResNum(20) );

    coordgroups = p38.extract(selection).coordGroups();
    cgroups = p38.extract(selection).cutGroups();
    elements = p38.extract(selection).elements();

    index = p38.extract(selection).cutGroupIndex();

    BOOST_CHECK_EQUAL( coordgroups.count(), cgroups.count() );
    BOOST_CHECK_EQUAL( elements.count(), cgroups.count() );
    BOOST_CHECK_EQUAL( index.count(), cgroups.count() );

    qDebug() << "Comparing values...";

    ngroups = cgroups.count();

    for (int i=0; i<ngroups; ++i)
    {
        const CoordGroup &coordgroup = coordgroups.at(i);
        const CutGroup &cutgroup = cgroups.at(i);
        const QVector<Element> &element = elements.at(i);

        uint natms = cutgroup.nAtoms();

        BOOST_CHECK_EQUAL( coordgroup.count(), natms );
        BOOST_CHECK_EQUAL( element.count(), int(natms) );

        for (AtomID i(0); i<natms; ++i)
        {
            BOOST_CHECK( coordgroup.at(i) == cutgroup.at(i) );
            BOOST_CHECK( element.at(i) == cutgroup.at(i) );
        }
    }

    qDebug() << "Verifying that the index is correct...";

    for (QHash<CutGroupID,quint32>::const_iterator it = index.constBegin();
         it != index.constEnd();
         ++it)
    {
        CutGroupID cgid = it.key();
        quint32 idx = it.value();

        BOOST_CHECK( cgroups.at(idx) == p38.extract().cutGroups().at(cgid) );
        BOOST_CHECK( coordgroups.at(idx) == p38.extract().coordGroups().at(cgid) );
        BOOST_CHECK( elements.at(idx) == p38.extract().elements().at(cgid) );
    }

    qDebug() << "Finished TestPropertyExtractor tests...";
}
