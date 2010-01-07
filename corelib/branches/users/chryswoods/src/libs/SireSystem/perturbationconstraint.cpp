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

#include "perturbationconstraint.h"

#include "SireCAS/symbol.h"
#include "SireCAS/values.h"

#include "SireMol/moleculegroup.h"
#include "SireMol/molecule.h"
#include "SireMol/perturbation.h"
#include "SireMol/mgname.h"
#include "SireMol/mgnum.h"

#include "SireSystem/system.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireSystem;
using namespace SireSystem::detail;
using namespace SireMol;
using namespace SireCAS;
using namespace SireBase;
using namespace SireStream;

/////////
///////// Implementation of PerturbationData
/////////

PerturbationData::PerturbationData()
{}

PerturbationData::PerturbationData(const PerturbationPtr &perturbation)
                 : pert(perturbation)
{}
    
PerturbationData::PerturbationData(const PerturbationData &other)
                 : pert(other.pert), props(other.props)
{}
    
PerturbationData::~PerturbationData()
{}
    
bool PerturbationData::wouldChange(const Molecule &molecule,
                                   const Values &values) const
{
    if (props.isEmpty())
        return true;
    else
    {
        for (QHash<QString,quint64>::const_iterator it = props.constBegin();
             it != props.constEnd();
             ++it)
        {
            if (molecule.version(it.key()) != it.value())
            {
                //one of the properties needed by this perturbation has
                //changed - see if this will change the molecule
                return pert.read().wouldChange(molecule, values);
            }
        }
        
        return false;
    }
}

Molecule PerturbationData::perturb(const Molecule &molecule, const Values &values)
{
    if (props.isEmpty())
    {
        if (not pert.read().wouldChange(molecule, values))
        {
            //now save the versions of the properties used by this perturbation
            foreach (QString property, pert.read().requiredProperties())
            {
                props.insert(property, molecule.version(property));
            }   
            
            return molecule;
        }
    }

    //apply the perturbation
    Molecule perturbed_mol = pert.read().perturb(molecule, values);
    
    //now save the versions of the properties used by this perturbation
    foreach (QString property, pert.read().requiredProperties())
    {
        props.insert(property, perturbed_mol.version(property));
    }
    
    return perturbed_mol;
}

/////////
///////// Implementation of PerturbationConstraint
/////////

static const RegisterMetaType<PerturbationConstraint> r_pertcons;

/** Serialise to a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator<<(QDataStream &ds,
                                          const PerturbationConstraint &pertcons)
{
    writeHeader(ds, r_pertcons, 1);
    
    SharedDataStream sds(ds);
    
    sds << pertcons.molgroup << pertcons.perts_property
        << static_cast<const MoleculeConstraint&>(pertcons);
        
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator>>(QDataStream &ds,
                                          PerturbationConstraint &pertcons)
{
    VersionID v = readHeader(ds, r_pertcons);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> pertcons.molgroup >> pertcons.perts_property
            >> static_cast<MoleculeConstraint&>(pertcons);
            
        pertcons.pertdata.clear();
        pertcons.perturbed_mols.clear();
        pertcons.pert_vals.clear();
    }
    else
        throw version_error(v, "1", r_pertcons, CODELOC);
        
    return ds;
}

/** Constructor */
PerturbationConstraint::PerturbationConstraint()
                       : ConcreteProperty<PerturbationConstraint,MoleculeConstraint>(),
                         perts_property("perturbations")
{}

/** Construct specifying the molecule group and perturbation property */
PerturbationConstraint::PerturbationConstraint(const MoleculeGroup &mgroup, 
                                               const PropertyMap &map)
                       : ConcreteProperty<PerturbationConstraint,MoleculeConstraint>(),
                         molgroup(mgroup), 
                         perts_property( map["perturbations"] )
{}
            
/** Copy constructor */
PerturbationConstraint::PerturbationConstraint(const PerturbationConstraint &other)
           : ConcreteProperty<PerturbationConstraint,MoleculeConstraint>(other),
             molgroup(other.molgroup), perts_property(other.perts_property),
             pertdata(other.pertdata), perturbed_mols(other.perturbed_mols),
             pert_vals(other.pert_vals)
{}

/** Destructor */
PerturbationConstraint::~PerturbationConstraint()
{}

/** Copy assignment operator */
PerturbationConstraint& 
PerturbationConstraint::operator=(const PerturbationConstraint &other)
{
    if (this != &other)
    {
        MoleculeConstraint::operator=(other);
        molgroup = other.molgroup;
        perts_property = other.perts_property;
        pertdata = other.pertdata;
        perturbed_mols = other.perturbed_mols;
        pert_vals = other.pert_vals;
    }
    
    return *this;
}

