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

#include "editor.hpp"
#include "mover.hpp"
#include "selector.hpp"
#include "evaluator.h"

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
        
        chain.selected_atoms = AtomSelection(chain.d->info());
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
    selected_atoms = AtomSelection(d->info());
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
AtomSelection Chain::selectedAtoms() const
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

/** Return the info object that describes this chain */
ChainInfo Chain::info() const
{
    return ChainInfo( d->info(), chainidx );
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

/** Return a selector that can change the selection of Chains */
Selector<Chain> Chain::selection() const
{
    return Selector<Chain>(*this);
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
AtomsInMol Chain::atoms(const AtomID &atomid) const
{
    return AtomsInMol( this->data(), chainidx + atomid );
}

/** Return all of the atoms in this chain */
AtomsInMol Chain::atoms() const
{
    return AtomsInMol( this->data(), chainidx.atoms() );
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
AtomsInMol Chain::selectAll(const AtomID &atomid) const
{
    return this->atoms(atomid);
}

/** Return all of the atoms in this chain */
AtomsInMol Chain::selectAllAtoms() const
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
ResInMol Chain::residues(const ResID &resid) const
{
    return ResInMol( this->data(), chainidx + resid );
}

/** Return all of the residues in this chain */
ResInMol Chain::residues() const
{
    return ResInMol( this->data(), chainidx.residues() );
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
ResInMol Chain::selectAll(const ResID &resid) const
{
    return this->residues(resid);
}

/** Return all of the residues in this chain */
ResInMol Chain::selectAllResidues() const
{
    return this->residues();
}
