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

#ifndef SQUIRE_SGTO_H
#define SQUIRE_SGTO_H

#include "gto.h"

#include "SireMaths/vector.h"
#include "SireMaths/trigmatrix.h"

#include "SireBase/array2d.hpp"

SIRE_BEGIN_HEADER

namespace Squire
{
class S_GTO;
class SS_GTO;

class CS_GTO;
class CSS_GTO;
}

QDataStream& operator<<(QDataStream&, const Squire::S_GTO&);
QDataStream& operator>>(QDataStream&, Squire::S_GTO&);

QDataStream& operator<<(QDataStream&, const Squire::SS_GTO&);
QDataStream& operator>>(QDataStream&, Squire::SS_GTO&);

QDataStream& operator<<(QDataStream&, const Squire::CS_GTO&);
QDataStream& operator>>(QDataStream&, Squire::CS_GTO&);

QDataStream& operator<<(QDataStream&, const Squire::CSS_GTO&);
QDataStream& operator>>(QDataStream&, Squire::CSS_GTO&);

namespace Squire
{

class PointCharge;
class PointDipole;

using SireMaths::Vector;
using SireMaths::TrigMatrix;

using SireBase::Array2D;

/** This is a single S-type Gaussian Type Orbital shell */
class SQUIRE_EXPORT S_GTO : public SireBase::ConcreteProperty<S_GTO,GTO>
{
public:
    S_GTO();
    S_GTO(double alpha, double scale=1);
    
    S_GTO(const S_GTO &other);
    
    ~S_GTO();
    
    static const char* typeName();
    
    S_GTO& operator=(const S_GTO &other);
    
    bool operator==(const S_GTO &other) const;
    bool operator!=(const S_GTO &other) const;
    
    int angularMomentum() const;
    int nOrbitals() const;
};

/** This is a single contracted S-type Gaussian Type Orbital shell */

/** This is a contracted S-type (l==0) Gaussian Type Orbital shell. */
class SQUIRE_EXPORT CS_GTO : public SireBase::ConcreteProperty<CS_GTO,CGTO>
{
public:
    CS_GTO();
    CS_GTO(const QVector<double> &alphas, const QVector<double> &scales);
    
    CS_GTO(const CS_GTO &other);
    
    ~CS_GTO();
    
    static const char* typeName();
    
    CS_GTO& operator=(const CS_GTO &other);

    bool operator==(const CS_GTO &other) const;
    bool operator!=(const CS_GTO &other) const;
    
    int angularMomentum() const;
    int nOrbitals() const;
};

/** This is the combined SS shell pair composed from two S-type
    GTO shells */
class SQUIRE_EXPORT SS_GTO
{

friend QDataStream& ::operator<<(QDataStream&, const SS_GTO&);
friend QDataStream& ::operator>>(QDataStream&, SS_GTO&);

public:
    SS_GTO();
    SS_GTO(const Vector &p0, const S_GTO &s0,
           const Vector &p1, const S_GTO &s1);

    SS_GTO(const SS_GTO &other);
    
    ~SS_GTO();
    
    static const char* typeName();
    
    SS_GTO& operator=(const SS_GTO &other);
    
    bool operator==(const SS_GTO &other) const;
    bool operator!=(const SS_GTO &other) const;
    
    const Vector& P() const;
    const Vector& Q() const;
    
    double R2() const;
    
    double eta() const;
    double zeta() const;
    
    double chi() const;
    
    double alpha_plus_beta() const;
    double alpha_times_beta() const;
    
    double G() const;
    double G_AB() const;
    double G_CD() const;

private:
    /** The center of this combined SS shell pair - for 
        the orbitals a and b, with centers A and B and 
        exponents alpha and beta, we get;
        
        P = (alpha*A + beta*B) / (alpha + beta) */
    Vector _P;
    
    /** The distance squared between the two centers */
    double _R2;
    
    /** The eta value of the combined gaussian. This is;
    
        eta = alpha+beta
    */
    double _eta;
    
    /** The chi value of the combined gaussian,
    
        chi = alpha*beta / (alpha + beta) 
    */
    double _chi;
    
    /** The scaling factor for this combined gaussian. This is;
    
        G = scl_a * scl_b * exp( (-alpha*beta/(alpha+beta))|A-B|^2 )
    */
    double _G;
};    

/** This is the combined SS shell pair composed from two contracted
    S-type GTO shells, or from a CS_GTO and a S_GTO */
class SQUIRE_EXPORT CSS_GTO
{

friend QDataStream& ::operator<<(QDataStream&, const CSS_GTO&);
friend QDataStream& ::operator>>(QDataStream&, CSS_GTO&);

public:
    CSS_GTO();

