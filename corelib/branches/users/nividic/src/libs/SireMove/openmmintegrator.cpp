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
#include "SireMol/mgname.h"
#include <iostream>
#include <QElapsedTimer>
#include <iomanip>
#include "fastio.h"

/* defines used by write_dcdstep */
#define NFILE_POS 8L
#define NSTEP_POS 20L

/* Define error codes that may be returned by the DCD routines */
#define DCD_SUCCESS      0  /* No problems                     */
#define DCD_EOF         -1  /* Normal EOF                      */
#define DCD_DNE         -2  /* DCD file does not exist         */
#define DCD_OPENFAILED  -3  /* Open of DCD file failed         */
#define DCD_BADREAD     -4  /* read call on DCD file failed    */
#define DCD_BADEOF      -5  /* premature EOF found in DCD file */
#define DCD_BADFORMAT   -6  /* format of DCD file is wrong     */
#define DCD_FILEEXISTS  -7  /* output file already exists      */
#define DCD_BADMALLOC   -8  /* malloc failed                   */
#define DCD_BADWRITE    -9  /* write call on DCD file failed   */

/* WRITE Macro to make porting easier */
#define WRITE(fd, buf, size) fio_fwrite(((void *) buf), (size), 1, (fd))


static int write_dcdheader(fio_fd fd, const char *remarks, int N, 
			      int ISTART, int NSAVC, double DELTA, int with_unitcell,
				  int charmm);				

static int write_dcdstep(fio_fd fd, int curframe, int curstep, int N, 
                  float *X, float *Y, float *Z, 
				  const double *unitcell, int charmm);
				  
				  
void integrator(const char * filename, OpenMM::Context & context_openmm,OpenMM::VerletIntegrator & integrator_openmm, 
				std::vector<OpenMM::Vec3> & positions_openmm, std::vector<OpenMM::Vec3> & velocities_openmm,
				bool DCD,bool wrap, int nmoves, int frequency_dcd, int flag_cutoff,int nats);
				
