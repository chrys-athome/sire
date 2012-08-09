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
#include <QTime>

#ifdef SIRE_USE_SSE
    #ifdef __SSE__
        #include <emmintrin.h>   // CONDITIONAL_INCLUDE
    #else
        #undef SIRE_USE_SSE
    #endif
#endif

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
        << gridff.total_coul_errors << gridff.delta_coul_errors
        << gridff.total_lj_errors << gridff.delta_lj_errors
        << gridff.total_time_saved << gridff.delta_time_saved
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
            >> gridff.total_coul_errors >> gridff.delta_coul_errors
            >> gridff.total_lj_errors >> gridff.delta_lj_errors
            >> gridff.total_time_saved >> gridff.delta_time_saved
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
         buffer_size(2.5), grid_spacing(1.0), 
         coul_cutoff(50), lj_cutoff(7.5),
         calc_grid_error(false)
{
    this->setSwitchingFunction(NoCutoff());
}

/** Construct a grid forcefield with a specified name */
GridFF::GridFF(const QString &name) 
       : ConcreteProperty<GridFF,InterGroupCLJFF>(name),
         buffer_size(2.5), grid_spacing(1.0), 
         coul_cutoff(50), lj_cutoff(7.5),
         total_time_saved(0), delta_time_saved(0),
         calc_grid_error(false)
{
    this->setSwitchingFunction(NoCutoff());
}

/** Copy constructor */
GridFF::GridFF(const GridFF &other)
       : ConcreteProperty<GridFF,InterGroupCLJFF>(other),
         gridbox(other.gridbox),
         buffer_size(other.buffer_size), grid_spacing(other.grid_spacing),
         coul_cutoff(other.coul_cutoff), lj_cutoff(other.lj_cutoff),
         dimx(other.dimx), dimy(other.dimy), dimz(other.dimz),
         gridpot(other.gridpot), 
         closemols_coords(other.closemols_coords),
         closemols_params(other.closemols_params),
         oldnrgs(other.oldnrgs),
         total_coul_errors(other.total_coul_errors),
         total_lj_errors(other.total_lj_errors),
         delta_coul_errors(other.delta_coul_errors),
         delta_lj_errors(other.delta_lj_errors),
         total_time_saved(other.total_time_saved),
         delta_time_saved(other.delta_time_saved),
         calc_grid_error(other.calc_grid_error)
{
    this->setSwitchingFunction(NoCutoff());
}

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
        closemols_coords = other.closemols_coords;
        closemols_params = other.closemols_params;
        oldnrgs = other.oldnrgs;
        total_coul_errors = other.total_coul_errors;
        delta_coul_errors = other.delta_coul_errors;
        total_lj_errors = other.total_lj_errors;
        delta_lj_errors = other.delta_lj_errors;
        total_time_saved = other.total_time_saved;
        delta_time_saved = other.delta_time_saved;
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
        gridpot.clear();
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

inline GridFF::Vector4::Vector4(const Vector &v, double chg)
              : x(v.x()), y(v.y()), z(v.z()), q(chg)
{}

void GridFF::appendTo(QVector<GridFF::Vector4> &coords_and_charges,
                      const Vector *coords, const detail::CLJParameter *params,
                      int nats)
{
    if (nats > 0)
    {
        coords_and_charges.reserve( coords_and_charges.count() + nats );
        
        for (int i=0; i<nats; ++i)
        {
            double q = params[i].reduced_charge;
            
            if (q != 0)
            {
                coords_and_charges.append( Vector4(coords[i],q) );
            }
        }
    }
}

inline QString toString(const __m128d &sseval)
{
    return QString("{ %1, %2 }").arg(*((const double*)&sseval))
                                .arg(*( ((const double*)&sseval) + 1));
}

