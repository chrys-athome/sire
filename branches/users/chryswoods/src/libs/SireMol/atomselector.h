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

    In addition, this allows me to have the atom selection code
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

    AtomSelector add(const SelectionFromMol &selection) const;
    AtomSelector remove(const SelectionFromMol &selection) const;

    AtomSelector invert() const;
};

}

Q_DECLARE_METATYPE(SireMol::AtomSelector)

SIRE_END_HEADER

#endif
