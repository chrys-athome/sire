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

#include "velocityverlet.h"
#include "ensemble.h"

#include "SireMol/moleculegroup.h"
#include "SireMol/partialmolecule.h"
#include "SireMol/molecule.h"
#include "SireMol/atommasses.h"
#include "SireMol/atomcoords.h"
#include "SireMol/moleditor.h"

#include "SireSystem/system.h"

#include "SireFF/forcetable.h"

#include "SireMaths/rangenerator.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include "SireUnits/units.h"
#include "SireUnits/temperature.h"
#include "SireUnits/convert.h"

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

static const RegisterMetaType<VelocityVerlet> r_velver;

/** Serialise to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, const VelocityVerlet &velver)
{
    writeHeader(ds, r_velver, 1);
    
    SharedDataStream sds(ds);
    
    sds << velver.frequent_save_velocities << static_cast<const Integrator&>(velver);
        
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, VelocityVerlet &velver)
{
    VersionID v = readHeader(ds, r_velver);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> velver.frequent_save_velocities >> static_cast<Integrator&>(velver);
    }
    else
        throw version_error(v, "1", r_velver, CODELOC);
        
    return ds;
}

/** Constructor */
VelocityVerlet::VelocityVerlet(bool frequent_save) 
               : ConcreteProperty<VelocityVerlet,Integrator>(),
                 frequent_save_velocities(frequent_save)
{}

/** Copy constructor */
VelocityVerlet::VelocityVerlet(const VelocityVerlet &other)
               : ConcreteProperty<VelocityVerlet,Integrator>(other),
                 frequent_save_velocities(other.frequent_save_velocities)
{}

/** Destructor */
VelocityVerlet::~VelocityVerlet()
{}

/** Copy assignment operator */
VelocityVerlet& VelocityVerlet::operator=(const VelocityVerlet &other)
{
    Integrator::operator=(other);
    frequent_save_velocities = other.frequent_save_velocities;
    
    return *this;
}

/** Comparison operator */
bool VelocityVerlet::operator==(const VelocityVerlet &other) const
{
    return frequent_save_velocities == other.frequent_save_velocities and
           Integrator::operator==(other);
}

/** Comparison operator */
bool VelocityVerlet::operator!=(const VelocityVerlet &other) const
{
    return not VelocityVerlet::operator==(other);
}

/** Return a string representation of this integrator */
QString VelocityVerlet::toString() const
{
    return QObject::tr("VelocityVerlet()");
}
                                                       
/** Integrate the coordinates of the atoms in the molecules in 'molgroup'
    using the forces in 'forcetable', using the optionally supplied 
    property map to find the necessary molecular properties 
    
    \throw SireMol::missing_molecule
    \throw SireBase::missing_property
    \throw SireError:invalid_cast
    \throw SireError::incompatible_error
*/
void VelocityVerlet::integrate(IntegratorWorkspace &workspace,
                               const Symbol &nrg_component,
                               SireUnits::Dimension::Time timestep,
                               int nmoves, bool record_stats) const
{
    AtomicVelocityWorkspace &ws = workspace.asA<AtomicVelocityWorkspace>();
    
    const double dt = timestep.value();

    const int nmols = ws.nMolecules();
    
    for (int imove=0; imove<nmoves; ++imove)
    {
        ws.calculateForces(nrg_component);
        
        //first integrate the coordinates - loop over all molecules
        for (int i=0; i<nmols; ++i)
        {
            const int nats = ws.nAtoms(i);
        
            Vector *x = ws.coordsArray(i);
            const Vector *f = ws.forceArray(i);
            Vector *p = ws.momentaArray(i);
            const double *m = ws.massArray(i);

            for (int j=0; j<nats; ++j)
            {
                if (m[j] != 0)
                {
                    // v(t + dt/2) = v(t) + (1/2) a(t) dt
                    p[j] += ((0.5*dt) * f[j]);

                    // r(t + dt) = r(t) + v(t + dt/2) dt
                    x[j] += (dt / m[j]) * p[j];
                }
            }
        }

        ws.commitCoordinates();
        ws.calculateForces(nrg_component);
        
        //now need to integrate the velocities
        for (int i=0; i<nmols; ++i)
        {
            const int nats = ws.nAtoms(i);
        
            const Vector *f = ws.forceArray(i);
            Vector *p = ws.momentaArray(i);
            const double *m = ws.massArray(i);

            for (int j=0; j<nats; ++j)
            {
                if (m[j] != 0)
                    p[j] += ((0.5*dt) * f[j]);
            }
        }
        
        if (frequent_save_velocities)
            ws.commitVelocities();
        
        if (record_stats)
            ws.collectStatistics();
    }
    
    if (not frequent_save_velocities)
        ws.commitVelocities();
}

