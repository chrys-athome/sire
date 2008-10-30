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

#ifndef SIREMOL_CGCGID_H
#define SIREMOL_CGCGID_H

#include "cgidentifier.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class CGCGID;
}

QDataStream& operator<<(QDataStream&, const SireMol::CGCGID&);
QDataStream& operator>>(QDataStream&, SireMol::CGCGID&);

namespace SireMol
{

/** This class holds a pair of CGIDs, which are used together
    to identify CutGroup(s)
    
    @author Christopher Woods
*/
class SIREMOL_EXPORT CGCGID : public CGID
{

friend QDataStream& ::operator<<(QDataStream&, const CGCGID&);
friend QDataStream& ::operator>>(QDataStream&, CGCGID&);

public:
    CGCGID();
    
    CGCGID(const CGID &id0, const CGID &id1);
    
    CGCGID(const CGCGID &other);
    
    ~CGCGID();
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<CGCGID>() );
    }
    
    const char* what() const
    {
        return CGCGID::typeName();
    }

    CGCGID* clone() const
    {
        return new CGCGID(*this);
    }

    QString toString() const;

    bool operator==(const CGCGID &other) const
    {
        return (cgid0 == other.cgid0 and cgid1 == other.cgid1) or
               (cgid0 == other.cgid1 and cgid1 == other.cgid0);
    }

    bool operator!=(const CGCGID &other) const
    {
        return not this->operator==(other);
    }

    bool operator==(const SireID::ID &other) const
    {
        return SireID::ID::compare<CGCGID>(*this, other);
    }

    uint hash() const
    {
        return cgid0.hash() + cgid1.hash();
    }

    bool isNull() const
    {
        return cgid0.isNull() and cgid1.isNull();
    }

    QList<CGIdx> map(const MolInfo &molinfo) const;

private:
    /** The pair of IDs that are combined */
    CGIdentifier cgid0, cgid1;
};

}

Q_DECLARE_METATYPE(SireMol::CGCGID);

SIRE_EXPOSE_CLASS( SireMol::CGCGID )

SIRE_END_HEADER

#endif
