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

#include "cljshiftfunction.h"
#include "gridinfo.h"

#include "SireMaths/multifloat.h"
#include "SireMaths/multidouble.h"
#include "SireMaths/multiint.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include "tbb/blocked_range.h"
#include "tbb/parallel_for.h"

#include <QElapsedTimer>
#include <QDebug>

using namespace SireMM;
using namespace SireMaths;
using namespace SireBase;
using namespace SireStream;

/////////
///////// Implementation of CLJShiftFunction
/////////

static const RegisterMetaType<CLJShiftFunction> r_shift;

QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const CLJShiftFunction &func)
{
    writeHeader(ds, r_shift, 1);
    
    ds << static_cast<const CLJCutoffFunction&>(func);
    
    return ds;
}

QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, CLJShiftFunction &func)
{
    VersionID v = readHeader(ds, r_shift);
    
    if (v == 1)
    {
        ds >> static_cast<CLJCutoffFunction&>(func);
    }
    else
        throw version_error(v, "1", r_shift, CODELOC);
    
    return ds;
}

CLJShiftFunction::CLJShiftFunction()
                 : ConcreteProperty<CLJShiftFunction,CLJCutoffFunction>()
{}

CLJShiftFunction::CLJShiftFunction(Length cutoff)
                 : ConcreteProperty<CLJShiftFunction,CLJCutoffFunction>(cutoff)
{}

CLJShiftFunction::CLJShiftFunction(Length coul_cutoff, Length lj_cutoff)
                 : ConcreteProperty<CLJShiftFunction,CLJCutoffFunction>(coul_cutoff, lj_cutoff)
{}

CLJShiftFunction::CLJShiftFunction(const Space &space, Length cutoff)
                 : ConcreteProperty<CLJShiftFunction,CLJCutoffFunction>(space, cutoff)
{}

CLJShiftFunction::CLJShiftFunction(const Space &space, Length coul_cutoff, Length lj_cutoff)
                 : ConcreteProperty<CLJShiftFunction,CLJCutoffFunction>(space, coul_cutoff,
                                                                        lj_cutoff)
{}

CLJShiftFunction::CLJShiftFunction(Length cutoff, COMBINING_RULES combining_rules)
                 : ConcreteProperty<CLJShiftFunction,CLJCutoffFunction>(cutoff, combining_rules)
{}

CLJShiftFunction::CLJShiftFunction(Length coul_cutoff, Length lj_cutoff,
                                   COMBINING_RULES combining_rules)
                 : ConcreteProperty<CLJShiftFunction,CLJCutoffFunction>(
                                   coul_cutoff, lj_cutoff, combining_rules)
{}

CLJShiftFunction::CLJShiftFunction(const Space &space, COMBINING_RULES combining_rules)
                 : ConcreteProperty<CLJShiftFunction,CLJCutoffFunction>(space, combining_rules)
{}

CLJShiftFunction::CLJShiftFunction(const Space &space, Length cutoff,
                                   COMBINING_RULES combining_rules)
                 : ConcreteProperty<CLJShiftFunction,CLJCutoffFunction>(
                                   space, cutoff, combining_rules)
{}

CLJShiftFunction::CLJShiftFunction(const Space &space, Length coul_cutoff, Length lj_cutoff,
                                   COMBINING_RULES combining_rules)
                 : ConcreteProperty<CLJShiftFunction,CLJCutoffFunction>(
                                   space, coul_cutoff, lj_cutoff, combining_rules)
{}

/** Copy constructor */
CLJShiftFunction::CLJShiftFunction(const CLJShiftFunction &other)
                 : ConcreteProperty<CLJShiftFunction,CLJCutoffFunction>(other)
{}

/** Destructor */
CLJShiftFunction::~CLJShiftFunction()
{}

/** Copy assignment operator */
CLJShiftFunction& CLJShiftFunction::operator=(const CLJShiftFunction &other)
{
    CLJCutoffFunction::operator=(other);
    return *this;
}

/** Comparison operator */
bool CLJShiftFunction::operator==(const CLJShiftFunction &other) const
{
    return CLJCutoffFunction::operator==(other);
}

/** Comparison operator */
bool CLJShiftFunction::operator!=(const CLJShiftFunction &other) const
{
    return not operator==(other);
}

const char* CLJShiftFunction::typeName()
{
    return QMetaType::typeName( qMetaTypeId<CLJShiftFunction>() );
}

const char* CLJShiftFunction::what() const
{
    return CLJShiftFunction::typeName();
}

CLJShiftFunction* CLJShiftFunction::clone() const
{
    return new CLJShiftFunction(*this);
}

/** Calculate the coulomb and LJ intermolecular energy of all of the atoms in 'atoms',
    returning the results in the arguments 'cnrg' and 'ljnrg' */
void CLJShiftFunction::calcVacEnergyGeo(const CLJAtoms &atoms,
                                        double &cnrg, double &ljnrg) const
{
    const MultiFloat *xa = atoms.x().constData();
    const MultiFloat *ya = atoms.y().constData();
    const MultiFloat *za = atoms.z().constData();
    const MultiFloat *qa = atoms.q().constData();
    const MultiFloat *siga = atoms.sigma().constData();
    const MultiFloat *epsa = atoms.epsilon().constData();
    const MultiInt *ida = atoms.ID().constData();

    const MultiFloat Rc(coul_cutoff);
    const MultiFloat Rlj(lj_cutoff);

    const MultiFloat one_over_Rc( 1.0 / coul_cutoff );
    const MultiFloat one_over_Rc2( 1.0 / (coul_cutoff*coul_cutoff) );
    const MultiFloat half(0.5);
    const MultiInt dummy_id = CLJAtoms::idOfDummy();
    const qint32 dummy_int = dummy_id[0];

    MultiFloat tmp, r, one_over_r, sig2_over_r2, sig6_over_r6;
    MultiDouble icnrg(0), iljnrg(0);
    MultiInt itmp;

    int n = atoms.x().count();
    
    for (int i=0; i<n; ++i)
    {
        for (int ii=0; ii<MultiFloat::size(); ++ii)
        {
            if (ida[i][ii] != dummy_int)
            {
                const MultiInt id( ida[i][ii] );
                const MultiFloat x( xa[i][ii] );
                const MultiFloat y( ya[i][ii] );
                const MultiFloat z( za[i][ii] );
                
                if (qa[i][ii] != 0)
                {
                    const MultiFloat q( qa[i][ii] );
                    
                    if (epsa[i][ii] == 0)
                    {
                        //coulomb calculation only
                        for (int j=i; j<n; ++j)
                        {
                            // if i == j then we double-calculate the energies, so must
                            // scale them by 0.5
                            const MultiFloat scale( i == j ? 0.5 : 1.0 );
                        
                            //calculate the distance between the fixed and mobile atoms
                            tmp = xa[j] - x;
                            r = tmp * tmp;
                            tmp = ya[j] - y;
                            r.multiplyAdd(tmp, tmp);
                            tmp = za[j] - z;
                            r.multiplyAdd(tmp, tmp);
                            r = r.sqrt();

                            one_over_r = r.reciprocal();
                    
                            //calculate the coulomb energy using shift-electrostatics
                            // energy = q0q1 * { 1/r - 1/Rc + 1/Rc^2 [r - Rc] }
                            tmp = r - Rc;
                            tmp *= one_over_Rc2;
                            tmp -= one_over_Rc;
                            tmp += one_over_r;
                            tmp *= q * qa[j];
                        
                            //apply the cutoff - compare r against Rc. This will
                            //return 1 if r is less than Rc, or 0 otherwise. Logical
                            //and will then remove all energies where r >= Rc
                            tmp &= r.compareLess(Rc);

                            //make sure that the ID of atoms1 is not zero, and is
                            //also not the same as the atoms0.
                            itmp = ida[j].compareEqual(dummy_id);
                            itmp |= ida[j].compareEqual(id);
                            
                            icnrg += scale * tmp.logicalAndNot(itmp);
                        }
                    }
                    else
                    {
                        //calculate both coulomb and LJ
                        const MultiFloat sig( siga[i][ii] );
                        const MultiFloat eps( epsa[i][ii] );

                        for (int j=i; j<n; ++j)
                        {
                            // if i == j then we double-calculate the energies, so must
                            // scale them by 0.5
                            const MultiFloat scale( i == j ? 0.5 : 1.0 );
                        
                            //calculate the distance between the fixed and mobile atoms
                            tmp = xa[j] - x;
                            r = tmp * tmp;
                            tmp = ya[j] - y;
                            r.multiplyAdd(tmp, tmp);
                            tmp = za[j] - z;
                            r.multiplyAdd(tmp, tmp);
                            r = r.sqrt();

                            one_over_r = r.reciprocal();
                    
                            //calculate the coulomb energy using shift-electrostatics
                            // energy = q0q1 * { 1/r - 1/Rc + 1/Rc^2 [r - Rc] }
                            tmp = r - Rc;
                            tmp *= one_over_Rc2;
                            tmp -= one_over_Rc;
                            tmp += one_over_r;
                            tmp *= q * qa[j];
                        
                            //apply the cutoff - compare r against Rc. This will
                            //return 1 if r is less than Rc, or 0 otherwise. Logical
                            //and will then remove all energies where r >= Rc
                            tmp &= r.compareLess(Rc);

                            //make sure that the ID of atoms1 is not zero, and is
                            //also not the same as the atoms0.
                            itmp = ida[j].compareEqual(dummy_id);
                            itmp |= ida[j].compareEqual(id);
                            
                            icnrg += scale * tmp.logicalAndNot(itmp);

                            //now the LJ energy
                            sig2_over_r2 = sig * siga[j] * one_over_r;
                            sig2_over_r2 = sig2_over_r2*sig2_over_r2;
                            sig6_over_r6 = sig2_over_r2*sig2_over_r2;
                            sig6_over_r6 = sig6_over_r6*sig2_over_r2;

                            tmp = sig6_over_r6 * sig6_over_r6;
                            tmp -= sig6_over_r6;
                            tmp *= eps;
                            tmp *= epsa[j];
                        
                            //apply the cutoff - compare r against Rlj. This will
                            //return 1 if r is less than Rlj, or 0 otherwise. Logical
                            //and will then remove all energies where r >= Rlj
                            tmp &= r.compareLess(Rlj);
                            iljnrg += scale * tmp.logicalAndNot(itmp);
                        }
                    }
                }
                else
                {
                    //LJ calculation only
                    const MultiFloat sig( siga[i][ii] );
                    const MultiFloat eps( epsa[i][ii] );

                    for (int j=i; j<n; ++j)
                    {
                        // if i == j then we double-calculate the energies, so must
                        // scale them by 0.5
                        const MultiFloat scale( i == j ? 0.5 : 1.0 );
                    
                        //calculate the distance between the fixed and mobile atoms
                        tmp = xa[j] - x;
                        r = tmp * tmp;
                        tmp = ya[j] - y;
                        r.multiplyAdd(tmp, tmp);
                        tmp = za[j] - z;
                        r.multiplyAdd(tmp, tmp);
                        r = r.sqrt();

                        one_over_r = r.reciprocal();

                        sig2_over_r2 = sig * siga[j] * one_over_r;
                        sig2_over_r2 = sig2_over_r2*sig2_over_r2;
                        sig6_over_r6 = sig2_over_r2*sig2_over_r2;
                        sig6_over_r6 = sig6_over_r6*sig2_over_r2;

                        tmp = sig6_over_r6 * sig6_over_r6;
                        tmp -= sig6_over_r6;
                        tmp *= eps;
                        tmp *= epsa[j];
                    
                        //apply the cutoff - compare r against Rlj. This will
                        //return 1 if r is less than Rlj, or 0 otherwise. Logical
                        //and will then remove all energies where r >= Rlj
                        tmp &= r.compareLess(Rlj);
                        iljnrg += scale * tmp.logicalAndNot(itmp);
                    }
                }
            }
        }
    }
    
    cnrg = icnrg.sum();
    ljnrg = iljnrg.sum();
}

/** Calculate the intermolecular energy between all atoms in 'atoms0' and all
    atoms in 'atoms1', returning the result in the arguments 'cnrg' and 'ljnrg' */
void CLJShiftFunction::calcVacEnergyGeo(const CLJAtoms &atoms0, const CLJAtoms &atoms1,
                                        double &cnrg, double &ljnrg, float min_distance) const
{
    const MultiFloat *x0 = atoms0.x().constData();
    const MultiFloat *y0 = atoms0.y().constData();
    const MultiFloat *z0 = atoms0.z().constData();
    const MultiFloat *q0 = atoms0.q().constData();
    const MultiFloat *sig0 = atoms0.sigma().constData();
    const MultiFloat *eps0 = atoms0.epsilon().constData();
    const MultiInt *id0 = atoms0.ID().constData();

    const MultiFloat *x1 = atoms1.x().constData();
    const MultiFloat *y1 = atoms1.y().constData();
    const MultiFloat *z1 = atoms1.z().constData();
    const MultiFloat *q1 = atoms1.q().constData();
    const MultiFloat *sig1 = atoms1.sigma().constData();
    const MultiFloat *eps1 = atoms1.epsilon().constData();
    const MultiInt *id1 = atoms1.ID().constData();
    
    const MultiFloat Rc(coul_cutoff);
    const MultiFloat Rlj(lj_cutoff);
    const MultiFloat one_over_Rc( 1.0 / coul_cutoff );
    const MultiFloat one_over_Rc2( 1.0 / (coul_cutoff*coul_cutoff) );
    const MultiFloat half(0.5);
    const MultiInt dummy_id = CLJAtoms::idOfDummy();
    const qint32 dummy_int = dummy_id[0];

    MultiFloat tmp, r, one_over_r, sig2_over_r2, sig6_over_r6;
    MultiDouble icnrg(0), iljnrg(0);
    MultiInt itmp;

    const int n0 = atoms0.x().count();
    const int n1 = atoms1.x().count();

    for (int i=0; i<n0; ++i)
    {
        for (int ii=0; ii<MultiFloat::count(); ++ii)
        {
            if (id0[i][ii] != dummy_int)
            {
                const MultiInt id(id0[i][ii]);
            
                if (q0[i][ii] != 0)
                {
                    const MultiFloat x(x0[i][ii]);
                    const MultiFloat y(y0[i][ii]);
                    const MultiFloat z(z0[i][ii]);
                    const MultiFloat q(q0[i][ii]);

                    if (eps0[i][ii] == 0)
                    {
                        //coulomb energy only
                        for (int j=0; j<n1; ++j)
                        {
                            //calculate the distance between the fixed and mobile atoms
                            tmp = x1[j] - x;
                            r = tmp * tmp;
                            tmp = y1[j] - y;
                            r.multiplyAdd(tmp, tmp);
                            tmp = z1[j] - z;
                            r.multiplyAdd(tmp, tmp);
                            r = r.sqrt();

                            one_over_r = r.reciprocal();
                    
                            //calculate the coulomb energy using shift-electrostatics
                            // energy = q0q1 * { 1/r - 1/Rc + 1/Rc^2 [r - Rc] }
                            tmp = r - Rc;
                            tmp *= one_over_Rc2;
                            tmp -= one_over_Rc;
                            tmp += one_over_r;
                            tmp *= q * q1[j];
                        
                            //apply the cutoff - compare r against Rc. This will
                            //return 1 if r is less than Rc, or 0 otherwise. Logical
                            //and will then remove all energies where r >= Rc
                            tmp &= r.compareLess(Rc);

                            //make sure that the ID of atoms1 is not zero, and is
                            //also not the same as the atoms0.
                            itmp = id1[j].compareEqual(dummy_id);
                            itmp |= id1[j].compareEqual(id);
                            
                            icnrg += tmp.logicalAndNot(itmp);
                        }
                    }
                    else
                    {
                        const MultiFloat sig(sig0[i][ii]);
                        const MultiFloat eps(eps0[i][ii]);

                        for (int j=0; j<n1; ++j)
                        {
                            //calculate the distance between the fixed and mobile atoms
                            tmp = x1[j] - x;
                            r = tmp * tmp;
                            tmp = y1[j] - y;
                            r.multiplyAdd(tmp, tmp);
                            tmp = z1[j] - z;
                            r.multiplyAdd(tmp, tmp);
                            r = r.sqrt();

                            one_over_r = r.reciprocal();
                    
                            //calculate the coulomb energy using shift-electrostatics
                            // energy = q0q1 * { 1/r - 1/Rc + 1/Rc^2 [r - Rc] }
                            tmp = r - Rc;
                            tmp *= one_over_Rc2;
                            tmp -= one_over_Rc;
                            tmp += one_over_r;
                            tmp *= q * q1[j];
                        
                            //apply the cutoff - compare r against Rc. This will
                            //return 1 if r is less than Rc, or 0 otherwise. Logical
                            //and will then remove all energies where r >= Rc
                            tmp &= r.compareLess(Rc);

                            //make sure that the ID of atoms1 is not zero, and is
                            //also not the same as the atoms0.
                            //logical and will remove all energies where id1 == 0 or id0 == id1
                            itmp = id1[j].compareEqual(dummy_id);
                            itmp |= id1[j].compareEqual(id);

                            icnrg += tmp.logicalAndNot(itmp);
                            
                            //Now do the LJ energy

                            //arithmetic combining rules
                            sig2_over_r2 = sig * sig1[j] * one_over_r;
                            sig2_over_r2 = sig2_over_r2*sig2_over_r2;
                            sig6_over_r6 = sig2_over_r2*sig2_over_r2;
                            sig6_over_r6 = sig6_over_r6*sig2_over_r2;

                            tmp = sig6_over_r6 * sig6_over_r6;
                            tmp -= sig6_over_r6;
                            tmp *= eps;
                            tmp *= eps1[j];
                        
                            //apply the cutoff - compare r against Rlj. This will
                            //return 1 if r is less than Rlj, or 0 otherwise. Logical
                            //and will then remove all energies where r >= Rlj
                            tmp &= r.compareLess(Rlj);
                            iljnrg += tmp.logicalAndNot(itmp);
                        }
                    }
                }
                else if (eps0[i][ii] != 0)
                {
                    //LJ energy only
                    const MultiFloat x(x0[i][ii]);
                    const MultiFloat y(y0[i][ii]);
                    const MultiFloat z(z0[i][ii]);
                    const MultiFloat sig(sig0[i][ii]);
                    const MultiFloat eps(eps0[i][ii]);

                    for (int j=0; j<n1; ++j)
                    {
                        //calculate the distance between the fixed and mobile atoms
                        tmp = x1[j] - x;
                        r = tmp * tmp;
                        tmp = y1[j] - y;
                        r.multiplyAdd(tmp, tmp);
                        tmp = z1[j] - z;
                        r.multiplyAdd(tmp, tmp);
                        r = r.sqrt();

                        one_over_r = r.reciprocal();
                
                        //arithmetic combining rules
                        sig2_over_r2 = sig * sig1[j] * one_over_r;
                        sig2_over_r2 = sig2_over_r2*sig2_over_r2;
                        sig6_over_r6 = sig2_over_r2*sig2_over_r2;
                        sig6_over_r6 = sig6_over_r6*sig2_over_r2;

                        tmp = sig6_over_r6 * sig6_over_r6;
                        tmp -= sig6_over_r6;
                        tmp *= eps;
                        tmp *= eps1[j];
                    
                        //apply the cutoff - compare r against Rlj. This will
                        //return 1 if r is less than Rlj, or 0 otherwise. Logical
                        //and will then remove all energies where r >= Rlj
                        tmp &= r.compareLess(Rlj);
                        itmp = id1[j].compareEqual(dummy_id);
                        itmp |= id1[j].compareEqual(id);

                        iljnrg += tmp.logicalAndNot(itmp);
                    }
                }
            }
        }
    }
    
    cnrg = icnrg.sum();
    ljnrg = iljnrg.sum();
}

