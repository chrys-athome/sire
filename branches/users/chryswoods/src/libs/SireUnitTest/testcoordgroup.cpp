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

#include "testcoordgroup.h"

#include "coordgroup.h"

using namespace SireTest;
using namespace SireVol;

TestCoordGroup::TestCoordGroup() : TestBase()
{}

TestCoordGroup::~TestCoordGroup()
{}

void TestCoordGroup::initialise(test_suite *test)
{
    test->add( BOOST_TEST_CASE( &TestCoordGroup::runTests ) );
}

void TestCoordGroup::runTests()
{
    qDebug() << "Running TestCoordGroup tests...";
    
    qDebug() << "Checking the null CoordGroup...";
    
    CoordGroupBase cgroup;
    
    BOOST_CHECK_EQUAL( cgroup.count(), quint32(0) );
    BOOST_CHECK_EQUAL( cgroup.count(), cgroup.size() );
    
    BOOST_CHECK( cgroup.isEmpty() );
    
    BOOST_CHECK_EQUAL( cgroup.constData(), (void*)0 );
    BOOST_CHECK( cgroup.aaBox() == AABox() );
    
    qDebug() << "Checking assignment from a QVector...";
    QVector<Vector> coordinates;
    
    coordinates.append( Vector(1) );
    coordinates.append( Vector(2) );
    coordinates.append( Vector(3) );
    
    cgroup = CoordGroupBase( coordinates );
    
    BOOST_CHECK_EQUAL( cgroup.count(), quint32(3) );
    BOOST_CHECK_EQUAL( cgroup.count(), cgroup.size() );
    
    BOOST_CHECK( cgroup[0] == coordinates[0] );
    BOOST_CHECK( cgroup[1] == coordinates[1] );
    BOOST_CHECK( cgroup[2] == coordinates[2] );
    
    BOOST_CHECK( cgroup.at(0) == coordinates.at(0) );
    BOOST_CHECK( cgroup.at(1) == coordinates.at(1) );
    BOOST_CHECK( cgroup.at(2) == coordinates.at(2) );
    
    BOOST_CHECK( not cgroup.isEmpty() );
    
    qDebug() << "Checking implicit copying...";
    CoordGroupBase cgroup2 = cgroup;
    
    BOOST_CHECK_EQUAL( cgroup.constData(), cgroup2.constData() );
    
    BOOST_CHECK_EQUAL( cgroup2.count(), quint32(3) );
    
    BOOST_CHECK( cgroup2[0] == coordinates[0] );
    BOOST_CHECK( cgroup2[1] == coordinates[1] );
    BOOST_CHECK( cgroup2[2] == coordinates[2] );
    
    BOOST_CHECK( cgroup == cgroup2 );
    BOOST_CHECK( not (cgroup != cgroup2) );
    
    BOOST_CHECK( not cgroup.maybeDifferent(cgroup2) );
    
    qDebug() << "Checking that the AABox is correct...";
    BOOST_CHECK( cgroup.aaBox() == AABox(coordinates) );
    BOOST_CHECK( cgroup2.aaBox() == cgroup.aaBox() );
    
    qDebug() << "Checking some advanced copying...";
    
    cgroup2 = CoordGroupBase(coordinates);
    
    BOOST_CHECK_EQUAL( cgroup2.count(), quint32(3) );
    
    BOOST_CHECK( cgroup2[0] == coordinates[0] );
    BOOST_CHECK( cgroup2[1] == coordinates[1] );
    BOOST_CHECK( cgroup2[2] == coordinates[2] );
    
    BOOST_CHECK( cgroup == cgroup2 );
    
    coordinates.append( Vector(4) );
    
    cgroup2 = CoordGroupBase(coordinates);
    
    BOOST_CHECK_EQUAL( cgroup2.count(), quint32(4) );
    
    BOOST_CHECK( cgroup2[0] == coordinates[0] );
    BOOST_CHECK( cgroup2[1] == coordinates[1] );
    BOOST_CHECK( cgroup2[2] == coordinates[2] );
    BOOST_CHECK( cgroup2[3] == coordinates[3] );
    
    BOOST_CHECK( cgroup != cgroup2 );
    
    qDebug() << "Finished TestCoordGroup tests...";
}
