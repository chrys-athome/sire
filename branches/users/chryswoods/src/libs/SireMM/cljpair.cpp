
#include <QObject>

#include "cljpair.h"

#include "SireMaths/maths.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireMM;

static const RegisterMetaType<CLJPair> r_cljpair("SireMM::CLJPair");

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const CLJPair &cljpair)
{
    writeHeader(ds, r_cljpair, 1) << cljpair.charge2() << cljpair.sigma()
                                  << cljpair.epsilon();
    
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, CLJPair &cljpair)
{
    VersionID v = readHeader(ds, r_cljpair);
    
    if (v == 1)
    {
        double chg2,sig,eps;
        
        ds >> chg2 >> sig >> eps;
        
        cljpair = CLJPair(chg2, sig, eps);
    }
    else
        throw version_error(v, "1", r_cljpair, CODELOC);
    
    return ds;
}

/** Construct a null pair */
CLJPair::CLJPair() : chg2(0.0), sig(0.0), eps(0.0)
{}

/** Construct a CLJPair that has charge^2 = chg2, combined sigma = sig and combined 
    epsilon = eps */
CLJPair::CLJPair(double c2, double s, double e) : chg2(c2), sig(s), eps(e)
{}

/** Construct a CLJPair from the passed CLJ parameter */
CLJPair::CLJPair(const CLJParameter &cljparam)
        : chg2( SireMaths::pow_2(cljparam.charge()) ),
          sig( cljparam.sigma() ),
          eps( cljparam.epsilon() )
{}

/** Destructor */
CLJPair::~CLJPair()
{}

/** Return a CLJPair that represents the combination of parameters clj0 and clj1 using
    arithmetic (Lorentz-Berthelot) combining rules */
CLJPair CLJPair::arithmetic(const CLJParameter &clj0, const CLJParameter &clj1)
{
    return CLJPair( clj0.charge() * clj1.charge(), 
                    0.5 * ( clj0.sigma() + clj1.sigma() ),
                    clj0.sqrtEpsilon() * clj1.sqrtEpsilon() );
}

/** Return the CLJPair that represents the combination of parameters clj0 and clj1 using
    geometric (OPLS type) combining rules */
CLJPair CLJPair::geometric(const CLJParameter &clj0, const CLJParameter &clj1)
{
    return CLJPair( clj0.charge() * clj1.charge(),
                    clj0.sqrtSigma() * clj1.sqrtSigma(),
                    clj0.sqrtEpsilon() * clj1.sqrtEpsilon() );
}

/** Return the LJ 'A' parameter */
double CLJPair::A() const
{
    return double(4.0) * epsilon() * SireMaths::pow_12( sigma() );
}    

/** Return the LJ 'B' parameter */
double CLJPair::B() const
{
    return double(4.0) * epsilon() * SireMaths::pow_6( sigma() );
}

/** Return the LJ 'rmin' parameter - this is the location of the minimum.
 
    rmin = 2^(1/6) * sigma
*/
double CLJPair::rmin() const
{
    // 2.0 ^ (1/6) = 1.122462048309372981439932526193103967671
    return sigma() * double(1.122462048309372981439932526193103967671);
}

/** Return a string representation of the CLJPair */
QString CLJPair::toString() const
{
    return QObject::tr("CLJPair( Charge^2=%1, Sigma=%2, Epsilon=%3 )")
                        .arg(charge2()).arg(sigma()).arg(epsilon());
}
