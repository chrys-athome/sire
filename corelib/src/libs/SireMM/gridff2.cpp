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

#include "gridff2.h"
#include "cljpotential.h"

#include "SireMol/atomcoords.h"
#include "SireMol/atomcharges.h"
#include "atomljs.h"

#include "SireMaths/constants.h"
#include "SireMaths/multifloat.h"
#include "SireMaths/multidouble.h"

#include "SireUnits/units.h"

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

static const RegisterMetaType<GridFF2> r_gridff2;

QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const GridFF2 &gridff2)
{
    writeHeader(ds, r_gridff2, 1);
    
    SharedDataStream sds(ds);
    
    sds << gridff2.gridbox << gridff2.dimx << gridff2.dimy << gridff2.dimz
        << gridff2.gridpot
        << gridff2.buffer_size << gridff2.grid_spacing
        << gridff2.coul_cutoff << gridff2.lj_cutoff
        << gridff2.fixedatoms_coords
        << gridff2.fixedatoms_params
        << gridff2.closemols_coords
        << gridff2.closemols_params;
    
    sds << quint32( gridff2.oldnrgs.count() );
    
    for (QHash<MolNum,CLJEnergy>::const_iterator it = gridff2.oldnrgs.constBegin();
         it != gridff2.oldnrgs.constEnd();
         ++it)
    {
        sds << it.key() << it.value().coulomb() << it.value().lj();
    }
    
    //collect together the used LJParameters and write those to the stream.
    //This will let us know if we need to update the LJIDs...
    QHash<quint32,LJParameter> used_ljs;
    
    LJParameterDB::lock();
    for (int i=0; i<gridff2.fixedatoms_params.count(); ++i)
    {
        const SireMM::detail::CLJParameter &param = gridff2.fixedatoms_params.constData()[i];
        used_ljs.insert(param.ljid, LJParameterDB::_locked_getLJParameter(param.ljid));
    }
    for (int i=0; i<gridff2.closemols_params.count(); ++i)
    {
        const SireMM::detail::CLJParameter &param = gridff2.closemols_params.constData()[i];
        used_ljs.insert(param.ljid, LJParameterDB::_locked_getLJParameter(param.ljid));
    }
    LJParameterDB::unlock();

    sds << used_ljs;
    
    sds << static_cast<const InterGroupCLJFF&>(gridff2);
    
    return ds;
}

QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, GridFF2 &gridff2)
{
    VersionID v = readHeader(ds, r_gridff2);

    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        gridff2 = GridFF2();
        
        gridff2.fixedatoms_coords.clear();
        gridff2.fixedatoms_params.clear();
        
        QVector<SireMM::detail::CLJParameter> fixedatoms_params;
        QVector<SireMM::detail::CLJParameter> closemols_params;
        QHash<quint32,LJParameter> used_ljs;
        
        sds >> gridff2.gridbox >> gridff2.dimx >> gridff2.dimy >> gridff2.dimz
            >> gridff2.gridpot
            >> gridff2.buffer_size >> gridff2.grid_spacing
            >> gridff2.coul_cutoff >> gridff2.lj_cutoff
            >> gridff2.fixedatoms_coords
            >> fixedatoms_params
            >> gridff2.closemols_coords
            >> closemols_params;
        
        quint32 noldnrgs;
        sds >> noldnrgs;
        
        gridff2.oldnrgs.clear();
        gridff2.oldnrgs.reserve(noldnrgs);
        
        for (quint32 i=0; i<noldnrgs; ++i)
        {
            MolNum molnum;
            double cnrg, ljnrg;
            
            sds >> molnum >> cnrg >> ljnrg;
            gridff2.oldnrgs.insert(molnum,CLJEnergy(cnrg,ljnrg));
        }
        
        sds >> used_ljs;

        QHash<quint32,quint32> ljidmap;

        LJParameterDB::lock();
        for (QHash<quint32,LJParameter>::const_iterator it = used_ljs.constBegin();
             it != used_ljs.constEnd();
             ++it)
        {
            quint32 newid = LJParameterDB::_locked_addLJParameter(it.value());
            
            if (it.key() != newid)
                ljidmap.insert(it.key(), newid);
        }
        LJParameterDB::unlock();
        
        if (not ljidmap.isEmpty())
        {
            //some of the LJIDs have changed
            for (int i=0; i<fixedatoms_params.count(); ++i)
            {
                SireMM::detail::CLJParameter &param = fixedatoms_params[i];
                param.ljid = ljidmap.value(param.ljid, param.ljid);
            }
            for (int i=0; i<closemols_params.count(); ++i)
            {
                SireMM::detail::CLJParameter &param = closemols_params[i];
                param.ljid = ljidmap.value(param.ljid, param.ljid);
            }
        }

        gridff2.fixedatoms_params = fixedatoms_params;
        gridff2.closemols_params = closemols_params;
        
        sds >> static_cast<InterGroupCLJFF&>(gridff2);

        gridff2.need_update_ljpairs = true;
    }
    else
        throw version_error(v, "1", r_gridff2, CODELOC);
        
    return ds;
}

/** Empty constructor */
GridFF2::GridFF2() 
       : ConcreteProperty<GridFF2,InterGroupCLJFF>(),
         buffer_size(2.5), grid_spacing(1.0), 
         coul_cutoff(50), lj_cutoff(7.5)
{
    this->setSwitchingFunction(NoCutoff());
}

/** Construct a grid forcefield with a specified name */
GridFF2::GridFF2(const QString &name) 
       : ConcreteProperty<GridFF2,InterGroupCLJFF>(name),
         buffer_size(2.5), grid_spacing(1.0), 
         coul_cutoff(50), lj_cutoff(7.5)
{
    this->setSwitchingFunction(NoCutoff());
}

/** Copy constructor */
GridFF2::GridFF2(const GridFF2 &other)
       : ConcreteProperty<GridFF2,InterGroupCLJFF>(other),
         gridbox(other.gridbox),
         buffer_size(other.buffer_size), grid_spacing(other.grid_spacing),
         coul_cutoff(other.coul_cutoff), lj_cutoff(other.lj_cutoff),
         dimx(other.dimx), dimy(other.dimy), dimz(other.dimz),
         gridpot(other.gridpot),
         fixedatoms_coords(other.fixedatoms_coords),
         fixedatoms_params(other.fixedatoms_params),
         closemols_coords(other.closemols_coords),
         closemols_params(other.closemols_params),
         oldnrgs(other.oldnrgs)
{
    this->setSwitchingFunction(NoCutoff());
}

/** Destructor */
GridFF2::~GridFF2()
{}

const char* GridFF2::typeName()
{
    return QMetaType::typeName( qMetaTypeId<GridFF2>() );
}

const char* GridFF2::what() const
{
    return GridFF2::typeName();
}

/** Copy assignment operator */
GridFF2& GridFF2::operator=(const GridFF2 &other)
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
        fixedatoms_coords = other.fixedatoms_coords;
        fixedatoms_params = other.fixedatoms_params;
        closemols_coords = other.closemols_coords;
        closemols_params = other.closemols_params;
        oldnrgs = other.oldnrgs;
    
        InterGroupCLJFF::operator=(other);
    }
    
    return *this;
}

/** Comparison operator */
bool GridFF2::operator==(const GridFF2 &other) const
{
    return buffer_size == other.buffer_size and
           grid_spacing == other.grid_spacing and
           InterGroupCLJFF::operator==(other);
}

/** Comparison operator */
bool GridFF2::operator!=(const GridFF2 &other) const
{
    return not GridFF2::operator==(other);
}

GridFF2* GridFF2::clone() const
{
    return new GridFF2(*this);
}

/** Add fixed atoms to the grid. This will copy the fixed atoms from 
    the passed GridFF2. This allows multiple grid forcefields to share the
    memory cost of a shared set of fixed atoms */
