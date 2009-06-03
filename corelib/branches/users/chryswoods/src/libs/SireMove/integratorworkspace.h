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

#ifndef SIREMOVE_INTEGRATORWORKSPACE_H
#define SIREMOVE_INTEGRATORWORKSPACE_H

#include <QUuid>

#include "SireBase/property.h"
#include "SireBase/properties.h"
#include "SireBase/majorminorversion.h"

#include "SireCAS/symbol.h"

#include "SireMol/moleculegroup.h"
#include "SireMol/atomvelocities.h"
#include "SireMol/atomforces.h"
#include "SireMol/atommasses.h"

#include "SireFF/forcetable.h"

SIRE_BEGIN_HEADER

namespace SireMove
{
class IntegratorWorkspace;
class NullIntegratorWorkspace;
class AtomicVelocityWorkspace;
}

QDataStream& operator<<(QDataStream&, const SireMove::IntegratorWorkspace&);
QDataStream& operator>>(QDataStream&, SireMove::IntegratorWorkspace&);

QDataStream& operator<<(QDataStream&, const SireMove::NullIntegratorWorkspace&);
QDataStream& operator>>(QDataStream&, SireMove::NullIntegratorWorkspace&);

QDataStream& operator<<(QDataStream&, const SireMove::AtomicVelocityWorkspace&);
QDataStream& operator>>(QDataStream&, SireMove::AtomicVelocityWorkspace&);

namespace SireMol
{
class MoleculeView;
}

namespace SireSystem
{
class System;
}

namespace SireMove
{

class VelocityGenerator;

using SireMol::MoleculeView;
using SireMol::MoleculeGroup;
using SireMol::MolGroupPtr;
using SireMol::MGNum;
using SireMol::MolNum;
using SireMol::MolID;
using SireMol::AtomForces;
using SireMol::AtomMasses;
using SireMol::AtomVelocities;

using SireSystem::System;

using SireFF::ForceTable;

using SireMaths::Vector;

using SireCAS::Symbol;

using SireBase::PropertyMap;

/** This is the base class of the workspaces which are used to 
    hold the intermediate values used when integrating the 
    dynamics of a system
    
    @author Christopher Woods
*/
class SIREMOVE_EXPORT IntegratorWorkspace : public SireBase::Property
{

friend QDataStream& ::operator<<(QDataStream&, const IntegratorWorkspace&);
friend QDataStream& ::operator>>(QDataStream&, IntegratorWorkspace&);

public:
    IntegratorWorkspace();
    IntegratorWorkspace(const MoleculeGroup &molgroup);
    
    IntegratorWorkspace(const IntegratorWorkspace &other);
    
    virtual ~IntegratorWorkspace();
    
    static const char* typeName()
    {
        return "SireBase::IntegratorWorkspace";
    }

    virtual IntegratorWorkspace* clone() const=0;

    const MoleculeGroup& moleculeGroup() const;

    const ForceTable& forceTable() const;

    virtual SireUnits::Dimension::MolarEnergy kineticEnergy() const=0;
    virtual SireUnits::Dimension::MolarEnergy 
                            kineticEnergy(const MoleculeView &molview) const=0;
    
    static NullIntegratorWorkspace& null();

protected:
    IntegratorWorkspace& operator=(const IntegratorWorkspace &other);
    
    bool operator==(const IntegratorWorkspace &other) const;
    bool operator!=(const IntegratorWorkspace &other) const;

    void updateFrom(SireSystem::System &system, const SireCAS::Symbol &nrg_component);

private:
    /** The molecule group containing the molecules being integrated */
    MolGroupPtr molgroup;

    /** The current forces acting on the molecules */
    ForceTable molforces;
    
    /** The energy component used to get the forces */
    SireCAS::Symbol last_nrg_component;
    
    /** The ID of the system used to get the forces */
    QUuid last_system_uid;
    
