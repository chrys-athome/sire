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
    
    static const char* typeName();
    
    QString toString() const;
    
    void integrate(System &system, IntegratorWorkspace &workspace,
                   const Symbol &nrg_component, const PropertyMap &map) const;

    void setTimeStep(const SireUnits::Dimension::Time &timestep);

    SireUnits::Dimension::Time timeStep() const;
    
    void setGenerator(const RanGenerator &generator);

    IntegratorWorkspacePtr createWorkspace() const;
    IntegratorWorkspacePtr createWorkspace(const MoleculeGroup &molgroup) const;

private:
    /** The timestep of integration */
    SireUnits::Dimension::Time timestep;

    /** The generator used to generate new velocities */
    VelGenPtr vel_generator;
};

}

Q_DECLARE_METATYPE( SireMove::VelocityVerlet )

SIRE_EXPOSE_CLASS( SireMove::VelocityVerlet )

SIRE_END_HEADER

#endif
