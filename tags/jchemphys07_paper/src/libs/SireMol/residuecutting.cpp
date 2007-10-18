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
