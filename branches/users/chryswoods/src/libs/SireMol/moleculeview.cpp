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

#include "moleculeview.h"
#include "moleculeview_inlines.h"

#include "molecule.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include "SireError/errors.h"

using namespace SireMol;
using namespace SireStream;

static const RegisterMetaType<MoleculeView> r_molview(MAGIC_ONLY,
                                                      "SireMol::MoleculeView");

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const MoleculeView &molview)
{
    writeHeader(ds, r_molview, 1);

    SharedDataStream sds(ds);

    sds << molview.d;

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, MoleculeView &molview)
{
    VersionID v = readHeader(ds, r_molview);

    if (v == 1)
    {
        SharedDataStream sds(ds);
        sds >> molview.d;
    }
    else
        throw version_error(v, "1", r_molview, CODELOC);

    return ds;
}

/** Null constructor */
MoleculeView::MoleculeView() : d( MoleculeData::null() )
{}

/** Copy constructor */
MoleculeView::MoleculeView(const MoleculeView &other)
             : d( other.d )
{}

/** Destructor */
MoleculeView::~MoleculeView()
{}

/** Return a complete view of this molecule */
Molecule MoleculeView::molecule() const
{
    return Molecule(*this);
}

/** Assert that this is the same molecule as 'other'

    \throw SireError::incompatible_error
*/
void MoleculeView::assertSameMolecule(const MoleculeView &other) const
{
    if (data().ID() != other.data().ID())
        throw SireError::incompatible_error( QObject::tr(
            "This molecule (\"%1\", ID == %2, Version == %3) is not "
            "the different to the other molecule (\"%4\", ID == %5, "
            "Version == %6)")
                .arg(data().info().name()).arg(data().ID())
                .arg(data().version().toString())
                .arg(other.data().info().name()).arg(other.data().ID())
                .arg(other.data().version().toString()), CODELOC );
}
/** Assert that this molecule has the same major version as 'other'
    - this also asserts that both molecules have the same ID number.

    \throw SireError::incompatible_error
*/
void MoleculeView::assertSameMajorVersion(const MoleculeView &other) const
{
    this->assertSameMolecule(other);

    if (data().version().major() != other.data().version().major())
        throw SireError::incompatible_error( QObject::tr(
            "This molecule (\"%1\", ID == %2, Version == %3) has a "
            "different major version to the other molecule (\"%4\", "
            "ID == %5, Version == %6)")
                .arg(data().info().name()).arg(data().ID())
                .arg(data().version().toString())
                .arg(other.data().info().name()).arg(other.data().ID())
                .arg(other.data().version().toString()), CODELOC );
}

/** Assert that this molecule has the same version as 'other'
    - this also asserts that both molecules have the same ID number.

    \throw SireError::incompatible_error
*/
void MoleculeView::assertSameVersion(const MoleculeView &other) const
{
    this->assertSameMolecule(other);

    if (data().version() != other.data().version())
        throw SireError::incompatible_error( QObject::tr(
            "This molecule (\"%1\", ID == %2, Version == %3) has a "
            "different version to the other molecule (\"%4\", "
            "ID == %5, Version == %6)")
                .arg(data().info().name()).arg(data().ID())
                .arg(data().version().toString())
                .arg(other.data().info().name()).arg(other.data().ID())
                .arg(other.data().version().toString()), CODELOC );
}
