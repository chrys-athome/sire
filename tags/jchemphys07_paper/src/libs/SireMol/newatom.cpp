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

#include "qhash_siremol.h"

#include "newatom.h"

#include "moleculeview_inlines.h"
#include "molecule.h"
#include "residue.h"
#include "cgatomid.h"
#include "atominfo.h"
#include "residueinfo.h"
#include "moleculeinfo.h"

#include "atomicproperties.h"

#include "SireBase/sharedpolypointer_cast.hpp"

#include "SireMaths/rotate.h"
#include "SireMaths/quaternion.h"
#include "SireMaths/matrix.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMol;
using namespace SireStream;
using namespace SireBase;
using namespace SireMaths;

static const RegisterMetaType<NewAtom> r_newatom;

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const NewAtom &atom)
{
    writeHeader(ds, r_newatom, 1);

    SharedDataStream(ds) << atom.cgatomid
                         << static_cast<const MoleculeView&>(atom);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, NewAtom &atom)
{
    VersionID v = readHeader(ds, r_newatom);

    if (v == 1)
    {
        ds >> atom.cgatomid
           >> static_cast<MoleculeView&>(atom);
    }
    else
        throw version_error(v, "1", r_newatom, CODELOC);

    return ds;
}

uint SIREMOL_EXPORT qHash(const NewAtom &atom)
{
    return qHash(atom.info().index());
}

/** Null constructor */
NewAtom::NewAtom() : MoleculeView()
{}

/** Construct an Atom that represents the atom at index 'i' in the
    molecule 'molecule'

    \throw SireError::invalid_index
*/
NewAtom::NewAtom(const CGAtomID &i, const Molecule &molecule)
        : MoleculeView(molecule), cgatomid(i)
{
    constData().info().assertAtomExists(i);
}

/** Construct an Atom that represents the atom at index 'i' in the
    molecule 'molecule'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
NewAtom::NewAtom(const IDMolAtom &i, const Molecule &molecule)
        : MoleculeView(molecule), cgatomid( i.index(molecule.info()) )
{}

/** Construct an Atom that represents the atom called 'name' in
    the residue 'residue'

    \throw SireMol::missing_atom
*/
NewAtom::NewAtom(const QString &name, const Residue &residue)
        : MoleculeView(residue), cgatomid( residue.info().at(name) )
{}

/** Construct an Atom that represents the atom at index 'i'
    in the residue 'residue'

    \throw SireError::invalid_index
*/
NewAtom::NewAtom(AtomID i, const Residue &residue)
        : MoleculeView(residue), cgatomid( residue.info().at(i) )
{}

/** Copy constructor */
NewAtom::NewAtom(const NewAtom &other)
        : MoleculeView(other), cgatomid(other.cgatomid)
{}

/** Destructor */
NewAtom::~NewAtom()
{}

/** Copy assignment operator */
NewAtom& NewAtom::operator=(const NewAtom &other)
{
    MoleculeView::operator=(other);
    cgatomid = other.cgatomid;

    return *this;
}

/** Are these the same? They are if they refer to the same
    atoms in the same version of the same molecule */
bool NewAtom::operator==(const NewAtom &other) const
{
    return cgatomid == other.cgatomid and
           MoleculeView::operator==(other);
}

/** Are these different? They are different if they point
    to different atoms, or to different molecules */
bool NewAtom::operator!=(const NewAtom &other) const
{
    return cgatomid != other.cgatomid or
           MoleculeView::operator!=(other);
}

/** Return the ID number of the molecule that contains this atom */
MoleculeID NewAtom::ID() const
{
    return data().ID();
}

/** Return the version number of the molecule that contains this atom */
const Version& NewAtom::version() const
{
    return data().version();
}

/** Return the AtomInfo for this Atom */
const AtomInfo& NewAtom::info() const
{
    return data().info().atom(cgatomid);
}

