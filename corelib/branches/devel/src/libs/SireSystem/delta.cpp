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

#include "delta.h"

#include "SireError/errors.h"
#include "SireBase/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireSystem;
using namespace SireMol;
using namespace SireFF;
using namespace SireBase;
using namespace SireCAS;
using namespace SireStream;

static const RegisterMetaType<Delta> r_delta;

QDataStream SIRESYSTEM_EXPORT &operator<<(QDataStream &ds, const Delta &delta)
{
    writeHeader(ds, r_delta, 1);
    
    SharedDataStream sds(ds);
    
    sds << delta.merge_count;
    
    if (delta.merge_count != 0)
    {
        sds << delta.old_mol << delta.new_mol
            << delta.old_mols << delta.new_mols
            << delta.old_components << delta.new_components
            << delta.old_properties << delta.new_properties
            << delta.old_ff_properties << delta.new_ff_properties;
    }
        
    return ds;
}

QDataStream SIRESYSTEM_EXPORT &operator>>(QDataStream &ds, Delta &delta)
{
    VersionID v = readHeader(ds, r_delta);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> delta.merge_count;
        
        if (delta.merge_count == 0)
        {
            delta = Delta();
        }
        else
        {
            sds >> delta.old_mol >> delta.new_mol
                >> delta.old_mols >> delta.new_mols
                >> delta.old_components >> delta.new_components
                >> delta.old_properties >> delta.new_properties
                >> delta.old_ff_properties >> delta.new_ff_properties;
        }
    }
    else
        throw version_error(v, "1", r_delta, CODELOC);
        
    return ds;
}

const char* Delta::typeName()
{
    return QMetaType::typeName( qMetaTypeId<Delta>() );
}

/** Null constructor */
Delta::Delta() : merge_count(0)
{}

/** Construct to record the change from 'old_molecule' to 'new_molecule' */
Delta::Delta(const Molecule &old_molecule, const Molecule &new_molecule)
      : merge_count(0)
{
    if (old_molecule.isEmpty())
    {
        if (new_molecule.isEmpty())
            return;
            
        new_mol = new_molecule;
        merge_count = 1;
    }
    else if (new_molecule.isEmpty())
    {
        old_mol = old_molecule;
        merge_count = 1;
    }
    else if (old_molecule.number() != new_molecule.number())
    {
        throw SireError::incompatible_error( QObject::tr(
                "You cannot create a delta between two different numbered molecules! "
                "(%1 and %2)").arg(old_molecule.toString(), new_molecule.toString()),
                    CODELOC );
    }
    else if (old_molecule.version() != new_molecule.version())
    {
        old_mol = old_molecule;
        new_mol = new_molecule;
        merge_count = 1;
    }
}

/** Construct to record the change from the molecules in 'old_molecules'
    to those in 'new_molecules' */
Delta::Delta(const Molecules &old_molecules, const Molecules &new_molecules)
      : old_mols(old_molecules), new_mols(new_molecules), merge_count(0)
{
    if (old_molecules.nMolecules() <= new_molecules.nMolecules())
    {
        for (Molecules::const_iterator it = old_molecules.constBegin();
             it != old_molecules.constEnd();
             ++it)
        {
            Molecules::const_iterator it2 = new_molecules.constFind(it.key());
            
            if (it2 != new_molecules.constEnd())
            {
                if (it.value().version() == it2.value().version())
                {
                    //the molecules are the same - no delta!
                    old_mols.remove(it.key());
                    new_mols.remove(it.key());
                }
            }
        }
    }
    else
    {
        for (Molecules::const_iterator it = new_molecules.constBegin();
             it != new_molecules.constEnd();
             ++it)
        {
            Molecules::const_iterator it2 = old_molecules.constFind(it.key());
            
            if (it2 != old_molecules.constEnd())
            {
                if (it.value().version() == it2.value().version())
                {
                    //the molecules are the same - no delta!
                    old_mols.remove(it.key());
                    new_mols.remove(it.key());
                }
            }
        }
    }
    
    if (old_mols.isEmpty() and new_mols.isEmpty())
        merge_count = 1;
}

/** Construct to record the change in the system non-energy-dependent
    component from 'old_value' to 'new_value' */
Delta::Delta(const Symbol &component, double old_value, double new_value)
      : merge_count(0)
{
    if (old_value != new_value)
    {
        old_components.set(component, old_value);
        new_components.set(component, new_value);
        merge_count = 1;
    }
}

/** Construct to record the change in the system from the global
    property called 'name' from 'old_value' to 'new_value' */
Delta::Delta(const QString &name, const Property &old_value, const Property &new_value)
      : merge_count(0)
{
    if (not old_value.equals(new_value))
    {
        old_properties.setProperty(name, old_value);
        new_properties.setProperty(name, new_value);
        merge_count = 1;
    }
}

/** Construct to record the change in the forcefield property for
    the forcefield with index 'ffidx' in the system from 
    'old_value' to 'new_value' */
Delta::Delta(const QString &name, const FFIdx &ffidx,
             const Property &old_value, const Property &new_value)
      : merge_count(0)
{
    if (not old_value.equals(new_value))
    {
        if (ffidx.isNull())
        {
            old_properties.setProperty(name, old_value);
            new_properties.setProperty(name, new_value);
        }
        else
        {
            old_ff_properties[ffidx].setProperty(name, old_value);
            new_ff_properties[ffidx].setProperty(name, new_value);
        }
        
        merge_count = 1;
    }
}

