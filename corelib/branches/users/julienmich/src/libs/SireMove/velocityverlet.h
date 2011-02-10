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
#include "SireUnits/temperature.h"

SIRE_BEGIN_HEADER

namespace SireMove
{
class VelocityVerlet;
class VelocityVerletBerendsen;
}

QDataStream& operator<<(QDataStream&, const SireMove::VelocityVerlet&);
QDataStream& operator>>(QDataStream&, SireMove::VelocityVerlet&);

QDataStream& operator<<(QDataStream&, const SireMove::VelocityVerletBerendsen&);
QDataStream& operator>>(QDataStream&, SireMove::VelocityVerletBerendsen&);

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
    VelocityVerlet(bool frequent_save_velocities = false);
    
    VelocityVerlet(const VelocityVerlet &other);
    
    ~VelocityVerlet();
    
    VelocityVerlet& operator=(const VelocityVerlet &other);
    
    bool operator==(const VelocityVerlet &other) const;
    bool operator!=(const VelocityVerlet &other) const;
    
    static const char* typeName();
    
    QString toString() const;
    
    Ensemble ensemble() const;
    
    bool isTimeReversible() const;
    
    void integrate(IntegratorWorkspace &workspace,
                   const Symbol &nrg_component, 
                   SireUnits::Dimension::Time timestep,
                   int nmoves, bool record_stats) const;

    IntegratorWorkspacePtr createWorkspace(const PropertyMap &map = PropertyMap()) const;
    IntegratorWorkspacePtr createWorkspace(const MoleculeGroup &molgroup,
                                           const PropertyMap &map = PropertyMap()) const;

private:
    /** Whether or not to save the velocities after every step, 
        or to save them at the end of all of the steps */
    bool frequent_save_velocities;
};

/** This class implements an atomistic velocity verlet dynamics integrator 
    with a Berendsen thermostat

    @author Julien Michel
*/
class SIREMOVE_EXPORT VelocityVerletBerendsen
          : public SireBase::ConcreteProperty<VelocityVerletBerendsen,Integrator>
{

friend QDataStream& ::operator<<(QDataStream&, const VelocityVerletBerendsen&);
friend QDataStream& ::operator>>(QDataStream&, VelocityVerletBerendsen&);

public:
    VelocityVerletBerendsen(bool frequent_save_velocities = false);

    VelocityVerletBerendsen(const VelocityVerletBerendsen &other);
    
    ~VelocityVerletBerendsen();
    
    VelocityVerletBerendsen& operator=(const VelocityVerletBerendsen &other);
    
    bool operator==(const VelocityVerletBerendsen &other) const;
    bool operator!=(const VelocityVerletBerendsen &other) const;
    
    static const char* typeName();
    
    QString toString() const;
    
    Ensemble ensemble() const;

    void setTemperature(SireUnits::Dimension::Temperature temp);
    
    void setTemperatureCoupling(SireUnits::Dimension::Time tau);

    bool isTimeReversible() const;
    
    void integrate(IntegratorWorkspace &workspace,
                   const Symbol &nrg_component, 
                   SireUnits::Dimension::Time timestep,
                   int nmoves, bool record_stats) const;

    IntegratorWorkspacePtr createWorkspace(const PropertyMap &map = PropertyMap()) const;
    IntegratorWorkspacePtr createWorkspace(const MoleculeGroup &molgroup,
                                           const PropertyMap &map = PropertyMap()) const;

private:
    /** Whether or not to save the velocities after every step, 
        or to save them at the end of all of the steps */
    bool frequent_save_velocities;

    /** The temperature at which the integration is performed*/
    SireUnits::Dimension::Temperature temperature;

    /** The lag time for temperature coupling */
    SireUnits::Dimension::Time tau;

};

}

Q_DECLARE_METATYPE( SireMove::VelocityVerlet )

SIRE_EXPOSE_CLASS( SireMove::VelocityVerlet )

Q_DECLARE_METATYPE( SireMove::VelocityVerletBerendsen )

SIRE_EXPOSE_CLASS( SireMove::VelocityVerletBerendsen )

SIRE_END_HEADER

#endif
