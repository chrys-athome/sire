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

#ifndef SIREMOL_ATOMSELECTOR_H
#define SIREMOL_ATOMSELECTOR_H

#include <QVector>
#include <QHash>

#include "moleculeview.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class AtomSelector;
}

QDataStream& operator<<(QDataStream&, const SireMol::AtomSelector&);
QDataStream& operator>>(QDataStream&, SireMol::AtomSelector&);

namespace SireMol
{

/** This class is used to add a nice API to the MoleculeView based classes to
    allow the selection of parts of the molecule.

    e.g. can type mol.selection().add( ResNum(4) )
                  not_res = res.selection().invert()
                  
    This will eventually scale into a complete selection syntax, e.g.
    to select atoms called CA or HA
    
    ca_or_ha = mol.selection().set(AtomName("CA")).add(AtomName("HA"))
    
    or in shorthand...
    
    ca_or_ha = mol.selection().set(AtomName("CA")).add(AtomName("HA"))
    
    CA in ASP residues
    
    asp_ca = mol.selection().set(ResName("ASP")).unite(AtomName("CA"))
    
    not HIS residues
    
    not_his = mol.selection().set(ResName("HIS")).invert()

    Eventually I will expand this further by creating a compound
    expression type, so that I can type phrases directly, e.g.
    
    ca_or_ha = mol.selection().set( AtomName("CA") or AtomName("HA") )
    asp_ca = mol.selection().set(ResName("ASP") and AtomName("CA"))
    not_his = mol.selection().set(not ResName("HIS"))

    This class allows me to have the atom selection code
    all in one place, without the need to update multiple MoleculeView
    derived classes APIs

    @author Christopher Woods
*/
class SIREMOL_EXPORT AtomSelector : public MolDataView
{

friend QDataStream& ::operator<<(QDataStream&, const AtomSelector&);
friend QDataStream& ::operator>>(QDataStream&, AtomSelector&);

public:
    AtomSelector();

    AtomSelector(const MoleculeView &molecule);
    AtomSelector(const MoleculeView &molecule,
                 const SelectionFromMol &selected_atoms);

    AtomSelector(const AtomSelector &other);

    ~AtomSelector();

    AtomSelector& operator=(const AtomSelector &other);

    AtomSelector selectAll() const;
    
    AtomSelector deselectAll() const;
    AtomSelector selectNone() const;
    AtomSelector clear() const;
    
    AtomSelector selectAll(const SelectionFromMol &selection) const;
    
    AtomSelector deselectAll(const SelectionFromMol &selection) const;
    AtomSelector selectNone(const SelectionFromMol &selection) const;
    AtomSelector clear(const SelectionFromMol &selection) const;

    AtomSelector set(const SelectionFromMol &selection) const;
    AtomSelector setSelection(const SelectionFromMol &selection) const;

    AtomSelector add(const SelectionFromMol &selection) const;
    AtomSelector remove(const SelectionFromMol &selection) const;

    AtomSelector select(const SelectionFromMol &selection) const;
    AtomSelector deselect(const SelectionFromMol &selection) const;

    AtomSelector invert() const;

    AtomSelector intersect(const SelectionFromMol &selection) const;
    AtomSelector unite(const SelectionFromMol &selection) const;
    AtomSelector subtract(const SelectionFromMol &selection) const;

    AtomSelector applyMask(const SelectionFromMol &selection) const;

    bool isEmpty() const;
    bool isComplete() const;

    int nSelected() const;
    int nSelected(const SelectionFromMol &selection) const;

    int nSelectedCutGroups() const;
    int nSelectedResidues() const;

    bool selectedAllCutGroups() const;
    bool selectedAllResidues() const;

    bool selected(const SelectionFromMol &selection) const;
    bool selectedAll(const SelectionFromMol &selection) const;

    bool selectedAll() const;
    
    bool selectedNone() const;
    bool selectedNone(const SelectionFromMol &selection) const;

    bool intersects(const SelectionFromMol &selection) const;
    bool contains(const SelectionFromMol &selection) const;

    QSet<AtomIndex> atomIndexes() const;
    QSet<CutGroupID> cutGroupIDs() const;
    QSet<ResNum> residueNumbers() const;
};

}

Q_DECLARE_METATYPE(SireMol::AtomSelector)

SIRE_END_HEADER

#endif
