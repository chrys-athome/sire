/**
  * @file
  *
  * C++ Implementation: EditRes
  *
  * Description:
  * Implementation of EditRes
  *
  * Author: Christopher Woods, (C) 2006
  *
  * Copyright: See COPYING file that comes with this distribution
  *
  */

#include "qhash_siremol.h"

#include "editres.h"

#include "cgatomid.h"
#include "cgnumatomid.h"
#include "resnumatomid.h"
#include "residatomid.h"

#include "atom.h"
#include "bond.h"
#include "angle.h"
#include "dihedral.h"
#include "improper.h"

#include "cutgroup.h"
#include "editmol.h"

#include "splitmolecule.h"
#include "atomidgroup.h"
#include "weightfunction.h"

#include "molecule.h"
#include "moleculedata.h"

#include "residueinfo.h"

#include "SireMol/errors.h"
#include "SireError/errors.h"

#include "SireVol/coordgroup.h"

#include "SireMaths/quaternion.h"
#include "SireMaths/matrix.h"
#include "SireMaths/triangle.h"
#include "SireMaths/line.h"
#include "SireMaths/torsion.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMol;
using namespace SireStream;

static const RegisterMetaType<EditRes> r_editres;

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const EditRes &editres)
{
    writeHeader(ds, r_editres, 1);

    SharedDataStream(ds) << editres.rnum << editres.d;

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, EditRes &editres)
{
    VersionID v = readHeader(ds, r_editres);

    if (v == 1)
    {
        SharedDataStream(ds) >> editres.rnum >> editres.d;
    }
    else
        throw version_error(v, "1", r_editres, CODELOC);

    return ds;
}

/** Construct an empty, null, residue */
EditRes::EditRes() : d( EditMolData::shared_null() )
{}

/** Construct an EditRes as a copy of the residue with number 'resnum'
    from the molecule 'editmol'

    \throw SireMol::missing_residue
*/
EditRes::EditRes(const EditMol &editmol, ResNum resnum)
        : d( editmol.d ), rnum(resnum)
{
    d->assertResidueExists(resnum);
}

/** Construct an EditRes as a copy of the residue with index 'resid'
    from the molecule 'editmol'

    \throw SireError::invalid_index
*/
EditRes::EditRes(const EditMol &editmol, ResID resid)
        : d( editmol.d ), rnum( editmol.d->at(resid) )
{}

/** Construct an EditRes as a copy of the first residue in 'editmol'
    that is called 'resname'

    \throw SireMol::missing_residue
*/
EditRes::EditRes(const EditMol &editmol, const QString &resname)
        : d( editmol.d ), rnum( editmol.residueNumber(resname) )
{}

/** Internal constructor used to construct from a shared pointer and
    residue number */
EditRes::EditRes(const QSharedDataPointer<EditMolData> &ptr, ResNum resnum)
        : d(ptr), rnum(resnum)
{
    d->assertResidueExists(resnum);
}

/** Copy constructor */
EditRes::EditRes(const EditRes &other)
        : d(other.d), rnum(other.rnum)
{}

/** Destructor */
EditRes::~EditRes()
{}

/** Comparison operator */
bool EditRes::operator==(const EditRes &other) const
{
    return rnum == other.rnum and ( d == other.d or
                                    *d == *(other.d) );
}

/** Comparison operator */
bool EditRes::operator!=(const EditRes &other) const
{
    return rnum != other.rnum or ( d != other.d and
                                   *d != *(other.d) );
}

/** Assignment operator */
EditRes& EditRes::operator=(const EditRes &other)
{
    d = other.d;
    rnum = other.rnum;

    return *this;
}

/** Assert that the atom 'atom' is in this residue.

    \throw SireError::invalid_arg
*/
void EditRes::assertSameResidue(const AtomIndex &atom) const
{
    if (atom.resNum() != rnum)
        throw SireError::invalid_arg( QObject::tr(
                "The atom %1 does not refer to this residue - %2(%3)")
                    .arg(atom.toString(), name()).arg(number()), CODELOC );
}

/** Assert that all of the atoms in 'atoms' are in this residue

    \throw SireError::invalid_arg
*/
void EditRes::assertSameResidue(const QSet<AtomIndex> &atoms) const
{
    for (QSet<AtomIndex>::const_iterator it = atoms.begin();
         it != atoms.end();
         ++it)
    {
        this->assertSameResidue(*it);
    }
}

/** Assert that this residue contains atoms that are in the
    CutGroup with number 'cgnum'

    \throw SireError::invalid_arg
*/
void EditRes::assertSameResidue(CutGroupNum cgnum) const
{
    if (not d->cutGroupNums(rnum).contains(cgnum))
        throw SireError::invalid_arg( QObject::tr(
                "The residue %1(%2) does not contain any atoms that are "
                "in the CutGroup with number %3")
                    .arg(name()).arg(number()).arg(cgnum), CODELOC );
}

/** Assert that this residue contains atoms that are in the
    CutGroup with ID == cgid

    \throw SireError::invalid_arg
*/
void EditRes::assertSameResidue(CutGroupID cgid) const
{
    if ( cgid < d->nCutGroups() and not
            ( d->cutGroupNums(rnum).contains(d->at(cgid)) ) )
    {
        throw SireError::invalid_arg( QObject::tr(
                "The residue %1(%2) does not contain any atoms that are "
                "in the CutGroup with ID %3")
                    .arg(name()).arg(number()).arg(cgid), CODELOC );
    }
}

