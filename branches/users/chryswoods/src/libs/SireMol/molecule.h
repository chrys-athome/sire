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

#ifndef SIREMOL_MOLECULE_H
#define SIREMOL_MOLECULE_H

SIRE_BEGIN_HEADER

namespace SireMol
{
class Molecule;
}

QDataStream& operator<<(QDataStream&, const SireMol::Molecule&);
QDataStream& operator>>(QDataStream&, SireMol::Molecule&);

uint qHash(const SireMol::Molecule &molecule);

namespace SireMol
{

/**
A Molecule represents a complete molecule. 

Most of the manipulation of a molecule is handled by the 'or/er' classes,
e.g. Mover, Selector, Editer, Evaluator.

These classes provide additional member functions, thereby allowing me
to keep the API of Molecule small.

Examples of use include;

mol = mol.move().translate( Vector(1,2,3) )
point = mol.evaluate().center()
mass = mol.evaluate().mass()

mol = mol.edit().rename( ResNum(43)[0], "ALA" ).commit()

Equally, we can quickly select well-defined subgroups within the
molecule, e.g. atom(s), residue(e), chain(s), CutGroup(s) and
segment(s), via the 'select' functions, e.g.

ala49 = mol.select( ResName("ala") + ResNum(49) );

or if there is more than one residue designate ALA:49

ala49_0 = mol.select( (ResName("ala")+ResNum(49))[0] );

or to get all of these residues, do

all_ala49 = mol.selectAll( ResName("ala") + ResNum(49) );

@author Christopher Woods
*/
class SIREMOL_EXPORT Molecule : public MoleculeView
{

friend QDataStream& ::operator<<(QDataStream&, const Molecule&);
friend QDataStream& ::operator>>(QDataStream&, Molecule&);

public:
    Molecule();

    Molecule(const Molecule &other);
    Molecule(const MoleculeView &other);

    ~Molecule();

    Molecule& operator=(const Molecule &other);
    Molecule& operator=(const MoleculeView &other);
    
    AtomSelection selectedAtoms() const;
    
    void update(const MoleculeView &other);
    
    const MolName& name() const;
    const MolNum& number() const;
    
    const MoleculeInfo& info() const;
    
    Mover<Molecule> move() const;
    Evaluator<Molecule> evaluate() const;
    Editor<Molecule> edit() const;
    Selector<Molecule> selection() const;
    
    Atom select(const AtomID &atomid) const;
    Residue select(const ResID &resid) const;
    Chain select(const ChainID &chainid) const;
    CutGroup select(const CGID &cgid) const;
    Segment select(const Segment &segid) const;
    
    AtomsInMol selectAll(const AtomID &atomid) const;
    ResiduesInMol selectAll(const ResID &resid) const;
    ChainsInMol selectAll(const ChainID &chainid) const;
    CutGroupsInMol selectAll(const CGID &cgid) const;
    SegmentsInMol selectAll(const SegID &segid) const;
    
    Atom atom(const AtomID &atomid) const;
    Residue residue(const ResidueID &resid) const;
    Chain chain(const ChainID &chainid) const;
    CutGroup cutGroup(const CGID &cgid) const;
    Segment segment(const SegID &segid) const;
    
    AtomsInMol atoms(const AtomID &atomid) const;
    AtomsInMol atoms(const ResidueID &resid) const;
    AtomsInMol atoms(const ChainID &chainid) const;
    AtomsInMol atoms(const CGID &cgid) const;
    AtomsInMol atoms(const SegID &segid) const;
    AtomsInMol atoms() const;
    
    ResInMol residues(const ResID &resid) const;
    ResInMol residues(const ChainID &chainid) const;
    ResInMol residues() const;
    
    ChainsInMol chains(const ChainID &chainid) const;
    ChainsInMol chains();
    
    CutGroupsInMol cutGroups(const CGID &cgid) const;
    CutGroupsInMol cutGroups();
    
    SegmentsInMol segments(const SegID &segid) const;
    SegmentsInMol segments();
    
    const Properties& properties() const;
    
    const Property& property(const QString &key) const;
    const Properties& metadata(const QString &key) const;
};

}

Q_DECLARE_METATYPE(SireMol::Molecule)

SIRE_END_HEADER

#endif
