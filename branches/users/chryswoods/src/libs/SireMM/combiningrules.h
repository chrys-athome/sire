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

    virtual void combine(const QVector<ChargeParameter> &chg0,
                         const QVector<LJParameter> &lj0,
                         const QVector<ChargeParameter> &chg1,
                         const QVector<LJParameter> &lj1,
                         CLJPairMatrix &cljmatrix) const=0;

    virtual void combine(const QVector<ChargeParameter> &chgs,
                         const QVector<LJParameter> &ljs,
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

    void combine(const QVector<ChargeParameter> &chg0,
                 const QVector<LJParameter> &lj0,
                 const QVector<ChargeParameter> &chg1,
                 const QVector<LJParameter> &lj1,
                 CLJPairMatrix &cljmatrix) const;

    void combine(const QVector<ChargeParameter> &chgs,
                 const QVector<LJParameter> &ljs,
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

    void combine(const QVector<ChargeParameter> &chg0,
                 const QVector<LJParameter> &lj0,
                 const QVector<ChargeParameter> &chg1,
                 const QVector<LJParameter> &lj1,
                 CLJPairMatrix &cljmatrix) const;

    void combine(const QVector<ChargeParameter> &chgs,
                 const QVector<LJParameter> &ljs,
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

    void combine(const QVector<ChargeParameter> &chg0,
                 const QVector<LJParameter> &lj0,
                 const QVector<ChargeParameter> &chg1,
                 const QVector<LJParameter> &lj1,
                 CLJPairMatrix &cljmatrix) const;

    void combine(const QVector<ChargeParameter> &chgs,
                 const QVector<LJParameter> &ljs,
                 CLJPairMatrix &cljmatrix) const;

private:
    /** Shared pointer to the combining rules themselves */
    SharedPolyPointer<CombiningRuleBase> d;
};

/** Combine the CLJ parameters in 'clj0' and 'clj1' and place the results
    in the matrix 'cljmatrix' */
inline void CombiningRules::combine(const QVector<ChargeParameter> &chg0,
                                    const QVector<LJParameter> &lj0,
                                    const QVector<ChargeParameter> &chg1,
                                    const QVector<LJParameter> &lj1,
                                    CLJPairMatrix &cljmatrix) const
{
    d->combine(chg0, lj0, chg1, lj1, cljmatrix);
}

/** Combine all pairs of the CLJ parameters in 'clj' and place the results
    in the matrix 'cljmatrix' */
inline void CombiningRules::combine(const QVector<ChargeParameter> &chgs,
                                    const QVector<LJParameter> &ljs,
                                    CLJPairMatrix &cljmatrix) const
{
    d->combine(chgs, ljs, cljmatrix);
}

}

Q_DECLARE_METATYPE(SireMM::CombiningRules)
Q_DECLARE_METATYPE(SireMM::ArithmeticCombiningRules)
Q_DECLARE_METATYPE(SireMM::GeometricCombiningRules)

SIRE_END_HEADER

#endif
