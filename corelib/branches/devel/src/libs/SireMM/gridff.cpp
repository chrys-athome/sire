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
         buffer_size(2.5), grid_spacing(1.0), 
         coul_cutoff(50), lj_cutoff(7.5),
         calc_grid_error(false)
{}

/** Construct a grid forcefield with a specified name */
GridFF::GridFF(const QString &name) 
       : ConcreteProperty<GridFF,InterGroupCLJFF>(name),
         buffer_size(2.5), grid_spacing(1.0), 
         coul_cutoff(50), lj_cutoff(7.5),
         calc_grid_error(false)
{}

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
        closemols_coords = other.closemols_coords;
        closemols_params = other.closemols_params;
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
    
    Vector maxpoint = gridbox.minCoords() + Vector( dimx * grid_spacing,
                                                    dimy * grid_spacing,
                                                    dimz * grid_spacing );

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
    
    qDebug() 
      << "Building the list of close molecules and adding far molecules to the grid...";

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
                        //evaluate the energy on each of the gridpoints...
                        addToGrid(it->get<1>(), params);
                    }
                }
            }
            else
            {
                if (spce.beyond(lj_cutoff, coordgroup, allcoords0))
                {
                    //evaluate the energy on each of the gridpoints...
                    addToGrid(coordgroup, params);
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
                    addToGrid(coordgroup, params);
                }
            }
        }
    }
    
    closemols_coords.squeeze();
    closemols_params.squeeze();
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

/** Recalculate the total energy */
void GridFF::recalculateEnergy()
{        
    if (gridpot.isEmpty())
        rebuildGrid();

    QTime t;
    t.start();

    double cnrg(0);
    double ljnrg(0);
    
    //loop through all of the molecules and calculate the energies.
    //First, calculate the CLJ energies for the closemols
    for (ChunkedVector<CLJMolecule>::const_iterator 
                    it = mols[0].moleculesByIndex().constBegin();
         it != mols[0].moleculesByIndex().constEnd();
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
            double icnrg, iljnrg;
            calculateEnergy(groups_array[igroup], params_array[igroup],
                            icnrg, iljnrg);
                            
            cnrg += icnrg;
            ljnrg += iljnrg;

            //save the energy of this group
        }
    }

    int ms_grid = t.elapsed();

    qDebug() << cnrg << ljnrg;

    InterGroupCLJFF::mustNowRecalculateFromScratch();
    
    t.start();
    InterGroupCLJFF::recalculateEnergy();
    int ms_exact = t.elapsed();
    
    double coul = this->energy( components().coulomb() );
    double lj = this->energy( components().lj() );
    
    qDebug() << "COULOMB" << cnrg << coul << (cnrg - coul) << ms_grid;
    qDebug() << "LJ     " << ljnrg << lj << (ljnrg - lj) << ms_exact;

    if (calc_grid_error)
        InterGroupCLJFF::recalculateEnergy();
}
