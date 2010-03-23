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

#ifndef SIRESYSTEM_DELTA_H
#define SIRESYSTEM_DELTA_H

#include "constraint.h"

#include "SireMol/molecule.h"
#include "SireMol/molecules.h"

#include "SireCAS/values.h"

SIRE_BEGIN_HEADER

namespace SireSystem
{
class Delta;
}

QDataStream& operator<<(QDataStream&, const SireSystem::Delta&);
QDataStream& operator>>(QDataStream&, SireSystem::Delta&);

namespace SireSystem
{

using SireBase::Property;
using SireBase::Properties;

using SireCAS::Symbol;
using SireCAS::Values;

using SireMol::Molecule;
using SireMol::Molecules;
using SireMol::MolNum;
using SireMol::MoleculeView;

using SireFF::FFIdx;

/** This class records what has changed in a system between moves,
    or between applications of a constraint. This records changes
    in molecules, changes in properties and changes in 
    System components. However, it does not record changes
    in energy-dependent components (e.g. energies of components
    of forcefields, or results of expressions that involve
    energy components).
    
    @author Christopher Woods
*/
class SIRESYSTEM_EXPORT Delta
{

friend QDataStream& ::operator<<(QDataStream&, const Delta&);
friend QDataStream& ::operator>>(QDataStream&, Delta&);

public:
    Delta();
    Delta(const Molecule &old_molecule, const Molecule &new_molecule);
    Delta(const Molecules &old_molecules, const Molecules &new_molecules);
    Delta(const Symbol &component, double old_value, double new_value);
    Delta(const QString &name, 
          const Property &old_value, const Property &new_value);
    Delta(const QString &name, const FFIdx &ffidx,
          const Property &old_value, const Property &new_value);
    Delta(const QString &name, const QList<FFIdx> &ffidxs,
          const Property &old_value, const Property &new_value);

    Delta(const Delta &delta0, const Delta &delta1);
          
    Delta(const Delta &other);
    
    ~Delta();
    
    Delta& operator=(const Delta &other);
    
    bool operator==(const Delta &other) const;
    bool operator!=(const Delta &other) const;
    
    Delta operator+(const Delta &other) const;

    static const char* typeName();

    QString toString() const;

    bool isEmpty() const;
    bool isNull() const;
    
    Delta add(const Delta &other) const;
    
    static Delta combine(const Delta &delta0, const Delta &delta1);
    
    bool involves(MolNum molnum) const;
    bool involves(const MoleculeView &molview) const;
    bool involves(const Molecules &molecules) const;
    bool involves(const Symbol &component) const;
    bool involves(const QString &property) const;
    bool involves(const QString &property, const FFIdx &ffidx) const;
    bool involves(const QString &property, const QList<FFIdx> &ffidxs) const;
    
    Molecule update(const Molecule &molecule) const;
    Molecules update(const Molecules &molecules) const;
    
    Molecules oldMolecules() const;
    Molecules newMolecules() const;
    
    Molecule oldMolecule(MolNum molnum) const;
    Molecule newMolecule(MolNum molnum) const;
    
    const Values& oldValues() const;
    const Values& newValues() const;
    
    double oldValue(const Symbol &component) const;
    double newValue(const Symbol &component) const;
    
    Properties oldProperties() const;
    Properties newProperties() const;
    
    const Property& oldProperty(const QString &property) const;
    const Property& newProperty(const QString &property) const;
    
    Properties oldProperties(const FFIdx &ffidx) const;
    Properties newProperties(const FFIdx &ffidx) const;
    
    const Property& oldProperty(const QString &property, const FFIdx &ffidx) const;
    const Property& newProperty(const QString &property, const FFIdx &ffidx) const;
    
    Properties oldProperties(const QList<FFIdx> &ffidxs) const;
    Properties newProperties(const QList<FFIdx> &ffidxs) const;
                             
    const Property& oldProperty(const QString &property,
                                const QList<FFIdx> &ffidxs) const;
                                
    const Property& newProperty(const QString &property,
                                const QList<FFIdx> &ffidxs) const;
    
private:
    void mergeOld(const Delta &delta0, const Delta &delta1);
    void mergeNew(const Delta &delta0, const Delta &delta1);
    
    void mergeOldComponents(const Values &comps0, const Values &comps1);
    void mergeNewComponents(const Values &comps0, const Values &comps1);

    void mergeOldMolecules(const Molecule &mol0, const Molecules &mols0,
                           const Molecule &mol1, const Molecules &mols1);
                           
    void mergeNewMolecules(const Molecule &mol0, const Molecules &mols0,
                           const Molecule &mol1, const Molecules &mols1);

    void mergeOldProperties(Properties &props,
                            const Properties &props0, const Properties &props1) const;

    void mergeNewProperties(Properties &props,
                            const Properties &props0, const Properties &props1) const;

    /** A single changed molecule before the delta - this is used when this
        delta involves only a single molecule */
    Molecule old_mol;
    
    /** A single changed molecule after the delta - this is used when this
        delta involves only a single molecule */
    Molecule new_mol;

    /** All of the changed molecules before the delta */
    Molecules old_mols;
    
    /** All of the changed molecules after the delta */
    Molecules new_mols;
    
    /** All of the changed (non-energy-dependent) system 
        components before the delta */
    Values old_components;
    
    /** All of the changed (non-energy-dependent) system
        components after the delta */
    Values new_components;
    
    /** All of the changed system properties before the delta */
    Properties old_properties;
    
    /** All of the changed system properties after the delta */
    Properties new_properties;
    
    /** All of the changed forcefield-specific properties
        before the delta, indexed by FFIdx (in the System) */
    QHash<FFIdx,Properties> old_ff_properties;
    
    /** All of the changed forcefield-specific properties
        after the delta, indexed by FFIdx (in the System) */
    QHash<FFIdx,Properties> new_ff_properties;

    /** An integer that is incremented whenever
        deltas are combined - this can be used to quickly
        tell if two deltas are different. This is equal
        to zero for an empty delta */
    quint32 merge_count;
};

}

Q_DECLARE_METATYPE( SireSystem::Delta )

SIRE_EXPOSE_CLASS( SireSystem::Delta )

SIRE_END_HEADER

#endif
