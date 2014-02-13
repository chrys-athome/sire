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

#include "cljgrid.h"
#include "cljcalculator.h"
#include "cljshiftfunction.h"

#include "SireVol/cartesian.h"

#include "SireUnits/dimensions.h"
#include "SireUnits/units.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMM;
using namespace SireVol;
using namespace SireMaths;
using namespace SireUnits;
using namespace SireStream;

static const RegisterMetaType<CLJGrid> r_grid(NO_ROOT);

QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const CLJGrid &grid)
{
    writeHeader(ds, r_grid, 1);
    
    SharedDataStream sds(ds);
    
    sds << grid.grid_box << grid.grid_spacing << grid.grid_buffer
        << grid.cljfunc << grid.grid_pots << grid.cljboxes
        << grid.close_atoms
        << grid.dimx << grid.dimy << grid.dimz;
    
    return ds;
}

QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, CLJGrid &grid)
{
    VersionID v = readHeader(ds, r_grid);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> grid.grid_box >> grid.grid_spacing >> grid.grid_buffer
            >> grid.cljfunc >> grid.grid_pots >> grid.cljboxes
            >> grid.close_atoms
            >> grid.dimx >> grid.dimy >> grid.dimz;
        
        if (grid.dimx * grid.dimy * grid.dimz != grid.grid_pots.count())
        {
            qDebug() << "Discrepancy in grid dimensions!";
            qDebug() << grid.dimx << grid.dimy << grid.dimz << grid.grid_pots.count();
            
            AABox dim = grid.grid_box;
            grid.grid_box = AABox();
            grid.clearGrid();
            grid.setGridDimensions(dim);
        }
    }
    else
        throw version_error(v, "1", r_grid, CODELOC);
    
    return ds;
}

static boost::shared_ptr<CLJFunction> global_func( new CLJShiftFunction() );

static const Length global_grid_spacing = 1 * angstrom;
static const Length global_grid_buffer = 2 * angstrom;

/** Constructor */
CLJGrid::CLJGrid() : grid_spacing(global_grid_spacing),
                     grid_buffer(global_grid_buffer), cljfunc(global_func),
                     dimx(0), dimy(0), dimz(0)
{}

/** Construct, specifying the dimensions of the grid */
CLJGrid::CLJGrid(const AABox &grid_dimensions)
        : grid_spacing(global_grid_spacing),
          grid_buffer(global_grid_buffer), cljfunc(global_func),
          dimx(0), dimy(0), dimz(0)
{
    setGridDimensions(grid_dimensions);
}

/** Construct, specifying the dimensions and spacing for the grid */
CLJGrid::CLJGrid(const AABox &grid_dimensions, Length spacing)
        : grid_spacing(spacing),
          grid_buffer(global_grid_buffer), cljfunc(global_func),
          dimx(0), dimy(0), dimz(0)
{
    setGridDimensions(grid_dimensions);
}

/** Construct, specifying the function to use to calculate the energy and 
    the grid dimensions */
CLJGrid::CLJGrid(const CLJFunction &func, const AABox &grid_dimensions)
        : grid_spacing(global_grid_spacing),
          grid_buffer(global_grid_buffer), cljfunc( func.clone() ),
          dimx(0), dimy(0), dimz(0)
{
    setGridDimensions(grid_dimensions);
}

/** Construct, specifying the function to use to calculate the energy and
    the grid dimensions and grid spacing */
CLJGrid::CLJGrid(const CLJFunction &func, const AABox &grid_dimensions, Length spacing)
        : grid_spacing(spacing),
          grid_buffer(global_grid_buffer), cljfunc( func.clone() ),
          dimx(0), dimy(0), dimz(0)
{
    setGridDimensions(grid_dimensions);
}

/** Copy constructor */
CLJGrid::CLJGrid(const CLJGrid &other)
        : grid_box(other.grid_box), grid_spacing(other.grid_spacing),
          grid_buffer(other.grid_buffer), grid_pots(other.grid_pots),
          cljfunc(other.cljfunc), cljboxes(other.cljboxes), close_atoms(other.close_atoms),
          dimx(other.dimx), dimy(other.dimy), dimz(other.dimz)
{}

