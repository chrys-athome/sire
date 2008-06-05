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