/** Assert that both of the atoms in the bond 'bond' refer
    to this residue

    \throw SireError::invalid_arg
*/
void EditRes::assertSameResidue(const Bond &bond) const
{
    if (bond.resNum0() != rnum or bond.resNum1() != rnum)
    {
        throw SireError::invalid_arg( QObject::tr(
                "The bond %1 does not refer to this residue - %2(%3)")
                    .arg(bond.toString(), name()).arg(number()), CODELOC );
    }
}

/** Return a copy of the atom at index 'atomid'

    \throw SireError::invalid_index
*/
Atom EditRes::operator[](AtomID atomid) const
{
    return d->atom( d->at(ResNumAtomID(rnum,atomid)) );
}

/** Return a copy of the atom with name 'nam'

    \throw SireMol::missing_atom
*/
Atom EditRes::operator[](const QString &nam) const
{
    return d->atom( d->at(AtomIndex(nam,rnum)) );
}

/** Return a copy of the atom with AtomIndex 'atom'

    \throw SireError::invalid_arg
    \throw SireMol::missing_atom
*/
Atom EditRes::operator[](const AtomIndex &atom) const
{
    this->assertSameResidue(atom);
    return d->atom(atom);
}

/** Return a copy of the CutGroup with number 'cgnum'

    \throw SireError::invalid_arg
    \throw SireMol::missing_cutgroup
*/
CutGroup EditRes::operator[](CutGroupNum cgnum) const
{
    this->assertSameResidue(cgnum);
    return d->cutGroup(cgnum);
}

/** Return a copy of the CutGroup with ID cgid

    \throw SireError::invalid_arg
    \throw SireMol::missing_cutgroup
*/
CutGroup EditRes::operator[](CutGroupID cgid) const
{
    this->assertSameResidue(cgid);
    CutGroupNum cgnum = d->at(cgid);

    return d->cutGroup(cgnum);
}

/** Return a copy of the molecule that contains this residue */
EditMol EditRes::molecule() const
{
    return EditMol(*this);
}

/** Return a copy of the atom at index 'atomid'

    \throw SireError::invalid_index
*/
Atom EditRes::at(AtomID atomid) const
{
    return this->operator[](atomid);
}

/** Return a copy of the atom called 'atomname'

    \throw SireMol::missing_atom
*/
Atom EditRes::at(const QString &atomname) const
{
    return this->operator[](atomname);
}

/** Return a copy of the atom 'atom'

    \throw SireError::invalid_arg
    \throw SireMol::missing_atom
*/
Atom EditRes::at(const AtomIndex &atom) const
{
    return this->operator[](atom);
}

/** Return a copy of the CutGroup with number 'cgnum'

    \throw SireError::invalid_arg
    \throw SireMol::missing_cutgroup
*/
CutGroup EditRes::at(CutGroupID cgid) const
{
    return this->operator[](cgid);
}

/** Return a copy of the CutGroup with number 'cgnum'

    \throw SireError::invalid_arg
    \throw SireMol::missing_cutgroup
*/
CutGroup EditRes::at(CutGroupNum cgnum) const
{
    return this->operator[](cgnum);
}

/** Return the connectivity of this residue */
ResidueBonds EditRes::connectivity() const
{
    return d->connectivity(rnum);
}

/** Return the info for this residue */
ResidueInfo EditRes::info() const
{
    return d->info().at(rnum);
}

/** Return copies of the CutGroups that contain atoms from this
    residue, indexed by CutGroupID */
QHash<CutGroupID,CutGroup> EditRes::cutGroupsByID() const
{
    QSet<CutGroupNum> cgnums = d->cutGroupNums(rnum);

    QHash<CutGroupID,CutGroup> cgroups;
    cgroups.reserve(cgnums.count());

    foreach (CutGroupNum cgnum, cgnums)
    {
        cgroups.insert( d->cutGroupID(cgnum), d->cutGroup(cgnum) );
    }

    return cgroups;
}

/** Return copies of the CutGroups that contain atoms from this
    residue, indexed by CutGroupNum */
QHash<CutGroupNum,CutGroup> EditRes::cutGroupsByNum() const
{
    return d->cutGroups(rnum);
}

/** Return a copy of the CutGroup with index 'cgid' - note that
    this CutGroup must contain atoms from this residue.

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_arg
*/
CutGroup EditRes::cutGroup(CutGroupID cgid) const
{
    return this->operator[](cgid);
}

/** Return a copy of the CutGroup with number 'cgnum' - note
    that this CutGroup must contain atoms from this residue

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_arg
*/
CutGroup EditRes::cutGroup(CutGroupNum cgnum) const
{
    return this->operator[](cgnum);
}

/** Return copies of the CoordGroups for the CutGroups that contain
    atoms from this residue, indexed by CutGroupID */
QHash<CutGroupID,CoordGroup> EditRes::coordGroupsByID() const
{
    QSet<CutGroupNum> cgnums = d->cutGroupNums(rnum);

    QHash<CutGroupID,CoordGroup> cgroups;
    cgroups.reserve(cgnums.count());

    foreach (CutGroupNum cgnum, cgnums)
    {
        cgroups.insert( d->cutGroupID(cgnum), d->coordGroup(cgnum) );
    }

    return cgroups;
}

/** Return copies of the CoordGroups for the CutGroups that contain
    atoms from this residue, indexed by CutGroupNum */
