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

#include <QMutex>

#include "space.h"
#include "cartesian.h"

#include "SireError/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireVol;
using namespace SireBase;
using namespace SireStream;

///////////////
/////////////// Implementation of Space
///////////////

static const RegisterMetaType<Space> r_Space(MAGIC_ONLY, "SireVol::Space");

/** Serialise to a binary datastream */
QDataStream SIREVOL_EXPORT &operator<<(QDataStream &ds,
                                       const Space &Space)
{
    writeHeader(ds, r_Space, 1)
            << static_cast<const Property&>(Space);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREVOL_EXPORT &operator>>(QDataStream &ds,
                                       Space &Space)
{
    VersionID v = readHeader(ds, r_Space);

    if (v == 1)
    {
        ds >> static_cast<Property&>(Space);
    }
    else
        throw version_error(v, "1", r_Space, CODELOC);

    return ds;
}

/** Construct a Space. */
Space::Space() : Property()
{}

/** Copy constructor */
Space::Space(const Space &other) : Property(other)
{}

/** Destructor */
Space::~Space()
{}

/** Change the volume of this space by 'delta' */
SpacePtr Space::changeVolume(SireUnits::Dimension::Volume delta) const
{
    return this->setVolume( this->volume() + delta );
}

/** Assert that 'other' is of the same type as this space

    \throw SireError::incompatible_error
*/
void Space::assertCompatible(const Space &other) const
{
    if ( QLatin1String(this->what()) != QLatin1String(other.what()) )
        throw SireError::incompatible_error( QObject::tr(
            "This space (of type \"%1\") is incompatible with "
            "a space of type \"%2\".")
                .arg(this->what()).arg(other.what()), CODELOC );
}

static SharedPolyPointer<Cartesian> shared_null;

/** Return the default space (Cartesian infinite box) */
const Cartesian& Space::null()
{
    if (shared_null.constData() == 0)
    {
        QMutexLocker lkr( SireBase::globalLock() );
        
        if (shared_null.constData() == 0)
            shared_null = new Cartesian();
    }
    
    return *(shared_null.constData());
}

/** Return a copy of the passed CoordGroup mapped from this space to 
    the other space 'space' */
CoordGroup Space::mapToSpace(const CoordGroup &group, const Space &space) const
{
    if (this->isA<Cartesian>())
        return space.mapFromCartesian(group);

    if (not space.isA<Cartesian>())
        throw SireError::incompatible_error( QObject::tr(
            "There is no algorithm available that can map a molecule "
            "from the space %1 to the space %2.")
                .arg(this->what()).arg(space.what()), CODELOC );
        
    
    return this->mapToCartesian(group);
}

/** Return a copy of the passed CoordGroups mapped individually from this
    space to the other space 'space' */
CoordGroupArray Space::mapToSpace(const CoordGroupArray &groups,
                                  const Space &space) const
{
    if (this->isA<Cartesian>())
        return space.mapFromCartesian(groups);

    if (not space.isA<Cartesian>())
        throw SireError::incompatible_error( QObject::tr(
            "There is no algorithm available that can map a molecule "
            "from the space %1 to the space %2.")
                .arg(this->what()).arg(space.what()), CODELOC );
        
    
    return this->mapToCartesian(groups);
}
                                   
/** Return a copy of the passed CoordGroups mapped as a single unit from this
    space to the other space 'space' */
CoordGroupArray Space::mapAsOneToSpace(const CoordGroupArray &groups,
                                       const Space &space) const
{
    if (this->isA<Cartesian>())
        return space.mapAsOneFromCartesian(groups);

    if (not space.isA<Cartesian>())
        throw SireError::incompatible_error( QObject::tr(
            "There is no algorithm available that can map a molecule "
            "from the space %1 to the space %2.")
                .arg(this->what()).arg(space.what()), CODELOC );
        
    
    return this->mapAsOneToCartesian(groups);
}

/** Return the CoordGroup mapped from the space 'space' to this space */
CoordGroup Space::mapFromSpace(const CoordGroup &group, const Space &space) const
{
    return space.mapToSpace(group, *this);
}

/** Return the array of CoordGroups mapped individually from the space
    'space' to this space */
CoordGroupArray Space::mapFromSpace(const CoordGroupArray &groups, 
                                    const Space &space) const
{
    return space.mapToSpace(groups, *this);
}

/** Return the array of CoordGroups mapped as a single unit from
    the space 'space' to this space */
CoordGroupArray Space::mapAsOneFromSpace(const CoordGroupArray &groups,
                                         const Space &space) const
{
    return space.mapAsOneToSpace(groups, *this);
}
