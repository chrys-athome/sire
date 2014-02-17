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
    if (isNull())
        return QObject::tr("GridIndex::null");
    else
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
    
    ds << info.grid_origin << info.grid_spacing
       << info.dimx << info.dimy << info.dimz;

    return ds;
}

QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, GridInfo &info)
{
    VersionID v = readHeader(ds, r_info);
    
    if (v == 1)
    {
        ds >> info.grid_origin >> info.grid_spacing
           >> info.dimx >> info.dimy >> info.dimz;
        
        info.inv_grid_spacing = 1.0f / info.grid_spacing;
    }
    else
        throw version_error(v, "1", r_info, CODELOC);
    
    return ds;
}

/** Constructor */
GridInfo::GridInfo()
         : grid_origin(-0.5), grid_spacing(1.0), inv_grid_spacing(1.0), dimx(0), dimy(0), dimz(0)
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
    
    grid_origin = dimensions.minCoords();
    inv_grid_spacing = 1.0f / grid_spacing;
}

/** Copy constructor */
GridInfo::GridInfo(const GridInfo &other)
         : grid_origin(other.grid_origin), grid_spacing(other.grid_spacing),
           inv_grid_spacing(other.inv_grid_spacing),
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
        grid_origin = other.grid_origin;
        grid_spacing = other.grid_spacing;
        inv_grid_spacing = other.inv_grid_spacing;
        dimx = other.dimx;
        dimy = other.dimy;
        dimz = other.dimz;
    }
    
    return *this;
}

/** Comparison operator */
bool GridInfo::operator==(const GridInfo &other) const
{
    return grid_origin == other.grid_origin and grid_spacing == other.grid_spacing and
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
    return QObject::tr("GridInfo{ dimensions() = %1, spacing = %2 A, [%3, %4, %5] }")
                .arg(dimensions().toString())
                .arg(grid_spacing)
                .arg(dimx).arg(dimy).arg(dimz);
}

GridIndex GridInfo::getitem(int i) const
{
    return this->at(i);
}

/** Return the index of the grid box that contains the point 'point'. Note
    that this returns a null index if the point is not in the grid */
GridIndex GridInfo::pointToGridIndex(const Vector &point) const
{
    const qint32 i = qint32( (point.x() - grid_origin.x()) * inv_grid_spacing );
    const qint32 j = qint32( (point.y() - grid_origin.y()) * inv_grid_spacing );
    const qint32 k = qint32( (point.z() - grid_origin.z()) * inv_grid_spacing );
    
    if (i < 0 or i >= dimx or
        j < 0 or j >= dimy or
        k < 0 or k >= dimz)
    {
        //the point is outside of the grid
        return GridIndex::null();
    }
    else
    {
        return GridIndex(i,j,k);
    }
}

/** Return array indicies of the eight grid points that are on the corners of the 
    box that contains the point 'point'. This returns eight '-1' values if the 
    point does not lie in the grid */
void GridInfo::pointToGridCorners(const Vector &point, QVector<int> &indicies) const
{
    indicies.resize(8);
    
    int *ia = indicies.data();
    
    const qint32 r = qint32( (point.x() - grid_origin.x()) * inv_grid_spacing );
    const qint32 s = qint32( (point.y() - grid_origin.y()) * inv_grid_spacing );
    const qint32 t = qint32( (point.z() - grid_origin.z()) * inv_grid_spacing );
    
    if (r < 0 or r >= dimx or
        s < 0 or s >= dimy or
        t < 0 or t >= dimz)
    {
        for (int i=0; i<8; ++i)
        {
            ia[i] = -1;
        }
    }
    else
    {
        NEED TO DO THIS
    }
}

/** Return the array index of the grid box that contains the point 'point'. 
    Note that this returns -1 if the point is not in the grid */
int GridInfo::pointToArrayIndex(const Vector &point) const
{
    return this->gridToArrayIndex( pointToGridIndex(point) );
}

/** Return the AABox that encompasses the grid box at point 'idx'.
    This returns an empty box if the point is not in the grid */
AABox GridInfo::box(const GridIndex &idx) const
{
    if (idx.isNull() or idx.i() >= dimx or idx.j() >= dimy or idx.k() >= dimz)
    {
        return AABox();
    }
    else
    {
        return AABox( grid_origin + Vector( (0.5+idx.i()) * grid_spacing,
                                            (0.5+idx.j()) * grid_spacing,
                                            (0.5+idx.k()) * grid_spacing ),
                      Vector(grid_spacing) );
    }
}

/** Return the AABox that encompasses the grid box at point 'i'. This
    returns an empty box if there is no such point in the grid */
AABox GridInfo::box(int i) const
{
    return box( arrayToGridIndex(i) );
}

/** Return the AABox that encompasses the grid box that contains the
    point 'p'. Note that returns a null AABox if the point is not in the grid */
AABox GridInfo::box(const Vector &point) const
{
    return box( pointToGridIndex(point) );
}