/** Destructor */
CLJGrid::~CLJGrid()
{}

/** Copy assignment operator */
CLJGrid& CLJGrid::operator=(const CLJGrid &other)
{
    if (this != &other)
    {
        grid_box = other.grid_box;
        grid_spacing = other.grid_spacing;
        grid_buffer = other.grid_buffer;
        grid_pots = other.grid_pots;
        cljfunc = other.cljfunc;
        cljboxes = other.cljboxes;
        close_atoms = other.close_atoms;
        dimx = other.dimx;
        dimy = other.dimy;
        dimz = other.dimz;
    }
    
    return *this;
}

/** Comparison operator */
bool CLJGrid::operator==(const CLJGrid &other) const
{
    return grid_box == other.grid_box and grid_spacing == other.grid_spacing and
           grid_buffer == other.grid_buffer and cljfunc->equals(*(other.cljfunc)) and
           cljboxes == other.cljboxes;
}

/** Comparison operator */
bool CLJGrid::operator!=(const CLJGrid &other) const
{
    return not operator==(other);
}

const char* CLJGrid::typeName()
{
    return QMetaType::typeName( qMetaTypeId<CLJGrid>() );
}

const char* CLJGrid::what() const
{
    return CLJGrid::typeName();
}

CLJGrid* CLJGrid::clone() const
{
    return new CLJGrid(*this);
}

/** Return the number of fixed atoms */
int CLJGrid::nFixedAtoms() const
{
    return cljboxes.nAtoms();
}

QString CLJGrid::toString() const
{
    return QObject::tr("CLJGrid( gridDimensions() == %1 (%5x%6x%7), gridSpacing() == %2, "
                       "cljFunction() == %3, nFixedAtoms() == %4 )")
                            .arg(gridDimensions().toString())
                            .arg(gridSpacing().toString())
                            .arg(cljFunction().toString())
                            .arg(nFixedAtoms())
                            .arg(dimx).arg(dimy).arg(dimz);
}

void CLJGrid::clearGrid()
{
    grid_pots.clear();
    close_atoms = CLJAtoms();
}

/** Add the passed atoms onto the set of fixed atoms */
void CLJGrid::addFixedAtoms(const CLJAtoms &atoms)
{
    cljboxes = cljboxes + CLJBoxes(atoms);
    clearGrid();
}

/** Set the fixed atoms equal to the passed atoms */
void CLJGrid::setFixedAtoms(const CLJAtoms &atoms)
{
    cljboxes = CLJBoxes(atoms);
    clearGrid();
}

/** Set the fixed atoms equal to the passed atoms */
void CLJGrid::setFixedAtoms(const CLJBoxes &atoms)
{
    cljboxes = atoms;
    clearGrid();
}

/** Return all of the fixed atoms */
CLJAtoms CLJGrid::fixedAtoms() const
{
    return cljboxes.atoms();
}

/** Set the function used to calculate the coulomb and LJ energy */
void CLJGrid::setCLJFunction(const CLJFunction &function)
{
    cljfunc.reset( function.clone() );
    clearGrid();
}

/** Return the function used to calculate the coulomb and LJ energy */
const CLJFunction& CLJGrid::cljFunction() const
{
    return *cljfunc;
}

/** Set the grid spacing */
void CLJGrid::setGridSpacing(Length spacing)
{
    if (grid_spacing != spacing.value())
    {
        grid_spacing = spacing.value();
        AABox dim = grid_box;
        grid_box = AABox();
        setGridDimensions(dim);
    }
}

/** Return the grid spacing */
Length CLJGrid::gridSpacing() const
{
    return Length(grid_spacing);
}

/** Set the grid buffer */
void CLJGrid::setGridBuffer(Length buffer)
{
    grid_buffer = buffer.value();
}

/** Return the grid buffer */
Length CLJGrid::gridBuffer() const
{
    return Length(grid_buffer);
}

