/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2008  Christopher Woods
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

#ifndef SIRESYSTEM_CONSTRAINTS_H
#define SIRESYSTEM_CONSTRAINTS_H

#include <QVector>

#include "constraint.h"

#include "SireBase/property.h"

SIRE_BEGIN_HEADER

namespace SireSystem
{
class Constraints;
}

QDataStream& operator<<(QDataStream&, const SireSystem::Constraints&);
QDataStream& operator>>(QDataStream&, SireSystem::Constraints&);

namespace SireMol
{
class MolNum;
class Molecules;
}

namespace SireSystem
{

using SireMol::MolNum;
using SireMol::Molecules;

/** This class is used to hold and organise a collection of Constraint objects

    @author Christopher Woods
*/
class SIRESYSTEM_EXPORT Constraints 
            : public SireBase::ConcreteProperty<Constraints,SireBase::Property>
{

friend QDataStream& ::operator<<(QDataStream&, const Constraints&);
friend QDataStream& ::operator>>(QDataStream&, Constraints&);

public:
    Constraints();
    
    Constraints(const Constraint &constraint);

    Constraints(const QVector<ConstraintPtr> &constraints);
    Constraints(const QList<ConstraintPtr> &constraints);
    
    Constraints(const Constraints &other);
    
    ~Constraints();
    
    Constraints& operator=(const Constraints &other);
    
    bool operator==(const Constraints &other) const;
    bool operator!=(const Constraints &other) const;
    
    static const char* typeName();
    
    const char* what() const
    {
        return Constraints::typeName();
    }
    
    const Constraint& operator[](int i) const;
    
    Constraints& operator+=(const Constraint &constraint);
    Constraints& operator+=(const Constraints &constraints);
    
    Constraints& operator-=(const Constraint &constraint);
    Constraints& operator-=(const Constraints &constraints);
    
    int count() const;
    int size() const;
    
    int nConstraints() const;

    QVector<ConstraintPtr> constraints() const;
    
    QVector<ConstraintPtr> componentConstraints() const;
    QVector<ConstraintPtr> moleculeConstraints() const;
    
    bool hasMoleculeConstraints() const;
    
    void add(const Constraint &constraint);
    void add(const Constraints &constraints);
    
    void remove(const Constraint &constraint);
    void remove(const Constraints &constraints);
    
    void removeAt(int i);
    
    bool areSatisfied(const System &system) const;
    void assertSatisfied(const System &system) const;
    
    bool moleculeConstraintsAreSatisfied(const System &system) const;
    void assertMoleculeConstraintsAreSatisfied(const System &system) const;
    
    void apply(System &system);
    
    void applyMoleculeConstraints(System &system);
    void applyMoleculeConstraints(System &system, MolNum molnum);
    void applyMoleculeConstraints(System &system, const Molecules &molecules);

private:
    void resolveMoleculeConstraints(System &system, Molecules changed_mols);

    /** The list of all component constraints that are to be 
        applied to the system - these are constraints that change
        the values of components or properties of the system */
    QVector<ConstraintPtr> cons;
    
    /** The list of all molecule constraints that are to be 
        applied to the system - these are constraints that change
        the coordinates or properties of molecules in the system */
    QVector<ConstraintPtr> molcons;
    
    /** The indicies of component constraints that may need to be
        reapplied if molecules change */
    QSet<int> mol_dependent_cons;
};

/** Return whether or not this set has any molecule constraints */
inline bool Constraints::hasMoleculeConstraints() const
{
    return not molcons.isEmpty();
}

}

Q_DECLARE_METATYPE( SireSystem::Constraints )

SIRE_EXPOSE_CLASS( SireSystem::Constraints )

SIRE_END_HEADER

#endif
