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

#include "testforcefields.h"

#include "SireFF/forcefields.h"
#include "SireFF/forcefield.h"

#include "SireMM/intercljff.h"
#include "SireMM/intergroupcljff.h"
#include "SireMM/intercoulombff.h"
#include "SireMM/intergroupcoulombff.h"
#include "SireMM/interljff.h"
#include "SireMM/intergroupljff.h"

#include "SireMM/chargeparameter.h"
#include "SireMM/ljparameter.h"
#include "SireMM/atomiccharges.h"
#include "SireMM/atomicljs.h"

#include "SireMM/switchingfunction.h"

#include "SireMol/partialmolecule.h"
#include "SireMol/molecule.h"

#include "SireVol/periodicbox.h"
#include "SireVol/cartesian.h"
#include "SireVol/space.h"

#include "SireIO/pdb.h"

#include "SireUnits/units.h"

#include <QTime>

using namespace SireTest;

using namespace SireMaths;
using namespace SireMM;
using namespace SireFF;
using namespace SireMol;
using namespace SireVol;
using namespace SireIO;
using namespace SireUnits;

TestForceFields::TestForceFields() : TestBase()
{}

TestForceFields::~TestForceFields()
{}

void TestForceFields::initialise(test_suite *test)
{
    test->add( BOOST_TEST_CASE( &TestForceFields::runTests ) );
}

void TestForceFields::runTests()
{
    qDebug() << "Running TestForceFields tests...";

    QTime t;

    t.start();

    QList<Molecule> mols = PDB().read(":water.pdb");

    BOOST_CHECK_EQUAL(mols.count(), 1679);

    //specify the space for the system
    Space periodic_box = PeriodicBox( Vector(-18.3854,-18.66855,-18.4445),
                                      Vector( 18.3854, 18.66855, 18.4445) );

    Space cartesian = Cartesian();

    SwitchingFunction long_cutoff = HarmonicSwitchingFunction(15.0, 14.5);
    SwitchingFunction short_cutoff = HarmonicSwitchingFunction(5.0, 4.5);

    //create parameters for all of the molecules...
    QVector<ChargeParameter> group_chgs;

    group_chgs.append( ChargeParameter(0) );                     // Oxygen
    group_chgs.append( ChargeParameter(0.52 * mod_electrons) );  // Hydrogen
    group_chgs.append( ChargeParameter(0.52 * mod_electrons) );  // Hydrogen
    group_chgs.append( ChargeParameter(-1.04 * mod_electrons) ); // M atom

    AtomicCharges chgs(group_chgs);

    QVector<LJParameter> group_ljs;

    //Oxygen is the only atom with an LJ parameter
    group_ljs.append( LJParameter(3.15365 * angstrom, 0.1550 * kcal_per_mol) );

    for (int i=0; i<3; ++i)
        group_ljs.append( LJParameter() );  // remaining atoms

    AtomicLJs ljs(group_ljs);

    for (QList<Molecule>::iterator it = mols.begin();
         it != mols.end();
         ++it)
    {
        it->setProperty( "charges", chgs );
        it->setProperty( "ljs", ljs );
    }

    int ms = t.elapsed();

    qDebug() << "Loading and parametising the system took" << ms << "ms";

    //ok, lets put all of the molecules into a single forcefield and check that
    //the energy is correct!
    InterCLJFF cljff(periodic_box, long_cutoff);

    cljff.add(mols, cljff.parameters().coulomb() == "charges" and
                    cljff.parameters().lj() == "ljs" );

    t.start();

    double nrg = cljff.energy();

    ms = t.elapsed();

    qDebug() << "Calculating the energy (" << nrg << ") took" << ms << "ms";

    double known_total_nrg = -16364.0932341;
    double known_total_cnrg = -19298.4644394;
    double known_total_ljnrg = 2934.3712053;

    BOOST_CHECK_CLOSE( nrg, known_total_nrg, 1e-6 );
    BOOST_CHECK_CLOSE( cljff.energy(cljff.components().coulomb()),
                       known_total_cnrg, 1e-6 );
    BOOST_CHECK_CLOSE( cljff.energy(cljff.components().lj()),
                       known_total_ljnrg, 1e-6 );

    //lets try using a cartesian space
    cljff.setProperty( "space", cartesian );

    t.start();

    nrg = cljff.energy();

    ms = t.elapsed();

    qDebug() << "Calculating the energy (" << nrg << ") took" << ms << "ms";

    double known_cartesian_nrg = -14467.270807;
    double known_cartesian_cnrg = -17238.581486;
    double known_cartesian_ljnrg = 2771.31067901;

    BOOST_CHECK_CLOSE( nrg, known_cartesian_nrg, 1e-6 );
    BOOST_CHECK_CLOSE( cljff.energy(cljff.components().coulomb()),
                       known_cartesian_cnrg, 1e-6 );
    BOOST_CHECK_CLOSE( cljff.energy(cljff.components().lj()),
                       known_cartesian_ljnrg, 1e-6 );

    //compare to a second forcefield
    InterCLJFF cljff2(cartesian, long_cutoff);

    cljff.add(mols, cljff.parameters().coulomb() == "charges" and
                    cljff.parameters().lj() == "ljs" );

    BOOST_CHECK_CLOSE( nrg, cljff2.energy(), 1e-6 );
    BOOST_CHECK_CLOSE( cljff.energy(cljff.components().coulomb()),
                       cljff2.energy(cljff2.components().coulomb()), 1e-6 );
    BOOST_CHECK_CLOSE( cljff.energy(cljff.components().lj()),
                       cljff2.energy(cljff2.components().lj()), 1e-6 );

    //lets try using a short cutoff
    cljff.setProperty( "switching function", short_cutoff );

    t.start();

    nrg = cljff.energy();

    ms = t.elapsed();

    double known_short_nrg = -14449.4979763;
    double known_short_cnrg = -17245.1723003;
    double known_short_ljnrg = 2795.67432402;

    BOOST_CHECK_CLOSE( nrg, known_short_nrg, 1e-6 );
    BOOST_CHECK_CLOSE( cljff.energy(cljff.components().coulomb()),
                       known_short_cnrg, 1e-6 );
    BOOST_CHECK_CLOSE( cljff.energy(cljff.components().lj()),
                       known_short_ljnrg, 1e-6 );

    //compare to a second forcefield
    cljff2 = InterCLJFF(cartesian, short_cutoff);
    cljff.add(mols, cljff.parameters().coulomb() == "charges" and
                    cljff.parameters().lj() == "ljs" );

    BOOST_CHECK_CLOSE( nrg, cljff2.energy(), 1e-6 );
    BOOST_CHECK_CLOSE( cljff.energy(cljff.components().coulomb()),
                       cljff2.energy(cljff2.components().coulomb()), 1e-6 );
    BOOST_CHECK_CLOSE( cljff.energy(cljff.components().lj()),
                       cljff2.energy(cljff2.components().lj()), 1e-6 );

    qDebug() << "Calculating the energy (" << nrg << ") took" << ms << "ms";

    qDebug() << "Finished TestForceFields tests...";
}