/** Calculate the coulomb and LJ intermolecular energy of all of the atoms in 'atoms',
    assuming periodic boundary conditions in a cubic box of size 'box_dimensions',
    returning the results in 'cnrg' and 'ljnrg' */
void CLJShiftFunction::calcBoxEnergyGeo(const CLJAtoms &atoms, const Vector &box_dimensions,
                                        double &cnrg, double &ljnrg) const
{
    const MultiFloat *xa = atoms.x().constData();
    const MultiFloat *ya = atoms.y().constData();
    const MultiFloat *za = atoms.z().constData();
    const MultiFloat *qa = atoms.q().constData();
    const MultiFloat *siga = atoms.sigma().constData();
    const MultiFloat *epsa = atoms.epsilon().constData();
    const MultiInt *ida = atoms.ID().constData();

    const MultiFloat Rc(coul_cutoff);
    const MultiFloat Rlj(lj_cutoff);

    const MultiFloat one_over_Rc( 1.0 / coul_cutoff );
    const MultiFloat one_over_Rc2( 1.0 / (coul_cutoff*coul_cutoff) );
    const MultiFloat half(0.5);
    const MultiInt dummy_id = CLJAtoms::idOfDummy();
    const qint32 dummy_int = dummy_id[0];

    MultiFloat tmp, r, one_over_r, sig2_over_r2, sig6_over_r6;
    MultiDouble icnrg(0), iljnrg(0);
    MultiInt itmp;

    const MultiFloat box_x( box_dimensions.x() );
    const MultiFloat box_y( box_dimensions.y() );
    const MultiFloat box_z( box_dimensions.z() );
    
    const MultiFloat half_box_x( 0.5 * box_dimensions.x() );
    const MultiFloat half_box_y( 0.5 * box_dimensions.y() );
    const MultiFloat half_box_z( 0.5 * box_dimensions.z() );

    int n = atoms.x().count();
    
    for (int i=0; i<n; ++i)
    {
        for (int ii=0; ii<MultiFloat::size(); ++ii)
        {
            if (ida[i][ii] != dummy_int)
            {
                const MultiInt id( ida[i][ii] );
                const MultiFloat x( xa[i][ii] );
                const MultiFloat y( ya[i][ii] );
                const MultiFloat z( za[i][ii] );
                const MultiFloat q( qa[i][ii] );
                const MultiFloat sig( siga[i][ii] );
                const MultiFloat eps( epsa[i][ii] );

                for (int j=i; j<n; ++j)
                {
                    // if i == j then we double-calculate the energies, so must
                    // scale them by 0.5
                    const MultiFloat scale( i == j ? 0.5 : 1.0 );
                
                    tmp = xa[j] - x;
                    tmp &= MULTIFLOAT_POS_MASK;  // this creates the absolute value :-)
                    tmp -= box_x.logicalAnd( half_box_x.compareLess(tmp) );
                    r = tmp * tmp;

                    tmp = ya[j] - y;
                    tmp &= MULTIFLOAT_POS_MASK;  // this creates the absolute value :-)
                    tmp -= box_y.logicalAnd( half_box_y.compareLess(tmp) );
                    r.multiplyAdd(tmp, tmp);

                    tmp = za[j] - z;
                    tmp &= MULTIFLOAT_POS_MASK;  // this creates the absolute value :-)
                    tmp -= box_z.logicalAnd( half_box_z.compareLess(tmp) );
                    r.multiplyAdd(tmp, tmp);

                    r = r.sqrt();

                    one_over_r = r.reciprocal();
            
                    //calculate the coulomb energy using shift-electrostatics
                    // energy = q0q1 * { 1/r - 1/Rc + 1/Rc^2 [r - Rc] }
                    tmp = r - Rc;
                    tmp *= one_over_Rc2;
                    tmp -= one_over_Rc;
                    tmp += one_over_r;
                    tmp *= q * qa[j];
                
                    //apply the cutoff - compare r against Rc. This will
                    //return 1 if r is less than Rc, or 0 otherwise. Logical
                    //and will then remove all energies where r >= Rc
                    tmp &= r.compareLess(Rc);

                    //make sure that the ID of atoms1 is not zero, and is
                    //also not the same as the atoms0.
                    itmp = ida[j].compareEqual(dummy_id);
                    itmp |= ida[j].compareEqual(id);
                    
                    icnrg += scale * tmp.logicalAndNot(itmp);

                    //now the LJ energy
                    sig2_over_r2 = sig * siga[j] * one_over_r;
                    sig2_over_r2 = sig2_over_r2*sig2_over_r2;
                    sig6_over_r6 = sig2_over_r2*sig2_over_r2;
                    sig6_over_r6 = sig6_over_r6*sig2_over_r2;

                    tmp = sig6_over_r6 * sig6_over_r6;
                    tmp -= sig6_over_r6;
                    tmp *= eps;
                    tmp *= epsa[j];
                
                    //apply the cutoff - compare r against Rlj. This will
                    //return 1 if r is less than Rlj, or 0 otherwise. Logical
                    //and will then remove all energies where r >= Rlj
                    tmp &= r.compareLess(Rlj);
                    iljnrg += scale * tmp.logicalAndNot(itmp);
                }
            }
        }
    }
    
    cnrg = icnrg.sum();
    ljnrg = iljnrg.sum();
}

/** Calculate the intermolecular energy between all atoms in 'atoms0' and all
    atoms in 'atoms1', assuming periodic boundary conditions in a cubic box
    of size 'box_dimensions, returning the result in the arguments 'cnrg' and 'ljnrg' */
void CLJShiftFunction::calcBoxEnergyGeo(const CLJAtoms &atoms0, const CLJAtoms &atoms1,
                                        const Vector &box_dimensions,
                                        double &cnrg, double &ljnrg, float min_distance) const
{
    const MultiFloat *x0 = atoms0.x().constData();
    const MultiFloat *y0 = atoms0.y().constData();
    const MultiFloat *z0 = atoms0.z().constData();
    const MultiFloat *q0 = atoms0.q().constData();
    const MultiFloat *sig0 = atoms0.sigma().constData();
    const MultiFloat *eps0 = atoms0.epsilon().constData();
    const MultiInt *id0 = atoms0.ID().constData();

    const MultiFloat *x1 = atoms1.x().constData();
    const MultiFloat *y1 = atoms1.y().constData();
    const MultiFloat *z1 = atoms1.z().constData();
    const MultiFloat *q1 = atoms1.q().constData();
    const MultiFloat *sig1 = atoms1.sigma().constData();
    const MultiFloat *eps1 = atoms1.epsilon().constData();
    const MultiInt *id1 = atoms1.ID().constData();
    
    const MultiFloat Rc(coul_cutoff);
    const MultiFloat Rlj(lj_cutoff);
    const MultiFloat one_over_Rc( 1.0 / coul_cutoff );
    const MultiFloat one_over_Rc2( 1.0 / (coul_cutoff*coul_cutoff) );
    const MultiFloat half(0.5);
    const MultiInt dummy_id = CLJAtoms::idOfDummy();
    const qint32 dummy_int = dummy_id[0];

    MultiFloat tmp, r, one_over_r, sig2_over_r2, sig6_over_r6;
    MultiDouble icnrg(0), iljnrg(0);
    MultiInt itmp;

    const MultiFloat box_x( box_dimensions.x() );
    const MultiFloat box_y( box_dimensions.y() );
    const MultiFloat box_z( box_dimensions.z() );
    
    const MultiFloat half_box_x( 0.5 * box_dimensions.x() );
    const MultiFloat half_box_y( 0.5 * box_dimensions.y() );
    const MultiFloat half_box_z( 0.5 * box_dimensions.z() );

    const int n0 = atoms0.x().count();
    const int n1 = atoms1.x().count();

    for (int i=0; i<n0; ++i)
    {
        for (int ii=0; ii<MultiFloat::count(); ++ii)
        {
            if (id0[i][ii] != dummy_int)
            {
                const MultiInt id(id0[i][ii]);
                const MultiFloat x(x0[i][ii]);
                const MultiFloat y(y0[i][ii]);
                const MultiFloat z(z0[i][ii]);
                const MultiFloat q(q0[i][ii]);

                const MultiFloat sig(sig0[i][ii]);
                const MultiFloat eps(eps0[i][ii]);

                for (int j=0; j<n1; ++j)
                {
                    tmp = x1[j] - x;
                    tmp &= MULTIFLOAT_POS_MASK;  // this creates the absolute value :-)
                    tmp -= box_x.logicalAnd( half_box_x.compareLess(tmp) );
                    r = tmp * tmp;

                    tmp = y1[j] - y;
                    tmp &= MULTIFLOAT_POS_MASK;  // this creates the absolute value :-)
                    tmp -= box_y.logicalAnd( half_box_y.compareLess(tmp) );
                    r.multiplyAdd(tmp, tmp);

                    tmp = z1[j] - z;
                    tmp &= MULTIFLOAT_POS_MASK;  // this creates the absolute value :-)
                    tmp -= box_z.logicalAnd( half_box_z.compareLess(tmp) );
                    r.multiplyAdd(tmp, tmp);
                    
                    r = r.sqrt();

                    one_over_r = r.reciprocal();
            
                    //calculate the coulomb energy using shift-electrostatics
                    // energy = q0q1 * { 1/r - 1/Rc + 1/Rc^2 [r - Rc] }
                    tmp = r - Rc;
                    tmp *= one_over_Rc2;
                    tmp -= one_over_Rc;
                    tmp += one_over_r;
                    tmp *= q * q1[j];
                    
                    //apply the cutoff - compare r against Rc. This will
                    //return 1 if r is less than Rc, or 0 otherwise. Logical
                    //and will then remove all energies where r >= Rc
                    tmp &= r.compareLess(Rc);

                    //make sure that the ID of atoms1 is not zero, and is
                    //also not the same as the atoms0.
                    //logical and will remove all energies where id1 == 0 or id0 == id1
                    itmp = id1[j].compareEqual(dummy_id);
                    itmp |= id1[j].compareEqual(id);

                    icnrg += tmp.logicalAndNot(itmp);
                    
                    //Now do the LJ energy

                    //arithmetic combining rules
                    sig2_over_r2 = sig * sig1[j] * one_over_r;
                    sig2_over_r2 = sig2_over_r2*sig2_over_r2;
                    sig6_over_r6 = sig2_over_r2*sig2_over_r2;
                    sig6_over_r6 = sig6_over_r6*sig2_over_r2;

                    tmp = sig6_over_r6 * sig6_over_r6;
                    tmp -= sig6_over_r6;
                    tmp *= eps;
                    tmp *= eps1[j];
                
                    //apply the cutoff - compare r against Rlj. This will
                    //return 1 if r is less than Rlj, or 0 otherwise. Logical
                    //and will then remove all energies where r >= Rlj
                    tmp &= r.compareLess(Rlj);
                    iljnrg += tmp.logicalAndNot(itmp);
                }
            }
        }
    }
    
    cnrg = icnrg.sum();
    ljnrg = iljnrg.sum();
}

/** Calculate the coulomb and LJ intermolecular energy of all of the atoms in 'atoms',
    returning the results in the arguments 'cnrg' and 'ljnrg' */
void CLJShiftFunction::calcVacEnergyAri(const CLJAtoms &atoms,
                                        double &cnrg, double &ljnrg) const
{
    const MultiFloat *xa = atoms.x().constData();
    const MultiFloat *ya = atoms.y().constData();
    const MultiFloat *za = atoms.z().constData();
    const MultiFloat *qa = atoms.q().constData();
    const MultiFloat *siga = atoms.sigma().constData();
    const MultiFloat *epsa = atoms.epsilon().constData();
    const MultiInt *ida = atoms.ID().constData();

    const MultiFloat Rc(coul_cutoff);
    const MultiFloat Rlj(lj_cutoff);

    const MultiFloat one_over_Rc( 1.0 / coul_cutoff );
    const MultiFloat one_over_Rc2( 1.0 / (coul_cutoff*coul_cutoff) );
    const MultiFloat half(0.5);
    const MultiInt dummy_id = CLJAtoms::idOfDummy();
    const qint32 dummy_int = dummy_id[0];

    MultiFloat tmp, r, one_over_r, sig2_over_r2, sig6_over_r6;
    MultiDouble icnrg(0), iljnrg(0);
    MultiInt itmp;

    int n = atoms.x().count();
    
    for (int i=0; i<n; ++i)
    {
        for (int ii=0; ii<MultiFloat::size(); ++ii)
        {
            if (ida[i][ii] != dummy_int)
            {
                const MultiInt id( ida[i][ii] );
                const MultiFloat x( xa[i][ii] );
                const MultiFloat y( ya[i][ii] );
                const MultiFloat z( za[i][ii] );
                
                if (qa[i][ii] != 0)
                {
                    const MultiFloat q( qa[i][ii] );
                    
                    if (epsa[i][ii] == 0)
                    {
                        //coulomb calculation only
                        for (int j=i; j<n; ++j)
                        {
                            // if i == j then we double-calculate the energies, so must
                            // scale them by 0.5
                            const MultiFloat scale( i == j ? 0.5 : 1.0 );
                        
                            //calculate the distance between the fixed and mobile atoms
                            tmp = xa[j] - x;
                            r = tmp * tmp;
                            tmp = ya[j] - y;
                            r.multiplyAdd(tmp, tmp);
                            tmp = za[j] - z;
                            r.multiplyAdd(tmp, tmp);
                            r = r.sqrt();

                            one_over_r = r.reciprocal();
                    
                            //calculate the coulomb energy using shift-electrostatics
                            // energy = q0q1 * { 1/r - 1/Rc + 1/Rc^2 [r - Rc] }
                            tmp = r - Rc;
                            tmp *= one_over_Rc2;
                            tmp -= one_over_Rc;
                            tmp += one_over_r;
                            tmp *= q * qa[j];
                        
                            //apply the cutoff - compare r against Rc. This will
                            //return 1 if r is less than Rc, or 0 otherwise. Logical
                            //and will then remove all energies where r >= Rc
                            tmp &= r.compareLess(Rc);

                            //make sure that the ID of atoms1 is not zero, and is
                            //also not the same as the atoms0.
                            itmp = ida[j].compareEqual(dummy_id);
                            itmp |= ida[j].compareEqual(id);
                            
                            icnrg += scale * tmp.logicalAndNot(itmp);
                        }
                    }
                    else
                    {
                        //calculate both coulomb and LJ
                        const MultiFloat sig( siga[i][ii] * siga[i][ii] );
                        const MultiFloat eps( epsa[i][ii] );

                        for (int j=i; j<n; ++j)
                        {
                            // if i == j then we double-calculate the energies, so must
                            // scale them by 0.5
                            const MultiFloat scale( i == j ? 0.5 : 1.0 );
                        
                            //calculate the distance between the fixed and mobile atoms
                            tmp = xa[j] - x;
                            r = tmp * tmp;
                            tmp = ya[j] - y;
                            r.multiplyAdd(tmp, tmp);
                            tmp = za[j] - z;
                            r.multiplyAdd(tmp, tmp);
                            r = r.sqrt();

                            one_over_r = r.reciprocal();
                    
                            //calculate the coulomb energy using shift-electrostatics
                            // energy = q0q1 * { 1/r - 1/Rc + 1/Rc^2 [r - Rc] }
                            tmp = r - Rc;
                            tmp *= one_over_Rc2;
                            tmp -= one_over_Rc;
                            tmp += one_over_r;
                            tmp *= q * qa[j];
                        
                            //apply the cutoff - compare r against Rc. This will
                            //return 1 if r is less than Rc, or 0 otherwise. Logical
                            //and will then remove all energies where r >= Rc
                            tmp &= r.compareLess(Rc);

                            //make sure that the ID of atoms1 is not zero, and is
                            //also not the same as the atoms0.
                            itmp = ida[j].compareEqual(dummy_id);
                            itmp |= ida[j].compareEqual(id);
                            
                            icnrg += scale * tmp.logicalAndNot(itmp);

                            //now the LJ energy
                            tmp = sig + (siga[j]*siga[j]);
                            tmp *= half;
                        
                            sig2_over_r2 = tmp * one_over_r;
                            sig2_over_r2 = sig2_over_r2*sig2_over_r2;
                            sig6_over_r6 = sig2_over_r2*sig2_over_r2;
                            sig6_over_r6 = sig6_over_r6*sig2_over_r2;

                            tmp = sig6_over_r6 * sig6_over_r6;
                            tmp -= sig6_over_r6;
                            tmp *= eps;
                            tmp *= epsa[j];
                        
                            //apply the cutoff - compare r against Rlj. This will
                            //return 1 if r is less than Rlj, or 0 otherwise. Logical
                            //and will then remove all energies where r >= Rlj
                            tmp &= r.compareLess(Rlj);
                            iljnrg += scale * tmp.logicalAndNot(itmp);
                        }
                    }
                }
                else
                {
                    //LJ calculation only
                    const MultiFloat sig( siga[i][ii] * siga[i][ii] );
                    const MultiFloat eps( epsa[i][ii] );

                    for (int j=i; j<n; ++j)
                    {
                        // if i == j then we double-calculate the energies, so must
                        // scale them by 0.5
                        const MultiFloat scale( i == j ? 0.5 : 1.0 );
                    
                        //calculate the distance between the fixed and mobile atoms
                        tmp = xa[j] - x;
                        r = tmp * tmp;
                        tmp = ya[j] - y;
                        r.multiplyAdd(tmp, tmp);
                        tmp = za[j] - z;
                        r.multiplyAdd(tmp, tmp);
                        r = r.sqrt();

                        one_over_r = r.reciprocal();

                        tmp = sig + (siga[j]*siga[j]);
                        tmp *= half;
                    
                        sig2_over_r2 = tmp * one_over_r;
                        sig2_over_r2 = sig2_over_r2*sig2_over_r2;
                        sig6_over_r6 = sig2_over_r2*sig2_over_r2;
                        sig6_over_r6 = sig6_over_r6*sig2_over_r2;

                        tmp = sig6_over_r6 * sig6_over_r6;
                        tmp -= sig6_over_r6;
                        tmp *= eps;
                        tmp *= epsa[j];
                    
                        //apply the cutoff - compare r against Rlj. This will
                        //return 1 if r is less than Rlj, or 0 otherwise. Logical
                        //and will then remove all energies where r >= Rlj
                        tmp &= r.compareLess(Rlj);
                        iljnrg += scale * tmp.logicalAndNot(itmp);
                    }
                }
            }
        }
    }
    
    cnrg = icnrg.sum();
    ljnrg = iljnrg.sum();
}

/** Calculate the intermolecular energy between all atoms in 'atoms0' and all
    atoms in 'atoms1', returning the result in the arguments 'cnrg' and 'ljnrg' */
