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

#include "g1ff.h"

#include "SireMol/mgnum.h"
#include "SireMol/mgname.h"
#include "SireMol/molnum.h"
#include "SireMol/molname.h"

#include "SireMol/mover.hpp"

#include "SireMol/errors.h"
#include "SireError/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireFF;
using namespace SireFF::detail;

using namespace SireMol;
using namespace SireStream;

static const RegisterMetaType<G1FF> r_g1ff( MAGIC_ONLY, "SireFF::G1FF" );

/** Serialise to a binary datastream */
QDataStream SIREFF_EXPORT &operator<<(QDataStream &ds,
                                      const G1FF &g1ff)
{
    writeHeader(ds, r_g1ff, 1);
    
    SharedDataStream sds(ds);
    
    sds << g1ff.molgroup << g1ff.allow_overlap_of_atoms
        << static_cast<const FF&>(g1ff);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREFF_EXPORT &operator>>(QDataStream &ds,
                                      G1FF &g1ff)
{
    VersionID v = readHeader(ds, r_g1ff);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        sds >> g1ff.molgroup >> g1ff.allow_overlap_of_atoms
            >> static_cast<FF&>(g1ff);
    }
    else
        throw version_error(v, "1", r_g1ff, CODELOC);
        
    return ds;
}

/** Constructor */
G1FF::G1FF(bool allow_overlap) : FF(), allow_overlap_of_atoms(allow_overlap)
{}

/** Copy constructor */
G1FF::G1FF(const G1FF &other) 
     : FF(other), molgroup(other.molgroup),
       allow_overlap_of_atoms(other.allow_overlap_of_atoms)
{}

/** Copy assignment operator */
G1FF& G1FF::operator=(const G1FF &other)
{
    molgroup = other.molgroup;
    allow_overlap_of_atoms = other.allow_overlap_of_atoms;
    FF::operator=(other);
    
    return *this;
}

/** Assert that this forcefield contains the group with number 'mgnum'

    \throw SireMol::missing_group
*/
void G1FF::assertContains(MGNum mgnum) const
{
    if (molgroup.number() != mgnum)
        throw SireMol::missing_group( QObject::tr(
            "The forcefield %1 does not contain a group with "
            "number %2. The only group it contains has number %4.")
                .arg(this->name())
                .arg(mgnum).arg(molgroup.number()), CODELOC );
}

/** Return a reference to the group with number 'mgnum'

    \throw SireMol::missing_group
*/
const MolGroup& G1FF::getGroup(MGNum mgnum) const
{
    G1FF::assertContains(mgnum);
    return molgroup;
}

/** Return const pointers to the groups with number 'mgnums'

    \throw SireMol::missing_group
*/
void G1FF::getGroups(const QList<MGNum> &mgnums,
                     QVarLengthArray<const MolGroup*,10> &groups) const
{
    groups.clear();
    
    foreach (MGNum mgnum, mgnums)
    {
        G1FF::assertContains(mgnum);
        groups.append( &molgroup );
    }
}

/** Return pointers to all of the groups in this forcefield */
QHash<MGNum,const MolGroup*> G1FF::getGroups() const
{
    QHash<MGNum,const MolGroup*> groups;
    groups.reserve(1);
    groups.insert( molgroup.number(), &molgroup );
    
    return groups;
}

/** Assert that i refers to a valid group

    \throw SireError::program_bug
*/
void G1FF::assertValidGroup(quint32 i) const
{
    if (i != 0)
        throw SireError::program_bug( QObject::tr(
            "G1FF should only ever use group index 0 - there is a bug "
            "somewhere as we've just been passed group index %1.")
                .arg(i), CODELOC );
}

/** Set the name of the group in this forcefield */
void G1FF::group_setName(quint32 i, const QString &new_name)
{
    assertValidGroup(i);
    molgroup.setName(new_name);
}

/** Assert that there is no overlap between the atoms in 
    'molview' and the atoms in 'group'
    
    \throw SireMol::duplicate_atom
*/
void G1FF::assertNoOverlap(const MolGroup &group,
                           const MoleculeView &molview) const
{
    if (group.intersects(molview))
        throw SireMol::duplicate_atom( QObject::tr(
            "Some of the atoms in the view of the molecule %1 (%2) "
            "are already present in the forcefield group %3 (%4).")
                .arg(molview.data().name()).arg(molview.data().number())
                .arg(group.name()).arg(group.number()), CODELOC );
}