QString file_name(int i);


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
                 CutoffType("nocutoff"), cutoff_distance(1.0 * nanometer),field_dielectric(78.3),
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
	
	const System & ptr_sys = ws.system();
		
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
	

	OpenMM::NonbondedForce * nonbond_openmm = new OpenMM::NonbondedForce();
	
	/*****************************************************************IMPORTANT********************************************************************/
	
	
	if(free_energy_calculation == false)
		system_openmm.addForce(nonbond_openmm);	
	
	OpenMM::CustomNonbondedForce * custom_nonbond_SoluteSolvent_openmm=NULL;

	
		
	//Long Range interaction non bonded force method setting

	if(flag_cutoff == NOCUTOFF){
				
		nonbond_openmm->setNonbondedMethod(OpenMM::NonbondedForce::NoCutoff);	
				
		if(free_energy_calculation == true){
			
																				   
			/*custom_nonbond_SoluteSolvent_openmm = new OpenMM::CustomNonbondedForce( "-4*eps*(dem^2-dem)-138.935456*q/r;"
																			   	   	"q=q1*q2;"
																			   	   	"dem=(sigma/r)^6;"
																			   		"sigma=0.5*(sigma1+sigma2);"
																			   		"eps=sqrt(eps1*eps2)");	*/
																			   		
			/*custom_nonbond_SoluteSolvent_openmm = new OpenMM::CustomNonbondedForce( "max(isSolute1,isSolute2)*(Hsoft - Hinter);"
																					"Hinter=4.0*eps*(LJdel^2-LJdel)+138.935456*q/r;"
																					"LJdel=(sigma/r)^6;"
																					"Hsoft=Hc+Hl;"
																					"Hc=lam_cl^(n+1)*138.935456*q/sqrt(diff_cl+r^2);"
																					"q=q1*q2;"
																					"diff_cl=(1.0-lam_cl);"
																					"lam_cl=min(1,max(0,lambda-1));"
																					"Hl=lam_lj*4.0*eps*(LJ^2-LJ);"
																					"eps=sqrt(eps1*eps2);"
																					"LJ=(sigma^2/soft)^3;"
																					"soft=(diff_lj*delta*sigma+r^2);"
																					"sigma=0.5*(sigma1+sigma2);"
																					"diff_lj=(1.0-lam_lj);"
																					"lam_lj=max(0,min(1,lambda))");*/
																					
			
			custom_nonbond_SoluteSolvent_openmm = new OpenMM::CustomNonbondedForce( "ZeroOne*(Hsoft)+ (1-ZeroOne)*(Hinter);"
																					"Hinter=4.0*eps*(LJdel^2-LJdel)+138.935456*q/r;"
																					"LJdel=(sigma/r)^6;"
																					"ZeroOne=(isSolute1-isSolute2)^2;"
																					"Hsoft=Hc+Hl;"
																					"Hc=lam_cl^(n+1)*138.935456*q/sqrt(diff_cl+r^2);"
																					"q=q1*q2;"
																					"diff_cl=(1.0-lam_cl);"
																					"lam_cl=min(1,max(0,lambda-1));"
																					"Hl=lam_lj*4.0*eps*(LJ^2-LJ);"
																					"eps=sqrt(eps1*eps2);"
																					"LJ=(sigma^2/soft)^3;"
																					"soft=(diff_lj*delta*sigma+r^2);"
																					"sigma=0.5*(sigma1+sigma2);"
																					"diff_lj=(1.0-lam_lj);"
																					"lam_lj=max(0,min(1,lambda))");																	
																			
			
			custom_nonbond_SoluteSolvent_openmm->addGlobalParameter("lambda",Alchemical_values[0]);
			
			int coulomb_Power = ptr_sys.property("coulombPower").toString().toInt();
			double shift_Delta = ptr_sys.property("shiftDelta").toString().toDouble();
			
			custom_nonbond_SoluteSolvent_openmm->addGlobalParameter("delta",shift_Delta);
			custom_nonbond_SoluteSolvent_openmm->addGlobalParameter("n",coulomb_Power);																   		
															   
			
			custom_nonbond_SoluteSolvent_openmm->setNonbondedMethod(OpenMM::CustomNonbondedForce::NoCutoff);
			
			cout << "Lambda = " << Alchemical_values[0] << " Coulomb Power = " << coulomb_Power << " Delta Shift = " << shift_Delta <<"\n";
				
		}			
		
		cout << "\nCut off type = " << CutoffType.toStdString() << "\n";
	}
	
	
	if(flag_cutoff == CUTOFFNONPERIODIC || flag_cutoff == CUTOFFPERIODIC){
	
		const double converted_cutoff_distance = convertTo(cutoff_distance.value(), nanometer);
		
		if(flag_cutoff == CUTOFFNONPERIODIC)
			nonbond_openmm->setNonbondedMethod(OpenMM::NonbondedForce::CutoffNonPeriodic);
		
		else
			nonbond_openmm->setNonbondedMethod(OpenMM::NonbondedForce::CutoffPeriodic);
			
		
		nonbond_openmm->setCutoffDistance(converted_cutoff_distance);
		
		//Set Dielectric constant media
		nonbond_openmm->setReactionFieldDielectric(field_dielectric);
		
		//system_openmm.setDefaultPeriodicBoxVectors(OpenMM::Vec3(0,0,0),OpenMM::Vec3(0,0,0),OpenMM::Vec3(0,0,0));
		
		
		if(free_energy_calculation == true){
	
	
			/*custom_nonbond_SoluteSolvent_openmm = new OpenMM::CustomNonbondedForce( "-4*eps*(dem^2-dem)-138.935456*q*(1.0/r+(krf*r*r)-crf);"
																			   	   	"q=q1*q2;"
																			   	   	"dem=(sigma/r)^6;"
																			   		"sigma=0.5*(sigma1+sigma2);"
																			   		"eps=sqrt(eps1*eps2)");*/
																			   		
								
			/*custom_nonbond_SoluteSolvent_openmm = new OpenMM::CustomNonbondedForce( "max(isSolute1,isSolute2)*(Hsoft-Hinter);"
																					"Hinter=4*eps*(LJdel^2-LJdel)+138.935456*q*(1.0/r+(krf*r*r)-crf);"
																					"LJdel=(sigma/r)^6;"
																					"Hsoft=Hc+Hl;"
																					"Hc=lam_cl^(n+1)*138.935456*q/sqrt(diff_cl+r^2);"
																					"q=q1*q2;"
																					"diff_cl=(1.0-lam_cl);"
																					"lam_cl=min(1,max(0,lambda-1));"
																					"Hl=lam_lj*4.0*eps*(LJ^2-LJ);"
																					"eps=sqrt(eps1*eps2);"
																					"LJ=(sigma^2/soft)^3;"
																					"soft=(diff_lj*delta*sigma+r^2);"
																					"sigma=0.5*(sigma1+sigma2);"
																					"diff_lj=(1.0-lam_lj);"
																					"lam_lj=max(0,min(1,lambda))");*/
																					
			
			
			custom_nonbond_SoluteSolvent_openmm = new OpenMM::CustomNonbondedForce( "ZeroOne*(Hsoft)+(1-ZeroOne)*(Hinter);"
																					"Hinter=4*eps*(LJdel^2-LJdel)+138.935456*q*(1.0/r+(krf*r*r)-crf);"
																					"ZeroOne=(isSolute1-isSolute2)^2;"
																					"LJdel=(sigma/r)^6;"
																					"Hsoft=Hc+Hl;"
																					"Hc=lam_cl^(n+1)*138.935456*q/sqrt(diff_cl+r^2);"
																					"q=q1*q2;"
																					"diff_cl=(1.0-lam_cl);"
																					"lam_cl=min(1,max(0,lambda-1));"
																					"Hl=lam_lj*4.0*eps*(LJ^2-LJ);"
																					"eps=sqrt(eps1*eps2);"
																					"LJ=(sigma^2/soft)^3;"
																					"soft=(diff_lj*delta*sigma+r^2);"
																					"sigma=0.5*(sigma1+sigma2);"
																					"diff_lj=(1.0-lam_lj);"
																					"lam_lj=max(0,min(1,lambda))");
																																												   		
																			   		
			custom_nonbond_SoluteSolvent_openmm->addGlobalParameter("lambda",Alchemical_values[0]);
		
			int coulomb_Power = ptr_sys.property("coulombPower").toString().toInt();
			double shift_Delta = ptr_sys.property("shiftDelta").toString().toDouble();
			
			custom_nonbond_SoluteSolvent_openmm->addGlobalParameter("delta",shift_Delta);
			custom_nonbond_SoluteSolvent_openmm->addGlobalParameter("n",coulomb_Power);																   		
																			   			
			double eps2 = (field_dielectric-1.0)/(2.0*field_dielectric+1.0);
			double kValue = eps2/(converted_cutoff_distance*converted_cutoff_distance*converted_cutoff_distance);
			
			
			custom_nonbond_SoluteSolvent_openmm->addGlobalParameter("krf",kValue);
			
			double cValue = (1.0/converted_cutoff_distance)*(3.0*field_dielectric)/(2.0*field_dielectric+1.0);
			
			custom_nonbond_SoluteSolvent_openmm->addGlobalParameter("crf",cValue);
			
			
			custom_nonbond_SoluteSolvent_openmm->setCutoffDistance(converted_cutoff_distance);
		
		
			if(flag_cutoff == CUTOFFNONPERIODIC)
				custom_nonbond_SoluteSolvent_openmm->setNonbondedMethod(OpenMM::CustomNonbondedForce::CutoffNonPeriodic);
		
			else
				custom_nonbond_SoluteSolvent_openmm->setNonbondedMethod(OpenMM::CustomNonbondedForce::CutoffPeriodic);
			
		
			cout << "Lambda = " << Alchemical_values[0] << " Coulomb Power = " << coulomb_Power << " Delta Shift = " << shift_Delta <<"\n";
		
		}
	
	
	
		cout << "\nCut off type = " << CutoffType.toStdString() << "\n";
		cout << "CutOff distance = " << converted_cutoff_distance  << " Nm" << "\n";
   	 	cout << "Dielectric constant= " << field_dielectric << "\n\n";
	
	
	
	
	}
	
	
	
	if(free_energy_calculation == true)
		system_openmm.addForce(custom_nonbond_SoluteSolvent_openmm);

		
	
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
    
    
    /*****************************************************************IMPORTANT********************************************************************/
	
	system_openmm.addForce(bondStretch_openmm);
    
	system_openmm.addForce(bondBend_openmm);
    
	system_openmm.addForce(bondTorsion_openmm);
	
	
	std::vector<std::pair<int,int> > bondPairs;
	
	
	
	//OpenMM vector coordinate
    
	std::vector<OpenMM::Vec3> positions_openmm(nats);		
	
    
	//OpenMM vector momenta
    
	std::vector<OpenMM::Vec3> velocities_openmm(nats);		
    
	int system_index = 0;

	//cout << "INITAL COORDINATES AND VELOCITIES\n\n";
	
	QElapsedTimer timer_IN;
	
	timer_IN.start();
	
	for (int i=0; i < nmols; ++i){
	
		const int nats_mol = ws.nAtoms(i);

		Vector *c = ws.coordsArray(i);
		
		Vector *p = ws.momentaArray(i);
	
		const double *m = ws.massArray(i);
		
		//cout << "Molecule = " << i <<"\n";

		for (int j=0; j < nats_mol; ++j){
	     
			positions_openmm[system_index] = OpenMM::Vec3(c[j].x() * (OpenMM::NmPerAngstrom) ,
							c[j].y() * (OpenMM::NmPerAngstrom),c[j].z() * (OpenMM::NmPerAngstrom));
	    
			
	     
			velocities_openmm[system_index] = OpenMM::Vec3(p[j].x()/m[j] * (OpenMM::NmPerAngstrom) ,
						  p[j].y()/m[j] * (OpenMM::NmPerAngstrom), p[j].z()/m[j] * (OpenMM::NmPerAngstrom));

	     
			system_openmm.addParticle(m[j]) ;
			     
			system_index = system_index + 1;
			

			
			/*cout << "\natom = " << system_index - 1  << " COORD X = " << c[j].x() 
							    					 << " COORD Y = " << c[j].y() 
							  						 << " COORD Z = " << c[j].z()<<"\n" ;*/
			
			/*cout << "atom = " << system_index - 1  << " MOMENTA X = " << p[j].x() 
												   << " MOMENTA Y = " << p[j].y() 
												   << " MOMENTA Z = " << p[j].z()<<"\n" ;
			
			cout << "atom = " << j << " MASS = " << m[j]<<"\n" ; */
			
	     
		}

	   
	}
      
	

	
	MoleculeGroup molgroup = ws.moleculeGroup();
	
	
	int num_atoms_till_i = 0;
	
	
	MoleculeGroup solute_group;
	
	if(free_energy_calculation == true){
		custom_nonbond_SoluteSolvent_openmm->addPerParticleParameter("q");
		custom_nonbond_SoluteSolvent_openmm->addPerParticleParameter("sigma");
		custom_nonbond_SoluteSolvent_openmm->addPerParticleParameter("eps");
		custom_nonbond_SoluteSolvent_openmm->addPerParticleParameter("isSolute");
		
		const QString solute = "solute";
			
		MGName Solute(solute);
		
    	solute_group = ptr_sys[Solute];
    
    }
   
		

	
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
			
			nonbond_openmm->addParticle(charge, sigma * OpenMM::NmPerAngstrom, epsilon * OpenMM::KJPerKcal);
			

			Atom atom = molecule.molecule().atoms()[j];
			
			//cout << "Is Solute = "<< (double) solute_group.contains(atom.molecule()) <<" J = "<< j << "\n";
			
			
			if( free_energy_calculation == true){
			
				std::vector<double> params(4);
				
				params[0] = charge;
				params[1] = sigma * OpenMM::NmPerAngstrom;
				params[2] = epsilon * OpenMM::KJPerKcal;
				params[3] = (double) solute_group.contains(atom.molecule());
				

				custom_nonbond_SoluteSolvent_openmm->addParticle(params);
				
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
	    
	    //Connectivity connectivity = molecule.property("connectivity").asA<Connectivity>();
	    
	    /* If the molecule does not have a connectivity, then we cannot get bonds/angles/dihedrals 
	    	(effectively assuming it is a monoatomic molecule)*/
	    
	    if ( !hasConnectivity ){
	    	num_atoms_till_i = num_atoms_till_i + num_atoms_molecule ;
	    	cout << "\n Atoms = " <<  num_atoms_molecule << "Num atoms till i =" << num_atoms_till_i <<"\n";
	    	cout << "\n*********************ION DETECTED**************************\n";
	    	
	        continue;
	    }    
		
		
		
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
	
	nonbond_openmm->createExceptionsFromBonds(bondPairs, Coulob14Scale, LennardJones14Scale);
	
	/*for(unsigned int i=0; i<bondPairs.size();i++){
			
		cout << "Bond excluded = ( " << bondPairs[i].first << " , " << bondPairs[i].second << " )";
		cout << "\n";
	}*/
	
	
	QVector< QPair<int,int> > excluded_vector_12_13;
	
	int num_ex=nonbond_openmm->getNumExceptions();
	
	//cout << "\n\n NUM pair excluded = " << num_ex <<"\n";
	
	if(free_energy_calculation == true){
			
			for (int i = 0 ; i< num_ex; i++){
	
				int  p1=0;
				int  p2=0;
	
				double  chargeprod=0;
				double  sigma_ex=0;
				double  epsilon_ex=0;

				nonbond_openmm->getExceptionParameters(i,p1,p2,chargeprod,sigma_ex,epsilon_ex);
				
			    custom_nonbond_SoluteSolvent_openmm->addExclusion(p1,p2);
				
				
				
				/*cout << "Index = " << i << " (" << p1 << " , " << p2 << ")" << " charge prod = " 
					 << chargeprod << " sigma  = " << sigma_ex << " epsilon = " << epsilon_ex << "\n";*/
				
				//nonbond_openmm->addException(19,5,0.0,1.0,0.0,false);
				//custom_nonbond_SoluteSolvent_openmm->addExclusion(19,5);
			
				
				//nonbond_openmm->getParticleParameters(p1,charge_p1,sigma_p1,epsilon_p1);
		
				//nonbond_openmm->getParticleParameters(p2,charge_p2,sigma_p2,epsilon_p2);
				
				//cout << "p1 = " << p1 << " charge  p1 = " << charge_p1 << " sigma p1 = " << sigma_p1 << " epasilon p1 = " << epsilon_p1 <<"\n";
				//cout << "p2 = " << p2 << " charge  p2 = " << charge_p2 << " sigma p2 = " << sigma_p2 << " epasilon p2 = " << epsilon_p1 <<"\n\n";
		
				/*if(chargeprod==0.0 && sigma_ex == 1.0 && epsilon_ex ==0.0){
					
					excluded_vector_12_13.append(qMakePair(p1,p2));			
				}	*/		
			}
	
			
			/*for (int i=0;i<excluded_vector_12_13.size();i++){
							
				cout << "( " << (excluded_vector_12_13[i]).first << " , " << (excluded_vector_12_13[i]).second << " )\n";										
			
			}*/
	}


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
	
	QElapsedTimer timer_OUT;
	
	timer_MD.start();
	
	//MD SIMULATION FOR NMOVES
	
	//OpenMM::CMMotionRemover(1); 
	
	
	
	if(free_energy_calculation == true){
	
		std::vector<OpenMM::Vec3> positions_openmm_start = positions_openmm;
    
		std::vector<OpenMM::Vec3> velocities_openmm_start = velocities_openmm;
	
		double delta = 0.001;
			
		const double beta = 1.0 / (0.0083144621 * convertTo(Temperature.value(), kelvin));
		
		cout << "\nBETA = " << beta <<"\n";
		
		int frequency_energy = 100; 
		
		int n_freq = nmoves/frequency_energy;
		
		cout << "NFREQ = "<< n_freq << "\n\n";
		
		
		bool dcd = true;
		
		bool wrap = false;
			
		int frequency_dcd = 1;
		
	
		
		for (int i=0; i<Alchemical_values.size();i++){
				
							
			double GF_acc = 0.0;
			
			double GB_acc = 0.0;
		
			double lam_val = 0.0;
		
					
			context_openmm.setParameter("lambda",Alchemical_values[i]);
			
			state_openmm=context_openmm.getState(infoMask);
			
			lam_val = context_openmm.getParameter("lambda");
					
			//cout << "Lambda = " << lam_val << " Potential energy lambda  = " << state_openmm.getPotentialEnergy() * OpenMM::KcalPerKJ << " kcal/mol" << "\n";
				
		
			for(int j=0;j<n_freq;j++){
			
				QString name = file_name(j);
					
				integrator(name.toStdString().c_str(), context_openmm,integrator_openmm, positions_openmm, 
						   velocities_openmm, dcd,wrap , frequency_energy, frequency_dcd, flag_cutoff, nats);
				
					
				//integrator_openmm.step(frequency_energy);
					
					
				cout<< "\nTotal Time = " << state_openmm.getTime() << " ps"<<"\n\n";		
					
				state_openmm=context_openmm.getState(infoMask);	
					
				lam_val = context_openmm.getParameter("lambda");
					
				double potential_energy_lambda = state_openmm.getPotentialEnergy();
					
				cout << "Lambda = " << lam_val << " Potential energy lambda MD = " << potential_energy_lambda  * OpenMM::KcalPerKJ << " kcal/mol" << "\n";
					
					
					
				context_openmm.setParameter("lambda",Alchemical_values[i]+delta);
			
				state_openmm=context_openmm.getState(infoMask);
					
				double potential_energy_lambda_plus_delta = state_openmm.getPotentialEnergy();
			
				lam_val = context_openmm.getParameter("lambda");
					
				cout << "Lambda = " << lam_val << " Potential energy plus  = " << potential_energy_lambda_plus_delta * OpenMM::KcalPerKJ << " kcal/mol" << "\n";
					
					
					
				context_openmm.setParameter("lambda",Alchemical_values[i]-delta);
					
				state_openmm=context_openmm.getState(infoMask);
					
				double potential_energy_lambda_minus_delta = state_openmm.getPotentialEnergy();
			
				lam_val = context_openmm.getParameter("lambda");
					
				cout << "Lambda = " << lam_val << " Potential energy minus  = " << potential_energy_lambda_minus_delta * OpenMM::KcalPerKJ  << " kcal/mol" << "\n"; 
					
					
					
				GF_acc = GF_acc + exp(-beta*(potential_energy_lambda_plus_delta - potential_energy_lambda));
					
					
				GB_acc = GB_acc + exp(-beta*(potential_energy_lambda_minus_delta - potential_energy_lambda));
						
				//cout << "\nGF accumulator partial = " << GF_acc << " -- GB accumulator partial = " << GB_acc << "\n\n";
					
				if(isnormal(GF_acc==0) || isnormal(GB_acc==0)){ 
					cout << "\n\n ********************** ERROR NAN!! ****************************\n\n";
					exit(-1); 
						
				}
				
				
				double avg_GF = GF_acc /((double) j+1);
			
				double avg_GB = GB_acc /((double) j+1);
				
				double Energy_GF = -(1.0/beta)*log(avg_GF);
			
				double Energy_GB = -(1.0/beta)*log(avg_GB);
				
				
				double Energy_Gradient_lamda = (Energy_GF - Energy_GB) / (2.0 * delta);
			
				cout << "\n\nEnergy Gradient = " << Energy_Gradient_lamda * OpenMM::KcalPerKJ << " kcal/(mol lambda)" << "\n\n";
					
					
				context_openmm.setParameter("lambda",Alchemical_values[i]);
					
				
			}//end for cycle
			
			/*cout << "\n\nGF accumulator total = " << GF_acc << " ** GB accumulator total = " << GB_acc << "\n"; 
			
			double avg_GF = GF_acc / ((double) n_freq);
			
			double avg_GB = GB_acc /((double) n_freq);
			
			
			cout << "\n\nAverage GF = " << avg_GF << " Average GB = " << avg_GB << "\n";
			
	
			double Energy_GF = -(1.0/beta)*log(avg_GF);
			
			double Energy_GB = -(1.0/beta)*log(avg_GB);
			
		
			cout <<"\n\nEnergy GF = "<< Energy_GF << " Energy GB = " << Energy_GB << "\n"; 
		
			
			double Energy_Gradient_lamda = (Energy_GF - Energy_GB) / (2.0 * delta);
			
			cout << "\n\nEnergy Gradient = " << Energy_Gradient_lamda * OpenMM::KcalPerKJ << " kcal/(mol lambda)" << "\n\n";*/
			
			
			
			context_openmm.setPositions(positions_openmm_start);
			
			context_openmm.setVelocities(velocities_openmm_start); 
			
			context_openmm.setTime(0.0);
			
	
		}
		
		cout << "\nMD Simulation time = " << timer_MD.elapsed() / 1000.0 << " s"<<"\n\n";
		
	
	}
	
	else{/* ******************* MD ***********************/
		
		timer_OUT.start();
		
		int frequency_dcd = 10;
		
		bool dcd = true;
		
		bool wrap = true;
		
		integrator("dynamic.dcd", context_openmm,integrator_openmm, positions_openmm, velocities_openmm, dcd,wrap , nmoves, frequency_dcd, flag_cutoff, nats);
		
		
		cout << "\nMD Simulation time = " << timer_MD.elapsed() / 1000.0 << " s"<<"\n\n";
	
	
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
		
		
		
		
		cout << "\nMD Simulation time = " << timer_MD.elapsed() / 1000.0 << " s"<<"\n\n";
			
	}
	
	
	timer_OUT.start();
	

	
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
		
		
		const QString string = "space" ;
		
		ptr_sys.setProperty(string,sp);
							   		 								   		 							   		
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



/*
 * Write a header for a new dcd file
 * Input: fd - file struct opened for binary writing
 *        remarks - string to be put in the remarks section of the header.  
 *                  The string will be truncated to 70 characters.
 *        natoms, istart, nsavc, delta - see comments in read_dcdheader
 * Output: 0 on success, negative error code on failure.
 * Side effects: Header information is written to the dcd file.
 */
static int write_dcdheader(fio_fd fd, const char *remarks, int N, 
                    int ISTART, int NSAVC, double DELTA, int with_unitcell,
                    int charmm) {
  int out_integer;
  float out_float;
  char title_string[200];
  time_t cur_time;
  struct tm *tmbuf;
  char time_str[81];

  out_integer = 84;
  WRITE(fd, (char *) & out_integer, sizeof(int));
  strcpy(title_string, "CORD");
  WRITE(fd, title_string, 4);
  fio_write_int32(fd, 0);      /* Number of frames in file, none written yet   */
  fio_write_int32(fd, ISTART); /* Starting timestep                            */
  fio_write_int32(fd, NSAVC);  /* Timesteps between frames written to the file */
  fio_write_int32(fd, 0);      /* Number of timesteps in simulation            */
  fio_write_int32(fd, 0);      /* NAMD writes NSTEP or ISTART - NSAVC here?    */
  fio_write_int32(fd, 0);
  fio_write_int32(fd, 0);
  fio_write_int32(fd, 0);
  fio_write_int32(fd, 0);
  if (charmm) {
    out_float = DELTA;
    WRITE(fd, (char *) &out_float, sizeof(float));
    if (with_unitcell) {
      fio_write_int32(fd, 1);
    } else {
      fio_write_int32(fd, 0);
    }
  } else {
    WRITE(fd, (char *) &DELTA, sizeof(double));
  }
  fio_write_int32(fd, 0);
  fio_write_int32(fd, 0);
  fio_write_int32(fd, 0);
  fio_write_int32(fd, 0);
  fio_write_int32(fd, 0);
  fio_write_int32(fd, 0);
  fio_write_int32(fd, 0);
  fio_write_int32(fd, 0);
  if (charmm) {
    fio_write_int32(fd, 24); /* Pretend to be Charmm version 24 */
  } else {
    fio_write_int32(fd, 0);
  }
  fio_write_int32(fd, 84);
  fio_write_int32(fd, 164);
  fio_write_int32(fd, 2);

  strncpy(title_string, remarks, 80);
  title_string[79] = '\0';
  WRITE(fd, title_string, 80);

  cur_time=time(NULL);
  tmbuf=localtime(&cur_time);
  strftime(time_str, 80, "REMARKS Created %d %B, %Y at %R", tmbuf);
  WRITE(fd, time_str, 80);

  fio_write_int32(fd, 164);
  fio_write_int32(fd, 4);
  fio_write_int32(fd, N);
  fio_write_int32(fd, 4);

  return DCD_SUCCESS;
}


/* 
 * Write a timestep to a dcd file
 * Input: fd - a file struct for which a dcd header has already been written
 *       curframe: Count of frames written to this file, starting with 1.
 *       curstep: Count of timesteps elapsed = istart + curframe * nsavc.
 *        natoms - number of elements in x, y, z arrays
 *        x, y, z: pointers to atom coordinates
 * Output: 0 on success, negative error code on failure.
 * Side effects: coordinates are written to the dcd file.
 */
static int write_dcdstep(fio_fd fd, int curframe, int curstep, int N, 
                  float *X, float *Y, float *Z, 
                  const double *unitcell, int charmm) {
  int out_integer;
  //int rc;

  if (charmm) {
    /* write out optional unit cell */
    if (unitcell != NULL) {
      out_integer = 48; /* 48 bytes (6 doubles) */
      fio_write_int32(fd, out_integer);
      WRITE(fd, unitcell, out_integer);
      fio_write_int32(fd, out_integer);
    }
  }

  /* write out coordinates */
  out_integer = N*4; /* N*4 bytes per X/Y/Z array (N floats per array) */
  fio_write_int32(fd, out_integer);
  if (fio_fwrite((void *) X, out_integer, 1, fd) != 1) return DCD_BADWRITE;
  fio_write_int32(fd, out_integer);
  fio_write_int32(fd, out_integer);
  if (fio_fwrite((void *) Y, out_integer, 1, fd) != 1) return DCD_BADWRITE;
  fio_write_int32(fd, out_integer);
  fio_write_int32(fd, out_integer);
  if (fio_fwrite((void *) Z, out_integer, 1, fd) != 1) return DCD_BADWRITE;
  fio_write_int32(fd, out_integer);

  /* update the DCD header information */
  fio_fseek(fd, NFILE_POS, FIO_SEEK_SET);
  fio_write_int32(fd, curframe);
  fio_fseek(fd, NSTEP_POS, FIO_SEEK_SET);
  fio_write_int32(fd, curstep);
  fio_fseek(fd, 0, FIO_SEEK_END);

  return DCD_SUCCESS;
}


void integrator(const char * filename, OpenMM::Context & context_openmm,OpenMM::VerletIntegrator & integrator_openmm, 
				std::vector<OpenMM::Vec3> & positions_openmm, std::vector<OpenMM::Vec3> & velocities_openmm,
				bool DCD, bool wrap, int nmoves, int frequency_dcd, int flag_cutoff,int nats){
		
		int cycles = 0;
			
		int steps = nmoves;
		
		float X[nats];
		float Y[nats];
		float Z[nats];
		
		double box_dims[6];
		
		double COG[3] = {0.0,0.0,0.0};
		
		fio_fd fd=NULL;
		
		double dt = integrator_openmm.getStepSize();
		
		int infoMask = 0;
	
		infoMask = OpenMM::State::Positions;
	
		infoMask = infoMask + OpenMM::State::Velocities; 
    
		infoMask = infoMask +  OpenMM::State::Energy;
		
		OpenMM::State state_openmm = context_openmm.getState(infoMask);
		
		double kinetic_energy = 0.0; 
		double potential_energy = 0.0; 
		
		OpenMM::Vec3 a;
		OpenMM::Vec3 b;
		OpenMM::Vec3 c;
		
		
		if(DCD == true){
		
			double delta = dt/0.0488821;
			
			cycles = nmoves/frequency_dcd;
	
			for(int i=0; i<nats;i++){
				X[i]=0.0;
				Y[i]=0.0;
				Z[i]=0.0;	
					
			}
		
		
			fio_open(filename,FIO_WRITE, &fd);
				
			steps=frequency_dcd;
			
			int box = 0;
		
			if(flag_cutoff == CUTOFFPERIODIC){
				box=1;	
			}
				
			write_dcdheader(fd, "Created by OpenMM", nats,0,frequency_dcd, delta, box,1);
			
			if(wrap == true){
				for(unsigned int i=0;i<positions_openmm.size();i++){
		
					COG[0] = COG[0] + positions_openmm[i][0]*(OpenMM::AngstromsPerNm); //X Cennter of Geometry
					COG[1] = COG[1] + positions_openmm[i][1]*(OpenMM::AngstromsPerNm); //Y Cennter of Geometry
					COG[2] = COG[2] + positions_openmm[i][2]*(OpenMM::AngstromsPerNm); //Z Cennter of Geometry
				}
		
				COG[0] = COG[0]/nats;
				COG[1] = COG[1]/nats;
				COG[2] = COG[2]/nats;

				cout << "\nCOG X = " << COG[0] << " GOG Y = " << COG[1] << " COG Z = " << COG[2] << "\n";
			
			}
		
		}
		else
			cycles=1;	
			
		
		/*for(unsigned int i=0;i<positions_openmm.size();i++){
				
				cout << "\natom = " << i << " COORD X = " << positions_openmm[i][0]*(OpenMM::AngstromsPerNm) 
									   	 << " COORD Y = " << positions_openmm[i][1]*(OpenMM::AngstromsPerNm) 
									     << " COORD Z = " << positions_openmm[i][2]*(OpenMM::AngstromsPerNm) <<"\n";		 
		}*/
		
		
		
	
		for(int i=0;i<cycles;i++){
		
		
			integrator_openmm.step(steps);
			
			
			state_openmm=context_openmm.getState(infoMask);	
	
			positions_openmm = state_openmm.getPositions();
		
			velocities_openmm = state_openmm.getVelocities();
	
			//cout<< "\nTotal Time = " << state_openmm.getTime() << " ps"<<"\n\n";
			
			kinetic_energy = state_openmm.getKineticEnergy(); 
	
			potential_energy = state_openmm.getPotentialEnergy(); 
			
	
			if(DCD==true){
			
				if(flag_cutoff == CUTOFFPERIODIC){
				
					state_openmm.getPeriodicBoxVectors(a,b,c);
	
					box_dims[0]=a[0] * OpenMM::AngstromsPerNm;
					box_dims[1]=0.0;
					box_dims[2]=b[1] * OpenMM::AngstromsPerNm;
					box_dims[3]=0.0;
					box_dims[4]=0.0;
					box_dims[5]=c[2] * OpenMM::AngstromsPerNm;
			
				}
			
				
				for(int j=0; j<nats;j++){
					
					X[j] = positions_openmm[j][0]*OpenMM::AngstromsPerNm;
					Y[j] = positions_openmm[j][1]*OpenMM::AngstromsPerNm;
					Z[j] = positions_openmm[j][2]*OpenMM::AngstromsPerNm;
					
				
					if((wrap == true) && (flag_cutoff == CUTOFFPERIODIC)){
						
						X[j] = X[j] - COG[0];
						Y[j] = Y[j] - COG[1];
						Z[j] = Z[j] - COG[2];
						
						
						X[j] = X[j] - box_dims[0]*round(X[j]/box_dims[0]);
						Y[j] = Y[j] - box_dims[2]*round(Y[j]/box_dims[2]);
						Z[j] = Z[j] - box_dims[5]*round(Z[j]/box_dims[5]);			
					
					}
					
					
					//cout << "X = "<< X[j] << " Y = " << Y[j] << " Z = " << Z[j] << "\n";
				
				}
				
				
				if(flag_cutoff == CUTOFFPERIODIC){
					
					write_dcdstep(fd, i+1, (i+1)*frequency_dcd, nats, X, Y, Z,box_dims, 1);		
								   		 
				}
				else
					write_dcdstep(fd, i+1, (i+1)*frequency_dcd, nats, X, Y, Z,NULL, 1);
					
			}

	
			/*cout <<"*Total Energy = " << (kinetic_energy + potential_energy) * OpenMM::KcalPerKJ << " Kcal/mol "
		 	 	<< " Kinetic Energy = " << kinetic_energy  * OpenMM::KcalPerKJ << " Kcal/mol " 
		 	 	<< " Potential Energy = " << potential_energy * OpenMM::KcalPerKJ << " Kcal/mol";
	
			cout<<"\n";*/
			
			//integrator_openmm.step(steps);
	
		}//end for

		if(DCD == true)
			fio_fclose(fd);

}


QString file_name(int i){

	QString num = QString::number(i);
	
	QString str;
	
	if(i<10)
		str = "0000";
	if((i>=10) && (i<100))
		str = "000";
	if((i>=100) && (i<1000))
		str = "00";
	if((i>=1000) & (i<10000))
		str = "0";
		
	str = str + num;
	
	str= str + ".dcd\0";
	
	return str;
	
}


