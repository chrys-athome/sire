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

#ifdef _BUILD_MOLPRO_

#include <QDebug>
#include <QTime>

#include "testmolproff.h"

#include "Squire/molproff.h"

#include "SireFF/forcefield.h"
#include "SireFF/forcefields.h"

#include "SireMM/intergroupcljff.h"
#include "SireMM/intercljff.h"
#include "SireMM/intergroupcoulombff.h"
#include "SireMM/chargeparameter.h"
#include "SireMM/ljparameter.h"
#include "SireMM/switchingfunction.h"

#include "SireMol/partialmolecule.h"
#include "SireMol/moleculeinfo.h"
#include "SireMol/molecule.h"
#include "SireMol/molecules.h"
#include "SireMol/propertyextractor.h"
#include "SireMol/moleculemover.h"
#include "SireMol/moleculegroup.h"

#include "SireMove/rigidbodymc.h"
#include "SireMove/uniformsampler.h"

#include "SireSystem/system.h"
#include "SireSystem/moves.h"

#include "SireVol/space.h"
#include "SireVol/periodicbox.h"

#include "SireIO/pdb.h"

#include "SireBase/findexe.h"

#include "SireUnits/units.h"

using namespace Squire;
using namespace SireTest;
using namespace SireIO;
using namespace SireVol;
using namespace SireMol;
using namespace SireUnits;
using namespace SireFF;
using namespace SireMM;
using namespace SireMaths;
using namespace SireBase;
using namespace SireSystem;
using namespace SireMove;
using namespace SireUnits;

TestMolproFF::TestMolproFF() : TestBase()
{}

TestMolproFF::~TestMolproFF()
{}

void TestMolproFF::initialise(test_suite *test)
{
    test->add( BOOST_TEST_CASE( &TestMolproFF::runTests ) );
}

