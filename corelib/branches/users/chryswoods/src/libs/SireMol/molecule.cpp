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

#include "molecule.h"
#include "segment.h"
#include "chain.h"
#include "residue.h"
#include "cutgroup.h"
#include "atom.h"

#include "molviewproperty.h"
#include "moleditor.h"

#include "evaluator.h"

#include "mover.hpp"
#include "selector.hpp"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include <QDebug>

using namespace SireMol;
using namespace SireStream;

RegisterMetaType<Molecule> r_mol;

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds,
                                       const Molecule &mol)
{
    writeHeader(ds, r_mol, 1);
    ds << static_cast<const MoleculeView&>(mol);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds,
                                       Molecule &mol)
{
    VersionID v = readHeader(ds, r_mol);
    
    if (v == 1)
    {
        ds >> static_cast<MoleculeView&>(mol);
    }
    else
        throw version_error(v, "1", r_mol, CODELOC);
        
    return ds;
}

/** Null constructor */
Molecule::Molecule() : ConcreteProperty<Molecule,MoleculeView>()
{}

/** Construct a new Molecule, called 'molname' */
Molecule::Molecule(const QString &molname) : ConcreteProperty<Molecule,MoleculeView>()
{
    this->operator=( this->edit().renumber().rename(molname).commit() );
}

/** Construct from the passed MoleculeData */
Molecule::Molecule(const MoleculeData &moldata)
         : ConcreteProperty<Molecule,MoleculeView>(moldata)
{}

/** Copy constructor */
Molecule::Molecule(const Molecule &other)
         : ConcreteProperty<Molecule,MoleculeView>(other)
{}

/** Destructor */
Molecule::~Molecule()
{}

/** Copy assignment operator */
Molecule& Molecule::operator=(const Molecule &other)
{
    d = other.d;
    return *this;
}

/** Return a string representation of this molecule */
QString Molecule::toString() const
{
    return QObject::tr( "Molecule( %1 : %2 : UID == %3 )" )
                .arg(this->name())
                .arg(this->number())
                .arg(this->data().info().UID().toString());
}

/** Return whether or not this is empty */
bool Molecule::isEmpty() const
{
    return d->info().nAtoms() == 0;
}

/** Return whether or not this is a complete molecule */
bool Molecule::selectedAll() const
{
    return not Molecule::isEmpty();
}

/** Return which atoms are selected in this view */
AtomSelection Molecule::selection() const
{
    return AtomSelection(*d);
}

/** Return the name of this molecule */
const MolName& Molecule::name() const
{
    return d->name();
}

/** Return the number of this molecule - this is used
    to identify the molecule */
MolNum Molecule::number() const
{
    return d->number();
}
 
/** Return the version number of this molecule - all molecules
    with the same ID number and version number must be identical */
quint64 Molecule::version() const
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
quint64 Molecule::version(const PropertyName &key) const
{
    return d->version(key);
}
 
/** Return the number of atoms in this molecule */
int Molecule::nAtoms() const
{
    return d->info().nAtoms();
}
 
/** Return the number of CutGroups in this molecule */
int Molecule::nCutGroups() const
{
    return d->info().nCutGroups();
}
 
/** Return the number of residues in this molecule */
int Molecule::nResidues() const
{
    return d->info().nResidues();
}
 
/** Return the number of chains in this molecule */
int Molecule::nChains() const
{
    return d->info().nChains();
}
 
/** Return the number of segments in this molecule */
int Molecule::nSegments() const
{
    return d->info().nSegments();
}
 
/** Return a Mover that moves all of the atoms in
    this molecule */
Mover<Molecule> Molecule::move() const
{
    return Mover<Molecule>(*this);
}

/** Return an Evaluator that evaluates values using
    all of the atoms in this molecule */
Evaluator Molecule::evaluate() const
{
    return Evaluator(*this);
}

/** Return an Editor that can edit any part of this molecule */
MolEditor Molecule::edit() const
{
    return MolEditor(*this);
}