/** Create an empty workspace */
IntegratorWorkspacePtr VelocityVerlet::createWorkspace(
                                                const PropertyMap &map) const
{
    return IntegratorWorkspacePtr( new AtomicVelocityWorkspace(map) );
}

/** Return the ensemble of this integrator */
Ensemble VelocityVerlet::ensemble() const
{
    return Ensemble::NVE();
}

/** Return whether or not this integrator is time-reversible */
bool VelocityVerlet::isTimeReversible() const
{
    return true;
}

/** Create a workspace for this integrator for the molecule group 'molgroup' */
IntegratorWorkspacePtr VelocityVerlet::createWorkspace(
                                                const MoleculeGroup &molgroup,
                                                const PropertyMap &map) const
{
    return IntegratorWorkspacePtr( new AtomicVelocityWorkspace(molgroup,map) );
}

const char* VelocityVerlet::typeName()
{
    return QMetaType::typeName( qMetaTypeId<VelocityVerlet>() );
}


/** Implementation of VelocityVerletBerendsen */

/** Serialise to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, const VelocityVerletBerendsen &velver)
{
    writeHeader(ds, r_velver, 1);
    
    SharedDataStream sds(ds);
    
    sds << velver.frequent_save_velocities 
	<< velver.temperature
	<< velver.tau
	<< static_cast<const Integrator&>(velver);
        
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, VelocityVerletBerendsen &velver)
{
    VersionID v = readHeader(ds, r_velver);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> velver.frequent_save_velocities 
	    >> velver.temperature
	    >> velver.tau 
	    >> static_cast<Integrator&>(velver);
    }
    else
        throw version_error(v, "1", r_velver, CODELOC);
        
    return ds;
}

/** Constructor */
VelocityVerletBerendsen::VelocityVerletBerendsen(bool frequent_save) 
               : ConcreteProperty<VelocityVerletBerendsen,Integrator>(),
                 frequent_save_velocities(frequent_save),
		 temperature(25 * celsius),
		 tau(1 * picosecond)
{}

/** Copy constructor */
VelocityVerletBerendsen::VelocityVerletBerendsen(const VelocityVerletBerendsen &other)
               : ConcreteProperty<VelocityVerletBerendsen,Integrator>(other),
                 frequent_save_velocities(other.frequent_save_velocities),
		 temperature(other.temperature),
		 tau(other.tau)
{}

/** Destructor */
VelocityVerletBerendsen::~VelocityVerletBerendsen()
{}

/** Copy assignment operator */
VelocityVerletBerendsen& VelocityVerletBerendsen::operator=(const VelocityVerletBerendsen &other)
{
    Integrator::operator=(other);
    frequent_save_velocities = other.frequent_save_velocities;
    temperature = other.temperature;
    tau = other.tau;
    
    return *this;
}

/** Comparison operator */
bool VelocityVerletBerendsen::operator==(const VelocityVerletBerendsen &other) const
{
    return frequent_save_velocities == other.frequent_save_velocities and
           Integrator::operator==(other) and temperature == other.temperature 
      and tau == other.tau;
}

/** Comparison operator */
bool VelocityVerletBerendsen::operator!=(const VelocityVerletBerendsen &other) const
{
    return not VelocityVerletBerendsen::operator==(other);
}

/** Return a string representation of this integrator */
QString VelocityVerletBerendsen::toString() const
{
    return QObject::tr("VelocityVerletBerendsen()");
}
                                                       
