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

#include "openmmintegrator.h"
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

//ADDED BY GAC
#include "SireMaths/vector.h"
#include <iostream>
#include <QElapsedTimer>
#include <iomanip>
//#include <fstream>

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


int OpenmmPDB(IntegratorWorkspace &workspace, std::vector<OpenMM::Vec3> &positions_openmm);


static const RegisterMetaType<OpenMMIntegrator> r_openmmint;


/** Serialise to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, const OpenMMIntegrator &velver)
{
    writeHeader(ds, r_openmmint, 1);
    
    SharedDataStream sds(ds);
    
    sds << velver.frequent_save_velocities << velver.CutoffType << velver.cutoff_distance << velver.field_dielectric
    	<< velver.Andersen_flag <<  velver.Andersen_frequency 
    	<< velver.MCBarostat_flag << velver.MCBarostat_frequency << velver.ConstraintType << velver.Pressure << velver.Temperature
    	<<velver.platform_type << velver.Alchemical_values 
    	<< static_cast<const Integrator&>(velver);
    
        
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, OpenMMIntegrator &velver)
{
    VersionID v = readHeader(ds, r_openmmint);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> velver.frequent_save_velocities >> velver.CutoffType >> velver.cutoff_distance >> velver.field_dielectric
    		>> velver.Andersen_flag >>  velver.Andersen_frequency 
    		>> velver.MCBarostat_flag >> velver.MCBarostat_frequency >> velver.ConstraintType >> velver.Pressure >> velver.Temperature
    		>> velver.platform_type >> velver.Alchemical_values 
        	>> static_cast<Integrator&>(velver);
    }
    else
        throw version_error(v, "1", r_openmmint, CODELOC);
        
    return ds;
}

/** Constructor */
OpenMMIntegrator::OpenMMIntegrator(bool frequent_save) 
               : ConcreteProperty<OpenMMIntegrator,Integrator>(),
                 frequent_save_velocities(frequent_save), 
                 CutoffType("nocutoff"), cutoff_distance(1.0 * nanometer),field_dielectric(1.0),
                 Andersen_flag(false),Andersen_frequency(90.0), MCBarostat_flag(false),
                 MCBarostat_frequency(25),ConstraintType("none"),
                 Pressure(1.0 * bar),Temperature(300.0 * kelvin),platform_type("Reference"),Alchemical_values()
           
{}

/** Copy constructor */
OpenMMIntegrator::OpenMMIntegrator(const OpenMMIntegrator &other)
               : ConcreteProperty<OpenMMIntegrator,Integrator>(other),
                 frequent_save_velocities(other.frequent_save_velocities),
                 CutoffType(other.CutoffType),cutoff_distance(other.cutoff_distance),
                 field_dielectric(other.field_dielectric), Andersen_flag(other.Andersen_flag),
                 Andersen_frequency(other.Andersen_frequency), MCBarostat_flag(other.MCBarostat_flag),
                 MCBarostat_frequency(other.MCBarostat_frequency),ConstraintType(other.ConstraintType), 
                 Pressure(other.Pressure), Temperature(other.Temperature),platform_type(other.platform_type),
                 Alchemical_values(other.Alchemical_values)
                 
                 
{}

/** Destructor */
OpenMMIntegrator::~OpenMMIntegrator()
{}

/** Copy assignment operator */
OpenMMIntegrator& OpenMMIntegrator::operator=(const OpenMMIntegrator &other)
{
    Integrator::operator=(other);
    frequent_save_velocities = other.frequent_save_velocities;
    
    return *this;
}

/** Comparison operator */
bool OpenMMIntegrator::operator==(const OpenMMIntegrator &other) const
{
    return frequent_save_velocities == other.frequent_save_velocities and
           Integrator::operator==(other);
}

/** Comparison operator */
bool OpenMMIntegrator::operator!=(const OpenMMIntegrator &other) const
{
    return not OpenMMIntegrator::operator==(other);
}

/** Return a string representation of this integrator */
QString OpenMMIntegrator::toString() const
{
    return QObject::tr("OpenMMIntegrator()");
}
                                                       
