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

#include <cmath>

#include "volchangingfunction.h"

#include "SireMaths/rangenerator.h"

#include "SireVol/space.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMove;
using namespace SireVol;
using namespace SireMaths;
using namespace SireBase;
using namespace SireStream;

////////////
//////////// Implementation of VolChangingFunctionBase
////////////

static const RegisterMetaType<VolChangingFunctionBase> r_volchangebase(MAGIC_ONLY,
                                               "SireMove::VolChangingFunctionBase");

/** Serialise to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds,
                                        const VolChangingFunctionBase&)
{
    writeHeader(ds, r_volchangebase, 0);
    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIRE_EXPORT &operator>>(QDataStream &ds,
                                    VolChangingFunctionBase&)
{
    VersionID v = readHeader(ds, r_volchangebase);

    if (v != 0)
        throw version_error(v, "0", r_volchangebase, CODELOC);

    return ds;
}

/** Constructor */
VolChangingFunctionBase::VolChangingFunctionBase()
                        : QSharedData()
{}

/** Copy constructor */
VolChangingFunctionBase::VolChangingFunctionBase(const VolChangingFunctionBase&)
                        : QSharedData()
{}

/** Destructor */
VolChangingFunctionBase::~VolChangingFunctionBase()
{}

////////////
//////////// Implementation of UniformVolumeChange
////////////

static const RegisterMetaType<UniformVolumeChange> r_unichange;

/** Serialise to a binary datastream */
QDataStream SIRE_EXPORT &operator<<(QDataStream &ds,
                                    const UniformVolumeChange &unichange)
{
    writeHeader(ds, r_unichange, 1)
        << double(unichange.maxdelta)
        << static_cast<const VolChangingFunctionBase&>(unichange);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIRE_EXPORT &operator>>(QDataStream &ds,
                                    UniformVolumeChange &unichange)
{
    VersionID v = readHeader(ds, r_unichange);

    if (v == 1)
    {
        double maxdelta;

        ds >> maxdelta
           >> static_cast<VolChangingFunctionBase&>(unichange);

        unichange.maxdelta = SireUnits::Dimension::Volume(maxdelta);
    }
    else
        throw version_error(v, "1", r_unichange, CODELOC);

    return ds;
}

/** Default constructor - makes no change at all! */
UniformVolumeChange::UniformVolumeChange()
                    : VolChangingFunctionBase(), maxdelta(0)
{}

/** Construct a changer that changes the space by a uniform maximum
    change of 'delta' */
UniformVolumeChange::UniformVolumeChange(SireUnits::Dimension::Volume delta)
                    : VolChangingFunctionBase(), maxdelta(std::abs(delta))
{}

/** Copy constructor */
UniformVolumeChange::UniformVolumeChange(const UniformVolumeChange &other)
                    : VolChangingFunctionBase(other),
                      maxdelta(other.maxdelta)
{}

/** Destructor */
UniformVolumeChange::~UniformVolumeChange()
{}

/** Return the maximum amount by which the volume will be changed */
SireUnits::Dimension::Volume UniformVolumeChange::maximumVolumeChange() const
{
    return maxdelta;
}

/** Change the space uniformly by a random amount */
Space UniformVolumeChange::change(const Space &old_space,
                                  RanGenerator &generator) const
{
    SireUnits::Dimension::Volume delta = generator.rand(-1,1) * maxdelta;

    return old_space.changeVolume(delta);
}

////////////
//////////// Implementation of VolChangingFunction
////////////

static SharedPolyPointer<VolChangingFunctionBase> shared_null;

static SharedPolyPointer<VolChangingFunctionBase> getSharedNull()
{
    if (shared_null.constData() == 0)
        shared_null = UniformVolumeChange();

    return shared_null;
}

static const RegisterMetaType<VolChangingFunction> r_volchanger;

/** Serialise to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds,
                                        const VolChangingFunction &volchanger)
{
    writeHeader(ds, r_volchanger, 1);

    SharedDataStream sds(ds);
    sds << volchanger.d;

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds,
                                        VolChangingFunction &volchanger)
{
    VersionID v = readHeader(ds, r_volchanger);

    if (v == 1)
    {
        SharedDataStream sds(ds);
        sds >> volchanger.d;
    }
    else
        throw version_error(v, "1", r_volchanger, CODELOC);

    return ds;
}

/** Null constructor */
VolChangingFunction::VolChangingFunction()
                    : d( getSharedNull() )
{}

/** Construct from the passed volume changing function */
VolChangingFunction::VolChangingFunction(const VolChangingFunctionBase &changer)
                    : d( changer )
{}

/** Copy constructor */
VolChangingFunction::VolChangingFunction(const VolChangingFunction &other)
                    : d( other.d )
{}

/** Destructor */
VolChangingFunction::~VolChangingFunction()
{}

/** Assign from a copy of the passed volume changer */
VolChangingFunction& VolChangingFunction::operator=(
                                        const VolChangingFunctionBase &changer)
{
    d = changer;
    return *this;
}

/** Copy assignment operator */
VolChangingFunction& VolChangingFunction::operator=(
                                        const VolChangingFunction &other)
{
    d = other.d;
    return *this;
}

/** Return the typename of the volume changing function */
const char* VolChangingFunction::what() const
{
    return d->what();
}

/** Return the base class of the volume changing function */
const VolChangingFunctionBase& VolChangingFunction::base() const
{
    return *d;
}

/** Use the changing function to randomly change the volume of the
    passed Space - return the new space */
Space VolChangingFunction::change(const Space &old_space,
                                  RanGenerator &generator) const
{
    return d->change(old_space, generator);
}
