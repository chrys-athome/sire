/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2009  Christopher Woods
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

#include "openmmfrenergyst.h"
#include "ensemble.h"

#include "SireMol/moleculegroup.h"
#include "SireMol/partialmolecule.h"
#include "SireMol/molecule.h"
#include "SireMol/atommasses.h"
#include "SireMol/atomcoords.h"
#include "SireMol/moleditor.h"

#include "SireMol/amberparameters.h"

#include "SireSystem/system.h"

#include "SireFF/forcetable.h"

#include "SireMaths/rangenerator.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include "SireUnits/units.h"
#include "SireUnits/temperature.h"
#include "SireUnits/convert.h"

// ADDED BY JM 
#include "SireMol/connectivity.h"
#include "SireMol/bondid.h"
#include "SireMol/atomcharges.h"
#include "SireMM/internalff.h"
#include "SireIO/amber.h"
#include "SireMM/atomljs.h"

#include "SireVol/periodicbox.h"

#include "SireMove/flexibility.h"

#include "SireMaths/constants.h"

//ADDED BY GAC
#include "SireMaths/vector.h"
#include "SireMol/mgname.h"
#include "SireMol/perturbation.h"
#include "SireMM/internalperturbation.h"
#include <iostream>
#include <QDebug>
#include <QTime>
#include "dcd_writer.h"


using namespace SireMove;
using namespace SireSystem;
using namespace SireMol;
using namespace SireFF;
using namespace SireCAS;
using namespace SireVol;
using namespace SireBase;
using namespace SireStream;
using namespace SireUnits;
using namespace SireUnits::Dimension;

//ADDED BY JM
using namespace SireMM;
using namespace SireIO;
//using namespace SireMaths;

//ADDED BY GAC
using namespace std;


enum {
    NOCUTOFF = 0,
    CUTOFFNONPERIODIC = 1,
    CUTOFFPERIODIC = 2
};

enum {

    NONE = 0,
    HBONDS = 1,
    ALLBONDS = 2,
    HANGLES = 3
    
};

static const RegisterMetaType<OpenMMFrEnergyST> r_openmmint;

namespace frenergyst{
    int show_status (double current, int total);
}

using namespace frenergyst;

/** Serialise to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, const OpenMMFrEnergyST &velver)
{
    writeHeader(ds, r_openmmint, 1);

    SharedDataStream sds(ds);

    sds << velver.frequent_save_velocities << velver.molgroup << velver.solute << velver.solutehard << velver.solutetodummy << velver.solutefromdummy
        << velver.CutoffType << velver.cutoff_distance << velver.field_dielectric
        << velver.Andersen_flag <<  velver.Andersen_frequency 
        << velver.MCBarostat_flag << velver.MCBarostat_frequency << velver.ConstraintType << velver.Pressure << velver.Temperature
        <<velver.platform_type << velver.Restraint_flag << velver.CMMremoval_frequency << velver.energy_frequency
        << velver.device_index <<velver.precision << velver.Alchemical_value << velver.coulomb_power << velver.shift_delta << velver.delta_alchemical << velver.buffer_coords
        << velver.gradients <<velver.perturbed_energies <<  velver.Integrator_type << velver.friction << velver.integration_tol << velver.timeskip
        << static_cast<const Integrator&>(velver);

    // Free OpenMM pointers??

    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, OpenMMFrEnergyST &velver)
{
    VersionID v = readHeader(ds, r_openmmint);

    if (v == 1){
        SharedDataStream sds(ds);

        sds >> velver.frequent_save_velocities >> velver.molgroup >> velver.solute >>velver.solutehard >> velver.solutetodummy >> velver.solutefromdummy
        >> velver.CutoffType >> velver.cutoff_distance >> velver.field_dielectric
        >> velver.Andersen_flag >>  velver.Andersen_frequency 
        >> velver.MCBarostat_flag >> velver.MCBarostat_frequency >> velver.ConstraintType >> velver.Pressure >> velver.Temperature
        >> velver.platform_type >> velver.Restraint_flag >> velver.CMMremoval_frequency >> velver.energy_frequency
        >> velver.device_index >> velver.precision >> velver.Alchemical_value >> velver.coulomb_power >> velver.shift_delta >> velver.delta_alchemical >> velver.buffer_coords
        >> velver.gradients >> velver.perturbed_energies >> velver.Integrator_type >> velver.friction >> velver.integration_tol >> velver.timeskip
        >> static_cast<Integrator&>(velver);

        // Maybe....need to reinitialise from molgroup because openmm system was not serialised...
        velver.isInitialised = false;
        
        qDebug() << " Re-initialisation of OpenMMFrEnergyST from datastream";

        velver.initialise();
    }
    else
        throw version_error(v, "1", r_openmmint, CODELOC);

    return ds;
}

/** Constructor*/
OpenMMFrEnergyST::OpenMMFrEnergyST(bool frequent_save) 
                : ConcreteProperty<OpenMMFrEnergyST,Integrator>(),
                frequent_save_velocities(frequent_save), 
                molgroup(MoleculeGroup()), solute(MoleculeGroup()) ,solutehard(MoleculeGroup()), solutetodummy(MoleculeGroup()), solutefromdummy(MoleculeGroup()), openmm_system(0), isInitialised(false), 
                CutoffType("nocutoff"), cutoff_distance(1.0 * nanometer),field_dielectric(78.3),
                Andersen_flag(false),Andersen_frequency(90.0), MCBarostat_flag(false),
                MCBarostat_frequency(25),ConstraintType("none"),
                Pressure(1.0 * bar),Temperature(300.0 * kelvin),platform_type("Reference"),Restraint_flag(false),
                CMMremoval_frequency(0), energy_frequency(100),device_index("0"), precision("single"), Alchemical_value(0.5),coulomb_power(0),
                shift_delta(2.0),delta_alchemical(0.001),buffer_coords(false),gradients(),perturbed_energies(),
                Integrator_type("leapfrogverlet"),friction(1.0 / picosecond ),integration_tol(0.001),timeskip(0.0 * picosecond)
{}

/** Constructor using the passed molecule groups */
OpenMMFrEnergyST::OpenMMFrEnergyST(const MoleculeGroup &molecule_group, const MoleculeGroup &solute_group,const MoleculeGroup &solute_hard, const MoleculeGroup &solute_todummy, const MoleculeGroup &solute_fromdummy,bool frequent_save) 
                : ConcreteProperty<OpenMMFrEnergyST,Integrator>(),
                frequent_save_velocities(frequent_save), 
                molgroup(molecule_group), solute(solute_group),solutehard(solute_hard), solutetodummy(solute_todummy),solutefromdummy(solute_fromdummy),openmm_system(0), isInitialised(false), 
                CutoffType("nocutoff"), cutoff_distance(1.0 * nanometer),field_dielectric(78.3),
                Andersen_flag(false),Andersen_frequency(90.0), MCBarostat_flag(false),
                MCBarostat_frequency(25),ConstraintType("none"),
                Pressure(1.0 * bar),Temperature(300.0 * kelvin),platform_type("Reference"),Restraint_flag(false),
                CMMremoval_frequency(0), energy_frequency(100),device_index("0"),precision("single"),Alchemical_value(0.5),coulomb_power(0),
                shift_delta(2.0),delta_alchemical(0.001),buffer_coords(false),gradients(),perturbed_energies(),
                Integrator_type("leapfrogverlet"),friction(1.0 / picosecond ),integration_tol(0.001),timeskip(0.0 * picosecond)
{}

/** Copy constructor */
OpenMMFrEnergyST::OpenMMFrEnergyST(const OpenMMFrEnergyST &other)
                : ConcreteProperty<OpenMMFrEnergyST,Integrator>(other),
                frequent_save_velocities(other.frequent_save_velocities),
                molgroup(other.molgroup), solute(other.solute), solutehard(other.solutehard), 
                solutetodummy(other.solutetodummy), solutefromdummy(other.solutefromdummy),
                openmm_system(other.openmm_system), isInitialised(other.isInitialised), 
                CutoffType(other.CutoffType),cutoff_distance(other.cutoff_distance),
                field_dielectric(other.field_dielectric), Andersen_flag(other.Andersen_flag),
                Andersen_frequency(other.Andersen_frequency), MCBarostat_flag(other.MCBarostat_flag),
                MCBarostat_frequency(other.MCBarostat_frequency),ConstraintType(other.ConstraintType), 
                Pressure(other.Pressure), Temperature(other.Temperature),platform_type(other.platform_type),
                Restraint_flag(other.Restraint_flag),CMMremoval_frequency(other.CMMremoval_frequency),
                energy_frequency(other.energy_frequency),device_index(other.device_index),precision(other.precision),Alchemical_value(other.Alchemical_value),
                coulomb_power(other.coulomb_power),shift_delta(other.shift_delta),
                delta_alchemical(other.delta_alchemical),buffer_coords(other.buffer_coords),gradients(other.gradients),perturbed_energies(other.perturbed_energies),
                Integrator_type(other.Integrator_type),friction(other.friction),integration_tol(other.integration_tol),timeskip(other.timeskip)
{}

/** Destructor */
OpenMMFrEnergyST::~OpenMMFrEnergyST()
{
    //delete openmm_system;
}

/** Copy assignment operator */
OpenMMFrEnergyST& OpenMMFrEnergyST::operator=(const OpenMMFrEnergyST &other)
{
    Integrator::operator=(other);
    frequent_save_velocities = other.frequent_save_velocities;
    molgroup = other.molgroup; 
    solute = other.solute;
    solutehard=other.solutehard;
    solutetodummy=other.solutetodummy;
    solutefromdummy=other.solutefromdummy;
    openmm_system = other.openmm_system;
    isInitialised = other.isInitialised;
    CutoffType = other.CutoffType;
    cutoff_distance = other.cutoff_distance;
    field_dielectric = other.field_dielectric;
    Andersen_flag = other.Andersen_flag;
    Andersen_frequency = other.Andersen_frequency;
    MCBarostat_flag = other.MCBarostat_flag;
    MCBarostat_frequency = other.MCBarostat_frequency;
    ConstraintType = other.ConstraintType;
    Pressure = other.Pressure;
    Temperature = other.Temperature;
    platform_type = other.platform_type;
    Restraint_flag = other.Restraint_flag;
    CMMremoval_frequency = other.CMMremoval_frequency;
    energy_frequency = other.energy_frequency;
    device_index = other.device_index;
    precision = other.precision;
    Alchemical_value = other.Alchemical_value;
    coulomb_power = other.coulomb_power;
    shift_delta = other.shift_delta;
    delta_alchemical = other.delta_alchemical;
    buffer_coords = other.buffer_coords;
    gradients = other.gradients;
    perturbed_energies = other.perturbed_energies;
    Integrator_type = other.Integrator_type;
    friction = other.friction;
    integration_tol = other.integration_tol;
    timeskip=other.timeskip;

    return *this;
}

/** Comparison operator */
bool OpenMMFrEnergyST::operator==(const OpenMMFrEnergyST &other) const
{
    return frequent_save_velocities == other.frequent_save_velocities and Integrator::operator==(other);
}

/** Comparison operator */
bool OpenMMFrEnergyST::operator!=(const OpenMMFrEnergyST &other) const
{
    return not OpenMMFrEnergyST::operator==(other);
}

/** Return a string representation of this integrator */
QString OpenMMFrEnergyST::toString() const
{
    return QObject::tr("OpenMMFrEnergyST()");
}



