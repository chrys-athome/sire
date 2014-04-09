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

#include "volmapmonitor.h"

#include "SireMol/atomelements.h"
#include "SireMol/atommasses.h"
#include "SireMol/atomcoords.h"
#include "SireMol/molecule.h"
#include "SireMol/partialmolecule.h"
#include "SireMol/moleculegroup.h"
#include "SireMol/mgnum.h"

#include "SireMM/gridinfo.h"

#include "SireSystem/system.h"

#include "SireUnits/units.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireSystem;
using namespace SireMol;
using namespace SireMM;
using namespace SireBase;
using namespace SireStream;
using namespace SireUnits::Dimension;
using namespace SireUnits;

static const Length default_grid_spacing = 1*angstrom;
static const bool default_skip_light_atoms = false;

namespace SireSystem
{
    namespace detail
    {
        class VolMapMonitorData : public QSharedData
        {
        public:
            VolMapMonitorData() : grid_spacing(default_grid_spacing),
                                  skip_light_atoms(default_skip_light_atoms)
            {}
            
            VolMapMonitorData(const VolMapMonitorData &other)
            {}
            
            ~VolMapMonitorData()
            {}
            
            MolGroupPtr molgroup;
            GridInfo grid_info;
            GridInfo tmp_grid_info;
            Length grid_spacing;
            Length tmp_grid_spacing;
            
            QVector<float> avg_occ;
            QVector<float> tmp_occ;
            
            PropertyMap map;
            
            qint64 nsamples;
            
            bool skip_light_atoms;
            
            void startEvaluation();
            void cancelEvaluation();
            void commitEvaluation();
            
            void resizeGrid();
            
            bool addToGrid(const Vector &coords, const Element &element);
        };
    }
}

static const RegisterMetaType<VolMapMonitor> r_mon;

QDataStream SIRESYSTEM_EXPORT &operator<<(QDataStream &ds, const VolMapMonitor &mon)
{
    writeHeader(ds, r_mon, 1);
    
    if (mon.d.constData() == 0)
        ds << false;
    else
    {
        ds << true;

        SharedDataStream sds(ds);
    
        sds << mon.d->molgroup
            << mon.d->grid_info
            << double( mon.d->grid_spacing.to(angstrom) )
            << mon.d->nsamples << mon.d->skip_light_atoms
            << mon.d->avg_occ
            << mon.d->map
            << static_cast<const SystemMonitor&>(mon);
    }
    
    return ds;
}

QDataStream SIRESYSTEM_EXPORT &operator>>(QDataStream &ds, VolMapMonitor &mon)
{
    VersionID v = readHeader(ds, r_mon);
    
    if (v == 1)
    {
        bool not_null;
        ds >> not_null;
        
        if (not_null)
        {
            if (mon.d.constData() == 0)
                mon.d = new SireSystem::detail::VolMapMonitorData();

            SharedDataStream sds(ds);

            double grid_spacing;

            sds >> mon.d->molgroup
                >> mon.d->grid_info
                >> grid_spacing
                >> mon.d->nsamples >> mon.d->skip_light_atoms
                >> mon.d->avg_occ
                >> mon.d->map
                >> static_cast<SystemMonitor&>(mon);
            
            mon.d->grid_spacing = grid_spacing * angstrom;
        }
        else
        {
            mon.d = 0;
        }
    }
    else
        throw version_error(v, "1", r_mon, CODELOC);
    
    return ds;
}

/** Null constructor */
VolMapMonitor::VolMapMonitor()
              : ConcreteProperty<VolMapMonitor,SystemMonitor>()
{}

/** Construct specifying the grid spacing */
VolMapMonitor::VolMapMonitor(const SireUnits::Dimension::Length &grid_spacing)
              : ConcreteProperty<VolMapMonitor,SystemMonitor>()
{
    setGridSpacing(grid_spacing);
}

/** Construct, specifying the molecule group to be monitored */
VolMapMonitor::VolMapMonitor(const MoleculeGroup &group, const PropertyMap &map)
              : ConcreteProperty<VolMapMonitor,SystemMonitor>()
{
    setGroup(group, map);
}

/** Construct, specifying the molecule group to be monitored and the grid spacing 
    for the occupancy grid */
VolMapMonitor::VolMapMonitor(const MoleculeGroup &group,
                             const SireUnits::Dimension::Length &grid_spacing,
                             const PropertyMap &map)
              : ConcreteProperty<VolMapMonitor,SystemMonitor>()
{
    setGroup(group, map);
    setGridSpacing(grid_spacing);
}

/** Construct, specifying the molecule group to be monitored
    and whether or not to ignore light atoms */
