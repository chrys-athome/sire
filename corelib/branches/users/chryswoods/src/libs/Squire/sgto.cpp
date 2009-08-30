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

#include "sgto.h"
#include "pointcharge.h"
#include "pointdipole.h"

#include "SireMaths/maths.h"
#include "SireMaths/boys.h"

#include "SireID/index.h"

#include "SireError/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace Squire;
using namespace SireMaths;
using namespace SireID;
using namespace SireBase;
using namespace SireStream;

//////////
////////// Implementation of S_GTO
//////////

static const RegisterMetaType<S_GTO> r_sgto;

/** Serialise to a binary datastream */
QDataStream SQUIRE_EXPORT &operator<<(QDataStream &ds, const S_GTO &sgto)
{
    writeHeader(ds, r_sgto, 1);
    
    ds << static_cast<const GTO&>(sgto);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SQUIRE_EXPORT &operator>>(QDataStream &ds, S_GTO &sgto)
{
    VersionID v = readHeader(ds, r_sgto);
    
    if (v == 1)
    {
        ds >> static_cast<GTO&>(sgto);
    }
    else
        throw version_error(v, "1", r_sgto, CODELOC);
    
    return ds;
}

/** Constructor */
S_GTO::S_GTO() : ConcreteProperty<S_GTO,GTO>()
{}

/** Construct with a specified value of alpha and (unnormalised) scale factor */
S_GTO::S_GTO(double alpha, double scale)
      : ConcreteProperty<S_GTO,GTO>(alpha, scale)
{}

/** Construct from another GTO - this creates the S GTO equivalent
    of the passed GTO */
S_GTO::S_GTO(const GTO &other) : ConcreteProperty<S_GTO,GTO>(other)
{}

/** Copy constructor */
S_GTO::S_GTO(const S_GTO &other) : ConcreteProperty<S_GTO,GTO>(other)
{}

/** Destructor */
S_GTO::~S_GTO()
{}

/** Copy assignment operator */
S_GTO& S_GTO::operator=(const S_GTO &other)
{
    GTO::operator=(other);
    return *this;
}

/** Comparison operator */
bool S_GTO::operator==(const S_GTO &other) const
{
    return GTO::operator==(other);
}

/** Comparison operator */
bool S_GTO::operator!=(const S_GTO &other) const
{
    return GTO::operator!=(other);
}

/** Return the angular momentum of this orbital shell (l==0) */
int S_GTO::angularMomentum() const
{
    return 0;
}

/** Return the number of orbitals in this shell (1) */
int S_GTO::nOrbitals() const
{
    return 0;
}

const char* S_GTO::typeName()
{
    return QMetaType::typeName( qMetaTypeId<S_GTO>() );
}

//////////
////////// Implementation of CS_GTO
//////////

static const RegisterMetaType<CS_GTO> r_csgto;

/** Serialise to a binary datastream */
QDataStream SQUIRE_EXPORT &operator<<(QDataStream &ds, const CS_GTO &csgto)
{
    writeHeader(ds, r_csgto, 1);
    
    ds << static_cast<const CGTO&>(csgto);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SQUIRE_EXPORT &operator>>(QDataStream &ds, CS_GTO &csgto)
{
    VersionID v = readHeader(ds, r_csgto);
    
    if (v == 1)
    {
        ds >> static_cast<CGTO&>(csgto);
    }
    else
        throw version_error(v, "1", r_csgto, CODELOC);
        
    return ds;
}

/** Constructor */
CS_GTO::CS_GTO() : ConcreteProperty<CS_GTO,CGTO>()
{}

/** Construct using the passed exponents and (unnormalised) scaling factors */
CS_GTO::CS_GTO(const QVector<double> &alphas, const QVector<double> &scales)
       : ConcreteProperty<CS_GTO,CGTO>(alphas, scales)
{}

/** Construct from another GTO - this creates the S GTO equivalent
    of the passed GTO */
CS_GTO::CS_GTO(const CGTO &other) : ConcreteProperty<CS_GTO,CGTO>(other)
{}

/** Copy constructor */
CS_GTO::CS_GTO(const CS_GTO &other) : ConcreteProperty<CS_GTO,CGTO>(other)
{}

/** Destructor */
CS_GTO::~CS_GTO()
{}

/** Copy assignment operator */
CS_GTO& CS_GTO::operator=(const CS_GTO &other)
{
    CGTO::operator=(other);
    return *this;
}

/** Comparison operator */
bool CS_GTO::operator==(const CS_GTO &other) const
{
    return CGTO::operator==(other);
}

/** Comparison operator */
bool CS_GTO::operator!=(const CS_GTO &other) const
{
    return CGTO::operator!=(other);
}

/** Return the angular momentum of this shell (l == 0) */
int CS_GTO::angularMomentum() const
{
    return 1;
}

/** Return the number of orbitals that make up this shell */
int CS_GTO::nOrbitals() const
{
    return 1;
}

const char* CS_GTO::typeName()
{
    return QMetaType::typeName( qMetaTypeId<CS_GTO>() );
}

//////////
////////// Implementation of SS_GTO
//////////

static const RegisterMetaType<SS_GTO> r_ssgto;

/** Serialise to a binary datastream */
QDataStream SQUIRE_EXPORT &operator<<(QDataStream &ds, const SS_GTO &ssgto)
{
    writeHeader(ds, r_ssgto, 1);
    
    ds << ssgto._P << ssgto._R2 << ssgto._eta << ssgto._chi 
       << ssgto._K << ssgto._overlap;
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SQUIRE_EXPORT &operator>>(QDataStream &ds, SS_GTO &ssgto)
{
    VersionID v = readHeader(ds, r_ssgto);
    
    if (v == 1)
    {
        ds >> ssgto._P >> ssgto._R2 >> ssgto._eta >> ssgto._chi 
           >> ssgto._K >> ssgto._overlap;
    }
    else
        throw version_error(v, "1", r_ssgto, CODELOC);
    
    return ds;
}

/** Null constructor */
SS_GTO::SS_GTO() : _R2(0), _eta(0), _chi(0), _K(0), _overlap(0)
{}

static const double sqrt_two_times_pi_to_5_4 = std::sqrt(2.0) * std::pow(pi, (5.0/4.0));
static const double four_over_pi2 = 4 / (pi*pi);

/** Construct a S-S shell pair from the passed two S orbital shells,
    located at the specified points */
SS_GTO::SS_GTO(const Vector &A, const S_GTO &a,
               const Vector &B, const S_GTO &b)
{
    //the product of two Gaussians is a Gaussian :-)
    const double alpha_times_beta = a.alpha() * b.alpha();
    const double alpha_plus_beta = a.alpha() + b.alpha();
    
    _P = (a.alpha()*A + b.alpha()*B) / alpha_plus_beta;
    _R2 = Vector::distance2(A, B);
    _eta = alpha_plus_beta;
    _chi = alpha_times_beta / alpha_plus_beta;
    
    const double scl = a.scale() * b.scale() *
                          std::pow( four_over_pi2 * a.alpha() * b.beta(), 0.75 );
    
    const double G = scl * std::exp(-_chi*_R2);
    _overlap = std::pow( pi / _eta, 1.5 ) * G;

    _K = sqrt_two_times_pi_to_5_4 * G / alpha_plus_beta;
}

/** Copy constructor */
SS_GTO::SS_GTO(const SS_GTO &other) 
       : _P(other._P), _R2(other._R2), _eta(other._eta), _chi(other._chi), 
         _K(other._K), _overlap(other._overlap)
{}

/** Destructor */
SS_GTO::~SS_GTO()
{}

/** Copy assignment operator */
SS_GTO& SS_GTO::operator=(const SS_GTO &other)
{
    _P = other._P;
    _R2 = other._R2;
    _eta = other._eta;
    _chi = other._chi;
    _K = other._K;
    _overlap = other._overlap;
    
    return *this;
}

/** Comparison operator */
bool SS_GTO::operator==(const SS_GTO &other) const
{
    return _P == other._P and _R2 == other._R2 and 
           _eta == other._eta and _K == other._K;
}

/** Comparison operator */
bool SS_GTO::operator!=(const SS_GTO &other) const
{
    return not this->operator==(other);
}

/** Return the center of this shell-pair

    P = Q = (alpha*A + beta*B) / (alpha + beta)
*/
const Vector& SS_GTO::P() const
{
    return _P;
}

/** Return the center of this shell-pair 

    Q = P = (alpha*A + beta*B) / (alpha + beta)
*/
const Vector& SS_GTO::Q() const
{
    return SS_GTO::P();
}

/** Return the distance-squared between the two orbitals
    that make up this shell-pair */
double SS_GTO::R2() const
{
    return _R2;
}

/** Return the eta value of this shell-pair 

    eta = zeta = alpha+beta
*/
double SS_GTO::eta() const
{
    return _eta;
}

/** Return the zeta value of this shell-pair 

    eta = zeta = alpha+beta
*/
double SS_GTO::zeta() const
{
    return SS_GTO::eta();
}

/** Return the chi value of this shell-pair.

    chi = (alpha * beta) / (alpha + beta)
*/
double SS_GTO::chi() const
{
    return _chi;
}

/** Return the value of alpha + beta for this shell pair */
double SS_GTO::alpha_plus_beta() const
{
    return SS_GTO::eta();
}

/** Return the value of alpha * beta for this shell pair */
double SS_GTO::alpha_times_beta() const
{
    return _chi * _eta;
}

/** Return the K value for this shell pair

    K = sqrt(2) * pi^(5/4) * scl_a * scl_b * exp( (-alpha*beta/(alpha+beta))|A-B|^2 )
                / (alpha_beta)

    (see Obara and Saika paper)
*/
double SS_GTO::K() const
{
    return _K;
}

/** Synonym for SS_GTO::K() */
double SS_GTO::K_AB() const
{
    return SS_GTO::K();
}

/** Synonym for SS_GTO::K() */
double SS_GTO::K_CD() const
{
    return SS_GTO::K();
}

/** Return the overlap integral for this pair of orbitals (s||s) */
double SS_GTO::overlap() const
{
    return _overlap;
}

const char* SS_GTO::typeName()
{
    return QMetaType::typeName( qMetaTypeId<SS_GTO>() );
}

//////////
////////// Implementation of CSS_GTO
//////////

static const RegisterMetaType<CSS_GTO> r_cssgto;

/** Serialise to a binary datastream */
QDataStream SQUIRE_EXPORT &operator<<(QDataStream &ds, const CSS_GTO &cssgto)
{
    writeHeader(ds, r_cssgto, 1);
    
    ds << cssgto._P << cssgto._R2 << cssgto._eta << cssgto._chi 
       << cssgto._K << cssgto._overlap;
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SQUIRE_EXPORT &operator>>(QDataStream &ds, CSS_GTO &cssgto)
{
    VersionID v = readHeader(ds, r_cssgto);
    
    if (v == 1)
    {
        ds >> cssgto._P >> cssgto._R2 >> cssgto._eta >> cssgto._chi 
           >> cssgto._K >> cssgto._overlap;
    }
    else
        throw version_error(v, "1", r_cssgto, CODELOC);
    
    return ds;
}

/** Null constructor */
CSS_GTO::CSS_GTO() : _R2(0)
{}

/** Construct a S-S shell pair from the passed S orbital shells,
    located at the specified points */
CSS_GTO::CSS_GTO(const Vector &A, const S_GTO &a,
                 const Vector &B, const S_GTO &b)
{
    _R2 = Vector::distance2(A, B);

    _P = Array2D<Vector>(1, 1);
    _eta = NMatrix::createRowMajor(1, 1);
    _chi = NMatrix::createRowMajor(1, 1);
    _K = NMatrix::createRowMajor(1, 1);
    _overlap = NMatrix::createRowMajor(1, 1);

    Vector *Pdata = _P.data();
    double *etadata = _eta.data();
    double *chidata = _chi.data();
    double *Kdata = _K.data();
    double *odata = _overlap.data();

    //the product of two Gaussians is a Gaussian :-)
    const double alpha_times_beta = a.alpha() * b.alpha();
    const double alpha_plus_beta = a.alpha() + b.alpha();
    
    Pdata[0] = (a.alpha()*A + b.alpha()*B) / alpha_plus_beta;
    etadata[0] = alpha_plus_beta;
    chidata[0] = alpha_times_beta / alpha_plus_beta;

    const double scl = a.scale() * b.scale() *
                          std::pow( four_over_pi2 * a.alpha() * b.beta(), 0.75 );
    
    const double G = scl * std::exp(-chidata[0]*_R2);
    odata[0] = std::pow( pi / etadata[0], 1.5 ) * G;
    Kdata[0] = sqrt_two_times_pi_to_5_4 * G / alpha_plus_beta;
}

/** Construct a S-S shell pair from the passed S and contracted S orbital shells,
    located at the specified points */
CSS_GTO::CSS_GTO(const Vector &A, const S_GTO &a,
                 const Vector &B, const CS_GTO &b)
{
    if (b.nContractions() == 0)
    {
        _R2 = 0;
        return;
    }

    _R2 = Vector::distance2(A, B);

    const int n = b.nContractions();

    _P = Array2D<Vector>(1, n);
    _eta = NMatrix::createRowMajor(1, n);
    _chi = NMatrix::createRowMajor(1, n);
    _K = NMatrix::createRowMajor(1, n);
    _overlap = NMatrix::createRowMajor(1, n);

    Vector *Pdata = _P.data();
    double *etadata = _eta.data();
    double *chidata = _chi.data();
    double *Kdata = _K.data();
    double *odata = _overlap.data();

    const double *balpha = b.alpha().constData();
    const double *bscl = b.scale().constData();

    for (int i=0; i<n; ++i)
    {
        //the product of two Gaussians is a Gaussian :-)
        const double alpha_times_beta = a.alpha() * balpha[i];
        const double alpha_plus_beta = a.alpha() + balpha[i];
    
        Pdata[i] = (a.alpha()*A + balpha[i]*B) / alpha_plus_beta;
        etadata[i] = alpha_plus_beta;
        chidata[i] = alpha_times_beta / alpha_plus_beta;
        
        const double scl = a.scale() * bscl[i] *
                          std::pow( four_over_pi2 * a.alpha() * balpha[i], 0.75 );

        const double G = scl * std::exp(-chidata[i]*_R2);
        odata[i] = std::pow( pi / etadata[i], 1.5 ) * G;
        Kdata[i] = sqrt_two_times_pi_to_5_4 * G / alpha_plus_beta;
    }
}

/** Construct a S-S shell pair from the passed S and contracted S orbital shells,
    located at the specified points */
CSS_GTO::CSS_GTO(const Vector &A, const CS_GTO &a,
                 const Vector &B, const S_GTO &b)
{
    this->operator=( CSS_GTO(B, b, A, a) );
}

/** Construct a S-S shell pair from the passed two contracted S orbital shells,
    located at the specified points */
CSS_GTO::CSS_GTO(const Vector &A, const CS_GTO &a,
                 const Vector &B, const CS_GTO &b)
{
    if (a.nContractions() == 0 or b.nContractions() == 0)
    {
        _R2 = 0;
        return;
    }

    _R2 = Vector::distance2(A, B);

    const int na = a.nContractions();
    const int nb = b.nContractions();

    _P = Array2D<Vector>(na, nb);
    _eta = NMatrix::createRowMajor(na, nb);
    _chi = NMatrix::createRowMajor(na, nb);
    _K = NMatrix::createRowMajor(na, nb);
    _overlap = NMatrix::createRowMajor(na, nb);

    Vector *Pdata = _P.data();
    double *etadata = _eta.data();
    double *chidata = _chi.data();
    double *Kdata = _K.data();
    double *odata = _overlap.data();

    const double *aalpha = a.alpha().constData();
    const double *ascl = a.scale().constData();
    const double *balpha = b.alpha().constData();
    const double *bscl = b.scale().constData();

    int idx = 0;

    for (int i=0; i<na; ++i)
    {
        const double a_alpha = aalpha[i];
        const double a_scale = ascl[i];
    
        const Vector a_alpha_times_A = a_alpha * A;
    
        for (int j=0; j<nb; ++j)
        {
            //the product of two Gaussians is a Gaussian :-)
            const double alpha_times_beta = a_alpha * balpha[j];
            const double alpha_plus_beta = a_alpha + balpha[j];
    
            Pdata[idx] = (a_alpha_times_A + balpha[j]*B) / alpha_plus_beta;
            etadata[idx] = alpha_plus_beta;
            chidata[idx] = alpha_times_beta / alpha_plus_beta;
            
            const double scl = a_scale * bscl[j] *
                         std::pow( four_over_pi2 * a_alpha * balpha[j], 0.75 );

            const double G = scl * std::exp(-chidata[idx]*_R2);
            odata[idx] = std::pow( pi / etadata[idx], 1.5 ) * G;
            Kdata[idx] = sqrt_two_times_pi_to_5_4 * G / alpha_plus_beta;
            
            ++idx;
        }
    }
}

/** Copy constructor */
CSS_GTO::CSS_GTO(const CSS_GTO &other) 
       : _P(other._P), _R2(other._R2), _eta(other._eta), _chi(other._chi), 
         _K(other._K), _overlap(other._overlap)
{}

/** Destructor */
CSS_GTO::~CSS_GTO()
{}

/** Copy assignment operator */
CSS_GTO& CSS_GTO::operator=(const CSS_GTO &other)
{
    _P = other._P;
    _R2 = other._R2;
    _eta = other._eta;
    _chi = other._chi;
    _K = other._K;
    _overlap = other._overlap;
    
    return *this;
}

/** Comparison operator */
bool CSS_GTO::operator==(const CSS_GTO &other) const
{
    return _P == other._P and _R2 == other._R2 and 
           _eta == other._eta and _K == other._K;
}

/** Comparison operator */
bool CSS_GTO::operator!=(const CSS_GTO &other) const
{
    return not this->operator==(other);
}

/** The number of primitive pairs in this shell-pair */
int CSS_GTO::nPairs() const
{
    return _K.memory().count();
}

/** Return the centers of this shell-pair
    (the matrix is stored in row-major order)

    P[i,j] = Q[i,j] = (alpha[i]*A + beta[j]*B) / (alpha[i] + beta[j])
*/
const Array2D<Vector>& CSS_GTO::P() const
{
    return _P;
}

/** Return the center of this shell-pair 
    (the matrix is stored in row-major order)

    Q[i,j] = P[i,j] = (alpha[i]*A + beta[j]*B) / (alpha[i] + beta[j])
*/
const Array2D<Vector>& CSS_GTO::Q() const
{
    return CSS_GTO::P();
}

/** Return the distance-squared between the two orbitals
    that make up this shell-pair */
double CSS_GTO::R2() const
{
    return _R2;
}

/** Return the eta value of this shell-pair 
    (the data is stored in row-major order - same as P and Q)

    eta[i,j] = zeta[i,j] = alpha[i]+beta[j]
*/
const NMatrix& CSS_GTO::eta() const
{
    return _eta;
}

/** Return the zeta value of this shell-pair 
    (the data is stored in row-major order - same as P and Q)

    zeta[i,j] = eta[i,j] = alpha[i]+beta[j]
*/
const NMatrix& CSS_GTO::zeta() const
{
    return CSS_GTO::eta();
}

/** Return the chi value of this shell-pair.
    (the data is stored in row-major order - same as P and Q)

    chi[i,j] = (alpha[i] * beta[j]) / (alpha[i] + beta[j])
*/
const NMatrix& CSS_GTO::chi() const
{
    return _chi;
}

/** Return the value of alpha + beta for this shell pair */
const NMatrix& CSS_GTO::alpha_plus_beta() const
{
    return CSS_GTO::eta();
}

/** Return the value of alpha * beta for this shell pair */
NMatrix CSS_GTO::alpha_times_beta() const
{
    NMatrix a_times_b( _chi );
    
    const int sz = _chi.memory().count();
    double *atbdata = a_times_b.data();
    const double *etadata = _eta.constData();
    
    for (int i=0; i<sz; ++i)
    {
        atbdata[i] *= etadata[i];
    }
    
    return a_times_b;
}

/** Return the K value for this shell pair

    K = sqrt(2) * pi^(5/4) * scl_a * scl_b * exp( (-alpha*beta/(alpha+beta))|A-B|^2 )
                / (alpha_beta)

    (see Obara and Saika paper)
*/
const NMatrix& CSS_GTO::K() const
{
    return _K;
}

/** Synonym for CSS_GTO::K() */
const NMatrix& CSS_GTO::K_AB() const
{
    return CSS_GTO::K();
}

/** Synonym for CSS_GTO::K() */
const NMatrix& CSS_GTO::K_CD() const
{
    return CSS_GTO::K();
}

/** Return the overlap integral for each pair of primitives (s||s) */
const NMatrix& CSS_GTO::overlap() const
{
    return _overlap;
}

const char* CSS_GTO::typeName()
{
    return QMetaType::typeName( qMetaTypeId<CSS_GTO>() );
}

////////////
//////////// Integrals involving just SS_GTO shell pairs
////////////
//////////// These are derived in;
//////////// "Efficient recursive computation of molecular integrals
////////////  over Cartesian Gaussian functions"
////////////
//////////// Obara and Saika
//////////// J. Chem. Phys., 84 (7), 3963-3974, 1986
////////////

namespace Squire
{

/////////////
///////////// Integrals involving just SS_GTO
/////////////
double SQUIRE_EXPORT kinetic_integral(const SS_GTO &P)
{
    // (s|nabla|s) = chi {3 - 2 chi (A-B)^2} (s||s)

    return P.chi() * (3 - 2*P.chi()*P.R2()) * P.overlap();
}

double SQUIRE_EXPORT overlap_integral(const SS_GTO &P)
{
    // (s||s) = (pi/eta)^(3/2) exp{-eta(A-B)^2}

    return P.overlap();
}

double SQUIRE_EXPORT potential_integral(const PointCharge &Q, const SS_GTO &P)
{
    // (s|Q|s) = 2 (pi/eta)^(1/2) (s||s) F0{ eta (P-Q)^2 }

    const double U = P.eta() * ((P.P() - Q.center()).length2());

    return -2 * Q.charge() * std::sqrt( P.eta() * one_over_pi ) * 
                        P.overlap() * boys_f0(U); 
}

double SQUIRE_EXPORT potential_integral(const PointCharge &Q, const SS_GTO &P, int m)
{
    const double U = P.eta() * ((P.P() - Q.center()).length2());

    return -2 * Q.charge() * std::sqrt( P.eta() * one_over_pi ) * 
                     P.overlap() * boys(m, U); 
}

double SQUIRE_EXPORT potential_integral(const PointDipole &Q, const SS_GTO &P)
{
    throw SireError::incomplete_code("Not implemented", CODELOC);
    return 0;
}

double SQUIRE_EXPORT potential_integral(const PointDipole &Q, const SS_GTO &P, int m)
{
    throw SireError::incomplete_code("Not implemented", CODELOC);
    return 0;
}

double SQUIRE_EXPORT electron_integral(const SS_GTO &P, const SS_GTO &Q)
{
    // (si sj | sk sl) = K_ij K_kl F0{ (zeta*eta/(zeta+eta)) (P-Q)^2 }

    const double zeta_plus_eta = P.zeta() + Q.eta();
    const double zeta_times_eta = P.zeta() * Q.eta();

    const double R2 = Vector::distance2( P.P(), Q.Q() );
    const double T = (zeta_times_eta/zeta_plus_eta) * R2;

    return P.K() * Q.K() * boys_f0(T) / std::sqrt(zeta_plus_eta);
}

double SQUIRE_EXPORT electron_integral(const SS_GTO &P, const SS_GTO &Q, int m)
{
    // (si sj | sk sl)^m = K_ij K_kl Fm{ (zeta*eta/(zeta+eta)) (P-Q)^2 }

    const double zeta_plus_eta = P.zeta() + Q.eta();
    const double zeta_times_eta = P.zeta() * Q.eta();

    const double R2 = Vector::distance2( P.P(), Q.Q() );
    const double T = (zeta_times_eta/zeta_plus_eta) *   R2;

    return P.K() * Q.K() * boys(m, T) / std::sqrt(zeta_plus_eta);
}

/////////////
///////////// Integrals involving just SS_GTO and CSS_GTO
/////////////

double SQUIRE_EXPORT electron_integral(const CSS_GTO &P, const SS_GTO &Q)
{
    double integral_sum(0);
    
    const int npairs = P.nPairs();
    
    const double *zeta = P.zeta().constData();
    const Vector *p = P.P().constData();
    const double *K = P.K().constData();
    
    for (int i=0; i<npairs; ++i)
    {
        const double zeta_plus_eta = zeta[i] + Q.eta();
        const double zeta_times_eta = zeta[i] * Q.eta();

        const double R2 = Vector::distance2( p[i], Q.Q() );

        const double T = (zeta_times_eta/zeta_plus_eta) * R2;

        integral_sum += K[i] * boys_f0(T) / zeta_plus_eta;
    }
    
    integral_sum *= Q.K();
    
    return integral_sum;
}

double SQUIRE_EXPORT electron_integral(const CSS_GTO &P, const SS_GTO &Q, int m)
{
    double integral_sum(0);
    
    const int npairs = P.nPairs();
    
    const double *zeta = P.zeta().constData();
    const Vector *p = P.P().constData();
    const double *K = P.K().constData();
    
    for (int i=0; i<npairs; ++i)
    {
        const double zeta_plus_eta = zeta[i] + Q.eta();
        const double zeta_times_eta = zeta[i] * Q.eta();

        const double R2 = Vector::distance2( p[i], Q.Q() );

        const double T = (zeta_times_eta/zeta_plus_eta) * R2;

        integral_sum += K[i] * boys(m, T) / zeta_plus_eta;
    }
    
    integral_sum *= Q.K();
    
    return integral_sum;
}

double SQUIRE_EXPORT electron_integral(const SS_GTO &P, const CSS_GTO &Q)
{
    return electron_integral(Q, P);
}

double SQUIRE_EXPORT electron_integral(const SS_GTO &P, const CSS_GTO &Q, int m)
{
    return electron_integral(Q, P, m);
}

/////////////
///////////// Integrals involving just CSS_GTO
/////////////
double SQUIRE_EXPORT kinetic_integral(const CSS_GTO &P)
{
    // (s|nabla|s) = chi {3 - 2 chi (A-B)^2} (s||s)

    const int npairs = P.nPairs();
    
    const double *chi = P.chi().constData();
    const double two_R2 = P.R2() * 2;
    const double *overlap = P.overlap().constData();
    
    double integral_sum = 0;
    
    for (int i=0; i<npairs; ++i)
    {
        integral_sum += chi[i] * (3 - two_R2*chi[i]) * overlap[i];
    }
    
    return integral_sum;
}

double SQUIRE_EXPORT overlap_integral(const CSS_GTO &P)
{
    const int npairs = P.nPairs();
    const double *odata = P.overlap().constData();

    double integral_sum = 0;

    for (int i=0; i<npairs; ++i)
    {
        integral_sum += odata[i];
    }
    
    return integral_sum;
}

double SQUIRE_EXPORT potential_integral(const PointCharge &Q, const CSS_GTO &P)
{
    const int npairs = P.nPairs();
    
    double integral_sum = 0;

    const double *eta = P.eta().constData();
    const Vector *p = P.P().constData();
    const double *overlap = P.overlap().constData();
    
    for (int i=0; i<npairs; ++i)
    {
        const double U = eta[i] * ((p[i] - Q.center()).length2());
        
        integral_sum += std::sqrt(eta[i] * one_over_pi) * overlap[i] * boys_f0(U);
    }
    
    integral_sum *= (-2 * Q.charge());
    
    return integral_sum;
}

double SQUIRE_EXPORT potential_integral(const PointCharge &Q, const CSS_GTO &P, int m)
{
    const int npairs = P.nPairs();
    
    double integral_sum = 0;

    const double *eta = P.eta().constData();
    const Vector *p = P.P().constData();
    const double *overlap = P.overlap().constData();
    
    for (int i=0; i<npairs; ++i)
    {
        const double U = eta[i] * ((p[i] - Q.center()).length2());
        
        integral_sum += std::sqrt(eta[i] * one_over_pi) * overlap[i] * boys(m, U);
    }
    
    integral_sum *= (-2 * Q.charge());
    
    return integral_sum;
}

double SQUIRE_EXPORT potential_integral(const PointDipole &Q, const CSS_GTO &P)
{
    throw SireError::incomplete_code("Not implemented", CODELOC);
    return 0;
}

double SQUIRE_EXPORT potential_integral(const PointDipole &Q, const CSS_GTO &P, int m)
{
    throw SireError::incomplete_code("Not implemented", CODELOC);
    return 0;
}

double SQUIRE_EXPORT electron_integral(const CSS_GTO &P, const CSS_GTO &Q)
{
    double integral_sum(0);
    
    const int np = P.nPairs();
    const int nq = Q.nPairs();
    
    const double *zeta = P.zeta().constData();
    const double *eta = Q.eta().constData();
    const double *K_AB = P.K_AB().constData();
    const double *K_CD = Q.K_CD().constData();
    
    const Vector *p = P.P().constData();
    const Vector *q = Q.Q().constData();
    
    for (int i=0; i<np; ++i)
    {
        const double zeta_i = zeta[i];
        const Vector &p_i = p[i];

        double my_sum = 0;
    
        for (int j=0; j<nq; ++j)
        {
            const double zeta_plus_eta = zeta_i + eta[j];
            const double zeta_times_eta = zeta_i * eta[j];

            const double R2 = Vector::distance2( p_i, q[j] ); 

            const double T = (zeta_times_eta/zeta_plus_eta) * R2;

            my_sum += K_CD[j] * boys_f0(T) / std::sqrt(zeta_plus_eta);
        }
        
        integral_sum += K_AB[i] * my_sum;
    }
    
    return integral_sum;
}

double SQUIRE_EXPORT electron_integral(const CSS_GTO &P, const CSS_GTO &Q, int m)
{
    double integral_sum(0);
    
    const int np = P.nPairs();
    const int nq = Q.nPairs();
    
    const double *zeta = P.zeta().constData();
    const double *eta = Q.eta().constData();
    const double *K_AB = P.K_AB().constData();
    const double *K_CD = P.K_CD().constData();
    
    const Vector *p = P.P().constData();
    const Vector *q = Q.Q().constData();
    
    for (int i=0; i<np; ++i)
    {
        const double zeta_i = zeta[i];
        const Vector &p_i = p[i];

        double my_sum = 0;
    
        for (int j=0; j<nq; ++j)
        {
            const double zeta_plus_eta = zeta_i + eta[j];
            const double zeta_times_eta = zeta_i * eta[j];

            const double R2 = Vector::distance2( p_i, q[j] ); 

            const double T = (zeta_times_eta/zeta_plus_eta) * R2;

            my_sum += K_CD[j] * boys(m, T) / zeta_plus_eta;
        }
        
        integral_sum += K_AB[i] * my_sum;
    }
    
    return integral_sum;
}

} // end of namespace Squire