void GridFF2::addFixedAtoms(const GridFF2 &other)
{
    if (other.fixedatoms_coords.isEmpty())
        return;
    
    if (fixedatoms_coords.isEmpty())
    {
        fixedatoms_coords = other.fixedatoms_coords;
        fixedatoms_params = other.fixedatoms_params;
    }
    else
    {
        fixedatoms_coords += other.fixedatoms_coords;
        fixedatoms_params += other.fixedatoms_params;
    }

    need_update_ljpairs = true;

    this->mustNowRecalculateFromScratch();
}

/** Add fixed atoms to the grid. These are atoms that will never change
    position or charge during the simulation, and that you wish to be
    included in the energy expression. The atoms can be placed here, and
    then do not need to be added to the simulation System. This is useful
    if you are simulating a small cutout of the system and do not want to 
    have all of the atoms loaded into the system during the simulation */
void GridFF2::addFixedAtoms(const MoleculeView &fixed_atoms, const PropertyMap &map)
{
    const PropertyName coords_property = map["coordinates"];
    const PropertyName chg_property = map["charge"];
    const PropertyName lj_property = map["LJ"];
    
    const QVector<Vector> coords = fixed_atoms.molecule().property(coords_property)
                                      .asA<AtomCoords>().toVector(fixed_atoms.selection());

    const QVector<SireUnits::Dimension::Charge> charges =
                            fixed_atoms.molecule().property(chg_property)
                                      .asA<AtomCharges>().toVector(fixed_atoms.selection());

    const QVector<LJParameter> ljs = fixed_atoms.molecule().property(lj_property)
                                      .asA<AtomLJs>().toVector(fixed_atoms.selection());
    
    int nats = coords.count();
    
    fixedatoms_coords.reserve( fixedatoms_coords.count() + nats );
    fixedatoms_params.reserve( fixedatoms_params.count() + nats );
    
    fixedatoms_coords += coords;
    
    LJParameterDB::lock();
    
    const double sqrt_one_over_4pieps0 = std::sqrt(SireUnits::one_over_four_pi_eps0);
    
    for (int i=0; i<nats; ++i)
    {
        SireMM::detail::CLJParameter cljparam;
        
        cljparam.reduced_charge = charges[i] * sqrt_one_over_4pieps0;
        cljparam.ljid = LJParameterDB::_locked_addLJParameter(ljs[i]);
        
        fixedatoms_params.append(cljparam);
    }
    
    LJParameterDB::unlock();
    
    fixedatoms_coords.squeeze();
    fixedatoms_params.squeeze();

    need_update_ljpairs = true;

    this->mustNowRecalculateFromScratch();
}

/** Add fixed atoms to the grid. These are atoms that will never change
    position or charge during the simulation, and that you wish to be
    included in the energy expression. The atoms can be placed here, and
    then do not need to be added to the simulation System. This is useful
    if you are simulating a small cutout of the system and do not want to 
    have all of the atoms loaded into the system during the simulation */
void GridFF2::addFixedAtoms(const SireMol::Molecules &fixed_atoms, const PropertyMap &map)
{
    const PropertyName coords_property = map["coordinates"];
    const PropertyName chg_property = map["charge"];
    const PropertyName lj_property = map["LJ"];

    const double sqrt_one_over_4pieps0 = std::sqrt(SireUnits::one_over_four_pi_eps0);
    
    LJParameterDB::lock();
    
    for (SireMol::Molecules::const_iterator it = fixed_atoms.constBegin();
         it != fixed_atoms.constEnd();
         ++it)
    {
        SireMol::Molecule mol = it->molecule();
        AtomSelection selection = it->selection();
        
        const QVector<Vector> coords = mol.property(coords_property)
                                        .asA<AtomCoords>().toVector(selection);
        
        const QVector<SireUnits::Dimension::Charge> charges = mol.property(chg_property)
                                        .asA<AtomCharges>().toVector(selection);
        
        const QVector<LJParameter> ljs = mol.property(lj_property)
                                            .asA<AtomLJs>().toVector(selection);

        int nats = coords.count();
        
        fixedatoms_coords.reserve( fixedatoms_coords.count() + nats );
        fixedatoms_params.reserve( fixedatoms_params.count() + nats );
        
        fixedatoms_coords += coords;
        
        for (int i=0; i<nats; ++i)
        {
            SireMM::detail::CLJParameter cljparam;
            
            cljparam.reduced_charge = charges[i] * sqrt_one_over_4pieps0;
            cljparam.ljid = LJParameterDB::_locked_addLJParameter(ljs[i]);

            fixedatoms_params.append(cljparam);
        }
    }
    
    LJParameterDB::unlock();

    need_update_ljpairs = true;
    
    fixedatoms_coords.squeeze();
    fixedatoms_params.squeeze();

    this->mustNowRecalculateFromScratch();
}

/** Add all of the atoms in the molecules in the passed molecule group to the set
    of fixed atoms */
void GridFF2::addFixedAtoms(const MoleculeGroup &group, const PropertyMap &map)
{
    this->addFixedAtoms(group.molecules(), map);
}

/** Set the buffer when building the grid. This adds a buffer space
    around the grid when it is built, to try to reduce the number of
    times it needs to be rebuilt */
void GridFF2::setBuffer(SireUnits::Dimension::Length buffer)
{
    buffer_size = buffer.value();
}

/** Set the grid spacing (the distance between grid points). The
    smaller the spacing the more memory is required to hold the grid,
    but the more accurate the energy */
void GridFF2::setGridSpacing(SireUnits::Dimension::Length spacing)
{
    if (grid_spacing != spacing.value())
    {
        grid_spacing = spacing.value();
        this->mustNowRecalculateFromScratch();
    }
}

/** Set the cutoff for the coulomb energy - this can be greater
    than the box size as multiple periodic images can be used */
void GridFF2::setCoulombCutoff(SireUnits::Dimension::Length cutoff)
{
    if (coul_cutoff != cutoff.value())
    {
        coul_cutoff = cutoff.value();
        this->mustNowRecalculateFromScratch();
    }
}

/** Set the cutoff for the LJ energy - this can be greater than
    the box size as multiple periodic images can be used */
void GridFF2::setLJCutoff(SireUnits::Dimension::Length cutoff)
{
    if (lj_cutoff != cutoff.value())
    {
        lj_cutoff = cutoff.value();
        this->mustNowRecalculateFromScratch();
    }
}

/** Turn on or off use of the force shifted potential */
bool GridFF2::setShiftElectrostatics(bool on)
{
    if (InterGroupCLJFF::setShiftElectrostatics(on))
    {
        this->mustNowRecalculateFromScratch();
        return true;
    }
    else
        return false;
}

/** Turn on or off the use of the reaction field */
bool GridFF2::setUseReactionField(bool on)
{
    if (InterGroupCLJFF::setUseReactionField(on))
    {
        this->mustNowRecalculateFromScratch();
        return true;
    }
    else
        return false;
}

/** Set the dielectric constant to use with the reaction field potential */
bool GridFF2::setReactionFieldDielectric(double dielectric)
{
    if (InterGroupCLJFF::setReactionFieldDielectric(dielectric))
    {
        this->mustNowRecalculateFromScratch();
        return true;
    }

    return false;
}

/** Return the buffer size used when building grids */
SireUnits::Dimension::Length GridFF2::buffer() const
{
    return SireUnits::Dimension::Length(buffer_size);
}

/** Return the spacing between grid points */
SireUnits::Dimension::Length GridFF2::spacing() const
{
    return SireUnits::Dimension::Length(grid_spacing);
}

/** Return the cutoff for the coulomb energy */
SireUnits::Dimension::Length GridFF2::coulombCutoff() const
{
    return SireUnits::Dimension::Length(coul_cutoff);
}

/** Return the cutoff for the LJ energy */
SireUnits::Dimension::Length GridFF2::ljCutoff() const
{
    return SireUnits::Dimension::Length(lj_cutoff);
}

