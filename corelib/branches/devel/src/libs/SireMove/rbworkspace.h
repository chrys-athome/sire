/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2010  Christopher Woods
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

#ifndef SIREMOVE_RBWORKSPACE_H
#define SIREMOVE_RBWORKSPACE_H

#include "integratorworkspace.h"

#include "SireMaths/matrix.h"

SIRE_BEGIN_HEADER

namespace SireMove
{
class RBWorkspace;
}

QDataStream& operator<<(QDataStream&, const SireMove::RBWorkspace&);
QDataStream& operator>>(QDataStream&, SireMove::RBWorkspace&);

namespace SireMove
{

using SireMaths::Matrix;

/** This class provides a workspace for integrators that perform
    rigid body integration of atomic velocities and coordinates
    
    @author Christopher Woods
*/
class SIREMOVE_EXPORT RBWorkspace
       : public SireBase::ConcreteProperty<RBWorkspace,IntegratorWorkspace>
{

friend QDataStream& ::operator<<(QDataStream&, const RBWorkspace&);
friend QDataStream& ::operator>>(QDataStream&, RBWorkspace&);

public:
    RBWorkspace(const PropertyMap &map = PropertyMap());
    RBWorkspace(const MoleculeGroup &molgroup,
                const PropertyMap &map = PropertyMap());
    
    RBWorkspace(const RBWorkspace &other);
    
    ~RBWorkspace();

    RBWorkspace& operator=(const RBWorkspace &other);
    
    bool operator==(const RBWorkspace &other) const;
    bool operator!=(const RBWorkspace &other) const;
    
    static const char* typeName();
    
    SireUnits::Dimension::MolarEnergy kineticEnergy() const;
    SireUnits::Dimension::MolarEnergy kineticEnergy(MolNum molnum) const;
    SireUnits::Dimension::MolarEnergy kineticEnergy(const MoleculeView &molview) const;

    int nBeads() const;

    int nAtoms(int i) const;    
    
    bool setSystem(const System &system);

    void regenerateVelocities(const VelocityGenerator &generator);
    
    void commitCoordinates();
    void commitVelocities();
    
    void commitCoordinatesAndVelocities();

protected:
    void changedProperty(const QString &property);

private:
    void rebuildFromScratch();

    /** The center of mass coordinates of each bead */
    QVector<Vector> bead_coordinates;
    
    /** The orientation matrix for each bead */
    QVector<Matrix> bead_orientations;
    
    /** All of the beads' linear momenta */
    QVector<Velocity3D> bead_linear_momenta;
    
    /** All of the beads' angular momenta */
    QVector<Matrix> bead_angular_momenta;
    
    /** All of the forces for all of the beads */
    QVector<Vector> bead_forces;
    
    /** All of the torques acting on all of the beads */
    QVector<Matrix> bead_torques;
    
    /** The mass of each bead */
    QVector<double> bead_masses;
    
    /** The diagonals of the inertia tensor for each bead */
    QVector<Vector> bead_inertia;
    
    /** The generator used to get the initial 
        linear and angular velocities */
    VelGenPtr vel_generator;
};

}

Q_DECLARE_METATYPE( SireMove::RBWorkspace )

SIRE_END_HEADER

#endif