/** Construct to record the change in the forcefield property for
    the forcefields with indicies in 'ffidxs' in the system from 
    'old_value' to 'new_value' */
Delta::Delta(const QString &name, const QList<FFIdx> &ffidxs,
             const Property &old_value, const Property &new_value)
      : merge_count(0)
{
    if (not old_value.equals(new_value))
    {
        foreach (FFIdx ffidx, ffidxs)
        {
            if (ffidx.isNull())
            {
                old_properties.setProperty(name, old_value);
                new_properties.setProperty(name, new_value);
            }
            else
            {
                old_ff_properties[ffidx].setProperty(name, old_value);
                new_ff_properties[ffidx].setProperty(name, new_value);
            }
        }
        
        merge_count = 1;
    }
}

/** Merge the components of comps1 into comps0 - note that any
    components that appear in both sets must be equal (as the change
    must start from the same point)
    
    \throw SireError::incompatible_error
*/
void Delta::mergeOldComponents(const Values &comps0, const Values &comps1)
{
    if (comps0.isEmpty())
    {
        old_components = comps1;
    }
    else if (comps1.isEmpty())
    {
        old_components = comps0;
    }
    else if (comps0 == comps1)
    {
        old_components = comps0;
    }
    else if (comps0.count() <= comps1.count())
    {
        old_components = comps1;
        
        for (Values::const_iterator it = comps0.constBegin();
             it != comps0.constEnd();
             ++it)
        {
            Values::const_iterator it2 = comps1.constFind(it.key());
            
            if (it2 != comps1.constEnd())
            {
                if (it.value() != it2.value())
                    throw SireError::incompatible_error( QObject::tr(
                            "Cannot combine the deltas as the values of "
                            "the pre-delta component %1 are different (%2 vs. %3)")
                                .arg( Symbol(it.key()).toString() )
                                .arg(it.value()).arg(it2.value()), CODELOC );
            }
            else
            {
                old_components.set( it );
            }
        }
    }
    else
    {
        old_components = comps0;
        
        for (Values::const_iterator it = comps1.constBegin();
             it != comps1.constEnd();
             ++it)
        {
            Values::const_iterator it2 = comps0.constFind(it.key());
            
            if (it2 != comps0.constEnd())
            {
                if (it.value() != it2.value())
                    throw SireError::incompatible_error( QObject::tr(
                            "Cannot combine the deltas as the values of "
                            "the pre-delta component %1 are different (%2 vs. %3)")
                                .arg( Symbol(it.key()).toString() )
                                .arg(it.value()).arg(it2.value()), CODELOC );
            }
            else
            {
                old_components.set( it );
            }
        }
    }
}

/** Merge the new components of comps1 into comps0 - note that the changes
    in comps1 overwrite the changes in comps0 */
void Delta::mergeNewComponents(const Values &comps0, const Values &comps1)
{
    if (comps0.isEmpty())
    {
        new_components = comps1;
    }
    else if (comps1.isEmpty())
    {
        new_components = comps0;
    }
    else if (comps0 == comps1)
    {
        new_components = comps0;
    }
    else if (comps0.count() <= comps1.count())
    {
        new_components = comps1;
        
        for (Values::const_iterator it = comps0.constBegin();
             it != comps0.constEnd();
             ++it)
        {
            if (not comps1.contains(it.key()))
                new_components.set(it);
        }
    }
    else
    {
        new_components = comps0;
        
        for (Values::const_iterator it = comps1.constBegin();
             it != comps1.constEnd();
             ++it)
        {
            new_components.set(it);
        }
    }
}

