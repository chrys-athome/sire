
#include "qhash_siremol.h"

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
    this->assertSameResidue(atom);
    return d->at( atom );
}

/** Return a copy of the CutGroup with ID == cgid - this CutGroup 
    must contain atoms from this residue or an exception will be 
    thrown
    
    \throw SireMol::missing_cutgroup
*/
CutGroup Residue::operator[](CutGroupID cgid) const
{
    this->assertSameResidue(cgid);
    return d->at( cgid );
}

/////////////////////////////////////////////////////////


///// Interface with molecule ///////////////////////////

/** Return a copy of the Molecule that contains this Residue */
Molecule Residue::molecule()
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
    this->assertSameResidue(cgid);
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
    this->assertSameResidue(cgid);
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
    this->assertSameResidue(atom.resNum());
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
    return getAtoms<AtomID>(atomids);
}

/** Return copies of the coordinates of the atoms whose indicies are in 
    'atomids', in a hash indexed by AtomID 
    
    \throw SireError::invalid_index
*/
QHash<AtomID,Vector> Residue::coordinates(const QSet<AtomID> &atomids) const
{
    return getCoords<AtomID>(atomids);
}

QHash<QString,Atom> Residue::atoms(const QSet<QString> &atomnames) const;
QHash<QString,Vector> Residue::coordinates(const QSet<QString> &atomnames) const;

QHash<AtomIndex,Atom> Residue::atoms(const QSet<AtomIndex> &atms) const;
QHash<AtomIndex,Vector> Residue::coordinates(const QSet<AtomIndex> &atms) const;

QString Residue::name() const;
QString Residue::resName() const;

ResNum Residue::number() const;
ResNum Residue::resNum() const;

bool Residue::isEmpty() const;

bool Residue::contains(CutGroupID cgid) const;
bool Residue::contains(AtomID atomid) const;
bool Residue::contains(const QString &atomname) const;
bool Residue::contains(const AtomIndex &atom) const;

bool Residue::contains(const Bond &bond) const;

int Residue::nAtoms() const;
int Residue::nAtoms(CutGroupID cgid) const;

int Residue::nCutGroups() const;

int Residue::nBonds() const;
int Residue::nIntraBonds() const;
int Residue::nInterBonds() const;

QStringList Residue::atomNames() const;

QHash<ResNum,Residue> Residue::bondedResidues() const;

SireMaths::Line Residue::bond(const Bond &bnd) const;
SireMaths::Triangle Residue::angle(const SireMol::Angle &ang) const;
SireMaths::Torsion Residue::dihedral(const Dihedral &dih) const;
SireMaths::Torsion Residue::improper(const Improper &improper) const;

double Residue::measure(const Bond &bnd) const;
SireMaths::Angle Residue::measure(const SireMol::Angle &ang) const;
SireMaths::Angle Residue::measure(const Dihedral &dih) const;
SireMaths::Angle Residue::measure(const Improper &improper) const;

double Residue::getWeight(const QStringList &group0, const QStringList &group1,
                 const WeightFunction &weightfunc) const;

double Residue::getWeight(const QSet<AtomIndex> &group0, const QSet<AtomIndex> &group1,
                 const WeightFunction &weightfunc) const;
/////////////////////////////////////////////////////////


//// Moving the residue /////////////////////////////////
void Residue::translate(const Vector &delta);
void Residue::translate(AtomID atomid, const Vector &delta);
void Residue::translate(const QSet<AtomID> &atomids, const Vector &delta);
void Residue::translate(const QString &atom, const Vector &delta);
void Residue::translate(const QStringList &atoms, const Vector &delta);
void Residue::translate(const AtomIndex &atom, const Vector &delta);
void Residue::translate(const QSet<AtomIndex> &atoms, const Vector &delta);

void Residue::rotate(const Quaternion &quat, const Vector &point);
void Residue::rotate(AtomID atomid, const Quaternion &quat, const Vector &point);
void Residue::rotate(const QSet<AtomID> &atomids, const Quaternion &quat, const Vector &point);
void Residue::rotate(const QString &atom, const Quaternion &quat, const Vector &point);
void Residue::rotate(const QStringList &atoms, const Quaternion &quat, const Vector &point);
void Residue::rotate(const AtomIndex &atom, const Quaternion &quat, const Vector &point);
void Residue::rotate(const QSet<AtomIndex> &atoms, const Quaternion &quat, const Vector &point);