void CLJShiftFunction::calcVacEnergyAri(const CLJAtoms &atoms0, const CLJAtoms &atoms1,
                                        double &cnrg, double &ljnrg, float min_distance) const
{
    const MultiFloat *x0 = atoms0.x().constData();
    const MultiFloat *y0 = atoms0.y().constData();
    const MultiFloat *z0 = atoms0.z().constData();
    const MultiFloat *q0 = atoms0.q().constData();
    const MultiFloat *sig0 = atoms0.sigma().constData();
    const MultiFloat *eps0 = atoms0.epsilon().constData();
    const MultiInt *id0 = atoms0.ID().constData();

    const MultiFloat *x1 = atoms1.x().constData();
    const MultiFloat *y1 = atoms1.y().constData();
    const MultiFloat *z1 = atoms1.z().constData();
    const MultiFloat *q1 = atoms1.q().constData();
    const MultiFloat *sig1 = atoms1.sigma().constData();
    const MultiFloat *eps1 = atoms1.epsilon().constData();
    const MultiInt *id1 = atoms1.ID().constData();
    
    const MultiFloat Rc(coul_cutoff);
    const MultiFloat Rlj(lj_cutoff);
    const MultiFloat one_over_Rc( 1.0 / coul_cutoff );
    const MultiFloat one_over_Rc2( 1.0 / (coul_cutoff*coul_cutoff) );
    const MultiFloat half(0.5);
    const MultiInt dummy_id = CLJAtoms::idOfDummy();
    const qint32 dummy_int = dummy_id[0];

    MultiFloat tmp, r, one_over_r, sig2_over_r2, sig6_over_r6;
    MultiDouble icnrg(0), iljnrg(0);
    MultiInt itmp;

    const int n0 = atoms0.x().count();
    const int n1 = atoms1.x().count();

    for (int i=0; i<n0; ++i)
    {
        for (int ii=0; ii<MultiFloat::count(); ++ii)
        {
            if (id0[i][ii] != dummy_int)
            {
                const MultiInt id(id0[i][ii]);
            
                if (q0[i][ii] != 0)
                {
                    const MultiFloat x(x0[i][ii]);
                    const MultiFloat y(y0[i][ii]);
                    const MultiFloat z(z0[i][ii]);
                    const MultiFloat q(q0[i][ii]);

                    if (eps0[i][ii] == 0)
                    {
                        //coulomb energy only
                        for (int j=0; j<n1; ++j)
                        {
                            //calculate the distance between the fixed and mobile atoms
                            tmp = x1[j] - x;
                            r = tmp * tmp;
                            tmp = y1[j] - y;
                            r.multiplyAdd(tmp, tmp);
                            tmp = z1[j] - z;
                            r.multiplyAdd(tmp, tmp);
                            r = r.sqrt();

                            one_over_r = r.reciprocal();
                    
                            //calculate the coulomb energy using shift-electrostatics
                            // energy = q0q1 * { 1/r - 1/Rc + 1/Rc^2 [r - Rc] }
                            tmp = r - Rc;
                            tmp *= one_over_Rc2;
                            tmp -= one_over_Rc;
                            tmp += one_over_r;
                            tmp *= q * q1[j];
                        
                            //apply the cutoff - compare r against Rc. This will
                            //return 1 if r is less than Rc, or 0 otherwise. Logical
                            //and will then remove all energies where r >= Rc
                            tmp &= r.compareLess(Rc);

                            //make sure that the ID of atoms1 is not zero, and is
                            //also not the same as the atoms0.
                            itmp = id1[j].compareEqual(dummy_id);
                            itmp |= id1[j].compareEqual(id);
                            
                            icnrg += tmp.logicalAndNot(itmp);
                        }
                    }
                    else
                    {
                        const MultiFloat sig(sig0[i][ii] * sig0[i][ii]);
                        const MultiFloat eps(eps0[i][ii]);

                        for (int j=0; j<n1; ++j)
                        {
                            //calculate the distance between the fixed and mobile atoms
                            tmp = x1[j] - x;
                            r = tmp * tmp;
                            tmp = y1[j] - y;
                            r.multiplyAdd(tmp, tmp);
                            tmp = z1[j] - z;
                            r.multiplyAdd(tmp, tmp);
                            r = r.sqrt();

                            one_over_r = r.reciprocal();
                    
                            //calculate the coulomb energy using shift-electrostatics
                            // energy = q0q1 * { 1/r - 1/Rc + 1/Rc^2 [r - Rc] }
                            tmp = r - Rc;
                            tmp *= one_over_Rc2;
                            tmp -= one_over_Rc;
                            tmp += one_over_r;
                            tmp *= q * q1[j];
                        
                            //apply the cutoff - compare r against Rc. This will
                            //return 1 if r is less than Rc, or 0 otherwise. Logical
                            //and will then remove all energies where r >= Rc
                            tmp &= r.compareLess(Rc);

                            //make sure that the ID of atoms1 is not zero, and is
                            //also not the same as the atoms0.
                            //logical and will remove all energies where id1 == 0 or id0 == id1
                            itmp = id1[j].compareEqual(dummy_id);
                            itmp |= id1[j].compareEqual(id);

                            icnrg += tmp.logicalAndNot(itmp);
                            
                            //Now do the LJ energy

                            //arithmetic combining rules
                            tmp = sig + (sig1[j]*sig1[j]);
                            tmp *= half;
                        
                            sig2_over_r2 = tmp * one_over_r;
                            sig2_over_r2 = sig2_over_r2*sig2_over_r2;
                            sig6_over_r6 = sig2_over_r2*sig2_over_r2;
                            sig6_over_r6 = sig6_over_r6*sig2_over_r2;

                            tmp = sig6_over_r6 * sig6_over_r6;
                            tmp -= sig6_over_r6;
                            tmp *= eps;
                            tmp *= eps1[j];
                        
                            //apply the cutoff - compare r against Rlj. This will
                            //return 1 if r is less than Rlj, or 0 otherwise. Logical
                            //and will then remove all energies where r >= Rlj
                            tmp &= r.compareLess(Rlj);
                            iljnrg += tmp.logicalAndNot(itmp);
                        }
                    }
                }
                else if (eps0[i][ii] != 0)
                {
                    //LJ energy only
                    const MultiFloat x(x0[i][ii]);
                    const MultiFloat y(y0[i][ii]);
                    const MultiFloat z(z0[i][ii]);
                    const MultiFloat sig(sig0[i][ii] * sig0[i][ii]);
                    const MultiFloat eps(eps0[i][ii]);

                    for (int j=0; j<n1; ++j)
                    {
                        //calculate the distance between the fixed and mobile atoms
                        tmp = x1[j] - x;
                        r = tmp * tmp;
                        tmp = y1[j] - y;
                        r.multiplyAdd(tmp, tmp);
                        tmp = z1[j] - z;
                        r.multiplyAdd(tmp, tmp);
                        r = r.sqrt();

                        one_over_r = r.reciprocal();
                
                        //arithmetic combining rules
                        tmp = sig + (sig1[j]*sig1[j]);
                        tmp *= half;
                    
                        sig2_over_r2 = tmp * one_over_r;
                        sig2_over_r2 = sig2_over_r2*sig2_over_r2;
                        sig6_over_r6 = sig2_over_r2*sig2_over_r2;
                        sig6_over_r6 = sig6_over_r6*sig2_over_r2;

                        tmp = sig6_over_r6 * sig6_over_r6;
                        tmp -= sig6_over_r6;
                        tmp *= eps;
                        tmp *= eps1[j];
                    
                        //apply the cutoff - compare r against Rlj. This will
                        //return 1 if r is less than Rlj, or 0 otherwise. Logical
                        //and will then remove all energies where r >= Rlj
                        tmp &= r.compareLess(Rlj);
                        itmp = id1[j].compareEqual(dummy_id);
                        itmp |= id1[j].compareEqual(id);

                        iljnrg += tmp.logicalAndNot(itmp);
                    }
                }
            }
        }
    }
    
    cnrg = icnrg.sum();
    ljnrg = iljnrg.sum();
}

/** Calculate the coulomb intermolecular energy of all atoms in 'atoms' */
double CLJShiftFunction::calcVacCoulombEnergyAri(const CLJAtoms &atoms) const
{
    const MultiFloat *xa = atoms.x().constData();
    const MultiFloat *ya = atoms.y().constData();
    const MultiFloat *za = atoms.z().constData();
    const MultiFloat *qa = atoms.q().constData();
    const MultiInt *ida = atoms.ID().constData();

    const MultiFloat Rc(coul_cutoff);

    const MultiFloat one_over_Rc( 1.0 / coul_cutoff );
    const MultiFloat one_over_Rc2( 1.0 / (coul_cutoff*coul_cutoff) );
    const MultiFloat half(0.5);
    const MultiInt dummy_id = CLJAtoms::idOfDummy();
    const qint32 dummy_int = dummy_id[0];

    MultiFloat tmp, r, one_over_r;
    MultiDouble icnrg(0);
    MultiInt itmp;

    int n = atoms.x().count();
    
    for (int i=0; i<n; ++i)
    {
        for (int ii=0; ii<MultiFloat::size(); ++ii)
        {
            if (ida[i][ii] != dummy_int and qa[i][ii] != 0)
            {
                const MultiInt id( ida[i][ii] );
                const MultiFloat x( xa[i][ii] );
                const MultiFloat y( ya[i][ii] );
                const MultiFloat z( za[i][ii] );
                const MultiFloat q( qa[i][ii] );
                
                //coulomb calculation only
                for (int j=i; j<n; ++j)
                {
                    // if i == j then we double-calculate the energies, so must
                    // scale them by 0.5
                    const MultiFloat scale( i == j ? 0.5 : 1.0 );
                
                    //calculate the distance between the fixed and mobile atoms
                    tmp = xa[j] - x;
                    r = tmp * tmp;
                    tmp = ya[j] - y;
                    r.multiplyAdd(tmp, tmp);
                    tmp = za[j] - z;
                    r.multiplyAdd(tmp, tmp);
                    r = r.sqrt();

                    one_over_r = r.reciprocal();
            
                    //calculate the coulomb energy using shift-electrostatics
                    // energy = q0q1 * { 1/r - 1/Rc + 1/Rc^2 [r - Rc] }
                    tmp = r - Rc;
                    tmp *= one_over_Rc2;
                    tmp -= one_over_Rc;
                    tmp += one_over_r;
                    tmp *= q * qa[j];
                
                    //apply the cutoff - compare r against Rc. This will
                    //return 1 if r is less than Rc, or 0 otherwise. Logical
                    //and will then remove all energies where r >= Rc
                    tmp &= r.compareLess(Rc);

                    //make sure that the ID of atoms1 is not zero, and is
                    //also not the same as the atoms0.
                    itmp = ida[j].compareEqual(dummy_id);
                    itmp |= ida[j].compareEqual(id);
                    
                    icnrg += scale * tmp.logicalAndNot(itmp);
                }
            }
        }
    }
    
    return icnrg.sum();
}

/** Calculate the coulomb intermolecular energy between all atoms in 'atoms0'
    and 'atoms1' */
double CLJShiftFunction::calcVacCoulombEnergyAri(const CLJAtoms &atoms0, const CLJAtoms &atoms1,
                                                 float min_distance) const
{
    const MultiFloat *x0 = atoms0.x().constData();
    const MultiFloat *y0 = atoms0.y().constData();
    const MultiFloat *z0 = atoms0.z().constData();
    const MultiFloat *q0 = atoms0.q().constData();
    const MultiInt *id0 = atoms0.ID().constData();

    const MultiFloat *x1 = atoms1.x().constData();
    const MultiFloat *y1 = atoms1.y().constData();
    const MultiFloat *z1 = atoms1.z().constData();
    const MultiFloat *q1 = atoms1.q().constData();
    const MultiInt *id1 = atoms1.ID().constData();
    
    const MultiFloat Rc(coul_cutoff);
    const MultiFloat one_over_Rc( 1.0 / coul_cutoff );
    const MultiFloat one_over_Rc2( 1.0 / (coul_cutoff*coul_cutoff) );
    const MultiFloat half(0.5);
    const MultiInt dummy_id = CLJAtoms::idOfDummy();
    const qint32 dummy_int = dummy_id[0];

    MultiFloat tmp, r, one_over_r;
    MultiDouble icnrg(0);
    MultiInt itmp;

    const int n0 = atoms0.x().count();
    const int n1 = atoms1.x().count();

    for (int i=0; i<n0; ++i)
    {
        for (int ii=0; ii<MultiFloat::count(); ++ii)
        {
            if (id0[i][ii] != dummy_int and q0[i][ii] != 0)
            {
                const MultiInt id(id0[i][ii]);
                const MultiFloat x(x0[i][ii]);
                const MultiFloat y(y0[i][ii]);
                const MultiFloat z(z0[i][ii]);
                const MultiFloat q(q0[i][ii]);

                //coulomb energy only
                for (int j=0; j<n1; ++j)
                {
                    //calculate the distance between the fixed and mobile atoms
                    tmp = x1[j] - x;
                    r = tmp * tmp;
                    tmp = y1[j] - y;
                    r.multiplyAdd(tmp, tmp);
                    tmp = z1[j] - z;
                    r.multiplyAdd(tmp, tmp);
                    r = r.sqrt();

                    one_over_r = r.reciprocal();
            
                    //calculate the coulomb energy using shift-electrostatics
                    // energy = q0q1 * { 1/r - 1/Rc + 1/Rc^2 [r - Rc] }
                    tmp = r - Rc;
                    tmp *= one_over_Rc2;
                    tmp -= one_over_Rc;
                    tmp += one_over_r;
                    tmp *= q * q1[j];
                
                    //apply the cutoff - compare r against Rc. This will
                    //return 1 if r is less than Rc, or 0 otherwise. Logical
                    //and will then remove all energies where r >= Rc
                    tmp &= r.compareLess(Rc);

                    //make sure that the ID of atoms1 is not zero, and is
                    //also not the same as the atoms0.
                    itmp = id1[j].compareEqual(dummy_id);
                    itmp |= id1[j].compareEqual(id);
                    
                    icnrg += tmp.logicalAndNot(itmp);
                }
            }
        }
    }
    
    return icnrg.sum();
}
    
/** Calculate the LJ intermolecular energy of all atoms in 'atoms' */
double CLJShiftFunction::calcVacLJEnergyAri(const CLJAtoms &atoms) const
{
    const MultiFloat *xa = atoms.x().constData();
    const MultiFloat *ya = atoms.y().constData();
    const MultiFloat *za = atoms.z().constData();
    const MultiFloat *siga = atoms.sigma().constData();
    const MultiFloat *epsa = atoms.epsilon().constData();
    const MultiInt *ida = atoms.ID().constData();

    const MultiFloat Rlj(lj_cutoff);

    const MultiFloat half(0.5);
    const MultiInt dummy_id = CLJAtoms::idOfDummy();
    const qint32 dummy_int = dummy_id[0];

    MultiFloat tmp, r, one_over_r, sig2_over_r2, sig6_over_r6;
    MultiDouble iljnrg(0);
    MultiInt itmp;

    int n = atoms.x().count();
    
    for (int i=0; i<n; ++i)
    {
        for (int ii=0; ii<MultiFloat::size(); ++ii)
        {
            if (ida[i][ii] != dummy_int and epsa[i][ii] != 0)
            {
                //LJ calculation only
                const MultiInt id( ida[i][ii] );
                const MultiFloat x( xa[i][ii] );
                const MultiFloat y( ya[i][ii] );
                const MultiFloat z( za[i][ii] );
                const MultiFloat sig( siga[i][ii] * siga[i][ii] );
                const MultiFloat eps( epsa[i][ii] );

                for (int j=i; j<n; ++j)
                {
                    // if i == j then we double-calculate the energies, so must
                    // scale them by 0.5
                    const MultiFloat scale( i == j ? 0.5 : 1.0 );
                
                    //calculate the distance between the fixed and mobile atoms
                    tmp = xa[j] - x;
                    r = tmp * tmp;
                    tmp = ya[j] - y;
                    r.multiplyAdd(tmp, tmp);
                    tmp = za[j] - z;
                    r.multiplyAdd(tmp, tmp);
                    r = r.sqrt();

                    one_over_r = r.reciprocal();

                    tmp = sig + (siga[j]*siga[j]);
                    tmp *= half;
                
                    sig2_over_r2 = tmp * one_over_r;
                    sig2_over_r2 = sig2_over_r2*sig2_over_r2;
                    sig6_over_r6 = sig2_over_r2*sig2_over_r2;
                    sig6_over_r6 = sig6_over_r6*sig2_over_r2;

                    tmp = sig6_over_r6 * sig6_over_r6;
                    tmp -= sig6_over_r6;
                    tmp *= eps;
                    tmp *= epsa[j];
                
                    //apply the cutoff - compare r against Rlj. This will
                    //return 1 if r is less than Rlj, or 0 otherwise. Logical
                    //and will then remove all energies where r >= Rlj
                    tmp &= r.compareLess(Rlj);
                    iljnrg += scale * tmp.logicalAndNot(itmp);
                }
            }
        }
    }
    
    return iljnrg.sum();
}

/** Calculate the LJ intermolecular energy between all atoms in 'atoms0'
    and 'atoms1' */
double CLJShiftFunction::calcVacLJEnergyAri(const CLJAtoms &atoms0, const CLJAtoms &atoms1,
                                            float min_distance) const
{
    const MultiFloat *x0 = atoms0.x().constData();
    const MultiFloat *y0 = atoms0.y().constData();
    const MultiFloat *z0 = atoms0.z().constData();
    const MultiFloat *sig0 = atoms0.sigma().constData();
    const MultiFloat *eps0 = atoms0.epsilon().constData();
    const MultiInt *id0 = atoms0.ID().constData();

    const MultiFloat *x1 = atoms1.x().constData();
    const MultiFloat *y1 = atoms1.y().constData();
    const MultiFloat *z1 = atoms1.z().constData();
    const MultiFloat *sig1 = atoms1.sigma().constData();
    const MultiFloat *eps1 = atoms1.epsilon().constData();
    const MultiInt *id1 = atoms1.ID().constData();
    
    const MultiFloat Rlj(lj_cutoff);
    const MultiFloat half(0.5);
    const MultiInt dummy_id = CLJAtoms::idOfDummy();
    const qint32 dummy_int = dummy_id[0];

    MultiFloat tmp, r, one_over_r, sig2_over_r2, sig6_over_r6;
    MultiDouble iljnrg(0);
    MultiInt itmp;

    const int n0 = atoms0.x().count();
    const int n1 = atoms1.x().count();

    for (int i=0; i<n0; ++i)
    {
        for (int ii=0; ii<MultiFloat::count(); ++ii)
        {
            if (id0[i][ii] != dummy_int and eps0[i][ii] != 0)
            {
                //LJ energy only
                const MultiFloat x(x0[i][ii]);
                const MultiFloat y(y0[i][ii]);
                const MultiFloat z(z0[i][ii]);
                const MultiInt id(id0[i][ii]);
                const MultiFloat sig(sig0[i][ii] * sig0[i][ii]);
                const MultiFloat eps(eps0[i][ii]);

                for (int j=0; j<n1; ++j)
                {
                    //calculate the distance between the fixed and mobile atoms
                    tmp = x1[j] - x;
                    r = tmp * tmp;
                    tmp = y1[j] - y;
                    r.multiplyAdd(tmp, tmp);
                    tmp = z1[j] - z;
                    r.multiplyAdd(tmp, tmp);
                    r = r.sqrt();

                    one_over_r = r.reciprocal();
            
                    //arithmetic combining rules
                    tmp = sig + (sig1[j]*sig1[j]);
                    tmp *= half;
                
                    sig2_over_r2 = tmp * one_over_r;
                    sig2_over_r2 = sig2_over_r2*sig2_over_r2;
                    sig6_over_r6 = sig2_over_r2*sig2_over_r2;
                    sig6_over_r6 = sig6_over_r6*sig2_over_r2;

                    tmp = sig6_over_r6 * sig6_over_r6;
                    tmp -= sig6_over_r6;
                    tmp *= eps;
                    tmp *= eps1[j];
                
                    //apply the cutoff - compare r against Rlj. This will
                    //return 1 if r is less than Rlj, or 0 otherwise. Logical
                    //and will then remove all energies where r >= Rlj
                    tmp &= r.compareLess(Rlj);
                    itmp = id1[j].compareEqual(dummy_id);
                    itmp |= id1[j].compareEqual(id);

                    iljnrg += tmp.logicalAndNot(itmp);
                }
            }
        }
    }
    
    return iljnrg.sum();
}