/** Merge the pre-delta molecules - any molecules in both deltas must
    be the same, as the deltas must have been calculated from the 
    same point
    
    \throw SireError::incompatible_error
*/
void Delta::mergeOldMolecules(const Molecule &mol0, const Molecules &mols0,
                              const Molecule &mol1, const Molecules &mols1)
{
    if (mols0.isEmpty() and mols1.isEmpty())
    {
        if (mol0.isEmpty())
        {
            old_mol = mol1;
        }
        else if (mol1.isEmpty())
        {
            old_mol = mol0;
        }
        else if (mol0.number() == mol1.number() and mol0.version() == mol1.version())
        {
            old_mol = mol0;
        }
        else if (mol0.number() != mol1.number())
        {
            old_mols.add(mol0);
            old_mols.add(mol1);
        }
        else
        {
            throw SireError::incompatible_error( QObject::tr(
                        "Cannot merge the delta because the pre-delta molecule "
                        "is different - %1 vs. %2.")
                            .arg(mol0.toString(), mol1.toString()), CODELOC );
        }
    }
    else if (mol0.isEmpty() and mol1.isEmpty())
    {
        if (mols0.nMolecules() <= mols1.nMolecules())
        {
            old_mols = mols1;
            
            for (Molecules::const_iterator it = mols0.constBegin();
                 it != mols1.constBegin();
                 ++it)
            {
                Molecules::const_iterator it2 = mols1.constFind(it.key());
                
                if (it2 != mols1.constEnd())
                {
                    if (it.value().version() != it2.value().version())
                        throw SireError::incompatible_error( QObject::tr(
                                "Cannot merge the delta as the molecule with number "
                                "%1 is different (%2 vs. %3)")
                                    .arg(it.key().toString())
                                    .arg(it.value().toString(), it2.value().toString()),
                                        CODELOC );
                }
                else 
                    old_mols.add(it.value().molecule());
            }
        }
        else
        {
            old_mols = mols0;
            
            for (Molecules::const_iterator it = mols1.constBegin();
                 it != mols1.constEnd();
                 ++it)
            {
                Molecules::const_iterator it2 = mols0.constFind(it.key());
                
                if (it2 != mols0.constEnd())
                {
                    if (it.value().version() != it2.value().version())
                        throw SireError::incompatible_error( QObject::tr(
                                "Cannot merge the delta as the molecule with number "
                                "%1 is different (%2 vs. %3)")
                                    .arg(it.key().toString())
                                    .arg(it.value().toString(), it2.value().toString()),
                                        CODELOC );
                }
                else 
                    old_mols.add(it.value().molecule());
            }
        }
    }
    else if (mol0.isEmpty() and mols0.isEmpty())
    {
        old_mol = mol1;
        old_mols = mols1;
    }
    else if (mol1.isEmpty() and mols1.isEmpty())
    {
        old_mol = mol0;
        old_mols = mols0;
    }
    else if (mol0.isEmpty())
    {
        //implies mols0 is not empty and mol1 is not empty
        Molecules::const_iterator it = mols0.constFind(mol1.number());
        
        if (it != mols0.constEnd())
        {
            if (it.value().version() != mol1.version())
                throw SireError::incompatible_error( QObject::tr(
                        "Cannot merge the delta as the molecule with number "
                        "%1 is different (%2 vs. %3)")
                            .arg(it.key().toString())
                            .arg(it.value().toString(), mol1.toString()),
                                CODELOC );
        }
        
        old_mols = mols0;
        old_mols.add(mol1);
    }
    else if (mol1.isEmpty())
    {
        //implies mols1 is not empty and mol0 is not empty
        Molecules::const_iterator it = mols1.constFind(mol0.number());
        
        if (it != mols1.constEnd())
        {
            if (it.value().version() != mol1.version())
                throw SireError::incompatible_error( QObject::tr(
                        "Cannot merge the delta as the molecule with number "
                        "%1 is different (%2 vs. %3)")
                            .arg(it.key().toString())
                            .arg(it.value().toString(), mol0.toString()),
                                CODELOC );
        }
        
        old_mols = mols1;
        old_mols.add(mol0);
    }
}

/** Merge the post-delta molecules - the contents of delta1 will overwrite
    the contents of delta0 (as delta1 is assumed to come after delta0) */
void Delta::mergeNewMolecules(const Molecule &mol0, const Molecules &mols0,
                              const Molecule &mol1, const Molecules &mols1)
{
    if (mols0.isEmpty() and mols1.isEmpty())
    {
        if (mol0.isEmpty())
        {
            new_mol = mol1;
        }
        else if (mol1.isEmpty())
        {
            new_mol = mol0;
        }
        else if (mol0.number() == mol1.number() and mol0.version() == mol1.version())
        {
            new_mol = mol0;
        }
        else if (mol0.number() != mol1.number())
        { 
#warning HERE
            old_mols.add(mol0);
            old_mols.add(mol1);
        }
        else
        {
            throw SireError::incompatible_error( QObject::tr(
                        "Cannot merge the delta because the pre-delta molecule "
                        "is different - %1 vs. %2.")
                            .arg(mol0.toString(), mol1.toString()), CODELOC );
        }
    }
    else if (mol0.isEmpty() and mol1.isEmpty())
    {
        if (mols0.nMolecules() <= mols1.nMolecules())
        {
            old_mols = mols1;
            
            for (Molecules::const_iterator it = mols0.constBegin();
                 it != mols1.constBegin();
                 ++it)
            {
                Molecules::const_iterator it2 = mols1.constFind(it.key());
                
                if (it2 != mols1.constEnd())
                {
                    if (it.value().version() != it2.value().version())
                        throw SireError::incompatible_error( QObject::tr(
                                "Cannot merge the delta as the molecule with number "
                                "%1 is different (%2 vs. %3)")
                                    .arg(it.key().toString())
                                    .arg(it.value().toString(), it2.value().toString()),
                                        CODELOC );
                }
                else 
                    old_mols.add(it.value().molecule());
            }
        }
        else
        {
            old_mols = mols0;
            
            for (Molecules::const_iterator it = mols1.constBegin();
                 it != mols1.constEnd();
                 ++it)
            {
                Molecules::const_iterator it2 = mols0.constFind(it.key());
                
                if (it2 != mols0.constEnd())
                {
                    if (it.value().version() != it2.value().version())
                        throw SireError::incompatible_error( QObject::tr(
                                "Cannot merge the delta as the molecule with number "
                                "%1 is different (%2 vs. %3)")
                                    .arg(it.key().toString())
                                    .arg(it.value().toString(), it2.value().toString()),
                                        CODELOC );
                }
                else 
                    old_mols.add(it.value().molecule());
            }
        }
    }
    else if (mol0.isEmpty() and mols0.isEmpty())
    {
        old_mol = mol1;
        old_mols = mols1;
    }
    else if (mol1.isEmpty() and mols1.isEmpty())
    {
        old_mol = mol0;
        old_mols = mols0;
    }
    else if (mol0.isEmpty())
    {
        //implies mols0 is not empty and mol1 is not empty
        Molecules::const_iterator it = mols0.constFind(mol1.number());
        
        if (it != mols0.constEnd())
        {
            if (it.value().version() != mol1.version())
                throw SireError::incompatible_error( QObject::tr(
                        "Cannot merge the delta as the molecule with number "
                        "%1 is different (%2 vs. %3)")
                            .arg(it.key().toString())
                            .arg(it.value().toString(), mol1.toString()),
                                CODELOC );
        }
        
        old_mols = mols0;
        old_mols.add(mol1);
    }
    else if (mol1.isEmpty())
    {
        //implies mols1 is not empty and mol0 is not empty
        Molecules::const_iterator it = mols1.constFind(mol0.number());
        
        if (it != mols1.constEnd())
        {
            if (it.value().version() != mol1.version())
                throw SireError::incompatible_error( QObject::tr(
                        "Cannot merge the delta as the molecule with number "
                        "%1 is different (%2 vs. %3)")
                            .arg(it.key().toString())
                            .arg(it.value().toString(), mol0.toString()),
                                CODELOC );
        }
        
        old_mols = mols1;
        old_mols.add(mol0);
    }
}