/** Integrate the coordinates of the atoms in the molecules in 'molgroup'
    using the forces in 'forcetable', using the optionally supplied 
    property map to find the necessary molecular properties 
    
    \throw SireMol::missing_molecule
    \throw SireBase::missing_property
    \throw SireError:invalid_cast
    \throw SireError::incompatible_error
*/
void VelocityVerletBerendsen::integrate(IntegratorWorkspace &workspace,
					const Symbol &nrg_component,
					SireUnits::Dimension::Time timestep,
					int nmoves, bool record_stats) const
{
    AtomicVelocityWorkspace &ws = workspace.asA<AtomicVelocityWorkspace>();
    
    const double dt = timestep.value();

    const int nmols = ws.nMolecules();
    
    for (int imove=0; imove<nmoves; ++imove)
    {
      	SireUnits::Dimension::MolarEnergy ekin0 = ws.kineticEnergy();
	//qDebug() << " START KINETIC ENERGY " << ekin0.toString();
	
        ws.calculateForces(nrg_component);
        
        //first integrate the coordinates - loop over all molecules
        for (int i=0; i<nmols; ++i)
        {
            const int nats = ws.nAtoms(i);
        
            Vector *x = ws.coordsArray(i);
            const Vector *f = ws.forceArray(i);
            Vector *p = ws.momentaArray(i);
            const double *m = ws.massArray(i);

            for (int j=0; j<nats; ++j)
            {
                if (m[j] != 0)
                {
                    // v(t + dt/2) = v(t) + (1/2) a(t) dt
                    p[j] += ((0.5*dt) * f[j]);

                    // r(t + dt) = r(t) + v(t + dt/2) dt
                    x[j] += (dt / m[j]) * p[j];
                }
            }
        }

        ws.commitCoordinates();
        ws.calculateForces(nrg_component);
        
        //now need to integrate the velocities
        for (int i=0; i<nmols; ++i)
        {
            const int nats = ws.nAtoms(i);
        
            const Vector *f = ws.forceArray(i);
            Vector *p = ws.momentaArray(i);
            const double *m = ws.massArray(i);
	    
            for (int j=0; j<nats; ++j)
            {
                if (m[j] != 0)
                    p[j] += ((0.5*dt) * f[j]);
            }
        }

	// Berendsen thermostat
	// now compute new kinetic energy
	//SireUnits::Dimension::MolarEnergy ekin = ws.kineticEnergy();
	//qDebug() << " KINETIC ENERGY AFTER MOMENTUM UPDATE " << ekin.toString();
	// rescale velocities to achieve mean target temperature
	//int ndofs = 3 * nmols ;  // NEED GOOD WAY OF CALCULATING NDOFS
	//SireUnits::Dimension::Temperature curr_temp = ( ( 2 * ekin.value() ) / ( ndofs * k_boltz ) ) * kelvin ;
	//qDebug() << " CURR TEMP IS " << curr_temp.toString() << " TGT TEMP IS " << temperature.toString();
	//qDebug() << "DT IS " << convertTo(dt, picosecond);

	//double tau = 0.002 * picosecond;
	//double lambda2 = 1.0 + ( dt / tau ) * ( ( temperature / curr_temp ) - 1.0 );
	//double lambda = sqrt(lambda2);
	//qDebug() << "DT / tau " << ( dt / tau );
	//qDebug() << "Scaling factor is " << lambda;
	
        //for (int i=0; i<nmols; ++i)
        //{
        //   const int nats = ws.nAtoms(i);
        //
        //    Vector *p = ws.momentaArray(i);
        //    const double *m = ws.massArray(i);
	//
	//  for (int j=0; j<nats; ++j)
	//  {
	//      if (m[j] != 0)
	//  p[j] *= lambda; 
	//  }
	//    }
	//SireUnits::Dimension::MolarEnergy ekin2 = ws.kineticEnergy();
	//qDebug() << " KINETIC ENERGY AFTER SCALING " << ekin2.toString();
	//SireUnits::Dimension::Temperature curr_temp2 = ( ( 2 * ekin2.value() ) / ( ndofs * k_boltz ) ) * kelvin ;
	//qDebug() << " NEW CURRENT TEMPERATURE " << curr_temp2.toString();
	// end of Berendsen 

        if (frequent_save_velocities)
            ws.commitVelocities();
        
        if (record_stats)
            ws.collectStatistics();
    }
    
    if (not frequent_save_velocities)
        ws.commitVelocities();
}

/** Create an empty workspace */
IntegratorWorkspacePtr VelocityVerletBerendsen::createWorkspace(
                                                const PropertyMap &map) const
{
    return IntegratorWorkspacePtr( new AtomicVelocityWorkspace(map) );
}

/** Return the ensemble of this integrator */
Ensemble VelocityVerletBerendsen::ensemble() const
{
    return Ensemble::NVT(temperature);
}

/** Return whether or not this integrator is time-reversible */
bool VelocityVerletBerendsen::isTimeReversible() const
{
    return true;
}

/** Create a workspace for this integrator for the molecule group 'molgroup' */
IntegratorWorkspacePtr VelocityVerletBerendsen::createWorkspace(
                                                const MoleculeGroup &molgroup,
                                                const PropertyMap &map) const
{
    return IntegratorWorkspacePtr( new AtomicVelocityWorkspace(molgroup,map) );
}

const char* VelocityVerletBerendsen::typeName()
{
    return QMetaType::typeName( qMetaTypeId<VelocityVerletBerendsen>() );
}

/** Set the temperature of the NVT simulation */
void VelocityVerletBerendsen::setTemperature(SireUnits::Dimension::Temperature temp)
{
  temperature = temp;
}

/** Set the lag time for coupling to the heat bath */
void VelocityVerletBerendsen::setTemperatureCoupling(SireUnits::Dimension::Time lag)
{
  tau = lag;
}

