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

#include "checkpoint.h"

#include "system.h"
#include "querysystem.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireSystem;
using namespace SireFF;
using namespace SireStream;

static const RegisterMetaType<CheckPoint> r_checkpoint;

/** Serialise to a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator<<(QDataStream &ds,
                                          const CheckPoint &checkpoint)
{
    writeHeader(ds, r_checkpoint, 1);

    SharedDataStream sds(ds);

    sds << checkpoint.sysdata
        << checkpoint.ffields
        << checkpoint.sysmonitors;

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator>>(QDataStream &ds,
                                          CheckPoint &checkpoint)
{
    VersionID v = readHeader(ds, r_checkpoint);

    if (v == 1)
    {
        SharedDataStream sds(ds);

        sds >> checkpoint.sysdata
            >> checkpoint.ffields
            >> checkpoint.sysmonitors;
    }
    else
        throw version_error(v, "1", r_checkpoint, CODELOC);

    return ds;
}

/** Null constructor */
CheckPoint::CheckPoint()
{}

/** Construct a checkpoint of 'system' */
CheckPoint::CheckPoint(const System &system)
{
    this->operator=(system);
}

/** Construct from a running simulation system */
CheckPoint::CheckPoint(const QuerySystem &system)
           : sysdata(system.info()),
             ffields(system.forceFields()),
             sysmonitors(system.monitors())
{}

/** Copy constructor */
CheckPoint::CheckPoint(const CheckPoint &other)
           : sysdata(other.sysdata),
             ffields(other.ffields),
             sysmonitors(other.sysmonitors)
{}

/** Destructor */
CheckPoint::~CheckPoint()
{}

/** Copy assignment operator */
CheckPoint& CheckPoint::operator=(const CheckPoint &other)
{
    if (this != &other)
    {
        sysdata = other.sysdata;
        ffields = other.ffields;
        sysmonitors = other.sysmonitors;
    }

    return *this;
}

/** Copy assignment from a System */
CheckPoint& CheckPoint::operator=(const System &system)
{
    System copy(system);

    copy.prepareForSimulation();

    sysdata = copy.info();
    ffields = copy.forceFields();
    sysmonitors = copy.monitors();

    return *this;
}

/** Copy assignment from a running simulation */
CheckPoint& CheckPoint::operator=(const QuerySystem &system)
{
    sysdata = system.info();
    ffields = system.forceFields();
    sysmonitors = system.monitors();

    return *this;
}

/** Comparison operator - two checkpoints are equal if they
    have the same ID and version */
bool CheckPoint::operator==(const CheckPoint &other) const
{
    return (this == &other) or
           ( ID() == other.ID() and version() == other.version() );
}

/** Comparison operator - two checkpoints are equal if they
    have the same ID and version */
bool CheckPoint::operator!=(const CheckPoint &other) const
{
    return (this != &other) and
           ( ID() != other.ID() or version() != other.version() );
}