/** Assert that there is no overlap between the atoms in 'molview'
    and any atoms that exist currently in this molecule
    
    \throw SireMol::duplicate_atom
*/
void G1FF::assertNoOverlap(const MoleculeView &molview) const
{
    this->assertNoOverlap(molgroup, molview);
}

/** Add the molecule view in 'molview' to this forcefield, using the 
    supplied property map to get the names of the properties that contain
    the required forcefield parameters
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void G1FF::group_add(quint32 i, const MoleculeView &molview,
                     const PropertyMap &map)
{
    assertValidGroup(i);

    if (molview.selection().isEmpty())
        return;

    if (not allow_overlap_of_atoms)
        this->assertNoOverlap(molview);

    FFMolGroupPvt old_state = molgroup;
    
    try
    {
        molgroup.add(molview);
        this->_pvt_rebuild(molview.data().number(), map);
    }
    catch(...)
    {
        molgroup = old_state;
        throw;
    }
}

/** Add the views of the molecule in 'molviews' to this forcefield, using the 
    supplied property map to get the names of the properties that contain
    the required forcefield parameters
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void G1FF::group_add(quint32 i, const ViewsOfMol &molviews, 
                     const PropertyMap &map)
{
    assertValidGroup(i);

    if (molviews.isEmpty())
        return;

    if (not allow_overlap_of_atoms)
    {
        this->assertNoOverlap(molviews);
        molviews.assertNoOverlap();
    }
    
    FFMolGroupPvt old_state = molgroup;
    
    try
    {
        molgroup.add(molviews);
        this->_pvt_rebuild(molviews.number(), map);
    }
    catch(...)
    {
        molgroup = old_state;
        throw;
    }
}

/** Add the molecules in 'molecules' to this forcefield, using the 
    supplied property map to get the names of the properties that contain
    the required forcefield parameters
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void G1FF::group_add(quint32 i, const Molecules &molecules, 
                     const PropertyMap &map)
{
    assertValidGroup(i);

    if (molecules.isEmpty())
        return;

    if (not allow_overlap_of_atoms)
    {
        //assert that there is no overlap between the molecules to be added
        //and the molecules already existing in this forcefield
        for (Molecules::const_iterator it = molecules.constBegin();
             it != molecules.constEnd();
             ++it)
        {
            this->assertNoOverlap(*it);
        
            //also assert that there is no overlap within the molecule
            it->assertNoOverlap();
        }
    }
    
    //save the old state of the molgroup
    FFMolGroupPvt old_state = molgroup;
    
    try
    {
        QList<ViewsOfMol> added_mols;
    
        if (not allow_overlap_of_atoms)
        {
            //add the molecules - they must be unique views of we'd
            //have thrown an exception earlier!
            added_mols = molgroup.addIfUnique(molecules);
        }
        else
        {
            //add the molecules...
            molgroup.add(molecules);
            
            //now convert the 'Molecules' into a list...
            for (Molecules::const_iterator it = molecules.constBegin();
                 it != molecules.constEnd();
                 ++it)
            {
                added_mols.append(*it);
            }
        }
        
        //now get the parameters for all of the molecules
        this->_pvt_rebuild(added_mols, map);
    }
    catch(...)
    {
        molgroup = old_state;
        throw;
    }
}

/** Add the views of the molecules in the group 'molgroup' to this forcefield, 
    using the supplied property map to get the names of the properties that 
    contain the required forcefield parameters
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void G1FF::group_add(quint32 i, const MolGroup &new_group, 
                     const PropertyMap &map)
{
    G1FF::group_add(i, new_group.molecules(), map);
}

/** Add the molecule view in 'molview' to this forcefield, using the 
    supplied property map to get the names of the properties that contain
    the required forcefield parameters
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
bool G1FF::group_addIfUnique(quint32 i, const MoleculeView &molview, 
                             const PropertyMap &map)
{
    assertValidGroup(i);

    if (molview.selection().isEmpty())
        return false;

    FFMolGroupPvt old_state = molgroup;
    
    try
    {
        if (molgroup.addIfUnique(molview))
        {
            if (not allow_overlap_of_atoms)
            {
                //the molecule view was added successfully
                // - we must ensure that this view did not overlap
                //   with any of the existing atoms
                this->assertNoOverlap(old_state, molview);
            }

            //now rebuild this molecule in the forcefield
            this->_pvt_rebuild(molview.data().number(), map);
            
            return true;
        }
        else
            return false;
    }
    catch(...)
    {
        molgroup = old_state;
        throw;
    }
    
    return false;
}
                             
/** Add the views of the molecule in 'molviews' to this forcefield, using the 
    supplied property map to get the names of the properties that contain
    the required forcefield parameters
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
ViewsOfMol G1FF::group_addIfUnique(quint32 i, const ViewsOfMol &molviews, 
                                   const PropertyMap &map)
{
    assertValidGroup(i);

    if (molviews.isEmpty())
        return ViewsOfMol();
          
    FFMolGroupPvt old_state = molgroup;
    
    try
    {
        ViewsOfMol added_views = molgroup.addIfUnique(molviews);
        
        if (not added_views.isEmpty())
        {
            if (not allow_overlap_of_atoms)
            {
                this->assertNoOverlap(old_state, added_views);
                added_views.assertNoOverlap();
            }

            this->_pvt_rebuild(molviews.number(), map);
        }
        
        return added_views;
    }
    catch(...)
    {
        molgroup = old_state;
        throw;
    }
    
    return ViewsOfMol();
}

/** Add the molecules in 'molecules' to this forcefield, using the 
    supplied property map to get the names of the properties that contain
    the required forcefield parameters
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
QList<ViewsOfMol> G1FF::group_addIfUnique(quint32 i, const Molecules &molecules, 
                                          const PropertyMap &map)
{
    assertValidGroup(i);

    if (molecules.isEmpty())
        return QList<ViewsOfMol>();
        
    FFMolGroupPvt old_state = molgroup;
    
    try
    {
        QList<ViewsOfMol> added_mols = molgroup.addIfUnique(molecules);
        
        if (not added_mols.isEmpty())
        {
            if (not allow_overlap_of_atoms)
            {
                //assert that there is no overlap between the added molecules
                //and the existing molecules
                foreach (const ViewsOfMol &added_mol, added_mols)
                {
                    this->assertNoOverlap(old_state, added_mol);
                    added_mol.assertNoOverlap();
                }
            }
            
            //now get the parameters
            this->_pvt_rebuild(added_mols, map);
        }
        
        return added_mols;
    }
    catch(...)
    {
        molgroup = old_state;
        throw;
    }
    
    return QList<ViewsOfMol>();
}

/** Add the views of the molecules in the group 'molgroup' to this forcefield, 
    using the supplied property map to get the names of the properties that 
    contain the required forcefield parameters
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
QList<ViewsOfMol> G1FF::group_addIfUnique(quint32 i, const MolGroup &new_group, 
                                          const PropertyMap &map)
{
    return G1FF::group_addIfUnique(i, new_group.molecules(), map);
}

/** Remove the molecule view in 'molview' from this forcefield */
bool G1FF::group_remove(quint32 i, const MoleculeView &molview)
{
    assertValidGroup(i);
    
    FFMolGroupPvt old_state = molgroup;
    
    try
    {
        if (molgroup.remove(molview))
        {
            this->_pvt_rebuild(molview.data().number(), map);
            return true;
        }
    }
    catch(...)
    {
        molgroup = old_state;
        throw;
    }
    
    return false;
}

