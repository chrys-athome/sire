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

#include "cuttingfunction.h"
#include "residuecutting.h"

#include "atomindex.h"
#include "editmoldata.h"
#include "cutgroupnum.h"

#include "SireStream/datastream.h"

using namespace SireMol;
using namespace SireBase;
using namespace SireStream;

static const RegisterMetaType<CuttingFunctionBase> r_funcbase(MAGIC_ONLY,
                                                     "SireMol::CuttingFunctionBase");

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

static const RegisterMetaType<CuttingFunction> r_func;

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
CuttingFunctionBase::CuttingFunctionBase() : QSharedData()
{}

/** Copy constructor */
CuttingFunctionBase::CuttingFunctionBase(const CuttingFunctionBase&) : QSharedData()
{}

/** Destructor */
CuttingFunctionBase::~CuttingFunctionBase()
{}

static const SharedPolyPointer<CuttingFunctionBase> shared_null( new ResidueCutting() );

/** Constructor - by default this provides ResidueCutting */
CuttingFunction::CuttingFunction() : func(shared_null)
{}

/** Construct from the passed function */
CuttingFunction::CuttingFunction(const CuttingFunctionBase &function)
                : func( function.clone() )
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
