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

#include "resid.h"
#include "residentifier.h"

#include "chainresid.h"
#include "groupatomids.h"
#include "groupgroupids.h"

#include "atom.h"
#include "selector.hpp"

#include "molinfo.h"

#include "mover.hpp"
#include "editor.hpp"

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

#include "SireStream/datastream.h"

#include "tostring.h"

using namespace SireMol;
using namespace SireID;

/** Constructor */
ResID::ResID() : ID()
{}

/** Copy constructor */
ResID::ResID(const ResID &other) : ID(other)
{}

/** Destructor */
ResID::~ResID()
{}
  
/** Return a specific residue that matches this ID */
Specify<ResID> ResID::operator[](int i) const
{
    return Specify<ResID>(*this, i);
}

/** Return a specific residue that matches this ID */
Specify<ResID> ResID::operator()(int i) const
{
    return this->operator[](i);
}

/** Return a range of residues that match this ID */
Specify<ResID> ResID::operator()(int i, int j) const
{
    return Specify<ResID>(*this, i, j);
}

/** Combine with another ID type */
IDAndSet<ResID> ResID::operator+(const ResID &other) const
{
    return IDAndSet<ResID>(*this, other);
}

/** Syntactic sugar for operator+ */
IDAndSet<ResID> ResID::operator&&(const ResID &other) const
{
    return this->operator+(other);
}

/** Syntactic sugar for operator+ */
IDAndSet<ResID> ResID::operator&(const ResID &other) const
{
    return this->operator+(other);
}

/** Combine with another ID type */
ChainResID ResID::operator+(const ChainID &other) const
{
    return ChainResID(other, *this);
}

/** Syntactic sugar for operator+ */
ChainResID ResID::operator&&(const ChainID &other) const
{
    return this->operator+(other);
}

/** Syntactic sugar for operator+ */
ChainResID ResID::operator&(const ChainID &other) const
{
    return this->operator+(other);
}

/** Combine with another ID type */
GroupAtomID<ResID,AtomID> ResID::operator+(const AtomID &other) const
{
    return GroupAtomID<ResID,AtomID>(*this, other);
}

/** Syntactic sugar for operator+ */
GroupAtomID<ResID,AtomID> ResID::operator&&(const AtomID &other) const
{
    return this->operator+(other);
}

/** Syntactic sugar for operator+ */
GroupAtomID<ResID,AtomID> ResID::operator&(const AtomID &other) const
{
    return this->operator+(other);
}

/** Combine with another ID type */
GroupGroupID<SegID,ResID> ResID::operator+(const SegID &other) const
{
    return GroupGroupID<SegID,ResID>(other, *this);
}

/** Syntactic sugar for operator+ */
GroupGroupID<SegID,ResID> ResID::operator&&(const SegID &other) const
{
    return this->operator+(other);
}

/** Syntactic sugar for operator+ */
GroupGroupID<SegID,ResID> ResID::operator&(const SegID &other) const
{
    return this->operator+(other);
}

/** Combine with another ID type */
GroupGroupID<CGID,ResID> ResID::operator+(const CGID &other) const
{
    return GroupGroupID<CGID,ResID>(other, *this);
}

/** Syntactic sugar for operator+ */
GroupGroupID<CGID,ResID> ResID::operator&&(const CGID &other) const
{
    return this->operator+(other);
}

/** Syntactic sugar for operator+ */
GroupGroupID<CGID,ResID> ResID::operator&(const CGID &other) const
{
    return this->operator+(other);
}

/** Return the match for this ID or 'other' */
IDOrSet<ResID> ResID::operator*(const ResID &other) const
{
    return IDOrSet<ResID>(*this, other);
}

/** Syntactic sugar for operator* */
IDOrSet<ResID> ResID::operator||(const ResID &other) const
{
    return this->operator*(other);
}

/** Syntactic sugar for operator* */
IDOrSet<ResID> ResID::operator|(const ResID &other) const
{
    return this->operator*(other);
}

/** Return the atoms in the matching residues */
AtomsIn<ResID> ResID::atoms() const
{
    return AtomsIn<ResID>(*this);
}

/** Return a specific atom in the matching residues */
AtomsIn<ResID> ResID::atom(int i) const
{
    return AtomsIn<ResID>(*this, i);
}