void GridFF::addToGrid(const QVector<GridFF::Vector4> &coords_and_charges)
{
    //QVector<double> gridpot2(gridpot);

    double *pot = gridpot.data();
    const Vector4 *array = coords_and_charges.constData();

    Vector minpoint = gridbox.minCoords();

    //loop over all grid points
    const int npts = dimx*dimy*dimz;
    const int nats = coords_and_charges.count();
    
    QTime t;
    t.start();
    
    #ifdef SIRE_USE_SSE
    {
        const int remainder = npts % 2;
        
        int i0=0;
        int j0=0;
        int k0=0;
        
        int i1=0;
        int j1=0;
        int k1=1;
        
        double gx0 = minpoint.x();
        double gy0 = minpoint.y();
        double gz0 = minpoint.z();
        
        double gx1 = minpoint.x();
        double gy1 = minpoint.y();
        double gz1 = minpoint.z()+grid_spacing;
        
        for (int ipt=0; ipt<(npts-1); ipt+=2)
        {
            //set the sse values - note that _mm_set_pd is backwards,
            //so the lower value is gx0, not gx1
            const __m128d sse_gx = _mm_set_pd(gx1,gx0);
            const __m128d sse_gy = _mm_set_pd(gy1,gy0);
            const __m128d sse_gz = _mm_set_pd(gz1,gz0);
            
            __m128d sse_half_total[2];
            __m128d sse_half_dist[2];
            double half_q[2];
            
            half_q[0] = 0;
            half_q[1] = 0;
            sse_half_dist[0] = _mm_set_pd(1,1);
            sse_half_dist[1] = _mm_set_pd(1,1);
            sse_half_total[0] = _mm_set_pd(0,0);
            sse_half_total[1] = _mm_set_pd(0,0);
            
            //loop through each atom
            for (int iat=0; iat<nats; ++iat)
            {
                const bool a_idx = (iat % 2);
                const bool b_idx = not a_idx;

                const Vector4 &c = array[iat];

                const __m128d sse_dx = _mm_sub_pd(_mm_set1_pd(c.x), sse_gx);
                const __m128d sse_dy = _mm_sub_pd(_mm_set1_pd(c.y), sse_gy);
                const __m128d sse_dz = _mm_sub_pd(_mm_set1_pd(c.z), sse_gz);

                const __m128d sse_dist = _mm_sqrt_pd(
                                    _mm_add_pd( _mm_mul_pd(sse_dx,sse_dx),
                                      _mm_add_pd( _mm_mul_pd(sse_dy,sse_dy),
                                                  _mm_mul_pd(sse_dz,sse_dz) ) ) );

                
                sse_half_dist[a_idx] = sse_dist;
                half_q[a_idx] = c.q;
                
                sse_half_total[b_idx] = _mm_add_pd(sse_half_total[b_idx],
                                          _mm_div_pd(_mm_set1_pd(half_q[b_idx]),
                                                     sse_half_dist[b_idx]));
            }

            const bool a_idx = (nats-1) % 2;
            sse_half_total[a_idx] = _mm_add_pd(sse_half_total[a_idx],
                                                _mm_div_pd(_mm_set1_pd(half_q[a_idx]),
                                                           sse_half_dist[a_idx]));

            __m128d sse_total = _mm_add_pd(sse_half_total[0],sse_half_total[1]);

            //qDebug() << ::toString(sse_total) << ::toString(sse_total2)
            //         << ::toString(sse_even_total)
            //         << ::toString(sse_odd_total);

            pot[ipt] += *((const double*)&sse_total);            
            pot[ipt+1] += *( ((const double*)&sse_total) + 1 );

            for (int ii=0; ii<2; ++ii)
            {
                //advance the indicies (twice, as two grid points per iteration)
                k0 += 1;
                gz0 += grid_spacing;
                
                if (k0 == dimz)
                {
                    k0 = 0;
                    gz0 = minpoint.z();
                    
                    j0 += 1;
                    gy0 += grid_spacing;
                    
                    if (j0 == dimy)
                    {
                        j0 = 0;
                        gy0 = minpoint.y();
                        
                        i0 += 1;
                        gx0 += grid_spacing;
                        
                        if (i0 == dimx)
                        {
                            i0 = 0;
                            gx0 = minpoint.x();
                        }
                    }
                }

                k1 += 1;
                gz1 += grid_spacing;
                
                if (k1 == dimz)
                {
                    k1 = 0;
                    gz1 = minpoint.z();
                    
                    j1 += 1;
                    gy1 += grid_spacing;
                    
                    if (j1 == dimy)
                    {
                        j1 = 0;
                        gy1 = minpoint.y();
                        
                        i1 += 1;
                        gx1 += grid_spacing;
                        
                        if (i1 == dimx)
                        {
                            i1 = 0;
                            gx1 = minpoint.x();
                        }
                    }
                }
            }
        }
        
        if (remainder)
        {
            //we need to process the last grid point
            const double gx = minpoint.x() + (dimx-1)*grid_spacing;
            const double gy = minpoint.y() + (dimy-1)*grid_spacing;
            const double gz = minpoint.z() + (dimz-1)*grid_spacing;
            
            double total = 0;
            
            //loop through each atom...
            for (int iat=0; iat<nats; ++iat)
            {
                const Vector4 &c = array[iat];

                const double dx = c.x - gx;
                const double dy = c.y - gy;
                const double dz = c.z - gz;

                total += c.q / std::sqrt(dx*dx + dy*dy + dz*dz);
            }
            
            pot[npts-1] += total;
        }
    }
    #else
    {
        int i=0;
        int j=0;
        int k=0;
        
        double gx = minpoint.x();
        double gy = minpoint.y();
        double gz = minpoint.z();

        for (int ipt=0; ipt<npts; ++ipt)
        {
            double total = 0;
            
            //loop through each atom...
            for (int iat=0; iat<nats; ++iat)
            {
                const Vector4 &c = array[iat];

                const double dx = c.x - gx;
                const double dy = c.y - gy;
                const double dz = c.z - gz;

                total += c.q / std::sqrt(dx*dx + dy*dy + dz*dz);
            }
                    
            pot[ipt] += total;
            
            k += 1;
            gz += grid_spacing;
            
            if (k == dimz)
            {
                k = 0;
                gz = minpoint.z();
                
                j += 1;
                gy += grid_spacing;
                
                if (j == dimy)
                {
                    j = 0;
                    gy = minpoint.y();
                    
                    i += 1;
                    gx += grid_spacing;
                    
                    if (i == dimx)
                    {
                        i = 0;
                        gx = minpoint.x();
                    }
                }
            }
        }
    }
    #endif

    if (false)
    {
        QVector<double> gridpot2(gridpot);
        double *pot = gridpot2.data();
    
        int i=0;
        int j=0;
        int k=0;
        
        double gx = minpoint.x();
        double gy = minpoint.y();
        double gz = minpoint.z();

        for (int ipt=0; ipt<npts; ++ipt)
        {
            double total = 0;
            
            //loop through each atom...
            for (int iat=0; iat<nats; ++iat)
            {
                const Vector4 &c = array[iat];

                const double dx = c.x - gx;
                const double dy = c.y - gy;
                const double dz = c.z - gz;

                total += c.q / std::sqrt(dx*dx + dy*dy + dz*dz);
            }
                    
            pot[ipt] += total;
            
            k += 1;
            gz += grid_spacing;
            
            if (k == dimz)
            {
                k = 0;
                gz = minpoint.z();
                
                j += 1;
                gy += grid_spacing;
                
                if (j == dimy)
                {
                    j = 0;
                    gy = minpoint.y();
                    
                    i += 1;
                    gx += grid_spacing;
                    
                    if (i == dimx)
                    {
                        i = 0;
                        gx = minpoint.x();
                    }
                }
            }
        }
        
        for (int i=0; i<npts; ++i)
        {
            if ( std::abs(gridpot[i]-gridpot2[i]) > 1e-5 )
            {
                qDebug() << "BROKEN" << i << npts
                         << gridpot[i] << gridpot2[i]
                         << (gridpot[i]-gridpot2[i]);
            }
        }
    }
    
    int ms = t.elapsed();
    
    qDebug() << "Added" << nats << "more atoms to" << npts << "grid points in"
             << ms << "ms";
}