    /** The version of the system when we got the forces */
    SireBase::Version last_system_version;
};

/** This is the null integrator workspace */
class SIREMOVE_EXPORT NullIntegratorWorkspace
        : public SireBase::ConcreteProperty<NullIntegratorWorkspace,IntegratorWorkspace>
{

friend QDataStream& ::operator<<(QDataStream&, const NullIntegratorWorkspace&);
friend QDataStream& ::operator>>(QDataStream&, NullIntegratorWorkspace&);

public:
    NullIntegratorWorkspace();
    NullIntegratorWorkspace(const NullIntegratorWorkspace &other);
    
    ~NullIntegratorWorkspace();
    
    NullIntegratorWorkspace& operator=(const NullIntegratorWorkspace &other);
    
    bool operator==(const NullIntegratorWorkspace &other) const;
    bool operator!=(const NullIntegratorWorkspace &other) const;

    static const char* typeName();

    SireUnits::Dimension::MolarEnergy kineticEnergy() const;
    SireUnits::Dimension::MolarEnergy kineticEnergy(const MoleculeView &molview) const;
};

/** This class provides a workspace for integrators that make use
    of atomic forces and velocities 
    
    @author Christopher Woods
*/
class SIREMOVE_EXPORT AtomicVelocityWorkspace
       : public SireBase::ConcreteProperty<AtomicVelocityWorkspace,IntegratorWorkspace>
{

friend QDataStream& ::operator<<(QDataStream&, const AtomicVelocityWorkspace&);
friend QDataStream& ::operator>>(QDataStream&, AtomicVelocityWorkspace&);

public:
    AtomicVelocityWorkspace();
    AtomicVelocityWorkspace(const MoleculeGroup &molgroup);
    
    AtomicVelocityWorkspace(const AtomicVelocityWorkspace &other);
    
    ~AtomicVelocityWorkspace();

    AtomicVelocityWorkspace& operator=(const AtomicVelocityWorkspace &other);
    
    bool operator==(const AtomicVelocityWorkspace &other) const;
    bool operator!=(const AtomicVelocityWorkspace &other) const;
    
    static const char* typeName();
    
    SireUnits::Dimension::MolarEnergy kineticEnergy() const;
    SireUnits::Dimension::MolarEnergy kineticEnergy(const MoleculeView &molview) const;
    
    QHash<MolNum,AtomForces> forces() const;
    QHash<MolNum,AtomVelocities> velocities() const;
    QHash<MolNum,AtomMasses> masses() const;

    AtomForces forces(const MolID &molid) const;
    AtomVelocities velocities(const MolID &molid) const;
    AtomMasses masses(const MolID &molid) const;

    int nMolecules() const;
    int count() const;
    int size() const;

    const QVector<Vector>& forceArray(int i) const;
    const QVector<Vector>& coordinateArray(int i) const;
    const QVector<Vector>& velocityArray(int i) const;
    const QVector<double>& reciprocalMassArray(int i) const;

    void setCoordinates(int i, const QVector<Vector> &new_coords);
    void setVelocities(int i, const QVector<Vector> &new_velocities);

    void updateFrom(System &system, const Symbol &nrg_component,
                    const VelocityGenerator &velgen,
                    const PropertyMap &map = PropertyMap());

    void updateSystem(System &system, const Symbol &nrg_component);

private:
    void assertValidIndex(int i) const;

    /** The array of atomic forces in internal units */
    QVector< QVector<Vector> > forces_array;
    
    /** The array of atomic coordinates, mapped to infinite space
        cartesian */
    QVector< QVector<Vector> > coords_array;
    
    /** The array of atomic velocities in internal units */
    QVector< QVector<Vector> > vels_array;
    
    /** The array of atomic masses (reciprocal of the mass) in internal units */
    QVector< QVector<double> > inv_masses_array;
    
    /** The property names used to get and set the properties */
    PropertyMap propnames;
};

typedef SireBase::PropPtr<IntegratorWorkspace> IntegratorWorkspacePtr;

}

Q_DECLARE_METATYPE( SireMove::NullIntegratorWorkspace )
Q_DECLARE_METATYPE( SireMove::AtomicVelocityWorkspace )

SIRE_EXPOSE_CLASS( SireMove::IntegratorWorkspace )
SIRE_EXPOSE_CLASS( SireMove::NullIntegratorWorkspace )
SIRE_EXPOSE_CLASS( SireMove::AtomicVelocityWorkspace )

SIRE_EXPOSE_PROPERTY( SireMove::IntegratorWorkspacePtr,
                      SireMove::IntegratorWorkspace )

SIRE_END_HEADER

#endif
