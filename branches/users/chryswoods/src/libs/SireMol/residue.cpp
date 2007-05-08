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

#include "cgatomid.h"
#include "cgnumatomid.h"
#include "residatomid.h"
#include "resnumatomid.h"
#include "idmolatom.h"

#include "residueinfo.h"

#include "newatom.h"
#include "cutgroup.h"
#include "residue.h"
#include "moleculeview_inlines.h"
#include "molecule.h"
#include "resnum.h"
#include "resid.h"
#include "bond.h"
#include "angle.h"
#include "dihedral.h"
#include "improper.h"
#include "atomidgroup.h"
#include "splitmolecule.h"
#include "weightfunction.h"
#include "residuebonds.h"

#include "SireError/errors.h"
#include "SireMol/errors.h"

#include "SireMaths/angle.h"
#include "SireMaths/quaternion.h"
#include "SireMaths/matrix.h"
#include "SireMaths/line.h"
#include "SireMaths/triangle.h"
#include "SireMaths/torsion.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireMol;

/** Hash a residue - this returns its residue number */
uint SIREMOL_EXPORT qHash(const Residue &res)
{
    return qHash(res.resNum());
}

static const RegisterMetaType<Residue> r_residue;

/** Serialise a Residue to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const Residue &res)
{
    writeHeader(ds, r_residue, 1)
            << res.rnum
            << static_cast<const MoleculeView&>(res);

    return ds;
}

/** Deserialise a Residue from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, Residue &res)
{
    VersionID v = readHeader(ds, r_residue);

    if (v == 1)
    {
        ds >> res.rnum
           >> static_cast<MoleculeView&>(res);
    }
    else
        throw version_error(v, "1", r_residue, CODELOC);

    return ds;
}

/** Construct an empty residue */
Residue::Residue() : MoleculeView(), rnum(0)
{}

/** Construct a residue that is a copy of the residue with number 'resnum'
    in the passed molecule

    \throw SireMol::missing_residue
*/
Residue::Residue(const Molecule &molecule, ResNum resnum)
        : MoleculeView(molecule), rnum(resnum)
{
    constData().info().assertResidueExists(rnum);
}

/** Construct a residue that is a copy of the residue at index 'resid' in
    the passed molecule

    \throw SireError::invalid_index
*/
Residue::Residue(const Molecule &molecule, ResID resid)
        : MoleculeView(molecule),
          rnum( molecule.info().residueNumber(resid) )
{}

/** Construct a residue that is a copy of the first residue called 'resname'
    in the passed molecule

    \throw SireMol::missing_residue
*/
Residue::Residue(const Molecule &molecule, const QString &resname)
        : MoleculeView(molecule),
          rnum( molecule.info().residueNumber(resname) )
{}

/** Construct a residue that is a copy of the residue that contains the
    atom 'atom' */
Residue::Residue(const NewAtom &atom)
        : MoleculeView(atom),
          rnum(atom.info().resNum())
{}

/** Copy constructor - this is fast as this class is implicitly shared */
Residue::Residue(const Residue &other)
        : MoleculeView(other), rnum(other.rnum)
{}

/** Destructor */
Residue::~Residue()
{}

///// Operators /////////////////////////////////////////

/** Comparison operator */
bool Residue::operator==(const Residue &other) const
{
    return rnum == other.rnum and
           MoleculeView::operator==(other);
}

/** Comparison operator */
bool Residue::operator!=(const Residue &other) const
{
    return rnum != other.rnum or
           MoleculeView::operator!=(other);
}

/** Assignment operator - this is fast as this class is implicitly shared */
Residue& Residue::operator=(const Residue &other)
{
    MoleculeView::operator=(other);
    rnum = other.rnum;
    return *this;
}

/** Return a copy of the atom at index 'i'

    \throw SireError::invalid_index
*/
Atom Residue::operator[](AtomID i) const
{
    return data().at( ResNumAtomID(rnum,i) );
}

/** Return a copy of the atom called 'atomname'

    \throw SireMol::missing_atom
*/
Atom Residue::operator[](const QString &atomname) const
{
    return data().at( AtomIndex(atomname,rnum) );
}

