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

#include "SireBase/shareddatapointer.hpp"
#include "SireID/index.h"

#include "cgidx.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class AtomSelection;
}

QDataStream& operator<<(QDataStream&, const SireMol::AtomSelection&);
QDataStream& operator>>(QDataStream&, SireMol::AtomSelection&);

uint qHash(const SireMol::AtomSelection &selection);

namespace SireMol
{

class Molecule;
class MoleculeView;
class MoleculeInfoData;
class MoleculeData;

class AtomIdx;
class CGIdx;
class ResIdx;
class ChainIdx;
class SegIdx;

class AtomID;
class CGID;
class ResID;
class ChainID;
class SegID;

class CGAtomIdx;

using SireID::Index;

using SireBase::SharedDataPointer;

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
    AtomSelection(const MoleculeData &moldata);

    AtomSelection(const AtomSelection &other);

    ~AtomSelection();

    AtomSelection& operator=(const AtomSelection &other);

    bool operator==(const AtomSelection &other) const;
    bool operator!=(const AtomSelection &other) const;

    bool isEmpty() const;

    int nSelected() const;

    int nSelected(CGIdx cgidx) const;
    int nSelected(AtomIdx atomidx) const;
    int nSelected(ResIdx residx) const;
    int nSelected(ChainIdx chainidx) const;
    int nSelected(SegIdx segidx) const;
    
    int nSelected(const CGID &cgid) const;
    int nSelected(const AtomID &atomid) const;
    int nSelected(const ResID &resid) const;
    int nSelected(const ChainID &chainid) const;
    int nSelected(const SegID &segid) const;
    
    int nSelected(const AtomSelection &selection) const;

    int nSelectedAtoms() const;
    int nSelectedCutGroups() const;
    int nSelectedResidues() const;
    int nSelectedChains() const;
    int nSelectedSegments() const;

    bool selectedAllAtoms() const;
    bool selectedAllCutGroups() const;
    bool selectedAllResidues() const;
    bool selectedAllChains() const;
    bool selectedAllSegments() const;

    bool selected(const CGAtomIdx &cgatomidx) const;
    bool selected(AtomIdx atomidx) const;
    bool selected(const AtomID &atomid) const;

    bool selected(CGIdx cgidx) const;
    bool selected(ResIdx residx) const;
    bool selected(ChainIdx chainidx) const;
    bool selected(SegIdx segidx) const;
    
    bool selected(const CGID &cgid) const;
    bool selected(const ResID &resid) const;
    bool selected(const ChainID &chainid) const;
    bool selected(const SegID &segid) const;

    bool selected(const AtomSelection &selection) const;

    bool selectedAll() const;

    bool selectedAll(AtomIdx atomidx) const;
    bool selectedAll(CGIdx cgidx) const;
    bool selectedAll(ResIdx residx) const;
    bool selectedAll(ChainIdx chainidx) const;
    bool selectedAll(SegIdx segidx) const;
    
    bool selectedAll(const AtomID &atomid) const;
    bool selectedAll(const CGID &cgid) const;
    bool selectedAll(const ResID &resid) const;
    bool selectedAll(const ChainID &chainid) const;
    bool selectedAll(const SegID &segid) const;

    bool selectedAll(const AtomSelection &selection) const;

    bool selectedNone() const;

    bool selectedNone(AtomIdx atomidx) const;
    bool selectedNone(CGIdx cgidx) const;
    bool selectedNone(ResIdx residx) const;
    bool selectedNone(ChainIdx chainidx) const;
    bool selectedNone(SegIdx segidx) const;
    
    bool selectedNone(const AtomID &atomid) const;
    bool selectedNone(const CGID &cgid) const;
    bool selectedNone(const ResID &resid) const;
    bool selectedNone(const ChainID &chainid) const;
    bool selectedNone(const SegID &segid) const;

    bool selectedNone(const AtomSelection &selection) const;

    AtomSelection selectAll() const;
    AtomSelection deselectAll() const;
    AtomSelection selectNone() const;

    AtomSelection select(AtomIdx atomidx) const;
    AtomSelection deselect(AtomIdx atomidx) const;
    AtomSelection selectOnly(AtomIdx atomidx) const;

    AtomSelection select(CGIdx cgidx) const;
    AtomSelection deselect(CGIdx cgidx) const;
    AtomSelection selectOnly(CGIdx cgidx) const;

    AtomSelection select(ResIdx residx) const;
    AtomSelection deselect(ResIdx residx) const;
    AtomSelection selectOnly(ResIdx residx) const;

    AtomSelection select(ChainIdx chainidx) const;
    AtomSelection deselect(ChainIdx chainidx) const;
    AtomSelection selectOnly(ChainIdx chainidx) const;
    
    AtomSelection select(SegIdx segidx) const;
    AtomSelection deselect(SegIdx segidx) const;
    AtomSelection selectOnly(SegIdx segidx) const;