/** Internal function used to rebuild the coulomb potential grid */
void GridFF::rebuildGrid()
{
    QTime t;

    qDebug() << "REBUILDING THE GRID FOR FORCEFIELD" << this->name().value();

    //Get a CoordGroup that contains all of the molecules in group 0
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
    
    Vector maxpoint = gridbox.minCoords() + Vector( (dimx-1) * grid_spacing,
                                                    (dimy-1) * grid_spacing,
                                                    (dimz-1) * grid_spacing );

    gridbox = AABox::from(gridbox.minCoords(), maxpoint);
    
    qDebug() << "Adjusted grid extends over" << gridbox.toString();
    
    //create space for the grid
    gridpot = QVector<double>(dimx*dimy*dimz, 0.0);
    gridpot.squeeze();

    closemols_coords.clear();
    closemols_params.clear();
    
    //now build the grid - we take coordgroups that are within the LJ cutoff
    //and calculate those manually (saving them in the closemols list), and
    //only add coulomb potentials of CoordGroups that are beyond the LJ cutoff
    const ChunkedVector<CLJMolecule> &cljmols = mols[1].moleculesByIndex();
    
    const Space &spce = this->space();
    Vector grid_center = group0_box.center();
    qDebug() << "Grid space equals:" << spce.toString();
    
    qDebug() << "Rebuilding the LJ parameter database...";
    CLJPotential::startEvaluation();
    CLJPotential::finishedEvaluation();
    
    qDebug() 
      << "Building the list of close molecules and adding far molecules to the grid...";

    QVector<Vector4> far_mols;
    int molcount = 0;
    int atomcount = 0;
    t.start();

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
                        closemols_coords += it->get<1>().toVector();
                        closemols_params += params.toQVector();
                    }
                    else
                    {
                        appendTo(far_mols, it->get<1>().constData(),
                                           params.constData(), coordgroup.count());
                    }
                }
            }
            else
            {
                if (spce.beyond(lj_cutoff, coordgroup, allcoords0))
                {
                    //evaluate the energy on each of the gridpoints...
                    appendTo(far_mols, coordgroup.constData(),
                             params.constData(), coordgroup.count());
                }
                else if (spce.minimumDistance(coordgroup, allcoords0) < lj_cutoff)
                {
                    //this image should be evaluated explicitly
                    closemols_coords += coordgroup.toVector();
                    closemols_params += params.toQVector();
                }
                else
                {
                    //evaluate the energy on each of the gridpoints...
                    appendTo(far_mols, coordgroup.constData(),
                             params.constData(), coordgroup.count());
                }
            }
        }
        
        molcount += 1;
        
        if (far_mols.count() > 4096)
        {
            atomcount += far_mols.count();
            addToGrid(far_mols);
            far_mols.clear();
            qDebug() << "Added" << molcount << "of" << cljmols.count()
                     << "molecules to the grid...";
        }
    }
    
    if (not far_mols.isEmpty())
    {
        atomcount += far_mols.count();
        addToGrid(far_mols);
        qDebug() << "Added" << molcount << "of" << cljmols.count()
                 << "molecules to the grid...";
    }
    
    qDebug() << "Adding" << molcount << "molecules (" << atomcount
             << "atoms) to the grid took" << t.elapsed() << "ms in total";
    
    closemols_coords.squeeze();
    closemols_params.squeeze();

    qDebug() << "Explicitly evaluating coulomb and LJ energy over the"
             << closemols_coords.count() << "closest atom(s)";
}

