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

#include "atom.h"

#include "editor.hpp"
#include "mover.hpp"
#include "selector.hpp"
#include "evaluator.h"

#include "cutgroup.h"
#include "residue.h"
#include "chain.h"
#include "segment.h"
#include "molecule.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMol;
using namespace SireStream;

static const RegisterMetaType<Atom> r_atom;

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const Atom &atom)
{
    writeHeader(ds, r_atom, 1);

    ds << atom.atomidx << static_cast<const MoleculeView&>(atom);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, Atom &atom)
{
    VersionID v = readHeader(ds, r_atom);

    if (v == 1)
    {
        ds >> atom.atomidx >> static_cast<MoleculeView&>(atom);
    }
    else
        throw version_error(v, "1", r_atom, CODELOC);

    return ds;
}

void SireMol::detail::assertSameSize(Atom*, int nats, int nprops)
{
    if (nats != nprops)
        throw SireError::incompatible_error( QObject::tr(
            "The number of supplied properties (%1) is not the same "
            "as the number of atoms (%2).")
                .arg(nprops).arg(nats), CODELOC );
}

/** Null constructor */
Atom::Atom() : MoleculeView(), atomidx( Index::null() )
{}

/** Construct the atom that that is identified by ID 'atomid'
    in the view 'molview' - this atom must be within this view
    
    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
*/
Atom::Atom(const MoleculeView &molview, const AtomID &atomid)
     : MoleculeView(molview)
{
    atomidx = d->info().atomIdx(atomid);
    molview.assertContains(atomidx);
}

/** Construct the atom that is identified by ID 'atomid'
    in the molecule whose data is in 'moldata'
    
    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
*/
Atom::Atom(const MoleculeData &moldata, const AtomID &atomid)
     : MoleculeView(moldata), atomidx( moldata.info().atomIdx(atomid) )
{}

/** Copy constructor */
Atom::Atom(const Atom &other)
     : MoleculeView(other), atomidx(other.atomidx)
{}

/** Destructor */
Atom::~Atom()
{}

/** Copy assignment operator */
Atom& Atom::operator=(const Atom &other)
{
    MoleculeView::operator=(other);
    atomidx = other.atomidx;
    
    return *this;
}

/** Comparison operator */
bool Atom::operator==(const Atom &other) const
{
    return atomidx == other.atomidx and MoleculeView::operator==(other);
}

/** Comparison operator */
bool Atom::operator!=(const Atom &other) const
{
    return atomidx != other.atomidx or MoleculeView::operator!=(other);
}

/** Return the selected atom! */
AtomSelection Atom::selection() const
{
    AtomSelection selected_atoms(data());
    
    selected_atoms.select(atomidx);
    
    return selected_atoms;
}

/** Return the name of the atom */
AtomName Atom::name() const
{
    return d->info().name(atomidx);
}

/** Return the number of the atom */
AtomNum Atom::number() const
{
    return d->info().number(atomidx);
}

/** Return the index number of this atom in the molecule */
AtomIdx Atom::index() const
{
    return atomidx;
}

/** Return a Mover that can be used to move this atom */
Mover<Atom> Atom::move() const
{
    return Mover<Atom>(*this);
}

/** Return an evaluator that can be used to evaluate properties
    of this atom */
Evaluator Atom::evaluate() const
{
    return Evaluator(*this);
}

/** Return an editor that can be used to edit this atom */
Editor<Atom> Atom::edit() const
{
    return Editor<Atom>(*this);
}

/** Return a selector that can change the atom selection */
Selector<Atom> Atom::selector() const
{
    return Selector<Atom>(*this);
}

/** Return the residue that this atom is in 

    \throw SireMol::missing_residue
*/
Residue Atom::residue() const
{
    return Residue(*d, d->info().parentResidue(atomidx));
}

/** Return the chain this atom is in 

    \throw SireMol::missing_chain
*/
Chain Atom::chain() const
{
    return Chain(*d, d->info().parentChain(atomidx));
}

/** Return the segment this atom is in 

    \throw SireMol::missing_segment
*/
Segment Atom::segment() const
{
    return Segment(*d, d->info().parentSegment(atomidx));
}

/** Return the CutGroup this atom is in */
CutGroup Atom::cutGroup() const
{
    return CutGroup(*d, d->info().parentCutGroup(atomidx));
}

/** Return whether or not there is an AtomProperty at key 'key' */
bool Atom::hasProperty(const PropertyName &key) const
{
    return d->hasPropertyOfType<AtomProp>(key);
}

/** Return whether or not there is an AtomProperty at metakey 'metakey' */
bool Atom::hasMetadata(const PropertyName &metakey) const
{
    return d->hasMetadataOfType<AtomProp>(metakey);
}

/** Return whether the metadata at metakey 'metakey' for the property
    at key 'key' is an AtomProperty
    
    \throw SireBase::missing_property
*/
bool Atom::hasMetadata(const PropertyName &key,
                       const PropertyName &metakey) const
{
    return d->hasMetadataOfType<AtomProp>(key, metakey);
}

///////
/////// Explicitly instantiate the Atom manipulator classes
///////

template class Editor<Atom>;
template class Selector<Atom>;
template class Mover<Atom>;

template class Editor< Selector<Atom> >;
template class Mover< Selector<Atom> >;
