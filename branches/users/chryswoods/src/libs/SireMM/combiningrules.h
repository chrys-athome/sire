/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
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

#ifndef SIREMM_COMBININGRULES_H
#define SIREMM_COMBININGRULES_H

#include <QVector>

#include "SireBase/sharedpolypointer.hpp"
#include "SireBase/property.h"

#include "cljpair.h"
#include "ljpair.h"

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
class SIREMM_EXPORT CombiningRuleBase : public SireBase::PropertyBase
{

friend QDataStream& ::operator<<(QDataStream&, const CombiningRuleBase&);
friend QDataStream& ::operator>>(QDataStream&, CombiningRuleBase&);

public:
    CombiningRuleBase();
    CombiningRuleBase(const CombiningRuleBase&);

    ~CombiningRuleBase();

    virtual CombiningRuleBase* clone() const=0;

    virtual void combine(const QVector<CLJParameter> &clj0,
                         const QVector<CLJParameter> &clj1,
                         CLJPairMatrix &cljmatrix) const=0;

    virtual void combine(const QVector<CLJParameter> &cljs,
                         CLJPairMatrix &cljmatrix) const=0;

    virtual void combine(const QVector<ChargeParameter> &chg0,
                         const QVector<LJParameter> &lj0,
                         const QVector<ChargeParameter> &chg1,
                         const QVector<LJParameter> &lj1,
                         CLJPairMatrix &cljmatrix) const=0;

    virtual void combine(const QVector<ChargeParameter> &chgs,
                         const QVector<LJParameter> &ljs,
                         CLJPairMatrix &cljmatrix) const=0;

    virtual void combine(const QVector<LJParameter> &lj0,
                         const QVector<LJParameter> &lj1,
                         LJPairMatrix &ljmatrix) const=0;

    virtual void combine(const QVector<LJParameter> &ljs,
                         LJPairMatrix &ljmatrix) const=0;
};

/**
This class implements arithmetic (Lorentz-Berthelot) combining rules

@author Christopher Woods
*/
class SIREMM_EXPORT ArithmeticCombiningRules 
         : public SireBase::ConcreteProperty<ArithmeticCombiningRules,
                                             CombiningRuleBase>
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
    
    using SireBase::PropertyBase::operator=;
    using SireBase::PropertyBase::operator==;
    using SireBase::PropertyBase::operator!=;

    ArithmeticCombiningRules& operator=(const ArithmeticCombiningRules &other);
    
    bool operator==(const ArithmeticCombiningRules &other) const;
    bool operator!=(const ArithmeticCombiningRules &other) const;
    
    void combine(const QVector<CLJParameter> &clj0,
                 const QVector<CLJParameter> &clj1,
                 CLJPairMatrix &cljmatrix) const;

    void combine(const QVector<CLJParameter> &cljs,
                 CLJPairMatrix &cljmatrix) const;

    void combine(const QVector<ChargeParameter> &chg0,
                 const QVector<LJParameter> &lj0,
                 const QVector<ChargeParameter> &chg1,
                 const QVector<LJParameter> &lj1,
                 CLJPairMatrix &cljmatrix) const;

    void combine(const QVector<ChargeParameter> &chgs,
                 const QVector<LJParameter> &ljs,
                 CLJPairMatrix &cljmatrix) const;

    void combine(const QVector<LJParameter> &lj0,
                 const QVector<LJParameter> &lj1,
                 LJPairMatrix &ljmatrix) const;

    void combine(const QVector<LJParameter> &ljs,
                 LJPairMatrix &ljmatrix) const;
};