/** Return a range of atoms in the matching residues */
AtomsIn<ResID> ResID::atoms(int i, int j) const
{
    return AtomsIn<ResID>(*this, i, j);
}

void ResID::processMatches(QList<ResIdx> &matches, const MolInfo&) const
{
    if (matches.isEmpty())
        throw SireMol::missing_residue( QObject::tr(
            "There are no residues that match the ID \"%1\"")
                .arg(this->toString()), CODELOC );
                
    qSort(matches);
}

/** Return all of the residues from the 'molecules' that match
    this ID
    
    \throw SireMol::missing_residue
*/
QHash< MolNum,Selector<Residue> >
ResID::selectAllFrom(const Molecules &molecules) const
{
    QHash< MolNum,Selector<Residue> > selected_residues;
    
    //loop over all molecules...
    for (Molecules::const_iterator it = molecules.constBegin();
         it != molecules.constEnd();
         ++it)
    {
        try
        {
            //try to find this residue in this molecule
            selected_residues.insert( it.key(),
                                       it->all().selectAll(*this) );
        }
        catch(...)
        {}
    }
    
    if (selected_residues.isEmpty())
        throw SireMol::missing_residue( QObject::tr(
            "There was no residue matching the ID \"%1\" in "
            "the set of molecules.")
                .arg(this->toString()), CODELOC );

    return selected_residues;
}

/** Return the residue from the molecules 'molecules' that matches
    this ID
    
    \throw SireMol::missing_residue
    \throw SireMol::duplicate_residue
*/
Residue ResID::selectFrom(const Molecules &molecules) const
{
    QHash< MolNum,Selector<Residue> > mols = this->selectAllFrom(molecules);
    
    if (mols.count() > 1)
        throw SireMol::duplicate_residue( QObject::tr(
            "More than one molecule contains a residue that "
            "matches this ID (%1). These molecules have numbers %2.")
                .arg(this->toString()).arg(Sire::toString(mols.keys())),
                    CODELOC );
                    
    const Selector<Residue> &residues = *(mols.constBegin());
    
    if (residues.count() > 1)
        throw SireMol::duplicate_residue( QObject::tr(
            "While only one molecule (MolNum == %1) "
            "contains a residue that matches this ID (%2), it contains "
            "more than one residue that matches.")
                .arg(residues.data().number()).arg(this->toString()),
                    CODELOC );
                    
    return residues[0];
}

/** Return the residue from the molecule group 'molgroup' that matches
    this ID
    
    \throw SireMol::missing_residue
    \throw SireMol::duplicate_residue
*/
Residue ResID::selectFrom(const MoleculeGroup &molgroup) const
{
    return ResID::selectFrom(molgroup.molecules());
}

/** Return the residues from the molecule group 'molgroup' that match
    this ID
    
    \throw SireMol::missing_residue
*/
QHash< MolNum,Selector<Residue> >
ResID::selectAllFrom(const MoleculeGroup &molgroup) const
{
    return ResID::selectAllFrom(molgroup.molecules());
}

/** Return the residue from the molecule groups 'molgroups' that matches 
    this ID
    
    \throw SireMol::missing_residue
    \throw SireMol::duplicate_residue
*/
Residue ResID::selectFrom(const MolGroupsBase &molgroups) const
{
    return ResID::selectFrom(molgroups.molecules());
}

/** Return the set of residues that match this ID in the molecule groups
    set 'molgroups' 
    
    \throw SireMol::missing_residue
*/
QHash< MolNum,Selector<Residue> >
ResID::selectAllFrom(const MolGroupsBase &molgroups) const
{
    return ResID::selectAllFrom(molgroups.molecules());
}

//fully instantiate Specify<ResID> and AtomsIn<ResID>
namespace SireID
{
    template class Specify<ResID>;
    template class IDAndSet<ResID>;
    template class IDOrSet<ResID>;
}

namespace SireMol
{
    template class AtomsIn<ResID>;
}

static const RegisterMetaType< Specify<ResID> > r_specify_resid;
static const RegisterMetaType< AtomsIn<ResID> > r_atomsin_resid;
static const RegisterMetaType< IDAndSet<ResID> > r_idandset_resid;
static const RegisterMetaType< IDOrSet<ResID> > r_idorset_resid;