void GridFF::calculateEnergy(const CoordGroup &coords0, 
                             const GridFF::CLJParameters::Array &params0,
                             double &cnrg, double &ljnrg)
{
    double icnrg = 0;
    double iljnrg = 0;

    const int nats0 = coords0.count();
    
    const Vector *coords0_array = coords0.constData();
    const detail::CLJParameter *params0_array = params0.constData();

    const Vector *coords1_array = closemols_coords.constData();
    const detail::CLJParameter *params1_array = closemols_params.constData();

    const int nats1 = closemols_coords.count();
        
    BOOST_ASSERT( closemols_coords.count() == closemols_params.count() );

    if (nats1 > 0)
    {
        #ifdef SIRE_USE_SSE
        {
            const int remainder = nats1 % 2;
            
            __m128d sse_cnrg = { 0, 0 };
            __m128d sse_ljnrg = { 0, 0 };

            const __m128d sse_one = { 1.0, 1.0 };
            
            for (quint32 i=0; i<nats0; ++i)
            {
                const Parameter &param0 = params0_array[i];
                
                __m128d sse_chg0 = _mm_set_pd( param0.reduced_charge, 
                                               param0.reduced_charge );
                
                const Vector &c0 = coords0_array[i];
                
                //process atoms in pairs (so can then use SSE)
                for (quint32 j=0; j<nats1-1; j += 2)
                {
                    const Parameter &param10 = params1_array[j];
                    const Parameter &param11 = params1_array[j+1];
                    
                    const Vector &c10 = coords1_array[j];
                    const Vector &c11 = coords1_array[j+1];
                    
                    __m128d sse_dist = _mm_set_pd( Vector::distance(c0,c10), 
                                                   Vector::distance(c0,c11) );
                    __m128d sse_chg1 = _mm_set_pd( param10.reduced_charge,
                                                   param11.reduced_charge );
                                       
                    const LJPair &ljpair0 = ljpairs.constData()[
                                            ljpairs.map(param0.ljid,
                                                        param10.ljid)];
                
                    const LJPair &ljpair1 = ljpairs.constData()[
                                            ljpairs.map(param0.ljid,
                                                        param11.ljid)];

                    __m128d sse_sig = _mm_set_pd( ljpair0.sigma(), ljpair1.sigma() );
                    __m128d sse_eps = _mm_set_pd( ljpair0.epsilon(), 
                                                  ljpair1.epsilon() );
                    
                    sse_dist = _mm_div_pd(sse_one, sse_dist);
                    
                    //calculate the coulomb energy
                    __m128d tmp = _mm_mul_pd(sse_chg0, sse_chg1);
                    tmp = _mm_mul_pd(tmp, sse_dist);
                    sse_cnrg = _mm_add_pd(sse_cnrg, tmp);
                    
                    //calculate (sigma/r)^6 and (sigma/r)^12
                    __m128d sse_sig_over_dist2 = _mm_mul_pd(sse_sig, sse_dist);
                    sse_sig_over_dist2 = _mm_mul_pd( sse_sig_over_dist2,  
                                                     sse_sig_over_dist2 );
                                                 
                    __m128d sse_sig_over_dist6 = _mm_mul_pd(sse_sig_over_dist2,
                                                            sse_sig_over_dist2);
                                                    
                    sse_sig_over_dist6 = _mm_mul_pd(sse_sig_over_dist6,
                                                    sse_sig_over_dist2);
                                                 
                    __m128d sse_sig_over_dist12 = _mm_mul_pd(sse_sig_over_dist6,
                                                             sse_sig_over_dist6);
                                          
                    //calculate LJ energy (the factor of 4 is added later)
                    tmp = _mm_sub_pd(sse_sig_over_dist12, 
                                     sse_sig_over_dist6);
                                             
                    tmp = _mm_mul_pd(tmp, sse_eps);
                    sse_ljnrg = _mm_add_pd(sse_ljnrg, tmp);
                }
                      
                if (remainder == 1)
                {
                    const Vector &c1 = coords1_array[nats1-1];
                    const Parameter &param1 = params1_array[nats1-1];

                    const double invdist = double(1) / Vector::distance(c0,c1);
                    
                    icnrg += param0.reduced_charge * param1.reduced_charge 
                                * invdist;

                    const LJPair &ljpair = ljpairs.constData()[
                                            ljpairs.map(param0.ljid,
                                                        param1.ljid)];
                    
                    double sig_over_dist6 = pow_6(ljpair.sigma()*invdist);
                    double sig_over_dist12 = pow_2(sig_over_dist6);

                    iljnrg += ljpair.epsilon() * (sig_over_dist12 - 
                                                  sig_over_dist6);
                                                      
                }
            }
            
            icnrg += *((const double*)&sse_cnrg) +
                     *( ((const double*)&sse_cnrg) + 1 );
                     
            iljnrg += *((const double*)&sse_ljnrg) +
                      *( ((const double*)&sse_ljnrg) + 1 );
        }
        #else
        {
            for (quint32 i=0; i<nats0; ++i)
            {
                distmat.setOuterIndex(i);
                const Parameter &param0 = params0_array[i];
                const Vector &c0 = coords0_array[i];
                
                for (quint32 j=0; j<nats1; ++j)
                {
                    const Parameter &param1 = params1_array[j];
                    const Vector &c1 = coords1_array[j];

                    const double invdist = double(1) / Vector::distance(c0,c1);
                    
                    icnrg += param0.reduced_charge * param1.reduced_charge 
                                * invdist;

                    const LJPair &ljpair = ljpairs.constData()[
                                            ljpairs.map(param0.ljid,
                                                        param1.ljid)];

                    double sig_over_dist6 = pow_6(ljpair.sigma()*invdist);
                    double sig_over_dist12 = pow_2(sig_over_dist6);

                    iljnrg += ljpair.epsilon() * (sig_over_dist12 - 
                                                  sig_over_dist6);
                }
            }
        }
        #endif
    }

    double gridnrg = 0;
    const double *gridpot_array = gridpot.constData();

    //now calculate the energy in the grid
    for (int i0=0; i0<nats0; ++i0)
    {
        const Vector &c0 = coords0[i0];
        const detail::CLJParameter &p0 = params0[i0];
        
        Vector grid_coords = c0 - gridbox.minCoords();
        
        int i_0 = int(grid_coords.x() / grid_spacing);
        int j_0 = int(grid_coords.y() / grid_spacing);
        int k_0 = int(grid_coords.z() / grid_spacing);
        
        if (i_0 < 0 or i_0 >= (dimx-1) or
            j_0 < 0 or j_0 >= (dimy-1) or
            k_0 < 0 or k_0 >= (dimz-1))
        {
            qDebug() << "POINT" << c0.toString() << "LIES OUTSIDE OF "
                     << "THE GRID?" << gridbox.toString();
        }
        else
        {
            //use tri-linear interpolation to get the potential at the atom
            //
            // This is described in 
            //
            // Davis, Madura and McCammon, Comp. Phys. Comm., 62, 187-197, 1991
            //
            // phi(x,y,z) = phi(i  ,j  ,k  )*(1-R)(1-S)(1-T) +
            //              phi(i+1,j  ,k  )*(  R)(1-S)(1-T) +
            //              phi(i  ,j+1,k  )*(1-R)(  S)(1-T) +
            //              phi(i  ,j  ,k+1)*(1-R)(1-S)(  T) +
            //              phi(i+1,j+1,k  )*(  R)(  S)(1-T) +
            //              phi(i+1,j  ,k+1)*(  R)(1-S)(  T) +
            //              phi(i  ,j+1,k+1)*(1-R)(  S)(  T) +
            //              phi(i+1,j+1,k+1)*(  R)(  S)(  T) +
            //
            // where R, S and T are the coordinates of the atom in 
            // fractional grid coordinates from the point (i,j,k), e.g.
            // (0,0,0) is (i,j,k) and (1,1,1) is (i+1,j+1,k+1)
            //
            const Vector c000 = gridbox.minCoords() + 
                                    Vector( i_0 * grid_spacing,
                                            j_0 * grid_spacing,
                                            k_0 * grid_spacing );

            const Vector RST = (c0 - c000) / grid_spacing;
            const double R = RST.x();
            const double S = RST.y();
            const double T = RST.z();
            
            int i000 = (i_0  ) * (dimy*dimz) + (j_0  )*dimz + (k_0  );
            int i001 = (i_0  ) * (dimy*dimz) + (j_0  )*dimz + (k_0+1);
            int i010 = (i_0  ) * (dimy*dimz) + (j_0+1)*dimz + (k_0  );
            int i100 = (i_0+1) * (dimy*dimz) + (j_0  )*dimz + (k_0  );
            int i011 = (i_0  ) * (dimy*dimz) + (j_0+1)*dimz + (k_0+1);
            int i101 = (i_0+1) * (dimy*dimz) + (j_0  )*dimz + (k_0+1);
            int i110 = (i_0+1) * (dimy*dimz) + (j_0+1)*dimz + (k_0  );
            int i111 = (i_0+1) * (dimy*dimz) + (j_0+1)*dimz + (k_0+1);
            
            double phi = (gridpot_array[i000] * (1-R)*(1-S)*(1-T)) + 
                         (gridpot_array[i001] * (1-R)*(1-S)*(  T)) +
                         (gridpot_array[i010] * (1-R)*(  S)*(1-T)) +
                         (gridpot_array[i100] * (  R)*(1-S)*(1-T)) +
                         (gridpot_array[i011] * (1-R)*(  S)*(  T)) +
                         (gridpot_array[i101] * (  R)*(1-S)*(  T)) +
                         (gridpot_array[i110] * (  R)*(  S)*(1-T)) +
                         (gridpot_array[i111] * (  R)*(  S)*(  T));                         
                              
            gridnrg += phi * p0.reduced_charge;
        }
    }
    
    cnrg = icnrg + gridnrg;
    ljnrg = 4.0*iljnrg;  // 4 epsilon (....)
}