    CSS_GTO(const Vector &p0, const S_GTO &s0,
            const Vector &p1, const S_GTO &s1);

    CSS_GTO(const Vector &p0, const S_GTO &s0,
            const Vector &p1, const CS_GTO &s1);

    CSS_GTO(const Vector &p0, const CS_GTO &s0,
            const Vector &p1, const S_GTO &s1);

    CSS_GTO(const Vector &p0, const CS_GTO &s0,
            const Vector &p1, const CS_GTO &s1);

    CSS_GTO(const CSS_GTO &other);
    
    ~CSS_GTO();
    
    static const char* typeName();
    
    CSS_GTO& operator=(const CSS_GTO &other);
    
    bool operator==(const CSS_GTO &other) const;
    bool operator!=(const CSS_GTO &other) const;
    
    int nContractions() const;
    
    const Array2D<Vector>& P(int i) const;
    const Array2D<Vector>& Q(int i) const;

    double R2() const;
    
    const TrigMatrix& eta() const;
    const TrigMatrix& G() const;

    const TrigMatrix& chi() const;
    
    const TrigMatrix& alpha_plus_beta() const;
    const TrigMatrix& alpha_times_beta() const;
    
    const TrigMatrix& G() const;
    const TrigMatrix& G_AB() const;
    const TrigMatrix& G_CD() const;

private:
    /** The centers of this combined CSS shell pair - for 
        orbitals a and b, with centers A and B, and exponents
        alpha[i] and beta[i,j], we get;
        
        P[i,j] = (alpha[i]*A + beta[j]*B) / (alpha + beta)
    */
    Array2D<Vector> _P;
    
    /** The distance between the two centers */
    double _R2;
    
    /** The alpha values of the combined gaussian. This is 
    
        eta[i,j] = alpha[i] + beta[j]
        
        note that eta[i,j] == eta[j,i]
    */
    TrigMatrix _eta;
    
    /** The chi value of the combined gaussian,
    
        chi[i,j] = alpha[i]*beta[j] / (alpha[i] + beta[j])

        note that chi[i,j] == chi[j,i]
    */
    TrigMatrix _chi;
    
    /** The scaling factors for this combined gaussian. This is;
    
        G[i,j] = scl_a[i] * scl_b[j] * exp( (-alpha[i]*beta[j]/(alpha[i]+beta[j])) R2 )

        note that G[i,j] == G[j,i]
    */
    TrigMatrix _G;
};    

//////////
////////// Functions involving S-orbitals (only)
//////////

double kinetic_integral(const SS_GTO &ss);
double overlap_integral(const SS_GTO &ss);

double potential_integral(const PointCharge &Q, const SS_GTO &ss);
double potential_integral(const PointDipole &Q, const SS_GTO &ss);

double electron_integral(const SS_GTO &ss0, const SS_GTO &ss1);

double kinetic_integral(const CSS_GTO &css);
double overlap_integral(const CSS_GTO &css);

double potential_integral(const PointCharge &Q, const CSS_GTO &css);
double potential_integral(const PointDipole &Q, const CSS_GTO &css);

double electron_integral(const SS_GTO &ss, const CSS_GTO &css);
double electron_integral(const CSS_GTO &css, const SS_GTO &ss);
double electron_integral(const CSS_GTO &css0, const CSS_GTO &css1);

}

Q_DECLARE_METATYPE( Squire::S_GTO )
Q_DECLARE_METATYPE( Squire::CS_GTO )
Q_DECLARE_METATYPE( Squire::SS_GTO )
Q_DECLARE_METATYPE( Squire::CSS_GTO )

SIRE_EXPOSE_CLASS( Squire::S_GTO )
SIRE_EXPOSE_CLASS( Squire::CS_GTO )
SIRE_EXPOSE_CLASS( Squire::SS_GTO )
SIRE_EXPOSE_CLASS( Squire::CSS_GTO )

SIRE_EXPOSE_FUNCTION( Squire::kinetic_integral )
SIRE_EXPOSE_FUNCTION( Squire::potential_integral )
SIRE_EXPOSE_FUNCTION( Squire::overlap_integral )
SIRE_EXPOSE_FUNCTION( Squire::electron_integral )

SIRE_END_HEADER

#endif
