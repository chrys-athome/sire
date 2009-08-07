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

#include "hf.h"

#include "SireMaths/boys.h"
#include "SireMaths/gamma.h"
#include "SireMaths/vector.h"

#include <QDebug>

using namespace Squire;
using namespace SireMaths;

/** Wrapper for boys function m=0 */
double BoysF0(double t)
{
    return boys(0,t);
}

/** Wrapper for boys function m=1 */
double BoysF1(double t)
{
    return boys(1,t);
}

#warning NEED NORMALISED FORMS OF INTEGRALS FROM NEW INTEGRALS.F90

/** Calculate the overlap integral for two s-type gaussian basis
    functions centred at A and B with exponents alpha and beta
    (thanks to Darragh for all these integrals - originally in
    Fortran90 - I've converted to C++ - any errors are my fault!!!)
*/
double OvInt(const Vector &A, const Vector &B, double alpha, double beta)
{
    double eta = 1.0 / (alpha+beta);
    double zeta = alpha*beta*eta;

    Vector AB = A - B;

    double R2 = Vector::dot(AB, AB);

    qDebug() << "eta" << eta << "zeta" << zeta << "AB" << AB.toString()
             << "R2" << R2;

    double xnorm = std::pow(4.0*alpha*beta/(pi*pi), 0.75);

    return xnorm * std::pow(pi*eta,1.5) * std::exp(-zeta*R2);
}

/** Calculate the kinetic energy integral for two s-type gaussian basis
    functions centred at A and B with exponents alpha and beta */
double KEInt(const Vector &A, const Vector &B, double alpha, double beta)
{
    double eta = 1.0 / (alpha+beta);
    double zeta = alpha * beta * eta;
    Vector AB = A - B;
    double R2 = Vector::dot(AB,AB);
    
    return zeta * std::pow(pi*eta, 1.5) * (3.0 - 2.0*zeta*R2) * std::exp(-zeta*R2);
}

/** Calculate the potential energy integral for two s-type gaussian basis 
    functions centred at A and B with exponents alpha and beta and a point
    charge Z centred at C */
double PEIntNuc(const Vector &A, const Vector &B, const Vector &C,
                double alpha, double beta, double Z)
{
    double eta = 1.0 / (alpha+beta);
    double zeta = alpha * beta * eta;
    Vector AB = A - B;
    double R2 = Vector::dot(AB, AB);

    Vector P = (alpha+beta) * (eta*(alpha*A + beta*B) - C);
    double argf1 = Vector::dot(P,P);
    
    return -2.0*Z*pi*eta*std::exp(-zeta*R2)*BoysF0(argf1);
}

/** Calculate the potential energy integral for two s-type gaussian basis
    functions centred at A and B with exponents alpha and beta and a point
    dipole mu centred a C */
double PEIntDip(const Vector &A, const Vector &B, const Vector &C,
                double alpha, double beta, const Vector &mu)
{
    double eta = 1.0 / (alpha + beta);
    double zeta = alpha * beta * eta;
    Vector AB = A - B;
    double R2 = Vector::dot(AB,AB);
    
    Vector P = (alpha+beta)*(eta*(alpha*A+beta*B)-C);
    double argf1 = Vector::dot(P,P);
    
    return -4.0*pi*std::exp(-zeta*R2)*BoysF1(argf1)*Vector::dot(mu,P);
}

/** Potential energy integral for two s-type gaussians centred at A and B with
    exponents alpha and beta due to the a repulsive term of gaussian form   
    centred at C with exponent gamma.  fac should be a positive number to
    leave interaction repulsive. */
double RepulsInt(const Vector &A, const Vector &B, const Vector &C,
                 double alpha, double beta, double gamma, double fac)
{
    double eta = 1.0 / (alpha + beta + gamma);
    Vector AB = A - B;
    Vector BC = B - C;
    Vector AC = A - C;
    
    double R2AB = Vector::dot(AB,AB);
    double R2BC = Vector::dot(BC,BC);
    double R2AC = Vector::dot(AC,AC);
    
    return std::pow(pi*eta, 1.5) * std::exp(-eta*(alpha*beta*R2AB + 
                                                  beta*gamma*R2BC +
                                                  alpha*gamma*R2AC));
}

HF::HF()
{}

HF::~HF()
{}

void HF::solve()
{
    Vector A(0,0,0);
    Vector B(0,0,1.4);
    Vector C(0,0,0);
    
    Vector mu(1,1.2,2.1);
    
    double alpha = 0.5;
    double beta = 0.5;

    qDebug() << "OvInt" << OvInt(A,B,alpha,beta);
    qDebug() << "KEInt" << KEInt(A,B,alpha,beta);
    qDebug() << "PEInt" << PEIntDip(A,B,C,alpha,beta,mu);
    qDebug() << "PEIntNuc" << PEIntNuc(A,B,C,alpha,beta,1);
}