VolMapMonitor::VolMapMonitor(const MoleculeGroup &group,
                             bool skip_light_atoms,
                             const PropertyMap &map)
              : ConcreteProperty<VolMapMonitor,SystemMonitor>()
{
    setGroup(group, map);
    setSkipLightAtoms(skip_light_atoms);
}

/** Construct, specifying the molecule group to be monitored, the grid spacing
    for the occupancy grid, and whether or not to ignore light atoms */
VolMapMonitor::VolMapMonitor(const MoleculeGroup &group,
                             const SireUnits::Dimension::Length &grid_spacing,
                             bool skip_light_atoms,
                             const PropertyMap &map)
              : ConcreteProperty<VolMapMonitor,SystemMonitor>()
{
    setGroup(group, map);
    setGridSpacing(grid_spacing);
    setSkipLightAtoms(skip_light_atoms);
}

/** Copy constructor */
VolMapMonitor::VolMapMonitor(const VolMapMonitor &other)
              : ConcreteProperty<VolMapMonitor,SystemMonitor>(), d(other.d)
{}

/** Destructor */
VolMapMonitor::~VolMapMonitor()
{}

/** Copy assignment operator */
VolMapMonitor& VolMapMonitor::operator=(const VolMapMonitor &other)
{
    d = other.d;
    return *this;
}

/** Comparison operator */
bool VolMapMonitor::operator==(const VolMapMonitor &other) const
{
    if (d.constData() == 0)
        return other.d.constData() == 0;
    else
        return d.constData() == other.d.constData() or
               (d->molgroup == other.d->molgroup and
                d->grid_info == other.d->grid_info and
                d->nsamples == other.d->nsamples and
                d->skip_light_atoms == other.d->skip_light_atoms and
                d->map == other.d->map and
                d->avg_occ == other.d->avg_occ);
}

/** Comparison operator */
bool VolMapMonitor::operator!=(const VolMapMonitor &other) const
{
    return not operator==(other);
}

const char* VolMapMonitor::typeName()
{
    return QMetaType::typeName( qMetaTypeId<VolMapMonitor>() );
}

const char* VolMapMonitor::what() const
{
    return VolMapMonitor::typeName();
}

QString VolMapMonitor::toString() const
{
    if (d.constData() == 0)
        return QObject::tr("VolMapMonitor::null");
    else
        return QObject::tr("VolMapMonitor( group() == %1, gridInfo() == %2, nSamples() == %3 )")
                    .arg(group().toString())
                    .arg(gridInfo().toString())
                    .arg(nSamples());
}

/** Return the molecule group whose atoms are being monitored */
const MoleculeGroup& VolMapMonitor::group() const
{
    if (d.constData() == 0)
        return MoleculeGroup::null();
    else
        return d->molgroup.read();
}

/** Set the molecule group being monitored (together with the property map 
    used to get the required properties from that group) */
void VolMapMonitor::setGroup(const MoleculeGroup &molgroup, const PropertyMap &map)
{
    if (molgroup.number() != group().number())
    {
        if (d.constData() == 0)
            d = new SireSystem::detail::VolMapMonitorData();
        
        clearStatistics();
        d->molgroup = molgroup;
        d->map = map;
    }
}

/** Return whether or not we are skipping light atoms (e.g. hydrogen) */
bool VolMapMonitor::skippingLightAtoms() const
{
    if (d.constData() == 0)
        return false;
    else
        return d->skip_light_atoms;
}

/** Turn on skipping light atoms */
void VolMapMonitor::setSkipLightAtoms(bool on)
{
    if (skippingLightAtoms() != on)
    {
        if (d.constData() == 0)
            d = new SireSystem::detail::VolMapMonitorData();
        
        d->skip_light_atoms = on;
    }
}

/** Return the spacing of the grid */
Length VolMapMonitor::gridSpacing() const
{
    if (d.constData() != 0)
        return d->grid_spacing;
    else
        return default_grid_spacing;
}

/** Set the grid spacing on which we monitor the space */
void VolMapMonitor::setGridSpacing(const Length &grid_spacing)
{
    if (grid_spacing != gridSpacing())
    {
        if (d.constData() == 0)
            d = new SireSystem::detail::VolMapMonitorData();
        
        clearStatistics();
        d->grid_spacing = grid_spacing;
    }
}

/** Return the grid dimensions */
GridInfo VolMapMonitor::gridInfo() const
{
    if (nSamples() == 0)
        return GridInfo();
    else
        return d->grid_info;
}