void Delta::mergeOld(const Delta &delta0, const Delta &delta1)
{
    mergeOldComponents(delta0.old_components, delta1.old_components);
    mergeOldMolecules(delta0.old_mol, delta0.old_mols,
                      delta1.old_mol, delta1.old_mols);
}

void Delta::mergeNew(const Delta &delta0, const Delta &delta1)
{
    mergeNewComponents(delta0.new_components, delta1.new_components);
    mergeNewMolecules(delta0.new_mol, delta0.new_mols,
                      delta1.new_mol, delta1.new_mols);
}

void Delta::removeNullChange()
{
    if (not old_mol.isEmpty())
    {
        if (old_mol.number() == new_mol.number() and 
            old_mol.version() == new_mol.version())
        {
            old_mol = Molecule();
            new_mol = Molecule();
        }
    }
    
    if (not old_mols.isEmpty())
    {
        QSet<MolNum> same_mols;
            
        if (old_mols.count() <= new_mols.count())
        {
            for (Molecules::const_iterator it = old_mols.constBegin();
                 it != old_mols.constEnd();
                 ++it)
            {
                Molecules::const_iterator it2 = new_mols.constFind(it.key());
                
                if (it2 != new_mols.constEnd())
                {
                    if (it.value().version() == it2.value().version())
                        same_mols.insert(it.key());
                }
            }
        }
        else
        {
            for (Molecules::const_iterator it = new_mols.constBegin();
                 it != new_mols.constEnd();
                 ++it)
            {
                Molecules::const_iterator it2 = old_mols.constFind(it.key());
                
                if (it2 != old_mols.constEnd())
                {
                    if (it.value().version() == it2.value().version())
                        same_mols.insert(it.key());
                }
            }
        }
        
        if (not same_mols.isEmpty())
        {
            foreach (MolNum same_mol, same_mols)
            {
                old_mols.remove(same_mol);
                new_mols.remove(same_mol);
            }
        
            if (old_mols.count() == 1 and new_mols.count() == 1)
            {
                if (old_mols.constBegin().key() == new_mols.constBegin().key())
                {
                    old_mol = old_mols.constBegin().value().molecule();
                    new_mol = new_mols.constBegin().value().molecule();
                    
                    old_mols = Molecules();
                    new_mols = Molecules();
                }
            }
        }
    }
    
    if (not old_components.isEmpty())
    {
        QSet<SymbolID> same_comps;
        
        if (old_components.count() <= new_components.count())
        {
            for (Values::const_iterator it = old_components.constBegin();
                 it != old_components.constEnd();
                 ++it)
            {
                Values::const_iterator it2 = new_components.constFind(it.key());
                
                if (it2 != new_components.constEnd())
                {
                    if (it.value() == it2.value())
                        same_comps.insert(it.key());
                }
            }
        }
        else
        {
            for (Values::const_iterator it = new_components.constBegin();
                 it != new_components.constEnd();
                 ++it)
            {
                Values::const_iterator it2 = old_components.constFind(it.key());
                
                if (it2 != old_components.constEnd())
                {
                    if (it.value() == it2.value())
                        same_comps.insert(it.key());
                }
            }
        }
        
        foreach (SymbolID same_comp, same_comps)
        {
            old_components.remove(same_comp);
            new_components.remove(same_comp);
        }
    }
    
    if (not old_properties.isEmpty())
    {
        QSet<QString> same_props;
        
        if (old_properties.count() <= new_properties.count())
        {
            for (Properties::const_iterator it = old_properties.constBegin();
                 it != old_properties.constEnd();
                 ++it)
            {
                Properties::const_iterator it2 = new_properties.constFind(it.key());
                
                if (it2 != new_properties.constEnd())
                {
                    if (it.value().read().equals(it2.value().read()))
                        same_props.insert(it.key());
                }
            }
        }
        else
        {
            for (Properties::const_iterator it = new_properties.constBegin();
                 it != new_properties.constEnd();
                 ++it)
            {
                Properties::const_iterator it2 = old_properties.constFind(it.key());
                
                if (it2 != old_properties.constEnd())
                {
                    if (it.value().read().equals(it2.value().read()))
                        same_props.insert(it.key());
                }
            }
        }
        
        foreach (QString same_prop, same_props)
        {
            old_properties.removeProperty(same_prop);
            new_properties.removeProperty(same_prop);
        }
    }
    
    if (not old_ff_properties.isEmpty())
    {
        for (QHash<FFIdx,Properties>::const_iterator idx = old_ff_properties.constBegin();
             idx != old_ff_properties.constEnd();
             ++idx)
        {
            const Properties &old_props = idx.value();
            
            QHash<FFIdx,Properties>::const_iterator 
                        idx2 = new_ff_properties.constFind(idx.key());
                        
            if (idx2 != new_ff_properties.constEnd())
            {
                const Properties &new_props = idx2.value();
                
                QSet<QString> same_props;
                
                if (old_props.count() <= new_props.count())
                {
                    for (Properties::const_iterator it = old_props.constBegin();
                         it != old_props.constEnd();
                         ++it)
                    {
                        Properties::const_iterator it2 = new_props.constFind(it.key());
                        
                        if (it2 != new_props.constEnd())
                        {
                            if (it.value().read().equals(it2.value().read()))
                                same_props.insert(it.key());
                        }
                    }
                }
                else
                {
                    for (Properties::const_iterator it = new_props.constBegin();
                         it != new_props.constEnd();
                         ++it)
                    {
                        Properties::const_iterator it2 = old_props.constFind(it.key());
                        
                        if (it2 != old_props.constEnd())
                        {
                            if (it.value().read().equals(it2.value().read()))
                                same_props.insert(it.key());
                        }
                    }
                }
                
                if (not same_props.isEmpty())
                {
                    foreach (QString same_prop, same_props)
                    {
                        old_ff_properties[idx.key()].removeProperty(same_prop);
                        new_ff_properties[idx.key()].removeProperty(same_prop);
                    }
                    
                    if (old_ff_properties[idx.key()].isEmpty())
                        old_ff_properties.remove(idx.key());
                        
                    if (new_ff_properties[idx.key()].isEmpty())
                        new_ff_properties.remove(idx.key());
                }
            }
        }
    }
}

