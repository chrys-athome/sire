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

#ifndef SIREMM_GRIDFF_H
#define SIREMM_GRIDFF_H

#include "intercljff.h"

namespace SireMM
{
class GridFF;
}

QDataStream& operator<<(QDataStream&, const SireMM::GridFF&);
QDataStream& operator>>(QDataStream&, SireMM::GridFF&);

namespace SireMM
{

/** This class calculates the coulomb and LJ energy between
    all molecules in group 1 and all molecules in group 2.
    The calculation is optimised, as the molecules in group 2
    are represented using a grid. This is ideal for situations
    where the molecules on group 2 move little, or not at all.
    
    @author Christopher Woods
*/
class SIREMM_EXPORT GridFF 
            : public SireBase::ConcreteProperty<GridFF,InterGroupCLJFF>
{

friend QDataStream& ::operator<<(QDataStream&, const GridFF&);
friend QDataStream& ::operator>>(QDataStream&, GridFF&);

public:
    GridFF();
    GridFF(const QString &name);
    
    GridFF(const GridFF &other);
    
    ~GridFF();
    
    static const char* typeName();
    
    const char* what() const;
    
    GridFF& operator=(const GridFF &other);
    
    bool operator==(const GridFF &other) const;
    bool operator!=(const GridFF &other) const;
    
    GridFF* clone() const;

    void setBuffer(SireUnits::Dimension::Length buffer);
    void setGridSpacing(SireUnits::Dimension::Length spacing);

    void setCoulombCutoff(SireUnits::Dimension::Length cutoff);
    void setLJCutoff(SireUnits::Dimension::Length cutoff);

    void setCalculateGridError(bool on);
    
    SireUnits::Dimension::Length buffer() const;
    SireUnits::Dimension::Length spacing() const;

    SireUnits::Dimension::Length coulombCutoff() const;
    SireUnits::Dimension::Length ljCutoff() const;

    bool calculatingGridError() const;

protected:
    void recalculateEnergy();

private:
    void rebuildGrid();

    typedef InterGroupCLJFF::Parameters CLJParameters;
    typedef InterGroupCLJFF::Molecule CLJMolecule;
    typedef InterGroupCLJFF::Molecules CLJMolecules;

    void calculateEnergy(const SireVol::CoordGroup &coords,
                         const CLJParameters::Array &params,
                         double &cnrg, double &ljnrg);

    void addToGrid(const SireVol::CoordGroup &cgroup,
                   const CLJParameters::Array &parameters);

    /** The AABox that describes the grid */
    SireVol::AABox gridbox;

    /** The distance from the atoms and the edge of the grid.
        This prevents the grid from being re-evaluated whenever
        the atoms move */
    double buffer_size;

    /** The grid spacing */
    double grid_spacing;

    /** The coulomb cutoff (cutoff applies from the
        center of the grid) */
    double coul_cutoff;

    /** The LJ cutoff (cutoff applies from the
        center of the grid) */
    double lj_cutoff;

    /** The number of grid points in x, y, and z */
    quint32 dimx, dimy, dimz;

    /** The grid of coulomb potentials */
    QVector<double> gridpot;
    
    /** The set of coordinates and parameters of group 2 
        molecules that are within the LJ cutoff of the center of the grid */
    QVector<SireMaths::Vector> closemols_coords;
    QVector<detail::CLJParameter> closemols_params;
    
    /** The energies of the group 1 CoordGroups the last time
        they were evaluated */
    QVector<CLJEnergy> oldnrgs;
    
    /** Whether or not to calculate the grid error */
    bool calc_grid_error;
};

}

Q_DECLARE_METATYPE( SireMM::GridFF )

SIRE_EXPOSE_CLASS( SireMM::GridFF )

#endif
