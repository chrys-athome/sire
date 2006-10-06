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

#include "editres.h"

#include "SireStream/datastream.h"

static const RegisterMetaType<EditRes> r_editres("SireMol::EditRes");

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const EditRes &editres)
{
    writeHeader(ds, r_editres, 1) << editres.rnum << editres.d;

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, EditRes &editres)
{
    VersionID v = readHeader(ds, r_editres);

    if (v == 1)
    {
        ds >> editres.rnum >> editres.d;
    }
    else
        throw version_error(v, "1", , CODELOC);

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
    return this->operator[]( d->at(cgid) );
}

/** Return a copy of the molecule that contains this residue */
EditMol EditRes::molecule() const
{
    return EditMol(*this);
}


//// Query functions ///////////////////////////
Atom EditRes::at(AtomID i) const;
Atom EditRes::at(const QString &atomname) const;
Atom EditRes::at(const AtomIndex &atom) const;

CutGroup EditRes::at(CutGroupID cgid) const;
CutGroup EditRes::at(CutGroupNum cgnum) const;

ResidueBonds EditRes::connectivity() const;

ResidueInfo EditRes::info() const;

QHash<CutGroupID,CutGroup> EditRes::cutGroupsByID() const;
QHash<CutGroupNum,CutGroup> EditRes::cutGroupsByNum() const;

CutGroup EditRes::cutGroup(CutGroupID cgid) const;
CutGroup EditRes::cutGroup(CutGroupNum cgnum) const;

QHash<CutGroupID,CoordGroup> EditRes::coordGroupsByID() const;
QHash<CutGroupNum,CoordGroup> EditRes::coordGroupsByNum() const;

CoordGroup EditRes::coordGroup(CutGroupID cgid) const;
CoordGroup EditRes::coordGroup(CutGroupNum cgnum) const;

Atom EditRes::atom(AtomID i) const;
Atom EditRes::atom(const QString &atomname) const;
Atom EditRes::atom(const AtomIndex &atom) const;

Vector EditRes::coordinates(AtomID i) const;
Vector EditRes::coordinates(const QString &atomname) const;
Vector EditRes::coordinates(const AtomIndex &atom) const;

QVector<Atom> EditRes::atoms() const;
QVector<Vector> EditRes::coordinates() const;

QHash<AtomID,Atom> EditRes::atoms(const QSet<AtomID> &atomids) const;
QHash<AtomID,Vector> EditRes::coordinates(const QSet<AtomID> &atomids) const;

QHash<QString,Atom> EditRes::atoms(const QSet<QString> &atomnames) const;
QHash<QString,Vector> EditRes::coordinates(const QSet<QString> &atomnames) const;

QHash<AtomIndex,Atom> EditRes::atoms(const QSet<AtomIndex> &atms) const;
QHash<AtomIndex,Vector> EditRes::coordinates(const QSet<AtomIndex> &atms) const;

QString EditRes::name() const;
QString EditRes::resName() const;

ResNum EditRes::number() const;
ResNum EditRes::resNum() const;

bool EditRes::isEmpty() const;

bool EditRes::contains(CutGroupID cgid) const;
bool EditRes::contains(CutGroupNum cgnum) const;
bool EditRes::contains(AtomID atomid) const;
bool EditRes::contains(const QString &atomname) const;
bool EditRes::contains(const AtomIndex &atom) const;

bool EditRes::contains(const Bond &bond) const;

int EditRes::nAtoms() const;
int EditRes::nAtoms(CutGroupID cgid) const;
int EditRes::nAtoms(CutGroupNum cgnum) const;

int EditRes::nCutGroups() const;

int EditRes::nBonds() const;
int EditRes::nIntraBonds() const;
int EditRes::nInterBonds() const;

QStringList EditRes::atomNames() const;

QHash<ResNum,Residue> EditRes::bondedResidues() const;
QHash<ResNum,Residue> EditRes::residuesBondedTo(AtomID atom) const;
QHash<ResNum,Residue> EditRes::residuesBondedTo(const QString &atomname) const;
QHash<ResNum,Residue> EditRes::residuesBondedTo(const AtomIndex &atom) const;

