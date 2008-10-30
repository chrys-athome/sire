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

#ifndef SIREMOL_CHAINCHAINID_H
#define SIREMOL_CHAINCHAINID_H

#include "chainidentifier.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class ChainChainID;
}

QDataStream& operator<<(QDataStream&, const SireMol::ChainChainID&);
QDataStream& operator>>(QDataStream&, SireMol::ChainChainID&);

namespace SireMol
{

/** This class holds a pair of ChainIDs, which are used together
    to identify chain(s)
    
    @author Christopher Woods
*/
class SIREMOL_EXPORT ChainChainID : public ChainID
{

friend QDataStream& ::operator<<(QDataStream&, const ChainChainID&);
friend QDataStream& ::operator>>(QDataStream&, ChainChainID&);

public:
    ChainChainID();
    
    ChainChainID(const ChainID &id0, const ChainID &id1);
    
    ChainChainID(const ChainChainID &other);
    
    ~ChainChainID();
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<ChainChainID>() );
    }
    
    const char* what() const
    {
        return ChainChainID::typeName();
    }

    ChainChainID* clone() const
    {
        return new ChainChainID(*this);
    }

    QString toString() const;

    bool operator==(const ChainChainID &other) const
    {
        return (chainid0 == other.chainid0 and chainid1 == other.chainid1) or
               (chainid0 == other.chainid1 and chainid1 == other.chainid0);
    }

    bool operator!=(const ChainChainID &other) const
    {
        return not this->operator==(other);
    }

    bool operator==(const SireID::ID &other) const
    {
        return SireID::ID::compare<ChainChainID>(*this, other);
    }

    uint hash() const
    {
        return chainid0.hash() + chainid1.hash();
    }

    bool isNull() const
    {
        return chainid0.isNull() and chainid1.isNull();
    }

    QList<ChainIdx> map(const MolInfo &molinfo) const;

private:
    /** The pair of IDs that are combined */
    ChainIdentifier chainid0, chainid1;
};

}

Q_DECLARE_METATYPE(SireMol::ChainChainID);

SIRE_EXPOSE_CLASS( SireMol::ChainChainID )

SIRE_END_HEADER

#endif
