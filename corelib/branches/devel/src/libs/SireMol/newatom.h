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

#ifndef SIREMOL_NEWATOM_H
#define SIREMOL_NEWATOM_H

#include <QVariant>

#include "idtypes.h"
#include "cgatomid.h"

#include "moleculeview.h"

#include "SireMaths/vector.h"

#include "SireError/errors.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class NewAtom;
}

QDataStream& operator<<(QDataStream&, const SireMol::NewAtom&);
QDataStream& operator>>(QDataStream&, SireMol::NewAtom&);

uint qHash(const SireMol::NewAtom&);

namespace SireMaths
{
class Quaternion;
class Matrix;
}

namespace SireMol
{

class Molecule;
class PartialMolecule;
class Residue;
class Element;
class AtomInfo;
class MoleculeVersion;

using SireMaths::Vector;
using SireMaths::Quaternion;
using SireMaths::Matrix;

/** This is a replacement for the Atom class that provides an
    Atom that is in context with its containing molecule.

    The main difference to the old Atom class is that this
    one is the MoleculeView of the
    Molecule that contains this Atom, and that therefore,
    the editing functions available are more limited than
    Atom (which can be heavily edited due to its
    use of multiple inheritance)

    This is a MoleculeView in the same way that
    Residue and Molecule are MoleculeViews

    @author Christopher Woods
*/
class SIREMOL_EXPORT NewAtom : public MoleculeView
{

friend QDataStream& ::operator<<(QDataStream&, const NewAtom&);
friend QDataStream& ::operator>>(QDataStream&, NewAtom&);

public:
    NewAtom();
    NewAtom(const CGAtomID &cgatomid, const Molecule &molecule);
    NewAtom(const IDMolAtom &idmolatom, const Molecule &molecule);

    NewAtom(const QString &name, const Residue &residue);
    NewAtom(AtomID i, const Residue &residue);

    NewAtom(const NewAtom &other);

    ~NewAtom();

    NewAtom& operator=(const NewAtom &other);

    NewAtom& operator=(const Vector &newcoords);

    NewAtom& operator+=(const Vector &delta);
    NewAtom& operator-=(const Vector &delta);

    bool operator==(const NewAtom &other) const;
    bool operator!=(const NewAtom &other) const;

    QString name() const;

    MoleculeID ID() const;
    const MoleculeVersion& version() const;

    QString idString() const;

    const AtomInfo& info() const;
    Element element() const;
    Vector coordinates() const;

    const CGAtomID& cgAtomID() const;

    QVariant property(const QString &name) const;

    QString toString() const;

    Molecule molecule() const;
    Residue residue() const;

    QSet<NewAtom> bondedAtoms() const;
    QSet<Residue> bondedResidues() const;

    operator Vector() const;
    operator Element() const;
    operator const AtomInfo&() const;

    void setCoordinates(double x, double y, double z);
    void setCoordinates(const Vector &newcoords);

    void translate(const Vector &delta);
    void rotate(const Quaternion &quat, const Vector &point);
    void rotate(const Matrix &rotmat, const Vector &point);

    bool withinBondRadii(const NewAtom &other, double err=0) const;

private:
    /** The index of this atom in the molecule */
    CGAtomID cgatomid;
};

}

Q_DECLARE_METATYPE(SireMol::NewAtom);

SIRE_END_HEADER

#endif