/** Integrate the coordinates of the atoms in the molecules in 'molgroup'
    using the forces in 'forcetable', using the optionally supplied 
    property map to find the necessary molecular properties 
    
    \throw SireMol::missing_molecule
    \throw SireBase::missing_property
    \throw SireError:invalid_cast
    \throw SireError::incompatible_error
*/
void OpenMMIntegrator::integrate(IntegratorWorkspace &workspace, const Symbol &nrg_component, SireUnits::Dimension::Time timestep, int nmoves, bool record_stats) const{
				       
	cout << "In OpenMMIntegrator::integrate()\n\n" ;
	
	// Initialise OpenMM

	// Convert the Sire data into data understood by OpenMM

	
	AtomicVelocityWorkspace &ws = workspace.asA<AtomicVelocityWorkspace>();
	
	const double dt = convertTo( timestep.value(), picosecond);
	
	cout << "time step = " << dt << " ps" << "\n";
	
	cout << "Number of moves = " << nmoves << "\n";

	const int nmols = ws.nMolecules();
    
	int nats = 0;
	
	int flag_cutoff;
	
	int flag_constraint;
	
	bool free_energy_calculation;
		
	cout << "Save frequency velocities = " << frequent_save_velocities << "\n";
	
	if (CutoffType == "nocutoff")
		flag_cutoff = NOCUTOFF;
	
	else if (CutoffType == "cutoffnonperiodic")
		flag_cutoff = CUTOFFNONPERIODIC;
	
	else if (CutoffType == "cutoffperiodic")
		flag_cutoff = CUTOFFPERIODIC;
	else
		throw SireError::program_bug(QObject::tr(
            "The CutOff method has not been specified. Possible choises: nocutoff, cutoffnonperiodic, cutoffperiodic"), CODELOC);
				

	if (ConstraintType == "none")
		flag_constraint = NONE;
	
	else if (ConstraintType == "hbonds")
		flag_constraint = HBONDS;
	
	else if(ConstraintType == "allbonds")
		flag_constraint = ALLBONDS;
		
	else if (ConstraintType == "hangles")
		flag_constraint = HANGLES;

	else
		throw SireError::program_bug(QObject::tr(
            "The Constraints method has not been specified. Possible choises: none, hbonds, allbonds, hangles"), CODELOC);
		
	cout << "\nConstraint Type = " << ConstraintType.toStdString() << "\n";
	
	
	// Free energy calculation flag
	
	if(Alchemical_values.size() == 0){
		free_energy_calculation = false;
		cout << "\nFree Energy calculation = OFF\n\n";
	}
	else{
		free_energy_calculation = true;
		cout << "\nFree Energy calculation = ON\n\n";
	}
	
	
	for (int i=0; i<nmols; ++i){
		nats = nats + ws.nAtoms(i);
	}

	cout << "There are " << nats << " atoms " << "There are " << nmols << " molecules" <<"\n" ;
	
	// INITIALIZE OpenMM
	
	const double Coulob14Scale = 1.0/1.2;
	
	const double LennardJones14Scale = 1.0/2.0;

	//Load Plugins from the OpenMM standard Plugin Directory 
	
	OpenMM::Platform::loadPluginsFromDirectory(OpenMM::Platform::getDefaultPluginsDirectory());
	
	//OpenMM system

	OpenMM::System system_openmm;

	
	//OpenMM non Bonded Forces
	
	OpenMM::NonbondedForce * nonbond_openmm=NULL; 
	
	OpenMM::CustomNonbondedForce * custom_nonbond_openmm=NULL;
	
    if(free_energy_calculation == false){  
		
		nonbond_openmm = new OpenMM::NonbondedForce();
		
		system_openmm.addForce(nonbond_openmm);
	}
	
	else{
	
		custom_nonbond_openmm = new OpenMM::CustomNonbondedForce("kgac*r");
		
		custom_nonbond_openmm->addGlobalParameter("kgac",Alchemical_values[0]);
      
		system_openmm.addForce(custom_nonbond_openmm);
		
	}
	
	
	
	//Long Range interaction non bonded force method setting

	if(flag_cutoff == NOCUTOFF){
				
		if(free_energy_calculation == false)
			nonbond_openmm->setNonbondedMethod(OpenMM::NonbondedForce::NoCutoff);
		
		else
			custom_nonbond_openmm->setNonbondedMethod(OpenMM::CustomNonbondedForce::NoCutoff);
		
		cout << "\nCut off type = " << CutoffType.toStdString() << "\n";
	}
	
	if(flag_cutoff == CUTOFFNONPERIODIC){
		
		if(free_energy_calculation == false)
			nonbond_openmm->setNonbondedMethod(OpenMM::NonbondedForce::CutoffNonPeriodic);
		
		else
			custom_nonbond_openmm->setNonbondedMethod(OpenMM::CustomNonbondedForce::CutoffNonPeriodic);
			
		cout << "\nCut off type = " << CutoffType.toStdString() << "\n";
	}
	
	
	if(flag_cutoff == CUTOFFPERIODIC){
	
		//Set CutOff distance
		
		const double converted_cutoff_distance = convertTo(cutoff_distance.value(), nanometer);
		
		if(free_energy_calculation == false){
			
			nonbond_openmm->setNonbondedMethod(OpenMM::NonbondedForce::CutoffPeriodic);
		
			nonbond_openmm->setCutoffDistance(converted_cutoff_distance);
		
			//Set Dielectric constant media
			nonbond_openmm->setReactionFieldDielectric(field_dielectric);
		}
		
		else{
			
			custom_nonbond_openmm->setNonbondedMethod(OpenMM::CustomNonbondedForce::CutoffPeriodic);
		
			custom_nonbond_openmm->setCutoffDistance(converted_cutoff_distance);
		
			//Set Dielectric constant media
			
			//custom_nonbond_openmm->setReactionFieldDielectric(field_dielectric);
		
		}
		
		
		cout << "\nCut off type = " << CutoffType.toStdString() << "\n";
		cout << "CutOff distance = " << converted_cutoff_distance  << " Nm" << "\n";
   	 	cout << "Dielectric constant= " << field_dielectric << "\n\n";
		
		
	}
	
	//Andersen thermostat
	
	if (Andersen_flag == true){
	
		const double converted_Temperature = convertTo(Temperature.value(), kelvin);
		
		OpenMM::AndersenThermostat * thermostat = new OpenMM::AndersenThermostat(converted_Temperature, Andersen_frequency);
		system_openmm.addForce(thermostat);
		
		cout << "\nAndersen Thermostat set\n";
		cout << "Temperature = " << converted_Temperature << " K\n";
		cout << "Frequency collisions = " << Andersen_frequency << " 1/ps\n";
	
	}
	
	//Monte Carlo Barostat
	
	if (MCBarostat_flag == true) {
	
		const double converted_Temperature = convertTo(Temperature.value(), kelvin);
		const double converted_Pressure = convertTo(Pressure.value(), bar);
		
		
		OpenMM::MonteCarloBarostat * barostat = new OpenMM::MonteCarloBarostat(converted_Pressure, converted_Temperature, MCBarostat_frequency);
		system_openmm.addForce(barostat);
		
		cout << "\nMonte Carlo Barostat set\n";
		cout << "Temperature = " << converted_Temperature << " K\n";
		cout << "Pressure = " << converted_Pressure << " bar\n";
		cout << "Frequency every " << MCBarostat_frequency << " steps\n";
		
	}
	
	
	//OpenMM Bonded Forces
      
	OpenMM::HarmonicBondForce * bondStretch_openmm = new OpenMM::HarmonicBondForce();
    
	OpenMM::HarmonicAngleForce * bondBend_openmm = new OpenMM::HarmonicAngleForce();
    
	OpenMM::PeriodicTorsionForce * bondTorsion_openmm = new OpenMM::PeriodicTorsionForce();
    
	system_openmm.addForce(bondStretch_openmm);
    
	system_openmm.addForce(bondBend_openmm);
    
	system_openmm.addForce(bondTorsion_openmm);
	
	
	std::vector<std::pair<int,int> > bondPairs;
	
	
	
	//OpenMM vector coordinate
    
	std::vector<OpenMM::Vec3> positions_openmm(nats);		

	double *system_coords;
	
	system_coords = new double [nats*3] ;
    
	//OpenMM vector momenta
    
	std::vector<OpenMM::Vec3> velocities_openmm(nats);		
     
	double *system_momenta;
	
	system_momenta = new double [nats*3] ;

	double *system_masses ;
	
	system_masses = new double [ nats ] ;

	int system_index = 0;

	//cout << "INITAL COORDINATES AND VELOCITIES\n\n";
	
	QElapsedTimer timer_IN;
	
	timer_IN.start();
	
	for (int i=0; i < nmols; ++i){
	
		const int nats = ws.nAtoms(i);

		Vector *c = ws.coordsArray(i);
		
		//const Vector *f = ws.forceArray(i);
	
		Vector *p = ws.momentaArray(i);
	
		const double *m = ws.massArray(i);
		
		//cout << "Molecule = " << i <<"\n";

		for (int j=0; j < nats; ++j){
	     
			/*system_coords[ 3 * system_index + 0 ] = c[j].x() ;
			system_coords[ 3 * system_index + 1 ] = c[j].y() ;
			system_coords[ 3 * system_index + 2 ] = c[j].z() ;
			
			system_momenta[3 * system_index + 0 ] = p[j].x() ;
			system_momenta[3 * system_index + 1 ] = p[j].y() ;
			system_momenta[3 * system_index + 2 ] = p[j].z() ;*/
	     
			positions_openmm[system_index] = OpenMM::Vec3(c[j].x() * (OpenMM::NmPerAngstrom) ,
							c[j].y() * (OpenMM::NmPerAngstrom),c[j].z() * (OpenMM::NmPerAngstrom));
	    
			
	     
			velocities_openmm[system_index] = OpenMM::Vec3(p[j].x()/m[j] * (OpenMM::NmPerAngstrom) ,
						  p[j].y()/m[j] * (OpenMM::NmPerAngstrom), p[j].z()/m[j] * (OpenMM::NmPerAngstrom));

			//system_masses[system_index] = m[j] ;
	     
			system_openmm.addParticle(m[j]) ;
			     
			system_index = system_index + 1;
			
			/*cout <<"\n";
			cout << "atom = " << system_index - 1  << " COORD X = " << c[j].x() << " COORD Y = " << c[j].y() << " COORD Z = " << c[j].z()<<"\n" ;
			cout << "atom = " << system_index - 1  << " MOMENTA X = " << p[j].x() << " MOMENTA Y = " << p[j].y() << " MOMENTA Z = " << p[j].z()<<"\n" ;*/
			//cout << "atom = " << j << " MASS = " << m[j]<<"\n" ; 
	     
		}
		
		//cout<<"\n";
	   
	}
      

	
	/*for (int i=0; i < system_index ; i++){
		
		cout << "atom = " << i << " COORD X = " << system_coords[3 * i + 0 ] << 
							 	  " COORD Y = " << system_coords[3 * i + 1 ]  << 
							 	  " COORD Z = " << system_coords[3 * i + 2 ]<<"\n";
							 
		cout << "atom = " << i << " MOMENTA X = " << system_momenta[3 * i + 0 ] << 
								  " MOMENTA Y = " << system_momenta[3 * i + 1 ] << 
								  " MOMENTA Z = " << system_momenta[3 * i + 2 ]<<"\n";
							      
		//cout << " MASS " << system_masses[ i ];
		
		cout << "\n";
		
      }*/
      
      
	
	MoleculeGroup molgroup = ws.moleculeGroup();
	
	
	int num_atoms_till_i = 0;
	
	
	
	custom_nonbond_openmm->addPerParticleParameter("q");
	custom_nonbond_openmm->addPerParticleParameter("sigma");
	custom_nonbond_openmm->addPerParticleParameter("eps");
    
	for (int i=0; i < nmols ; i++){
	
		const Vector *c = ws.coordsArray(i);
	
		Molecule molecule = molgroup.moleculeAt(i).molecule();
		
		int num_atoms_molecule = molecule.nAtoms();
		
		/*cout << "\nMOLECULE NUM = "<< i <<" N_ATOMS MOLECULE = " 
									 << num_atoms_molecule 
									 << " NUM ATOMS UNTILL CURRENT MOLUCULE = "
									 << num_atoms_till_i << "\n\n" ; */
		
	      
		//NON BONDED TERMS  
		
		//Lennard Jones
	  
		AtomLJs atomvdws = molecule.property("LJ").asA<AtomLJs>();
	
		AtomCharges atomcharges = molecule.property("charge").asA<AtomCharges>();
	
		QVector<SireMM::LJParameter> ljparameters = atomvdws.toVector();
	
		QVector<SireUnits::Dimension::Charge> charges = atomcharges.toVector();
	
		for(int j=0; j< ljparameters.size();j++){
	
			double sigma = ljparameters[j].sigma();
		
			double epsilon = ljparameters[j].epsilon();
		
			double charge = charges[j].value();
			
			if(free_energy_calculation == false){  		
		
				nonbond_openmm->addParticle(charge, sigma * OpenMM::NmPerAngstrom, epsilon * OpenMM::KJPerKcal);
			
			}
			
			else{
			
				std::vector<double> params(3);
				
				params[0] = charge;
				params[1] = sigma * OpenMM::NmPerAngstrom;
				params[2] = epsilon * OpenMM::KJPerKcal;
				
				custom_nonbond_openmm->addParticle(params);
			
			}

			//cout<< atomvdws.toString();
	
			/*cout << "sigma :" << sigma <<"\n";
		
			cout << "epsilon :" << epsilon << "\n";
		
			cout << "charges : " << charge << "\n";
	    
			cout << "\n";*/
		}
	     
		
		//BONDED TERMS     
	      
	      
		// The connectivity...
	    bool hasConnectivity = molecule.hasProperty("connectivity");
	    
	    /* If the molecule does not have a connectivity, then we cannot get bonds/angles/dihedrals 
	    	(effectively assuming it is a monoatomic molecule)*/
	    
	    if ( !hasConnectivity )
	        continue;
	        
		Connectivity connectivity = molecule.property("connectivity").asA<Connectivity>();
		
		// The bonded parameters are stored in "amberparameters"
	
		AmberParameters amber_params = molecule.property("amberparameters").asA<AmberParameters>();
		
		//Bonds
		
		QList<BondID> bonds_ff = amber_params.getAllBonds();
		
		QVector<BondID> bonds = bonds_ff.toVector();
		
	
		for (int j=0; j < bonds_ff.length() ; j++){
		
			BondID bond_ff = bonds_ff[j];
	    
			QList<double> bond_params = amber_params.getParams(bond_ff);
	    
			double k = bond_params[0];
			double r0 = bond_params[1];
			
			int idx0 = bonds[j].atom0().asA<AtomIdx>().value();
			int idx1 = bonds[j].atom1().asA<AtomIdx>().value();
			
			//Select the atom type
			QString atom0 =  molecule.atom(AtomIdx(idx0)).toString();
			QString atom1 =  molecule.atom(AtomIdx(idx1)).toString();
			
						
			idx0 = idx0 + num_atoms_till_i;
			idx1 = idx1 + num_atoms_till_i;
			
			
			
			if(flag_constraint == NONE ){
				
				bondStretch_openmm->addBond(idx0,idx1,r0 * OpenMM::NmPerAngstrom, 
				k * 2.0 * OpenMM::KJPerKcal * OpenMM::AngstromsPerNm * OpenMM::AngstromsPerNm); 
				
				//cout << "\nBOND ADDED TO "<< atom0.toStdString() << " AND " << atom1.toStdString() << "\n";
				
			}
			
			else if ( flag_constraint == ALLBONDS || flag_constraint == HANGLES ) {
			
				system_openmm.addConstraint(idx0,idx1, r0 *  OpenMM::NmPerAngstrom);
				
				//cout << "\nALLBONDS or HANGLES ADDED BOND CONSTRAINT TO " << atom0.toStdString() << " AND " << atom1.toStdString() << "\n";
				
			}
			
			else if ( flag_constraint == HBONDS ) {
			
				if ( (atom0[6] == 'H') || (atom1[6] == 'H') ){
					
					system_openmm.addConstraint(idx0,idx1, r0 *  OpenMM::NmPerAngstrom);
					
					//cout << "\nHBONDS ADDED BOND CONSTRAINT TO " << atom0.toStdString() << " AND " << atom1.toStdString() << "\n";
					
				}
					
				else {
					
					bondStretch_openmm->addBond(idx0,idx1,r0 * OpenMM::NmPerAngstrom, 
												k * 2.0 * OpenMM::KJPerKcal * OpenMM::AngstromsPerNm * OpenMM::AngstromsPerNm);
					
					//cout << "\nHBONDS ADDED BOND TO " << atom0.toStdString() << " AND " << atom1.toStdString() << "\n";
				
				}	
			
			}
			
			
			
			
			//Bond exclusion List
			
			bondPairs.push_back(std::make_pair(idx0,idx1));
	    
			/*cout << "Bond between atom global index " << idx0 << " and " << idx1 <<"\n"; 
			
			cout << "k_bond = " << k << " r0 = " << r0<<"\n";*/

			//cout << "Bond local" << bond_ff.toString() << " has k " << k << " r0 " << r0; 
	    
			//cout << "\n";
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
		  
			int idx2= angles[j].atom2().asA<AtomIdx>().value();
			
			
			QString atom0 =  molecule.atom(AtomIdx(idx0)).toString();
			QString atom1 =  molecule.atom(AtomIdx(idx1)).toString();
			QString atom2 =  molecule.atom(AtomIdx(idx2)).toString();
			
			Vector diff = c[idx2] - c[idx0];
			
			/*cout << "\nATOM0 = " << atom0.toStdString() << " coords = [A] (" << c[idx0].x() << " ," << c[idx0].y() << " ,"<<  c[idx0].z()<< ")\n";
			cout << "\nATOM1 = " << atom1.toStdString() << " coords = [A] (" << c[idx1].x() << " ," << c[idx1].y() << " ,"<<  c[idx1].z()<< ")\n";
			cout << "\nATOM2 = " << atom2.toStdString() << " coords = [A] (" << c[idx2].x() << " ," << c[idx2].y() << " ,"<<  c[idx2].z()<< ")\n\n";*/
			 
		
			idx0 = idx0 + num_atoms_till_i;
			idx1 = idx1 + num_atoms_till_i;
			idx2 = idx2 + num_atoms_till_i;
			
			if ( flag_constraint == HANGLES ){
			
				if( ((atom0[6] == 'H') && (atom2[6] == 'H')) ){
								
					system_openmm.addConstraint(idx0,idx2, diff.length() *  OpenMM::NmPerAngstrom);
					
					/*cout << "Diff coords = [A] (" << diff.x() << " ," << diff.y() << " ,"<<  diff.z()<< ")\n";
					
					cout << "\nHANGLES H-X-H ADDED ANGLE CONSTRAINT TO " << atom0.toStdString() << " AND " 
														   << atom1.toStdString() << " AND " 
														   << atom2.toStdString() << " Distance = " << diff.length() << " A \n";*/
					
				}
				
				else if ( ((atom0[6] == 'H') && (atom1[6] == 'O')) || ((atom1[6] == 'O') && (atom2[6] == 'H')) ) {
				
					system_openmm.addConstraint(idx0,idx2, diff.length() *  OpenMM::NmPerAngstrom);
					
					/*cout << "\nHANGLES H-O-X or X-O-H ADDED ANGLE CONSTRAINT TO " << atom0.toStdString() << " AND " 
														   << atom1.toStdString() << " AND " 
														   << atom2.toStdString() << " Distance = " << diff.length() << " A \n";*/
				
				}
				
				else{
				
					bondBend_openmm->addAngle(idx0,idx1,idx2, theta0 , k * 2.0 * OpenMM::KJPerKcal);
					
					/*cout << "\nHANLGES ADDED ANGLE BOND TO " << atom0.toStdString() << " AND " 
													 << atom1.toStdString() << " AND "
													 << atom2.toStdString() << "\n";*/
				}
				
			}
			
			else {
				
				bondBend_openmm->addAngle(idx0,idx1,idx2, theta0 , k * 2.0 * OpenMM::KJPerKcal);
				
				/*cout << "\nADDED ANGLE BOND TO " << atom0.toStdString() << " AND " 
													 << atom1.toStdString() << " AND "
													 << atom2.toStdString() << "\n";*/
				
			}
		  
			/*cout << "Angle between atom global index " << idx0 << " and " <<idx1 <<" and "<< idx2<<"\n";
			
			cout << "k_angle = " << k << " theta0 = " << theta0<<"\n"; */
	    
			//cout << "Angle local " << angle_ff.toString() << " has k " << k << " theta0 " << theta0; 
		  
			//cout << "\n";
		  
		}

	
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
	    
			// Variable number of parameters
	    
			for (int k=0 ; k < dihedral_params.length() ; k = k + 3 ){
			
				double v = dihedral_params[ k ];
			
				int periodicity = dihedral_params[ k + 1 ];
			
				double phase = dihedral_params[ k + 2 ];
			
				bondTorsion_openmm->addTorsion(idx0, idx1, idx2, idx3, periodicity, phase , v * OpenMM::KJPerKcal);
			
				/*cout << "Dihedral between atom global index " << idx0 << " and " << idx1 << " and " << idx2 << " and " << idx3<<"\n";
				
				cout << "Amplitude_dih = " << v << " periodicity " << periodicity << " phase " << phase<<"\n";*/

				//cout << "Dihedral local" << dihedral_ff.toString() << " v " << v << " periodicity " << periodicity << " phase " << phase;
				
				//cout << "\n";
				
			}
		}
		
		
		//Improper Dihedrals

		QList<ImproperID> impropers_ff = amber_params.getAllImpropers();
		
		QVector<ImproperID> impropers = impropers_ff.toVector();

		for (int j=0; j < impropers_ff.length() ; j++ ){
		
			ImproperID improper_ff = impropers_ff[j];
		
			QList<double> improper_params = amber_params.getParams(improper_ff);
	    
			// Variable number of parameters
			
			int idx0 = impropers[j].atom0().asA<AtomIdx>().value() + num_atoms_till_i;
		
			int idx1 = impropers[j].atom1().asA<AtomIdx>().value() + num_atoms_till_i;
		
			int idx2 = impropers[j].atom2().asA<AtomIdx>().value() + num_atoms_till_i;
		
			int idx3 = impropers[j].atom3().asA<AtomIdx>().value() + num_atoms_till_i;
			
	    
			for (int k=0 ; k < improper_params.length() ; k = k + 3 ){
				
				double v = improper_params[ k ];
				
				double periodicity = improper_params[ k + 1 ];
				
				double phase = improper_params[ k + 2 ];
				
				bondTorsion_openmm->addTorsion(idx0, idx1, idx2, idx3, periodicity, phase , v * OpenMM::KJPerKcal);
				
				/*cout << "Improper Dihedral between atom global index " << idx0 << " and " << idx1 << " and " << idx2 << " and " << idx3<<"\n";
			
				cout << "Amplitude_impr = " << v << " periodicity " << periodicity << " phase " << phase <<"\n";*/
				
				//cout << "\n";
			}
		}
		
		num_atoms_till_i = num_atoms_till_i + num_atoms_molecule ;

      
	}// end of loop over mols
	
	
	//Exclude the 1-2, 1-3 bonded atoms from nonbonded forces, and scale down 1-4 bonded atoms
	
	//nonbond_openmm->createExceptionsFromBonds(bondPairs, Coulob14Scale, LennardJones14Scale);**********DA ATTIVATE IMPORTANTE*****************
	
	/*for(unsigned int i=0; i<bondPairs.size();i++){
			
		cout << "Bond excluded = ( " << bondPairs[i].first << " , " << bondPairs[i].second << " )";
		cout << "\n";
	}*/
	
	//OpenMM Integrator
	
	OpenMM::VerletIntegrator integrator_openmm(dt);//dt in pico seconds
	

	//OpenMM Context
	
	
	
	OpenMM::Platform& platform_openmm = OpenMM::Platform::getPlatformByName(platform_type.toStdString()); 

	OpenMM::Context context_openmm(system_openmm,integrator_openmm,platform_openmm);  
	
	if(flag_cutoff == CUTOFFPERIODIC){
		
		const System & ptr_sys = ws.system();
	
		const PropertyName &space_property = PropertyName("space");
	
    	const PeriodicBox &space = ptr_sys.property(space_property).asA<PeriodicBox>();
    
    	const double Box_x_Edge_Length = space.dimensions()[0] * OpenMM::NmPerAngstrom; //units in nm
    
    	const double Box_y_Edge_Length = space.dimensions()[1] * OpenMM::NmPerAngstrom; //units in nm
    
    	const double Box_z_Edge_Length = space.dimensions()[2] * OpenMM::NmPerAngstrom; //units in nm
    
    	cout << "\nBOX SIZE [A] = (" << space.dimensions()[0] << " , " << space.dimensions()[1] << " ,  " << space.dimensions()[2] << ")\n\n";
		
		//Set Periodic Box Condition
	
		context_openmm.setPeriodicBoxVectors(OpenMM::Vec3(Box_x_Edge_Length,0,0),
											 OpenMM::Vec3(0,Box_y_Edge_Length,0),
											 OpenMM::Vec3(0,0,Box_z_Edge_Length));
	}
	  
	//Add the coordinates  and velocities of the atoms to the OpenMM context
    
	context_openmm.setPositions(positions_openmm);  
    
	context_openmm.setVelocities(velocities_openmm);
	
	
	
	cout << "\nCOPY IN Simulation time = " << timer_IN.elapsed() / 1000.0 << " s"<<"\n\n";
	
	
	cout << "\n\nREMARK  Using OpenMM platform = " <<context_openmm.getPlatform().getName().c_str()<<"\n";
	
	int infoMask = 0;
	
	infoMask = OpenMM::State::Positions;
	
	infoMask = infoMask + OpenMM::State::Velocities; 
    
	infoMask = infoMask +  OpenMM::State::Energy;
	
	
	OpenMM::State state_openmm=context_openmm.getState(infoMask);
	
	
	double kinetic_energy = 0.0; 
	double potential_energy = 0.0; 
	
	kinetic_energy = state_openmm.getKineticEnergy(); 
	
	potential_energy = state_openmm.getPotentialEnergy(); 
	
	cout<< "Before MD" <<"\n";
	
	cout <<"Total Energy = " << (kinetic_energy + potential_energy) * OpenMM::KcalPerKJ << " Kcal/mol "
		 << " Kinetic Energy = " << kinetic_energy  * OpenMM::KcalPerKJ << " Kcal/mol " 
		 << " Potential Energy = " << potential_energy * OpenMM::KcalPerKJ << " Kcal/mol";
	
	cout<<"\n";
	

	
	//Time benchmark
	
	QElapsedTimer timer_MD;
	
	timer_MD.start();
	
	//MD SIMULATION FOR NMOVES
	
	//OpenMM::CMMotionRemover(1); 
	
	integrator_openmm.step(nmoves);
	
	cout << "\nMD Simulation time = " << timer_MD.elapsed() / 1000.0 << " s"<<"\n\n";
	
	
	QElapsedTimer timer_OUT;
	
	timer_OUT.start();
	
	
	state_openmm=context_openmm.getState(infoMask);	
	
	positions_openmm = state_openmm.getPositions();
		
	velocities_openmm = state_openmm.getVelocities();
	
	
	cout<< "Total Time = " << state_openmm.getTime() << " ps"<<"\n\n";
	
	
	kinetic_energy = state_openmm.getKineticEnergy(); 
	
	potential_energy = state_openmm.getPotentialEnergy(); 
	
	
	cout<< "After MD" <<"\n";
	
	cout <<"*Total Energy = " << (kinetic_energy + potential_energy) * OpenMM::KcalPerKJ << " Kcal/mol "
		 << " Kinetic Energy = " << kinetic_energy  * OpenMM::KcalPerKJ << " Kcal/mol " 
		 << " Potential Energy = " << potential_energy * OpenMM::KcalPerKJ << " Kcal/mol";
	
	cout<<"\n";
	
	
	
	//OpenmmPDB(workspace,positions_openmm);
	
	
	
	// Call openMM for N MD steps
	
	// USE VERLET INTEGRATOR
	
	/*cout << "\nSimulation Parameters";
	cout << "Number of moves = " << nmoves << " DT = " << dt <<" ps";
	cout << "\nResults\n";
	
	double kinetic_energy = 0.0;
	double potential_energy = 0.0;
		

	int MAX_FRAMES = 5;
	
	for(int  frame = 0; frame<MAX_FRAMES; frame++){
	
		OpenMM::State state_openmm = context_openmm.getState(infoMask);
		
		if(frame == 0 || frame == MAX_FRAMES-1){
				
			cout<<"\n\nFrame = " <<frame<<"\n\n";
		
			cout<< "Total Time = " << state_openmm.getTime() << " ps";
		
		}
		
		//const std::vector<OpenMM::Vec3>& positions_check = state_openmm.getPositions();
		
		positions_openmm = state_openmm.getPositions();
		
		momenta_openmm = state_openmm.getVelocities();
		
		//for(unsigned int i=0;i<positions_check.size();i++){
			//cout << "Cuda Coord x = " << positions_check [i][0] * (OpenMM::AngstromsPerNm) << " A "<< "Cuda Coord y = "  << positions_check [i][1] * (OpenMM::AngstromsPerNm)<< " A " <<"Cuda Coord z = "<< positions_check [i][2] * (OpenMM::AngstromsPerNm) << " A ";
		//}
		
		
		if(frame == 0 || frame == MAX_FRAMES-1){
		
			kinetic_energy = state_openmm.getKineticEnergy();
	
			potential_energy = state_openmm.getPotentialEnergy();
			
			cout<<"\n";
	
			cout <<"Total Energy = " << (kinetic_energy + potential_energy) * OpenMM::KcalPerKJ << " Kcal/mol "
				 << " Kinetic Energy = " << kinetic_energy  * OpenMM::KcalPerKJ << " Kcal/mol " 
				 << " Potential Energy = " << potential_energy * OpenMM::KcalPerKJ << " Kcal/mol";
			cout<<"\n";
		
		}
		
		integrator_openmm.step(1);
		
	}*/
	
	/*cout<<"\n";

	
	cout << "FINAL COORDINATES AND VELOCITIES " <<"\n\n";
	
	for(unsigned int i=0;i<positions_openmm.size();i++){
			
			cout<< "atom = " << i << " COORD X = " << positions_openmm[i][0] * (OpenMM::AngstromsPerNm) << 
									 " COORD Y = " << positions_openmm[i][1] * (OpenMM::AngstromsPerNm) << 
									 " COORD Z = " << positions_openmm[i][2] * (OpenMM::AngstromsPerNm) <<"\n";
			
			
			cout << "atom = " << i << " MOMENTA X = " << momenta_openmm[i][0] * (OpenMM::AngstromsPerNm) <<
			 						  " MOMENTA Y = " << momenta_openmm[i][1] * (OpenMM::AngstromsPerNm) << 
			 						  " MOMENTA Z = " << momenta_openmm[i][2] * (OpenMM::AngstromsPerNm) <<"\n";
			 						  
			cout<<"\n";
	}
	
	cout<< "\n";
	
	cout << "Atoms = " << nats << " Molecules = " << nmols<<"\n";*/
	
	
	//Copy back to Sire positions and velocities 
	
	int k=0;
	
	for(int i=0; i<nmols;i++){
		
		Vector *sire_coords = ws.coordsArray(i);
		
		Vector *sire_momenta = ws.momentaArray(i);	
		
		const double *m = ws.massArray(i);
		
		for(int j=0; j<ws.nAtoms(i);j++){	
		
			sire_coords[j] = Vector(positions_openmm[j+k][0] * (OpenMM::AngstromsPerNm),
									positions_openmm[j+k][1] * (OpenMM::AngstromsPerNm),
									positions_openmm[j+k][2] * (OpenMM::AngstromsPerNm));
									
			sire_momenta[j] =  Vector(velocities_openmm[j+k][0] * m[j] * (OpenMM::AngstromsPerNm),
									  velocities_openmm[j+k][1] * m[j] * (OpenMM::AngstromsPerNm),
									  velocities_openmm[j+k][2] * m[j] * (OpenMM::AngstromsPerNm));
					
		}
		
		k=k+ws.nAtoms(i);
	
	}
	
	ws.commitCoordinates();
	ws.commitVelocities();
	
	
	if (MCBarostat_flag == true) {
	
		
		OpenMM::Vec3 a;
		OpenMM::Vec3 b;
		OpenMM::Vec3 c;
	
		state_openmm.getPeriodicBoxVectors(a,b,c);
	
		cout << "\n\nNEW BOX DIMENSIONS [A] = (" << a[0] * OpenMM::AngstromsPerNm << ", " 
										   		 << b[1] * OpenMM::AngstromsPerNm << ", " 
										   		 << c[2] * OpenMM::AngstromsPerNm << ")\n\n";
										   		 
		Vector new_dims = Vector(a[0] * OpenMM::AngstromsPerNm, b[1] * OpenMM::AngstromsPerNm, c[2] * OpenMM::AngstromsPerNm);
		
		
		System & ptr_sys = ws.nonConstsystem();
		
	    PeriodicBox  sp = ptr_sys.property("space").asA<PeriodicBox>();
	    
		sp.setDimensions(new_dims);
		
		//cout << "\nBOX SIZE GAC = (" << sp.dimensions()[0] << " , " << sp.dimensions()[1] << " ,  " << sp.dimensions()[2] << ")\n\n";
		
		
		const QString stringa = "space" ;
		
		ptr_sys.setProperty(stringa,sp);
							   		 								   		 							   		
	} 
	
	
	
	//cout << " record_stats " << record_stats ;
	
	if (record_stats)
        ws.collectStatistics();
        
        
        
    cout << "\n\nCOPY OUT Simulation time = " << timer_OUT.elapsed() / 1000.0 << " s"<<"\n\n";    
        
	
	/*cout << "FINAL COORDINATES AND VELOCITIES AFTER COMMIT" <<"\n\n";
	
	const int nmols_gac = ws.nMolecules();
	
	int system_index_gac = 0;
		
	for (int i=0; i < nmols_gac; ++i){
	
		const int nats_gac = ws.nAtoms(i);

		Vector *c_gac = ws.coordsArray(i);
	
		Vector *p_gac = ws.momentaArray(i);

		for (int j=0; j < nats_gac; ++j){
	     
			
			
			cout <<"\n";
			cout << "atom = " << system_index_gac  << " COORD X = " << c_gac[j].x() << " COORD Y = " << c_gac[j].y() << " COORD Z = " << c_gac[j].z()<<"\n" ;
			cout << "atom = " << system_index_gac  << " MOMENTA X = " << p_gac[j].x() << " MOMENTA Y = " << p_gac[j].y() << " MOMENTA Z = " << p_gac[j].z()<<"\n" ;
				     
	     	system_index_gac = system_index_gac + 1;
	     
		}
		
		
	}*/
		
	
	
	cout << "\n---------------------------------------------\n";
	

	return;

}


