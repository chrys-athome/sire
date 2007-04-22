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
#include "partialmolecule.h"
#include "atomselector.h"
#include "moleculemover.h"
#include "propertyextractor.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include "SireError/errors.h"

using namespace SireMol;
using namespace SireMol::detail;
using namespace SireStream;

///////////////
/////////////// Implementation of MolDataViewPvt
///////////////

/** Null constructor */
MolDataViewPvt::MolDataViewPvt() : QSharedData()
{}

/** Construct from a passed MoleculeData */
MolDataViewPvt::MolDataViewPvt(const MoleculeData &moldata)
           : QSharedData(),
             d( new MoleculeData(moldata) ),
             selected_atoms(moldata.info())
{}

/** Construct from a passed MoleculeData and selection

    \throw SireError::incompatible_error
*/
MolDataViewPvt::MolDataViewPvt(const MoleculeData &moldata,
                       const AtomSelection &selection)
           : QSharedData(),
             d( new MoleculeData(moldata) ),
             selected_atoms(selection)
{
    selected_atoms.assertCompatibleWith(moldata.info());
}

/** Copy constructor */
MolDataViewPvt::MolDataViewPvt(const MolDataViewPvt &other)
           : QSharedData(),
             d(other.d),
             selected_atoms(other.selected_atoms)
{}

/** Destructor */
MolDataViewPvt::~MolDataViewPvt()
{}

/** Static null MolDataViewPvt */
QSharedDataPointer<MolDataViewPvt> MolDataViewPvt::shared_null( new MolDataViewPvt() );

/** Copy assignment operator */
MolDataViewPvt& MolDataViewPvt::operator=(const MolDataViewPvt &other)
{
    if (this != &other)
    {
        d = other.d;
        selected_atoms = other.selected_atoms;
    }

    return *this;
}

///////////////
/////////////// Implementation of MolDataView
///////////////

static const RegisterMetaType<MolDataView> r_moldataview(MAGIC_ONLY,
                                                         "SireMol::MolDataView");

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds,
                                       const MolDataView &moldataview)
{
    writeHeader(ds, r_moldataview, 1);

    SharedDataStream sds(ds);

    sds << moldataview.d->d
        << moldataview.d->selected_atoms;

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds,
                                       MolDataView &moldataview)
{
    VersionID v = readHeader(ds, r_moldataview);

    if (v == 1)
    {
        SharedDataStream sds(ds);
        sds >> moldataview.d->d
            >> moldataview.d->selected_atoms;
    }
    else
        throw version_error(v, "1", r_moldataview, CODELOC);

    return ds;
}

/** Null constructor */
MolDataView::MolDataView()
            : d(MolDataViewPvt::shared_null)
{}

/** Construct from the passed MoleculeData */
MolDataView::MolDataView(const MoleculeData &moldata)
            : d( new MolDataViewPvt(moldata) )
{}

/** Construct from the passed MoleculeData and atom selection

    \throw SireError::incompatible_error
*/
MolDataView::MolDataView(const MoleculeData &moldata,
                         const AtomSelection &selected_atoms)
            : d( new MolDataViewPvt(moldata,selected_atoms) )
{}

/** Copy constructor */
MolDataView::MolDataView(const MolDataView &other)
            : d(other.d)
{}

/** Destructor */
MolDataView::~MolDataView()
{}

/** Comparison function */
bool MolDataView::_pvt_isEqual(const MolDataView &other) const
{
    return *this == other;
}

/** Change this molecule so that it is at the same state as 'other'.
    This changes the actual molecule data to match 'other', but preserves
    the current selection 
    
    \throw SireError::incompatible_error
*/
void MolDataView::_pvt_change(const MolDataView &other)
{
    if (other.data().ID() != this->data().ID())
    {
        throw SireError::incompatible_error( QObject::tr(
            "You can only change a molecule to another version of itself. "
            "This molecule has ID == %1, but you are trying to change it "
            "into a molecule with ID == %2!")
                .arg(this->data().ID())
                .arg(other.data().ID()),
                    CODELOC );
    }
    else if (other.data().version() != this->data().version())
    {
        //there has been a change
        d->d = other.d->d;
    }
}

///////////////
/////////////// Implementation of MoleculeView
///////////////

static const RegisterMetaType<MoleculeView> r_molview(MAGIC_ONLY,
                                                      "SireMol::MoleculeView");

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds,
                                       const MoleculeView &molview)
{
    writeHeader(ds, r_molview, 1)
        << static_cast<const MolDataView&>(molview);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, MoleculeView &molview)
{
    VersionID v = readHeader(ds, r_molview);

    if (v == 1)
    {
        ds >> static_cast<MolDataView&>(molview);
    }
    else
        throw version_error(v, "1", r_molview, CODELOC);

    return ds;
}

/** Destructor */
MoleculeView::~MoleculeView()
{}

/** Return a complete view of this molecule */
Molecule MoleculeView::molecule() const
{
    return Molecule(*this);
}

/** Return a mover object used to move the selected atoms of this molecule */
MoleculeMover MoleculeView::move() const
{
    return MoleculeMover(*this);
}

/** Return a mover object used to move the selected atoms 'selected_atoms'
    of this molecule (only the intersection of 'selected_atoms' and the atoms
    already selected as part of this view will be moved) */
MoleculeMover MoleculeView::move(const SelectionFromMol &selected_atoms) const
{
    return MoleculeMover(*this, selected_atoms);
}

/** Return an extractor object that can be used to extract properties about
    the selected atoms of this molecule */
PropertyExtractor MoleculeView::extract() const
{
    return PropertyExtractor(*this);
}

/** Return an extractor object that can be used to extract properties about
    the selected atoms 'selected_atoms' of this molecule (the selection will
    be the intersection of the atoms already selected as part of this
    molecule and 'selected_atoms' */
PropertyExtractor MoleculeView::extract(const SelectionFromMol &selected_atoms) const
{
    return PropertyExtractor(*this, selected_atoms);
}

/** Return a selector object that can be used to change the selected atoms
    in the molecule. The initial selection comes from the selection
    that forms this view */
AtomSelector MoleculeView::selection() const
{
    return AtomSelector(*this);
}

/** Return a selector object that can be used to change the selected atoms
    in the molecule. The initial selection come from the intersection of
    'selected_atoms' with the selection that forms this view.

    \throw SireError::incompatible_error
*/
AtomSelector MoleculeView::selection(const SelectionFromMol &selected_atoms) const
{
    return AtomSelector(*this, selected_atoms);
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
