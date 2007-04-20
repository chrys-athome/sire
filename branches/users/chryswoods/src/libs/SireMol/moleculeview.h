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

#ifndef SIREMOL_MOLECULEVIEW_H
#define SIREMOL_MOLECULEVIEW_H

#include <QSharedDataPointer>
#include <QSharedData>

#include "atomselection.h"
#include "moleculeversion.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class MoleculeView;
class MolDataView;
}

QDataStream& operator<<(QDataStream&, const SireMol::MolDataView&);
QDataStream& operator>>(QDataStream&, SireMol::MolDataView&);

QDataStream& operator<<(QDataStream&, const SireMol::MoleculeView&);
QDataStream& operator>>(QDataStream&, SireMol::MoleculeView&);

namespace SireMol
{

class AtomSelector;
class MoleculeMover;
class PropertyExtractor;

class MoleculeData;
class Molecule;

namespace detail
{

/** This private class holds the MoleculeData and AtomSelection objects
    used by MolDataView. An implicitly shared pointer is used so that
    MolDataView (and derived classes like Molecule and PartialMolecule)
    have the same size as a pointer, thereby allowing them to be stored
    efficiently in Qt template classes.

    @author Christopher Woods
*/
class MolDataViewPvt : public QSharedData
{
public:
    MolDataViewPvt();

    MolDataViewPvt(const MoleculeData &moldata);
    MolDataViewPvt(const MoleculeData &moldata,
               const AtomSelection &selected_atoms);

    MolDataViewPvt(const MolDataViewPvt &other);

    ~MolDataViewPvt();

    MolDataViewPvt& operator=(const MolDataViewPvt &other);

    bool operator==(const MolDataViewPvt &other) const;
    bool operator!=(const MolDataViewPvt &other) const;

    /** Pointer to the molecule's data */
    QSharedDataPointer<MoleculeData> d;

    /** All of the atoms that are selected
        from the molecule */
    AtomSelection selected_atoms;

    static QSharedDataPointer<MolDataViewPvt> shared_null;
};

} // end of namespace detail

/** This is the base class of all views of the MoleculeData. Derived
    classes include;

    MoleculeView  - base class of all Molecule views, e.g. Molecule,
                    PartialMolecule, Residue and NewAtom

    PropertyExtractor - used to extract properties from the MoleculeData

    MoleculeMover - used to move the MoleculeData

    @author Christopher Woods
*/
class SIREMOL_EXPORT MolDataView
{

friend QDataStream& ::operator<<(QDataStream&, const MolDataView&);
friend QDataStream& ::operator>>(QDataStream&, MolDataView&);

protected:
    MolDataView();

    MolDataView(const MoleculeData &moldata);

    MolDataView(const MoleculeData &moldata,
                const AtomSelection &selected_atoms);

    MolDataView(const MolDataView &other);

    ~MolDataView();

    MolDataView& operator=(const MolDataView &other)
    {
        if (this != &other)
            d = other.d;

        return *this;
    }

    bool operator==(const MolDataView &other) const;
    bool operator!=(const MolDataView &other) const;

    const MoleculeData& data() const;
    const MoleculeData& constData() const;

    MoleculeData& data();

    const AtomSelection& _pvt_selection() const
    {
        return d->selected_atoms;
    }

    const AtomSelection& _pvt_constSelection() const
    {
        return d->selected_atoms;
    }

    AtomSelection& _pvt_selection()
    {
        return d->selected_atoms;
    }

    const AtomSelection& selectedAtoms() const
    {
        return d->selected_atoms;
    }

private:
    /** Shared pointer to the underlying MolDataViewPvt */
    QSharedDataPointer<detail::MolDataViewPvt> d;
};

/** This is the base class of all views of a Molecule. All Molecule classes
    are actually a view of MoleculeData - Molecule can see all of MoleculeData,
    Residue can see only the parts of MoleculeData that correspond to that
    residue etc.

    @author Christopher Woods
*/
class SIREMOL_EXPORT MoleculeView : public MolDataView
{

friend QDataStream& ::operator<<(QDataStream&, const MoleculeView&);
friend QDataStream& ::operator>>(QDataStream&, MoleculeView&);

public:
    /** Null constructor */
    MoleculeView()
    {}

    /** Copy constructor */
    MoleculeView(const MoleculeView &other) : MolDataView(other)
    {}

    ~MoleculeView();

    Molecule molecule() const;

    /** Return the atoms that are part of this view of the molecule */
    const AtomSelection& selectedAtoms() const
    {
        return _pvt_selection();
    }

    MoleculeMover move() const;
    MoleculeMover move(const SelectionFromMol &selection) const;

    PropertyExtractor extract() const;
    PropertyExtractor extract(const SelectionFromMol &selected_atoms) const;

    AtomSelector selection() const;
    AtomSelector selection(const SelectionFromMol &selected_atoms) const;

    void assertSameMolecule(const MoleculeView &other) const;
    void assertSameMajorVersion(const MoleculeView &other) const;
    void assertSameVersion(const MoleculeView &other) const;

protected:
    MoleculeView(const MoleculeData &moldata);
    MoleculeView(const MoleculeData &moldata,
                 const AtomSelection &selected_atoms);

    /** Copy assignment operator */
    MoleculeView& operator=(const MoleculeView &other)
    {
        MolDataView::operator=(other);
        return *this;
    }
};

}

Q_DECLARE_METATYPE(SireMol::MoleculeView);

SIRE_END_HEADER

#endif
