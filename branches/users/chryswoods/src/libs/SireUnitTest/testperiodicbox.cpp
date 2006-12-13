/**
  * @file
  *
  * C++ Implementation: TestPeriodicBox
  *
  * Description:
  * Implementation for TestPeriodicBox
  *
  * @author Christopher Woods, (C) 2006
  *
  * Copyright: See COPYING file that comes with this distribution
  *
  */

#include <QSharedData>
#include <QByteArray>
#include <QDataStream>
#include <QTime>

#include "testperiodicbox.h"

#include "SireVol/periodicbox.h"

#include "SireMol/molecule.h"

#include "SireIO/pdb.h"

#include "SireStream/datastream.h"

using namespace SireTest;
using namespace SireVol;
using namespace SireMol;
using namespace SireIO;
using namespace SireMaths;
using namespace SireStream;

TestPeriodicBox::TestPeriodicBox() : TestBase()
{}

TestPeriodicBox::~TestPeriodicBox()
{}

void TestPeriodicBox::initialise(test_suite *test)
{
    test->add( BOOST_TEST_CASE( &TestPeriodicBox::runTests ) );
}

void TestPeriodicBox::runTests()
{
    try
    {

    qDebug() << "TestPeriodicBox::runTests() begin...";

    //load a TIP4P molecule
    QList<Molecule> mols = PDB().read(":tip4p.pdb");

    BOOST_CHECK( mols.count() == 1 );

    Molecule tip4p0 = mols[0];

    BOOST_CHECK( tip4p0.nAtoms() == 4 );
    BOOST_CHECK( tip4p0.nResidues() == 1 );
    BOOST_CHECK( tip4p0.nCutGroups() == 1 );

    //store the original coordinates of the molecule...
    QVector<CoordGroup> orig_coords = tip4p0.coordGroups();

    BOOST_CHECK( orig_coords.count() == 1 );
    BOOST_CHECK( orig_coords.at(0).count() == 4 );

    //create a 10x20x40 periodic box, centered on 10,10,10
    Vector mincoords = Vector(5,0,-10);
    Vector maxcoords = Vector(15,20,30);

    PeriodicBox box( mincoords, maxcoords );

    BOOST_CHECK( mincoords == box.minCoords() );
    BOOST_CHECK( maxcoords == box.maxCoords() );

    BOOST_CHECK( Vector(10,10,10) == box.center() );

    //the box does not contain the water in the central cell
    BOOST_CHECK( not box.contains( tip4p0.coordGroups().at(0).aaBox().center() ) );

    QVector<CoordGroup> new_coords = box.moveToCenterBox(orig_coords);

    BOOST_CHECK( orig_coords.count() == new_coords.count() );
    BOOST_CHECK( orig_coords.at(0).count() == new_coords.at(0).count() );

    Molecule tip4p1 = tip4p0;

    tip4p1.setCoordinates(new_coords);

    BOOST_CHECK( std::abs(box.minimumDistance(orig_coords.at(0), new_coords.at(0))) < 1e-10 );

    BOOST_CHECK( box.contains( tip4p1.coordGroups().at(0).aaBox().center() ) );

    //loop through and check that the vectors between all of the atoms are equal
    //(and thus the atoms have been translated by a constant vector)
    Vector delta = new_coords.at(0).at(0) - orig_coords.at(0).at(0);

    for (int i=1; i<4; ++i)
    {
        Vector newdelta = new_coords.at(0).at(i) - orig_coords.at(0).at(i);

        BOOST_CHECK( delta == newdelta );
    }

    //now move the molecule back to the nearest image to the original
    //(this should overlay it exactly)
    new_coords = box.getMinimumImage(new_coords, orig_coords.at(0).aaBox().center());

    BOOST_CHECK( new_coords.count() == 1 );
    BOOST_CHECK( new_coords.at(0).count() == 4 );

    tip4p1.setCoordinates(new_coords);

    BOOST_CHECK( std::abs(box.minimumDistance(orig_coords.at(0), new_coords.at(0))) < 1e-10 );

    //should all be equal
    for (int i=0; i<4; ++i)
    {
        for (int j=0; j<3; ++j)
        {
            BOOST_CHECK_CLOSE( new_coords.at(0).at(i)[j], orig_coords.at(0).at(i)[j], 1e-6 );
        }
    }

    //now get the closest copies to the original
    Cartesian infspace;
    
    BOOST_CHECK( std::abs(infspace.minimumDistance(orig_coords.at(0), orig_coords.at(0))) < 1e-10 );
    
    new_coords = box.getMinimumImage(new_coords, orig_coords.at(0).aaBox().center());
    
    BOOST_CHECK( std::abs(infspace.minimumDistance(new_coords.at(0), orig_coords.at(0))) < 1e-10 );

    new_coords = box.getMinimumImage(new_coords, Vector(1000.0,1000.0,0.0));
    
    BOOST_CHECK( std::abs(box.minimumDistance(new_coords.at(0), orig_coords.at(0))) < 1e-10 );
    BOOST_CHECK( infspace.minimumDistance(new_coords.at(0), orig_coords.at(0)) > 1000 );

    qDebug() << infspace.minimumDistance(new_coords.at(0), orig_coords.at(0));
    qDebug() << box.minimumDistance(new_coords.at(0), orig_coords.at(0));

    QList< tuple<double,CoordGroup> > closegroups;
    
    closegroups = box.getCopiesWithin(orig_coords.at(0), orig_coords.at(0), 5.0);
    
    BOOST_CHECK_EQUAL( closegroups.count(), 1 );
    BOOST_CHECK_CLOSE( closegroups[0].get<0>(), 0.0, 1e-6 );

    closegroups = box.getCopiesWithin(orig_coords.at(0), orig_coords.at(0), 10.0);
    
    BOOST_CHECK( closegroups.count() > 1 );
    
    tuple<double,CoordGroup> group;
    
    foreach( group, closegroups )
    {
        BOOST_CHECK( std::abs(box.minimumDistance(group.get<1>(),orig_coords.at(0))) < 1e-10 );
        BOOST_CHECK( group.get<0>() <= 10.0 );
    }
    
    closegroups = box.getCopiesWithin(orig_coords.at(0), orig_coords.at(0), 20.0);
    
    foreach( group, closegroups )
    {
        BOOST_CHECK( std::abs(box.minimumDistance(group.get<1>(),orig_coords.at(0))) < 1e-10 );
        BOOST_CHECK( group.get<0>() <= 20.0 );
    }
    
    QTime t;
    t.start();
    closegroups = box.getCopiesWithin(orig_coords.at(0), orig_coords.at(0), 500.0);
    int ms = t.elapsed();
    
    qDebug() << closegroups.count() << ms;
    
    t.start();
    foreach( group, closegroups )
    {
        BOOST_CHECK( std::abs(box.minimumDistance(group.get<1>(),orig_coords.at(0))) < 1e-10 );
        BOOST_CHECK( group.get<0>() <= 500.0 );
    }
    qDebug() << t.elapsed();
    
    BOOST_CHECK( closegroups.count() > 1 );

    qDebug() << "TestPeriodicBox::runTests() complete";

    }
    catch(const SireError::exception &e)
    {
        qDebug() << e.toString();
        throw;
    }
}