/** Return the property map used to find the properties needed by this monitor */
PropertyMap VolMapMonitor::map() const
{
    if (d.constData() == 0)
        return PropertyMap();
    else
        return d->map;
}

/** Set the property map to be used by this monitor */
void VolMapMonitor::setPropertyMap(const PropertyMap &m)
{
    if (map() != m)
    {
        if (d.constData() == 0)
            d = new SireSystem::detail::VolMapMonitorData();
        
        clearStatistics();
        d->map = m;
    }
}

/** Return the average occupancy. This is a linear array that can be
    accessed using the accompanying GridInfo returned by gridInfo() */
QVector<float> VolMapMonitor::averageOccupancy() const
{
    if (nSamples() == 0)
        return QVector<float>();
    else
        return d->avg_occ;
}

/** Return the number of samples recorded so far */
qint64 VolMapMonitor::nSamples() const
{
    if (d.constData() == 0)
        return 0;
    else
        return d->nsamples;
}

/** Clear all statistics */
void VolMapMonitor::clearStatistics()
{
    if (d.constData() != 0)
    {
        d->nsamples = 0;
        d->avg_occ = QVector<float>();
        d->grid_info = GridInfo();
    }
}

void SireSystem::detail::VolMapMonitorData::startEvaluation()
{
    if (not avg_occ.isEmpty())
    {
        tmp_occ = QVector<float>(avg_occ.count(), 0.0);
        tmp_grid_info = grid_info;
        tmp_grid_spacing = grid_spacing;
    }
}

void SireSystem::detail::VolMapMonitorData::cancelEvaluation()
{
    tmp_occ.clear();
    tmp_grid_info = GridInfo();
    tmp_grid_spacing = Length(0);
}

void SireSystem::detail::VolMapMonitorData::commitEvaluation()
{
    if (grid_spacing != tmp_grid_spacing)
    {
        //we have to throw away the old grid as the grid spacing has changed!
        if (not avg_occ.isEmpty())
        {
            qDebug() << "WARNING: Throwing away prior data because the grid spacing has changed!";
            avg_occ.clear();
            grid_info = GridInfo();
            grid_spacing = tmp_grid_spacing;
        }
    }

    if (avg_occ.isEmpty())
    {
        //just copy across the data
        grid_info = tmp_grid_info;
        avg_occ = tmp_occ;
    }
    else
    {
        if (grid_info != tmp_grid_info)
        {
            //we need to expand the old grid to match the new...
            QVector<float> new_avg_occ( tmp_grid_info.nPoints() );
            
            for (int i=0; i<tmp_grid_info.nPoints(); ++i)
            {
                int old_idx = grid_info.pointToArrayIndex(tmp_grid_info.point(i));
                
                if (old_idx < 0 or old_idx >= grid_info.nPoints())
                    throw SireError::program_bug( QObject::tr(
                            "Problem remapping grid: %1 to %2, %3 is %4")
                                .arg(grid_info.toString())
                                .arg(tmp_grid_info.toString())
                                .arg(i).arg(tmp_grid_info.point(i).toString()), CODELOC );

                new_avg_occ.data()[i] = avg_occ.constData()[old_idx];
            }
            
            avg_occ = new_avg_occ;
            grid_info = tmp_grid_info;
        }
        
        if (avg_occ.count() != tmp_occ.count())
        {
            throw SireError::program_bug( QObject::tr(
                    "Something went wrong with committing the grid? %1 vs. %2")
                        .arg(avg_occ.count()).arg(tmp_occ.count()), CODELOC );
        }
        
        float *a = avg_occ.data();
        const float *b = tmp_occ.constData();
        
        for (int i=0; i<avg_occ.count(); ++i)
        {
            a[i] += b[i];
        }
    }
    
    tmp_occ.clear();
    tmp_grid_info = GridInfo();
}

