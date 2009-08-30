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
////////// Implementation of SP_GTO
//////////

static const RegisterMetaType<SP_GTO> r_spgto;

/** Serialise to a binary datastream */
QDataStream SQUIRE_EXPORT &operator<<(QDataStream &ds, const SP_GTO &sp)
{
    writeHeader(ds, r_spgto, 1);
    
    ds << sp.ss << sp.p_minus_a << sp.norm_scl;
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SQUIRE_EXPORT &operator>>(QDataStream &ds, SP_GTO &sp)
{
    VersionID v = readHeader(ds, r_spgto);
    
    if (v == 1)
    {
        ds >> sp.ss >> sp.p_minus_a >> sp.norm_scl;
    }
    else
        throw version_error(v, "1", r_spgto, CODELOC);
        
    return ds;
}

/** Constructor */
SP_GTO::SP_GTO() : norm_scl(0)
{}

/** Construct combining orbital 'a' at position 'A' with orbital 'b' at
    position 'B' */
SP_GTO::SP_GTO(const Vector &A, const P_GTO &a,
               const Vector &B, const S_GTO &b)
       : ss(A, S_GTO(a), B, S_GTO(b))
{
    p_minus_a = ss.P() - A;
    norm_scl = std::sqrt(4 * a.alpha());
}

/** Construct combining orbital 'a' at position 'A' with orbital 'b' at
    position 'B' */
SP_GTO::SP_GTO(const Vector &A, const S_GTO &a,
               const Vector &B, const P_GTO &b)
       : ss(A, S_GTO(a), B, S_GTO(b))
{
    p_minus_a = ss.P() - B;
    norm_scl = std::sqrt(4 * b.beta());
}
  
/** Copy constructor */     
SP_GTO::SP_GTO(const SP_GTO &other)
       : ss(other.ss), p_minus_a(other.p_minus_a), norm_scl(other.norm_scl)
{}

/** Destructor */
SP_GTO::~SP_GTO()
{}

const char* SP_GTO::typeName()
{
    return QMetaType::typeName( qMetaTypeId<SP_GTO>() );
}

/** Copy assignment operator */
SP_GTO& SP_GTO::operator=(const SP_GTO &other)
{
    if (this != &other)
    {
        ss = other.ss;
        p_minus_a = other.p_minus_a;
        norm_scl = other.norm_scl;
    }
    
    return *this;
}

/** Comparison operator */
bool SP_GTO::operator==(const SP_GTO &other) const
{
    return ss == other.ss and p_minus_a == other.p_minus_a
             and norm_scl == other.norm_scl;
}

/** Comparison operator */
bool SP_GTO::operator!=(const SP_GTO &other) const
{
    return not this->operator==(other);
}

/** Return the 'SS' combined shell that can be used to
    calculate integrals using recursion */
const SS_GTO& SP_GTO::SS() const
{
    return ss;
}

/** Return the vector from the center of the P orbital shell
    to the center of mass of the combined gaussian */
const Vector& SP_GTO::P_minus_A() const
{
    return p_minus_a;
}

/** Return the additional scaling constant needed to normalise
    the integrals */
double SP_GTO::scale() const
{
    return norm_scl;
}

//////////
////////// Implementation of PP_GTO
//////////

static const RegisterMetaType<PP_GTO> r_ppgto;

/** Serialise to a binary datastream */
QDataStream SQUIRE_EXPORT &operator<<(QDataStream &ds, const PP_GTO &pp)
{
    writeHeader(ds, r_ppgto, 1);
    
    ds << pp.ss << pp.p_minus_a << pp.p_minus_b << pp.norm_scl;
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SQUIRE_EXPORT &operator>>(QDataStream &ds, PP_GTO &pp)
{
    VersionID v = readHeader(ds, r_ppgto);
    
    if (v == 1)
    {
        ds >> pp.ss >> pp.p_minus_a >> pp.p_minus_b >> pp.norm_scl;
    }
    else
        throw version_error(v, "1", r_ppgto, CODELOC);
        
    return ds;
}

/** Constructor */
PP_GTO::PP_GTO() : norm_scl(0)
{}

/** Construct combining orbital 'a' at position 'A' with orbital 'b' at
    position 'B' */
PP_GTO::PP_GTO(const Vector &A, const P_GTO &a,
               const Vector &B, const P_GTO &b)
       : ss(A, S_GTO(a), B, S_GTO(b))
{
    p_minus_a = ss.P() - A;
    p_minus_b = ss.P() - B;
    
    norm_scl = std::sqrt( 16 * a.alpha() * b.beta() );
}
  
/** Copy constructor */     
PP_GTO::PP_GTO(const PP_GTO &other)
       : ss(other.ss), p_minus_a(other.p_minus_a),
         p_minus_b(other.p_minus_b), norm_scl(other.norm_scl)
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
        ss = other.ss;
        p_minus_a = other.p_minus_a;
        p_minus_b = other.p_minus_b;
        norm_scl = other.norm_scl;
    }
    
    return *this;
}