/** Ensure that the next energy evaluation is from scratch */
void GridFF::mustNowRecalculateFromScratch()
{
    gridpot.clear();
    closemols_coords.clear();
    closemols_params.clear();
    oldnrgs.clear();
    
    InterGroupCLJFF::mustNowRecalculateFromScratch();
}

/** Any additions mean that the forcefield must be recalculated from scratch */
void GridFF::_pvt_added(quint32 groupid, const PartialMolecule &mol, 
                        const PropertyMap &map)
{
    this->mustNowRecalculateFromScratch();
    InterGroupCLJFF::_pvt_added(groupid, mol, map);
}

/** Any removals mean that the forcefield must be recalculated from scratch */
void GridFF::_pvt_removed(quint32 groupid, const PartialMolecule &mol)
{
    this->mustNowRecalculateFromScratch();
    InterGroupCLJFF::_pvt_removed(groupid, mol);
}

/** Any changes to group 1 mean that the forcefield must be recalculated from scratch */
void GridFF::_pvt_changed(quint32 groupid, const SireMol::Molecule &molecule)
{
    InterGroupCLJFF::_pvt_changed(groupid, molecule);
}

/** Any changes to group 1 mean that the forcefield must be recalculated from scratch */
void GridFF::_pvt_changed(quint32 groupid, const QList<SireMol::Molecule> &molecules)
{
    InterGroupCLJFF::_pvt_changed(groupid, molecules);
}