/** This internal function is called during evaluation to resize the temporary grid */
void SireSystem::detail::VolMapMonitorData::resizeGrid()
{
    //we need to create the grid - create a grid that encompasses all of the atoms in
    //the molecule group, and that extends a small way either side
    MoleculeGroup::const_iterator it = molgroup.read().constBegin();
    
    AABox box = it.value().evaluate().aaBox();
    
    for (++it; it != molgroup.read().constEnd(); ++it)
    {
        box += it.value().evaluate().aaBox();
    }
    
    //construct a default grid, with a 5 A buffer around the edge
    box = AABox::from( box.minCoords() - Vector(5,5,5),
                       box.maxCoords() + Vector(5,5,5) );
    
    if (not tmp_grid_info.isEmpty())
    {
        qDebug() << "Resizing the volume occupation grid...";
    
        //make sure that the new and old grids overlap perfectly
        const AABox old_box = tmp_grid_info.dimensions();

        Vector mincoords = old_box.minCoords().min( box.minCoords() );
        Vector maxcoords = old_box.maxCoords().max( box.maxCoords() );
        
        Vector dmin = old_box.minCoords() - mincoords;
        Vector dmax = maxcoords - old_box.maxCoords();

        const float gs = tmp_grid_spacing.value();
        const float inv_gs = 1.0 / gs;

        if (not dmin.isZero())
        {
            if (dmin.x() > 0)
                dmin.setX( (int(dmin.x() * inv_gs) + 1) * gs );

            if (dmin.y() > 0)
                dmin.setY( (int(dmin.y() * inv_gs) + 1) * gs );

            if (dmin.z() > 0)
                dmin.setZ( (int(dmin.z() * inv_gs) + 1) * gs );
        }
        
        if (not dmax.isZero())
        {
            if (dmax.x() > 0)
                dmax.setX( (int(dmax.x() * inv_gs) + 1) * gs );

            if (dmax.y() > 0)
                dmax.setY( (int(dmax.y() * inv_gs) + 1) * gs );

            if (dmax.z() > 0)
                dmax.setZ( (int(dmax.z() * inv_gs) + 1) * gs );
        }
        
        mincoords -= dmin;
        maxcoords += dmax;
        
        box = AABox::from(mincoords, maxcoords);
        
        GridInfo new_tmp_grid_info( box, tmp_grid_spacing );
        
        QVector<float> new_tmp_occ(new_tmp_grid_info.nPoints(), 0.0);
        
        for (int i=0; i<new_tmp_grid_info.nPoints(); ++i)
        {
            int old_idx = tmp_grid_info.pointToArrayIndex( new_tmp_grid_info.point(i) );
        
            if (old_idx < 0 or old_idx >= tmp_grid_info.nPoints())
                throw SireError::program_bug( QObject::tr(
                        "Problem remapping grid: %1 to %2, %3 is %4")
                            .arg(tmp_grid_info.toString())
                            .arg(new_tmp_grid_info.toString())
                            .arg(i).arg(new_tmp_grid_info.point(i).toString()), CODELOC );
            
            new_tmp_occ.data()[i] = tmp_occ.constData()[old_idx];
        }
        
        tmp_grid_info = new_tmp_grid_info;
        tmp_occ = new_tmp_occ;
        
        qDebug() << "Resized grid equals" << tmp_grid_info.toString();
    }
    else
    {
        tmp_grid_info = GridInfo(box, grid_spacing);

        bool ok = true;
    
        //make sure that we don't have ridiculously large grids!
        while (tmp_grid_info.nPoints() > (256*256*256))
        {
            tmp_grid_spacing *= 1.1;
            tmp_grid_info = GridInfo(box, tmp_grid_spacing);
            ok = false;
        }
    
        qDebug() << "Collecting occupation stats on the grid" << tmp_grid_info.toString();
    
        if (not ok)
        {
            qDebug() << "Had to reduce the grid spacing to" << tmp_grid_spacing.value()
                     << "A to ensure that the grid does not get too big!";
        }
    
        tmp_occ = QVector<float>(tmp_grid_info.nPoints(), 0.0);
    }
}

/** Internal function used to add the passed atom to the grid. This returns true if the
    atom was successfully added, or false if the grid needs resizing because the atom
    is outside the grid */
bool SireSystem::detail::VolMapMonitorData::addToGrid(const Vector &coords, const Element &element)
{
    const float rad = element.vdwRadius();
    const float rad2 = rad*rad;
    
    GridIndex idx = tmp_grid_info.pointToGridIndex(coords);
    
    if (idx.isNull())
        return false;
    
    const int nboxes = int(rad / grid_spacing.value()) + 2;

    const Vector center_point = tmp_grid_info.point(idx);

    QVarLengthArray<int> occupied_points;
    
    for (int i=-nboxes; i<=nboxes; ++i)
    {
        for (int j=-nboxes; j<=nboxes; ++j)
        {
            for (int k=-nboxes; k<=nboxes; ++k)
            {
                Vector grid_point = center_point + Vector(i*grid_spacing.value(),
                                                          j*grid_spacing.value(),
                                                          k*grid_spacing.value());
            
                if (Vector::distance2(grid_point,coords) <= rad2)
                {
                    int point_idx = tmp_grid_info.gridToArrayIndex(idx.i() + i,
                                                                   idx.j() + j,
                                                                   idx.k() + k);

                    if (point_idx < 0 or point_idx >= tmp_occ.count())
                        return false;
                
                    occupied_points.push_back(point_idx);
                }
            }
        }
    }
    
    //we've found all of the occupied points for this atom - add them onto the grid
    float *tmp_o = tmp_occ.data();
    
    for (int i=0; i<occupied_points.count(); ++i)
    {
        tmp_o[ occupied_points.constData()[i] ] = 1;
    }
    
    return true;
}