/** Return a copy of the atom with index 'atom' - this will throw
    an invalid_arg exception if atom.resNum() is not the number of
    this residue

    \throw SireError::invalid_arg
    \throw SireMol::missing_residue
*/
Atom Residue::operator[](const AtomIndex &atom) const
{
    info().assertSameResidue(atom);
    return data().at( atom );
}

/** Return a copy of the CutGroup with ID == cgid - this CutGroup
    must contain atoms from this residue or an exception will be
    thrown

    \throw SireMol::missing_cutgroup
*/
CutGroup Residue::operator[](CutGroupID cgid) const
{
    info().assertSameResidue(cgid);
    return data().at( cgid );
}

/** Return a copy of the CutGroup with number 'cgnum' - this CutGroup
    must contain atoms from this residue or an exception will be
    thrown

    \throw SireMol::missing_cutgroup
*/
CutGroup Residue::operator[](CutGroupNum cgnum) const
{
    return this->operator[]( data().info().cutGroupID(cgnum) );
}

/////////////////////////////////////////////////////////


///// Querying the residue //////////////////////////////

/** Return the ID number of the molecule that contains this residue */
MoleculeID Residue::ID() const
{
    return data().ID();
}

/** Return the version number of the molecule that contains this residue */
const Version& Residue::version() const
{
    return data().version();
}

/** Return a string identifying this residue */
QString Residue::idString() const
{
    return QObject::tr("%1 (%2) in \"%3\" (%4 %5)")
                          .arg(name()).arg(number())
                          .arg(data().info().name()).arg(ID())
                          .arg(data().version().toString());
}

/** Return a copy of the atom at index 'i'

    \throw SireError::invalid_index
*/
Atom Residue::at(AtomID i) const
{
    return this->operator[](i);
}

/** Return a copy of the atom called 'atomname'

    \throw SireMol::missing_atom
*/
Atom Residue::at(const QString &atomname) const
{
    return this->operator[](atomname);
}

/** Return a copy of the atom with index 'atom' - this will throw
    an invalid_arg exception if atom.resNum() is not the number of
    this residue

    \throw SireError::invalid_arg
    \throw SireMol::missing_residue
*/
Atom Residue::at(const AtomIndex &atom) const
{
    return this->operator[](atom);
}

/** Return a copy of the CutGroup with ID == cgid - this CutGroup
    must contain atoms from this residue or an exception will be
    thrown

    \throw SireMol::missing_cutgroup
*/
CutGroup Residue::at(CutGroupID cgid) const
{
    return this->operator[](cgid);
}

/** Return a copy of the CutGroup with number 'cgnum' - this CutGroup
    must contain atoms from this residue or an exception will be
    thrown

    \throw SireMol::missing_cutgroup
*/
CutGroup Residue::at(CutGroupNum cgnum) const
{
    return this->operator[](cgnum);
}

/** Return the connectivity of this residue */
ResidueBonds Residue::connectivity() const
{
    return data().connectivity(rnum);
}

/** Return the metainfo for this residue */
const ResidueInfo& Residue::info() const
{
    return data().info().at(rnum);
}

/** Return copies of the CutGroups that contain atoms that are
    in this residue */
QHash<CutGroupID,CutGroup> Residue::cutGroups() const
{
    return data().cutGroups(rnum);
}

/** Return a copy of the CutGroup with ID == cgid

    \throw SireMol::missing_cutgroup
*/
CutGroup Residue::cutGroup(CutGroupID cgid) const
{
    info().assertSameResidue(cgid);
    return data().cutGroup(cgid);
}

/** Return a copy of the CoordGroups containing the coordinates of
    CutGroups that contain atoms from this residue */
QHash<CutGroupID,CoordGroup> Residue::coordGroups() const
{
    return data().coordGroups(rnum);
}

/** Return a copy of the CoordGroup for the CutGroup with ID == cgid

    \throw SireMol::missing_cutgroup
*/
CoordGroup Residue::coordGroup(CutGroupID cgid) const
{
    info().assertSameResidue(cgid);
    return data().coordGroup(cgid);
}

/** Return a copy of the atom at index 'i'

    \throw SireError::invalid_index
*/
Atom Residue::atom(AtomID i) const
{
    return this->operator[](i);
}

/** Return a copy of the atom with name 'atomname'

    \throw SireMol::missing_atom
*/
Atom Residue::atom(const QString &atomname) const
{
    return this->operator[](atomname);
}

