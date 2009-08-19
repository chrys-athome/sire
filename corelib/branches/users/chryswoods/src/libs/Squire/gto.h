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

#ifndef SQUIRE_GTO_H
#define SQUIRE_GTO_H

#include "SireMaths/vector.h"

#include "orbital.h"

SIRE_BEGIN_HEADER

namespace Squire
{
class GTO;
class C_GTO;

class S_GTO;
class CS_GTO;

class P_GTO;
class CP_GTO;

class D_GTO;
class CD_GTO;

class F_GTO;
class CF_GTO;

class GPlus_GTO;
class CGPlus_GTO;
}

QDataStream& operator<<(QDataStream&, const Squire::GTO&);
QDataStream& operator>>(QDataStream&, Squire::GTO&);

QDataStream& operator<<(QDataStream&, const Squire::C_GTO&);
QDataStream& operator>>(QDataStream&, Squire::C_GTO&);

QDataStream& operator<<(QDataStream&, const Squire::S_GTO&);
QDataStream& operator>>(QDataStream&, Squire::S_GTO&);

QDataStream& operator<<(QDataStream&, const Squire::CS_GTO&);
QDataStream& operator>>(QDataStream&, Squire::CS_GTO&);

QDataStream& operator<<(QDataStream&, const Squire::P_GTO&);
QDataStream& operator>>(QDataStream&, Squire::P_GTO&);

QDataStream& operator<<(QDataStream&, const Squire::CP_GTO&);
QDataStream& operator>>(QDataStream&, Squire::CP_GTO&);

QDataStream& operator<<(QDataStream&, const Squire::D_GTO&);
QDataStream& operator>>(QDataStream&, Squire::D_GTO&);

QDataStream& operator<<(QDataStream&, const Squire::CD_GTO&);
QDataStream& operator>>(QDataStream&, Squire::CD_GTO&);

QDataStream& operator<<(QDataStream&, const Squire::F_GTO&);
QDataStream& operator>>(QDataStream&, Squire::F_GTO&);

QDataStream& operator<<(QDataStream&, const Squire::CF_GTO&);
QDataStream& operator>>(QDataStream&, Squire::CF_GTO&);

QDataStream& operator<<(QDataStream&, const Squire::GPlus_GTO&);
QDataStream& operator>>(QDataStream&, Squire::GPlus_GTO&);

QDataStream& operator<<(QDataStream&, const Squire::CGPlus_GTO&);
QDataStream& operator>>(QDataStream&, Squire::CGPlus_GTO&);

namespace Squire
{

class PointCharge;

/** This is the base class of all single Gaussian Type Orbital shells (GTOs)
    (e.g. S_GTO (l==0), P_GTO (l==1), D_GTO (l==2), F_GTO (l==3) and
    GPlus_GTO (l>=4))
    
    An orbital shell contains all of the orbitals for a particular shell
    
    @author Christopher Woods
*/
class SQUIRE_EXPORT GTO : public OrbitalShell
{

friend QDataStream& ::operator<<(QDataStream&, const GTO&);
friend QDataStream& ::operator>>(QDataStream&, GTO&);

public:
    GTO();
    ~GTO();
    
    const Vector& center() const;

    double alpha() const;
    double scale() const;

    static int angularMomentum();

protected:
    /** The center of this orbital (x,y,z) */
    Vector cent;
    
    /** The orbital exponent (alpha) */
    double alfa;
    
    /** The multiplication factor (including normalisation
        constant) */
    double scl;
};

/** This is the base class of all Contracted Gaussian Type Orbitals (C_GTO).
    (e.g. CS_GTO (l==0), CP_GTO (l==1), CD_GTO (l==2), CF_GTO (l==3) and
    CGPlus_GTO (l>=4))

    This contains just a single orbital shell, containing the 
    correct number of orbitals for its angular momentum
    (1 for S, 3 for P, 5 for D etc.)
    
    An orbital shell contains all of the orbitals for a particular shell
    
    @author Christopher Woods
*/
class SQUIRE_EXPORT C_GTO : public OrbitalShell
{

friend QDataStream& ::operator<<(QDataStream&, const C_GTO&);
friend QDataStream& ::operator>>(QDataStream&, C_GTO&);

public:
    C_GTO();
    ~C_GTO();
    
    static int angularMomentum();

    int nContractions() const;
    
    double alpha(int i) const;
    double scale(int i) const;
    
    const double* alphaData() const;
    const double* scaleData() const;
    
protected:
    /** The center of this orbital (x,y,z) */
    Vector cent;
    
    /** The orbital exponents (alpha) */
    QVector<double> alfas;
    
