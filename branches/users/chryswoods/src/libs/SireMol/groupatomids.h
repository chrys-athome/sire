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

#ifndef SIREMOL_GROUPATOMIDS_H
#define SIREMOL_GROUPATOMIDS_H

#include "moleculeinfodata.h"

SIRE_BEGIN_HEADER

namespace SireMol
{

/** This is the base class of GroupAtomID, used to abstract   
    template-independent parts away from the template code.
    
    @author Christopher Woods
*/
class GroupAtomIDBase : public AtomID
{
public:
    GroupAtomIDBase();
    
    GroupAtomIDBase(const GroupAtomIDBase &other);
    
    ~GroupAtomIDBase();
    
protected:
    void throwMissingAtom(const MoleculeInfo &molinfo) const;
};

/** This class represents an Atom ID that is comprised of both 
    an AtomID part and an ID of a group in the molecule (e.g.
    a Residue, Segment, Chain or CutGroup)
    
    @author Christopher Woods
*/
template<class GROUP, class ATOM>
class GroupAtomID : public GroupAtomIDBase
{

static QString typname;

public:
    GroupAtomID();
    
    GroupAtomID(const GROUP &groupid, const ATOM &atomid);
    
    GroupAtomID(const GroupAtomID &other);
    
    ~GroupAtomID();
    
    static const char* typeName()
    {
        return qPrintable(typname);
    }
    
    const char* what() const
    {
        return GroupAtomID<GROUP,ATOM>::typeName();
    }
    
    GroupAtomID<GROUP,ATOM>* clone() const
    {
        return new GroupAtomID<GROUP,ATOM>(*this);
    }
    
    QString toString() const;
    
    QList<AtomIdx> map(const MoleculeInfoData &molinfo);

private:
    typename GROUP::Identifier groupid;
    typename ATOM::Identifier atomid;
};

/** Null constructor */
template<class GROUP, class ATOM>
GroupAtomID<GROUP,ATOM>::GroupAtomID()
                        : GroupAtomIDBase()
{}

/** Construct using the passed group and atom IDs */
template<class GROUP, class ATOM>
GroupAtomID<GROUP,ATOM>::GroupAtomID(const GROUP &group,
                                     const ATOM &atom)
                        : GroupAtomIDBase(),
                          groupid(group), atomid(atom)
{}

/** Copy constructor */
template<class GROUP, class ATOM>
GroupAtomID<GROUP,ATOM>::GroupAtomID(const GroupAtomID<GROUP,ATOM> &groupatomid)
                        : GroupAtomIDBase(other),
                          groupid(other.groupid), atomid(other.atomid)
{}

/** Destructor */
template<class GROUP, class ATOM>
GroupAtomID<GROUP,ATOM>::~GroupAtomID()
{}

/** Return a string representation of this ID */
template<class GROUP, class ATOM>
QString GroupAtomID<GROUP,ATOM>::toString() const
{
    return QString("%1 and %2").arg(groupid.toString(), atomid.toString());
}

/** Map this combined ID back to the indicies of the atoms that match this ID 

    \throw ???::missing_GROUP
    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
template<class GROUP, class ATOM>
QList<AtomIdx> GroupAtomID<GROUP,ATOM>::map(const MoleculeInfoData &molinfo) const
{
    if (this->isNull())
        return molinfo.getAtoms();
    else if (atomid.isNull())
        return groupid.map(molinfo);
    else if (groupid.isNull())
        return atomid.map(molinfo);
    
    QList<AtomIdx> atomidxs = 
                MoleculeInfoData::intersection(atomid.map(molinfo),
                                               molinfo.getAtomsIn(groupid) );
                                             
    if (atomidxs.isEmpty())
        this->throwMissingAtom(molinfo);
            
    qSort(atomidxs);
    return atomidxs;
}

typedef GroupAtomID<ResID,AtomID> ResAtomID;
typedef GroupAtomID<ChainID,AtomID> ChainAtomID;
typedef GroupAtomID<SegID,AtomID> SegAtomID;
typedef GroupAtomID<CGID,AtomID> CGAtomID;

ResAtomID operator+(const ResID &resid, const AtomID &atomid);
ResAtomID operator+(const AtomID &atomid, const ResID &resid);

ChainAtomID operator+(const ChainID &chainid, const AtomID &atomid);
ChainAtomID operator+(const AtomID &atomid, const ChainID &chainid);

SegAtomID operator+(const SegID &segid, const AtomID &atomid);
SegAtomID operator+(const AtomID &atomid, const SegID &segid);

CGAtomID operator+(const CGID &cgid, const AtomID &atomid);
CGAtomID operator+(const AtomID &atomid, const CGID &cgid);

}

SIRE_END_HEADER

#endif
