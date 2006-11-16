#ifndef SIREMM_CLJPAIR_H
#define SIREMM_CLJPAIR_H

#include "SireBase/pairmatrix.hpp"

#include "cljparameter.h"

SIRE_BEGIN_HEADER

namespace SireMM
{
class CLJPair;
}

class QDataStream;
QDataStream& operator<<(QDataStream&, const SireMM::CLJPair&);
QDataStream& operator>>(QDataStream&, SireMM::CLJPair&);

namespace SireMM
{

class CLJPair;

/** Typedef a matrix to hold all of the CLJ pairs */
typedef SireBase::PairMatrix<CLJPair> CLJPairMatrix;

/** Typedef a function pointer that may be used to combine together CLJ parameters */
typedef CLJPair (*CLJCombineFuncPtr)(const CLJParameter&, const CLJParameter&);

/**
A CLJPair holds the combined charge and Lennard Jones parameters of two CLJParameter objects.

@author Christopher Woods
*/
class SIREMM_EXPORT CLJPair
{
public:
    CLJPair();
    CLJPair(const CLJParameter &cljparam);
    CLJPair(double chg2, double sig, double eps);
    ~CLJPair();

    double charge2() const;
    double sigma() const;
    double epsilon() const;

    double A() const;
    double B() const;
    double rmin() const;

    QString toString() const;

    static CLJPair arithmetic(const CLJParameter &a, const CLJParameter &b);
    static CLJPair geometric(const CLJParameter &a, const CLJParameter &b);

private:
    /** The charge^2 of the two CLJParameters that form this pair, in
        |electron charges|^2 */
    double chg2;

    /** The combined sigma parameter of the two CLJParameters, in A */
    double sig;

    /** The combined epsilon parameters of the two CLJParameters, in kcal mol-1 */
    double eps;
};

/** Return the charge^2 for this combined parameter */
inline double CLJPair::charge2() const
{
    return chg2;
}

/** Return the combined sigma parameter */
inline double CLJPair::sigma() const
{
    return sig;
}

/** Return the combined epsilon parameter */
inline double CLJPair::epsilon() const
{
    return eps;
}

/** Return a CLJPair that represents the combination of parameters clj0 and clj1 using
    arithmetic (Lorentz-Berthelot) combining rules */
inline CLJPair CLJPair::arithmetic(const CLJParameter &clj0, const CLJParameter &clj1)
{
    return CLJPair( clj0.charge() * clj1.charge(),
                    0.5 * ( clj0.sigma() + clj1.sigma() ),
                    clj0.sqrtEpsilon() * clj1.sqrtEpsilon() );
}

/** Return the CLJPair that represents the combination of parameters clj0 and clj1 using
    geometric (OPLS type) combining rules */
inline CLJPair CLJPair::geometric(const CLJParameter &clj0, const CLJParameter &clj1)
{
    return CLJPair( clj0.charge() * clj1.charge(),
                    clj0.sqrtSigma() * clj1.sqrtSigma(),
                    clj0.sqrtEpsilon() * clj1.sqrtEpsilon() );
}

}

Q_DECLARE_TYPEINFO(SireMM::CLJPair, Q_MOVABLE_TYPE);
Q_DECLARE_METATYPE(SireMM::CLJPair)

SIRE_END_HEADER

#endif
