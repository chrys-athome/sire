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

#ifndef SIREMOL_PARTIALMOLECULE_H
#define SIREMOL_PARTIALMOLECULE_H

#include "moleculedata.h"
#include "atomselection.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class PartialMolecule;
}

QDataStream& operator<<(QDataStream&, const SireMol::PartialMolecule&);
QDataStream& operator>>(QDataStream&, SireMol::PartialMolecule&);

namespace SireMol
{

class Molecule;
class Residue;
class NewAtom;

/** This class provides a view to an arbitrary part of a molecule
    (which can range from just a single atom all the way through to
     the entire molecule). As such, this class can be used to
     represent Molecule, Residue and NewAtom, as well as everything
     in between!

    @author Christopher Woods
*/
class SIREMOL_EXPORT PartialMolecule
{

friend QDataStream& ::operator<<(QDataStream&, const PartialMolecule&);
friend QDataStream& ::operator>>(QDataStream&, PartialMolecule&);

public:
    PartialMolecule();
    PartialMolecule(const Molecule &molecule);
    PartialMolecule(const Molecule &molecule,
                    const AtomSelection &selected_atoms);
    PartialMolecule(const Residue &residue);
    PartialMolecule(const NewAtom &atom);

    PartialMolecule(const PartialMolecule &other);

    ~PartialMolecule();

    PartialMolecule& operator=(const Molecule &molecule);
    PartialMolecule& operator=(const Residue &residue);
    PartialMolecule& operator=(const NewAtom &atom);
    PartialMolecule& operator=(const PartialMolecule &other);

    bool operator==(const PartialMolecule &other) const;
    bool operator!=(const PartialMolecule &other) const;

    MoleculeID ID() const;
    const MoleculeVersion& version() const;

    const MoleculeInfo& info() const;

    const ResidueInfo& info(ResNum resnum) const;
    const ResidueInfo& info(ResID resid) const;

    Molecule molecule() const;

    Residue residue(ResNum resnum) const;
    Residue residue(ResID resid) const;
    Residue residue(const QString &resname) const;

    NewAtom atom(const IDMolAtom &atomid) const;

    const AtomSelection& selection() const;

    bool change(const PartialMolecule &molecule) const;
    bool add(const PartialMolecule &molecule) const;
    bool remove(const PartialMolecule &molecule) const;

    bool contains(const PartialMolecule &molecule) const;

    // Interface from AtomSelection

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

    void applyMask(const QSet<CutGroupID> &cgids);
    void applyMask(const QSet<ResNum> &resnums);
    void applyMask(const AtomSelection &other);

    QList<AtomIndex> selected() const;

    QSet<CutGroupID> selectedCutGroups() const;
    QSet<ResNum> selectedResidues() const;

private:
    /** The actual molecular data */
    MoleculeData moldata;

    /** The atoms which have been selected */
    AtomSelection slctd_atms;
};

}

Q_DECLARE_METATYPE(SireMol::PartialMolecule);

SIRE_END_HEADER

#endif
