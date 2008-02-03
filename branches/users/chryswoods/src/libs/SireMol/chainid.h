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

#ifndef SIREMOL_CHAINID_H
#define SIREMOL_CHAINID_H

#include "SireID/id.h"

#include "specify.hpp"
#include "atomsin.hpp"
#include "resin.hpp"

SIRE_BEGIN_HEADER

namespace SireMol
{

template<class T>
class Selector;

class MolInfo;

class ChainIdx;
class ChainIdentifier;

class Chain;

class Molecules;
class MolGroup;
class MolGroupsBase;
class MolNum;

/** This is the base class of all identifiers that are used 
    to identify a chain within a molecule

    @author Christopher Woods
*/
class SIREMOL_EXPORT ChainID : public SireID::ID
{

public:
    typedef ChainIdx Index;
    typedef ChainIdentifier Identifier;

    ChainID();
    ChainID(const ChainID &other);

    virtual ~ChainID();
    
    Specify<ChainID> operator[](int i) const;
    Specify<ChainID> operator()(int i) const;
    Specify<ChainID> operator()(int i, int j) const;
    
    AtomsIn<ChainID> atoms() const;
    AtomsIn<ChainID> atom(int i) const;
    AtomsIn<ChainID> atoms(int i, int j) const;
    
    ResIn<ChainID> residues() const;
    ResIn<ChainID> residue(int i) const;
    ResIn<ChainID> residues(int i, int j) const;
    
    static const char* typeName()
    {
        return "SireMol::ChainID";
    }
    
    virtual ChainID* clone() const=0;

    /** Map this ID back to the indicies of the chains in the molecule, 
        using the passed MoleculeInfo to do the mapping */
    virtual QList<ChainIdx> map(const MolInfo &molinfo) const=0;

    virtual Chain selectFrom(const Molecules &molecules) const;
    virtual QHash< MolNum,Selector<Chain> >
                    selectAllFrom(const Molecules &molecules) const;

    virtual Chain selectFrom(const MolGroup &molgroup) const;
    virtual QHash< MolNum,Selector<Chain> >
                    selectAllFrom(const MolGroup &molgroup) const;
    
    virtual Chain selectFrom(const MolGroupsBase &molgroups) const;
    virtual QHash< MolNum,Selector<Chain> > 
                    selectAllFrom(const MolGroupsBase &molgroups) const;
};

}

#include "chainidentifier.h"
#include "chainidx.h"
#include "residx.h"
#include "atomidx.h"

Q_DECLARE_METATYPE( SireMol::Specify<SireMol::ChainID> );
Q_DECLARE_METATYPE( SireMol::AtomsIn<SireMol::ChainID> );
Q_DECLARE_METATYPE( SireMol::ResIn<SireMol::ChainID> );

SIRE_END_HEADER

#endif