void OpenMMFrEnergyST::initialise()  {

    bool Debug = false;

    if (true){
        qDebug() << "Initialising OpenMMFrEnergyST";
        const std::string version = OpenMM::Platform::getOpenMMVersion();
        qDebug() << "OpenMM Version: " << QString::fromUtf8( version.data(), version.size() );
    }

    // Create a workspace using the stored molgroup

    const MoleculeGroup moleculegroup = this->molgroup.read();

    if ( moleculegroup.isEmpty() ){
        throw SireError::program_bug(QObject::tr("Cannot initialise OpenMMFrEnergyST because molgroup has not been defined"), CODELOC);
    }

    const MoleculeGroup solute = this->solute.read();
    
    if ( solute.isEmpty() ){
        throw SireError::program_bug(QObject::tr("Cannot initialise OpenMMFrEnergyST because solute group has not been defined"), CODELOC);
    }


    const MoleculeGroup solutehard = this->solutehard.read();
    
    const MoleculeGroup solutetodummy = this->solutetodummy.read();

    const MoleculeGroup solutefromdummy = this->solutefromdummy.read();

    AtomicVelocityWorkspace ws = this->createWorkspace(moleculegroup).read().asA<AtomicVelocityWorkspace>();

    const int nmols = ws.nMolecules();

    //const System & ptr_sys = ws.system();


    int nats = 0;

    for (int i=0; i<nmols; ++i){
        nats = nats + ws.nAtoms(i);
    }

    if (Debug)
        qDebug() << "There are " << nats << " atoms " << "There are " << nmols << " molecules" <<"\n" ;

    int flag_cutoff;
    int flag_constraint;

    if (CutoffType == "nocutoff")
        flag_cutoff = NOCUTOFF;
    else if (CutoffType == "cutoffnonperiodic")
        flag_cutoff = CUTOFFNONPERIODIC;
    else if (CutoffType == "cutoffperiodic")
        flag_cutoff = CUTOFFPERIODIC;
    else
        throw SireError::program_bug(QObject::tr("The CutOff method has not been specified. Possible choises: nocutoff, cutoffnonperiodic, cutoffperiodic"), CODELOC);

    if (Debug)
        qDebug() << "\nCutoffType = " << CutoffType << "\n";

    if (ConstraintType == "none")
        flag_constraint = NONE;
    else if (ConstraintType == "hbonds")
        flag_constraint = HBONDS;
    else if(ConstraintType == "allbonds")
        flag_constraint = ALLBONDS;
    else if (ConstraintType == "hangles")
        flag_constraint = HANGLES;
    else
        throw SireError::program_bug(QObject::tr("The Constraints method has not been specified. Possible choises: none, hbonds, allbonds, hangles"), CODELOC);

    if (Debug)
        qDebug() << "\nConstraint Type = " << ConstraintType<< "\n";

    //Load Plugins from the OpenMM standard Plugin Directory 
    OpenMM::Platform::loadPluginsFromDirectory(OpenMM::Platform::getDefaultPluginsDirectory());

    OpenMM::System * system_openmm = new OpenMM::System();


    //The Standard Non Bonded is only defined to extract 1-2,1-3,1-4 pairs from the system 
    OpenMM::NonbondedForce * nonbond_openmm = new OpenMM::NonbondedForce();

    nonbond_openmm->setUseDispersionCorrection(false);

    //CUSTOM NON BONDED FORCE FIELD

    OpenMM::CustomNonbondedForce * custom_force_field = NULL;
    
    //1-4 interactions
    OpenMM::CustomBondForce * custom_intra_14_clj = NULL;
    OpenMM::CustomBondForce * custom_intra_14_todummy = NULL;
    OpenMM::CustomBondForce * custom_intra_14_fromdummy = NULL;
    OpenMM::CustomBondForce * custom_intra_14_fromdummy_todummy = NULL;
    

    if ( flag_cutoff == NOCUTOFF ){

/*        custom_force_field = new OpenMM::CustomNonbondedForce("Hsoft * (1.0 - isHARD) + Hclj * isHARD;"
                                                             "isHARD = isHD1 * isHD2;"
                                                             "Hsoft = Hls + Hcs;"
                                                             "Hcs = (lambda^n) * 138.935456 * q_prod/sqrt(diff_cl+r^2);"
                                                             "diff_cl = (1.0-lambda) * 0.01;"
                                                             "Hls = 4.0 * eps_avg * (LJ*LJ-LJ);"
                                                             "LJ=((sigma_avg * sigma_avg)/soft)^3;"
                                                             "soft=(diff_lj*delta*sigma_avg + r*r);"
                                                             "diff_lj=(1.0-lambda) * 0.1;"
                                                             "lambda = Logic_lam * lam + Logic_om_lam * (1-lam) + Logic_mix_lam * max(lam,1-lam);"
                                                             "Logic_om_lam = max((1-isHD1)*(1-isHD2)*isTD1*isTD2*(1-isFD1)*(1-isFD2), B_om_lam);"
                                                             "B_om_lam = max(isHD1*(1-isHD2)*isTD1*(1-isTD2)*(1-isFD1)*(1-isFD2), C_om_lam);"
                                                             "C_om_lam = max(isHD1*(1-isHD2)*(1-isTD1)*isTD2*(1-isFD1)*(1-isFD2) , D_om_lam);"
                                                             "D_om_lam = max((1-isHD1)*isHD2*isTD1*(1-isTD2)*(1-isFD1)*(1-isFD2), E_om_lam);"
                                                             "E_om_lam = (1-isHD1)*isHD2*(1-isTD1)*isTD2*(1-isFD1)*(1-isFD2);"
                                                             "Logic_lam = max((1-isHD1)*(1-isHD2)*(1-isTD1)*(1-isTD2)*isFD1*isFD2, B_lam);"
                                                             "B_lam = max(isHD1*(1-isHD2)*(1-isTD1)*(1-isTD2)*isFD1*(1-isFD2), C_lam);"
                                                             "C_lam = max(isHD1*(1-isHD2)*(1-isTD1)*(1-isTD2)*(1-isFD1)*isFD2 , D_lam);"
                                                             "D_lam = max((1-isHD1)*isHD2*(1-isTD1)*(1-isTD2)*isFD1*(1-isFD2), E_lam);"
                                                             "E_lam = (1-isHD1)*isHD2*(1-isTD1)*(1-isTD2)*(1-isFD1)*isFD2;"
                                                             "Logic_mix_lam = max((1-isHD1)*(1-isHD2)*isTD1*(1-isTD2)*isFD1*(1-isFD2), B_mix);"
                                                             "B_mix = max((1-isHD1)*(1-isHD2)*isTD1*(1-isTD2)*(1-isFD1)*isFD2, C_mix);"
                                                             "C_mix = max((1-isHD1)*(1-isHD2)*(1-isTD1)*isTD2*isFD1*(1-isFD2) , D_mix);"
                                                             "D_mix= (1-isHD1)*(1-isHD2)*(1-isTD1)*isTD2*(1-isFD1)*isFD2;"
                                                             "Hclj = Hl + Hc;"
                                                             "Hl = 4 * eps_avg * ((sigma_avg/r)^12-(sigma_avg/r)^6);"
                                                             "Hc = 138.935456 * q_prod/r;"
                                                             "q_prod = (qend1 * lam+(1.0-lam) * qstart1) * (qend2 * lam+(1.0-lam) * qstart2);"
                                                             "eps_avg = sqrt((epend1*lam+(1.0-lam)*epstart1)*(epend2*lam+(1.0-lam)*epstart2));"
                                                             "sigma_avg = 0.5*((sigmaend1*lam+(1.0-lam)*sigmastart1)+(sigmaend2*lam+(1.0-lam)*sigmastart2))");
*/

        custom_force_field = new OpenMM::CustomNonbondedForce("Hls + Hcs;"
                                                              "Hcs = (lambda^n) * 138.935456 * q_prod/sqrt(diff_cl+r^2);"
                                                              "diff_cl = (1.0-lambda) * 0.01;"
                                                              "Hls = 4.0 * eps_avg * (LJ*LJ-LJ);"
                                                              "LJ=((sigma_avg * sigma_avg)/soft)^3;"
                                                              "soft=(diff_lj*delta*sigma_avg + r*r);"
                                                              "diff_lj=(1.0-lambda) * 0.1;"
                                                              "lambda = Logic_lam * lam + Logic_om_lam * (1-lam) + Logic_mix_lam * max(lam,1-lam) + Logic_hard;"
                                                              "Logic_hard = isHD1 * isHD2 * (1-isTD1) * (1-isTD2) * (1-isFD1) * (1-isFD2);"
                                                              "Logic_om_lam = max((1-isHD1)*(1-isHD2)*isTD1*isTD2*(1-isFD1)*(1-isFD2), B_om_lam);"
                                                              "B_om_lam = max(isHD1*(1-isHD2)*isTD1*(1-isTD2)*(1-isFD1)*(1-isFD2), C_om_lam);"
                                                              "C_om_lam = max(isHD1*(1-isHD2)*(1-isTD1)*isTD2*(1-isFD1)*(1-isFD2) , D_om_lam);"
                                                              "D_om_lam = max((1-isHD1)*isHD2*isTD1*(1-isTD2)*(1-isFD1)*(1-isFD2), E_om_lam);"
                                                              "E_om_lam = (1-isHD1)*isHD2*(1-isTD1)*isTD2*(1-isFD1)*(1-isFD2);"
                                                              "Logic_lam = max((1-isHD1)*(1-isHD2)*(1-isTD1)*(1-isTD2)*isFD1*isFD2, B_lam);"
                                                              "B_lam = max(isHD1*(1-isHD2)*(1-isTD1)*(1-isTD2)*isFD1*(1-isFD2), C_lam);"
                                                              "C_lam = max(isHD1*(1-isHD2)*(1-isTD1)*(1-isTD2)*(1-isFD1)*isFD2 , D_lam);"
                                                              "D_lam = max((1-isHD1)*isHD2*(1-isTD1)*(1-isTD2)*isFD1*(1-isFD2), E_lam);"
                                                              "E_lam = (1-isHD1)*isHD2*(1-isTD1)*(1-isTD2)*(1-isFD1)*isFD2;"
                                                              "Logic_mix_lam = max((1-isHD1)*(1-isHD2)*isTD1*(1-isTD2)*isFD1*(1-isFD2), B_mix);"
                                                              "B_mix = max((1-isHD1)*(1-isHD2)*isTD1*(1-isTD2)*(1-isFD1)*isFD2, C_mix);"
                                                              "C_mix = max((1-isHD1)*(1-isHD2)*(1-isTD1)*isTD2*isFD1*(1-isFD2) , D_mix);"
                                                              "D_mix= (1-isHD1)*(1-isHD2)*(1-isTD1)*isTD2*(1-isFD1)*isFD2;"
                                                              "q_prod = (qend1 * lam+(1.0-lam) * qstart1) * (qend2 * lam+(1.0-lam) * qstart2);"
                                                              "eps_avg = sqrt((epend1*lam+(1.0-lam)*epstart1)*(epend2*lam+(1.0-lam)*epstart2));"
                                                              "sigma_avg = 0.5*((sigmaend1*lam+(1.0-lam)*sigmastart1)+(sigmaend2*lam+(1.0-lam)*sigmastart2))");


        custom_force_field->addGlobalParameter("lam",Alchemical_value);
        custom_force_field->addGlobalParameter("delta",shift_delta);
        custom_force_field->addGlobalParameter("n",coulomb_power);
        
        custom_force_field->setNonbondedMethod(OpenMM::CustomNonbondedForce::NoCutoff);


        custom_intra_14_clj = new OpenMM::CustomBondForce("Hl+Hc;"
                                                          "Hl=4*eps_avg*((sigma_avg/r)^12-(sigma_avg/r)^6);"
                                                          "Hc=138.935456*q_prod/r;"
                                                          "eps_avg = sqrt(lamhd*lamhd*eaend + (1-lamhd)*(1-lamhd)*eastart + lamhd*(1-lamhd)*emix);"
                                                          "sigma_avg = lamhd*saend + (1-lamhd)*sastart;"
                                                          "q_prod = lamhd*lamhd*qpend + (1-lamhd)*(1-lamhd)*qpstart + lamhd*(1-lamhd)*qmix");
        
        custom_intra_14_clj->addGlobalParameter("lamhd",Alchemical_value);
        

        custom_intra_14_todummy = new OpenMM::CustomBondForce("Hcs + Hls;"
                                                              "Hcs=(lamtd^ntd)*138.935456*q_prod/sqrt(diff_cl+r^2);"
                                                              "diff_cl=(1.0-lamtd)*0.01;"
                                                              "Hls=4.0*eps_avg*(LJ*LJ-LJ);"
                                                              "LJ=((sigma_avg*sigma_avg)/soft)^3;"
                                                              "soft=(diff_lj*deltatd*sigma_avg+r*r);"
                                                              "diff_lj=(1.0-lamtd)*0.1;"
                                                              "eps_avg = sqrt((1-lamtd)*(1-lamtd)*eaend + lamtd*lamtd*eastart + lamtd*(1-lamtd)*emix);"
                                                              "sigma_avg = (1-lamtd)*saend + lamtd*sastart;"
                                                              "q_prod = (1-lamtd)*(1-lamtd)*qpend + lamtd*lamtd*qpstart + lamtd*(1-lamtd)*qmix");


        custom_intra_14_todummy->addGlobalParameter("lamtd",1.0 - Alchemical_value);
        custom_intra_14_todummy->addGlobalParameter("deltatd",shift_delta);
        custom_intra_14_todummy->addGlobalParameter("ntd", coulomb_power);
        
        
        custom_intra_14_fromdummy = new OpenMM::CustomBondForce("Hcs + Hls;"
                                                                "Hcs=(lamfd^nfd)*138.935456*q_prod/sqrt(diff_cl+r^2);"
                                                                "diff_cl=(1.0-lamfd)*0.01;"
                                                                "Hls=4.0*eps_avg*(LJ*LJ-LJ);"
                                                                "LJ=((sigma_avg*sigma_avg)/soft)^3;"
                                                                "soft=(diff_lj*deltafd*sigma_avg+r*r);"
                                                                "diff_lj=(1.0-lamfd)*0.1;"
                                                                "eps_avg = sqrt(lamfd*lamfd*eaend + (1-lamfd)*(1-lamfd)*eastart + lamfd*(1-lamfd)*emix);"
                                                                "sigma_avg = lamfd*saend + (1-lamfd)*sastart;"
                                                                "q_prod = lamfd*lamfd*qpend + (1-lamfd)*(1-lamfd)*qpstart + lamfd*(1-lamfd)*qmix");

        custom_intra_14_fromdummy->addGlobalParameter("lamfd",Alchemical_value);
        custom_intra_14_fromdummy->addGlobalParameter("deltafd",shift_delta);
        custom_intra_14_fromdummy->addGlobalParameter("nfd",coulomb_power);
        
        
        custom_intra_14_fromdummy_todummy = new OpenMM::CustomBondForce("Hcs + Hls;"
                                                                        "Hcs=(lamFTD^nftd)*138.935456*q_prod/sqrt(diff_cl+r^2);"
                                                                        "diff_cl=(1.0-lamFTD)*0.01;"
                                                                        "Hls=4.0*eps_avg*(LJ*LJ-LJ);"
                                                                        "LJ=((sigma_avg*sigma_avg)/soft)^3;"
                                                                        "soft=(diff_lj*deltaftd*sigma_avg+r*r);"
                                                                        "diff_lj=(1.0-lamFTD)*0.1;"
                                                                        "eps_avg = sqrt(lamftd*lamftd*eaend + (1-lamftd)*(1-lamftd)*eastart + lamftd*(1-lamftd)*emix);"
                                                                        "sigma_avg = lamftd*saend + (1-lamftd)*sastart;"
                                                                        "q_prod = lamftd*lamftd*qpend + (1-lamftd)*(1-lamftd)*qpstart + lamftd*(1-lamftd)*qmix;"
                                                                        "lamFTD = max(lamftd,1-lamftd)");

        custom_intra_14_fromdummy_todummy->addGlobalParameter("lamftd",Alchemical_value);
        custom_intra_14_fromdummy_todummy->addGlobalParameter("deltaftd",shift_delta);
        custom_intra_14_fromdummy_todummy->addGlobalParameter("nftd",coulomb_power);

        if (true){
            qDebug() << "\nCut off type = " << CutoffType << "\n";
            qDebug() <<  "Lambda = " << Alchemical_value << " Coulomb Power = " << coulomb_power << " Delta Shift = " << shift_delta <<"\n";
        }

    }
    else{

        const double converted_cutoff_distance = convertTo(cutoff_distance.value(), nanometer);

        double eps2 = (field_dielectric - 1.0)/(2*field_dielectric+1.0);
        double kvalue = eps2/(converted_cutoff_distance * converted_cutoff_distance * converted_cutoff_distance);
        double cvalue = (1.0/converted_cutoff_distance)*(3.0*field_dielectric)/(2.0*field_dielectric+1.0);

/*
        custom_force_field = new OpenMM::CustomNonbondedForce("Hsoft * (1.0 - isHARD) + Hclj * isHARD;"
                                                             "isHARD = isHD1 * isHD2;"
                                                             "Hsoft = Hls + Hcs;"
                                                             "Hcs = (lambda^n) * 138.935456 * q_prod*(1/sqrt(diff_cl+r*r) + krflam*(diff_cl+r*r)-crflam);"
                                                             "crflam = crf * src;"
                                                             "krflam = krf * src * src * src;"
                                                             "src = cutoff/sqrt(diff_cl + cutoff*cutoff);"
                                                             "diff_cl = (1.0-lambda) * 0.01;"
                                                             "Hls = 4.0 * eps_avg * (LJ*LJ-LJ);"
                                                             "LJ=((sigma_avg * sigma_avg)/soft)^3;"
                                                             "soft=(diff_lj*delta*sigma_avg + r*r);"
                                                             "diff_lj=(1.0-lambda) * 0.1;"
                                                             "lambda = Logic_lam * lam + Logic_om_lam * (1-lam) + Logic_mix_lam * max(lam,1-lam);"
                                                             "Logic_om_lam = max((1-isHD1)*(1-isHD2)*isTD1*isTD2*(1-isFD1)*(1-isFD2), B_om_lam);"
                                                             "B_om_lam = max(isHD1*(1-isHD2)*isTD1*(1-isTD2)*(1-isFD1)*(1-isFD2), C_om_lam);"
                                                             "C_om_lam = max(isHD1*(1-isHD2)*(1-isTD1)*isTD2*(1-isFD1)*(1-isFD2) , D_om_lam);"
                                                             "D_om_lam = max((1-isHD1)*isHD2*isTD1*(1-isTD2)*(1-isFD1)*(1-isFD2), E_om_lam);"
                                                             "E_om_lam = (1-isHD1)*isHD2*(1-isTD1)*isTD2*(1-isFD1)*(1-isFD2);"
                                                             "Logic_lam = max((1-isHD1)*(1-isHD2)*(1-isTD1)*(1-isTD2)*isFD1*isFD2, B_lam);"
                                                             "B_lam = max(isHD1*(1-isHD2)*(1-isTD1)*(1-isTD2)*isFD1*(1-isFD2), C_lam);"
                                                             "C_lam = max(isHD1*(1-isHD2)*(1-isTD1)*(1-isTD2)*(1-isFD1)*isFD2 , D_lam);"
                                                             "D_lam = max((1-isHD1)*isHD2*(1-isTD1)*(1-isTD2)*isFD1*(1-isFD2), E_lam);"
                                                             "E_lam = (1-isHD1)*isHD2*(1-isTD1)*(1-isTD2)*(1-isFD1)*isFD2;"
                                                             "Logic_mix_lam = max((1-isHD1)*(1-isHD2)*isTD1*(1-isTD2)*isFD1*(1-isFD2), B_mix);"
                                                             "B_mix = max((1-isHD1)*(1-isHD2)*isTD1*(1-isTD2)*(1-isFD1)*isFD2, C_mix);"
                                                             "C_mix = max((1-isHD1)*(1-isHD2)*(1-isTD1)*isTD2*isFD1*(1-isFD2) , D_mix);"
                                                             "D_mix= (1-isHD1)*(1-isHD2)*(1-isTD1)*isTD2*(1-isFD1)*isFD2;"
                                                             "Hclj = Hl + Hc;"
                                                             "Hl = 4 * eps_avg * ((sigma_avg/r)^12-(sigma_avg/r)^6);"
                                                             "Hc = 138.935456 * q_prod*(1/r + krf * r*r - crf);"
                                                             "q_prod = (qend1 * lam+(1.0-lam) * qstart1) * (qend2 * lam+(1.0-lam) * qstart2);"
                                                             "eps_avg = sqrt((epend1*lam+(1.0-lam)*epstart1)*(epend2*lam+(1.0-lam)*epstart2));"
                                                             "sigma_avg = 0.5*((sigmaend1*lam+(1.0-lam)*sigmastart1)+(sigmaend2*lam+(1.0-lam)*sigmastart2))");
*/



        custom_force_field = new OpenMM::CustomNonbondedForce("Hls + Hcs;"
                                                              "Hcs = (lambda^n) * 138.935456 * q_prod*(1/sqrt(diff_cl+r*r) + krflam*(diff_cl+r*r)-crflam);"
                                                              "crflam = crf * src;"
                                                              "krflam = krf * src * src * src;"
                                                              "src = cutoff/sqrt(diff_cl + cutoff*cutoff);"
                                                              "diff_cl = (1.0-lambda) * 0.01;"
                                                              "Hls = 4.0 * eps_avg * (LJ*LJ-LJ);"
                                                              "LJ=((sigma_avg * sigma_avg)/soft)^3;"
                                                              "soft=(diff_lj*delta*sigma_avg + r*r);"
                                                              "diff_lj=(1.0-lambda) * 0.1;"
                                                              "lambda = Logic_lam * lam + Logic_om_lam * (1-lam) + Logic_mix_lam * max(lam,1-lam) + Logic_hard;"
                                                              "Logic_hard = isHD1 * isHD2 * (1-isTD1) * (1-isTD2) * (1-isFD1) * (1-isFD2);"
                                                              "Logic_om_lam = max((1-isHD1)*(1-isHD2)*isTD1*isTD2*(1-isFD1)*(1-isFD2), B_om_lam);"
                                                              "B_om_lam = max(isHD1*(1-isHD2)*isTD1*(1-isTD2)*(1-isFD1)*(1-isFD2), C_om_lam);"
                                                              "C_om_lam = max(isHD1*(1-isHD2)*(1-isTD1)*isTD2*(1-isFD1)*(1-isFD2) , D_om_lam);"
                                                              "D_om_lam = max((1-isHD1)*isHD2*isTD1*(1-isTD2)*(1-isFD1)*(1-isFD2), E_om_lam);"
                                                              "E_om_lam = (1-isHD1)*isHD2*(1-isTD1)*isTD2*(1-isFD1)*(1-isFD2);"
                                                              "Logic_lam = max((1-isHD1)*(1-isHD2)*(1-isTD1)*(1-isTD2)*isFD1*isFD2, B_lam);"
                                                              "B_lam = max(isHD1*(1-isHD2)*(1-isTD1)*(1-isTD2)*isFD1*(1-isFD2), C_lam);"
                                                              "C_lam = max(isHD1*(1-isHD2)*(1-isTD1)*(1-isTD2)*(1-isFD1)*isFD2 , D_lam);"
                                                              "D_lam = max((1-isHD1)*isHD2*(1-isTD1)*(1-isTD2)*isFD1*(1-isFD2), E_lam);"
                                                              "E_lam = (1-isHD1)*isHD2*(1-isTD1)*(1-isTD2)*(1-isFD1)*isFD2;"
                                                              "Logic_mix_lam = max((1-isHD1)*(1-isHD2)*isTD1*(1-isTD2)*isFD1*(1-isFD2), B_mix);"
                                                              "B_mix = max((1-isHD1)*(1-isHD2)*isTD1*(1-isTD2)*(1-isFD1)*isFD2, C_mix);"
                                                              "C_mix = max((1-isHD1)*(1-isHD2)*(1-isTD1)*isTD2*isFD1*(1-isFD2) , D_mix);"
                                                              "D_mix= (1-isHD1)*(1-isHD2)*(1-isTD1)*isTD2*(1-isFD1)*isFD2;"
                                                              "q_prod = (qend1 * lam+(1.0-lam) * qstart1) * (qend2 * lam+(1.0-lam) * qstart2);"
                                                              "eps_avg = sqrt((epend1*lam+(1.0-lam)*epstart1)*(epend2*lam+(1.0-lam)*epstart2));"
                                                              "sigma_avg = 0.5*((sigmaend1*lam+(1.0-lam)*sigmastart1)+(sigmaend2*lam+(1.0-lam)*sigmastart2))");


        custom_force_field->setCutoffDistance(converted_cutoff_distance);

        custom_force_field->addGlobalParameter("lam",Alchemical_value);
        custom_force_field->addGlobalParameter("delta",shift_delta);
        custom_force_field->addGlobalParameter("n",coulomb_power);
        custom_force_field->addGlobalParameter("krf",kvalue);
        custom_force_field->addGlobalParameter("crf",cvalue);
        custom_force_field->addGlobalParameter("cutoff",converted_cutoff_distance);
        
        
        if(flag_cutoff == CUTOFFNONPERIODIC){
            custom_force_field->setNonbondedMethod(OpenMM::CustomNonbondedForce::CutoffNonPeriodic);
            
        }
        else{
            custom_force_field->setNonbondedMethod(OpenMM::CustomNonbondedForce::CutoffPeriodic);
        }

        //NO REACTION FIELD IS APPLIED TO 1-4 INTERACTIONS

        custom_intra_14_clj = new OpenMM::CustomBondForce("withinCutoff*(Hl+Hc);"
                                                          "withinCutoff=step(cutoffhd-r);"
                                                          "Hl=4*eps_avg*((sigma_avg/r)^12-(sigma_avg/r)^6);"
                                                          "Hc=138.935456*q_prod/r;"
                                                          "eps_avg = sqrt(lamhd*lamhd*eaend + (1-lamhd)*(1-lamhd)*eastart + lamhd*(1-lamhd)*emix);"
                                                          "sigma_avg = lamhd*saend + (1-lamhd)*sastart;"
                                                          "q_prod = lamhd*lamhd*qpend + (1-lamhd)*(1-lamhd)*qpstart + lamhd*(1-lamhd)*qmix");

        custom_intra_14_clj->addGlobalParameter("lamhd",Alchemical_value);
        custom_intra_14_clj->addGlobalParameter("cutoffhd",converted_cutoff_distance);
        
        
        custom_intra_14_todummy = new OpenMM::CustomBondForce("withinCutoff*(Hcs + Hls);"
                                                              "withinCutoff=step(cutofftd-r);"
                                                              "Hcs=(lamtd^ntd)*138.935456*q_prod/sqrt(diff_cl+r^2);"
                                                              "diff_cl=(1.0-lamtd)*0.01;"
                                                              "Hls=4.0*eps_avg*(LJ*LJ-LJ);"
                                                              "LJ=((sigma_avg*sigma_avg)/soft)^3;"
                                                              "soft=(diff_lj*deltatd*sigma_avg+r*r);"
                                                              "diff_lj=(1.0-lamtd)*0.1;"
                                                              "eps_avg = sqrt((1-lamtd)*(1-lamtd)*eaend + lamtd*lamtd*eastart + lamtd*(1-lamtd)*emix);"
                                                              "sigma_avg = (1-lamtd)*saend + lamtd*sastart;"
                                                              "q_prod = (1-lamtd)*(1-lamtd)*qpend + lamtd*lamtd*qpstart + lamtd*(1-lamtd)*qmix");
                                                              
        custom_intra_14_todummy->addGlobalParameter("lamtd",1.0 - Alchemical_value);
        custom_intra_14_todummy->addGlobalParameter("deltatd",shift_delta);
        custom_intra_14_todummy->addGlobalParameter("ntd", coulomb_power);
        custom_intra_14_todummy->addGlobalParameter("cutofftd", converted_cutoff_distance);

        custom_intra_14_fromdummy = new OpenMM::CustomBondForce("withinCutoff*(Hcs + Hls);"
                                                                "withinCutoff=step(cutofffd-r);"
                                                                "Hcs=(lamfd^nfd)*138.935456*q_prod/sqrt(diff_cl+r^2);"
                                                                "diff_cl=(1.0-lamfd)*0.01;"
                                                                "Hls=4.0*eps_avg*(LJ*LJ-LJ);"
                                                                "LJ=((sigma_avg*sigma_avg)/soft)^3;"
                                                                "soft=(diff_lj*deltafd*sigma_avg+r*r);"
                                                                "diff_lj=(1.0-lamfd)*0.1;"
                                                                "eps_avg = sqrt(lamfd*lamfd*eaend + (1-lamfd)*(1-lamfd)*eastart + lamfd*(1-lamfd)*emix);"
                                                                "sigma_avg = lamfd*saend + (1-lamfd)*sastart;"
                                                                "q_prod = lamfd*lamfd*qpend + (1-lamfd)*(1-lamfd)*qpstart + lamfd*(1-lamfd)*qmix");

        custom_intra_14_fromdummy->addGlobalParameter("lamfd",Alchemical_value);
        custom_intra_14_fromdummy->addGlobalParameter("deltafd",shift_delta);
        custom_intra_14_fromdummy->addGlobalParameter("nfd",coulomb_power);
        custom_intra_14_fromdummy->addGlobalParameter("cutofffd",converted_cutoff_distance);


        custom_intra_14_fromdummy_todummy = new OpenMM::CustomBondForce("withinCutoff*(Hcs + Hls);"
                                                                        "withinCutoff=step(cutoffftd-r);"
                                                                        "Hcs=(lamFTD^nftd)*138.935456*q_prod/sqrt(diff_cl+r^2);"
                                                                        "diff_cl=(1.0-lamFTD)*0.01;"
                                                                        "Hls=4.0*eps_avg*(LJ*LJ-LJ);"
                                                                        "LJ=((sigma_avg*sigma_avg)/soft)^3;"
                                                                        "soft=(diff_lj*deltaftd*sigma_avg+r*r);"
                                                                        "diff_lj=(1.0-lamFTD)*0.1;"
                                                                        "eps_avg = sqrt(lamftd*lamftd*eaend + (1-lamftd)*(1-lamftd)*eastart + lamftd*(1-lamftd)*emix);"
                                                                        "sigma_avg = lamftd*saend + (1-lamftd)*sastart;"
                                                                        "q_prod = lamftd*lamftd*qpend + (1-lamftd)*(1-lamftd)*qpstart + lamftd*(1-lamftd)*qmix;"
                                                                        "lamFTD = max(lamftd,1-lamftd)");

        custom_intra_14_fromdummy_todummy->addGlobalParameter("lamftd",Alchemical_value);
        custom_intra_14_fromdummy_todummy->addGlobalParameter("deltaftd",shift_delta);
        custom_intra_14_fromdummy_todummy->addGlobalParameter("nftd",coulomb_power);
        custom_intra_14_fromdummy_todummy->addGlobalParameter("cutoffftd",converted_cutoff_distance);


        if (true) {
            qDebug() << "\nCut off type = " << CutoffType;
            qDebug() << "CutOff distance = " << converted_cutoff_distance  << " Nm";
            qDebug() << "Dielectric constant = " << field_dielectric;
            qDebug() << "Lambda = " << Alchemical_value << " Coulomb Power = " << coulomb_power << " Delta Shift = " << shift_delta;

        }

    }

    // Andersen thermostat
    if (Andersen_flag == true){
        const double converted_Temperature = convertTo(Temperature.value(), kelvin);

        OpenMM::AndersenThermostat * thermostat = new OpenMM::AndersenThermostat(converted_Temperature, Andersen_frequency);

        //Set The random seed
        //thermostat->setRandomNumberSeed(1234567);

        system_openmm->addForce(thermostat);

        if (Debug){
            qDebug() << "\nAndersen Thermostat set\n";
            qDebug() << "Temperature = " << converted_Temperature << " K\n";
            qDebug() << "Frequency collisions = " << Andersen_frequency << " 1/ps\n";
        }
    }


    // Monte Carlo Barostat
    if (MCBarostat_flag == true){
        const double converted_Temperature = convertTo(Temperature.value(), kelvin);
        const double converted_Pressure = convertTo(Pressure.value(), bar);

        OpenMM::MonteCarloBarostat * barostat = new OpenMM::MonteCarloBarostat(converted_Pressure, converted_Temperature, MCBarostat_frequency);

        //Set The random seed
        //barostat->setRandomNumberSeed(1234567);

        system_openmm->addForce(barostat);

        if (Debug){
            qDebug() << "\nMonte Carlo Barostat set\n";
            qDebug() << "Temperature = " << converted_Temperature << " K\n";
            qDebug() << "Pressure = " << converted_Pressure << " bar\n";
            qDebug() << "Frequency every " << MCBarostat_frequency << " steps\n";
        }

    }
/***********************************************************************BONDED INTERACTIONS****************************************************************/


    OpenMM::HarmonicBondForce * bondStretch_openmm = new OpenMM::HarmonicBondForce();

    OpenMM::HarmonicAngleForce * bondBend_openmm = new OpenMM::HarmonicAngleForce();

    OpenMM::PeriodicTorsionForce * bondTorsion_openmm = new OpenMM::PeriodicTorsionForce();


    OpenMM::CustomBondForce* solute_bond_perturbation = NULL;

    OpenMM::CustomAngleForce* solute_angle_perturbation = NULL;


    solute_bond_perturbation = new OpenMM::CustomBondForce("B*(r-req)^2;"
                                                            "B=bend*lambond+(1.0-lambond)*bstart;"
                                                            "req=rend*lambond+(1.0-lambond)*rstart");


    solute_bond_perturbation->addGlobalParameter("lambond",Alchemical_value);

    solute_angle_perturbation = new OpenMM::CustomAngleForce("A*(theta-thetaeq)^2;"
                                                            "A=aend*lamangle+(1.0-lamangle)*astart;"
                                                            "thetaeq=thetaend*lamangle+(1.0-lamangle)*thetastart");

    solute_angle_perturbation->addGlobalParameter("lamangle",Alchemical_value);



    // Check whether positional restraints have been defined for a set of atoms in that molecule
    // You can get the information out by getting the property and casting to VariantProperty
    //From VariantProperty you have the QVariant, so you can call .toDouble() and .toInt() there
    //so VariantProperty num = mol.property(QString("AtomNum(%1)").arg(i)).asA<VariantProperty>();
    //AtomNum atomnum( num.toInt() );
    //double x = mol.property(QString("x(%1)").arg(i)).asA<VariantProperty>().toDouble();
    //double y = ...; double z = ...;
    //QVector< QPair<AtomNum,Vector> > vals;
    //vals.append( QPair<AtomNum,Vector>(AtomNum(num.toInt()), Vector(x,y,z) ) );

/***************************************************************************RESTRAINTS************************************************************/

    OpenMM::CustomExternalForce * positionalRestraints_openmm = NULL;

    if (Restraint_flag == true){

        positionalRestraints_openmm = new OpenMM::CustomExternalForce("k*( (x-xref)^2 + (y-yref)^2 + (z-zref)^2 )");

        positionalRestraints_openmm->addPerParticleParameter("xref");
        positionalRestraints_openmm->addPerParticleParameter("yref");
        positionalRestraints_openmm->addPerParticleParameter("zref");
        positionalRestraints_openmm->addPerParticleParameter("k");

        system_openmm->addForce(positionalRestraints_openmm);

        if (true)
            qDebug() << "\n\nRestraint is ON\n\n";
    
    }


    //OpenMM vector coordinate
    std::vector<OpenMM::Vec3> positions_openmm(nats);

    //OpenMM vector momenta
    std::vector<OpenMM::Vec3> velocities_openmm(nats);

    std::vector<std::pair<int,int> > bondPairs;

    int system_index = 0;

    // To avoid possible mismatch between the index in which atoms are added to the openmm system arrays and 
    // their atomic numbers in sire, one array is populated while filling up the openmm global arrays
    //  AtomNumtoopenmmIndex
    QHash<int, int> AtomNumToOpenMMIndex;


    for (int i=0; i < nmols; ++i){

        const int nats_mol = ws.nAtoms(i);

        const double *m = ws.massArray(i);

        MolNum molnum = moleculegroup.molNumAt(i);

        const ViewsOfMol &molview = moleculegroup[molnum].data();

        const Molecule &mol = molview.molecule();

        Selector<Atom> molatoms = mol.atoms();

        for (int j=0; j < nats_mol; ++j){

            system_openmm->addParticle(m[j]) ;

            Atom at = molatoms.at(j);
            AtomNum atnum = at.number();

            if (Debug)
                qDebug() << " openMM_index " << system_index << " Sire Atom Number " << atnum.toString() << " Mass particle = " << m[j];

            AtomNumToOpenMMIndex[atnum.value()] = system_index;

            // JM Nov 12
            // The code below implements a ThreeParticleAverageSite for virtual sites for EPW atoms present in a WAT residue
            // This is very AMBER specific. 

            AtomName atname = at.name();

            if (Debug)
                qDebug() << " atname " << atname.value() << " mol " << i;

            if ( atname == AtomName("EPW") ){

                ResName resname = at.residue().name();

                if ( resname == ResName("WAT") ){
                    
                    Atom oatom = molatoms.select( AtomName("O") );
                    Atom h1atom = molatoms.select( AtomName("H1") );
                    Atom h2atom = molatoms.select( AtomName("H2") );

                    AmberParameters amber_params = mol.property("amberparameters").asA<AmberParameters>();
                    QList<BondID> bonds_ff = amber_params.getAllBonds();

                    double distoh = -1.0;
                    double disthh = -1.0;
                    double distoe = -1.0;

                    for (int k=0; k < bonds_ff.length() ; k++){

                        BondID bond_ff = bonds_ff[k];
                        QList<double> bond_params = amber_params.getParams(bond_ff);

                        double r0 = bond_params[1];

                        AtomName at0name = mol.select( bond_ff.atom0() ).name();
                        AtomName at1name = mol.select( bond_ff.atom1() ).name();

                        // qDebug() << " at0name " << at0name.toString() << " at1name " << at1name.toString();

                        if ( ( at0name == AtomName("O") and at1name == AtomName("H1") ) or ( at0name == AtomName("H1") and at1name == AtomName("O") ) ){
                            distoh = r0;
                        }
                        else if ( ( at0name == AtomName("H1") and at1name == AtomName("H2") ) or ( at0name == AtomName("H2") and at1name == AtomName("H1") ) ){
                            disthh = r0;
                        }
                        else if ( ( at0name == AtomName("EPW") and at1name== AtomName("O") ) or ( at0name == AtomName("O") and at1name == AtomName("EPW") ) ){
                            distoe = r0;
                        }
                    }

                    if ( distoh < 0 or disthh < 0 or distoe < 0){
                        throw SireError::program_bug(QObject::tr(
                            "Could not find expected atoms in TIP4P water molecule."), CODELOC);
                    }

                    //qDebug() << " distoe " << distoe << " distoh " << distoh << " disthh " << disthh;

                    double weightH = distoe / sqrt( (distoh*distoh) - ( 0.25 * disthh*disthh) );

                    int o_index = AtomNumToOpenMMIndex[oatom.number().value()];
                    int h1_index = AtomNumToOpenMMIndex[h1atom.number().value()];
                    int h2_index = AtomNumToOpenMMIndex[h2atom.number().value()];

                    if (Debug)
                        qDebug() << "virtual site " << system_index << " o " << o_index << " h1 " << h1_index << " h2 " << h2_index << " 1 - weightH " << 1 - weightH << " weightH/2 " << weightH/2 ;

                    OpenMM::ThreeParticleAverageSite * vsite =  new OpenMM::ThreeParticleAverageSite(o_index, h1_index, h2_index, 1-weightH, weightH/2, weightH/2);

                    system_openmm->setVirtualSite( system_index, vsite );

                }
            }

            system_index = system_index + 1;

        }// end of loop on atoms in molecule

    }//end of loop on molecules in workspace


    int num_atoms_till_i = 0;

    /*NON BONDED PER PARTICLE PARAMETERS*/
    
    custom_force_field->addPerParticleParameter("qstart");
    custom_force_field->addPerParticleParameter("qend");
    custom_force_field->addPerParticleParameter("epstart");
    custom_force_field->addPerParticleParameter("epend");
    custom_force_field->addPerParticleParameter("sigmastart");
    custom_force_field->addPerParticleParameter("sigmaend");
    custom_force_field->addPerParticleParameter("isHD");
    custom_force_field->addPerParticleParameter("isTD");
    custom_force_field->addPerParticleParameter("isFD");
    
    custom_intra_14_clj->addPerBondParameter("qpstart");
    custom_intra_14_clj->addPerBondParameter("qpend");
    custom_intra_14_clj->addPerBondParameter("qmix");
    custom_intra_14_clj->addPerBondParameter("eastart");
    custom_intra_14_clj->addPerBondParameter("eaend");
    custom_intra_14_clj->addPerBondParameter("emix");
    custom_intra_14_clj->addPerBondParameter("sastart");
    custom_intra_14_clj->addPerBondParameter("saend");

    custom_intra_14_todummy->addPerBondParameter("qpstart");
    custom_intra_14_todummy->addPerBondParameter("qpend");
    custom_intra_14_todummy->addPerBondParameter("qmix");
    custom_intra_14_todummy->addPerBondParameter("eastart");
    custom_intra_14_todummy->addPerBondParameter("eaend");
    custom_intra_14_todummy->addPerBondParameter("emix");
    custom_intra_14_todummy->addPerBondParameter("sastart");
    custom_intra_14_todummy->addPerBondParameter("saend");
    
    custom_intra_14_fromdummy->addPerBondParameter("qpstart");
    custom_intra_14_fromdummy->addPerBondParameter("qpend");
    custom_intra_14_fromdummy->addPerBondParameter("qmix");
    custom_intra_14_fromdummy->addPerBondParameter("eastart");
    custom_intra_14_fromdummy->addPerBondParameter("eaend");
    custom_intra_14_fromdummy->addPerBondParameter("emix");
    custom_intra_14_fromdummy->addPerBondParameter("sastart");
    custom_intra_14_fromdummy->addPerBondParameter("saend");
    
    custom_intra_14_fromdummy_todummy->addPerBondParameter("qpstart");
    custom_intra_14_fromdummy_todummy->addPerBondParameter("qpend");
    custom_intra_14_fromdummy_todummy->addPerBondParameter("qmix");
    custom_intra_14_fromdummy_todummy->addPerBondParameter("eastart");
    custom_intra_14_fromdummy_todummy->addPerBondParameter("eaend");
    custom_intra_14_fromdummy_todummy->addPerBondParameter("emix");
    custom_intra_14_fromdummy_todummy->addPerBondParameter("sastart");
    custom_intra_14_fromdummy_todummy->addPerBondParameter("saend");


    /*BONDED PER PARTICLE PARAMETERS*/

    solute_bond_perturbation->addPerBondParameter("bstart");
    solute_bond_perturbation->addPerBondParameter("bend");
    solute_bond_perturbation->addPerBondParameter("rstart");
    solute_bond_perturbation->addPerBondParameter("rend");

    solute_angle_perturbation->addPerAngleParameter("astart");
    solute_angle_perturbation->addPerAngleParameter("aend");
    solute_angle_perturbation->addPerAngleParameter("thetastart");
    solute_angle_perturbation->addPerAngleParameter("thetaend");


    Molecule solutemol = solute.moleculeAt(0).molecule();


    int nions = 0;
    
    
    QVector<bool> perturbed_energies_tmp(8);
    
    for(int i = 0; i<perturbed_energies_tmp.size();i++)
        perturbed_energies_tmp[i] = false;
    
    
    for (int i=0; i < nmols ; i++){

        if(Debug)
            qDebug()<< "Molecule number = " << i;
            
        const Vector *c = ws.coordsArray(i);

        Molecule molecule = moleculegroup.moleculeAt(i).molecule();

        int num_atoms_molecule = molecule.nAtoms();
        
        std::vector<double> custom_non_bonded_params(9);

        // NON BONDED TERMS

        // The atomic parameters
        AtomLJs atomvdws = molecule.property("LJ").asA<AtomLJs>();
        AtomCharges atomcharges = molecule.property("charge").asA<AtomCharges>();
        QVector<SireMM::LJParameter> ljparameters = atomvdws.toVector();
        QVector<SireUnits::Dimension::Charge> charges = atomcharges.toVector();
        
        QVector<SireUnits::Dimension::Charge> start_charges;
        QVector<SireUnits::Dimension::Charge> final_charges;
        QVector<SireMM::LJParameter> start_LJs;
        QVector<SireMM::LJParameter> final_LJs;

        if(molecule.hasProperty("perturbations")){
            AtomCharges atomcharges_start = molecule.property("initial_charge").asA<AtomCharges>();
            AtomCharges atomcharges_final = molecule.property("final_charge").asA<AtomCharges>();
            
            start_charges = atomcharges_start.toVector();
            final_charges = atomcharges_final.toVector();
            
            AtomLJs atomvdws_start = molecule.property("initial_LJ").asA<AtomLJs>();
            AtomLJs atomvdws_final = molecule.property("final_LJ").asA<AtomLJs>();
            start_LJs = atomvdws_start.toVector();
            final_LJs = atomvdws_final.toVector();
        }

        for(int j=0; j< ljparameters.size();j++){

            double sigma = ljparameters[j].sigma();
            double epsilon = ljparameters[j].epsilon();
            double charge = charges[j].value();

            nonbond_openmm->addParticle(charge, sigma * OpenMM::NmPerAngstrom, epsilon * OpenMM::KJPerKcal);

            Atom atom = molecule.molecule().atoms()[j];

            if(molecule.hasProperty("perturbations")){
                if(!solutehard.isEmpty() && solutehard.moleculeAt(0).atoms().contains(atom)){//hard solute atom

                    double charge_start = start_charges[j].value();
                    double charge_final = final_charges[j].value();
                
                    double epsilon_start = start_LJs[j].epsilon();
                    double epsilon_final = final_LJs[j].epsilon();
                    double sigma_start = start_LJs[j].sigma();
                    double sigma_final = final_LJs[j].sigma();
                
                    custom_non_bonded_params[0] = charge_start;
                    custom_non_bonded_params[1] = charge_final;
                    custom_non_bonded_params[2] = epsilon_start * OpenMM::KJPerKcal;
                    custom_non_bonded_params[3] = epsilon_final * OpenMM::KJPerKcal; 
                    custom_non_bonded_params[4] = sigma_start * OpenMM::NmPerAngstrom;
                    custom_non_bonded_params[5] = sigma_final * OpenMM::NmPerAngstrom;
                    custom_non_bonded_params[6] = 1.0;//isHard
                    custom_non_bonded_params[7] = 0.0;//isTodummy
                    custom_non_bonded_params[8] = 0.0;//isFromdummy

                    if(Debug)
                        qDebug() << "hard solute = " << atom.index();

                }
                else if(!solutetodummy.isEmpty() && solutetodummy.moleculeAt(0).atoms().contains(atom)){//to dummy solute atom

                    double charge_start = start_charges[j].value();
                    double charge_final = final_charges[j].value();
                
                    double epsilon_start = start_LJs[j].epsilon();
                    double epsilon_final = final_LJs[j].epsilon();
                    double sigma_start = start_LJs[j].sigma();
                    double sigma_final = final_LJs[j].sigma();
                
                
                    custom_non_bonded_params[0] = charge_start;
                    custom_non_bonded_params[1] = charge_final;
                    custom_non_bonded_params[2] = epsilon_start * OpenMM::KJPerKcal;
                    custom_non_bonded_params[3] = epsilon_final * OpenMM::KJPerKcal;
                    custom_non_bonded_params[4] = sigma_start * OpenMM::NmPerAngstrom;
                    custom_non_bonded_params[5] = sigma_final * OpenMM::NmPerAngstrom;
                    custom_non_bonded_params[6] = 0.0;//isHard
                    custom_non_bonded_params[7] = 1.0;//isTodummy
                    custom_non_bonded_params[8] = 0.0;//isFromdummy

                    if(Debug)
                        qDebug() << "to dummy solute = " << atom.index();

                }
                else if(!solutefromdummy.isEmpty() && solutefromdummy.moleculeAt(0).atoms().contains(atom)){//from dummy solute atom

                    double charge_start = start_charges[j].value();
                    double charge_final = final_charges[j].value();

                    double epsilon_start = start_LJs[j].epsilon();
                    double epsilon_final = final_LJs[j].epsilon();
                    double sigma_start = start_LJs[j].sigma();
                    double sigma_final = final_LJs[j].sigma();
                
                    custom_non_bonded_params[0] = charge_start;
                    custom_non_bonded_params[1] = charge_final;
                    custom_non_bonded_params[2] = epsilon_start * OpenMM::KJPerKcal;
                    custom_non_bonded_params[3] = epsilon_final * OpenMM::KJPerKcal;
                    custom_non_bonded_params[4] = sigma_start * OpenMM::NmPerAngstrom;
                    custom_non_bonded_params[5] = sigma_final * OpenMM::NmPerAngstrom;
                    custom_non_bonded_params[6] = 0.0;//isHard
                    custom_non_bonded_params[7] = 0.0;//isTodummy
                    custom_non_bonded_params[8] = 1.0;//isFromdummy

                    if(Debug)
                        qDebug() << "from dummy solute = " << atom.index();

                }
            }
            else{//solvent atom like hard

                custom_non_bonded_params[0] = charge;
                custom_non_bonded_params[1] = charge;
                custom_non_bonded_params[2] = epsilon * OpenMM::KJPerKcal;
                custom_non_bonded_params[3] = epsilon * OpenMM::KJPerKcal;
                custom_non_bonded_params[4] = sigma * OpenMM::NmPerAngstrom;
                custom_non_bonded_params[5] = sigma * OpenMM::NmPerAngstrom;
                custom_non_bonded_params[6] = 1.0;//isHard
                custom_non_bonded_params[7] = 0.0;//isTodummy
                custom_non_bonded_params[8] = 0.0;//isFromdummy

                if(Debug)
                    qDebug() << "Solvent = " << atom.index();

            }

            if(Debug){
                qDebug() << "Charge start = " << custom_non_bonded_params[0];
                qDebug() << "Charge end = " << custom_non_bonded_params[1];
                qDebug() << "Eps start = " << custom_non_bonded_params[2];
                qDebug() << "Eps end = " << custom_non_bonded_params[3];
                qDebug() << "Sig start = " << custom_non_bonded_params[4];
                qDebug() << "Sig end = " << custom_non_bonded_params[5];
                qDebug() << "is Hard = " << custom_non_bonded_params[6];
                qDebug() << "is To dummy = " << custom_non_bonded_params[7];
                qDebug() << "is From dummy = " << custom_non_bonded_params[8] << "\n";
            }
            
            custom_force_field->addParticle(custom_non_bonded_params);

        }


/****************************************************RESTRAINTS*******************************************************/

        if(Restraint_flag == true){

            bool hasRestrainedAtoms = molecule.hasProperty("restrainedatoms");

            if(hasRestrainedAtoms){

                Properties restrainedAtoms = molecule.property("restrainedatoms").asA<Properties>();

                int nrestrainedatoms = restrainedAtoms.property(QString("nrestrainedatoms")).asA<VariantProperty>().toInt();

                if (true)
                    qDebug() << " nrestrainedatoms " << nrestrainedatoms ;

                    for (int i=0; i < nrestrainedatoms ; i++){

                        int atomnum = restrainedAtoms.property(QString("AtomNum(%1)").arg(i)).asA<VariantProperty>().toInt();
                        double xref = restrainedAtoms.property(QString("x(%1)").arg(i)).asA<VariantProperty>().toDouble();
                        double yref = restrainedAtoms.property(QString("y(%1)").arg(i)).asA<VariantProperty>().toDouble();
                        double zref = restrainedAtoms.property(QString("z(%1)").arg(i)).asA<VariantProperty>().toDouble();
                        double k = restrainedAtoms.property(QString("k(%1)").arg(i)).asA<VariantProperty>().toDouble();

                        int openmmindex = AtomNumToOpenMMIndex[atomnum];

                        if (true){
                            qDebug() << "atomnum " << atomnum << " openmmindex " << openmmindex << " x " << xref << " y " << yref << " z " << zref << " k " << k;
                            qDebug() << "Atom name =  " << molecule.atom(AtomIdx(openmmindex)).toString();
                        }
                        
                        
                        int posrestrdim = 4;
                        std::vector<double> params(posrestrdim);

                        params[0] = xref * OpenMM::NmPerAngstrom;
                        params[1] = yref * OpenMM::NmPerAngstrom;
                        params[2] = zref * OpenMM::NmPerAngstrom;
                        params[3] = k  * ( OpenMM::KJPerKcal * OpenMM::AngstromsPerNm * OpenMM::AngstromsPerNm );

                        positionalRestraints_openmm->addParticle(openmmindex, params);
                    }
            }
        }//end of restraint flag


        // IONS


        bool hasConnectivity = molecule.hasProperty("connectivity");

        if ( !hasConnectivity ){

            num_atoms_till_i = num_atoms_till_i + num_atoms_molecule ;

            if (Debug){
                qDebug() << "\nAtoms = " <<  num_atoms_molecule << " Num atoms till i =" << num_atoms_till_i <<"\n";
                qDebug() << "\n*********************MONOATOMIC MOLECULE DETECTED**************************\n";
            }
            nions=nions+1;
            continue;
        }
        


        //BONDED TERMS

        QList< BondID > bond_pert_list;
        QList< BondID > bond_pert_swap_list;
        QList< AngleID > angle_pert_list;
        QList< AngleID > angle_pert_swap_list;
        QList< DihedralID > dihedral_pert_list;
        QList< DihedralID > dihedral_pert_swap_list;
        QList< ImproperID > improper_pert_list;
        QList< ImproperID > improper_pert_swap_list;

        if(molecule.isSameMolecule(solutemol)){//Solute molecule perturbation

            Perturbations pert_params = molecule.property("perturbations").asA<Perturbations>();

            QList< PropPtr<Perturbation> > perturbation_list = pert_params.perturbations();

            std::vector<double> solute_bond_perturbation_params(4);
            std::vector<double> solute_angle_perturbation_params(4);
            std::vector<double> solute_torsion_perturbation_params(1);

            for (QList< PropPtr<Perturbation> >::const_iterator it = perturbation_list.constBegin();it != perturbation_list.constEnd();++it){

                const Perturbation &pert = *it;

                if (pert.isA<InternalPerturbation>()){

                    QString str = pert.what();
                    
                    if(str == "SireMM::TwoAtomPerturbation"){
                        const TwoAtomPerturbation &two = pert.asA<TwoAtomPerturbation>();

                        int idx0 = two.atom0().asA<AtomIdx>().value() + num_atoms_till_i;
                        int idx1 = two.atom1().asA<AtomIdx>().value() + num_atoms_till_i;
                        double rstart = two.initialForms()[Symbol("r0")].toString().toDouble();
                        double bstart = two.initialForms()[Symbol("k")].toString().toDouble();
                        double rend = two.finalForms()[Symbol("r0")].toString().toDouble();
                        double bend = two.finalForms()[Symbol("k")].toString().toDouble();

                        solute_bond_perturbation_params[0]=bstart * OpenMM::KJPerKcal * OpenMM::AngstromsPerNm * OpenMM::AngstromsPerNm;
                        solute_bond_perturbation_params[1]=bend * OpenMM::KJPerKcal * OpenMM::AngstromsPerNm * OpenMM::AngstromsPerNm;
                        solute_bond_perturbation_params[2]=rstart * OpenMM::NmPerAngstrom;
                        solute_bond_perturbation_params[3]=rend * OpenMM::NmPerAngstrom;

                        solute_bond_perturbation->addBond(idx0,idx1,solute_bond_perturbation_params);

                        bond_pert_list.append(BondID(two.atom0(),two.atom1()));
                        bond_pert_swap_list.append(BondID(two.atom1(),two.atom0()));

                        bondPairs.push_back(std::make_pair(idx0,idx1));
                        
                        if(Debug){
                            qDebug() << "Atom0 = " << two.atom0().asA<AtomIdx>().value() << 
                                        "Atom1 = "<< two.atom1().asA<AtomIdx>().value() ;
                            qDebug() << "IDX0 = " << idx0 <<  "IDX1 = " << idx1  << "\n";
                            qDebug() << "rstart = " << rstart <<
                                        "rend = " << rend ;
                            qDebug() << "bstart = " << bstart <<
                                        "bend = " << bend << "\n";
                        }
                    }
                    if(str == "SireMM::ThreeAtomPerturbation"){
                        const ThreeAtomPerturbation &three = pert.asA<ThreeAtomPerturbation>();

                        int idx0 = three.atom0().asA<AtomIdx>().value() + num_atoms_till_i;
                        int idx1 = three.atom1().asA<AtomIdx>().value() + num_atoms_till_i;
                        int idx2 = three.atom2().asA<AtomIdx>().value() + num_atoms_till_i;
                        double astart = three.initialForms()[Symbol("k")].toString().toDouble();
                        double thetastart = three.initialForms()[Symbol("theta0")].toString().toDouble();
                        double aend = three.finalForms()[Symbol("k")].toString().toDouble();
                        double thetaend = three.finalForms()[Symbol("theta0")].toString().toDouble();
                        
                        solute_angle_perturbation_params[0] = astart * OpenMM::KJPerKcal; 
                        solute_angle_perturbation_params[1] = aend * OpenMM::KJPerKcal;
                        solute_angle_perturbation_params[2] = thetastart;
                        solute_angle_perturbation_params[3] = thetaend;
                        
                        solute_angle_perturbation->addAngle(idx0,idx1,idx2,solute_angle_perturbation_params);
                        
                        angle_pert_list.append(AngleID(three.atom0(),three.atom1(),three.atom2()));
                        angle_pert_swap_list.append(AngleID(three.atom2(),three.atom1(),three.atom0()));

                        if(Debug){
                            qDebug() << "Atom0 = " << three.atom0().asA<AtomIdx>().value() << 
                                        "Atom1 = "<< three.atom1().asA<AtomIdx>().value() << 
                                        "Atom2 = "<< three.atom2().asA<AtomIdx>().value() ;
                            
                            qDebug() << "IDX0 = " << idx0 <<  "IDX1 = " << idx1 <<  "IDX2 = " << idx2 << "\n";

                            qDebug() << "astart = " << astart <<
                                        "aend = " << aend;

                            qDebug() << "thetastart = " << thetastart <<
                                        "thetaend = " << thetaend << "\n";
                        }
                    }
                    if(str == "SireMM::FourAtomPerturbation"){

                        const FourAtomPerturbation &four = pert.asA<FourAtomPerturbation>();
                        int idx0 = four.atom0().asA<AtomIdx>().value() + num_atoms_till_i;
                        int idx1 = four.atom1().asA<AtomIdx>().value() + num_atoms_till_i;
                        int idx2 = four.atom2().asA<AtomIdx>().value() + num_atoms_till_i;
                        int idx3 = four.atom3().asA<AtomIdx>().value() + num_atoms_till_i;
                        
                        QString tmp = four.perturbExpression().toOpenMMString();
                        tmp.replace(QString("phi"),QString("theta"));
                        tmp = "( " + tmp + " ) * " + "KJPerKcal";
                        
                        std::string openmm_str = tmp.toStdString();

                        if(Debug){
                            qDebug() << "IDX0 = " << idx0 <<  "IDX1 = " << idx1 <<  "IDX2 = " << idx2 <<  "IDX3 = " << idx3;
                            qDebug() << "Dihedral String = " << openmm_str.c_str();
                            qDebug() << "Dihedral Normal String = " << four.perturbExpression().toString() << "\n";
                        }

                        OpenMM::CustomTorsionForce* solute_torsion_perturbation = NULL;
 
                        solute_torsion_perturbation = new OpenMM::CustomTorsionForce(openmm_str);
                        solute_torsion_perturbation->addPerTorsionParameter("KJPerKcal");
                        solute_torsion_perturbation_params[0]=4.184;
                        solute_torsion_perturbation->addGlobalParameter("lambda",Alchemical_value);
                        solute_torsion_perturbation->addTorsion(idx0,idx1,idx2,idx3,solute_torsion_perturbation_params);

                        //********************************BONDED ENERGY TORSIONS ARE ADDED TO THE SYSTEM*****************************
                        system_openmm->addForce(solute_torsion_perturbation);
                        
                        perturbed_energies_tmp[7] = true; //Torsions are added to the system

                        dihedral_pert_list.append(DihedralID(four.atom0(),four.atom1(),four.atom2(),four.atom3()));
                        dihedral_pert_swap_list.append(DihedralID(four.atom3(),four.atom1(),four.atom2(),four.atom0()));
                        
                        improper_pert_list.append(ImproperID(four.atom0(),four.atom1(),four.atom2(),four.atom3()));
                        improper_pert_swap_list.append(ImproperID(four.atom0(),four.atom1(),four.atom3(),four.atom2()));

                        if(Debug){
                            qDebug() << "Atom0 = " << four.atom0().asA<AtomIdx>().value() << 
                                        "Atom1 = " << four.atom1().asA<AtomIdx>().value() << 
                                        "Atom2 = " << four.atom2().asA<AtomIdx>().value() << 
                                        "Atom3 = " << four.atom3().asA<AtomIdx>().value() << "\n";
                        }
                    }

                }
            }//end for perturbations

        }//end solute molecule perturbation


        // The bonded parameters are stored in "amberparameters"
        AmberParameters amber_params = molecule.property("amberparameters").asA<AmberParameters>();
        QList<BondID> bonds_ff = amber_params.getAllBonds();
        QVector<BondID> bonds = bonds_ff.toVector();
        
        //BOND

        for (int j=0; j < bonds_ff.length() ; j++){

            BondID bond_ff = bonds_ff[j];
            QList<double> bond_params = amber_params.getParams(bond_ff);
            double k = bond_params[0];
            double r0 = bond_params[1];

            int idx0 = bonds[j].atom0().asA<AtomIdx>().value();
            int idx1 = bonds[j].atom1().asA<AtomIdx>().value();

            if(molecule.isSameMolecule(solutemol)){

                if(bond_pert_list.indexOf(bond_ff) != -1 || bond_pert_swap_list.indexOf(bond_ff) != -1 ){//Solute molecule. Check if the current solute bond is in the perturbed bond list
                    if(Debug)
                        qDebug() << "Found Perturbed Bond\n";
                    continue;
                }
                else{
                    if(Debug)
                        qDebug() << "Solute normal Bond - Atom0 = "<< idx0 << "Atom1 = " << idx1 << " r0 = "<< r0 << "k = " << k <<"\n";

                    idx0 = idx0 + num_atoms_till_i;
                    idx1 = idx1 + num_atoms_till_i;
                    bondStretch_openmm->addBond(idx0,idx1,r0 * OpenMM::NmPerAngstrom, k * 2.0 * OpenMM::KJPerKcal * OpenMM::AngstromsPerNm * OpenMM::AngstromsPerNm);
                    bondPairs.push_back(std::make_pair(idx0,idx1));
                    continue;
                }

            }

            //Select the atom type
            QString atom0 =  molecule.atom(AtomIdx(idx0)).toString();
            QString atom1 =  molecule.atom(AtomIdx(idx1)).toString();

            idx0 = idx0 + num_atoms_till_i;
            idx1 = idx1 + num_atoms_till_i;

            if(flag_constraint == NONE ){

                bondStretch_openmm->addBond(idx0,idx1,r0 * OpenMM::NmPerAngstrom, k * 2.0 * OpenMM::KJPerKcal * OpenMM::AngstromsPerNm * OpenMM::AngstromsPerNm); 

                //cout << "\nBOND ADDED TO "<< atom0.toStdString() << " AND " << atom1.toStdString() << "\n";
            }
            else if ( flag_constraint == ALLBONDS || flag_constraint == HANGLES ){
                system_openmm->addConstraint(idx0,idx1, r0 *  OpenMM::NmPerAngstrom);
                //cout << "\nALLBONDS or HANGLES ADDED BOND CONSTRAINT TO " << atom0.toStdString() << " AND " << atom1.toStdString() << "\n";
            }
            else if ( flag_constraint == HBONDS ){

                if ( (atom0[6] == 'H') || (atom1[6] == 'H') ){
                    system_openmm->addConstraint(idx0,idx1, r0 *  OpenMM::NmPerAngstrom);
                    //cout << "\nHBONDS ADDED BOND CONSTRAINT TO " << atom0.toStdString() << " AND " << atom1.toStdString() << "\n";
                }
                else{
                    bondStretch_openmm->addBond(idx0,idx1,r0 * OpenMM::NmPerAngstrom, k * 2.0 * OpenMM::KJPerKcal * OpenMM::AngstromsPerNm * OpenMM::AngstromsPerNm);
                    //cout << "\nHBONDS ADDED BOND TO " << atom0.toStdString() << " AND " << atom1.toStdString() << "\n";
                }
            }

            //Bond exclusion List
            bondPairs.push_back(std::make_pair(idx0,idx1));
        }


        //Angles

        QList<AngleID> angles_ff = amber_params.getAllAngles();
        QVector<AngleID> angles = angles_ff.toVector();

        for (int j=0; j < angles_ff.length() ; j++){

            AngleID angle_ff = angles_ff[j];
            QList<double> angle_params = amber_params.getParams(angle_ff);


            double k = angle_params[0];
            double theta0 = angle_params[1];// It is already in radiant

            int idx0 = angles[j].atom0().asA<AtomIdx>().value();
            int idx1 = angles[j].atom1().asA<AtomIdx>().value();
            int idx2 = angles[j].atom2().asA<AtomIdx>().value();

            if(molecule.isSameMolecule(solutemol)){

             if(angle_pert_list.indexOf(angle_ff) != -1 || angle_pert_swap_list.indexOf(angle_ff) != -1){//Solute molecule. Check if the current solute angle is in the perturbed angle list
                    if(Debug)
                        qDebug() << "Found Perturbed Angle\n";
                    continue;
                }
                else{

                    if(Debug)
                        qDebug() << "Solute normal Angle - Atom0 = "<< idx0 << "Atom1 = " << idx1 << "Atom2 = " << idx2 <<"theta0 = "<< theta0 <<" k = "<< k << "\n";

                    idx0 = idx0 + num_atoms_till_i;
                    idx1 = idx1 + num_atoms_till_i;
                    idx2 = idx2 + num_atoms_till_i;
                    bondBend_openmm->addAngle(idx0,idx1,idx2, theta0 , k * 2.0 * OpenMM::KJPerKcal);
                    continue;
                }
            }
            if(Debug)
                qDebug() << "Angle - Atom0 = "<< idx0 << "Atom1 = " << idx1 << "Atom2 = " << idx2 <<"\n";

            QString atom0 =  molecule.atom(AtomIdx(idx0)).toString();
            QString atom1 =  molecule.atom(AtomIdx(idx1)).toString();
            QString atom2 =  molecule.atom(AtomIdx(idx2)).toString();


            Vector diff = c[idx2] - c[idx0];

            idx0 = idx0 + num_atoms_till_i;
            idx1 = idx1 + num_atoms_till_i;
            idx2 = idx2 + num_atoms_till_i;

            if ( flag_constraint == HANGLES ){

                if( ((atom0[6] == 'H') && (atom2[6] == 'H')) ){
                    system_openmm->addConstraint(idx0,idx2, diff.length() *  OpenMM::NmPerAngstrom);
                }
                else if ( ((atom0[6] == 'H') && (atom1[6] == 'O')) || ((atom1[6] == 'O') && (atom2[6] == 'H')) ){
                    system_openmm->addConstraint(idx0,idx2, diff.length() *  OpenMM::NmPerAngstrom);
                }
                else{
                    bondBend_openmm->addAngle(idx0,idx1,idx2, theta0 , k * 2.0 * OpenMM::KJPerKcal);
                }
            }
            else{
                bondBend_openmm->addAngle(idx0,idx1,idx2, theta0 , k * 2.0 * OpenMM::KJPerKcal);
            }
        }//end of angles

        //Dihedrals

        QList<DihedralID> dihedrals_ff = amber_params.getAllDihedrals();
        QVector<DihedralID> dihedrals = dihedrals_ff.toVector();

        for (int j=0; j < dihedrals_ff.length() ; j++ ){

            DihedralID dihedral_ff = dihedrals_ff[j];
            QList<double> dihedral_params = amber_params.getParams(dihedral_ff);

            int idx0 = dihedrals[j].atom0().asA<AtomIdx>().value() + num_atoms_till_i;
            int idx1 = dihedrals[j].atom1().asA<AtomIdx>().value() + num_atoms_till_i;
            int idx2 = dihedrals[j].atom2().asA<AtomIdx>().value() + num_atoms_till_i;
            int idx3 = dihedrals[j].atom3().asA<AtomIdx>().value() + num_atoms_till_i;


            if(Debug){
                qDebug() << "TOTAL Dihedral between atom global index " << idx0 - num_atoms_till_i << 
                                " and " << idx1 - num_atoms_till_i << 
                                " and " << idx2 - num_atoms_till_i << 
                                " and " << idx3 - num_atoms_till_i <<"\n";
            }

            if(molecule.isSameMolecule(solutemol)){//Solute molecule. Check if the current solute dihedral is in the perturbed dihedral list
                if(dihedral_pert_list.indexOf(dihedral_ff)!=-1 || dihedral_pert_swap_list.indexOf(dihedral_ff)!=-1){
                    if(Debug)
                        qDebug() << "Found Perturbed Dihedral\n";
                    continue;
                }
            }

            // Variable number of parameters
            for (int k=0 ; k < dihedral_params.length() ; k = k + 3 ){
                double v = dihedral_params[ k ];
                int periodicity = dihedral_params[ k + 1 ];
                double phase = dihedral_params[ k + 2 ];
                bondTorsion_openmm->addTorsion(idx0, idx1, idx2, idx3, periodicity, phase , v * OpenMM::KJPerKcal);
                if(Debug){
                        qDebug() << "Dihedral between atom global index " << idx0 - num_atoms_till_i << 
                                " and " << idx1 - num_atoms_till_i << 
                                " and " << idx2 - num_atoms_till_i << 
                                " and " << idx3 - num_atoms_till_i <<"\n";
                        qDebug() << "Amplitude_dih = " << v << " periodicity " << periodicity << " phase " << phase<<"\n";
                }
            }
        } // end of dihedrals


        //Improper Dihedrals

        QList<ImproperID> impropers_ff = amber_params.getAllImpropers();
        QVector<ImproperID> impropers = impropers_ff.toVector();

        for (int j=0; j < impropers_ff.length() ; j++ ){
            ImproperID improper_ff = impropers_ff[j];
            QList<double> improper_params = amber_params.getParams(improper_ff);

    
            int idx0 = impropers[j].atom0().asA<AtomIdx>().value() + num_atoms_till_i;
            int idx1 = impropers[j].atom1().asA<AtomIdx>().value() + num_atoms_till_i;
            int idx2 = impropers[j].atom2().asA<AtomIdx>().value() + num_atoms_till_i;
            int idx3 = impropers[j].atom3().asA<AtomIdx>().value() + num_atoms_till_i;
            
            if(Debug){
                        qDebug() << "TOTAL Improper between atom global index " << idx0 - num_atoms_till_i<< 
                                " and " << idx1 - num_atoms_till_i << 
                                " and " << idx2 - num_atoms_till_i << 
                                " and " << idx3 - num_atoms_till_i <<"\n";
                       
            }


            if(molecule.isSameMolecule(solutemol)){//Solute molecule. Check if the current solute dihedral is in the perturbed improper list
                if(improper_pert_list.indexOf(improper_ff)!=-1 || improper_pert_swap_list.indexOf(improper_ff)!=-1){
                    if(Debug)
                        qDebug() << "Found Perturbed Improper\n";
                    continue;
                }
            }

            // Variable number of parameters
            for (int k=0 ; k < improper_params.length() ; k = k + 3 ){
                double v = improper_params[ k ];
                double periodicity = improper_params[ k + 1 ];
                double phase = improper_params[ k + 2 ];

                bondTorsion_openmm->addTorsion(idx0, idx1, idx2, idx3, periodicity, phase , v * OpenMM::KJPerKcal);
                if(Debug){
                        qDebug() << "Improper between atom global index " << idx0 - num_atoms_till_i<< 
                                " and " << idx1 - num_atoms_till_i << 
                                " and " << idx2 - num_atoms_till_i << 
                                " and " << idx3 - num_atoms_till_i <<"\n";
                        qDebug() << "Amplitude_imp = " << v << " periodicity " << periodicity << " phase " << phase<<"\n";
                }
            }
        }//end of impropers

        num_atoms_till_i = num_atoms_till_i + num_atoms_molecule ;


    }// end of loop over molecules

    if(Debug){
        if(nions!=0)
            qDebug() << "\n\nNumber of ions = " << nions << "\n\n";
    }

    //Exclude the 1-2, 1-3 bonded atoms from nonbonded forces, and scale down 1-4 bonded atoms

    const double Coulomb14Scale = 1.0/1.2;

    const double LennardJones14Scale = 1.0/2.0;

    nonbond_openmm->createExceptionsFromBonds(bondPairs, Coulomb14Scale, LennardJones14Scale);

    if(CMMremoval_frequency > 0){
        OpenMM::CMMotionRemover * cmmotionremover = new OpenMM::CMMotionRemover(CMMremoval_frequency);

        system_openmm->addForce(cmmotionremover);

        if (Debug)
            qDebug() << "\n\nWill remove Center of Mass motion every " << CMMremoval_frequency << " steps\n\n";
    }


    int num_exceptions = nonbond_openmm->getNumExceptions();

    if(Debug)
        qDebug() << "NUM EXCEPTIONS = " << num_exceptions << "\n";
    

    for(int i=0;i<num_exceptions;i++){

            int p1,p2;
            
            double charge_prod,sigma_avg,epsilon_avg;


            nonbond_openmm->getExceptionParameters(i,p1,p2,charge_prod,sigma_avg,epsilon_avg);

            if(Debug)
                qDebug() << "Exception = " << i << " p1 = " << p1 << " p2 = " << p2 << " charge prod = " << charge_prod << " sigma avg = " << sigma_avg << " epsilon_avg = " << epsilon_avg << "\n";

            if(!(charge_prod==0 && sigma_avg==1 && epsilon_avg==0)){//1-4 interactions
                
                QVector<double> perturbed_14_tmp(13);
                
                std::vector<double> p1_params(9);
                std::vector<double> p2_params(9);
                
                custom_force_field->getParticleParameters(p1,p1_params);
                custom_force_field->getParticleParameters(p2,p2_params);
                
                double Qstart_p1 = p1_params[0];
                double Qend_p1 = p1_params[1];
                double Epstart_p1 = p1_params[2];
                double Epend_p1 = p1_params[3];
                double Sigstart_p1 = p1_params[4];
                double Sigend_p1 = p1_params[5];
                double isHard_p1 = p1_params[6];
                double isTodummy_p1 = p1_params[7];
                double isFromdummy_p1 = p1_params[8];
              
                double Qstart_p2 = p2_params[0];
                double Qend_p2 = p2_params[1];
                double Epstart_p2 = p2_params[2];
                double Epend_p2 = p2_params[3];
                double Sigstart_p2 = p2_params[4];
                double Sigend_p2 = p2_params[5];
                double isHard_p2 = p2_params[6];
                double isTodummy_p2 = p2_params[7];
                double isFromdummy_p2 = p2_params[8];
                
                double charge_prod_start,charge_prod_end,charge_prod_mix;
                double sigma_avg_start, sigma_avg_end;
                double epsilon_avg_start,epsilon_avg_end,epsilon_avg_mix;
                
                
                charge_prod_start = Qstart_p1 * Qstart_p2 * Coulomb14Scale;
                charge_prod_end = Qend_p1 * Qend_p2 * Coulomb14Scale;
                charge_prod_mix = (Qend_p1 * Qstart_p2 + Qstart_p1 * Qend_p2)  * Coulomb14Scale;
                
                sigma_avg_start = (Sigstart_p1 + Sigstart_p2)/2.0;
                sigma_avg_end = (Sigend_p1 + Sigend_p2)/2.0;
                
                epsilon_avg_start = Epstart_p1 * Epstart_p2 * LennardJones14Scale * LennardJones14Scale;
                epsilon_avg_end = Epend_p1 * Epend_p2 * LennardJones14Scale * LennardJones14Scale;
                epsilon_avg_mix = (Epend_p1 * Epstart_p2 + Epstart_p1 * Epend_p2) * LennardJones14Scale * LennardJones14Scale;
                

                std::vector<double> params(8);
            
                params[0] = charge_prod_start;
                params[1] = charge_prod_end;
                params[2] = charge_prod_mix;
                params[3] = epsilon_avg_start;
                params[4] = epsilon_avg_end;
                params[5] = epsilon_avg_mix;
                params[6] = sigma_avg_start;
                params[7] = sigma_avg_end;
                
                
                if(Debug){
                    
                    qDebug() << "Particle p1 = " << p1 << "\nQstart = " << Qstart_p1 << "\nQend = " << Qend_p1
                             << "\nEpstart = " << Epstart_p1 << "\nEpend = " << Epend_p1
                             << "\nSgstart = " << Sigstart_p1 << "\nSgend = " << Sigend_p1
                             << "\nisHard = " << isHard_p1 << "\nisTodummy = " << isTodummy_p1 << "\nisFromdummy = " << isFromdummy_p1 <<"\n";
                    qDebug() << "Particle p2 = " << p2 << "\nQstart = " << Qstart_p2 << "\nQend = " << Qend_p2
                             << "\nEpstart = " << Epstart_p2 << "\nEpend = " << Epend_p2
                             << "\nSgstart = " << Sigstart_p2 << "\nSgend = " << Sigend_p2
                             << "\nisHard = " << isHard_p2 << "\nisTodummy = " << isTodummy_p2 << "\nisFromdummy = " << isFromdummy_p2 <<"\n";
                    
                    
                    
                    qDebug() << "Product Charge start = " << charge_prod_start << "\nProduct Charge end = " << charge_prod_end << "\nProduct Chrage mixed = " << charge_prod_mix
                             << "\nEpsilon average start = " << epsilon_avg_start << "\nEpsilon average end = " << epsilon_avg_end << "\nEpsilon average mixed = " << charge_prod_mix
                             << "\nSigma average start = " << sigma_avg_start << "\nSigma average end = " << sigma_avg_end << "\n";
                }

                if((isHard_p1 == 1.0 && isHard_p2 == 1.0)){
                    
                    custom_intra_14_clj->addBond(p1,p2,params);
    

                    if(Debug)
                        qDebug() << "Added clj Hard 1-4\n";
                }
                else if((isTodummy_p1 == 1.0 && isTodummy_p2 == 1.0) || (isHard_p1 == 1.0 && isTodummy_p2 == 1.0) || (isHard_p2 == 1.0 && isTodummy_p1 == 1.0)){

                    custom_intra_14_todummy->addBond(p1,p2,params);
                
                    if(Debug)
                        qDebug() << "Added soft TO dummy 1-4\n";
                }
                
                else if((isFromdummy_p1 == 1.0 && isFromdummy_p2 == 1.0) || (isHard_p1 == 1.0 && isFromdummy_p2 == 1.0) || (isHard_p2 == 1.0 && isFromdummy_p1 == 1.0)){
                    
                    custom_intra_14_fromdummy->addBond(p1,p2,params);
                                        
                    if(Debug)
                        qDebug() << "Added soft FROM dummy 1-4\n";

                }
                
                else if((isFromdummy_p1 == 1.0 && isTodummy_p2 == 1.0) || (isFromdummy_p2 == 1.0 && isTodummy_p1 == 1.0)) {

                    custom_intra_14_fromdummy_todummy->addBond(p1,p2,params);

                    if(Debug)
                        qDebug() << "Added soft FROM dummy TO dummy 1-4\n";

                }


            }//end if 1-4 interactions

            custom_force_field->addExclusion(p1,p2);

    }

    /***********************************************************************NON BONDED INTERACTIONS*****************************************************************/

    int npairs = (custom_force_field->getNumParticles() * (custom_force_field->getNumParticles() - 1))/2;

    if(Debug){
        qDebug() << "Num pairs  = " << npairs;
        qDebug() << "Num bonds 1-4 Hard = " << custom_intra_14_clj->getNumBonds();
        qDebug() << "Num bonds 1-4 To Dummy = " << custom_intra_14_todummy->getNumBonds();
        qDebug() << "Num bonds 1-4 From Dummy = " << custom_intra_14_fromdummy->getNumBonds();
        qDebug() << "Num bonds 1-4 From Dummy To Dummy = " << custom_intra_14_fromdummy_todummy->getNumBonds();
    }

    if(npairs != num_exceptions){
        system_openmm->addForce(custom_force_field);
        perturbed_energies_tmp[0] = true;//Custom non bonded 1-5 is added to the system 
        if(Debug)
            qDebug() << "Added 1-5";
    }
    
    if(custom_intra_14_clj->getNumBonds() != 0){
        system_openmm->addForce(custom_intra_14_clj);
        perturbed_energies_tmp[1] = true;//Custom non bonded 1-4 is added to the system
        if(Debug)
            qDebug() << "Added 1-4 CLJ";
    }
    
    if(custom_intra_14_todummy->getNumBonds() != 0){
        system_openmm->addForce(custom_intra_14_todummy);
        perturbed_energies_tmp[2] = true;//Custom non bonded 1-4 is added to the system
        if(Debug)
            qDebug() << "Added 1-4 To Dummy";
    }
    
    
    if(custom_intra_14_fromdummy->getNumBonds() != 0){
        system_openmm->addForce(custom_intra_14_fromdummy);
        perturbed_energies_tmp[3] = true;//Custom non bonded 1-4 is added to the system
        if(Debug)
            qDebug() << "Added 1-4 From Dummy";

    }
    if(custom_intra_14_fromdummy_todummy->getNumBonds() != 0){
        system_openmm->addForce(custom_intra_14_fromdummy_todummy);
        perturbed_energies_tmp[4] = true;//Custom non bonded 1-4 is added to the system
        if(Debug)
            qDebug() << "Added 1-4 From Dummy To Dummy";
    }

    /***********************************************************************BONDED INTERACTIONS*****************************************************************/

    if(bondStretch_openmm->getNumBonds() != 0){
        system_openmm->addForce(bondStretch_openmm);
        if(Debug)
            qDebug() << "Added Internal Bond energy term";
    }

    if(bondBend_openmm->getNumAngles() != 0){
        system_openmm->addForce(bondBend_openmm);
        if(Debug)
            qDebug() << "Added Internal Angle energy term";
    }

    if(bondTorsion_openmm->getNumTorsions() != 0){
        system_openmm->addForce(bondTorsion_openmm);
        if(Debug)
            qDebug() << "Added Internal Torsion energy term";
    }

    if(solute_bond_perturbation->getNumBonds() != 0){
        system_openmm->addForce(solute_bond_perturbation);
        perturbed_energies_tmp[5] = true;//Custom bonded is added to the system
        if(Debug)
            qDebug() << "Added Perturbed Internal Bond energy term";
    }

    if(solute_angle_perturbation->getNumAngles() != 0){
        system_openmm->addForce(solute_angle_perturbation);
        perturbed_energies_tmp[6] = true;//Custom bonded is added to the system
        if(Debug)
            qDebug() << "Added Perturbed Internal Angle energy term";
    }
    
    
    perturbed_energies = perturbed_energies_tmp;

    //IMPORTANT: PERTURBED ENERGY TORSIONS ARE ADDED ABOVE
    
    this->openmm_system = system_openmm;
    this->isInitialised = true;

}

