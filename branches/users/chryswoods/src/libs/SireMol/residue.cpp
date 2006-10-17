
#include "qhash_siremol.h"

#include "cgatomid.h"
#include "cgnumatomid.h"
#include "residatomid.h"
#include "resnumatomid.h"

#include "residueinfo.h"

#include "cutgroup.h"
#include "residue.h"
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
#include "SireStream/shareddatastream.h"

using namespace SireStream;
using namespace SireMol;

/** Hash a residue - this returns its residue number */
uint SIREMOL_EXPORT qHash(const Residue &res)
{
    return qHash(res.resNum());
}

static const RegisterMetaType<Residue> r_residue("SireMol::Residue");

/** Serialise a Residue to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const Residue &res)
{
    writeHeader(ds, r_residue, 1) << res.rnum;

    SharedDataStream(ds) << res.d;

    return ds;
}

/** Deserialise a Residue from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, Residue &res)
{
    VersionID v = readHeader(ds, r_residue);

    if (v == 1)
    {
        SharedDataStream sds(ds);

        sds >> res.rnum >> res.d;
    }
    else
        throw version_error(v, "1", r_residue, CODELOC);

    return ds;
}

/** Construct an empty residue */
Residue::Residue() : rnum(0)
{}

/** Construct a residue that is a copy of the residue with number 'resnum'
    in the passed molecule

    \throw SireMol::missing_residue
*/
Residue::Residue(const Molecule &molecule, ResNum resnum)
        : d(molecule.d), rnum(resnum)
{
    d->info().assertResidueExists(rnum);
}

/** Construct a residue that is a copy of the residue at index 'resid' in
    the passed molecule

    \throw SireError::invalid_index
*/
Residue::Residue(const Molecule &molecule, ResID resid)
        : d(molecule.d), rnum( d->info().residueNumber(resid) )
{}

/** Construct a residue that is a copy of the first residue called 'resname'
    in the passed molecule

    \throw SireMol::missing_residue
*/
Residue::Residue(const Molecule &molecule, const QString &resname)
        : d(molecule.d), rnum( d->info().residueNumber(resname) )
{}

/** Copy constructor - this is fast as this class is implicitly shared */
Residue::Residue(const Residue &other)
        : d(other.d), rnum(other.rnum)
{}

/** Destructor */
Residue::~Residue()
{}

///// Operators /////////////////////////////////////////

/** Comparison operator */
bool Residue::operator==(const Residue &other) const
{
    return rnum == other.rnum and
           ( d.data() == other.d.data() or *d == *(other.d) );
}

/** Comparison operator */
bool Residue::operator!=(const Residue &other) const
{
    return rnum != other.rnum or
           ( d.data() != other.d.data() and *d != *(other.d) );
}

/** Assignment operator - this is fast as this class is implicitly shared */
Residue& Residue::operator=(const Residue &other)
{
    d = other.d;
    rnum = other.rnum;
    return *this;
}

/** Return a copy of the atom at index 'i'

    \throw SireError::invalid_index
*/
Atom Residue::operator[](AtomID i) const
{
    return d->at( ResNumAtomID(rnum,i) );
}

/** Return a copy of the atom called 'atomname'

    \throw SireMol::missing_atom
*/
Atom Residue::operator[](const QString &atomname) const
{
    return d->at( AtomIndex(atomname,rnum) );
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
    return d->at( atom );
}

/** Return a copy of the CutGroup with ID == cgid - this CutGroup
    must contain atoms from this residue or an exception will be
    thrown

    \throw SireMol::missing_cutgroup
*/
CutGroup Residue::operator[](CutGroupID cgid) const
{
    info().assertSameResidue(cgid);
    return d->at( cgid );
}

/** Return a copy of the CutGroup with number 'cgnum' - this CutGroup
    must contain atoms from this residue or an exception will be
    thrown

    \throw SireMol::missing_cutgroup
*/
CutGroup Residue::operator[](CutGroupNum cgnum) const
{
    return this->operator[]( d->info().cutGroupID(cgnum) );
}

/////////////////////////////////////////////////////////


///// Interface with molecule ///////////////////////////

/** Return a copy of the Molecule that contains this Residue */
Molecule Residue::molecule() const
{
    return Molecule(*this);
}

