/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2012  Christopher Woods
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

#include "gridff.h"
#include "cljpotential.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include <QDebug>

using namespace SireMM;
using namespace SireStream;
using namespace SireVol;
using namespace SireMol;
using namespace SireMaths;
using namespace SireBase;

static const RegisterMetaType<GridFF> r_gridff;

QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const GridFF &gridff)
{
    writeHeader(ds, r_gridff, 1);
    
    SharedDataStream sds(ds);
    
    sds << gridff.buffer_size << gridff.grid_spacing
        << gridff.coul_cutoff << gridff.lj_cutoff
        << gridff.calc_grid_error
        << static_cast<const InterGroupCLJFF&>(gridff);
    
    return ds;
}

QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, GridFF &gridff)
{
    VersionID v = readHeader(ds, r_gridff);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        gridff = GridFF();
        
        sds >> gridff.buffer_size >> gridff.grid_spacing
            >> gridff.coul_cutoff >> gridff.lj_cutoff
            >> gridff.calc_grid_error
            >> static_cast<InterGroupCLJFF&>(gridff);
    }
    else
        throw version_error(v, "1", r_gridff, CODELOC);
        
    return ds;
}

/** Empty constructor */
GridFF::GridFF() 
       : ConcreteProperty<GridFF,InterGroupCLJFF>(),
         buffer_size(2), grid_spacing(0.25), 
         coul_cutoff(15), lj_cutoff(5),
         calc_grid_error(false)
{}

/** Construct a grid forcefield with a specified name */
GridFF::GridFF(const QString &name) 
       : ConcreteProperty<GridFF,InterGroupCLJFF>(name),
         buffer_size(2), grid_spacing(0.25), 
         coul_cutoff(15), lj_cutoff(5),
         calc_grid_error(false)
{}

/** Copy constructor */
GridFF::GridFF(const GridFF &other)
       : ConcreteProperty<GridFF,InterGroupCLJFF>(other),
         gridbox(other.gridbox),
         buffer_size(other.buffer_size), grid_spacing(other.grid_spacing),
         coul_cutoff(other.coul_cutoff), lj_cutoff(other.lj_cutoff),
         dimx(other.dimx), dimy(other.dimy), dimz(other.dimz),
         gridpot(other.gridpot), closemols(other.closemols),
         oldnrgs(other.oldnrgs),
         calc_grid_error(other.calc_grid_error)
{}

/** Destructor */
GridFF::~GridFF()
{}

const char* GridFF::typeName()
{
    return QMetaType::typeName( qMetaTypeId<GridFF>() );
}

const char* GridFF::what() const
{
    return GridFF::typeName();
}

/** Copy assignment operator */
GridFF& GridFF::operator=(const GridFF &other)
{
    if (this != &other)
    {
        gridbox = other.gridbox;
        buffer_size = other.buffer_size;
        grid_spacing = other.grid_spacing;
        coul_cutoff = other.coul_cutoff;
        lj_cutoff = other.lj_cutoff;
        dimx = other.dimx;
        dimy = other.dimy;
        dimz = other.dimz;
        gridpot = other.gridpot;
        closemols = other.closemols;
        oldnrgs = other.oldnrgs;
        calc_grid_error = other.calc_grid_error;
    
        InterGroupCLJFF::operator=(other);
    }
    
    return *this;
}

/** Comparison operator */
bool GridFF::operator==(const GridFF &other) const
{
    return buffer_size == other.buffer_size and
           grid_spacing == other.grid_spacing and
           calc_grid_error == other.calc_grid_error and
           InterGroupCLJFF::operator==(other);
}

/** Comparison operator */
bool GridFF::operator!=(const GridFF &other) const
{
    return not GridFF::operator==(other);
}

GridFF* GridFF::clone() const
{
    return new GridFF(*this);
}

/** Set the buffer when building the grid. This adds a buffer space
    around the grid when it is built, to try to reduce the number of
    times it needs to be rebuilt */