/** Remove all of the views in 'molviews' from this forcefield */
ViewsOfMol G1FF::group_remove(quint32 i, const ViewsOfMol &molviews)
{
    assertValidGroup(i);
    
    FFMolGroupPvt old_state = molgroup;
    
    try
    {
        ViewsOfMol removed_views = molgroup.remove(molviews);
        
        if (not removed_views.isEmpty())
            this->_pvt_rebuild(removed_views.number());
            
        return removed_views;
    }
    catch(...)
    {
        molgroup = old_state;
        throw;
    }
    
    return ViewsOfMol();
}

/** Remove the molecules in 'molecules' from this forcefield */
QList<ViewsOfMol> G1FF::group_remove(quint32 i, const Molecules &molecules)
{
    assertValidGroup(i);
    
    FFMolGroupPvt old_state = molgroup;
    
    try
    {
        QList<ViewsOfMol> removed_mols = molgroup.remove(molecules);
        
        if (not removed_mols.isEmpty())
            this->_pvt_rebuild(removed_mols);
            
        return removed_mols;
    }
    catch(...)
    {
        molgroup = old_state;
        throw;
    }
    
    return QList<ViewsOfMol>();
}

/** Remove all of the molecules in the molecule group 'molgroup' from this
    forcefield */
QList<ViewsOfMol> G1FF::group_remove(quint32 i, const MolGroup &new_group)
{
    return G1FF::group_remove(i, new_group.molecules());
}