void OpenMMFrEnergyST::integrate(IntegratorWorkspace &workspace, const Symbol &nrg_component, SireUnits::Dimension::Time timestep, int nmoves, bool record_stats) {

    bool Debug = true; 

    QTime timer;

    timer.start();
    
    if (Debug)
        qDebug() << "In OpenMMFrEnergyST::integrate()\n\n" ;

    // Check that the openmm system has been initialised
    // !! Should check that the workspace is compatible with molgroup
    if ( not this->isInitialised){

    qDebug() << "Not initialised ! ";
    throw SireError::program_bug(QObject::tr(
        "OpenMMFrEnergyST should have been initialised before calling integrate."), CODELOC);
    }

    OpenMM::System *system_openmm = openmm_system;

    int nats = system_openmm->getNumParticles();

    if (Debug)
        qDebug() << " openmm nats " << nats;


    // Integrator 
    
    const double dt = convertTo( timestep.value(), picosecond);
    const double converted_Temperature = convertTo(Temperature.value(), kelvin);
    const double converted_friction = convertTo( friction.value(), picosecond);


    OpenMM::Integrator * integrator_openmm = NULL;

    if (Integrator_type == "leapfrogverlet")
        integrator_openmm = new OpenMM::VerletIntegrator(dt);//dt in picosecond
    else if (Integrator_type == "variableleapfrogverlet")
        integrator_openmm = new OpenMM::VariableVerletIntegrator(integration_tol);//integration tolerance error unitless
    else if (Integrator_type == "langevin")
         integrator_openmm = new OpenMM::LangevinIntegrator(converted_Temperature, converted_friction, dt);
    else if (Integrator_type == "variablelangevin")
        integrator_openmm = new OpenMM::VariableLangevinIntegrator(converted_Temperature, converted_friction, integration_tol);
    else if (Integrator_type == "brownian")
        integrator_openmm = new OpenMM::BrownianIntegrator(converted_Temperature, converted_friction, dt);
    else 
        throw SireError::program_bug(QObject::tr("The user defined Integrator type is not supported. Available types are leapfrogverlet, variableleapfrogverlet, langevin, variablelangevin, brownian"), CODELOC);

    if (Debug){
        qDebug() << "Using Integrator: " << Integrator_type;

        qDebug() << "Integration step = " << dt <<" ps";

        if(Integrator_type == "variablelangevin" || Integrator_type == "variableleapfrogverlet"){
            qDebug() << "Integration Tol = " << integration_tol;
        }
        if(Integrator_type == "langevin" || Integrator_type == "variablelangevin" || Integrator_type == "brownian"){
            qDebug() << "Converted Friction = " << converted_friction << "1/ps";
        }
     }

    OpenMM::Platform& platform_openmm = OpenMM::Platform::getPlatformByName(platform_type.toStdString());

    if (platform_type == "OpenCL"){

        const std::string prop = std::string("OpenCLDeviceIndex");
        const std::string prec = std::string("OpenCLPrecision");

        platform_openmm.setPropertyDefaultValue(prop, device_index.toStdString() );
        platform_openmm.setPropertyDefaultValue(prec, precision.toStdString() );
        
        if (Debug){
            qDebug() << "Setting up OpenCL default Index to " << device_index;
            qDebug() << "Setting up OpenCL precision to" << precision;
        }
    }
    else if (platform_type == "CUDA"){
        const std::string prop = std::string("CudaDeviceIndex");
        const std::string prec = std::string("CudaPrecision");
        platform_openmm.setPropertyDefaultValue(prop, device_index.toStdString() );
        platform_openmm.setPropertyDefaultValue(prec, precision.toStdString() );

        if (Debug){
            qDebug() << "Setting up CUDA default Index to " << device_index;
            qDebug() << "Setting up CUDA precision to" << precision;
        }
        
    }
    // JM Dec 12. Do we need to set for Reference?

    // Creating a context is the bottleneck in the setup step 
    // Another implementation could have the context created once during initialisation. 
    // But then have to figure out how to properly allocate/free context on the heap and make it 
    // compatible with sire objects
    OpenMM::Context context_openmm( *system_openmm, *integrator_openmm, platform_openmm);  

    if (Debug)
        qDebug() << "\n Using OpenMM platform = " <<context_openmm.getPlatform().getName().c_str()<<"\n";

    // Now update coordinates / velocities / dimensions with sire data

    //OpenMM vector coordinate
    std::vector<OpenMM::Vec3> positions_openmm(nats);

    //OpenMM vector momenta
    std::vector<OpenMM::Vec3> velocities_openmm(nats);

    AtomicVelocityWorkspace &ws = workspace.asA<AtomicVelocityWorkspace>();

    // Conversion factor because sire units of time are in AKMA, whereas OpenMM uses picoseconds

    double AKMAPerPs = 0.04888821;
    double PsPerAKMA = 1 / AKMAPerPs;

    const int nmols = ws.nMolecules();

    int system_index = 0;

    for (int i=0; i < nmols; ++i){

        const int nats_mol = ws.nAtoms(i);

        Vector *c = ws.coordsArray(i);
        Vector *p = ws.momentaArray(i);
        const double *m = ws.massArray(i);

        for (int j=0; j < nats_mol; ++j){

            positions_openmm[system_index] = OpenMM::Vec3(c[j].x() * (OpenMM::NmPerAngstrom), c[j].y() * (OpenMM::NmPerAngstrom), c[j].z() * (OpenMM::NmPerAngstrom));

            if (m[j] > SireMaths::small){
                velocities_openmm[system_index] = OpenMM::Vec3(p[j].x()/m[j] * (OpenMM::NmPerAngstrom) * PsPerAKMA,p[j].y()/m[j] * (OpenMM::NmPerAngstrom) * PsPerAKMA,p[j].z()/m[j] * (OpenMM::NmPerAngstrom) * PsPerAKMA);
            }
            else{
                velocities_openmm[system_index] =  OpenMM::Vec3(0.0, 0.0, 0.0);
            }
            
            if(Debug){
            
	      //qDebug() << "Particle num " << system_index;
	      //qDebug() << "X = " << positions_openmm[system_index][0] << " Y = " << positions_openmm[system_index][1] << " Z = " << positions_openmm[system_index][2];
	      //qDebug() << "Vx = " << velocities_openmm[system_index][0] << " Vy = " << velocities_openmm[system_index][1] << " Vz = " << velocities_openmm[system_index][2] << "\n";
            }
            
            
            system_index++;
        }
    }

    if ( system_index != nats ){
        if (Debug)
            qDebug() << " system_index " << system_index << " nats " << nats;
        throw SireError::program_bug(QObject::tr("The number of atoms in the openmm system does not match the number of atoms in the sire workspace"), CODELOC);
    }

    context_openmm.setPositions(positions_openmm);  
    context_openmm.setVelocities(velocities_openmm);

    if ( CutoffType == "cutoffperiodic" ){

        const System & ptr_sys = ws.system();
        const PropertyName &space_property = PropertyName("space");
        const PeriodicBox &space = ptr_sys.property(space_property).asA<PeriodicBox>();

        const double Box_x_Edge_Length = space.dimensions()[0] * OpenMM::NmPerAngstrom; //units in nm
        const double Box_y_Edge_Length = space.dimensions()[1] * OpenMM::NmPerAngstrom; //units in nm
        const double Box_z_Edge_Length = space.dimensions()[2] * OpenMM::NmPerAngstrom; //units in nm

        if (Debug)
            qDebug() << "\nBOX SIZE [A] = (" << space.dimensions()[0] << " , " << space.dimensions()[1] << " ,  " << space.dimensions()[2] << ")\n\n";

        //Set Periodic Box Condition

        context_openmm.setPeriodicBoxVectors( OpenMM::Vec3(Box_x_Edge_Length,0,0),OpenMM::Vec3(0,Box_y_Edge_Length,0),OpenMM::Vec3(0,0,Box_z_Edge_Length) );
    }

    if (Debug)
        qDebug() << " Setup dynamics, time elapsed ms " << timer.elapsed() << " ms ";

    /** Now perform some steps of dynamics */
        timer.restart();

    if (Debug)
        qDebug() << " Doing " << nmoves << " steps of dynamics ";

    
    int n_samples = nmoves / energy_frequency;

    if(nmoves < energy_frequency)
        throw SireError::program_bug(QObject::tr("You are requesting to save energy every  %1 steps, which is above the total number of %2 steps.").arg(energy_frequency, nmoves), CODELOC);

    if(Debug)
        qDebug() << "Number Energy Samples = "<< n_samples << "\n\n";

    int MAXSAMPLES = 1000;

    // Limit excessive internal buffering
    if ( buffer_coords){
        if  ( n_samples > MAXSAMPLES ){
            throw SireError::program_bug(QObject::tr("You are requesting to buffer %1 frames, which is above the hardcoded limit of %2.").arg(n_samples, MAXSAMPLES), CODELOC);
        }
    }
    
    QVector< std::vector<OpenMM::Vec3> > buffered_positions;
    QVector< Vector > buffered_dimensions;


    OpenMM::Vec3 a;
    OpenMM::Vec3 b;
    OpenMM::Vec3 c;


    const double beta = 1.0 / (0.0083144621 * convertTo(Temperature.value(), kelvin)); //mol/kJ

    int infoMask = 0;

    infoMask = OpenMM::State::Positions;
    infoMask = infoMask + OpenMM::State::Velocities; 
    infoMask = infoMask +  OpenMM::State::Energy;

    OpenMM::State state_openmm;//OpenMM State

    double GF_acc = 0.0;

    double GB_acc = 0.0;

    double sample_count=0.0;

    /*state_openmm=context_openmm.getState(infoMask);
    qDebug() << "TOTAL Energy = " <<  state_openmm.getPotentialEnergy() * OpenMM::KcalPerKJ + state_openmm.getKineticEnergy() * OpenMM::KcalPerKJ << " kcal/mol";
    qDebug() << "Potential Energy = " <<  state_openmm.getPotentialEnergy() * OpenMM::KcalPerKJ << " kcal/mol";
    qDebug() << "Kinetic Energy = " <<  state_openmm.getKineticEnergy() * OpenMM::KcalPerKJ << " kcal/mol";
    return;*/
    //qDebug()  <<"*Lambda = " << Alchemical_value << " Potential energy lambda  = " << QString::number(state_openmm.getPotentialEnergy() * OpenMM::KcalPerKJ,'g',9) << " [A + A^2] kcal" << "\n";


    if(buffer_coords)
        qDebug() << "Saving atom coordinates every " << energy_frequency << "\n";


    //show_status(sample_count, n_samples);

    if(Debug){
        for( int i=0;i<perturbed_energies.size();i++)
            qDebug() << "Perturbed energy flag index" << i << " Value = " << perturbed_energies[i];
    }


/*    vector<double> center(3);
    center[0] = 0.0;
    center[1] = 0.0;
    center[2] = 0.0; 

    DCD dcd("traj.dcd", true,CUTOFFPERIODIC, center, energy_frequency, nats, dt,  context_openmm, ws );*/


    //Time skipping
    const double time_skip = convertTo( timeskip.value(), picosecond);

    if(time_skip != 0.0){

        if(Debug)
            qDebug() << "Time to Skip = " << time_skip << "ps";

        int new_nmoves = time_skip / dt;

        if(new_nmoves >= nmoves){
            throw SireError::program_bug(QObject::tr("Time to Skip is greater than the simulation time"), CODELOC);
            exit(-1);
        }

        integrator_openmm->step(new_nmoves);

        n_samples = (nmoves - new_nmoves) / energy_frequency;
    }

    while(sample_count<n_samples){

        //*********************MD STEPS****************************
        integrator_openmm->step(energy_frequency);

        //dcd.write((int) sample_count);//Save a frame

        state_openmm=context_openmm.getState(infoMask);

        if(Debug)
            qDebug()<< "Total Time = " << state_openmm.getTime() << " ps";


        double potential_energy_lambda = state_openmm.getPotentialEnergy();

        double potential_energy_lambda_plus_delta;

        double potential_energy_lambda_minus_delta;

        double plus;

        double minus;

        if(buffer_coords){

            positions_openmm = state_openmm.getPositions();
            buffered_positions.append( positions_openmm );

            if (MCBarostat_flag == true){
                state_openmm.getPeriodicBoxVectors(a,b,c);
                Vector dims = Vector( a[0] * OpenMM::AngstromsPerNm, b[1] * OpenMM::AngstromsPerNm, c[2] * OpenMM::AngstromsPerNm);
                buffered_dimensions.append( dims );
            }
        }

        if(Debug){
            qDebug() << "\nLambda = " << Alchemical_value;
            printf("*Potential energy lambda = %f kcal/mol\n" , state_openmm.getPotentialEnergy() * OpenMM::KcalPerKJ);
        }


        if(potential_energy_lambda > 1000000.0){
            throw SireError::program_bug(QObject::tr("********************* Energy Too High. Error! *******************"), CODELOC);
            exit(-1);
        }

        if((Alchemical_value + delta_alchemical)>1.0){
                        
            //NON BONDED TERMS
            if(perturbed_energies[0])
                context_openmm.setParameter("lam",Alchemical_value - delta_alchemical);//1-5 HD
            //1-4 Interactions
            if(perturbed_energies[1])
                context_openmm.setParameter("lamhd",Alchemical_value - delta_alchemical);//1-4 HD
            if(perturbed_energies[2])
                context_openmm.setParameter("lamtd",1.0 - (Alchemical_value - delta_alchemical));//1-4 To Dummy
            if(perturbed_energies[3])
                context_openmm.setParameter("lamfd",Alchemical_value - delta_alchemical);//1-4 From Dummy
            if(perturbed_energies[4])
                context_openmm.setParameter("lamftd",Alchemical_value - delta_alchemical);//1-4 From Dummy to Dummy
            
            //BONDED PERTURBED TERMS
            if(perturbed_energies[5])
                context_openmm.setParameter("lambond",Alchemical_value - delta_alchemical);//Bonds
            if(perturbed_energies[6])
                context_openmm.setParameter("lamangle",Alchemical_value - delta_alchemical);//Angles
            if(perturbed_energies[7])
                context_openmm.setParameter("lambda",Alchemical_value - delta_alchemical);//Torsions

            state_openmm=context_openmm.getState(infoMask);

            potential_energy_lambda_minus_delta = state_openmm.getPotentialEnergy();

            //minus =  exp(-beta * potential_energy_lambda_minus_delta) * exp(beta * potential_energy_lambda);
            //plus = exp(beta * potential_energy_lambda_minus_delta) * exp(-beta*potential_energy_lambda);

            minus =  exp(-beta * (potential_energy_lambda_minus_delta - potential_energy_lambda));
            plus = exp(beta * (potential_energy_lambda_minus_delta - potential_energy_lambda));

            if(Debug){
                qDebug() << "Lambda + delta > 1.0\n";
                qDebug() << "Lambda - delta = " << Alchemical_value - delta_alchemical << " Potential energy minus  = " << potential_energy_lambda_minus_delta * OpenMM::KcalPerKJ  << " kcal/mol" << "\n"; 
            }
            
        }
        else if((Alchemical_value - delta_alchemical)<0.0){

            //NON BONDED TERMS
            if(perturbed_energies[0])
                context_openmm.setParameter("lam",Alchemical_value + delta_alchemical);//1-5 HD
            //1-4 Interactions
            if(perturbed_energies[1])
                context_openmm.setParameter("lamhd",Alchemical_value + delta_alchemical);//1-4 HD
            if(perturbed_energies[2])
                context_openmm.setParameter("lamtd",1.0 - (Alchemical_value + delta_alchemical));//1-4 To Dummy
            if(perturbed_energies[3])
                context_openmm.setParameter("lamfd",Alchemical_value + delta_alchemical);//1-4 From Dummy
            if(perturbed_energies[4])
                context_openmm.setParameter("lamftd",Alchemical_value + delta_alchemical);//1-4 From Dummy to Dummy
            
            //BONDED PERTURBED TERMS
            if(perturbed_energies[5])
                context_openmm.setParameter("lambond",Alchemical_value + delta_alchemical);//Bonds
            if(perturbed_energies[6])
                context_openmm.setParameter("lamangle",Alchemical_value + delta_alchemical);//Angles
            if(perturbed_energies[7])
                context_openmm.setParameter("lambda",Alchemical_value + delta_alchemical);//Torsions
            
            state_openmm=context_openmm.getState(infoMask);

            potential_energy_lambda_plus_delta = state_openmm.getPotentialEnergy();

            //plus = exp(-beta * potential_energy_lambda_plus_delta) * exp(beta * potential_energy_lambda);
            //minus = exp(beta * potential_energy_lambda_plus_delta) * exp(-beta * potential_energy_lambda);
            
            plus = exp(-beta * (potential_energy_lambda_plus_delta - potential_energy_lambda));
            minus = exp(beta * (potential_energy_lambda_plus_delta - potential_energy_lambda));

            if(Debug){
                qDebug() << "Lambda + delta = " << Alchemical_value + delta_alchemical << " Potential energy plus  = " << potential_energy_lambda_plus_delta * OpenMM::KcalPerKJ << " kcal/mol" << "\n";
                qDebug() << "Lambda - delta < 0.0\n";
            }
            
        }
        else{
            
            //NON BONDED TERMS
            if(perturbed_energies[0])
                context_openmm.setParameter("lam",Alchemical_value + delta_alchemical);//1-5 HD
            //1-4 Interactions
            if(perturbed_energies[1])
                context_openmm.setParameter("lamhd",Alchemical_value + delta_alchemical);//1-4 HD
            if(perturbed_energies[2])
                context_openmm.setParameter("lamtd",1.0 - (Alchemical_value + delta_alchemical));//1-4 To Dummy
            if(perturbed_energies[3])
                context_openmm.setParameter("lamfd",Alchemical_value + delta_alchemical);//1-4 From Dummy
            if(perturbed_energies[4])
                context_openmm.setParameter("lamftd",Alchemical_value + delta_alchemical);//1-4 From Dummy to Dummy
            
            //BONDED PERTURBED TERMS
            if(perturbed_energies[5])
                context_openmm.setParameter("lambond",Alchemical_value + delta_alchemical);//Bonds
            if(perturbed_energies[6])
                context_openmm.setParameter("lamangle",Alchemical_value + delta_alchemical);//Angles
            if(perturbed_energies[7])
                context_openmm.setParameter("lambda",Alchemical_value + delta_alchemical);//Torsions

            state_openmm=context_openmm.getState(infoMask);

            potential_energy_lambda_plus_delta = state_openmm.getPotentialEnergy();

            
            if(Debug){
	      printf("*Lambda + Delta_Lambda %f potential_energy %f kcal/mol\n", Alchemical_value + delta_alchemical, potential_energy_lambda_plus_delta * OpenMM::KcalPerKJ) ;
            }

            
            //NON BONDED TERMS
            if(perturbed_energies[0])
                context_openmm.setParameter("lam",Alchemical_value - delta_alchemical);//1-5 HD
            //1-4 Interactions
            if(perturbed_energies[1])
                context_openmm.setParameter("lamhd",Alchemical_value - delta_alchemical);//1-4 HD
            if(perturbed_energies[2])
                context_openmm.setParameter("lamtd",1.0 - (Alchemical_value - delta_alchemical));//1-4 To Dummy
            if(perturbed_energies[3])
                context_openmm.setParameter("lamfd",Alchemical_value - delta_alchemical);//1-4 From Dummy
            if(perturbed_energies[4])
                context_openmm.setParameter("lamftd",Alchemical_value - delta_alchemical);//1-4 From Dummy to Dummy
        
            //BONDED PERTURBED TERMS
            if(perturbed_energies[5])
                context_openmm.setParameter("lambond",Alchemical_value - delta_alchemical);//Bonds
            if(perturbed_energies[6])
                context_openmm.setParameter("lamangle",Alchemical_value - delta_alchemical);//Angles
            if(perturbed_energies[7])
                context_openmm.setParameter("lambda",Alchemical_value - delta_alchemical);//Torsions

                                            
            state_openmm=context_openmm.getState(infoMask);

            potential_energy_lambda_minus_delta = state_openmm.getPotentialEnergy();

            //plus = exp(-beta * potential_energy_lambda_plus_delta) * exp(beta * potential_energy_lambda);
            //minus =  exp(-beta * potential_energy_lambda_minus_delta) * exp(beta * potential_energy_lambda);

            plus = exp(-beta * (potential_energy_lambda_plus_delta - potential_energy_lambda));
            minus = exp(-beta * (potential_energy_lambda_minus_delta - potential_energy_lambda));
            

            if(Debug){
	      printf("*Lambda - Delta_Lambda %f potential_energy %f kcal/mol\n", Alchemical_value - delta_alchemical, potential_energy_lambda_minus_delta * OpenMM::KcalPerKJ) ;
	    }
            

        }//end else


        GF_acc = GF_acc + plus;
        GB_acc = GB_acc + minus;

        double avg_GF = GF_acc /(sample_count + 1.0);

        double avg_GB = GB_acc /(sample_count + 1.0);


        double Energy_GF = -(1.0/beta)*log(avg_GF);

        double Energy_GB = -(1.0/beta)*log(avg_GB);

        double Energy_Gradient_lamda = (Energy_GF - Energy_GB) / (2 * delta_alchemical);


        if(Debug)
            qDebug() << "\n\n*Energy Gradient = " << Energy_Gradient_lamda * OpenMM::KcalPerKJ << " kcal/(mol lambda)" << "\n\n";


        if(buffer_coords && sample_count!=(n_samples-1))
            gradients.append(Energy_Gradient_lamda * OpenMM::KcalPerKJ);
        if(sample_count==(n_samples-1))
            gradients.append(Energy_Gradient_lamda * OpenMM::KcalPerKJ);

        
        //NON BONDED TERMS
        if(perturbed_energies[0])
            context_openmm.setParameter("lam",Alchemical_value);//1-5 HD
        //1-4 Interactions
        if(perturbed_energies[1])
            context_openmm.setParameter("lamhd",Alchemical_value);//1-4 HD
        if(perturbed_energies[2])
            context_openmm.setParameter("lamtd",1.0 - Alchemical_value);//1-4 To Dummy
        if(perturbed_energies[3])
            context_openmm.setParameter("lamfd",Alchemical_value);//1-4 From Dummy
        if(perturbed_energies[4])
            context_openmm.setParameter("lamftd",Alchemical_value);//1-4 From Dummy to Dummy
        
        //BONDED PERTURBED TERMS
        if(perturbed_energies[5])
            context_openmm.setParameter("lambond",Alchemical_value);//Bonds
        if(perturbed_energies[6])
            context_openmm.setParameter("lamangle",Alchemical_value);//Angles
        if(perturbed_energies[7])
            context_openmm.setParameter("lambda",Alchemical_value);//Torsions

        
        /*state_openmm=context_openmm.getState(infoMask);
        double dummy = state_openmm.getPotentialEnergy();
        qDebug() << "\nDifference dummy = " << dummy - potential_energy_lambda<< "\n\n";*/

        sample_count=sample_count + 1.0;
        
        //show_status(sample_count, n_samples);


    }//end while


    /**********************************************TO BE REMOVED*****************************************/
    //dcd.close();


    if (Debug)
        qDebug() << "Done dynamics, time elapsed " << timer.elapsed() << " ms\n";

    // Now update the sire coordinates/velocities and box dimensions 
    timer.restart();

    state_openmm = context_openmm.getState(infoMask);
    positions_openmm = state_openmm.getPositions();
    velocities_openmm = state_openmm.getVelocities();

    // Vector of Vector of molecules that are vector of atomic coordinates...

    QVector< QVector< QVector< Vector > > > buffered_workspace(n_samples);

    for (int i=0; i < buffered_workspace.size() ; i++){

        buffered_workspace[i].resize(nmols);

        for (int j=0 ; j < nmols; j++){

            int nats = ws.nAtoms(j);

            buffered_workspace[i][j].resize(nats);

        }

    }

    int k=0;

    for(int i=0; i<nmols;i++){

        Vector *sire_coords = ws.coordsArray(i);
        Vector *sire_momenta = ws.momentaArray(i);

        const double *m = ws.massArray(i);

        for(int j=0; j < ws.nAtoms(i) ; j++){

            sire_coords[j] = Vector(positions_openmm[j+k][0] * (OpenMM::AngstromsPerNm),positions_openmm[j+k][1] * (OpenMM::AngstromsPerNm),positions_openmm[j+k][2] * (OpenMM::AngstromsPerNm));

            if(buffer_coords){

                for (int l=0; l < n_samples ; l++){

                    //qDebug() << " i " << i << " j " << j << " k " << k << " l " << l;

                    Vector buffered_atcoord = Vector(  buffered_positions[l][j+k][0] * (OpenMM::AngstromsPerNm), buffered_positions[l][j+k][1] * (OpenMM::AngstromsPerNm),buffered_positions[l][j+k][2] * (OpenMM::AngstromsPerNm));

                    buffered_workspace[l][i][j] = buffered_atcoord;


                }

            }

            sire_momenta[j] = Vector(velocities_openmm[j+k][0] * m[j] * (OpenMM::AngstromsPerNm) * AKMAPerPs,velocities_openmm[j+k][1] * m[j] * (OpenMM::AngstromsPerNm) * AKMAPerPs, velocities_openmm[j+k][2] * m[j] * (OpenMM::AngstromsPerNm) * AKMAPerPs);
        }

        k= k + ws.nAtoms(i);
    }

    if ( buffer_coords == false)
        ws.commitCoordinatesAndVelocities();
    else
        ws.commitBufferedCoordinatesAndVelocities( buffered_workspace );


    //Now the box dimensions

    if (MCBarostat_flag == true) {

        state_openmm.getPeriodicBoxVectors(a,b,c);

        Vector new_dims = Vector(a[0] * OpenMM::AngstromsPerNm, b[1] * OpenMM::AngstromsPerNm, c[2] * OpenMM::AngstromsPerNm);

        System & ptr_sys = ws.nonConstsystem();
        PeriodicBox  sp = ptr_sys.property("space").asA<PeriodicBox>();

        sp.setDimensions(new_dims);
        const QString string = "space" ;
        ptr_sys.setProperty(string, sp);

        // Buffer dimensions if necessary 

        for (int k=0; k < buffered_dimensions.size() ; k++){

            const QString buffered_space = "buffered_space_" + QString::number(k) ;

            PeriodicBox buff_space = PeriodicBox( buffered_dimensions[k] );

            ptr_sys.setProperty( buffered_space, buff_space);

        }

        if(Debug)
            qDebug() << "NEW BOX DIMENSIONS [A] = (" << a[0] * OpenMM::AngstromsPerNm << ", " << b[1] * OpenMM::AngstromsPerNm << ", " << c[2] * OpenMM::AngstromsPerNm << ")\n\n";

    }

    // Clear all buffers 

    buffered_workspace.clear();
    buffered_dimensions.clear();

    if (Debug)
        qDebug() << "Updating system coordinates, time elapsed " << timer.elapsed() << " ms\n";

    return;

}

