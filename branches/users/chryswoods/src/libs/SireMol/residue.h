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

#ifndef SIREMOL_RESIDUE_H
#define SIREMOL_RESIDUE_H

#include "moleculeview.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class Residue;
}

QDataStream& operator<<(QDataStream&, const SireMol::Residue&);
QDataStream& operator>>(QDataStream&, SireMol::Residue&);

namespace SireMol
{

/**
This class represents a Residue in a Molecule.

@author Christopher Woods
*/
class SIREMOL_EXPORT Residue : public MoleculeView
{

friend QDataStream& ::operator<<(QDataStream&, const Residue&);
friend QDataStream& ::operator>>(QDataStream&, Residue&);

public:
    Residue();
    
    Residue(const MoleculeView &molview, const ResID &resid);
    Residue(const Residue &other);

    ~Residue();

    Residue& operator=(const Residue &other);
    Residue& operator=(const MoleculeView &other);
    
    AtomSelection selectedAtoms() const;
    
    void update(const MoleculeView &other);

    ResName name() const;
    ResNum number() const;
    
    ResidueInfo info() const;
    
    Mover<Residue> move() const;
    Evaluator<Residue> evaluate() const;
    Editor<Residue> edit() const;
    Selector<Residue> selection() const;
    
    Molecule molecule() const;
    Chain chain() const;
    
    Atom select(const AtomID &atomid) const;
    
    AtomsInMol selectAll(const AtomID &atomid) const;
    AtomsInMol selectAll() const;
    
    Atom atom(const AtomID &atomid) const;
    
    AtomsInMol atoms(const AtomID &atomid) const;
    AtomsInMol atoms() const;

    Properties properties() const;
    
    Property property(const QString &key) const;
    const Properties& metadata(const QString &key) const;

private:
    /** The index of the residue in the molecule */
    ResIdx residx;
    
    /** The atoms that are selected as part of this residue */
    AtomSelection selected_atoms;
};

}

Q_DECLARE_METATYPE(SireMol::Residue)

SIRE_END_HEADER

#endif
