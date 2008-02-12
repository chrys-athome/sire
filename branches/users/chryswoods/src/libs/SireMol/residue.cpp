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

#include "residue.h"

#include "mover.hpp"
#include "selector.hpp"
#include "evaluator.h"
#include "reseditor.h"

#include "molecule.h"
#include "chain.h"
#include "atom.h"

#include "atomidcombos.h"

#include "SireError/errors.h"
#include "SireBase/errors.h"
#include "SireMol/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMol;
using namespace SireBase;
using namespace SireStream;

RegisterMetaType<Residue> r_res;

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const Residue &res)
{
    writeHeader(ds, r_res, 1);

    SharedDataStream sds(ds);
    
    sds << res.residx << static_cast<const MoleculeView&>(res);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, Residue &res)
{
    VersionID v = readHeader(ds, r_res);

    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> res.residx >> static_cast<MoleculeView&>(res);
        
        res.selected_atoms = AtomSelection(res.data());
        res.selected_atoms.select(res.residx);
    }
    else
        throw version_error(v, "1", r_res, CODELOC);

    return ds;
}

void SireMol::detail::assertSameSize(Residue*, int nats, int nprops)
{
    if (nats != nprops)
        throw SireError::incompatible_error( QObject::tr(
            "The number of supplied properties (%1) is not the same "
            "as the number of residues (%2).")
                .arg(nprops).arg(nats), CODELOC );
}

/** Null constructor */
Residue::Residue() : MoleculeView()
{}

/** Construct as a specified residue */
Residue::Residue(const MoleculeData &moldata, const ResID &resid)
        : MoleculeView(moldata)
{
    residx = moldata.info().resIdx(resid);
    selected_atoms = AtomSelection(moldata);
    selected_atoms.select(residx);
}
        
/** Copy constructor */
Residue::Residue(const Residue &other)
        : MoleculeView(other), residx(other.residx),
          selected_atoms(other.selected_atoms)
{}

/** Destructor */
Residue::~Residue()
{}

/** Copy assignment operator */
Residue& Residue::operator=(const Residue &other)
{
    if (this != &other)
    {
        MoleculeView::operator=(other);
        residx = other.residx;
        selected_atoms = other.selected_atoms;
    }
    
    return *this;
}

/** Comparison operator */
bool Residue::operator==(const Residue &other) const
{
    return residx == other.residx and 
           MoleculeView::operator==(other);
}

/** Comparison operator */
bool Residue::operator!=(const Residue &other) const
{
    return residx != other.residx or
           MoleculeView::operator!=(other);
}

/** Return the identities of the atoms that are selected as
    part of this residue */
AtomSelection Residue::selection() const
{
    return selected_atoms;
}

/** Return the name of this residue */
ResName Residue::name() const
{
    return d->info().name(residx);
}

/** Return the number of this residue */
ResNum Residue::number() const
{
    return d->info().number(residx);
}

/** Return the index of this residue in the molecule */
ResIdx Residue::index() const
{
    return residx;
}

/** Return a Mover that moves all of the atoms in this residue */
Mover<Residue> Residue::move() const
{
    return Mover<Residue>(*this);
}

/** Return an Evaluator that evaluates values using all of 
    the atoms in the residue */
Evaluator Residue::evaluate() const
{
    return Evaluator(*this);
}

/** Return an editor that can be used to edit any of the  
    atoms of this residue */
ResEditor Residue::edit() const
{
    return ResEditor(*this);
}

/** Return a selector that can change the selection of residues */
Selector<Residue> Residue::selector() const
{
    return Selector<Residue>(*this);
}

/** Return the number of atoms in this residue */
int Residue::nAtoms() const
{
    return d->info().nAtoms(residx);
}

/** Return the indicies of the atoms in this residue, in the
    order that they appear in this residue */
const QList<AtomIdx>& Residue::atomIdxs() const
{
    return d->info().getAtomsIn(residx);
}

/** Return whether or not this residue contains the atom 
    at index 'atomidx' */
bool Residue::contains(AtomIdx atomidx) const
{
    return d->info().contains(residx, atomidx);
}

/** Return whether or not this residue contains all of 
    the atoms identified by the ID 'atomid' */
bool Residue::contains(const AtomID &atomid) const
{
    return d->info().contains(residx, atomid);
}

/** Return whether or not this residue contains some of  
    the atoms identified by the ID 'atomid' */
bool Residue::intersects(const AtomID &atomid) const
{
    return d->info().intersects(residx, atomid);
}

/** Return the molecule that contains this residue */
Molecule Residue::molecule() const
{
    return Molecule(*d);
}

/** Return the chain that contain this residue - this throws
    an exception if this residue is not part of a chain
    
    \throw SireMol::missing_chain
*/
Chain Residue::chain() const
{
    ChainIdx chainidx = d->info().parentChain(residx);
    
    if (chainidx.isNull())
        throw SireMol::missing_chain( QObject::tr(
            "The residue %1:%2 (index %3) in the molecule \"%4\" "
            "is not part of a chain!")
                .arg(name()).arg(number()).arg(residx).arg(d->name()),
                    CODELOC );

    return Chain(this->data(), chainidx);
}

