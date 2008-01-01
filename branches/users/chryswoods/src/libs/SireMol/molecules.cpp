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

#include "molecules.h"
#include "partialmolecule.h"
#include "selector.hpp"
#include "atomselection.h"
#include "viewsofmol.h"

#include "mover.hpp"
#include "editor.hpp"

#include "atom.h"
#include "cutgroup.h"
#include "residue.h"
#include "chain.h"
#include "segment.h"

#include "SireMol/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include "tostring.h"

using namespace SireMol;
using namespace SireStream;

static const RegisterMetaType<Molecules> r_mols;

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const Molecules &mols)
{
    writeHeader(ds, r_mols, 1);

    SharedDataStream sds(ds);

    sds << mols.mols;

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, Molecules &mols)
{
    VersionID v = readHeader(ds, r_mols);

    if (v == 1)
    {
        SharedDataStream sds(ds);
        sds >> mols.mols;
    }
    else
        throw version_error(v, "1", r_mols, CODELOC);

    return ds;
}

/** Add the view 'view' to this set of molecules - this
    adds this view even if it already exists in this set */
void Molecules::add(const MoleculeView &molview)
{
    PartialMolecule mol(molview);
    
    if (mol.selection().isEmpty())
        return;

    QHash<MolNum,ViewsOfMol>::iterator it = mols.find(mol.number());

    if ( it != mols.end() )
    {
        it->add(mol.selection());
    }
    else
    {
        mols.insert( mol.number(), mol );
    }
}

/** Add the view 'view' to this set of molecules - this
    adds this view only if it doesn't already exist in 
    this set - this returns whether or not the view
    was added */
bool Molecules::addIfUnique(const MoleculeView &molview)
{
    PartialMolecule mol(molview);
    
    if (mol.selection().isEmpty())
        return false;
        
    QHash<MolNum,ViewsOfMol>::iterator it = mols.find(mol.number());
    
    if ( it != mols.end() )
    {
        if (it->addIfUnique(mol.selection()))
            return true;
        else
            return false;
    }
    else
    {
        mols.insert( mol.number(), mol );
        return true;
    }
}

/** Add the views in 'molviews' to this set. This adds
    all of the views, even if they are already present
    in this set. */
void Molecules::add(const ViewsOfMol &molviews)
{
    if (molviews.selection().nSelectedAtoms() == 0)
        return;

    QHash<MolNum,ViewsOfMol>::iterator it = mols.find(molviews.number());
    
    if ( it != mols.end() )
    {
        it->add(molviews.selections());
    }
    else
    {
        mols.insert( molviews.number(), molviews );
    }
}

/** Add the views in 'molviews' to this set. This only
    adds the views that don't already exist in this set, 
    and returns the views that have been added */
ViewsOfMol Molecules::addIfUnique(const ViewsOfMol &molviews)
{
    if (molviews.selection().nSelectedAtoms() == 0)
        return ViewsOfMol();
        
    QHash<MolNum,ViewsOfMol>::iterator it = mols.find(molviews.number());
    
    if ( it != mols.end() )
    {
        QList<AtomSelection> added_views = it->addIfUnique(molviews.selections());
        
        if (added_views.isEmpty())
            return ViewsOfMol();
        else
            return ViewsOfMol(molviews.data(), added_views);
    }
    else
    {
        mols.insert( molviews.number(), molviews );
        return molviews;
    }
}

/** Add all of the molecules in 'molecules' to this set. 
    This adds all of the molecules, even if they already
    exist in this set */
void Molecules::add(const Molecules &molecules)
{
    for (QHash<MolNum,ViewsOfMol>::const_iterator it = molecules.mols.constBegin();
         it != molecules.mols.constEnd();
         ++it)
    {
        this->add(it.value());
    }
}

/** Add all of the views of the molecules in 'molecules' to 
    this set, only if they don't already exist in this set.
    This returns the views that were added to this set. */
QList<ViewsOfMol> Molecules::addIfUnique(const Molecules &molecules)
{
    QList<ViewsOfMol> added_mols;
    
    for (QHash<MolNum,ViewsOfMol>::const_iterator it = molecules.mols.constBegin();
         it != molecules.mols.constEnd();
         ++it)
    {
        ViewsOfMol added_views = this->addIfUnique(*it);
        
        if (not added_views.isEmpty())
            added_mols.append(added_views);
    }
    
    return added_mols;
} 

/** Remove the view 'molview' from this set. This only
    removes the first copy of this view from this set
    (if multiple copies are present), and returns
    whether or not a view was removed */
