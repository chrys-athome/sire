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

#include "partialmolecule.h"
#include "moleculeview_inlines.h"

#include "SireStream/datastream.h"

using namespace SireMol;
using namespace SireStream;

static const RegisterMetaType<PartialMolecule> r_partialmol;

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds,
                                       const PartialMolecule &partialmol)
{
    writeHeader(ds, r_partialmol, 1)
        << static_cast<const MoleculeView&>(partialmol);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds,
                                       PartialMolecule &partialmol)
{
    VersionID v = readHeader(ds, r_partialmol);

    if (v == 1)
    {
        ds >> static_cast<MoleculeView&>(partialmol);
    }
    else
        throw version_error(v, "1", r_partialmol, CODELOC);

    return ds;
}

/** Empty constructor */
PartialMolecule::PartialMolecule() : MoleculeView()
{}

/** Construct a PartialMolecule that represents 'molecule' */
PartialMolecule::PartialMolecule(const MolDataView &molecule)
                : MoleculeView(molecule)
{}

/** Construct a PartialMolecule that represents 'molecule', with
    the atoms in 'selection' additionally selected */
PartialMolecule::PartialMolecule(const MolDataView &molecule,
                                 const SelectionFromMol &selection)
                : MoleculeView(molecule)
{
    _pvt_selection() = selectedAtoms().intersect(selection);
}

/** Copy constructor */
PartialMolecule::PartialMolecule(const PartialMolecule &other)
                : MoleculeView(other)
{}

/** Destructor */
PartialMolecule::~PartialMolecule()
{}

/** Copy assignment - make equal to a Molecule */
PartialMolecule& PartialMolecule::operator=(const MolDataView &molecule)
{
    MoleculeView::operator=(molecule);
    return *this;
}

/** Comparison operator */
bool PartialMolecule::operator==(const PartialMolecule &other) const
{
    return _pvt_isEqual(other);
}

/** Comparison operator */
bool PartialMolecule::operator!=(const PartialMolecule &other) const
{
    return not _pvt_isEqual(other);
}

/** Return the info object describing this molecule */
const MoleculeInfo& PartialMolecule::info() const
{
    return data().info();
}

/** Return the name of the molecule */
QString PartialMolecule::name() const
{
    return info().name();
}

/** Return the ID number of this molecule - the ID number is
    used to uniquely identify a molecule */
MoleculeID PartialMolecule::ID() const
{
    return data().ID();
}

/** Return the version number of the molecule - this changes
    whenever the molecule is changed. Two molecules with the
    same ID and version are identical. */
const Version& PartialMolecule::version() const
{
    return data().version();
}

/** Change this molecule so that it is at the same state as 'other'.

    \throw SireError::incompatible_error
*/
PartialMolecule PartialMolecule::change(const MoleculeView &other) const
{
    PartialMolecule retval(*this);

    retval._pvt_change(other);

    return retval;
}
