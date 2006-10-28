/** 
  * @file
  *
  * C++ Implementation: CombiningRules, CombiningRuleBase,
  *                     ArithmeticCombiningRules and GeometricCombiningRules
  *
  * Description: 
  * Implementation of CombiningRules, CombiningRuleBase, 
  * ArithmeticCombiningRules and GeometricCombiningRules
  *
  * @author Christopher Woods, (C) 2006
  *
  * Copyright: See COPYING file that comes with this distribution
  *
  */
  
#include "combiningrules.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMM;
using namespace SireStream;

///////////
/////////// Implementation of CombiningRuleBase
///////////

static const RegisterMetaType<CombiningRuleBase> 
                          r_combinebase("SireMM::CombiningRuleBase", MAGIC_ONLY);

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const CombiningRuleBase&)
{
    writeHeader(ds, r_combinebase, 0);
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, CombiningRuleBase&)
{
    VersionID v = readHeader(ds, r_combinebase);
    
    if (v != 0)
        throw version_error(v, "1", r_combinebase, CODELOC);
    
    return ds;
}

/** Constructor */
CombiningRuleBase::CombiningRuleBase()
{}
    
/** Copy constructor */
CombiningRuleBase::CombiningRuleBase(const CombiningRuleBase&)
{}

/** Destructor */
CombiningRuleBase::~CombiningRuleBase()
{}

///////////
/////////// Implementation of ArithmeticCombiningRules
///////////

static const RegisterMetaType<ArithmeticCombiningRules> 
                                    r_arith("SireMM::ArithmeticCombiningRules");
                                    
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

/** Combine the CLJ parameters in 'clj0' and 'clj1' using
    arithmetic combining rules and place the results
    in the matrix 'cljmatrix' */
void ArithmeticCombiningRules::combine(const QVector<CLJParameter> &clj0,
                                       const QVector<CLJParameter> &clj1,
                                       CLJPairMatrix &cljmatrix) const
{
    int nclj0 = clj0.count();
    int nclj1 = clj1.count();
    
    cljmatrix.redimension(nclj0,nclj1);
    
    if (nclj0 > 0 and nclj1 > 0)
    {
        const CLJParameter *clj0array = clj0.constData();
        const CLJParameter *clj1array = clj1.constData();
        
        for (int i=0; i<nclj0; ++i)
        {
            const CLJParameter &cljparam0 = clj0array[i];
            cljmatrix.setOuterIndex(i);
            
            for (int j=0; j<nclj1; ++j)
            {
                cljmatrix[j] = CLJPair::arithmetic(cljparam0,clj1array[j]);
            }
        }
    }
}

/** Combine all pairs of the CLJ parameters in 'clj', using arithmetic
    combining rules, and place the results in the matrix 'cljmatrix' */
void ArithmeticCombiningRules::combine(const QVector<CLJParameter> &clj,
                                       CLJPairMatrix &cljmatrix) const
{
    int nclj = clj.count();
    
    cljmatrix.redimension(nclj,nclj);
    
    if (nclj > 0)
    {
        const CLJParameter *cljarray  = clj.constData();
    
        for (int i=0; i<nclj-1; ++i)
        {
            const CLJParameter &cljparam = cljarray[i];
        
            cljmatrix(i,i) = cljparam;
        
            for (int j=i+1; j<nclj; ++j)
            {
                cljmatrix(i,j) = CLJPair::arithmetic(cljparam,cljarray[j]);
            }
        }
        
        cljmatrix(nclj-1,nclj-1) = cljarray[nclj-1];
    }
}

///////////
/////////// Implementation of GeometricCombiningRules
///////////

static const RegisterMetaType<GeometricCombiningRules> 
                                    r_geom("SireMM::GeometricCombiningRules");
                                    
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

/** Combine the CLJ parameters in 'clj0' and 'clj1' using
    geometric combining rules and place the results
    in the matrix 'cljmatrix' */
void GeometricCombiningRules::combine(const QVector<CLJParameter> &clj0,
                                       const QVector<CLJParameter> &clj1,
                                       CLJPairMatrix &cljmatrix) const
{
    int nclj0 = clj0.count();
    int nclj1 = clj1.count();
    
    cljmatrix.redimension(nclj0,nclj1);
    
    if (nclj0 > 0 and nclj1 > 0)
    {
        const CLJParameter *clj0array = clj0.constData();
        const CLJParameter *clj1array = clj1.constData();
        
        for (int i=0; i<nclj0; ++i)
        {
            const CLJParameter &cljparam0 = clj0array[i];
            cljmatrix.setOuterIndex(i);
            
            for (int j=0; j<nclj1; ++j)
            {
                cljmatrix[j] = CLJPair::geometric(cljparam0,clj1array[j]);
            }
        }
    }
}

/** Combine all pairs of the CLJ parameters in 'clj', using geometric
    combining rules, and place the results in the matrix 'cljmatrix' */
void GeometricCombiningRules::combine(const QVector<CLJParameter> &clj,
                                       CLJPairMatrix &cljmatrix) const
{
    int nclj = clj.count();
    
    cljmatrix.redimension(nclj,nclj);
    
    if (nclj > 0)
    {
        const CLJParameter *cljarray  = clj.constData();
    
        for (int i=0; i<nclj-1; ++i)
        {
            const CLJParameter &cljparam = cljarray[i];
        
            cljmatrix(i,i) = cljparam;
        
            for (int j=i+1; j<nclj; ++j)
            {
                cljmatrix(i,j) = CLJPair::geometric(cljparam,cljarray[j]);
            }
        }
        
        cljmatrix(nclj-1,nclj-1) = cljarray[nclj-1];
    }
}

///////////
/////////// Implementation of CombiningRules
///////////

static const RegisterMetaType<CombiningRules> r_combrules("SireMM::CombiningRules");

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const CombiningRules &combrules)
{
    writeHeader(ds, r_combrules, 1);
     
    SharedDataStream(ds) << combrules.d;
    
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, CombiningRules &combrules)
{
    VersionID v = readHeader(ds, r_combrules);
    
    if (v == 1)
    {
        SharedDataStream(ds) >> combrules.d;
    }
    else
        throw version_error(v, "1", r_combrules, CODELOC);
    
    return ds;
}

static const DynamicSharedPtr<CombiningRuleBase> shared_null( 
                                            new ArithmeticCombiningRules() );

/** Default constructor (arithmetic combining rules) */
CombiningRules::CombiningRules() : d(shared_null)
{}
    
/** Construct from the passed combining rules */
CombiningRules::CombiningRules(const CombiningRuleBase &rules)
               : d( rules.clone() )
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

/** Return the type name of these combining rules. */
const char* CombiningRules::what() const
{
    return d->what();
}