/** Construct to merge two deltas into one. This copies in
    all of the pre-delta and post-delta values from delta0
    and delta1. If there are any clashes then an exception
    is raised (as obviously the two deltas are resulting
    from two changes that are attempting to change the
    same thing in different ways)

    \throw SireError::incompatible_error
*/
Delta::Delta(const Delta &delta0, const Delta &delta1) : merge_count(0)
{
    if (delta0.isEmpty())
    {
        if (not delta1.isEmpty())
            this->operator=(delta1);
    }
    else if (delta1.isEmpty())
    {
        this->operator=(delta0);
    }
    else
    {
        this->mergeOld(delta0, delta1);
        this->mergeNew(delta0, delta1);
    
        //remove any deltas where we have changed, then changed back
        this->removeNullChange();
    
        merge_count = qMax(delta0.merge_count, delta1.merge_count) + 1;
    }
}
       
/** Copy constructor */
Delta::Delta(const Delta &other)
      : old_mol(other.old_mol), new_mol(other.new_mol),
        old_mols(other.old_mols), new_mols(other.new_mols),
        old_components(other.old_components), new_components(other.new_components),
        old_properties(other.old_properties), new_properties(other.new_properties),
        old_ff_properties(other.old_ff_properties),
        new_ff_properties(other.new_ff_properties),
        merge_count(other.merge_count)
{}

/** Destructor */
Delta::~Delta()
{}

/** Copy assignment operator */
Delta& Delta::operator=(const Delta &other)
{
    if (this != &other)
    {
        merge_count = other.merge_count;
        old_mol = other.old_mol;
        new_mol = other.new_mol;
        old_mols = other.old_mols;
        new_mols = other.new_mols;
        old_components = other.old_components;
        new_components = other.new_components;
        old_properties = other.old_properties;
        new_properties = other.new_properties;
        old_ff_properties = other.old_ff_properties;
        new_ff_properties = other.new_ff_properties;
    }
    
    return *this;
}

/** Comparison operator */
bool Delta::operator==(const Delta &other) const
{
    if (this == &other)
        return true;

    else if (merge_count != other.merge_count)
        return false;

    else if (merge_count == 0)
        return true;
    
    else
    {
        return old_mol.equals(other.old_mol) and new_mol.equals(other.new_mol) and
               old_mols == other.old_mols and new_mols == other.new_mols and
               old_components == other.old_components and
               new_components == other.new_components and
               old_properties == other.old_properties and
               new_properties == other.new_properties and
               old_ff_properties == other.old_ff_properties and
               new_ff_properties == other.new_ff_properties;
    }
}

/** Comparison operator */
bool Delta::operator!=(const Delta &other) const
{
    return not Delta::operator==(other);
}

/** Return the combination of this delta with 'other'

    \throw SireError::incompatible_error
*/
Delta Delta::operator+(const Delta &other) const
{
    return Delta(*this, other);
}