void GridFF::setBuffer(SireUnits::Dimension::Length buffer)
{
    buffer_size = buffer.value();
}

/** Set the grid spacing (the distance between grid points). The
    smaller the spacing the more memory is required to hold the grid,
    but the more accurate the energy */
void GridFF::setGridSpacing(SireUnits::Dimension::Length spacing)
{
    if (grid_spacing != spacing.value())
    {
        grid_spacing = spacing.value();
        gridpot.clear();
    }
}

/** Set the cutoff for the coulomb energy - this can be greater
    than the box size as multiple periodic images can be used */
void GridFF::setCoulombCutoff(SireUnits::Dimension::Length cutoff)
{
    if (coul_cutoff != cutoff.value())
    {
        coul_cutoff = cutoff.value();
        gridpot.clear();
    }
}

/** Set the cutoff for the LJ energy - this can be greater than
    the box size as multiple periodic images can be used */
void GridFF::setLJCutoff(SireUnits::Dimension::Length cutoff)
{
    if (lj_cutoff != cutoff.value())
    {
        lj_cutoff = cutoff.value();
        closemols.clear();
    }
}

/** Set whether or not to calculate the exact energy too, so that
    the grid error can be monitored */
void GridFF::setCalculateGridError(bool on)
{
    calc_grid_error = on;
}

/** Return the buffer size used when building grids */
SireUnits::Dimension::Length GridFF::buffer() const
{
    return SireUnits::Dimension::Length(buffer_size);
}

/** Return the spacing between grid points */
SireUnits::Dimension::Length GridFF::spacing() const
{
    return SireUnits::Dimension::Length(grid_spacing);
}

/** Return the cutoff for the coulomb energy */
SireUnits::Dimension::Length GridFF::coulombCutoff() const
{
    return SireUnits::Dimension::Length(coul_cutoff);
}

/** Return the cutoff for the LJ energy */
SireUnits::Dimension::Length GridFF::ljCutoff() const
{
    return SireUnits::Dimension::Length(lj_cutoff);
}

/** Return whether or not the grid error is being monitored */
bool GridFF::calculatingGridError() const
{
    return calc_grid_error;
}

void GridFF::addToGrid(const CoordGroup &coords, 
                       const GridFF::CLJParameters::Array &params)
{
    BOOST_ASSERT( coords.count() == params.count() );

    double *pot = gridpot.data();

    Vector minpoint = gridbox.minCoords();

    //loop through each atom...
    for (int iat=0; iat<coords.count(); ++iat)
    {
        const Vector &c = coords[iat];
        const detail::CLJParameter &p = params[iat];
        
        //now loop through every point and calculate the coulomb energy
        int ipt = 0;
        
        for (quint32 i=0; i<dimx; ++i)
        {
            double dx = c.x() - (minpoint.x() + (i * grid_spacing));
        
            for (quint32 j=0; j<dimy; ++j)
            {
                double dy = c.y() - (minpoint.y() + (j * grid_spacing));
            
                for (quint32 k=0; k<dimz; ++k)
                {
                    double dz = c.z() - (minpoint.z() + (k * grid_spacing));
                
                    pot[ipt] += p.reduced_charge / std::sqrt(dx*dx + dy*dy + dz*dz);
                    
                    ipt += 1;
                }
            }
        }
    }
}

