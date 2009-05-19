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

#ifndef SIREMOVE_MOLECULARDYNAMICS_H
#define SIREMOVE_MOLECULARDYNAMICS_H

#include "dynamics.h"
#include "integrator.h"

#include "SireMol/moleculegroup.h"

#include "SireFF/forcetable.h"

SIRE_BEGIN_HEADER

namespace SireMove
{
class MolecularDynamics;
}

namespace SireVol
{
class Space;
}

namespace SireBase
{
class PropertyMap;
}

QDataStream& operator<<(QDataStream&, const SireMove::MolecularDynamics&);
QDataStream& operator>>(QDataStream&, SireMove::MolecularDynamics&);

namespace SireMove
{

using SireMol::MoleculeGroup;
using SireMol::MolGroupPtr;

using SireFF::ForceTable;

/** This class implements a molecular dynamics move. Note that
    presently only NVE (microcanonical) moves are supported.

    @author Christopher Woods
*/
class SIREMOVE_EXPORT MolecularDynamics
          : public SireBase::ConcreteProperty<MolecularDynamics,Dynamics>
{

friend QDataStream& ::operator<<(QDataStream&, const MolecularDynamics&);
friend QDataStream& ::operator>>(QDataStream&, MolecularDynamics&);

public:
    MolecularDynamics();
    
    MolecularDynamics(const MoleculeGroup &molgroup);
    
    MolecularDynamics(const Integrator &integrator);
    
    MolecularDynamics(const MoleculeGroup &molgroup, const Integrator &integrator);
    MolecularDynamics(const Integrator &integrator, const MoleculeGroup &molgroup);
    
    MolecularDynamics(const MolecularDynamics &other);
    
    ~MolecularDynamics();
    
    MolecularDynamics& operator=(const MolecularDynamics &other);
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<MolecularDynamics>() );
    }
    
    MolecularDynamics* clone() const
    {
        return new MolecularDynamics(*this);
    }
    
    bool operator==(const MolecularDynamics &other) const;
    bool operator!=(const MolecularDynamics &other) const;
    
    QString toString() const;
    
    int nMoves() const;
    
    const MoleculeGroup& moleculeGroup() const;
    const Integrator& integrator() const;
    
    void setMoleculeGroup(const MoleculeGroup &molgroup);

    void setIntegrator(const Integrator &integrator);
    
    void setTimeStep(const SireUnits::Dimension::Time &timestep);
    
    SireUnits::Dimension::Time timeStep() const;

    SireUnits::Dimension::MolarEnergy kineticEnergy() const;
    
    void move(System &system, int nmoves, bool record_stats=true);

    void clearStatistics();
    
    void setGenerator(const RanGenerator &generator);

private:
    /** The integrator used to solve Newton's laws */
    IntegratorPtr intgrator;
    
    /** The number of moves performed using this object */
    quint32 num_moves;
};

}

Q_DECLARE_METATYPE( SireMove::MolecularDynamics )

SIRE_EXPOSE_CLASS( SireMove::MolecularDynamics )

SIRE_END_HEADER

#endif