bool Molecules::remove(const MoleculeView &molview)
{
    if (not mols.contains(molview.number()))
        return false;

    QHash<MolNum,ViewsOfMol>::iterator it = mols.find(molview.data().number());
    
    if (it != mols.end())
    {
        bool removed_view = it->remove(molview.selection());
        
        if (removed_view and it->selection().isEmpty())
            mols.remove(it.key());
            
        return removed_view;
    }
}

/** Remove all copies of the view 'view' from this set.
    This returns whether any views were removed */
bool Molecules::removeAll(const MoleculeView &molview)
{
    if (not mols.contains(molview.number()))
        return false;
        
    QHash<MolNum,ViewsOfMol>::iterator it = mols.find(molview.data().number());
    
    if (it != mols.end())
    {
        bool removed_view = it->removeAll(molview.selection());
        
        if (removed_view and it->selection().isEmpty())
            mols.remove(it.key());
            
        return removed_view;
    }
}

/** Remove all of the views in 'molviews' from this set. This
    only removes the first copy of the view if multiple copies
    exist, and returns the views that were successfully removed. */
ViewsOfMol Molecules::remove(const ViewsOfMol &molviews)
{
    if (not mols.contains(molviews.number()))
        return ViewsOfMol();

    QHash<MolNum,ViewsOfMol>::iterator it = mols.find(molviews.number());
    
    if (it != mols.end())
    {
        QList<AtomSelection> removed_views = it->remove(molviews.selections());
        
        if (removed_views.isEmpty())
            return ViewsOfMol();
        
        if (it->selection().isEmpty())
            mols.remove(it.key());
            
        return ViewsOfMol(molviews.data(), removed_views);
    }
}

/** Remove all of the views in 'molviews' from this set. This
    removes all copies of the views if multiple copies exist,
    and returns the views that were successfully removed. */
ViewsOfMol Molecules::removeAll(const ViewsOfMol &molviews)
{
    if (not mols.contains(molviews.number()))
        return ViewsOfMol();

    QHash<MolNum,ViewsOfMol>::iterator it = mols.find(molviews.number());
    
    if (it != mols.end())
    {
        QList<AtomSelection> removed_views = it->removeAll(molviews.selections());
        
        if (removed_views.isEmpty())
            return ViewsOfMol();
        
        if (it->selection().isEmpty())
            mols.remove(it.key());
            
        return ViewsOfMol(molviews.data(), removed_views);
    }
}

/** Remove all of the views of all of the molecules in 'molecules'.
    This only removes the first copy of any views that appear
    multiple times in this set. This returns all of the views that
    were successfully removed */
QList<ViewsOfMol> Molecules::remove(const Molecules &molecules)
{
    if (molecules.isEmpty())
        return molecules;

    QList<ViewsOfMol> removed_mols;

    for (QHash<MolNum,ViewsOfMol>::const_iterator it = molecules.mols.constBegin();
         it != molecules.mols.constEnd();
         ++it)
    {
        ViewsOfMol removed_views = this->remove(*it);
        
        if (not removed_views.isEmpty())
            removed_mols.append(removed_views);
    }
    
    return removed_mols;
}

/** Remove all of the views of all of the molecules in 'molecules'.
    This removes all copies of any views that appear
    multiple times in this set. This returns all of the views that
    were successfully removed */
QList<ViewsOfMol> Molecules::removeAll(const Molecules &molecules)
{
    if (molecules.isEmpty())
        return molecules;

    QList<ViewsOfMol> removed_mols;

    for (QHash<MolNum,ViewsOfMol>::const_iterator it = molecules.mols.constBegin();
         it != molecules.mols.constEnd();
         ++it)
    {
        ViewsOfMol removed_views = this->removeAll(*it);
        
        if (not removed_views.isEmpty())
            removed_mols.append(removed_views);
    }
    
    return removed_mols;
}

/** Remove all views of the molecule with number 'molnum'. This
    returns the views of the molecule in this set, if it exists,
    or an empty set of views if it doesn't exist. */
ViewsOfMol Molecules::remove(MolNum molnum)
{
    return mols.take(molnum);
}

/** Synonym for Molecules::addIfUnique(molview) */
bool Molecules::unite(const MoleculeView &molview)
{
    return this->addIfUnique(molview);
}

/** Synonym for Molecules::addIfUnique(molviews) */
ViewsOfMol Molecules::unite(const ViewsOfMol &molviews)
{
    return this->addIfUnique(molviews);
}

/** Synonym for Molecules::addIfUnique(molecules) */
QList<ViewsOfMol> Molecules::unite(const Molecules &molecules)
{
    return this->addIfUnique(molecules);
}

