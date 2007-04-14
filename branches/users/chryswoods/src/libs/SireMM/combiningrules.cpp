/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2007  Christopher Woods
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

#include "combiningrules.h"

#include "SireError/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMM;
using namespace SireBase;
using namespace SireStream;

///////////
/////////// Implementation of CombiningRuleBase
///////////

static const RegisterMetaType<CombiningRuleBase>
                          r_combinebase(MAGIC_ONLY, "SireMM::CombiningRuleBase");

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds,
                                      const CombiningRuleBase &combinebase)
{
    writeHeader(ds, r_combinebase, 1)
               << static_cast<const PropertyBase&>(combinebase);

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds,
                                      CombiningRuleBase &combinebase)
{
    VersionID v = readHeader(ds, r_combinebase);

    if (v == 1)
    {
        ds >> static_cast<PropertyBase&>(combinebase);
    }
    else
        throw version_error(v, "1", r_combinebase, CODELOC);

    return ds;
}

/** Constructor */
CombiningRuleBase::CombiningRuleBase() : PropertyBase()
{}

/** Copy constructor */
CombiningRuleBase::CombiningRuleBase(const CombiningRuleBase &other)
                  : PropertyBase(other)
{}

/** Destructor */
CombiningRuleBase::~CombiningRuleBase()
{}

///////////
/////////// Implementation of ArithmeticCombiningRules
///////////

static const RegisterMetaType<ArithmeticCombiningRules> r_arith;

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds,
                                      const ArithmeticCombiningRules &arith)
{
    writeHeader(ds, r_arith, 1)
                      << static_cast<const CombiningRuleBase&>(arith);
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds,
                                      ArithmeticCombiningRules &arith)
{
    VersionID v = readHeader(ds, r_arith);

    if (v == 1)
    {
        ds >> static_cast<CombiningRuleBase&>(arith);
    }
    else
        throw version_error(v, "1", r_arith, CODELOC);

    return ds;
}

/** Constructor */
ArithmeticCombiningRules::ArithmeticCombiningRules()
                         : CombiningRuleBase()
{}

/** Copy constructor */
ArithmeticCombiningRules::ArithmeticCombiningRules(
                                    const ArithmeticCombiningRules &other)
                         : CombiningRuleBase(other)
{}

/** Destructor */
ArithmeticCombiningRules::~ArithmeticCombiningRules()
{}

/** Combine the LJ paramters using arithmetic combining
    rules and place the results in the matrix 'ljmatrix' */
void ArithmeticCombiningRules::combine(const QVector<LJParameter> &lj0,
                                        const QVector<LJParameter> &lj1,
                                        LJPairMatrix &ljmatrix) const
{
    int nlj0 = lj0.count();
    int nlj1 = lj1.count();

    ljmatrix.redimension(nlj0, nlj1);

    if (nlj0 > 0 and nlj1 > 0)
    {
        const LJParameter *lj0array = lj0.constData();
        const LJParameter *lj1array = lj1.constData();

        for (int i=0; i<nlj0; ++i)
        {
            const LJParameter &lj0param = lj0array[i];

            ljmatrix.setOuterIndex(i);

            for (int j=0; j<nlj1; ++j)
            {
                ljmatrix[j] = LJPair::arithmetic(lj0param, lj1array[j]);
            }
        }
    }
}

/** Combine the LJ paramters using arithmetic combining
    rules and place the results in the matrix 'ljmatrix' */
void ArithmeticCombiningRules::combine(const QVector<LJParameter> &ljs,
                                       LJPairMatrix &ljmatrix) const
{
    int nljs = ljs.count();

    ljmatrix.redimension(nljs,nljs);

    if (nljs > 0)
    {
        const LJParameter *ljarray = ljs.constData();

        for (int i=0; i<nljs-1; ++i)
        {
            const LJParameter &ljparam = ljarray[i];

            ljmatrix.setOuterIndex(i);

            ljmatrix[i] = ljparam;

            for (int j=i+1; j<nljs; ++j)
            {
                ljmatrix[j] = LJPair::arithmetic(ljparam, ljarray[j]);
            }
        }

        ljmatrix(nljs-1,nljs-1) = ljarray[nljs-1];
    }
}

/** Combine the charge and LJ parameters using
    arithmetic combining rules and place the results
    in the matrix 'cljmatrix' */