inline GridFF2::Vector4::Vector4(const Vector &v, double chg)
              : x(v.x()), y(v.y()), z(v.z()), q(chg)
{}

void GridFF2::appendTo(QVector<GridFF2::Vector4> &coords_and_charges,
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

#ifdef SIRE_USE_SSE
    inline QString toString(const __m128d &sseval)
    {
        return QString("{ %1, %2 }").arg(*((const double*)&sseval))
                                    .arg(*( ((const double*)&sseval) + 1));
    }
#endif

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

/*    ix = ipt / (dimy*dimz);
    iy = (ipt - ix*dimy*dimz) / dimz;
    iz = ipt % dimz;*/
}

/** Convert the indicies ix, iy, iz for the 3-dimensional array of dimension
    dimx, dimy, dimz into the returned index into the corresponding 1-dimensional array */
static int gridIndexToArrayIndex(int ix, int iy, int iz, int dimx, int dimy, int dimz)
{
    return ix*(dimy*dimz) + iy*dimz + iz;
}

void getGridPoint(int ipt, const Vector &min, int dimx, int dimy, int dimz,
                  double grid_spacing, MultiDouble &x, MultiDouble &y, MultiDouble &z)
{
    int ix, iy, iz;
    arrayIndexToGridIndex(ipt, dimx, dimy, dimz, ix, iy, iz);

    x = MultiDouble( min.x() + ix*grid_spacing );
    y = MultiDouble( min.y() + iy*grid_spacing );
    z = MultiDouble( min.z() + iz*grid_spacing );
}

void getGridPoint(int ipt, const Vector &min, int dimx, int dimy, int dimz,
                  double grid_spacing, MultiFloat &x, MultiFloat &y, MultiFloat &z)
{
    int ix, iy, iz;
    arrayIndexToGridIndex(ipt, dimx, dimy, dimz, ix, iy, iz);

    x = MultiFloat( min.x() + ix*grid_spacing );
    y = MultiFloat( min.y() + iy*grid_spacing );
    z = MultiFloat( min.z() + iz*grid_spacing );
}

/** Function used to add the potential from the passed points to the grid */
void GridFF2::addToGrid(const QVector<float> &vx,
                        const QVector<float> &vy,
                        const QVector<float> &vz,
                        const QVector<float> &vq)
{
    const QVector<MultiFloat> mx = MultiFloat::fromArray(vx);
    const QVector<MultiFloat> my = MultiFloat::fromArray(vy);
    const QVector<MultiFloat> mz = MultiFloat::fromArray(vz);
    const QVector<MultiFloat> mq = MultiFloat::fromArray(vq);

    Vector minpoint = gridbox.minCoords();

    const int npts = dimx*dimy*dimz;
    const int nvecs = mx.count();

    double *pot = gridpot.data();
    const MultiFloat *ax = mx.constData();
    const MultiFloat *ay = my.constData();
    const MultiFloat *az = mz.constData();
    const MultiFloat *aq = mq.constData();
    
    QTime t;
    t.start();
    
    const MultiFloat Rc( coul_cutoff );
    MultiFloat r, tmp;
    MultiFloat gx, gy, gz;
    MultiDouble cnrg;
    
    if (shiftElectrostatics())
    {
        const MultiFloat one_over_Rc( 1.0 / coul_cutoff );
        const MultiFloat one_over_Rc2( 1.0 / (coul_cutoff*coul_cutoff) );

        //loop over each grid point
        for (int ipt=0; ipt<npts; ++ipt)
        {
            getGridPoint(ipt, minpoint, dimx, dimy, dimz, grid_spacing, gx, gy, gz);
            
            cnrg = 0;
            
            for (int ivec=0; ivec<nvecs; ++ivec)
            {
                //calculate the distance between the atom and grid point (r)
                tmp = ax[ivec] - gx;
                r = tmp * tmp;
                tmp = ay[ivec] - gy;
                r.multiplyAdd(tmp, tmp);
                tmp = az[ivec] - gz;
                r.multiplyAdd(tmp, tmp);
                r = r.sqrt();
                
                //calculate the coulomb energy using shift-electrostatics
                // energy = q * { 1/r - 1/Rc + 1/Rc^2 [r - Rc] }
                tmp = r - Rc;
                tmp *= one_over_Rc2;
                tmp -= one_over_Rc;
                tmp += r.reciprocal();
                tmp *= aq[ivec];
                
                //apply the cutoff - compare r against Rc. This will
                //return 1 if r is less than Rc, or 0 otherwise. Logical
                //and will then remove all energies where r >= Rc
                cnrg += tmp.logicalAnd( r.compareLess(Rc) );
            }
            
            pot[ipt] += cnrg.sum();
        }
    }
    else if (useReactionField())
    {
        const MultiFloat rf_dielectric( reactionFieldDielectric() );
        const MultiFloat k_rf( (1.0 / pow_3(coul_cutoff)) * ( (reactionFieldDielectric()-1) /
                                                      (2*reactionFieldDielectric() + 1) ) );
        const MultiFloat c_rf( (1.0 / coul_cutoff) * ( (3*reactionFieldDielectric()) /
                                                      (2*reactionFieldDielectric() + 1) ) );

        //loop over each grid point
        for (int ipt=0; ipt<npts; ++ipt)
        {
            getGridPoint(ipt, minpoint, dimx, dimy, dimz, grid_spacing, gx, gy, gz);
            
            cnrg = 0;
            
            for (int ivec=0; ivec<nvecs; ++ivec)
            {
                //calculate the distance between the atom and grid point (r)
                tmp = ax[ivec] - gx;
                r = tmp * tmp;
                tmp = ay[ivec] - gy;
                r.multiplyAdd(tmp, tmp);
                tmp = az[ivec] - gz;
                r.multiplyAdd(tmp, tmp);
                
                //calculate the coulomb energy using reaction field
                // energy = q * { 1/r - c_rf + k_rf * r^2 }
                // where k_rf = (1 / r_c^3) * (eps - 1)/(2 eps + 1)
                // c_rf = (1/r_c) * (3 eps)/(2 eps + 1)
                tmp = r;
                r = r.sqrt();
                tmp *= k_rf;
                tmp -= c_rf;
                tmp += r.reciprocal();
                tmp *= aq[ivec];
                
                //apply the cutoff - compare r against Rc. This will
                //return 1 if r is less than Rc, or 0 otherwise. Logical
                //and will then remove all energies where r >= Rc
                cnrg += tmp.logicalAnd( r.compareLess(Rc) );
            }
            
            pot[ipt] += cnrg.sum();
        }
    }
    else
    {
        //use the simple atomistic cutoff

        //loop over each grid point
        for (int ipt=0; ipt<npts; ++ipt)
        {
            getGridPoint(ipt, minpoint, dimx, dimy, dimz, grid_spacing, gx, gy, gz);
            
            cnrg = 0;
            
            for (int ivec=0; ivec<nvecs; ++ivec)
            {
                //calculate the distance between the atom and grid point (r)
                tmp = ax[ivec] - gx;
                r = tmp * tmp;
                tmp = ay[ivec] - gy;
                r.multiplyAdd(tmp, tmp);
                tmp = az[ivec] - gz;
                r.multiplyAdd(tmp, tmp);
                r = r.sqrt();
                
                //calculate the coulomb energy using coulomb's law
                // energy = q * { 1/r }
                tmp = aq[ivec] / r;
                
                //apply the cutoff - compare r against Rc. This will
                //return 1 if r is less than Rc, or 0 otherwise. Logical
                //and will then remove all energies where r >= Rc
                cnrg += tmp.logicalAnd( r.compareLess(Rc) );
            }
            
            pot[ipt] += cnrg.sum();
        }
    }
    
    int ms = t.elapsed();
    
    qDebug() << "Added" << vx.count() << "more atoms to" << npts << "grid points in"
             << ms << "ms";
}

