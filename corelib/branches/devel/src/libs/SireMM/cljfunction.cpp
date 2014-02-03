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

#include "cljfunction.h"

#include "SireMaths/multidouble.h"

#include "SireError/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMM;
using namespace SireMaths;
using namespace SireBase;
using namespace SireStream;

/////////
///////// Implementation of CLJFunction
/////////

static const RegisterMetaType<CLJFunction> r_cljfunc(MAGIC_ONLY, "SireMM::CLJFunction");

QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const CLJFunction &cljfunc)
{
    writeHeader(ds, r_cljfunc, 1);
    
    ds << cljfunc.use_arithmetic << static_cast<const Property&>(cljfunc);
    
    return ds;
}

QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, CLJFunction &cljfunc)
{
    VersionID v = readHeader(ds, r_cljfunc);
    
    if (v == 1)
    {
        ds >> cljfunc.use_arithmetic >> static_cast<Property&>(cljfunc);
    }
    else
        throw version_error(v, "1", r_cljfunc, CODELOC);
    
    return ds;
}

/** Constructor */
CLJFunction::CLJFunction() : Property(), use_arithmetic(true)
{}

/** Copy constructor */
CLJFunction::CLJFunction(const CLJFunction &other)
            : Property(other), use_arithmetic(other.use_arithmetic)
{}

/** Destructor */
CLJFunction::~CLJFunction()
{}

/** Copy assignment operator */
CLJFunction& CLJFunction::operator=(const CLJFunction &other)
{
    use_arithmetic = other.use_arithmetic;
    Property::operator=(other);
    return *this;
}

/** Comparison operator */
bool CLJFunction::operator==(const CLJFunction &other) const
{
    return use_arithmetic == other.use_arithmetic and Property::operator==(other);
}

/** Tell the function to use arithmetic combining rules for LJ parameters */
void CLJFunction::setArithmeticCombiningRules(bool on)
{
    use_arithmetic = on;
}

/** Tell the function to use geometric combining rules for LJ parameters */
void CLJFunction::setGeometricCombiningRules(bool on)
{
    use_arithmetic = not on;
}

/** Return whether or not this function uses a cutoff */
bool CLJFunction::hasCutoff() const
{
    return false;
}

/** Return the coulomb cutoff if this function uses one */
Length CLJFunction::coulombCutoff() const
{
    return Length( std::numeric_limits<double>::max() );
}

/** Return the LJ cutoff if this function uses one */
Length CLJFunction::ljCutoff() const
{
    return Length( std::numeric_limits<double>::max() );
}

/** Return whether or not arithmetic combining rules are used */
bool CLJFunction::usingArithmeticCombiningRules() const
{
    return use_arithmetic;
}

/** Return whether or not geometric combining rules are used */
bool CLJFunction::usingGeometricCombiningRules() const
{
    return not use_arithmetic;
}

/** Calculate the coulomb energy between all atoms in 'atoms' */
double CLJFunction::calcCoulombEnergyAri(const CLJAtoms &atoms) const
{
    double cnrg, ljnrg;
    calcEnergyAri(atoms, cnrg, ljnrg);
    return cnrg;
}

/** Calculate the coulomb energy between the atoms in 'atoms0' and the atoms in 'atoms1' */
double CLJFunction::calcCoulombEnergyAri(const CLJAtoms &atoms0, const CLJAtoms &atoms1) const
{
    double cnrg, ljnrg;
    calcEnergyAri(atoms0, atoms1, cnrg, ljnrg);
    return cnrg;
}

/** Calculate the coulomb energy between all atoms in 'atoms' */
double CLJFunction::calcCoulombEnergyGeo(const CLJAtoms &atoms) const
{
    double cnrg, ljnrg;
    calcEnergyGeo(atoms, cnrg, ljnrg);
    return cnrg;
}

/** Calculate the coulomb energy between the atoms in 'atoms0' and the atoms in 'atoms1' */
double CLJFunction::calcCoulombEnergyGeo(const CLJAtoms &atoms0, const CLJAtoms &atoms1) const
{
    double cnrg, ljnrg;
    calcEnergyGeo(atoms0, atoms1, cnrg, ljnrg);
    return cnrg;
}

/** Calculate the LJ energy between all atoms in 'atoms' */
double CLJFunction::calcLJEnergyAri(const CLJAtoms &atoms) const
{
    double cnrg, ljnrg;
    calcEnergyAri(atoms, cnrg, ljnrg);
    return ljnrg;
}

/** Calculate the LJ energy between the atoms in 'atoms0' and the atoms in 'atoms1' */
double CLJFunction::calcLJEnergyAri(const CLJAtoms &atoms0, const CLJAtoms &atoms1) const
{
    double cnrg, ljnrg;
    calcEnergyAri(atoms0, atoms1, cnrg, ljnrg);
    return ljnrg;
}