/** Return a copy of the atom with index 'atom' - this will throw
    an invalid_arg exception if atom.resNum() is not the number of
    this residue

    \throw SireError::invalid_arg
    \throw SireMol::missing_residue
*/
Atom Residue::atom(const AtomIndex &atom) const
{
    return this->operator[](atom);
}

/** Return a copy of the coordinates of the atom at index 'i'

    \throw SireError::invalid_index
*/
Vector Residue::coordinates(AtomID i) const
{
    return data().coordinates( ResNumAtomID(rnum,i) );
}

/** Return a copy of the coordinates of the atom with name 'atomname'

    \throw SireMol::missing_atom
*/
Vector Residue::coordinates(const QString &atomname) const
{
    return data().coordinates( AtomIndex(atomname,rnum) );
}

/** Return a copy of the coordinates of the atom with AtomIndex 'atom'.
    - this will throw an invalid_arg exception if atom.resNum() is not
    the number of this residue

    \throw SireError::invalid_arg
    \throw SireMol::missing_residue
*/
Vector Residue::coordinates(const AtomIndex &atom) const
{
    info().assertSameResidue(atom);
    return data().coordinates(atom);
}

/** Return an array of all of the atoms in this residue, in the same
    order as they are in the residue */
QVector<Atom> Residue::atoms() const
{
    return data().atoms(rnum);
}

/** Return an array of the coordinates of the atoms in the
    residue in the same order as they are in this residue */
QVector<Vector> Residue::coordinates() const
{
    return data().coordinates(rnum);
}

template<class T>
QHash<T,Atom> getAtoms(const Residue &res, const QSet<T> &idxs)
{
    QHash<T,Atom> atms;
    atms.reserve(idxs.count());

    for (typename QSet<T>::const_iterator it = idxs.begin();
         it != idxs.end();
         ++it)
    {
        atms.insert( *it, res.atom(*it) );
    }

    return atms;
}

template<class T>
QHash<T,Vector> getCoords(const Residue &res, const QSet<T> &idxs)
{
    QHash<T,Vector> coords;
    coords.reserve(idxs.count());

    for (typename QSet<T>::const_iterator it = idxs.begin();
         it != idxs.end();
         ++it)
    {
        coords.insert( *it, res.coordinates(*it) );
    }

    return coords;
}

/** Return copies of the atoms whose indicies are in 'atomids'
    in a hash indexed by AtomID

    \throw SireError::invalid_index
*/
QHash<AtomID,Atom> Residue::atoms(const QSet<AtomID> &atomids) const
{
    return getAtoms<AtomID>(*this, atomids);
}

/** Return copies of the coordinates of the atoms whose indicies are in
    'atomids', in a hash indexed by AtomID

    \throw SireError::invalid_index
*/
QHash<AtomID,Vector> Residue::coordinates(const QSet<AtomID> &atomids) const
{
    return getCoords<AtomID>(*this, atomids);
}

/** Return copies of all of the atoms in the residue that have their name
    in 'atomnames', in a hash indexed by atom name

    \throw SireMol::missing_atom
*/
QHash<QString,Atom> Residue::atoms(const QSet<QString> &atomnames) const
{
    return getAtoms<QString>(*this, atomnames);
}

/** Return copies of the coordinates of the atoms whose names are in
    'atomnames', in a hash indexed by atom name

    \throw SireMol::missing_atom
*/
QHash<QString,Vector> Residue::coordinates(const QSet<QString> &atomnames) const
{
    return getCoords<QString>(*this, atomnames);
}

/** Return copies of the atoms that have an AtomIndex in 'atms' - each
    of the AtomIndexes must refer to this residue or else an
    invalid_arg exception will be thrown. This returns a hash indexed
    by AtomIndex

    \throw SireError::invalid_arg
    \throw SireMol::missing_atom
*/
QHash<AtomIndex,Atom> Residue::atoms(const QSet<AtomIndex> &atms) const
{
    return getAtoms<AtomIndex>(*this, atms);
}

/** Return copies of the coordinates of atoms that have an AtomIndex in
    'atms' - each of the AtomIndexes must refer to this residue or else
    an invalid_arg exception will be thrown. This returns a hash indexed
    by AtomIndex.

    \throw SireError::invalid_arg
    \throw SireMol::missing_atom
*/
QHash<AtomIndex,Vector> Residue::coordinates(const QSet<AtomIndex> &atms) const
{
    return getCoords<AtomIndex>(*this, atms);
}

