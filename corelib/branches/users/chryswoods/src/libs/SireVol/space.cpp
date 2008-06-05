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
    if ( QLatin1String(this->what()) != QLatin1String(other->what()) )
        throw SireError::incompatible_error( QObject::tr(
            "This space (of type \"%1\") is incompatible with "
            "a space of type \"%2\".")
                .arg(this->what()).arg(other->what()), CODELOC );
}

//////////////
////////////// Implementation of Space
//////////////

RegisterMetaType<Space> r_space;

/** Serialise a Space to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds,
                                       const Space &space)
{
    writeHeader(ds, r_space, 1);
    
    SharedDataStream sds(ds);
    
    sds << static_cast<const Property&>(space);
    
    return ds;
}

/** Deserialise a Space from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds,
                                       Space &space)
{
    VersionID v = readHeader(ds, r_space);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        sds >> static_cast<Property&>(space);
    }
    else
        throw version_error(v, "1", r_space, CODELOC);
        
    return ds;
}

static Space *_pvt_shared_null = 0;

const Space& Space::shared_null()
{
    if (_pvt_shared_null == 0)
        _pvt_shared_null = new Space( Cartesian() );
        
    return *_pvt_shared_null;
}

/** Null constructor - constructs a simple, infinite cartesian volume */
Space::Space() : Property(Space::shared_null())
{}

/** Construct from a passed property

    \throw SireError::invalid_cast
*/
Space::Space(const PropertyBase &property)
      : Property(property.asA<SpaceBase>())
{}

/** Construct from passed SpaceBase */
Space::Space(const SpaceBase &space)
      : Property(space)
{}

/** Copy constructor */
Space::Space(const Space &other)
      : Property(other)
{}

/** Destructor */
Space::~Space()
{}

/** Copy assignment operator from a Property object

    \throw SireError::invalid_cast
*/
Space& Space::operator=(const PropertyBase &property)
{
    Property::operator=(property.asA<SpaceBase>());
    return *this;
}

/** Copy assignment operator from another SpaceBase */
Space& Space::operator=(const SpaceBase &other)
{
    Property::operator=(other);
    return *this;
}

/** Dereference this pointer */
const SpaceBase* Space::operator->() const
{
    return static_cast<const SpaceBase*>(&(d()));
}

/** Dereference this pointer */
const SpaceBase& Space::operator*() const
{
    return static_cast<const SpaceBase&>(d());
}

/** Return a read-only reference to the contained object */
const SpaceBase& Space::read() const
{
    return static_cast<const SpaceBase&>(d());
}

/** Return a modifiable reference to the contained object.
    This will trigger a copy of the object if more than
    one pointer is pointing to it. */
SpaceBase& Space::edit()
{
    return static_cast<SpaceBase&>(d());
}
    
/** Return a raw pointer to the SpaceBase object */
const SpaceBase* Space::data() const
{
    return static_cast<const SpaceBase*>(&(d()));
}

/** Return a raw pointer to the SpaceBase object */
const SpaceBase* Space::constData() const
{
    return static_cast<const SpaceBase*>(&(d()));
}
    
/** Return a raw pointer to the SpaceBase object */
SpaceBase* Space::data()
{
    return static_cast<SpaceBase*>(&(d()));
}

/** Automatic casting operator */
Space::operator const SpaceBase&() const
{
    return static_cast<const SpaceBase&>(d());
}