/** Calculate the coulomb and LJ intermolecular energy of all of the atoms in 'atoms',
    assuming periodic boundary conditions in a cubic box of size 'box_dimensions',
    returning the results in 'cnrg' and 'ljnrg' */
void CLJShiftFunction::calcBoxEnergyAri(const CLJAtoms &atoms, const Vector &box_dimensions,
                                        double &cnrg, double &ljnrg) const
{
    const MultiFloat *xa = atoms.x().constData();
    const MultiFloat *ya = atoms.y().constData();
    const MultiFloat *za = atoms.z().constData();
    const MultiFloat *qa = atoms.q().constData();
    const MultiFloat *siga = atoms.sigma().constData();
    const MultiFloat *epsa = atoms.epsilon().constData();
    const MultiInt *ida = atoms.ID().constData();

    const MultiFloat Rc(coul_cutoff);
    const MultiFloat Rlj(lj_cutoff);

    const MultiFloat one_over_Rc( 1.0 / coul_cutoff );
    const MultiFloat one_over_Rc2( 1.0 / (coul_cutoff*coul_cutoff) );
    const MultiFloat half(0.5);
    const MultiInt dummy_id = CLJAtoms::idOfDummy();
    const qint32 dummy_int = dummy_id[0];

    MultiFloat tmp, r, one_over_r, sig2_over_r2, sig6_over_r6;
    MultiDouble icnrg(0), iljnrg(0);
    MultiInt itmp;

    const MultiFloat box_x( box_dimensions.x() );
    const MultiFloat box_y( box_dimensions.y() );
    const MultiFloat box_z( box_dimensions.z() );
    
    const MultiFloat half_box_x( 0.5 * box_dimensions.x() );
    const MultiFloat half_box_y( 0.5 * box_dimensions.y() );
    const MultiFloat half_box_z( 0.5 * box_dimensions.z() );

    int n = atoms.x().count();
    
    for (int i=0; i<n; ++i)
    {
        for (int ii=0; ii<MultiFloat::size(); ++ii)
        {
            if (ida[i][ii] != dummy_int)
            {
                const MultiInt id( ida[i][ii] );
                const MultiFloat x( xa[i][ii] );
                const MultiFloat y( ya[i][ii] );
                const MultiFloat z( za[i][ii] );
                const MultiFloat q( qa[i][ii] );
                const MultiFloat sig( siga[i][ii] * siga[i][ii] );
                const MultiFloat eps( epsa[i][ii] );

                for (int j=i; j<n; ++j)
                {
                    // if i == j then we double-calculate the energies, so must
                    // scale them by 0.5
                    const MultiFloat scale( i == j ? 0.5 : 1.0 );
                
                    tmp = xa[j] - x;
                    tmp &= MULTIFLOAT_POS_MASK;  // this creates the absolute value :-)
                    tmp -= box_x.logicalAnd( half_box_x.compareLess(tmp) );
                    r = tmp * tmp;

                    tmp = ya[j] - y;
                    tmp &= MULTIFLOAT_POS_MASK;  // this creates the absolute value :-)
                    tmp -= box_y.logicalAnd( half_box_y.compareLess(tmp) );
                    r.multiplyAdd(tmp, tmp);

                    tmp = za[j] - z;
                    tmp &= MULTIFLOAT_POS_MASK;  // this creates the absolute value :-)
                    tmp -= box_z.logicalAnd( half_box_z.compareLess(tmp) );
                    r.multiplyAdd(tmp, tmp);

                    r = r.sqrt();

                    one_over_r = r.reciprocal();
            
                    //calculate the coulomb energy using shift-electrostatics
                    // energy = q0q1 * { 1/r - 1/Rc + 1/Rc^2 [r - Rc] }
                    tmp = r - Rc;
                    tmp *= one_over_Rc2;
                    tmp -= one_over_Rc;
                    tmp += one_over_r;
                    tmp *= q * qa[j];
                
                    //apply the cutoff - compare r against Rc. This will
                    //return 1 if r is less than Rc, or 0 otherwise. Logical
                    //and will then remove all energies where r >= Rc
                    tmp &= r.compareLess(Rc);

                    //make sure that the ID of atoms1 is not zero, and is
                    //also not the same as the atoms0.
                    itmp = ida[j].compareEqual(dummy_id);
                    itmp |= ida[j].compareEqual(id);
                    
                    icnrg += scale * tmp.logicalAndNot(itmp);

                    //now the LJ energy
                    tmp = sig + (siga[j]*siga[j]);
                    tmp *= half;
                
                    sig2_over_r2 = tmp * one_over_r;
                    sig2_over_r2 = sig2_over_r2*sig2_over_r2;
                    sig6_over_r6 = sig2_over_r2*sig2_over_r2;
                    sig6_over_r6 = sig6_over_r6*sig2_over_r2;

                    tmp = sig6_over_r6 * sig6_over_r6;
                    tmp -= sig6_over_r6;
                    tmp *= eps;
                    tmp *= epsa[j];
                
                    //apply the cutoff - compare r against Rlj. This will
                    //return 1 if r is less than Rlj, or 0 otherwise. Logical
                    //and will then remove all energies where r >= Rlj
                    tmp &= r.compareLess(Rlj);
                    iljnrg += scale * tmp.logicalAndNot(itmp);
                }
            }
        }
    }
    
    cnrg = icnrg.sum();
    ljnrg = iljnrg.sum();
}

/** Calculate the intermolecular energy between all atoms in 'atoms0' and all
    atoms in 'atoms1', assuming periodic boundary conditions in a cubic box
    of size 'box_dimensions, returning the result in the arguments 'cnrg' and 'ljnrg' */
void CLJShiftFunction::calcBoxEnergyAri(const CLJAtoms &atoms0, const CLJAtoms &atoms1,
                                        const Vector &box_dimensions,
                                        double &cnrg, double &ljnrg, float min_distance) const
{
    const MultiFloat *x0 = atoms0.x().constData();
    const MultiFloat *y0 = atoms0.y().constData();
    const MultiFloat *z0 = atoms0.z().constData();
    const MultiFloat *q0 = atoms0.q().constData();
    const MultiFloat *sig0 = atoms0.sigma().constData();
    const MultiFloat *eps0 = atoms0.epsilon().constData();
    const MultiInt *id0 = atoms0.ID().constData();

    const MultiFloat *x1 = atoms1.x().constData();
    const MultiFloat *y1 = atoms1.y().constData();
    const MultiFloat *z1 = atoms1.z().constData();
    const MultiFloat *q1 = atoms1.q().constData();
    const MultiFloat *sig1 = atoms1.sigma().constData();
    const MultiFloat *eps1 = atoms1.epsilon().constData();
    const MultiInt *id1 = atoms1.ID().constData();
    
    const MultiFloat Rc(coul_cutoff);
    const MultiFloat Rlj(lj_cutoff);
    const MultiFloat one_over_Rc( 1.0 / coul_cutoff );
    const MultiFloat one_over_Rc2( 1.0 / (coul_cutoff*coul_cutoff) );
    const MultiFloat half(0.5);
    const MultiInt dummy_id = CLJAtoms::idOfDummy();
    const qint32 dummy_int = dummy_id[0];

    MultiFloat tmp, r, one_over_r, sig2_over_r2, sig6_over_r6;
    MultiDouble icnrg(0), iljnrg(0);
    MultiInt itmp;

    const MultiFloat box_x( box_dimensions.x() );
    const MultiFloat box_y( box_dimensions.y() );
    const MultiFloat box_z( box_dimensions.z() );
    
    const MultiFloat half_box_x( 0.5 * box_dimensions.x() );
    const MultiFloat half_box_y( 0.5 * box_dimensions.y() );
    const MultiFloat half_box_z( 0.5 * box_dimensions.z() );

    const int n0 = atoms0.x().count();
    const int n1 = atoms1.x().count();

    for (int i=0; i<n0; ++i)
    {
        for (int ii=0; ii<MultiFloat::count(); ++ii)
        {
            if (id0[i][ii] != dummy_int)
            {
                const MultiInt id(id0[i][ii]);
                const MultiFloat x(x0[i][ii]);
                const MultiFloat y(y0[i][ii]);
                const MultiFloat z(z0[i][ii]);
                const MultiFloat q(q0[i][ii]);

                const MultiFloat sig(sig0[i][ii] * sig0[i][ii]);
                const MultiFloat eps(eps0[i][ii]);

                for (int j=0; j<n1; ++j)
                {
                    tmp = x1[j] - x;
                    tmp &= MULTIFLOAT_POS_MASK;  // this creates the absolute value :-)
                    tmp -= box_x.logicalAnd( half_box_x.compareLess(tmp) );
                    r = tmp * tmp;

                    tmp = y1[j] - y;
                    tmp &= MULTIFLOAT_POS_MASK;  // this creates the absolute value :-)
                    tmp -= box_y.logicalAnd( half_box_y.compareLess(tmp) );
                    r.multiplyAdd(tmp, tmp);

                    tmp = z1[j] - z;
                    tmp &= MULTIFLOAT_POS_MASK;  // this creates the absolute value :-)
                    tmp -= box_z.logicalAnd( half_box_z.compareLess(tmp) );
                    r.multiplyAdd(tmp, tmp);
                    
                    r = r.sqrt();

                    one_over_r = r.reciprocal();
            
                    //calculate the coulomb energy using shift-electrostatics
                    // energy = q0q1 * { 1/r - 1/Rc + 1/Rc^2 [r - Rc] }
                    tmp = r - Rc;
                    tmp *= one_over_Rc2;
                    tmp -= one_over_Rc;
                    tmp += one_over_r;
                    tmp *= q * q1[j];
                    
                    //apply the cutoff - compare r against Rc. This will
                    //return 1 if r is less than Rc, or 0 otherwise. Logical
                    //and will then remove all energies where r >= Rc
                    tmp &= r.compareLess(Rc);

                    //make sure that the ID of atoms1 is not zero, and is
                    //also not the same as the atoms0.
                    //logical and will remove all energies where id1 == 0 or id0 == id1
                    itmp = id1[j].compareEqual(dummy_id);
                    itmp |= id1[j].compareEqual(id);

                    icnrg += tmp.logicalAndNot(itmp);
                    
                    //Now do the LJ energy

                    //arithmetic combining rules
                    tmp = sig + (sig1[j]*sig1[j]);
                    tmp *= half;
                
                    sig2_over_r2 = tmp * one_over_r;
                    sig2_over_r2 = sig2_over_r2*sig2_over_r2;
                    sig6_over_r6 = sig2_over_r2*sig2_over_r2;
                    sig6_over_r6 = sig6_over_r6*sig2_over_r2;

                    tmp = sig6_over_r6 * sig6_over_r6;
                    tmp -= sig6_over_r6;
                    tmp *= eps;
                    tmp *= eps1[j];
                
                    //apply the cutoff - compare r against Rlj. This will
                    //return 1 if r is less than Rlj, or 0 otherwise. Logical
                    //and will then remove all energies where r >= Rlj
                    tmp &= r.compareLess(Rlj);
                    iljnrg += tmp.logicalAndNot(itmp);
                }
            }
        }
    }
    
    cnrg = icnrg.sum();
    ljnrg = iljnrg.sum();
}

/** This function does support calculations using a grid */
bool CLJShiftFunction::supportsGridCalculation() const
{
    return true;
}

namespace detail
{
    class CLJShiftVacGridCalculator
    {
    public:
        CLJShiftVacGridCalculator()
                : nats(0), x(0), y(0), z(0), q(0), id(0),
                  gridpot_array(0)
        {}
        
        CLJShiftVacGridCalculator(const GridInfo &_grid_info, int _nats,
                                  const MultiFloat *_x, const MultiFloat *_y,
                                  const MultiFloat *_z, const MultiFloat *_q,
                                  const MultiInt *_id, float coul_cutoff,
                                  double *_gridpot_array)
                : grid_info(_grid_info), nats(_nats), x(_x), y(_y), z(_z), q(_q), id(_id),
                  Rc(coul_cutoff), one_over_Rc( 1.0f / coul_cutoff ),
                  one_over_Rc2( 1.0f / (coul_cutoff*coul_cutoff) ),
                  gridpot_array(_gridpot_array)
        {}
        
        ~CLJShiftVacGridCalculator()
        {}
        
        void operator()(const tbb::blocked_range<int> &range) const
        {
            const MultiInt dummy_id = CLJAtoms::idOfDummy();
            MultiFloat tmp, r, one_over_r, itmp;
        
            for (int i = range.begin(); i != range.end(); ++i)
            {
                const Vector grid_point = grid_info.point(i);
                
                const MultiFloat px(grid_point.x());
                const MultiFloat py(grid_point.y());
                const MultiFloat pz(grid_point.z());
                
                MultiDouble pot(0);
                
                for (int j=0; j<nats; ++j)
                {
                    //calculate the distance between the atom and grid point
                    tmp = px - x[j];
                    r = tmp * tmp;
                    tmp = py - y[j];
                    r.multiplyAdd(tmp, tmp);
                    tmp = pz - z[j];
                    r.multiplyAdd(tmp, tmp);
                    r = r.sqrt();

                    one_over_r = r.reciprocal();
            
                    //calculate the coulomb energy using shift-electrostatics
                    // energy = q0q1 * { 1/r - 1/Rc + 1/Rc^2 [r - Rc] }
                    tmp = r - Rc;
                    tmp *= one_over_Rc2;
                    tmp -= one_over_Rc;
                    tmp += one_over_r;
                    tmp *= q[j];
                
                    //apply the cutoff - compare r against Rc. This will
                    //return 1 if r is less than Rc, or 0 otherwise. Logical
                    //and will then remove all energies where r >= Rc
                    tmp &= r.compareLess(Rc);

                    //make sure that the ID of atoms1 is not zero, and is
                    //also not the same as the atoms0.
                    itmp = id[j].compareEqual(dummy_id);
                    pot += tmp.logicalAndNot(itmp);
                }
                
                gridpot_array[i] = pot.sum();
            }
        }
        
    private:
        const GridInfo grid_info;
        const int nats;
    
        const MultiFloat* const x;
        const MultiFloat* const y;
        const MultiFloat* const z;
        const MultiFloat* const q;
        const MultiInt* id;
        
        const MultiFloat Rc;
        const MultiFloat one_over_Rc;
        const MultiFloat one_over_Rc2;
        
        double *gridpot_array;
    };
}

/** Calculate the energy on the grid from the passed atoms using vacuum boundary conditions */
void CLJShiftFunction::calcVacGrid(const CLJAtoms &atoms, const GridInfo &info,
                                   QVector<double> &gridpot) const
{
    QElapsedTimer t;
    t.start();

    ::detail::CLJShiftVacGridCalculator calc(info, atoms.x().count(), atoms.x().constData(),
                                             atoms.y().constData(), atoms.z().constData(),
                                             atoms.q().constData(), atoms.ID().constData(),
                                             coul_cutoff, gridpot.data());

    tbb::parallel_for(tbb::blocked_range<int>(0,info.nPoints(),4096), calc);
    
    qint64 ns = t.nsecsElapsed();
    qDebug() << "Building the grid took" << (0.000001*ns) << "ms";
    
    double sum = 0;
    
    for (int i=0; i<gridpot.count(); ++i)
    {
        sum += gridpot.constData()[i];
    }
    
    qDebug() << "Sum of grid potentials is" << sum;
}

/** Calculate the energy on the grid from the passed atoms using vacuum boundary conditions */
void CLJShiftFunction::calcBoxGrid(const CLJAtoms &atoms, const GridInfo &info,
                                   QVector<double> &gridpot) const
{
}

/////////
///////// Implementation of CLJIntraShiftFunction
/////////

static const RegisterMetaType<CLJIntraShiftFunction> r_intra;

QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const CLJIntraShiftFunction &intra)
{
    writeHeader(ds, r_intra, 1);
    
    ds << static_cast<const CLJIntraFunction&>(intra);
    
    return ds;
}

QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, CLJIntraShiftFunction &intra)
{
    VersionID v = readHeader(ds, r_intra);
    
    if (v == 1)
    {
        ds >> static_cast<CLJIntraFunction&>(intra);
    }
    else
        throw version_error(v, "1", r_intra, CODELOC);
    
    return ds;
}

CLJIntraShiftFunction::CLJIntraShiftFunction()
                      : ConcreteProperty<CLJIntraShiftFunction,CLJIntraFunction>()
{}

CLJIntraShiftFunction::CLJIntraShiftFunction(Length cutoff)
                      : ConcreteProperty<CLJIntraShiftFunction,CLJIntraFunction>(cutoff)
{}

CLJIntraShiftFunction::CLJIntraShiftFunction(Length coul_cutoff, Length lj_cutoff)
                      : ConcreteProperty<CLJIntraShiftFunction,CLJIntraFunction>(
                            coul_cutoff, lj_cutoff)
{}

CLJIntraShiftFunction::CLJIntraShiftFunction(const Space &space, Length cutoff)
                      : ConcreteProperty<CLJIntraShiftFunction,CLJIntraFunction>(
                            space, cutoff)
{}

CLJIntraShiftFunction::CLJIntraShiftFunction(const Space &space,
                                             Length coul_cutoff, Length lj_cutoff)
                      : ConcreteProperty<CLJIntraShiftFunction,CLJIntraFunction>(
                            space, coul_cutoff, lj_cutoff)
{}

CLJIntraShiftFunction::CLJIntraShiftFunction(Length cutoff, COMBINING_RULES combining_rules)
                      : ConcreteProperty<CLJIntraShiftFunction,CLJIntraFunction>(
                            cutoff, combining_rules)
{}

CLJIntraShiftFunction::CLJIntraShiftFunction(Length coul_cutoff, Length lj_cutoff,
                                             COMBINING_RULES combining_rules)
                 : ConcreteProperty<CLJIntraShiftFunction,CLJIntraFunction>(
                            coul_cutoff, lj_cutoff, combining_rules)
{}