/** Set the dimensions of the grid */
void CLJGrid::setGridDimensions(const AABox &dimensions)
{
    if (grid_box != dimensions)
    {
        Vector nboxes = (2.0 / grid_spacing) * dimensions.halfExtents();
        
        dimx = quint32( std::ceil(nboxes.x()) );
        dimy = quint32( std::ceil(nboxes.y()) );
        dimz = quint32( std::ceil(nboxes.z()) );
        
        Vector maxcoords = dimensions.minCoords() + Vector( dimx*grid_spacing,
                                                            dimy*grid_spacing,
                                                            dimz*grid_spacing );
        
        grid_box = AABox::from( dimensions.minCoords(), maxcoords );
        
        clearGrid();
    }
}

/** Set the dimensions of the grid so that it encompasses all of the atoms in 'atoms'
    (with a buffer of 'buffer' around all atoms) using a grid with spacing 'grid_spacing' */
void CLJGrid::setGridDimensions(const CLJAtoms &atoms, Length spacing, Length buffer)
{
    grid_spacing = spacing.value();
    grid_buffer = buffer.value();
    
    if (atoms.x().isEmpty())
    {
        //build an empty grid
        setGridDimensions( AABox::from( -Vector(buffer), Vector(buffer) ) );
    }
    else
    {
        MultiFloat min_x( std::numeric_limits<float>::max() );
        MultiFloat max_x( -std::numeric_limits<float>::max() );
        
        MultiFloat min_y( min_x );
        MultiFloat max_y( max_x );
        
        MultiFloat min_z( min_x );
        MultiFloat max_z( max_x );
        
        const int nats = atoms.x().count();
        
        const MultiFloat *x = atoms.x().constData();
        const MultiFloat *y = atoms.y().constData();
        const MultiFloat *z = atoms.z().constData();
        
        for (int i=0; i<nats; ++i)
        {
            min_x = min_x.min(x[i]);
            max_x = max_x.max(x[i]);

            min_y = min_y.min(y[i]);
            max_y = max_y.max(y[i]);

            min_z = min_z.min(z[i]);
            max_z = max_z.max(z[i]);
        }
        
        Vector mincoords( min_x.min(), min_y.min(), min_z.min() );
        Vector maxcoords( max_x.max(), max_y.max(), max_z.max() );
        
        mincoords -= Vector(grid_buffer);
        maxcoords += Vector(grid_buffer);
        
        setGridDimensions( AABox::from(mincoords,maxcoords) );
    }
}

/** Set the dimensions of the grid so that it encompasses all of the atoms in 'atoms'
    (plus a gridBuffer() buffer around all atoms) */
void CLJGrid::setGridDimensions(const CLJAtoms &atoms)
{
    setGridDimensions(atoms, Length(grid_spacing), Length(grid_buffer));
}

/** Set the dimensions of the grid so that it encompasses all of the atoms in 'atoms'
    (plus a gridBuffer() buffer around all atoms) using a grid with spacing 'grid_spacing' */
void CLJGrid::setGridDimensions(const CLJAtoms &atoms, Length spacing)
{
    setGridDimensions(atoms, spacing, Length(grid_buffer));
}

/** Return the dimensions of the grid */
AABox CLJGrid::gridDimensions() const
{
    return grid_box;
}

/** Convert the index 'ipt' for a 1-dimensional array into the indicies
    ix, iy, iz for the 3-dimensional array of dimension dimx, dimy, dimz */
static void arrayIndexToGridIndex(int ipt, int dimx, int dimy, int dimz,
                                  int &ix, int &iy, int &iz)
{
    ix = ipt / (dimy*dimz);
    ipt -= ix*dimy*dimz;

    iy = ipt / dimz;
    ipt -= iy*dimz;
    
    iz = ipt;
}

/** Convert the indicies ix, iy, iz for the 3-dimensional array of dimension
    dimx, dimy, dimz into the returned index into the corresponding 1-dimensional array */
static int gridIndexToArrayIndex(int ix, int iy, int iz, int dimx, int dimy, int dimz)
{
    return ix*(dimy*dimz) + iy*dimz + iz;
}

/** Function called to calculate the potential grid. Note that this is called by 
    a const function, so we must be thread-safe when we update the potential */
