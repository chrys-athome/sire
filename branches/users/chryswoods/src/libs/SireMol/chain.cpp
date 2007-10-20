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

#include "chain.h"

#include "molecule.h"
#include "residue.h"
#include "atom.h"

#include "editor.hpp"
#include "mover.hpp"
#include "selector.hpp"
#include "evaluator.h"

#include "chainresid.h"
#include "groupatomids.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMol;
using namespace SireStream;

RegisterMetaType<Chain> r_chain;

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const Chain &chain)
{
    writeHeader(ds, r_chain, 1);

    SharedDataStream sds(ds);
    
    sds << chain.chainidx << static_cast<const MoleculeView&>(chain);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, Chain &chain)
{
    VersionID v = readHeader(ds, r_chain);

    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> chain.chainidx >> static_cast<MoleculeView&>(chain);
        
        chain.selected_atoms = AtomSelection(chain.data());
        chain.selected_atoms.select(chain.chainidx);
    }
    else
        throw version_error(v, "1", r_chain, CODELOC);

    return ds;
}

/** Null constructor */
Chain::Chain() : MoleculeView(), chainidx( ChainIdx::null() )
{}

/** Construct the chain at ID 'chainid' in the molecule whose data
    is in 'moldata'
    
    \throw SireMol::missing_chain
    \throw SireMol::duplicate_chain
    \throw SireError::invalid_index
*/
Chain::Chain(const MoleculeData &moldata, const ChainID &chainid)
      : MoleculeView(moldata), chainidx( moldata.info().chainIdx(chainid) )
{
    selected_atoms = AtomSelection(moldata);
    selected_atoms.select(chainidx);
}

/** Copy constructor */
Chain::Chain(const Chain &other)
      : MoleculeView(other), chainidx(other.chainidx),
        selected_atoms(other.selected_atoms)
{}

/** Destructor */
Chain::~Chain()
{}

/** Copy assignment operator */
Chain& Chain::operator=(const Chain &other)
{
    MoleculeView::operator=(other);
    chainidx = other.chainidx;
    selected_atoms = other.selected_atoms;
    return *this;
}

/** Comparison operator */
bool Chain::operator==(const Chain &other) const
{
    return chainidx == other.chainidx and 
           MoleculeView::operator==(other);
}

/** Comparison operator */
bool Chain::operator!=(const Chain &other) const
{
    return chainidx != other.chainidx or
           MoleculeView::operator!=(other);
}

/** Return the atoms that are in this chain */
AtomSelection Chain::selection() const
{
    return selected_atoms;
}

/** Return the name of this chain */
ChainName Chain::name() const
{
    return d->info().name(chainidx);
}

/** Return the index of this chain in the molecule */
ChainIdx Chain::index() const
{
    return chainidx;
}

/** Return an object that can move a copy of this chain */
Mover<Chain> Chain::move() const
{
    return Mover<Chain>(*this);
}

/** Return an evaluator that can evaluate properties 
    of this chain */
Evaluator Chain::evaluate() const
{
    return Evaluator(*this);
}

/** Return an editor that can edit this chain */
Editor<Chain> Chain::edit() const
{
    return Editor<Chain>(*this);
}

/** Return the number of atoms in this Chain */
int Chain::nAtoms() const
{
    return d->info().nAtoms(chainidx);
}

/** Return the indicies of the atoms in this chain, in the
    order that they appear in this chain */
QList<AtomIdx> Chain::atomIdxs() const
{
    return d->info().getAtomsIn(chainidx);
}

/** Return a selector that change the selection of chains */
Selector<Chain> Chain::selector() const
{
    return Selector<Chain>(*this);
}

/** Return whether or not this chain contains the atom 
    at index 'atomidx' */
bool Chain::contains(AtomIdx atomidx) const
{
    return d->info().contains(chainidx, atomidx);
}

/** Return whether or not this chain contains all of 
    the atoms identified by the ID 'atomid' */
bool Chain::contains(const AtomID &atomid) const
{
    return d->info().contains(chainidx, atomid);
}

/** Return whether or not this chain contains some of  
    the atoms identified by the ID 'atomid' */
bool Chain::intersects(const AtomID &atomid) const
{
    return d->info().intersects(chainidx, atomid);
}

/** Return the number of residues in this chain */
int Chain::nResidues() const
{
    return d->info().nResidues(chainidx);
}

/** Return the list of indicies of residues that are
    in this chain, in the order they appear in this chain */
const QList<ResIdx>& Chain::resIdxs() const
{
    return d->info().getResiduesIn(chainidx);
}

/** Return whether or not this chain contains the 
    residue at index 'residx' */
bool Chain::contains(ResIdx residx) const
{
    return d->info().contains(chainidx, residx);
}

/** Return whether or not this chain contains all
    of the residues identified by the ID 'resid' */
bool Chain::contains(const ResID &resid) const
{
    return d->info().contains(chainidx, resid);
}

/** Return whether or not this chain contains some
    of the residues identified by the ID 'resid' */
bool Chain::intersects(const ResID &resid) const
{
    return d->info().intersects(chainidx, resid);
}

/** Return the molecule that contains this chain */
Molecule Chain::molecule() const
{
    return Molecule(this->data());
}

/** Return the atom in this chain that also has ID 'atomid'

    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
*/
Atom Chain::atom(const AtomID &atomid) const
{
    return Atom( this->data(), chainidx + atomid );
}

/** Return the atoms in this chain that also have the ID 'atomid'

    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
Selector<Atom> Chain::atoms(const AtomID &atomid) const
{
    return Selector<Atom>( this->data(), chainidx + atomid );
}

/** Return all of the atoms in this chain */
Selector<Atom> Chain::atoms() const
{
    return Selector<Atom>( this->data(), chainidx.atoms() );
}

/** Return the atom in this chain that also has ID 'atomid'

    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
    \throw SireError::invalid_index
*/
Atom Chain::select(const AtomID &atomid) const
{
    return this->atom(atomid);
}

/** Return the atoms in this chain that also have the ID 'atomid'

    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
Selector<Atom> Chain::selectAll(const AtomID &atomid) const
{
    return this->atoms(atomid);
}

/** Return all of the atoms in this chain */
Selector<Atom> Chain::selectAllAtoms() const
{
    return this->atoms();
}

/** Return the residue in this chain that also has ID 'resid'

    \throw SireMol::missing_residue
    \throw SireMol::duplicate_residue
    \throw SireError::invalid_index
*/
Residue Chain::residue(const ResID &resid) const
{
    return Residue( this->data(), chainidx + resid );
}

/** Return the residues in this chain that also have ID 'resid'

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
Selector<Residue> Chain::residues(const ResID &resid) const
{
    return Selector<Residue>( this->data(), chainidx + resid );
}

/** Return all of the residues in this chain */
Selector<Residue> Chain::residues() const
{
    return Selector<Residue>( this->data(), chainidx.residues() );
}

/** Return the residue in this chain that also has ID 'resid'

    \throw SireMol::missing_residue
    \throw SireMol::duplicate_residue
    \throw SireError::invalid_index
*/
Residue Chain::select(const ResID &resid) const
{
    return this->residue(resid);
}

/** Return the residues in this chain that also have ID 'resid'

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
Selector<Residue> Chain::selectAll(const ResID &resid) const
{
    return this->residues(resid);
}

/** Return all of the residues in this chain */
Selector<Residue> Chain::selectAllResidues() const
{
    return this->residues();
}

/////// explicitly instantiate chain templates
template class Editor<Chain>;
template class Selector<Chain>;
template class Mover<Chain>;

template class Editor< Selector<Chain> >;
template class Mover< Selector<Chain> >;