CLJIntraShiftFunction::CLJIntraShiftFunction(const Space &space, COMBINING_RULES combining_rules)
                 : ConcreteProperty<CLJIntraShiftFunction,CLJIntraFunction>(
                            space, combining_rules)
{}

CLJIntraShiftFunction::CLJIntraShiftFunction(const Space &space, Length cutoff,
                                             COMBINING_RULES combining_rules)
                 : ConcreteProperty<CLJIntraShiftFunction,CLJIntraFunction>(
                            space, cutoff, combining_rules)
{}

CLJIntraShiftFunction::CLJIntraShiftFunction(const Space &space, Length coul_cutoff,
                                             Length lj_cutoff,
                                             COMBINING_RULES combining_rules)
                 : ConcreteProperty<CLJIntraShiftFunction,CLJIntraFunction>(
                            space, coul_cutoff, lj_cutoff, combining_rules)
{}

/** Copy constructor */
CLJIntraShiftFunction::CLJIntraShiftFunction(const CLJIntraShiftFunction &other)
                 : ConcreteProperty<CLJIntraShiftFunction,CLJIntraFunction>(other)
{}

/** Destructor */
CLJIntraShiftFunction::~CLJIntraShiftFunction()
{}

/** Copy assignment operator */
CLJIntraShiftFunction& CLJIntraShiftFunction::operator=(const CLJIntraShiftFunction &other)
{
    CLJIntraFunction::operator=(other);
    return *this;
}

/** Comparison operator */
bool CLJIntraShiftFunction::operator==(const CLJIntraShiftFunction &other) const
{
    return CLJIntraFunction::operator==(other);
}

/** Comparison operator */
bool CLJIntraShiftFunction::operator!=(const CLJIntraShiftFunction &other) const
{
    return not operator==(other);
}

const char* CLJIntraShiftFunction::typeName()
{
    return QMetaType::typeName( qMetaTypeId<CLJIntraShiftFunction>() );
}

const char* CLJIntraShiftFunction::what() const
{
    return CLJIntraShiftFunction::typeName();
}

CLJIntraShiftFunction* CLJIntraShiftFunction::clone() const
{
    return new CLJIntraShiftFunction(*this);
}

/** Calculate the coulomb and LJ intramolecular energy of all of the atoms in 'atoms',
    returning the results in the arguments 'cnrg' and 'ljnrg'.
    Note that all of the atoms must be part of the same molecule, and must
    have their intramolecular non-bonded scale factors loaded into this function */
void CLJIntraShiftFunction::calcVacEnergyGeo(const CLJAtoms &atoms,
                                             double &cnrg, double &ljnrg) const
{
    const MultiFloat *xa = atoms.x().constData();
    const MultiFloat *ya = atoms.y().constData();
    const MultiFloat *za = atoms.z().constData();
    const MultiFloat *qa = atoms.q().constData();
    const MultiFloat *siga = atoms.sigma().constData();
    const MultiFloat *epsa = atoms.epsilon().constData();
    const MultiInt *ida = atoms.ID().constData();

    const MultiFloat Rc(coul_cutoff);
    const MultiFloat Rlj(lj_cutoff);

    const MultiFloat one_over_Rc( 1.0 / coul_cutoff );
    const MultiFloat one_over_Rc2( 1.0 / (coul_cutoff*coul_cutoff) );
    const MultiFloat half(0.5);
    const MultiInt dummy_id = CLJAtoms::idOfDummy();
    const qint32 dummy_int = dummy_id[0];

    MultiFloat tmp, r, one_over_r, sig2_over_r2, sig6_over_r6;
    MultiFloat bond_mask;
    MultiDouble icnrg(0), iljnrg(0);
    MultiInt itmp;

    int n = atoms.x().count();
    
    for (int i=0; i<n; ++i)
    {
        for (int ii=0; ii<MultiFloat::size(); ++ii)
        {
            if (ida[i][ii] != dummy_int)
            {
                const MultiInt id( ida[i][ii] );
                const MultiFloat x( xa[i][ii] );
                const MultiFloat y( ya[i][ii] );
                const MultiFloat z( za[i][ii] );
                const MultiFloat q( qa[i][ii] );
                const MultiFloat sig( siga[i][ii] );
                const MultiFloat eps( epsa[i][ii] );

                const bool *row = bondMatrix().constData()[id[0]].constData();

                for (int j=i; j<n; ++j)
                {
                    // if i == j then we double-calculate the energies, so must
                    // scale them by 0.5
                    MultiFloat scale( i == j ? 0.5 : 1.0 );
                
                    //get the bond mask to screen out bonded interactions
                    for (int k=0; k<MultiFloat::count(); ++k)
                    {
                        bond_mask.quickSet(k, row[ida[j][k]] ? 0.0 : 1.0);
                    }
                
                    scale *= bond_mask;
                
                    //calculate the distance between the fixed and mobile atoms
                    tmp = xa[j] - x;
                    r = tmp * tmp;
                    tmp = ya[j] - y;
                    r.multiplyAdd(tmp, tmp);
                    tmp = za[j] - z;
                    r.multiplyAdd(tmp, tmp);
                    r = r.sqrt();

                    one_over_r = r.reciprocal();
            
                    //calculate the coulomb energy using shift-electrostatics
                    // energy = q0q1 * { 1/r - 1/Rc + 1/Rc^2 [r - Rc] }
                    tmp = r - Rc;
                    tmp *= one_over_Rc2;
                    tmp -= one_over_Rc;
                    tmp += one_over_r;
                    tmp *= q * qa[j];
                
                    //apply the cutoff - compare r against Rc. This will
                    //return 1 if r is less than Rc, or 0 otherwise. Logical
                    //and will then remove all energies where r >= Rc
                    tmp &= r.compareLess(Rc);

                    //make sure that the ID of atoms1 is not zero, and is
                    //also not the same as the atoms0 and that we are not
                    //including the energy of the atom with itself
                    itmp = ida[j].compareEqual(dummy_id);
                    itmp |= ida[j].compareEqual(id);

                    icnrg += scale * tmp.logicalAndNot(itmp);

                    //now the LJ energy
                    sig2_over_r2 = sig * siga[j] * one_over_r;
                    sig2_over_r2 = sig2_over_r2*sig2_over_r2;
                    sig6_over_r6 = sig2_over_r2*sig2_over_r2;
                    sig6_over_r6 = sig6_over_r6*sig2_over_r2;

                    tmp = sig6_over_r6 * sig6_over_r6;
                    tmp -= sig6_over_r6;
                    tmp *= eps;
                    tmp *= epsa[j];
                
                    //apply the cutoff - compare r against Rlj. This will
                    //return 1 if r is less than Rlj, or 0 otherwise. Logical
                    //and will then remove all energies where r >= Rlj
                    tmp &= r.compareLess(Rlj);
                    iljnrg += scale * tmp.logicalAndNot(itmp);
                }
            }
        }
    }
    
    cnrg = icnrg.sum();
    ljnrg = iljnrg.sum();
}

/** Calculate the intramolecular energy between all atoms in 'atoms0' and all
    atoms in 'atoms1', returning the result in the arguments 'cnrg' and 'ljnrg'.
    Note that all of the atoms must be part of the same molecule, and must
    have their intramolecular non-bonded scale factors loaded into this function */
void CLJIntraShiftFunction::calcVacEnergyGeo(const CLJAtoms &atoms0, const CLJAtoms &atoms1,
                                             double &cnrg, double &ljnrg, float min_distance) const
{
    const MultiFloat *x0 = atoms0.x().constData();
    const MultiFloat *y0 = atoms0.y().constData();
    const MultiFloat *z0 = atoms0.z().constData();
    const MultiFloat *q0 = atoms0.q().constData();
    const MultiFloat *sig0 = atoms0.sigma().constData();
    const MultiFloat *eps0 = atoms0.epsilon().constData();
    const MultiInt *id0 = atoms0.ID().constData();

    const MultiFloat *x1 = atoms1.x().constData();
    const MultiFloat *y1 = atoms1.y().constData();
    const MultiFloat *z1 = atoms1.z().constData();
    const MultiFloat *q1 = atoms1.q().constData();
    const MultiFloat *sig1 = atoms1.sigma().constData();
    const MultiFloat *eps1 = atoms1.epsilon().constData();
    const MultiInt *id1 = atoms1.ID().constData();
    
    const MultiFloat Rc(coul_cutoff);
    const MultiFloat Rlj(lj_cutoff);
    const MultiFloat one_over_Rc( 1.0 / coul_cutoff );
    const MultiFloat one_over_Rc2( 1.0 / (coul_cutoff*coul_cutoff) );
    const MultiFloat half(0.5);
    const MultiInt dummy_id = CLJAtoms::idOfDummy();
    const qint32 dummy_int = dummy_id[0];

    MultiFloat tmp, r, one_over_r, sig2_over_r2, sig6_over_r6;
    MultiDouble icnrg(0), iljnrg(0);
    MultiInt itmp;

    const int n0 = atoms0.x().count();
    const int n1 = atoms1.x().count();
    
    bool not_bonded = true;
    
    if (min_distance < 5.5)
    {
        not_bonded = isNotBonded(atoms0.ID(), atoms1.ID());
    }
    
    if ( Q_LIKELY(not_bonded) )
    {
        for (int i=0; i<n0; ++i)
        {
            for (int ii=0; ii<MultiFloat::count(); ++ii)
            {
                if (id0[i][ii] != dummy_int)
                {
                    const MultiInt id(id0[i][ii]);
                    const MultiFloat x(x0[i][ii]);
                    const MultiFloat y(y0[i][ii]);
                    const MultiFloat z(z0[i][ii]);
                    const MultiFloat q(q0[i][ii]);

                    const MultiFloat sig(sig0[i][ii]);
                    const MultiFloat eps(eps0[i][ii]);

                    for (int j=0; j<n1; ++j)
                    {
                        //calculate the distance between the fixed and mobile atoms
                        tmp = x1[j] - x;
                        r = tmp * tmp;
                        tmp = y1[j] - y;
                        r.multiplyAdd(tmp, tmp);
                        tmp = z1[j] - z;
                        r.multiplyAdd(tmp, tmp);
                        r = r.sqrt();

                        one_over_r = r.reciprocal();
                
                        //calculate the coulomb energy using shift-electrostatics
                        // energy = q0q1 * { 1/r - 1/Rc + 1/Rc^2 [r - Rc] }
                        tmp = r - Rc;
                        tmp *= one_over_Rc2;
                        tmp -= one_over_Rc;
                        tmp += one_over_r;
                        tmp *= q * q1[j];
                    
                        //apply the cutoff - compare r against Rc. This will
                        //return 1 if r is less than Rc, or 0 otherwise. Logical
                        //and will then remove all energies where r >= Rc
                        tmp &= r.compareLess(Rc);

                        //make sure that the ID of atoms1 is not zero
                        itmp = id1[j].compareEqual(dummy_id);

                        icnrg += tmp.logicalAndNot(itmp);
                        
                        //Now do the LJ energy

                        sig2_over_r2 = sig * sig1[j] * one_over_r;
                        sig2_over_r2 = sig2_over_r2*sig2_over_r2;
                        sig6_over_r6 = sig2_over_r2*sig2_over_r2;
                        sig6_over_r6 = sig6_over_r6*sig2_over_r2;

                        tmp = sig6_over_r6 * sig6_over_r6;
                        tmp -= sig6_over_r6;
                        tmp *= eps;
                        tmp *= eps1[j];
                    
                        //apply the cutoff - compare r against Rlj. This will
                        //return 1 if r is less than Rlj, or 0 otherwise. Logical
                        //and will then remove all energies where r >= Rlj
                        tmp &= r.compareLess(Rlj);
                        iljnrg += tmp.logicalAndNot(itmp);
                    }
                }
            }
        }
    }
    else
    {
        MultiFloat bonded_mask(0);
    
        for (int i=0; i<n0; ++i)
        {
            for (int ii=0; ii<MultiFloat::count(); ++ii)
            {
                if (id0[i][ii] != dummy_int)
                {
                    const MultiInt id(id0[i][ii]);
                    const MultiFloat x(x0[i][ii]);
                    const MultiFloat y(y0[i][ii]);
                    const MultiFloat z(z0[i][ii]);
                    const MultiFloat q(q0[i][ii]);

                    const MultiFloat sig(sig0[i][ii]);
                    const MultiFloat eps(eps0[i][ii]);

                    const bool *row = bondMatrix().constData()[id[0]].constData();

                    for (int j=0; j<n1; ++j)
                    {
                        //create a mask to cancel out calculations of bonded pairs
                        for (int k=0; k<MultiInt::count(); ++k)
                        {
                            bonded_mask.quickSet(k, row[id1[j][k]] ? 0.0 : 1.0);
                        }

                        //calculate the distance between the fixed and mobile atoms
                        tmp = x1[j] - x;
                        r = tmp * tmp;
                        tmp = y1[j] - y;
                        r.multiplyAdd(tmp, tmp);
                        tmp = z1[j] - z;
                        r.multiplyAdd(tmp, tmp);
                        r = r.sqrt();

                        one_over_r = r.reciprocal();
                
                        //calculate the coulomb energy using shift-electrostatics
                        // energy = q0q1 * { 1/r - 1/Rc + 1/Rc^2 [r - Rc] }
                        tmp = r - Rc;
                        tmp *= one_over_Rc2;
                        tmp -= one_over_Rc;
                        tmp += one_over_r;
                        tmp *= q * q1[j];
                    
                        //apply the cutoff - compare r against Rc. This will
                        //return 1 if r is less than Rc, or 0 otherwise. Logical
                        //and will then remove all energies where r >= Rc
                        tmp &= r.compareLess(Rc);

                        //make sure that the ID of atoms1 is not zero
                        itmp = id1[j].compareEqual(dummy_id);

                        icnrg += bonded_mask * tmp.logicalAndNot(itmp);
                        
                        //Now do the LJ energy
                        sig2_over_r2 = sig * sig1[j] * one_over_r;
                        sig2_over_r2 = sig2_over_r2*sig2_over_r2;
                        sig6_over_r6 = sig2_over_r2*sig2_over_r2;
                        sig6_over_r6 = sig6_over_r6*sig2_over_r2;

                        tmp = sig6_over_r6 * sig6_over_r6;
                        tmp -= sig6_over_r6;
                        tmp *= eps;
                        tmp *= eps1[j];
                    
                        //apply the cutoff - compare r against Rlj. This will
                        //return 1 if r is less than Rlj, or 0 otherwise. Logical
                        //and will then remove all energies where r >= Rlj
                        tmp &= r.compareLess(Rlj);
                        iljnrg += bonded_mask * tmp.logicalAndNot(itmp);
                    }
                }
            }
        }
    }
    
    cnrg = icnrg.sum();
    ljnrg = iljnrg.sum();
}

/** Calculate the coulomb and LJ intramolecular energy of all of the atoms in 'atoms',
    assuming periodic boundary conditions in a cubic box of size 'box_dimensions',
    returning the results in 'cnrg' and 'ljnrg'.
    Note that all of the atoms must be part of the same molecule, and must
    have their intramolecular non-bonded scale factors loaded into this function */
void CLJIntraShiftFunction::calcBoxEnergyGeo(const CLJAtoms &atoms, const Vector &box_dimensions,
                                             double &cnrg, double &ljnrg) const
{
    const MultiFloat *xa = atoms.x().constData();
    const MultiFloat *ya = atoms.y().constData();
    const MultiFloat *za = atoms.z().constData();
    const MultiFloat *qa = atoms.q().constData();
    const MultiFloat *siga = atoms.sigma().constData();
    const MultiFloat *epsa = atoms.epsilon().constData();
    const MultiInt *ida = atoms.ID().constData();

    const MultiFloat Rc(coul_cutoff);
    const MultiFloat Rlj(lj_cutoff);

    const MultiFloat one_over_Rc( 1.0 / coul_cutoff );
    const MultiFloat one_over_Rc2( 1.0 / (coul_cutoff*coul_cutoff) );
    const MultiFloat half(0.5);
    const MultiInt dummy_id = CLJAtoms::idOfDummy();
    const qint32 dummy_int = dummy_id[0];

    MultiFloat tmp, r, one_over_r, sig2_over_r2, sig6_over_r6;
    MultiFloat bond_mask;
    MultiDouble icnrg(0), iljnrg(0);
    MultiInt itmp;

    const MultiFloat box_x( box_dimensions.x() );
    const MultiFloat box_y( box_dimensions.y() );
    const MultiFloat box_z( box_dimensions.z() );
    
    const MultiFloat half_box_x( 0.5 * box_dimensions.x() );
    const MultiFloat half_box_y( 0.5 * box_dimensions.y() );
    const MultiFloat half_box_z( 0.5 * box_dimensions.z() );

    int n = atoms.x().count();
    
    for (int i=0; i<n; ++i)
    {
        for (int ii=0; ii<MultiFloat::size(); ++ii)
        {
            if (ida[i][ii] != dummy_int)
            {
                const MultiInt id( ida[i][ii] );
                const MultiFloat x( xa[i][ii] );
                const MultiFloat y( ya[i][ii] );
                const MultiFloat z( za[i][ii] );
                const MultiFloat q( qa[i][ii] );
                const MultiFloat sig( siga[i][ii] );
                const MultiFloat eps( epsa[i][ii] );

                const bool *row = bondMatrix().constData()[id[0]].constData();

                for (int j=i; j<n; ++j)
                {
                    // if i == j then we double-calculate the energies, so must
                    // scale them by 0.5
                    MultiFloat scale( i == j ? 0.5 : 1.0 );
                
                    //get the bond mask to screen out bonded interactions
                    for (int k=0; k<MultiFloat::count(); ++k)
                    {
                        bond_mask.quickSet(k, row[ida[j][k]] ? 0.0 : 1.0);
                    }
                
                    scale *= bond_mask;
                
                    //calculate the distance between the fixed and mobile atoms
                    tmp = xa[j] - x;
                    tmp &= MULTIFLOAT_POS_MASK;  // this creates the absolute value :-)
                    tmp -= box_x.logicalAnd( half_box_x.compareLess(tmp) );
                    r = tmp * tmp;

                    tmp = ya[j] - y;
                    tmp &= MULTIFLOAT_POS_MASK;  // this creates the absolute value :-)
                    tmp -= box_y.logicalAnd( half_box_y.compareLess(tmp) );
                    r.multiplyAdd(tmp, tmp);

                    tmp = za[j] - z;
                    tmp &= MULTIFLOAT_POS_MASK;  // this creates the absolute value :-)
                    tmp -= box_z.logicalAnd( half_box_z.compareLess(tmp) );
                    r.multiplyAdd(tmp, tmp);

                    r = r.sqrt();

                    one_over_r = r.reciprocal();
            
                    //calculate the coulomb energy using shift-electrostatics
                    // energy = q0q1 * { 1/r - 1/Rc + 1/Rc^2 [r - Rc] }
                    tmp = r - Rc;
                    tmp *= one_over_Rc2;
                    tmp -= one_over_Rc;
                    tmp += one_over_r;
                    tmp *= q * qa[j];
                
                    //apply the cutoff - compare r against Rc. This will
                    //return 1 if r is less than Rc, or 0 otherwise. Logical
                    //and will then remove all energies where r >= Rc
                    tmp &= r.compareLess(Rc);

                    //make sure that the ID of atoms1 is not zero, and is
                    //also not the same as the atoms0 and that we are not
                    //including the energy of the atom with itself
                    itmp = ida[j].compareEqual(dummy_id);
                    itmp |= ida[j].compareEqual(id);

                    icnrg += scale * tmp.logicalAndNot(itmp);

                    //now the LJ energy
                    sig2_over_r2 = sig * siga[j] * one_over_r;
                    sig2_over_r2 = sig2_over_r2*sig2_over_r2;
                    sig6_over_r6 = sig2_over_r2*sig2_over_r2;
                    sig6_over_r6 = sig6_over_r6*sig2_over_r2;

                    tmp = sig6_over_r6 * sig6_over_r6;
                    tmp -= sig6_over_r6;
                    tmp *= eps;
                    tmp *= epsa[j];
                
                    //apply the cutoff - compare r against Rlj. This will
                    //return 1 if r is less than Rlj, or 0 otherwise. Logical
                    //and will then remove all energies where r >= Rlj
                    tmp &= r.compareLess(Rlj);
                    iljnrg += scale * tmp.logicalAndNot(itmp);
                }
            }
        }
    }
    
    cnrg = icnrg.sum();
    ljnrg = iljnrg.sum();
}

