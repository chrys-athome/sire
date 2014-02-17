/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2014  Christopher Woods
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

#include "gridinfo.h"

#include "SireMaths/vector.h"

#include "SireError/errors.h"

#include "SireStream/datastream.h"

using namespace SireMM;
using namespace SireMaths;
using namespace SireStream;

/////////
///////// Implementation of GridIndex
/////////

static const RegisterMetaType<GridIndex> r_index(NO_ROOT);

QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const GridIndex &idx)
{
    writeHeader(ds, r_index, 1);
    ds << idx.i() << idx.j() << idx.k();
    return ds;
}

QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, GridIndex &idx)
{
    VersionID v = readHeader(ds, r_index);
    
    if (v == 1)
    {
        qint32 i,j,k;
        ds >> i >> j >> k;
        idx = GridIndex(i,j,k);
    }
    else
        throw version_error( v, "1", r_index, CODELOC );
    
    return ds;
}

const char* GridIndex::typeName()
{
    return "SireMM::GridIndex";
}

const char* GridIndex::what() const
{
    return GridIndex::typeName();
}

QString GridIndex::toString() const
{
    return QObject::tr("GridIndex( %1, %2, %3 )")
                    .arg(_i).arg(_j).arg(_k);
}

/////////
///////// Implementation of GridInfo
/////////

static const RegisterMetaType<GridInfo> r_info(NO_ROOT);

QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const GridInfo &info)
{
    writeHeader(ds, r_info, 1);
    
    ds << info.grid_dimensions << info.grid_spacing
       << info.dimx << info.dimy << info.dimz;

    return ds;
}

QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, GridInfo &info)
{
    VersionID v = readHeader(ds, r_info);
    
    if (v == 1)
    {
        ds >> info.grid_dimensions >> info.grid_spacing
           >> info.dimx >> info.dimy >> info.dimz;
    }
    else
        throw version_error(v, "1", r_info, CODELOC);
    
    return ds;
}

/** Constructor */
GridInfo::GridInfo() : grid_spacing(1.0), dimx(0), dimy(0), dimz(0)
{}

/** Construct a grid of specified dimensions and spacing */
GridInfo::GridInfo(const AABox &dimensions, Length spacing)
         : grid_spacing(spacing.value()), dimx(0), dimy(0), dimz(0)
{
    if (grid_spacing <= 0)
    {
        throw SireError::invalid_arg( QObject::tr(
                "You cannot construct a grid with zero or negative grid spacing! "
                "(%1 A)").arg(grid_spacing), CODELOC );
    }
    
    Vector nboxes = (2.0 / grid_spacing) * dimensions.halfExtents();
    
    dimx = quint32( std::ceil(nboxes.x()) );
    dimy = quint32( std::ceil(nboxes.y()) );
    dimz = quint32( std::ceil(nboxes.z()) );
    
    Vector maxcoords = dimensions.minCoords() + Vector( dimx*grid_spacing,
                                                        dimy*grid_spacing,
                                                        dimz*grid_spacing );
    
    grid_dimensions = AABox::from( dimensions.minCoords(), maxcoords );
}

/** Copy constructor */
GridInfo::GridInfo(const GridInfo &other)
         : grid_dimensions(other.grid_dimensions), grid_spacing(other.grid_spacing),
           dimx(other.dimx), dimy(other.dimy), dimz(other.dimz)
{}

/** Destructor */
GridInfo::~GridInfo()
{}

/** Copy assignment operator */
GridInfo& GridInfo::operator=(const GridInfo &other)
{
    if (this != &other)
    {
        grid_dimensions = other.grid_dimensions;
        grid_spacing = other.grid_spacing;
        dimx = other.dimx;
        dimy = other.dimy;
        dimz = other.dimz;
    }
    
    return *this;
}

/** Comparison operator */
bool GridInfo::operator==(const GridInfo &other) const
{
    return grid_dimensions == other.grid_dimensions and grid_spacing == other.grid_spacing and
           dimx == other.dimx and dimy == other.dimy and dimz == other.dimz;
}

/** Comparison operator */
bool GridInfo::operator!=(const GridInfo &other) const
{
    return not operator==(other);
}

const char* GridInfo::typeName()
{
    return QMetaType::typeName( qMetaTypeId<GridInfo>() );
}

const char* GridInfo::what() const
{
    return GridInfo::typeName();
}

QString GridInfo::toString() const
{
    return QObject::tr("GridInfo( dimensions() = %1, spacing = %2 A, [%1, %2, %3] )")
                .arg(dimensions().toString())
                .arg(grid_spacing)
                .arg(dimx).arg(dimy).arg(dimz);
}

Vector GridInfo::getitem(int i) const
{
    return this->at(i);
}
