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

#include "atomicproperties.h"

#include "cgatomid.h"

#include "SireStream/datastream.h"
#include "SireError/errors.h"

using namespace SireMol;
using namespace SireStream;

static const RegisterMetaType<AtomicProperties> r_atomprops(MAGIC_ONLY,
                                                        "SireMol::AtomicProperties");

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds,
                                       const AtomicProperties &atomprops)
{
    writeHeader(ds, r_atomprops, 1) << static_cast<const MoleculeProperty&>(atomprops);
    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds,
                                       AtomicProperties &atomprops)
{
    VersionID v = readHeader(ds, r_atomprops);

    if (v == 1)
    {
        ds >> static_cast<MoleculeProperty&>(atomprops);
    }
    else
        throw version_error(v, "1", r_atomprops, CODELOC);

    return ds;
}

/** Constructor */
AtomicProperties::AtomicProperties() : MoleculeProperty()
{}

/** Copy constructor */
AtomicProperties::AtomicProperties(const AtomicProperties &other)
                 : MoleculeProperty(other)
{}

/** Destructor */
AtomicProperties::~AtomicProperties()
{}

/** Throw an error caused by a missing CutGroup

    \throw SireError::invalid_index
*/
void AtomicProperties::throwMissingCutGroup(CutGroupID cgid, int ncg) const
{
    throw SireError::invalid_index( QObject::tr(
          "There is no CutGroup with ID == %1 in the AtomicCharges "
          "(Max ID == %2)")
              .arg(cgid).arg(ncg-1), CODELOC );
}

/** Throw an error caused by a missing atom

    \throw SireError::invalid_index
*/
void AtomicProperties::throwMissingAtom(const CGAtomID &cgatomid, int natms) const
{
    throw SireError::invalid_index( QObject::tr(
          "There is no Atom with ID == %1 in the CutGroup with "
          "ID == %2 in the AtomicCharges (Max ID == %2)")
              .arg(cgatomid.atomID()).arg(cgatomid.cutGroupID()).arg(natms-1),
                      CODELOC );
}