/** Internal function used to rebuild the coulomb potential grid */
void GridFF::rebuildGrid()
{
    qDebug() << "REBUILDING THE GRID";

    //Get an CoordGroup that contains all of the molecules in group 0
    CoordGroup allcoords0;
    {
        QVector< QVector<Vector> > coords;

        for (ChunkedVector<CLJMolecule>::const_iterator
                                it = mols[0].moleculesByIndex().constBegin();
             it != mols[0].moleculesByIndex().constEnd();
             ++it)
        {
            coords.append( (*it).coordinates().merge().toVector() );
        }

        allcoords0 = CoordGroup(coords);
    }

    AABox group0_box = allcoords0.aaBox();
    
    qDebug() << "Molecules contained in" << group0_box.toString();
    
    //now add the buffer region onto the box - this extends the grid by
    //buffer_size in all dimensions
    if (buffer_size > 0)
        gridbox = AABox::from( group0_box.minCoords() - Vector(buffer_size),
                               group0_box.maxCoords() + Vector(buffer_size) );
                         
    qDebug() << "The potential grid extends over" << gridbox.toString();
    
    //now work out how many gridpoints are needed in each dimension
    if (grid_spacing <= 0)
        grid_spacing = 0.5;
        
    Vector boxsize = gridbox.maxCoords() - gridbox.minCoords();
    
    dimx = 1 + int(boxsize.x() / grid_spacing);
    dimy = 1 + int(boxsize.y() / grid_spacing);
    dimz = 1 + int(boxsize.z() / grid_spacing);
    
    qDebug() << "Box grid has points:" << dimx << "x" << dimy << "x" << dimz;

    const quint32 MAX_DIM = 250;

    while ( dimx > MAX_DIM or dimy > MAX_DIM or dimz > MAX_DIM )
    {
        qDebug() << "WARNING: DIMENSION EXCEEDS" << MAX_DIM;

        double grid_x = boxsize.x() / MAX_DIM;
        double grid_y = boxsize.y() / MAX_DIM;
        double grid_z = boxsize.z() / MAX_DIM;
        
        grid_spacing = qMax( grid_x, qMax(grid_y,grid_z) );

        qDebug() << "RESIZING GRID TO" << grid_spacing << "A";

        dimx = 1 + int(boxsize.x() / grid_spacing);
        dimy = 1 + int(boxsize.y() / grid_spacing);
        dimz = 1 + int(boxsize.z() / grid_spacing);
        
        qDebug() << "New box grid has points:" << dimx << "x" << dimy << "x" << dimz;
    }
    
    Vector maxpoint = gridbox.minCoords() + Vector( dimx * grid_spacing,
                                                    dimy * grid_spacing,
                                                    dimz * grid_spacing );

    gridbox = AABox::from(gridbox.minCoords(), maxpoint);
    
    qDebug() << "Adjusted grid extends over" << gridbox.toString();
    
    //create space for the grid
    gridpot = QVector<double>(dimx*dimy*dimz, 0.0);
    gridpot.squeeze();
    
    //now build the grid - we take coordgroups that are within the LJ cutoff
    //and calculate those manually (saving them in the closemols list), and
    //only add coulomb potentials of CoordGroups that are beyond the LJ cutoff
    const ChunkedVector<CLJMolecule> &cljmols = mols[1].moleculesByIndex();
    
    const Space &spce = this->space();
    Vector grid_center = group0_box.center();
    
    for (ChunkedVector<CLJMolecule>::const_iterator it = cljmols.constBegin();
         it != cljmols.constEnd();
         ++it)
    {
        const CLJMolecule &cljmol = *it;

        //loop through each CutGroup of this molecule
        const int ngroups = cljmol.coordinates().count();
        
        const CoordGroup *groups_array = cljmol.coordinates().constData();
        
        const CLJParameters::Array *params_array 
                                = cljmol.parameters().atomicParameters().constData();

        for (int igroup=0; igroup<ngroups; ++igroup)
        {
            const CoordGroup &coordgroup = groups_array[igroup];
            const CLJParameters::Array &params = params_array[igroup];
            
            if (spce.isPeriodic())
            {
                CoordGroup minimum_image = spce.getMinimumImage(coordgroup,
                                                                grid_center);

                //get all copies of the CoordGroup within the cutoff distance
                //of the molecules in group0
                QList< boost::tuple<double,CoordGroup> > images =
                    spce.getCopiesWithin(minimum_image, allcoords0, coul_cutoff);
                    
                //any CoordGroups that are within the LJ cutoff of the 
                //molecules in group0 should be evaluated explicitly
                for (QList< boost::tuple<double,CoordGroup> >::const_iterator
                                                it = images.constBegin();
                     it != images.constEnd();
                     ++it)
                {
                    if (it->get<0>() < lj_cutoff)
                    {
                        //this image should be evaluated explicitly
                        closemols.append( QPair<CoordGroup,CLJParameters::Array>
                                                (it->get<1>(), params) );
                    }
                    else
                    {
                        //evaluate the energy on each of the gridpoints...
                        addToGrid(it->get<1>(), params);
                    }
                }
            }
            else
            {
                if (spce.minimumDistance(coordgroup, allcoords0) < lj_cutoff)
                {
                    //this image should be evaluated explicitly
                    closemols.append( QPair<CoordGroup,CLJParameters::Array>
                                                (coordgroup,params) );
                }
                else
                {
                    //evaluate the energy on each of the gridpoints...
                    addToGrid(coordgroup, params);
                }
            }
        }
    }
    
    qDebug() << "There are" << closemols.count() << "close groups...";
}