/** Any removals mean that the forcefield must be recalculated from scratch */
void GridFF::_pvt_removedAll(quint32 groupid)
{
    this->mustNowRecalculateFromScratch();
    InterGroupCLJFF::_pvt_removedAll(groupid);
}

/** Recalculate the total energy */
void GridFF::recalculateEnergy()
{
    boost::shared_ptr<InterGroupCLJFF> test_gridff;
    boost::shared_ptr<QTime> t;

    if (calc_grid_error)
    {
        test_gridff.reset( new InterGroupCLJFF(*this) );
        t.reset( new QTime() );
        t->start();
    }
    
    bool must_recalculate = false;

    //we need to recalculate if either something has changed in group 1,
    //or nothing at all has changed (in which case we assume that changes
    //are not being recorded), or if the grid hasn't been created,
    //or if molecules have been added or removed to group 0.
    if (gridpot.isEmpty())
    {
        //the grid is empty
        qDebug() << "GRID RECALCULATED AS IT DOES NOT EXIST";
        must_recalculate = true;
    }
    else if (not changed_mols[1].isEmpty())
    {
        //check to see if the parts in this forcefield have changed
        for (QHash<MolNum,ChangedMolecule>::const_iterator
                                      it = this->changed_mols[1].constBegin();
             it != this->changed_mols[1].constEnd();
             ++it)
        {
            if (not (it->newParts().isEmpty() and it->oldParts().isEmpty()))
            {
                //a part of this molecule in the forcefield has changed
                qDebug() << "MOLECULE" << it.key().toString()
                         << "FROM GROUP 2 HAS CHANGED. RECALCULATING THE GRID!!!";
                         
                must_recalculate = true;
                break;
            }
        }
        
        if (not must_recalculate)
        {
            changed_mols[1].clear();
        
            if (changed_mols[0].isEmpty())
            {
                //there is nothing to do :-)
                changed_mols[1].clear();
                this->setClean();
                return;
            }
            
            //otherwise a part of the molecule in group 0 has changed,
            //so evaluate the change
        }
    }
    else if (changed_mols[0].isEmpty())
    {
        //probably not recording changes - assume everything has changed
        qDebug() << "RECALCULATING AS NOTHING APPEARS TO HAVE CHANGED"
                 << "BUT WE HAVE BEEN MARKED AS DIRTY";
        must_recalculate = true;
    }
    
    if (must_recalculate)
    {
        this->mustNowRecalculateFromScratch();
        this->rebuildGrid();

        double total_cnrg(0);
        double total_ljnrg(0);

        //calculate all of the energies from scratch and store
        //them in 'oldnrgs'
        oldnrgs.reserve( mols[0].count() );
    
        //loop through all of the molecules and calculate the energies.
        //First, calculate the CLJ energies for the closemols
        for (ChunkedVector<CLJMolecule>::const_iterator 
                    it = mols[0].moleculesByIndex().constBegin();
             it != mols[0].moleculesByIndex().constEnd();
             ++it)
        {
            const CLJMolecule &cljmol = *it;

            double cnrg(0);
            double ljnrg(0);
        
            //loop through each CutGroup of this molecule
            const int ngroups = cljmol.coordinates().count();
        
            const CoordGroup *groups_array = cljmol.coordinates().constData();
        
            const CLJParameters::Array *params_array 
                                = cljmol.parameters().atomicParameters().constData();

            for (int igroup=0; igroup<ngroups; ++igroup)
            {
                const CoordGroup &group = groups_array[igroup];

                if (not gridbox.contains(group.aaBox()))
                {
                    //this group lies outside the grid - we need to recalculate
                    //the grid
                    qDebug() << "MOLECULE" << cljmol.number().toString() << "HAS MOVED"
                             << "OUTSIDE THE GRID. MUST RECALCULATE!";
                    this->mustNowRecalculateFromScratch();
                    this->recalculateEnergy();
                    return;
                }
                        
                double icnrg, iljnrg;
                calculateEnergy(group, params_array[igroup],
                                icnrg, iljnrg);
                                      
                cnrg += icnrg;
                ljnrg += iljnrg;

            }
            
            oldnrgs.insert(cljmol.number(), CLJEnergy(cnrg,ljnrg));
            
            total_cnrg += cnrg;
            total_ljnrg += ljnrg;
        }

        this->components().setEnergy(*this, CLJEnergy(total_cnrg,total_ljnrg));
    }
    else
    {
        double delta_cnrg(0);
        double delta_ljnrg(0);
    
        for (QHash<MolNum,ChangedMolecule>::const_iterator
                                      it = this->changed_mols[0].constBegin();
             it != this->changed_mols[0].constEnd();
             ++it)
        {
            const CLJMolecule &cljmol = it->newMolecule();
            
            //loop through each CutGroup of this molecule
            const int ngroups = cljmol.coordinates().count();
        
            const CoordGroup *groups_array = cljmol.coordinates().constData();
        
            const CLJParameters::Array *params_array 
                                = cljmol.parameters().atomicParameters().constData();

            double cnrg(0);
            double ljnrg(0);
        
            for (int igroup=0; igroup<ngroups; ++igroup)
            {
                const CoordGroup &group = groups_array[igroup];

                if (not gridbox.contains(group.aaBox()))
                {
                    //this group lies outside the grid - we need to recalculate
                    //the grid
                    qDebug() << "MOLECULE" << cljmol.number().toString() << "HAS MOVED"
                             << "OUTSIDE THE GRID. MUST RECALCULATE!";
                    this->mustNowRecalculateFromScratch();
                    this->recalculateEnergy();
                    return;
                }

                double icnrg, iljnrg;
                calculateEnergy(group, params_array[igroup],
                                icnrg, iljnrg);
                            
                cnrg += icnrg;
                ljnrg += iljnrg;

            }
            
            CLJEnergy old_nrg = oldnrgs[cljmol.number()];
            oldnrgs[cljmol.number()] = CLJEnergy(cnrg,ljnrg);
            
            delta_cnrg += (cnrg - old_nrg.coulomb());
            delta_ljnrg += (ljnrg - old_nrg.lj());
        }
         
        //change the energy
        this->components().changeEnergy(*this, CLJEnergy(delta_cnrg,delta_ljnrg));
        
        //clear the changed molecules
        this->changed_mols[0].clear();
    }
    
    this->setClean();

    if (calc_grid_error)
    {
        int grid_ms = t->elapsed();

        t->start();
        double nrg = test_gridff->energy();
        int ms = t->elapsed();
        
        double dcoul = this->energy(components().coulomb()) -
                       test_gridff->energy(components().coulomb());
                       
        double dlj = this->energy(components().lj()) -
                     test_gridff->energy(components().lj());
                     
        if (must_recalculate)
        {
            if (total_coul_errors.count() == 0)
            {
                //need to construct the histograms
                total_coul_errors = Histogram(-10*dcoul, 10*dcoul, int(200));
                total_lj_errors = Histogram(-10*dlj, 10*dlj, int(200));
            }

            total_coul_errors.accumulate(dcoul);
            total_lj_errors.accumulate(dlj);
            total_time_saved += (ms - grid_ms);
        }
        else
        {
            if (delta_coul_errors.count() == 0)
            {
                //need to construct the histograms
                delta_coul_errors = Histogram(-10*dcoul, 10*dcoul, int(200));
                delta_lj_errors = Histogram(-10*dlj, 10*dlj, int(200));
            }
            
            delta_coul_errors.accumulate(dcoul);
            delta_lj_errors.accumulate(dlj);
            delta_time_saved += (ms - grid_ms);
        }
    }
}

