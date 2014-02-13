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

using SireVol::AABox;
using SireUnits::Dimension::Length;

/** Very simple class providing a grid index */
class SIREMM_EXPORT GridIndex
{
public:
    GridIndex(int i=0, int j=0, int k=0);
    GridIndex(const GridIndex &other);
    
    ~GridIndex();
    
    GridIndex& operator=(const GridIndex &other);
    
    bool operator==(const GridIndex &other) const;
    bool operator!=(const GridIndex &other) const;
    
    static const char* typeName();
    
    const char* what() const;
    
    QString toString() const;
    
    int i() const;
    int j() const;
    int k() const;
    
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
    
    int dimX() const;
    int dimY() const;
    int dimZ() const;
    
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

/** Get the index into the 1D array for the point corresponding to grid 
    indicies i,j,k */
int GridInfo::gridToArrayIndex(int i, int j, int k) const
{
    return i*(dimy*dimz) + j*dimz + k;
}



#endif // SIRE_SKIP_INLINE_FUNCTIONS

}

Q_DECLARE_METATYPE( SireMM::GridInfo )

SIRE_EXPOSE_CLASS( SireMM::GridInfo )

SIRE_END_HEADER

#endif