    AtomSelection select(const QList<AtomIdx> &atomidxs) const;
    AtomSelection deselect(const QList<AtomIdx> &atomidxs) const;
    AtomSelection selectOnly(const QList<AtomIdx> &atomidxs) const;

    AtomSelection select(const QList<CGIdx> &cgidxs) const;
    AtomSelection deselect(const QList<CGIdx> &cgidxs) const;
    AtomSelection selectOnly(const QList<CGIdx> &cgidxs) const;

    AtomSelection select(const QList<ResIdx> &residxs) const;
    AtomSelection deselect(const QList<ResIdx> &residxs) const;
    AtomSelection selectOnly(const QList<ResIdx> &residxs) const;

    AtomSelection select(const QList<ChainIdx> &chainidxs) const;
    AtomSelection deselect(const QList<ChainIdx> &chainidxs) const;
    AtomSelection selectOnly(const QList<ChainIdx> &chainidxs) const;

    AtomSelection select(const QList<SegIdx> &segidxs) const;
    AtomSelection deselect(const QList<SegIdx> &segidxs) const;
    AtomSelection selectOnly(const QList<SegIdx> &segidxs) const;

    AtomSelection select(const AtomID &atomid) const;
    AtomSelection deselect(const AtomID &atomid) const;
    AtomSelection selectOnly(const AtomID &atomid) const;

    AtomSelection select(const CGID &cgid) const;
    AtomSelection deselect(const CGID &cgid) const;
    AtomSelection selectOnly(const CGID &cgid) const;

    AtomSelection select(const ResID &resid) const;
    AtomSelection deselect(const ResID &resid) const;
    AtomSelection selectOnly(const ResID &resid) const;

    AtomSelection select(const ChainID &chainid) const;
    AtomSelection deselect(const ChainID &chainid) const;
    AtomSelection selectOnly(const ChainID &chainid) const;
    
    AtomSelection select(const SegID &segid) const;
    AtomSelection deselect(const SegID &segid) const;
    AtomSelection selectOnly(const SegID &segid) const;

    AtomSelection select(const AtomSelection &selection) const;
    AtomSelection deselect(const AtomSelection &selection) const;
    AtomSelection selectOnly(const AtomSelection &selection) const;

    AtomSelection invert() const;
    
    bool intersects(AtomIdx atomidx) const;
    bool intersects(CGIdx cgidx) const;
    bool intersects(ResIdx residx) const;
    bool intersects(ChainIdx chainidx) const;
    bool intersects(SegIdx segidx) const;
    
    bool intersects(const AtomID &atomid) const;
    bool intersects(const CGID &cgid) const;
    bool intersects(const ResID &resid) const;
    bool intersects(const ChainID &chainid) const;
    bool intersects(const SegID &segid) const;

    bool intersects(const AtomSelection &selection) const;
    
    bool contains(AtomIdx atomidx) const;
    bool contains(CGIdx cgidx) const;
    bool contains(ResIdx residx) const;
    bool contains(ChainIdx chainidx) const;
    bool contains(SegIdx segidx) const;
    
    bool contains(const AtomID &atomid) const;
    bool contains(const CGID &cgid) const;
    bool contains(const ResID &resid) const;
    bool contains(const ChainID &chainid) const;
    bool contains(const SegID &segid) const;

    bool contains(const AtomSelection &selection) const;

    AtomSelection intersect(AtomIdx atomidx) const;
    AtomSelection intersect(CGIdx cgidx) const;
    AtomSelection intersect(ResIdx residx) const;
    AtomSelection intersect(ChainIdx chainidx) const;
    AtomSelection intersect(SegIdx segidx) const;

    AtomSelection intersect(const QList<AtomIdx> &atomidx) const;
    AtomSelection intersect(const QList<CGIdx> &cgidx) const;
    AtomSelection intersect(const QList<ResIdx> &residx) const;
    AtomSelection intersect(const QList<ChainIdx> &chainidx) const;
    AtomSelection intersect(const QList<SegIdx> &segidx) const;
    
    AtomSelection intersect(const AtomID &atomid) const;
    AtomSelection intersect(const CGID &cgid) const;
    AtomSelection intersect(const ResID &resid) const;
    AtomSelection intersect(const ChainID &chainid) const;
    AtomSelection intersect(const SegID &segid) const;

    AtomSelection intersect(const AtomSelection &selection) const;

    AtomSelection unite(AtomIdx atomidx) const;
    AtomSelection unite(CGIdx cgidx) const;
    AtomSelection unite(ResIdx residx) const;
    AtomSelection unite(ChainIdx chainidx) const;
    AtomSelection unite(SegIdx segidx) const;

