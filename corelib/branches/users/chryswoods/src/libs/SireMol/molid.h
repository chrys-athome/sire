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

#include "SireID/idset.hpp"

SIRE_BEGIN_HEADER

namespace SireMol
{

using SireID::IDSet;

class MolIdx;
class MolIdentifier;
class MolNum;

class MolMolID;
class MolAtomID;

class SpecifyMol;

class AtomID;

class Molecules;
class MoleculeGroup;
class MolGroupsBase;

/** This is the base class of all identifiers that are used 
    to identify a Molecule

    @author Christopher Woods
*/
class SIREMOL_EXPORT MolID : public SireID::ID
{
public:
    typedef MolNum Index;
    typedef MolIdentifier Identifier;
    typedef Molecules SearchObject;

    MolID();
    
    MolID(const MolID &other);
    
    virtual ~MolID();

    static const char* typeName()
    {
        return "SireMol::MolID";
    }

    virtual MolID* clone() const=0;
    
    SpecifyMol operator[](int i) const;
    SpecifyMol operator()(int i) const;
    SpecifyMol operator()(int i, int j) const;
    
    MolMolID operator+(const MolID &other) const;
    MolAtomID operator+(const AtomID &other) const;

    IDSet<MolID> operator*(const MolID &other) const;

    MolMolID operator&&(const MolID &other) const;
    MolAtomID operator&&(const AtomID &other) const;
    
    IDSet<MolID> operator||(const MolID &other) const;
    
    virtual QList<MolNum> map(const Molecules &molecules) const=0;
    virtual QList<MolNum> map(const MoleculeGroup &molgroup) const=0;
    virtual QList<MolNum> map(const MolGroupsBase &molgroupsbase) const=0;

protected:
    void processMatches(QList<MolNum> &matches, const Molecules &mols) const;
};

}

namespace SireID
{

using SireMol::MolID;
using SireMol::MolIdentifier;
using SireMol::MolNum;
using SireMol::Molecules;
using SireMol::MoleculeGroup;
using SireMol::MolGroupsBase;

template<>
class IDSet<MolID> : public MolID
{

friend QDataStream& ::operator<<<>(QDataStream&, const IDSet<MolID>&);
friend QDataStream& ::operator>><>(QDataStream&, IDSet<MolID>&);

public:
    IDSet();
    IDSet(const MolID &id);
    IDSet(const MolID &id0, const MolID &id1);
    
    IDSet(const QList<MolIdentifier> &ids);
    
    template<class T>
    IDSet(const T &ids);
    
    IDSet(const IDSet<MolID> &other);
    
    ~IDSet();
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId< IDSet<MolID> >() );
    }
    
    const char* what() const
    {
        return IDSet<MolID>::typeName();
    }
    
    IDSet<MolID>* clone() const
    {
        return new IDSet<MolID>(*this);
    }
    
    bool isNull() const;
    
    uint hash() const;
                
    QString toString() const;

    const QSet<MolIdentifier>& IDs() const;
    
    IDSet<MolID>& operator=(const IDSet<MolID> &other);
    IDSet<MolID>& operator=(const MolID &other);
    
    bool operator==(const SireID::ID &other) const;
    using SireID::ID::operator!=;
   
    bool operator==(const IDSet<MolID> &other) const;
    bool operator!=(const IDSet<MolID> &other) const;
    
    bool operator==(const MolID &other) const;
    bool operator!=(const MolID &other) const;
    
    QList<MolNum> map(const Molecules &mols) const;
    QList<MolNum> map(const MoleculeGroup &molgroup) const;
    QList<MolNum> map(const MolGroupsBase &molgroups) const;

private:
    void add(const MolID &id);
    QList<MolNum> process(QList<MolNum> molnums) const;

    QSet<MolIdentifier> ids;
};

/** Construct from the passed list of IDs */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
IDSet<MolID>::IDSet(const T &new_ids) : MolID()
{
    for (typename T::const_iterator it = new_ids.constBegin();
         it != new_ids.constEnd();
         ++it)
    {
        this->add(*it);
    }
}

}

Q_DECLARE_METATYPE( SireID::IDSet<SireMol::MolID> )

SIRE_EXPOSE_CLASS( SireMol::MolID )
SIRE_EXPOSE_ALIAS( SireID::IDSet<SireMol::MolID>, SireMol::IDSet_MolID_ )

SIRE_END_HEADER

#endif
