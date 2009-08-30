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

#ifndef SQUIRE_PGTO_H
#define SQUIRE_PGTO_H

#include "gto.h"
#include "sgto.h"

#include "SireMaths/vector.h"
#include "SireMaths/matrix.h"
#include "SireMaths/nmatrix.h"

#include "SireBase/array2d.hpp"

SIRE_BEGIN_HEADER

namespace Squire
{
class P_GTO;
class CP_GTO;

class PP_GTO;
class SP_GTO;
class CPP_GTO;
class CSP_GTO;
}

QDataStream& operator<<(QDataStream&, const Squire::P_GTO&);
QDataStream& operator>>(QDataStream&, Squire::P_GTO&);

QDataStream& operator<<(QDataStream&, const Squire::CP_GTO&);
QDataStream& operator>>(QDataStream&, Squire::CP_GTO&);

QDataStream& operator<<(QDataStream&, const Squire::PP_GTO&);
QDataStream& operator>>(QDataStream&, Squire::PP_GTO&);

QDataStream& operator<<(QDataStream&, const Squire::SP_GTO&);
QDataStream& operator>>(QDataStream&, Squire::SP_GTO&);

QDataStream& operator<<(QDataStream&, const Squire::CPP_GTO&);
QDataStream& operator>>(QDataStream&, Squire::CPP_GTO&);

QDataStream& operator<<(QDataStream&, const Squire::CSP_GTO&);
QDataStream& operator>>(QDataStream&, Squire::CSP_GTO&);

namespace Squire
{

class PointCharge;
class PointDipole;

using SireMaths::Vector;
using SireMaths::Matrix;
using SireMaths::NMatrix;

using SireBase::Array2D;
 
/** This is a single P-type shell of Gaussian Type Orbitals */
class SQUIRE_EXPORT P_GTO : public SireBase::ConcreteProperty<P_GTO,GTO>
{

friend QDataStream& ::operator<<(QDataStream&, const P_GTO&);
friend QDataStream& ::operator>>(QDataStream&, P_GTO&);

public:
    P_GTO();
    P_GTO(double alpha, double scale=1);
    
    P_GTO(const P_GTO &other);
    
    ~P_GTO();
    
    static const char* typeName();
    
    P_GTO& operator=(const P_GTO &other);
    
    bool operator==(const P_GTO &other) const;
    bool operator!=(const P_GTO &other) const;
    
    int angularMomentum() const;
    int nOrbitals() const;
};

/** This is a combined S-P GTO shell pair */
class SQUIRE_EXPORT SP_GTO
{

friend QDataStream& ::operator<<(QDataStream&, const SP_GTO&);
friend QDataStream& ::operator>>(QDataStream&, SP_GTO&);

public:
    SP_GTO();
    SP_GTO(const Vector &A, const S_GTO &a,
           const Vector &B, const P_GTO &b);
    SP_GTO(const Vector &A, const P_GTO &a,
           const Vector &B, const S_GTO &b);
           
    SP_GTO(const SP_GTO &other);
    
    ~SP_GTO();
    
    static const char* typeName();
    
    SP_GTO& operator=(const SP_GTO &other);
    
    bool operator==(const SP_GTO &other) const;
    bool operator!=(const SP_GTO &other) const;
    
    const Vector& P_minus_A() const;
    const Vector& Q_minus_C() const;
    
    const SS_GTO& SS() const;

    double scale() const;

private:
    /** The combined SS GTO (used for Obara-Saika recursion) */
    SS_GTO ss;
    
    /** The vector from the center of the P-orbital to
        the 'center of mass' of the shell-pair */
    Vector p_minus_a;
    
    /** The extra factor for the scaling constant */
    double norm_scl;
};

/** This is a combined P-P GTO shell pair */
class SQUIRE_EXPORT PP_GTO
{

friend QDataStream& ::operator<<(QDataStream&, const PP_GTO&);
friend QDataStream& ::operator>>(QDataStream&, PP_GTO&);

public:
    PP_GTO();
    PP_GTO(const Vector &A, const P_GTO &a,
           const Vector &B, const P_GTO &b);
           
    PP_GTO(const PP_GTO &other);
    
    ~PP_GTO();
    
    static const char* typeName();
    
    PP_GTO& operator=(const PP_GTO &other);
    
    bool operator==(const PP_GTO &other) const;
    bool operator!=(const PP_GTO &other) const;

    const Vector& P_minus_A() const;
    const Vector& P_minus_B() const;

    const Vector& Q_minus_C() const;
    const Vector& Q_minus_D() const;

    double scale() const;

    const SS_GTO& SS() const;

private:
    /** The combined SS-GTO (used for obara-saika recursion) */
    SS_GTO ss;

    /** The vector from the center of the first P orbital to the 
        center of mass of the gaussian */
    Vector p_minus_a;
    
    /** The vector from the center of the second P orbital to the
        center of mass of the gaussian */
    Vector p_minus_b;
    
    /** The extra factor for the scaling constant */
    double norm_scl;
};

//////////
////////// Functions involving SP-orbitals
//////////

Vector kinetic_integral(const SP_GTO &sp);
Vector overlap_integral(const SP_GTO &sp);

Vector potential_integral(const PointCharge &Q, const SP_GTO &sp);
Vector potential_integral(const PointDipole &Q, const SP_GTO &sp);

Vector electron_integral(const SS_GTO &ss0, const SP_GTO &sp1);
Vector electron_integral(const SP_GTO &sp0, const SS_GTO &ss1);

Matrix electron_integral(const SP_GTO &sp0, const SP_GTO &sp1);

//////////
////////// Functions involving P-orbitals (only)
//////////

Matrix kinetic_integral(const PP_GTO &pp);
Matrix overlap_integral(const PP_GTO &pp);

Matrix potential_integral(const PointCharge &Q, const PP_GTO &pp);
Matrix potential_integral(const PointDipole &Q, const PP_GTO &pp);

Matrix electron_integral(const SS_GTO &P, const PP_GTO &Q);
Matrix electron_integral(const PP_GTO &P, const SS_GTO &Q);

Array2D<Matrix> electron_integral(const PP_GTO &pp0, const PP_GTO &pp1);

}

Q_DECLARE_METATYPE( Squire::P_GTO )
Q_DECLARE_METATYPE( Squire::SP_GTO )
Q_DECLARE_METATYPE( Squire::PP_GTO )

SIRE_EXPOSE_CLASS( Squire::P_GTO )
SIRE_EXPOSE_CLASS( Squire::SP_GTO )
SIRE_EXPOSE_CLASS( Squire::PP_GTO )

SIRE_EXPOSE_FUNCTION( Squire::kinetic_integral )
SIRE_EXPOSE_FUNCTION( Squire::overlap_integral )
SIRE_EXPOSE_FUNCTION( Squire::potential_integral )
SIRE_EXPOSE_FUNCTION( Squire::electron_integral )

SIRE_END_HEADER

#endif
