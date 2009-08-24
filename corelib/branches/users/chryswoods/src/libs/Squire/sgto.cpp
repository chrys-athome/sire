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
    
    ds << ssgto._P << ssgto._R2 << ssgto._eta << ssgto._chi << ssgto._G;
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SQUIRE_EXPORT &operator>>(QDataStream &ds, SS_GTO &ssgto)
{
    VersionID v = readHeader(ds, r_ssgto);
    
    if (v == 1)
    {
        ds >> ssgto._P >> ssgto._R2 >> ssgto._eta >> ssgto._chi >> ssgto._G;
    }
    else
        throw version_error(v, "1", r_ssgto, CODELOC);
    
    return ds;
}

/** Null constructor */
SS_GTO::SS_GTO() : _R2(0), _eta(0), _chi(0), _G(0)
{}

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
    _G = a.scale() * b.scale() * std::exp(-_chi*_R2);
}

/** Copy constructor */
SS_GTO::SS_GTO(const SS_GTO &other) 
       : _P(other._P), _R2(other._R2), _eta(other._eta), _chi(other._chi), _G(other._G)
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
    _G = other._G;
    
    return *this;
}

/** Comparison operator */
bool SS_GTO::operator==(const SS_GTO &other) const
{
    return _P == other._P and _R2 == other._R2 and 
           _eta == other._eta and _G == other._G;
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

/** Return the scale factor for this shell pair */
double SS_GTO::G() const
{
    return _G;
}

/** Synonym for SS_GTO::G() */
double SS_GTO::G_AB() const
{
    return SS_GTO::G();
}

/** Synonym for SS_GTO::G_CD() */
double SS_GTO::G_CD() const
{
    return SS_GTO::G();
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
    
    ds << cssgto._P << cssgto._R2 << cssgto._eta << cssgto._chi << cssgto._G;
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SQUIRE_EXPORT &operator>>(QDataStream &ds, CSS_GTO &cssgto)
{
    VersionID v = readHeader(ds, r_cssgto);
    
    if (v == 1)
    {
        ds >> cssgto._P >> cssgto._R2 >> cssgto._eta >> cssgto._chi >> cssgto._G;
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
    _G = NMatrix::createRowMajor(1, 1);

    Vector *Pdata = _P.data();
    double *etadata = _eta.data();
    double *chidata = _chi.data();
    double *Gdata = _G.data();

    //the product of two Gaussians is a Gaussian :-)
    const double alpha_times_beta = a.alpha() * b.alpha();
    const double alpha_plus_beta = a.alpha() + b.alpha();
    
    Pdata[0] = (a.alpha()*A + b.alpha()*B) / alpha_plus_beta;
    etadata[0] = alpha_plus_beta;
    chidata[0] = alpha_times_beta / alpha_plus_beta;
    Gdata[0] = a.scale() * b.scale() * std::exp(-chidata[0]*_R2);
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
    _G = NMatrix::createRowMajor(1, n);

    Vector *Pdata = _P.data();
    double *etadata = _eta.data();
    double *chidata = _chi.data();
    double *Gdata = _G.data();

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
        Gdata[i] = a.scale() * bscl[i] * std::exp(-chidata[i]*_R2);
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
    _G = NMatrix::createRowMajor(na, nb);

    Vector *Pdata = _P.data();
    double *etadata = _eta.data();
    double *chidata = _chi.data();
    double *Gdata = _G.data();

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
            Gdata[idx] = a_scale * bscl[j] * std::exp(-chidata[idx]*_R2);
            
            ++idx;
        }
    }
}

/** Copy constructor */
CSS_GTO::CSS_GTO(const CSS_GTO &other) 
       : _P(other._P), _R2(other._R2), _eta(other._eta), _chi(other._chi), _G(other._G)
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
    _G = other._G;
    
    return *this;
}

/** Comparison operator */
bool CSS_GTO::operator==(const CSS_GTO &other) const
{
    return _P == other._P and _R2 == other._R2 and 
           _eta == other._eta and _G == other._G;
}

/** Comparison operator */
bool CSS_GTO::operator!=(const CSS_GTO &other) const
{
    return not this->operator==(other);
}

