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

#include "SireSystem/energymonitor.h"
#include "SireSystem/system.h"

#include "SireMol/partialmolecule.h"
#include "SireMol/atomcoords.h"
#include "SireMol/atomcharges.h"
#include "SireMM/atomljs.h"
#include "SireMM/ljpair.h"

#include "SireBase/array2d.hpp"

#include "SireUnits/units.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireSystem;
using namespace SireMol;
using namespace SireMaths;
using namespace SireMM;
using namespace SireUnits;
using namespace SireUnits::Dimension;
using namespace SireBase;
using namespace SireStream;

using std::pair;

static const RegisterMetaType<EnergyMonitor> r_nrgmonitor;

QDataStream SIRESYSTEM_EXPORT &operator<<(QDataStream &ds, 
                                          const EnergyMonitor &nrgmonitor)
{
    writeHeader(ds, r_nrgmonitor, 1);
    
    SharedDataStream sds(ds);
    
    sds << nrgmonitor.grp0 << nrgmonitor.grp1 << nrgmonitor.accum
        << nrgmonitor.coul_nrgs << nrgmonitor.lj_nrgs
        << static_cast<const SystemMonitor&>(nrgmonitor);
        
    return ds;
}

QDataStream SIRESYSTEM_EXPORT &operator>>(QDataStream &ds,
                                          EnergyMonitor &nrgmonitor)
{
    VersionID v = readHeader(ds, r_nrgmonitor);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> nrgmonitor.grp0 >> nrgmonitor.grp1 >> nrgmonitor.accum
            >> nrgmonitor.coul_nrgs >> nrgmonitor.lj_nrgs
            >> static_cast<SystemMonitor&>(nrgmonitor);
    }
    else
        throw version_error(v, "1", r_nrgmonitor, CODELOC);
        
    return ds;
}

/** Null constructor */
EnergyMonitor::EnergyMonitor() : ConcreteProperty<EnergyMonitor,SystemMonitor>()
{}

/** Construct to monitor the energies between all pairs of molecule views in the
    two passed groups. This will accumulate the average and standard deviation
    of each of the energies */
EnergyMonitor::EnergyMonitor(const MoleculeGroup &group0, 
                             const MoleculeGroup &group1)
              : ConcreteProperty<EnergyMonitor,SystemMonitor>(),
                grp0(group0), grp1(group1), accum( AverageAndStddev() )
{}
            
/** Construct to monitor the energies between all pairs of molecule views in 
    the two passed groups, accumulating the energies using the passed
    accumulator */
EnergyMonitor::EnergyMonitor(const MoleculeGroup &group0, 
                             const MoleculeGroup &group1,
                             const SireMaths::Accumulator &accumulator)
              : ConcreteProperty<EnergyMonitor,SystemMonitor>(),
                grp0(group0), grp1(group1), accum(accumulator)
{}

/** Copy constructor */
EnergyMonitor::EnergyMonitor(const EnergyMonitor &other)
              : ConcreteProperty<EnergyMonitor,SystemMonitor>(other),
                grp0(other.grp0), grp1(other.grp1), accum(other.accum),
                coul_nrgs(other.coul_nrgs), lj_nrgs(other.lj_nrgs) 
{}

/** Destructor */
EnergyMonitor::~EnergyMonitor()
{}

/** Copy assignment operator */
EnergyMonitor& EnergyMonitor::operator=(const EnergyMonitor &other)
{
    if (this != &other)
    {
        grp0 = other.grp0;
        grp1 = other.grp1;
        accum = other.accum;
        coul_nrgs = other.coul_nrgs;
        lj_nrgs = other.lj_nrgs;
        SystemMonitor::operator=(other);
    }
    
    return *this;
}

/** Comparison operator */
bool EnergyMonitor::operator==(const EnergyMonitor &other) const
{
    return this == &other or
           (grp0 == other.grp0 and grp1 == other.grp1 and
            accum == other.accum and 
            coul_nrgs == other.coul_nrgs and
            lj_nrgs == other.lj_nrgs and
            SystemMonitor::operator==(other));
}

/** Comparison operator */
bool EnergyMonitor::operator!=(const EnergyMonitor &other) const
{
    return not EnergyMonitor::operator==(other);
}

