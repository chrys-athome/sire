/**
  * @file
  *
  * C++ Implementation: AtomCutting
  *
  * Description: Implementation of AtomCutting
  *
  *
  * @author Christopher Woods, (C) 2006
  *
  * Copyright: See COPYING file that comes with this distribution
  *
  */

#include "atomcutting.h"
#include "cutgroupnum.h"
#include "atomindex.h"
#include "editmoldata.h"

#include "SireStream/datastream.h"

using namespace SireMol;
using namespace SireStream;

static const RegisterMetaType<AtomCutting> r_atmcut;

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const AtomCutting &atmcut)
{
    writeHeader(ds, r_atmcut, 0)
               << static_cast<const CuttingFunctionBase&>(atmcut);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, AtomCutting &atmcut)
{
    VersionID v = readHeader(ds, r_atmcut);

    if (v == 0)
    {
        ds >> static_cast<CuttingFunctionBase&>(atmcut);
    }
    else
        throw version_error(v, "0", r_atmcut, CODELOC);

    return ds;
}

/** Constructor */
AtomCutting::AtomCutting() : CuttingFunctionBase()
{}

/** Copy constructor */
AtomCutting::AtomCutting(const AtomCutting &other)
               : CuttingFunctionBase(other)
{}

/** Destructor */
AtomCutting::~AtomCutting()
{}

/** Return the number of the CutGroup into which the atom 'atom' should
    be placed. This will try to use 1 + nCutGroups(), but will move up
    from there if that CutGroup is already occupied */
CutGroupNum AtomCutting::operator()(const AtomIndex &atom,
                                    const EditMolData &moldata) const
{
    CutGroupNum cgnum( moldata.nCutGroups() + 1 );

    while (moldata.contains(cgnum))
    {
        if (moldata.isEmpty(cgnum))
            return cgnum;
        else
            ++cgnum;
    }

    return cgnum;
}
