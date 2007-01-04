/**
  * @file
  *
  * C++ Implementation: ResidueCutting
  *
  * Description: Implementation of ResidueCutting
  *
  *
  * @author Christopher Woods, (C) 2006
  *
  * Copyright: See COPYING file that comes with this distribution
  *
  */

#include "residuecutting.h"
#include "cutgroupnum.h"
#include "atomindex.h"
#include "editmoldata.h"

#include "SireStream/datastream.h"

using namespace SireMol;
using namespace SireStream;

static const RegisterMetaType<ResidueCutting> r_rescut;

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const ResidueCutting &rescut)
{
    writeHeader(ds, r_rescut, 0)
               << static_cast<const CuttingFunctionBase&>(rescut);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, ResidueCutting &rescut)
{
    VersionID v = readHeader(ds, r_rescut);

    if (v == 0)
    {
        ds >> static_cast<CuttingFunctionBase&>(rescut);
    }
    else
        throw version_error(v, "0", r_rescut, CODELOC);

    return ds;
}

/** Constructor */
ResidueCutting::ResidueCutting() : CuttingFunctionBase()
{}

/** Copy constructor */
ResidueCutting::ResidueCutting(const ResidueCutting &other)
               : CuttingFunctionBase(other)
{}

/** Destructor */
ResidueCutting::~ResidueCutting()
{}

/** Return the number of the CutGroup into which the atom 'atom' should
    be placed. For residue cutting, this is the same as the residue number. */
CutGroupNum ResidueCutting::operator()(const AtomIndex &atom, const EditMolData&) const
{
    return CutGroupNum( atom.resNum() );
}