void TestMolproFF::runTests()
{
    qDebug() << "Running TestMolproFF tests...";

    try
    {

    qDebug() << "Need new Molpro tests!!!";

    return;

    QTime t;

    //molpro *must* be in the user's path or else we can't run any
    //of these tests...

    //load the molecules
    QList<Molecule> mols = PDB().read(":water.pdb");
    BOOST_CHECK_EQUAL( mols.count(), 1679 );

    PartialMolecule tip4p = PDB().read(":tip4p.pdb").at(0);
    BOOST_CHECK_EQUAL( tip4p.info().nAtoms(), 4 );

    //create the space and switching function
    Space space = PeriodicBox( Vector(-18.3854,-18.66855,-18.4445),
                               Vector( 18.3854, 18.66855, 18.4445) );

    SwitchingFunction switchfunc = HarmonicSwitchingFunction(15.0, 14.5);

    //create a Molpro forcefield
    MolproFF molproff(space, switchfunc);
    molproff.setMolproExe( findExe("molpro") );

    BOOST_CHECK( molproff.getProperty("space") == space );
    BOOST_CHECK( molproff.getProperty("switching function") == switchfunc );

    //add TIP4P to the QM region
    molproff.addToQM(tip4p);

    BOOST_CHECK( molproff.contains(tip4p) );
    BOOST_CHECK( molproff.contains(tip4p, molproff.groups().qm()) );

    //calculate the gas-phase energy of the tip4p water
    t.start();
    double nrg = molproff.energy();
    int ms = t.elapsed();

    qDebug() << "Tip4P energy ==" << nrg << "kcal mol-1, took" << ms << "ms";

    BOOST_CHECK_CLOSE( nrg, molproff.energy(), 1e-6 );
    BOOST_CHECK_CLOSE( nrg, molproff.energy( molproff.components().qm() ), 1e-6 );

    BOOST_CHECK_CLOSE( nrg, -47707.4, 1e-4 );

    //set the TIP4P energy as the zero energy
    molproff.setEnergyOrigin(nrg);

    BOOST_CHECK_CLOSE( molproff.energy(), 0.0, 1e-6 );

    //add the one of the solvent molecules to the MM region - need to parametise
    //them all though...
    //create parameters for all of the molecules...
    QVector<ChargeParameter> group_chgs;

    group_chgs.append( ChargeParameter(0) );                     // Oxygen
    group_chgs.append( ChargeParameter(0.52 * mod_electron) );  // Hydrogen
    group_chgs.append( ChargeParameter(0.52 * mod_electron) );  // Hydrogen
    group_chgs.append( ChargeParameter(-1.04 * mod_electron) ); // M atom

    AtomicCharges chgs(group_chgs);

    QVector<LJParameter> group_ljs;

    //Oxygen is the only atom with an LJ parameter
    group_ljs.append( LJParameter(3.15365 * angstrom, 0.1550 * kcal_per_mol) );

    for (int i=0; i<3; ++i)
        group_ljs.append( LJParameter() );  // remaining atoms

    AtomicLJs ljs(group_ljs);

    Molecule mol_tip4p(tip4p);
    mol_tip4p.setProperty("charges", chgs);
    mol_tip4p.setProperty("ljs", ljs);

    tip4p = mol_tip4p;

    for (QList<Molecule>::iterator it = mols.begin();
         it != mols.end();
         ++it)
    {
        it->setProperty("charges", chgs);
        it->setProperty("ljs", ljs);
    }

    PartialMolecule other_tip4p = mols[0];

    //translate this so that it is close to the QM tip4p
    Vector diff = tip4p.extract().geometricCenter() -
                  other_tip4p.extract().geometricCenter();

    other_tip4p = other_tip4p.move().translate( diff + Vector(3,0,0) );

    diff = tip4p.extract().geometricCenter() -
           other_tip4p.extract().geometricCenter();

    //molecules should now be within 3 A of one another
    BOOST_CHECK_CLOSE( diff.length(), 3.0, 1e-4 );

    molproff.addToMM(other_tip4p, molproff.parameters().coulomb() == "charges");

    t.start();
    nrg = molproff.energy();
    ms = t.elapsed();

    qDebug() << "QM/MM energy ==" << nrg << "kcal mol-1, took" << ms << "ms";

    BOOST_CHECK( nrg < 0 );
    BOOST_CHECK_CLOSE( nrg, -1.68629, 1e-3 );

    //compare this energy to the coulomb interaction between these molecules
    InterGroupCoulombFF coulff(space, switchfunc);

    coulff.addToA(tip4p, coulff.parameters().coulomb() == "charges");
    coulff.addToB(other_tip4p, coulff.parameters().coulomb() == "charges");

    t.start();
    double cnrg = coulff.energy();
    ms = t.elapsed();

    BOOST_CHECK_CLOSE( cnrg, -1.65823, 1e-3 );

    qDebug() << "MM energy ==" << cnrg << "kcal mol-1, took" << ms << "ms";

    //check that the MM and QM energies are within 10% of one another
    BOOST_CHECK_CLOSE( nrg, cnrg, 10.0 );

    //now move the other water so that it is 10 A from tip4p
    diff = tip4p.extract().geometricCenter() -
           other_tip4p.extract().geometricCenter();

    other_tip4p = other_tip4p.move().translate( diff + Vector(10,0,0) );

    diff = tip4p.extract().geometricCenter() -
           other_tip4p.extract().geometricCenter();

    //molecules should now be within 3 A of one another
    BOOST_CHECK_CLOSE( diff.length(), 10.0, 1e-4 );

    molproff.change(other_tip4p);
    coulff.change(other_tip4p);

    t.start();
    nrg = molproff.energy();
    ms = t.elapsed();

    qDebug() << "QM/MM energy ==" << nrg << "kcal mol-1, took" << ms << "ms";

    t.start();
    cnrg = coulff.energy();
    ms = t.elapsed();

    qDebug() << "MM energy ==" << cnrg << "kcal mol-1, took" << ms << "ms";

    //check that the MM and QM energies are within 10% of one another
    BOOST_CHECK_CLOSE( nrg, cnrg, 10.0 );

    //now translate the water outside the cutoff
    diff = tip4p.extract().geometricCenter() -
           other_tip4p.extract().geometricCenter();

    other_tip4p = other_tip4p.move().translate( diff + Vector(18,0,0) );

    diff = tip4p.extract().geometricCenter() -
           other_tip4p.extract().geometricCenter();

    //molecules should now be within 3 A of one another
    BOOST_CHECK_CLOSE( diff.length(), 18.0, 1e-4 );

    molproff.change(other_tip4p);
    coulff.change(other_tip4p);

    t.start();
    nrg = molproff.energy();
    ms = t.elapsed();

    qDebug() << "QM/MM energy ==" << nrg << "kcal mol-1, took" << ms << "ms";

    t.start();
    cnrg = coulff.energy();
    ms = t.elapsed();

    qDebug() << "MM energy ==" << cnrg << "kcal mol-1, took" << ms << "ms";

    //check that the MM and QM energies are both now zero (well, near zero, given
    //numerical imprecision)
    BOOST_CHECK( std::abs(nrg) < 1e-10 );
    BOOST_CHECK( std::abs(cnrg) < 1e-10 );

    //now back to 3 A distance...
    diff = tip4p.extract().geometricCenter() -
           other_tip4p.extract().geometricCenter();

    other_tip4p = other_tip4p.move().translate( diff + Vector(3,0,0) );

    diff = tip4p.extract().geometricCenter() -
           other_tip4p.extract().geometricCenter();

    //molecules should now be within 3 A of one another
    BOOST_CHECK_CLOSE( diff.length(), 3.0, 1e-4 );

    molproff.change(other_tip4p);
    coulff.change(other_tip4p);

    t.start();
    nrg = molproff.energy();
    ms = t.elapsed();

    qDebug() << "QM/MM energy ==" << nrg << "kcal mol-1, took" << ms << "ms";

    t.start();
    cnrg = coulff.energy();
    ms = t.elapsed();

    qDebug() << "MM energy ==" << cnrg << "kcal mol-1, took" << ms << "ms";

    //check that the MM and QM energies are within 10% of one another
    BOOST_CHECK_CLOSE( nrg, cnrg, 10.0 );

    //now try multiple translations - move outside the sphere, then back in again
    double oldnrg = nrg;
    double oldcnrg = cnrg;

    diff = tip4p.extract().geometricCenter() -
           other_tip4p.extract().geometricCenter();

    other_tip4p = other_tip4p.move().translate( diff + Vector(18,0,0) );

    diff = tip4p.extract().geometricCenter() -
           other_tip4p.extract().geometricCenter();

    //molecules should now be within 3 A of one another
    BOOST_CHECK_CLOSE( diff.length(), 18.0, 1e-4 );

    molproff.change(other_tip4p);
    coulff.change(other_tip4p);

    other_tip4p = other_tip4p.move().translate( diff + Vector(3,0,0) );

    diff = tip4p.extract().geometricCenter() -
           other_tip4p.extract().geometricCenter();

    //molecules should now be within 3 A of one another
    BOOST_CHECK_CLOSE( diff.length(), 3.0, 1e-4 );

    molproff.change(other_tip4p);
    coulff.change(other_tip4p);
    t.start();
    nrg = molproff.energy();
    ms = t.elapsed();

    qDebug() << "QM/MM energy ==" << nrg << "kcal mol-1, took" << ms << "ms";

    t.start();
    cnrg = coulff.energy();
    ms = t.elapsed();

    qDebug() << "MM energy ==" << cnrg << "kcal mol-1, took" << ms << "ms";

    //check that the MM and QM energies are within 10% of one another
    BOOST_CHECK_CLOSE( nrg, cnrg, 10.0 );

    BOOST_CHECK_CLOSE( nrg, oldnrg, 1e-6 );
    BOOST_CHECK_CLOSE( cnrg, oldcnrg, 1e-6 );

    //ok, lets add another MM molecule...
    PartialMolecule another_tip4p = mols[2];

    //translate it so it is within 8 A of the water
    diff = tip4p.extract().geometricCenter() -
           another_tip4p.extract().geometricCenter();

    another_tip4p = another_tip4p.move().translate( diff + Vector(8,0,0) );

    diff = tip4p.extract().geometricCenter() -
           another_tip4p.extract().geometricCenter();

    //molecules should now be within 3 A of one another
    BOOST_CHECK_CLOSE( diff.length(), 8.0, 1e-4 );

    molproff.addToMM( another_tip4p, molproff.parameters().coulomb() == "charges" );
    coulff.addToB( another_tip4p, coulff.parameters().coulomb() == "charges" );

    t.start();
    nrg = molproff.energy();
    ms = t.elapsed();

    qDebug() << "QM/MM energy ==" << nrg << "kcal mol-1, took" << ms << "ms";

    t.start();
    cnrg = coulff.energy();
    ms = t.elapsed();

    qDebug() << "MM energy ==" << cnrg << "kcal mol-1, took" << ms << "ms";

    BOOST_CHECK_CLOSE( nrg, cnrg, 10.0 );

    //now translate the water outside the cutoff
    diff = tip4p.extract().geometricCenter() -
           other_tip4p.extract().geometricCenter();

    other_tip4p = other_tip4p.move().translate( diff + Vector(18,0,0) );

    diff = tip4p.extract().geometricCenter() -
           other_tip4p.extract().geometricCenter();

    //molecules should now be within 3 A of one another
    BOOST_CHECK_CLOSE( diff.length(), 18.0, 1e-4 );

    molproff.change(other_tip4p);
    coulff.change(other_tip4p);

    t.start();
    nrg = molproff.energy();
    ms = t.elapsed();

    qDebug() << "QM/MM energy ==" << nrg << "kcal mol-1, took" << ms << "ms";

    t.start();
    cnrg = coulff.energy();
    ms = t.elapsed();

    qDebug() << "MM energy ==" << cnrg << "kcal mol-1, took" << ms << "ms";

    //check that the MM and QM energies are within 10% of one another
    BOOST_CHECK_CLOSE( nrg, cnrg, 10.0 );

    //now back to 3 A distance...
    diff = tip4p.extract().geometricCenter() -
           other_tip4p.extract().geometricCenter();

    other_tip4p = other_tip4p.move().translate( diff + Vector(3,0,0) );

    diff = tip4p.extract().geometricCenter() -
           other_tip4p.extract().geometricCenter();

    //molecules should now be within 3 A of one another
    BOOST_CHECK_CLOSE( diff.length(), 3.0, 1e-4 );

    molproff.change(other_tip4p);
    coulff.change(other_tip4p);

    t.start();
    nrg = molproff.energy();
    ms = t.elapsed();

    qDebug() << "QM/MM energy ==" << nrg << "kcal mol-1, took" << ms << "ms";

    t.start();
    cnrg = coulff.energy();
    ms = t.elapsed();

    qDebug() << "MM energy ==" << cnrg << "kcal mol-1, took" << ms << "ms";

    //check that the MM and QM energies are within 10% of one another
    BOOST_CHECK_CLOSE( nrg, cnrg, 10.0 );

    //now try multiple movements...
    oldnrg = nrg;
    oldcnrg = cnrg;

    diff = tip4p.extract().geometricCenter() -
           other_tip4p.extract().geometricCenter();

    other_tip4p = other_tip4p.move().translate( diff + Vector(18,0,0) );

    diff = tip4p.extract().geometricCenter() -
           other_tip4p.extract().geometricCenter();

    //molecules should now be within 3 A of one another
    BOOST_CHECK_CLOSE( diff.length(), 18.0, 1e-4 );

    molproff.change(other_tip4p);
    coulff.change(other_tip4p);

    other_tip4p = other_tip4p.move().translate( diff + Vector(3,0,0) );

    diff = tip4p.extract().geometricCenter() -
           other_tip4p.extract().geometricCenter();

    //molecules should now be within 3 A of one another
    BOOST_CHECK_CLOSE( diff.length(), 3.0, 1e-4 );

    molproff.change(other_tip4p);
    coulff.change(other_tip4p);
    t.start();
    nrg = molproff.energy();
    ms = t.elapsed();

    qDebug() << "QM/MM energy ==" << nrg << "kcal mol-1, took" << ms << "ms";

    t.start();
    cnrg = coulff.energy();
    ms = t.elapsed();

    qDebug() << "MM energy ==" << cnrg << "kcal mol-1, took" << ms << "ms";

    //check that the MM and QM energies are within 10% of one another
    BOOST_CHECK_CLOSE( nrg, cnrg, 10.0 );

    BOOST_CHECK_CLOSE( nrg, oldnrg, 1e-6 );
    BOOST_CHECK_CLOSE( cnrg, oldcnrg, 1e-6 );

    //move the molecule outside the array...
    diff = tip4p.extract().geometricCenter() -
           other_tip4p.extract().geometricCenter();

    other_tip4p = other_tip4p.move().translate( diff + Vector(18,0,0) );

    diff = tip4p.extract().geometricCenter() -
           other_tip4p.extract().geometricCenter();

    //molecules should now be within 3 A of one another
    BOOST_CHECK_CLOSE( diff.length(), 18.0, 1e-4 );

    molproff.change(other_tip4p);
    coulff.change(other_tip4p);

    t.start();
    nrg = molproff.energy();
    ms = t.elapsed();

    qDebug() << "QM/MM energy ==" << nrg << "kcal mol-1, took" << ms << "ms";

    t.start();
    cnrg = coulff.energy();
    ms = t.elapsed();

    qDebug() << "MM energy ==" << cnrg << "kcal mol-1, took" << ms << "ms";

    BOOST_CHECK_CLOSE( nrg, cnrg, 10.0 );

    oldnrg = nrg;
    oldcnrg = cnrg;

    //now move it outside the array again
    diff = tip4p.extract().geometricCenter() -
           other_tip4p.extract().geometricCenter();

    other_tip4p = other_tip4p.move().translate( diff + Vector(17.8,0,0) );

    diff = tip4p.extract().geometricCenter() -
           other_tip4p.extract().geometricCenter();

    //molecules should now be within 3 A of one another
    BOOST_CHECK_CLOSE( diff.length(), 17.8, 1e-4 );

    molproff.change(other_tip4p);
    coulff.change(other_tip4p);

    diff = tip4p.extract().geometricCenter() -
           another_tip4p.extract().geometricCenter();

    another_tip4p = another_tip4p.move().translate( diff + Vector(18,0,0) );

    diff = tip4p.extract().geometricCenter() -
           another_tip4p.extract().geometricCenter();

    //molecules should now be within 3 A of one another
    BOOST_CHECK_CLOSE( diff.length(), 18.0, 1e-4 );

    molproff.change(another_tip4p);
    coulff.change(another_tip4p);

    diff = tip4p.extract().geometricCenter() -
           another_tip4p.extract().geometricCenter();

    another_tip4p = another_tip4p.move().translate( diff + Vector(8,0,0) );

    diff = tip4p.extract().geometricCenter() -
           another_tip4p.extract().geometricCenter();

    //molecules should now be within 3 A of one another
    BOOST_CHECK_CLOSE( diff.length(), 8.0, 1e-4 );

    molproff.change(another_tip4p);
    coulff.change(another_tip4p);

    t.start();
    nrg = molproff.energy();
    ms = t.elapsed();

    qDebug() << "QM/MM energy ==" << nrg << "kcal mol-1, took" << ms << "ms";

    t.start();
    cnrg = coulff.energy();
    ms = t.elapsed();

    qDebug() << "MM energy ==" << cnrg << "kcal mol-1, took" << ms << "ms";

    BOOST_CHECK_CLOSE( nrg, cnrg, 10.0 );

    BOOST_CHECK_CLOSE( nrg, oldnrg, 1e-6 );
    BOOST_CHECK_CLOSE( cnrg, oldcnrg, 1e-6 );

    //create a small simulation and ensure that the QM energy is updated correctly
    molproff.addToMM(mols, molproff.parameters().coulomb() == "charges");

    InterCLJFF intercljff(space, switchfunc);

    intercljff.add(mols, intercljff.parameters().coulomb() == "charges" and
                         intercljff.parameters().lj() == "ljs");

    intercljff.add(tip4p, intercljff.parameters().coulomb() == "charges" and
                          intercljff.parameters().lj() == "ljs");

    InterGroupCLJFF groupcljff(space, switchfunc);

    groupcljff.addToA(tip4p, groupcljff.parameters().coulomb() == "charges" and
                             groupcljff.parameters().lj() == "ljs");

    groupcljff.addToB(mols, groupcljff.parameters().coulomb() == "charges" and
                            groupcljff.parameters().lj() == "ljs");

    nrg = molproff.energy();
    cnrg = groupcljff.energy( groupcljff.components().coulomb() );

    //these energies should be within 25% of one another...
    BOOST_CHECK_CLOSE( nrg, cnrg, 25.0 );

    ForceFields ffields;
    ffields.add(intercljff);
    ffields.add(molproff);

    MoleculeGroup all_mols("all", mols);
    all_mols.add(tip4p);

    System system(all_mols, ffields);

    system.run( RigidBodyMC(UniformSampler(all_mols)), 500 );

    MolproFF sys_molproff = system.forceFields().forceField(molproff.ID()).asA<MolproFF>();

    double sys_nrg = sys_molproff.energy();

    molproff.change(system.forceFields().molecules());
    nrg = molproff.energy();

    BOOST_CHECK_CLOSE( nrg, sys_nrg, 1e-4 );

    //recalculate from scratch
    MolproFF molproff2(space, switchfunc);

    molproff2.setMolproExe(molproff.molproExe());
    molproff2.setEnergyOrigin(molproff.energyOrigin());

    molproff2.addToQM( molproff.qmMolecules() );
    molproff2.addToMM( molproff.mmMolecules(), molproff2.parameters().coulomb() == "charges" );

    double new_nrg = molproff2.energy();

    BOOST_CHECK_CLOSE( nrg, new_nrg, 1e-6 );

    }
    catch (const SireError::exception &e)
    {
        qDebug() << e.toString();
        throw;
    }

    qDebug() << "Finished TestMolproFF tests...";
}

#endif // end of '#ifdef _BUILD_MOLPRO_'
