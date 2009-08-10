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

#include "squire_lapack.h"

#include "SireMaths/boys.h"
#include "SireMaths/gamma.h"
#include "SireMaths/vector.h"

#include "SireBase/array2d.hpp"

#include "SireError/errors.h"

#include <QDebug>

using namespace Squire;
using namespace SireMaths;
using namespace SireBase;            

//////////
////////// Implementation of S_GTO
//////////

S_GTO::S_GTO() : alfa(0)
{}

S_GTO::S_GTO(const Vector &center, double alpha)
      : cent(center), alfa(alpha)
{}

S_GTO::S_GTO(const S_GTO &other)
      : cent(other.cent), alfa(other.alfa)
{}

S_GTO::~S_GTO()
{}

S_GTO& S_GTO::operator=(const S_GTO &other)
{
    cent = other.cent;
    alfa = other.alfa;
    return *this;
}

const Vector& S_GTO::center() const
{
    return cent;
}

double S_GTO::alpha() const
{
    return alfa;
}

int S_GTO::angularMomentum()
{
    return 0;
}

//////////
////////// Implementation of PointCharge
//////////

PointCharge::PointCharge() : chg(0)
{}

PointCharge::PointCharge(const Vector &center, double charge)
            : cent(center), chg(charge)
{}

PointCharge::PointCharge(const PointCharge &other)
            : cent(other.cent), chg(other.chg)
{}

PointCharge::~PointCharge()
{}

PointCharge& PointCharge::operator=(const PointCharge &other)
{
    cent = other.cent;
    chg = other.chg;
    return *this;
}

const Vector& PointCharge::center() const
{
    return cent;
}

double PointCharge::charge() const
{
    return chg;
}

//////////
////////// Implementation of integral functions
//////////

const double pi2 = pi*pi;

double SQUIRE_EXPORT Squire::overlap_integral(const S_GTO &s0, const S_GTO &s1)
{
    double eta = 1 / (s0.alpha() + s1.alpha());
    double zeta = s0.alpha() * s1.alpha() * eta;
    
    double R2 = Vector::distance2(s0.center(), s1.center());
    
    double xnorm = std::pow(4*s0.alpha()*s1.alpha()/pi2, 0.75);
    
    return xnorm * std::pow(pi*eta,1.5) * std::exp(-zeta*R2);
}

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

    double xnorm = std::pow(4.0*alpha*beta/(pi*pi), 0.75);

    return xnorm * std::pow(pi*eta,1.5) * std::exp(-zeta*R2);
}

double SQUIRE_EXPORT Squire::kinetic_integral(const S_GTO &s0, const S_GTO &s1)
{
    double eta = 1.0 / (s0.alpha() + s1.alpha());
    double zeta = s0.alpha() * s1.alpha() * eta;
    
    double R2 = Vector::distance2(s0.center(), s1.center());
    
    double xnorm = std::pow(4*s0.alpha()*s1.alpha()/pi2, 0.75);
    
    return xnorm * zeta * std::pow(pi*eta, 1.5) * (3 - 2*zeta*R2)
                                                * std::exp(-zeta*R2);
}

/** Calculate the kinetic energy integral for two s-type gaussian basis
    functions centred at A and B with exponents alpha and beta */
double KEInt(const Vector &A, const Vector &B, double alpha, double beta)
{
    double eta = 1.0 / (alpha+beta);
    double zeta = alpha * beta * eta;
    Vector AB = A - B;
    double R2 = Vector::dot(AB,AB);
    double xnorm=std::pow(4*alpha*beta/(pi*pi), 0.75);
    
    return xnorm * zeta * 
                std::pow(pi*eta, 1.5) * (3.0 - 2.0*zeta*R2) * std::exp(-zeta*R2);
}

double SQUIRE_EXPORT Squire::potential_integral(const PointCharge &chg,
                                                const S_GTO &s0, const S_GTO &s1)
{
    double eta = 1 / (s0.alpha() + s1.alpha());
    double zeta = s0.alpha() * s1.alpha() * eta;
    double R2 = Vector::distance2(s0.center(), s1.center());
    
    Vector P = (eta*(s0.alpha()*s0.center() + s1.alpha()*s1.center())) - chg.center();

    double argf1 = (s0.alpha() + s1.alpha()) * P.length2();
    double xnorm = std::pow(4*s0.alpha()*s1.alpha()/pi2, 0.75);
    
    return -2*xnorm*chg.charge()*pi*eta*std::exp(-zeta*R2)*boys_f0(argf1);
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
    double xnorm = std::pow(4*alpha*beta/(pi*pi), 0.75);
    
    return -4.0*xnorm*pi*std::exp(-zeta*R2)*boys_f1(argf1)*Vector::dot(mu,P);
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
    double xnorm = std::pow(4*alpha*beta/(pi*pi), 0.75);
    
    return xnorm * std::pow(pi*eta, 1.5) * std::exp(-eta*(alpha*beta*R2AB + 
                                                  beta*gamma*R2BC +
                                                  alpha*gamma*R2AC));
}

double nuclear_energy(const QVector<PointCharge> &charges)
{
    const int ncharges = charges.count();
    
    double nrg = 0;
    
    for (int i=0; i<ncharges-1; ++i)
    {
        for (int j=i+1; j<ncharges; ++j)
        {
            nrg += charges[i].charge() * charges[j].charge() / 
                       Vector::distance(charges[i].center(), charges[j].center());
        }
    }
    
    return nrg;
}

//////////
////////// Implementation of HF
//////////

HF::HF()
{}

