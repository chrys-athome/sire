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

SIRE_BEGIN_HEADER

namespace SireMol
{

/** This helper class is used to provide the '.atoms()' functionality
    of the group ID classes. This allows the class to an atom, or
    range of atoms by index from the group that has been identified.
    
    @author Christopher Woods
*/
template<class ID>
class AtomsIn : public ID
{
public:
    AtomsIn(int i, const ID &id) : ID(id), strt(i), end(i)
    {}
    
    AtomsIn(int i, int j, const ID &id) : ID(id), strt(i), end(j)
    {}
    
    AtomsIn(const AtomsIn<ID> &other)
          : ID(other), strt(other.strt), end(other.end)
    {}
    
    ~AtomsIn()
    {}
    
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
    
    QString toString() const;
    
    QList<AtomIdx> map(const MoleculeInfoData &molinfo) const;

private:
    /** The indicies of the range of atoms that this specifies */
    int strt,end;
};

/** Return a string representation of this ID */
template<class ID>
QString AtomsIn<ID>::toString() const
{
    if (strt == end)
        return QString("(%1).atom(%2)").arg(ID::toString()).arg(strt);
    else
        return QString("(%1).atoms(%2,%3)")
                    .arg(ID::toString()).arg(strt).arg(end);
}

/** Map this ID to the indicies of matching atoms 

    \throw ???::missing_ID
    \throw SireError::invalid_index
*/
template<class ID>
QList<AtomIdx> AtomsIn<ID>::map(const MoleculeInfoData &molinfo) const
{
    //first get the list of atom indicies from the base class...
    QList<ID::Index> idxs = ID::map(molinfo);
    
    QList<AtomIdx> atomidxs;
    
    foreach (ID::Index idx, idxs)
    {
        atomidxs.append( molinfo.getAtomsIn(idx) );
    }
    
    //now map _i and _j to the indicies...
    int nats = atomidxs.count();
    
    this->sanitise(strt, end, nats);
    
    QList<AtomIdx> specified_atomidxs;
    
    for (int i=strt, i<=end; ++i)
    {
        specified_atomidxs.append( atomidxs[i] );
    }
    
    return specified_atomidxs;
}

}

SIRE_END_HEADER

#endif
