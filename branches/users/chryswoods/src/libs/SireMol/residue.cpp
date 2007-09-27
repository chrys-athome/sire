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

using namespace SireMol;

/** Null constructor */
Residue::Residue() : MoleculeView()
{}

/** Construct as a specified residue */
Residue::Residue(const MoleculeView &molview, const ResID &resid)
        : MoleculeView(molview)
{
    residx = resid.map(d->info());
    selected_atoms = AtomSelection(d->info());
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

/** Copy assignment from another MoleculeView. If
    'other' is a Residue (or derived from Residue)
    then this copies correctly - otherwise this
    throws an exception
    
    \throw SireError::invalid_cast
*/
Residue& Residue::operator=(const MoleculeView &other)
{
    if (not other.isA<Residue>())
        throw SireError::invalid_cast( QObject::tr(
            "Cannot assign a Residue from a non-Residue view (%1)!")
                .arg(other.what()), CODELOC );
                
    return this->operator=(other.asA<Residue>());
}

/** Return the identities of the atoms that are selected as
    part of this residue */
AtomSelection Residue::selectedAtoms() const
{
    return selected_atoms;
}

/** Update this residue using the passed view */
void Residue::update(const MoleculeView &other)
{
    other.assertSameMolecule(*this);
    d = other.d;
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

/** Return an info object that describes the names
    and arrangement of this residue */
ResidueInfo Residue::info() const
{
    return d->info(residx);
}

/** Return a Mover that moves all of the atoms in this residue */
Mover<Residue> Residue::move() const
{
    return Mover<Residue>(*this, selected_atoms);
}

/** Return an Evaluator that evaluates values using all of 
    the atoms in the residue */
Evaluator<Residue> Residue::evaluate() const
{
    return Evaluator<Residue>(*this, selected_atoms);
}

/** Return an editor that can be used to edit any of the  
    atoms of this residue */
Editor<Residue> Residue::edit() const
{
    return Editor<Residue>(*this, selected_atoms);
}

/** Return a selector that can be used to change the 
    selection of atoms in this residue */
Selector<Residue> Residue::selection() const
{
    return Selector<Residue>(*this, selected_atoms);
}

/** Return the molecule that contains this residue */
Molecule Residue::molecule() const
{
    return Molecule(*this);
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
                .arg(name()).arg(number()).arg(residx).arg(d->info().name()),
                    CODELOC );

    return Chain(*this, chainidx);
}

/** Return the atom that is in this residue *and* also has the 
    ID 'atomid'
    
    \throw SireMol::missing_atom
    \throw SireMol::dulplicate_atom
    \throw SireError::invalid_index
*/
Atom Residue::atom(const AtomID &atomid) const
{
    return Atom(*this, residx + atomid);
}

/** Return the atom(s) that are in this residue *and* also
    match the ID 'atomid'
    
    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
AtomsInMol Residue::atoms(const AtomID &atomid) const
{
    return AtomsInMol(*this, residx + atomid);
}

/** Return all of the atoms that are in this residue */
AtomsInMol Residue::atoms() const
{
    return AtomsInMol(*this, residx);
}

/** Return the property associated with the key 'key' - if 
    this is a molecular property, then it will be masked so
    that only the parts of the property that are relevant for
    this Residue are returned 
    
    \throw SireBase::missing_property
*/
Property Residue::property(const QString &key) const
{
    Property value = d->property(key);
    
    if (value.isA<MolecularProperty>())
        return value.asA<MolecularProperty>().mask(selected_atoms);
    else
        return value;
}

/** Return the metadata for the property associated with the key 'key'

    \throw SireBase::missing_property
*/
const Properties& Residue::metadata(const QString &key) const
{
    return d->metadata(key);
}
