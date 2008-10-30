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

#include <QSet>

#include "chainchainid.h"
#include "chainidx.h"
#include "molinfo.h"

#include "SireMol/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMol;
using namespace SireStream;

static const RegisterMetaType<ChainChainID> r_chainchainid;

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const ChainChainID &chainchainid)
{
    writeHeader(ds, r_chainchainid, 1);
    
    SharedDataStream sds(ds);
    
    sds << chainchainid.chainid0 << chainchainid.chainid1;
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, ChainChainID &chainchainid)
{
    VersionID v = readHeader(ds, r_chainchainid);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> chainchainid.chainid0 >> chainchainid.chainid1;
    }
    else
        throw version_error( v, "1", r_chainchainid, CODELOC );
        
    return ds;
}

/** Constructor */
ChainChainID::ChainChainID() : ChainID()
{}

/** Construct from two ChainIDs */
ChainChainID::ChainChainID(const ChainID &id0, const ChainID &id1)
             : ChainID(), chainid0(id0), chainid1(id1)
{}

/** Copy constructor */
ChainChainID::ChainChainID(const ChainChainID &other)
             : ChainID(other),
               chainid0(other.chainid0), chainid1(other.chainid1)
{}

/** Destructor */
ChainChainID::~ChainChainID()
{}

/** Return a string representation of this ID */
QString ChainChainID::toString() const
{
    if (this->isNull())
        return "*";
    else if (chainid0.isNull())
        return chainid1.toString();
    else if (chainid1.isNull())
        return chainid0.toString();
    else
        return QString("%1 and %2").arg(chainid0.toString(), chainid1.toString());
}

/** Map this pair of Chain IDs to the indicies of matching chains

    \throw SireMol::missing_chain
    \throw SireError::invalid_index
*/
QList<ChainIdx> ChainChainID::map(const MolInfo &molinfo) const
{
    if (this->isNull())
        return molinfo.getChains();
    else if (chainid0.isNull())
        return chainid1.map(molinfo);
    else if (chainid1.isNull())
        return chainid0.map(molinfo);
    else
    {
        QList<ChainIdx> chainidxs = 
                          MolInfo::intersection( chainid0.map(molinfo),
                                                 chainid1.map(molinfo) );
                
        if (chainidxs.isEmpty())
            throw SireMol::missing_chain( QObject::tr(
                "There is no chain matching the ID %1.")
                    .arg(this->toString()), CODELOC );
    
        qSort(chainidxs);
        
        return chainidxs;
    }
}
