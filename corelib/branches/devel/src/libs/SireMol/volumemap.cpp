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

#include "volumemap.h"

#include "molecule.h"
#include "atomcoords.h"
#include "atomelements.h"

#include "SireUnits/units.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMol;
using namespace SireBase;
using namespace SireVol;
using namespace SireUnits;
using namespace SireStream;

static const RegisterMetaType<VolumeMap> r_volmap;

QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const VolumeMap &volmap)
{
    writeHeader(ds, r_volmap, 1);
    
    SharedDataStream sds(ds);
    
    sds << volmap.grid_info << volmap.grid_spacing.to(angstrom)
        << qint32(volmap.map_type) << qint32(volmap.fill_type)
        << volmap.occ << volmap.nsamples << volmap.max_grid_points
        << static_cast<const Property&>(volmap);
    
    return ds;
}

QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, VolumeMap &volmap)
{
    VersionID v = readHeader(ds, r_volmap);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        double grid_spacing;
        qint32 map_type, fill_type;
        
        sds >> volmap.grid_info >> grid_spacing
            >> map_type >> fill_type
            >> volmap.occ >> volmap.nsamples >> volmap.max_grid_points
            >> static_cast<Property&>(volmap);
        
        volmap.grid_spacing = grid_spacing * angstrom;
        volmap.map_type = VolumeMap::MapType(map_type);
        volmap.fill_type = VolumeMap::FillType(fill_type);
    }
    else
        throw version_error(v, "1", r_volmap, CODELOC);
    
    return ds;
}

static const Length default_grid_spacing = 0.5 * angstrom;
static const VolumeMap::MapType default_map_type = VolumeMap::AVERAGE;
static const VolumeMap::FillType default_fill_type = VolumeMap::VDW_RADIUS;
static const qint32 default_max_grid_points = 256*256*256;  // 64 MB!!!, but 128^3 A^3 at default

static const Length min_grid_spacing = 0.01 * angstrom;

/** Default constructor */
VolumeMap::VolumeMap() : ConcreteProperty<VolumeMap,Property>(),
                         grid_spacing(default_grid_spacing),
                         map_type(default_map_type),
                         fill_type(default_fill_type),
                         nsamples(0), max_grid_points(default_max_grid_points)
{}

/** Construct, specifying the grid spacing */
VolumeMap::VolumeMap(const Length &spacing)
          : ConcreteProperty<VolumeMap,Property>(),
            grid_spacing(spacing),
            map_type(default_map_type),
            fill_type(default_fill_type),
            nsamples(0), max_grid_points(default_max_grid_points)
{
    if (grid_spacing.value() < min_grid_spacing)
    {
        grid_spacing = min_grid_spacing;
    }
}

/** Construct, specifying the type of map to build */
VolumeMap::VolumeMap(MapType map_type)
          : ConcreteProperty<VolumeMap,Property>(),
            grid_spacing(default_grid_spacing),
            map_type(map_type),
            fill_type(default_fill_type),
            nsamples(0), max_grid_points(default_max_grid_points)
{}

/** Construct, specifying the method used to assign atoms to grid points */
VolumeMap::VolumeMap(FillType fill_type)
          : ConcreteProperty<VolumeMap,Property>(),
            grid_spacing(default_grid_spacing),
            map_type(default_map_type),
            fill_type(fill_type),
            nsamples(0), max_grid_points(default_max_grid_points)
{}

/** Construct, specifying the grid spacing and type of map to build */
VolumeMap::VolumeMap(const Length &spacing, MapType map_type)
          : ConcreteProperty<VolumeMap,Property>(),
            grid_spacing(spacing),
            map_type(map_type),
            fill_type(default_fill_type),
            nsamples(0), max_grid_points(default_max_grid_points)
{
    if (grid_spacing.value() < min_grid_spacing)
    {
        grid_spacing = min_grid_spacing;
    }
}

/** Construct, specifying the grid spacing and method of assigning atoms to points */
VolumeMap::VolumeMap(const Length &spacing, FillType fill_type)
          : ConcreteProperty<VolumeMap,Property>(),
            grid_spacing(spacing),
            map_type(default_map_type),
            fill_type(fill_type),
            nsamples(0), max_grid_points(default_max_grid_points)
{
    if (grid_spacing.value() < min_grid_spacing)
    {
        grid_spacing = min_grid_spacing;
    }
}

/** Construct, specifying the type of map and method of assigning atoms to points */
VolumeMap::VolumeMap(FillType fill_type, MapType map_type)
          : ConcreteProperty<VolumeMap,Property>(),
            grid_spacing(default_grid_spacing),
            map_type(map_type),
            fill_type(fill_type),
            nsamples(0), max_grid_points(default_max_grid_points)
{}

/** Construct, specifying the grid spacing, map type and method of assigning atoms to points */
VolumeMap::VolumeMap(const Length &spacing, FillType fill_type, MapType map_type)
          : ConcreteProperty<VolumeMap,Property>(),
            grid_spacing(spacing),
            map_type(map_type),
            fill_type(fill_type),
            nsamples(0), max_grid_points(default_max_grid_points)
{
    if (grid_spacing.value() < min_grid_spacing)
    {
        grid_spacing = min_grid_spacing;
    }
}

/** Copy constructor */
VolumeMap::VolumeMap(const VolumeMap &other)
          : ConcreteProperty<VolumeMap,Property>(other),
            grid_info(other.grid_info), grid_spacing(other.grid_spacing),
            map_type(other.map_type), fill_type(other.fill_type),
            occ(other.occ), nsamples(other.nsamples), max_grid_points(other.max_grid_points)
{}

