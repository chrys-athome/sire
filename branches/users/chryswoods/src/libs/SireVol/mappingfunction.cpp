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

#include "mappingfunction.h"
#include "cartesian.h"

#include "SireError/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireVol;
using namespace SireBase;
using namespace SireStream;

//////////////
////////////// Implementation of MappingFunctionBase
//////////////

static const RegisterMetaType<MappingFunctionBase> r_mapfuncbase(MAGIC_ONLY,
                                                       "SireVol::MappingFunctionBase");

/** Serialise to a binary datastream */
QDataStream SIREVOL_EXPORT &operator<<(QDataStream &ds,
                                       const MappingFunctionBase&)
{
    writeHeader(ds, r_mapfuncbase, 0);
    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREVOL_EXPORT &operator>>(QDataStream &ds,
                                       MappingFunctionBase&)
{
    VersionID v = readHeader(ds, r_mapfuncbase);

    if (v != 0)
        throw version_error(v, "0", r_mapfuncbase, CODELOC);

    return ds;
}

/** Constructor */
MappingFunctionBase::MappingFunctionBase() : QSharedData()
{}

/** Copy constructor */
MappingFunctionBase::MappingFunctionBase(const MappingFunctionBase&)
                    : QSharedData()
{}

/** Destructor */
MappingFunctionBase::~MappingFunctionBase()
{}

/** Copy assignment operator */
MappingFunctionBase& MappingFunctionBase::operator=(const MappingFunctionBase&)
{
    return *this;
}

//////////////
////////////// Implementation of MapFromCartesianFunction
//////////////

static const RegisterMetaType<MapFromCartesianFunction> r_mapcart;

/** Serialise to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds,
                                        const MapFromCartesianFunction &mapcart)
{
    writeHeader(ds, r_mapcart, 1)
        << static_cast<const MappingFunctionBase&>(mapcart);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds,
                                        MapFromCartesianFunction &mapcart)
{
    VersionID v = readHeader(ds, r_mapcart);

    if (v == 1)
    {
        ds >> static_cast<MappingFunctionBase&>(mapcart);
    }
    else
        throw version_error(v, "1", r_mapcart, CODELOC);

    return ds;
}

/** Constructor */
MapFromCartesianFunction::MapFromCartesianFunction()
                         : MappingFunctionBase()
{}

/** Copy constructor */
MapFromCartesianFunction::MapFromCartesianFunction(
                                const MapFromCartesianFunction &other)
                         : MappingFunctionBase(other)
{}

/** Destructor */
MapFromCartesianFunction::~MapFromCartesianFunction()
{}

void MapFromCartesianFunction::assertCompatible(const Space &old_space) const
{
    if ( QLatin1String(old_space.what()) != QLatin1String(Cartesian::typeName()) )
    {
        throw SireError::incompatible_error( QObject::tr(
            "The MapFromCartesianFunction can be used only to map from "
            "the Cartesian space. You cannot map from \"%1\".")
                .arg(old_space.what()), CODELOC );
    }
}

/** Map the coordinates from 'old_space', which *must* be a Cartesian
    space, into 'new_space'

    \throw SireError::incompatible_error
*/
CoordGroup MapFromCartesianFunction::mapCoordinates(const CoordGroup &coords,
                                                    const Space &old_space,
                                                    const Space &new_space) const
{
    this->assertCompatible(old_space);
    return new_space.mapFromCartesian(coords);
}

/** Map the coordinates from 'old_space', which *must* be a Cartesian
    space, into 'new_space'

    \throw SireError::incompatible_error
*/
QVector<CoordGroup>
MapFromCartesianFunction::mapCoordinates(const QVector<CoordGroup> &coords,
                                         const Space &old_space,
                                         const Space &new_space) const
{
    this->assertCompatible(old_space);
    return new_space.mapFromCartesian(coords);
}

//////////////
////////////// Implementation of MapFromSelfFunction
//////////////

static const RegisterMetaType<MapFromSelfFunction> r_mapself;

/** Serialise to a binary datastream */
QDataStream SIREVOL_EXPORT &operator<<(QDataStream &ds,
                                       const MapFromSelfFunction &mapself)
{
    writeHeader(ds, r_mapself, 1)
          << static_cast<const MappingFunctionBase&>(mapself);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREVOL_EXPORT &operator>>(QDataStream &ds,
                                       MapFromSelfFunction &mapself)
{
    VersionID v = readHeader(ds, r_mapself);

    if (v == 1)
    {
        ds >> static_cast<MappingFunctionBase&>(mapself);
    }
    else
        throw version_error(v, "1", r_mapself, CODELOC);

    return ds;
}

/** Constructor */
MapFromSelfFunction::MapFromSelfFunction()
                    : MappingFunctionBase()
{}

/** Copy constructor */
MapFromSelfFunction::MapFromSelfFunction(const MapFromSelfFunction &other)
                    : MappingFunctionBase(other)
{}

/** Destructor */
MapFromSelfFunction::~MapFromSelfFunction()
{}

/** Map 'coords' from 'old_space' to 'new_space' - both spaces must
    be of the same type!

    \throw SireError::incompatible_error
*/
CoordGroup MapFromSelfFunction::mapCoordinates(const CoordGroup &coords,
                                               const Space &old_space,
                                               const Space &new_space) const
{
    return new_space.mapFromSelf(coords, old_space);
}

/** Map 'coords' from 'old_space' to 'new_space' - both spaces must
    be of the same type!

    \throw SireError::incompatible_error
*/
QVector<CoordGroup>
MapFromSelfFunction::mapCoordinates(const QVector<CoordGroup> &coords,
                                    const Space &old_space,
                                    const Space &new_space) const
{
    return new_space.mapFromSelf(coords, old_space);
}

//////////////
////////////// Implementation of MappingFunction
//////////////

static SharedPolyPointer<MappingFunctionBase> shared_null;

static SharedPolyPointer<MappingFunctionBase> getSharedNull()
{
    if (shared_null.constData() == 0)
        shared_null = new MapFromSelfFunction();

    return shared_null;
}

static const RegisterMetaType<MappingFunction> r_mapfunc;

/** Serialise to a binary datastream */
QDataStream SIREVOL_EXPORT &operator<<(QDataStream &ds,
                                       const MappingFunction &mapfunc)
{
    writeHeader(ds, r_mapfunc, 1);

    SharedDataStream sds(ds);
    sds << mapfunc.d;

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREVOL_EXPORT &operator>>(QDataStream &ds, MappingFunction &mapfunc)
{
    VersionID v = readHeader(ds, r_mapfunc);

    if (v == 1)
    {
        SharedDataStream sds(ds);
        sds >> mapfunc.d;
    }
    else
        throw version_error(v, "1", r_mapfunc, CODELOC);

    return ds;
}

/** Null constructor - the default mapping function is a MapFromSelfFunction */
MappingFunction::MappingFunction()
                : d( getSharedNull() )
{}

/** Construct from the passed function */
MappingFunction::MappingFunction(const MappingFunctionBase &function)
                : d( function )
{}

/** Copy constructor */
MappingFunction::MappingFunction(const MappingFunction &other)
                : d( other.d )
{}

/** Destructor */
MappingFunction::~MappingFunction()
{}

/** Copy assignment */
MappingFunction& MappingFunction::operator=(const MappingFunction &other)
{
    d = other.d;
    return *this;
}

/** Copy from the passed function */
MappingFunction& MappingFunction::operator=(const MappingFunctionBase &function)
{
    d = function;
    return *this;
}

/** Return the typename of this function */
const char* MappingFunction::what() const
{
    return d->what();
}

/** Return a reference to the base class itself */
const MappingFunctionBase& MappingFunction::base() const
{
    return *d;
}