QHash<CutGroupNum,CoordGroup> EditRes::coordGroupsByNum() const
{
    return d->coordGroups(rnum);
}

/** Return a copy of the CoordGroup for the CutGroup with
    ID == cgid

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_arg
*/
CoordGroup EditRes::coordGroup(CutGroupID cgid) const
{
    this->assertSameResidue(cgid);

    return d->coordGroup( d->at(cgid) );
}

/** Return a copy of the CoordGroup for the CutGroup with
    number 'cgnum'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_arg
*/
CoordGroup EditRes::coordGroup(CutGroupNum cgnum) const
{
    this->assertSameResidue(cgnum);
    return d->coordGroup(cgnum);
}

/** Return a copy of the atom at index 'atomid'

    \throw SireError::invalid_index
*/
Atom EditRes::atom(AtomID atomid) const
{
    return this->operator[](atomid);
}

/** Return a copy of the atom with name 'atomname'

    \throw SireMol::missing_atom
*/
Atom EditRes::atom(const QString &atomname) const
{
    return this->operator[](atomname);
}

/** Return a copy of the atom 'atom'

    \throw SireError::invalid_index
    \throw SireMol::missing_atom
*/
Atom EditRes::atom(const AtomIndex &atom) const
{
    return this->operator[](atom);
}

/** Return a copy of the coordinates of the atom at index 'atomid'

    \throw SireError::invalid_index
*/
Vector EditRes::coordinates(AtomID atomid) const
{
    return d->coordinates( d->at(ResNumAtomID(rnum,atomid)) );
}

/** Return a copy of the coordinates of the atom with
    name 'atomname'

    \throw SireMol::missing_atom
*/
Vector EditRes::coordinates(const QString &atomname) const
{
    return d->coordinates( AtomIndex(atomname,rnum) );
}

/** Return a copy of the coordinates of the atom 'atom'

    \throw SireError::invalid_index
    \throw SireMol::missing_atom
*/
Vector EditRes::coordinates(const AtomIndex &atom) const
{
    this->assertSameResidue(atom);
    return d->coordinates(atom);
}

/** Return copies of all of the atoms in this residue */
QVector<Atom> EditRes::atoms() const
{
    return d->atoms(rnum);
}

/** Return copies of the coordinates for all of the atoms
    in this residue. */
QVector<Vector> EditRes::coordinates() const
{
    return d->coordinates(rnum);
}

template<class T>
QHash<T,Atom> getAtoms(const EditRes &editres, const QSet<T> &idxs)
{
    QHash<T,Atom> atoms;
    atoms.reserve(idxs.count());

    for (typename QSet<T>::const_iterator it = idxs.begin();
         it != idxs.end();
         ++it)
    {
        atoms.insert( *it, editres.atom(*it) );
    }

    return atoms;
}

/** Return copies of the atoms whose indicies are in 'atomids'

    \throw SireError::invalid_index
*/
QHash<AtomID,Atom> EditRes::atoms(const QSet<AtomID> &atomids) const
{
    return getAtoms<AtomID>(*this, atomids);
}

template<class T>
QHash<T,Vector> getCoords(const EditRes &editres, const QSet<T> &idxs)
{
    QHash<T,Vector> coords;
    coords.reserve(idxs.count());

    for (typename QSet<T>::const_iterator it = idxs.begin();
         it != idxs.end();
         ++it)
    {
        coords.insert( *it, editres.coordinates(*it) );
    }

    return coords;
}

/** Return copies of the coordinates for the atoms whose indicies are
    in 'atomids'

    \throw SireError::invalid_index
*/
QHash<AtomID,Vector> EditRes::coordinates(const QSet<AtomID> &atomids) const
{
    return getCoords<AtomID>(*this, atomids);
}

/** Return copies of the atoms whose names are in 'atomnames'

    \throw SireMol::missing_atom
*/
QHash<QString,Atom> EditRes::atoms(const QSet<QString> &atomnames) const
{
    return getAtoms<QString>(*this, atomnames);
}

/** Return copies of the coordinates of the atoms whose names
    are in 'atomnames'

    \throw SireMol::missing_atom
*/
QHash<QString,Vector> EditRes::coordinates(const QSet<QString> &atomnames) const
{
    return getCoords<QString>(*this, atomnames);
}

/** Return copies of the atoms whose AtomIndex objects are in 'atms'

    \throw SireError::invalid_arg
    \throw SireMol::missing_atom
*/
QHash<AtomIndex,Atom> EditRes::atoms(const QSet<AtomIndex> &atms) const
{
    return getAtoms<AtomIndex>(*this, atms);
}

/** Return copies of the coordinates of the atoms whose AtomIndex objects
    are in 'atms'

    \throw SireError::invalid_arg
    \throw SireMol::missing_atom
*/
QHash<AtomIndex,Vector> EditRes::coordinates(const QSet<AtomIndex> &atms) const
{
    return getCoords<AtomIndex>(*this, atms);
}

/** Return the name of this residue */
QString EditRes::name() const
{
    return d->residueName(rnum);
}

/** Return the name of this residue */
QString EditRes::resName() const
{
    return this->name();
}

/** Return the number of this residue */
ResNum EditRes::number() const
{
    return rnum;
}

/** Return the number of this residue */
ResNum EditRes::resNum() const
{
    return this->number();
}

