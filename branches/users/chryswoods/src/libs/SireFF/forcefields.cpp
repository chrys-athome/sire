/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2007  Christopher Woods
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

#include "SireCAS/qhash_sirecas.h"
#include "SireFF/qhash_sireff.h"

#include <QMap>

#include "tostring.h"

#include "forcefields.h"

#include "ffgroupid.h"

#include "SireFF/errors.h"
#include "SireBase/errors.h"
#include "SireError/errors.h"

#include "SireMol/molecule.h"
#include "SireMol/partialmolecule.h"
#include "SireMol/residue.h"
#include "SireMol/newatom.h"

#include "SireBase/property.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireFF;
using namespace SireCAS;
using namespace SireMol;
using namespace SireBase;
using namespace Sire;

using namespace SireStream;

static const RegisterMetaType<ForceFields> r_ffields;

/** Serialise to a binary data stream */
QDataStream SIRESYSTEM_EXPORT &operator<<(QDataStream &ds, const ForceFields &ffields)
{
    writeHeader(ds, r_ffields, 1);

    SharedDataStream sds(ds);

    sds << ffields.ffields << static_cast<const ForceFieldsBase&>(ffields);

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIRESYSTEM_EXPORT &operator>>(QDataStream &ds, ForceFields &ffields)
{
    VersionID v = readHeader(ds, r_ffields);

    if (v == 1)
    {
        SharedDataStream sds(ds);

        sds >> ffields.ffields >> static_cast<ForceFieldsBase&>(ffields);
    }
    else
        throw version_error(v, "1", r_ffields, CODELOC);

    return ds;
}

/** Empty constructor */
ForceFields::ForceFields() : ForceFieldsBase()
{}

/** Construct from another set of ForceFields */
ForceFields::ForceFields(const ForceFieldsBase &base)
            : ForceFieldsBase(base),
              ffields(base.forceFields())
{}

/** Construct from a list of forcefields - if a forcefield is duplicated 
    in the list then the last one in the list is used */
ForceFields::ForceFields(const QList<ForceField> &ffields)
            : ForceFieldsBase()
{
    for (QList<ForceField>::const_iterator it = ffields.begin();
         it != ffields.end();
         ++it)
    {
        if ( this->contains(it->ID()) )
            this->set(*it);
        else
            this->add(*it);
    }
}
            
/** Construct from a hash of forcefields (the key must correspond to 
    the associated forcefields ID!) */
ForceFields::ForceFields(const QHash<ForceFieldID,ForceField> &ffields)
            : ForceFieldsBase()
{
    for (QHash<ForceFieldID,ForceField>::const_iterator it = ffields.begin();
         it != ffields.end();
         ++it)
    {
        BOOST_ASSERT( it.key() == it->ID() );
        
        this->add(it.value());
    }
}

/** Copy constructor */
ForceFields::ForceFields(const ForceFields &other)
            : ForceFieldsBase(other), ffields(other.ffields)
{}

/** Destructor */
ForceFields::~ForceFields()
{}

/** Copy assignment */
ForceFields& ForceFields::operator=(const ForceFieldsBase &other)
{
    ForceFieldsBase::operator=(other);
    ffields = other.forceFields();

    return *this;
}

/** Copy assignment */
ForceFields& ForceFields::operator=(const ForceFields &other)
{
    ForceFieldsBase::operator=(other);
    ffields = other.ffields;
    
    return *this;
}

/** Return a reference to the forcefield with ID == ffid

    \throw SireFF::missing_forcefield
*/
ForceField& ForceFields::getForceField(ForceFieldID ffid)
{
    QHash<ForceFieldID,ForceField>::iterator it = ffields.find(ffid);

    if (it == ffields.constEnd())
        throw SireFF::missing_forcefield( QObject::tr(
            "There is no forcefield with ID == %1 is this set! "
            "Available forcefields are %2.")
                .arg(ffid).arg(toString(ffields.keys())), CODELOC );

    return *it;
}

/** Return a reference to the forcefield with ID == ffid

    \throw SireFF::missing_forcefield
*/
const ForceField& ForceFields::getForceField(ForceFieldID ffid) const
{
    QHash<ForceFieldID,ForceField>::const_iterator it = ffields.find(ffid);

    if (it == ffields.constEnd())
        throw SireFF::missing_forcefield( QObject::tr(
            "There is no forcefield with ID == %1 is this set! "
            "Available forcefields are %2.")
                .arg(ffid).arg(toString(ffields.keys())), CODELOC );

    return *it;
}

/** Return a const reference to the forcefield with ID == ffid

    \throw SireFF::missing_forcefield
*/
const ForceField& ForceFields::getConstForceField(ForceFieldID ffid) const
{
    return this->getForceField(ffid);
}

/** Add the forcefield 'ffield' to this set. This will change
    all of the forcefields in this set so that they contain molecules
    that are at the same version as the molecules in 'ffield'

    \throw SireFF::duplicate_forcefield
*/
void ForceFields::add(const ForceField &ffield)
{
    if ( ffields.contains(ffield.ID()) )
        throw SireFF::duplicate_forcefield( QObject::tr(
            "Cannot add the forcefield \"%1\" (%2 %3) to this set "
            "as a copy of this forcefield already exists! (version == %4)")
                .arg(ffield.name()).arg(ffield.ID())
                .arg(ffield.version().toString(),
                     ffields.value(ffield.ID()).version().toString()),
                        CODELOC );

    //take a copy of this set so that we can maintain the invariant
    ForceFields orig = *this;

    try
    {
        //ensure that all forcefields have the same version
        //as these molecules
        this->change( ffield.molecules() );
        
        ffields.insert(ffield.ID(), ffield);

        //record the location of all of the molecules
        QSet<MoleculeID> molids = ffield.moleculeIDs();

        //record the location of all of the molecules
        ForceFieldsBase::addToIndex(molids, ffield.ID());
    }
    catch(...)
    {
        //something went wrong - restore the original
        *this = orig;

        //rethrow the exception
        throw;
    }
}

/** Set the forcefield in this group equal to 'ffield'. This 
    forcefield must be contained within this group. This will
    synchronise all of the molecules so that they have the
    same version as the molecules in this forcefield
    
    \throw SireFF::missing_forcefield
*/
void ForceFields::set(const ForceField &ffield)
{
    if ( not ffields.contains(ffield.ID()) )
        throw SireFF::missing_forcefield( QObject::tr(
            "Cannot update the forcefield \"%1\" (%2 %3) as it does not "
            "yet exist in this set.")
                .arg(ffield.name()).arg(ffield.ID())
                .arg(ffield.version().toString()), CODELOC );

    //do nothing if the version numbers are the same
    const ForceField &old_ffield = this->getConstForceField(ffield.ID());

    if (old_ffield.ID() == ffield.ID())
        return;

    //take a copy of this set so that we can maintain the invariant
    ForceFields orig = *this;

    try
    {
        //get the molecules in the old ffield...
        QSet<MoleculeID> old_molids = old_ffield.moleculeIDs();
        
        //update the forcefield...
        ffields.insert(ffield.ID(), ffield);

        //record the location of, and synchronise all of the molecules
        QSet<MoleculeID> molids = ffield.moleculeIDs();

        foreach (MoleculeID molid, old_molids)
        {
            if (not molids.contains(molid))
                ForceFieldsBase::removeFromIndex(molid, ffield.ID());
        }
        
        foreach (MoleculeID molid, molids)
        {
            if (not old_molids.contains(molid))
                ForceFieldsBase::addToIndex(molid, ffield.ID());
        }
        
        //now update all of the forcefields to have the same version
        //of the molecules as the changed forcefield
        this->change( ffield.molecules() );
    }
    catch(...)
    {
        //something went wrong - restore the original
        *this = orig;

        //rethrow the exception
        throw;
    }

}

/** Remove the forcefield with ID == ffid. This will also remove
    any molecules contained in that forcefield, and any expressions
    that depend on that forcefield. */
void ForceFields::remove(ForceFieldID ffid)
{
    ForceFieldsBase::removeFromIndex(ffid);
    ffields.remove(ffid);
}

/** Remove all forcefields that are called 'ffname' */
void ForceFields::remove(const QString &ffname)
{
    QSet<ForceFieldID> ffids;

    for (QHash<ForceFieldID,ForceField>::const_iterator it = ffields.constBegin();
         it != ffields.constEnd();
         ++it)
    {
        if (it->name() == ffname)
            ffids.insert(it.key());
    }

    foreach( ForceFieldID ffid, ffids )
    {
        this->remove(ffid);
    }
}

/** Return all of the forcefields in this set, indexed by
    ForceFieldID */
QHash<ForceFieldID,ForceField> ForceFields::forceFields() const
{
    return ffields;
}

/** Return the complete set of IDs of all of the forcefields
    in this set */
QSet<ForceFieldID> ForceFields::forceFieldIDs() const
{
    return ffields.keys().toSet();
}

/** Set the property called 'name' in the forcefield with ID == ffid 
    to the value 'value'
    
    \throw SireFF::missing_forcefield
    \throw SireBase::missing_property
*/
void ForceFields::setProperty(ForceFieldID ffid, const QString &name, 
                              const Property &value)
{
    this->getForceField(ffid).setProperty(name, value);
}
                     
/** Return all of the properties of all of the forcefields indexed
    by name */
QHash< QString, QHash<ForceFieldID,Property> > ForceFields::properties() const
{
    QHash< QString, QHash<ForceFieldID,Property> > props;
    
    for (QHash<ForceFieldID,ForceField>::const_iterator it = ffields.begin();
         it != ffields.end();
         ++it)
    {
        QHash<QString,Property> ffprops = it->properties();
        
        for (QHash<QString,Property>::const_iterator it2 = ffprops.constBegin();
             it2 != ffprops.constEnd();
             ++it2)
        {
            props[it2.key()].insert( it.key(), it2.value() );
        }
    }
    
    return props;
}

/** Change the molecule 'molecule'

    \throw SireBase::missing_property
    \throw SireError::invalid_cast
*/
void ForceFields::change(const PartialMolecule &molecule)
{
    const QSet<ForceFieldID> ffids = this->forceFieldsReferringTo(molecule.ID());

    if (not ffids.isEmpty())
    {
        //do we already contain this version of the molecule?
        // (all forcefields contain the same version, so only need
        //  to check the first forcefield that contains the molecule)
        ForceFieldID first_ffid = *(ffids.constBegin());
        
        if (ffields.constFind(first_ffid)->molecule(molecule.ID()).version()
                     == molecule.version())
        {
            //we already contain this version of the molecule
            return;
        }
    
        //we contain this molecule!
        if (ffids.count() == 1)
        {
            //no need to maintain state as only a single forcefield to change...
            ffields[first_ffid].change(molecule);

            //this is guaranteed not to throw
            ForceFieldsBase::changed(first_ffid);
        }
        else
        {
            //we need to save ffields so that it can be restored if there
            //is a problem
            QHash<ForceFieldID,ForceField> orig_ffields = ffields;

            try
            {
                foreach( ForceFieldID ffid, ffids )
                {
                    ffields[ffid].change(molecule);
                }

                //notify the system that the molecule has changed
                //(this is guaranteed not to throw)
                ForceFieldsBase::changed(ffids);
            }
            catch(...)
            {
                //restore the original forcefields
                ffields = orig_ffields;
                throw;
            }
        }
    }
}

/** Change the molecules  in 'mols' */
void ForceFields::change(const QHash<MoleculeID,PartialMolecule> &molecules)
{
    if (molecules.isEmpty())
        return;
    else if (molecules.count() == 1)
    {
        this->change( *(molecules.begin()) );
        return;
    }
    else
    {
        //get the list of molecules that are actually contained in these
        //forcefields that are of a different version to the molecules
        //in 'molecules'
        QHash<MoleculeID,PartialMolecule> mols_in_ffields = molecules;
        
        //also accumulate the list of forcefields that contain any one 
        //of these molecules
        QSet<ForceFieldID> ffids_with_mols;
        
        for (QHash<MoleculeID,PartialMolecule>::const_iterator it = molecules.begin();
             it != molecules.end();
             ++it)
        {
            QSet<ForceFieldID> ffids = this->forceFieldsReferringTo(it.key());
                
            //if this is empty then this molecule is not in this forcefield!
            if (ffids.isEmpty())
                mols_in_ffields.remove(it.key());
            else
            {
                //all forcefields have the same version of the molecule, so just
                //check the first forcefield...
                ForceFieldID first_ffid = *(ffids.constBegin());
                
                if (ffields.constFind(first_ffid)->molecule(it.key()).version()
                            == it->version())
                {
                    //the molecule exists in this forcefield, but is already
                    //at this version
                    mols_in_ffields.remove(it.key());
                }
                else
                    //record all forcefields that contain this molecule
                    ffids_with_mols.unite(ffids);
            }
        }
        
        if (mols_in_ffields.count() == 0)
            return;
        else if (mols_in_ffields.count() == 1)
        {
            this->change( *(mols_in_ffields.constBegin()) );
            return;
        }
    
        //must maintain the invariant
        ForceFields ffields_orig = *this;

        try
        {
            //loop over each force and change the molecules...
            foreach (ForceFieldID ffid, ffids_with_mols)
            {
                ffields.find(ffid)->change(mols_in_ffields);
            }

            ForceFieldsBase::changed(ffids_with_mols);
        }
        catch(...)
        {
            //restore the invariant
            *this = ffields_orig;
            throw;
        }
    }
}
    
/** Add the molecule 'molecule' to the group 'group' in the forcefield
    with ID == ffid 
    
    \throw SireFF::missing_forcefield
    \throw SireFF::missing_group
    \throw SireError::incompatible_error
    \throw SireError::invalid_cast
    \throw SireBase::missing_property
*/
void ForceFields::addTo( ForceFieldID ffid, const FFBase::Group &group,
                         const PartialMolecule &mol,
                         const ParameterMap &map )
{
    QSet<ForceFieldID> ffids = this->forceFieldsReferringTo(mol.ID());

    if (ffids.isEmpty())
    {
        //we don't yet have this molecule - add it now
        this->getForceField(ffid).addTo(group, mol, map);
        ForceFieldsBase::addToIndex(mol.ID(), ffid);
    }
    else
    {
        //get the old copy of the molecule
        PartialMolecule old_mol = ForceFieldsBase::molecule( mol.ID(), 
                                                             *(ffids.constBegin()) );
        
        //update all forcefields to match the new molecule version
        if (old_mol.version() != mol.version())
            this->change(mol);
            
        //add the molecule
        try
        {
            this->getForceField(ffid).addTo(group, mol, map);
            ForceFieldsBase::addToIndex(mol.ID(), ffid);
        }
        catch(...)
        {
            //restore the old version of the molecule (if necessary)
            if (old_mol.version() != mol.version())
                this->change(old_mol);
                
            throw;
        }
    }
}

/** Remove the molecule 'molecule' from the group 'group' in the forcefield 
    with ID == ffid
    
    \throw SireFF::missing_forcefield
    \throw SireFF::missing_group
*/
void ForceFields::removeFrom(ForceFieldID ffid, const FFBase::Group &group,
                             const PartialMolecule &mol)
{
    if (ForceFieldsBase::refersTo(mol.ID(), ffid))
    {
        ForceField &ffield = this->getForceField(ffid);
    
        if (ffield.removeFrom(group, mol))
        {
            if (not ffield.refersTo(mol.ID()))
                ForceFieldsBase::removeFromIndex(mol.ID(), ffid);
        }
    }
}

/** Return whether or not the group 'group' in the forcefield 'ffid' 
    refers to the molecule with ID == molid 
    
    \throw SireFF::missing_forcefield
    \throw SireFF::missing_group
*/
bool ForceFields::refersTo(MoleculeID molid, ForceFieldID ffid,
                           const FFBase::Group &group) const
{
    return this->getForceField(ffid).refersTo(molid, group);
}

/** Return the IDs of all groups that refer to the molecule with ID == molid */
QSet<FFGroupID> ForceFields::forceFieldGroupsReferringTo(MoleculeID molid) const
{
    QSet<ForceFieldID> ffids = this->forceFieldsReferringTo(molid);
    
    QSet<FFGroupID> ffgroupids;
    
    foreach (ForceFieldID ffid, ffids)
    {
        QSet<FFBase::Group> groups = this->getForceField(ffid).groupsReferringTo(molid);
        
        foreach (FFBase::Group group, groups)
        {
            ffgroupids.insert( FFGroupID(ffid,group) );
        }
    }
    
    return ffgroupids;
}

/** Return the IDs of all molecules that are in the group 'group'
    in the forcefield with ID == ffid
    
    \throw SireFF::missing_forcefield
    \throw SireFF::missing_group
*/
QSet<MoleculeID> ForceFields::moleculeIDs(ForceFieldID ffid,
                                          const FFBase::Group &group) const
{
    return this->getForceField(ffid).moleculeIDs(group);
}

/** Return the part of the molecule with ID == molid that is in the group
    'group' in the forcefield with ID == ffid
    
    \throw SireMol::missing_molecule
    \throw SireFF::missing_forcefield
    \throw SireFF::missing_group
*/
PartialMolecule ForceFields::molecule(MoleculeID molid, ForceFieldID ffid,
                                      const FFBase::Group &group) const
{
    return this->getForceField(ffid).molecule(molid, group);
}
                                      
/** Assert that all of the FFComponents in the expression 'expression'
    actually exist in this set of forcefields!

    \throw SireFF::missing_forcefield
    \throw SireFF::missing_component
*/
void ForceFields::assertValidComponents(const FFExpression &expression) const
{
    QSet<FFComponent> ffcomps = expression.components();

    foreach( FFComponent ffcomp, ffcomps )
    {
        this->getForceField(ffcomp.forceFieldID()).assertContains(ffcomp);
    }
}

/** Assert that this set of forcefields contains the same forcefields
    as 'other' (same forcefields, but may be at different versions) */
void ForceFields::assertSameContents(const ForceFieldsBase &other) const
{
    if (this->forceFieldIDs() != other.forceFieldIDs())
    {
        throw SireError::incompatible_error( QObject::tr(
            "The two forcefields objects have different forcefield contents!"),
                CODELOC );
    }
}

/** Completely reindex all of the molecules in the forcefields */
void ForceFields::reindex()
{
    this->clearIndex();
    
    for (QHash<ForceFieldID,ForceField>::const_iterator it = ffields.constBegin();
         it != ffields.constEnd();
         ++it)
    {
        this->addToIndex( it->moleculeIDs(), it.key() );
    }
}

/** Copy the forcefields parts of 'other' to this forcefield */
void ForceFields::_pvt_copy(const ForceFieldsBase &other)
{
    ffields = other.forceFields();
}

/** Return the IDs of all forcefields that are called 'ffname' 

    \throw SireFF::missing_forcefield
*/
QSet<ForceFieldID> ForceFields::getFFIDs(const QString &ffname) const
{
    QSet<ForceFieldID> ffids;

    for (QHash<ForceFieldID,ForceField>::const_iterator it = ffields.begin();
         it != ffields.end();
         ++it)
    {
        if (it->name() == ffname)
            ffids.insert(it.key());
    }
    
    if (ffids.isEmpty())
        throw SireFF::missing_forcefield( QObject::tr(
            "There is no forcefield called \"%1\" in this set.")
                .arg(ffname), CODELOC );
    
    return ffids;
}

/** Return all of the energy components of the forcefield with ID == ffid

    \throw SireFF::missing_forcefield
*/
Values ForceFields::getEnergies(ForceFieldID ffid)
{
    return this->getForceField(ffid).energies();
}

/** Return all of the energy components of the forcefields whose
    IDs are in ffids

    \throw SireFF::missing_forcefield
*/
Values ForceFields::getEnergies(const QSet<ForceFieldID> &ffids)
{
    if (ffids.isEmpty())
        return Values();
    else if (ffids.count() == 1)
        return this->getEnergies( *(ffids.begin()) );
    else
    {
        Values nrgs;

        foreach (ForceFieldID ffid, ffids)
        {
            nrgs += this->getEnergies(ffid);
        }

        return nrgs;
    }
}

/** Return the values of all of the passed energy components */
Values ForceFields::getEnergies(const QSet<FFComponent> &components)
{
    if (components.isEmpty())
        return Values();

    Values nrgs;
    nrgs.reserve(components.count());

    //sort the components by forcefield...
    QHash< ForceFieldID,QSet<FFComponent> > sorted_components;

    for (QSet<FFComponent>::const_iterator it = components.begin();
         it != components.end();
         ++it)
    {
        sorted_components[it->forceFieldID()].insert(*it);
    }

    //now add the energy of each component
    for (QHash< ForceFieldID,QSet<FFComponent> >::const_iterator
                                    it = sorted_components.constBegin();
         it != sorted_components.constEnd();
         ++it)
    {
        const QSet<FFComponent> &ffcomps = *it;

        if (ffcomps.count() == 1)
        {
            const FFComponent &ffcomp = *(ffcomps.begin());
            nrgs.set( ffcomp, this->getEnergy(ffcomp) );
        }
        else
        {
            nrgs += this->getForceField(it.key()).energies(ffcomps);
        }
    }

    return nrgs;
}

/** Return the total energy of all of the forcefields in this set.

    This calculates the energy as the sum of the total energies
    of all of the forcefields.
*/
double ForceFields::getTotalEnergy()
{
    double nrg = 0;

    for (QHash<ForceFieldID,ForceField>::iterator it = ffields.begin();
         it != ffields.end();
         ++it)
    {
        nrg += it->energy();
    }

    return nrg;
}

/** Return the energy of the component 'component'

    \throw SireFF::missing_forcefield
    \throw SireFF::missing_component
*/
double ForceFields::getEnergy(const FFComponent &component)
{
    return this->getForceField(component.forceFieldID()).energy(component);
}