void ArithmeticCombiningRules::combine(const QVector<CLJParameter> &clj0,
                                       const QVector<CLJParameter> &clj1,
                                       CLJPairMatrix &cljmatrix) const
{
    int nclj0 = clj0.count();
    int nclj1 = clj1.count();

    cljmatrix.redimension(nclj0, nclj1);

    if (nclj0 > 0 and nclj1 > 0)
    {
        const CLJParameter *clj0array = clj0.constData();
        const CLJParameter *clj1array = clj1.constData();

        for (int i=0; i<nclj0; ++i)
        {
            const CLJParameter &clj0param = clj0array[i];

            cljmatrix.setOuterIndex(i);

            for (int j=0; j<nclj1; ++j)
            {
                cljmatrix[j] = CLJPair::arithmetic(clj0param, clj1array[j]);
            }
        }
    }
}

/** Combine the charge and LJ parameters using
    arithmetic combining rules and place the results
    in the matrix 'cljmatrix' */
void ArithmeticCombiningRules::combine(const QVector<CLJParameter> &cljs,
                                        CLJPairMatrix &cljmatrix) const
{
    int ncljs = cljs.count();

    cljmatrix.redimension(ncljs,ncljs);

    if (ncljs > 0)
    {
        const CLJParameter *cljarray = cljs.constData();

        for (int i=0; i<ncljs-1; ++i)
        {
            const CLJParameter &cljparam = cljarray[i];

            cljmatrix.setOuterIndex(i);

            cljmatrix[i] = cljparam;

            for (int j=i+1; j<ncljs; ++j)
            {
                cljmatrix[i] = CLJPair::arithmetic(cljparam, cljarray[j]);
            }
        }

        cljmatrix(ncljs-1,ncljs-1) = cljarray[ncljs-1];
    }
}

/** Combine the charge and LJ parameters using
    arithmetic combining rules and place the results
    in the matrix 'cljmatrix' */
void ArithmeticCombiningRules::combine(const QVector<ChargeParameter> &chg0,
                                       const QVector<LJParameter> &lj0,
                                       const QVector<ChargeParameter> &chg1,
                                       const QVector<LJParameter> &lj1,
                                       CLJPairMatrix &cljmatrix) const
{
    int nclj0 = chg0.count();
    int nclj1 = chg1.count();

    BOOST_ASSERT( nclj0 == lj0.count() );
    BOOST_ASSERT( nclj1 == lj1.count() );

    cljmatrix.redimension(nclj0,nclj1);

    if (nclj0 > 0 and nclj1 > 0)
    {
        const ChargeParameter *chg0array = chg0.constData();
        const LJParameter *lj0array = lj0.constData();

        const ChargeParameter *chg1array = chg1.constData();
        const LJParameter *lj1array = lj1.constData();

        for (int i=0; i<nclj0; ++i)
        {
            CLJParameter cljparam0( chg0array[i], lj0array[i] );

            cljmatrix.setOuterIndex(i);

            for (int j=0; j<nclj1; ++j)
            {
                cljmatrix[j] = CLJPair::arithmetic( cljparam0,
                                                    CLJParameter(chg1array[j],
                                                                 lj1array[j]) );
            }
        }
    }
}

/** Combine all pairs of the charge and LJ parameters, using arithmetic
    combining rules, and place the results in the matrix 'cljmatrix' */
void ArithmeticCombiningRules::combine(const QVector<ChargeParameter> &chgs,
                                       const QVector<LJParameter> &ljs,
                                       CLJPairMatrix &cljmatrix) const
{
    int nclj = chgs.count();

    BOOST_ASSERT( nclj == ljs.count() );

    cljmatrix.redimension(nclj,nclj);

    if (nclj > 0)
    {
        const ChargeParameter *chgarray  = chgs.constData();
        const LJParameter *ljarray = ljs.constData();

        for (int i=0; i<nclj-1; ++i)
        {
            CLJParameter cljparam( chgarray[i], ljarray[i] );

            cljmatrix(i,i) = cljparam;

            for (int j=i+1; j<nclj; ++j)
            {
                cljmatrix(i,j) = CLJPair::arithmetic( cljparam,
                                                      CLJParameter(chgarray[j],
                                                                   ljarray[j]) );
            }
        }

        cljmatrix(nclj-1,nclj-1) = CLJParameter(chgarray[nclj-1], ljarray[nclj-1]);
    }
}

///////////
/////////// Implementation of GeometricCombiningRules
///////////