/** Return whether this residue is empty (has no atoms) */
bool EditRes::isEmpty() const
{
    return d->isEmpty(rnum);
}

/** Return whether this residue contains atoms that
    are in the CutGroup with index 'cgid' */
bool EditRes::contains(CutGroupID cgid) const
{
    if (d->contains(cgid))
    {
        return this->contains( d->at(cgid) );
    }
    else
        return false;
}

/** Return whether this residue contains atoms that
    are in the CutGroup with number 'cgnum' */
bool EditRes::contains(CutGroupNum cgnum) const
{
    return d->contains(cgnum) and d->cutGroupNums(rnum).contains(cgnum);
}

/** Return whether this residue contains at atom at index 'atomid' */
bool EditRes::contains(AtomID atomid) const
{
    return d->contains( ResNumAtomID(rnum,atomid) );
}

/** Return whether this residue contains an atom called 'atomname' */
bool EditRes::contains(const QString &atomname) const
{
    return d->contains( AtomIndex(atomname,rnum) );
}

/** Return whether this residue contains the atom 'atom' */
bool EditRes::contains(const AtomIndex &atom) const
{
    return atom.resNum() == rnum and d->contains(atom);
}

/** Return whether or not this residue contains the bond 'bond' */
bool EditRes::contains(const Bond &bond) const
{
    return connectivity().contains(bond);
}

/** Return the total number of atoms in this residue */
int EditRes::nAtoms() const
{
    return d->nAtoms(rnum);
}

/** Return the total number of atoms *from this residue*
    that are in the CutGroup with ID 'cgid'

    \throw SireError::invalid_index
    \throw SireMol::missing_cutgroup
*/
int EditRes::nAtoms(CutGroupID cgid) const
{
    this->assertSameResidue(cgid);

    return this->nAtoms( d->at(cgid) );
}

/** Return the total number of atoms *from this residue*
    that are in the CutGroup with number 'cgnum'

    \throw SireError::invalid_arg
    \throw SireMol::missing_cutgroup
*/
int EditRes::nAtoms(CutGroupNum cgnum) const
{
    this->assertSameResidue(cgnum);
    return d->nAtoms(cgnum, rnum);
}

/** Return the total number of CutGroups that contain
    atoms from this residue */
int EditRes::nCutGroups() const
{
    return d->nCutGroups(rnum);
}

/** Return the total number of bonds in this residue */
int EditRes::nBonds() const
{
    return connectivity().nBonds();
}

/** Return the total number of intra-residue bonds in this
    residue */
int EditRes::nIntraBonds() const
{
    return connectivity().nIntraBonds();
}

/** Return the total number of inter-residue bonds in this
    residue */
int EditRes::nInterBonds() const
{
    return connectivity().nInterBonds();
}

/** Return the names of the atoms in this molecule */
QStringList EditRes::atomNames() const
{
    return d->atomNames(rnum);
}

/** Internal function used to return copies of the residues whose
    numbers are in 'resnums'

    \throw SireMol::missing_residue
*/
QHash<ResNum,EditRes> EditRes::getResidues(const QSet<ResNum> &resnums) const
{
    QHash<ResNum,EditRes> residues;
    residues.reserve(resnums.count());

    foreach (ResNum resnum, resnums)
    {
        residues.insert( resnum, EditRes(d,resnum) );
    }

    return residues;
}

/** Return copies of the residues that are bonded to this residue */
QHash<ResNum,EditRes> EditRes::bondedResidues() const
{
    return this->getResidues( connectivity().bondedResidues() );
}

/** Return copies of the residues that are bonded to the atom at index
    'atomid'

    \throw SireError::invalid_index
*/
QHash<ResNum,EditRes> EditRes::residuesBondedTo(AtomID atom) const
{
    return getResidues( connectivity().residuesBondedTo( d->at(atom).name() ) );
}

/** Return copies of the residues that are bonded to the atom with name
    'atomname'

    \throw SireMol::missing_atom
*/
QHash<ResNum,EditRes> EditRes::residuesBondedTo(const QString &atomname) const
{
    d->assertAtomExists( AtomIndex(atomname,rnum) );
    return getResidues( connectivity().residuesBondedTo(atomname) );
}

/** Return copies of the residues that are bonded to the atom 'atom'

    \throw SireError::invalid_arg
    \throw SireMol::missing_atom
*/
QHash<ResNum,EditRes> EditRes::residuesBondedTo(const AtomIndex &atom) const
{
    this->assertSameResidue(atom);
    return getResidues( connectivity().residuesBondedTo(atom.name()) );
}