/////////////////////////////////////////////////////////


///// Querying the residue //////////////////////////////

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
    return d->connectivity(rnum);
}

/** Return the metainfo for this residue */
const ResidueInfo& Residue::info() const
{
    return d->info().at(rnum);
}

/** Return copies of the CutGroups that contain atoms that are
    in this residue */
QHash<CutGroupID,CutGroup> Residue::cutGroups() const
{
    return d->cutGroups(rnum);
}

/** Return a copy of the CutGroup with ID == cgid

    \throw SireMol::missing_cutgroup
*/
CutGroup Residue::cutGroup(CutGroupID cgid) const
{
    info().assertSameResidue(cgid);
    return d->cutGroup(cgid);
}

/** Return a copy of the CoordGroups containing the coordinates of
    CutGroups that contain atoms from this residue */
QHash<CutGroupID,CoordGroup> Residue::coordGroups() const
{
    return d->coordGroups(rnum);
}

/** Return a copy of the CoordGroup for the CutGroup with ID == cgid

    \throw SireMol::missing_cutgroup
*/
CoordGroup Residue::coordGroup(CutGroupID cgid) const
{
    info().assertSameResidue(cgid);
    return d->coordGroup(cgid);
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
    return d->coordinates( ResNumAtomID(rnum,i) );
}

/** Return a copy of the coordinates of the atom with name 'atomname'

    \throw SireMol::missing_atom
*/
Vector Residue::coordinates(const QString &atomname) const
{
    return d->coordinates( AtomIndex(atomname,rnum) );
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
    return d->coordinates(atom);
}

/** Return an array of all of the atoms in this residue, in the same
    order as they are in the residue */
QVector<Atom> Residue::atoms() const
{
    return d->atoms(rnum);
}

/** Return an array of the coordinates of the atoms in the
    residue in the same order as they are in this residue */
QVector<Vector> Residue::coordinates() const
{
    return d->coordinates(rnum);
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

    return d->getWeight(g0, g1, weightfunc);
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

    return d->getWeight(g0, g1, weightfunc);
}

/////////////////////////////////////////////////////////


//// Moving the residue /////////////////////////////////

/** Translate the entire residue by 'delta' */
void Residue::translate(const Vector &delta)
{
    d->translate(rnum, delta);
}

/** Translate the atom at index 'atomid' by 'delta'

    \throw SireError::invalid_index
*/
void Residue::translate(AtomID atomid, const Vector &delta)
{
    d->translate(rnum, atomid, delta);
}

/** Translate the atoms with indicies in 'atomids' by 'delta'

    \throw SireError::invalid_index
*/
void Residue::translate(const QSet<AtomID> &atomids, const Vector &delta)
{
    d->translate(rnum, atomids, delta);
}

/** Translate the atom called 'atom' by 'delta'

    \throw SireMol::missing_atom
*/
void Residue::translate(const QString &atom, const Vector &delta)
{
    d->translate(AtomIndex(atom,rnum), delta);
}

/** Translate the atoms whose names are in 'atoms' by 'delta'

    \throw SireMol::missing_atom
*/
void Residue::translate(const QStringList &atoms, const Vector &delta)
{
    d->translate(rnum, atoms, delta);
}

/** Translate the atom 'atom' by delta - the residue number of 'atom'
    must be the same as this residue or else an invalid_arg exception
    will be thrown

    \throw SireError::invalid_arg
    \throw SireMol::missing_atom
*/
void Residue::translate(const AtomIndex &atom, const Vector &delta)
{
    info().assertSameResidue(atom);
    d->translate(atom, delta);
}

/** Translate the atoms 'atoms' by 'delta' - the residue numbers of
    each of the AtomIndex objects in 'atoms' must be the same as this
    residue or else an invalid_arg exception will be thrown.

    \throw SireError::invalid_arg
    \throw SireMol::missing_atom
*/
void Residue::translate(const QSet<AtomIndex> &atoms, const Vector &delta)
{
    info().assertSameResidue(atoms);
    d->translate(atoms, delta);
}

/** Rotate the whole residue using the quaternion 'quat' about the point
    'point'
*/
void Residue::rotate(const Quaternion &quat, const Vector &point)
{
    d->rotate(rnum, quat, point);
}