/** Get the cufott type: nocutoff, cutoffnonperiodic, cutoffperiodic */
QString OpenMMIntegrator::getCutoffType(void){

	return CutoffType;

}

/** Set the cufott type: nocutoff, cutoffnonperiodic, cutoffperiodic */
void OpenMMIntegrator::setCutoffType(QString cutoff_type){

	CutoffType = cutoff_type;

}


/** Get the cutoff distance in A */
SireUnits::Dimension::Length OpenMMIntegrator::getCutoff_distance(void){

	return cutoff_distance;

}

/** Set the cutoff distance in A */
void OpenMMIntegrator::setCutoff_distance(SireUnits::Dimension::Length distance){

	cutoff_distance = distance;	

}

/** Get the dielectric constant */
double OpenMMIntegrator::getField_dielectric(void){

	return field_dielectric;
}

/** Set the dielectric constant */
void OpenMMIntegrator::setField_dielectric(double dielectric){
	
	field_dielectric=dielectric;

}

/** Set Andersen thermostat */

void OpenMMIntegrator::setAndersen(bool andersen){
	Andersen_flag = andersen;	
}

/** Get Andersen thermostat status on/off */
bool OpenMMIntegrator::getAndersen(void){
	
	return 	Andersen_flag;
	
}


/** Get the Andersen Thermostat frequency collision */
double OpenMMIntegrator::getAndersen_frequency(void){
	
	return Andersen_frequency;
	
}