inline double getDist(double p, double minp, double maxp)
{
    if (p < minp)
        return minp - p;
    else if (p > maxp)
        return p - maxp;
    else
        return std::min(p - minp, maxp - p);
}

/** Return the minimum distance between a point and the passed AABox. This
    returns 0 if the point is inside the box */
static double minimumDistanceToGrid(const Vector &coords, const AABox &gridbox)
{
    const Vector mincoords = gridbox.minCoords();
    const Vector maxcoords = gridbox.maxCoords();

    int ix(0), iy(0), iz(0);
    double dx(0), dy(0), dz(0);
    
    if (coords.x() < mincoords.x())
    {
        ix = -1;
        dx = mincoords.x() - coords.x();
    }
    else if (coords.x() > maxcoords.x())
    {
        ix = 1;
        dx = coords.x() - maxcoords.x();
    }
    
    if (coords.y() < mincoords.y())
    {
        iy = -1;
        dy = mincoords.y() - coords.y();
    }
    else if (coords.y() > maxcoords.y())
    {
        iy = 1;
        dy = coords.y() - maxcoords.y();
    }
    
    if (coords.z() < mincoords.z())
    {
        iz = -1;
        dz = mincoords.z() - coords.z();
    }
    else if (coords.z() > maxcoords.z())
    {
        iz = 1;
        dz = coords.z() - maxcoords.z();
    }
    
    if (ix == 0 and iy == 0 and iz == 0)
        //the box contains the point
        return 0;
    else if (ix == 0 and iy == 0)
    {
        //the point is above or below the xy face
        return dz;
    }
    else if (ix == 0 and iz == 0)
    {
        //the point is above or below the xz face
        return dy;
    }
    else if (iy == 0 and iz == 0)
    {
        //the point is above or below the yz face
        return dx;
    }
    else if (ix == 0)
    {
        //the point is next to the yz corner
        return std::sqrt(dy*dy + dz*dz);
    }
    else if (iy == 0)
    {
        //the point is next to the xz corner
        return std::sqrt(dx*dx + dz*dz);
    }
    else if (iz == 0)
    {
        //the point is next to the xy corner
        return std::sqrt(dx*dx + dy*dy);
    }
    else
    {
        //the point is off the 3D corner
        return std::sqrt(dx*dx + dy*dy + dz*dz);
    }
}

/** Internal function used to rebuild the coulomb potential grid */
void GridFF2::rebuildGrid()
{
    QTime t;

    if (mols[0].moleculesByIndex().isEmpty())
        return;

    qDebug() << "REBUILDING THE GRID FOR FORCEFIELD" << this->name().value()
             << "USING A LJ_CUTOFF OF" << lj_cutoff << "A, A COULOMB CUTOFF OF"
             << coul_cutoff << "A AND A GRID SPACING OF" << grid_spacing << "A...";

    //find the bounding box that contains all of the atoms from group0
    AABox group0_box;
    
    for (ChunkedVector<CLJMolecule>::const_iterator
                                it = mols[0].moleculesByIndex().constBegin();
         it != mols[0].moleculesByIndex().constEnd();
         ++it)
    {
        const CoordGroupArray &coords = (*it).coordinates();
        
        for (int i=0; i<coords.count(); ++i)
        {
            if (group0_box.isNull())
            {
                group0_box = coords.constData()[i].aaBox();
            }
            else
            {
                group0_box += coords.constData()[i].aaBox();
            }
        }
    }
    
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
    
    dimx = 2 + int(boxsize.x() / grid_spacing);
    dimy = 2 + int(boxsize.y() / grid_spacing);
    dimz = 2 + int(boxsize.z() / grid_spacing);
    
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
    const Vector &grid_center = gridbox.center();
    
    qDebug() << "Adjusted grid extends over" << gridbox.toString();
    
    //create space for the grid
    gridpot = QVector<double>(dimx*dimy*dimz, 0.0);
    gridpot.squeeze();

    closemols_coords.clear();
    closemols_params.clear();
    
    //now build the grid - we take atoms that are within the LJ cutoff
    //and calculate those manually (saving them in the closemols list), and
    //only add coulomb potentials of CoordGroups that are beyond the LJ cutoff
    const ChunkedVector<CLJMolecule> &cljmols = mols[1].moleculesByIndex();
    
    const Space &spce = this->space();
    qDebug() << "Grid space equals:" << spce.toString();
    
    QVector<float> far_mols_x;
    QVector<float> far_mols_y;
    QVector<float> far_mols_z;
    QVector<float> far_mols_q;

    far_mols_x.reserve(1024);
    far_mols_y.reserve(1024);
    far_mols_z.reserve(1024);
    far_mols_q.reserve(1024);

    int atomcount = 0;
    int gridcount = 0;
    
    closemols_coords.reserve(cljmols.count() + (fixedatoms_coords.count() / 2));
    closemols_params.reserve(cljmols.count() + (fixedatoms_coords.count() / 2));
    
    if (fixedatoms_coords.count() > 0)
    {
        qDebug() << "Adding all of the fixed atom points to the grid...";

        for (int i=0; i<fixedatoms_coords.count(); ++i)
        {
            Vector coords = fixedatoms_coords.constData()[i];
            
            if (spce.isPeriodic())
            {
                coords = spce.getMinimumImage(coords, grid_center);
            }
            
            const SireMM::detail::CLJParameter &params = fixedatoms_params.constData()[i];

            if (params.reduced_charge == 0 and params.ljid == 0)
                continue;
            
            //calculate the closest distance between this point and the grid
            double dist = ::minimumDistanceToGrid(coords, gridbox);
            
            //only explicitly evaluate points within the LJ cutoff of the grid
            if (dist < lj_cutoff)
            {
                atomcount += 1;
                closemols_coords.append(coords);
                closemols_params.append(params);
            }
            //all other points are evaluated using the grid
            else
            {
                if (params.reduced_charge != 0)
                {
                    far_mols_x.append(coords.x());
                    far_mols_y.append(coords.y());
                    far_mols_z.append(coords.z());
                    far_mols_q.append(params.reduced_charge);
                
                    if (far_mols_x.count() > 1023)
                    {
                        addToGrid(far_mols_x, far_mols_y, far_mols_z, far_mols_q);
                        gridcount += far_mols_x.count();
                        far_mols_x.clear();
                        far_mols_y.clear();
                        far_mols_z.clear();
                        far_mols_q.clear();
                        //qDebug() << "Added" << i+1 << "of" << fixedatoms_coords.count()
                        //         << "fixed atoms to the grid...";
                    }
                }
            }
        }
        
        addToGrid(far_mols_x, far_mols_y, far_mols_z, far_mols_q);
        gridcount += far_mols_x.count();
        far_mols_x.clear();
        far_mols_y.clear();
        far_mols_z.clear();
        far_mols_q.clear();
        qDebug() << "Added all of the fixed atoms to the grid.";
        qDebug() << "The number of explicitly evaluated atoms is now" << atomcount;
        qDebug() << "The number of grid evaluated atoms is now" << gridcount;
    }
    
    if (not cljmols.isEmpty())
    {
        qDebug()
            << "Building the list of close molecules and adding far molecules to the grid...";

        int nmols = 0;
    
        for (ChunkedVector<CLJMolecule>::const_iterator it = cljmols.constBegin();
             it != cljmols.constEnd();
             ++it)
        {
            nmols += 1;
        
            const CLJMolecule &cljmol = *it;

            //loop through each CutGroup of this molecule
            const int ngroups = cljmol.coordinates().count();
            
            const CoordGroup *groups_array = cljmol.coordinates().constData();
            
            const CLJParameters::Array *params_array 
                                    = cljmol.parameters().atomicParameters().constData();

            for (int igroup=0; igroup<ngroups; ++igroup)
            {
                CoordGroup coordgroup = groups_array[igroup];
                const CLJParameters::Array &paramsgroup = params_array[igroup];

                for (int i=0; i<coordgroup.count(); ++i)
                {
                    Vector coords = coordgroup.constData()[i];
            
                    if (spce.isPeriodic())
                    {
                        coords = spce.getMinimumImage(coords, grid_center);
                    }
            
                    const SireMM::detail::CLJParameter &params = paramsgroup.constData()[i];
            
                    if (params.reduced_charge == 0 and params.ljid == 0)
                        continue;
            
                    //calculate the closest distance between this point and the grid
                    double dist = ::minimumDistanceToGrid(coords, gridbox);
            
                    //only explicitly evaluate points within the LJ cutoff of the grid
                    if (dist < lj_cutoff)
                    {
                        atomcount += 1;
                        closemols_coords.append(coords);
                        closemols_params.append(params);
                    }
                    //all other points are evaluated using the grid
                    else
                    {
                        if (params.reduced_charge != 0)
                        {
                            far_mols_x.append(coords.x());
                            far_mols_y.append(coords.y());
                            far_mols_z.append(coords.z());
                            far_mols_q.append(params.reduced_charge);
                
                            if (far_mols_x.count() > 1023)
                            {
                                addToGrid(far_mols_x, far_mols_y, far_mols_z, far_mols_q);
                                gridcount += far_mols_x.count();
                                far_mols_x.clear();
                                far_mols_y.clear();
                                far_mols_z.clear();
                                far_mols_q.clear();
                                qDebug() << "Added" << nmols << "of" << cljmols.count()
                                         << "molecules to the grid...";
                            }
                        }
                    }
                }
            }
        }
        
        addToGrid(far_mols_x, far_mols_y, far_mols_z, far_mols_q);
        gridcount += far_mols_x.count();
        far_mols_x.clear();
        far_mols_y.clear();
        far_mols_z.clear();
        far_mols_q.clear();
        qDebug() << "Added all of the group 1 molecules to the grid.";
        qDebug() << "The number of explicitly evaluated atoms is now" << atomcount;
        qDebug() << "The number of grid evaluated atoms is now" << gridcount;
    }
 
    {
        double grid_sum = 0;
        
        for (int ipt=0; ipt<(dimx*dimy*dimz); ++ipt)
        {
            grid_sum += gridpot.at(ipt);
        }
        
        qDebug() << "Sum of grid potentials is" << grid_sum;
    }

    closemols_coords.squeeze();
    closemols_params.squeeze();
}

