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

#include "moleculemover.h"
#include "moleculeview_inlines.h"

#include "SireVol/coordgroup.h"
#include "SireVol/space.h"

#include "SireStream/datastream.h"

using namespace SireMol;
using namespace SireVol;
using namespace SireStream;

static const RegisterMetaType<MoleculeMover> r_molmover;

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds,
                                       const MoleculeMover &molmover)
{
    writeHeader(ds, r_molmover, 1)
          << static_cast<const MolDataView&>(molmover);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds,
                                       MoleculeMover &molmover)
{
    VersionID v = readHeader(ds, r_molmover);

    if (v == 1)
    {
        ds >> static_cast<MolDataView&>(molmover);
    }
    else
        throw version_error(v, "1", r_molmover, CODELOC);

    return ds;
}

/** Null constructor */
MoleculeMover::MoleculeMover() : MolDataView()
{}

/** Construct the mover to move atoms from 'molecule' */
MoleculeMover::MoleculeMover(const MoleculeView &molecule)
             : MolDataView(molecule)
{}

/** Construct the mover to move atoms from the 'selection'
    within the molecule 'molecule' 
    
    \throw SireError::incompatible_error
*/
MoleculeMover::MoleculeMover(const MoleculeView &molecule,
                             const SelectionFromMol &selection)
              : MolDataView(molecule)
{
    _pvt_selection() = selectedAtoms().intersect(selection);
}

/** Copy constructor */
MoleculeMover::MoleculeMover(const MoleculeMover &other)
             : MolDataView(other)
{}

/** Copy assignment operator */
MoleculeMover& MoleculeMover::operator=(const MoleculeMover &other)
{
    MolDataView::operator=(other);
    return *this;
}

/** Destructor */
MoleculeMover::~MoleculeMover()
{}

/** Map the whole of this molecule into the Space 'space'. This move
    is unusual in that it will move the entire molecule, regardless of
    how much is currently selected */
MoleculeMover MoleculeMover::mapInto(const Space &space) const
{
    //get the CoordGroups containing the current coordinates
    QVector<CoordGroup> coords = data().coordGroups();

    //map them all into the space
    QVector<CoordGroup> mapped_coords = space.moveToCenterBox(coords);

    //has anything changed?
    if (coords.constData() == mapped_coords.constData())
        //no - the arrays are the same
        return *this;
    else
    {
        //at least one of the CutGroups may have changed

        //update the molecule
        MoleculeMover newmol = *this;
        newmol.data().setCoordinates(mapped_coords);

        return newmol;
    }
}