/** Return the geometric line representig the bond 'bnd'.
    All atoms in the bond must be in this residue.

    \throw SireError::invalid_arg
    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
SireMaths::Line EditRes::bond(const Bond &bnd) const
{
    return SireMaths::Line( coordinates(bnd.atom0()),
                            coordinates(bnd.atom1()) );
}

/** Return the geometric triangle representig the angle 'ang'.
    All atoms in the angle must be in this residue.

    \throw SireError::invalid_arg
    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
SireMaths::Triangle EditRes::angle(const SireMol::Angle &ang) const
{
    return SireMaths::Triangle( coordinates(ang.atom0()),
                                coordinates(ang.atom1()),
                                coordinates(ang.atom2()) );
}

/** Return the geometric torsion representig the dihedral 'dih'.
    All atoms in the dihedral must be in this residue.

    \throw SireError::invalid_arg
    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
SireMaths::Torsion EditRes::dihedral(const Dihedral &dih) const
{
    return SireMaths::Torsion( coordinates(dih.atom0()),
                               coordinates(dih.atom1()),
                               coordinates(dih.atom2()),
                               coordinates(dih.atom3()) );
}

/** Return the geometric torsion representig the improper angle 'ang'.
    All atoms in the improper angle must be in this residue.

    \throw SireError::invalid_arg
    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
SireMaths::Torsion EditRes::improper(const Improper &imp) const
{
    return SireMaths::Torsion( coordinates(imp.atom0()),
                               coordinates(imp.atom1()),
                               coordinates(imp.atom2()),
                               coordinates(imp.atom3()) );
}

/** Return the length of the bond 'bnd'

    All atoms in the bond must be in this residue.

    \throw SireError::invalid_arg
    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
double EditRes::measure(const Bond &bnd) const
{
    return bond(bnd).length();
}

/** Return the size of the angle 'ang'

    All atoms in the angle must be in this residue.

    \throw SireError::invalid_arg
    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
SireMaths::Angle EditRes::measure(const SireMol::Angle &ang) const
{
    return angle(ang).angle();
}

/** Return the size of the dihedral 'dih'

    All atoms in the dihedral must be in this residue.

    \throw SireError::invalid_arg
    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
SireMaths::Angle EditRes::measure(const Dihedral &dih) const
{
    return dihedral(dih).angle();
}

/** Return the size of the improper angle 'imp'

    All atoms in the improper angle must be in this residue.

    \throw SireError::invalid_arg
    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
SireMaths::Angle EditRes::measure(const Improper &improper) const
{
    throw SireError::incomplete_code( QObject::tr("Need to write this!"), CODELOC );
    return 0.0;
}

/** Return the relative weights of the two groups, 'group0' and 'group1'
    using 'weightfunc' to calculate the weights.

    \throw SireMol::missing_atom
*/
double EditRes::getWeight(const QStringList &group0, const QStringList &group1,
                          const WeightFunction &weightfunc) const
{
    return d->getWeight( rnum, group0, group1, weightfunc );
}

/** Return the relative weights of the two groups, 'group0' and 'group1'
    using 'weightfunc' to calculate the weights.

    \throw SireError::invalid_arg
    \throw SireMol::missing_atom
*/
double EditRes::getWeight(const QSet<AtomIndex> &group0, const QSet<AtomIndex> &group1,
                          const WeightFunction &weightfunc) const
{
    this->assertSameResidue(group0);
    this->assertSameResidue(group1);

    return d->getWeight( AtomIDGroup(group0), AtomIDGroup(group1), weightfunc );
}

/** Set the name of this residue to 'name' */
void EditRes::setName(QString name)
{
    d->setResidueName(rnum, name);
}

/** Set the number of this residue to 'newnum' - note that there must
    not be an existing residue with this number in this molecule

    \throw SireMol::duplicate_residue
*/
void EditRes::setNumber(ResNum newnum)
{
    d->renumberResidue(rnum, newnum);
    rnum = newnum;
}

/** Add an atom called 'atm' to this residue.
    No atom with this name must exist in this residue.

    \throw SireMol::duplicate_atom
*/
void EditRes::add(const QString &atm)
{
    d->add( Atom(atm,rnum) );
}

/** Add the atom 'atm' to this residue. No atom with
    the same name must exist in this residue.

    \throw SireMol::duplicate_atom
*/
void EditRes::add(const Atom &atm)
{
    d->add( Atom(atm,rnum) );
}

/** Add the atom called 'atm' to this residue, and place it
    into the CutGroup with number 'cgnum'
    No atom with this name must exist in this residue.

    \throw SireMol::duplicate_atom
*/
void EditRes::add(const QString &atm, CutGroupNum cgnum)
{
    d->add( Atom(atm,rnum), cgnum );
}

/** Add the atom 'atm' to this residue, and place it
    into the CutGroup with number 'cgnum'

    No atom with this name must exist in this residue.

    \throw SireMol::duplicate_atom
*/
void EditRes::add(const Atom &atm, CutGroupNum cgnum)
{
    d->add( Atom(atm,rnum), cgnum );
}

/** Remove the atom called 'atm' from this residue

    \throw SireMol::missing_atom
*/
void EditRes::remove(const QString &atm)
{
    d->remove( AtomIndex(atm,rnum) );
}

/** Remove the atom 'atm' from this residue

    \throw SireError::invalid_arg
    \throw SireMol::missing_atom
*/
void EditRes::remove(const AtomIndex &atm)
{
    this->assertSameResidue(atm);
    d->remove(atm);
}

/** Add the bond between the atoms called 'atm0' and 'atm1'

    \throw SireMol::missing_atom
*/
void EditRes::addBond(const QString &atm0, const QString &atm1)
{
    d->add( Bond(AtomIndex(atm0,rnum),AtomIndex(atm1,rnum)) );
}

/** Remove the bond between the atoms called 'atm0' and 'atm1'

    \throw SireMol::missing_atom
*/
void EditRes::removeBond(const QString &atm0, const QString &atm1)
{
    d->remove( Bond(AtomIndex(atm0,rnum),AtomIndex(atm1,rnum)) );
}

/** Add the bond 'bond'

    \throw SireError::invalid_arg
    \throw SireMol::missing_atom
*/
void EditRes::add(const Bond &bond)
{
    this->assertSameResidue(bond);
    d->add(bond);
}