void GridFF2::calculateEnergy(const CoordGroup &coords0, 
                             const GridFF2::CLJParameters::Array &params0,
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
        if (shiftElectrostatics())
        {
            //use the force-shifted cutoff with an alpha value of 0
            const double Rc = coul_cutoff;
            const double Rlj = lj_cutoff;
            
            const double one_over_Rc = double(1) / Rc;
            const double one_over_Rc2 = double(1) / (Rc*Rc);
        
            #ifdef SIRE_USE_SSE
            {
                const int remainder = nats1 % 2;
                
                __m128d sse_cnrg = { 0, 0 };
                __m128d sse_ljnrg = { 0, 0 };

                const __m128d sse_one = { 1.0, 1.0 };
                
                const __m128d sse_Rc = { Rc, Rc };
                const __m128d sse_Rlj = { Rlj, Rlj };
                
                const __m128d sse_one_over_Rc = { one_over_Rc, one_over_Rc };
                const __m128d sse_one_over_Rc2 = { one_over_Rc2, one_over_Rc2 };
                
                for (int j=0; j<nats1-1; j += 2)
                {
                    const Parameter &param10 = params1_array[j];
                    const Parameter &param11 = params1_array[j+1];

                    const Vector &c10 = coords1_array[j];
                    const Vector &c11 = coords1_array[j+1];
                    
                    for (int i=0; i<nats0; ++i)
                    {
                        const Parameter &param0 = params0_array[i];
                    
                        __m128d sse_chg0 = _mm_set_pd( param0.reduced_charge,
                                                       param0.reduced_charge );
                    
                        const Vector &c0 = coords0_array[i];

                        __m128d sse_r;
                        {
                            __m128d dx = _mm_set_pd( c0.x() - c10.x(), c0.x() - c11.x() );
                            __m128d dy = _mm_set_pd( c0.y() - c10.y(), c0.y() - c11.y() );
                            __m128d dz = _mm_set_pd( c0.z() - c10.z(), c0.z() - c11.z() );
                        
                            __m128d dx2 = _mm_mul_pd(dx, dx);
                            __m128d dy2 = _mm_mul_pd(dy, dy);
                            __m128d dz2 = _mm_mul_pd(dz, dz);
                        
                            __m128d sse_r2 = _mm_add_pd(dx2,dy2);
                            sse_r2 = _mm_add_pd(sse_r2, dz2);
                        
                            sse_r = _mm_sqrt_pd(sse_r2);
                        }
                        
                        const __m128d sse_one_over_r = _mm_div_pd(sse_one, sse_r);

                        //calculate the coulomb energy
                        {
                            const __m128d sse_in_cutoff = _mm_cmplt_pd(sse_r, sse_Rc);
                            __m128d nrg = _mm_sub_pd(sse_r, sse_Rc);
                            nrg = _mm_mul_pd(nrg, sse_one_over_Rc2);
                            nrg = _mm_add_pd(nrg, sse_one_over_r);
                            nrg = _mm_sub_pd(nrg, sse_one_over_Rc);

                            __m128d sse_chg = _mm_set_pd( param10.reduced_charge,
                                                          param11.reduced_charge );
                        
                            sse_chg = _mm_mul_pd(sse_chg, sse_chg0);
                        
                            nrg = _mm_mul_pd(sse_chg, nrg);

                            nrg = _mm_and_pd(nrg, sse_in_cutoff);
                            
                            sse_cnrg = _mm_add_pd(sse_cnrg, nrg);
                        }
                        
                        //calculate the LJ energy
                        {
                            const __m128d sse_in_cutoff = _mm_cmplt_pd(sse_r, sse_Rlj);
                            const LJPair &ljpair0 = ljpairs.constData()[
                                                        ljpairs.map(param0.ljid,
                                                                    param10.ljid)];
                    
                            const LJPair &ljpair1 = ljpairs.constData()[
                                                        ljpairs.map(param0.ljid,
                                                                    param11.ljid)];

                            const __m128d sse_sig = _mm_set_pd( ljpair0.sigma(), ljpair1.sigma() );
                            const __m128d sse_eps = _mm_set_pd( ljpair0.epsilon(),
                                                                ljpair1.epsilon() );
                        
                            //calculate (sigma/r)^6 and (sigma/r)^12
                            __m128d sse_sig_over_r2 = _mm_mul_pd(sse_sig, sse_one_over_r);
                            sse_sig_over_r2 = _mm_mul_pd( sse_sig_over_r2,
                                                          sse_sig_over_r2 );
                                                     
                            __m128d sse_sig_over_r6 = _mm_mul_pd(sse_sig_over_r2,
                                                                 sse_sig_over_r2);
                                                        
                            sse_sig_over_r6 = _mm_mul_pd(sse_sig_over_r6,
                                                         sse_sig_over_r2);
                                                     
                            __m128d sse_sig_over_r12 = _mm_mul_pd(sse_sig_over_r6,
                                                                  sse_sig_over_r6);
                                              
                            //calculate LJ energy (the factor of 4 is added later)
                            __m128d tmp = _mm_sub_pd(sse_sig_over_r12,
                                                     sse_sig_over_r6);
                                                 
                            tmp = _mm_mul_pd(tmp, sse_eps);
                            tmp = _mm_and_pd(tmp, sse_in_cutoff);
                            sse_ljnrg = _mm_add_pd(sse_ljnrg, tmp);
                        }
                    }
                }
                
                if (remainder == 1)
                {
                    const Vector &c1 = coords1_array[nats1-1];
                    const Parameter &param1 = params1_array[nats1-1];

                    for (int i=0; i<nats0; ++i)
                    {
                        const Vector &c0 = coords0_array[i];
                        const Parameter &param0 = params0_array[i];

                        const double r = Vector::distance(c0,c1);
                        const double one_over_r = double(1) / r;
                        
                        if (r < Rc)
                        {
                            icnrg += param0.reduced_charge * param1.reduced_charge
                                        * (one_over_r - one_over_Rc + one_over_Rc2*(r-Rc));
                        }

                        if (r < Rlj)
                        {
                            const LJPair &ljpair = ljpairs.constData()[
                                                    ljpairs.map(param0.ljid,
                                                                param1.ljid)];
                        
                            double sig_over_dist6 = pow_6(ljpair.sigma()*one_over_r);
                            double sig_over_dist12 = pow_2(sig_over_dist6);

                            iljnrg += ljpair.epsilon() * (sig_over_dist12 -
                                                          sig_over_dist6);
                        }
                    }
                }
                
                icnrg += *((const double*)&sse_cnrg) +
                         *( ((const double*)&sse_cnrg) + 1 );
                         
                iljnrg += *((const double*)&sse_ljnrg) +
                          *( ((const double*)&sse_ljnrg) + 1 );
            }
            #else
            {
                //we use the force-shifted cutoff potential described in equation 18
                //in Fennell and Gezelter, J. Chem. Phys., 124, 234104, 2006
                //We use alpha=0, as I have seen that a 25 A cutoff gives stable results
                //with alpha=0, and this way we avoid changing the hamiltonian significantly
                //by having an erfc function
                for (int j=0; j<nats1; ++j)
                {
                    const Parameter &param1 = params1_array[j];
                    const Vector &c1 = coords1_array[j];
                    
                    for (int i=0; i<nats0; ++i)
                    {
                        const Parameter &param0 = params0_array[i];
                        const Vector &c0 = coords0_array[i];

                        const double r = Vector::distance(c0,c1);
                        const double one_over_r = double(1) / r;
                        
                        if (r < Rc)
                        {
                            icnrg += (param0.reduced_charge * param1.reduced_charge) *
                                        (one_over_r - one_over_Rc + one_over_Rc2 * (r-Rc));
                        }
                        
                        if (r < Rlj)
                        {
                            const LJPair &ljpair = ljpairs.constData()[
                                                    ljpairs.map(param0.ljid,
                                                                param1.ljid)];

                            double sig_over_dist6 = pow_6(ljpair.sigma()*one_over_r);
                            double sig_over_dist12 = pow_2(sig_over_dist6);

                            iljnrg += ljpair.epsilon() * (sig_over_dist12 -
                                                          sig_over_dist6);
                        }
                    }
                }
            }
            #endif
        }
        else if (useReactionField())
        {
            //use the reaction field potential
            const double Rc = coul_cutoff;
            const double Rlj = lj_cutoff;
            
            const double rf_dielectric = reactionFieldDielectric();
            const double k_rf = (1.0 / pow_3(Rc)) * ( (rf_dielectric-1) /
                                                      (2*rf_dielectric + 1) );
            const double c_rf = (1.0 / Rc) * ( (3*rf_dielectric) /
                                               (2*rf_dielectric + 1) );            
        
            #ifdef SIRE_USE_SSE
            {
                const int remainder = nats1 % 2;
                
                __m128d sse_cnrg = { 0, 0 };
                __m128d sse_ljnrg = { 0, 0 };

                const __m128d sse_one = { 1.0, 1.0 };
                
                const __m128d sse_Rc = { Rc, Rc };
                const __m128d sse_Rlj = { Rlj, Rlj };
                
                const __m128d sse_k_rf = { k_rf, k_rf };
                const __m128d sse_c_rf = { c_rf, c_rf };
                
                for (int j=0; j<nats1-1; j += 2)
                {
                    const Parameter &param10 = params1_array[j];
                    const Parameter &param11 = params1_array[j+1];

                    const Vector &c10 = coords1_array[j];
                    const Vector &c11 = coords1_array[j+1];

                    for (int i=0; i<nats0; ++i)
                    {
                        const Parameter &param0 = params0_array[i];
                    
                        __m128d sse_chg0 = _mm_set_pd( param0.reduced_charge,
                                                       param0.reduced_charge );
                    
                        const Vector &c0 = coords0_array[i];

                        __m128d sse_r;
                        {
                            __m128d dx = _mm_set_pd( c0.x() - c10.x(), c0.x() - c11.x() );
                            __m128d dy = _mm_set_pd( c0.y() - c10.y(), c0.y() - c11.y() );
                            __m128d dz = _mm_set_pd( c0.z() - c10.z(), c0.z() - c11.z() );
                        
                            __m128d dx2 = _mm_mul_pd(dx, dx);
                            __m128d dy2 = _mm_mul_pd(dy, dy);
                            __m128d dz2 = _mm_mul_pd(dz, dz);
                        
                            __m128d sse_r2 = _mm_add_pd(dx2,dy2);
                            sse_r2 = _mm_add_pd(sse_r2, dz2);
                        
                            sse_r = _mm_sqrt_pd(sse_r2);
                        }
                        
                        const __m128d sse_one_over_r = _mm_div_pd(sse_one, sse_r);
                        
                        //calculate the coulomb energy
                        {
                            const __m128d sse_in_cutoff = _mm_cmplt_pd(sse_r, sse_Rc);

                            __m128d nrg = _mm_mul_pd(sse_r, sse_r);
                            nrg = _mm_mul_pd(nrg, sse_k_rf);
                            nrg = _mm_sub_pd(nrg, sse_c_rf);
                            nrg = _mm_add_pd(nrg, sse_one_over_r);

                            __m128d sse_chg = _mm_set_pd( param10.reduced_charge,
                                                          param11.reduced_charge );
                    
                            sse_chg = _mm_mul_pd(sse_chg, sse_chg0);
                    
                            nrg = _mm_mul_pd(sse_chg, nrg);

                            nrg = _mm_and_pd(nrg, sse_in_cutoff);
                            
                            sse_cnrg = _mm_add_pd(sse_cnrg, nrg);
                        }
                        
                        //calculate the LJ energy
                        {
                            const __m128d sse_in_cutoff = _mm_cmplt_pd(sse_r, sse_Rlj);
                            const LJPair &ljpair0 = ljpairs.constData()[
                                                ljpairs.map(param0.ljid,
                                                            param10.ljid)];
                    
                            const LJPair &ljpair1 = ljpairs.constData()[
                                                        ljpairs.map(param0.ljid,
                                                                    param11.ljid)];

                            const __m128d sse_sig = _mm_set_pd( ljpair0.sigma(), ljpair1.sigma() );
                            const __m128d sse_eps = _mm_set_pd( ljpair0.epsilon(),
                                                                ljpair1.epsilon() );
                        
                            //calculate (sigma/r)^6 and (sigma/r)^12
                            __m128d sse_sig_over_r2 = _mm_mul_pd(sse_sig, sse_one_over_r);
                            sse_sig_over_r2 = _mm_mul_pd( sse_sig_over_r2,
                                                          sse_sig_over_r2 );
                                                     
                            __m128d sse_sig_over_r6 = _mm_mul_pd(sse_sig_over_r2,
                                                                 sse_sig_over_r2);
                                                        
                            sse_sig_over_r6 = _mm_mul_pd(sse_sig_over_r6,
                                                         sse_sig_over_r2);
                                                     
                            __m128d sse_sig_over_r12 = _mm_mul_pd(sse_sig_over_r6,
                                                                  sse_sig_over_r6);
                                              
                            //calculate LJ energy (the factor of 4 is added later)
                            __m128d tmp = _mm_sub_pd(sse_sig_over_r12,
                                                     sse_sig_over_r6);
                                                 
                            tmp = _mm_mul_pd(tmp, sse_eps);
                            tmp = _mm_and_pd(tmp, sse_in_cutoff);
                            sse_ljnrg = _mm_add_pd(sse_ljnrg, tmp);
                        }
                    }
                }
                
                if (remainder == 1)
                {
                    const Vector &c1 = coords1_array[nats1-1];
                    const Parameter &param1 = params1_array[nats1-1];

                    for (int i=0; i<nats0; ++i)
                    {
                        const Vector &c0 = coords0_array[i];
                        const Parameter &param0 = params0_array[i];

                        const double r = Vector::distance(c0,c1);
                        const double one_over_r = double(1) / r;
                        
                        if (r < Rc)
                        {
                            icnrg += param0.reduced_charge * param1.reduced_charge
                                        * (one_over_r + k_rf*r*r - c_rf);
                        }
                        
                        if (r < Rlj)
                        {
                            const LJPair &ljpair = ljpairs.constData()[
                                                    ljpairs.map(param0.ljid,
                                                                param1.ljid)];
                        
                            double sig_over_dist6 = pow_6(ljpair.sigma()*one_over_r);
                            double sig_over_dist12 = pow_2(sig_over_dist6);

                            iljnrg += ljpair.epsilon() * (sig_over_dist12 -
                                                          sig_over_dist6);
                        }
                    }
                }
                
                icnrg += *((const double*)&sse_cnrg) +
                         *( ((const double*)&sse_cnrg) + 1 );
                         
                iljnrg += *((const double*)&sse_ljnrg) +
                          *( ((const double*)&sse_ljnrg) + 1 );
            }
            #else
            {
                //we use the force-shifted cutoff potential described in equation 18
                //in Fennell and Gezelter, J. Chem. Phys., 124, 234104, 2006
                //We use alpha=0, as I have seen that a 25 A cutoff gives stable results
                //with alpha=0, and this way we avoid changing the hamiltonian significantly
                //by having an erfc function
                for (int j=0; j<nats1; ++j)
                {
                    const Parameter &param1 = params1_array[j];
                    const Vector &c1 = coords1_array[j];
                    
                    for (int i=0; i<nats0; ++i)
                    {
                        const Parameter &param0 = params0_array[i];
                        const Vector &c0 = coords0_array[i];

                        const double r = Vector::distance(c0,c1);
                        const double one_over_r = double(1) / r;
                        
                        if (r < Rc)
                        {
                            icnrg += (param0.reduced_charge * param1.reduced_charge) *
                                     (one_over_r + k_rf*r*r - c_rf);
                        }
                        
                        if (r < Rlj)
                        {
                            const LJPair &ljpair = ljpairs.constData()[
                                                    ljpairs.map(param0.ljid,
                                                                param1.ljid)];

                            double sig_over_dist6 = pow_6(ljpair.sigma()*one_over_r);
                            double sig_over_dist12 = pow_2(sig_over_dist6);

                            iljnrg += ljpair.epsilon() * (sig_over_dist12 -
                                                          sig_over_dist6);
                        }
                    }
                }
            }
            #endif
        }
        else
        {
            //use a simple atom-based cutoff
            const double Rc = coul_cutoff;
            const double Rlj = lj_cutoff;
        
            #ifdef SIRE_USE_SSE
            {
                const int remainder = nats1 % 2;
                
                __m128d sse_cnrg = { 0, 0 };
                __m128d sse_ljnrg = { 0, 0 };

                const __m128d sse_one = { 1.0, 1.0 };
                const __m128d sse_Rc = { Rc, Rc };
                const __m128d sse_Rlj = { Rlj, Rlj };
                
                for (int j=0; j<nats1-1; j += 2)
                {
                    const Parameter &param10 = params1_array[j];
                    const Parameter &param11 = params1_array[j+1];

                    const Vector &c10 = coords1_array[j];
                    const Vector &c11 = coords1_array[j+1];

                    for (int i=0; i<nats0; ++i)
                    {
                        const Parameter &param0 = params0_array[i];
                    
                        __m128d sse_chg0 = _mm_set_pd( param0.reduced_charge,
                                                       param0.reduced_charge );
                    
                        const Vector &c0 = coords0_array[i];
                    
                        __m128d sse_r;
                        {
                            __m128d dx = _mm_set_pd( c0.x() - c10.x(), c0.x() - c11.x() );
                            __m128d dy = _mm_set_pd( c0.y() - c10.y(), c0.y() - c11.y() );
                            __m128d dz = _mm_set_pd( c0.z() - c10.z(), c0.z() - c11.z() );
                        
                            __m128d dx2 = _mm_mul_pd(dx, dx);
                            __m128d dy2 = _mm_mul_pd(dy, dy);
                            __m128d dz2 = _mm_mul_pd(dz, dz);
                        
                            __m128d sse_r2 = _mm_add_pd(dx2,dy2);
                            sse_r2 = _mm_add_pd(sse_r2, dz2);
                        
                            sse_r = _mm_sqrt_pd(sse_r2);
                        }
                        
                        const __m128d sse_one_over_r = _mm_div_pd(sse_one, sse_r);
                        
                        //calculate the coulomb energy
                        {
                            const __m128d sse_in_cutoff = _mm_cmplt_pd(sse_r, sse_Rc);

                            __m128d sse_chg = _mm_set_pd( param10.reduced_charge,
                                                          param11.reduced_charge );
                        
                            sse_chg = _mm_mul_pd(sse_chg, sse_chg0);
                        
                            __m128d nrg = _mm_mul_pd(sse_chg, sse_one_over_r);

                            nrg = _mm_and_pd(nrg, sse_in_cutoff);
                            
                            sse_cnrg = _mm_add_pd(sse_cnrg, nrg);
                        }
                        
                        //calculate the LJ energy
                        {
                            const __m128d sse_in_cutoff = _mm_cmplt_pd(sse_r, sse_Rlj);
                        
                            const LJPair &ljpair0 = ljpairs.constData()[
                                                        ljpairs.map(param0.ljid,
                                                                    param10.ljid)];
                    
                            const LJPair &ljpair1 = ljpairs.constData()[
                                                        ljpairs.map(param0.ljid,
                                                                    param11.ljid)];

                            const __m128d sse_sig = _mm_set_pd( ljpair0.sigma(), ljpair1.sigma() );
                            const __m128d sse_eps = _mm_set_pd( ljpair0.epsilon(),
                                                                ljpair1.epsilon() );
                        
                            //calculate (sigma/r)^6 and (sigma/r)^12
                            __m128d sse_sig_over_r2 = _mm_mul_pd(sse_sig, sse_one_over_r);
                            sse_sig_over_r2 = _mm_mul_pd( sse_sig_over_r2,
                                                          sse_sig_over_r2 );
                                                     
                            __m128d sse_sig_over_r6 = _mm_mul_pd(sse_sig_over_r2,
                                                                 sse_sig_over_r2);
                                                        
                            sse_sig_over_r6 = _mm_mul_pd(sse_sig_over_r6,
                                                         sse_sig_over_r2);
                                                     
                            __m128d sse_sig_over_r12 = _mm_mul_pd(sse_sig_over_r6,
                                                                  sse_sig_over_r6);
                                              
                            //calculate LJ energy (the factor of 4 is added later)
                            __m128d tmp = _mm_sub_pd(sse_sig_over_r12,
                                                     sse_sig_over_r6);
                                                 
                            tmp = _mm_mul_pd(tmp, sse_eps);
                            tmp = _mm_and_pd(tmp, sse_in_cutoff);
                            sse_ljnrg = _mm_add_pd(sse_ljnrg, tmp);
                        }
                    }
                }
                          
                if (remainder == 1)
                {
                    const Vector &c1 = coords1_array[nats1-1];
                    const Parameter &param1 = params1_array[nats1-1];

                    for (int i=0; i<nats0; ++i)
                    {
                        const Vector &c0 = coords0_array[i];
                        const Parameter &param0 = params0_array[i];

                        const double r = Vector::distance(c0,c1);
                        const double one_over_r = double(1) / r;
                        
                        if (r < Rc)
                        {
                            icnrg += param0.reduced_charge * param1.reduced_charge
                                                * one_over_r;
                        }

                        if (r < Rlj)
                        {
                            const LJPair &ljpair = ljpairs.constData()[
                                                    ljpairs.map(param0.ljid,
                                                                param1.ljid)];
                        
                            double sig_over_dist6 = pow_6(ljpair.sigma()*one_over_r);
                            double sig_over_dist12 = pow_2(sig_over_dist6);

                            iljnrg += ljpair.epsilon() * (sig_over_dist12 -
                                                          sig_over_dist6);
                        }
                    }
                }
                
                icnrg += *((const double*)&sse_cnrg) +
                         *( ((const double*)&sse_cnrg) + 1 );
                         
                iljnrg += *((const double*)&sse_ljnrg) +
                          *( ((const double*)&sse_ljnrg) + 1 );
            }
            #else
            {
                // use an atomistic cutoff
                for (int j=0; j<nats1; ++j)
                {
                    const Parameter &param1 = params1_array[j];
                    const Vector &c1 = coords1_array[j];
                    
                    for (int i=0; i<nats0; ++i)
                    {
                        const Parameter &param0 = params0_array[i];
                        const Vector &c0 = coords0_array[i];

                        const double r = Vector::distance(c0,c1);
                        const double one_over_r = double(1) / r;
                        
                        if (r < Rc)
                        {
                            icnrg += (param0.reduced_charge * param1.reduced_charge) *
                                        one_over_r;
                        }
                        
                        if (r < Rlj)
                        {
                            const LJPair &ljpair = ljpairs.constData()[
                                                    ljpairs.map(param0.ljid,
                                                                param1.ljid)];

                            double sig_over_dist6 = pow_6(ljpair.sigma()*one_over_r);
                            double sig_over_dist12 = pow_2(sig_over_dist6);

                            iljnrg += ljpair.epsilon() * (sig_over_dist12 -
                                                          sig_over_dist6);
                        }
                    }
                }
            }
            #endif
        }
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
        
        if (i_0 < 0 or i_0 >= int(dimx-1) or
            j_0 < 0 or j_0 >= int(dimy-1) or
            k_0 < 0 or k_0 >= int(dimz-1))
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
                              
            gridnrg += phi * p0.reduced_charge;
        }
    }
    
    cnrg = icnrg + gridnrg;
    ljnrg = 4.0*iljnrg;  // 4 epsilon (....)
}

