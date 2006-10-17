/**
  * @file
  *
  * C++ Implementation: CuttingFunction, CuttingFunctionBase
  *
  * Description: Implementation of CuttingFunction and CuttingFunctionBase
  *
  *
  * @author Christopher Woods, (C) 2006
  *
  * Copyright: See COPYING file that comes with this distribution
  *
  */

#include "cuttingfunction.h"
#include "residuecutting.h"

#include "atomindex.h"
#include "editmoldata.h"
#include "cutgroupnum.h"

#include "SireStream/datastream.h"

using namespace SireMol;
using namespace SireBase;
using namespace SireStream;

static const RegisterMetaType<CuttingFunctionBase> r_funcbase(
                                            "SireMol::CuttingFunctionBase", MAGIC_ONLY);

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const CuttingFunctionBase&)
{
    writeHeader(ds, r_funcbase, 0);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, CuttingFunctionBase&)
{
    VersionID v = readHeader(ds, r_funcbase);

    if (v != 0)
        throw version_error(v, "0", r_funcbase, CODELOC);

    return ds;
}

static const RegisterMetaType<CuttingFunction> r_func("SireMol::CuttingFunction");

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const CuttingFunction &cutfunc)
{
    writeHeader(ds, r_func, 1) << cutfunc.func;

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, CuttingFunction &cutfunc)
{
    VersionID v = readHeader(ds, r_func);

    if (v == 1)
    {
        ds >> cutfunc.func;
    }
    else
        throw version_error(v, "1", r_func, CODELOC);

    return ds;
}

/** Constructor */
CuttingFunctionBase::CuttingFunctionBase()
{}

/** Copy constructor */
CuttingFunctionBase::CuttingFunctionBase(const CuttingFunctionBase&)
{}

/** Destructor */
CuttingFunctionBase::~CuttingFunctionBase()
{}

static const DynamicSharedPtr<CuttingFunctionBase> shared_null( new ResidueCutting() );

/** Constructor - by default this provides ResidueCutting */
CuttingFunction::CuttingFunction() : func(shared_null)
{}

/** Construct from the passed function */
CuttingFunction::CuttingFunction(const CuttingFunctionBase &function)
                : func( function )
{}

/** Copy constructor */
CuttingFunction::CuttingFunction(const CuttingFunction &other)
                : func(other.func)
{}

/** Destructor */
CuttingFunction::~CuttingFunction()
{}

/** Return the number of the CutGroup into which the atom 'atom'
    should be placed in the molecule represented by 'moldata' */
CutGroupNum CuttingFunction::operator()(const AtomIndex &atom,
                                        const EditMolData &moldata) const
{
    return func->operator()(atom, moldata);
}

/** Return the name of the class that is used to calculate the
    CutGroup number */
const char* CuttingFunction::what() const
{
    return func->what();
}
