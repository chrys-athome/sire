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
    ds << static_cast<const Property&>(cljfunc);
    return ds;
}

QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, CLJFunction &cljfunc)
{
    VersionID v = readHeader(ds, r_cljfunc);
    
    if (v == 1)
    {
        ds >> static_cast<Property&>(cljfunc);
    }
    else
        throw version_error(v, "1", r_cljfunc, CODELOC);
    
    return ds;
}

/** Constructor */
CLJFunction::CLJFunction() : Property()
{}

/** Copy constructor */
CLJFunction::CLJFunction(const CLJFunction &other) : Property(other)
{}

/** Destructor */
CLJFunction::~CLJFunction()
{}

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

/** Calculate the coulomb and LJ intermolecular energy of all of the atoms in 'atoms',
    returning the results in the arguments 'cnrg' and 'ljnrg' */
void CLJVacShiftAriFunction::operator()(const CLJAtoms &atoms,
                                        double &cnrg, double &ljnrg) const
{
    cnrg = 0;
    ljnrg = 0;
}

/** Calculate the intermolecular energy between all atoms in 'atoms0' and all
    atoms in 'atoms1', returning the result in the arguments 'cnrg' and 'ljnrg' */
void CLJVacShiftAriFunction::operator()(const CLJAtoms &atoms0, const CLJAtoms &atoms1,
                                        double &cnrg, double &ljnrg) const
{
    const MultiFloat *x0 = atoms0.x().constData();
    const MultiFloat *y0 = atoms0.y().constData();
    const MultiFloat *z0 = atoms0.z().constData();
    const MultiFloat *q0 = atoms0.q().constData();
    const MultiFloat *sig0 = atoms0.sigma().constData();
    const MultiFloat *eps0 = atoms0.epsilon().constData();
    const MultiUInt *id0 = atoms0.ID().constData();

    const MultiFloat *x1 = atoms1.x().constData();
    const MultiFloat *y1 = atoms1.y().constData();
    const MultiFloat *z1 = atoms1.z().constData();
    const MultiFloat *q1 = atoms1.q().constData();
    const MultiFloat *sig1 = atoms1.sigma().constData();
    const MultiFloat *eps1 = atoms1.epsilon().constData();
    const MultiUInt *id1 = atoms1.ID().constData();
    
    const MultiFloat Rc(coul_cutoff);
    const MultiFloat Rlj(lj_cutoff);
    const MultiFloat one_over_Rc( 1.0 / coul_cutoff );
    const MultiFloat one_over_Rc2( 1.0 / (coul_cutoff*coul_cutoff) );
    const MultiFloat zero(0);
    const MultiFloat half(0.5);

    MultiFloat tmp, r, one_over_r, sig2_over_r2, sig6_over_r6;
    MultiDouble icnrg(0), iljnrg(0);

    for (int i=0; i<atoms0.x().count(); ++i)
    {
        for (int ii=0; ii<MultiFloat::count(); ++ii)
        {
            //if (id0[i][ii] != 0)
            {
                if (q0[i][ii] != 0)
                {
                    const MultiFloat x(x0[i][ii]);
                    const MultiFloat y(y0[i][ii]);
                    const MultiFloat z(z0[i][ii]);
                    const MultiFloat q(q0[i][ii]);
                
                    if (eps0[i][ii] == 0)
                    {
                        //coulomb energy only
                        for (int j=0; j<atoms1.x().count(); ++j)
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
                            icnrg += tmp.logicalAnd( r.compareLess(Rc) );
                        }
                    }
                    else
                    {
                        const MultiFloat sig(sig0[i][ii] * sig0[i][ii]);
                        const MultiFloat eps(eps0[i][ii]);

                        for (int j=0; j<atoms1.x().count(); ++j)
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
                            icnrg += tmp.logicalAnd( r.compareLess(Rc) );
                    
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
                            iljnrg += tmp.logicalAnd( r.compareLess(Rlj) );
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

                    for (int j=0; j<atoms1.x().count(); ++j)
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
                        iljnrg += tmp.logicalAnd( r.compareLess(Rlj) );
                    }
                }
            }
        }
    }
    
    cnrg = icnrg.sum();
    ljnrg = iljnrg.sum();
}
