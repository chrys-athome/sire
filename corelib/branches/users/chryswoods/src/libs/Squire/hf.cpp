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

S_GTO::S_GTO() : alfa(0), scl(1)
{}

S_GTO::S_GTO(const Vector &center, double alpha, double scale)
      : cent(center), alfa(alpha), scl(scale)
{
    //set scl equal to 'scl' times the normalisation
    //factor for this orbital
    scl *= std::pow(2*alfa/pi, 0.75);
}

S_GTO::S_GTO(const S_GTO &other)
      : cent(other.cent), alfa(other.alfa), scl(other.scl)
{}

S_GTO::~S_GTO()
{}

S_GTO& S_GTO::operator=(const S_GTO &other)
{
    cent = other.cent;
    alfa = other.alfa;
    scl = other.scl;
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

double S_GTO::scale() const
{
    return scl;
}

int S_GTO::angularMomentum()
{
    return 0;
}

S_GTO S_GTO::multiply(const S_GTO &s0, const S_GTO &s1)
{
    //the product of two Gaussians is a Gaussian :-)
    const double alpha_times_beta = s0.alpha() * s1.alpha();
    const double alpha_plus_beta = s0.alpha() + s1.alpha();
    const double R2 = Vector::distance2(s0.center(), s1.center());
    
    S_GTO mult;
    
    mult.cent = (s0.alpha()*s0.center() + s1.alpha()*s1.center()) / alpha_plus_beta;
    mult.alfa = alpha_plus_beta;
    mult.scl = s0.scale()*s1.scale()*std::exp(-alpha_times_beta*R2 / alpha_plus_beta);

    return mult;
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

double twoe(double A, double B, double C, double D,
            double RAB2, double RCD2, double RPQ2)
{
    const double pi = 3.14159265358998;
    
    return 2.000 * std::pow(pi,2.5) / ((A+B)*(C+D)*std::sqrt(A+B+C+D))
         * boys_f0((A+B)*(C+D)*RPQ2/(A+B+C+D))
         * std::exp(-A*B*RAB2/(A+B)-C*D*RCD2/(C+D));
}

double SQUIRE_EXPORT Squire::electron_integral(const S_GTO &A, const S_GTO &B,
                                               const S_GTO &C, const S_GTO &D)
{
    const double two_pi_to_2_5 = 2 * std::pow(pi, 2.5);

    //multiply A and B together (to make P) and C and D to make Q
    S_GTO P = S_GTO::multiply(A, B);
    S_GTO Q = S_GTO::multiply(C, D);
    
    // from Szabo and Ostland
    // (AB|CD) = 2 pi^5/2 / [(alpha+beta)*(gamma+delta)(alpha+beta+gamma+delta)^1/2]
    //            * exp[ -alpha beta/(alpha+beta)|Ra-Rb|^2 
    //                   -gamma delta/(gamma+delta)|Rc-Rd|^2
    //            * F0[(alpha+beta)(gamma+delta)/(alpha+beta+gamma+delta)|Rp-Rq|^2]

    const double abcd = P.alpha() + Q.alpha();
    const double ab_times_cd = P.alpha() * Q.alpha();

    return (two_pi_to_2_5 / (ab_times_cd * std::sqrt(abcd))) *
           P.scale() * Q.scale() *
           boys_f0( ab_times_cd * Vector::distance2(P.center(),Q.center()) / abcd );
}

double SQUIRE_EXPORT Squire::overlap_integral(const S_GTO &s0, const S_GTO &s1)
{
    double eta = 1 / (s0.alpha() + s1.alpha());
    double zeta = s0.alpha() * s1.alpha() * eta;
    
    double R2 = Vector::distance2(s0.center(), s1.center());
    
    return s0.scale() * s1.scale() * std::pow(pi*eta,1.5) * std::exp(-zeta*R2);
}

double SQUIRE_EXPORT Squire::kinetic_integral(const S_GTO &s0, const S_GTO &s1)
{
    double eta = 1.0 / (s0.alpha() + s1.alpha());
    double zeta = s0.alpha() * s1.alpha() * eta;
    
    double R2 = Vector::distance2(s0.center(), s1.center());
    
    return s0.scale() * s1.scale() * zeta * std::pow(pi*eta, 1.5) * (3 - 2*zeta*R2)
                                                * std::exp(-zeta*R2);
}

double SQUIRE_EXPORT Squire::potential_integral(const PointCharge &chg,
                                                const S_GTO &s0, const S_GTO &s1)
{
    double eta = 1 / (s0.alpha() + s1.alpha());
    double zeta = s0.alpha() * s1.alpha() * eta;
    double R2 = Vector::distance2(s0.center(), s1.center());
    
    Vector P = (eta*(s0.alpha()*s0.center() + s1.alpha()*s1.center())) - chg.center();

    double argf1 = (s0.alpha() + s1.alpha()) * P.length2();
    
    return -2*s0.scale()*s1.scale()*chg.charge()*pi*eta*
                    std::exp(-zeta*R2)*boys_f0(argf1);
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

    qDebug() << "overlap\n" << overlap_matrix.toString();
    qDebug() << "overlap eigenvalues" << eig;
    qDebug() << "overlap eigenvectors\n" << diagonal_overlap.toString();

    Array2D<double> dtest = diagonal_overlap;
    
    for (int i=0; i<eig.count(); ++i)
    {
        for (int j=0; j<eig.count(); ++j)
        {
            dtest(j,i) /= std::sqrt(eig[j]);
        }
    }

    qDebug() << "dtest\n" << dtest.toString() << "\n" << dtest.transpose().toString();

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
    
    qDebug() << "SDIAG\n" << sdiag.toString();
    
    Array2D<double> tmp = dgemm('N', 'T', sdiag, diagonal_overlap);
    Array2D<double> Xmat = dgemm('N', 'N', diagonal_overlap, tmp);

    return Xmat;
}

static Array2D<double> make_core_fock_matrix(const QVector<S_GTO> &orbitals,
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
    
    qDebug() << "KINETIC MATRIX\n" << kinetic_matrix.toString();
    
    //nuclear-electron energy
    for (int k=0; k<ncharges; ++k)
    {
        Array2D<double> my_nuclear_matrix(norbitals, norbitals);
    
        for (int i=0; i<norbitals; ++i)
        {
            for (int j=0; j<norbitals; ++j)
            {
                my_nuclear_matrix(i,j) = potential_integral(charges[k],
                                                       orbitals[i], orbitals[j]);
                                                       
                nuclear_matrix(i,j) += potential_integral(charges[k],
                                                       orbitals[i], orbitals[j]);
            }
        }
        
        qDebug() << "NUCLEAR MATRIX" << k << "\n" << my_nuclear_matrix.toString();
    }

    qDebug() << "NUCLEAR MATRIX\n" << nuclear_matrix.toString();
    
    for (int i=0; i<norbitals; ++i)
    {
        for (int j=0; j<norbitals; ++j)
        {
            fock_matrix(i,j) = kinetic_matrix(i,j) + nuclear_matrix(i,j);
        }
    }
    
    for (int i=0; i<norbitals; ++i)
    {
        for (int j=0; j<norbitals; ++j)
        {
            for (int k=0; k<norbitals; ++k)
            {
                for (int l=0; l<norbitals; ++l)
                {
                    qDebug() << i+1 << j+1 << k+1 << l+1 
                             << electron_integral(
                                    orbitals[i], orbitals[j], orbitals[k], 
                                    orbitals[l]);
                }
            }
        }
    }
    
    return fock_matrix;
}

Array2D<double> make_fock_matrix(const Array2D<double> &core_fock_matrix,
                                 const Array2D<double> &density_matrix,
                                 const QVector<S_GTO> &orbitals)
{
    Array2D<double> fock_matrix = core_fock_matrix;
    
    const int norbitals = orbitals.count();
    
    for (int i=0; i<norbitals; ++i)
    {
        for (int j=0; j<norbitals; ++j)
        {
            double v = 0;
            
            for (int k=0; k<norbitals; ++k)
            {
                for (int l=0; l<norbitals; ++l)
                {
                    v += density_matrix(k,l)*
                            (electron_integral(orbitals[i],orbitals[j],
                                                         orbitals[k],orbitals[l])
                             - 0.5*electron_integral(orbitals[i],orbitals[l],
                                                     orbitals[k],orbitals[j]));
                }
            }
            
            fock_matrix(i,j) += v;
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
    
    //and the Fock matrix
    Array2D<double> core_fock_matrix = make_core_fock_matrix(orbs, chgs);
    
    qDebug() << "OVERLAP MATRIX\n" << overlap_matrix.toString();
    qDebug() << "CORE FOCK MATRIX\n" << core_fock_matrix.toString();

    //make the orthonormalization matrix (canonical orthonormalization)
    Array2D<double> X( overlap_matrix );

    int info = 0;
    QVector<double> eig = dsyev('V','U',X,info);
    
    if (info != 0)
        throw SireError::program_bug( QObject::tr(
            "Error in dsyev and I can't work out what to do?!? (%1)")
                .arg(info), CODELOC );
    
    for (int i=0; i<eig.count(); ++i)
    {
        for (int j=0; j<eig.count(); ++j)
        {
            X(i,j) /= std::sqrt(eig[j]);
        }
    }
    
    qDebug() << "X\n" << X.toString();
    
    Array2D<double> density_matrix(norbitals, norbitals, 0);
    double e_elec = 0;
    
    for (int it=0; it<10; ++it)
    {
        //build the new fock matrix from the current density matrix
        Array2D<double> fock_matrix = make_fock_matrix(core_fock_matrix,
                                                       density_matrix,
                                                       orbs);

        qDebug() << "FULL FOCK MATRIX\n" << fock_matrix.toString();
        
        //transform the fock matrix -  F' = X F X^T
        #warning dgemm wrong way around as C order matrix when need fortran order!!!
        Array2D<double> xfock = dgemm('N', 'T', fock_matrix, X);
        xfock = dgemm('N', 'N', X, xfock);
        
        qDebug() << "TRANSFORMED FOCK MATRIX\n" << xfock.toString();
        
        //diagonalise the transformed fock matrix
        int info = 0;
        QVector<double> orbital_eigenvalues = dsyev('V', 'U', xfock, info);
        
        if (info != 0)
            throw SireError::program_bug( QObject::tr(
                "Diagonalisation of the fock matrix failed! (%1)")
                    .arg(info), CODELOC );
                    
        qDebug() << "Orbital eigenvalues:" << orbital_eigenvalues;

        qDebug() << "Orbital eigenvectors:\n" << xfock.toString();
        
        e_elec = orbital_eigenvalues[0];
        
        qDebug() << "Electronic energy =" << e_elec << "Hartrees";

        // C' == xfock
        Array2D<double> C = dgemm('T', 'N', X, xfock);
        
        qDebug() << "C\n" << C.toString();
        
        //compute new density matrix
        Array2D<double> new_density_matrix(norbitals, norbitals, 0);

        for (int k=0; k<norbitals/2; ++k)
        {
            for (int i=0; i<norbitals; ++i)
            {
                for (int j=0; j<norbitals; ++j)
                {
                    new_density_matrix(j,i) = 2*C(k,i)*C(k,j);
                }
            }
        }

        qDebug() << "new_density_matrix\n" << new_density_matrix.toString();
        
        density_matrix = new_density_matrix;
    }
    
    double e_nuc = nuclear_energy(chgs);
    
    qDebug() << "Potential energy =" << e_nuc << "Hartrees";
    
    qDebug() << "Total energy =" << e_nuc+e_elec << "Hartrees";
    
    qDebug() << "The end!";
    
    #else // SQUIRE_HAVE_BLAS_AND_LAPACK
    qDebug() << "NO BLAS OR LAPACK SUPPORT!!!";
    #endif
}