/** Calculate the LJ energy between all atoms in 'atoms' */
double CLJFunction::calcLJEnergyGeo(const CLJAtoms &atoms) const
{
    double cnrg, ljnrg;
    calcEnergyGeo(atoms, cnrg, ljnrg);
    return ljnrg;
}

/** Calculate the LJ energy between the atoms in 'atoms0' and the atoms in 'atoms1' */
double CLJFunction::calcLJEnergyGeo(const CLJAtoms &atoms0, const CLJAtoms &atoms1) const
{
    double cnrg, ljnrg;
    calcEnergyGeo(atoms0, atoms1, cnrg, ljnrg);
    return ljnrg;
}

void CLJFunction::operator()(const CLJAtoms &atoms,
                             double &cnrg, double &ljnrg) const
{
    if (atoms.isEmpty())
    {
        cnrg = 0;
        ljnrg = 0;
    }
    else
    {
        if (use_arithmetic)
            this->calcEnergyAri(atoms, cnrg, ljnrg);
        else
            this->calcEnergyGeo(atoms, cnrg, ljnrg);
    }
}

void CLJFunction::operator()(const CLJAtoms &atoms0, const CLJAtoms &atoms1,
                             double &cnrg, double &ljnrg) const
{
    if (atoms0.isEmpty() or atoms1.isEmpty())
    {
        cnrg = 0;
        ljnrg = 0;
        return;
    }
    else if (atoms0.count() > atoms1.count())
    {
        if (use_arithmetic)
            this->calcEnergyAri(atoms1, atoms0, cnrg, ljnrg);
        else
            this->calcEnergyGeo(atoms1, atoms0, cnrg, ljnrg);
    }
    else
    {
        if (use_arithmetic)
            this->calcEnergyAri(atoms0, atoms1, cnrg, ljnrg);
        else
            this->calcEnergyGeo(atoms0, atoms1, cnrg, ljnrg);
    }
}

/** Return the total energy between 'atoms', returning the coulomb part in 'cnrg'
    and the LJ part in 'ljnrg' */
void CLJFunction::total(const CLJAtoms &atoms, double &cnrg, double &ljnrg) const
{
    return this->operator()(atoms, cnrg, ljnrg);
}

/** Return the total energy between 'atoms0' and 'atoms1', returning the coulomb part in 'cnrg'
    and the LJ part in 'ljnrg' */
void CLJFunction::total(const CLJAtoms &atoms0, const CLJAtoms &atoms1,
                        double &cnrg, double &ljnrg) const
{
    return this->operator()(atoms0, atoms1, cnrg, ljnrg);
}

/** Return the coulomb energy between the atoms in 'atoms' */
double CLJFunction::coulomb(const CLJAtoms &atoms) const
{
    if (atoms.isEmpty())
    {
        return 0;
    }
    else
    {
        if (use_arithmetic)
            return this->calcCoulombEnergyAri(atoms);
        else
            return this->calcCoulombEnergyGeo(atoms);
    }
}

/** Return the coulomb energy between the atoms in 'atoms0' and in 'atoms1' */
double CLJFunction::coulomb(const CLJAtoms &atoms0, const CLJAtoms &atoms1) const
{
    if (atoms0.isEmpty() or atoms1.isEmpty())
    {
        return 0;
    }
    else
    {
        if (use_arithmetic)
            return this->calcCoulombEnergyAri(atoms0, atoms1);
        else
            return this->calcCoulombEnergyGeo(atoms0, atoms1);
    }
}

/** Return the LJ energy between the atoms in 'atoms' */
double CLJFunction::lj(const CLJAtoms &atoms) const
{
    if (atoms.isEmpty())
    {
        return 0;
    }
    else
    {
        if (use_arithmetic)
            return this->calcLJEnergyAri(atoms);
        else
            return this->calcLJEnergyGeo(atoms);
    }
}

/** Return the LJ energy between the atoms in 'atoms0' and in 'atoms1' */
double CLJFunction::lj(const CLJAtoms &atoms0, const CLJAtoms &atoms1) const
{
    if (atoms0.isEmpty() or atoms1.isEmpty())
    {
        return 0;
    }
    else
    {
        if (use_arithmetic)
            return this->calcLJEnergyAri(atoms0, atoms1);
        else
            return this->calcLJEnergyGeo(atoms0, atoms1);
    }
}

/////////
///////// Implementation of CLJVacShiftAriFunction
/////////

static const RegisterMetaType<CLJVacShiftAriFunction> r_cljvacshiftari;

QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const CLJVacShiftAriFunction &func)
{
    writeHeader(ds, r_cljvacshiftari, 1);
    
    ds << func.coul_cutoff << func.lj_cutoff
       << static_cast<const CLJFunction&>(func);
    
    return ds;
}

QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, CLJVacShiftAriFunction &func)
{
    VersionID v = readHeader(ds, r_cljvacshiftari);
    
    if (v == 1)
    {
        ds >> func.coul_cutoff >> func.lj_cutoff
           >> static_cast<CLJFunction&>(func);
    }
    else
        throw version_error(v, "1", r_cljvacshiftari, CODELOC);
    
    return ds;
}

