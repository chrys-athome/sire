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

#include "qhash_sireff.h"

#include "ffgroupid.h"

#include "SireStream/datastream.h"

using namespace SireFF;
using namespace SireStream;

static const RegisterMetaType<FFGroupID> r_ffgroupid;

/** Serialise to a binary data stream */
QDataStream SIREFF_EXPORT &operator<<(QDataStream &ds, const FFGroupID &ffgroupid)
{
    writeHeader(ds, r_ffgroupid, 1) << ffgroupid.id 
                                    << quint32(ffgroupid.groupid);
    
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREFF_EXPORT &operator>>(QDataStream &ds, FFGroupID &ffgroupid)
{
    VersionID v = readHeader(ds, r_ffgroupid);
    
    if (v == 1)
    {
        quint32 groupid;
    
        ds >> ffgroupid.id >> groupid;
        
        ffgroupid.groupid = FFBase::Group(groupid);
    }
    else
        throw version_error(v, "1", r_ffgroupid, CODELOC);
    
    return ds;
}

/** Hash this groupid */
unsigned int qHash(const FFGroupID &ffgroupid)
{
    return (ffgroupid.ID() << 16) | (ffgroupid.group() & 0x0000FFFF);
}

/** Null constructor */
FFGroupID::FFGroupID()
{}

/** Construct an ID for the specified forcefield and group */
FFGroupID::FFGroupID(ForceFieldID ffid, const FFBase::Group &group)
          : id(ffid), groupid(group)
{}

/** Construct an ID for the specified forcefield and group */
FFGroupID::FFGroupID(const boost::tuple<ForceFieldID,FFBase::Group> &ids)
          : id(ids.get<0>()), groupid(ids.get<1>())
{}

/** Copy constructor */
FFGroupID::FFGroupID(const FFGroupID &other)
          : id(other.id), groupid(other.groupid)
{}

/** Destructor */
FFGroupID::~FFGroupID()
{}

/** Copy assignment */
FFGroupID& FFGroupID::operator=(const FFGroupID &other)
{
    id = other.id;
    groupid = other.groupid;
    
    return *this;
}

/** Comparison operator */
bool FFGroupID::operator==(const FFGroupID &other) const
{
    return id == other.id and groupid == other.groupid;
}

/** Comparison operator */
bool FFGroupID::operator!=(const FFGroupID &other) const
{
    return id != other.id or groupid != other.groupid;
}

/** Return a string representation of this ID */
QString FFGroupID::toString() const
{
    return QString("%1:%2").arg(id).arg(groupid);
}
