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

#include "viewsofmol.h"
#include "partialmolecule.h"

#include "editor.hpp"
#include "mover.hpp"
#include "selector.hpp"

#include "molecule.h"
#include "segment.h"
#include "chain.h"
#include "residue.h"
#include "cutgroup.h"
#include "atom.h"

#include "SireError/errors.h"
#include "SireMol/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMol;
using namespace SireStream;

RegisterMetaType<ViewsOfMol> r_molviews;

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, 
                                       const ViewsOfMol &molviews)
{
    writeHeader(ds, r_molviews, 1);

    SharedDataStream sds(ds);
    
    sds << molviews.selected_atoms << molviews.views
        << static_cast<const MoleculeView&>(molviews);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, 
                                       ViewsOfMol &molviews)
{
    VersionID v = readHeader(ds, r_molviews);

    if (v == 1)
    {
        SharedDataStream sds(ds);
        sds >> molviews.selected_atoms >> molviews.views
            >> static_cast<MoleculeView&>(molviews);
    }
    else
        throw version_error(v, "1", r_molviews, CODELOC);

    return ds;
}

/** Null constructor */
ViewsOfMol::ViewsOfMol() : MoleculeView()
{}

/** Construct an empty view of the molecule whose data
    is in 'moldata' */
ViewsOfMol::ViewsOfMol(const MoleculeData &moldata)
           : MoleculeView(moldata), selected_atoms(moldata)
{}

/** Construct the views of the passed molecule */
ViewsOfMol::ViewsOfMol(const MoleculeData &moldata,
                       const QList<AtomSelection> &molviews)
           : MoleculeView(moldata)
{
    if (molviews.isEmpty())
    {
        selected_atoms = AtomSelection(moldata);
    }
    else if (molviews.count() == 1)
    {
        selected_atoms = molviews.first();
        selected_atoms.assertCompatibleWith(moldata);
    }
    else
    {
        views = molviews;
        views.at(0).assertCompatibleWith(moldata);
        
        selected_atoms = AtomSelection::unite(views);
    }
}

/** Construct just a single view of a molecule */
ViewsOfMol::ViewsOfMol(const MoleculeView &view)
           : MoleculeView(view), selected_atoms(view.selectedAtoms())
{}

/** Set this equal to the multiple views held in 'selection' */
template<class T>
void ViewsOfMol::setEqualTo(const Selector<T> &selection)
{
    MoleculeView::operator=(selection);
    selected_atoms = AtomSelection(this->data());
    views.clear();

    int nviews = selection.count();
    
    if (nviews == 1)
    { 
        selected_atoms = selection.at(0).selectedAtoms();
    }
    else if (nviews > 1)
    {
        for (int i=0; i<nviews; ++i)
        {
            views.append( selection.at(i).selectedAtoms() );
        }
        
        selected_atoms = AtomSelection::unite(views);
    }
}

/** Construct from the set of atoms */
ViewsOfMol::ViewsOfMol(const Selector<Atom> &atoms)
           : MoleculeView()
{
    this->setEqualTo(atoms);
}

/** Construct from the set of CutGroups */
ViewsOfMol::ViewsOfMol(const Selector<CutGroup> &cgroups)
           : MoleculeView()
{
    this->setEqualTo(cgroups);
}

/** Construct from the set of residues */
ViewsOfMol::ViewsOfMol(const Selector<Residue> &residues)
           : MoleculeView()
{
    this->setEqualTo(residues);
}

/** Construct from the set of chains */
ViewsOfMol::ViewsOfMol(const Selector<Chain> &chains)
           : MoleculeView()
{
    this->setEqualTo(chains);
}

/** Construct from the set of segments */
ViewsOfMol::ViewsOfMol(const Selector<Segment> &segments)
           : MoleculeView()
{
    this->setEqualTo(segments);
}

/** Copy constructor */
ViewsOfMol::ViewsOfMol(const ViewsOfMol &other)
           : MoleculeView(other), selected_atoms(other.selected_atoms),
             views(other.views)
{}

/** Destructor */
ViewsOfMol::~ViewsOfMol()
{}

/** Copy assignment operator */
ViewsOfMol& ViewsOfMol::operator=(const ViewsOfMol &other)
{
    MoleculeView::operator=(other);
    selected_atoms = other.selected_atoms;
    views = other.views;
    
    return *this;
}

/** Copy assignment operator */
ViewsOfMol& ViewsOfMol::operator=(const MoleculeView &view)
{
    MoleculeView::operator=(view);
    selected_atoms = view.selectedAtoms();
    views.clear();
    
    return *this;
}

/** Copy assignment operator from a set of atoms */
ViewsOfMol& ViewsOfMol::operator=(const Selector<Atom> &atoms)
{
    this->setEqualTo(atoms);
    return *this;
}

/** Copy assignment operator from a set of CutGroups */
ViewsOfMol& ViewsOfMol::operator=(const Selector<CutGroup> &cgroups)
{
    this->setEqualTo(cgroups);
    return *this;
}

