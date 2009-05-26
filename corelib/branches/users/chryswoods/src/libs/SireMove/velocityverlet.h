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

#ifndef SIREMOVE_VELOCITYVERLET_H
#define SIREMOVE_VELOCITYVERLET_H

#include "integrator.h"
#include "velocitygenerator.h"

#include "SireMol/atomvelocities.h"
#include "SireMol/atomforces.h"

#include "SireFF/forcetable.h"

#include "SireCAS/symbol.h"

#include "SireBase/majorminorversion.h"
#include "SireBase/packedarray2d.h"

SIRE_BEGIN_HEADER

namespace SireMove
{
class VelocityVerlet;
}

QDataStream& operator<<(QDataStream&, const SireMove::VelocityVerlet&);
QDataStream& operator>>(QDataStream&, SireMove::VelocityVerlet&);

namespace SireMove
{

using SireBase::PackedArray2D;
using SireBase::Properties;

using SireMaths::Vector;

using SireMol::AtomVelocities;
using SireMol::AtomForces;
using SireMol::MolID;
using SireMol::MolNum;

/** This class implements an atomistic velocity verlet dynamics integrator
 
    @author Christopher Woods
*/
class SIREMOVE_EXPORT VelocityVerlet
          : public SireBase::ConcreteProperty<VelocityVerlet,Integrator>
{

friend QDataStream& ::operator<<(QDataStream&, const VelocityVerlet&);
friend QDataStream& ::operator>>(QDataStream&, VelocityVerlet&);

public:
    VelocityVerlet();
    VelocityVerlet(const SireUnits::Dimension::Time &timestep);
    
    VelocityVerlet(const VelocityVerlet &other);
    
    ~VelocityVerlet();
    
    VelocityVerlet& operator=(const VelocityVerlet &other);
    
    bool operator==(const VelocityVerlet &other) const;
    bool operator!=(const VelocityVerlet &other) const;
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<VelocityVerlet>() );
    }
    
    QString toString() const;
    
    void integrate(System &system, const Symbol &nrg_component, const PropertyMap &map);

    void setTimeStep(const SireUnits::Dimension::Time &timestep);

    SireUnits::Dimension::Time timeStep() const;

    SireUnits::Dimension::MolarEnergy kineticEnergy() const;
    SireUnits::Dimension::MolarEnergy kineticEnergy(const MoleculeView &molview) const;
    
    ForceTable forceTable() const;
    
    QHash<MolNum,AtomVelocities> velocities() const;
    QHash<MolNum,AtomForces> forces() const;
    
    AtomVelocities velocities(const MolID &molid) const;
    AtomForces forces(const MolID &molid) const;
    
    void clearStatistics();
    
    void clearVelocities();
    void clearForces();
    
    void setGenerator(const RanGenerator &generator);

private:
    void recalculateVelocities();
    void updateTables(const MoleculeGroup &new_molgroup);
    void updateFrom(System &system, const Symbol &nrg_component);

    /** The timestep of integration */
    SireUnits::Dimension::Time timestep;
    
    /** The forces on the atoms */
    SireFF::ForceTable f;
    
    /** All of the velocities of the atoms */
    QVector< PackedArray2D<Vector> > v;

    /** All of the coordinates of the atoms (in infinite cartesian space) */
    QVector< PackedArray2D<Vector> > r;
    
    /** All of the inverse masses of the atoms */
    QVector< PackedArray2D<double> > inv_m;
    
    /** The generator used to generate new velocities */
    VelGenPtr vel_generator;
    
    /** The unique ID number of the last system to be integrated */
    QUuid last_sys_uid;
    
    /** The version of the System the last time it 
        was integrated */
    SireBase::Version last_sys_version;
    
    /** The energy component used for the last move */
    Symbol last_nrg_component;
    
    /** The kinetic energy of the last timestep */
    SireUnits::Dimension::MolarEnergy last_kinetic_nrg;
    
    /** Whether or not the velocities need to be recalculated from scratch */
    bool recalc_velocities;
};

}

Q_DECLARE_METATYPE( SireMove::VelocityVerlet )

SIRE_EXPOSE_CLASS( SireMove::VelocityVerlet )

SIRE_END_HEADER

#endif