/** Rotate the atom with index 'atomid' using the quaternion 'quat' about
    the point 'point'

    \throw SireError::invalid_index
*/
void Residue::rotate(AtomID atomid, const Quaternion &quat, const Vector &point)
{
    d->rotate(rnum, atomid, quat, point);
}

/** Rotate the atoms with indexes in 'atomids' using the quaternion 'quat'
    about the point 'point'

    \throw SireError::invalid_index
*/
void Residue::rotate(const QSet<AtomID> &atomids, const Quaternion &quat,
                     const Vector &point)
{
    d->rotate(rnum, atomids, quat, point);
}

/** Rotate the atom called 'atomname' using the quaternion 'quat'
    about the point 'point'

    \throw SireMol::missing_atom
*/
void Residue::rotate(const QString &atomname, const Quaternion &quat,
                     const Vector &point)
{
    d->rotate(AtomIndex(atomname,rnum), quat, point);
}

/** Rotate the atoms whose names are in 'atomnames' using the quaternion 'quat'
    about the point 'point'

    \throw SireMol::missing_atom
*/
void Residue::rotate(const QStringList &atomnames, const Quaternion &quat,
                     const Vector &point)
{
    d->rotate(rnum, atomnames, quat, point);
}

/** Rotate the atom with AtomIndex 'atom' using the quaternion 'quat'
    about the point 'point'. The residue number of the AtomIndex must
    be the same as for this residue or else an invalid_arg exception
    will be thrown.

    \throw SireError::invalid_arg
    \throw SireMol::missing_atom
*/
void Residue::rotate(const AtomIndex &atom, const Quaternion &quat,
                     const Vector &point)
{
    info().assertSameResidue(atom);
    d->rotate(atom, quat, point);
}

/** Rotate the atoms whose AtomIndex objects are in 'atoms', using
    the quaternion 'quat' about the point 'point'. The residue number
    of the AtomIndex objects must all be the same as the number of
    this residue, or else an invalid_arg exception will be thrown.

    \throw SireError::invalid_arg
    \throw SireMol::missing_atom
*/
void Residue::rotate(const QSet<AtomIndex> &atoms, const Quaternion &quat,
                     const Vector &point)
{
    info().assertSameResidue(atoms);
    d->rotate(atoms, quat, point);
}

/** Rotate the whole residue using the matrix 'rotmat' about the point
    'point'
*/
void Residue::rotate(const Matrix &rotmat, const Vector &point)
{
    d->rotate(rnum, rotmat, point);
}

/** Rotate the atom with index 'atomid' using the matrix 'rotmat' about
    the point 'point'

    \throw SireError::invalid_index
*/
void Residue::rotate(AtomID atomid, const Matrix &rotmat, const Vector &point)
{
    d->rotate(rnum, atomid, rotmat, point);
}

/** Rotate the atoms with indexes in 'atomids' using the matrix 'rotmat'
    about the point 'point'

    \throw SireError::invalid_index
*/
void Residue::rotate(const QSet<AtomID> &atomids, const Matrix &rotmat,
                     const Vector &point)
{
    d->rotate(rnum, atomids, rotmat, point);
}

/** Rotate the atom called 'atomname' using the matrix 'rotmat'
    about the point 'point'

    \throw SireMol::missing_atom
*/
void Residue::rotate(const QString &atomname, const Matrix &rotmat,
                     const Vector &point)
{
    d->rotate(AtomIndex(atomname,rnum), rotmat, point);
}

/** Rotate the atoms whose names are in 'atomnames' using the matrix 'rotmat'
    about the point 'point'

    \throw SireMol::missing_atom
*/
void Residue::rotate(const QStringList &atomnames, const Matrix &rotmat,
                     const Vector &point)
{
    d->rotate(rnum, atomnames, rotmat, point);
}

/** Rotate the atom with AtomIndex 'atom' using the matrix 'rotmat'
    about the point 'point'. The residue number of the AtomIndex must
    be the same as for this residue or else an invalid_arg exception
    will be thrown.

    \throw SireError::invalid_arg
    \throw SireMol::missing_atom
*/
void Residue::rotate(const AtomIndex &atom, const Matrix &rotmat,
                     const Vector &point)
{
    info().assertSameResidue(atom);
    d->rotate(atom, rotmat, point);
}