void Residue::rotate(const Matrix &rotmat, const Vector &point);
void Residue::rotate(AtomID atomid, const Matrix &rotmat, const Vector &point);
void Residue::rotate(const QSet<AtomID> &atomids, const Matrix &rotmat, const Vector &point);
void Residue::rotate(const QString &atom, const Matrix &rotmat, const Vector &point);
void Residue::rotate(const QStringList &atoms, const Matrix &rotmat, const Vector &point);
void Residue::rotate(const AtomIndex &atom, const Matrix &rotmat, const Vector &point);
void Residue::rotate(const QSet<AtomIndex> &atoms, const Matrix &rotmat, const Vector &point);

void Residue::setCoordinates(const QVector<Vector> &newcoords) const;

void Residue::setCoordinates(AtomID atomid, const Vector &newcoords) const;
void Residue::setCoordinates(const QHash<AtomID,Vector> &newcoords) const;

void Residue::setCoordinates(const QString &atomname, const Vector &newcoords) const;
void Residue::setCoordinates(const QHash<QString,Vector> &newcoords) const;

void Residue::setCoordinates(const AtomIndex &atom, const Vector &newcoords) const;
void Residue::setCoordinates(const QHash<AtomIndex,Vector> &newcoords) const;
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
                     const WeightFunction &weightfunc, const AtomIndexSet &anchors)
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
void Residue::change(const Bond &bnd, double delta, const AtomIndexSet &anchors)
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
                     const WeightFunction &weightfunc, const AtomIndexSet &anchors)
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
                     const AtomIndexSet &anchors)
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
                     const WeightFunction &weightfunc, const AtomIndexSet &anchors)
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
                     const AtomIndexSet &anchors)
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
                     const WeightFunction &weightfunc, const AtomIndexSet &anchors)
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
void Residue::change(const Bond &bnd, const SireMaths::Angle &delta, const AtomIndexSet &anchors)
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
                     const WeightFunction &weightfunc, const AtomIndexSet &anchors)
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
                     const AtomIndexSet &anchors)
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
                  const WeightFunction &weightfunc, const AtomIndexSet &anchors)
{
    change(bnd, size-moldata.measure(bnd), weightfunc, anchors);
}

/** Overloaded function used to use the default RelFromMass() weight function

    \throw SireMol::anchor_error
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/
void Residue::set(const Bond &bnd, double size, const AtomIndexSet &anchors)
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
                  const WeightFunction &weightfunc, const AtomIndexSet &anchors)
{
    change(ang, size-moldata.measure(ang), weightfunc, anchors);
}

/** Overloaded function used to use the default RelFromMass() weight function

    \throw SireMol::anchor_error
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/
void Residue::set(const SireMol::Angle &ang, const SireMaths::Angle &size,
                  const AtomIndexSet &anchors)
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
                  const WeightFunction &weightfunc, const AtomIndexSet &anchors)
{
    change(dih, size-moldata.measure(dih), weightfunc, anchors);
}

/** Overloaded function used to use the default RelFromMass() weight function

    \throw SireMol::anchor_error
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/
void Residue::set(const Dihedral &dih, const SireMaths::Angle &size, const AtomIndexSet &anchors)
{
    change(dih, size-moldata.measure(dih), anchors);
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
                     const WeightFunction &weightfunc, const AtomIndexSet &anchors)
{
    change(dih, size-moldata.measure(dih), weightfunc, anchors);
}

/** Overloaded function used to use the default RelFromMass() weight function

    \throw SireMol::anchor_error
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/
void Residue::setAll(const Dihedral &dih, const SireMaths::Angle &size,
                     const AtomIndexSet &anchors)
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
                  const WeightFunction &weightfunc, const AtomIndexSet &anchors)
{
    set(improper, size-moldata.measure(improper), weightfunc, anchors);
}

/** Overloaded function used to use the default RelFromMass() weight function

    \throw SireMol::anchor_error
    \throw SireMol::missing_atom
    \throw SireMol::ring_error
*/
void Residue::set(const Improper &improper, const SireMaths::Angle &size,
                  const AtomIndexSet &anchors)
{
    set(improper, size, RelFromMass(), anchors);
}

/////////////////////////////////////////////////////////