/** Return the histogram of errors in the calculation of total coulomb energies */
const Histogram& GridFF::totalCoulombErrorHistogram() const
{
    return total_coul_errors;
}

/** Return the histogram of errors in the calculation of delta coulomb energies */
const Histogram& GridFF::deltaCoulombErrorHistogram() const
{
    return delta_coul_errors;
}

/** Return the histogram of errors in the calculation of total LJ energies */
const Histogram& GridFF::totalLJErrorHistogram() const
{
    return total_lj_errors;
}

/** Return the histogram of errors in the calculation of delta LJ energies */
const Histogram& GridFF::deltaLJErrorHistogram() const
{
    return delta_lj_errors;
}

/** Return the total time saved using a grid as opposed to the explicit
    calculation of the total energy */
double GridFF::totalTotalTimeSaved() const
{
    return total_time_saved;
}

/** Return the total time saved using a grid as opposed to the explicit 
    calculate of the delta energies */
double GridFF::totalDeltaTimeSaved() const
{
    return delta_time_saved;
}

/** Return the average time saved using a grid as opposed to the explicit
    calculation of the total energy */
double GridFF::averageTotalTimeSaved() const
{
    double sum = total_coul_errors.sumOverBins();
    
    if (sum > 0)
        return total_time_saved / sum;
    else
        return 0;
}

/** Return the average time saved using a grid as opposed to the explicit 
    calculate of the delta energies */
double GridFF::averageDeltaTimeSaved() const
{
    double sum = delta_coul_errors.sumOverBins();
    
    if (sum > 0)
        return delta_time_saved / sum;
    else
        return 0;
}
