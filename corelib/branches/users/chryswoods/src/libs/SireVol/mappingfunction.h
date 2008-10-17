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

#ifndef SIREVOL_MAPPINGFUNCTION_H
#define SIREVOL_MAPPINGFUNCTION_H

#include <QVector>

#include "coordgroup.h"
#include "space.h"

#include "SireBase/property.h"

SIRE_BEGIN_HEADER

namespace SireVol
{
class MappingFunction;

class MapFromCartesianFunction;
class MapFromSelfFunction;
}

QDataStream& operator<<(QDataStream&, const SireVol::MappingFunction&);
QDataStream& operator>>(QDataStream&, SireVol::MappingFunction&);

QDataStream& operator<<(QDataStream&, const SireVol::MapFromCartesianFunction&);
QDataStream& operator>>(QDataStream&, SireVol::MapFromCartesianFunction&);

QDataStream& operator<<(QDataStream&, const SireVol::MapFromSelfFunction&);
QDataStream& operator>>(QDataStream&, SireVol::MapFromSelfFunction&);

namespace SireVol
{

/** This is the base class of all mapping functions. A mapping function
    is used to map CoordGroups from one space into another

    @author Christopher Woods
*/
class SIREVOL_EXPORT MappingFunction : public SireBase::Property
{

friend QDataStream& ::operator<<(QDataStream&, const MappingFunction&);
friend QDataStream& ::operator>>(QDataStream&, MappingFunction&);

public:
    MappingFunction();

    MappingFunction(const MappingFunction &other);

    virtual ~MappingFunction();

    static const char* typeName()
    {
        return "SireVol::MappingFunction";
    }

    /** Return a clone of this object */
    virtual MappingFunction* clone() const=0;

    /** Map the coordinates 'coords' from the old space 'old_space'
        to the new space 'new_space'. The spaces must be compatible
        with this function.

        \throw SireError::incompatible_error
    */
    virtual QVector<CoordGroup> map(const QVector<CoordGroup> &coords,
                                    const Space &old_space,
                                    const Space &new_space) const=0;

    QVector<CoordGroup> operator()(const QVector<CoordGroup> &coords,
                                   const Space &old_space,
                                   const Space &new_space) const;

    /** Map the coordinates 'coords' from the old space 'old_space'
        to the new space 'new_space'. The spaces must be compatible
        with this function.

        \throw SireError::incompatible_error
    */
    virtual CoordGroup map(const CoordGroup &coords,
                           const Space &old_space,
                           const Space &new_space) const=0;

    CoordGroup operator()(const CoordGroup &coords,
                          const Space &old_space,
                          const Space &new_space) const;
                          
    static const MapFromCartesianFunction& null();
};

/** Convienient syntax for "map" that lets you use
    this MappingFunction object like a normal function.

    \throw SireError::incompatible_error
*/
inline QVector<CoordGroup>
MappingFunction::operator()(const QVector<CoordGroup> &coords,
                            const Space &old_space, const Space &new_space) const
{
    return this->map(coords, old_space, new_space);
}

/** Convienient syntax for "map" that lets you use
    this MappingFunction object like a normal function.

    \throw SireError::incompatible_error
*/
inline CoordGroup MappingFunction::operator()(const CoordGroup &coords,
                                              const Space &old_space,
                                              const Space &new_space) const
{
    return this->map(coords, old_space, new_space);
}

/** This is a mapping function that maps coordinates from an infinite
    cartesian space, old_space, into the new space 'new_space'

    @author Christopher Woods
*/
class SIREMOVE_EXPORT MapFromCartesianFunction 
           : public SireBase::ConcreteProperty<MapFromCartesianFunction,MappingFunction>
{

friend QDataStream& ::operator<<(QDataStream&, const MapFromCartesianFunction&);
friend QDataStream& ::operator>>(QDataStream&, MapFromCartesianFunction&);

public:
    MapFromCartesianFunction();

    MapFromCartesianFunction(const MapFromCartesianFunction&);

    ~MapFromCartesianFunction();

    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<MapFromCartesianFunction>() );
    }

    const char* what() const
    {
        return MapFromCartesianFunction::typeName();
    }

    MapFromCartesianFunction* clone() const
    {
        return new MapFromCartesianFunction(*this);
    }

    CoordGroup map(const CoordGroup &coords,
                   const Space &old_space, const Space &new_space) const;

    QVector<CoordGroup> map(const QVector<CoordGroup> &coords,
                            const Space &old_space,
                            const Space &new_space) const;

private:
    void assertCompatible(const Space &old_space) const;
};

/** This is a mapping function that maps coordinates from
    'old_space' to 'new_space', where both of these spaces
    are of the same type.

    @author Christopher Woods
*/
class SIREMOVE_EXPORT MapFromSelfFunction 
                : public SireBase::ConcreteProperty<MapFromSelfFunction,MappingFunction>
{

friend QDataStream& ::operator<<(QDataStream&, const MapFromSelfFunction&);
friend QDataStream& ::operator>>(QDataStream&, MapFromSelfFunction&);

public:
    MapFromSelfFunction();

    MapFromSelfFunction(const MapFromSelfFunction&);

    ~MapFromSelfFunction();

    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<MapFromSelfFunction>() );
    }

    CoordGroup map(const CoordGroup &coords,
                   const Space &old_space, const Space &new_space) const;

    QVector<CoordGroup> map(const QVector<CoordGroup> &coords,
                            const Space &old_space,
                            const Space &new_space) const;
};

typedef SireBase::PropPtr<MappingFunction> MappingFunctionPtr;

}

Q_DECLARE_METATYPE(SireVol::MapFromCartesianFunction);
Q_DECLARE_METATYPE(SireVol::MapFromSelfFunction);

SIRE_EXPOSE_CLASS( SireVol::MappingFunction )
SIRE_EXPOSE_CLASS( SireVol::MapFromCartesianFunction )
SIRE_EXPOSE_CLASS( SireVol::MapFromSelfFunction )

SIRE_EXPOSE_PROPERTY( SireVol::MappingFunctionPtr, SireVol::MappingFunction )

SIRE_END_HEADER

#endif