/** Return a string representation of this delta */
QString Delta::toString() const
{
    if (isEmpty())
        return QObject::tr("Delta::null");
        
    QStringList parts;
    
    if ( not (old_mol.isEmpty() and new_mol.isEmpty()) )
    {
        parts.append( QObject::tr("    %1 => %2")
                            .arg(old_mol.toString(), new_mol.toString()) );
    }
    else if ( not (old_mols.isEmpty() and new_mols.isEmpty()) )
    {
        parts.append( QObject::tr("    %1 => %2")
                        .arg( Sire::toString(old_mols.molNums()),
                              Sire::toString(new_mols.molNums()) ) );
    }
    
    if ( not (old_components.isEmpty() and new_components.isEmpty()) )
    {
        parts.append( QObject::tr("    %1 => %2")
                        .arg( old_components.toString(), new_components.toString() ) );
    }
    
    if ( not (old_properties.isEmpty() and new_properties.isEmpty()) )
    {
        parts.append( QObject::tr("    %1 => %2")
                        .arg( old_properties.toString(), new_properties.toString()) );
    }
    
    if ( not (old_ff_properties.isEmpty() and new_ff_properties.isEmpty()) )
    {
        parts.append( QObject::tr("    %1 => %2")
                        .arg( Sire::toString( old_ff_properties ),
                              Sire::toString( new_ff_properties ) ) );
    }
    
    if (parts.count() == 1)
    {
        return QObject::tr("Delta( %1 )").arg(parts.at(0).simplified());
    }
    else
    {
        return QObject::tr("Delta(\n%1\n     )").arg(parts.join(",\n"));
    }
}

/** Return whether or not this delta is empty (represents no change) */
bool Delta::isEmpty() const
{
    return merge_count == 0;
}

/** Return whether or not this delta is null (represents no change) */
bool Delta::isNull() const
{
    return isEmpty();
}

/** Return the combination of this delta with 'other'

    \throw SireError::incompatible_error
*/
Delta Delta::add(const Delta &other) const
{
    return Delta(*this, other);
}

/** Return the combination of this delta with 'other'

    \throw SireError::incompatible_error
*/
Delta Delta::combine(const Delta &delta0, const Delta &delta1)
{
    return Delta(delta0, delta1);
}

/** Return whether or not this delta involves the molecule with 
    number 'molnum' */
bool Delta::involves(MolNum molnum) const
{
    if (isEmpty())
        return false;
    else
        return old_mol.number() == molnum or new_mol.number() == molnum or
               old_mols.contains(molnum) or new_mols.contains(molnum);
}

/** Return whether or not this delta involves any of the molecules
    that are in 'molecules' */
bool Delta::involves(const Molecules &molecules) const
{
    if (isEmpty())
        return false;

    else if (old_mol.isEmpty() and new_mol.isEmpty())
    {
        if (old_mols.nMolecules() < molecules.nMolecules() and
            new_mols.nMolecules() < molecules.nMolecules())
        {
            for (Molecules::const_iterator it = old_mols.constBegin();
                 it != old_mols.constEnd();
                 ++it)
            {
                if (molecules.contains(it.key()))
                    return true;
            }
            
            for (Molecules::const_iterator it = new_mols.constBegin();
                 it != new_mols.constEnd();
                 ++it)
            {
                if (molecules.contains(it.key()))
                    return true;
            }
            
            return false;
        }
        else
        {
            for (Molecules::const_iterator it = molecules.constBegin();
                 it != molecules.constEnd();
                 ++it)
            {
                if (old_mols.contains(it.key()) or new_mols.contains(it.key()))
                    return true;
            }

            return false;
        }
    }
    else if (old_mol.isEmpty())
    {
        return molecules.contains(new_mol.number());
    }
    else if (new_mol.isEmpty())
    {
        return molecules.contains(old_mol.number());
    }
    else 
    {
        return molecules.contains(old_mol.number()) or 
               molecules.contains(new_mol.number());
    }
}

/** Return whether or not this delta involves a change in the
    non-energy-dependent system component 'component' */
bool Delta::involves(const Symbol &component) const
{
    if (isEmpty())
        return false;
    else
        return old_components.contains(component) or new_components.contains(component);
}

/** Return whether or not this delta involves a change in the 
    global system property 'property' */
bool Delta::involves(const QString &property) const
{
    if (isEmpty())
        return false;
    else
        return old_properties.hasProperty(property) or 
               new_properties.hasProperty(property);
}

/** Return whether or not this delta involves a change in the forcefield
    specific property 'property' for the forcefield with index 'ffidx'
    in the system */
bool Delta::involves(const QString &property, const FFIdx &ffidx) const
{
    if (isEmpty())
        return false;
    else
        return old_ff_properties.value(ffidx).hasProperty(property) or
               new_ff_properties.value(ffidx).hasProperty(property);
}

/** Return whether or not this delta involves a change in the forcefield
    specific property 'property' for any of the forcefields with index 
    in 'ffidxs' in the system */
bool Delta::involves(const QString &property, const QList<FFIdx> &ffidxs) const
{
    if (isEmpty())
        return false;
    else
    {
        foreach (FFIdx ffidx, ffidxs)
        {
            if (this->involves(property, ffidx))
                return true;
        }
    
        return false;
    }
}