/** Ensure that the next energy evaluation is from scratch */
void GridFF2::mustNowRecalculateFromScratch()
{
    gridpot.clear();
    closemols_coords.clear();
    closemols_params.clear();
    oldnrgs.clear();
    
    InterGroupCLJFF::mustNowRecalculateFromScratch();
}

/** Any additions mean that the forcefield must be recalculated from scratch */
void GridFF2::_pvt_added(quint32 groupid, const PartialMolecule &mol, 
                        const PropertyMap &map)
{
    this->mustNowRecalculateFromScratch();
    InterGroupCLJFF::_pvt_added(groupid, mol, map);
}

/** Any removals mean that the forcefield must be recalculated from scratch */
void GridFF2::_pvt_removed(quint32 groupid, const PartialMolecule &mol)
{
    this->mustNowRecalculateFromScratch();
    InterGroupCLJFF::_pvt_removed(groupid, mol);
}

/** Any changes to group 1 mean that the forcefield must be recalculated from scratch */
void GridFF2::_pvt_changed(quint32 groupid, const SireMol::Molecule &molecule)
{
    InterGroupCLJFF::_pvt_changed(groupid, molecule);
}

/** Any changes to group 1 mean that the forcefield must be recalculated from scratch */
void GridFF2::_pvt_changed(quint32 groupid, const QList<SireMol::Molecule> &molecules)
{
    InterGroupCLJFF::_pvt_changed(groupid, molecules);
}