/** Set the Andersen Thermostat frequency collision */
void OpenMMIntegrator::setAndersen_frequency(double freq){
	
	Andersen_frequency=freq;
	
}


/** Get the bath Temperature */
SireUnits::Dimension::Temperature OpenMMIntegrator::getTemperature(void){
		
	return Temperature;	
}

/** Set the Temperature */
void OpenMMIntegrator::setTemperature(SireUnits::Dimension::Temperature temperature){
		
	Temperature = temperature;	
}

/** Set Monte Carlo Barostat on/off */
void OpenMMIntegrator::setMCBarostat(bool MCBarostat){
	MCBarostat_flag = MCBarostat;	
}

/** Get Andersen thermostat status on/off */
bool OpenMMIntegrator::getMCBarostat(void){
	
	return 	MCBarostat_flag;
	
}

/** Get the Monte Carlo Barostat frequency in time speps */
int OpenMMIntegrator::getMCBarostat_frequency(void){
	
	return  MCBarostat_frequency;
	
}

/** Set the Monte Carlo Barostat frequency in time speps */
void OpenMMIntegrator::setMCBarostat_frequency(int freq){
	
	MCBarostat_frequency=freq;
	
}

/** Get the Presure */
SireUnits::Dimension::Pressure OpenMMIntegrator::getPressure(void){
		
	return Pressure;	
}