int frenergyst::show_status (double current, int total)
{

    double percent = (( current)/((double) total))*100;

    char buffer[102];


    for(int i = 1; i < percent; i++){
        buffer[i]='=';
    }

    buffer[(int) percent] = '>';

    for(int i = percent+1 ; i< 100; i++)
        buffer[i]=' ';

    buffer[0] = '[';

    buffer[100] = ']';

    buffer[101] = ' ';

    printf("%s %4.2f%%\r", buffer, percent);

    if(percent == 100)
        printf("\n\n");

    return 0;
}


/** Get the cufott type: nocutoff, cutoffnonperiodic, cutoffperiodic */
QString OpenMMFrEnergyST::getCutoffType(void){

    return CutoffType;

}

/** Set the cufott type: nocutoff, cutoffnonperiodic, cutoffperiodic */
void OpenMMFrEnergyST::setCutoffType(QString cutoff_type){

    CutoffType = cutoff_type;

}

/** Get the cutoff distance in A */
SireUnits::Dimension::Length OpenMMFrEnergyST::getCutoff_distance(void){

    return cutoff_distance;

}

/** Set the cutoff distance in A */
void OpenMMFrEnergyST::setCutoff_distance(SireUnits::Dimension::Length distance){

    cutoff_distance = distance;    

}

