/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2009  Christopher Woods
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

#include "pgto.h"
#include "pointcharge.h"
#include "pointdipole.h"

#include "SireMaths/boys.h"

#include "SireBase/trigarray2d.hpp"

#include "SireError/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace Squire;
using namespace SireMaths;
using namespace SireBase;
using namespace SireStream;

//////////
////////// Implementation of P_GTO
//////////

static const RegisterMetaType<P_GTO> r_pgto;

/** Serialise to a binary datastream */
QDataStream SQUIRE_EXPORT &operator<<(QDataStream &ds, const P_GTO &p)
{
    writeHeader(ds, r_pgto, 1);
    
    ds << static_cast<const GTO&>(p);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SQUIRE_EXPORT &operator>>(QDataStream &ds, P_GTO &p)
{
    VersionID v = readHeader(ds, r_pgto);
    
    if (v == 1)
    {
        ds >> static_cast<GTO&>(p);
    }
    else
        throw version_error(v, "1", r_pgto, CODELOC);
        
    return ds;
}

/** Constructor */
P_GTO::P_GTO() : ConcreteProperty<P_GTO,GTO>()
{}

/** Construct with the specified alpha value and scaling factor */
P_GTO::P_GTO(double alpha, double scale)
      : ConcreteProperty<P_GTO,GTO>(alpha, scale)
{}

/** Copy constructor */
P_GTO::P_GTO(const P_GTO &other) : ConcreteProperty<P_GTO,GTO>(other)
{}

/** Destructor */
P_GTO::~P_GTO()
{}

const char* P_GTO::typeName()
{
    return QMetaType::typeName( qMetaTypeId<P_GTO>() );
}

/** Copy assignment operator */
P_GTO& P_GTO::operator=(const P_GTO &other)
{
    GTO::operator=(other);
    return *this;
}

/** Comparison operator */
bool P_GTO::operator==(const P_GTO &other) const
{
    return GTO::operator==(other);
}

/** Comparison operator */
bool P_GTO::operator!=(const P_GTO &other) const
{
    return GTO::operator!=(other);
}

/** The angular momentum of P-GTOs is 1 */
int P_GTO::angularMomentum() const
{
    return 1;
}

/** There are 3 P-orbitals per shell (px, py and pz) */
int P_GTO::nOrbitals() const
{
    return 3;
}

//////////
////////// Implementation of PS_GTO
//////////

static const RegisterMetaType<PS_GTO> r_psgto;

/** Serialise to a binary datastream */
QDataStream SQUIRE_EXPORT &operator<<(QDataStream &ds, const PS_GTO &ps)
{
    writeHeader(ds, r_psgto, 1);
    
    ds << ps.p_minus_a << ps.norm_scl << static_cast<const GTOPair&>(ps);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SQUIRE_EXPORT &operator>>(QDataStream &ds, PS_GTO &ps)
{
    VersionID v = readHeader(ds, r_psgto);
    
    if (v == 1)
    {
        ds >> ps.p_minus_a >> ps.norm_scl >> static_cast<GTOPair&>(ps);
    }
    else
        throw version_error(v, "1", r_psgto, CODELOC);
        
    return ds;
}

/** Constructor */
PS_GTO::PS_GTO() 
       : ConcreteProperty<PS_GTO,GTOPair>(), norm_scl(0)
{}

/** Construct combining orbital 'a' at position 'A' with orbital 'b' at
    position 'B' */
PS_GTO::PS_GTO(const Vector &A, const P_GTO &a,
               const Vector &B, const S_GTO &b)
       : ConcreteProperty<PS_GTO,GTOPair>(A, a, B, b)
{
    p_minus_a = P() - A;
    norm_scl = std::sqrt(4 * a.alpha());
}

/** Construct combining orbital 'a' at position 'A' with orbital 'b' at
    position 'B' */
PS_GTO::PS_GTO(const Vector &A, const S_GTO &a,
               const Vector &B, const P_GTO &b)
       : ConcreteProperty<PS_GTO,GTOPair>(B, b, A, a)
{
    p_minus_a = P() - B;
    norm_scl = std::sqrt(4 * b.beta());
}
  
/** Copy constructor */     
PS_GTO::PS_GTO(const PS_GTO &other)
       : ConcreteProperty<PS_GTO,GTOPair>(other),
         p_minus_a(other.p_minus_a), norm_scl(other.norm_scl)
{}

/** Destructor */
PS_GTO::~PS_GTO()
{}

const char* PS_GTO::typeName()
{
    return QMetaType::typeName( qMetaTypeId<PS_GTO>() );
}

/** Copy assignment operator */
PS_GTO& PS_GTO::operator=(const PS_GTO &other)
{
    if (this != &other)
    {
        GTOPair::operator=(other);
        p_minus_a = other.p_minus_a;
        norm_scl = other.norm_scl;
    }
    
    return *this;
}

/** Comparison operator */
bool PS_GTO::operator==(const PS_GTO &other) const
{
    return GTOPair::operator==(other) and p_minus_a == other.p_minus_a
             and norm_scl == other.norm_scl;
}

/** Comparison operator */
bool PS_GTO::operator!=(const PS_GTO &other) const
{
    return not this->operator==(other);
}

/** Return the angular momentum of the first GTO shell in this pair */
int PS_GTO::angularMomentum0() const
{
    return 1;
}

/** Return the angular momentum of the second GTO shell in this pair */
int PS_GTO::angularMomentum1() const
{
    return 0;
}

/** Return the number of orbitals in the first GTO shell in this pair */
int PS_GTO::nOrbitals0() const
{
    return 3;
}

/** Return the number of orbitals in the second GTO shell in this pair */
int PS_GTO::nOrbitals1() const
{
    return 1;
}

//////////
////////// Implementation of PP_GTO
//////////

static const RegisterMetaType<PP_GTO> r_ppgto;

/** Serialise to a binary datastream */
QDataStream SQUIRE_EXPORT &operator<<(QDataStream &ds, const PP_GTO &pp)
{
    writeHeader(ds, r_ppgto, 1);
    
    ds << pp.p_minus_a << pp.p_minus_b << pp.norm_scl
       << static_cast<const GTOPair&>(pp);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SQUIRE_EXPORT &operator>>(QDataStream &ds, PP_GTO &pp)
{
    VersionID v = readHeader(ds, r_ppgto);
    
    if (v == 1)
    {
        ds >> pp.p_minus_a >> pp.p_minus_b >> pp.norm_scl
           >> static_cast<GTOPair&>(pp);
    }
    else
        throw version_error(v, "1", r_ppgto, CODELOC);
        
    return ds;
}

/** Constructor */
PP_GTO::PP_GTO() : ConcreteProperty<PP_GTO,GTOPair>(), norm_scl(0)
{}

/** Construct combining orbital 'a' at position 'A' with orbital 'b' at
    position 'B' */
PP_GTO::PP_GTO(const Vector &A, const P_GTO &a,
               const Vector &B, const P_GTO &b)
       : ConcreteProperty<PP_GTO,GTOPair>(A, a, B, b)
{
    p_minus_a = P() - A;
    p_minus_b = P() - B;
    
    norm_scl = std::sqrt( 16 * a.alpha() * b.beta() );
}
  
/** Copy constructor */     
PP_GTO::PP_GTO(const PP_GTO &other)
       : ConcreteProperty<PP_GTO,GTOPair>(other),
         p_minus_a(other.p_minus_a), p_minus_b(other.p_minus_b), 
         norm_scl(other.norm_scl)
{}

/** Destructor */
PP_GTO::~PP_GTO()
{}

const char* PP_GTO::typeName()
{
    return QMetaType::typeName( qMetaTypeId<PP_GTO>() );
}

/** Copy assignment operator */
PP_GTO& PP_GTO::operator=(const PP_GTO &other)
{
    if (this != &other)
    {
        GTOPair::operator=(other);
        p_minus_a = other.p_minus_a;
        p_minus_b = other.p_minus_b;
        norm_scl = other.norm_scl;
    }
    
    return *this;
}

/** Comparison operator */
bool PP_GTO::operator==(const PP_GTO &other) const
{
    return GTOPair::operator==(other) and p_minus_a == other.p_minus_a and
           p_minus_b == other.p_minus_b and norm_scl == other.norm_scl;
}

/** Comparison operator */
bool PP_GTO::operator!=(const PP_GTO &other) const
{
    return not this->operator==(other);
}

/** Return the vector from the center of the first P orbital shell
    to the center of mass of the combined gaussian */
const Vector& PP_GTO::P_minus_A() const
{
    return p_minus_a;
}

/** Return the vector from the center of the second P orbital shell
    to the center of mass of the combined gaussian */
const Vector& PP_GTO::P_minus_B() const
{
    return p_minus_b;
}

/** Synonym for P_minus_A */
const Vector& PP_GTO::Q_minus_C() const
{
    return PP_GTO::P_minus_A();
}

/** Synonym for Q_minus_D */
const Vector& PP_GTO::Q_minus_D() const
{
    return PP_GTO::P_minus_B();
}

/** Return the additional scaling constant needed to normalise the
    integrals involving this shell-pair */
double PP_GTO::scale() const
{
    return norm_scl;
}

/** Return the angular momentum of the first GTO shell in this pair */
int PP_GTO::angularMomentum0() const
{
    return 1;
}

/** Return the angular momentum of the second GTO shell in this pair */
int PP_GTO::angularMomentum1() const
{
    return 1;
}

/** Return the number of orbitals in the first GTO shell in this pair */
int PP_GTO::nOrbitals0() const
{
    return 3;
}

/** Return the number of orbitals in the second GTO shell in this pair */
int PP_GTO::nOrbitals1() const
{
    return 3;
}

//////////
////////// Integrals
//////////

namespace Squire
{

// Integrals derived using Obara-Saika recursion - see Mathematica
// notebook (techdocs/obara_saika.nb) for details

//////
////// The overlap, kinetic and potential integrals
//////

/** Return the overlap integral (p||s) */
Vector SQUIRE_EXPORT overlap_integral(const PS_GTO &ps)
{
    // (pi||s) = (Pi - Ai)(s||s)
    return (ps.scale() * ps.ss()) * ps.P_minus_A();
}

/** Return the kinetic energy integral, (p|nabla|s) */
Vector SQUIRE_EXPORT kinetic_integral(const PS_GTO &ps)
{
    // (p|nabla|s) = ss xi (5 - 2 R^2 xi) (Pi-Ai)
    return ( ps.scale() * ps.ss() * ps.xi() * 
              (5 - 2*ps.R2()*ps.xi()) ) * ps.P_minus_A();
}

/** Return the potential energy integral with an array of point charges,
    (p|C|s) */
Vector SQUIRE_EXPORT potential_integral(const QVector<PointCharge> &C, const PS_GTO &P)
{
    // (p|C|s) = 2 ss Sqrt[zeta/pi] ( F0(U) (Pi-Ai) - F1(U) (Pi-Ci) )
    // U = zeta * (P-C)^2

    const int nc = C.count();
    const PointCharge *c = C.constData();

    Vector total;
    
    for (int i=0; i<nc; ++i)
    {
        const Vector P_minus_C = P.P() - c[i].center();
        const double U = P.zeta() * P_minus_C.length2();

        double boys[2];
        multi_boys_2(U, boys);

        total += c[i].charge() * (boys[0] * P.P_minus_A() - boys[1] * P_minus_C);
    }

    return (-2 * P.scale() * P.ss() * std::sqrt(P.zeta() * one_over_pi)) * total;
}

/** Return the potential energy integral with a single point charge, (p|C|s) */
Vector SQUIRE_EXPORT potential_integral(const PointCharge &C, const PS_GTO &ps)
{
    QVector<PointCharge> Cs(1, C);
    return potential_integral(Cs, ps);
}

/** Return the potential energy integral with an array of point charges,
    (p|C|s)^m */
Vector SQUIRE_EXPORT potential_integral(const QVector<PointCharge> &C, 
                                        const PS_GTO &P, int m)
{
    // (p|C|s)^m = 2 ss Sqrt[zeta/pi] ( Fm(U) (Pi-Ai) - Fm+1(U) (Pi-Ci) )
    // U = zeta * (P-C)^2

    const int nc = C.count();
    const PointCharge *c = C.constData();

    Vector total;
    
    for (int i=0; i<nc; ++i)
    {
        const Vector P_minus_C = P.P() - c[i].center();
        const double U = P.zeta() * P_minus_C.length2();

        double boys[2];
        multi_boys_2(U, boys, m);

        total += c[i].charge() * (boys[0] * P.P_minus_A() - boys[1] * P_minus_C);
    }

    return (-2 * P.scale() * P.ss() * std::sqrt(P.zeta() * one_over_pi)) * total;
}

/** Return the potential energy integral with a single point charge, (p|C|s)^m */
Vector SQUIRE_EXPORT potential_integral(const PointCharge &C, 
                                        const PS_GTO &ps, int m)
{
    QVector<PointCharge> Cs(1, C);
    return potential_integral(Cs, ps, m);
}

/** Return the potential energy integral with an array of point dipoles, (p|C|s) */
Vector SQUIRE_EXPORT potential_integral(const QVector<PointDipole> &C,
                                        const PS_GTO &ps)
{
    throw SireError::incomplete_code( "Not implemented", CODELOC );
    return Vector();
}

/** Return the potential energy integral with a single point dipole, (p|C|s) */
Vector SQUIRE_EXPORT potential_integral(const PointDipole &C, const PS_GTO &ps)
{
    QVector<PointDipole> Cs(1, C);
    return potential_integral(Cs, ps);
}

/** Return the potential energy integral with an array of point dipoles, (p|C|s)^m */
Vector SQUIRE_EXPORT potential_integral(const QVector<PointDipole> &C,
                                        const PS_GTO &ps, int m)
{
    throw SireError::incomplete_code( "Not implemented", CODELOC );
    return Vector();
}

/** Return the potential energy integral with a single point dipole, (p|C|s)^m */
Vector SQUIRE_EXPORT potential_integral(const PointDipole &C, 
                                        const PS_GTO &ps, int m)
{
    QVector<PointDipole> Cs(1, C);
    return potential_integral(Cs, ps, m);
}

/** Return the kinetic energy integral (p|nabla|p) */
Matrix SQUIRE_EXPORT kinetic_integral(const PP_GTO &pp)
{
    // (p|k|p) = -(1/2) (s||s) xi ( 2 (-7 + 2 R^2 xi) (Pi-Ai) (Pj-Bj) + 
    //                              delta_ij zeta (-5 + 2 R^2 xi) ) 

    const double prefac0 = -0.5 * pp.scale() * pp.ss() * pp.xi();
                            
    
    const double prefac1 = prefac0 * ( 2 * (-7 + 2*pp.R2()*pp.xi()) );
    
    Matrix mat;
    double *m = mat.data();

    const double *pa = pp.P_minus_A().constData();
    const double *pb = pp.P_minus_B().constData();
                                                                
    //do the off-diagonal elements
    for (int i=0; i<2; ++i)
    {
        const double pa_prefac = pa[i] * prefac1;
    
        for (int j=i+1; j<3; ++j)
        {
            const double val = pa_prefac * pb[j];
            m[ mat.offset(i,j) ] = val;
            m[ mat.offset(j,i) ] = val;
        }
    }
    
    //do the diagonal elements
    const double extra = prefac0 *
                           pp.zeta() * (-5 + 2 * pp.R2() * pp.xi());
    
    for (int i=0; i<3; ++i)
    {
        m[ mat.offset(i,i) ] = (prefac1 * pa[i] * pb[i]) + extra;
    }
    
    return mat;
}

/** Return the overlap integral (p||p) */
Matrix SQUIRE_EXPORT overlap_integral(const PP_GTO &pp)
{
    // (p||p) = ss (Pi-Ai) (Pj-Bj) + delta_ij (1/2 ss zeta) 
    Matrix mat;
    
    const double prefac = pp.scale() * pp.ss();
        
    const double *pa = pp.P_minus_A().constData();
    const double *pb = pp.P_minus_B().constData();
   
    double *m = mat.data();
    
    //do the off-diagonal elements
    for (int i=0; i<2; ++i)
    {
        const double prefac_pa = prefac * pa[i];
    
        for (int j=i+1; j<3; ++j)
        {
            const double val = prefac_pa * pb[j];
            m[ mat.offset(i,j) ] = val;
            m[ mat.offset(j,i) ] = val;
        }
    }

    //do the diagonal elements
    const double extra = 1 / (2 * pp.zeta());

    for (int i=0; i<3; ++i)
    {
        m[ mat.offset(i,i) ] = prefac * (pa[i]*pb[i] + extra);
    }

    return mat;
}

/** Return the potential integral with an array of point charges, (p|C|p) */
Matrix SQUIRE_EXPORT potential_integral(const QVector<PointCharge> &C, 
                                        const PP_GTO &P)
{
    // (p|C|p) = -2 * Q * ss * Sqrt[zeta/pi] * {
    //                             F0(U) (Pi-Ai)(Pj-Bj) +
    //                             F2(U) (Pi-Ci)(Pj-Cj) - 
    //                             F1(U)[ (Pj-Bj)(Pi-Ci) + (Pi-Ai)(Pj-Cj) ]
    //                                     + delta_ij( 0.5*zeta( F0(U) - F1(U) ) }

    // U = zeta * (P-C)^2

    const int nc = C.count();
    const PointCharge *c = C.constData();
    
    const double prefac = -2 * P.scale() * P.ss() * std::sqrt(P.zeta()*one_over_pi);
    
    Matrix mat(0);
    double *m = mat.data();
    
    const double *pa = P.P_minus_A().constData();
    const double *pb = P.P_minus_B().constData();
    
    for (int ic=0; ic<nc; ++ic)
    {
        const double U = P.zeta() * Vector::distance2(P.P(), c[ic].center());
        
        double boys[3];
        multi_boys_3(U, boys);
        
        const double q_prefac = c[ic].charge() * prefac;
        const Vector P_minus_C = P.P() - c[ic].center();
        const double *pc = P_minus_C.constData();
        
        //do the off-diagonal elements
        for (int i=0; i<2; ++i)
        {
            for (int j=i+1; j<3; ++j)
            {
                const double val = q_prefac * 
                                    ( boys[0]*pa[i]*pb[j] +
                                      boys[2]*pc[i]*pc[j] -
                                      boys[1]*(pb[j]*pc[i] + pa[i]*pc[j]) );
                
                m[ mat.offset(i,j) ] += val;
            }
        }
        
        //do the diagonal elements
        const double extra = 0.5 * P.zeta() * (boys[0] - boys[1]);
        
        for (int i=0; i<3; ++i)
        {
            m[ mat.offset(i,i) ] = q_prefac * 
                                    ( boys[0]*pa[i]*pb[i] +
                                      boys[2]*pc[i]*pc[i] -
                                      boys[1]*(pb[i]*pc[i] + pa[i]*pc[i]) + extra );
        }
    }

    //now copy mat[i,j] to mat[j,i]
    for (int i=0; i<2; ++i)
    {
        for (int j=i+1; j<3; ++j)
        {
            m[ mat.offset(j,i) ] = m[ mat.offset(i,j) ];
        }
    }
    
    return mat;
}

/** Return the potential integral with a single point charge (p|C|p) */
Matrix SQUIRE_EXPORT potential_integral(const PointCharge &C, const PP_GTO &P)
{
    QVector<PointCharge> Cs(1, C);
    return potential_integral(Cs, P);
}

/** Return the potential integral with an array of point charges, (p|C|p)^m */
Matrix SQUIRE_EXPORT potential_integral(const QVector<PointCharge> &C, 
                                        const PP_GTO &P, int M)
{
    // (p|C|p)^m = -2 * Q * ss * Sqrt[zeta/pi] * {
    //                             Fm(U) (Pi-Ai)(Pj-Bj) +
    //                             Fm+2(U) (Pi-Ci)(Pj-Cj) - 
    //                             Fm+1(U)[ (Pj-Bj)(Pi-Ci) + (Pi-Ai)(Pj-Cj) ]
    //                                       + delta_ij( 0.5*zeta( F0(U) - F1(U) ) }

    // U = zeta * (P-C)^2

    const int nc = C.count();
    const PointCharge *c = C.constData();
    
    const double prefac = -2 * P.scale() * P.ss() * std::sqrt(P.zeta()*one_over_pi);
    
    Matrix mat(0);
    double *m = mat.data();
    
    const double *pa = P.P_minus_A().constData();
    const double *pb = P.P_minus_B().constData();
    
    for (int ic=0; ic<nc; ++ic)
    {
        const double U = P.zeta() * Vector::distance2(P.P(), c[ic].center());
        
        double boys[3];
        multi_boys_3(U, boys, M);
        
        const double q_prefac = c[ic].charge() * prefac;
        const Vector P_minus_C = P.P() - c[ic].center();
        const double *pc = P_minus_C.constData();
        
        //do the off-diagonal elements
        for (int i=0; i<2; ++i)
        {
            for (int j=i+1; j<3; ++j)
            {
                const double val = q_prefac * 
                                    ( boys[0]*pa[i]*pb[j] +
                                      boys[2]*pc[i]*pc[j] -
                                      boys[1]*(pb[j]*pc[i] + pa[i]*pc[j]) );
                
                m[ mat.offset(i,j) ] += val;
            }
        }
        
        //do the diagonal elements
        const double extra = 0.5 * P.zeta() * (boys[0] - boys[1]);
        
        for (int i=0; i<3; ++i)
        {
            m[ mat.offset(i,i) ] = q_prefac * 
                                    ( boys[0]*pa[i]*pb[i] +
                                      boys[2]*pc[i]*pc[i] -
                                      boys[1]*(pb[i]*pc[i] + pa[i]*pc[i]) + extra );
        }
    }

    //now copy mat[i,j] to mat[j,i]
    for (int i=0; i<2; ++i)
    {
        for (int j=i+1; j<3; ++j)
        {
            m[ mat.offset(j,i) ] = m[ mat.offset(i,j) ];
        }
    }
    
    return mat;
}

/** Return the potential integral with a single point charge (p|C|p)^m */
Matrix SQUIRE_EXPORT potential_integral(const PointCharge &C, const PP_GTO &P, int m)
{
    QVector<PointCharge> Cs(1, C);
    return potential_integral(Cs, P, m);
}

/** Return the potential integral with an array of point dipoles, (p|C|p) */
Matrix SQUIRE_EXPORT potential_integral(const QVector<PointDipole> &C, const PP_GTO &P)
{
    throw SireError::incomplete_code( "Not implemented", CODELOC );
    return Matrix();
}

/** Return the potential integral with a single point dipole, (p|C|p) */
Matrix SQUIRE_EXPORT potential_integral(const PointDipole &C, const PP_GTO &P)
{
    QVector<PointDipole> Cs(1,C);
    return potential_integral(Cs, P);
}

/** Return the potential integral with an array of point dipoles, (p|C|p)^m */
Matrix SQUIRE_EXPORT potential_integral(const QVector<PointDipole> &C, 
                                        const PP_GTO &P, int m)
{
    throw SireError::incomplete_code( "Not implemented", CODELOC );
    return Matrix();
}

/** Return the potential integral with a single point dipole, (p|C|p)^m */
Matrix SQUIRE_EXPORT potential_integral(const PointDipole &C, 
                                        const PP_GTO &P, int m)
{
    QVector<PointDipole> Cs(1,C);
    return potential_integral(Cs, P, m);
}

/////////
///////// The electron repulsion integrals
/////////

static Vector my_electron_integral(const PS_GTO &P, const SS_GTO &Q, 
                                   const double boys[2])
{
    // (ps|ss) = preFac [ Fm(T) (Pi-Ai) + Fm+1(T) (Wi-Pi) ]
    const double prefac = P.scale() * GTOPair::preFac(P, Q);
    const Vector W_minus_P = GTOPair::W(P,Q) - P.P();

    return prefac * ( (P.P_minus_A() * boys[0]) + (W_minus_P * boys[1]) );
}

/** Return the electron repulsion integral (ps|ss) */
Vector SQUIRE_EXPORT electron_integral(const PS_GTO &P, const SS_GTO &Q)
{
    double boys[2];
    multi_boys_2(GTOPair::T(P,Q), boys);

    return my_electron_integral(P, Q, boys);
}

/** Return the electron repulsion integral (ss|ps) */
Vector SQUIRE_EXPORT electron_integral(const SS_GTO &P, const PS_GTO &Q)
{
    return electron_integral(Q, P);
}

/** Return the auxilliary electron repulsion integral (ps|ss)^m */
Vector SQUIRE_EXPORT electron_integral(const PS_GTO &P, const SS_GTO &Q, int m)
{
    double boys[2];
    multi_boys_2(GTOPair::T(P,Q), boys, m);

    return my_electron_integral(P, Q, boys);
}

/** Return the auxilliary electron repulsion integral (ss|ps)^m */
Vector SQUIRE_EXPORT electron_integral(const SS_GTO &P, const PS_GTO &Q, int m)
{
    return electron_integral(Q, P, m);
}

static Matrix my_electron_integral(const PS_GTO &P, const PS_GTO &Q,
                                   const double boys[3])
{
    // (ps|ps) = prefac * { F0(T) (Pi-Ai) (Qk-Ck) + 
    //                      F2(T) (Wi-Pi) (Wk-Qk) +
    //                      F1(T) [(Qk-Ck)(Wi-Pi) + (Pi-Ai)(Wk-Qk) +
    //                                (delta_ik / 2(zeta+eta)) ] }

    const double prefac = P.scale() * Q.scale() * GTOPair::preFac(P, Q);

    const Vector W = GTOPair::W(P, Q);
    const Vector W_minus_P = W - P.P();
    const Vector W_minus_Q = W - Q.Q();
    
    Matrix mat;
    double *m = mat.data();
    
    const double *pa = P.P_minus_A().constData();
    const double *qc = Q.Q_minus_C().constData();
    const double *wp = W_minus_P.constData();
    const double *wq = W_minus_Q.constData();
    
    //do the off-diagonal elements
    for (int i=0; i<2; ++i)
    {
        for (int k=i+1; k<3; ++k)
        {
            const double val = prefac * (
                                 boys[0]*pa[i]*qc[k] + boys[2]*wp[i]*wq[k] + 
                                 boys[1]*(qc[k]*wp[i] + pa[i]*wq[k])
                                         );

            m[ mat.offset(i,k) ] = val;
            m[ mat.offset(k,i) ] = val;
        }
    }
    
    //do the diagonal elements
    const double extra_ii = 0.5 / (P.zeta()+Q.eta());
    
    for (int i=0; i<3; ++i)
    {
        m[ mat.offset(i,i) ] = prefac * (
                                 boys[0]*pa[i]*qc[i] + boys[2]*wp[i]*wq[i] + 
                                 boys[1]*(qc[i]*wp[i] + pa[i]*wq[i] + extra_ii) );
    }
    
    return mat;
}

/** Return the electron repulsion integral (ps|ps) */
Matrix SQUIRE_EXPORT electron_integral(const PS_GTO &P, const PS_GTO &Q)
{
    double boys[3];
    multi_boys_3( GTOPair::T(P,Q), boys );
    return my_electron_integral(P, Q, boys);
}

/** Return the auxillary electron repulsion integral (ps|ps)^m */
Matrix SQUIRE_EXPORT electron_integral(const PS_GTO &P, const PS_GTO &Q, int m)
{
    double boys[3];
    multi_boys_3( GTOPair::T(P,Q), boys, m );
    return my_electron_integral(P, Q, boys);
}

static Matrix my_electron_integral(const PP_GTO &P, const SS_GTO &Q, 
                                   const double boys[3])
{
    // (pp|ss) = prefac * { F2(T) (Wi-Pi)(Wj-Pj) + 
    //                      F0(T) (Pi-Ai)(Pj-Bj) +
    //                      F1(T) [(Pj-Bj)(Wi-Pi) + (Pi-Ai)(Wj-Pj)] +
    //                      delta_ij/2zeta [F0(T) - rho/zeta F1(T)] }

    const double prefac = P.scale() * GTOPair::preFac(P,Q);
    const double rho = GTOPair::rho(P,Q);
    const Vector W_minus_P = GTOPair::W(P,Q) - P.P();

    Matrix mat;
    double *m = mat.data();
    
    const double *pa = P.P_minus_A().constData();
    const double *pb = P.P_minus_B().constData();
    const double *wp = W_minus_P.constData();
    
    //do the off-diagonal elements
    for (int i=0; i<2; ++i)
    {
        for (int j=i+1; j<3; ++j)
        {
            const double val = prefac * (
                                   boys[0]*pa[i]*pb[j] + 
                                   boys[2]*wp[i]*wp[j] +
                                   boys[1]*(pb[j]*wp[i] + pa[i]*wp[j]) );
        
            m[ mat.offset(i,j) ] = val;
            m[ mat.offset(j,i) ] = val;
        }
    }
    
    const double one_over_zeta = 1 / P.zeta();
    
    const double extra = 0.5 * one_over_zeta * (boys[0] - 
                                                boys[1] * rho * one_over_zeta);
                                                
    for (int i=0; i<3; ++i)
    {
        m[ mat.offset(i,i) ] = prefac * (
                                   boys[0]*pa[i]*pb[i] + 
                                   boys[2]*wp[i]*wp[i] +
                                   boys[1]*(pb[i]*wp[i] + pa[i]*wp[i]) + extra );
    }
    
    return mat;
}

/** Return the electron repulsion integral (pp|ss) */
Matrix SQUIRE_EXPORT electron_integral(const PP_GTO &P, const SS_GTO &Q)
{
    double boys[3];
    multi_boys_3( GTOPair::T(P,Q), boys );
    return my_electron_integral(P, Q, boys);
}

/** Return the electron repulsion integral (ss|pp) */
Matrix SQUIRE_EXPORT electron_integral(const SS_GTO &P, const PP_GTO &Q)
{
    return electron_integral(Q,P);
}

/** Return the auxilliary electron repulsion integral (pp|ss)^m */
Matrix SQUIRE_EXPORT electron_integral(const PP_GTO &P, const SS_GTO &Q, int m)
{
    double boys[3];
    multi_boys_3( GTOPair::T(P,Q), boys, m );
    return my_electron_integral(P, Q, boys);
}

/** Return the auxilliary electron repulsion integral (ss|pp)^m */
Matrix SQUIRE_EXPORT electron_integral(const SS_GTO &P, const PP_GTO &Q, int m)
{
    return electron_integral(Q,P,m);
}

static TrigArray2D<Vector> 
my_electron_integral(const PP_GTO &P, const PS_GTO &Q, const double boys[4])
{
    // (pp|ps) = prefactor * { F3(T) (Wi-Pi)(Wj-Pj)(Wk-Qk) +
    // 
    //                         F0(T) (Pi-Ai)(Pj-Bj)(Qk-Ck) +
    //
    //                         F1(T) [(Pj-Bj)(Qk-Ck)(Wi-Pi) + 
    //                                (Pi-Ai)(Qk-Ck)(Wj-Pj) +
    //                                (Pi-Ai)(Pj-Bj)(Wk-Qk)] +
    //
    //                         F2(T) [(Qk-Ck)(Wi-Pi)(Wj-Pj) +
    //                                (Pj-Bj)(Wi-Pi)(Wk-Qk) +
    //                                (Pi-Ai)(Wj-Pj)(Wk-Qk)] +
    //
    //        delta_ij/(2zeta)[ F0(T)(Qk-Ck) + 
    //                          F1(T)[(Wk-Qj) - (rho/zeta)(Qk-Ck)] -
    //                          F2(T)[rho/zeta (Wk-Qk)] ] +
    //
    //        delta_ik/2(zeta+eta)[ F1(T)(Pj-Bj) + F2(T)(Wj-Pj) ] +
    //
    //        delta_jk/2(zeta+eta)[ F1(T)(Pi-Ai) + F2(T)(Wi-Pi) ] }

    const double prefac = P.scale() * Q.scale() * GTOPair::preFac(P,Q);
    const double rho = GTOPair::rho(P,Q);

    const Vector W = GTOPair::W(P,Q);
    const Vector W_minus_P = W - P.P();
    const Vector W_minus_Q = W - Q.Q();
    
    const double *pa = P.P_minus_A().constData();
    const double *pb = P.P_minus_B().constData();
    const double *qc = Q.Q_minus_C().constData();
    const double *wp = W_minus_P.constData();
    const double *wq = W_minus_Q.constData();

    const double inv_zeta = 0.5 / P.zeta();
    const double inv_zeta_eta = 1.0 / (2.0*(P.zeta()+Q.eta()));
    
    const Vector extra_ij = inv_zeta * 
                               (boys[0]*Q.Q_minus_C() + 
                                boys[1]*(W_minus_Q - (rho/P.zeta())*Q.Q_minus_C()) -
                                boys[2]*(rho/P.zeta())*W_minus_Q);
                                        
    const Vector extra_ik = inv_zeta_eta * (boys[1]*P.P_minus_B() + boys[2]*W_minus_P);
    const Vector extra_jk = inv_zeta_eta * (boys[1]*P.P_minus_A() + boys[2]*W_minus_P);

    const double *ij = extra_ij.constData();
    const double *ik = extra_ik.constData();
    const double *jk = extra_jk.constData();

    TrigArray2D<Vector> mat(3);
    
    Vector *m = mat.data();
    
    //do the off-diagonal elements
    for (int i=0; i<2; ++i)
    {
        for (int j=i+1; j<3; ++j)
        {
            Vector v;
            double *vdata = v.data();
            
            //do the common parts of i,j,k
            for (int k=0; k<3; ++k)
            {
                vdata[k] = boys[3]*wp[i]*wp[j]*wq[k] + 
                           boys[0]*pa[i]*pb[j]*qc[k] +
                           boys[1]*(pb[j]*qc[k]*wp[i] + 
                                    pa[i]*qc[k]*wp[j] +
                                    pa[i]*pb[j]*wq[k]) +
                           boys[2]*(qc[k]*wp[i]*wp[j] +
                                    pb[j]*wp[i]*wq[k] +
                                    pa[i]*wp[j]*wq[k]);
            }
            
            //do i == k
            vdata[i] += ik[j];
            
            //do j == k
            vdata[j] += jk[i];
            
            m[ mat.offset(i,j) ] = prefac * v;
        }
    }

    //do the diagonal elements
    for (int i=0; i<3; ++i)
    {
        Vector v;
        double *vdata = v.data();
        
        //do the common parts of i,j,k
        for (int k=0; k<3; ++k)
        {
            vdata[k] = boys[3]*wp[i]*wp[i]*wq[k] + 
                       boys[0]*pa[i]*pb[i]*qc[k] +
                       boys[1]*(pb[i]*qc[k]*wp[i] + 
                                pa[i]*qc[k]*wp[i] +
                                pa[i]*pb[i]*wq[k]) +
                       boys[2]*(qc[k]*wp[i]*wp[i] +
                                pb[i]*wp[i]*wq[k] +
                                pa[i]*wp[i]*wq[k]) + ij[k];
        }
        
        //do i == j == k
        vdata[i] += ik[i] + jk[i];
        
        m[ mat.offset(i,i) ] = prefac * v;
    }

    return mat;
}

/** Return the electron repulsion integral (pp|ps) */
TrigArray2D<Vector> SQUIRE_EXPORT electron_integral(const PP_GTO &P, const PS_GTO &Q)
{
    double boys[4];
    multi_boys_4( GTOPair::T(P,Q), boys );
    return my_electron_integral(P,Q,boys);
}

/** Return the electron repulsion integral (ps,pp) */
TrigArray2D<Vector> SQUIRE_EXPORT electron_integral(const PS_GTO &P, const PP_GTO &Q)
{
    return electron_integral(Q,P);
}

/** Return the auxilliary electron repulsion integral (pp|ps)^m */
TrigArray2D<Vector> SQUIRE_EXPORT electron_integral(const PP_GTO &P, 
                                                    const PS_GTO &Q, int m)
{
    double boys[4];
    multi_boys_4( GTOPair::T(P,Q), boys, m );
    return my_electron_integral(P,Q,boys);
}

/** Return the auxilliary electron repulsion integral (ps,pp)^m */
TrigArray2D<Vector> SQUIRE_EXPORT electron_integral(const PS_GTO &P, 
                                                    const PP_GTO &Q, int m)
{
    return electron_integral(Q,P,m);
}

static TrigArray2D<Matrix>
my_electron_integral(const PP_GTO &P, const PP_GTO &Q, const double boys[5])
{
    return TrigArray2D<Matrix>(3);
}

/** Return the electron repulsion integral (pp|pp) */
TrigArray2D<Matrix> SQUIRE_EXPORT electron_integral(const PP_GTO &P, const PP_GTO &Q)
{
    double boys[5];
    multi_boys_n( GTOPair::T(P,Q), boys, 5 );
    return my_electron_integral(P, Q, boys);
}

/** Return the auxilliary electron repulsion integral (pp|pp)^m */
TrigArray2D<Matrix> SQUIRE_EXPORT electron_integral(const PP_GTO &P,
                                                    const PP_GTO &Q, int m)
{
    double boys[5];
    multi_boys_n( GTOPair::T(P,Q), boys, 5, m );
    return my_electron_integral(P, Q, boys);
}

} // end of namespace Squire
