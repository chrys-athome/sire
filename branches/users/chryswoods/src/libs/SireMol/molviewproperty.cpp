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

#include "molecularproperty.h"
#include "moleculeinfo.h"

#include "SireBase/version.h"

#include "SireError/errors.h"

#include "SireStream/datastream.h"

using namespace SireMol;
using namespace SireBase;
using namespace SireStream;

static const RegisterMetaType<MolecularProperty> r_molprop(MAGIC_ONLY,
                                                          "SireMol::MolecularProperty");

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds,
                                       const MolecularProperty &molprop)
{
    writeHeader(ds, r_molprop, 1) << static_cast<const PropertyBase&>(molprop);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds,
                                       MolecularProperty &molprop)
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
MolecularProperty::MolecularProperty() : PropertyBase()
{}

/** Copy constructor */
MolecularProperty::MolecularProperty(const MolecularProperty &other)
                 : PropertyBase(other)
{}

/** Destructor */
MolecularProperty::~MolecularProperty()
{}

/** Assert that this property is compatible with the MoleculeInfo 'info'

    \throw SireError::incompatible_error
*/
void MolecularProperty::assertCompatibleWith(const MoleculeInfo &info) const
{
    if (not this->isCompatibleWith(molecule.info()))
        throw SireError::incompatible_error( QObject::tr(
                "This property is incompatible with the molecule \"%1\" (%2)")
                    .arg(info.name()).arg(info.number()), CODELOC );
}