/** Monitor the system */
void VolMapMonitor::monitor(System &system)
{
    if (d.constData() == 0)
        return;
    
    //now update the molecule group, if needed
    if (system.contains(group().number()))
    {
        d->molgroup = system[group().number()];
    }
    else
    {
        d->molgroup.edit().update(system.molecules());
    }
    
    if (d->molgroup.read().isEmpty())
        return;
    
    const PropertyName coords_property = d->map["coordinates"];
    const PropertyName element_property = d->map["element"];
    
    if (d->nsamples == 0)
    {
        //we need to create the grid - create a grid that encompasses all of the atoms in
        //the molecule group, and that extends a small way either side
        MoleculeGroup::const_iterator it = d->molgroup.read().constBegin();
        
        AABox box = it.value().evaluate().aaBox();
        
        for (++it; it != d->molgroup.read().constEnd(); ++it)
        {
            box += it.value().evaluate().aaBox();
        }
        
        //construct a default grid, with a 5 A buffer around the edge
        box = AABox::from( box.minCoords() - Vector(5,5,5),
                           box.maxCoords() + Vector(5,5,5) );
        
        d->grid_info = GridInfo(box, d->grid_spacing);
        
        bool ok = true;
        
        //make sure that we don't have ridiculously large grids!
        while (d->grid_info.nPoints() > (256*256*256))
        {
            d->grid_spacing *= 1.1;
            d->grid_info = GridInfo(box, d->grid_spacing);
            ok = false;
        }
        
        if (not ok)
        {
            qDebug() << "Had to reduce the grid spacing to" << d->grid_spacing.value()
                     << "A to ensure that the grid does not get too big!";
        }
        
        d->avg_occ = QVector<float>(d->grid_info.nPoints(), 0.0);
    }
    
    try
    {
        d->startEvaluation();
    
        //now loop over all of the atoms and add them onto the grid
        for (MoleculeGroup::const_iterator it = d->molgroup.read().constBegin();
             it != d->molgroup.read().constEnd();
             ++it)
        {
            const AtomCoords &coords = it.value().data().property(coords_property).asA<AtomCoords>();
            const AtomElements &elems = it.value().data().property(element_property)
                                                         .asA<AtomElements>();

            if (it.value().selectedAll())
            {
                //loop over all atoms and add them to the grid
                for (int i=0; i<coords.nCutGroups(); ++i)
                {
                    const Vector *ca = coords.data(CGIdx(i));
                    const Element *ea = elems.data(CGIdx(i));
                    
                    for (int j=0; j<coords.nAtoms(CGIdx(i)); ++j)
                    {
                        if ((not d->skip_light_atoms) or ea[j].nProtons() >= 6)
                        {
                            while (not d->addToGrid(ca[j], ea[j]))
                            {
                                //the grid needs to be bigger
                                d->resizeGrid();
                            }
                        }
                    }
                }
            }
            else
            {
                //not selected all atoms...
                const AtomSelection selected_atoms = it.value().selection();
                
                for (int i=0; i<coords.nCutGroups(); ++i)
                {
                    const Vector *ca = coords.data(CGIdx(i));
                    const Element *ea = elems.data(CGIdx(i));

                    if (selected_atoms.selectedAll(CGIdx(i)))
                    {
                        for (int j=0; j<coords.nAtoms(CGIdx(i)); ++j)
                        {
                            if ((not d->skip_light_atoms) or ea[j].nProtons() >= 6)
                            {
                                while (not d->addToGrid(ca[j], ea[j]))
                                {
                                    //the grid needs to be bigger
                                    d->resizeGrid();
                                }
                            }
                        }
                    }
                    else
                    {
                        foreach (Index j, selected_atoms.selectedAtoms(CGIdx(i)))
                        {
                            if ((not d->skip_light_atoms) or ea[j.value()].nProtons() >= 6)
                            {
                                while (not d->addToGrid(ca[j.value()], ea[j.value()]))
                                {
                                    //the grid needs to be bigger
                                    d->resizeGrid();
                                }
                            }
                        }
                    }
                }
            }
        }
        
        d->commitEvaluation();
    }
    catch(...)
    {
        d->cancelEvaluation();
        throw;
    }
}