/** Return the state of the changed molecules before the delta */
Molecules Delta::oldMolecules() const
{
    if (isEmpty())
        return Molecules();

    else if (old_mol.isEmpty())
        return old_mols;

    else
        return old_mol;
}

/** Return the state of the changed molecules after the delta */
Molecules Delta::newMolecules() const
{
    if (isEmpty())
        return Molecules();
        
    else if (new_mol.isEmpty())
        return new_mols;

    else
        return new_mol;
}

/** Return the state of the molecule with number 'molnum' 
    before the delta - this returns an empty molecule if
    this molecule is not affected by the delta */
Molecule Delta::oldMolecule(MolNum molnum) const
{
    if (isEmpty())
        return Molecule();

    else if (old_mol.number() == molnum)
        return old_mol;

    else if (old_mols.contains(molnum))
        return old_mols[molnum].molecule();

    else
        return Molecule();
}

/** Return the state of the molecule with number 'molnum' 
    after the delta - this returns an empty molecule if
    this molecule is not affected by the delta */
Molecule Delta::newMolecule(MolNum molnum) const
{
    if (isEmpty())
        return Molecule();
        
    else if (new_mol.number() == molnum)
        return new_mol;
        
    else if (new_mols.contains(molnum))
        return new_mols[molnum].molecule();

    else
        return Molecule();
}

/** Return the state of the changed non-energy-dependent system
    components before the delta */
const Values& Delta::oldValues() const
{
    return old_components;
}

/** Return the state of the changed non-energy-dependent system
    components after the delta */
const Values& Delta::newValues() const
{
    return new_components;
}

/** Return the value of the non-energy-dependent system component
    before the delta - this returns 0 if this component is
    not changed by the delta */
double Delta::oldValue(const Symbol &component) const
{
    if (isEmpty())
        return 0;
    else
        return old_components[component];
}

/** Return the value of the non-energy-dependent system component
    after the delta - this returns 0 if this component is
    not changed by the delta */
double Delta::newValue(const Symbol &component) const
{
    if (isEmpty())
        return 0;
    else
        return new_components[component];
}

/** Return the changed global system properties before the delta */
Properties Delta::oldProperties() const
{
    return old_properties;
}

/** Return the changed global system properties after the delta */
Properties Delta::newProperties() const
{
    return new_properties;
}

/** Return the global system property 'property' before the delta.
    This returns Property::null if this property was not affected
    by the delta */
const Property& Delta::oldProperty(const QString &property) const
{
    if (isEmpty())
        return Property::null();
    
    else if (old_properties.hasProperty(property))
        return old_properties.property(property);

    else
        return Property::null();
}

/** Return the global system property 'property' after the delta.
    This returns Property::null if this property was not affected
    by the delta */
const Property& Delta::newProperty(const QString &property) const
{
    if (isEmpty())
        return Property::null();
    
    else if (new_properties.hasProperty(property))
        return new_properties.property(property);

    else
        return Property::null();
}

/** Return the forcefield property 'property' for the forcefield with
    index 'ffidx' for before the delta  - this returns Property::null
    if this property is not affected by the delta */
const Property& Delta::oldProperty(const QString &property, const FFIdx &ffidx) const
{
    if (isEmpty())
        return Property::null();

    else if (ffidx.isNull())
        return this->oldProperty(property);

    else 
    {
        Properties props = old_ff_properties.value(ffidx);
        
        if (props.hasProperty(property))
            return props.property(property);
        else
            return Property::null();
    }
}

/** Return the forcefield property 'property' for the forcefield with
    index 'ffidx' for after the delta  - this returns Property::null
    if this property is not affected by the delta */
const Property& Delta::newProperty(const QString &property, const FFIdx &ffidx) const
{
    if (isEmpty())
        return Property::null();

    else if (ffidx.isNull())
        return this->newProperty(property);

    else 
    {
        Properties props = new_ff_properties.value(ffidx);
        
        if (props.hasProperty(property))
            return props.property(property);
        else
            return Property::null();
    }
}

/** Return the forcefield properties for the forcefield with
    index 'ffidx' for before the delta  - this returns an empty set
    if no forcefield properties are affected by the delta */
Properties Delta::oldProperties(const FFIdx &ffidx) const
{
    if (ffidx.isNull())
        return this->oldProperties();
    else 
        return old_ff_properties.value(ffidx);
}

/** Return the forcefield properties for the forcefield with
    index 'ffidx' for after the delta  - this returns an empty set
    if no forcefield properties are affected by the delta */
Properties Delta::newProperties(const FFIdx &ffidx) const
{
    if (ffidx.isNull())
        return this->newProperties();
    else 
        return new_ff_properties.value(ffidx);
}