/** Any removals mean that the forcefield must be recalculated from scratch */
void GridFF2::_pvt_removedAll(quint32 groupid)
{
    this->mustNowRecalculateFromScratch();
    InterGroupCLJFF::_pvt_removedAll(groupid);
}

/** Recalculate the total energy */
void GridFF2::recalculateEnergy()
{
    CLJPotential::startEvaluation();

    if (mols[0].isEmpty() or (mols[1].isEmpty() and fixedatoms_coords.isEmpty()))
    {
        //one of the two groups is empty, so the energy must be zero
        this->components().setEnergy(*this, CLJEnergy(0,0));
        CLJPotential::finishedEvaluation();
        this->setClean();
        return;
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
            if (it->nothingChanged())
            {
                continue;
            }
            else if (it->changedAll())
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
            else
            {
                //only calculate the change in energy for the part of the
                //molecule that has changed
                const CLJMolecule &oldmol = it->oldParts();
                const CLJMolecule &newmol = it->newParts();
                
                //calculate the energy of the old parts
                double old_cnrg(0);
                double old_ljnrg(0);
                {
                    const int ngroups = oldmol.coordinates().count();
            
                    const CoordGroup *groups_array = oldmol.coordinates().constData();
            
                    const CLJParameters::Array *params_array
                                    = oldmol.parameters().atomicParameters().constData();
            
                    for (int igroup=0; igroup<ngroups; ++igroup)
                    {
                        const CoordGroup &group = groups_array[igroup];

                        if (not gridbox.contains(group.aaBox()))
                        {
                            //this group lies outside the grid - we need to recalculate
                            //the grid
                            qDebug() << "MOLECULE" << oldmol.number().toString() << "HAS MOVED"
                                     << "OUTSIDE THE GRID. MUST RECALCULATE!";
                            this->mustNowRecalculateFromScratch();
                            this->recalculateEnergy();
                            return;
                        }

                        double icnrg, iljnrg;
                        calculateEnergy(group, params_array[igroup],
                                        icnrg, iljnrg);
                                    
                        old_cnrg += icnrg;
                        old_ljnrg += iljnrg;
                    }
                }
                
                //calculate the energy of the new parts
                double new_cnrg(0);
                double new_ljnrg(0);
                {
                    const int ngroups = newmol.coordinates().count();
            
                    const CoordGroup *groups_array = newmol.coordinates().constData();
            
                    const CLJParameters::Array *params_array
                                    = newmol.parameters().atomicParameters().constData();
            
                    for (int igroup=0; igroup<ngroups; ++igroup)
                    {
                        const CoordGroup &group = groups_array[igroup];

                        if (not gridbox.contains(group.aaBox()))
                        {
                            //this group lies outside the grid - we need to recalculate
                            //the grid
                            qDebug() << "MOLECULE" << newmol.number().toString() << "HAS MOVED"
                                     << "OUTSIDE THE GRID. MUST RECALCULATE!";
                            this->mustNowRecalculateFromScratch();
                            this->recalculateEnergy();
                            return;
                        }

                        double icnrg, iljnrg;
                        calculateEnergy(group, params_array[igroup],
                                        icnrg, iljnrg);
                                    
                        new_cnrg += icnrg;
                        new_ljnrg += iljnrg;
                    }
                }

                CLJEnergy old_nrg = oldnrgs[oldmol.number()];
                CLJEnergy new_nrg = CLJEnergy(old_nrg.coulomb() + new_cnrg - old_cnrg,
                                              old_nrg.lj() + new_ljnrg - old_ljnrg);
                
                oldnrgs[oldmol.number()] = new_nrg;
                
                delta_cnrg += (new_nrg.coulomb() - old_nrg.coulomb());
                delta_ljnrg += (new_nrg.lj() - old_nrg.lj());
            }
        }
        
        //change the energy
        this->components().changeEnergy(*this, CLJEnergy(delta_cnrg,delta_ljnrg));
        
        //clear the changed molecules
        this->changed_mols[0].clear();
    }
 
    CLJPotential::finishedEvaluation();
    this->setClean();
}
