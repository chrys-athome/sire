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

using namespace SireMol;

/** Null constructor */
Molecule::Molecule() : MoleculeView()
{}

/** Copy constructor */
Molecule::Molecule(const Molecule &other)
         : MoleculeView(other)
{}

/** Copy from another MoleculeView */
Molecule::Molecule(const MoleculeView &other)
         : MoleculeView(other)
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

/** Copy assignment from a general MoleculeView */
Molecule& Molecule::operator=(const MoleculeView &view)
{
    d = view.d;
    return *this;
}

/** Return which atoms are selected in this view */
AtomSelection Molecule::selectedAtoms() const
{
    return AtomSelection(d->info());
}

/** Update this view to use the new version - this will
    throw an exception if the new version is incompatible with
    this view 
    
    \throw SireError::incompatible_error
*/
void Molecule::update(const MoleculeView &view)
{
    view.assertSameMolecule(*this); 
    d = view.d;
}

/** Return the name of this molecule */
const MolName& Molecule::name() const
{
    return d->info().name();
}

/** Return the number of this molecule - this is used
    to identify the molecule */
const MolNum& Molecule::number() const
{
    return d->info().number();
}

/** Return the info object that contains the information
    about the names of the parts of the molecule, and how
    they are grouped together */
const MoleculeInfo& Molecule::info() const
{
    return d->info();
}

/** Return a Mover that moves all of the atoms in
    this molecule */
Mover<Molecule> Molecule::move() const
{
    return Mover<Molecule>(*this);
}

/** Return an Evaluator that evaluates values using
    all of the atoms in this molecule */
Evaluator<Molecule> Molecule::evaluate() const
{
    return Evaluator<Molecule>(*this);
}

/** Return an Editor that can edit any part of this molecule */
Editor<Molecule> Molecule::edit() const
{
    return Editor<Molecule>(*this);
}

/** Return a Selector that can be used to modify the atoms 
    selected in this molecule */
Selector<Molecule> Molecule::selection() const
{
    return Selector<Molecule>(*this);
}

/** Return the atom identified by 'atomid'

    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
*/
Atom Molecule::select(const AtomID &atomid) const
{
    return Atom(*this, atomid);
}

/** Return the residue identified by 'resid'

    \throw SireMol::missing_residue
    \throw SireMol::duplicate_residue
    \throw SireError::invalid_index
*/
Residue Molecule::select(const ResID &resid) const
{
    return Residue(*this, resid);
}

/** Return the chain identified by 'chainid'

    \throw SireMol::missing_chain
    \throw SireMol::duplicate_chain
    \throw SireError::invalid_index
*/
Chain Molecule::select(const ChainID &chainid) const
{
    return Chain(*this, chainid);
}

/** Return the CutGroup identified by 'cgid' 

    \throw SireMol::missing_cutgroup
    \throw SireMol::duplicate_cutgroup
    \throw SireError::invalid_index
*/
CutGroup Molecule::select(const CGID &cgid) const
{
    return CutGroup(*this, cgid);
}

/** Return the segment identified by 'segid'

    \throw SireMol::missing_segment
    \throw SireMol::duplicate_segment
    \throw SireError::invalid_index
*/
Segment Molecule::select(const Segment &segid) const
{
    return Segment(*this, segid);
}

/** Return the atom(s) in this molecule that match 'atomid'

    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
AtomsInMol Molecule::selectAll(const AtomID &atomid) const
{
    return AtomsInMol(*this, atomid);
}

/** Return the residue(s) in this molecule that match 'resid'

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
ResiduesInMol Molecule::selectAll(const ResID &resid) const
{
    return ResiduesInMol(*this, resid);
}

/** Return the chain(s) in this molecule that match 'chainid'

    \throw SireMol::missing_chain
    \throw SireError::invalid_index
*/
ChainsInMol Molecule::selectAll(const ChainID &chainid) const
{
    return ChainsInMol(*this, chainid);
}

/** Return the CutGroup(s) in this molecule that match 'cgid' 

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
CutGroupsInMol Molecule::selectAll(const CGID &cgid) const
{
    return CutGroupsInMol(*this, cgid);
}

/** Return the segment(s) in this molecule that match 'segid'

    \throw SireMol::missing_segment
    \throw SireMol::invalid_index
*/
SegmentsInMol Molecule::selectAll(const SegID &segid) const
{
    return SegmentsInMol(*this, segid);
}

/** Return all of the properties of this molecule */
const Properties& Molecule::properties() const
{
    return d->properties();
}

/** Return the property associated with the key 'key' 

    \throw SireMol::missing_property
*/
const Property& Molecule::property(const QString &key) const
{
    return d->property(key);
}

/** Return the metadata for the property associated with the key 'key'

    \throw SireMol::missing_property
*/
const Properties& Molecule::metadata(const QString &key) const
{
    return d->metadata(key);
}
