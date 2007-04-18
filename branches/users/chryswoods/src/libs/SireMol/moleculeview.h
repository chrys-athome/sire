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

#ifndef SIREMOL_MOLECULEVIEW_H
#define SIREMOL_MOLECULEVIEW_H

#include <QSharedDataPointer>

#include "sireglobal.h"

#include "moleculeversion.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class MoleculeView;
}

QDataStream& operator<<(QDataStream&, const SireMol::MoleculeView&);
QDataStream& operator>>(QDataStream&, SireMol::MoleculeView&);

namespace SireMol
{

class MoleculeData;
class Molecule;

/** This is the base class of all views of a Molecule. All Molecule classes
    are actually a view of MoleculeData - Molecule can see all of MoleculeData,
    Residue can see only the parts of MoleculeData that correspond to that
    residue etc.

    @author Christopher Woods
*/
class SIREMOL_EXPORT MoleculeView
{

friend QDataStream& ::operator<<(QDataStream&, const MoleculeView&);
friend QDataStream& ::operator>>(QDataStream&, MoleculeView&);

public:
    MoleculeView();

    MoleculeView(const MoleculeView &other);

    ~MoleculeView();

    Molecule molecule() const;

    void assertSameMolecule(const MoleculeView &other) const;
    void assertSameMajorVersion(const MoleculeView &other) const;
    void assertSameVersion(const MoleculeView &other) const;

protected:
    MoleculeView& operator=(const MoleculeView &other);

    bool operator==(const MoleculeView &other) const;
    bool operator!=(const MoleculeView &other) const;

    const MoleculeData& data() const;
    const MoleculeData& constData() const;

    MoleculeData& data();

private:
    /** Shared pointer to the underlying MoleculeData */
    QSharedDataPointer<MoleculeData> d;
};

}

SIRE_END_HEADER

#endif
