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

#include "SireFF/errors.h"

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
    try
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
    cljff.setName("CLJ");

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

    cljff2.add(mols, cljff.parameters().coulomb() == "charges" and
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

    double known_short_nrg = -13747.4563227;
    double known_short_cnrg = -16816.1191665;
    double known_short_ljnrg = 3068.66284384;

    BOOST_CHECK_CLOSE( nrg, known_short_nrg, 1e-6 );
    BOOST_CHECK_CLOSE( cljff.energy(cljff.components().coulomb()),
                       known_short_cnrg, 1e-6 );
    BOOST_CHECK_CLOSE( cljff.energy(cljff.components().lj()),
                       known_short_ljnrg, 1e-6 );

    //compare to a second forcefield
    cljff2 = InterCLJFF(cartesian, short_cutoff);
    cljff2.add(mols, cljff2.parameters().coulomb() == "charges" and
                     cljff2.parameters().lj() == "ljs" );

    BOOST_CHECK_CLOSE( nrg, cljff2.energy(), 1e-6 );
    BOOST_CHECK_CLOSE( cljff.energy(cljff.components().coulomb()),
                       cljff2.energy(cljff2.components().coulomb()), 1e-6 );
    BOOST_CHECK_CLOSE( cljff.energy(cljff.components().lj()),
                       cljff2.energy(cljff2.components().lj()), 1e-6 );

    qDebug() << "Calculating the energy (" << nrg << ") took" << ms << "ms";

    qDebug() << "Testing composite forcefields...";

    InterLJFF ljff(periodic_box, long_cutoff);
    InterCoulombFF coulff(periodic_box, long_cutoff);

    ljff.setName("LJ");
    coulff.setName("Coul");

    ljff.add(mols, ljff.parameters().lj() == "ljs");
    coulff.add(mols, coulff.parameters().coulomb() == "charges");

    cljff.setProperty("space", periodic_box);
    cljff.setProperty("switching function", long_cutoff);

    t.start();
    nrg = ljff.energy();
    ms = t.elapsed();

    qDebug() << "LJFF energy (" << nrg << ") took" << ms << "ms";

    t.start();
    nrg = coulff.energy();
    ms = t.elapsed();

    qDebug() << "CoulombFF energy (" << nrg << ") took" << ms << "ms";

    BOOST_CHECK_CLOSE( cljff.energy(cljff.components().coulomb()),
                       coulff.energy(), 1e-6 );

    BOOST_CHECK_CLOSE( cljff.energy(cljff.components().coulomb()),
                       coulff.energy(coulff.components().coulomb()), 1e-6 );

    BOOST_CHECK_CLOSE( cljff.energy(cljff.components().lj()),
                       ljff.energy(), 1e-6 );

    BOOST_CHECK_CLOSE( cljff.energy(cljff.components().lj()),
                       ljff.energy(ljff.components().lj()), 1e-6 );

    cljff.setProperty("space", cartesian);
    coulff.setProperty("space", cartesian);
    ljff.setProperty("space", cartesian);

    t.start();
    nrg = ljff.energy();
    ms = t.elapsed();

    qDebug() << "LJFF energy (" << nrg << ") took" << ms << "ms";

    t.start();
    nrg = coulff.energy();
    ms = t.elapsed();

    qDebug() << "CoulombFF energy (" << nrg << ") took" << ms << "ms";

    BOOST_CHECK_CLOSE( cljff.energy(cljff.components().coulomb()),
                       coulff.energy(), 1e-6 );

    BOOST_CHECK_CLOSE( cljff.energy(cljff.components().coulomb()),
                       coulff.energy(coulff.components().coulomb()), 1e-6 );

    BOOST_CHECK_CLOSE( cljff.energy(cljff.components().lj()),
                       ljff.energy(), 1e-6 );

    BOOST_CHECK_CLOSE( cljff.energy(cljff.components().lj()),
                       ljff.energy(ljff.components().lj()), 1e-6 );

    cljff.setProperty("switching function", short_cutoff);
    coulff.setProperty("switching function", short_cutoff);
    ljff.setProperty("switching function", short_cutoff);

    t.start();
    nrg = ljff.energy();
    ms = t.elapsed();

    qDebug() << "LJFF energy (" << nrg << ") took" << ms << "ms";

    t.start();
    nrg = coulff.energy();
    ms = t.elapsed();

    qDebug() << "CoulombFF energy (" << nrg << ") took" << ms << "ms";

    BOOST_CHECK_CLOSE( cljff.energy(cljff.components().coulomb()),
                       coulff.energy(), 1e-6 );

    BOOST_CHECK_CLOSE( cljff.energy(cljff.components().coulomb()),
                       coulff.energy(coulff.components().coulomb()), 1e-6 );

    BOOST_CHECK_CLOSE( cljff.energy(cljff.components().lj()),
                       ljff.energy(), 1e-6 );

    BOOST_CHECK_CLOSE( cljff.energy(cljff.components().lj()),
                       ljff.energy(ljff.components().lj()), 1e-6 );

    qDebug() << "Testing group forcefields...";

    InterGroupCLJFF cljff_a_b(periodic_box, long_cutoff);
    InterGroupLJFF ljff_a_b(periodic_box, long_cutoff);
    InterGroupCoulombFF coulff_a_b(periodic_box, long_cutoff);

    cljff_a_b.setName("CLJ{A,B}");
    ljff_a_b.setName("LJ{A,B}");
    coulff_a_b.setName("Coul{A,B}");

    //divide the molecules evenly between both groups...
    QList<Molecule> a_mols = mols.mid(0, mols.count() / 2);
    QList<Molecule> b_mols = mols.mid(mols.count() / 2);

    cljff_a_b.addToA(a_mols, cljff_a_b.parameters().coulomb() == "charges" and
                             cljff_a_b.parameters().lj() == "ljs");
    cljff_a_b.addToB(b_mols, cljff_a_b.parameters().coulomb() == "charges" and
                             cljff_a_b.parameters().lj() == "ljs");

    coulff_a_b.addToA(a_mols, coulff_a_b.parameters().coulomb() == "charges");
    coulff_a_b.addToB(b_mols, coulff_a_b.parameters().coulomb() == "charges");

    ljff_a_b.addToA(a_mols, ljff_a_b.parameters().lj() == "ljs");
    ljff_a_b.addToB(b_mols, ljff_a_b.parameters().lj() == "ljs");

    t.start();
    nrg = cljff_a_b.energy();
    ms = t.elapsed();

    qDebug() << "CLJFF energy (" << nrg << ") took" << ms << "ms";

    t.start();
    nrg = coulff_a_b.energy();
    ms = t.elapsed();

    qDebug() << "CoulFF energy (" << nrg << ") took" << ms << "ms";

    t.start();
    nrg = ljff_a_b.energy();
    ms = t.elapsed();

    qDebug() << "LJFF energy (" << nrg << ") took" << ms << "ms";

    BOOST_CHECK_CLOSE( cljff_a_b.energy( cljff_a_b.components().coulomb() ),
                       coulff_a_b.energy(), 1e-6 );
    BOOST_CHECK_CLOSE( cljff_a_b.energy( cljff_a_b.components().lj() ),
                       ljff_a_b.energy(), 1e-6 );

    InterCLJFF cljff_a(periodic_box, long_cutoff);
    InterCLJFF cljff_b(periodic_box, long_cutoff);

    cljff_a.setName("CLJ{A}");
    cljff_b.setName("CLJ{B}");

    cljff_a.add(a_mols, cljff_a.parameters().coulomb() == "charges" and
                        cljff_a.parameters().lj() == "ljs");
    cljff_b.add(b_mols, cljff_b.parameters().coulomb() == "charges" and
                        cljff_b.parameters().lj() == "ljs");

    InterCoulombFF coulff_a(periodic_box, long_cutoff);
    InterCoulombFF coulff_b(periodic_box, long_cutoff);

    coulff_a.setName("Coul{A}");
    coulff_b.setName("COul{B}");

    coulff_a.add(a_mols, coulff_a.parameters().coulomb() == "charges" );
    coulff_b.add(b_mols, coulff_b.parameters().coulomb() == "charges" );

    InterLJFF ljff_a(periodic_box, long_cutoff);
    InterLJFF ljff_b(periodic_box, long_cutoff);

    ljff_a.setName("LJ{A}");
    ljff_b.setName("LJ{B}");

    ljff_a.add(a_mols, ljff_a.parameters().lj() == "ljs" );
    ljff_b.add(b_mols, ljff_b.parameters().lj() == "ljs" );

    qDebug() << "Checking the sum of group energies...";

    cljff.setProperty( "space", periodic_box );
    cljff.setProperty( "switching function", long_cutoff );

    coulff.setProperty( "space", periodic_box );
    coulff.setProperty( "switching function", long_cutoff );

    ljff.setProperty( "space", periodic_box );
    ljff.setProperty( "switching function", long_cutoff );

    t.start();
    nrg = cljff_a_b.energy() + cljff_a.energy() + cljff_b.energy();
    ms = t.elapsed();

    BOOST_CHECK_CLOSE( nrg, cljff.energy(), 1e-6 );

    qDebug() << "CLJFF energy (" << nrg << ") took" << ms << "ms";

    t.start();
    nrg = coulff_a_b.energy() + coulff_a.energy() + coulff_b.energy();
    ms = t.elapsed();

    BOOST_CHECK_CLOSE( nrg, coulff.energy(), 1e-6 );

    qDebug() << "CoulFF energy (" << nrg << ") took" << ms << "ms";

    t.start();
    nrg = ljff_a_b.energy() + ljff_a.energy() + ljff_b.energy();
    ms = t.elapsed();

    BOOST_CHECK_CLOSE( nrg, ljff.energy(), 1e-6 );

    qDebug() << "LJFF energy (" << nrg << ") took" << ms << "ms";

    cljff.setProperty( "space", cartesian );
    cljff_a.setProperty( "space", cartesian );
    cljff_b.setProperty( "space", cartesian );
    cljff_a_b.setProperty( "space", cartesian );

    coulff.setProperty( "space", cartesian );
    coulff_a.setProperty( "space", cartesian );
    coulff_b.setProperty( "space", cartesian );
    coulff_a_b.setProperty( "space", cartesian );

    ljff.setProperty( "space", cartesian );
    ljff_a.setProperty( "space", cartesian );
    ljff_b.setProperty( "space", cartesian );
    ljff_a_b.setProperty( "space", cartesian );

    t.start();
    nrg = cljff_a_b.energy() + cljff_a.energy() + cljff_b.energy();
    ms = t.elapsed();

    BOOST_CHECK_CLOSE( nrg, cljff.energy(), 1e-6 );

    qDebug() << "CLJFF energy (" << nrg << ") took" << ms << "ms";

    t.start();
    nrg = coulff_a_b.energy() + coulff_a.energy() + coulff_b.energy();
    ms = t.elapsed();

    BOOST_CHECK_CLOSE( nrg, coulff.energy(), 1e-6 );

    qDebug() << "CoulFF energy (" << nrg << ") took" << ms << "ms";

    t.start();
    nrg = ljff_a_b.energy() + ljff_a.energy() + ljff_b.energy();
    ms = t.elapsed();

    BOOST_CHECK_CLOSE( nrg, ljff.energy(), 1e-6 );

    qDebug() << "LJFF energy (" << nrg << ") took" << ms << "ms";

    cljff.setProperty( "switching function", short_cutoff );
    cljff_a.setProperty( "switching function", short_cutoff );
    cljff_b.setProperty( "switching function", short_cutoff );
    cljff_a_b.setProperty( "switching function", short_cutoff );

    coulff.setProperty( "switching function", short_cutoff );
    coulff_a.setProperty( "switching function", short_cutoff );
    coulff_b.setProperty( "switching function", short_cutoff );
    coulff_a_b.setProperty( "switching function", short_cutoff );

    ljff.setProperty( "switching function", short_cutoff );
    ljff_a.setProperty( "switching function", short_cutoff );
    ljff_b.setProperty( "switching function", short_cutoff );
    ljff_a_b.setProperty( "switching function", short_cutoff );

    t.start();
    nrg = cljff_a_b.energy() + cljff_a.energy() + cljff_b.energy();
    ms = t.elapsed();

    BOOST_CHECK_CLOSE( nrg, cljff.energy(), 1e-6 );

    qDebug() << "CLJFF energy (" << nrg << ") took" << ms << "ms";

    t.start();
    nrg = coulff_a_b.energy() + coulff_a.energy() + coulff_b.energy();
    ms = t.elapsed();

    BOOST_CHECK_CLOSE( nrg, coulff.energy(), 1e-6 );

    qDebug() << "CoulFF energy (" << nrg << ") took" << ms << "ms";

    t.start();
    nrg = ljff_a_b.energy() + ljff_a.energy() + ljff_b.energy();
    ms = t.elapsed();

    BOOST_CHECK_CLOSE( nrg, ljff.energy(), 1e-6 );

    qDebug() << "LJFF energy (" << nrg << ") took" << ms << "ms";

    qDebug() << "Testing the ForceFields class...";

    ForceFields ffields;

    BOOST_CHECK_EQUAL( ffields.energy(), 0 );
    BOOST_CHECK_EQUAL( ffields.forceFields().count(), 0 );
    BOOST_CHECK_EQUAL( ffields.forceFieldIDs().count(), 0 );

    BOOST_CHECK( ffields.add(cljff) );

    BOOST_CHECK_EQUAL( ffields.energy(), cljff.energy() );
    BOOST_CHECK_EQUAL( ffields.forceFields().count(), 1 );
    BOOST_CHECK( ffields.forceFields().contains(cljff.ID()) );
    BOOST_CHECK_EQUAL( ffields.forceFieldIDs().count(), 1 );
    BOOST_CHECK( ffields.forceFieldIDs().contains(cljff.ID()) );

    BOOST_CHECK( ffields.add(ljff) );

    BOOST_CHECK_EQUAL( ffields.energy(), cljff.energy() + ljff.energy()  );
    BOOST_CHECK_EQUAL( ffields.forceFields().count(), 2 );
    BOOST_CHECK( ffields.forceFields().contains(ljff.ID()) );
    BOOST_CHECK_EQUAL( ffields.forceFieldIDs().count(), 2 );
    BOOST_CHECK( ffields.forceFieldIDs().contains(ljff.ID()) );

    BOOST_CHECK( ffields.add(coulff) );

    BOOST_CHECK_EQUAL( ffields.energy(), cljff.energy() + ljff.energy() +
                                         coulff.energy()  );
    BOOST_CHECK_EQUAL( ffields.forceFields().count(), 3 );
    BOOST_CHECK( ffields.forceFields().contains(coulff.ID()) );
    BOOST_CHECK_EQUAL( ffields.forceFieldIDs().count(), 3 );
    BOOST_CHECK( ffields.forceFieldIDs().contains(coulff.ID()) );

    BOOST_CHECK( ffields.add(ljff_a_b) );

    BOOST_CHECK_EQUAL( ffields.energy(), cljff.energy() + ljff.energy() +
                                         coulff.energy() + ljff_a_b.energy()  );
    BOOST_CHECK_EQUAL( ffields.forceFields().count(), 4 );
    BOOST_CHECK( ffields.forceFields().contains(ljff_a_b.ID()) );
    BOOST_CHECK_EQUAL( ffields.forceFieldIDs().count(), 4 );
    BOOST_CHECK( ffields.forceFieldIDs().contains(ljff_a_b.ID()) );

    qDebug() << "Testing forcefield expressions...";

    BOOST_CHECK_CLOSE( ffields.energy( cljff.components().coulomb() +
                                       ljff_a_b.components().total() ),
                       cljff.energy(cljff.components().coulomb()) + ljff_a_b.energy(),
                       1e-6 );

    Symbol lambda("lambda");

    FFExpression ffexp( "T1",
                        cljff.components().coulomb() +
                        lambda * ljff_a_b.components().total() );

    ffields.add(ffexp);
    ffields.setParameter(lambda, 0.5);

    qDebug() << ffexp.toString();

    Values vals;
    vals.add( lambda == 0.5 );

    vals += cljff.energies();
    vals += ljff_a_b.energies();

    BOOST_CHECK_CLOSE( ffexp.evaluate(vals),
                       cljff.energy(cljff.components().coulomb()) +
                       0.5 * ljff_a_b.energy(ljff_a_b.components().total()),
                       1e-6 );

    BOOST_CHECK_CLOSE( ffexp.evaluate(vals), ffields.energy(ffexp.function()), 1e-6 );

    vals.add( lambda == 0.3 );
    ffields.setParameter(lambda, 0.3);

    BOOST_CHECK_CLOSE( ffexp.evaluate(vals), ffields.energy(ffexp.function()), 1e-6 );

    ffields.setTotal( ffexp );

    BOOST_CHECK_CLOSE( ffexp.evaluate(vals),
                       cljff.energy(cljff.components().coulomb()) +
                       0.3 * ljff_a_b.energy(ljff_a_b.components().total()),
                       1e-6 );

    BOOST_CHECK_CLOSE( ffexp.evaluate(vals), ffields.energy(), 1e-6 );

    FFExpression ffexp2( "T2",
                         (lambda*ffexp.function()) + cljff_a_b.components().coulomb()
                           - 5 * ljff_b.components().lj() );

    BOOST_CHECK( ffields.add(cljff_a_b) );
    BOOST_CHECK( ffields.add(ljff_b) );

    vals = ffields.energies();

    qDebug() << "All forcefield values...";
    for (QHash<SymbolID,double>::const_iterator it = vals.values().constBegin();
         it != vals.values().constEnd();
         ++it)
    {
        qDebug() << Symbol(it.key()).toString() << "==" << *it;
    }

    BOOST_CHECK_CLOSE( vals.value(ffexp.function()),
                       cljff.energy(cljff.components().coulomb()) +
                       0.3 * ljff_a_b.energy(ljff_a_b.components().total()),
                       1e-6 );

    qDebug() << ffexp2.toString();

    try
    {
        ffields.add(ffexp2);
    }
    catch(const SireFF::missing_forcefield&)
    {
        qDebug() << "Caught expected exception caused by missing"
                 << "forcefields in function!";
    }

    BOOST_CHECK_CLOSE( ffexp2.evaluate(vals),
                       (0.3 * (cljff.energy(cljff.components().coulomb()) +
                               0.3 * ljff_a_b.energy(ljff_a_b.components().total()))
                       ) + cljff_a_b.energy(cljff_a_b.components().coulomb())
                         - 5 * ljff_b.energy(ljff_b.components().lj()),
                       1e-6 );

    BOOST_CHECK_CLOSE( ffields.energy(ffexp2), ffexp2.evaluate(vals), 1e-6 );



    qDebug() << "Finished TestForceFields tests...";

    }
    catch(const SireError::exception &error)
    {
        qDebug() << error.toString();
        throw;
    }
}
