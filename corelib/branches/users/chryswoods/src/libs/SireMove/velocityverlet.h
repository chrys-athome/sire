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

SIRE_BEGIN_HEADER

namespace SireMove
{
class VelocityVerlet;
}

QDataStream& operator<<(QDataStream&, const SireMove::VelocityVerlet&);
QDataStream& operator>>(QDataStream&, SireMove::VelocityVerlet&);

namespace SireMove
{

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
    
    void integrate(MoleculeGroup &molgroup, const ForceTable &forcetable,
                   const PropertyMap &map = PropertyMap());

    void setTimeStep(const SireUnits::Dimension::Time &timestep);

    SireUnits::Dimension::Time timeStep() const;

    SireUnits::Dimension::MolarEnergy kineticEnergy() const;
    
    void clearStatistics();
    
    void setGenerator(const RanGenerator &generator);

private:
    /** The timestep of integration */
    SireUnits::Dimension::Time timestep;
    
    /** The velocities... */
    ForceTable v;
};

}

Q_DECLARE_METATYPE( SireMove::VelocityVerlet )

SIRE_EXPOSE_CLASS( SireMove::VelocityVerlet )

SIRE_END_HEADER

#endif
