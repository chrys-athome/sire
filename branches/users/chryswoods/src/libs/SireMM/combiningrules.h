/**
  * @file
  *
  * C++ Interface: CombiningRules, CombiningRuleBase,
  *                ArithmeticCombiningRules, GeometricCombiningRules
  *
  * Description:
  * Interface for CombiningRules, which is a thin wrapper
  * around the polymorphic CombiningRuleBase class hierarchy
  * (which include ArithmeticCombiningRules and GeometricCombiningRules)
  *
  * @author Christopher Woods, (C) 2006
  *
  * Copyright: See COPYING file that comes with this distribution
  *
  */
#ifndef SIREMM_COMBININGRULES_H
#define SIREMM_COMBININGRULES_H

#include <QVector>

#include "SireBase/sharedpolypointer.hpp"

#include "cljpair.h"

SIRE_BEGIN_HEADER

namespace SireMM
{
class CombiningRules;
class CombiningRuleBase;
class ArithmeticCombiningRules;
class GeometricCombiningRules;
}

QDataStream& operator<<(QDataStream&, const SireMM::CombiningRules&);
QDataStream& operator>>(QDataStream&, SireMM::CombiningRules&);

QDataStream& operator<<(QDataStream&, const SireMM::CombiningRuleBase&);
QDataStream& operator>>(QDataStream&, SireMM::CombiningRuleBase&);

QDataStream& operator<<(QDataStream&, const SireMM::ArithmeticCombiningRules&);
QDataStream& operator>>(QDataStream&, SireMM::ArithmeticCombiningRules&);

QDataStream& operator<<(QDataStream&, const SireMM::GeometricCombiningRules&);
QDataStream& operator>>(QDataStream&, SireMM::GeometricCombiningRules&);

namespace SireMM
{

using SireBase::SharedPolyPointer;

/**
This is the base class of all Combining Rules classes - these provide the
rules used to combine atomic parameters for atom-atom interaction energy
calculations.

@author Christopher Woods
*/
class SIREMM_EXPORT CombiningRuleBase : public QSharedData
{

friend QDataStream& ::operator<<(QDataStream&, const CombiningRuleBase&);
friend QDataStream& ::operator>>(QDataStream&, CombiningRuleBase&);

public:
    CombiningRuleBase();
    CombiningRuleBase(const CombiningRuleBase&);

    virtual ~CombiningRuleBase();

    virtual CombiningRuleBase* clone() const=0;

    virtual const char* what() const=0;

    virtual void combine(const QVector<CLJParameter> &clj0,
                         const QVector<CLJParameter> &clj1,
                         CLJPairMatrix &cljmatrix) const=0;

    virtual void combine(const QVector<CLJParameter> &clj,
                         CLJPairMatrix &cljmatrix) const=0;
};

/**
This class implements arithmetic (Lorentz-Berthelot) combining rules

@author Christopher Woods
*/
class SIREMM_EXPORT ArithmeticCombiningRules : public CombiningRuleBase
{

friend QDataStream& ::operator<<(QDataStream&, const ArithmeticCombiningRules&);
friend QDataStream& ::operator>>(QDataStream&, ArithmeticCombiningRules&);

public:
    ArithmeticCombiningRules();
    ArithmeticCombiningRules(const ArithmeticCombiningRules &other);

    ~ArithmeticCombiningRules();

    static const char* typeName()
    {
        return "SireMM::ArithmeticCombiningRules";
    }

    const char* what() const
    {
        return ArithmeticCombiningRules::typeName();
    }

    ArithmeticCombiningRules* clone() const
    {
        return new ArithmeticCombiningRules(*this);
    }

    void combine(const QVector<CLJParameter> &clj0,
                 const QVector<CLJParameter> &clj1,
                 CLJPairMatrix &cljmatrix) const;

    void combine(const QVector<CLJParameter> &clj,
                 CLJPairMatrix &cljmatrix) const;
};

/**
This class implements geometric combining rules

@author Christopher Woods
*/
class SIREMM_EXPORT GeometricCombiningRules : public CombiningRuleBase
{

friend QDataStream& ::operator<<(QDataStream&, const GeometricCombiningRules&);
friend QDataStream& ::operator>>(QDataStream&, GeometricCombiningRules&);

public:
    GeometricCombiningRules();
    GeometricCombiningRules(const GeometricCombiningRules &other);

    ~GeometricCombiningRules();

    static const char* typeName()
    {
        return "SireMM::GeometricCombiningRules";
    }

    const char* what() const
    {
        return GeometricCombiningRules::typeName();
    }

    GeometricCombiningRules* clone() const
    {
        return new GeometricCombiningRules(*this);
    }

    void combine(const QVector<CLJParameter> &clj0,
                 const QVector<CLJParameter> &clj1,
                 CLJPairMatrix &cljmatrix) const;

    void combine(const QVector<CLJParameter> &clj,
                 CLJPairMatrix &cljmatrix) const;
};

/**
This class provides an abstraction for the different combining rules used to combine atomic parameters for atom-atom energies.

@author Christopher Woods
*/
class SIREMM_EXPORT CombiningRules
{

friend QDataStream& ::operator<<(QDataStream&, const CombiningRules&);
friend QDataStream& ::operator>>(QDataStream&, CombiningRules&);

public:
    CombiningRules();
    CombiningRules(const CombiningRuleBase &rules);

    CombiningRules(const CombiningRules &other);

    ~CombiningRules();

    CombiningRules& operator=(const CombiningRules &other);

    const char* what() const;

    void combine(const QVector<CLJParameter> &clj0,
                 const QVector<CLJParameter> &clj1,
                 CLJPairMatrix &cljmatrix) const;

    void combine(const QVector<CLJParameter> &clj,
                 CLJPairMatrix &cljmatrix) const;

private:
    /** Shared pointer to the combining rules themselves */
    SharedPolyPointer<CombiningRuleBase> d;
};

/** Combine the CLJ parameters in 'clj0' and 'clj1' and place the results
    in the matrix 'cljmatrix' */
inline void CombiningRules::combine(const QVector<CLJParameter> &clj0,
                             const QVector<CLJParameter> &clj1,
                             CLJPairMatrix &cljmatrix) const
{
    d->combine(clj0, clj1, cljmatrix);
}

/** Combine all pairs of the CLJ parameters in 'clj' and place the results
    in the matrix 'cljmatrix' */
inline void CombiningRules::combine(const QVector<CLJParameter> &clj,
                             CLJPairMatrix &cljmatrix) const
{
    d->combine(clj, cljmatrix);
}

}

Q_DECLARE_METATYPE(SireMM::CombiningRules)
Q_DECLARE_METATYPE(SireMM::ArithmeticCombiningRules)
Q_DECLARE_METATYPE(SireMM::GeometricCombiningRules)

SIRE_END_HEADER

#endif