/** Update this molecule with the passed molecule data.

    \throw SireError::incompatible_error
*/
void Molecule::update(const MoleculeData &moldata)
{
    //check that the new data is compatible (has same molecule
    //number)
    if (d->number() != moldata.number())
    {
        throw SireError::incompatible_error( QObject::tr(
            "You can only update a molecule with the molecule data "
            "for the same molecule (same molecule number) You are "
            "trying to update molecule %1 with molecule %2.")
                .arg(d->number()).arg(moldata.number()),
                    CODELOC );
    }
    
    d = moldata;
}

/** Return the atom identified by 'atomid'

    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
*/
Atom Molecule::select(const AtomID &atomid) const
{
    return Atom(*d, atomid);
}

/** Return the residue identified by 'resid'

    \throw SireMol::missing_residue
    \throw SireMol::duplicate_residue
    \throw SireError::invalid_index
*/
Residue Molecule::select(const ResID &resid) const
{
    return Residue(*d, resid);
}

/** Return the chain identified by 'chainid'

    \throw SireMol::missing_chain
    \throw SireMol::duplicate_chain
    \throw SireError::invalid_index
*/
Chain Molecule::select(const ChainID &chainid) const
{
    return Chain(*d, chainid);
}

/** Return the CutGroup identified by 'cgid' 

    \throw SireMol::missing_cutgroup
    \throw SireMol::duplicate_cutgroup
    \throw SireError::invalid_index
*/
CutGroup Molecule::select(const CGID &cgid) const
{
    return CutGroup(*d, cgid);
}

/** Return the segment identified by 'segid'

    \throw SireMol::missing_segment
    \throw SireMol::duplicate_segment
    \throw SireError::invalid_index
*/
Segment Molecule::select(const SegID &segid) const
{
    return Segment(*d, segid);
}

/** Return the atom(s) in this molecule that match 'atomid'

    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
Selector<Atom> Molecule::selectAll(const AtomID &atomid) const
{
    return Selector<Atom>(*d, atomid);
}

/** Return the residue(s) in this molecule that match 'resid'

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
Selector<Residue> Molecule::selectAll(const ResID &resid) const
{
    return Selector<Residue>(*d, resid);
}

/** Return the chain(s) in this molecule that match 'chainid'

    \throw SireMol::missing_chain
    \throw SireError::invalid_index
*/
Selector<Chain> Molecule::selectAll(const ChainID &chainid) const
{
    return Selector<Chain>(*d, chainid);
}

/** Return the CutGroup(s) in this molecule that match 'cgid' 

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
Selector<CutGroup> Molecule::selectAll(const CGID &cgid) const
{
    return Selector<CutGroup>(*d, cgid);
}

/** Return the segment(s) in this molecule that match 'segid'

    \throw SireMol::missing_segment
    \throw SireMol::invalid_index
*/
Selector<Segment> Molecule::selectAll(const SegID &segid) const
{
    return Selector<Segment>(*d, segid);
}
   
/** Return the selection of all atoms in the molecule */ 
Selector<Atom> Molecule::selectAllAtoms() const
{
    return Selector<Atom>(*d);
}

/** Return the selection of all CutGroups in the molecule */
Selector<CutGroup> Molecule::selectAllCutGroups() const
{
    return Selector<CutGroup>(*d);
}

/** Return the selection of all residues in the molecule */
Selector<Residue> Molecule::selectAllResidues() const
{
    return Selector<Residue>(*d);
}

/** Return the selection of all chains in the molecule */
Selector<Chain> Molecule::selectAllChains() const
{
    return Selector<Chain>(*d);
}

/** Return the selection of all segments in the molecule */
Selector<Segment> Molecule::selectAllSegments() const
{
    return Selector<Segment>(*d);
}

/** Return the atom at ID == atomid

    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
*/
Atom Molecule::atom(const AtomID &atomid) const
{
    return this->select(atomid);
}

/** Return the residue identified by 'resid'

    \throw SireMol::missing_residue
    \throw SireMol::duplicate_residue
    \throw SireError::invalid_index
*/
Residue Molecule::residue(const ResID &resid) const
{
    return this->select(resid);
}

