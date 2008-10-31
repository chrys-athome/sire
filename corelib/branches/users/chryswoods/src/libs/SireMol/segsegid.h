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

#ifndef SIREMOL_SEGSEGID_H
#define SIREMOL_SEGSEGID_H

#include "segidentifier.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class SegSegID;
}

QDataStream& operator<<(QDataStream&, const SireMol::SegSegID&);
QDataStream& operator>>(QDataStream&, SireMol::SegSegID&);

namespace SireMol
{

/** This class holds a pair of SegIDs, which are used together
    to identify atom(s)
    
    @author Christopher Woods
*/
class SIREMOL_EXPORT SegSegID : public SegID
{

friend QDataStream& ::operator<<(QDataStream&, const SegSegID&);
friend QDataStream& ::operator>>(QDataStream&, SegSegID&);

public:
    SegSegID();
    
    SegSegID(const SegID &id0, const SegID &id1);
    
    SegSegID(const SegSegID &other);
    
    ~SegSegID();
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<SegSegID>() );
    }
    
    const char* what() const
    {
        return SegSegID::typeName();
    }

    SegSegID* clone() const
    {
        return new SegSegID(*this);
    }

    QString toString() const;

    bool operator==(const SegSegID &other) const
    {
        return (segid0 == other.segid0 and segid1 == other.segid1) or
               (segid0 == other.segid1 and segid1 == other.segid0);
    }

    bool operator!=(const SegSegID &other) const
    {
        return not this->operator==(other);
    }

    bool operator==(const SireID::ID &other) const
    {
        return SireID::ID::compare<SegSegID>(*this, other);
    }

    uint hash() const
    {
        return segid0.hash() + segid1.hash();
    }

    bool isNull() const
    {
        return segid0.isNull() and segid1.isNull();
    }

    QList<SegIdx> map(const MolInfo &molinfo) const;

private:
    /** The pair of IDs that are combined */
    SegIdentifier segid0, segid1;
};

}

Q_DECLARE_METATYPE(SireMol::SegSegID);

SIRE_EXPOSE_CLASS( SireMol::SegSegID )

SIRE_END_HEADER

#endif
