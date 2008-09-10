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

#ifndef SIREMOL_MOLECULEGROUPS_H
#define SIREMOL_MOLECULEGROUPS_H

#include "molgroups.h"

#include "SireBase/property.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class MoleculeGroups;
}

QDataStream& operator<<(QDataStream&, const SireMol::MoleculeGroups&);
QDataStream& operator>>(QDataStream&, SireMol::MoleculeGroups&);

namespace SireMol
{

/** This is the polymorphic pointer holder for the 
    MoleculeGroups (MolGroupsBase derived) classes.
    
    MoleculeGroups are objects that hold lots of individual
    MoleculeGroup objects (which themselves hold lots
    of individual views of different molecules).
    Examples of classes derived from MolGroupsBase
    are the forcefield classes and the system classes.
    
    @author Christopher Woods
*/
class SIREMOL_EXPORT MoleculeGroups : public SireBase::Property
{

friend QDataStream& ::operator<<(QDataStream&, const MoleculeGroups&);
friend QDataStream& ::operator>>(QDataStream&, MoleculeGroups&);

public:
    MoleculeGroups();
    MoleculeGroups(const PropertyBase &property);
    MoleculeGroups(const MolGroupsBase &molgroup);

    MoleculeGroups(const MoleculeGroups &other);
    
    ~MoleculeGroups();
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<MoleculeGroups>() );
    }
    
    const char* what() const
    {
        return MoleculeGroups::typeName();
    }
    
    virtual MoleculeGroups& operator=(const PropertyBase &property);
    virtual MoleculeGroups& operator=(const MolGroupsBase &other);

    const MolGroupsBase* operator->() const;
    const MolGroupsBase& operator*() const;
    
    const MolGroupsBase& read() const;
    MolGroupsBase& edit();
    
    const MolGroupsBase* data() const;
    const MolGroupsBase* constData() const;
    
    MolGroupsBase* data();
    
    operator const MolGroupsBase&() const;

    static const MoleculeGroups& shared_null();
};

}

Q_DECLARE_METATYPE(SireMol::MoleculeGroups);

SIRE_EXPOSE_PROPERTY( SireMol::MoleculeGroups, SireMol::MolGroupsBase )

SIRE_END_HEADER

#endif
