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

#ifndef SIREMOL_RESID_H
#define SIREMOL_RESID_H

#include "SireID/id.h"

#include "specify.hpp"
#include "atomsin.hpp"

SIRE_BEGIN_HEADER

namespace SireMol
{

template<class T>
class Selector;

class MolInfo;

class ResIdx;
class ResIdentifier;

class Residue;

class Molecules;
class MolGroup;
class MolGroupsBase;
class MolNum;

/** This is the base class of all identifiers that are used 
    to identify a residue within a molecule

    @author Christopher Woods
*/
class SIREMOL_EXPORT ResID : public SireID::ID
{

public:
    typedef ResIdx Index;
    typedef ResIdentifier Identifier;

    ResID();

    ResID(const ResID &other);

    virtual ~ResID();
    
    Specify<ResID> operator[](int i) const;
    Specify<ResID> operator()(int i) const;
    Specify<ResID> operator()(int i, int j) const;

    AtomsIn<ResID> atoms() const;
    AtomsIn<ResID> atom(int i) const;
    AtomsIn<ResID> atoms(int i, int j) const;
    
    static const char* typeName()
    {
        return "SireMol::ResID";
    }
    
    virtual ResID* clone() const=0;

    /** Map this ID back to the indicies of the residues in the molecule, 
        using the passed MoleculeInfo to do the mapping */
    virtual QList<ResIdx> map(const MolInfo &molinfo) const=0;

    virtual Residue selectFrom(const Molecules &molecules) const;
    virtual QHash< MolNum,Selector<Residue> >
                selectAllFrom(const Molecules &molecules) const;

    virtual Residue selectFrom(const MolGroup &molgroup) const;
    virtual QHash< MolNum,Selector<Residue> >
                selectAllFrom(const MolGroup &molgroup) const;
    
    virtual Residue selectFrom(const MolGroupsBase &molgroups) const;
    virtual QHash< MolNum,Selector<Residue> > 
                selectAllFrom(const MolGroupsBase &molgroups) const;

};

}

#include "residentifier.h"
#include "residx.h"
#include "atomidx.h"

Q_DECLARE_METATYPE( SireMol::Specify<SireMol::ResID> );
Q_DECLARE_METATYPE( SireMol::AtomsIn<SireMol::ResID> );

SIRE_END_HEADER

#endif