/** Constructor */
CLJVacShiftAriFunction::CLJVacShiftAriFunction()
                       : ConcreteProperty<CLJVacShiftAriFunction,CLJFunction>(),
                         coul_cutoff(15), lj_cutoff(15)
{}

/** Construct, specifying the length for the coulomb and LJ cutoff */
CLJVacShiftAriFunction::CLJVacShiftAriFunction(Length cutoff)
                       : ConcreteProperty<CLJVacShiftAriFunction,CLJFunction>(),
                         coul_cutoff(cutoff.value()), lj_cutoff(cutoff.value())
{
    if (coul_cutoff < 0)
    {
        coul_cutoff = 0;
        lj_cutoff = 0;
    }
}

/** Construct, specifying the length of the coulomb and LJ cutoff */
CLJVacShiftAriFunction::CLJVacShiftAriFunction(Length coul, Length lj)
                       : ConcreteProperty<CLJVacShiftAriFunction,CLJFunction>(),
                         coul_cutoff(coul.value()), lj_cutoff(lj.value())
{
    if (coul_cutoff < 0)
        coul_cutoff = 0;
    
    if (lj_cutoff < 0)
        lj_cutoff = 0;
    
    if (coul_cutoff < lj_cutoff)
    {
        throw SireError::incompatible_error( QObject::tr(
                "The CLJVacShiftAriFunction does not support having a Coulomb cutoff "
                "(%1 A) that is shorter than the Lennard Jones cutoff (%2 A).")
                    .arg(coul_cutoff).arg(lj_cutoff), CODELOC );
    }
}

/** Copy constructor */
CLJVacShiftAriFunction::CLJVacShiftAriFunction(const CLJVacShiftAriFunction &other)
                       : ConcreteProperty<CLJVacShiftAriFunction,CLJFunction>(other),
                         coul_cutoff(other.coul_cutoff), lj_cutoff(other.lj_cutoff)
{}

/** Destructor */
CLJVacShiftAriFunction::~CLJVacShiftAriFunction()
{}

/** Copy assignment operator */
CLJVacShiftAriFunction& CLJVacShiftAriFunction::operator=(const CLJVacShiftAriFunction &other)
{
    coul_cutoff = other.coul_cutoff;
    lj_cutoff = other.lj_cutoff;
    return *this;
}

/** Comparison operator */
bool CLJVacShiftAriFunction::operator==(const CLJVacShiftAriFunction &other) const
{
    return coul_cutoff == other.coul_cutoff and
           lj_cutoff == other.lj_cutoff;
}

/** Comparison operator */
bool CLJVacShiftAriFunction::operator!=(const CLJVacShiftAriFunction &other) const
{
    return not operator==(other);
}

const char* CLJVacShiftAriFunction::typeName()
{
    return QMetaType::typeName( qMetaTypeId<CLJVacShiftAriFunction>() );
}

const char* CLJVacShiftAriFunction::what() const
{
    return CLJVacShiftAriFunction::typeName();
}

CLJVacShiftAriFunction* CLJVacShiftAriFunction::clone() const
{
    return new CLJVacShiftAriFunction(*this);
}

/** Return that this function does indeed use a cutoff */
bool CLJVacShiftAriFunction::hasCutoff() const
{
    return true;
}

/** Return the coulomb cutoff */
Length CLJVacShiftAriFunction::coulombCutoff() const
{
    return Length(coul_cutoff);
}

/** Return the LJ cutoff */
Length CLJVacShiftAriFunction::ljCutoff() const
{
    return Length(lj_cutoff);
}

/** Calculate the coulomb and LJ intermolecular energy of all of the atoms in 'atoms',
    returning the results in the arguments 'cnrg' and 'ljnrg' */
void CLJVacShiftAriFunction::calcEnergyGeo(const CLJAtoms &atoms,
                                           double &cnrg, double &ljnrg) const
{
    cnrg = 0;
    ljnrg = 0;
}

/** Calculate the intermolecular energy between all atoms in 'atoms0' and all
    atoms in 'atoms1', returning the result in the arguments 'cnrg' and 'ljnrg' */
void CLJVacShiftAriFunction::calcEnergyGeo(const CLJAtoms &atoms0, const CLJAtoms &atoms1,
                                           double &cnrg, double &ljnrg) const
{
    cnrg = 0;
    ljnrg = 0;
}

/** Calculate the coulomb and LJ intermolecular energy of all of the atoms in 'atoms',
    returning the results in the arguments 'cnrg' and 'ljnrg' */
void CLJVacShiftAriFunction::calcEnergyAri(const CLJAtoms &atoms,
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
void CLJVacShiftAriFunction::calcEnergyAri(const CLJAtoms &atoms0, const CLJAtoms &atoms1,
                                           double &cnrg, double &ljnrg) const
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
