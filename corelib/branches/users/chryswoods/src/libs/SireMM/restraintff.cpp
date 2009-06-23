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

#include "restraintff.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMM;
using namespace SireFF;
using namespace SireMol;
using namespace SireCAS;
using namespace SireBase;
using namespace SireStream;

static const RegisterMetaType<RestraintFF> r_restraintff;

/** Serialise to a binary datastream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const RestraintFF &restraintff)
{
    writeHeader(ds, r_restraintff, 1);
    
    SharedDataStream sds(ds);
    
    sds << restraintff.restraints_by_index
        << restraintff.old_restraints_by_index
        << restraintff.recalc_from_scratch
        << static_cast<const G1FF&>(restraintff)
        << static_cast<const FF3D&>(restraintff);
        
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, RestraintFF &restraintff)
{
    VersionID v = readHeader(ds, r_restraintff);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> restraintff.restraints_by_index
            >> restraintff.old_restraints_by_index
            >> restraintff.recalc_from_scratch
            >> static_cast<G1FF&>(restraintff)
            >> static_cast<FF3D&>(restraintff);

        restraintff.reindexRestraints();
    }
    else
        throw version_error( v, "1", r_restraintff, CODELOC );
        
    return ds;
}

/** Internal function called to reindex the restraints */
void RestraintFF::reindexRestraints()
{
    const Restraint3DPtr *restraints_array = restraints_by_index.constData();
    quint32 nrestraints = restraints_by_index.count();
    
    restraints_by_molnum.clear();
    user_values.clear();
    builtin_symbols.clear();
    
    for (quint32 i=0; i<nrestraints; ++i)
    {
        const Restraint3D &restraint = restraints_array[i].read();
        
        Molecules mols = restraint.molecules();
        
        for (Molecules::const_iterator it = mols.constBegin();
             it != mols.constEnd();
             ++it)
        {
            restraints_by_molnum[it->number()].append(i);
        }
    
        user_values += restraint.userValues();
        builtin_symbols += restraint.builtinSymbols();
    }
    
    this->mustNowRecalculateFromScratch();
}

RestraintFF::RestraintFF();
RestraintFF::RestraintFF(const QString &name);

RestraintFF::RestraintFF(const RestraintFF &other);

RestraintFF::~RestraintFF();

const char* RestraintFF::typeName()
{
    return QMetaType::typeName( qMetaTypeId<RestraintFF>() );
}

RestraintFF& RestraintFF::operator=(const RestraintFF &other);

bool RestraintFF::operator==(const RestraintFF &other) const;
bool RestraintFF::operator!=(const RestraintFF &other) const;

const RestraintComponent& RestraintFF::_pvt_components() const;

/** Recalculate the restraint energy */
void RestraintFF::recalculateEnergy()
{
    if (old_restraints_by_index.isEmpty())
    {
        //recalculate the energy from scratch
        const Restraint3DPtr *restraints_array = restraints_by_index.constData();
        const int nrestraints = restraints_by_index.count();
        
        RestraintEnergy nrg(0);
        
        #pragma omp parallel
        {
            const Restraint3DPtr *my_restraints_array = restraints_array;
            
            RestraintEnergy my_nrg(0);
            
            #pragma omp for schedule(dynamic)
            for (int i=0; i<nrestraints; ++i)
            {
                my_nrg += my_restraints_array[i].read().energy();
            }
            
            #pragma omp critical
            {
                nrg += my_nrg;
            }
        }
        
        this->components().setEnergy(*this, total_nrg);

        recalc_from_scratch = false;
    }
    else
    {
        //evaluate the change in energy
        RestraintEnergy delta_nrg(0);
        
        const Restraint3DPtr *restraints_array = restraints_by_index.constData();
        const int nrestraints = restraints_by_index.count();
        
        for (QHash<quint32,Restraint3DPtr>::const_iterator 
                                        it = old_restraints_by_index.constBegin();
             it != old_restraints_by_index.constEnd();
             ++it)
        {
            BOOST_ASSERT( it.key() < nrestraints );
            
            delta_nrg += restraints_array[it.key()].read().energy();
            delta_nrg -= it.value().read().energy();
        }
        
        this->components().changeEnergy(*this, delta_nrg);
        old_restraints_by_index.clear();

        recalc_from_scratch = false;
    }
}