void CLJGrid::calculateGrid()
{
    //first, divide the atoms into two sets - those within the LJ cutoff of
    //the grid, and those outside
    CLJAtoms far_atoms;
    CLJAtoms near_atoms;
    {
        QVector<CLJAtom> near_atms;
        QVector<CLJAtom> far_atms;

        const Cartesian space;
        const float lj_cutoff = cljfunc->ljCutoff();
        const float coul_cutoff = cljfunc->coulombCutoff();

        for (QMap<CLJBoxIndex,CLJBoxPtr>::const_iterator it = cljboxes.occupiedBoxes().constBegin();
             it != cljboxes.occupiedBoxes().constEnd();
             ++it)
        {
            const CLJAtoms &atoms = it.value().read().atoms();
            
            for (int i=0; i<atoms.count(); ++i)
            {
                CLJAtom atom = atoms[i];
                
                if (atom.ID() != 0)
                {
                    double mindist = space.minimumDistance(atom.coordinates(), grid_box);
                    
                    if (mindist < lj_cutoff)
                    {
                        near_atms.append(atom);
                    }
                    else if (mindist < coul_cutoff and atom.charge().value() != 0)
                    {
                        far_atms.append(atom);
                    }
                }
            }
        }
        
        near_atoms = CLJAtoms(near_atms);
        far_atoms = CLJAtoms(far_atms);
    }
    
    //now, go through any far atoms and add their potentials to the grid
    QVector<float> pot = cljfunc->calculate(far_atoms, 
}

/** Calculate the total energy of interaction between the passed atoms and
    all of the atoms added to the grid */
void CLJGrid::total(const CLJAtoms &atms, double &cnrg, double &ljnrg) const
{
    cnrg = 0;
    ljnrg = 0;
    
    if (cljboxes.isEmpty() or atms.isEmpty())
        return;
    
    CLJCalculator cljcalc;
    CLJBoxes atoms(atms);
    
    if (not cljfunc->hasCutoff() or cljfunc->isPeriodic() or
        (cljfunc->coulombCutoff().value() < cljfunc->ljCutoff().value() + 5))
    {
        //do not use a grid
        tuple<double,double> nrgs = cljcalc.calculate(*cljfunc, atoms, cljboxes);
        
        cnrg = nrgs.get<0>();
        ljnrg = nrgs.get<1>();
    }
    else
    {
        //there is a big enough difference between the coulomb and LJ cutoffs that
        //a grid is worthwhile
        if (grid_pots.isEmpty())
        {
            //we haven't yet calculated the grid! We need to (just in time) calculate it.
            const_cast<CLJGrid*>(this)->calculateGrid();
        }

        //calculate the energy between the atoms and the close atoms
        tuple<double,double> nrgs = cljcalc.calculate(*cljfunc, atoms, close_atoms);
        
        //now calculate the grid energy of each atom
        const float *gridpot_array = grid_pots.constData();

        double grid_nrg = 0;

        const MultiFloat *x = atms.x().constData();
        const MultiFloat *y = atms.y().constData();
        const MultiFloat *z = atms.z().constData();
        const MultiFloat *q = atms.q().constData();
        
        const int nats = atms.x().count();

        const MultiFloat inv_grid_spacing( 1.0 / grid_spacing );
        const MultiFloat grid_ox( grid_box.minCoords().x() );
        const MultiFloat grid_oy( grid_box.minCoords().y() );
        const MultiFloat grid_oz( grid_box.minCoords().z() );

        MultiFloat gx, gy, gz;

        bool all_within_grid = true;

        for (int i=0; i<nats; ++i)
        {
            gx = grid_ox - x[i];
            gy = grid_oy - y[i];
            gz = grid_oz - z[i];
            
            gx *= inv_grid_spacing;
            gy *= inv_grid_spacing;
            gz *= inv_grid_spacing;
        
            for (int ii=0; ii<MultiFloat::count(); ++ii)
            {
                int i_0 = int(gx[ii]);
                int j_0 = int(gy[ii]);
                int k_0 = int(gz[ii]);
            
                if (i_0 < 0 or i_0 >= int(dimx-1) or
                    j_0 < 0 or j_0 >= int(dimy-1) or
                    k_0 < 0 or k_0 >= int(dimz-1))
                {
                    Vector p( x[i][ii], y[i][ii], z[i][ii] );
                
                    qDebug() << "POINT" << p.toString() << "LIES OUTSIDE OF "
                             << "THE GRID?" << grid_box.toString();
                    
                    all_within_grid = false;
                    break;
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
                    const Vector c000 = grid_box.minCoords() +
                                            Vector( i_0 * grid_spacing,
                                                    j_0 * grid_spacing,
                                                    k_0 * grid_spacing );

                    const double R = (x[i][ii] - c000.x()) / grid_spacing;
                    const double S = (y[i][ii] - c000.y()) / grid_spacing;
                    const double T = (z[i][ii] - c000.z()) / grid_spacing;
                    
                    int i000 = gridIndexToArrayIndex(i_0  , j_0  , k_0  , dimx, dimy, dimz);
                    int i001 = gridIndexToArrayIndex(i_0  , j_0  , k_0+1, dimx, dimy, dimz);
                    int i010 = gridIndexToArrayIndex(i_0  , j_0+1, k_0  , dimx, dimy, dimz);
                    int i100 = gridIndexToArrayIndex(i_0+1, j_0  , k_0  , dimx, dimy, dimz);
                    int i011 = gridIndexToArrayIndex(i_0  , j_0+1, k_0+1, dimx, dimy, dimz);
                    int i101 = gridIndexToArrayIndex(i_0+1, j_0  , k_0+1, dimx, dimy, dimz);
                    int i110 = gridIndexToArrayIndex(i_0+1, j_0+1, k_0  , dimx, dimy, dimz);
                    int i111 = gridIndexToArrayIndex(i_0+1, j_0+1, k_0+1, dimx, dimy, dimz);
                    
                    double phi = (gridpot_array[i000] * (1-R)*(1-S)*(1-T)) + 
                                 (gridpot_array[i001] * (1-R)*(1-S)*(  T)) +
                                 (gridpot_array[i010] * (1-R)*(  S)*(1-T)) +
                                 (gridpot_array[i100] * (  R)*(1-S)*(1-T)) +
                                 (gridpot_array[i011] * (1-R)*(  S)*(  T)) +
                                 (gridpot_array[i101] * (  R)*(1-S)*(  T)) +
                                 (gridpot_array[i110] * (  R)*(  S)*(1-T)) +
                                 (gridpot_array[i111] * (  R)*(  S)*(  T));                         
                                      
                    grid_nrg += phi * q[i][ii];
                }
            }
            
            if (not all_within_grid)
                break;
        }
        
        if (all_within_grid)
        {
            cnrg = nrgs.get<0>() + grid_nrg;
            ljnrg = nrgs.get<1>();
        }
        else
        {
            //some atoms were outside the grid. It is easier to just recalculate everything
            //from scratch then to try and recalculate the grid
            nrgs = cljcalc.calculate(*cljfunc, atoms, cljboxes);
            cnrg = nrgs.get<0>();
            ljnrg = nrgs.get<1>();
        }
    }
}

/** Return the coulomb and LJ energies of the passed atoms with the fixed
    atoms added to this grid */
boost::tuple<double,double> CLJGrid::calculate(const CLJAtoms &atoms) const
{
    double cnrg, ljnrg;
    this->total(atoms, cnrg, ljnrg);
    return boost::tuple<double,double>(cnrg,ljnrg);
}

/** Return the coulomb energy of the passed atoms interacting with 
    the fixed atoms on this grid */
double CLJGrid::coulomb(const CLJAtoms &atoms) const
{
    double cnrg, ljnrg;
    this->total(atoms, cnrg, ljnrg);
    return cnrg;
}

/** Return the LJ energy of the passed atoms interacting with
    the fixed atoms on this grid */
double CLJGrid::lj(const CLJAtoms &atoms) const
{
    double cnrg, ljnrg;
    this->total(atoms, cnrg, ljnrg);
    return ljnrg;
}
