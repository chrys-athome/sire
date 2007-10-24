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

void Molecules::_pvt_add(const MoleculeView &molview)
{
    PartialMolecule mol(molview);
    
    if (mol.selection().isEmpty())
        return;

    QHash<MolNum,ViewsOfMol>::iterator it = mols.find(mol.number());

    if ( it != mols.end() )
    {
        *it = it->add(mol.selection());
    }
    else
    {
        mols.insert( mol.number(), mol );
    }
}

void Molecules::_pvt_add(const ViewsOfMol &molviews)
{
    if (molviews.selection().nSelectedAtoms() == 0)
        return;

    QHash<MolNum,ViewsOfMol>::iterator it = mols.find(molviews.number());
    
    if ( it != mols.end() )
    {
        *it = it->add(molviews.selections());
    }
    else
    {
        mols.insert( molviews.number(), molviews );
    }
}

void Molecules::_pvt_add(const Molecules &molecules)
{
    for (QHash<MolNum,ViewsOfMol>::const_iterator it = molecules.mols.constBegin();
         it != molecules.mols.constEnd();
         ++it)
    {
        this->_pvt_add(it.value());
    }
}

void Molecules::_pvt_sub(const MoleculeView &molview)
{
    QHash<MolNum,ViewsOfMol>::iterator it = mols.find(molview.data().number());
    
    if (it != mols.end())
    {
        *it = it->remove(molview.selection());
        
        if (it->selection().isEmpty())
            mols.remove(it.key());
    }
}

void Molecules::_pvt_sub(const ViewsOfMol &molviews)
{
    QHash<MolNum,ViewsOfMol>::iterator it = mols.find(molviews.number());
    
    if (it != mols.end())
    {
        *it = it->remove(molviews.selections());
        
        if (it->selection().isEmpty())
            mols.remove(it.key());
    }
}

void Molecules::_pvt_sub(const Molecules &molecules)
{
    for (QHash<MolNum,ViewsOfMol>::const_iterator it = molecules.mols.constBegin();
         it != molecules.mols.constEnd();
         ++it)
    {
        this->_pvt_sub(it.value());
    }
}

void Molecules::_pvt_unite(const MoleculeView &molview)
{
    QHash<MolNum,ViewsOfMol>::iterator it = mols.find(molview.data().number());
    
    if (it != mols.end())
        *it = it->unite(molview);
    else
    {
        mols.insert(molview.data().number(), molview);
    }
}

void Molecules::_pvt_unite(const ViewsOfMol &molviews)
{
    QHash<MolNum,ViewsOfMol>::iterator it = mols.find(molviews.number());
    
    if (it != mols.end())
    {
        *it = it->unite(molviews);
    }
    else
    {
        mols.insert(molviews.number(), molviews);
    }
}

void Molecules::_pvt_unite(const Molecules &molecules)
{
    for (QHash<MolNum,ViewsOfMol>::const_iterator it = molecules.mols.constBegin();
         it != molecules.mols.constEnd();
         ++it)
    {
        this->_pvt_unite(it.value());
    }
}

void Molecules::_pvt_update(const MoleculeData &moldata)
{
    QHash<MolNum,ViewsOfMol>::iterator it = mols.find(moldata.number());
    
    if (it != mols.end())
        it->update(moldata);
}

void Molecules::_pvt_removeDupes()
{
    for (QHash<MolNum,ViewsOfMol>::iterator it = mols.begin();
         it != mols.end();
         ++it)
    {
        *it = it->removeDuplicates();
    }
}

void Molecules::_pvt_uniteViews()
{
    for (QHash<MolNum,ViewsOfMol>::iterator it = mols.begin();
         it != mols.end();
         ++it)
    {
        *it = it->join();
    }
}

/** Construct an empty set of molecules */
Molecules::Molecules()
{}

/** Construct a set that contains only the passed view
    of a molecule */
Molecules::Molecules(const MoleculeView &molecule)
{
    this->_pvt_add(molecule);
}

/** Construct a set that contains the passed views 
    of a molecule */
Molecules::Molecules(const ViewsOfMol &molviews)
{
    this->_pvt_add(molviews);
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
    
    ret._pvt_add(other);
    
    return ret;
}

/** Return the Molecules once the views in 'other' have been 
    removed */
Molecules Molecules::operator-(const Molecules &other) const
{
    Molecules ret(*this);
    
    ret._pvt_sub(other);
    
    return ret;
}

/** Return the molecule at number 'molnum'

    \throw SireMol::missing_molecule
*/
const ViewsOfMol& Molecules::molecule(MolNum molnum) const
{
    return this->operator[](molnum);
}

/** Add the view of the molecule 'molview' to this set. This
    adds the view as a duplicate if it already exists in this 
    set. Also the version of the molecule that is contained
    in this set is returned if this molecule already exists
    in this set */