/** Get the dielectric constant */
double OpenMMFrEnergyST::getField_dielectric(void){

    return field_dielectric;
}

/** Set the dielectric constant */
void OpenMMFrEnergyST::setField_dielectric(double dielectric){
    
    field_dielectric=dielectric;

}

/** Set Andersen thermostat */

void OpenMMFrEnergyST::setAndersen(bool andersen){
    Andersen_flag = andersen;    
}

/** Get Andersen thermostat status on/off */
bool OpenMMFrEnergyST::getAndersen(void){
    
    return     Andersen_flag;
    
}

/** Get the Andersen Thermostat frequency collision */
double OpenMMFrEnergyST::getAndersen_frequency(void){
    
    return Andersen_frequency;
    
}

/** Set the Andersen Thermostat frequency collision */
void OpenMMFrEnergyST::setAndersen_frequency(double freq){
    
    Andersen_frequency=freq;
    
}

/** Get the bath Temperature */
SireUnits::Dimension::Temperature OpenMMFrEnergyST::getTemperature(void){
        
    return Temperature;
}

/** Set the Temperature */
void OpenMMFrEnergyST::setTemperature(SireUnits::Dimension::Temperature temperature){
        
    Temperature = temperature;
}

/** Set Monte Carlo Barostat on/off */
void OpenMMFrEnergyST::setMCBarostat(bool MCBarostat){
    MCBarostat_flag = MCBarostat;
}