/** Set the Pressure */
void OpenMMIntegrator::setPressure(SireUnits::Dimension::Pressure pressure){
		
	Pressure = pressure;	
}


/** Get the Constraint type: none, hbonds, allbonds, hangles */
QString OpenMMIntegrator::getConstraintType(void){
	
	return ConstraintType;

}

/** Set the Constraint type: none, hbonds, allbonds, hangles */
void OpenMMIntegrator::setConstraintType(QString constrain){

	ConstraintType = constrain;

}

/** Get the OpenMM Platform: CUDA, OpenCL, CPU */
QString OpenMMIntegrator::getPlatform(void){

	return platform_type;
	
}

/** Set the OpenMM Platform: CUDA, OpenCL, CPU */
void OpenMMIntegrator::setPlatform(QString platform){

	platform_type = platform;

}

/** Get the alchemical values used to calculate the free energy change via TI method*/
QVector<double> OpenMMIntegrator::getAlchemical_values(void){

	return Alchemical_values;

}

/** Set the alchemical values used to calculate the free energy change via TI method*/
void OpenMMIntegrator::setAlchemical_values(QVector<double> lambda_values){

	Alchemical_values = lambda_values;

}




/** Create an empty workspace */
IntegratorWorkspacePtr OpenMMIntegrator::createWorkspace(
                                                const PropertyMap &map) const
{
    return IntegratorWorkspacePtr( new AtomicVelocityWorkspace(map) );
}