/** Return the forcefield properties for the forcefields whose
    indicies are in 'ffidxs' for before the delta - this returns
    an empty set if no relevant forcefield properties are
    affected by this delta
    
    \throw SireBase::duplicate_property
*/
Properties Delta::oldProperties(const QList<FFIdx> &ffidxs) const
{
    if (isEmpty())
        return Properties();
        
    else if (ffidxs.isEmpty())
        return this->oldProperties();
        
    else if (ffidxs.count() == 1)
        return this->oldProperties(ffidxs.at(0));
    
    else
    {
        Properties props = old_ff_properties.value( ffidxs.at(0) );
        
        for (int i=1; i<ffidxs.count(); ++i)
        {
            Properties next_props = old_ff_properties.value( ffidxs.at(i) );
            
            if (props.isEmpty())
                props = next_props;
            else
            {
                for (Properties::const_iterator it = next_props.constBegin();
                     it != next_props.constEnd();
                     ++it)
                {
                    if (props.hasProperty(it.key()))
                    {
                        if (not props.property(it.key()).equals(it.value().read()))
                            throw SireBase::duplicate_property( QObject::tr(
                                "Two properties with the same name (%1) in different "
                                "forcefields %2 have different values! %3 vs. %4")
                                    .arg(it.key())
                                    .arg(Sire::toString(ffidxs))
                                    .arg(props.property(it.key()).toString())
                                    .arg(it.value().read().toString()), CODELOC );
                    }
                    else
                    {
                        props.setProperty(it.key(), it.value().read());
                    }
                }
            }
        }
        
        return props;
    }
}
                         
/** Return the forcefield properties for the forcefields whose
    indicies are in 'ffidxs' for after the delta - this returns
    an empty set if no relevant forcefield properties are
    affected by this delta
    
    \throw SireBase::duplicate_property
*/
Properties Delta::newProperties(const QList<FFIdx> &ffidxs) const
{
    if (isEmpty())
        return Properties();
        
    else if (ffidxs.isEmpty())
        return this->newProperties();
        
    else if (ffidxs.count() == 1)
        return this->newProperties(ffidxs.at(0));
    
    else
    {
        Properties props = new_ff_properties.value( ffidxs.at(0) );
        
        for (int i=1; i<ffidxs.count(); ++i)
        {
            Properties next_props = new_ff_properties.value( ffidxs.at(i) );
            
            if (props.isEmpty())
                props = next_props;
            else
            {
                for (Properties::const_iterator it = next_props.constBegin();
                     it != next_props.constEnd();
                     ++it)
                {
                    if (props.hasProperty(it.key()))
                    {
                        if (not props.property(it.key()).equals(it.value().read()))
                            throw SireBase::duplicate_property( QObject::tr(
                                "Two properties with the same name (%1) in different "
                                "forcefields %2 have different values! %3 vs. %4")
                                    .arg(it.key())
                                    .arg(Sire::toString(ffidxs))
                                    .arg(props.property(it.key()).toString())
                                    .arg(it.value().read().toString()), CODELOC );
                    }
                    else
                    {
                        props.setProperty(it.key(), it.value().read());
                    }
                }
            }
        }
        
        return props;
    }
}

/** Return the value of the property 'property' in the forcefields whose
    indicies are in 'ffidxs' for before the delta. Property::null is
    returned if this property in these forcefields are not affected
    by this delta
    
    \throw SireBase::duplicate_property
*/
const Property& Delta::oldProperty(const QString &property,
                                   const QList<FFIdx> &ffidxs) const
{
    if (isEmpty())
        return Property::null();
        
    else if (ffidxs.isEmpty())
        return this->oldProperty(property);

    else if (ffidxs.count() == 1)
        return this->oldProperty(property, ffidxs.at(0));
        
    else
    {
        const Property *prop = 0;
        
        foreach (FFIdx ffidx, ffidxs)
        {
            Properties props = old_ff_properties.value(ffidx);
            
            if (props.hasProperty(property))
            {
                if (prop)
                {
                    if (not prop->equals(props.property(property)))
                    {
                        throw SireBase::duplicate_property( QObject::tr(
                            "Two properties with the same name (%1) in different "
                            "forcefields %2 have different values! %3 vs. %4")
                                .arg(property)
                                .arg(ffidx.toString())
                                .arg(props.property(property).toString())
                                .arg(prop->toString()), CODELOC );
                    }
                }
                else
                {
                    prop = &(props.property(property));
                }
            }
        }
        
        if (prop)
            return *prop;
        else
            return Property::null();
    }
}
                            
/** Return the value of the property 'property' in the forcefields whose
    indicies are in 'ffidxs' for after the delta. Property::null is
    returned if this property in these forcefields are not affected
    by this delta
    
    \throw SireBase::duplicate_property
*/
const Property& Delta::newProperty(const QString &property,
                                   const QList<FFIdx> &ffidxs) const
{
    if (isEmpty())
        return Property::null();
        
    else if (ffidxs.isEmpty())
        return this->newProperty(property);

    else if (ffidxs.count() == 1)
        return this->newProperty(property, ffidxs.at(0));
        
    else
    {
        const Property *prop = 0;
        
        foreach (FFIdx ffidx, ffidxs)
        {
            Properties props = new_ff_properties.value(ffidx);
            
            if (props.hasProperty(property))
            {
                if (prop)
                {
                    if (not prop->equals(props.property(property)))
                    {
                        throw SireBase::duplicate_property( QObject::tr(
                            "Two properties with the same name (%1) in different "
                            "forcefields %2 have different values! %3 vs. %4")
                                .arg(property)
                                .arg(ffidx.toString())
                                .arg(props.property(property).toString())
                                .arg(prop->toString()), CODELOC );
                    }
                }
                else
                {
                    prop = &(props.property(property));
                }
            }
        }
        
        if (prop)
            return *prop;
        else
            return Property::null();
    }
}
