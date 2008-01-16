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

#ifndef SIREMOL_MOLID_H
#define SIREMOL_MOLID_H

#include "SireID/id.h"

SIRE_BEGIN_HEADER

namespace SireMol
{

class MolIdx;
class MolIdentifier;
class MolNum;

class MolGroup;
class MolGroupsBase;

/** This is the base class of all identifiers that are used 
    to identify a Molecule

    @author Christopher Woods
*/
class SIREMOL_EXPORT MolID : public SireID::ID
{
public:
    typedef MolIdx Index;
    typedef MolIdentifier Identifier;

    MolID();
    
    MolID(const MolID &other);
    
    virtual ~MolID();

    static const char* typeName()
    {
        return "SireMol::MolID";
    }

    virtual MolID* clone() const=0;
    
    virtual QList<MolNum> map(const MolGroup &molgroup) const=0;
    
    virtual QList<MolNum> map(const MolGroupsBase &molgroupsbase) const;
};

class SIREMOL_EXPORT MolNumList : public MolID
{
public:
    MolNumList();
    MolNumList(const QList<MolNum> &molnums);
    
    MolNumList(const MolNumList &other);
    
    ~MolNumList();
    
    static const char* typeName()
    {
        return "SireMol::MolNumList";
    }
    
    const char* what() const
    {
        return MolNumList::typeName();
    }
    
    MolNumList* clone() const
    {
        return new MolNumList(*this);
    }
    
    bool isNull() const;
    
    uint hash() const;
    
    QString toString() const;
    
    MolNumList& operator=(const MolNumList &other);
    
    bool operator==(const SireID::ID &other) const
    {
        return SireID::ID::compare<MolNumList>(*this, other);
    }

    bool operator==(const MolNumList &other) const;
    bool operator!=(const MolNumList &other) const;
    
    QList<MolNum> map(const MolGroup &molgroup) const;
    QList<MolNum> map(const MolGroupsBase &molgroups) const;

private:
    /** List of molecule numbers */
    QList<MolNum> molnums;
};

}

SIRE_END_HEADER

#endif