/** Comparison operator */
bool PerturbationConstraint::operator==(const PerturbationConstraint &other) const
{
    return molgroup == other.molgroup and perts_property == other.perts_property;
}

/** Comparison operator */
bool PerturbationConstraint::operator!=(const PerturbationConstraint &other) const
{
    return not PerturbationConstraint::operator==(other);
}

const char* PerturbationConstraint::typeName()
{
    return QMetaType::typeName( qMetaTypeId<PerturbationConstraint>() );
}

/** Return a string representation of this constraint */
QString PerturbationConstraint::toString() const
{
    if (molgroup.constData() == 0)
        return QObject::tr("PerturbationConstraint::null");
    else
        return QObject::tr("PerturbationConstraint( [%1:%2], %3 )")
                .arg(molgroup.read().name().value())
                .arg(molgroup.read().number().value())
                .arg(perts_property.toString());
}

/** Return the molecule group that is acted on by this constraint */
const MoleculeGroup& PerturbationConstraint::moleculeGroup() const
{
    return *molgroup;
}

/** Return the property used to find the perturbations to apply
    to the molecules in this constraint */
PropertyName PerturbationConstraint::perturbationProperty() const
{
    return perts_property;
}

/** Return whether or not this involves the molecule with number 'molnum' */
bool PerturbationConstraint::involvesMolecule(MolNum molnum) const
{
    return molgroup->contains(molnum);
}

/** Return whether or not this involves any of the molecules in 'molecules' */
bool PerturbationConstraint::involvesMoleculesFrom(const Molecules &molecules) const
{
    if (molecules.count() <= molgroup->nMolecules())
    {
        for (Molecules::const_iterator it = molecules.constBegin();
             it != molecules.constEnd();
             ++it)
        {
            if (molgroup->contains(it.key()))
                return true;
        }
    }
    else
    {
        for (Molecules::const_iterator it = molgroup->molecules().constBegin();
             it != molgroup->molecules().constEnd();
             ++it)
        {
            if (molecules.contains(it.key()))
                return true;
        }
    }
    
    return false;
}

/** This is called once for each new molecule in the group to update
    it and put the perturbed molecule (if it needs perturbing) into
    the perturbed_mols hash */
void PerturbationConstraint::pvt_update(const Molecule &molecule, 
                                        const System &system,
                                        bool is_new_system)
{
    //get the current version of the molecule in the molecule group
    const MoleculeGroup &mgroup = moleculeGroup();
    MolNum molnum = molecule.number();

    bool new_values = false;

    //see if we need to rebuild all of the perturbation data
    if (not pertdata.contains(molnum))
    {
        new_values = true;
    }
    else if (is_new_system)
    {
        //check to see if the values of any symbols have changed...
        QHash<MolNum,Values>::const_iterator it = pert_vals.constFind(molnum);
        
        if (it != pert_vals.constEnd())
        {
            const Values &values = *it;
        
            foreach (Symbol symbol, values.symbols())
            {
                if (values[symbol] != system.constant(symbol))
                {
                    //this symbol has changed - need to rebuild them all
                    new_values = true;
                    break;
                }
            }
        }
        else
        {
            //we don't have the values for this molecule?
            new_values = true;
        }
    }
    else if (mgroup.contains(molnum))
    {
        const MoleculeData &oldmol = mgroup[molnum].data();
    
        if (molecule.version(perts_property) != oldmol.version(perts_property))
            new_values = true;
    }
    else
    {
        new_values = true;
    }

    if (new_values)
    {
        //the symbols may have changed, or the available 
        //perturbations may have changed, so rebuild everything from scratch
        perturbed_mols.remove(molnum);
        pertdata.remove(molnum);
        pert_vals.remove(molnum);
        
        const Perturbation &perturbation = molecule.property(perts_property)
                                                   .asA<Perturbation>();

        Values values = system.constants(perturbation.requiredSymbols());
        pert_vals.insert(molnum, values);
        
        //perturb the molecule
        Molecule perturbed_mol = molecule;
        
        if (perturbation.wouldChange(molecule, values))
            perturbed_mol = perturbation.perturb(molecule, values);
        
        //now save information about all of the perturbations
        //(so that they can be applied individually in future)
        QList<PerturbationPtr> perts = perturbation.children();
                                               
        PertDataList &pertlist = pertdata[molnum];
        
        for (QList<PerturbationPtr>::const_iterator it = perts.constBegin();
             it != perts.constEnd();
             ++it)
        {
            QSharedDataPointer<PerturbationData> d( new PerturbationData(*it) );
            pertlist.append(d);
        }
        
        if (molecule.version() != perturbed_mol.version())
        {
            //the molecule has been changed by the perturbations
            perturbed_mols.insert(molnum, perturbed_mol);
        }
    }
    else
    {
        const MoleculeData &oldmol = mgroup[molnum].data();
    
        //if the molecule hasn't changed, then it still obeys the 
        //constraints
        if (oldmol.version() == molecule.version())
        {
            perturbed_mols.remove(molnum);
            return;
        }

        //is this the same as the perturbed molecule?
        if (perturbed_mols.contains(molnum))
        {
            if (perturbed_mols.value(molnum).version() == molecule.version())
            {
                //yes - we've just updated the system to use the perturbed molecule
                perturbed_mols.remove(molnum);
                return;
            }
        }

        const Values &values = *(pert_vals.constFind(molnum));

        //the molecule has changed, but does it still obey the constraints?
        {
            PertDataList perts = pertdata.value(molnum);
                
            bool would_change = false;
                
            for (PertDataList::const_iterator it = perts.constBegin();
                 it != perts.constEnd();
                 ++it)
            {
                if ( (*it)->wouldChange(molecule, values) )
                {
                    would_change = true;
                    break;
                }
            }
                
            if (not would_change)
            {
                perturbed_mols.remove(molnum);
                return;
            }
        }
        
        //ok - we now know that the molecule will need to be updated
        // - apply the perturbation
        {
            PertDataList &perts = pertdata[molnum];
            
            Molecule perturbed_mol = molecule;
            
            for (PertDataList::iterator it = perts.begin();
                 it != perts.end();
                 ++it)
            
            {
                if ((*it)->wouldChange(molecule, values))
                    perturbed_mol = (*it)->perturb(perturbed_mol,values);
            }
            
            if (perturbed_mol.version() != molecule.version())
                //this really changed the molecule
                perturbed_mols.insert(molnum, perturbed_mol);
            else
                perturbed_mols.remove(molnum);
        }
    }
}

