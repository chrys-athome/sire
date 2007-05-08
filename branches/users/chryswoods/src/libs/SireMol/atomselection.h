/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
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

#ifndef SIREMOL_ATOMSELECTION_H
#define SIREMOL_ATOMSELECTION_H

#include <QSet>
#include <QHash>

#include "moleculeinfo.h"
#include "cutgroupid.h"
#include "atomid.h"
#include "idmolatom.h"

#include "selectionfrommol.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class AtomSelection;
}

QDataStream& operator<<(QDataStream&, const SireMol::AtomSelection&);
QDataStream& operator>>(QDataStream&, SireMol::AtomSelection&);

namespace SireMol
{

class Molecule;
class MoleculeView;

/** This class holds information about a selection of atoms in a Molecule.
    The selection is held in the most memory-efficient manner possible,
    and takes advantage of the CutGroup-based layout of Molecule objects.

    This is a const-class, which returns new AtomSelections that
    represent any change.

    @author Christopher Woods
*/
class SIREMOL_EXPORT AtomSelection
{

friend QDataStream& ::operator<<(QDataStream&, const AtomSelection&);
friend QDataStream& ::operator>>(QDataStream&, AtomSelection&);

friend class SelectionFromMol; //so can modify a single AtomSelection!

public:
    AtomSelection();

    AtomSelection(const MoleculeView &molecule);
    AtomSelection(const MoleculeInfo &molinfo);

    AtomSelection(const AtomSelection &other);

    ~AtomSelection();

    AtomSelection& operator=(const AtomSelection &other);

    bool operator==(const AtomSelection &other) const;
    bool operator!=(const AtomSelection &other) const;

    const MoleculeInfo& info() const;

    bool isEmpty() const;

    int nSelected() const;

    int nSelected(CutGroupID cgid) const;
    int nSelected(ResNum resnum) const;
    int nSelected(const AtomSelection &selection) const;
    int nSelected(const SelectionFromMol &selection) const;

    int nSelectedCutGroups() const;
    int nSelectedResidues() const;

    bool selectedAllCutGroups() const;
    bool selectedAllResidues() const;

    bool selected(const CGAtomID &cgatomid) const;
    bool selected(const IDMolAtom &atomid) const;

    bool selectedAll() const;

    bool selectedAll(CutGroupID cgid) const;
    bool selectedAll(ResNum resnum) const;

    bool selectedAll(const AtomSelection &selection) const;
    bool selectedAll(const SelectionFromMol &selection) const;

    bool selectedNone() const;

    bool selectedNone(CutGroupID cgid) const;
    bool selectedNone(ResNum resnum) const;

    bool selectedNone(const AtomSelection &selection) const;
    bool selectedNone(const SelectionFromMol &selection) const;

    AtomSelection selectAll() const;
    AtomSelection deselectAll() const;
    AtomSelection selectNone() const;

    AtomSelection selectAll(CutGroupID cgid) const;
    AtomSelection deselectAll(CutGroupID cgid) const;

    AtomSelection selectAll(ResNum resnum) const;
    AtomSelection deselectAll(ResNum resnum) const;

    AtomSelection selectAll(const AtomSelection &selection) const;
    AtomSelection deselectAll(const AtomSelection &selection) const;

    AtomSelection selectAll(const SelectionFromMol &selection) const;
    AtomSelection deselectAll(const SelectionFromMol &selection) const;

    AtomSelection select(CutGroupID cgid) const;
    AtomSelection deselect(CutGroupID cgid) const;

    AtomSelection setSelection(CutGroupID cgid) const;

    AtomSelection select(ResNum resnum) const;
    AtomSelection deselect(ResNum resnum) const;

    AtomSelection setSelection(ResNum resnum) const;

    AtomSelection select(const CGAtomID &cgatomid) const;
    AtomSelection deselect(const CGAtomID &cgatomid) const;

    AtomSelection setSelection(const CGAtomID &cgatomid) const;

    AtomSelection select(const IDMolAtom &atomid) const;
    AtomSelection deselect(const IDMolAtom &atomid) const;

