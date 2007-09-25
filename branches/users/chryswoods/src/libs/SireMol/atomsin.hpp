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

#ifndef SIREMOL_ATOMSIN_HPP
#define SIREMOL_ATOMSIN_HPP

#include "moleculeinfodata.h"

#include "SireID/index.h"

SIRE_BEGIN_HEADER

namespace SireMol
{

/** This helper class is used to provide the '.atoms()' functionality
    of the group ID classes. This allows the class to an atom, or
    range of atoms by index from the group that has been identified.
    
    @author Christopher Woods
*/
template<class ID>
class AtomsIn : public AtomID
{
public:
    AtomsIn(const ID &id, SireID::Index i);
    AtomsIn(const ID &id, SireID::Index i, SireID::Index j);
    
    AtomsIn(const AtomsIn<ID> &other);
    
    ~AtomsIn();
    
    static const char* typeName()
    {
        return qPrintable(typname);
    }
    
    const char* what() const
    {
        return AtomsIn<ID>::typeName();
    }
    
    AtomsIn<ID>* clone() const
    {
        return new AtomsIn<ID>(*this);
    }
    
    AtomsIn<ID>& operator=(const AtomsIn<ID> &other);

    bool operator==(const AtomsIn<ID> &other) const;
    bool operator==(const SireID::ID &other) const;
    
    bool operator!=(const AtomsIn<ID> &other) const
    {
        return not this->operator==(other);
    }
    
    bool operator!=(const SireID::ID &other) const
    {
        return not this->operator==(other);
    }
    
    uint hash() const
    {
        return qHash(groupid);
    }
    
    QString toString() const;
    
    QList<AtomIdx> map(const MoleculeInfoData &molinfo) const;

private:
    /** The ID of the group that contains the atoms */
    ID groupid;

    /** The indicies of the range of atoms that this specifies */
    SireID::Index strt,end;
};

/** Construct for a specified atom */
template<class ID>
SIRE_OUTOFLINE_TEMPLATE
AtomsIn<ID>::AtomsIn(const ID &id, SireID::Index i)
            : AtomID(), group(id), strt(i), end(i)
{}

/** Construct for a range of atoms */
template<class ID>
SIRE_OUTOFLINE_TEMPLATE
AtomsIn<ID>::AtomsIn(const ID &id, SireID::Index i, SireID:Index j)
            : AtomID(), groupid(id), strt(i), end(j)
{}

/** Copy constructor */
template<class ID>
SIRE_OUTOFLINE_TEMPLATE
AtomsIn<ID>::AtomsIn(const AtomsIn<ID> &other)
            : AtomID(other), groupid(other.groupid), 
              strt(other.strt), end(other.end)
{}

/** Destructor */
template<class ID>
SIRE_OUTOFLINE_TEMPLATE
AtomsIn<ID>::~AtomsIn()
{}

/** Return a string representation of this ID */
template<class ID>
SIRE_OUTOFLINE_TEMPLATE
QString AtomsIn<ID>::toString() const
{
    if (strt == end)
        return QString("(%1).atom(%2)").arg(groupid.toString()).arg(strt);
    else
        return QString("(%1).atoms(%2,%3)")
                    .arg(groupid.toString()).arg(strt).arg(end);
}

/** Copy assignment operator */
template<class ID>
SIRE_OUTOFLINE_TEMPLATE
AtomsIn<ID>& AtomsIn<ID>::operator=(const AtomsIn<ID> &other)
{
    if (this != &other)
    {
        AtomID::operator=(other);
        groupid = other.groupid;
        strt = other.strt;
        end = other.end;
    }

    return *this;
}

/** Comparison operator */
template<class ID>
SIRE_OUTOFLINE_TEMPLATE
bool AtomsIn<ID>::operator==(const AtomsIn<ID> &other) const
{
    return strt == other.strt and end == other.end and
           groupid == other.groupid;
}

/** Comparison operator */
template<class ID>
SIRE_OUTOFLINE_TEMPLATE
bool AtomsIn<ID>::operator==(const SireID::ID &other) const
{
    return SireID::ID::compare< AtomsIn<ID> >(*this, other);
}

/** Map this ID to the indicies of matching atoms 

    \throw ???::missing_ID
    \throw SireError::invalid_index
*/
template<class ID>
SIRE_OUTOFLINE_TEMPLATE
QList<AtomIdx> AtomsIn<ID>::map(const MoleculeInfoData &molinfo) const
{
    //first get the list of atom indicies from the base class...
    QList<ID::Index> idxs = groupid.map(molinfo);
    
    QList<AtomIdx> atomidxs;
    
    foreach (ID::Index idx, idxs)
    {
        atomidxs.append( molinfo.getAtomsIn(idx) );
    }
    
    //now map _i and _j to the indicies...
    int nats = atomidxs.count();
    
    int sane_strt = strt.map(nats);
    int sane_end = end.map(nats);
    
    if (sane_strt > sane_end)
        qSwap(sane_strt, sane_end);
    
    if (sane_end - sane_strt = nats)
    {
        return atomidxs;
    }
    else
    {
        QList<AtomIdx> specified_atomidxs;
    
        for (int i=sane_strt, i<=sane_end; ++i)
        {
            specified_atomidxs.append( atomidxs[i] );
        }
    
        return specified_atomidxs;
    }
}

}

SIRE_END_HEADER

#endif