/** Rotate the atoms whose AtomIndex objects are in 'atoms', using
    the matrix 'rotmat' about the point 'point'. The residue number
    of the AtomIndex objects must all be the same as the number of
    this residue, or else an invalid_arg exception will be thrown.

    \throw SireError::invalid_arg
    \throw SireMol::missing_atom
*/
void Residue::rotate(const QSet<AtomIndex> &atoms, const Matrix &rotmat,
                     const Vector &point)
{
    info().assertSameResidue(atoms);
    d->rotate(atoms, rotmat, point);
}

/** Set the coordinates of the residue to 'newcoords'. This must have
    the same size as the number of atoms in this residue.

    \throw SireError::incompatible_error
*/
void Residue::setCoordinates(const QVector<Vector> &newcoords)
{
    d->setCoordinates(rnum, newcoords);
}

/** Set the coordinates of the atom with index 'atomid' to 'newcoords'.

    \throw SireError::invalid_index
*/
void Residue::setCoordinates(AtomID atomid, const Vector &newcoords)
{
   d->setCoordinates(rnum, atomid, newcoords);
}

/** Set the coordinates of the atoms with the specified indicies to the
    supplied values.

    \throw SireError::invalid_index
*/
void Residue::setCoordinates(const QHash<AtomID,Vector> &newcoords)
{
    d->setCoordinates(rnum, newcoords);
}

/** Set the coordinates of the atom with name 'atomname' to 'newcoords'

    \throw SireMol::missing_atom
*/
void Residue::setCoordinates(const QString &atomname, const Vector &newcoords)
{
    d->setCoordinates( AtomIndex(atomname,rnum), newcoords );
}

/** Set the coordinates of the atoms whose names are supplied to the
    corresponding coordinates.

    \throw SireMol::missing_atom
*/
void Residue::setCoordinates(const QHash<QString,Vector> &newcoords)
{
    d->setCoordinates(rnum, newcoords);
}

/** Set the coordinates for the atom 'atom' to 'newcoords'. The residue
    number of 'atom' must be the same as for this residue or else an
    invalid_arg exception will be thrown.

    \throw SireError::invalid_arg
    \throw SireMol::missing_atom
*/
void Residue::setCoordinates(const AtomIndex &atom, const Vector &newcoords)
{
    info().assertSameResidue(atom);
    d->setCoordinates(atom, newcoords);
}

/** Set the coordinates of the specified atoms to the supplied values.
    The residue number for each of the AtomIndex objects must be equal
    to that of this residue or else an invalid_arg exception will be
    thrown.

    \throw SireError::invalid_arg
    \throw SireMol::missing_atom
*/
void Residue::setCoordinates(const QHash<AtomIndex,Vector> &newcoords)
{
    for (QHash<AtomIndex,Vector>::const_iterator it = newcoords.begin();
         it != newcoords.end();
         ++it)
    {
        info().assertSameResidue(it.key());
    }

    d->setCoordinates( newcoords );
}

/////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////

//// Internal geometry moves ////////////////////////////

/** Change the bond 'bnd', only moving the atoms in this residue. This will not
    move any other atoms in the molecule. Note that the move will ignore any
    inter-residue bonding, so the move may (badly) change interresidue bond
    lengths and angles.

    If you wish to perform a move that will not change any inter-residue bonds
    or angles, then you need to add the atoms that are involved in inter-residue
    bonds to the list of anchors. The list of atoms in inter-residue bonds
    can be obtained from the editres.connectivity().anchors() function, e.g.
    call

    Note that the bond must contain at least one atom that is in this residue
    or else an exception will be thrown

    \code
    residue.change(bnd, delta, weightfunc, residue.connectivity().anchors());
    \endcode

    \throw SireMol::anchor_error
    \throw SireMol::missing_atom
    \throw SireMol::ring_error

*/
void Residue::change(const Bond &bnd, double delta,
                     const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors)
{
    if (SireMaths::isZero(delta))
        return;

    //split the residue into two based on the two atoms of the bond
    boost::tuple<AtomIDGroup,AtomIDGroup> groups = splitResidue(bnd, connectivity());

    AtomIDGroup group0 = groups.get<0>();
    AtomIDGroup group1 = groups.get<1>();

    //tell the moldata to move the two groups by delta along the vector of the bond...
    d->change(bnd, delta, group0, group1, weightfunc, anchors);
}

