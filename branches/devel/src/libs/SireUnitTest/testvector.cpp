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

#include "testvector.h"

#include "SireMaths/vector.h"

using namespace SireTest;
using SireMaths::Vector;

TestVector::TestVector() : TestBase()
{}

TestVector::~TestVector()
{}

void TestVector::initialise(test_suite *test)
{
    test->add( BOOST_TEST_CASE( &TestVector::runTests ) );
}

void TestVector::runTests()
{
    Vector v0(1.0);
    Vector v1(1.0,1.0,1.0);

    BOOST_CHECK(v0 == v1);

    v1 = v0 + Vector(0.0,1.0,2.0);

    BOOST_CHECK_EQUAL(v1.x(), 1.0);
    BOOST_CHECK_EQUAL(v1.y(), 2.0);
    BOOST_CHECK_EQUAL(v1.z(), 3.0);

    v0 += Vector(0.0,1.0,2.0);

    //rgb values are clamped between 0 and 1
    BOOST_CHECK_EQUAL(v0.r(), 1.0);
    BOOST_CHECK_EQUAL(v0.g(), 1.0);
    BOOST_CHECK_EQUAL(v0.b(), 1.0);

    v0 *= 2.0;
    BOOST_CHECK_EQUAL(v0[0], 2.0);
    BOOST_CHECK_EQUAL(v0[1], 4.0);
    BOOST_CHECK_EQUAL(v0[2], 6.0);

    BOOST_CHECK_CLOSE(v0.length(), std::sqrt(2.0*2.0 + 4.0*4.0 + 6.0*6.0), 1e-6 );
    BOOST_CHECK_CLOSE(v1.length2(), (1.0*1.0 + 2.0*2.0 + 3.0*3.0), 1e-6 );

    BOOST_CHECK_EQUAL(v0.count(), 3);
    BOOST_CHECK_EQUAL(v0.normalise().length(), 1.0);

    Vector v2 = Vector::fromString( v0.toString() );

    BOOST_CHECK_EQUAL(v0[0], v2.at(0));
    BOOST_CHECK_EQUAL(v0[1], v2.at(1));
    BOOST_CHECK_EQUAL(v0[2], v2.at(2));

    BOOST_CHECK_EQUAL( Vector::dot(v2,v2), v2.length2() );
    BOOST_CHECK_EQUAL( Vector::dot(v1,v2), v1.x()*v2.x() + v1.y()*v2.y() + v1.z()*v2.z() );

    v1.setMax(v2);
    BOOST_CHECK_EQUAL(v1.x(), std::max(v1.x(),v2.x()) );
    BOOST_CHECK_EQUAL(v1.y(), std::max(v1.y(),v2.y()) );
    BOOST_CHECK_EQUAL(v1.z(), std::max(v1.z(),v2.z()) );

    v2.setMin(v0);
    BOOST_CHECK_EQUAL(v2.x(), std::max(v0.x(),v2.x()) );
    BOOST_CHECK_EQUAL(v2.y(), std::max(v0.y(),v2.y()) );
    BOOST_CHECK_EQUAL(v2.z(), std::max(v0.z(),v2.z()) );

    BOOST_CHECK_EQUAL(v2.invLength(), 1.0 / v2.length());
    BOOST_CHECK_EQUAL(v2.invLength2(), 1.0 / v2.length2());

}