    /** The multiplication factors (including normalisation
        constant) */
    QVector<double> scls;
};

/** This is a single S-type (l==0) Gaussian Type Orbital Shell.
    An S-orbital shell contains just a single S-orbital
*/
class SQUIRE_EXPORT S_GTO : public GTO
{
public:
    S_GTO();
    S_GTO(const Vector &cent, double alpha, double scale=1);
    S_GTO(const S_GTO &other);
    
    ~S_GTO();
    
    S_GTO& operator=(const S_GTO &other);
    
    static int angularMomentum();
    static int nOrbitals();

    S_GTO operator*(const S_GTO &other) const;
    CS_GTO operator*(const CS_GTO &other) const;
};

/** This is a contracted S-type (l==0) Gaussian Type Orbital shell. */
class SQUIRE_EXPORT CS_GTO : public C_GTO
{
public:
    CS_GTO();
    
    CS_GTO(const CS_GTO &other);
    
    ~CS_GTO();
    
    CS_GTO& operator=(const CS_GTO &other);
    
    static int angularMomentum();
    static int nOrbitals();

    CS_GTO operator*(const CS_GTO &other) const;
    CS_GTO operator*(const S_GTO &other) const;
};

////////
//////// All of the functions used to calculate integrals
//////// involving these orbitals
////////

/////////
///////// Integrals for general GTO shells
/////////
NMatrix kinetic(const GTO &A, const GTO &B);
NMatrix potential(const PointCharge &Q, const GTO &A, const GTO &B);
NMatrix overlap(const GTO &A, const GTO &B);

double sum_electron_repulsion(const GTO &A, const GTO &B,
                              const GTO &C, const GTO &D);

/////////
///////// Integrals for general C_GTOs
/////////
NMatrix kinetic(const C_GTO &A, const C_GTO &B);
NMatrix potential(const PointCharge &Q, const C_GTO &A, const C_GTO &B);
NMatrix overlap(const C_GTO &A, const C_GTO &B);

double sum_electron_repulsion(const C_GTO &A, const C_GTO &B,
                              const C_GTO &C, const C_GTO &D);

/////////
///////// Integrals for S_GTO / S_GTO
/////////
double kinetic(const S_GTO &A, const S_GTO &B);
double kinetic(const SS_GTO &AB);

double potential(const PointCharge &Q, const S_GTO &A, const S_GTO &B);
double potential(const PointCharge &Q, const SS_GTO &AB);

double overlap(const S_GTO &A, const S_GTO &B);
double overlap(const SS_GTO &AB);

double sum_electron_repulsion(const S_GTO &A, const S_GTO &B,
                              const S_GTO &C, const S_GTO &D);
                              
double sum_electron_repulsion(const SS_GTO &AB, const SS_GTO &CD);

//////////
////////// Integrals for CS_GTO / CS_GTO
//////////
double kinetic(const CS_GTO &A, const CS_GTO &B);
double kinetic(const CS_GTO &AB);

double potential(const PointCharge &Q, const CS_GTO &A, const CS_GTO &B);
double potential(const PointCharge &Q, const CS_GTO &AB);

double overlap(const CS_GTO &A, const CS_GTO &B);
double overlap(const CS_GTO &AB);

double electron_repulsion(const CS_GTO &A, const CS_GTO &B,
                          const CS_GTO &C, const CS_GTO &D);
                          
double electron_repulsion(const CS_GTO &AB, const CS_GTO &CD);

double sum_electron_repulsion(const CS_GTO &A, const CS_GTO &B,
                              const CS_GTO &C, const CS_GTO &D);
                              
double sum_electron_repulsion(const CS_GTO &AB, const CS_GTO &CD);

//////////
////////// Integrals for S_GTO/P_GTO
//////////
Vector kinetic(const S_GTO &S, const P_GTO &P);
Vector kinetic(const P_GTO &P, const S_GTO &S);

Vector kinetic(const SP_GTO &SP);

Vector potential(const PointCharge &Q, const S_GTO &S, const P_GTO &P);
Vector potential(const PointCharge &Q, const P_GTO &P, const S_GTO &S);

Vector potential(const PointCharge &Q, const SP_GTO &SP);

Vector overlap(const S_GTO &S, const P_GTO &P);
Vector overlap(const P_GTO &P, const S_GTO &S);

Vector overlap(const SP_GTO &SP);

double sum_electron_repulsion(const P_GTO &A, const P_GTO &B,
                              const P_GTO &C, const P_GTO &D);

double sum_electron_repulsion(

}

Q_DECLARE_METATYPE( Squire::S_GTO )
Q_DECLARE_METATYPE( Squire::CS_GTO )

SIRE_EXPOSE_CLASS( Squire::GTO )
SIRE_EXPOSE_CLASS( Squire::C_GTO )

SIRE_EXPOSE_CLASS( Squire::S_GTO )
SIRE_EXPOSE_CLASS( Squire::CS_GTO )

SIRE_END_HEADER

#endif