/** Remove the molecule view in 'molview' from this forcefield */
bool G1FF::group_removeAll(quint32 i, const MoleculeView &molview)
{
    assertValidGroup(i);
    
    FFMolGroupPvt old_state = molgroup;
    
    try
    {
        if (molgroup.removeAll(molview))
        {
            this->_pvt_rebuild(molview.data().number());
            return true;
        }
    }
    catch(...)
    {
        molgroup = old_state;
        throw;
    }
    
    return false;
}

/** Remove all of the views in 'molviews' from this forcefield */
ViewsOfMol G1FF::group_removeAll(quint32 i, const ViewsOfMol &molviews)
{
    assertValidGroup(i);
    
    FFMolGroupPvt old_state = molgroup;
    
    try
    {
        ViewsOfMol removed_views = molgroup.removeAll(molviews);
        
        if (not removed_views.isEmpty())
            this->_pvt_rebuild(molviews.number());
            
        return removed_views;
    }
    catch(...)
    {
        molgroup = old_state;
        throw;
    }
    
    return ViewsOfMol();
}

/** Remove the molecules in 'molecules' from this forcefield */
QList<ViewsOfMol> G1FF::group_removeAll(quint32 i, const Molecules &molecules)
{
    assertValidGroup(i);
    
    FFMolGroupPvt old_state = molgroup;
    
    try
    {
        QList<ViewsOfMol> removed_mols = molgroup.removeAll(molecules);
        
        if (not removed_mols.isEmpty())
            this->_pvt_rebuild(removed_mols);
            
        return removed_mols;
    }
    catch(...)
    {
        molgroup = old_state;
        throw;
    }
    
    return QList<ViewsOfMol>();
}

/** Remove all of the molecules in the molecule group 'molgroup' from this
    forcefield */
QList<ViewsOfMol> G1FF::group_removeAll(quint32 i, const MolGroup &new_group)
{
    return G1FF::group_removeAll(i, new_group.molecules());
}

/** Remove the molecule with number 'molnum' from this forcefield */
ViewsOfMol G1FF::group_remove(quint32 i, MolNum molnum)
{
    assertValidGroup(i);
    
    FFMolGroupPvt old_state = molgroup;
    
    try
    {
        ViewsOfMol removed_mol = molgroup.remove(molnum);
        
        if (not removed_mol.isEmpty())
            this->_pvt_rebuild(molnum);
            
        return removed_mol;
    }
    catch(...)
    {
        molgroup = old_state;
        throw;
    }
    
    return ViewsOfMol();
}

/** Remove all of the molecules whose numbers are in 'molnum' from
    this forcefield */
QList<ViewsOfMol> G1FF::group_remove(quint32 i, const QSet<MolNum> &molnums)
{
    assertValidGroup(i);
    
    FFMolGroupPvt old_state = molgroup;
    
    try
    {
        QList<ViewsOfMol> removed_mols = molgroup.remove(molnums);
        
        if (not removed_mols.isEmpty())
            this->_pvt_rebuild(removed_mols);
            
        return removed_mols;
    }
    catch(...)
    {
        molgroup = old_state;
        throw;
    }
    
    return QList<ViewsOfMol>();
}

/** Completely remove all molecules from this forcefield */
void G1FF::group_removeAll(quint32 i)
{
    molgroup.removeAll();
    this->_pvt_removeAll();
}

/** Update the molecule whose data is in 'moldata' to use this
    version of the molecule data in this forcefield */
bool G1FF::group_update(quint32 i, const MoleculeData &moldata)
{
    assertValidGroup(i);
    
    FFMolGroupPvt old_state = molgroup;
    
    try
    {
        if (molgroup.update(moldata))
        {
            this->_pvt_update(moldata.number());
            return true;
        }
    }
    catch(...)
    {
        molgroup = old_state;
        throw;
    }
    
    return false;
}

/** Update this forcefield so that it uses the same version of the
    molecules as in 'molecules' */
