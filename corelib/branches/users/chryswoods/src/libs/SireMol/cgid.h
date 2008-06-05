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

#ifndef SIREMOL_CGID_H
#define SIREMOL_CGID_H

#include "SireID/id.h"

#include "specify.hpp"
#include "atomsin.hpp"

SIRE_BEGIN_HEADER

namespace SireMol
{

template<class T>
class Selector;

class MolInfo;

class CGIdx;
class CGIdentifier;

class CutGroup;

class Molecules;
class MolGroup;
class MolGroupsBase;
class MolNum;

/** This is the base class of all identifiers that are used 
    to identify a CutGroup

    @author Christopher Woods
*/
class SIREMOL_EXPORT CGID : public SireID::ID
{

public:
    typedef CGIdx Index;
    typedef CGIdentifier Identifier;

    CGID();
    CGID(const CGID &other);

    virtual ~CGID();

    Specify<CGID> operator[](int i) const;
    Specify<CGID> operator()(int i) const;
    Specify<CGID> operator()(int i, int j) const;
    
    AtomsIn<CGID> atoms() const;
    AtomsIn<CGID> atom(int i) const;
    AtomsIn<CGID> atoms(int i, int j) const;
    
    static const char* typeName()
    {
        return "SireMol::CGID";
    }
    
    virtual CGID* clone() const=0;

    /** Map this ID back to the indicies of the CutGroups
        within the molecule described by the info in 'molinfo' */
    virtual QList<CGIdx> map(const MolInfo &molinfo) const=0;

    virtual CutGroup selectFrom(const Molecules &molecules) const;
    virtual QHash< MolNum,Selector<CutGroup> >
                selectAllFrom(const Molecules &molecules) const;

    virtual CutGroup selectFrom(const MolGroup &molgroup) const;
    virtual QHash< MolNum,Selector<CutGroup> >
                selectAllFrom(const MolGroup &molgroup) const;
    
    virtual CutGroup selectFrom(const MolGroupsBase &molgroups) const;
    virtual QHash< MolNum,Selector<CutGroup> > 
                selectAllFrom(const MolGroupsBase &molgroups) const;

};

}

#include "cgidx.h"
#include "cgidentifier.h"
#include "atomidx.h"

Q_DECLARE_METATYPE( SireMol::Specify<SireMol::CGID> );
Q_DECLARE_METATYPE( SireMol::AtomsIn<SireMol::CGID> );

SIRE_END_HEADER

#endif