/** Calculate the intramolecular energy between all atoms in 'atoms0' and all
    atoms in 'atoms1', assuming periodic boundary conditions in a cubic box
    of size 'box_dimensions, returning the result in the arguments 'cnrg' and 'ljnrg'.
    Note that all of the atoms must be part of the same molecule, and must
    have their intramolecular non-bonded scale factors loaded into this function */
void CLJIntraShiftFunction::calcBoxEnergyGeo(const CLJAtoms &atoms0, const CLJAtoms &atoms1,
                                             const Vector &box_dimensions,
                                             double &cnrg, double &ljnrg, float min_distance) const
{
    const MultiFloat *x0 = atoms0.x().constData();
    const MultiFloat *y0 = atoms0.y().constData();
    const MultiFloat *z0 = atoms0.z().constData();
    const MultiFloat *q0 = atoms0.q().constData();
    const MultiFloat *sig0 = atoms0.sigma().constData();
    const MultiFloat *eps0 = atoms0.epsilon().constData();
    const MultiInt *id0 = atoms0.ID().constData();

    const MultiFloat *x1 = atoms1.x().constData();
    const MultiFloat *y1 = atoms1.y().constData();
    const MultiFloat *z1 = atoms1.z().constData();
    const MultiFloat *q1 = atoms1.q().constData();
    const MultiFloat *sig1 = atoms1.sigma().constData();
    const MultiFloat *eps1 = atoms1.epsilon().constData();
    const MultiInt *id1 = atoms1.ID().constData();
    
    const MultiFloat Rc(coul_cutoff);
    const MultiFloat Rlj(lj_cutoff);
    const MultiFloat one_over_Rc( 1.0 / coul_cutoff );
    const MultiFloat one_over_Rc2( 1.0 / (coul_cutoff*coul_cutoff) );
    const MultiFloat half(0.5);
    const MultiInt dummy_id = CLJAtoms::idOfDummy();
    const qint32 dummy_int = dummy_id[0];

    MultiFloat tmp, r, one_over_r, sig2_over_r2, sig6_over_r6;
    MultiDouble icnrg(0), iljnrg(0);
    MultiInt itmp;

    const MultiFloat box_x( box_dimensions.x() );
    const MultiFloat box_y( box_dimensions.y() );
    const MultiFloat box_z( box_dimensions.z() );
    
    const MultiFloat half_box_x( 0.5 * box_dimensions.x() );
    const MultiFloat half_box_y( 0.5 * box_dimensions.y() );
    const MultiFloat half_box_z( 0.5 * box_dimensions.z() );

    const int n0 = atoms0.x().count();
    const int n1 = atoms1.x().count();
    
    bool not_bonded = true;
    
    if (min_distance < 5.5)
    {
        not_bonded = isNotBonded(atoms0.ID(), atoms1.ID());
    }
    
    if ( Q_LIKELY(not_bonded) )
    {
        for (int i=0; i<n0; ++i)
        {
            for (int ii=0; ii<MultiFloat::count(); ++ii)
            {
                if (id0[i][ii] != dummy_int)
                {
                    const MultiInt id(id0[i][ii]);
                    const MultiFloat x(x0[i][ii]);
                    const MultiFloat y(y0[i][ii]);
                    const MultiFloat z(z0[i][ii]);
                    const MultiFloat q(q0[i][ii]);

                    const MultiFloat sig(sig0[i][ii]);
                    const MultiFloat eps(eps0[i][ii]);

                    for (int j=0; j<n1; ++j)
                    {
                        //calculate the distance between the fixed and mobile atoms
                        tmp = x1[j] - x;
                        tmp &= MULTIFLOAT_POS_MASK;  // this creates the absolute value :-)
                        tmp -= box_x.logicalAnd( half_box_x.compareLess(tmp) );
                        r = tmp * tmp;

                        tmp = y1[j] - y;
                        tmp &= MULTIFLOAT_POS_MASK;  // this creates the absolute value :-)
                        tmp -= box_y.logicalAnd( half_box_y.compareLess(tmp) );
                        r.multiplyAdd(tmp, tmp);

                        tmp = z1[j] - z;
                        tmp &= MULTIFLOAT_POS_MASK;  // this creates the absolute value :-)
                        tmp -= box_z.logicalAnd( half_box_z.compareLess(tmp) );
                        r.multiplyAdd(tmp, tmp);
                        
                        r = r.sqrt();

                        one_over_r = r.reciprocal();
                
                        //calculate the coulomb energy using shift-electrostatics
                        // energy = q0q1 * { 1/r - 1/Rc + 1/Rc^2 [r - Rc] }
                        tmp = r - Rc;
                        tmp *= one_over_Rc2;
                        tmp -= one_over_Rc;
                        tmp += one_over_r;
                        tmp *= q * q1[j];
                    
                        //apply the cutoff - compare r against Rc. This will
                        //return 1 if r is less than Rc, or 0 otherwise. Logical
                        //and will then remove all energies where r >= Rc
                        tmp &= r.compareLess(Rc);

                        //make sure that the ID of atoms1 is not zero
                        itmp = id1[j].compareEqual(dummy_id);

                        icnrg += tmp.logicalAndNot(itmp);
                        
                        //Now do the LJ energy
                        sig2_over_r2 = sig * sig1[j] * one_over_r;
                        sig2_over_r2 = sig2_over_r2*sig2_over_r2;
                        sig6_over_r6 = sig2_over_r2*sig2_over_r2;
                        sig6_over_r6 = sig6_over_r6*sig2_over_r2;

                        tmp = sig6_over_r6 * sig6_over_r6;
                        tmp -= sig6_over_r6;
                        tmp *= eps;
                        tmp *= eps1[j];
                    
                        //apply the cutoff - compare r against Rlj. This will
                        //return 1 if r is less than Rlj, or 0 otherwise. Logical
                        //and will then remove all energies where r >= Rlj
                        tmp &= r.compareLess(Rlj);
                        iljnrg += tmp.logicalAndNot(itmp);
                    }
                }
            }
        }
    }
    else
    {
        MultiFloat bonded_mask(0);
    
        for (int i=0; i<n0; ++i)
        {
            for (int ii=0; ii<MultiFloat::count(); ++ii)
            {
                if (id0[i][ii] != dummy_int)
                {
                    const MultiInt id(id0[i][ii]);
                    const MultiFloat x(x0[i][ii]);
                    const MultiFloat y(y0[i][ii]);
                    const MultiFloat z(z0[i][ii]);
                    const MultiFloat q(q0[i][ii]);

                    const MultiFloat sig(sig0[i][ii]);
                    const MultiFloat eps(eps0[i][ii]);

                    const bool *row = bondMatrix().constData()[id[0]].constData();

                    for (int j=0; j<n1; ++j)
                    {
                        //create a mask to cancel out calculations of bonded pairs
                        for (int k=0; k<MultiInt::count(); ++k)
                        {
                            bonded_mask.quickSet(k, row[id1[j][k]] ? 0.0 : 1.0);
                        }

                        //calculate the distance between the fixed and mobile atoms
                        tmp = x1[j] - x;
                        tmp &= MULTIFLOAT_POS_MASK;  // this creates the absolute value :-)
                        tmp -= box_x.logicalAnd( half_box_x.compareLess(tmp) );
                        r = tmp * tmp;

                        tmp = y1[j] - y;
                        tmp &= MULTIFLOAT_POS_MASK;  // this creates the absolute value :-)
                        tmp -= box_y.logicalAnd( half_box_y.compareLess(tmp) );
                        r.multiplyAdd(tmp, tmp);

                        tmp = z1[j] - z;
                        tmp &= MULTIFLOAT_POS_MASK;  // this creates the absolute value :-)
                        tmp -= box_z.logicalAnd( half_box_z.compareLess(tmp) );
                        r.multiplyAdd(tmp, tmp);
                        
                        r = r.sqrt();

                        one_over_r = r.reciprocal();
                
                        //calculate the coulomb energy using shift-electrostatics
                        // energy = q0q1 * { 1/r - 1/Rc + 1/Rc^2 [r - Rc] }
                        tmp = r - Rc;
                        tmp *= one_over_Rc2;
                        tmp -= one_over_Rc;
                        tmp += one_over_r;
                        tmp *= q * q1[j];
                    
                        //apply the cutoff - compare r against Rc. This will
                        //return 1 if r is less than Rc, or 0 otherwise. Logical
                        //and will then remove all energies where r >= Rc
                        tmp &= r.compareLess(Rc);

                        //make sure that the ID of atoms1 is not zero
                        itmp = id1[j].compareEqual(dummy_id);

                        icnrg += bonded_mask * tmp.logicalAndNot(itmp);
                        
                        //Now do the LJ energy
                        sig2_over_r2 = sig * sig1[j] * one_over_r;
                        sig2_over_r2 = sig2_over_r2*sig2_over_r2;
                        sig6_over_r6 = sig2_over_r2*sig2_over_r2;
                        sig6_over_r6 = sig6_over_r6*sig2_over_r2;

                        tmp = sig6_over_r6 * sig6_over_r6;
                        tmp -= sig6_over_r6;
                        tmp *= eps;
                        tmp *= eps1[j];
                    
                        //apply the cutoff - compare r against Rlj. This will
                        //return 1 if r is less than Rlj, or 0 otherwise. Logical
                        //and will then remove all energies where r >= Rlj
                        tmp &= r.compareLess(Rlj);
                        iljnrg += bonded_mask * tmp.logicalAndNot(itmp);
                    }
                }
            }
        }
    }
    
    cnrg = icnrg.sum();
    ljnrg = iljnrg.sum();
}

/** Calculate the coulomb and LJ intramolecular energy of all of the atoms in 'atoms',
    returning the results in the arguments 'cnrg' and 'ljnrg'.
    Note that all of the atoms must be part of the same molecule, and must
    have their intramolecular non-bonded scale factors loaded into this function */
void CLJIntraShiftFunction::calcVacEnergyAri(const CLJAtoms &atoms,
                                             double &cnrg, double &ljnrg) const
{
    const MultiFloat *xa = atoms.x().constData();
    const MultiFloat *ya = atoms.y().constData();
    const MultiFloat *za = atoms.z().constData();
    const MultiFloat *qa = atoms.q().constData();
    const MultiFloat *siga = atoms.sigma().constData();
    const MultiFloat *epsa = atoms.epsilon().constData();
    const MultiInt *ida = atoms.ID().constData();

    const MultiFloat Rc(coul_cutoff);
    const MultiFloat Rlj(lj_cutoff);

    const MultiFloat one_over_Rc( 1.0 / coul_cutoff );
    const MultiFloat one_over_Rc2( 1.0 / (coul_cutoff*coul_cutoff) );
    const MultiFloat half(0.5);
    const MultiInt dummy_id = CLJAtoms::idOfDummy();
    const qint32 dummy_int = dummy_id[0];

    MultiFloat tmp, r, one_over_r, sig2_over_r2, sig6_over_r6;
    MultiFloat bond_mask;
    MultiDouble icnrg(0), iljnrg(0);
    MultiInt itmp;

    int n = atoms.x().count();
    
    for (int i=0; i<n; ++i)
    {
        for (int ii=0; ii<MultiFloat::size(); ++ii)
        {
            if (ida[i][ii] != dummy_int)
            {
                const MultiInt id( ida[i][ii] );
                const MultiFloat x( xa[i][ii] );
                const MultiFloat y( ya[i][ii] );
                const MultiFloat z( za[i][ii] );
                const MultiFloat q( qa[i][ii] );
                const MultiFloat sig( siga[i][ii] * siga[i][ii] );
                const MultiFloat eps( epsa[i][ii] );

                const bool *row = bondMatrix().constData()[id[0]].constData();

                for (int j=i; j<n; ++j)
                {
                    // if i == j then we double-calculate the energies, so must
                    // scale them by 0.5
                    MultiFloat scale( i == j ? 0.5 : 1.0 );
                
                    //get the bond mask to screen out bonded interactions
                    for (int k=0; k<MultiFloat::count(); ++k)
                    {
                        bond_mask.quickSet(k, row[ida[j][k]] ? 0.0 : 1.0);
                    }
                
                    scale *= bond_mask;
                
                    //calculate the distance between the fixed and mobile atoms
                    tmp = xa[j] - x;
                    r = tmp * tmp;
                    tmp = ya[j] - y;
                    r.multiplyAdd(tmp, tmp);
                    tmp = za[j] - z;
                    r.multiplyAdd(tmp, tmp);
                    r = r.sqrt();

                    one_over_r = r.reciprocal();
            
                    //calculate the coulomb energy using shift-electrostatics
                    // energy = q0q1 * { 1/r - 1/Rc + 1/Rc^2 [r - Rc] }
                    tmp = r - Rc;
                    tmp *= one_over_Rc2;
                    tmp -= one_over_Rc;
                    tmp += one_over_r;
                    tmp *= q * qa[j];
                
                    //apply the cutoff - compare r against Rc. This will
                    //return 1 if r is less than Rc, or 0 otherwise. Logical
                    //and will then remove all energies where r >= Rc
                    tmp &= r.compareLess(Rc);

                    //make sure that the ID of atoms1 is not zero, and is
                    //also not the same as the atoms0 and that we are not
                    //including the energy of the atom with itself
                    itmp = ida[j].compareEqual(dummy_id);
                    itmp |= ida[j].compareEqual(id);

                    icnrg += scale * tmp.logicalAndNot(itmp);

                    //now the LJ energy
                    tmp = sig + (siga[j]*siga[j]);
                    tmp *= half;
                
                    sig2_over_r2 = tmp * one_over_r;
                    sig2_over_r2 = sig2_over_r2*sig2_over_r2;
                    sig6_over_r6 = sig2_over_r2*sig2_over_r2;
                    sig6_over_r6 = sig6_over_r6*sig2_over_r2;

                    tmp = sig6_over_r6 * sig6_over_r6;
                    tmp -= sig6_over_r6;
                    tmp *= eps;
                    tmp *= epsa[j];
                
                    //apply the cutoff - compare r against Rlj. This will
                    //return 1 if r is less than Rlj, or 0 otherwise. Logical
                    //and will then remove all energies where r >= Rlj
                    tmp &= r.compareLess(Rlj);
                    iljnrg += scale * tmp.logicalAndNot(itmp);
                }
            }
        }
    }
    
    cnrg = icnrg.sum();
    ljnrg = iljnrg.sum();
}

/** Calculate the intramolecular energy between all atoms in 'atoms0' and all
    atoms in 'atoms1', returning the result in the arguments 'cnrg' and 'ljnrg'.
    Note that all of the atoms must be part of the same molecule, and must
    have their intramolecular non-bonded scale factors loaded into this function */
