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

#ifndef SIREMOL_SEGID_H
#define SIREMOL_SEGID_H

#include "SireID/id.h"

#include "atomsin.hpp"
#include "specify.hpp"
#include "idset.hpp"

SIRE_BEGIN_HEADER

namespace SireMol
{

template<class T>
class Selector;

template<class GROUPID, class ATOMID>
class GroupAtomID;

template<class G0, class G1>
class GroupGroupID;

class SegSegID;

class MolInfo;

class SegIdx;
class SegIdentifier;

class Segment;

class Molecules;
class MoleculeGroup;
class MolGroupsBase;
class MolNum;

/** This is the base class of all identifiers that are used 
    to identify a Segment within a Molecule

    @author Christopher Woods
*/
class SIREMOL_EXPORT SegID : public SireID::ID
{

public:
    typedef SegIdx Index;
    typedef SegIdentifier Identifier;
    typedef MolInfo SearchObject;

    SegID();

    SegID(const SegID &other);

    virtual ~SegID();

    Specify<SegID> operator[](int i) const;
    Specify<SegID> operator()(int i) const;
    Specify<SegID> operator()(int i, int j) const;

    SegSegID operator+(const SegID &other) const;
    GroupAtomID<SegID,AtomID> operator+(const AtomID &other) const;
    GroupGroupID<SegID,CGID> operator+(const CGID &other) const;
    GroupGroupID<SegID,ResID> operator+(const ResID &other) const;
    GroupGroupID<SegID,ChainID> operator+(const ChainID &other) const;

    SegSegID operator&&(const SegID &other) const;
    GroupAtomID<SegID,AtomID> operator&&(const AtomID &other) const;
    GroupGroupID<SegID,CGID> operator&&(const CGID &other) const;
    GroupGroupID<SegID,ResID> operator&&(const ResID &other) const;
    GroupGroupID<SegID,ChainID> operator&&(const ChainID &other) const;

    IDSet<SegID> operator*(const SegID &other) const;
    IDSet<SegID> operator||(const SegID &other) const;

    AtomsIn<SegID> atoms() const;
    AtomsIn<SegID> atom(int i) const;
    AtomsIn<SegID> atoms(int i, int j) const;

    static const char* typeName()
    {
        return "SireMol::SegID";
    }

    virtual SegID* clone() const=0;

    /** Map this ID back to the indicies of the segments in the molecule, 
        using the passed MoleculeInfo to do the mapping */
    virtual QList<SegIdx> map(const MolInfo &molinfo) const=0;

    virtual Segment selectFrom(const Molecules &molecules) const;
    virtual QHash< MolNum,Selector<Segment> >
                selectAllFrom(const Molecules &molecules) const;

    virtual Segment selectFrom(const MoleculeGroup &molgroup) const;
    virtual QHash< MolNum,Selector<Segment> >
                selectAllFrom(const MoleculeGroup &molgroup) const;
    
    virtual Segment selectFrom(const MolGroupsBase &molgroups) const;
    virtual QHash< MolNum,Selector<Segment> > 
                selectAllFrom(const MolGroupsBase &molgroups) const;

protected:
    void processMatches(QList<SegIdx> &matches, const MolInfo &molinfo) const;
};

}

#include "segidentifier.h"
#include "segidx.h"
#include "atomidx.h"

Q_DECLARE_METATYPE( SireMol::Specify<SireMol::SegID> );
Q_DECLARE_METATYPE( SireMol::AtomsIn<SireMol::SegID> );
Q_DECLARE_METATYPE( SireMol::IDSet<SireMol::SegID> );

SIRE_EXPOSE_CLASS( SireMol::SegID )
SIRE_EXPOSE_ALIAS( SireMol::Specify<SireMol::SegID>, SireMol::Specify_SegID_ )
SIRE_EXPOSE_ALIAS( SireMol::AtomsIn<SireMol::SegID>, SireMol::AtomsIn_SegID_ )
SIRE_EXPOSE_ALIAS( SireMol::IDSet<SireMol::SegID>, SireMol::IDSet_SegID_ )

#ifdef SIRE_INSTANTIATE_TEMPLATES
template class SireMol::Specify<SireMol::SegID>;
template class SireMol::AtomsIn<SireMol::SegID>;
template class SireMol::IDSet<SireMol::SegID>;
#endif

SIRE_END_HEADER

#endif