static const RegisterMetaType<GeometricCombiningRules> r_geom;

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds,
                                      const GeometricCombiningRules &geom)
{
    writeHeader(ds, r_geom, 1)
                      << static_cast<const CombiningRuleBase&>(geom);
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds,
                                      GeometricCombiningRules &geom)
{
    VersionID v = readHeader(ds, r_geom);

    if (v == 1)
    {
        ds >> static_cast<CombiningRuleBase&>(geom);
    }
    else
        throw version_error(v, "1", r_geom, CODELOC);

    return ds;
}

/** Constructor */
GeometricCombiningRules::GeometricCombiningRules()
                         : CombiningRuleBase()
{}

/** Copy constructor */
GeometricCombiningRules::GeometricCombiningRules(
                                    const GeometricCombiningRules &other)
                         : CombiningRuleBase(other)
{}

/** Destructor */
GeometricCombiningRules::~GeometricCombiningRules()
{}

/** Combine the LJ paramters using geometric combining
    rules and place the results in the matrix 'ljmatrix' */
void GeometricCombiningRules::combine(const QVector<LJParameter> &lj0,
                                      const QVector<LJParameter> &lj1,
                                      LJPairMatrix &ljmatrix) const
{
    int nlj0 = lj0.count();
    int nlj1 = lj1.count();

    ljmatrix.redimension(nlj0, nlj1);

    if (nlj0 > 0 and nlj1 > 0)
    {
        const LJParameter *lj0array = lj0.constData();
        const LJParameter *lj1array = lj1.constData();

        for (int i=0; i<nlj0; ++i)
        {
            const LJParameter &lj0param = lj0array[i];

            ljmatrix.setOuterIndex(i);

            for (int j=0; j<nlj1; ++j)
            {
                ljmatrix[j] = LJPair::geometric(lj0param, lj1array[j]);
            }
        }
    }
}

/** Combine the LJ paramters using geometric combining
    rules and place the results in the matrix 'ljmatrix' */
void GeometricCombiningRules::combine(const QVector<LJParameter> &ljs,
                                      LJPairMatrix &ljmatrix) const
{
    int nljs = ljs.count();

    ljmatrix.redimension(nljs,nljs);

    if (nljs > 0)
    {
        const LJParameter *ljarray = ljs.constData();

        for (int i=0; i<nljs-1; ++i)
        {
            const LJParameter &ljparam = ljarray[i];

            ljmatrix.setOuterIndex(i);

            ljmatrix[i] = ljparam;

            for (int j=i+1; j<nljs; ++j)
            {
                ljmatrix[j] = LJPair::geometric(ljparam, ljarray[j]);
            }
        }

        ljmatrix(nljs-1,nljs-1) = ljarray[nljs-1];
    }
}

/** Combine the charge and LJ parameters using
    geometric combining rules and place the results
    in the matrix 'cljmatrix' */
void GeometricCombiningRules::combine(const QVector<CLJParameter> &clj0,
                                      const QVector<CLJParameter> &clj1,
                                      CLJPairMatrix &cljmatrix) const
{
    int nclj0 = clj0.count();
    int nclj1 = clj1.count();

    cljmatrix.redimension(nclj0, nclj1);

    if (nclj0 > 0 and nclj1 > 0)
    {
        const CLJParameter *clj0array = clj0.constData();
        const CLJParameter *clj1array = clj1.constData();

        for (int i=0; i<nclj0; ++i)
        {
            const CLJParameter &clj0param = clj0array[i];

            cljmatrix.setOuterIndex(i);

            for (int j=0; j<nclj1; ++j)
            {
                cljmatrix[j] = CLJPair::geometric(clj0param, clj1array[j]);
            }
        }
    }
}

/** Combine the charge and LJ parameters using
    geometric combining rules and place the results
    in the matrix 'cljmatrix' */
void GeometricCombiningRules::combine(const QVector<CLJParameter> &cljs,
                                      CLJPairMatrix &cljmatrix) const
{
    int ncljs = cljs.count();

    cljmatrix.redimension(ncljs,ncljs);

    if (ncljs > 0)
    {
        const CLJParameter *cljarray = cljs.constData();

        for (int i=0; i<ncljs-1; ++i)
        {
            const CLJParameter &cljparam = cljarray[i];

            cljmatrix.setOuterIndex(i);

            cljmatrix[i] = cljparam;

            for (int j=i+1; j<ncljs; ++j)
            {
                cljmatrix[i] = CLJPair::geometric(cljparam, cljarray[j]);
            }
        }

        cljmatrix(ncljs-1,ncljs-1) = cljarray[ncljs-1];
    }
}

/** Combine the charge and LJ parameters using
    geometric combining rules and place the results
    in the matrix 'cljmatrix' */
