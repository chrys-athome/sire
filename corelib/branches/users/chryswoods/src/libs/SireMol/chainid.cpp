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

#include "chainid.h"
#include "chainidentifier.h"
#include "specify.hpp"

#include "chainchainid.h"
#include "chainresid.h"
#include "groupatomids.h"
#include "groupgroupids.h"

#include "atom.h"
#include "selector.hpp"

#include "mover.hpp"
#include "editor.hpp"

#include "molinfo.h"

#include "partialmolecule.h"
#include "segment.h"
#include "chain.h"
#include "residue.h"
#include "cutgroup.h"
#include "atom.h"

#include "molecules.h"
#include "moleculegroup.h"
#include "moleculegroups.h"

#include "SireMol/errors.h"

#include "tostring.h"

using namespace SireMol;
using namespace SireID;

/** Constructor */
ChainID::ChainID() : ID()
{}

/** Copy constructor */
ChainID::ChainID(const ChainID &other) : ID(other)
{}

/** Destructor */
ChainID::~ChainID()
{}

/** Combine with another ID object */
ChainChainID ChainID::operator+(const ChainID &other) const
{
    return ChainChainID(*this, other);
}

/** Combine with another ID object */
ChainResID ChainID::operator+(const ResID &other) const
{
    return ChainResID(*this, other);
}

/** Combine with another ID object */
GroupAtomID<ChainID,AtomID> ChainID::operator+(const AtomID &other) const
{
    return GroupAtomID<ChainID,AtomID>(*this, other);
}

/** Combine with another ID object */
GroupGroupID<SegID,ChainID> ChainID::operator+(const SegID &other) const
{
    return GroupGroupID<SegID,ChainID>(other, *this);
}

/** Combine with another ID object */
GroupGroupID<CGID,ChainID> ChainID::operator+(const CGID &other) const
{
    return GroupGroupID<CGID,ChainID>(other, *this);
}

/** Return the atoms in the matching residues */
AtomsIn<ChainID> ChainID::atoms() const
{
    return AtomsIn<ChainID>(*this);
}

/** Return a specific atom in the matching residues */
AtomsIn<ChainID> ChainID::atom(int i) const
{
    return AtomsIn<ChainID>(*this, i);
}

/** Return a range of atoms in the matching residues */
AtomsIn<ChainID> ChainID::atoms(int i, int j) const
{
    return AtomsIn<ChainID>(*this, i, j);
}

/** Return the atoms in the matching residues */
ResIn<ChainID> ChainID::residues() const
{
    return ResIn<ChainID>(*this);
}

/** Return a specific atom in the matching residues */
ResIn<ChainID> ChainID::residue(int i) const
{
    return ResIn<ChainID>(*this, i);
}

/** Return a range of atoms in the matching residues */
ResIn<ChainID> ChainID::residues(int i, int j) const
{
    return ResIn<ChainID>(*this, i, j);
}
  
/** Return a specific atom that matches this ID */
Specify<ChainID> ChainID::operator[](int i) const
{
    return Specify<ChainID>(*this, i);
}

/** Return a specific atom that matches this ID */
Specify<ChainID> ChainID::operator()(int i) const
{
    return this->operator[](i);
}

/** Return a range of atoms that match this ID */
Specify<ChainID> ChainID::operator()(int i, int j) const
{
    return Specify<ChainID>(*this, i, j);
}

/** Return all of the chains from the 'molecules' that match
    this ID
    
    \throw SireMol::missing_chain
*/
QHash< MolNum,Selector<Chain> >
ChainID::selectAllFrom(const Molecules &molecules) const
{
    QHash< MolNum,Selector<Chain> > selected_chains;
    
    //loop over all molecules...
    for (Molecules::const_iterator it = molecules.constBegin();
         it != molecules.constEnd();
         ++it)
    {
        try
        {
            //try to find this atom in this molecule
            selected_chains.insert( it.key(),
                                       it->all().selectAll(*this) );
        }
        catch(...)
        {}
    }
    
    if (selected_chains.isEmpty())
        throw SireMol::missing_chain( QObject::tr(
            "There was no chain matching the ID \"%1\" in "
            "the set of molecules.")
                .arg(this->toString()), CODELOC );

    return selected_chains;
}

/** Return the chain from the molecules 'molecules' that matches
    this ID
    
    \throw SireMol::missing_chain
    \throw SireMol::duplicate_chain
*/
Chain ChainID::selectFrom(const Molecules &molecules) const
{
    QHash< MolNum,Selector<Chain> > mols = this->selectAllFrom(molecules);
    
    if (mols.count() > 1)
        throw SireMol::duplicate_chain( QObject::tr(
            "More than one molecule contains a chain that "
            "matches this ID (%1). These molecules have numbers %2.")
                .arg(this->toString()).arg(Sire::toString(mols.keys())),
                    CODELOC );
                    
    const Selector<Chain> &chains = *(mols.constBegin());
    
    if (chains.count() > 1)
        throw SireMol::duplicate_chain( QObject::tr(
            "While only one molecule (MolNum == %1) "
            "contains a chain that matches this ID (%2), it contains "
            "more than one chain that matches.")
                .arg(chains.data().number()).arg(this->toString()),
                    CODELOC );
                    
    return chains[0];
}

/** Return the chain from the molecule group 'molgroup' that matches
    this ID
    
    \throw SireMol::missing_chain
    \throw SireMol::duplicate_chain
*/
Chain ChainID::selectFrom(const MoleculeGroup &molgroup) const
{
    return ChainID::selectFrom(molgroup.molecules());
}

/** Return the Chains from the molecule group 'molgroup' that match
    this ID
    
    \throw SireMol::missing_chain
*/
QHash< MolNum,Selector<Chain> >
ChainID::selectAllFrom(const MoleculeGroup &molgroup) const
{
    return ChainID::selectAllFrom(molgroup.molecules());
}

/** Return the chain from the molecule groups 'molgroups' that matches 
    this ID
    
    \throw SireMol::missing_chain
    \throw SireMol::duplicate_chain
*/
Chain ChainID::selectFrom(const MolGroupsBase &molgroups) const
{
    return ChainID::selectFrom(molgroups.molecules());
}

/** Return the set of Chains that match this ID in the molecule groups
    set 'molgroups' 
    
    \throw SireMol::missing_chain
*/
QHash< MolNum,Selector<Chain> >
ChainID::selectAllFrom(const MolGroupsBase &molgroups) const
{
    return ChainID::selectAllFrom(molgroups.molecules());
}

//fully instantiate template classes
template class Specify<ChainID>;
template class AtomsIn<ChainID>;
template class ResIn<ChainID>;


