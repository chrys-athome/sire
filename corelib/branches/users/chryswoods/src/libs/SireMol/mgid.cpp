/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2008  Christopher Woods
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

#include "mgid.h"
#include "mgidx.h"
#include "mgname.h"
#include "mgnum.h"
#include "mgidentifier.h"
#include "moleculegroups.h"

#include "SireBase/incremint.h"

#include "SireMol/errors.h"
#include "SireError/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include "tostring.h"

using namespace SireMol;
using namespace SireBase;
using namespace SireStream;

////////
//////// Implementation of MGID
////////

MGID::MGID() : SireID::ID()
{}

MGID::MGID(const MGID &other) : SireID::ID(other)
{}

MGID::~MGID()
{}

MGMGID MGID::operator+(const MGID &other) const
{
    return MGMGID(*this, other);
}

MGMGID MGID::operator&&(const MGID &other) const
{
    return this->operator+(other);
}

IDSet<MGID> MGID::operator*(const MGID &other) const
{
    return IDSet<MGID>(*this, other);
}

IDSet<MGID> MGID::operator||(const MGID &other) const
{
    return this->operator*(other);
}

void MGID::processMatches(QList<MGNum> &matches, const MolGroupsBase&) const
{
    if (matches.isEmpty())
        throw SireMol::missing_group( QObject::tr(
                "There is no group in the passed groups that matches "
                "the ID \"%1\".")
                    .arg(this->toString()), CODELOC );
}

////////
//////// Implementation of MGIdx
////////

MGIdx::MGIdx() : SireID::Index_T_<MGIdx>(), MGID()
{}

MGIdx::MGIdx(qint32 idx) : SireID::Index_T_<MGIdx>(idx), MGID()
{}

MGIdx::MGIdx(const MGIdx &other) : SireID::Index_T_<MGIdx>(other), MGID(other)
{}

MGIdx::~MGIdx()
{}

QList<MGNum> MGIdx::map(const MolGroupsBase &molgroups) const
{
    return molgroups.map(*this);
}

////////
//////// Implementation of MGName
////////

MGName::MGName() : SireID::Name(), MGID()
{}

MGName::MGName(const QString &name) : SireID::Name(name), MGID()
{}

MGName::MGName(const MGName &other) : SireID::Name(other), MGID(other)
{}

MGName::~MGName()
{}

QList<MGNum> MGName::map(const MolGroupsBase &molgroups) const
{
    return molgroups.map(*this);
}

////////
//////// Implementation of MGNum
////////

MGNum::MGNum() : SireID::Number(), MGID()
{}

MGNum::MGNum(quint32 num) : SireID::Number(num), MGID()
{}

MGNum::MGNum(const MGNum &other) : SireID::Number(other), MGID(other)
{}

MGNum::~MGNum()
{}

QList<MGNum> MGNum::map(const MolGroupsBase &molgroups) const
{
    return molgroups.map(*this);
}

////////
//////// Implementation of MGMGID
////////

static const RegisterMetaType<MGMGID> r_mgmgid;

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const MGMGID &mgmgid)
{
    writeHeader(ds, r_mgmgid, 1);
    
    SharedDataStream sds(ds);
    sds << mgmgid.mgid0 << mgmgid.mgid1;
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, MGMGID &mgmgid)
{
    VersionID v = readHeader(ds, r_mgmgid);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        sds >> mgmgid.mgid0 >> mgmgid.mgid1;
    }
    else
        throw version_error( v, "1", r_mgmgid, CODELOC );
        
    return ds;
}

/** Null constructor */
MGMGID::MGMGID() : MGID()
{}

/** Construct something that will match only groups that
    match mgid0 and mgid1 */
MGMGID::MGMGID(const MGID &id0, const MGID &id1)
       : MGID(), mgid0(id0), mgid1(id1)
{}

/** Copy constructor */
MGMGID::MGMGID(const MGMGID &other)
       : MGID(), mgid0(other.mgid0), mgid1(other.mgid1)
{}

/** Destructor */
MGMGID::~MGMGID()
{}

/** Copy assignment operator */
MGMGID& MGMGID::operator=(const MGMGID &other)
{
    mgid0 = other.mgid0;
    mgid1 = other.mgid1;
    return *this;
}

/** Comparison operator */
bool MGMGID::operator==(const MGMGID &other) const
{
    return (mgid0 == other.mgid0 and mgid1 == other.mgid1) or
           (mgid0 == other.mgid1 and mgid1 == other.mgid0);
}

/** Comparison operator */
bool MGMGID::operator!=(const MGMGID &other) const
{
    return not this->operator==(other);
}

/** Return whether or not this is null */
bool MGMGID::isNull() const
{
    return mgid0.isNull() and mgid1.isNull();
}

/** Hash this MGMGID */
uint MGMGID::hash() const
{
    return mgid0.hash() + mgid1.hash();
}

/** Return a string representation of this ID */
QString MGMGID::toString() const
{
    if (mgid0.isNull())
    {
        if (mgid1.isNull())
        {
            return QObject::tr("null");
        }
        else
            return mgid1.toString();
    }
    else
    {
        if (mgid1.isNull())
            return mgid0.toString();
        else
            return QObject::tr("%1 and %2")
                        .arg(mgid0.toString(), mgid1.toString());
    }
}

/** Map the two IDs to the list of matching molecule group numbers */
QList<MGNum> MGMGID::map(const MolGroupsBase &molgroups) const
{
    if (mgid0.isNull())
        return mgid1.map(molgroups);
    else if (mgid1.isNull())
        return mgid0.map(molgroups);
        
    QList<MGNum> mgnums = MolInfo::intersection( mgid0.map(molgroups),
                                                 mgid1.map(molgroups) );
                
    if (mgnums.isEmpty())
        throw SireMol::missing_group( QObject::tr(
            "There is no group matching the ID %1.")
                .arg(this->toString()), CODELOC );
    
    return mgnums;
}

///////

template class IDSet<MGID>;

static const RegisterMetaType< IDSet<MGID> > r_idset_mgid;

