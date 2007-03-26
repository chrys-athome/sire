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

#include "moleculeproperty.h"
#include "molecule.h"
#include "moleculeinfo.h"
#include "moleculeid.h"
#include "moleculeversion.h"

#include "SireError/errors.h"

#include "SireStream/datastream.h"

using namespace SireMol;
using namespace SireBase;
using namespace SireStream;

static const RegisterMetaType<MoleculeProperty> r_molprop(MAGIC_ONLY,
                                                          "SireMol::MoleculeProperty");

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds,
                                       const MoleculeProperty &molprop)
{
    writeHeader(ds, r_molprop, 1) << static_cast<const PropertyBase&>(molprop);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds,
                                       MoleculeProperty &molprop)
{
    VersionID v = readHeader(ds, r_molprop);

    if (v == 1)
    {
        ds >> static_cast<PropertyBase&>(molprop);
    }
    else
        throw version_error(v, "1", r_molprop, CODELOC);

    return ds;
}

/** Constructor */
MoleculeProperty::MoleculeProperty() : PropertyBase()
{}

/** Copy constructor */
MoleculeProperty::MoleculeProperty(const MoleculeProperty &other)
                 : PropertyBase(other)
{}

/** Destructor */
MoleculeProperty::~MoleculeProperty()
{}

/** Assert that this property is compatible with the molecule 'molecule'

    \throw SireError::incompatible_error
*/
void MoleculeProperty::assertCompatibleWith(const Molecule &molecule) const
{
    if (not this->isCompatibleWith(molecule))
        throw SireError::incompatible_error( QObject::tr(
                "This property is incompatible with the molecule \"%1\" (%2:%3)")
                    .arg(molecule.info().name()).arg(molecule.ID())
                    .arg(molecule.version().toString()), CODELOC );
}