    AtomSelection unite(const QList<AtomIdx> &atomidx) const;
    AtomSelection unite(const QList<CGIdx> &cgidx) const;
    AtomSelection unite(const QList<ResIdx> &residx) const;
    AtomSelection unite(const QList<ChainIdx> &chainidx) const;
    AtomSelection unite(const QList<SegIdx> &segidx) const;
    
    AtomSelection unite(const AtomID &atomid) const;
    AtomSelection unite(const CGID &cgid) const;
    AtomSelection unite(const ResID &resid) const;
    AtomSelection unite(const ChainID &chainid) const;
    AtomSelection unite(const SegID &segid) const;

    AtomSelection unite(const AtomSelection &selection) const;

    AtomSelection subtract(AtomIdx atomidx) const;
    AtomSelection subtract(CGIdx cgidx) const;
    AtomSelection subtract(ResIdx residx) const;
    AtomSelection subtract(ChainIdx chainidx) const;
    AtomSelection subtract(SegIdx segidx) const;

    AtomSelection subtract(const QList<AtomIdx> &atomidx) const;
    AtomSelection subtract(const QList<CGIdx> &cgidx) const;
    AtomSelection subtract(const QList<ResIdx> &residx) const;
    AtomSelection subtract(const QList<ChainIdx> &chainidx) const;
    AtomSelection subtract(const QList<SegIdx> &segidx) const;
    
    AtomSelection subtract(const AtomID &atomid) const;
    AtomSelection subtract(const CGID &cgid) const;
    AtomSelection subtract(const ResID &resid) const;
    AtomSelection subtract(const ChainID &chainid) const;
    AtomSelection subtract(const SegID &segid) const;

    AtomSelection subtract(const AtomSelection &selection) const;

    AtomSelection mask(AtomIdx atomidx) const;
    AtomSelection mask(CGIdx cgidx) const;
    AtomSelection mask(ResIdx residx) const;
    AtomSelection mask(ChainIdx chainidx) const;
    AtomSelection mask(SegIdx segidx) const;

    AtomSelection mask(const QList<AtomIdx> &atomidx) const;
    AtomSelection mask(const QList<CGIdx> &cgidx) const;
    AtomSelection mask(const QList<ResIdx> &residx) const;
    AtomSelection mask(const QList<ChainIdx> &chainidx) const;
    AtomSelection mask(const QList<SegIdx> &segidx) const;
    
    AtomSelection mask(const AtomID &atomid) const;
    AtomSelection mask(const CGID &cgid) const;
    AtomSelection mask(const ResID &resid) const;
    AtomSelection mask(const ChainID &chainid) const;
    AtomSelection mask(const SegID &segid) const;

    AtomSelection mask(const AtomSelection &selection) const;

    QVector<AtomIdx> selectedAtoms() const;

    QSet<Index> selectedAtoms(CGIdx cgid) const;
    QList<CGIdx> selectedCutGroups() const;

    void assertCompatibleWith(const MoleculeData &moldata) const;
    void assertCompatibleWith(const MoleculeView &molview) const;
    void assertCompatibleWith(const AtomSelection &other) const;

private:
    const MoleculeInfoData& info() const;

    bool _pvt_selected(const CGAtomIdx &cgatomidx) const;
    bool _pvt_selected(AtomIdx atomidx) const;
    
    bool _pvt_selectedAll(CGIdx cgidx) const;
    bool _pvt_selectedAll(const QVector<CGAtomIdx> &atomidxs) const;

    int _pvt_nSelected(ResIdx residx) const;

    void _pvt_select(AtomIdx atomidx);
    void _pvt_deselect(AtomIdx atomidx);
    
    void _pvt_select(CGIdx cgidx);
    void _pvt_deselect(CGIdx cgidx);
    
    void _pvt_select(const CGAtomIdx &cgatomidx);
    void _pvt_deselect(const CGAtomIdx &cgatomidx);
    
    void _pvt_select(CGIdx cgidx, const QSet<Index> &atoms);
    
    void _pvt_select(const QVector<CGAtomIdx> &cgatomidxs);
    void _pvt_deselect(const QVector<CGAtomIdx> &cgatomidxs);

    template<class IDX>
    void _pvt_selectFromIndex(IDX idx);
    
    template<class IDX>
    void _pvt_deselectFromIndex(IDX idx);

    template<class IDX>
    void _pvt_select(const QList<IDX> &idxs, int n);
    
    template<class IDX>
    void _pvt_deselect(const QList<IDX> &idxs, int n);

    /** The indicies of selected atoms, arranged by CGIdx */
    QHash< CGIdx, QSet<Index> > selected_atoms;

    /** The MoleculeInfo describing the molecule whose parts
        are being selected by this object */
    SharedDataPointer<MoleculeInfoData> d;

    /** The total number of selected atoms */
    qint32 nselected;
};

}

Q_DECLARE_METATYPE(SireMol::AtomSelection);

SIRE_END_HEADER

#endif
