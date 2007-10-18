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

#include "moleculecutting.h"
#include "cutgroupnum.h"
#include "atomindex.h"
#include "editmoldata.h"

#include "SireStream/datastream.h"

using namespace SireMol;
using namespace SireStream;

static const RegisterMetaType<MoleculeCutting> r_molcut;

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