/** Update the views of the molecule whose data is in 'moldata'
    in this set so that it is at the same version as 'moldata'.
    This does nothing if this molecule is not in this set.
    This returns whether or not the molecule was updated */
bool Molecules::update(const MoleculeData &moldata)
{
    QHash<MolNum,ViewsOfMol>::const_iterator it = mols.constFind(moldata.number());
    
    if (it != mols.end() and *it != moldata)
    {
        mols.find(moldata.number())->update(moldata);
        return true;
    }
    else
        return false;
}

/** Update the views of the molecule viewed by 'molview' 
    in this set so that they have the same molecule version
    as 'molview'. This returns whether or not the molecule
    was updated. */
bool Molecules::update(const MoleculeView &molview)
{
    return this->update(molview.data());
}

/** Update the views in this set so that they have the 
    same molecule versions as the molecules in 'molecules'.
    This returns the molecules that have been updated. */
QList<Molecule> Molecules::update(const Molecules &molecules)
{
    QList<Molecule> updated_mols;
    
    if (this->count() <= molecules.count())
    {
        for (QHash<MolNum,ViewsOfMol>::iterator it = mols.begin();
             it != mols.end();
             ++it)
        {
            QHash<MolNum,ViewsOfMol>::const_iterator 
                                        mol = molecules.mols.find(it.key());
                                        
            if (mol != molecules.mols.constEnd() and 
                mol->data() != it->data())
            {
                //this molecule needs to be updated
                updated_mols.append( Molecule(mol->data()) );
                it->update(mol->data());
            }
        }
    }
    else
    {
        for (Molecules::const_iterator it = molecules.constBegin();
             it != molecules.constEnd();
             ++it)
        {
            QHash<MolNum,ViewsOfMol>::const_iterator
                                        mol = mols.constFind(it.key());
                                        
            if (mol != mols.constEnd() and mol->data() != it->data())
            {
                //this molecule needs to be updated
                updated_mols.append( Molecule(it->data()) );
                
                mols.find(it.key())->update(it->data());
            }
        }
    }

    return updated_mols;
}

/** This removes all duplicated views from this set. This returns  
    whether or not this changes the set (whether or not there
    were any duplicates!) */
bool Molecules::removeDuplicates()
{
    bool changed = false;

    for (QHash<MolNum,ViewsOfMol>::iterator it = mols.begin();
         it != mols.end();
         ++it)
    {
        int n_old_views = it->nViews();
        *it = it->removeDuplicates();
        
        if (not changed)
            changed = n_old_views != it->nViews();
    }
    
    return changed;
}

/** Unite all of the views in this set so that each molecule has
    only a single view that is the union of all of its views.
    Return whether or not this changes the set. */
bool Molecules::uniteViews()
{
    bool changed = false;

    for (QHash<MolNum,ViewsOfMol>::iterator it = mols.begin();
         it != mols.end();
         ++it)
    {
        if (not changed)
            changed = it->nViews() > 1;
    
        *it = it->join();
    }
    
    return changed;
}

/** Construct an empty set of molecules */
Molecules::Molecules()
{}

/** Construct a set that contains only the passed view
    of a molecule */
Molecules::Molecules(const MoleculeView &molecule)
{
    this->add(molecule);
}

/** Construct a set that contains the passed views 
    of a molecule */
Molecules::Molecules(const ViewsOfMol &molviews)
{
    this->add(molviews);
}

/** Copy constructor */
Molecules::Molecules(const Molecules &other)
          : mols(other.mols)
{}

/** Destructor */
Molecules::~Molecules()
{}

/** Copy assignment operator */
Molecules& Molecules::operator=(const Molecules &other)
{
    mols = other.mols;
    return *this;
}
    
/** Comparison operator */
bool Molecules::operator==(const Molecules &other) const
{
    return mols == other.mols;
}

/** Comparison operator */
bool Molecules::operator!=(const Molecules &other) const
{
    return mols != other.mols;
}

/** Return the numbers of all of the molecules in this set */
QSet<MolNum> Molecules::molNums() const
{
    return mols.keys().toSet();
}

/** Assert that this set contains any of the atoms of 
    the molecule with number 'molnum' 
    
    \throw SireMol::missing_molecule
*/
void Molecules::assertContains(MolNum molnum) const
{
    if (not mols.contains(molnum))
        throw SireMol::missing_molecule( QObject::tr(
            "The molecule with number %d is not present in this set "
            "(that contains the molecules with numbers %2).")
                .arg(molnum)
                .arg( Sire::toString(this->molNums()) ), CODELOC );
}