/** Return the atom that is in this residue *and* also has the 
    ID 'atomid'
    
    \throw SireMol::missing_atom
    \throw SireMol::dulplicate_atom
    \throw SireError::invalid_index
*/
Atom Residue::atom(const AtomID &atomid) const
{
    return Atom(this->data(), residx + atomid);
}

/** Return the atom(s) that are in this residue *and* also
    match the ID 'atomid'
    
    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
Selector<Atom> Residue::atoms(const AtomID &atomid) const
{
    return Selector<Atom>(this->data(), residx + atomid);
}

/** Return all of the atoms that are in this residue */
Selector<Atom> Residue::atoms() const
{
    return Selector<Atom>(this->data(), residx.atoms());
}

/** Return the atom in this residue *and* has the ID 'atomid'

    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
*/
Atom Residue::select(const AtomID &atomid) const
{
    return this->atom(atomid);
}

/** Return the atoms in this residue *and* having the ID 'atomid'

    \throw SireMol::missing_atom
*/
Selector<Atom> Residue::selectAll(const AtomID &atomid) const
{
    return this->atoms(atomid);
}

/** Return all of the atoms in this residue */
Selector<Atom> Residue::selectAll() const
{
    return this->atoms();
}

/** Return whether or not there is a ResProperty at key 'key' */
bool Residue::hasProperty(const PropertyName &key) const
{
    return d->hasPropertyOfType<ResProp>(key);
}

/** Return whether or not there is a ResProperty at metakey 'metakey' */
bool Residue::hasMetadata(const PropertyName &metakey) const
{
    return d->hasMetadataOfType<ResProp>(metakey);
}

/** Return whether the metadata at metakey 'metakey' for the property
    at key 'key' is a ResProperty
    
    \throw SireBase::missing_property
*/
bool Residue::hasMetadata(const PropertyName &key,
                       const PropertyName &metakey) const
{
    return d->hasMetadataOfType<ResProp>(key, metakey);
}

/** Assert that this residue contains a residue property at key 'key'

    \throw SireBase::missing_property
*/
void Residue::assertContainsProperty(const PropertyName &key) const
{
    if (not this->hasProperty(key))
        throw SireBase::missing_property( QObject::tr(
            "There is no residue property at key \"%1\" for the "
            "residue %2 (%3) in the molecule \"%4\".")
                .arg(key, this->name()).arg(this->number())
                .arg(d->name()), CODELOC );
}

/** Assert that this residue contains some residue metadata at metakey 'metakey'

    \throw SireBase::missing_property
*/
void Residue::assertContainsMetadata(const PropertyName &metakey) const
{
    if (not this->hasMetadata(metakey))
        throw SireBase::missing_property( QObject::tr(
            "There is no residue metadata at metakey \"%1\" for the "
            "residue %2 (%3) in the molecule \"%4\".")
                .arg(metakey, this->name()).arg(this->number())
                .arg(d->name()), CODELOC );
}

/** Assert that this residue contains some residue metadata
    at metakey 'metakey' for the property at key 'key'

    \throw SireBase::missing_property
*/
void Residue::assertContainsMetadata(const PropertyName &key,
                                     const PropertyName &metakey) const
{
    if (not this->hasMetadata(key,metakey))
        throw SireBase::missing_property( QObject::tr(
            "There is no residue metadata at metakey \"%1\" for the "
            "property at key \"%2\" for the "
            "residue %2 (%3) in the molecule \"%4\".")
                .arg(metakey, key, this->name()).arg(this->number())
                .arg(d->name()), CODELOC );
}

/** Return the keys of all ResProperty properties */
QStringList Residue::propertyKeys() const
{
    return d->properties().propertyKeysOfType<ResProp>();
}

/** Return the metakeys of all ResProperty metadata */
QStringList Residue::metadataKeys() const
{
    return d->properties().metadataKeysOfType<ResProp>();
}

/** Return the metakeys of all ResProperty metadata for 
    the property at key 'key'
    
    \throw SireBase::missing_property
*/
QStringList Residue::metadataKeys(const PropertyName &key) const
{
    return d->properties().metadataKeysOfType<ResProp>(key);
}

bool SireMol::detail::has_property(const Residue*, const MoleculeData &moldata,
                          const PropertyName &key)
{
    return moldata.hasPropertyOfType<ResProp>(key);
}

bool SireMol::detail::has_metadata(const Residue*, const MoleculeData &moldata,
                                   const PropertyName &metakey)
{
    return moldata.hasMetadataOfType<ResProp>(metakey);
}

bool SireMol::detail::has_metadata(const Residue*, const MoleculeData &moldata,
                                   const PropertyName &key, const PropertyName &metakey)
{
    return moldata.hasMetadataOfType<ResProp>(key, metakey);
}

/////
///// explicitly instantiate the Residue manipulator classes
/////

template class Mover<Residue>;
template class Selector<Residue>;

template class Mover< Selector<Residue> >;