void CLJIntraShiftFunction::calcVacEnergyAri(const CLJAtoms &atoms0, const CLJAtoms &atoms1,
                                             double &cnrg, double &ljnrg, float min_distance) const
{
    const MultiFloat *x0 = atoms0.x().constData();
    const MultiFloat *y0 = atoms0.y().constData();
    const MultiFloat *z0 = atoms0.z().constData();
    const MultiFloat *q0 = atoms0.q().constData();
    const MultiFloat *sig0 = atoms0.sigma().constData();
    const MultiFloat *eps0 = atoms0.epsilon().constData();
    const MultiInt *id0 = atoms0.ID().constData();

    const MultiFloat *x1 = atoms1.x().constData();
    const MultiFloat *y1 = atoms1.y().constData();
    const MultiFloat *z1 = atoms1.z().constData();
    const MultiFloat *q1 = atoms1.q().constData();
    const MultiFloat *sig1 = atoms1.sigma().constData();
    const MultiFloat *eps1 = atoms1.epsilon().constData();
    const MultiInt *id1 = atoms1.ID().constData();
    
    const MultiFloat Rc(coul_cutoff);
    const MultiFloat Rlj(lj_cutoff);
    const MultiFloat one_over_Rc( 1.0 / coul_cutoff );
    const MultiFloat one_over_Rc2( 1.0 / (coul_cutoff*coul_cutoff) );
    const MultiFloat half(0.5);
    const MultiInt dummy_id = CLJAtoms::idOfDummy();
    const qint32 dummy_int = dummy_id[0];

    MultiFloat tmp, r, one_over_r, sig2_over_r2, sig6_over_r6;
    MultiDouble icnrg(0), iljnrg(0);
    MultiInt itmp;

    const int n0 = atoms0.x().count();
    const int n1 = atoms1.x().count();
    
    bool not_bonded = true;
    
    if (min_distance < 5.5)
    {
        not_bonded = isNotBonded(atoms0.ID(), atoms1.ID());
    }
    
    if ( Q_LIKELY(not_bonded) )
    {
        for (int i=0; i<n0; ++i)
        {
            for (int ii=0; ii<MultiFloat::count(); ++ii)
            {
                if (id0[i][ii] != dummy_int)
                {
                    const MultiInt id(id0[i][ii]);
                    const MultiFloat x(x0[i][ii]);
                    const MultiFloat y(y0[i][ii]);
                    const MultiFloat z(z0[i][ii]);
                    const MultiFloat q(q0[i][ii]);

                    const MultiFloat sig(sig0[i][ii] * sig0[i][ii]);
                    const MultiFloat eps(eps0[i][ii]);

                    for (int j=0; j<n1; ++j)
                    {
                        //calculate the distance between the fixed and mobile atoms
                        tmp = x1[j] - x;
                        r = tmp * tmp;
                        tmp = y1[j] - y;
                        r.multiplyAdd(tmp, tmp);
                        tmp = z1[j] - z;
                        r.multiplyAdd(tmp, tmp);
                        r = r.sqrt();

                        one_over_r = r.reciprocal();
                
                        //calculate the coulomb energy using shift-electrostatics
                        // energy = q0q1 * { 1/r - 1/Rc + 1/Rc^2 [r - Rc] }
                        tmp = r - Rc;
                        tmp *= one_over_Rc2;
                        tmp -= one_over_Rc;
                        tmp += one_over_r;
                        tmp *= q * q1[j];
                    
                        //apply the cutoff - compare r against Rc. This will
                        //return 1 if r is less than Rc, or 0 otherwise. Logical
                        //and will then remove all energies where r >= Rc
                        tmp &= r.compareLess(Rc);

                        //make sure that the ID of atoms1 is not zero
                        itmp = id1[j].compareEqual(dummy_id);

                        icnrg += tmp.logicalAndNot(itmp);
                        
                        //Now do the LJ energy

                        //arithmetic combining rules
                        tmp = sig + (sig1[j]*sig1[j]);
                        tmp *= half;
                    
                        sig2_over_r2 = tmp * one_over_r;
                        sig2_over_r2 = sig2_over_r2*sig2_over_r2;
                        sig6_over_r6 = sig2_over_r2*sig2_over_r2;
                        sig6_over_r6 = sig6_over_r6*sig2_over_r2;

                        tmp = sig6_over_r6 * sig6_over_r6;
                        tmp -= sig6_over_r6;
                        tmp *= eps;
                        tmp *= eps1[j];
                    
                        //apply the cutoff - compare r against Rlj. This will
                        //return 1 if r is less than Rlj, or 0 otherwise. Logical
                        //and will then remove all energies where r >= Rlj
                        tmp &= r.compareLess(Rlj);
                        iljnrg += tmp.logicalAndNot(itmp);
                    }
                }
            }
        }
    }
    else
    {
        MultiFloat bonded_mask(0);
    
        for (int i=0; i<n0; ++i)
        {
            for (int ii=0; ii<MultiFloat::count(); ++ii)
            {
                if (id0[i][ii] != dummy_int)
                {
                    const MultiInt id(id0[i][ii]);
                    const MultiFloat x(x0[i][ii]);
                    const MultiFloat y(y0[i][ii]);
                    const MultiFloat z(z0[i][ii]);
                    const MultiFloat q(q0[i][ii]);

                    const MultiFloat sig(sig0[i][ii] * sig0[i][ii]);
                    const MultiFloat eps(eps0[i][ii]);

                    const bool *row = bondMatrix().constData()[id[0]].constData();

                    for (int j=0; j<n1; ++j)
                    {
                        //create a mask to cancel out calculations of bonded pairs
                        for (int k=0; k<MultiInt::count(); ++k)
                        {
                            bonded_mask.quickSet(k, row[id1[j][k]] ? 0.0 : 1.0);
                        }

                        //calculate the distance between the fixed and mobile atoms
                        tmp = x1[j] - x;
                        r = tmp * tmp;
                        tmp = y1[j] - y;
                        r.multiplyAdd(tmp, tmp);
                        tmp = z1[j] - z;
                        r.multiplyAdd(tmp, tmp);
                        r = r.sqrt();

                        one_over_r = r.reciprocal();
                
                        //calculate the coulomb energy using shift-electrostatics
                        // energy = q0q1 * { 1/r - 1/Rc + 1/Rc^2 [r - Rc] }
                        tmp = r - Rc;
                        tmp *= one_over_Rc2;
                        tmp -= one_over_Rc;
                        tmp += one_over_r;
                        tmp *= q * q1[j];
                    
                        //apply the cutoff - compare r against Rc. This will
                        //return 1 if r is less than Rc, or 0 otherwise. Logical
                        //and will then remove all energies where r >= Rc
                        tmp &= r.compareLess(Rc);

                        //make sure that the ID of atoms1 is not zero
                        itmp = id1[j].compareEqual(dummy_id);

                        icnrg += bonded_mask * tmp.logicalAndNot(itmp);
                        
                        //Now do the LJ energy

                        //arithmetic combining rules
                        tmp = sig + (sig1[j]*sig1[j]);
                        tmp *= half;
                    
                        sig2_over_r2 = tmp * one_over_r;
                        sig2_over_r2 = sig2_over_r2*sig2_over_r2;
                        sig6_over_r6 = sig2_over_r2*sig2_over_r2;
                        sig6_over_r6 = sig6_over_r6*sig2_over_r2;

                        tmp = sig6_over_r6 * sig6_over_r6;
                        tmp -= sig6_over_r6;
                        tmp *= eps;
                        tmp *= eps1[j];
                    
                        //apply the cutoff - compare r against Rlj. This will
                        //return 1 if r is less than Rlj, or 0 otherwise. Logical
                        //and will then remove all energies where r >= Rlj
                        tmp &= r.compareLess(Rlj);
                        iljnrg += bonded_mask * tmp.logicalAndNot(itmp);
                    }
                }
            }
        }
    }
    
    cnrg = icnrg.sum();
    ljnrg = iljnrg.sum();
}

/** Calculate the coulomb and LJ intramolecular energy of all of the atoms in 'atoms',
    assuming periodic boundary conditions in a cubic box of size 'box_dimensions',
    returning the results in 'cnrg' and 'ljnrg'.
    Note that all of the atoms must be part of the same molecule, and must
    have their intramolecular non-bonded scale factors loaded into this function */
void CLJIntraShiftFunction::calcBoxEnergyAri(const CLJAtoms &atoms, const Vector &box_dimensions,
                                             double &cnrg, double &ljnrg) const
{
    const MultiFloat *xa = atoms.x().constData();
    const MultiFloat *ya = atoms.y().constData();
    const MultiFloat *za = atoms.z().constData();
    const MultiFloat *qa = atoms.q().constData();
    const MultiFloat *siga = atoms.sigma().constData();
    const MultiFloat *epsa = atoms.epsilon().constData();
    const MultiInt *ida = atoms.ID().constData();

    const MultiFloat Rc(coul_cutoff);
    const MultiFloat Rlj(lj_cutoff);

    const MultiFloat one_over_Rc( 1.0 / coul_cutoff );
    const MultiFloat one_over_Rc2( 1.0 / (coul_cutoff*coul_cutoff) );
    const MultiFloat half(0.5);
    const MultiInt dummy_id = CLJAtoms::idOfDummy();
    const qint32 dummy_int = dummy_id[0];

    MultiFloat tmp, r, one_over_r, sig2_over_r2, sig6_over_r6;
    MultiFloat bond_mask;
    MultiDouble icnrg(0), iljnrg(0);
    MultiInt itmp;

    const MultiFloat box_x( box_dimensions.x() );
    const MultiFloat box_y( box_dimensions.y() );
    const MultiFloat box_z( box_dimensions.z() );
    
    const MultiFloat half_box_x( 0.5 * box_dimensions.x() );
    const MultiFloat half_box_y( 0.5 * box_dimensions.y() );
    const MultiFloat half_box_z( 0.5 * box_dimensions.z() );

    int n = atoms.x().count();
    
    for (int i=0; i<n; ++i)
    {
        for (int ii=0; ii<MultiFloat::size(); ++ii)
        {
            if (ida[i][ii] != dummy_int)
            {
                const MultiInt id( ida[i][ii] );
                const MultiFloat x( xa[i][ii] );
                const MultiFloat y( ya[i][ii] );
                const MultiFloat z( za[i][ii] );
                const MultiFloat q( qa[i][ii] );
                const MultiFloat sig( siga[i][ii] * siga[i][ii] );
                const MultiFloat eps( epsa[i][ii] );

                const bool *row = bondMatrix().constData()[id[0]].constData();

                for (int j=i; j<n; ++j)
                {
                    // if i == j then we double-calculate the energies, so must
                    // scale them by 0.5
                    MultiFloat scale( i == j ? 0.5 : 1.0 );
                
                    //get the bond mask to screen out bonded interactions
                    for (int k=0; k<MultiFloat::count(); ++k)
                    {
                        bond_mask.quickSet(k, row[ida[j][k]] ? 0.0 : 1.0);
                    }
                
                    scale *= bond_mask;
                
                    //calculate the distance between the fixed and mobile atoms
                    tmp = xa[j] - x;
                    tmp &= MULTIFLOAT_POS_MASK;  // this creates the absolute value :-)
                    tmp -= box_x.logicalAnd( half_box_x.compareLess(tmp) );
                    r = tmp * tmp;

                    tmp = ya[j] - y;
                    tmp &= MULTIFLOAT_POS_MASK;  // this creates the absolute value :-)
                    tmp -= box_y.logicalAnd( half_box_y.compareLess(tmp) );
                    r.multiplyAdd(tmp, tmp);

                    tmp = za[j] - z;
                    tmp &= MULTIFLOAT_POS_MASK;  // this creates the absolute value :-)
                    tmp -= box_z.logicalAnd( half_box_z.compareLess(tmp) );
                    r.multiplyAdd(tmp, tmp);

                    r = r.sqrt();

                    one_over_r = r.reciprocal();
            
                    //calculate the coulomb energy using shift-electrostatics
                    // energy = q0q1 * { 1/r - 1/Rc + 1/Rc^2 [r - Rc] }
                    tmp = r - Rc;
                    tmp *= one_over_Rc2;
                    tmp -= one_over_Rc;
                    tmp += one_over_r;
                    tmp *= q * qa[j];
                
                    //apply the cutoff - compare r against Rc. This will
                    //return 1 if r is less than Rc, or 0 otherwise. Logical
                    //and will then remove all energies where r >= Rc
                    tmp &= r.compareLess(Rc);

                    //make sure that the ID of atoms1 is not zero, and is
                    //also not the same as the atoms0 and that we are not
                    //including the energy of the atom with itself
                    itmp = ida[j].compareEqual(dummy_id);
                    itmp |= ida[j].compareEqual(id);

                    icnrg += scale * tmp.logicalAndNot(itmp);

                    //now the LJ energy
                    tmp = sig + (siga[j]*siga[j]);
                    tmp *= half;
                
                    sig2_over_r2 = tmp * one_over_r;
                    sig2_over_r2 = sig2_over_r2*sig2_over_r2;
                    sig6_over_r6 = sig2_over_r2*sig2_over_r2;
                    sig6_over_r6 = sig6_over_r6*sig2_over_r2;

                    tmp = sig6_over_r6 * sig6_over_r6;
                    tmp -= sig6_over_r6;
                    tmp *= eps;
                    tmp *= epsa[j];
                
                    //apply the cutoff - compare r against Rlj. This will
                    //return 1 if r is less than Rlj, or 0 otherwise. Logical
                    //and will then remove all energies where r >= Rlj
                    tmp &= r.compareLess(Rlj);
                    iljnrg += scale * tmp.logicalAndNot(itmp);
                }
            }
        }
    }
    
    cnrg = icnrg.sum();
    ljnrg = iljnrg.sum();
}

/** Calculate the intramolecular energy between all atoms in 'atoms0' and all
    atoms in 'atoms1', assuming periodic boundary conditions in a cubic box
    of size 'box_dimensions, returning the result in the arguments 'cnrg' and 'ljnrg'.
    Note that all of the atoms must be part of the same molecule, and must
    have their intramolecular non-bonded scale factors loaded into this function */
void CLJIntraShiftFunction::calcBoxEnergyAri(const CLJAtoms &atoms0, const CLJAtoms &atoms1,
                                             const Vector &box_dimensions,
                                             double &cnrg, double &ljnrg, float min_distance) const
{
    const MultiFloat *x0 = atoms0.x().constData();
    const MultiFloat *y0 = atoms0.y().constData();
    const MultiFloat *z0 = atoms0.z().constData();
    const MultiFloat *q0 = atoms0.q().constData();
    const MultiFloat *sig0 = atoms0.sigma().constData();
    const MultiFloat *eps0 = atoms0.epsilon().constData();
    const MultiInt *id0 = atoms0.ID().constData();

    const MultiFloat *x1 = atoms1.x().constData();
    const MultiFloat *y1 = atoms1.y().constData();
    const MultiFloat *z1 = atoms1.z().constData();
    const MultiFloat *q1 = atoms1.q().constData();
    const MultiFloat *sig1 = atoms1.sigma().constData();
    const MultiFloat *eps1 = atoms1.epsilon().constData();
    const MultiInt *id1 = atoms1.ID().constData();
    
    const MultiFloat Rc(coul_cutoff);
    const MultiFloat Rlj(lj_cutoff);
    const MultiFloat one_over_Rc( 1.0 / coul_cutoff );
    const MultiFloat one_over_Rc2( 1.0 / (coul_cutoff*coul_cutoff) );
    const MultiFloat half(0.5);
    const MultiInt dummy_id = CLJAtoms::idOfDummy();
    const qint32 dummy_int = dummy_id[0];

    MultiFloat tmp, r, one_over_r, sig2_over_r2, sig6_over_r6;
    MultiDouble icnrg(0), iljnrg(0);
    MultiInt itmp;

    const MultiFloat box_x( box_dimensions.x() );
    const MultiFloat box_y( box_dimensions.y() );
    const MultiFloat box_z( box_dimensions.z() );
    
    const MultiFloat half_box_x( 0.5 * box_dimensions.x() );
    const MultiFloat half_box_y( 0.5 * box_dimensions.y() );
    const MultiFloat half_box_z( 0.5 * box_dimensions.z() );

    const int n0 = atoms0.x().count();
    const int n1 = atoms1.x().count();
    
    bool not_bonded = true;
    
    if (min_distance < 5.5)
    {
        not_bonded = isNotBonded(atoms0.ID(), atoms1.ID());
    }
    
    if ( Q_LIKELY(not_bonded) )
    {
        for (int i=0; i<n0; ++i)
        {
            for (int ii=0; ii<MultiFloat::count(); ++ii)
            {
                if (id0[i][ii] != dummy_int)
                {
                    const MultiInt id(id0[i][ii]);
                    const MultiFloat x(x0[i][ii]);
                    const MultiFloat y(y0[i][ii]);
                    const MultiFloat z(z0[i][ii]);
                    const MultiFloat q(q0[i][ii]);

                    const MultiFloat sig(sig0[i][ii] * sig0[i][ii]);
                    const MultiFloat eps(eps0[i][ii]);

                    for (int j=0; j<n1; ++j)
                    {
                        //calculate the distance between the fixed and mobile atoms
                        tmp = x1[j] - x;
                        tmp &= MULTIFLOAT_POS_MASK;  // this creates the absolute value :-)
                        tmp -= box_x.logicalAnd( half_box_x.compareLess(tmp) );
                        r = tmp * tmp;

                        tmp = y1[j] - y;
                        tmp &= MULTIFLOAT_POS_MASK;  // this creates the absolute value :-)
                        tmp -= box_y.logicalAnd( half_box_y.compareLess(tmp) );
                        r.multiplyAdd(tmp, tmp);

                        tmp = z1[j] - z;
                        tmp &= MULTIFLOAT_POS_MASK;  // this creates the absolute value :-)
                        tmp -= box_z.logicalAnd( half_box_z.compareLess(tmp) );
                        r.multiplyAdd(tmp, tmp);
                        
                        r = r.sqrt();

                        one_over_r = r.reciprocal();
                
                        //calculate the coulomb energy using shift-electrostatics
                        // energy = q0q1 * { 1/r - 1/Rc + 1/Rc^2 [r - Rc] }
                        tmp = r - Rc;
                        tmp *= one_over_Rc2;
                        tmp -= one_over_Rc;
                        tmp += one_over_r;
                        tmp *= q * q1[j];
                    
                        //apply the cutoff - compare r against Rc. This will
                        //return 1 if r is less than Rc, or 0 otherwise. Logical
                        //and will then remove all energies where r >= Rc
                        tmp &= r.compareLess(Rc);

                        //make sure that the ID of atoms1 is not zero
                        itmp = id1[j].compareEqual(dummy_id);

                        icnrg += tmp.logicalAndNot(itmp);
                        
                        //Now do the LJ energy

                        //arithmetic combining rules
                        tmp = sig + (sig1[j]*sig1[j]);
                        tmp *= half;
                    
                        sig2_over_r2 = tmp * one_over_r;
                        sig2_over_r2 = sig2_over_r2*sig2_over_r2;
                        sig6_over_r6 = sig2_over_r2*sig2_over_r2;
                        sig6_over_r6 = sig6_over_r6*sig2_over_r2;

                        tmp = sig6_over_r6 * sig6_over_r6;
                        tmp -= sig6_over_r6;
                        tmp *= eps;
                        tmp *= eps1[j];
                    
                        //apply the cutoff - compare r against Rlj. This will
                        //return 1 if r is less than Rlj, or 0 otherwise. Logical
                        //and will then remove all energies where r >= Rlj
                        tmp &= r.compareLess(Rlj);
                        iljnrg += tmp.logicalAndNot(itmp);
                    }
                }
            }
        }
    }
    else
    {
        MultiFloat bonded_mask(0);
    
        for (int i=0; i<n0; ++i)
        {
            for (int ii=0; ii<MultiFloat::count(); ++ii)
            {
                if (id0[i][ii] != dummy_int)
                {
                    const MultiInt id(id0[i][ii]);
                    const MultiFloat x(x0[i][ii]);
                    const MultiFloat y(y0[i][ii]);
                    const MultiFloat z(z0[i][ii]);
                    const MultiFloat q(q0[i][ii]);

                    const MultiFloat sig(sig0[i][ii] * sig0[i][ii]);
                    const MultiFloat eps(eps0[i][ii]);

                    const bool *row = bondMatrix().constData()[id[0]].constData();

                    for (int j=0; j<n1; ++j)
                    {
                        //create a mask to cancel out calculations of bonded pairs
                        for (int k=0; k<MultiInt::count(); ++k)
                        {
                            bonded_mask.quickSet(k, row[id1[j][k]] ? 0.0 : 1.0);
                        }

                        //calculate the distance between the fixed and mobile atoms
                        tmp = x1[j] - x;
                        tmp &= MULTIFLOAT_POS_MASK;  // this creates the absolute value :-)
                        tmp -= box_x.logicalAnd( half_box_x.compareLess(tmp) );
                        r = tmp * tmp;

                        tmp = y1[j] - y;
                        tmp &= MULTIFLOAT_POS_MASK;  // this creates the absolute value :-)
                        tmp -= box_y.logicalAnd( half_box_y.compareLess(tmp) );
                        r.multiplyAdd(tmp, tmp);

                        tmp = z1[j] - z;
                        tmp &= MULTIFLOAT_POS_MASK;  // this creates the absolute value :-)
                        tmp -= box_z.logicalAnd( half_box_z.compareLess(tmp) );
                        r.multiplyAdd(tmp, tmp);
                        
                        r = r.sqrt();

                        one_over_r = r.reciprocal();
                
                        //calculate the coulomb energy using shift-electrostatics
                        // energy = q0q1 * { 1/r - 1/Rc + 1/Rc^2 [r - Rc] }
                        tmp = r - Rc;
                        tmp *= one_over_Rc2;
                        tmp -= one_over_Rc;
                        tmp += one_over_r;
                        tmp *= q * q1[j];
                    
                        //apply the cutoff - compare r against Rc. This will
                        //return 1 if r is less than Rc, or 0 otherwise. Logical
                        //and will then remove all energies where r >= Rc
                        tmp &= r.compareLess(Rc);

                        //make sure that the ID of atoms1 is not zero
                        itmp = id1[j].compareEqual(dummy_id);

                        icnrg += bonded_mask * tmp.logicalAndNot(itmp);
                        
                        //Now do the LJ energy

                        //arithmetic combining rules
                        tmp = sig + (sig1[j]*sig1[j]);
                        tmp *= half;
                    
                        sig2_over_r2 = tmp * one_over_r;
                        sig2_over_r2 = sig2_over_r2*sig2_over_r2;
                        sig6_over_r6 = sig2_over_r2*sig2_over_r2;
                        sig6_over_r6 = sig6_over_r6*sig2_over_r2;

                        tmp = sig6_over_r6 * sig6_over_r6;
                        tmp -= sig6_over_r6;
                        tmp *= eps;
                        tmp *= eps1[j];
                    
                        //apply the cutoff - compare r against Rlj. This will
                        //return 1 if r is less than Rlj, or 0 otherwise. Logical
                        //and will then remove all energies where r >= Rlj
                        tmp &= r.compareLess(Rlj);
                        iljnrg += bonded_mask * tmp.logicalAndNot(itmp);
                    }
                }
            }
        }
    }
    
    cnrg = icnrg.sum();
    ljnrg = iljnrg.sum();
}

