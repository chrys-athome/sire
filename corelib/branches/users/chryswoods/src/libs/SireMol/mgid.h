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

#ifndef SIREMOL_MGID_H
#define SIREMOL_MGID_H

#include "SireID/id.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class MGNumList;
}

QDataStream& operator<<(QDataStream&, const SireMol::MGNumList&);
QDataStream& operator>>(QDataStream&, SireMol::MGNumList&);

namespace SireMol
{

class MGIdx;
class MGIdentifier;
class MGNum;

class MolGroupsBase;

/** This is the base class of all identifiers that are used 
    to identify a MoleculeGroup

    @author Christopher Woods
*/
class SIREMOL_EXPORT MGID : public SireID::ID
{
public:
    typedef MGIdx Index;
    typedef MGIdentifier Identifier;

    MGID();
    
    MGID(const MGID &other);
    
    virtual ~MGID();

    static const char* typeName()
    {
        return "SireMol::MGID";
    }

    virtual MGID* clone() const=0;

    virtual QList<MGNum> map(const MolGroupsBase &molgroups) const=0;
};

class SIREMOL_EXPORT MGNumList : public MGID
{

friend QDataStream& ::operator<<(QDataStream&, const MGNumList&);
friend QDataStream& ::operator>>(QDataStream&, MGNumList&);

public:
    MGNumList();
    MGNumList(const QList<MGNum> &mgnums);
    
    MGNumList(const MGNumList &other);
    
    ~MGNumList();
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<MGNumList>() );
    }
    
    const char* what() const
    {
        return MGNumList::typeName();
    }
    
    MGNumList* clone() const
    {
        return new MGNumList(*this);
    }
    
    bool isNull() const;
    
    uint hash() const;
    
    QString toString() const;
    
    MGNumList& operator=(const MGNumList &other);
    
    bool operator==(const SireID::ID &other) const
    {
        return SireID::ID::compare<MGNumList>(*this, other);
    }

    bool operator==(const MGNumList &other) const;
    bool operator!=(const MGNumList &other) const;
    
    QList<MGNum> map(const MolGroupsBase &molgroups) const;

private:
    /** List of molecule group numbers */
    QList<MGNum> mgnums;
};

}

Q_DECLARE_METATYPE( SireMol::MGNumList )

SIRE_EXPOSE_CLASS( SireMol::MGID )
SIRE_EXPOSE_CLASS( SireMol::MGNumList )

SIRE_END_HEADER

#endif
