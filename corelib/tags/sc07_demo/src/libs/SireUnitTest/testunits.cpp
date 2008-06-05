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
#include "SireUnits/temperature.h"
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
    qDebug() << QObject::tr("PI = %1, 1/4*Pi*Epsilon0 = %2 UnitCharge-2 A kcal mol-1\n"
                            "(epsilon0 == %3 F m-1)")
                    .arg(pi).arg(one_over_four_pi_eps0)
                    .arg(convertTo(epsilon0,farad/meter));

    qDebug() << QObject::tr("Gas constant = %1 kcal mol-1 K-1\n"
                            "(R = %2 J mol-1 K-1, kb = %3 J K-1)")
                    .arg(gasr)
                    .arg(convertTo(gasr, J_per_mol / kelvin))
                    .arg(convertTo(k_boltz, joule/kelvin));

    qDebug() << QObject::tr("One atmosphere = %1 kcal mol-1 A-3\n"
                            "(%2 pascal, %3 bar)")
                    .arg(atm).arg(convertTo(atm, pascal))
                    .arg(convertTo(atm,bar));

    qDebug() << QObject::tr("Electron charge = %1 mod_electrons (%2 C, %3 Faraday)\n"
                            "(1 Faraday = %4 C mol-1)")
                           .arg(mod_electron).arg(convertTo(mod_electron,coulomb))
                           .arg(convertTo(mod_electron,faraday))
                           .arg(convert( 1, faraday, coulomb / mole ));

    qDebug() << QObject::tr("One femtosecond == %1 AKMA time units (1 AKMA == %2 fs).")
                      .arg(convertFrom(1, femtosecond))
                      .arg(convert(1, akma_time, femtosecond));

    //test conversion of epsilon_0 from SI units to internal units
    BOOST_CHECK_CLOSE( convertFrom(8.854187817e-12,
                                  (coulomb*coulomb) / (joule*meter)),
                       epsilon0, 1e-6 );

    BOOST_CHECK_CLOSE( convertFrom(1.380650524e-23, joule / kelvin), k_boltz, 1e-4 );

    BOOST_CHECK_CLOSE( double(akma_time.in(femtosecond)), 48.8882, 1e-4 );

    BOOST_CHECK_CLOSE( double(-e_charge), double(mod_electron), 1e-6 );
    BOOST_CHECK_CLOSE( convertTo(mod_electron, coulomb), 1.60217646263e-19, 1e-6 );
    BOOST_CHECK_CLOSE( double(1 * faraday), double(96485.3415 * coulomb_per_mol), 1e-6 );

    //check that all of the units are consistent!
    BOOST_CHECK_CLOSE( double( kilogram * meter * meter / (second*second) ),
                       double(joule), 1e-6 );

    BOOST_CHECK_CLOSE( double( kg_per_mol * meter * meter / (second*second) ),
                       double(J_per_mol), 1e-6 );

    //this should not compile if dimension checking isn't working!
    qDebug() << kilogram * meter * meter / (second * second) << joule;

    BOOST_CHECK( std::abs(kilogram * meter * meter / (second*second) - joule)
                                                  < joule * 0.00001 );

    qDebug() << kg_per_mol * meter * meter / (second * second) << J_per_mol;

    BOOST_CHECK( std::abs(kg_per_mol * meter * meter / (second*second) - J_per_mol)
                                                  < J_per_mol * 0.00001 );

    //test conversion of gas constant from SI units to internal units
    BOOST_CHECK_CLOSE( convertFrom(8.31447215, (joule/mole) / kelvin), gasr, 1e-6 );

    //test conversion of atmospheres into internal pressure units (kcal mol-1 A-3)
    BOOST_CHECK_CLOSE( convertFrom(101325.0, joule / (meter*meter*meter)), double(atm), 1e-6 );

    //now check that the constant agree with those calculated and used in ProtoMS2
    BOOST_CHECK_CLOSE( one_over_four_pi_eps0, 332.06371, 1e-6 );
    BOOST_CHECK_CLOSE( gasr, 0.00198720653680688336, 1e-6 );
    BOOST_CHECK_CLOSE( double(atm), 1.45839756e-05, 1e-6 );

    BOOST_CHECK_CLOSE( double( kilogram * (meter*meter) / (second*second) ),
                       double( joule ), 1e-6 );

    //check conversions
    BOOST_CHECK_CLOSE( convertFrom(5.5, kcal_per_mol), convertFrom(5500.0, cal_per_mol), 1e-6 );
    BOOST_CHECK_CLOSE( convertFrom(15.31, angstrom), convertFrom(1.531, nanometer), 1e-6 );

    BOOST_CHECK_CLOSE( double(25 * celsius), double(298.15 * kelvin), 1e-6 );
    BOOST_CHECK_CLOSE( double(5*celsius + 10*celsius), double(15*celsius), 1e-6 );
    BOOST_CHECK_CLOSE( double(5*fahrenheit + 10*fahrenheit), double(15*fahrenheit), 1e-6 );
    BOOST_CHECK_CLOSE( double(100 * fahrenheit), double(((100-32)/1.8) * celsius), 1e-6 );

    BOOST_CHECK_CLOSE( (100 * fahrenheit).to(celsius), (100-32)/1.8, 1e-6 );

    qDebug() << "100 Fahrenheit ==" << (100*fahrenheit).to(celsius) << "Celsius";

    qDebug() << QObject::tr("Finished TestUnits tests");
}