/** Return the intramolecular coulomb energy of the atoms in 'atoms' */
double CLJIntraShiftFunction::calcVacCoulombEnergyAri(const CLJAtoms &atoms) const
{
    const MultiFloat *xa = atoms.x().constData();
    const MultiFloat *ya = atoms.y().constData();
    const MultiFloat *za = atoms.z().constData();
    const MultiFloat *qa = atoms.q().constData();
    const MultiInt *ida = atoms.ID().constData();

    const MultiFloat Rc(coul_cutoff);

    const MultiFloat one_over_Rc( 1.0 / coul_cutoff );
    const MultiFloat one_over_Rc2( 1.0 / (coul_cutoff*coul_cutoff) );
    const MultiFloat half(0.5);
    const MultiInt dummy_id = CLJAtoms::idOfDummy();
    const qint32 dummy_int = dummy_id[0];

    MultiFloat tmp, r, one_over_r;
    MultiFloat bond_mask;
    MultiDouble icnrg(0), iljnrg(0);
    MultiInt itmp;

    int n = atoms.x().count();
    
    for (int i=0; i<n; ++i)
    {
        for (int ii=0; ii<MultiFloat::size(); ++ii)
        {
            if (ida[i][ii] != dummy_int)
            {
                const MultiInt id( ida[i][ii] );
                const MultiFloat x( xa[i][ii] );
                const MultiFloat y( ya[i][ii] );
                const MultiFloat z( za[i][ii] );
                const MultiFloat q( qa[i][ii] );

                const bool *row = bondMatrix().constData()[id[0]].constData();

                for (int j=i; j<n; ++j)
                {
                    // if i == j then we double-calculate the energies, so must
                    // scale them by 0.5
                    MultiFloat scale( i == j ? 0.5 : 1.0 );
                
                    //get the bond mask to screen out bonded interactions
                    for (int k=0; k<MultiFloat::count(); ++k)
                    {
                        bond_mask.quickSet(k, row[ida[j][k]] ? 0.0 : 1.0);
                    }
                
                    scale *= bond_mask;
                
                    //calculate the distance between the fixed and mobile atoms
                    tmp = xa[j] - x;
                    r = tmp * tmp;
                    tmp = ya[j] - y;
                    r.multiplyAdd(tmp, tmp);
                    tmp = za[j] - z;
                    r.multiplyAdd(tmp, tmp);
                    r = r.sqrt();

                    one_over_r = r.reciprocal();
            
                    //calculate the coulomb energy using shift-electrostatics
                    // energy = q0q1 * { 1/r - 1/Rc + 1/Rc^2 [r - Rc] }
                    tmp = r - Rc;
                    tmp *= one_over_Rc2;
                    tmp -= one_over_Rc;
                    tmp += one_over_r;
                    tmp *= q * qa[j];
                
                    //apply the cutoff - compare r against Rc. This will
                    //return 1 if r is less than Rc, or 0 otherwise. Logical
                    //and will then remove all energies where r >= Rc
                    tmp &= r.compareLess(Rc);

                    //make sure that the ID of atoms1 is not zero, and is
                    //also not the same as the atoms0 and that we are not
                    //including the energy of the atom with itself
                    itmp = ida[j].compareEqual(dummy_id);
                    itmp |= ida[j].compareEqual(id);

                    icnrg += scale * tmp.logicalAndNot(itmp);
                }
            }
        }
    }
    
    return icnrg.sum();
}

/** Return the intramolecular coulomb energy between the atoms in 'atoms0' and 'atom1' */
double CLJIntraShiftFunction::calcVacCoulombEnergyAri(const CLJAtoms &atoms0,
                                                      const CLJAtoms &atoms1,
                                                      float min_distance) const
{
    const MultiFloat *x0 = atoms0.x().constData();
    const MultiFloat *y0 = atoms0.y().constData();
    const MultiFloat *z0 = atoms0.z().constData();
    const MultiFloat *q0 = atoms0.q().constData();
    const MultiInt *id0 = atoms0.ID().constData();

    const MultiFloat *x1 = atoms1.x().constData();
    const MultiFloat *y1 = atoms1.y().constData();
    const MultiFloat *z1 = atoms1.z().constData();
    const MultiFloat *q1 = atoms1.q().constData();
    const MultiInt *id1 = atoms1.ID().constData();
    
    const MultiFloat Rc(coul_cutoff);
    const MultiFloat one_over_Rc( 1.0 / coul_cutoff );
    const MultiFloat one_over_Rc2( 1.0 / (coul_cutoff*coul_cutoff) );
    const MultiFloat half(0.5);
    const MultiInt dummy_id = CLJAtoms::idOfDummy();
    const qint32 dummy_int = dummy_id[0];

    MultiFloat tmp, r, one_over_r;
    MultiDouble icnrg(0);
    MultiInt itmp;

    const int n0 = atoms0.x().count();
    const int n1 = atoms1.x().count();
    
    bool not_bonded = true;
    
    if (min_distance < 5.5)
    {
        not_bonded = isNotBonded(atoms0.ID(), atoms1.ID());
    }
    
    if ( Q_LIKELY(not_bonded) )
    {
        for (int i=0; i<n0; ++i)
        {
            for (int ii=0; ii<MultiFloat::count(); ++ii)
            {
                if (id0[i][ii] != dummy_int)
                {
                    const MultiInt id(id0[i][ii]);
                    const MultiFloat x(x0[i][ii]);
                    const MultiFloat y(y0[i][ii]);
                    const MultiFloat z(z0[i][ii]);
                    const MultiFloat q(q0[i][ii]);

                    for (int j=0; j<n1; ++j)
                    {
                        //calculate the distance between the fixed and mobile atoms
                        tmp = x1[j] - x;
                        r = tmp * tmp;
                        tmp = y1[j] - y;
                        r.multiplyAdd(tmp, tmp);
                        tmp = z1[j] - z;
                        r.multiplyAdd(tmp, tmp);
                        r = r.sqrt();

                        one_over_r = r.reciprocal();
                
                        //calculate the coulomb energy using shift-electrostatics
                        // energy = q0q1 * { 1/r - 1/Rc + 1/Rc^2 [r - Rc] }
                        tmp = r - Rc;
                        tmp *= one_over_Rc2;
                        tmp -= one_over_Rc;
                        tmp += one_over_r;
                        tmp *= q * q1[j];
                    
                        //apply the cutoff - compare r against Rc. This will
                        //return 1 if r is less than Rc, or 0 otherwise. Logical
                        //and will then remove all energies where r >= Rc
                        tmp &= r.compareLess(Rc);

                        //make sure that the ID of atoms1 is not zero
                        itmp = id1[j].compareEqual(dummy_id);

                        icnrg += tmp.logicalAndNot(itmp);
                    }
                }
            }
        }
    }
    else
    {
        MultiFloat bonded_mask(0);
    
        for (int i=0; i<n0; ++i)
        {
            for (int ii=0; ii<MultiFloat::count(); ++ii)
            {
                if (id0[i][ii] != dummy_int)
                {
                    const MultiInt id(id0[i][ii]);
                    const MultiFloat x(x0[i][ii]);
                    const MultiFloat y(y0[i][ii]);
                    const MultiFloat z(z0[i][ii]);
                    const MultiFloat q(q0[i][ii]);

                    const bool *row = bondMatrix().constData()[id[0]].constData();

                    for (int j=0; j<n1; ++j)
                    {
                        //create a mask to cancel out calculations of bonded pairs
                        for (int k=0; k<MultiInt::count(); ++k)
                        {
                            bonded_mask.quickSet(k, row[id1[j][k]] ? 0.0 : 1.0);
                        }

                        //calculate the distance between the fixed and mobile atoms
                        tmp = x1[j] - x;
                        r = tmp * tmp;
                        tmp = y1[j] - y;
                        r.multiplyAdd(tmp, tmp);
                        tmp = z1[j] - z;
                        r.multiplyAdd(tmp, tmp);
                        r = r.sqrt();

                        one_over_r = r.reciprocal();
                
                        //calculate the coulomb energy using shift-electrostatics
                        // energy = q0q1 * { 1/r - 1/Rc + 1/Rc^2 [r - Rc] }
                        tmp = r - Rc;
                        tmp *= one_over_Rc2;
                        tmp -= one_over_Rc;
                        tmp += one_over_r;
                        tmp *= q * q1[j];
                    
                        //apply the cutoff - compare r against Rc. This will
                        //return 1 if r is less than Rc, or 0 otherwise. Logical
                        //and will then remove all energies where r >= Rc
                        tmp &= r.compareLess(Rc);

                        //make sure that the ID of atoms1 is not zero
                        itmp = id1[j].compareEqual(dummy_id);

                        icnrg += bonded_mask * tmp.logicalAndNot(itmp);
                    }
                }
            }
        }
    }
    
    return icnrg.sum();
}

/** Return the intramolecular LJ energy of the atoms in 'atoms' */
double CLJIntraShiftFunction::calcVacLJEnergyAri(const CLJAtoms &atoms) const
{
    const MultiFloat *xa = atoms.x().constData();
    const MultiFloat *ya = atoms.y().constData();
    const MultiFloat *za = atoms.z().constData();
    const MultiFloat *siga = atoms.sigma().constData();
    const MultiFloat *epsa = atoms.epsilon().constData();
    const MultiInt *ida = atoms.ID().constData();

    const MultiFloat Rlj(lj_cutoff);

    const MultiFloat half(0.5);
    const MultiInt dummy_id = CLJAtoms::idOfDummy();
    const qint32 dummy_int = dummy_id[0];

    MultiFloat tmp, r, one_over_r, sig2_over_r2, sig6_over_r6;
    MultiFloat bond_mask;
    MultiDouble iljnrg(0);
    MultiInt itmp;

    int n = atoms.x().count();
    
    for (int i=0; i<n; ++i)
    {
        for (int ii=0; ii<MultiFloat::size(); ++ii)
        {
            if (ida[i][ii] != dummy_int and epsa[i][ii] != 0)
            {
                const MultiInt id( ida[i][ii] );
                const MultiFloat x( xa[i][ii] );
                const MultiFloat y( ya[i][ii] );
                const MultiFloat z( za[i][ii] );
                const MultiFloat sig( siga[i][ii] * siga[i][ii] );
                const MultiFloat eps( epsa[i][ii] );

                const bool *row = bondMatrix().constData()[id[0]].constData();

                for (int j=i; j<n; ++j)
                {
                    // if i == j then we double-calculate the energies, so must
                    // scale them by 0.5
                    MultiFloat scale( i == j ? 0.5 : 1.0 );
                
                    //get the bond mask to screen out bonded interactions
                    for (int k=0; k<MultiFloat::count(); ++k)
                    {
                        bond_mask.quickSet(k, row[ida[j][k]] ? 0.0 : 1.0);
                    }
                
                    scale *= bond_mask;
                
                    //calculate the distance between the fixed and mobile atoms
                    tmp = xa[j] - x;
                    r = tmp * tmp;
                    tmp = ya[j] - y;
                    r.multiplyAdd(tmp, tmp);
                    tmp = za[j] - z;
                    r.multiplyAdd(tmp, tmp);
                    r = r.sqrt();

                    one_over_r = r.reciprocal();

                    tmp = sig + (siga[j]*siga[j]);
                    tmp *= half;
                
                    sig2_over_r2 = tmp * one_over_r;
                    sig2_over_r2 = sig2_over_r2*sig2_over_r2;
                    sig6_over_r6 = sig2_over_r2*sig2_over_r2;
                    sig6_over_r6 = sig6_over_r6*sig2_over_r2;

                    tmp = sig6_over_r6 * sig6_over_r6;
                    tmp -= sig6_over_r6;
                    tmp *= eps;
                    tmp *= epsa[j];

                    //make sure that the ID of atoms1 is not zero, and is
                    //also not the same as the atoms0 and that we are not
                    //including the energy of the atom with itself
                    itmp = ida[j].compareEqual(dummy_id);
                    itmp |= ida[j].compareEqual(id);
                
                    //apply the cutoff - compare r against Rlj. This will
                    //return 1 if r is less than Rlj, or 0 otherwise. Logical
                    //and will then remove all energies where r >= Rlj
                    tmp &= r.compareLess(Rlj);
                    iljnrg += scale * tmp.logicalAndNot(itmp);
                }
            }
        }
    }
    
    return iljnrg.sum();
}

/** Return the intramolecular LJ energy between the atoms in 'atoms0' and 'atom1' */
double CLJIntraShiftFunction::calcVacLJEnergyAri(const CLJAtoms &atoms0, const CLJAtoms &atoms1,
                                                 float min_distance) const
{
    const MultiFloat *x0 = atoms0.x().constData();
    const MultiFloat *y0 = atoms0.y().constData();
    const MultiFloat *z0 = atoms0.z().constData();
    const MultiFloat *sig0 = atoms0.sigma().constData();
    const MultiFloat *eps0 = atoms0.epsilon().constData();
    const MultiInt *id0 = atoms0.ID().constData();

    const MultiFloat *x1 = atoms1.x().constData();
    const MultiFloat *y1 = atoms1.y().constData();
    const MultiFloat *z1 = atoms1.z().constData();
    const MultiFloat *sig1 = atoms1.sigma().constData();
    const MultiFloat *eps1 = atoms1.epsilon().constData();
    const MultiInt *id1 = atoms1.ID().constData();
    
    const MultiFloat Rlj(lj_cutoff);
    const MultiFloat half(0.5);
    const MultiInt dummy_id = CLJAtoms::idOfDummy();
    const qint32 dummy_int = dummy_id[0];

    MultiFloat tmp, r, one_over_r, sig2_over_r2, sig6_over_r6;
    MultiDouble iljnrg(0);
    MultiInt itmp;

    const int n0 = atoms0.x().count();
    const int n1 = atoms1.x().count();
    
    bool not_bonded = true;
    
    if (min_distance < 5.5)
    {
        not_bonded = isNotBonded(atoms0.ID(), atoms1.ID());
    }
    
    if ( Q_LIKELY(not_bonded) )
    {
        for (int i=0; i<n0; ++i)
        {
            for (int ii=0; ii<MultiFloat::count(); ++ii)
            {
                if (id0[i][ii] != dummy_int)
                {
                    const MultiInt id(id0[i][ii]);
                    const MultiFloat x(x0[i][ii]);
                    const MultiFloat y(y0[i][ii]);
                    const MultiFloat z(z0[i][ii]);
                    const MultiFloat sig(sig0[i][ii] * sig0[i][ii]);
                    const MultiFloat eps(eps0[i][ii]);

                    for (int j=0; j<n1; ++j)
                    {
                        //calculate the distance between the fixed and mobile atoms
                        tmp = x1[j] - x;
                        r = tmp * tmp;
                        tmp = y1[j] - y;
                        r.multiplyAdd(tmp, tmp);
                        tmp = z1[j] - z;
                        r.multiplyAdd(tmp, tmp);
                        r = r.sqrt();

                        one_over_r = r.reciprocal();
                
                        //arithmetic combining rules
                        tmp = sig + (sig1[j]*sig1[j]);
                        tmp *= half;
                    
                        sig2_over_r2 = tmp * one_over_r;
                        sig2_over_r2 = sig2_over_r2*sig2_over_r2;
                        sig6_over_r6 = sig2_over_r2*sig2_over_r2;
                        sig6_over_r6 = sig6_over_r6*sig2_over_r2;

                        tmp = sig6_over_r6 * sig6_over_r6;
                        tmp -= sig6_over_r6;
                        tmp *= eps;
                        tmp *= eps1[j];

                        itmp = id1[j].compareEqual(dummy_id);
                    
                        //apply the cutoff - compare r against Rlj. This will
                        //return 1 if r is less than Rlj, or 0 otherwise. Logical
                        //and will then remove all energies where r >= Rlj
                        tmp &= r.compareLess(Rlj);
                        iljnrg += tmp.logicalAndNot(itmp);
                    }
                }
            }
        }
    }
    else
    {
        MultiFloat bonded_mask(0);
    
        for (int i=0; i<n0; ++i)
        {
            for (int ii=0; ii<MultiFloat::count(); ++ii)
            {
                if (id0[i][ii] != dummy_int)
                {
                    const MultiInt id(id0[i][ii]);
                    const MultiFloat x(x0[i][ii]);
                    const MultiFloat y(y0[i][ii]);
                    const MultiFloat z(z0[i][ii]);

                    const MultiFloat sig(sig0[i][ii] * sig0[i][ii]);
                    const MultiFloat eps(eps0[i][ii]);

                    const bool *row = bondMatrix().constData()[id[0]].constData();

                    for (int j=0; j<n1; ++j)
                    {
                        //create a mask to cancel out calculations of bonded pairs
                        for (int k=0; k<MultiInt::count(); ++k)
                        {
                            bonded_mask.quickSet(k, row[id1[j][k]] ? 0.0 : 1.0);
                        }

                        //calculate the distance between the fixed and mobile atoms
                        tmp = x1[j] - x;
                        r = tmp * tmp;
                        tmp = y1[j] - y;
                        r.multiplyAdd(tmp, tmp);
                        tmp = z1[j] - z;
                        r.multiplyAdd(tmp, tmp);
                        r = r.sqrt();

                        one_over_r = r.reciprocal();
                
                        //arithmetic combining rules
                        tmp = sig + (sig1[j]*sig1[j]);
                        tmp *= half;
                    
                        sig2_over_r2 = tmp * one_over_r;
                        sig2_over_r2 = sig2_over_r2*sig2_over_r2;
                        sig6_over_r6 = sig2_over_r2*sig2_over_r2;
                        sig6_over_r6 = sig6_over_r6*sig2_over_r2;

                        tmp = sig6_over_r6 * sig6_over_r6;
                        tmp -= sig6_over_r6;
                        tmp *= eps;
                        tmp *= eps1[j];

                        itmp = id1[j].compareEqual(dummy_id);
                    
                        //apply the cutoff - compare r against Rlj. This will
                        //return 1 if r is less than Rlj, or 0 otherwise. Logical
                        //and will then remove all energies where r >= Rlj
                        tmp &= r.compareLess(Rlj);
                        iljnrg += bonded_mask * tmp.logicalAndNot(itmp);
                    }
                }
            }
        }
    }
    
    return iljnrg.sum();
}
