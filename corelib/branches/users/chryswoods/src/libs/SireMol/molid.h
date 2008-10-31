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
class MolNumList;
}

QDataStream& operator<<(QDataStream&, const SireMol::MolNumList&);
QDataStream& operator>>(QDataStream&, SireMol::MolNumList&);

namespace SireMol
{

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
    
    virtual QList<MolNum> map(const Molecules &molecules) const=0;
    virtual QList<MolNum> map(const MoleculeGroup &molgroup) const=0;
    virtual QList<MolNum> map(const MolGroupsBase &molgroupsbase) const=0;
};

class SIREMOL_EXPORT MolNumList : public MolID
{

friend QDataStream& ::operator<<(QDataStream&, const MolNumList&);
friend QDataStream& ::operator>>(QDataStream&, MolNumList&);

public:
    MolNumList();
    MolNumList(const QList<MolNum> &molnums);
    
    MolNumList(const MolNumList &other);
    
    ~MolNumList();
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<MolNumList>() );
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
    
    QList<MolNum> map(const Molecules &molecules) const;
    QList<MolNum> map(const MoleculeGroup &molgroup) const;
    QList<MolNum> map(const MolGroupsBase &molgroups) const;

private:
    /** List of molecule numbers */
    QList<MolNum> molnums;
};

}

Q_DECLARE_METATYPE( SireMol::MolNumList )

SIRE_EXPOSE_CLASS( SireMol::MolID )
SIRE_EXPOSE_CLASS( SireMol::MolNumList )

SIRE_END_HEADER

#endif