/** Return the name of this residue */
QString Residue::name() const
{
    return info().name();
}

/** Return the name of this residue */
QString Residue::resName() const
{
    return info().name();
}

/** Return this residue's number */
ResNum Residue::number() const
{
    return rnum;
}

/** Return this residue's number */
ResNum Residue::resNum() const
{
    return rnum;
}

/** Return whether or not this residue is empty (has no atoms) */
bool Residue::isEmpty() const
{
    return info().isEmpty();
}

/** Return whether or not this residue contains atoms that are in the
    CutGroup with ID == 'cgid' */
bool Residue::contains(CutGroupID cgid) const
{
    return info().contains(cgid);
}

/** Return whether or not this residue contains an atom with
    index 'atomid' */
bool Residue::contains(AtomID atomid) const
{
    return info().contains(atomid);
}

/** Return whether or not this residue contains an atom called
    'atomname' */
bool Residue::contains(const QString &atomname) const
{
    return info().contains(atomname);
}

/** Return whether or not this residue contains an atom with
    AtomIndex 'atom' */
bool Residue::contains(const AtomIndex &atom) const
{
    return atom.resNum() == rnum and this->contains(atom.name());
}

/** Return whether or not this residue contains the bond 'bond' */
bool Residue::contains(const Bond &bond) const
{
    return connectivity().contains(bond);
}

/** Return the total number of atoms in this residue */
int Residue::nAtoms() const
{
    return info().nAtoms();
}

/** Return the total number of atoms in the CutGroup with ID == cgid
    that are also in this residue

    \throw SireMol::missing_cutgroup
*/
int Residue::nAtoms(CutGroupID cgid) const
{
    return info().nAtoms(cgid);
}

/** Return the number of CutGroups that have atoms from this residue */
int Residue::nCutGroups() const
{
    return info().nCutGroups();
}

/** Return the number of bonds that involve atoms from this residue */
int Residue::nBonds() const
{
    return connectivity().nBonds();
}

/** Return the total number of intra-residue bonds in this residue */
int Residue::nIntraBonds() const
{
    return connectivity().nIntraBonds();
}

/** Return the total number of inter-residue bonds that involve
    atoms in this residue */
int Residue::nInterBonds() const
{
    return connectivity().nInterBonds();
}

/** Return a list of the names of all atoms in this residue, in the
    order of the atoms in this residue */
QStringList Residue::atomNames() const
{
    return info().atomNames();
}

/** Return copies of the Residues bonded to this residue, in a hash
    indexed by residue number */
QHash<ResNum,Residue> Residue::bondedResidues() const
{
    //get the residue numbers of all residues bonded to this one
    QSet<ResNum> resnums = connectivity().bondedResidues();

    //return the residues associated with these numbers
    return molecule().residues(resnums);
}

/** Return copies of the Residues that are bonded to the atom with
    name 'atomname'

    \throw SireMol::missing_atom
*/
QHash<ResNum,Residue> Residue::residuesBondedTo(const QString &atomname) const
{
    info().assertAtomExists(atomname);

    //get the residue numbers of all residues bonded to this atom
    QSet<ResNum> resnums = connectivity().residuesBondedTo(atomname);

    //return the residues associated with these numbers
    return molecule().residues(resnums);
}

/** Return copies of the Residues that are bonded to the atom with
    AtomIndex 'atom' - this AtomIndex must have the same residue number
    as this residue, else an invalid_arg exception will be thrown

    \throw SireError::invalid_arg
    \throw SireMol::missing_atom
*/
QHash<ResNum,Residue> Residue::residuesBondedTo(const AtomIndex &atom) const
{
    info().assertSameResidue(atom);

    return residuesBondedTo(atom.name());
}

/** Return copies of the Residues that are bonded to the atom with
    index 'atomid'

    \throw SireError::invalid_index
*/
QHash<ResNum,Residue> Residue::residuesBondedTo(AtomID atomid) const
{
    return residuesBondedTo( info().atomName(atomid) );
}

