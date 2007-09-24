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

#ifndef SIREMOL_SPECIFY_HPP
#define SIREMOL_SPECIFY_HPP

#include "moleculeinfodata.h"

SIRE_BEGIN_HEADER

namespace SireMol
{

/** This class is used to help form specified ID matches, 
    e.g. the third residue called alanine ( ResName("ALA")[2] )
    or the last three atoms called "CA" ( AtomName("CA")(-3,-1) )
    
    @author Christopher Woods
*/
template<class ID>
class Specify : public ID
{
public:
    Specify(const ID &id, int i);
    Specify(const ID &id, int i, int j);
    
    Specify(const Specify<ID> &other);
    
    ~Specify();
    
    static const char* typeName()
    {
        return qPrintable(typname);
    }
    
    const char* what() const
    {
        return Specify<ID>::typeName();
    }
    
    Specify<ID,IDX>* clone() const
    {
        return new Specify<ID>(*this);
    }
    
    QString toString() const;
    
    QList<ID::Index> map(const MoleculeInfoData &molinfo) const;

private:
    int strt, end;
};

/** Map this ID to the indicies of the matching parts of the molecule */
template<class ID>
QList<ID::Index> Specify<ID>::map(const MoleculeInfoData &molinfo) const
{
    //first get all of the matches
    QList<ID::Index> idxs = ID::map(molinfo);
    
    //now get the specified matches
    int nmatches = idxs.count();
    
    this->sanitise(strt, end, nmatches);
    
    if (end - strt == nmatches)
        return idxs;
    else
    {
        QList<ID::Index> specified_idxs;
    
        for (int i=strt; i<=end; ++i)
        {
            specified_idxs.append(idxs[i]);
        }
        
        return specified_idxs;
    }
}

}

SIRE_END_HEADER

#endif