/** The number of primitive pairs in this shell-pair */
int CSS_GTO::nPairs() const
{
    return _G.memory().count();
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

/** Return the scale factor for this shell pair */
const NMatrix& CSS_GTO::G() const
{
    return _G;
}

/** Synonym for CSS_GTO::G() */
const NMatrix& CSS_GTO::G_AB() const
{
    return CSS_GTO::G();
}

/** Synonym for CSS_GTO::G_CD() */
const NMatrix& CSS_GTO::G_CD() const
{
    return CSS_GTO::G();
}

const char* CSS_GTO::typeName()
{
    return QMetaType::typeName( qMetaTypeId<CSS_GTO>() );
}

////////////
//////////// Integrals involving just SS_GTO shell pairs
////////////
//////////// These are derived in;
//////////// Molecular Integrals Over Gaussian Basis Functions
//////////// Peter M. W. Gill
//////////// Advanced Quantum Chemistry, 1994
////////////

namespace Squire
{

const double two_pi_to_2_5 = 2 * std::pow(pi, 2.5);

/////////////
///////////// Integrals involving just SS_GTO
/////////////
double SQUIRE_EXPORT kinetic_integral(const SS_GTO &P)
{
    return P.chi() * std::pow( pi / P.eta(), 1.5 ) * (3 - 2*P.chi()*P.R2()) * P.G();
}

double SQUIRE_EXPORT overlap_integral(const SS_GTO &P)
{
    return std::pow( pi / P.eta(), 1.5 ) * P.G();
}

double SQUIRE_EXPORT potential_integral(const PointCharge &Q, const SS_GTO &P)
{
    const double T = P.eta() * ((P.P() - Q.center()).length2());
    
    return -2 * P.G() * Q.charge() * pi * boys_f0(T) / P.eta();
}

double SQUIRE_EXPORT potential_integral(const PointDipole &Q, const SS_GTO &P)
{
    throw SireError::incomplete_code("Not implemented", CODELOC);
    return 0;
}

double SQUIRE_EXPORT electron_integral(const SS_GTO &P, const SS_GTO &Q)
{
    const double zeta_plus_eta = P.zeta() + Q.eta();
    const double zeta_times_eta = P.zeta() * Q.eta();

    const double R2 = Vector::distance2( P.P(), Q.Q() );
    const double T = (zeta_times_eta/zeta_plus_eta) * R2;

    return (two_pi_to_2_5 / (zeta_times_eta * std::sqrt(zeta_plus_eta))) *
           P.G_AB() * Q.G_CD() *
           boys_f0( T );
}

/////////////
///////////// Integrals involving just SS_GTO and CSS_GTO
/////////////

double SQUIRE_EXPORT electron_integral(const CSS_GTO &P, const SS_GTO &Q)
{
    double integral_sum(0);
    
    const int npairs = P.nPairs();
    
    for (int i=0; i<npairs; ++i)
    {
        const double zeta_plus_eta = P.zeta().constData()[i] + Q.eta();
        const double zeta_times_eta = P.zeta().constData()[i] * Q.eta();

        const double R2 = Vector::distance2( P.P().constData()[i], Q.Q() );

        const double T = (zeta_times_eta/zeta_plus_eta) * R2;

        integral_sum += (two_pi_to_2_5 / (zeta_times_eta * std::sqrt(zeta_plus_eta))) *
                         P.G().constData()[i] * Q.G_CD() *
                         boys_f0( T );
    }
    
    return integral_sum;
}

double SQUIRE_EXPORT electron_integral(const SS_GTO &P, const CSS_GTO &Q)
{
    return electron_integral(Q, P);
}

/////////////
///////////// Integrals involving just CSS_GTO
/////////////
double SQUIRE_EXPORT kinetic_integral(const CSS_GTO &P)
{
    const int npairs = P.nPairs();
    
    double integral_sum = 0;
    
    qDebug() << "npairs ==" << npairs;
    
    for (int i=0; i<npairs; ++i)
    {
        const double val = P.chi().constData()[i] * 
                            std::pow( pi / P.eta().constData()[i], 1.5 ) * 
                                       (3 - 2*P.chi().constData()[i] * P.R2()) * 
                                                P.G().constData()[i];

        integral_sum += val;
        qDebug() << val;
    }
    
    return integral_sum;
}

double SQUIRE_EXPORT overlap_integral(const CSS_GTO &P)
{
    const int npairs = P.nPairs();

    double integral_sum = 0;

    for (int i=0; i<npairs; ++i)
    {
        integral_sum += std::pow( pi / P.eta().constData()[i], 1.5 ) * 
                            P.G().constData()[i];
    }
    
    return integral_sum;
}

double SQUIRE_EXPORT potential_integral(const PointCharge &Q, const CSS_GTO &P)
{
    const int npairs = P.nPairs();
    
    double integral_sum = 0;
    
    for (int i=0; i<npairs; ++i)
    {
        const double T = P.eta().constData()[i] * 
                            ((P.P().constData()[i] - Q.center()).length2());
    
        integral_sum += -2 * P.G().constData()[i] * Q.charge() * pi * boys_f0(T) / 
                                    P.eta().constData()[i];
    }
    
    return integral_sum;
}

double SQUIRE_EXPORT potential_integral(const PointDipole &Q, const CSS_GTO &P)
{
    throw SireError::incomplete_code("Not implemented", CODELOC);
    return 0;
}

double SQUIRE_EXPORT electron_integral(const CSS_GTO &P, const CSS_GTO &Q)
{
    double integral_sum(0);
    
    const int np = P.nPairs();
    const int nq = Q.nPairs();
    
    for (int i=0; i<np; ++i)
    {
        for (int j=0; j<nq; ++j)
        {
            const double zeta_plus_eta = P.zeta().constData()[i] + Q.eta().constData()[j];
            const double zeta_times_eta = P.zeta().constData()[i] * Q.eta().constData()[j];

            const double R2 = Vector::distance2( P.P().constData()[i], 
                                                 Q.Q().constData()[j] );

            const double T = (zeta_times_eta/zeta_plus_eta) * R2;

            integral_sum += (two_pi_to_2_5 / (zeta_times_eta * std::sqrt(zeta_plus_eta))) *
                             P.G().constData()[i] * 
                             Q.G().constData()[j] *
                             boys_f0( T );
        }
    }
    
    return integral_sum;
}


} // end of namespace Squire