/** Call to update the constraint with the new version of the passed system,
    where only the molecule with the passed number has changed */
void PerturbationConstraint::pvt_update(const System &system, MolNum molnum)
{
    //do we have a new molecule group?
    const MoleculeGroup &old_group = moleculeGroup();

    if ( system.contains(old_group.number()) )
    {
        const MoleculeGroup &new_group = system[old_group.number()];
        
        if (new_group.contains(molnum))
        {
            this->pvt_update(new_group[molnum].molecule(), system, false);
        }
        else
        {
            //make sure we are not storing any information about this molecule
            //any more
            pertdata.remove(molnum);
            perturbed_mols.remove(molnum);
            pert_vals.remove(molnum);
        }
        
        //update to the new molecule group
        molgroup = new_group;
    }
    else
    {
        this->pvt_update(system, false);
    }
}

/** Call to update the constraint with the new version of the passed system,
    where only the passed molecules have changed */
void PerturbationConstraint::pvt_update(const System &system,
                                        const Molecules &changed_mols)
{
    //do we have a new molecule group?
    const MoleculeGroup &old_group = moleculeGroup();

    if ( system.contains(old_group.number()) )
    {
        const MoleculeGroup &new_group = system[old_group.number()];
        
        for (Molecules::const_iterator it = changed_mols.constBegin();
             it != changed_mols.constEnd();
             ++it)
        {
            if (new_group.contains(it.key()))
            {
                this->pvt_update( new_group[it.key()].molecule(), system, false );
            }
            else
            {
                //make sure we are not storing any information about this molecule
                pertdata.remove(it.key());
                perturbed_mols.remove(it.key());
                pert_vals.remove(it.key());
            }
        }
        
        //update to the new molecule group
        molgroup = new_group;
    }
    else
    {
        this->pvt_update(system, false);
    }
}

/** Call to update the constraint with the new version of the passed system */
void PerturbationConstraint::pvt_update(const System &system, bool is_new_system)
{
    //do we have a new molecule group?
    const MoleculeGroup &old_group = moleculeGroup();
    
    if ( system.contains(old_group.number()) )
    {
        const MoleculeGroup &new_group = system[old_group.number()];
        
        if (new_group.version().majorVersion() != old_group.version().majorVersion())
        {
            //this is a new molecule group - check for any molecules that
            //have been removed
            foreach (MolNum molnum, pertdata.keys())
            {
                if (not new_group.contains(molnum))
                {
                    pertdata.remove(molnum);
                    pert_vals.remove(molnum);
                }
            }
            
            foreach (MolNum molnum, perturbed_mols.keys())
            {
                if (not new_group.contains(molnum))
                {
                    perturbed_mols.remove(molnum);
                }
            }
        }
        
        //now update all of the contained molecules
        for (Molecules::const_iterator it = new_group.molecules().constBegin();
             it != new_group.molecules().constEnd();
             ++it)
        {
            this->pvt_update(it->molecule(), system, is_new_system);
        }
        
        //now save the new molecule group
        molgroup = new_group;
    }
    else
    {
        //there isn't a new molecule group, but check that the molecules
        //are still up to date
        MolGroupPtr new_group = molgroup;
        QList<Molecule> new_mols = new_group.edit().update(system.molecules());

        if (is_new_system)
        {
            for (Molecules::const_iterator it = new_group.read().molecules().constBegin();
                 it != new_group.read().molecules().constEnd();
                 ++it)
            {
                this->pvt_update(it->molecule(), system, true);
            }
        }
        else
        {
            foreach (Molecule new_mol, new_mols)
            {
                this->pvt_update(new_mol, system, false);
            }
        }
        
        molgroup = new_group;
    }
}

