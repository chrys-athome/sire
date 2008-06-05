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

#include "groupatomids.h"
#include "atomidentifier.h"
#include "residentifier.h"
#include "chainidentifier.h"
#include "segidentifier.h"
#include "cgidentifier.h"

#include "SireMol/errors.h"

using namespace SireMol;

//////
////// Implementation of GroupAtomIDBase
//////

GroupAtomIDBase::GroupAtomIDBase() : AtomID()
{}

GroupAtomIDBase::GroupAtomIDBase(const GroupAtomIDBase &other)
                : AtomID(other)
{}

GroupAtomIDBase::~GroupAtomIDBase()
{}

void GroupAtomIDBase::throwMissingAtom(const MolInfo&) const
{
    throw SireMol::missing_atom( QObject::tr(
        "There is no atom that matches the ID %1 in the molecule.")
            .arg(this->toString()), CODELOC );
}

//////
////// Explicitly instantiate the GroupAtomID classes
//////

template class GroupAtomID<ResID,AtomID>;
template class GroupAtomID<ChainID,AtomID>;
template class GroupAtomID<SegID,AtomID>;
template class GroupAtomID<CGID,AtomID>;

//////
////// Implementation of operators that create the GroupAtomID classes
//////

GroupAtomID<ResID,AtomID> SireMol::operator+(const ResID &resid,
                                             const AtomID &atomid)
{
    return GroupAtomID<ResID,AtomID>(resid,atomid);
}

GroupAtomID<ResID,AtomID> SireMol::operator+(const AtomID &atomid,
                                             const ResID &resid)
{
    return GroupAtomID<ResID,AtomID>(resid,atomid);
}

GroupAtomID<ChainID,AtomID> SireMol::operator+(const ChainID &chainid,
                                               const AtomID &atomid)
{
    return GroupAtomID<ChainID,AtomID>(chainid,atomid);
}

GroupAtomID<ChainID,AtomID> SireMol::operator+(const AtomID &atomid,
                                               const ChainID &chainid)
{
    return GroupAtomID<ChainID,AtomID>(chainid,atomid);
}

GroupAtomID<SegID,AtomID> SireMol::operator+(const SegID &segid,
                                             const AtomID &atomid)
{
    return GroupAtomID<SegID,AtomID>(segid,atomid);
}

GroupAtomID<SegID,AtomID> SireMol::operator+(const AtomID &atomid,
                                             const SegID &segid)
{
    return GroupAtomID<SegID,AtomID>(segid,atomid);
}

GroupAtomID<CGID,AtomID> SireMol::operator+(const CGID &cgid,
                                            const AtomID &atomid)
{
    return GroupAtomID<CGID,AtomID>(cgid,atomid);
}

GroupAtomID<CGID,AtomID> SireMol::operator+(const AtomID &atomid,
                                            const CGID &cgid)
{
    return GroupAtomID<CGID,AtomID>(cgid,atomid);
}
