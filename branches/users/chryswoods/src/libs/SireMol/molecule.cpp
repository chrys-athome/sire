/**
  * @file
  *
  * C++ Implementation: Molecule
  *
  * Description:
  * Implementation of Molecule
  *
  * @author Christopher Woods, (C) 2006
  *
  * Copyright: See COPYING file that comes with this distribution
  *
  */

#include "qhash_siremol.h"

#include "moleculedata.h"

using namespace SireStream;
using namespace SireMol;

static const RegisterMetaType<Molecule> r_molecule("SireMol::Molecule");

/** Serialise to a binary datastream */
QDataStream& operator<<(QDataStream &ds, const Molecule &mol)
{
    writeHeader(ds, r_molecule, 1);

    SharedDataStream(ds) << mol.d;

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream& operator>>(QDataStream &ds, Molecule &mol)
{
    VersionID v = readHeader(ds, r_molecule);

    if (v == 1)
    {
        SharedDataStream(ds) >> mol.d;
    }
    else
        throw version_error(v, "1", r_molecule, CODELOC);

    return ds;
}

/** Create an empty molecule */
Molecule::Molecule() : d( MoleculeData::null() )
{}

/** Construct a molecule called 'molname' as a copy of the passed EditMol,
    using the EditMol -> Molecule conversion function 'converter'
*/
Molecule::Molecule(const EditMol &editmol, const ConvertFunction &convertfunc)
         : d( new MoleculeData(editmol, convertfunc) )
{}

/** Construct a Molecule that is a copy of the molecule that contains the
    residue 'residue' */
Molecule::Molecule(const Residue &residue) : d( residue.d )
{}

/** Copy constructor - this is fast as this class is implicitly shared */
Molecule::Molecule(const Molecule &other) : d( other.d )
{}

/** Destructor */
Molecule::~Molecule()
{}

///// Operators /////////////////////////////////////////

/** Comparison operator */
bool Molecule::operator==(const Molecule &other) const
{
    return d.data() == other.d.data() or
           *d == *(other.d);
}

/** Comparison operator */
bool Molecule::operator!=(const Molecule &other) const
{
    return d.data() != other.d.data() and
           *d != *(other.d);
}

/** Assignment operator - this is fast as this class is implicitly shared */
Molecule& Molecule::operator=(const Molecule &other)
{
    d = other.d;
    return *this;
}

/** Return a copy of the CutGroup with ID == cgid

    \throw SireMol::missing_cutgroup
*/
CutGroup Molecule::operator[](CutGroupID cgid) const
{
    return d->at(cgid);
}

/** Return a copy of the residue at index 'resid'

    \throw SireError::invalid_index
*/
Residue Molecule::operator[](ResID resid) const
{
    return d->at(resid);
}

/** Return a copy of the residue with number 'resnum'

    \throw SireMol::missing_residue
*/
Residue Molecule::operator[](ResNum resnum) const
{
    return d->at(resnum);
}

/** Return a copy of the atom at index 'atomid'

    \throw SireError::invalid_index
*/
Atom Molecule::operator[](AtomID atomid) const
{
    return d->at(atomid);
}

/** Return a copy of the atom at index 'cgatomid'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
Atom Molecule::operator[](const CGAtomID &cgatomid) const
{
    return d->at(cgatomid);
}

/** Return a copy of the atom at index 'resatomid'

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
Atom Molecule::operator[](const ResNumAtomID &resatomid) const
{
    return d->at(resatomid);
}

/** Return a copy of the atom at index 'resatomid'

    \throw SireError::invalid_index
*/
Atom Molecule::operator[](const ResIDAtomID &resatomid) const
{
    return d->at(resatomid);
}

/** Return a copy of the atom with AtomIndex 'atm'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
Atom Molecule::operator[](const AtomIndex &atm) const
{
    return d->at(atm);
}

/////////////////////////////////////////////////////////


///// Interfacing with Residue //////////////////////////

/** Return a copy of the residue with number 'resnum' in this molecule

    \throw SireMol::missing_residue
*/
Residue Molecule::residue(ResNum resnum) const
{
    return Residue(*this, resnum);
}

/** Return a copy of the residue at index 'resid' in this molecule

    \throw SireError::invalid_index
*/
Residue Molecule::residue(ResID resid) const
{
    return Residue(*this, resid);
}

/** Return a copy of the first residue in this molecule that is called
    'resname'

    \throw SireMol::missing_residue
*/
Residue Molecule::residue(const QString &resname) const
{
    return Residue(*this, resname);
}

/** Return copies of all of the residues in this molecule.
    This is fast as this class is implicitly shared. */
QHash<ResNum,Residue> Molecule::residues() const
{
    const QVector<ResNum> &resnums = d->residueNumbers();

    int nres = resnums.count();

    QHash<ResNum,Residue> residus;
    residus.reserve(nres);

    for (int i=0; i<nres; ++i)
    {
        residus.insert( resnums[i], Residue(*this, resnums[i]) );
    }

    return residus;
}

/** Return a copy of the residue with residue number 'resnum'

    \throw SireMol::missing_residue
*/
Residue Molecule::at(ResNum resnum) const
{
    return Residue(*this, resnum);
}

/** Return a copy of the residue at index 'resid' in this molecule

    \throw SireError::invalid_index
*/
Residue Molecule::at(ResID resid) const
{
    return Residue(*this, resid);
}

/////////////////////////////////////////////////////////


////// Interfacing with EditMol /////////////////////////
EditMol Molecule::toEditMol() const
{
    return d->toEditMol();
}

void Molecule::update(const EditMol &editmol, const ConvertFunction &convertfunc)
{
    d->update( editmol, convertfunc );
}

/////////////////////////////////////////////////////////


///// Querying the molecule /////////////////////////////

/** Return a copy of the CutGroup with ID == cgid

    \throw SireMol::missing_cutgroup
*/
CutGroup Molecule::at(CutGroupID cgid) const
{
    return d->at(cgid);
}

/** Return a copy of the atom at index 'atomid'

    \throw SireError::invalid_index
*/
Atom Molecule::at(AtomID atomid) const
{
    return d->at(atomid);
}

/** Return a copy of the atom at index 'cgatomid'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
Atom Molecule::at(const CGAtomID &cgatomid) const
{
    return d->at(cgatomid);
}

/** Return a copy of the atom at index 'resatomid'

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
Atom Molecule::at(const ResNumAtomID &resatomid) const
{
    return d->at(resatomid);
}

/** Return a copy of the atom at index 'resatomid'

    \throw SireError::invalid_index
*/
Atom Molecule::at(const ResIDAtomID &resatomid) const
{
    return d->at(resatomid);
}

/** Return a copy of the atom with AtomIndex 'atm'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
Atom Molecule::at(const AtomIndex &atm) const
{
    return d->at(atm);
}

/** Return a copy of the connectivity of this molecule */
MoleculeBonds Molecule::connectivity() const
{
    return d->connectivity();
}

/** Return a copy of the connectivity of the residue with
    number 'resnum'

    \throw SireMol::missing_residue
*/
ResidueBonds Molecule::connectivity(ResNum resnum) const
{
    return d->connectivity(resnum);
}

/** Return a copy of the connectivity of the residue at
    index 'resid'

    \throw SireError::invalid_index
*/
ResidueBonds Molecule::connectivity(ResID resid) const
{
    return d->connectivity(resid);
}

/** Return the MoleculeInfo that holds the metainfo for this
    molecule */
const MoleculeInfo& info() const
{
    return d->info();
}

/** Return an array holding a copy of all of the atoms in this molecule */
QVector<Atom> atoms() const
{
    return d->atoms();
}

/** Return an array holding a copy of all of the atoms in the residue
    with number 'resnum'

    \throw SireMol::missing_residue
*/
QVector<Atom> atoms(ResNum resnum) const
{
    return d->atoms(resnum);
}

/** Return an array holding a copy of all of the atoms in the residue
    at index 'resid' in this molecule.

    \throw SireError::invalid_index
*/
QVector<Atom> atoms(ResID resid) const
{
    return d->atoms(resid);
}

QHash<CutGroupID,CutGroup> cutGroups() const;
QHash<CutGroupID,CutGroup> cutGroups(ResNum resnum) const;
QHash<CutGroupID,CutGroup> cutGroups(ResID resid) const;

CutGroup cutGroup(CutGroupID id) const;

QHash<CutGroupID,CoordGroup> coordGroups() const;
QHash<CutGroupID,CoordGroup> coordGroups(ResNum resnum) const;
QHash<CutGroupID,CoordGroup> coordGroups(ResID resid) const;

CoordGroup coordGroup(CutGroupID id) const;

Atom atom(CutGroupID cgid, AtomID atomid) const;
Atom atom(const CGAtomID &cgatmid) const;
Atom atom(ResNum resnum, AtomID atomid) const;
Atom atom(const ResNumAtomID &resatomid) const;
Atom atom(ResID resid, AtomID atomid) const;
Atom atom(const ResIDAtomID &resatomid) const;
Atom atom(const AtomIndex &atm) const;
Atom atom(ResNum resnum, const QString &atomname) const;

Vector coordinates(CutGroupID cgid, AtomID atomid) const;
Vector coordinates(const CGAtomID &cgatomid) const;
Vector coordinates(ResNum resnum, AtomID atomid) const;
Vector coordinates(const ResNumAtomID &resatomid) const;
Vector coordinates(ResID resid, AtomID atomid) const;
Vector coordinates(const ResIDAtomID &resatomid) const;
Vector coordinates(const AtomIndex &atm) const;
Vector coordinates(ResNum resnum, const QString &atomname) const;

QHash<CGAtomID,Vector> coordinates(const QSet<CGAtomID> &cgatomids) const;
QHash<ResNumAtomID,Vector> coordinates(const QSet<ResNumAtomID> &resatomids) const;
QHash<ResIDAtomID,Vector> coordinates(const QSet<ResIDAtomID> &resatomids) const;
QHash<AtomIndex,Vector> coordinates(const QSet<AtomIndex> &atoms) const;

QVector<Vector> coordinates(CutGroupID cgid);
QHash< CutGroupID,QVector<Vector> >
coordinates(const QSet<CutGroupID> &cgids) const;

QVector<Vector> coordinates(ResNum resnum);
QHash< ResNum,QVector<Vector> >
coordinates(const QSet<ResNum> &resnums) const;

QVector<Vector> coordinates(ResID resid);
QHash< ResID,QVector<Vector> >
coordinates(const QSet<ResID> &resids) const;

const QString& name() const;

QString residueName(ResNum resnum) const;
QString residueName(ResID resid) const;

ResNum residueNumber(ResID resid) const;

bool isEmpty() const;
bool isEmpty(ResNum resnum) const;
bool isEmpty(ResID resid) const;
bool isEmpty(CutGroupID cgid) const;

QVector<ResNum> residueNumbers() const;
QStringList residueNames() const;

QVector<ResNum> residueNumbers(const QString &resnam) const;
QVector<ResNum> residueNumbers(CutGroupID cgid) const;

QHash<ResNum,Residue> residuesBondedTo(ResNum resnum) const;
QHash<ResNum,Residue> residuesBondedTo(ResID resid) const;

bool contains(CutGroupID cgid) const;
bool contains(ResNum resnum) const;
bool contains(ResID resid) const;

bool contains(ResNum resnum, const QString &atomname) const;
bool contains(const AtomIndex &atm) const;
bool contains(CutGroupID cgid, AtomID atomid) const;
bool contains(const CGAtomID &cgatomid) const;
bool contains(ResNum resnum, AtomID atomid) const;
bool contains(const ResNumAtomID &resatomid) const;
bool contains(ResID resid, AtomID atomid) const;
bool contains(const ResIDAtomID &resatomid) const;

bool contains(const Bond &bond) const;

int nAtoms() const;
int nAtoms(ResNum resnum) const;
int nAtoms(ResID resid) const;
int nAtoms(CutGroupID id) const;

int nResidues() const;

int nCutGroups() const;

QStringList atomNames(ResNum resnum) const;
QStringList atomNames(ResID resid) const;

SireMaths::Line bond(const Bond &bnd) const;
SireMaths::Triangle angle(const SireMol::Angle &ang) const;
SireMaths::Torsion dihedral(const Dihedral &dih) const;
SireMaths::Torsion improper(const Improper &improper) const;

double measure(const Bond &bnd) const;
SireMaths::Angle measure(const SireMol::Angle &ang) const;
SireMaths::Angle measure(const Dihedral &dih) const;
SireMaths::Angle measure(const Improper &improper) const;

double getWeight(const AtomIDGroup &group0, const AtomIDGroup &group1,
                 const WeightFunction &weightfunc) const;
/////////////////////////////////////////////////////////


//////// Moving the molecule ////////////////////
void translate(const Vector &delta);
void translate(const AtomIDGroup &group, const Vector &delta);
void translate(const AtomIndex &atom, const Vector &delta);
void translate(const QSet<AtomIndex> &atoms, const Vector &delta);
void translate(ResNum resnum, const QStringList &atoms, const Vector &delta);
void translate(ResNum resnum, const Vector &delta);
void translate(const QSet<ResNum> &resnums, const Vector &delta);
void translate(ResID resid, const QStringList &atoms, const Vector &delta);
void translate(ResID resid, const Vector &delta);
void translate(const QSet<ResID> &resids, const Vector &delta);
void translate(CutGroupID cgid, const Vector &delta);
void translate(const QSet<CutGroupID> &cgids, const Vector &delta);

void rotate(const Quaternion &quat, const Vector &point);
void rotate(const AtomIDGroup &group, const Quaternion &quat, const Vector &point);
void rotate(const AtomIndex &atom, const Quaternion &quat, const Vector &point);
void rotate(const QSet<AtomIndex> &atoms, const Quaternion &quat, const Vector &point);
void rotate(ResNum resnum, const QStringList &atoms, const Quaternion &quat,
            const Vector &point);
void rotate(ResNum resnum, const Quaternion &quat, const Vector &point);
void rotate(const QSet<ResNum> &resnums, const Quaternion &quat, const Vector &point);
void rotate(ResID resid, const QStringList &atoms, const Quaternion &quat,
            const Vector &point);
void rotate(ResID resid, const Quaternion &quat, const Vector &point);
void rotate(const QSet<ResID> &resids, const Quaternion &quat, const Vector &point);
void rotate(CutGroupID cgid, const Quaternion &quat, const Vector &point);
void rotate(const QSet<CutGroupID> &cgids, const Vector &delta);

void rotate(const Matrix &matrix, const Vector &point);
void rotate(const AtomIDGroup &group, const Matrix &matrix, const Vector &point);
void rotate(const AtomIndex &atom, const Matrix &matrix, const Vector &point);
void rotate(const QSet<AtomIndex> &atoms, const Matrix &matrix, const Vector &point);
void rotate(ResNum resnum, const QStringList &atoms, const Matrix &matrix,
            const Vector &point);
void rotate(ResNum resnum, const Matrix &matrix, const Vector &point);
void rotate(const QSet<ResNum> &resnums, const Matrix &matrix, const Vector &point);
void rotate(ResID resid, const QStringList &atoms, const Matrix &matrix,
            const Vector &point);
void rotate(ResID resid, const Matrix &matrix, const Vector &point);
void rotate(const QSet<ResID> &resids, const Matrix &matrix, const Vector &point);
void rotate(CutGroupID cgid, const Matrix &matrix, const Vector &point);
void rotate(const QSet<CutGroupID> &cgids, const Matrix &matrix, const Vector &point);

void setCoordinates(CutGroupID cgid, const CoordGroup &newcoords);
void setCoordinates(const QHash<CutGroupID,CoordGroup> &newcoords);

void setCoordinates(CutGroupID cgid, const QVector<Vector> &newcoords);
void setCoordinates(const QHash< CutGroupID,QVector<Vector> > &newcoords);

void setCoordinates(ResNum resnum, const QVector<Vector> &newcoords);
void setCoordinates(const QHash< ResNum,QVector<Vector> > &newcoords);

void setCoordinates(ResID resid, const QVector<Vector> &newcoords);
void setCoordinates(const QHash< ResID,QVector<Vector> > &newcoords);

void setCoordinates(const AtomIndex &atom, const Vector &newcoords);
void setCoordinates(const QHash<AtomIndex,Vector> &newcoords);

void setCoordinates(const CGAtomID &cgatomid, const Vector &newcoords);
void setCoordinates(const QHash<CGAtomID,Vector> &newcoords);

void setCoordinates(const ResNumAtomID &resatomid, const Vector &newcoords);
void setCoordinates(const QHash<ResNumAtomID,Vector> &newcoords);

void setCoordinates(const ResIDAtomID &resatomid, const Vector &newcoords);
void setCoordinates(const QHash<ResIDAtomID,Vector> &newcoords);
/////////////////////////////////////////////////


///// Internal geometry moves ///////////////////////////

/** Change the length of the bond 'bnd' by 'delta'. The atoms passed passed in 'anchors'
    are anchors, e.g. atoms that may not be moved. The WeightFunction 'weightfunc' is used
    to control how the two parts of the residue that are moved (the two sides of
    the moving bond) are weighted when they are moved.

    This will throw an exception if either the atoms in the bond are not present
    in this molecule, if both sides of the bond are anchored, or if the bond
    is part of a ring.

    \throw SireMol::missing_atom
    \throw SireMol::anchor_error
    \throw SireMol::ring_error

*/
void Molecule::change(const Bond &bnd, double delta,
                      const WeightFunction &weightfunc,
                      const QSet<AtomIndex> &anchors)
{
    //Split the molecule into two groups based on the bond
    boost::tuple<AtomIDGroup,AtomIDGroup> groups = splitMolecule(bnd, connectivity());

    AtomIDGroup group0 = groups.get<0>();
    AtomIDGroup group1 = groups.get<1>();

    d->change(bnd, delta, group0, group1, weightfunc, anchors);
}

/** Overloaded function that changes the bond length using the RelFromMass weight function

    \throw SireMol::missing_atom
    \throw SireMol::anchor_error
    \throw SireMol::ring_error
*/
void Molecule::change(const Bond &bond, double delta, const QSet<AtomIndex> &anchors)
{
    change(bond, delta, RelFromMass(), anchors);
}

/** Change the size of the angle 'ang' by 'delta'. The atoms passed passed in 'anchors'
    are anchors, e.g. atoms that may not be moved. The WeightFunction 'weightfunc' is used to
    control how the two parts of the molecule that are moved (the two sides of the moving
    angle) are weighted when they are moved.

    This will throw an exception if either the atoms in the angle are not present
    in this molecule, if both sides of the angle are anchored, or if the angle
    is part of a ring.

    \throw SireMol::missing_atom
    \throw SireMol::anchor_error
    \throw SireMol::ring_error

*/
void Molecule::change(const SireMol::Angle &ang, const SireMaths::Angle &delta,
                      const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors)
{
    //split the molecule into two parts based on the angle
    boost::tuple<AtomIDGroup,AtomIDGroup> groups = splitMolecule(ang, connectivity());

    AtomIDGroup group0 = groups.get<0>();
    AtomIDGroup group1 = groups.get<1>();

    d->change(ang, delta, group0, group1, weightfunc, anchors);
}

/** Overload of the function that uses a default weight function

    \throw SireMol::missing_atom
    \throw SireMol::anchor_error
    \throw SireMol::ring_error
*/
void Molecule::change(const SireMol::Angle &ang, const SireMaths::Angle &delta,
                      const QSet<AtomIndex> &anchors)
{
    change(ang, delta, RelFromMass(), anchors);
}

/** Change the size of the dihedral 'dih' by 'delta'. The atoms passed passed in 'anchors'
    are anchors, e.g. atoms that may not be moved. The WeightFunction 'weightfunc' is used to
    control how the two parts of the molecule that are moved (the two sides of the moving
    dihedral) are weighted when they are moved.

    This will throw an exception if either the atoms in the dihedral are not present
    in this molecule, if both sides of the angle are anchored, or if the dihedral
    is part of a ring.

    \throw SireMol::missing_atom
    \throw SireMol::anchor_error
    \throw SireMol::ring_error

*/
void Molecule::change(const Dihedral &dih, const SireMaths::Angle &delta,
                      const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors)
{
    //split the molecule into two parts based on the dihedral
    boost::tuple<AtomIDGroup,AtomIDGroup> groups = splitMolecule(dih, connectivity());

    AtomIDGroup group0 = groups.get<0>();
    AtomIDGroup group1 = groups.get<1>();

    d->change(Bond(dih.atom1(),dih.atom2()), delta, group0, group1, weightfunc, anchors);
}

/** Overload of the function that uses a default weight function

    \throw SireMol::missing_atom
    \throw SireMol::anchor_error
    \throw SireMol::ring_error
*/
void Molecule::change(const Dihedral &dih, const SireMaths::Angle &delta,
                      const QSet<AtomIndex> &anchors)
{
    change(dih,delta,RelFromMass(),anchors);
}

/** Change the size of the dihedral 'dih' by 'delta'. The atoms passed passed in 'anchors'
    are anchors, e.g. atoms that may not be moved. The WeightFunction 'weightfunc' is used to
    control how the two parts of the molecule that are moved (the two sides of the moving
    dihedral) are weighted when they are moved. This moves all atoms that are either side
    of the dihedral.

    This will throw an exception if either the atoms in the dihedral are not present
    in this molecule, if both sides of the angle are anchored, or if the dihedral
    is part of a ring.

    \throw SireMol::missing_atom
    \throw SireMol::anchor_error
    \throw SireMol::ring_error
*/
void Molecule::change(const Bond &bnd, const SireMaths::Angle &delta,
                      const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors)
{
    //split the molecule into two parts based on the bond
    boost::tuple<AtomIDGroup,AtomIDGroup> groups = splitMolecule(bnd, connectivity());

    AtomIDGroup group0 = groups.get<0>();
    AtomIDGroup group1 = groups.get<1>();

    d->change(bnd, delta, group0, group1, weightfunc, anchors);
}

/** Overload of the function that uses a default WeightFunction */
void Molecule::change(const Bond &dih, const SireMaths::Angle &delta, const QSet<AtomIndex> &anchors)
{
    change(dih, delta, RelFromMass(), anchors);
}

/** Change the size of the improper angle 'improper' by 'delta'. The atoms passed passed in 'anchors'
    are anchors, e.g. atoms that may not be moved. The WeightFunction 'weightfunc' is used to
    control how the two parts of the molecule that are moved (the two sides of the moving
    improper angle) are weighted when they are moved.

    This will throw an exception if either the atoms in the improper are not present
    in this molecule, if both sides of the angle are anchored, or if the improper
    is part of a ring.

    \throw SireMol::missing_atom
    \throw SireMol::anchor_error
    \throw SireMol::ring_error

*/
void Molecule::change(const Improper &improper, const SireMaths::Angle &delta,
                      const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors)
{
    //split the molecule into two parts based on the improper
    boost::tuple<AtomIDGroup,AtomIDGroup> groups = splitMolecule(improper, connectivity());

    AtomIDGroup group0 = groups.get<0>();
    AtomIDGroup group1 = groups.get<1>();

    d->change(improper, delta, group0, group1, weightfunc, anchors);
}

/** Overload of the function that uses a default weight function

    \throw SireMol::missing_atom
    \throw SireMol::anchor_error
    \throw SireMol::ring_error
*/
void Molecule::change(const Improper &improper, const SireMaths::Angle &delta,
                      const QSet<AtomIndex> &anchors)
{
    change(improper, delta, RelFromMass(), anchors);
}

/** This function is similar to Molecule::changeBond, except that it sets the specified
    bond to a particular length

    \throw SireMol::missing_atom
    \throw SireMol::anchor_error
    \throw SireMol::ring_error
*/
void Molecule::set(const Bond &bnd, double val,
                   const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors)
{
    change(bnd, val-measure(bnd), weightfunc, anchors);
}

/** Overload of the function that uses a default weight function

    \throw SireMol::missing_atom
    \throw SireMol::anchor_error
    \throw SireMol::ring_error
*/
void Molecule::set(const Bond &bnd, double val, const QSet<AtomIndex> &anchors)
{
    change(bnd, val-measure(bnd), anchors);
}

/** This function is similar to Molecule::changeAngle, except that it set the specified angle
    to a particular size. Note that this will probably change the size of other angles, so
    if you wish to set a whole series of angles, then you will need to think carefully about
    the order in which you set them.

    \throw SireMol::missing_atom
    \throw SireMol::anchor_error
    \throw SireMol::ring_error
*/
void Molecule::set(const SireMol::Angle &ang, const SireMaths::Angle &val,
                   const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors)
{
    change(ang, val-measure(ang), weightfunc, anchors);
}

/** Overload of the function that uses a default weight function

    \throw SireMol::missing_atom
    \throw SireMol::anchor_error
    \throw SireMol::ring_error
*/
void Molecule::set(const SireMol::Angle &ang, const SireMaths::Angle &val,
                   const QSet<AtomIndex> &anchors)
{
    change(ang, val-measure(ang), anchors);
}

/** This function is similar to Molecule::changeDihedral, except that it sets the specified
    dihedral to a particular size. Note that this will probably change the size of other
    dihedrals, so think about the order in which you apply multiple calls of this function

    \throw SireMol::missing_atom
    \throw SireMol::anchor_error
    \throw SireMol::ring_error
*/
void Molecule::set(const Dihedral &dih, const SireMaths::Angle &val,
                   const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors)
{
    change(dih, val-measure(dih), weightfunc, anchors);
}

/** Overload of the function that uses a default weight function

    \throw SireMol::missing_atom
    \throw SireMol::anchor_error
    \throw SireMol::ring_error
*/
void Molecule::set(const Dihedral &dih, const SireMaths::Angle &val,
                   const QSet<AtomIndex> &anchors)
{
    change(dih, val-measure(dih), anchors);
}

/** This function is similar to Molecule::change(const Bond&), except that it sets the specified
    dihedral to a particular size.

    \throw SireMol::missing_atom
    \throw SireMol::anchor_error
    \throw SireMol::ring_error
*/
void Molecule::setAll(const Dihedral &dih, const SireMaths::Angle &val,
                      const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors)
{
    change(dih, val-measure(dih), weightfunc, anchors);
}

/** Overload of the function that uses a default weight function

    \throw SireMol::missing_atom
    \throw SireMol::anchor_error
    \throw SireMol::ring_error
*/
void Molecule::setAll(const Dihedral &dih, const SireMaths::Angle &val,
                      const QSet<AtomIndex> &anchors)
{
    change(dih, val-measure(dih), anchors);
}

/** This function is similar to Molecule::change(const Improper&), except that it
    sets the specified improper angle to a particular size.

    \throw SireMol::missing_atom
    \throw SireMol::anchor_error
    \throw SireMol::ring_error
*/
void Molecule::set(const Improper &improper, const SireMaths::Angle &size,
                   const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors)
{
    change(improper, size-measure(improper), weightfunc, anchors);
}

/** Overload of the function that uses a default weight function

    \throw SireMol::missing_atom
    \throw SireMol::anchor_error
    \throw SireMol::ring_error
*/
void Molecule::set(const Improper &improper, const SireMaths::Angle &size,
                   const QSet<AtomIndex> &anchors)
{
    change(improper, size-measure(improper), anchors);
}

/////////////////////////////////////////////////////////