/** Update the restraints with the new molecule data in 'moldata' */
void RestraintFF::updateRestraints(const MoleculeData &moldata)
{
    //get the list of restraints potentially affected by this change in molecule
    QList<quint32> restraint_idxs = restraints_by_molnum.value(moldata.number());
    
    if (restraint_idxs.isEmpty())
        return;
        
    else if (restraint_idxs.count() == 1)
    {
        quint32 idx = *(restraint_idxs.constBegin());
        
        bool save_old_state = not ( recalc_from_scratch or 
                                    old_restraints_by_index.contains(idx) );
        
        Restraint3DPtr old_restraint;
        
        if (save_old_state)
            old_restraint = restraints_by_index.at(idx);
        
        restraints_by_index[idx].update(moldata);
        
        if (save_old_state)
            old_restraints_by_index.insert(idx, old_restraint);
    }
    else
    {
        std::auto_ptr<RestraintFF> old_state( this->clone() );
        
        try
        {
            foreach (quint32 idx, restraint_idxs)
            {
                bool save_old_state = not ( recalc_from_scratch or
                                            old_restraints_by_index.contains(idx) );
                                            
                Restraint3DPtr old_restraint;
                
                if (save_old_state)
                    old_restraint = restraints_by_index.at(idx);
                    
                restraints_by_index[idx].edit().update(moldata);
                
                if (save_old_state)
                    old_restraints_by_index.insert(idx, old_restraint);
            }
        }
        catch(...)
        {
            this->assign( *old_state );
            throw;
        }
    }
}
    
/** Update the restraints with the new molecule data in 'molecules' */
void RestraintFF::updateRestraints(const Molecules &molecules)
{
    //get the list of restraints potentially affected by this change in molecules
    QList<quint32> restraint_idxs;
    
    for (Molecules::const_iterator it = molecules.constBegin();
         it != molecules.constEnd();
         ++it)
    {
        restraint_idxs += restraints_by_molnum.value(it.key());
    }
    
    if (restraint_idxs.isEmpty())
        return;
        
    else if (restraint_idxs.count() == 1)
    {
        quint32 idx = *(restraint_idxs.constBegin());
        
        bool save_old_state = not ( recalc_from_scratch or 
                                    old_restraints_by_index.contains(idx) );
        
        Restraint3DPtr old_restraint;
        
        if (save_old_state)
            old_restraint = restraints_by_index.at(idx);
        
        restraints_by_index[idx].update(molecules);
        
        if (save_old_state)
            old_restraints_by_index.insert(idx, old_restraint);
    }
    else
    {
        std::auto_ptr<RestraintFF> old_state( this->clone() );
        
        try
        {
            foreach (quint32 idx, restraint_idxs)
            {
                bool save_old_state = not ( recalc_from_scratch or
                                            old_restraints_by_index.contains(idx) );
                                            
                Restraint3DPtr old_restraint;
                
                if (save_old_state)
                    old_restraint = restraints_by_index.at(idx);
                    
                restraints_by_index[idx].edit().update(molecules);
                
                if (save_old_state)
                    old_restraints_by_index.insert(idx, old_restraint);
            }
        }
        catch(...)
        {
            this->assign( *old_state );
            throw;
        }
    }
}

void RestraintFF::_pvt_updateName();

void RestraintFF::_pvt_added(const SireMol::PartialMolecule &mol, const PropertyMap&);
                
void RestraintFF::_pvt_removed(const SireMol::PartialMolecule &mol);

void RestraintFF::_pvt_changed(const SireMol::Molecule &mol);

void RestraintFF::_pvt_changed(const QList<SireMol::Molecule> &mols);

void RestraintFF::_pvt_removedAll();

bool RestraintFF::_pvt_wouldChangeProperties(SireMol::MolNum molnum, 
                                             const PropertyMap &map) const;

void RestraintFF::_pvt_added(const ViewsOfMol &mol, const PropertyMap &map);
                        
void RestraintFF::_pvt_removed(const ViewsOfMol &mol);

void RestraintFF::_pvt_removedAll(const PartialMolecule &mol);
void RestraintFF::_pvt_removedAll(const ViewsOfMol &mol);

const Space& RestraintFF::space() const;

bool RestraintFF::setSpace(const Space &space);

bool RestraintFF::setValue(const Symbol &symbol, double value);
double RestraintFF::getValue(const Symbol &symbol);
bool RestraintFF::hasValue(const Symbol &symbol);

bool RestraintFF::setProperty(const QString &name, const Property &property);
const Property& RestraintFF::property(const QString &name) const;
bool RestraintFF::containsProperty(const QString &name) const;
const Properties& RestraintFF::properties() const;