/** Get Andersen thermostat status on/off */
bool OpenMMFrEnergyST::getMCBarostat(void){
    
    return     MCBarostat_flag;
    
}

/** Get the Monte Carlo Barostat frequency in time speps */
int OpenMMFrEnergyST::getMCBarostat_frequency(void){
    
    return  MCBarostat_frequency;
    
}

/** Set the Monte Carlo Barostat frequency in time speps */
void OpenMMFrEnergyST::setMCBarostat_frequency(int freq){
    
    MCBarostat_frequency=freq;
    
}

/** Get the Presure */
SireUnits::Dimension::Pressure OpenMMFrEnergyST::getPressure(void){
        
    return Pressure;
}

/** Set the Pressure */
void OpenMMFrEnergyST::setPressure(SireUnits::Dimension::Pressure pressure){

    Pressure = pressure;
}

/** Get the Constraint type: none, hbonds, allbonds, hangles */
QString OpenMMFrEnergyST::getConstraintType(void){

    return ConstraintType;

}

/** Set the Constraint type: none, hbonds, allbonds, hangles */
void OpenMMFrEnergyST::setConstraintType(QString constrain){

    ConstraintType = constrain;

}

/** Get the OpenMMMD Platform: CUDA, OpenCL, CPU */
QString OpenMMFrEnergyST::getPlatform(void){

    return platform_type;

}