/** Remove the bond 'bond'

    \throw SireError::invalid_arg
    \throw SireMol::missing_atom
*/
void EditRes::remove(const Bond &bond)
{
    this->assertSameResidue(bond);
    d->remove(bond);
}

/** Automatically add all intra-residue bonds within this residue
    using the vdw radii of the atoms. */
void EditRes::addAutoBonds()
{
    d->addAutoBonds(rnum);
}

/** Automatically add all of the intra-residue bonds within this residue
    using the bond function 'bondfunc' */
void EditRes::addAutoBonds(const BondAddingFunction &bondfunc)
{
    d->addAutoBonds(rnum, bondfunc);
}

/** Remove all bonds that involve the atom called 'atomname'

    \throw SireMol::missing_atom
*/
void EditRes::removeAllBonds(const QString &atomname)
{
    d->removeAllBonds( AtomIndex(atomname,rnum) );
}

/** Remove all bonds from this residue */
void EditRes::removeAllBonds()
{
    d->removeAllBonds(rnum);
}

/** Apply the template 'tmpl' using the template function 'tmplfunc' */
void EditRes::applyTemplate(const EditRes &tmpl, const TemplateFunction &tmplfunc)
{
    #warning EditRes::applyTemplate() is not written!
}

/** Translate all of the atoms in this residue by 'delta' */
void EditRes::translate(const Vector &delta)
{
    d->translate(delta);
}

/** Translate the atom at index 'atomid' by 'delta'

    \throw SireError::invalid_index
*/
void EditRes::translate(AtomID atomid, const Vector &delta)
{
    d->translate( d->at(ResNumAtomID(rnum,atomid)), delta );
}

/** Translate all of the atoms whose indicies are in 'atomids' by 'delta' */
void EditRes::translate(const QSet<AtomID> &atomids, const Vector &delta)
{
    d->translate( rnum, atomids, delta );
}

/** Translate the atom called 'atom' by 'delta'

    \throw SireMol::missing_atom
*/
void EditRes::translate(const QString &atom, const Vector &delta)
{
    d->translate( AtomIndex(atom,rnum), delta );
}

/** Translate the atoms whose names are in 'atoms' by 'delta'

    \throw SireMol::missing_atom
*/
void EditRes::translate(const QStringList &atoms, const Vector &delta)
{
    d->translate( rnum, atoms, delta );
}

/** Translate the atom 'atom' by 'delta'

    \throw SireError::invalid_arg
    \throw SireMol::missing_atom
*/
void EditRes::translate(const AtomIndex &atom, const Vector &delta)
{
    this->assertSameResidue(atom);
    d->translate(atom, delta);
}

/** Translate the atoms in 'atoms' by 'delta'

    \throw SireError::invalid_arg
    \throw SireMol::missing_atom
*/
void EditRes::translate(const QSet<AtomIndex> &atoms, const Vector &delta)
{
    this->assertSameResidue(atoms);
    d->translate(atoms, delta);
}

/** Rotate the entire residue using the quaternion 'quat' about the point 'point' */
void EditRes::rotate(const Quaternion &quat, const Vector &point)
{
    d->rotate(rnum, quat, point);
}

/** Rotate the atom at index 'atomid' using the quaternion 'quat' about the point 'point'

    \throw SireError::invalid_index
*/
void EditRes::rotate(AtomID atomid, const Quaternion &quat, const Vector &point)
{
    d->rotate( d->at(ResNumAtomID(rnum,atomid)), quat, point );
}

/** Rotate all of the atoms whose indicies are in 'atomids'
    using the quaternion 'quat' about the point 'point'

    \throw SireError::invalid_index
*/
void EditRes::rotate(const QSet<AtomID> &atomids, const Quaternion &quat,
                     const Vector &point)
{
    d->rotate(rnum, atomids, quat, point);
}

/** Rotate the atom called 'atom' using the quaternion 'quat' about the point 'point'

    \throw SireMol::missing_atom
*/
void EditRes::rotate(const QString &atom, const Quaternion &quat, const Vector &point)
{
    d->rotate( AtomIndex(atom,rnum), quat, point );
}

/** Rotate all of the atoms whose names are in 'atoms'
    using the quaternion 'quat' about the point 'point'

    \throw SireMol::missing_atom
*/
void EditRes::rotate(const QStringList &atoms, const Quaternion &quat,
                     const Vector &point)
{
    d->rotate( rnum, atoms, quat, point );
}

/** Rotate the atom 'atom' using the quaternion 'quat' about the point 'point'

    \throw SireError::invalid_arg
    \throw SireMol::missing_atom
*/
void EditRes::rotate(const AtomIndex &atom, const Quaternion &quat,
                     const Vector &point)
{
    this->assertSameResidue(atom);
    d->rotate(atom, quat, point);
}

/** Rotate the atoms in 'atoms' using the quaternion 'quat'
    about the point 'point'

    \throw SireError::invalid_arg
    \throw SireMol::missing_atom
*/
void EditRes::rotate(const QSet<AtomIndex> &atoms, const Quaternion &quat,
                     const Vector &point)
{
    this->assertSameResidue(atoms);
    d->rotate(atoms, quat, point);
}

/** Rotate the entire residue using the matrix 'matrix' about the point 'point' */
void EditRes::rotate(const Matrix &matrix, const Vector &point)
{
    d->rotate(rnum, matrix, point);
}