Molecules Molecules::add(const MoleculeView &molview) const
{
    Molecules ret(*this);
    ret._pvt_add(molview);
    return ret;
}

/** Add the views of the molecule 'molviews'. This adds
    duplicates of any views that already exist in this set */
Molecules Molecules::add(const ViewsOfMol &molviews) const
{
    Molecules ret(*this);
    ret._pvt_add(molviews);
    return ret;
}

/** Add all of the views contained in 'molecules'. This adds
    duplicates of any views that already exist in this set. */
Molecules Molecules::add(const Molecules &molecules) const
{
    Molecules ret(*this);
    ret._pvt_add(molecules);
    return ret;
}

/** Return the union of this set with passed view of a molecule.
    Unlike the add(..) functions, unite(...) only adds a 
    view if it doesn't already exist in this set */
Molecules Molecules::unite(const MoleculeView &molview) const
{
    Molecules ret(*this);
    ret._pvt_unite(molview);
    return ret;
}

/** Return the union of the this set with the passed views of a molecule.
    Unlike the add(..) functions, unite(...) only adds a 
    view if it doesn't already exist in this set */
Molecules Molecules::unite(const ViewsOfMol &molviews) const
{
    Molecules ret(*this);
    ret._pvt_unite(molviews);
    return ret;
}

/** Return the union of this set with 'other'.
    Unlike the add(..) functions, unite(...) only adds a 
    view if it doesn't already exist in this set */
Molecules Molecules::unite(const Molecules &other) const
{
    Molecules ret(*this);
    ret._pvt_unite(other);
    return ret;
}

/** Return the set with the view 'view' removed. 
    - note that this only removes this
    view if exactly this view is contained in this set - also
    it removes all duplicates of this view as well! */
Molecules Molecules::remove(const MoleculeView &molview) const
{
    Molecules ret(*this);
    ret._pvt_sub(molview);
    return ret;
}

/** Return the set with the views in 'views' removed .
    - this only removes views
    that exactly match in this set, and it removes all
    of the duplicates as well! */
Molecules Molecules::remove(const ViewsOfMol &molviews) const
{
    Molecules ret(*this);
    ret._pvt_sub(molviews);
    return ret;
}

/** Return the set with the molecules in 'molecules' removed.
    This only removes exactly matching views, and it removes
    all duplicates as well! */
Molecules Molecules::remove(const Molecules &molecules) const
{
    Molecules ret(*this);
    ret._pvt_sub(molecules);
    return ret;
}

/** Return the set with all views of the molecule with number 'molnum' 
    completely removed */
Molecules Molecules::remove(MolNum molnum) const
{
    Molecules ret(*this);
    ret.mols.remove(molnum);
    return ret;
}

/** Return the set with the view 'view' removed. 
    - note that this only removes this
    view if exactly this view is contained in this set - also
    it removes all duplicates of this view as well! */
Molecules Molecules::subtract(const MoleculeView &molview) const
{
    return this->remove(molview);
}

/** Return the set with the views in 'views' removed .
    - this only removes views
    that exactly match in this set, and it removes all
    of the duplicates as well! */
Molecules Molecules::subtract(const ViewsOfMol &molviews) const
{
    return this->remove(molviews);
}

/** Return the set with the molecules in 'molecules' removed.
    This only removes exactly matching views, and it removes
    all duplicates as well! */
Molecules Molecules::subtract(const Molecules &molecules) const
{
    return this->remove(molecules);
}

/** Return the set with all views of the molecule with number 'molnum' 
    completely removed */
Molecules Molecules::subtract(MolNum molnum) const
{
    return this->remove(molnum);
}

/** Return the set where all of the views of each molecule
    have been joined together so that there is now only
    a single view of each molecule in the set */
Molecules Molecules::uniteViews() const
{
    Molecules ret(*this);
    ret._pvt_uniteViews();
    return ret;
}

/** Return the set where the data for the molecule is 
    updated to 'moldata' */
Molecules Molecules::update(const MoleculeData &moldata) const
{
    Molecules ret(*this);
    ret._pvt_update(moldata);
    return ret;
}

/** Return the set where the data for the molecule is
    updated to be the same as the molecule viewed in 'molview' */
Molecules Molecules::update(const MoleculeView &molview) const
{
    return this->update(molview.data());
}

/** Return the set where the data for all of the molecules is
    updated to the same version as the molecule in 'molecules' */
Molecules Molecules::update(const Molecules &molecules) const
{
    Molecules ret(*this);
    
    for (QHash<MolNum,ViewsOfMol>::const_iterator it = molecules.mols.begin();
         it != molecules.mols.end();
         ++it)
    {
        ret._pvt_update(it->data());
    }
    
    return ret;
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