/** Set the OpenMM Platform: CUDA, OpenCL, CPU */
void OpenMMFrEnergyST::setPlatform(QString platform){

    platform_type = platform;

}

/** Get the OpenMMMD Platform: CUDA, OpenCL, CPU */
QString OpenMMFrEnergyST::getDeviceIndex(void){

    return device_index;

}

/** Set the OpenMM Precision */
void OpenMMFrEnergyST::setPrecision(QString prec){

    precision = prec;

}


/** Get the OpenMMMD Precision */
QString OpenMMFrEnergyST::getPrecision(void){

    return precision;

}

/** Set the OpenMM Platform: CUDA, OpenCL, CPU */
void OpenMMFrEnergyST::setDeviceIndex(QString deviceidx){

    device_index = deviceidx;

}


/** Get the Restaint mode*/
bool OpenMMFrEnergyST::getRestraint(void){

    return Restraint_flag;

}

/** Set the Retraint mode */
void OpenMMFrEnergyST::setRestraint(bool Restraint){

    Restraint_flag = Restraint;
}

/** Get the Center of Mass motion removal frequency */
int OpenMMFrEnergyST::getCMMremoval_frequency(void){

    return CMMremoval_frequency;
}

/** Set the Center of Mass motion removal frequency */
void OpenMMFrEnergyST::setCMMremoval_frequency(int frequency){

    CMMremoval_frequency = frequency;
}