/** Rotate the atom at index 'atomid' using the matrix 'matrix' about the point 'point'

    \throw SireError::invalid_index
*/
void EditRes::rotate(AtomID atomid, const Matrix &matrix, const Vector &point)
{
    d->rotate( d->at(ResNumAtomID(rnum,atomid)), matrix, point );
}

/** Rotate all of the atoms whose indicies are in 'atomids'
    using the matrix 'matrix' about the point 'point'

    \throw SireError::invalid_index
*/
void EditRes::rotate(const QSet<AtomID> &atomids, const Matrix &matrix,
                     const Vector &point)
{
    d->rotate(rnum, atomids, matrix, point);
}

/** Rotate the atom called 'atom' using the matrix 'matrix' about the point 'point'

    \throw SireMol::missing_atom
*/
void EditRes::rotate(const QString &atom, const Matrix &matrix, const Vector &point)
{
    d->rotate( AtomIndex(atom,rnum), matrix, point );
}

/** Rotate all of the atoms whose names are in 'atoms'
    using the matrix 'matrix' about the point 'point'

    \throw SireMol::missing_atom
*/
void EditRes::rotate(const QStringList &atoms, const Matrix &matrix,
                     const Vector &point)
{
    d->rotate( rnum, atoms, matrix, point );
}

/** Rotate the atom 'atom' using the matrix 'matrix' about the point 'point'

    \throw SireError::invalid_arg
    \throw SireMol::missing_atom
*/
void EditRes::rotate(const AtomIndex &atom, const Matrix &matrix,
                     const Vector &point)
{
    this->assertSameResidue(atom);
    d->rotate(atom, matrix, point);
}

/** Rotate the atoms in 'atoms' using the matrix 'matrix'
    about the point 'point'

    \throw SireError::invalid_arg
    \throw SireMol::missing_atom
*/
void EditRes::rotate(const QSet<AtomIndex> &atoms, const Matrix &matrix,
                     const Vector &point)
{
    this->assertSameResidue(atoms);
    d->rotate(atoms, matrix, point);
}

/** Set the coordinates of all of the atoms in the residue to
    'newcoords'

    \throw SireError::incompatible_error
*/
void EditRes::setCoordinates(const QVector<Vector> &newcoords)
{
    d->setCoordinates(rnum, newcoords);
}

/** Set the coordinates of the atom at index 'atomid' to
    'newcoords'

    \throw SireError::invalid_index
*/
void EditRes::setCoordinates(AtomID atomid, const Vector &newcoords)
{
    d->setCoordinates( d->at(ResNumAtomID(rnum,atomid)), newcoords );
}

/** Set the coordinates of the specified atoms to the values
    in 'newcoords'

    \throw SireError::invalid_index
*/
void EditRes::setCoordinates(const QHash<AtomID,Vector> &newcoords)
{
    d->setCoordinates( rnum, newcoords );
}

/** Set the coordinates of the atom called 'atomname' to 'newcoords'

    \throw SireMol::missing_atom
*/
void EditRes::setCoordinates(const QString &atomname, const Vector &newcoords)
{
    d->setCoordinates( AtomIndex(atomname,rnum), newcoords );
}

/** Set the coordinates of the specified atoms to 'newcoords'

    \throw SireMol::missing_atom
*/
void EditRes::setCoordinates(const QHash<QString,Vector> &newcoords)
{
    d->setCoordinates( rnum, newcoords );
}

/** Set the coordinates of the atom 'atom' to 'newcoords'

    \throw SireError::invalid_arg
    \throw SireMol::missing_atom
*/
void EditRes::setCoordinates(const AtomIndex &atom, const Vector &newcoords)
{
    this->assertSameResidue(atom);
    d->setCoordinates(atom, newcoords);
}

/** Set the coordinates of the specified atoms to 'newcoords'

    \throw SireError::invalid_arg
    \throw SireMol::missing_atom
*/
void EditRes::setCoordinates(const QHash<AtomIndex,Vector> &newcoords)
{
    for (QHash<AtomIndex,Vector>::const_iterator it = newcoords.begin();
         it != newcoords.end();
         ++it)
    {
        this->assertSameResidue(it.key());
    }

    d->setCoordinates( newcoords );
}

