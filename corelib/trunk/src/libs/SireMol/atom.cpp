
#include <QObject>

#include "atom.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireMol;

static const RegisterMetaType<Atom> r_atom("SireMol::Atom");

/** Serialise an atom to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const Atom &atom)
{
    writeHeader(ds, r_atom, 1) << atom.number() << atom.index() 
                               << atom.element() << atom.vector();
    
    return ds;
}

/** Deserialise an atom from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, Atom &atom)
{
    VersionID v = readHeader(ds, r_atom);
    
    if (v == 1)
    {
        ds >> atom.atmnum >> atom.index() >> atom.element() >> atom.vector();
    }
    else
        throw version_error(v, "1", r_atom, CODELOC);
    
    return ds;
}

/** Construct a null atom */
Atom::Atom() : AtomIndex(QString::null, 0), Element(), Vector(), atmnum(0)
{}

/** Copy constructor */
Atom::Atom(const Atom &other) 
     : AtomIndex(other), Element(other), Vector(other), atmnum(other.atmnum)
{}

/** Construct a new atom, specifying its details... */
Atom::Atom(const AtomIndex &idx, const Element &element, const Vector &coords)
     : AtomIndex(idx), Element(element), Vector(coords), atmnum(0)
{}

/** Construct a new atom, specifying its details... */
Atom::Atom(const AtomIndex &atm) : AtomIndex(atm), Element(atm.name()), Vector(), atmnum(0)
{}

/** Construct a copy of the atom, using a new residue number 'resnum' */
Atom::Atom(const Atom &other, ResNum resnum)
     : AtomIndex(other.name(),resnum), Element(other), Vector(other), atmnum(other.atmnum)
{}

/** More overloaded constructors - these behave as the above constructors, except that they
    also provide the first argument which is used to set the (mostly unnecessery) atom number. */
Atom::Atom(AtomNum num, const AtomIndex &atomindex, const Element &element, const Vector &coords)
     : AtomIndex(atomindex), Element(element), Vector(coords), atmnum(num)
{}

/** More overloaded constructors - these behave as the above constructors, except that they
    also provide the first argument which is used to set the (mostly unnecessery) atom number. */
Atom::Atom(AtomNum atm, const Atom &other)
     : AtomIndex(other), Element(other), Vector(other), atmnum(atm)
{}

/** More overloaded constructors - these behave as the above constructors, except that they
    also provide the first argument which is used to set the (mostly unnecessery) atom number. */
Atom::Atom(AtomNum atm, const Atom &other, ResNum resnum)
     : AtomIndex(other.name(),resnum), Element(other), Vector(other), atmnum(atm)
{}

/** Convienience constructors - these are used to allow string based creation of 
    atoms. This is particularly helpful when scripting the creation of atoms.
     
    Create an atom called 'name' in residue 0, with element guessed from 'name' and 
    coordinates (0,0,0) and atom number 0.
*/
Atom::Atom(const QString &name) : AtomIndex(name), Element(name), Vector(), atmnum(0)
{}

/** Convienience constructors - these are used to allow string based creation of 
    atoms. This is particularly helpful when scripting the creation of atoms.
     
    Create an atom called 'name' in residue 0, with element guessed from 'elem' and 
    coordinates (0,0,0) and atom number 0.
*/
Atom::Atom(const QString &name, const QString &elem)
     : AtomIndex(name), Element(elem), Vector(), atmnum(0)
{}
    
/** Convienience constructors - these are used to allow string based creation of 
    atoms. This is particularly helpful when scripting the creation of atoms.
     
    Create an atom called 'name' in residue resnum, with element guessed from 'name' and 
    coordinates (0,0,0) and atom number 0.
*/
Atom::Atom(const QString &name, ResNum resnum)
     : AtomIndex(name,resnum), Element(name), Vector(), atmnum(0)
{}

/** Convienience constructors - these are used to allow string based creation of 
    atoms. This is particularly helpful when scripting the creation of atoms.
     
    Create an atom called 'name' in residue resnum, with element guessed from 'elem' and 
    coordinates (0,0,0) and atom number 0.
*/
Atom::Atom(const QString &name, ResNum resnum, const QString &elem)
     : AtomIndex(name,resnum), Element(elem), Vector(), atmnum(0)
{}

/** Convienience constructors - these are used to allow string based creation of 
    atoms. This is particularly helpful when scripting the creation of atoms.
     
    Create an atom called 'name' in residue 0, with element guessed from 'name' and 
    coordinates coords and atom number 0.
*/
Atom::Atom(const QString &name, const Vector &coords)
     : AtomIndex(name), Element(name), Vector(coords), atmnum(0)
{}

/** Convienience constructors - these are used to allow string based creation of 
    atoms. This is particularly helpful when scripting the creation of atoms.
     
    Create an atom called 'name' in residue 0, with element guessed from 'elem' and 
    coordinates coords and atom number 0.
*/
Atom::Atom(const QString &name, const QString &elem, const Vector &coords)
     : AtomIndex(name), Element(elem), Vector(coords), atmnum(0)
{}
    
/** Convienience constructors - this allows easy changing of the name of an atom */
Atom::Atom(const QString &name, const Atom &other)
     : AtomIndex(name,other.resNum()), Element(other), Vector(other), atmnum(other.atmnum)
{}
    
/** Convienience constructors - these are used to allow string based creation of 
    atoms. This is particularly helpful when scripting the creation of atoms.
     
    Create an atom called 'name' in residue 0, with element guessed from 'name' and 
    coordinates (0,0,0) and atom number num.
*/
Atom::Atom(AtomNum num, const QString &name) : AtomIndex(name), Element(name), Vector(), atmnum(num)
{}
    
/** Convienience constructors - these are used to allow string based creation of 
    atoms. This is particularly helpful when scripting the creation of atoms.
     
    Create an atom called 'name' in residue 0, with element guessed from 'elem' and 
    coordinates (0,0,0) and atom number num.
*/
Atom::Atom(AtomNum num, const QString &name, const QString &elem)
     : AtomIndex(name), Element(elem), Vector(), atmnum(num)
{}
    
/** Convienience constructors - these are used to allow string based creation of 
    atoms. This is particularly helpful when scripting the creation of atoms.
     
    Create an atom called 'name' in residue 0, with element guessed from 'name' and 
    coordinates coords and atom number num.
*/
Atom::Atom(AtomNum num, const QString &name, const Vector &coords)
     : AtomIndex(name), Element(name), Vector(coords), atmnum(num)
{}

/** Convienience constructors - these are used to allow string based creation of 
    atoms. This is particularly helpful when scripting the creation of atoms.
     
    Create an atom called 'name' in residue 0, with element guessed from 'elem' and 
    coordinates coords and atom number num.
*/
Atom::Atom(AtomNum num, const QString &name, const QString &elem, const Vector &coords)
     : AtomIndex(name), Element(elem), Vector(coords), atmnum(num)
{}

/** Destructor */
Atom::~Atom()
{}

/** Return a string representation of the atom */
QString Atom::toString() const
{
    return QObject::tr("[%1|%2|%3]")
             .arg(AtomIndex::name(),resNum().toString()).arg(Element::symbol());
}

/** Return whether two atoms are within the sum of their bond-order radii.
    This is normally a good indication that the atoms are bonded together.
    (err is used to add an error to the sum of the bond-order radii) */
bool Atom::withinBondRadii(const Atom &atm0, const Atom &atm1, double err)
{
    double bonddist = atm0.bondOrderRadius() + atm1.bondOrderRadius() + err; 
    
    return Vector::distance2(atm0,atm1) < bonddist*bonddist;
}