Symbols RestraintFF::symbols() const;
Symbols RestraintFF::userSymbols() const;
Symbols RestraintFF::builtinSymbols() const;

Values RestraintFF::values() const;
Values RestraintFF::userValues() const;
Values RestraintFF::builtinValues() const;

RestraintFF RestraintFF::differentiate(const Symbol &symbol) const;

/** Return whether or not this forcefield contains the restraint
    'restraint' */
bool RestraintFF::contains(const Restraint3D &restraint) const
{
    for (int i=0; i<restraints_by_index.count(); ++i)
    {
        if (restraints_by_index.at(i).read().equals(restraint))
            return true;
    }
    
    return false;
}

/** Add the passed restraint to this forcefield. This does nothing
    if this restraint is already part of this forcefield. This returns
    whether or not this changes this forcefield */
bool RestraintFF::add(const Restraint3D &restraint)
{
    //make sure that there are no current user values that clash
    //with the builtin symbols of this restraint
    const Symbols user_symbols = user_values.symbols();
    
    if (restraint.builtinSymbols().intersects(user_symbols))
        throw SireError::incompatible_error( QObject::tr(
            "Cannot add the restraint %1 to the forcefield %2 as there "
            "is a clash between the built-in symbols of this restraint (%3) "
            "and the user-supplied symbols of this forcefield (%4).")
                .arg(restraint.toString(), this->toString())
                .arg( Sire::toString(restraint.builtinSymbols()) )
                .arg( Sire::toString(user_symbols) ), CODELOC );

    Restraint3DPtr new_restraint( restraint );
    
    //update the restraint with the current symbol values
    foreach (Symbol symbol, user_symbols)
    {
        new_restraint.edit().setValue(symbol, user_values[symbol]);
    }
    
    //update the restraint with the current versions of the molecules
    if (not this->isEmpty())
    {
        new_restraint.edit().update( this->molecules() );
    }
    
    //now see if we have this restraint already?
    if (not this->contains(new_restraint))
    {
        //we don't - add the new restraint
        std::auto_ptr<RestraintFF> old_state( this->clone() );
        
        try
        {
            //add the restraint onto the list of existing restraints...
            restraints_by_index.append( new_restraint );
            
            //now add the molecules in this restraint
            this->add( new_restraint.molecules() );
            
            //finally, reindex the restraints
            this->reindexRestraints();
            
            return true;
        }
        catch(...)
        {
            this->assign(*old_state);
            throw;
        }
    }
    
    return false;
}

/** Return the array of all restraints */
QVector<Restraint3DPtr> RestraintFF::restraints() const
{
    return restraints_by_index;
}

/** Return the number of restraints in this forcefield */
int RestraintFF::nRestraints() const
{
    return restraints_by_index.count();
}

/** Return the ith restraint

    \throw SireError::index_error
*/
const Restraint3D& RestraintFF::restraintAt(int i) const
{
    return restraints_by_index.at( Index(i).map(this->nRestraints()) );
}

/** Remove the ith restraint

    \throw SireError::invalid_index
*/
void RestraintFF::removeRestraintAt(int i)
{
    i = Index(i).map( this->nRestraints() );
    
    std::auto_ptr<RestraintFF> old_state( this->clone() );
    
    try
    {
        //get the molecules in this restraint
        Molecules mols = restraints_by_index.at(i).read().molecules();
    
        //lose this restraint from the list
        restraints_by_index.remove(i);
        
        //reindex the restraints before removing the molecules so as
        //to ensure that we don't trigger infinite recursion as 
        //removing more molecules causes the restraint to be removed again
        this->reindexRestraints();
        
        this->remove(mols);
    }
    catch(...)
    {
        this->assign( *old_state );
        throw;
    }
}

/** Remove the restraint 'restraint' from this forcefield. This does
    nothing if this restraint is not in this forcefield. This returns
    whether or not the restraint was removed. */
bool RestraintFF::remove(const Restraint3D &restraint)
{
    for (int i=0; i<restraints_by_index.count(); ++i)
    {
        if (restraints_by_index.at(i).read().equals(restraint))
        {
            this->removeRestraintAt(i);
            return true;
        }
    }
    
    return false;
}

/** Force recalculation of the restraint energy from scratch */
void RestraintFF::mustNowRecalculateFromScratch()
{
    old_restraints_by_idx.clear();
}

void RestraintFF::force(ForceTable &forcetable, double scale_force=1);

void RestraintFF::force(ForceTable &forcetable, const Symbol &symbol,
                        double scale_force);
