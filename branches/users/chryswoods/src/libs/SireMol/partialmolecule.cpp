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

#include "partialmolecule.h"
#include "molecule.h"
#include "atom.h"

#include "evaluator.h"

#include "mover.hpp"
#include "selector.hpp"
#include "editor.hpp"

#include "SireError/errors.h"
#include "SireMol/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include "tostring.h"

using namespace SireMol;
using namespace SireBase;
using namespace SireStream;

RegisterMetaType<PartialMolecule> r_partialmol;

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, 
                                       const PartialMolecule &partialmol)
{
    writeHeader(ds, r_partialmol, 1);

    SharedDataStream sds(ds);
    sds << partialmol.selected_atoms
        << static_cast<const MoleculeView&>(partialmol);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, 
                                       PartialMolecule &partialmol)
{
    VersionID v = readHeader(ds, r_partialmol);

    if (v == 1)
    {
        SharedDataStream sds(ds);
        sds >> partialmol.selected_atoms
            >> static_cast<MoleculeView&>(partialmol);
    }
    else
        throw version_error(v, "1", r_partialmol, CODELOC);

    return ds;
}

/** Null constructor */
PartialMolecule::PartialMolecule() : MoleculeView()
{}

/** Construct from the passed view */
PartialMolecule::PartialMolecule(const MoleculeView &molview)
                : MoleculeView(molview),
                  selected_atoms(molview.selection())
{}

/** Construct from the selected atoms of the passed molecule whose
    data is in 'moldata' */
PartialMolecule::PartialMolecule(const MoleculeData &moldata,
                                 const AtomSelection &atoms)
                : MoleculeView(moldata),
                  selected_atoms(atoms)
{
    atoms.assertCompatibleWith(moldata);
}

/** Copy constructor */
PartialMolecule::PartialMolecule(const PartialMolecule &other)
                : MoleculeView(other),
                  selected_atoms(other.selected_atoms)
{}           

/** Destructor */
PartialMolecule::~PartialMolecule()
{}

/** Copy assignment operator */
PartialMolecule& PartialMolecule::operator=(const MoleculeView &other)
{
    MoleculeView::operator=(other);
    selected_atoms = other.selection();
    return *this;
}

/** Copy assignment operator */
PartialMolecule& PartialMolecule::operator=(const PartialMolecule &other)
{
    MoleculeView::operator=(other);
    selected_atoms = other.selected_atoms;
    return *this;
}

/** Comparison operator */
bool PartialMolecule::operator==(const PartialMolecule &other) const
{
    return MoleculeView::operator==(other) and
           selected_atoms == other.selected_atoms;
}

/** Comparison operator */
bool PartialMolecule::operator!=(const PartialMolecule &other) const
{
    return MoleculeView::operator!=(other) or
           selected_atoms != other.selected_atoms;
}

/** Return the name of this molecule */
const MolName& PartialMolecule::name() const
{
    return d->name();
}

/** Return the identifying number of this molecule */
MolNum PartialMolecule::number() const
{
    return d->number();
}
 
/** Return the version number of this molecule - all molecules
    with the same ID number and version number must be identical */
quint64 PartialMolecule::version() const
{
    return d->version();
}
 
/** Return the version number of the property at key 'key'.
    All molecules with the same ID number and same property version
    number must have the same value of this property
    (although this says nothing about any metadata associated
    with this property)
    
    \throw SireBase::missing_property 
*/
quint64 PartialMolecule::version(const PropertyName &key) const
{
    return d->version(key);
}

/** Return the number of atoms in this view */
int PartialMolecule::nAtoms() const
{
    return selected_atoms.nSelected();
}

/** Return a mover that can move all of the atoms in this view */
Mover<PartialMolecule> PartialMolecule::move() const
{
    return Mover<PartialMolecule>(*this);
}

/** Return an evaluator that can evaluate properties 
    over all of the atoms in this view */
Evaluator PartialMolecule::evaluate() const
{
    return Evaluator(*this);
}

/** Return the atoms that are part of this view */
AtomSelection PartialMolecule::selection() const
{
    return selected_atoms;
}

/** Return the molecule that contains this view */
Molecule PartialMolecule::molecule() const
{
    return Molecule(*d);
}

/** Select the atom in this view at index 'atomidx' 

    \throw SireError::invalid_index
*/
Atom PartialMolecule::select(AtomIdx atomidx) const
{
    selected_atoms.assertSelected(atomidx);
    return Atom(*d, atomidx);
}