/** Overloaded function that uses the default RelFromMass weight function

    \throw SireMol::anchor_error
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/
void Residue::change(const Bond &bnd, double delta, const QSet<AtomIndex> &anchors)
{
    change(bnd, delta, RelFromMass(), anchors);
}

/** Change the angle 'ang' by an angle of 'delta'.

    This has the same restrictions as Residue::change(Bond), i.e. the move will
    only move atoms in this residue

    Note that the angle must contain at least one atom that is in this residue
    or an exception will be thrown

    \throw SireMol::anchor_error
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/
void Residue::change(const SireMol::Angle &ang, const SireMaths::Angle &delta,
                     const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors)
{
    if (delta.isZero())
        return;

    //split the residue into two parts...
    boost::tuple<AtomIDGroup,AtomIDGroup> groups = splitResidue(ang, connectivity());

    AtomIDGroup group0 = groups.get<0>();
    AtomIDGroup group1 = groups.get<1>();

    //tell the moldata to move the two groups by delta about this angle
    d->change(ang, delta, group0, group1, weightfunc, anchors);
}

/** Overloaded function that uses the default RelFromMass() weight function

    \throw SireMol::anchor_error
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/
void Residue::change(const SireMol::Angle &ang, const SireMaths::Angle &delta,
                     const QSet<AtomIndex> &anchors)
{
    change(ang, delta, RelFromMass(), anchors);
}

/** Change the dihedral 'dih' by an angle of 'delta'.

    This has the same restrictions as Residue::change(Bond), i.e. the move
    will only move atoms in this residue

    Note that the dihedral must contain at least one atom that is in this
    residue or else an exception will be thrown

    \throw SireMol::anchor_error
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/
void Residue::change(const Dihedral &dih, const SireMaths::Angle &delta,
                     const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors)
{
    if (delta.isZero())
        return;

    boost::tuple<AtomIDGroup,AtomIDGroup> groups = splitResidue(dih, connectivity());

    AtomIDGroup group0 = groups.get<0>();
    AtomIDGroup group1 = groups.get<1>();

    //tell the moldata to move the two groups around the dihedral
    d->change(Bond(dih.atom1(),dih.atom2()), delta, group0, group1, weightfunc, anchors);
}

/** Overloaded function used to use the default RelFromMass() weight function

    \throw SireMol::anchor_error
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/
void Residue::change(const Dihedral &dih, const SireMaths::Angle &delta,
                     const QSet<AtomIndex> &anchors)
{
    change(dih, delta, RelFromMass(), anchors);
}

/** Overloaded function that moves all atoms of a dihedral (about the central
    bond of the dihedral)

    \throw SireMol::anchor_error
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/
void Residue::change(const Bond &dih, const SireMaths::Angle &delta,
                     const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors)
{
    if (delta.isZero())
        return;

    boost::tuple<AtomIDGroup,AtomIDGroup> groups = splitResidue(dih, connectivity());

    AtomIDGroup group0 = groups.get<0>();
    AtomIDGroup group1 = groups.get<1>();

    //tell the moldata to move the two groups around the dihedral
    d->change(dih, delta, group0, group1, weightfunc, anchors);
}

/** Overloaded function use to use the default RelFromMass() weight function

    \throw SireMol::anchor_error
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/
void Residue::change(const Bond &bnd, const SireMaths::Angle &delta, const QSet<AtomIndex> &anchors)
{
    change(bnd, delta, RelFromMass(), anchors);
}

/** Change the improper angle by an angle of 'delta'.

    This has the same restrictions as Residue::change(Bond), i.e. the move
    will only move atoms in this residue

    Note that the improper angle must contain at least one atom that is in this
    residue or else an exception will be thrown

    \throw SireMol::anchor_error
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/
void Residue::change(const Improper &improper, const SireMaths::Angle &delta,
                     const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors)
{
    if (delta.isZero())
        return;

    boost::tuple<AtomIDGroup,AtomIDGroup> groups = splitResidue(improper, connectivity());

    AtomIDGroup group0 = groups.get<0>();
    AtomIDGroup group1 = groups.get<1>();

    //tell the moldata to move the two groups around the dihedral
    d->change(improper, delta, group0, group1, weightfunc, anchors);
}

/** Overloaded function used to use the default RelFromMass() function

    \throw SireMol::anchor_error
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/
void Residue::change(const Improper &improper, const SireMaths::Angle &delta,
                     const QSet<AtomIndex> &anchors)
{
    change(improper, delta, RelFromMass(), anchors);
}

/** This is a convienience function that allows you to directly set the length of
    a bond. Note that this uses Residue::change(Bond), so has the same restrictions.

    Note that at least one of the atoms in the bond must be in this residue

    \throw SireMol::anchor_error
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/
void Residue::set(const Bond &bnd, double size,
                  const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors)
{
    change(bnd, size - this->measure(bnd), weightfunc, anchors);
}

/** Overloaded function used to use the default RelFromMass() weight function

    \throw SireMol::anchor_error
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/
void Residue::set(const Bond &bnd, double size, const QSet<AtomIndex> &anchors)
{
    set(bnd, size, RelFromMass(), anchors);
}

/** This is a convienience function that allows you to directly set the size of
    an angle. Note that this uses Residue::change(Angle), so has the same restrictions.

    Note that at least one of the atoms in the angle must be in this residue

    \throw SireMol::anchor_error
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/
void Residue::set(const SireMol::Angle &ang, const SireMaths::Angle &size,
                  const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors)
{
    change(ang, size - this->measure(ang), weightfunc, anchors);
}

/** Overloaded function used to use the default RelFromMass() weight function

    \throw SireMol::anchor_error
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/
void Residue::set(const SireMol::Angle &ang, const SireMaths::Angle &size,
                  const QSet<AtomIndex> &anchors)
{
    set(ang, size, RelFromMass(), anchors);
}

/** This is a convienience function that allows you to directly set the size of
    a dihedral. Note that this uses Residue::change(Dihedral), so has the same restrictions.

    Note that at least one of the atoms in the dihedral must be in this residue

    \throw SireMol::anchor_error
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/
void Residue::set(const Dihedral &dih, const SireMaths::Angle &size,
                  const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors)
{
    change(dih, size - this->measure(dih), weightfunc, anchors);
}

/** Overloaded function used to use the default RelFromMass() weight function

    \throw SireMol::anchor_error
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/
void Residue::set(const Dihedral &dih, const SireMaths::Angle &size, const QSet<AtomIndex> &anchors)
{
    change(dih, size - this->measure(dih), anchors);
}

/** This is a convienience function that allows you to directly set the size of
    a dihedral. Note that this uses Residue::change(Bond,SireMaths::Angle),
    so has the same restrictions.

    Note that this moves all of the atoms either side of the central bond of the dihedral

    Note that at least one of the atoms in the dihedral must be in this residue

    \throw SireMol::anchor_error
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/
void Residue::setAll(const Dihedral &dih, const SireMaths::Angle &size,
                     const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors)
{
    change(dih, size - this->measure(dih), weightfunc, anchors);
}

/** Overloaded function used to use the default RelFromMass() weight function

    \throw SireMol::anchor_error
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/
void Residue::setAll(const Dihedral &dih, const SireMaths::Angle &size,
                     const QSet<AtomIndex> &anchors)
{
    setAll(dih, size, RelFromMass(), anchors);
}

/** This is a convienience function that allows you to directly set the size of
    an improper angle. Note that this uses Residue::change(Improper),
    so has the same restrictions.

    Note that at least one of the atoms in the improper angle must be in this residue

    \throw SireMol::anchor_error
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/
void Residue::set(const Improper &improper, const SireMaths::Angle &size,
                  const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors)
{
    set(improper, size - this->measure(improper), weightfunc, anchors);
}

/** Overloaded function used to use the default RelFromMass() weight function

    \throw SireMol::anchor_error
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/
void Residue::set(const Improper &improper, const SireMaths::Angle &size,
                  const QSet<AtomIndex> &anchors)
{
    set(improper, size, RelFromMass(), anchors);
}

/////////////////////////////////////////////////////////
