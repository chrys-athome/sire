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

#include "space.h"
#include "cartesian.h"

#include "SireError/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireVol;
using namespace SireBase;
using namespace SireStream;

///////////////
/////////////// Implementation of SpaceBase
///////////////

static const RegisterMetaType<SpaceBase> r_spacebase(MAGIC_ONLY, "SireVol::SpaceBase");

/** Serialise to a binary datastream */
QDataStream SIREVOL_EXPORT &operator<<(QDataStream &ds,
                                       const SpaceBase &spacebase)
{
    writeHeader(ds, r_spacebase, 1)
            << static_cast<const PropertyBase&>(spacebase);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREVOL_EXPORT &operator>>(QDataStream &ds,
                                       SpaceBase &spacebase)
{
    VersionID v = readHeader(ds, r_spacebase);

    if (v == 1)
    {
        ds >> static_cast<PropertyBase&>(spacebase);
    }
    else
        throw version_error(v, "1", r_spacebase, CODELOC);

    return ds;
}

/** Construct a SpaceBase. */
SpaceBase::SpaceBase() : PropertyBase()
{}

/** Copy constructor */
SpaceBase::SpaceBase(const SpaceBase &other) : PropertyBase(other)
{}

/** Destructor */
SpaceBase::~SpaceBase()
{}

/** Change the volume of this space by 'delta' */
Space SpaceBase::changeVolume(SireUnits::Dimension::Volume delta) const
{
    return this->setVolume( this->volume() + delta );
}

/** Assert that 'other' is of the same type as this space

    \throw SireError::incompatible_error
*/
void SpaceBase::assertCompatible(const Space &other) const
{
    if ( QLatin1String(this->what()) != QLatin1String(other.what()) )
        throw SireError::incompatible_error( QObject::tr(
            "This space (of type \"%1\") is incompatible with "
            "a space of type \"%2\".")
                .arg(this->what()).arg(other.what()), CODELOC );
}

//////////////
////////////// Implementation of Space
//////////////

static const RegisterMetaType<Space> r_space;

/** Serialise to a binary datastream */
QDataStream SIREVOL_EXPORT &operator<<(QDataStream &ds, const Space &space)
{
    writeHeader(ds, r_space, 1);

    SharedDataStream sds(ds);

    sds << space.d;

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREVOL_EXPORT &operator>>(QDataStream &ds, Space &space)
{
    VersionID v = readHeader(ds, r_space);

    if (v == 1)
    {
        SharedDataStream sds(ds);
        sds >> space.d;
    }
    else
        throw version_error(v, "1", r_space, CODELOC);

    return ds;
}

static SharedPolyPointer<SpaceBase> shared_null( new Cartesian() );

/** Null constructor - this is a infinite Cartesian volume */
Space::Space() : d( shared_null )
{}

/** Construct from the base class */
Space::Space(const SpaceBase &other)
      : d( other.clone() )
{}

/** Construct from a property */
Space::Space(const Property &property)
      : d( property.base() )
{}

/** Copy constructor - fast as this class is implicitly shared */
Space::Space(const Space &other)
      : d( other.d )
{}

/** Destructor */
Space::~Space()
{}

/** Assign from the base class */
Space& Space::operator=(const SpaceBase &other)
{
    d = other;
    return *this;
}

/** Assignment operator */
Space& Space::operator=(const Space &other)
{
    d = other.d;
    return *this;
}

/** Assign from a Property */
Space& Space::operator=(const Property &property)
{
    d = property.base();
    return *this;
}

/** Comparison operator */
bool Space::operator==(const Space &other) const
{
    return d == other.d or *d == *(other.d);
}

/** Comparison operator */
bool Space::operator!=(const Space &other) const
{
    return d != other.d and *d != *(other.d);
}

/** Return the type of Space */
const char* Space::what() const
{
    return d->what();
}