/**
This class implements geometric combining rules

@author Christopher Woods
*/
class SIREMM_EXPORT GeometricCombiningRules 
         : public SireBase::ConcreteProperty<GeometricCombiningRules,
                                             CombiningRuleBase>
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

    using PropertyBase::operator=;
    using PropertyBase::operator==;
    using PropertyBase::operator!=;

    GeometricCombiningRules& operator=(const GeometricCombiningRules &other);
    
    bool operator==(const GeometricCombiningRules &other) const;
    bool operator!=(const GeometricCombiningRules &other) const;
    
    void combine(const QVector<CLJParameter> &clj0,
                 const QVector<CLJParameter> &clj1,
                 CLJPairMatrix &cljmatrix) const;

    void combine(const QVector<CLJParameter> &cljs,
                 CLJPairMatrix &cljmatrix) const;

    void combine(const QVector<ChargeParameter> &chg0,
                 const QVector<LJParameter> &lj0,
                 const QVector<ChargeParameter> &chg1,
                 const QVector<LJParameter> &lj1,
                 CLJPairMatrix &cljmatrix) const;

    void combine(const QVector<ChargeParameter> &chgs,
                 const QVector<LJParameter> &ljs,
                 CLJPairMatrix &cljmatrix) const;

    void combine(const QVector<LJParameter> &lj0,
                 const QVector<LJParameter> &lj1,
                 LJPairMatrix &ljmatrix) const;

    void combine(const QVector<LJParameter> &ljs,
                 LJPairMatrix &ljmatrix) const;
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
    CombiningRules(const SireBase::Property &property);

    CombiningRules(const CombiningRules &other);

    ~CombiningRules();

    CombiningRules& operator=(const CombiningRules &other);
    CombiningRules& operator=(const CombiningRuleBase &other);
    CombiningRules& operator=(const SireBase::Property &property);

    const char* what() const;

    void combine(const QVector<CLJParameter> &clj0,
                 const QVector<CLJParameter> &clj1,
                 CLJPairMatrix &cljmatrix) const;

    void combine(const QVector<CLJParameter> &cljs,
                 CLJPairMatrix &cljmatrix) const;

    void combine(const QVector<ChargeParameter> &chg0,
                 const QVector<LJParameter> &lj0,
                 const QVector<ChargeParameter> &chg1,
                 const QVector<LJParameter> &lj1,
                 CLJPairMatrix &cljmatrix) const;

    void combine(const QVector<ChargeParameter> &chgs,
                 const QVector<LJParameter> &ljs,
                 CLJPairMatrix &cljmatrix) const;

    void combine(const QVector<LJParameter> &lj0,
                 const QVector<LJParameter> &lj1,
                 LJPairMatrix &ljmatrix) const;

    void combine(const QVector<LJParameter> &ljs,
                 LJPairMatrix &ljmatrix) const;

    template<class T>
    bool isA() const
    {
        return d->isA<T>();
    }

    template<class T>
    const T& asA() const
    {
        return d->asA<T>();
    }

    /** Allow implicit conversion to a Property */
    operator SireBase::Property() const
    {
        return SireBase::Property(*d);
    }

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

/** Combine all pairs of the CLJ parameters in 'cljs' and place the results
    in the matrix 'cljmatrix' */
inline void CombiningRules::combine(const QVector<CLJParameter> &cljs,
                                    CLJPairMatrix &cljmatrix) const
{
    d->combine(cljs, cljmatrix);
}

/** Combine the charge parameters in chg0 and chg1 and the LJ parameters
    in lj0 and lj1 and place the results in the matrix 'cljmatrix' */
inline void CombiningRules::combine(const QVector<ChargeParameter> &chg0,
                                    const QVector<LJParameter> &lj0,
                                    const QVector<ChargeParameter> &chg1,
                                    const QVector<LJParameter> &lj1,
                                    CLJPairMatrix &cljmatrix) const
{
    d->combine(chg0, lj0, chg1, lj1, cljmatrix);
}

/** Combine all pairs of the charge parameters in 'chgs' and the LJ
    parameters in 'ljs' and place the results in the matrix 'cljmatrix' */
inline void CombiningRules::combine(const QVector<ChargeParameter> &chgs,
                                    const QVector<LJParameter> &ljs,
                                    CLJPairMatrix &cljmatrix) const
{
    d->combine(chgs, ljs, cljmatrix);
}

/** Combine the LJ parameters in 'lj0' and 'lj1' and place the results
    in the matrix 'ljmatrix' */
inline void CombiningRules::combine(const QVector<LJParameter> &lj0,
                                    const QVector<LJParameter> &lj1,
                                    LJPairMatrix &ljmatrix) const
{
    d->combine(lj0, lj1, ljmatrix);
}

/** Combine all pairs of the LJ parameters in 'ljs' and place the results
    in the matrix 'ljmatrix' */
inline void CombiningRules::combine(const QVector<LJParameter> &ljs,
                                    LJPairMatrix &ljmatrix) const
{
    d->combine(ljs, ljmatrix);
}

}

Q_DECLARE_METATYPE(SireMM::CombiningRules)
Q_DECLARE_METATYPE(SireMM::ArithmeticCombiningRules)
Q_DECLARE_METATYPE(SireMM::GeometricCombiningRules)

SIRE_END_HEADER

#endif