/** Return the typename of the class */
const char* EnergyMonitor::typeName()
{
    return QMetaType::typeName( qMetaTypeId<EnergyMonitor>() );
}

/** Return the array of all accumulated coulomb energies */
Array2D<AccumulatorPtr> EnergyMonitor::coulombEnergies() const
{
    return coul_nrgs;
}

/** Return the array of all accumulated LJ energies */
Array2D<AccumulatorPtr> EnergyMonitor::ljEnergies() const
{
    return lj_nrgs;
}

/** Return the molecule group containing the first group of views */
const MoleculeGroup& EnergyMonitor::group0() const
{
    return grp0.read();
}

/** Return the molecule group containing the second group of views */
const MoleculeGroup& EnergyMonitor::group1() const
{
    return grp1.read();
}

/** Return the array of the first group of molecule views in the same order as they
    appear in the arrays of energies */
QVector<SireMol::PartialMolecule> EnergyMonitor::views0() const
{
    int n = grp0->nViews();
    
    QVector<PartialMolecule> views(n);
    
    for (int i=0; i<n; ++i)
    {
        views[i] = grp0->viewAt(i);
    }
    
    return views;
}

/** Return the array of the second group of molecule views in the same order as they
    appear in the arrays of energies */
QVector<SireMol::PartialMolecule> EnergyMonitor::views1() const
{
    int n = grp1->nViews();
    
    QVector<PartialMolecule> views(n);
    
    for (int i=0; i<n; ++i)
    {
        views[i] = grp1->viewAt(i);
    }
    
    return views;
}

/** Clear all statistics */
void EnergyMonitor::clearStatistics()
{
    coul_nrgs = Array2D<AccumulatorPtr>();
    lj_nrgs = Array2D<AccumulatorPtr>();
}

inline pair<double,double> getCLJEnergy(
                const QVector<Vector> &coords0, const QVector<Charge> &chgs0,
                const QVector<LJParameter> &ljs0,
                const QVector<Vector> &coords1, const QVector<Charge> &chgs1,
                const QVector<LJParameter> &ljs1)
{
    const int nats0 = coords0.count();
    const int nats1 = coords1.count();
    
    double cnrg = 0;
    double ljnrg = 0;
    
    bool arithmetic_combining_rules = true;
    
    for (int i=0; i<nats0; ++i)
    {
        const Vector &coord0 = coords0.at(i);
        const Charge &chg0 = chgs0.at(i);
        const LJParameter &lj0 = ljs0.at(i);
        
        for (int j=0; j<nats1; ++j)
        {
            const Vector &coord1 = coords1.at(j);
            const Charge &chg1 = chgs1.at(j);
            const LJParameter &lj1 = ljs1.at(j);
            
            LJPair ljpair;

            if (arithmetic_combining_rules)
                ljpair = LJPair::arithmetic(lj0, lj1);
            else
                ljpair = LJPair::geometric(lj0, lj1);
            
            double one_over_r2 = Vector::invDistance2(coord0, coord1);
            double one_over_r = std::sqrt(one_over_r2);
            double one_over_r6 = one_over_r2 * one_over_r2 * one_over_r2;
            double one_over_r12 = one_over_r6 * one_over_r6;
            
            ljnrg += ljpair.A() * one_over_r12 - ljpair.B() * one_over_r6;
            
            cnrg += chg0.value() * chg1.value() * one_over_r 
                        * one_over_four_pi_eps0;
        }
    }
    
    return pair<double,double>(cnrg,ljnrg);
}