    AtomSelection setSelection(const IDMolAtom &atomid) const;

    AtomSelection select(const AtomSelection &selection) const;
    AtomSelection deselect(const AtomSelection &selection) const;

    AtomSelection setSelection(const AtomSelection &selection) const;

    AtomSelection select(const SelectionFromMol &selection) const;
    AtomSelection deselect(const SelectionFromMol &selection) const;

    AtomSelection setSelection(const SelectionFromMol &selection) const;

    AtomSelection invert() const;

    bool intersects(const AtomSelection &selection) const;
    bool intersects(const SelectionFromMol &selection) const;

    bool contains(const AtomSelection &selection) const;
    bool contains(const SelectionFromMol &selection) const;

    AtomSelection intersect(const AtomSelection &selection) const;
    AtomSelection intersect(const SelectionFromMol &selection) const;

    AtomSelection unite(const AtomSelection &selection) const;
    AtomSelection unite(const SelectionFromMol &selection) const;

    AtomSelection subtract(const AtomSelection &selection) const;
    AtomSelection subtract(const SelectionFromMol &selection) const;

    AtomSelection applyMask(const QSet<CutGroupID> &cgids) const;
    AtomSelection applyMask(const QSet<ResNum> &resnums) const;
    AtomSelection applyMask(const AtomSelection &selection) const;
    AtomSelection applyMask(const SelectionFromMol &selection) const;

    void assertCompatibleWith(const MoleculeInfo &molinfo) const;
    void assertCompatibleWith(const Molecule &molecule) const;
    void assertCompatibleWith(const AtomSelection &other) const;
    void assertCompatibleWith(const SelectionFromMol &other) const;

    QSet<AtomIndex> selectedAtoms() const;

    QSet<CGAtomID> selectedAtoms(CutGroupID cgid) const;
    QSet<ResNumAtomID> selectedAtoms(ResNum resnum) const;

    QSet<CutGroupID> selectedCutGroups() const;
    QSet<ResNum> selectedResidues() const;

    bool isSingleAtom() const;
    bool isSingleCutGroup() const;
    bool isSingleResidue() const;

    CGAtomID asSingleAtom() const;
    CutGroupID asSingleCutGroup() const;
    ResNum asSingleResidue() const;

private:
    bool _pvt_selected(const CGAtomID &cgatomid) const;

    void _unsafe_select(const CGAtomID &cgatomid);
    void _unsafe_deselect(const CGAtomID &cgatomid);

    void _pvt_selectAll();
    void _pvt_deselectAll();

    void _pvt_select(CutGroupID cgid);
    void _pvt_deselect(CutGroupID cgid);

    void _pvt_select(ResNum resnum);
    void _pvt_deselect(ResNum resnum);

    void _pvt_select(const AtomSelection &selection);
    void _pvt_deselect(const AtomSelection &selection);

    void _pvt_select(const SelectionFromMol &selection);
    void _pvt_deselect(const SelectionFromMol &selection);

    void _pvt_select(const CGAtomID &cgatomid);
    void _pvt_deselect(const CGAtomID &cgatomid);

    void _pvt_select(const IDMolAtom &atomid);
    void _pvt_deselect(const IDMolAtom &atomid);

    void _pvt_invert();

    void _pvt_applyMask(const QSet<CutGroupID> &cgids);
    void _pvt_applyMask(const QSet<ResNum> &resnums);
    void _pvt_applyMask(const AtomSelection &selection);

    /** The AtomIDs of selected atoms, arranged by CutGroupID */
    QHash< CutGroupID, QSet<AtomID> > selected_atoms;

    /** The MoleculeInfo describing the molecule whose parts
        are being selected by this object */
    MoleculeInfo molinfo;

    /** The total number of selected atoms */
    quint32 nselected;
};

/** Return the info object that describes the molecule whose atoms
    this selection can select */
inline const MoleculeInfo& AtomSelection::info() const
{
    return molinfo;
}

}

Q_DECLARE_METATYPE(SireMol::AtomSelection);

SIRE_END_HEADER

#endif