/** Get the frequency of buffering coordinates */
int OpenMMFrEnergyST::getEnergyFrequency(){

    return energy_frequency;
}

/** Set the Center of Mass motion removal frequency */
void OpenMMFrEnergyST::setEnergyFrequency(int frequency){

    energy_frequency = frequency;
}

/** Get the alchemical value used to calculate the free energy change via TI method*/
double OpenMMFrEnergyST::getAlchemical_value(void){

    return Alchemical_value;

}

/** Set the alchemical value used to calculate the free energy change via TI method*/
void OpenMMFrEnergyST::setAlchemical_value(double lambda_value){

    Alchemical_value = max(0.0,min(1.0,lambda_value));

}

/** Get the coulomb power used in the soft core potential*/
int OpenMMFrEnergyST::getCoulomb_power(void){

    return coulomb_power;

}

/** Set the coulomb power used in the soft core potential*/
void OpenMMFrEnergyST::setCoulomb_power(int coulomb){

    coulomb_power = coulomb;

}


/** Get the shift used in the soft core potential*/
double OpenMMFrEnergyST::getShift_delta(void){

    return shift_delta;

}

/** Set the shift used in the soft core potential*/
void OpenMMFrEnergyST::setShift_delta(double shiftdelta){

    shift_delta = shiftdelta;

}

/** Get the delta alchemical used in the FEP method*/
double OpenMMFrEnergyST:: getDeltaAlchemical(void){

    return delta_alchemical;

}

/** Set the delta alchemical used in the FEP method*/
void OpenMMFrEnergyST::setDeltatAlchemical(double deltaalchemical){

    delta_alchemical = deltaalchemical;

}

/** Get the flag to buffer coordinate during the free energy calculation*/
bool OpenMMFrEnergyST::getBufferCoords(void){

    return buffer_coords;


}


/** Set the flag to buffer coordinate during the free energy calculation*/
void OpenMMFrEnergyST::setBufferCoords(bool buffer){

     buffer_coords = buffer;

}

/** Calculated Gradient*/
QVector<double> OpenMMFrEnergyST::getGradients(void){

    return gradients;

}

/** Get the Integrator type*/
QString OpenMMFrEnergyST::getIntegrator(void) {
  return Integrator_type;
}

/** Set the Integrator type*/
void OpenMMFrEnergyST::setIntegrator(QString intgrator) {
  Integrator_type = intgrator;
}

/** Get the friction used in specific Integrator type*/
SireUnits::Dimension::Time OpenMMFrEnergyST::getFriction(void) {
  return friction;
}

/** Set the friction used in specific Integrator type*/
void OpenMMFrEnergyST::setFriction(SireUnits::Dimension::Time thefriction) {
  friction = thefriction;
} 

/** Get the integration tollerance */
double OpenMMFrEnergyST::getIntegration_tollerance(void){

    return integration_tol;

}

/** Set the integration tollerance*/
void OpenMMFrEnergyST::setIntegration_tollerance(double tollerance){

    integration_tol = tollerance;

}

SireUnits::Dimension::Time OpenMMFrEnergyST::getTimetoSkip(void){

    return timeskip;

}

void OpenMMFrEnergyST::setTimetoSkip(SireUnits::Dimension::Time skip){

    timeskip = skip;

}


/** Create an empty workspace */
IntegratorWorkspacePtr OpenMMFrEnergyST::createWorkspace(const PropertyMap &map) const
{
    return IntegratorWorkspacePtr( new AtomicVelocityWorkspace(map) );
}

/** Return the ensemble of this integrator */
Ensemble OpenMMFrEnergyST::ensemble() const
{
    return Ensemble::NVE();
}

/** Return whether or not this integrator is time-reversible */
bool OpenMMFrEnergyST::isTimeReversible() const
{
    return true;
}

/** Create a workspace for this integrator for the molecule group 'molgroup' */
IntegratorWorkspacePtr OpenMMFrEnergyST::createWorkspace(const MoleculeGroup &molgroup, const PropertyMap &map) const
{
    return IntegratorWorkspacePtr( new AtomicVelocityWorkspace(molgroup,map) );
}

const char* OpenMMFrEnergyST::typeName()
{
    return QMetaType::typeName( qMetaTypeId<OpenMMFrEnergyST>() );
}