/** This returns the molecules that must change to apply the constraint */
Molecules PerturbationConstraint::applyConstraint() const
{
    if (perturbed_mols.isEmpty())
        return Molecules();
        
    else
    {
        Molecules mols_to_change;
        
        for (QHash<MolNum,Molecule>::const_iterator it = perturbed_mols.constBegin();
             it != perturbed_mols.constEnd();
             ++it)
        {
            mols_to_change.add( it.value() );
        }
        
        return mols_to_change;
    }
}

/** Update this constraint with the passed system, returning the
    molecules that must change to maintain the constraint */
Molecules PerturbationConstraint::update(const System &system)
{
    Molecules mols_to_change;
	
    if (system.UID() != this->sysUID() or
        system.version() != this->sysVersion())
    {
        PerturbationConstraint old_state(*this);
        
        bool new_system = (system.UID() != this->sysUID() or
                           system.version().majorVersion() 
                                            != this->sysVersion().majorVersion());
        
        try
        {
            this->updatedFrom(system);
            this->pvt_update(system, new_system);
            mols_to_change = this->applyConstraint();
        }
        catch(...)
        {
            PerturbationConstraint::operator=(old_state);
            throw;
        }
    }
    else
        mols_to_change = this->applyConstraint();

    return mols_to_change;
}

/** Update this constraint so that it is applied to the system 'system'
    and return the molecules from system that need to change to 
    ensure that this constraint is maintained. This provides the
    hint that only the molecule with number 'changed_mol' has changed
    since the last update. */
Molecules PerturbationConstraint::update(const System &system, MolNum changed_mol)
{
    Molecules mols_to_change;
	
    if (system.UID() != this->sysUID() or
        system.version() != this->sysVersion())
    {
        PerturbationConstraint old_state(*this);
        
        bool new_system = (system.UID() != this->sysUID() or
                           system.version().majorVersion() 
                                            != this->sysVersion().majorVersion());
        
        try
        {
            this->updatedFrom(system);
            
            if (new_system)
                this->pvt_update(system, new_system);
            else
                this->pvt_update(system, changed_mol);

            mols_to_change = this->applyConstraint();
        }
        catch(...)
        {
            PerturbationConstraint::operator=(old_state);
            throw;
        }
    }
    else
        mols_to_change = this->applyConstraint();

    return mols_to_change;
}

/** Update this constraint so that it is applied to the system 'system'
    and return the molecules from system that need to change to 
    ensure that this constraint is maintained. This provides the
    hint that only the molecules in 'molecules' have changed
    since the last update. */
Molecules PerturbationConstraint::update(const System &system, 
                                         const Molecules &molecules)
{
    Molecules mols_to_change;
	
    if (system.UID() != this->sysUID() or
        system.version() != this->sysVersion())
    {
        PerturbationConstraint old_state(*this);
        
        bool new_system = (system.UID() != this->sysUID() or
                           system.version().majorVersion() 
                                            != this->sysVersion().majorVersion());
        
        try
        {
            this->updatedFrom(system);
            
            if (new_system)
                this->pvt_update(system, new_system);
            else
                this->pvt_update(system, molecules);

            mols_to_change = this->applyConstraint();
        }
        catch(...)
        {
            PerturbationConstraint::operator=(old_state);
            throw;
        }
    }
    else
        mols_to_change = this->applyConstraint();

    return mols_to_change;
}

/** Return whether or not this perturbation is satisfied for the passed system */
bool PerturbationConstraint::isSatisfied(const System &system) const
{
    if (system.UID() == this->sysUID() and system.version() == this->sysVersion())
    {
        return perturbed_mols.isEmpty();
    }
    else
    {
        PerturbationConstraint c(*this);
        Molecules changed_mols = c.update(system);
        
        return changed_mols.isEmpty();
    }
}
