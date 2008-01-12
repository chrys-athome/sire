/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
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

#ifndef SIREMOL_ATOMID_H
#define SIREMOL_ATOMID_H

#include "SireID/id.h"

SIRE_BEGIN_HEADER

namespace SireMol
{

template<class ID>
class Specify;

class MoleculeInfoData;
class AtomIdx;
class AtomIdentifier;

/** This is the base class of all identifiers that are used 
    to identify an atom

    @author Christopher Woods
*/
class SIREMOL_EXPORT AtomID : public SireID::ID
{
public:
    typedef AtomIdx Index;
    typedef AtomIdentifier Identifier;

    AtomID();
    AtomID(const AtomID &other);
    
    virtual ~AtomID();
    
    Specify<AtomID> operator[](int i) const;
    Specify<AtomID> operator()(int i) const;
    Specify<AtomID> operator()(int i, int j) const;

    static const char* typeName()
    {
        return "SireMol::AtomID";
    }

    virtual AtomID* clone() const=0;

    /** Map this ID back to the indicies of the matching atoms in the molecule, 
        using the passed MoleculeInfo to do the mapping */
    virtual QList<AtomIdx> map(const MoleculeInfoData &molinfo) const=0;
    
    virtual QHash< MolNum,Selector<Atom> > 
                selectAllFrom(const MolGroupsBase &molgroups) const;
};

}

Q_DECLARE_METATYPE( SireMol::Specify<SireMol::AtomID> );

SIRE_END_HEADER

#endif