void GeometricCombiningRules::combine(const QVector<ChargeParameter> &chg0,
                                      const QVector<LJParameter> &lj0,
                                      const QVector<ChargeParameter> &chg1,
                                      const QVector<LJParameter> &lj1,
                                      CLJPairMatrix &cljmatrix) const
{
    int nclj0 = chg0.count();
    int nclj1 = chg1.count();

    BOOST_ASSERT( nclj0 == lj0.count() );
    BOOST_ASSERT( nclj1 == lj1.count() );

    cljmatrix.redimension(nclj0,nclj1);

    if (nclj0 > 0 and nclj1 > 0)
    {
        const ChargeParameter *chg0array = chg0.constData();
        const LJParameter *lj0array = lj0.constData();

        const ChargeParameter *chg1array = chg1.constData();
        const LJParameter *lj1array = lj1.constData();

        for (int i=0; i<nclj0; ++i)
        {
            const ChargeParameter &chgparam0 = chg0array[i];
            const LJParameter &ljparam0 = lj0array[i];

            cljmatrix.setOuterIndex(i);

            for (int j=0; j<nclj1; ++j)
            {
                const LJParameter &ljparam1 = lj1array[j];

                cljmatrix[j] = CLJPair( chgparam0.charge() * chg1array[j].charge(),
                                        ljparam0.sqrtSigma() * ljparam1.sqrtSigma(),
                                        ljparam0.sqrtEpsilon() * ljparam1.sqrtEpsilon() );
            }
        }
    }
}

/** Combine all pairs of the charge and LJ parameters, using geometric
    combining rules, and place the results in the matrix 'cljmatrix' */
void GeometricCombiningRules::combine(const QVector<ChargeParameter> &chgs,
                                      const QVector<LJParameter> &ljs,
                                      CLJPairMatrix &cljmatrix) const
{
    int nclj = chgs.count();

    BOOST_ASSERT( nclj == ljs.count() );

    cljmatrix.redimension(nclj,nclj);

    if (nclj > 0)
    {
        const ChargeParameter *chgarray  = chgs.constData();
        const LJParameter *ljarray = ljs.constData();

        for (int i=0; i<nclj-1; ++i)
        {
            CLJParameter cljparam( chgarray[i], ljarray[i] );

            cljmatrix(i,i) = cljparam;

            for (int j=i+1; j<nclj; ++j)
            {
                cljmatrix(i,j) = CLJPair::geometric( cljparam,
                                                     CLJParameter(chgarray[j],
                                                                  ljarray[j]) );
            }
        }

        cljmatrix(nclj-1,nclj-1) = CLJParameter(chgarray[nclj-1], ljarray[nclj-1]);
    }
}

///////////
/////////// Implementation of CombiningRules
///////////

static const RegisterMetaType<CombiningRules> r_combrules;

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const CombiningRules &combrules)
{
    writeHeader(ds, r_combrules, 1);

    SharedDataStream sds(ds);
    sds << combrules.d;

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, CombiningRules &combrules)
{
    VersionID v = readHeader(ds, r_combrules);

    if (v == 1)
    {
        SharedDataStream sds(ds);
        sds >> combrules.d;
    }
    else
        throw version_error(v, "1", r_combrules, CODELOC);

    return ds;
}

static const SharedPolyPointer<CombiningRuleBase> shared_null(
                                            new ArithmeticCombiningRules() );

/** Default constructor (arithmetic combining rules) */
CombiningRules::CombiningRules() : d(shared_null)
{}

/** Construct from the passed combining rules */
CombiningRules::CombiningRules(const CombiningRuleBase &rules)
               : d(rules)
{}

/** Construct from a Property */
CombiningRules::CombiningRules(const Property &property)
               : d(property.base())
{}

/** Copy constructor */
CombiningRules::CombiningRules(const CombiningRules &other)
               : d( other.d )
{}

/** Destructor */
CombiningRules::~CombiningRules()
{}

/** Assignment operator */
CombiningRules& CombiningRules::operator=(const CombiningRules &other)
{
    d = other.d;
    return *this;
}

/** Assignment operator */
CombiningRules& CombiningRules::operator=(const CombiningRuleBase &other)
{
    d = other;
    return *this;
}

/** Assign from a property */
CombiningRules& CombiningRules::operator=(const Property &property)
{
    d = property.base();
    return *this;
}

/** Return the type name of these combining rules. */
const char* CombiningRules::what() const
{
    return d->what();
}