HF::~HF()
{}

void HF::add(const S_GTO &orbital)
{
    orbs.append(orbital);
    orbs.squeeze();
}

void HF::add(const PointCharge &charge)
{
    chgs.append(charge);
    chgs.squeeze();
}

static Array2D<double> make_overlap_matrix(const QVector<S_GTO> &orbitals)
{
    const int norbitals = orbitals.count();
    
    Array2D<double> overlap_matrix(norbitals, norbitals);

    //calculate the overlap matrix - this is the overlap integral
    //of all pairs of orbitals
    for (int i=0; i<norbitals; ++i)
    {
        for (int j=0; j<norbitals; ++j)
        {
            overlap_matrix(i,j) = overlap_integral( orbitals[i], orbitals[j] );
        }
    }

    return overlap_matrix;
}

static Array2D<double> make_xnorm_matrix(const QVector<S_GTO> &orbitals,
                                         const Array2D<double> &overlap_matrix)
{
    const int norbitals = orbitals.count();

    //diagonalise a copy of the overlap matrix
    Array2D<double> diagonal_overlap = overlap_matrix;
    
    int info = 0;
    
    QVector<double> eig = dsyev('V','U',diagonal_overlap,info);
    
    if (info != 0)
        throw SireError::program_bug( QObject::tr(
            "Error in dsyev and I can't work out what to do?!? (%1)")
                .arg(info), CODELOC );

    //create matrix whose diagonal is the square root of the eigenvalues
    Array2D<double> sdiag(norbitals, norbitals, 0);
    
    for (int i=0; i<norbitals; ++i)
    {
        if (eig[i] > 0)
        {
            double sqrt_eig = std::sqrt(eig[i]);
            
            if (sqrt_eig > 0)
                sdiag(i,i) = 1 / sqrt_eig;
        }
    }
    
    Array2D<double> tmp = dgemm('N', 'T', sdiag, diagonal_overlap);
    Array2D<double> Xmat = dgemm('N', 'N', diagonal_overlap, tmp);

    return Xmat;
}

static Array2D<double> make_fock_matrix(const QVector<S_GTO> &orbitals,
                                        const QVector<PointCharge> &charges)
{
    const int norbitals = orbitals.count();
    const int ncharges = charges.count();

    Array2D<double> fock_matrix(norbitals, norbitals);
    Array2D<double> kinetic_matrix(norbitals, norbitals);
    Array2D<double> nuclear_matrix(norbitals, norbitals, 0);
    
    //kinetic energy
    for (int i=0; i<norbitals; ++i)
    {
        for (int j=0; j<norbitals; ++j)
        {
            kinetic_matrix(i,j) = kinetic_integral(orbitals[i], orbitals[j]); 
        }
    }
    
    //nuclear-electron energy
    for (int k=0; k<ncharges; ++k)
    {
        for (int i=0; i<norbitals; ++i)
        {
            for (int j=0; j<norbitals; ++j)
            {
                nuclear_matrix(i,j) += potential_integral(charges[k],
                                                       orbitals[i], orbitals[j]);
            }
        }
    }
    
    for (int i=0; i<norbitals; ++i)
    {
        for (int j=0; j<norbitals; ++j)
        {
            fock_matrix(i,j) = kinetic_matrix(i,j) + nuclear_matrix(i,j);
        }
    }
    
    return fock_matrix;
}

void HF::solve()
{
    #ifdef SQUIRE_HAVE_BLAS_AND_LAPACK

    const int norbitals = orbs.count();

    if (norbitals == 0)
        return;

    //need to create the overlap matrix
    Array2D<double> overlap_matrix = make_overlap_matrix(orbs);
    
    //and the orthonormalization matrix
    Array2D<double> xnorm_matrix = make_xnorm_matrix(orbs, overlap_matrix);
    
    //and the Fock matrix
    Array2D<double> fock_matrix = make_fock_matrix(orbs, chgs);
    
    qDebug() << "OVERLAP MATRIX\n" << overlap_matrix.toString();
    qDebug() << "NORMALISATION MATRIX\n" << xnorm_matrix.toString();
    qDebug() << "FOCK MATRIX\n" << fock_matrix.toString();
    
    //transform the fock matrix -  F' = X F X^T
    #warning dgemm wrong way around as C order matrix when need fortran order!!!
    Array2D<double> xfock = dgemm('T', 'N', fock_matrix, xnorm_matrix);
    xfock = dgemm('T', 'N', xnorm_matrix, xfock);
    
    qDebug() << "TRANSFORMED FOCK MATRIX\n" << xfock.toString();
    
    //diagonalise the transformmed fock matrix
    int info = 0;
    QVector<double> orbital_eigenvalues = dsyev('V', 'U', xfock, info);
    
    if (info != 0)
        throw SireError::program_bug( QObject::tr(
            "Diagonalisation of the fock matrix failed! (%1)")
                .arg(info), CODELOC );
                
    qDebug() << "Orbital eigenvalues:" << orbital_eigenvalues;
    
    double e_elec = orbital_eigenvalues[0];
    
    qDebug() << "Electronic energy =" << e_elec << "Hartrees";
    
    double e_nuc = nuclear_energy(chgs);
    
    qDebug() << "Potential energy =" << e_nuc << "Hartrees";
    
    qDebug() << "Total energy =" << e_nuc+e_elec << "Hartrees";
    
    qDebug() << "The end!";
    
    #else // SQUIRE_HAVE_BLAS_AND_LAPACK
    qDebug() << "NO BLAS OR LAPACK SUPPORT!!!";
    #endif
}