/** Destructor */
VolumeMap::~VolumeMap()
{}

/** Copy assignment operator */
VolumeMap& VolumeMap::operator=(const VolumeMap &other)
{
    if (this != &other)
    {
        grid_info = other.grid_info;
        grid_spacing = other.grid_spacing;
        map_type = other.map_type;
        fill_type = other.fill_type;
        occ = other.occ;
        nsamples = other.nsamples;
        max_grid_points = other.max_grid_points;
    }
    
    return *this;
}

/** Comparison operator */
bool VolumeMap::operator==(const VolumeMap &other) const
{
    return this == &other or
           (grid_info == other.grid_info and grid_spacing == other.grid_spacing and
            map_type == other.map_type and fill_type == other.fill_type and
            occ == other.occ and nsamples == other.nsamples and
            max_grid_points == other.max_grid_points);
}

/** Comparison operator */
bool VolumeMap::operator!=(const VolumeMap &other) const
{
    return not operator==(other);
}

const char* VolumeMap::typeName()
{
    return QMetaType::typeName( qMetaTypeId<VolumeMap>() );
}

const char* VolumeMap::what() const
{
    return VolumeMap::typeName();
}

/** Return whether or not this map is empty */
bool VolumeMap::isEmpty() const
{
    return nsamples == 0;
}

QString VolumeMap::toString() const
{
    if (isEmpty())
        return QObject::tr("VolumeMap( gridSpacing() == %1 A )").arg(grid_spacing.to(angstrom));
    else
        return QObject::tr("VolumeMap( gridInfo() == %1, nSamples() == %2 )")
                    .arg(gridInfo().toString()).arg(nsamples);
}

/** Return the number of grid points in the grid */
int VolumeMap::nGridPoints() const
{
    return grid_info.nPoints();
}

/** Return the maximum number of grid points available to this map */
int VolumeMap::nMaxGridPoints() const
{
    return max_grid_points;
}

/** Set the maximum number of grid points available to this map. Note that
    if this is less than the current number of points, then it prevents
    the map from growing (but doesn't shrink the map) */
void VolumeMap::setNMaxGridPoints(int nmax)
{
    max_grid_points = nmax;
}

/** Return the grid spacing */
Length VolumeMap::gridSpacing() const
{
    return grid_spacing;
}

/** Clear the current grid */
void VolumeMap::clear()
{
    nsamples = 0;
    occ = QVector<float>();
    grid_info = GridInfo();
}

/** Set the desired grid spacing. If this is not the same as the current
    grid, then the current map is re-mapped onto the new grid */
void VolumeMap::setGridSpacing(Length new_spacing)
{
    if (new_spacing < min_grid_spacing)
        new_spacing = min_grid_spacing;
    
    if (grid_spacing != new_spacing)
    {
        redimensionGrid(new_spacing);
    }
    
    grid_spacing = new_spacing;
}

/** Return the type of map */
VolumeMap::MapType VolumeMap::mapType() const
{
    return map_type;
}

/** Set the type of map. Supported types are;

    AVERAGE - accumulates the average occupancy of each point
    MAXIMUM - accumulates the maximum occupancy of each point
    SUM     - accumulates the summed occupancy of each point

    Default is AVERAGE

    Note that changing the map type will clear the current grid
*/
void VolumeMap::setMapType(MapType new_type)
{
    if (new_type != map_type)
        clear();
    
    map_type = new_type;
}

/** Return the method used to assign atoms to grid points */
VolumeMap::FillType VolumeMap::fillType() const
{
    return fill_type;
}

/** Set the method to assign atoms to grid points. Supported methods are;

    POINT_ATOMS - atoms are only assigned to the grid point closest to their center
    VDW_RADIUS  - atoms are assigned to all points underneath their VDW radius
    COVALENT_RADIUS - atoms are assigned to all points underneath their covalent radius
    BOND_ORDER_RADIUS - atoms are assigned to all points underneath their bond order radius
    
    Default is VDW_RADIUS
    
    Note that changing the fill type will clear the current grid
*/
void VolumeMap::setFillType(FillType new_type)
{
    if (new_type != fill_type)
        clear();
    
    fill_type = new_type;
}

/** Return information about the grid. Note that the grid will grow automatically
    to cover atoms as they are added to the map */
const GridInfo& VolumeMap::gridInfo() const
{
    return grid_info;
}

/** Return the current occupancy map. This array of values should be read in conjunction
    with the current GridInfo */
const QVector<float>& VolumeMap::occupancy() const
{
    return occ;
}

void VolumeMap::add(const MoleculeView &molecule, const PropertyMap &map = PropertyMap());
void VolumeMap::add(const MoleculeView &mol0, const MoleculeView &mol1,
         const PropertyMap &map = PropertyMap());
void VolumeMap::add(const QList<PartialMolecule> &molecules, const PropertyMap &map = PropertyMap());

void VolumeMap::add(const Molecules &molecules, const PropertyMap &map = PropertyMap());
void VolumeMap::add(const Molecules &mols0, const Molecules &mols1,
         const PropertyMap &map = PropertyMap());
void VolumeMap::add(const QList<Molecules> &molecules, const PropertyMap &map = PropertyMap());

void VolumeMap::add(const MoleculeGroup &molecules, const PropertyMap &map = PropertyMap());
void VolumeMap::add(const MoleculeGroup &mols0, const MoleculeGroup &mols1,
         const PropertyMap &map = PropertyMap());
void VolumeMap::add(const QList<MoleculeGroup> &molecules, const PropertyMap &map = PropertyMap());

void VolumeMap::add(const VolumeMap &other);
void VolumeMap::add(const GridInfo &gridinfo, const QVector<float> &values);
