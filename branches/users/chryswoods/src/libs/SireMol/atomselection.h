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
class Residue;
class NewAtom;

/** This class holds information about a selection of atoms in a Molecule.
    The selection is held in the most memory-efficient manner possible,
    and takes advantage of the CutGroup-based layout of Molecule objects.

    @author Christopher Woods
*/
class SIREMOL_EXPORT AtomSelection
{

friend QDataStream& ::operator<<(QDataStream&, const AtomSelection&);
friend QDataStream& ::operator>>(QDataStream&, AtomSelection&);

public:
    AtomSelection();

    AtomSelection(const Molecule &molecule);
    AtomSelection(const Residue &residue);
    AtomSelection(const NewAtom &atom);

    AtomSelection(const AtomSelection &other);

    ~AtomSelection();

    AtomSelection& operator=(const AtomSelection &other);

    bool operator==(const AtomSelection &other) const;
    bool operator!=(const AtomSelection &other) const;

    bool isEmpty() const;

    int nSelected() const;

    int nSelected(CutGroupID cgid) const;
    int nSelected(ResNum resnum) const;

    int nSelectedCutGroups() const;
    int nSelectedResidues() const;

    bool selected(const CGAtomID &cgatomid) const;
    bool selected(const IDMolAtom &atomid) const;

    bool selectedAll() const;

    bool selectedAll(CutGroupID cgid) const;
    bool selectedAll(ResNum resnum) const;

    bool selectedNone() const;

    bool selectedNone(CutGroupID cgid) const;
    bool selectedNone(ResNum resnum) const;

    void selectAll();
    void deselectAll();

    void selectAll(CutGroupID cgid);
    void deselectAll(CutGroupID cgid);

    void selectAll(ResNum resnum);
    void deselectAll(ResNum resnum);

    void selectAll(const AtomSelection &selection);
    void deselectAll(const AtomSelection &selection);

    void select(const CGAtomID &cgatomid);
    void deselect(const CGAtomID &cgatomid);

    void select(const IDMolAtom &atomid);
    void deselect(const IDMolAtom &atomid);

    void invert();

    bool intersects(const AtomSelection &other) const;
    bool contains(const AtomSelection &other) const;

    AtomSelection intersect(const AtomSelection &other) const;
    AtomSelection unite(const AtomSelection &other) const;
    AtomSelection subtract(const AtomSelection &other) const;

    void applyMask(const QSet<CutGroupID> &cgids);
    void applyMask(const QSet<ResNum> &resnums);
    void applyMask(const AtomSelection &other);

    void assertCompatibleWith(const MoleculeInfo &molinfo) const;
    void assertCompatibleWith(const Molecule &molecule) const;
    void assertCompatibleWith(const AtomSelection &other) const;

    QList<AtomIndex> selected() const;

    QSet<CutGroupID> selectedCutGroups() const;
    QSet<ResNum> selectedResidues() const;

private:
    bool _pvt_selected(const CGAtomID &cgatomid) const;

    void _pvt_select(const CGAtomID &cgatomid);
    void _pvt_deselect(const CGAtomID &cgatomid);

    /** The AtomIDs of selected atoms, arranged by CutGroupID */
    QHash< CutGroupID, QSet<AtomID> > selected_atoms;

    /** The MoleculeInfo describing the molecule whose parts
        are being selected by this object */
    MoleculeInfo molinfo;

    /** The total number of selected atoms */
    int nselected;
};

}

Q_DECLARE_METATYPE(SireMol::AtomSelection);

SIRE_END_HEADER

#endif