/** Return the chain identified by 'chainid'

    \throw SireMol::missing_chain
    \throw SireMol::duplicate_chain
    \throw SireError::invalid_index
*/
Chain Molecule::chain(const ChainID &chainid) const
{
    return this->select(chainid);
}

/** Return the CutGroup identified by 'cgid' 

    \throw SireMol::missing_cutgroup
    \throw SireMol::duplicate_cutgroup
    \throw SireError::invalid_index
*/
CutGroup Molecule::cutGroup(const CGID &cgid) const
{
    return this->select(cgid);
}

/** Return the segment identified by 'segid'

    \throw SireMol::missing_segment
    \throw SireMol::duplicate_segment
    \throw SireError::invalid_index
*/
Segment Molecule::segment(const SegID &segid) const
{
    return this->select(segid);
}

/** Return the atom(s) in this molecule that match 'atomid'

    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
Selector<Atom> Molecule::atoms(const AtomID &atomid) const
{
    return this->selectAll(atomid);
}

/** Return the atom(s) in this molecule that 
    are in the residue(s) that match 'resid'

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
Selector<Atom> Molecule::atoms(const ResID &resid) const
{
    return this->selectAll(resid.atoms());
}

/** Return the atom(s) in this molecule that
    are in the chain(s) that match 'chainid'

    \throw SireMol::missing_chain
    \throw SireError::invalid_index
*/
Selector<Atom> Molecule::atoms(const ChainID &chainid) const
{
    return this->selectAll(chainid.atoms());
}

/** Return the atom(s) in this molecule that 
    are in the CutGroup(s) that match 'cgid' 

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
Selector<Atom> Molecule::atoms(const CGID &cgid) const
{
    return this->selectAll(cgid.atoms());
}

/** Return the atom(s) in this molecule that
    are in the segment(s) that match 'segid'

    \throw SireMol::missing_segment
    \throw SireMol::invalid_index
*/
Selector<Atom> Molecule::atoms(const SegID &segid) const
{
    return this->selectAll(segid.atoms());
}

/** Return all of the atoms in this molecule */
Selector<Atom> Molecule::atoms() const
{
    return this->selectAllAtoms();
}

/** Return the residue(s) in this molecule that match 'resid'

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
Selector<Residue> Molecule::residues(const ResID &resid) const
{
    return this->selectAll(resid);
}

/** Return the residue(s) in this molecule that 
    are in the chain(s) that match 'chainid'

    \throw SireMol::missing_chain
    \throw SireError::invalid_index
*/
Selector<Residue> Molecule::residues(const ChainID &chainid) const
{
    return this->selectAll(chainid.residues());
}

/** Return all of the residues in this molecule */
Selector<Residue> Molecule::residues() const
{
    return this->selectAllResidues();
}

/** Return the chain(s) in this molecule that match 'chainid'

    \throw SireMol::missing_chain
    \throw SireError::invalid_index
*/
Selector<Chain> Molecule::chains(const ChainID &chainid) const
{
    return this->selectAll(chainid);
}

/** Return all of the chains in this molecule */
Selector<Chain> Molecule::chains()
{
    return this->selectAllChains();
}

/** Return the CutGroup(s) in this molecule that match 'cgid' 

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
Selector<CutGroup> Molecule::cutGroups(const CGID &cgid) const
{
    return this->selectAll(cgid);
}

/** Return all of the CutGroups in this molecule */
Selector<CutGroup> Molecule::cutGroups()
{
    return this->selectAllCutGroups();
}

/** Return the segment(s) in this molecule that match 'segid'

    \throw SireMol::missing_segment
    \throw SireMol::invalid_index
*/
Selector<Segment> Molecule::segments(const SegID &segid) const
{
    return this->selectAll(segid);
}

/** Return all of the segments in this molecule */
Selector<Segment> Molecule::segments()
{
    return this->selectAllSegments();
}

/** Return all of the properties of this molecule */
const Properties& Molecule::properties() const
{
    return d->properties();
}

/** Return the property associated with the key 'key' 

    \throw SireMol::missing_property
*/
const Property& Molecule::property(const PropertyName &key) const
{
    return d->property(key);
}

