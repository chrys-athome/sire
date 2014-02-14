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

#ifndef SIREMM_GRIDINFO_H
#define SIREMM_GRIDINFO_H

#include "SireVol/aabox.h"
#include "SireUnits/dimensions.h"

SIRE_BEGIN_HEADER

namespace SireMM
{
class GridInfo;
}

QDataStream& operator<<(QDataStream&, const SireMM::GridInfo&);
QDataStream& operator>>(QDataStream&, SireMM::GridInfo&);

namespace SireMM
{

using SireMaths::Vector;
using SireVol::AABox;
using SireUnits::Dimension::Length;

/** Very simple class providing a grid index */
class SIREMM_EXPORT GridIndex
{
public:
    GridIndex(int i=0, int j=0, int k=0) : _i(i), _j(j), _k(k)
    {}
    
    GridIndex(const GridIndex &other) : _i(other._i), _j(other._j), _k(other._k)
    {}
    
    ~GridIndex()
    {}
    
    GridIndex& operator=(const GridIndex &other)
    {
        _i = other._i;
        _j = other._j;
        _k = other._k;
        return *this;
    }
    
    bool operator==(const GridIndex &other) const
    {
        return _i == other._i and _j == other._j and _k == other._k;
    }
    
    bool operator!=(const GridIndex &other) const
    {
        return not operator==(other);
    }
    
    static const char* typeName();
    
    const char* what() const;
    
    QString toString() const;
    
    int i() const
    {
        return _i;
    }
    
    int j() const
    {
        return _j;
    }
    
    int k() const
    {
        return _k;
    }
    
private:
    int _i, _j, _k;
};

/** This is a simple class that describes a 3D regular grid

    @author Christopher Woods
*/
class SIREMM_EXPORT GridInfo
{

friend QDataStream& ::operator<<(QDataStream&, const GridInfo&);
friend QDataStream& ::operator>>(QDataStream&, GridInfo&);

public:
    GridInfo();
    GridInfo(const AABox &dimensions, Length spacing);

    GridInfo(const GridInfo &other);

    ~GridInfo();
    
    GridInfo& operator=(const GridInfo &other);
    
    bool operator==(const GridInfo &other) const;
    bool operator!=(const GridInfo &other) const;
    
    static const char* typeName();
    
    const char* what() const;
    
    QString toString() const;
    
    Vector operator[](int i) const;
    Vector operator[](const GridIndex &idx) const;
    
    Vector at(int i) const;
    Vector at(const GridIndex &idx) const;
    Vector at(int i, int j, int k) const;
    
    Vector getitem(int i) const;
    
    qint32 dimX() const;
    qint32 dimY() const;
    qint32 dimZ() const;
    
    int nPoints() const;
    int count() const;
    int size() const;
    
    bool isEmpty() const;
    
    Length spacing() const;
    const AABox& dimensions() const;

    int gridToArrayIndex(int i, int j, int k) const;
    int gridToArrayIndex(const GridIndex &idx) const;

    GridIndex arrayToGridIndex(int i) const;

private:
    /** The dimensions of the grid */
    AABox grid_dimensions;
    
    /** The spacing of the grid */
    float grid_spacing;
    
    /** The number of grid points along x, y, and z */
    qint32 dimx, dimy, dimz;
};

#ifndef SIRE_SKIP_INLINE_FUNCTIONS

/** Get the grid index from the passed array index */
inline GridIndex GridInfo::arrayToGridIndex(int i) const
{
    int ix = i / (dimy*dimz);
    i -= ix*dimy*dimz;

    int iy = i / dimz;
    i -= iy*dimz;

    return GridIndex(ix,iy,i);
}

/** Get the index into the 1D array for the point corresponding to grid 
    indicies i,j,k */
inline int GridInfo::gridToArrayIndex(int i, int j, int k) const
{
    return i*(dimy*dimz) + j*dimz + k;
}

/** Return the coordinates at the point with index 'idx' */
inline Vector GridInfo::operator[](const GridIndex &idx) const
{
    return grid_dimensions.minCoords() + Vector(idx.i() * grid_spacing,
                                                idx.j() * grid_spacing,
                                                idx.k() * grid_spacing);
}

/** Return the coordinates of the ith grid point */
inline Vector GridInfo::operator[](int i) const
{
    return operator[](arrayToGridIndex(i));
}

/** Return the coordinates of the point at grid index [i,j,k] */
inline Vector GridInfo::at(int i, int j, int k) const
{
    return operator[](GridIndex(i,j,k));
}

/** Return the coordinates at the point with index 'idx' */
inline Vector GridInfo::at(const GridIndex &idx) const
{
    return operator[](idx);
}

/** Return the coordinates of the ith grid point */
inline Vector GridInfo::at(int i) const
{
    return operator[](i);
}

/** Return the number of grid points along the x dimension */
inline qint32 GridInfo::dimX() const
{
    return dimx;
}

/** Return the number of grid points along the y dimension */
inline qint32 GridInfo::dimY() const
{
    return dimy;
}

/** Return the number of grid points along the z dimension */
inline qint32 GridInfo::dimZ() const
{
    return dimz;
}

/** Return the total number of grid points */
inline int GridInfo::nPoints() const
{
    return dimx * dimy * dimz;
}

/** Return the total number of grid points */
inline int GridInfo::count() const
{
    return nPoints();
}

/** Return the total number of grid points */
inline int GridInfo::size() const
{
    return nPoints();
}

/** Return whether or not this grid is empty */
inline bool GridInfo::isEmpty() const
{
    return dimx == 0;
}

/** Return the grid spacing */
inline Length GridInfo::spacing() const
{
    return Length(grid_spacing);
}

/** Return the dimensions of the grid */
inline const AABox& GridInfo::dimensions() const
{
    return grid_dimensions;
}

#endif // SIRE_SKIP_INLINE_FUNCTIONS

}

Q_DECLARE_METATYPE( SireMM::GridInfo )

SIRE_EXPOSE_CLASS( SireMM::GridInfo )

SIRE_END_HEADER

#endif
