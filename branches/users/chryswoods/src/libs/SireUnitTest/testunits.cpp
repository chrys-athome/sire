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

#include "testunits.h"

#include "SireUnits/units.h"
#include "SireUnits/convert.h"

using namespace SireUnits;
using namespace SireTest;

TestUnits::TestUnits()
{}

TestUnits::~TestUnits()
{}

void TestUnits::initialise(test_suite *test)
{
    test->add( BOOST_TEST_CASE( &TestUnits::runTests ) );
}

void TestUnits::runTests()
{
    qDebug() << QObject::tr("Running TestUnits tests...");

    //print out the values of the physical constants
    qDebug() << QObject::tr("PI = %1, 1/4*Pi*Epsilon0 = %2 (UnitCharge-2 A kcal mol-1)")
                    .arg(pi).arg(one_over_four_pi_eps0);
    qDebug() << QObject::tr("Gas constant = %1 (kcal mol-1 K-1)")
                    .arg(gasr);
    qDebug() << QObject::tr("One atmosphere = %1 (kcal mol-1 A-3)").arg(1*atm);

    //test conversion of epsilon_0 from SI units to internal units
    BOOST_CHECK_CLOSE( convertFrom(8.854187817e-12, (coulomb*coulomb) / (joule*meter)), epsilon0, 1e-6 );

    //test conversion of gas constant from SI units to internal units
    BOOST_CHECK_CLOSE( convertFrom(8.31447215, (joule/mole) / kelvin), gasr, 1e-6 );

    //test conversion of atmospheres into internal pressure units (kcal mol-1 A-3)
    BOOST_CHECK_CLOSE( convertFrom(101325.0, joule / (meter*meter*meter)), double(atm), 1e-6 );

    //now check that the constant agree with those calculated and used in ProtoMS2
    BOOST_CHECK_CLOSE( one_over_four_pi_eps0, 332.06371, 1e-6 );
    BOOST_CHECK_CLOSE( gasr, 0.00198720653680688336, 1e-6 );
    BOOST_CHECK_CLOSE( double(atm), 1.45839756e-05, 1e-6 );

    //check conversions
    BOOST_CHECK_CLOSE( convertFrom(5.5, kcal_per_mol), convertFrom(5500.0, cal_per_mol), 1e-6 );
    BOOST_CHECK_CLOSE( convertFrom(15.31, angstrom), convertFrom(1.531, nanometer), 1e-6 );
}