/** Return the metadata for the metakey 'metakey'

    \throw SireMol::missing_property
*/
const Property& Molecule::metadata(const PropertyName &metakey) const
{
    return d->metadata(metakey);
}

/** Return the metadata for the metakey 'metakey' for 
    the property at key 'key'
    
    \throw SireBase::missing_property
*/
const Property& Molecule::metadata(const PropertyName &key,
                                   const PropertyName &metakey) const
{
    return d->metadata(key, metakey);
}

/** Set the property at key 'key' to the value 'value'. If this
    is a property derived from MolViewProperty then this 
    property is checked to ensure it is compatible with this molecule
    
    \throw SireError::incompatible_error
*/
void Molecule::setProperty(const QString &key, const Property &value)
{
    if (value.isA<MolViewProperty>())
    {
        value.asA<MolViewProperty>().assertCompatibleWith(this->data().info());
    }
        
    d->setProperty(key, value);
}

/** Set the metadata at metakey 'metakey' to the value 'value'.
    If this is a property derived from MolViewProperty then this 
    property is checked to ensure it is compatible with this molecule
    
    \throw SireError::incompatible_error
*/
void Molecule::setMetadata(const QString &metakey, const Property &value)
{
    if (value.isA<MolViewProperty>())
        value.asA<MolViewProperty>().assertCompatibleWith(d->info());
        
    d->setMetadata(metakey, value);
}

/** Set the metadata at metakey 'metakey' to the value 'value'.
    If this is a property derived from MolViewProperty then this 
    property is checked to ensure it is compatible with this molecule
    
    \throw SireError::incompatible_error
*/
void Molecule::setMetadata(const QString &key, const QString &metakey,
                           const Property &value)
{
    if (value.isA<MolViewProperty>())
        value.asA<MolViewProperty>().assertCompatibleWith(d->info());
        
    d->setMetadata(key, metakey, value);
}

/** Return whether or not this molecule posseses a property at key 'key' */
bool Molecule::hasProperty(const PropertyName &key) const
{
    return d->hasProperty(key);
}

/** Return whether or not this molecule posseses metadata with
    metakey 'metakey' */
bool Molecule::hasMetadata(const PropertyName &metakey) const
{
    return d->hasMetadata(metakey);
}

/** Return whether or not the property of this molecule at
    key 'key' has metadata at metakey 'metakey'
    
    \throw SireBase::missing_property
*/
bool Molecule::hasMetadata(const PropertyName &key,
                           const PropertyName &metakey) const
{
    return d->hasMetadata(key, metakey);
}

/** Assert that this molecule contains a property at key 'key'

    \throw SireBase::missing_property
*/
void Molecule::assertContainsProperty(const PropertyName &key) const
{
    d->properties().assertContainsProperty(key);
}

/** Assert that this molecule contains some metadata at metakey 'metakey'

    \throw SireBase::missing_property
*/
void Molecule::assertContainsMetadata(const PropertyName &metakey) const
{
    d->properties().assertContainsMetadata(metakey);
}

/** Assert that this molecule contains some metadata at metakey 'metakey'
    associated with the property at key 'key'
    
    \throw SireBase::missing_property
*/
void Molecule::assertContainsMetadata(const PropertyName &key,
                                      const PropertyName &metakey) const
{
    d->properties().assertContainsMetadata(key, metakey);
}

/** Return the keys of all of the properties in this molecule */
QStringList Molecule::propertyKeys() const
{
    return d->propertyKeys();
}

/** Return the metakeys of all the metadata in this molecule */
QStringList Molecule::metadataKeys() const
{
    return d->metadataKeys();
}

/** Return the metakeys for all of the metadata for the property
    at key 'key'
    
    \throw SireBase::missing_property
*/
QStringList Molecule::metadataKeys(const PropertyName &key) const
{
    return d->metadataKeys(key);
}

const char* Molecule::typeName()
{
    return QMetaType::typeName( qMetaTypeId<Molecule>() );
}

namespace SireMol
{
    ///// explicitly instantiate manipulator classes
    template class Mover<Molecule>;
}