/** Change the bond 'bnd', only moving the atoms in this residue. This will not
    move any other atoms in the molecule. Note that the move will ignore any
    inter-residue bonding, so the move may (badly) change interresidue bond
    lengths and angles.

    If you wish to perform a move that will not change any inter-residue bonds
    or angles, then you need to add the atoms that are involved in inter-residue
    bonds to the list of anchors. The list of atoms in inter-residue bonds
    can be obtained from the editres.connectivity().anchors() function, e.g.
    call

    \code
    editres.change(bnd, delta, weightfunc, editres.connectivity().anchors());
    \endcode

    Note that the bond must contain at least one atom that is in this residue
    or else an exception will be thrown

    \throw SireMol::anchor_error
    \throw SireMol::missing_atom
    \throw SireMol::ring_error

*/
void EditRes::change(const Bond &bnd, double delta,
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
void EditRes::change(const Bond &bnd, double delta, const QSet<AtomIndex> &anchors)
{
    change(bnd, delta, RelFromMass(), anchors);
}

/** Change the angle 'ang' by an angle of 'delta'.

    This has the same restrictions as EditRes::change(Bond), i.e. the move will
    only move atoms in this residue

    Note that the angle must contain at least one atom that is in this residue
    or an exception will be thrown

    \throw SireMol::anchor_error
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/
void EditRes::change(const SireMol::Angle &ang, const SireMaths::Angle &delta,
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
void EditRes::change(const SireMol::Angle &ang, const SireMaths::Angle &delta,
                     const QSet<AtomIndex> &anchors)
{
    change(ang, delta, RelFromMass(), anchors);
}

/** Change the dihedral 'dih' by an angle of 'delta'.

    This has the same restrictions as EditRes::change(Bond), i.e. the move
    will only move atoms in this residue

    Note that the dihedral must contain at least one atom that is in this
    residue or else an exception will be thrown

    \throw SireMol::anchor_error
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/
void EditRes::change(const Dihedral &dih, const SireMaths::Angle &delta,
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
void EditRes::change(const Dihedral &dih, const SireMaths::Angle &delta,
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
void EditRes::change(const Bond &dih, const SireMaths::Angle &delta,
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
void EditRes::change(const Bond &bnd, const SireMaths::Angle &delta, const QSet<AtomIndex> &anchors)
{
    change(bnd, delta, RelFromMass(), anchors);
}

/** Change the improper angle by an angle of 'delta'.

    This has the same restrictions as EditRes::change(Bond), i.e. the move
    will only move atoms in this residue

    Note that the improper angle must contain at least one atom that is in this
    residue or else an exception will be thrown

    \throw SireMol::anchor_error
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/
void EditRes::change(const Improper &improper, const SireMaths::Angle &delta,
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
void EditRes::change(const Improper &improper, const SireMaths::Angle &delta,
                     const QSet<AtomIndex> &anchors)
{
    change(improper, delta, RelFromMass(), anchors);
}

/** This is a convienience function that allows you to directly set the length of
    a bond. Note that this uses EditRes::change(Bond), so has the same restrictions.

    Note that at least one of the atoms in the bond must be in this residue

    \throw SireMol::anchor_error
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/
void EditRes::set(const Bond &bnd, double size,
                  const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors)
{
    change(bnd, size-d->measure(bnd), weightfunc, anchors);
}

/** Overloaded function used to use the default RelFromMass() weight function

    \throw SireMol::anchor_error
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/
void EditRes::set(const Bond &bnd, double size, const QSet<AtomIndex> &anchors)
{
    set(bnd, size, RelFromMass(), anchors);
}

/** This is a convienience function that allows you to directly set the size of
    an angle. Note that this uses EditRes::change(Angle), so has the same restrictions.

    Note that at least one of the atoms in the angle must be in this residue

    \throw SireMol::anchor_error
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/
void EditRes::set(const SireMol::Angle &ang, const SireMaths::Angle &size,
                  const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors)
{
    change(ang, size-d->measure(ang), weightfunc, anchors);
}

/** Overloaded function used to use the default RelFromMass() weight function

    \throw SireMol::anchor_error
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/
void EditRes::set(const SireMol::Angle &ang, const SireMaths::Angle &size,
                  const QSet<AtomIndex> &anchors)
{
    set(ang, size, RelFromMass(), anchors);
}

/** This is a convienience function that allows you to directly set the size of
    a dihedral. Note that this uses EditRes::change(Dihedral), so has the same restrictions.

    Note that at least one of the atoms in the dihedral must be in this residue

    \throw SireMol::anchor_error
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/
void EditRes::set(const Dihedral &dih, const SireMaths::Angle &size,
                  const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors)
{
    change(dih, size-d->measure(dih), weightfunc, anchors);
}

/** Overloaded function used to use the default RelFromMass() weight function

    \throw SireMol::anchor_error
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/
void EditRes::set(const Dihedral &dih, const SireMaths::Angle &size, const QSet<AtomIndex> &anchors)
{
    set(dih, size, RelFromMass(), anchors);
}

/** This is a convienience function that allows you to directly set the size of
    a dihedral. Note that this uses EditRes::change(Bond,SireMaths::Angle),
    so has the same restrictions.

    Note that this moves all of the atoms either side of the central bond of the dihedral

    Note that at least one of the atoms in the dihedral must be in this residue

    \throw SireMol::anchor_error
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/
void EditRes::setAll(const Dihedral &dih, const SireMaths::Angle &size,
                     const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors)
{
    change(dih, size - d->measure(dih), weightfunc, anchors);
}

/** Overloaded function used to use the default RelFromMass() weight function

    \throw SireMol::anchor_error
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/
void EditRes::setAll(const Dihedral &dih, const SireMaths::Angle &size,
                     const QSet<AtomIndex> &anchors)
{
    change(dih, size - d->measure(dih), anchors);
}

/** This is a convienience function that allows you to directly set the size of
    an improper angle. Note that this uses EditRes::change(Improper),
    so has the same restrictions.

    Note that at least one of the atoms in the improper angle must be in this residue

    \throw SireMol::anchor_error
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/
void EditRes::set(const Improper &improper, const SireMaths::Angle &size,
                  const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors)
{
    change(improper, size-d->measure(improper), weightfunc, anchors);
}

/** Overloaded function used to use the default RelFromMass() weight function

    \throw SireMol::anchor_error
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/
void EditRes::set(const Improper &improper, const SireMaths::Angle &size,
                  const QSet<AtomIndex> &anchors)
{
    set(improper, size, RelFromMass(), anchors);
}