/** Return the geometric line representing the bond 'bnd' - both atoms
    in the bond must be in this residue.

    \throw SireError::invalid_arg
    \throw SireMol::missing_atom
*/
SireMaths::Line Residue::bond(const Bond &bnd) const
{
    return SireMaths::Line( this->coordinates(bnd.atom0()),
                            this->coordinates(bnd.atom1()) );
}

/** Return the geometric triangle representing the angle 'ang' - all
    atoms in the angle must be in this residue

    \throw SireError::invalid_arg
    \throw SireMol::missing_atom
*/
SireMaths::Triangle Residue::angle(const SireMol::Angle &ang) const
{
    return SireMaths::Triangle( this->coordinates(ang.atom0()),
                                this->coordinates(ang.atom1()),
                                this->coordinates(ang.atom2()) );
}

/** Return the geometric torsion representing the dihedral 'dih' - all
    atoms in the dihedral must be in this residue

    \throw SireError::invalid_arg
    \throw SireMol::missing_atom
*/
SireMaths::Torsion Residue::dihedral(const Dihedral &dih) const
{
    return SireMaths::Torsion( this->coordinates(dih.atom0()),
                               this->coordinates(dih.atom1()),
                               this->coordinates(dih.atom2()),
                               this->coordinates(dih.atom3()) );
}

/** Return the geometric torsion representing the improper angle 'imp' -
    all atoms in the improper must be in this residue

    \throw SireError::invalid_arg
    \throw SireMol::missing_atom
*/
SireMaths::Torsion Residue::improper(const Improper &imp) const
{
    return SireMaths::Torsion( this->coordinates(imp.atom0()),
                               this->coordinates(imp.atom1()),
                               this->coordinates(imp.atom2()),
                               this->coordinates(imp.atom3()) );
}

/** Return the length of the bond 'bnd' - both atoms in the bond must
    be in this residue

    \throw SireError::invalid_arg
    \throw SireMol::missing_atom
*/
double Residue::measure(const Bond &bnd) const
{
    return this->bond(bnd).length();
}

/** Return the size of the angle 'ang' - all atoms in the angle must be
    in this residue

    \throw SireError::invalid_arg
    \throw SireMol::missing_atom
*/
SireMaths::Angle Residue::measure(const SireMol::Angle &ang) const
{
    return this->angle(ang).angle();
}

/** Return the size of the dihedral 'dih' - all atoms in the dihedral
    must be in this residue

    \throw SireError::invalid_arg
    \throw SireMol::missing_atom
*/
SireMaths::Angle Residue::measure(const Dihedral &dih) const
{
    return this->dihedral(dih).angle();
}

/** Return the size of the improper angle 'imp' - all atoms in the improper
    must be in this residue

    \throw SireError::invalid_arg
    \throw SireMol::missing_atom
*/
SireMaths::Angle Residue::measure(const Improper &imp) const
{
    throw SireError::incomplete_code("Need to write this!", CODELOC);

    return 0.0;
}

/** Return the relative weights of group0 and group1 using the weight function
    'weightfunc' and listing atoms by their name

    \throw SireMol::missing_atom
*/
double Residue::getWeight(const QStringList &group0, const QStringList &group1,
                          const WeightFunction &weightfunc) const
{
    AtomIDGroup g0;
    AtomIDGroup g1;

    foreach (QString atom, group0)
        g0.add( AtomIndex(atom,rnum) );

    foreach (QString atom, group1)
        g1.add( AtomIndex(atom,rnum) );

    return data().getWeight(g0, g1, weightfunc);
}

/** Return the relative weights of group0 and group1 using the weight function
    'weightfunc'. Each of the atoms in the two groups must have the same residue
    number as this residue or an invalid_arg exception will be thrown.

    \throw SireError::invalid_arg
    \throw SireMol::missing_atom
*/
double Residue::getWeight(const QSet<AtomIndex> &group0, const QSet<AtomIndex> &group1,
                          const WeightFunction &weightfunc) const
{
    AtomIDGroup g0;
    AtomIDGroup g1;

    foreach (AtomIndex atom, group0)
    {
        info().assertSameResidue(atom);
        g0.add(atom);
    }

    foreach (AtomIndex atom, group1)
    {
        info().assertSameResidue(atom);
        g1.add(atom);
    }

    return data().getWeight(g0, g1, weightfunc);
}

/////////////////////////////////////////////////////////
