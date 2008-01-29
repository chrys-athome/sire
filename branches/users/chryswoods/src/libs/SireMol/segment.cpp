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

#include "segment.h"

#include "atom.h"
#include "molecule.h"

#include "mover.hpp"
#include "selector.hpp"
#include "evaluator.h"
#include "segeditor.h"

#include "groupatomids.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMol;
using namespace SireStream;

RegisterMetaType<Segment> r_seg;

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const Segment &seg)
{
    writeHeader(ds, r_seg, 1);

    SharedDataStream sds(ds);
    
    sds << seg.segidx << static_cast<const MoleculeView&>(seg);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, Segment &seg)
{
    VersionID v = readHeader(ds, r_seg);

    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> seg.segidx >> static_cast<MoleculeView&>(seg);
        
        seg.selected_atoms = AtomSelection(seg.data());
        seg.selected_atoms.select(seg.segidx);
    }
    else
        throw version_error(v, "1", r_seg, CODELOC);

    return ds;
}

/** Null constructor */
Segment::Segment() : MoleculeView(), segidx( SegIdx::null() )
{}

/** Construct the Segment at ID 'cgid' in the molecule whose data
    is in 'moldata'
    
    \throw SireMol::missing_Segment
    \throw SireMol::duplicate_Segment
    \throw SireError::invalid_index
*/
Segment::Segment(const MoleculeData &moldata, const SegID &segid)
      : MoleculeView(moldata), segidx( moldata.info().segIdx(segid) )
{
    selected_atoms = AtomSelection(moldata);
    selected_atoms.select(segidx);
}

/** Copy constructor */
Segment::Segment(const Segment &other)
        : MoleculeView(other), segidx(other.segidx),
          selected_atoms(other.selected_atoms)
{}

/** Destructor */
Segment::~Segment()
{}

/** Copy assignment operator */
Segment& Segment::operator=(const Segment &other)
{
    MoleculeView::operator=(other);
    segidx = other.segidx;
    selected_atoms = other.selected_atoms;
    return *this;
}

/** Comparison operator */
bool Segment::operator==(const Segment &other) const
{
    return segidx == other.segidx and 
           MoleculeView::operator==(other);
}

/** Comparison operator */
bool Segment::operator!=(const Segment &other) const
{
    return segidx != other.segidx or
           MoleculeView::operator!=(other);
}

/** Return the atoms that are in this Segment */
AtomSelection Segment::selection() const
{
    return selected_atoms;
}

/** Return the name of this Segment */
const SegName& Segment::name() const
{
    return d->info().name(segidx);
}

/** Return the index of this Segment in the molecule */
SegIdx Segment::index() const
{
    return segidx;
}

/** Return an object that can move a copy of this Segment */
Mover<Segment> Segment::move() const
{
    return Mover<Segment>(*this);
}

/** Return an evaluator that can evaluate properties 
    of this Segment */
Evaluator Segment::evaluate() const
{
    return Evaluator(*this);
}

/** Return an editor that can edit this Segment */
SegEditor Segment::edit() const
{
    return SegEditor(*this);
}

/** Return a selector that can be used to change the selection
    of segments from the molecule */
Selector<Segment> Segment::selector() const
{
    return Selector<Segment>(*this);
}

/** Return the number of atoms in this Segment */
int Segment::nAtoms() const
{
    return d->info().nAtoms(segidx);
}

/** Return the indicies of the atoms in this segment, in the
    order that they appear in this segment */
const QList<AtomIdx>& Segment::atomIdxs() const
{
    return d->info().getAtomsIn(segidx);
}

/** Return whether or not this segment contains the atom 
    at index 'atomidx' */
bool Segment::contains(AtomIdx atomidx) const
{
    return d->info().contains(segidx, atomidx);
}

/** Return whether or not this segment contains all of 
    the atoms identified by the ID 'atomid' */
bool Segment::contains(const AtomID &atomid) const
{
    return d->info().contains(segidx, atomid);
}

/** Return whether or not this segment contains some of  
    the atoms identified by the ID 'atomid' */
bool Segment::intersects(const AtomID &atomid) const
{
    return d->info().intersects(segidx, atomid);
}

/** Return the molecule that contains this Segment */
Molecule Segment::molecule() const
{
    return Molecule(this->data());
}

/** Return the atom in this Segment that also has ID 'atomid'

    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
*/
Atom Segment::atom(const AtomID &atomid) const
{
    return Atom( this->data(), segidx + atomid );
}

/** Return the atoms in this Segment that also have the ID 'atomid'

    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
Selector<Atom> Segment::atoms(const AtomID &atomid) const
{
    return Selector<Atom>( this->data(), segidx + atomid );
}

/** Return all of the atoms in this Segment */
Selector<Atom> Segment::atoms() const
{
    return Selector<Atom>( this->data(), segidx.atoms() );
}

/** Return the atom in this Segment that also has ID 'atomid'

    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
*/
Atom Segment::select(const AtomID &atomid) const
{
    return this->atom(atomid);
}

/** Return the atoms in this Segment that also have the ID 'atomid'

    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
Selector<Atom> Segment::selectAll(const AtomID &atomid) const
{
    return this->atoms(atomid);
}

/** Return all of the atoms in this Segment */
Selector<Atom> Segment::selectAll() const
{
    return this->atoms();
}

/** Return whether or not there is a SegProperty at key 'key' */
bool Segment::hasProperty(const PropertyName &key) const
{
    return d->hasPropertyOfType<SegProp>(key);
}

/** Return whether or not there is a SegProperty at metakey 'metakey' */
bool Segment::hasMetadata(const PropertyName &metakey) const
{
    return d->hasMetadataOfType<SegProp>(metakey);
}

/** Return whether the metadata at metakey 'metakey' for the property
    at key 'key' is a SegProperty
    
    \throw SireBase::missing_property
*/
bool Segment::hasMetadata(const PropertyName &key,
                       const PropertyName &metakey) const
{
    return d->hasMetadataOfType<SegProp>(key, metakey);
}

bool SireMol::detail::has_property(const Segment*, const MoleculeData &moldata,
                          const PropertyName &key)
{
    return moldata.hasPropertyOfType<SegProp>(key);
}

bool SireMol::detail::has_metadata(const Segment*, const MoleculeData &moldata,
                                   const PropertyName &metakey)
{
    return moldata.hasMetadataOfType<SegProp>(metakey);
}

bool SireMol::detail::has_metadata(const Segment*, const MoleculeData &moldata,
                                   const PropertyName &key, const PropertyName &metakey)
{
    return moldata.hasMetadataOfType<SegProp>(key, metakey);
}

/////// explicitly instantiate the templates
template class Mover<Segment>;
template class Selector<Segment>;

template class Mover< Selector<Segment> >;