/** Accumulate energies from the passed system */
void EnergyMonitor::monitor(System &system)
{
    EnergyMonitor old_state(*this);

    try
    {
        if (system.contains(grp0->number()))
            grp0 = system[grp0->number()];
            
        if (system.contains(grp1->number()))
            grp1 = system[grp1->number()];
            
        // extract the charge, LJ and coordinates of all of the views
        const PropertyName charge_prop("charge");
        const PropertyName lj_prop("LJ");
        const PropertyName coords_prop("coordinates");
        
        const int n0 = grp0->nViews();
        const int n1 = grp1->nViews();

        //has the number of views changed?
        if (n0 != coul_nrgs.nRows() or n1 != coul_nrgs.nColumns())
        {
            Array2D<AccumulatorPtr> coul_nrgs2(n0,n1,accum);
            Array2D<AccumulatorPtr> lj_nrgs2(n0,n1,accum);
            
            for (int i=0; i < qMin(n0,coul_nrgs.nRows()); ++i)
            {
                for (int j=0; j < qMin(n1,coul_nrgs.nColumns()); ++j)
                {
                    coul_nrgs2(i,j) = coul_nrgs(i,j);
                    lj_nrgs2(i,j) = lj_nrgs(i,j);
                }
            }
            
            coul_nrgs = coul_nrgs2;
            lj_nrgs = lj_nrgs2;
        }

        QVector< QVector<Vector> > grp0_coords(n0);
        QVector< QVector<Charge> > grp0_chgs(n0);
        QVector< QVector<LJParameter> > grp0_ljs(n0);
        
        QVector< QVector<Vector> > grp1_coords(n1);
        QVector< QVector<Charge> > grp1_chgs(n1);
        QVector< QVector<LJParameter> > grp1_ljs(n1);

        for (int i=0; i<n0; ++i)
        {
            const PartialMolecule &mol0 = grp0->viewAt(i);
            
            if (mol0.selectedAll())
            {
                grp0_coords[i] = mol0.property(coords_prop)
                                     .asA<AtomCoords>()
                                     .toVector();
            
                grp0_chgs[i] = mol0.property(charge_prop)
                                   .asA<AtomCharges>()
                                   .toVector();
                                                  
                grp0_ljs[i] = mol0.property(lj_prop)
                                  .asA<AtomLJs>()
                                  .toVector();
            }
            else
            {
                const AtomSelection selected_atoms = mol0.selection();

                grp0_coords[i] = mol0.property(coords_prop)
                                     .asA<AtomCoords>()
                                     .toVector(selected_atoms);
            
                grp0_chgs[i] = mol0.property(charge_prop)
                                   .asA<AtomCharges>()
                                   .toVector(selected_atoms);
                                                  
                grp0_ljs[i] = mol0.property(lj_prop)
                                  .asA<AtomLJs>()
                                  .toVector(selected_atoms);
            }
        }

        for (int i=0; i<n1; ++i)
        {
            const PartialMolecule &mol1 = grp1->viewAt(i);
            
            if (mol1.selectedAll())
            {
                grp1_coords[i] = mol1.property(coords_prop)
                                     .asA<AtomCoords>()
                                     .toVector();
            
                grp1_chgs[i] = mol1.property(charge_prop)
                                   .asA<AtomCharges>()
                                   .toVector();
                                                  
                grp1_ljs[i] = mol1.property(lj_prop)
                                  .asA<AtomLJs>()
                                  .toVector();
            }
            else
            {
                const AtomSelection selected_atoms = mol1.selection();

                grp1_coords[i] = mol1.property(coords_prop)
                                     .asA<AtomCoords>()
                                     .toVector(selected_atoms);
            
                grp1_chgs[i] = mol1.property(charge_prop)
                                   .asA<AtomCharges>()
                                   .toVector(selected_atoms);
                                                  
                grp1_ljs[i] = mol1.property(lj_prop)
                                  .asA<AtomLJs>()
                                  .toVector(selected_atoms);
            }
        }
    
        //calculate the energy of each view-pair
        for (int i=0; i<n0; ++i)
        {
            const QVector<Vector> &mol0_coords = grp0_coords.at(i);
            const QVector<Charge> &mol0_chgs = grp0_chgs.at(i);
            const QVector<LJParameter> &mol0_ljs = grp0_ljs.at(i);
            
            for (int j=0; j<n1; ++j)
            {
                const QVector<Vector> mol1_coords = grp1_coords.at(j);
                const QVector<Charge> mol1_chgs = grp1_chgs.at(j);
                const QVector<LJParameter> mol1_ljs = grp1_ljs.at(j);
                
                pair<double,double> cljnrg 
                        = getCLJEnergy(mol0_coords, mol0_chgs, mol0_ljs,
                                       mol1_coords, mol1_chgs, mol1_ljs);
            
                coul_nrgs(i,j).edit().accumulate(cljnrg.first);
                lj_nrgs(i,j).edit().accumulate(cljnrg.second);
            }
        }
    }
    catch(...)
    {
        EnergyMonitor::operator=(old_state);
        throw;
    }
}