/** Return the atom in this view that matches the ID 'atomid'

    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
*/
Atom PartialMolecule::select(const AtomID &atomid) const
{
    AtomSelection selection = selected_atoms;
    selection.intersect(atomid);

    QVector<AtomIdx> atomidxs = selection.selectedAtoms();
    
    if (atomidxs.isEmpty())
        throw SireMol::missing_atom( QObject::tr(
            "There is no atom that matches the ID %1 in this view "
            "of the molecule called \"%2\".")
                .arg(atomid.toString(), this->name()), CODELOC );
                
    else if (atomidxs.count() > 1)
        throw SireMol::duplicate_atom( QObject::tr(
            "There are more than one atom in this view of the "
            "molecule called \"%1\" that match the ID %2 ( "
            "atoms at indicies %3).")
                .arg(this->name(), atomid.toString(), Sire::toString(atomidxs)),
                    CODELOC );
                    
    return Atom(*d, atomidxs.at(0));
}

/** Return all of the atoms from this view that match the ID 'atomid'

    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
Selector<Atom> PartialMolecule::selectAll(const AtomID &atomid) const
{
    AtomSelection selection = selected_atoms;
    selection.intersect(atomid);
    
    if (selection.isEmpty())
        throw SireMol::missing_atom( QObject::tr(
            "There is no atom that matches the ID %1 in this view "
            "of the molecule called \"%2\".")
                .arg(atomid.toString(), this->name()), CODELOC );

    return Selector<Atom>(*d, selection);
}

/** Return all of the atoms in this view */
Selector<Atom> PartialMolecule::selectAll() const
{
    return Selector<Atom>(*d, selected_atoms);
}

/** Select the atom in this view at index 'atomidx' 

    \throw SireError::invalid_index
*/
Atom PartialMolecule::atom(AtomIdx atomidx) const
{
    return this->select(atomidx);
}

/** Return the atom in this view that matches the ID 'atomid'

    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
*/
Atom PartialMolecule::atom(const AtomID &atomid) const
{
    return this->select(atomid);
}

/** Return all of the atoms from this view that match the ID 'atomid'

    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
Selector<Atom> PartialMolecule::atoms(const AtomID &atomid) const
{
    return this->selectAll(atomid);
}

/** Return all of the atoms in this view */
Selector<Atom> PartialMolecule::atoms() const
{
    return this->selectAll();
}

/** Return whether or not this molecule has a property at key 'key' */
bool PartialMolecule::hasProperty(const PropertyName &key) const
{
    return d->hasProperty(key);
}

/** Return whether or not this molecule has some metadata
    at the metakey 'metakey' */
bool PartialMolecule::hasMetadata(const PropertyName &metakey) const
{
    return d->hasMetadata(metakey);
}

/** Return whether or not the property at key 'key' has
    some metadata at metakey 'metakey'
    
    \throw SireBase::missing_property
*/
bool PartialMolecule::hasMetadata(const PropertyName &key, 
                                  const PropertyName &metakey) const
{
    return d->hasMetadata(key, metakey);
}

/** Return the keys of all of the properties contained in this molecule */
QStringList PartialMolecule::propertyKeys() const
{
    return d->propertyKeys();
}

/** Return the keys of all of the metadata contained directly by 
    this molecule */
QStringList PartialMolecule::metadataKeys() const
{
    return d->metadataKeys();
}

/** Return the keys of all metadata for the property at key 'key' 

    \throw SireBase::missing_property
*/
QStringList PartialMolecule::metadataKeys(const PropertyName &key) const
{
    return d->metadataKeys(key);
}

/** Return the property at key 'key'. Note that if this is a MolViewProperty
    (e.g. like AtomProperty, ResProperty etc.) then this property
    will be masked so that only values for the atoms that
    are part of this view will be returned.
    
    \throw SireBase::missing_property
*/
Property PartialMolecule::property(const PropertyName &key) const
{
    return d->property(key);
}

/** Return the metadata at metakey 'metakey'. Note that if this 
    is a MolViewProperty then this metadata will be masked so that
    only values for the atoms that are part of this view will
    be returned
    
    \throw SireBase::missing_property
*/
Property PartialMolecule::metadata(const PropertyName &metakey) const
{
    return d->metadata(metakey);
}

/** Return the metadata at the metakey 'metakey' for the property
    at key 'key'. If this is a MolViewProperty then this metadata
    will be masked so that only values for the atoms that are
    part of this view will be returned
    
    \throw SireBase::missing_property
*/
Property PartialMolecule::metadata(const PropertyName &key,
                                   const PropertyName &metakey) const
{
    return d->metadata(key, metakey);
}

/////////
///////// Explicit instantiation of the template classes
/////////

template class Mover<PartialMolecule>;