/** Copy assignment operator from a set of residues */
ViewsOfMol& ViewsOfMol::operator=(const Selector<Residue> &residues)
{
    this->setEqualTo(residues);
    return *this;
}

/** Copy assignment operator from a set of chains */
ViewsOfMol& ViewsOfMol::operator=(const Selector<Chain> &chains)
{
    this->setEqualTo(chains);
    return *this;
}

/** Copy assignment operator from a set of segments */
ViewsOfMol& ViewsOfMol::operator=(const Selector<Segment> &segments)
{
    this->setEqualTo(segments);
    return *this;
}

/** Comparison operator */
bool ViewsOfMol::operator==(const ViewsOfMol &other) const
{
    return MoleculeView::operator==(other) and
           selected_atoms == other.selected_atoms and
           views == other.views;
}

/** Comparison operator */
bool ViewsOfMol::operator!=(const ViewsOfMol &other) const
{
    return MoleculeView::operator!=(other) or
           selected_atoms != other.selected_atoms or
           views != other.views;
}

/** Return the number of views in this set */
int ViewsOfMol::nViews() const
{
    if (views.isEmpty())
    {
        if (selected_atoms.selectedNone())
            return 0;
        else
            return 1;
    }
    else
        return views.count();
}

/** Return the number of views in this set */
int ViewsOfMol::count() const
{
    return this->nViews();
}

/** Return the ith view in this set

    \throw SireError::invalid_index
*/
PartialMolecule ViewsOfMol::operator[](int i) const
{
    i = Index(i).map( this->nViews() );
    
    if ( i == 0 and views.isEmpty() )
        return PartialMolecule(*d, selected_atoms);
    else
        return PartialMolecule(*d, views.at(i));
}

/** Return the ith view in this set

    \throw SireError::invalid_index
*/
PartialMolecule ViewsOfMol::at(int i) const
{
    return this->operator[](i);
}

/** Return the views where 'view' has been added to the set 

    \throw SireError::incompatible_error
*/
ViewsOfMol ViewsOfMol::add(const AtomSelection &view) const
{
    int nviews = this->nViews();
    
    if (nviews == 0)
        return ViewsOfMol(*d, view);
    else if (nviews == 1)
    {
        view.assertCompatibleWith(*d);
    
        ViewsOfMol ret(*this);
        ret.views.append(selected_atoms);
        ret.views.append(view);
        
        if (not selected_atoms.selectedAll())
            ret.selected_atoms = selected_atoms.unite(view);
        
        return ret;
    }
    else
    {
        view.assertCompatibleWith(*d);
        
        ViewsOfMol ret(*this);
        ret.views.append(view);
        
        if (not selected_atoms.selectedAll())
            ret.selected_atoms = selected_atoms.unite(view);
            
        return ret;
    }
}

/** Return the set with the ith view removed

    \throw SireError::invalid_index
*/
ViewsOfMol ViewsOfMol::removeAt(int i) const
{
    i = Index(i).map( this->nViews() );
    
    if (views.count() == 0)
        return ViewsOfMol(*d);
        
    else
    {
        ViewsOfMol ret(*this);
        ret.views.removeAt(i);
        
        if (ret.views.isEmpty())
            return ViewsOfMol(*this);
        else if (ret.views.count() == 1)
        {
            ret.selected_atoms = ret.views.at(0);
            ret.views.clear();
            return ret;
        }
        else
        {
            ret.selected_atoms = AtomSelection::unite(ret.views);
            return ret;
        }
    }
}

/** Return the views where 'view' has been removed. This only
    removes the first copy of this view if it can be found in this set
    
    \throw SireError::incompatible_error
*/
ViewsOfMol ViewsOfMol::remove(const AtomSelection &view) const
{
    if (not selected_atoms.contains(view))
        return *this;
        
    if (views.count() == 0)
    {
        if (selected_atoms == view)
            return ViewsOfMol(*d);
        else
            return *this;
    }
    else
    {
        int i = views.indexOf(view);
        
        if (i >= 0)
            return this->removeAt(i);
        else
            return *this;
    }
}

/** Return the views where all copies of 'view' have been removed.

    \throw SireError::incompatible_error
*/
ViewsOfMol ViewsOfMol::removeAll(const AtomSelection &view) const
{
    if (not selected_atoms.contains(view))
        return *this;
    
    if (views.isEmpty())
    {
        if (selected_atoms == view)
            return ViewsOfMol(*d);
        else
            return *this;
    }
    else
    {
        ViewsOfMol ret(*this);
        
        if (ret.views.removeAll(view) > 0)
        {
            if (ret.views.isEmpty())
                return ViewsOfMol(*d);
            
            else if (ret.views.count() == 1)
            {
                ret.selected_atoms = ret.views.at(0);
                ret.views.clear();
            }
            else
            {
                ret.selected_atoms = AtomSelection::unite(ret.views);
            }
        }
        
        return ret;
    }
}