/** Recalculate the total energy */
void GridFF::recalculateEnergy()
{        
    if (closemols.isEmpty() or gridpot.isEmpty())
        rebuildGrid();

    double cnrg(0);
    double ljnrg(0);
    double gridnrg(0);
    
    //loop through all of the molecules and calculate the energies.
    //First, calculate the CLJ energies for the closemols
    for (ChunkedVector<CLJMolecule>::const_iterator 
                    it = mols[0].moleculesByIndex().constBegin();
         it != mols[0].moleculesByIndex().constEnd();
         ++it)
    {
        double icnrg(0);
        double iljnrg(0);
        double igridnrg(0);
    
        const CLJMolecule &cljmol = *it;
        
        //loop through each CutGroup of this molecule
        const int ngroups = cljmol.coordinates().count();
        
        const CoordGroup *groups_array = cljmol.coordinates().constData();
        
        const CLJParameters::Array *params_array 
                                = cljmol.parameters().atomicParameters().constData();

        for (int igroup=0; igroup<ngroups; ++igroup)
        {
            const CoordGroup &coords0 = groups_array[igroup];
            const CLJParameters::Array &params0 = params_array[igroup];
            
            const int nats0 = coords0.count();
            
            //calculate the CLJ energy with each group in closemols
            for ( QVector< QPair<CoordGroup,CLJParameters::Array> >::const_iterator it2
                             = closemols.constBegin();
                  it2 != closemols.constEnd();
                  ++it2)
            {
                const CoordGroup &coords1 = it2->first;
                const CLJParameters::Array &params1 = it2->second;
                
                const int nats1 = coords1.count();
                
                for (int i0=0; i0<nats0; ++i0)
                {
                    const Vector &c0 = coords0[i0];
                    const detail::CLJParameter &p0 = params0[i0];
                    
                    for (int i1=0; i1<nats1; ++i1)
                    {
                        const Vector &c1 = coords1[i1];
                        const detail::CLJParameter &p1 = params1[i1];
                        
                        const LJPair &ljpair = ljpairs.constData()[
                                                    ljpairs.map(p0.ljid,p1.ljid) ];
                                                    
                        const double invdist = double(1) / Vector::distance(c0,c1);
                        
                        icnrg += p0.reduced_charge * p1.reduced_charge 
                                     * invdist;
                    
                        double sig_over_dist6 = pow_6(ljpair.sigma()*invdist);
                        double sig_over_dist12 = pow_2(sig_over_dist6);
    
                        iljnrg += ljpair.epsilon() * (sig_over_dist12 - 
                                                      sig_over_dist6);
                    }
                }
            }
            
            //now calculate the energy over the points on the grid...
            
        }
        
        cnrg += icnrg;
        ljnrg += iljnrg;
        gridnrg += igridnrg;
        
        //save the old energy of this molecule
    }

    if (calc_grid_error)
        InterGroupCLJFF::recalculateEnergy();
}
