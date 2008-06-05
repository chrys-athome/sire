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

#ifndef SIREMOL_MOLECULEMOVER_H
#define SIREMOL_MOLECULEMOVER_H

#include <QVector>
#include <QHash>

#include "moleculeview.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class MoleculeMover;
}

QDataStream& operator<<(QDataStream&, const SireMol::MoleculeMover&);
QDataStream& operator>>(QDataStream&, SireMol::MoleculeMover&);

namespace SireVol
{
class Space;
}

namespace SireMol
{

using SireVol::Space;

/** This class is used to add a nice API to the MoleculeView based classes to
    allow the movement of parts of the molecule,

    e.g. can type mol.move().translate(delta)
                  res.move().change( bnd, delta )
                  mol.move().angle( atm0, atm1, atm2, delta )
                  mol.move().mapInto( space, &cgids_that_moved )
                  atom.move().rotate( matrix, point );

                  mol.move().change(bnd0,delta,&moved).change(bnd1,delta,&moved)
                            .change(ang0,delta,&moved).change(ang1,delta,&moved)

    All of the move functions take an optional pointer to a QSet<CutGroupID>
    which can be used to keep track of which CutGroups have been affected
    by the moves.

    In addition, this allows me to have the atom moving code
    all in one place, without the need to update multiple MoleculeView
    derived classes APIs

    @author Christopher Woods
*/
class SIREMOL_EXPORT MoleculeMover : public MolDataView
{

friend QDataStream& ::operator<<(QDataStream&, const MoleculeMover&);
friend QDataStream& ::operator>>(QDataStream&, MoleculeMover&);

public:
    MoleculeMover();

    MoleculeMover(const MoleculeView &molecule);

    MoleculeMover(const MoleculeView &molecule,
                  const SelectionFromMol &selection);

    MoleculeMover(const MoleculeMover &other);

    ~MoleculeMover();

    MoleculeMover& operator=(const MoleculeMover &other);

    MoleculeMover mapInto(const Space &space) const;

//     MoleculeMover translate(const Vector &delta) const;
//
//     MoleculeMover bond(const AtomIndex &atom0, const AtomIndex &atom1,
//                        double delta) const;
};

}

Q_DECLARE_METATYPE(SireMol::MoleculeMover)

SIRE_END_HEADER

#endif