/** Return a string identifying this atom */
QString NewAtom::idString() const
{
    return QObject::tr("%1 in %2 (%3) in \"%3\" (%4 %5)")
                          .arg(name())
                          .arg(data().info().residueName(info().resNum()))
                          .arg(info().resNum())
                          .arg(data().info().name()).arg(ID())
                          .arg(data().version().toString());
}

/** Return the name of this atom */
QString NewAtom::name() const
{
    return this->info().name();
}

/** Return a copy of the chemical element of this atom */
Element NewAtom::element() const
{
    return this->info().element();
}

/** Return a reference to the coordinates of this atom */
Vector NewAtom::coordinates() const
{
    return data().coordinates(cgatomid);
}

/** Return the CGAtomID of the atom */
const CGAtomID& NewAtom::cgAtomID() const
{
    return cgatomid;
}

/** Allow implicit conversion to a Vector (to get the coordinates) */
NewAtom::operator Vector() const
{
    return this->coordinates();
}

/** Allow implicit conversion to an Element */
NewAtom::operator Element() const
{
    return this->element();
}

/** Allow implicit conversion to an AtomInfo object */
NewAtom::operator const AtomInfo&() const
{
    return this->info();
}

/** Return a copy of the property called 'name' for this
    atom. Note that this must be an AtomProperty class or
    an exception will be thrown

    \throw SireBase::missing_property
    \throw SireError::invalid_cast
*/
QVariant NewAtom::property(const QString &name) const
{
    Property property = data().getProperty(name);

    if (not property.isA<AtomicProperties>())
        throw SireError::invalid_cast( QObject::tr(
            "Cannot convert a property of type \"%1\" to an atomic property!")
                .arg(property.what()), CODELOC );

    return property.asA<AtomicProperties>().value(cgatomid);
}

/** Return a string describing this atom */
QString NewAtom::toString() const
{
    Residue res = this->residue();

    return QString("%1 : %2(%3) : %4 (%5 - %6)")
                 .arg(this->name())
                 .arg(res.name()).arg(res.number())
                 .arg(data().info().name()).arg(data().ID())
                 .arg(data().version().major()).arg(data().version().minor());
}

/** Return a copy of the molecule that contains this atom */
Molecule NewAtom::molecule() const
{
    return Molecule(*this);
}

/** Return a copy of the residue that contains this atom */
Residue NewAtom::residue() const
{
    return Residue(*this);
}

/** Return the set of all atoms that are bonded to this atom */
QSet<NewAtom> NewAtom::bondedAtoms() const
{
    QSet<AtomIndex> bondedatms = data().connectivity().atomsBondedTo(*this);

    QSet<NewAtom> atoms;
    atoms.reserve(bondedatms.count());

    for (QSet<AtomIndex>::const_iterator it = bondedatms.constBegin();
         it != bondedatms.constEnd();
         ++it)
    {
        atoms.insert( NewAtom(*it, molecule()) );
    }

    return atoms;
}

/** Return a set of all residues that are bonded to this atom
    (includes the residue that contains this atom) */
QSet<Residue> NewAtom::bondedResidues() const
{
    Residue res = this->residue();

    QSet<ResNum> bondedres = data().connectivity().resNumsBondedTo(res.number());

    QSet<Residue> allres;
    allres.reserve( bondedres.count() + 1 );

    allres.insert(res);

    for (QSet<ResNum>::const_iterator it = bondedres.constBegin();
         it != bondedres.constEnd();
         ++it)
    {
        allres.insert( molecule().residue(*it) );
    }

    return allres;
}

/** Return whether this atom is within bonding distance of 'other', to
    within an error of 'err' angstroms */
bool NewAtom::withinBondRadii(const NewAtom &other, double err) const
{
    double bonddist = info().bondOrderRadius() + other.info().bondOrderRadius() + err;

    return Vector::distance2(*this, other) < bonddist*bonddist;
}
