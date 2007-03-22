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

    const MoleculeInfo& info() const;

    const ResidueInfo& info(ResNum resnum) const;
    const ResidueInfo& info(ResID resid) const;

    Molecule molecule() const;

    Residue residue(ResNum resnum) const;
    Residue residue(ResID resid) const;
    Residue residue(const QString &resname) const;

    NewAtom atom(const IDMolAtom &atomid) const;

    const AtomSelection& selection() const;

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