SireMaths::Line EditRes::bond(const Bond &bnd) const;
SireMaths::Triangle EditRes::angle(const SireMol::Angle &ang) const;
SireMaths::Torsion EditRes::dihedral(const Dihedral &dih) const;
SireMaths::Torsion EditRes::improper(const Improper &improper) const;

double EditRes::measure(const Bond &bnd) const;
SireMaths::Angle EditRes::measure(const SireMol::Angle &ang) const;
SireMaths::Angle EditRes::measure(const Dihedral &dih) const;
SireMaths::Angle EditRes::measure(const Improper &improper) const;

double EditRes::getWeight(const QStringList &group0, const QStringList &group1,
                 const WeightFunction &weightfunc) const;

double EditRes::getWeight(const QSet<AtomIndex> &group0, const QSet<AtomIndex> &group1,
                 const WeightFunction &weightfunc) const;

double EditRes::getWeight(const QStringList &group0, const QStringList &group1,
                 const WeightFunction &weightfunc) const;

void EditRes::setName(QString name);
void EditRes::setNumber(ResNum newnum);

void EditRes::clear();

void EditRes::add(const QString &atm);
void EditRes::add(const Atom &atm);

void EditRes::add(const QString &atm, CutGroupNum cgnum);
void EditRes::add(const Atom &atm, CutGroupNum cgnum);

void EditRes::remove(const QString &atm);
void EditRes::remove(const AtomIndex &atm);

void EditRes::addBond(const QString &atmnam0, const QString &atmnam1);
void EditRes::removeBond(const QString &atmnam0, const QString &atmnam1);

void EditRes::add(const Bond &bond);
void EditRes::remove(const Bond &bond);

void EditRes::addAutoBonds();
void EditRes::addAutoBonds(const BondAddingFunction &bondfunc);

void EditRes::removeAllBonds(const QString &atomname);
void EditRes::removeAllBonds();

void EditRes::applyTemplate(const EditRes &tmpl, const TemplateFunction &tmplfunc);

void EditRes::translate(const Vector &delta);
void EditRes::translate(AtomID atomid, const Vector &delta);
void EditRes::translate(const QSet<AtomID> &atomids, const Vector &delta);
void EditRes::translate(const QString &atom, const Vector &delta);
void EditRes::translate(const QStringList &atoms, const Vector &delta);
void EditRes::translate(const AtomIndex &atom, const Vector &delta);
void EditRes::translate(const QSet<AtomIndex> &atoms, const Vector &delta);

void EditRes::rotate(const Quaternion &quat, const Vector &point);
void EditRes::rotate(AtomID atomid, const Quaternion &quat, const Vector &point);
void EditRes::rotate(const QSet<AtomID> &atomids, const Quaternion &quat, const Vector &point);
void EditRes::rotate(const QString &atom, const Quaternion &quat, const Vector &point);
void EditRes::rotate(const QStringList &atoms, const Quaternion &quat, const Vector &point);
void EditRes::rotate(const AtomIndex &atom, const Quaternion &quat, const Vector &point);
void EditRes::rotate(const QSet<AtomIndex> &atoms, const Quaternion &quat, const Vector &point);

void EditRes::rotate(const Matrix &rotmat, const Vector &point);
void EditRes::rotate(AtomID atomid, const Matrix &rotmat, const Vector &point);
void EditRes::rotate(const QSet<AtomID> &atomids, const Matrix &rotmat, const Vector &point);
void EditRes::rotate(const QString &atom, const Matrix &rotmat, const Vector &point);
void EditRes::rotate(const QStringList &atoms, const Matrix &rotmat, const Vector &point);
void EditRes::rotate(const AtomIndex &atom, const Matrix &rotmat, const Vector &point);
void EditRes::rotate(const QSet<AtomIndex> &atoms, const Matrix &rotmat, const Vector &point);

void EditRes::setCoordinates(const QVector<Vector> &newcoords);

void EditRes::setCoordinates(AtomID atomid, const Vector &newcoords);
void EditRes::setCoordinates(const QHash<AtomID,Vector> &newcoords);

void EditRes::setCoordinates(const QString &atomname, const Vector &newcoords);
void EditRes::setCoordinates(const QHash<QString,Vector> &newcoords);

void EditRes::setCoordinates(const AtomIndex &atom, const Vector &newcoords);
void EditRes::setCoordinates(const QHash<AtomIndex,Vector> &newcoords);

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
