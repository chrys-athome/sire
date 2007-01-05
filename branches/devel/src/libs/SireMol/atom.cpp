
#include <QObject>

#include "atom.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireMol;

static const RegisterMetaType<Atom> r_atom;

/** Serialise an atom to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const Atom &atom)
{
    writeHeader(ds, r_atom, 1) << atom.info() << atom.vector();

    return ds;
}

/** Deserialise an atom from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, Atom &atom)
{
    VersionID v = readHeader(ds, r_atom);

    if (v == 1)
    {
        ds >> atom.info() >> atom.vector();
    }
    else
        throw version_error(v, "1", r_atom, CODELOC);

    return ds;
}

/** Construct a null atom */
Atom::Atom() : AtomInfo(), Vector()
{}

/** Copy constructor */
Atom::Atom(const Atom &other) : AtomInfo(other), Vector(other)
{}

/** Construct a new atom, specifying its details... */
Atom::Atom(const AtomIndex &idx, const Element &element, const Vector &coords)
     : AtomInfo(idx,element), Vector(coords)
{}

/** Construct a new atom, specifying its details... */
Atom::Atom(const AtomIndex &atm) : AtomInfo(atm), Vector()
{}

/** Construct a copy of the atom, using a new residue number 'resnum' */
Atom::Atom(const Atom &other, ResNum resnum)
     : AtomInfo(other,resnum), Vector()
{}

/** More overloaded constructors - these behave as the above constructors, except that they
    also provide the first argument which is used to set the (mostly unnecessery) atom number. */
Atom::Atom(AtomNum num, const AtomIndex &atomindex,
           const Element &element, const Vector &coords)
     : AtomInfo(num, atomindex, element), Vector(coords)
{}

/** More overloaded constructors - these behave as the above constructors, except that they
    also provide the first argument which is used to set the (mostly unnecessery) atom number. */
Atom::Atom(AtomNum atm, const Atom &other)
     : AtomInfo(atm,other), Vector(other)
{}

/** More overloaded constructors - these behave as the above constructors, except that they
    also provide the first argument which is used to set the (mostly unnecessery) atom number. */
Atom::Atom(AtomNum atm, const Atom &other, ResNum resnum)
     : AtomInfo(atm,other,resnum), Vector(other)
{}

/** Convienience constructors - these are used to allow string based creation of
    atoms. This is particularly helpful when scripting the creation of atoms.

    Create an atom called 'name' in residue 0, with element guessed from 'name' and
    coordinates (0,0,0) and atom number 0.
*/
Atom::Atom(const QString &name) : AtomInfo(name), Vector()
{}

/** Convienience constructors - these are used to allow string based creation of
    atoms. This is particularly helpful when scripting the creation of atoms.

    Create an atom called 'name' in residue 0, with element guessed from 'elem' and
    coordinates (0,0,0) and atom number 0.
*/
Atom::Atom(const QString &name, const QString &elem)
     : AtomInfo(name, elem), Vector()
{}

/** Convienience constructors - these are used to allow string based creation of
    atoms. This is particularly helpful when scripting the creation of atoms.

    Create an atom called 'name' in residue resnum, with element guessed from 'name' and
    coordinates (0,0,0) and atom number 0.
*/
Atom::Atom(const QString &name, ResNum resnum)
     : AtomInfo(name, resnum), Vector()
{}

/** Convienience constructors - these are used to allow string based creation of
    atoms. This is particularly helpful when scripting the creation of atoms.

    Create an atom called 'name' in residue resnum, with element guessed from 'elem' and
    coordinates (0,0,0) and atom number 0.
*/
Atom::Atom(const QString &name, ResNum resnum, const QString &elem)
     : AtomInfo(name, resnum, elem), Vector()
{}

/** Convienience constructors - these are used to allow string based creation of
    atoms. This is particularly helpful when scripting the creation of atoms.

    Create an atom called 'name' in residue 0, with element guessed from 'name' and
    coordinates coords and atom number 0.
*/
Atom::Atom(const QString &name, const Vector &coords)
     : AtomInfo(name), Vector(coords)
{}

/** Convienience constructors - these are used to allow string based creation of
    atoms. This is particularly helpful when scripting the creation of atoms.

    Create an atom called 'name' in residue 0, with element guessed from 'elem' and
    coordinates coords and atom number 0.
*/
Atom::Atom(const QString &name, const QString &elem, const Vector &coords)
     : AtomInfo(name,elem), Vector(coords)
{}

/** Convienience constructors - this allows easy changing of the name of an atom */
Atom::Atom(const QString &name, const Atom &other)
     : AtomInfo(name,other), Vector(other)
{}

/** Convienience constructors - these are used to allow string based creation of
    atoms. This is particularly helpful when scripting the creation of atoms.

    Create an atom called 'name' in residue 0, with element guessed from 'name' and
    coordinates (0,0,0) and atom number num.
*/
Atom::Atom(AtomNum num, const QString &name) : AtomInfo(num,name), Vector()
{}

/** Convienience constructors - these are used to allow string based creation of
    atoms. This is particularly helpful when scripting the creation of atoms.

    Create an atom called 'name' in residue 0, with element guessed from 'elem' and
    coordinates (0,0,0) and atom number num.
*/
Atom::Atom(AtomNum num, const QString &name, const QString &elem)
     : AtomInfo(num,name,elem), Vector()
{}

/** Convienience constructors - these are used to allow string based creation of
    atoms. This is particularly helpful when scripting the creation of atoms.

    Create an atom called 'name' in residue 0, with element guessed from 'name' and
    coordinates coords and atom number num.
*/
Atom::Atom(AtomNum num, const QString &name, const Vector &coords)
     : AtomInfo(num,name), Vector(coords)
{}

/** Convienience constructors - these are used to allow string based creation of
    atoms. This is particularly helpful when scripting the creation of atoms.

    Create an atom called 'name' in residue 0, with element guessed from 'elem' and
    coordinates coords and atom number num.
*/
Atom::Atom(AtomNum num, const QString &name, const QString &elem, const Vector &coords)
     : AtomInfo(num,name,elem), Vector(coords)
{}

/** Construct an Atom by combining coordinates with the AtomInfo */
Atom::Atom(const AtomInfo &atominfo, const Vector &coords)
     : AtomInfo(atominfo), Vector(coords)
{}

/** Destructor */
Atom::~Atom()
{}

/** Return whether two atoms are within the sum of their bond-order radii.
    This is normally a good indication that the atoms are bonded together.
    (err is used to add an error to the sum of the bond-order radii) */
bool Atom::withinBondRadii(const Atom &atm0, const Atom &atm1, double err)
{
    double bonddist = atm0.bondOrderRadius() + atm1.bondOrderRadius() + err;

    return Vector::distance2(atm0,atm1) < bonddist*bonddist;
}