/** Return the set with all views removed */
ViewsOfMol ViewsOfMol::removeAll() const
{
    return ViewsOfMol(*d);
}

/** Return the molecule that is the union of all of the
    views in this set */
PartialMolecule ViewsOfMol::join() const
{
    return PartialMolecule(*d, selected_atoms);
}

/** Return the molecule that is the union of all of the
    views in this set */
PartialMolecule ViewsOfMol::all() const
{
    return this->join();
}

/** Return the molecule that contains these views */
Molecule ViewsOfMol::molecule() const
{
    return Molecule(*d);
}

/** Return the mover that can move all of the atoms 
    in all of the views */
Mover<ViewsOfMol> ViewsOfMol::move() const
{
    return Mover<ViewsOfMol>(*this);
}

/** Return all of the atoms selected across all of the 
    views in this set */
AtomSelection ViewsOfMol::selectedAtoms() const
{
    return selected_atoms;
}

/** Return the atoms selected in the ith view in this set
    
    \throw SireError:invalid_index
*/
AtomSelection ViewsOfMol::selectedAtoms(int i) const
{
    i = Index(i).map( this->nViews() );
    
    if (views.isEmpty())
        return selected_atoms;
    else
        return views.at(i);
}

/** Return the mover that can move all of the atoms
    in the ith view in this set 
    
    \throw SireError::invalid_index
*/
Mover<ViewsOfMol> ViewsOfMol::move(int i) const
{
    return Mover<ViewsOfMol>(*this, selectedAtoms(i));
}

/** Return an editor that can edit all of the atoms 
    in the views */
Editor<ViewsOfMol> ViewsOfMol::edit() const
{
    return Editor<ViewsOfMol>(*this);
}

/** Return an editor that can edit all of the
    atoms in the ith view of this set
    
    \throw SireError::invalid_index
*/
Editor<ViewsOfMol> ViewsOfMol::edit(int i) const
{
    return Editor<ViewsOfMol>(*this, selectedAtoms(i));
}

/** Return an evaluator that can evaluate properties
    over all of the atoms in the views */
Evaluator ViewsOfMol::evaluate() const
{
    return Evaluator(*this);
}

/** Return an evaluator that can evaluate properties
    over the atoms in the ith view of this set 
    
    \throw SireError::invalid_index
*/
Evaluator ViewsOfMol::evaluate(int i) const
{
    return Evaluator(*this, selectedAtoms(i));
}

/** Return whether or not any of the views contains
    the atom at index 'atomidx' 
    
    \throw SireError::invalid_index
*/
bool ViewsOfMol::contains(AtomIdx atomidx) const
{
    return selected_atoms.contains(atomidx);
}

/** Return whether or not the views between them contain
    all of the atoms identified by 'atomid'
    
    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
bool ViewsOfMol::contains(const AtomID &atomid) const
{
    return selected_atoms.contains(atomid);
}

/** Return whether or not any of the views contains any
    of the atoms identified by 'atomid'
    
    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
bool ViewsOfMol::intersects(const AtomID &atomid) const
{
    return selected_atoms.intersects(atomid);
}

/** Return the atom from any of the views in this set
    identified by 'atomid'
    
    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
*/
Atom ViewsOfMol::select(const AtomID &atomid) const
{
    selected_atoms.assertContains(atomid);
    return Atom(*d, atomid);
}

/** Return all of the atoms from the views in this set
    that match the ID 'atomid'
    
    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
Selector<Atom> ViewsOfMol::selectAll(const AtomID &atomid) const
{
    AtomSelection atms = selected_atoms.intersect(atomid);
    
    if (atms.isEmpty())
        throw SireMol::missing_atom( QObject::tr(
            "There are no atoms that match %1 in the views of the "
            "molecule \"%2\".")
                .arg(atomid.toString(), d->name()), CODELOC );
                
    return Selector<Atom>(*d, atms);
}

/** Return all of the atoms that are selected by any of the 
    views in this molecule */
Selector<Atom> ViewsOfMol::selectAll() const
{
    return Selector<Atom>(*d, selected_atoms);
}

/** Return the atom in this set identified by the ID 'atomid'

    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
*/
Atom ViewsOfMol::atom(const AtomID &atomid) const
{
    return this->select(atomid);
}

/** Return the atoms in these views that match the ID 'atomid'

    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
Selector<Atom> ViewsOfMol::atoms(const AtomID &atomid) const
{
    return this->selectAll(atomid);
}

/** Return all of the atoms in the view in this set */
Selector<Atom> ViewsOfMol::atoms() const
{
    return this->selectAll();
}

/** Return the indicies of all of the atoms contained in  
    these views */
QList<AtomIdx> ViewsOfMol::atomIdxs() const
{
    return selected_atoms.selectedAtoms().toList();
}

////////
//////// Explicitly instantiate templates
////////

template class Mover<ViewsOfMol>;
template class Editor<ViewsOfMol>;