/** Comparison operator */
bool PP_GTO::operator==(const PP_GTO &other) const
{
    return ss == other.ss and p_minus_a == other.p_minus_a and
           p_minus_b == other.p_minus_b and norm_scl == other.norm_scl;
}

/** Comparison operator */
bool PP_GTO::operator!=(const PP_GTO &other) const
{
    return not this->operator==(other);
}

/** Return the 'SS' combined shell that can be used to
    calculate integrals using recursion */
const SS_GTO& PP_GTO::SS() const
{
    return ss;
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

/** Return the additional scaling constant needed to normalise the
    integrals involving this shell-pair */
double PP_GTO::scale() const
{
    return norm_scl;
}

//////////
////////// Integrals
//////////

namespace Squire
{

Vector SQUIRE_EXPORT overlap_integral(const SP_GTO &sp)
{
    // (pi||s) = (Pi - Ai)(s||s)
    return sp.scale() * sp.P_minus_A() * sp.SS().overlap();
}

Vector SQUIRE_EXPORT kinetic_integral(const SP_GTO &sp)
{
    // (pi|nabla|s) = (Pi-Ai)(s|nabla|s) + 2 chi (pi||s)
    return (sp.scale() * (kinetic_integral(sp.SS()) + 
            2 * sp.SS().chi()*sp.SS().overlap())) * 
                    sp.P_minus_A();
}

Vector SQUIRE_EXPORT potential_integral(const PointCharge &Q, const SP_GTO &sp)
{
    // (s|C|s)^m = 2 (pi/eta)^(1/2) (s||s) Fm{ eta (P-C)^2 }

    // (pi|C|s) = (Pi-Ai)(s|C|s)^0 - (Pi-Ci)(s|C|s)^1
    //
    //          = 2 * Q * (pi/eta)^(1/2)(s||s) { (Pi-Ai)F0(eta(P-C)^2) - 
    //                                           (Pi-Ci)F1(eta(P-C)^2) }

    const double prefac = -2 * sp.scale() * Q.charge() * 
                            std::sqrt(sp.SS().eta() * one_over_pi) * sp.SS().overlap();

    const Vector P_minus_C = sp.SS().P() - Q.center();
    const double U = sp.SS().eta() * P_minus_C.length2();

    const double boys0 = boys_f0(U);
    
    if (U != 0)
    {
        //use upward recursion to get boysf1
        const double boys1 = (1 / (2*U)) * (boys0 - std::exp(-U));
    
        return prefac * ((boys0 * sp.P_minus_A()) - (boys1 * P_minus_C));
    }
    else
        return prefac * (boys0 * sp.P_minus_A());
}

Vector SQUIRE_EXPORT potential_integral(const PointDipole &Q, const SP_GTO &sp)
{
    throw SireError::incomplete_code( "Not implemented", CODELOC );
    return Vector();
}

Vector SQUIRE_EXPORT electron_integral(const SP_GTO &sp0, const SS_GTO &ss1)
{
    throw SireError::incomplete_code( "Not implemented", CODELOC );
    return Vector();
}

Vector SQUIRE_EXPORT electron_integral(const SS_GTO &ss0, const SP_GTO &sp1)
{
    throw SireError::incomplete_code( "Not implemented", CODELOC );
    return Vector();
}

Matrix SQUIRE_EXPORT electron_integral(const SP_GTO &sp0, const SP_GTO &sp1)
{
    throw SireError::incomplete_code( "Not implemented", CODELOC );
    return Matrix();
}

Matrix SQUIRE_EXPORT kinetic_integral(const PP_GTO &pp)
{
    // (pi|nabla|pj) = (Pj-Bj)(pi|nabla|s) + delta_ij/2zeta (s|nabla|s)
    //                    + 2chi(pi||pj)

    //      = (Pj-Bj)(Pi-Ai)(s|nablas|s) + 2chi(Pj-Bj)(Pi-Ai)(s||s)
    //               + 2chi(pi||pj) + delta_ij/2zeta (s|nablas|s)
    
    //      = (Pj-Bj)(Pi-Ai)(s|nablas|s) + 2chi(Pj-Bj)(Pi-Ai)(s||s)
    //               + 2chi[(Pj-Bj)(Pi-Ai)(s||s) + (delta_ij/2zeta)(s||s)]
    //               + delta_ij/2zeta (s|nablas|s)

    //      = (Pj-Bj)(Pi-Ai)(s|nablas|s) + 4chi(Pj-Bj)(Pi-Ai)(s||s)
    //               + 2chi[delta_ij/2zeta)(s||s)]
    //               + delta_ij/2zeta (s|nablas|s)

    //      = (Pj-Bj)(Pi-Ai)(s|nablas|s) + 4chi(Pj-Bj)(Pi-Ai)(s||s)
    //               + delta_ij/2zeta[2chi(s||s) + (s|nablas|s)]

    //      = (Pj-Bj)(Pi-Ai)[ (s|nablas|s) + 4chi(s||s) ]
    //               + delta_ij/2zeta[2chi(s||s) + (s|nablas|s)]

    //  (s|nabla|s) = chi {3 - 2 chi (A-B)^2} (s||s)
    //
    //      = (Pj-Bj)(Pi-Ai){ chi(s||s)[ 7 - 2chi(A-B)^2 ] }
    //               + delta_ij/2zeta{ chi(s||s)[5 - 2chi(A-B)^2] }

    const double chi_ss = pp.scale() * pp.SS().chi() * pp.SS().overlap();
    const double two_chi_r2 = 2 * pp.SS().chi() * pp.SS().R2();
    
    const double base = chi_ss * ( 7 - two_chi_r2 );
    
    Matrix mat;
    double *m = mat.data();
    
    const double *p_minus_a = pp.P_minus_A().constData();
    const double *p_minus_b = pp.P_minus_B().constData();
    
    //do the off-diagonal elements (symmetric matrix)
    for (int i=0; i<2; ++i)
    {
        const double pi_ss = base * p_minus_a[i];
    
        for (int j=i+1; j<3; ++j)
        {
            const double pipj_ss = pi_ss * p_minus_b[j];
            m[ mat.offset(i,j) ] = pipj_ss;
            m[ mat.offset(j,i) ] = pipj_ss;
        }
    }

    const double extra = (chi_ss*(5 - two_chi_r2)) / (2*pp.SS().zeta());

    //now do the diagonal
    for (int i=0; i<3; ++i)
    {
        m[ mat.offset(i,i) ] = (p_minus_a[i] * p_minus_b[i] * base) + extra;
    }

    return mat;
}

Matrix SQUIRE_EXPORT overlap_integral(const PP_GTO &pp)
{
    // (pi||pj) = (Pj-Bj)(pi||s) + (delta_ij / 2 zeta)(s||s)
    
    // (pi||s) = (Pi-Ai)(s||s)
    
    // (pi||pj) = (Pj-Bj)(Pi-Ai)(s||s) + (delta_ij / 2 zeta)(s||s)
    
    Matrix mat;
    
    const double *p_minus_a = pp.P_minus_A().constData();
    const double *p_minus_b = pp.P_minus_B().constData();
   
    const double ss = pp.SS().overlap();
    
    double *m = mat.data();
    
    for (int i=0; i<2; ++i)
    {
        const double pi_ss = pp.scale() * p_minus_a[i] * ss;
    
        for (int j=i+1; j<3; ++j)
        {
            const double pipj = pi_ss * p_minus_b[j];
            m[ mat.offset(i,j) ] = pipj;
            m[ mat.offset(j,i) ] = pipj;
        }
    }

    const double ss_over_two_zeta = ss / (2 * pp.SS().zeta());
    
    for (int i=0; i<3; ++i)
    {
        const double pipi = p_minus_a[i]*p_minus_b[i]*ss;
        m[ mat.offset(i,i) ] = pp.scale() * (pipi + ss_over_two_zeta);
    }

    return mat;
}

Matrix SQUIRE_EXPORT potential_integral(const PointCharge &Q, const PP_GTO &pp)
{
    // (pi|C|pj) = (Pj-Bj)(pi|C|s)^0 - (Pj-Cj)(pi|C|s)^1 + 
    //               delta_ij/2zeta{(s|C|s)^0 - (s|C|s)^1}
    //
    //           = (Pj-Bj)(Pi-Ai)(s|C|s)^0 - (Pj-Bj)(Pi-Ci)(s|C|s)^1 -
    //             (Pj-Cj)(Pi-Ai)(s|C|s)^1 + (Pj-Cj)(Pi-Ci)(s|C|s)^2 +
    //               delta_ij/2zeta{(s|C|s)^0 - (s|C|s)^1}
    
    // (s|C|s)^m =  2*Q*(zeta/pi)^1/2 (s||s) Fm(U)
    //
    // (pi|C|pj) = 2*Q*(zeta/pi)^1/2 (s||s) * 
    //             {(Pj-Bj)(Pi-Ai)F0(U) - [(Pj-Bj)(Pi-Ci)+(Pj-Cj)(Pi-Ai)]F1(U) +
    //              (Pj-Cj)(Pi-Ci)F2(U) + 
    //                delta_ij/2zeta{F0(U) - F1(U)}

    const double prefac = -2 * pp.scale() * Q.charge() * 
                            std::sqrt(pp.SS().eta() * one_over_pi) * pp.SS().overlap();

    const Vector P_minus_C = pp.SS().P() - Q.center();
    const double U = pp.SS().eta() * P_minus_C.length2();

    Matrix mat;
    double *m = mat.data();
    
    const double *p_minus_a = pp.P_minus_A().constData();
    const double *p_minus_b = pp.P_minus_B().constData();
    const double *p_minus_c = P_minus_C.constData();
    
    double boys0 = 1;
    double boys1 = 1.0 / 3.0;
    double boys2 = 1.0 / 5.0;
    
    if (U != 0)
    {
        boys0 = boys_f0(U);
    
        //use upward recursion to get boysf1 and boysf2
        const double one_over_two_U = 1 / (2*U);
        const double exp_minus_U = std::exp(-U);
        
        boys1 = one_over_two_U * (boys0 - exp_minus_U);
        boys2 = one_over_two_U * (3*boys1 - exp_minus_U);
    }
    
    //fill in the off-diagonal elements
    for (int i=0; i<2; ++i)
    {
        for (int j=i+1; j<3; ++j)
        {
            const double v = prefac * ( (p_minus_b[j]*p_minus_a[i]*boys0) - 
                              ( ((p_minus_b[j]*p_minus_c[i])+(p_minus_c[j]*p_minus_a[i]))
                                    * boys1 ) +
                                (p_minus_c[j]*p_minus_c[i]*boys2) );
                                
            m[ mat.offset(i,j) ] = v;
            m[ mat.offset(j,i) ] = v;
        }
    }
    
    //fill in the diagonal
    for (int i=0; i<3; ++i)
    {
        m[ mat.offset(i,i) ] = prefac * ( (p_minus_b[i]*p_minus_a[i]*boys0) - 
                              ( ((p_minus_b[i]*p_minus_c[i])+(p_minus_c[i]*p_minus_a[i]))
                                    * boys1 ) +
                                (p_minus_c[i]*p_minus_c[i]*boys2) +
                                 ( (boys0-boys1) / (2*pp.SS().eta()) ) );
    }
    
    return mat;
}

Matrix SQUIRE_EXPORT potential_integral(const PointDipole &Q, const PP_GTO &pp)
{
    throw SireError::incomplete_code( "Not implemented", CODELOC );
    return Matrix();
}

Array2D<Matrix> SQUIRE_EXPORT electron_integral(const PP_GTO &pp0, const PP_GTO &pp1)
{
    throw SireError::incomplete_code( "Not implemented", CODELOC );
    return Array2D<Matrix>();
}

} // end of namespace Squire
