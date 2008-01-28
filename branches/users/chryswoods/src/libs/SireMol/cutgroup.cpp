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

#include "cutgroup.h"

#include "atom.h"
#include "molecule.h"

#include "editor.hpp"
#include "mover.hpp"
#include "selector.hpp"
#include "evaluator.h"

#include "cgatomidx.h"
#include "groupatomids.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMol;
using namespace SireStream;

RegisterMetaType<CutGroup> r_cg;

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const CutGroup &cg)
{
    writeHeader(ds, r_cg, 1);

    SharedDataStream sds(ds);
    
    sds << cg.cgidx << static_cast<const MoleculeView&>(cg);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, CutGroup &cg)
{
    VersionID v = readHeader(ds, r_cg);

    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> cg.cgidx >> static_cast<MoleculeView&>(cg);
    }
    else
        throw version_error(v, "1", r_cg, CODELOC);

    return ds;
}

/** Null constructor */
CutGroup::CutGroup() : MoleculeView(), cgidx( CGIdx::null() )
{}

/** Construct the CutGroup at ID 'cgid' in the molecule whose data
    is in 'moldata'
    
    \throw SireMol::missing_CutGroup
    \throw SireMol::duplicate_CutGroup
    \throw SireError::invalid_index
*/
CutGroup::CutGroup(const MoleculeData &moldata, const CGID &cgid)
         : MoleculeView(moldata), cgidx( moldata.info().cgIdx(cgid) )
{}

/** Copy constructor */
CutGroup::CutGroup(const CutGroup &other)
         : MoleculeView(other), cgidx(other.cgidx)
{}

/** Destructor */
CutGroup::~CutGroup()
{}

/** Copy assignment operator */
CutGroup& CutGroup::operator=(const CutGroup &other)
{
    MoleculeView::operator=(other);
    cgidx = other.cgidx;
    return *this;
}

/** Comparison operator */
bool CutGroup::operator==(const CutGroup &other) const
{
    return cgidx == other.cgidx and 
           MoleculeView::operator==(other);
}

/** Comparison operator */
bool CutGroup::operator!=(const CutGroup &other) const
{
    return cgidx != other.cgidx or
           MoleculeView::operator!=(other);
}

/** Return the atoms that are in this CutGroup */
AtomSelection CutGroup::selection() const
{
    AtomSelection selected_atoms(this->data());
    selected_atoms.select(cgidx);
    return selected_atoms;
}

/** Return the name of this CutGroup */
const CGName& CutGroup::name() const
{
    return d->info().name(cgidx);
}

/** Return the index of this CutGroup in the molecule */
CGIdx CutGroup::index() const
{
    return cgidx;
}

/** Return an object that can move a copy of this CutGroup */
Mover<CutGroup> CutGroup::move() const
{
    return Mover<CutGroup>(*this);
}

/** Return an evaluator that can evaluate properties 
    of this CutGroup */
Evaluator CutGroup::evaluate() const
{
    return Evaluator(*this);
}

/** Return an editor that can edit this CutGroup */
Editor<CutGroup> CutGroup::edit() const
{
    return Editor<CutGroup>(*this);
}

/** Return a selector that can change the selection of CutGroups */
Selector<CutGroup> CutGroup::selector() const
{
    return Selector<CutGroup>(*this);
}

/** Return the number of atoms in this CutGroup */
int CutGroup::nAtoms() const
{
    return d->info().nAtoms(cgidx);
}

/** Return the indicies of the atoms in this CutGroup, in the
    order they appear in this CutGroup */
const QList<AtomIdx>& CutGroup::atomIdxs() const
{
    return d->info().getAtomsIn(cgidx);
}

/** Return whether or not this CutGroup contains the atom 
    at index 'atomidx' in the molecule */
bool CutGroup::contains(AtomIdx atomidx) const
{
    return d->info().contains(cgidx, atomidx);
}

/** Return whether or not this CutGroup contains all of 
    the atoms that match the ID 'atomid' */
bool CutGroup::contains(const AtomID &atomid) const
{
    return d->info().contains(cgidx, atomid);
}

/** Return whether or not this CutGroup contains some of
    the atoms that match the ID 'atomid' */
bool CutGroup::intersects(const AtomID &atomid) const
{
    return d->info().intersects(cgidx, atomid);
}

/** Return the molecule that contains this CutGroup */
Molecule CutGroup::molecule() const
{
    return Molecule(this->data());
}

/** Return the atom in this CutGroup that also has ID 'atomid'

    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
*/
Atom CutGroup::atom(const AtomID &atomid) const
{
    return Atom( this->data(), cgidx + atomid );
}

/** Return the atoms in this CutGroup that also have the ID 'atomid'

    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
Selector<Atom> CutGroup::atoms(const AtomID &atomid) const
{
    return Selector<Atom>( this->data(), cgidx + atomid );
}

/** Return all of the atoms in this CutGroup */
Selector<Atom> CutGroup::atoms() const
{
    return Selector<Atom>( this->data(), cgidx.atoms() );
}

/** Return the atom in this CutGroup that also has ID 'atomid'

    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
*/
Atom CutGroup::select(const AtomID &atomid) const
{
    return this->atom(atomid);
}

/** Return the atoms in this CutGroup that also have the ID 'atomid'

    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
Selector<Atom> CutGroup::selectAll(const AtomID &atomid) const
{
    return this->atoms(atomid);
}

/** Return all of the atoms in this CutGroup */
Selector<Atom> CutGroup::selectAll() const
{
    return this->atoms();
}

/** Return whether or not there is a CGProperty at key 'key' */
bool CutGroup::hasProperty(const PropertyName &key) const
{
    return d->hasPropertyOfType<CGProp>(key);
}

/** Return whether or not there is a CGProperty at metakey 'metakey' */
bool CutGroup::hasMetadata(const PropertyName &metakey) const
{
    return d->hasMetadataOfType<CGProp>(metakey);
}

/** Return whether the metadata at metakey 'metakey' for the property
    at key 'key' is a CGProperty
    
    \throw SireBase::missing_property
*/
bool CutGroup::hasMetadata(const PropertyName &key,
                       const PropertyName &metakey) const
{
    return d->hasMetadataOfType<CGProp>(key, metakey);
}

bool detail::has_property(const CutGroup*, const MoleculeData &moldata,
                          const PropertyName &key)
{
    return moldata.hasPropertyOfType<CGProp>(key);
}

bool detail::has_metadata(const CutGroup*, const MoleculeData &moldata,
                          const PropertyName &metakey)
{
    return moldata.hasMetadataOfType<CGProp>(metakey);
}

bool detail::has_metadata(const CutGroup*, const MoleculeData &moldata,
                          const PropertyName &key, const PropertyName &metakey)
{
    return moldata.hasMetadataOfType<CGProp>(key, metakey);
}

////// explicitly instantiate the CutGroup templates
template class Editor<CutGroup>;
template class Mover<CutGroup>;
template class Selector<CutGroup>;

template class Editor< Selector<CutGroup> >;
template class Mover< Selector<CutGroup> >;
