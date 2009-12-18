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

#ifndef SIRESYSTEM_PERTURBATIONCONSTRAINT_H
#define SIRESYSTEM_PERTURBATIONCONSTRAINT_H

#include "moleculeconstraint.h"

SIRE_BEGIN_HEADER

namespace SireSystem
{
class PerturbationConstraint;
}

QDataStream& operator<<(QDataStream&, const SireSystem::PerturbationConstraint&);
QDataStream& operator>>(QDataStream&, SireSystem::PerturbationConstraint&);

namespace SireSystem
{

/** This constraint is used to constrain part or parts of a z-matrix
    to particular values - this is useful during single-topology
    free energy simulations
    
    @author Christopher Woods
*/
class SIRESYSTEM_EXPORT PerturbationConstraint
         : public SireBase::ConcreteProperty<PerturbationConstraint,MoleculeConstraint>
{

friend QDataStream& ::operator<<(QDataStream&, const PerturbationConstraint&);
friend QDataStream& ::operator>>(QDataStream&, PerturbationConstraint&);

public:
    PerturbationConstraint();
    PerturbationConstraint(const MoleculeGroup &molgroup, 
                   const PropertyMap &map = PropertyMap());
                   
    PerturbationConstraint(const PerturbationConstraint &other);
    
    ~PerturbationConstraint();
    
    PerturbationConstraint& operator=(const PerturbationConstraint &other);
    
    bool operator==(const PerturbationConstraint &other) const;
    bool operator!=(const PerturbationConstraint &other) const;
    
    static const char* typeName();
    
    QString toString() const;
    
    const MoleculeGroup& moleculeGroup() const;
    const PropertyMap& propertyMap() const;
    
    bool involvesMolecule(MolNum molnum) const;
    bool involvesMoleculesFrom(const Molecules &molecules) const;
    
    Molecules update(const System &system);
    Molecules update(const System &system, MolNum changed_mol);
    Molecules update(const System &system, const Molecules &molecules);

    bool isSatisfied(const System &system) const;

private:
    /** The molecule group containing the molecules that are affected
        by these perturbations */
    MolGroupPtr molgroup;
    
    /** The symbol containing the value of the lambda value 
        for this perturbation */
    Symbol lamda;
    
    /** The property map that is used to locate the perturbations
        to apply - perurbations are applied using a 
        'perturbations' property */
    PropertyMap map;
};

}


SIRE_END_HEADER

#endif
