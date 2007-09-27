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

#ifndef SIREMOL_ATOMSINMOL_H
#define SIREMOL_ATOMSINMOL_H

#include "moleculeview.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class AtomsInMol;
}

namespace SireMol
{

/** This class represents a collection of atoms in a molecule.
    This class is distinct from PartialMolecule, which is just
    an arbitrary group within a Molecule - which this class
    is a collection of atoms, and has functions that allow
    trivial access to the individual atoms.
    
    @author Christopher Woods
*/
class SIREMOL_EXPORT AtomsInMol : public MoleculeView
{
public:
    AtomsInMol();
    
    AtomsInMol(const MoleculeView &view);
    AtomsInMol(const MoleculeView &view, const AtomID &atomid);
    AtomsInMol(const MoleculeView &view, const ResID &resid);
    AtomsInMol(const MoleculeView &view, const ChainID &chainid);
    AtomsInMol(const MoleculeView &view, const CGID &cgid);
    AtomsInMol(const MoleculeView &view, const SegID &segid);
    
    AtomsInMol(const AtomsInMol &other);
    
    ~AtomsInMol();
    
    AtomsInMol& operator=(const AtomsInMol &other);
    AtomsInMol& operator=(const MoleculeView &other);
    
    AtomSelection selectedAtoms() const;
    
    void update(const MoleculeView &other);
    
    Atom operator[](int i) const;
    Atom operator()(int i) const;
    AtomsInMol operator()(int i, int j) const;
    
    Atom at(int i) const;
    AtomsInMol at(int i, int j) const;
    
    Mover<AtomsInMol> move() const;
    Mover<AtomsInMol> move(int i) const;
    Mover<AtomsInMol> move(int i, int j) const;
    
    Evaluator<AtomsInMol> evaluate() const;
    Evaluator<AtomsInMol> evaluate(int i) const;
    Evaluator<AtomsInMol> evaluate(int i, int j) const;
    
    Editor<AtomsInMol> edit() const;
    Editor<AtomsInMol> edit(int i) const;
    Editor<AtomsInMol> edit(int i, int j) const;
    
    Selector<AtomsInMol> selection() const;
    
    Atom select(const AtomID &atomid) const;
    
    AtomsInMol selectAll(const AtomID &atomid) const;
    
    Atom atom(const AtomID &atomid) const;
    AtomsInMol atoms(const AtomID &atomid) const;
    
    const Property& property(const QString &key) const;
    const Properties& metadata(const QString &key) const;

private:
    /** The list of indicies of the atoms that are selected */
    QList<AtomIdx> atomidxs;
    
    /** The selected atoms */
    AtomSelection selected_atoms;
};

}

SIRE_END_HEADER

#endif
