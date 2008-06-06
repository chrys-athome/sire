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

#ifndef SIREMOL_RESIDUESIN_HPP
#define SIREMOL_RESIDUESIN_HPP

#include "moleculeinfodata.h"

#include "SireID/index.h"

SIRE_BEGIN_HEADER

namespace SireMol
{

/** This helper class is used to provide the '.residues()' functionality
    of the group ID classes. This allows a class to identify the index to a residue, or
    range of residues by index from the group that has been identified.
    
    @author Christopher Woods
*/
template<class GROUP>
class ResiduesIn : public ResID
{
private:
    static QString typname;

public:
    ResiduesIn(const GROUP &id, qint32 i);
    ResiduesIn(const GROUP &id, qint32 i, qint32 j);
    
    ResiduesIn(const ResiduesIn<GROUP> &other);
    
    ~ResiduesIn();
    
    static const char* typeName()
    {
        return qPrintable(typname);
    }
    
    const char* what() const
    {
        return ResiduesIn<GROUP>::typeName();
    }
    
    ResiduesIn<GROUP>* clone() const
    {
        return new ResiduesIn<GROUP>(*this);
    }
    
    ResiduesIn<GROUP>& operator=(const ResiduesIn<GROUP> &other);

    bool operator==(const ResiduesIn<GROUP> &other) const;
    bool operator==(const SireID::ID &other) const;
    
    bool operator!=(const ResiduesIn<GROUP> &other) const
    {
        return not this->operator==(other);
    }
    
    bool operator!=(const SireID::ID &other) const
    {
        return not this->operator==(other);
    }
    
    bool isNull() const
    {
        return false;
    }
    
    uint hash() const
    {
        return qHash(groupid);
    }
    
    QString toString() const;
    
    QList<ResIDx> map(const MoleculeInfoData &molinfo) const;

private:
    /** The ID of the group that contains the atoms */
    GROUP groupid;

    /** The indicies of the range of atoms that this specifies */
    SireID::Index strt,end;
};

#ifndef SIRE_SKIP_INLINE_FUNCTIONS

template<class GROUP>
QString ResiduesIn<GROUP>::typname 
                    = QString("SireMol::ResiduesIn<%1>").arg(GROUP::typeName());

/** Construct for a specified atom */
template<class GROUP>
SIRE_OUTOFLINE_TEMPLATE
ResiduesIn<GROUP>::ResiduesIn(const GROUP &id, qint32 i)
            : ResID(), groupid(id), strt(i), end(i)
{}

/** Construct for a range of atoms */
template<class GROUP>
SIRE_OUTOFLINE_TEMPLATE
ResiduesIn<GROUP>::ResiduesIn(const GROUP &id, qint32 i, qint32 j)
            : ResID(), groupid(id), strt(i), end(j)
{}

/** Copy constructor */
template<class GROUP>
SIRE_OUTOFLINE_TEMPLATE
ResiduesIn<GROUP>::ResiduesIn(const ResiduesIn<GROUP> &other)
            : ResID(other), groupid(other.groupid), 
              strt(other.strt), end(other.end)
{}

/** Destructor */
template<class GROUP>
SIRE_OUTOFLINE_TEMPLATE
ResiduesIn<GROUP>::~ResiduesIn()
{}

/** Return a string representation of this ID */
template<class GROUP>
SIRE_OUTOFLINE_TEMPLATE
QString ResiduesIn<GROUP>::toString() const
{
    if (strt == end)
        return QString("(%1).residue(%2)").arg(groupid.toString()).arg(strt);
    else if (strt == 0 and end == -1)
        return QString("(%1).residues()").arg(groupid.toString());
    else
        return QString("(%1).residues(%2,%3)")
                    .arg(groupid.toString()).arg(strt).arg(end);
}

/** Copy assignment operator */
template<class GROUP>
SIRE_OUTOFLINE_TEMPLATE
ResiduesIn<GROUP>& ResiduesIn<GROUP>::operator=(const ResiduesIn<GROUP> &other)
{
    if (this != &other)
    {
        ResID::operator=(other);
        groupid = other.groupid;
        strt = other.strt;
        end = other.end;
    }

    return *this;
}

/** Comparison operator */
template<class GROUP>
SIRE_OUTOFLINE_TEMPLATE
bool ResiduesIn<GROUP>::operator==(const ResiduesIn<GROUP> &other) const
{
    return strt == other.strt and end == other.end and
           groupid == other.groupid;
}

/** Comparison operator */
template<class GROUP>
SIRE_OUTOFLINE_TEMPLATE
bool ResiduesIn<GROUP>::operator==(const SireID::ID &other) const
{
    return SireID::ID::compare< ResiduesIn<GROUP> >(*this, other);
}

/** Map this ID to the indicies of matching atoms 

    \throw ???::missing_ID
    \throw SireError::invalid_index
*/
template<class GROUP>
SIRE_OUTOFLINE_TEMPLATE
QList<ResIDx> ResiduesIn<GROUP>::map(const MoleculeInfoData &molinfo) const
{
    //first get the list of the indicies of the matching groups
    QList<typename GROUP::Index> idxs = groupid.map(molinfo);
    
    //now get a list of the indicies of all of the atoms in these groups
    QList<ResIdx> residxs;
    
    foreach (typename GROUP::Index idx, idxs)
    {
        residxs += molinfo.getResiduesIn(idx);
    }
    
    //now map _i and _j to the indicies...
    int nats = residxs.count();
    
    int sane_strt = strt.map(nats);
    int sane_end = end.map(nats);
    
    if (sane_strt > sane_end)
        qSwap(sane_strt, sane_end);
    
    //now extract only the desired atom indicies
    if (sane_end - sane_strt == nats)
    {
        return residxs;
    }
    else
    {
        QList<ResIDx> specified_residxs;
    
        for (int i=sane_strt; i<=sane_end; ++i)
        {
            specified_residxs.append( residxs[i] );
        }
    
        return specified_residxs;
    }
}

#endif //SIRE_SKIP_INLINE_FUNCTIONS

}

SIRE_END_HEADER

#endif