/** Return the ensemble of this integrator */
Ensemble OpenMMIntegrator::ensemble() const
{
    return Ensemble::NVE();
}

/** Return whether or not this integrator is time-reversible */
bool OpenMMIntegrator::isTimeReversible() const
{
    return true;
}

/** Create a workspace for this integrator for the molecule group 'molgroup' */
IntegratorWorkspacePtr OpenMMIntegrator::createWorkspace(
                                                const MoleculeGroup &molgroup,
                                                const PropertyMap &map) const
{
    return IntegratorWorkspacePtr( new AtomicVelocityWorkspace(molgroup,map) );
}

const char* OpenMMIntegrator::typeName()
{
    return QMetaType::typeName( qMetaTypeId<OpenMMIntegrator>() );
}







int OpenmmPDB(IntegratorWorkspace &workspace, std::vector<OpenMM::Vec3> &positions_openmm){

    AtomicVelocityWorkspace &ws = workspace.asA<AtomicVelocityWorkspace>();

    const int nmols = ws.nMolecules();
    
    MoleculeGroup molgroup = ws.moleculeGroup();

    //ofstream file_write("data.pdb",ios::app);

   /*if(!file_write) {
        cout << "Error File .pdb";
        return -1;
    }*/

    static int counter=0;
    static int call=0;
    
    int index = 0;

    //file_write << "MODEL      " << call << "\n";

    for (int i=0; i < nmols; i++){
 
        Molecule molecule = molgroup.moleculeAt(i).molecule();
        
       	const int nats = ws.nAtoms(i);
       	
        QString molecule_name = molecule.residue(ResIdx(0)).toString();
       

        for (int j=0; j<nats ;j++){
        
            QString atom =  molecule.atom(AtomIdx(j)).toString();
                      
            cout.width(6); cout << left << "HETATM"; 
            cout.width(5); cout << right << counter + 1; 
            cout.width(4); cout << uppercase << right << atom.mid(6,2).toStdString();
            cout.width(1); cout << " "; 
            cout.width(3); cout << uppercase << right << molecule_name.mid(9,3).toStdString(); 
            cout.width(1); cout << " ";
            cout.width(4); cout << right << i+1;
            cout.width(1); cout << " ";
            
            cout.setf(ios::fixed, ios::floatfield);
			cout.precision(3);
            
            cout.width(8); cout << right  << positions_openmm[index][0];
            cout.width(8); cout << right  << positions_openmm[index][1];
            cout.width(8); cout << right  << positions_openmm[index][2]; 
            cout.precision(2);
            cout.width(6); cout << right << 1.00;
            cout.width(6); cout << right << 0.00 << endl;
            
   			index = index + 1 ;	
   			counter = counter +1;         

        }

    }
    
    
    call = call+1;

    //file_write.close();
    
    return 0;
} 