/** Return the view(s) of the molecule that has number 'molnum'

    \throw SireMol::missing_molecule
*/
const ViewsOfMol& Molecules::operator[](MolNum molnum) const
{
    QHash<MolNum,ViewsOfMol>::const_iterator it = mols.find(molnum);
    
    if (it == mols.end())
        throw SireMol::missing_molecule( QObject::tr(
            "The molecule with number %d is not present in this set "
            "(that contains the molecules with numbers %2).")
                .arg(molnum)
                .arg( Sire::toString(this->molNums()) ), CODELOC );
                
    return *it;
}

/** Return the Molecules that has had 'other' added to it. Note
    that if any molecules are in both this and 'other', then
    the version of the molecule in this set will be used 
    in the returned set. */
Molecules Molecules::operator+(const Molecules &other) const
{
    Molecules ret(*this);
    
    ret.add(other);
    
    return ret;
}

/** Return the Molecules once the views in 'other' have been 
    removed */
Molecules Molecules::operator-(const Molecules &other) const
{
    Molecules ret(*this);
    
    ret.remove(other);
    
    return ret;
}

/** Synonym for Molecules::add(other) (except that
    it returns this set, not the added molecules) */
Molecules& Molecules::operator+=(const Molecules &other)
{
    this->add(other);
    return *this;
}

/** Synonym for Molecules::remove(other) (except that
    it returns this set, not the removed molecules) */
Molecules& Molecules::operator-=(const Molecules &other)
{
    this->remove(other);
    return *this;
}

/** Return the molecule at number 'molnum'

    \throw SireMol::missing_molecule
*/
const ViewsOfMol& Molecules::molecule(MolNum molnum) const
{
    return this->operator[](molnum);
}

/** Return whether this set is empty */
bool Molecules::isEmpty() const
{
    return mols.isEmpty();
}

/** Return whether or not this set contains any view
    of the molecule at number 'molnum' */
bool Molecules::contains(MolNum molnum) const
{
    return mols.contains(molnum);
}

/** Return whether or not this set contains the view 'molview'
       - note that this means that it actually contains this
         specific view! */
bool Molecules::contains(const MoleculeView &molview) const
{
    QHash<MolNum,ViewsOfMol>::const_iterator it = mols.find(molview.data().number());
    
    if (it != mols.end())
        return it->contains(molview.selection());
    else
        return false;
}

/** Return whether or not this set contains all of the views 
    in 'molviews' - note that this means that it actually
    contains each specific view! */
bool Molecules::contains(const ViewsOfMol &molviews) const
{
    QHash<MolNum,ViewsOfMol>::const_iterator it = mols.find(molviews.number());
    
    if (it != mols.end())
        return it->contains(molviews.selections());
    else
        return false;
}

/** Return whether or not this set contains all of the views
    of all of the molecules in 'molecules' */
bool Molecules::contains(const Molecules &molecules) const
{
    for (QHash<MolNum,ViewsOfMol>::const_iterator it = molecules.mols.begin();
         it != molecules.mols.end();
         ++it)
    {
        if (not this->contains(it.value()))
            return false;
    }
    
    return true;
}

/** Return whether or not this set intersects with the view 'molview'
     - this returns true if any of the atoms in 'molview' are
       also present in any of the views of that molecule in this set */
bool Molecules::intersects(const MoleculeView &molview) const
{
    QHash<MolNum,ViewsOfMol>::const_iterator it = mols.find(molview.data().number());
    
    if (it != mols.end())
        return it->intersects(molview.selection());
    else
        return false;
}

/** Return whether or not this set intersects with 'molecules' -
    this returns true if any of the atoms in any of the views
    of any of the molecules in 'molecules' are also present in any
    of the views of any of the molecules in this set */
bool Molecules::intersects(const Molecules &molecules) const
{
    for (QHash<MolNum,ViewsOfMol>::const_iterator it = molecules.mols.begin();
         it != molecules.mols.end();
         ++it)
    {
        if (this->intersects(it.value()))
            return true;
    }
    
    return false;
}

/** Return the number of molecules in this set */
int Molecules::count() const
{
    return mols.count();
}

/** Return the number of molecules in this set */
int Molecules::nMolecules() const
{
    return mols.count();
}

/** Return an iterator that points to the first molecule 
    in this set */
Molecules::const_iterator Molecules::begin() const
{
    return mols.begin();
}

/** Return an iterator that points one past the last
    molecule in this set */
Molecules::const_iterator Molecules::end() const
{
    return mols.end();
}

/** Return an iterator that points to the first molecule 
    in this set */
Molecules::const_iterator Molecules::constBegin() const
{
    return mols.begin();
}

/** Return an iterator that points one past the last
    molecule in this set */
Molecules::const_iterator Molecules::constEnd() const
{
    return mols.end();
}
