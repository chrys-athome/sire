/**
  * @file
  *
  * C++ Implementation: MoleculeCutting
  *
  * Description: Implementation of MoleculeCutting
  *
  *
  * @author Christopher Woods, (C) 2006
  *
  * Copyright: See COPYING file that comes with this distribution
  *
  */

#include "moleculecutting.h"
#include "cutgroupnum.h"
#include "atomindex.h"
#include "editmoldata.h"

#include "SireStream/datastream.h"

using namespace SireMol;
using namespace SireStream;

static const RegisterMetaType<MoleculeCutting> r_molcut("SireMol::MoleculeCutting");

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const MoleculeCutting &molcut)
{
    writeHeader(ds, r_molcut, 0)
               << static_cast<const CuttingFunctionBase&>(molcut);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, MoleculeCutting &molcut)
{
    VersionID v = readHeader(ds, r_molcut);

    if (v == 0)
    {
        ds >> static_cast<CuttingFunctionBase&>(molcut);
    }
    else
        throw version_error(v, "0", r_molcut, CODELOC);

    return ds;
}

/** Constructor */
MoleculeCutting::MoleculeCutting() : CuttingFunctionBase()
{}

/** Copy constructor */
MoleculeCutting::MoleculeCutting(const MoleculeCutting &other)
                : CuttingFunctionBase(other)
{}

/** Destructor */
MoleculeCutting::~MoleculeCutting()
{}

/** All atoms are placed in the CutGroup with number '1' */
CutGroupNum MoleculeCutting::operator()(const AtomIndex&, const EditMolData&) const
{
    return CutGroupNum(1);
}