QList<Molecule> G1FF::group_update(quint32 i, const Molecules &molecules)
{
    assertValidGroup(i);
    
    FFMolGroupPvt old_state = molgroup;
    
    try
    {
        QList<Molecule> updated_mols = molgroup.update(molecules);
        
        if (not updated_mols.isEmpty())
            this->_pvt_update(updated_mols);
            
        return updated_mols;
    }
    catch(...)
    {
        molgroup = old_state;
        throw;
    }
    
    return QList<Molecule>();
}

/** Update the molecule group in this forcefield so that it has
    the same molecule versions as in 'new_group' */
QList<Molecule> G1FF::group_update(quint32 i, const MolGroup &new_group)
{
    return G1FF::group_update(i, new_group.molecules());
}

/** Set the contents of this forcefield so that it only contains the 
    view of the molecule in 'molview' (using the supplied property
    map to find the properties that contain the required parameters
    for this forcefield) */
bool G1FF::group_setContents(quint32 i, const MoleculeView &molview, 
                             const PropertyMap &map)
{
    assertValidGroup(i);
    
    FFMolGroupPvt old_state = molgroup;
    
    try
    {
        if (this->_pvt_changedProperties(molview.data().number(), map))
        {
            //we are changing the properties used to get the parameters
            molgroup.setContents(molview);
            this->_pvt_removeAll();
            this->_pvt_rebuild(molview.data().number(), map);
            return true;
        }
    
        if (molgroup.setContents(molview))
        {
            //we are changing the contents of this forcefield
            this->_pvt_removeAll();
            this->_pvt_rebuild(molview.data().number(), map);
            return true;
        }
    }
    catch(...)
    {
        molgroup = old_state;
        throw;
    }
    
    return false;
}

/** Set the contents of this forcefield so that it contains just
    the views of the molecule in 'molviews' */
bool G1FF::group_setContents(quint32 i, const ViewsOfMol &molviews, 
                             const PropertyMap &map)
{
    assertValidGroup(i);
    
    if (not allow_overlap_of_atoms)
        molviews.assertNoOverlap();
        
    FFMolGroupPvt old_state = molgroup;
    
    try
    {
        if (this->_pvt_changedProperties(molviews.number(), map))
        {
            molgroup.setContents(molviews);
            this->_pvt_removeAll();
            this->_pvt_rebuild(molviews.number(), map);
            return true;
        }
        
        if (molgroup.setContents(molviews))
        {
            this->_pvt_removeAll();
            this->_pvt_rebuild(molviews.number(), map);
            return true;
        }
    }
    catch(...)
    {
        molgroup = old_state;
        throw;
    }
    
    return false;
}

/** Set the contents of this forcefield so that it only contains 'molecules' */
bool G1FF::group_setContents(quint32 i, const Molecules &molecules, 
                             const PropertyMap &map)
{
    assertValidGroup(i);
    
    if (not allow_overlap_of_atoms)
    {
        for (Molecules::const_iterator it = molecules.constBegin();
             it != molecules.constEnd();
             ++it)
        {
            it->assertNoOverlap();
        }
    }
    
    FFMolGroupPvt old_state = molgroup;
    
    try
    {
        if (this->_pvt_changedProperties(molecules, map))
        {
            molgroup.setContents(molecules);
            this->_pvt_removeAll();
            
            QList<ViewsOfMol> added_mols;
            
            for (Molecules::const_iterator it = molecules.constBegin();
                 it != molecules.constEnd();
                 ++it)
            {
                added_mols.append(*it);
            }
            
            this->_pvt_rebuild(added_mols, map);
            return true;
        }
        
        if (molgroup.setContents(molecules))
        {
            this->_pvt_removeAll();
            
            QList<ViewsOfMol> added_mols;
            
            for (Molecules::const_iterator it = molecules.constBegin();
                 it != molecules.constEnd();
                 ++it)
            {
                added_mols.append(*it);
            }
            
            this->_pvt_rebuild(added_mols, map);
            return true;
        }
        
    }
    catch(...)
    {
        molgroup = old_state;
        throw;
    }
    
    return false;
}

/** Set the contents of this forcefield so that it contains only
    the molecules in 'molgroup' */
bool G1FF::group_setContents(quint32 i, const MolGroup &new_group, 
                             const PropertyMap &map)
{
    assertValidGroup(i);
    
    ... - keep order the same!!!
}

void G1FF::group_validateSane(quint32 i) const;
*/
