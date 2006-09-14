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

#include "molecule.h"
#include "residue.h"
#include "moleculebonds.h"
#include "residuebonds.h"
#include "atomidgroup.h"
#include "bond.h"
#include "angle.h"
#include "dihedral.h"
#include "improper.h"
#include "splitmolecule.h"
#include "weightfunction.h"
#include "moleculesignature.h"

#include "SireMaths/quaternion.h"
#include "SireMaths/angle.h"
#include "SireMaths/line.h"
#include "SireMaths/triangle.h"
#include "SireMaths/torsion.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireMol;

static const RegisterMetaType<Molecule> r_molecule("SireMol::Molecule");

/** Serialise a molecule to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const Molecule &mol)
{
    writeHeader(ds, r_molecule, 1) << mol.moldata;

    return ds;
}

/** Deserialise a molecule from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, Molecule &mol)
{
    VersionID v = readHeader(ds, r_molecule);

    if (v == 1)
    {
        ds >> mol.moldata;
    }
    else
        throw version_error(v, "1", r_molecule, CODELOC);

    return ds;
}

/** Create an empty molecule */
Molecule::Molecule()
{}

/** Construct a molecule called 'molname' as a copy of the passed EditMol,
    using the EditMol -> Molecule conversion function 'converter'
*/
Molecule::Molecule(const EditMol &editmol, const ConvertFunction &converter)
         : moldata(editmol,converter)
{}

/** Construct a molecule that is a view on the molecular data in 'mdata' */
Molecule::Molecule(const MoleculeData &mdata) : moldata(mdata)
{}

/** Copy constructor - this is fast as this class is implicitly shared */
Molecule::Molecule(const Molecule &other) : moldata(other.moldata)
{}

/** Destructor */
Molecule::~Molecule()
{}

///// Operators /////////////////////////////////////////

/** Comparison operator */
bool Molecule::operator==(const Molecule &other) const
{
    return moldata == other.moldata;
}

/** Comparison operator */
bool Molecule::operator!=(const Molecule &other) const
{
    return moldata != other.moldata;
}

/** Assignment operator - this is fast as this class is implicitly shared */
Molecule& Molecule::operator=(const Molecule &other)
{
    moldata = other.moldata;
    return *this;
}

/** Assignment operator - this will copy the contents of 'residue' into this
    molecule. */
Molecule& Molecule::operator=(const Residue &residue)
{
    moldata.merge(residue);
}

/** Return the 'ith' residue - this is fast
    as the data is implicitly shared

    \throw SireError::index_error
*/
Residue Molecule::operator[](int i) const
{
    return moldata.at(i);
}

/////////////////////////////////////////////////////////


///// Memory mangement and interface with residue ///////

/** Return the ID number of this molecule */
MoleculeID Molecule::ID() const
{
    return moldata.ID();
}

/** Get a new ID number for this molecule - this will reset the
    molecule version to 1-0 */
void Molecule::setNewID()
{
    //now tell the moldata to get new ID numbers
    moldata.setNewID();
}

/** Return the residue 'resnum' - this is fast as the
    data is implicitly shared

    \throw SireMol::missing_residue
*/
Residue Molecule::residue(ResNum resnum) const
{
    return moldata.residue(resnum);
}

/** Return the 'ith' residue - this is fast
    as the data is implicitly shared

    \throw SireMol::index_error
*/
Residue Molecule::at(int i) const
{
    return moldata.at(i);
}

/** Return a list of all of the residues in this molecule.
    This is fast as the data is implicitly shared */
QHash<ResNum,Residue> Molecule::residues() const
{
    return moldata.residues();
}

/////////////////////////////////////////////////////////


///// Querying the molecule /////////////////////////////

/** Return a string description of the molecule */
QString Molecule::toString() const
{
    int nres = nResidues();
    int nats = nAtoms();
    int nbnds = nBonds();
    int ncuts = nCutGroups();

    return QObject::tr("Molecule(\"%1\" : %2). nResidues() == %3, nAtoms() == %4, "
                           "nBonds() == %5 and nCutGroups() == %6.")
                     .arg(qstr(ID()), name(), qstr(nres))
                     .arg(qstr(nats), qstr(nbnds), qstr(ncuts));

}

/** Return whether or not the molecule is null (has no atoms or residues) */
bool Molecule::isNull() const
{
    return moldata.isEmpty();
}

/** Return whether or not the molecule is empty (has no atoms or residues) */
bool Molecule::isEmpty() const
{
    return moldata.isEmpty();
}

/** Return the name of this molecule */
QString Molecule::name() const
{
    return moldata.name();
}

/** Return the MoleculeInfo object for this molecule */
const MoleculeInfo& Molecule::info() const
{
    return moldata.info();
}

/** Return the array of CutGroups in this molecule - the
    index of the CutGroup in the array is its CutGroupID */
QVector<CutGroup> Molecule::cutGroups() const
{
    return moldata.cutGroups();
}

/** Return the CutGroup with ID == id

    \throw SireMol::missing_cutgroup
*/
const CutGroup& Molecule::cutGroup(CutGroupID id) const
{
    return moldata.cutGroup(id);
}

/** Return the array of coordinates of the atoms in this molecule.
    The index of the CoordGroup in the array is its CutGroupID */
QVector<CoordGroup> Molecule::coordinates() const
{
    return moldata.coordinates();
}

/** Return the coordinates of the CutGroup with ID == id

    \throw SireMol::missing_cutgroup
*/
CoordGroup Molecule::coordinates(CutGroupID id) const
{
    return moldata.coordinates(id);
}

/** Return the connectivity of this molecule */
MoleculeBonds Molecule::connectivity() const
{
    return moldata.connectivity();
}

/** Return the name of the residue with number 'resnum'

    \throw SireMol::missing_residue
*/
QString Molecule::residueName(ResNum resnum) const
{
    return moldata.info().residue(resnum).name();
}

/** Return the list of residue numbers of residues in this molecule (in the order
    that they were added to the molecule) */
QList<ResNum> Molecule::residueNumbers() const
{
    return moldata.info().residueNumbers();
}

/** Return the list of residue numbers of residues called 'resnam' (in the order
    that they were added to the molecule). Returns an empty list if there are no
    residues with this name in the molecule. */
QList<ResNum> Molecule::residueNumbers(const QString &resnam) const
{
    return moldata.info().residueNumbers(resnam);
}

/** Return the list of the names of the residues in this molecule - this is in the
    same order as the list of residue numbers returned by 'residueNumbers()' */
QStringList Molecule::residueNames() const
{
    return moldata.info().residueNames();
}

/** Return the names of all of the residues indexed by their number */
QHash<ResNum,QString> Molecule::residueIDs() const
{
    return moldata.info().residueIDs();
}

/** Return a copy of the atom with AtomIndex 'atm'

    \throw SireMol::missing_atom
*/
Atom Molecule::atom( const AtomIndex &atm ) const
{
    return moldata.atom(atm);
}

/** Return a copy of all of the atoms in this molecule */
QList<Atom> Molecule::atoms() const
{
    return moldata.atoms();
}

/** Return an array of copies of all of the atoms in this molecule */
QVector<Atom> Molecule::atomVector() const
{
    return moldata.atomVector();
}

/** Return a copy of all of the atoms in residue with number 'resnum'

    \throw SireMol::missing_residue
*/
QList<Atom> Molecule::atoms(ResNum resnum) const
{
    return moldata.atoms(resnum);
}

/** Return an array of copies of all of the atoms in the
    residue with number 'resnum'

    \throw SireMol::missing_residue
*/
QVector<Atom> Molecule::atomVector(ResNum resnum) const
{
    return moldata.atomVector(resnum);
}

/** Return the list of all of the bonds in this molecule */
QList<Bond> Molecule::bonds() const
{
    return connectivity().bonds();
}

/** Return the list of bonds in the residue with number 'resnum' */
QList<Bond> Molecule::bonds(ResNum resnum) const
{
      return connectivity().residue(resnum).bonds();
}

/** Return the count of the number of atoms in this molecule */
int Molecule::nAtoms() const
{
    return moldata.nAtoms();
}

/** Return the number of atoms in the residue with number 'resnum'

    \throw SireMol::missing_residue
*/
int Molecule::nAtoms(ResNum resnum) const
{
    return moldata.nAtoms(resnum);
}

/** Return the number of atoms in the CutGroup with CutGroupID 'id'

    \throw SireMol::missing_cutgroup
*/
int Molecule::nAtoms(CutGroupID id) const
{
    return moldata.nAtoms(id);
}

/** Return the count of the number of bonds in this molecule */
int Molecule::nBonds() const
{
    return moldata.connectivity().nBonds();
}

/** Return the count of the number of residues in this molecule */
int Molecule::nResidues() const
{
    return moldata.nResidues();
}

/** Return the count of the number of CutGroups in this molecule */
int Molecule::nCutGroups() const
{
    return moldata.nCutGroups();
}

/** Return the number of residues in this molecule */
int Molecule::count() const
{
    return moldata.nResidues();
}

/** Return the geometric line representing the bond 'bnd'

    \throw SireMol::missing_atom
*/
SireMaths::Line Molecule::bond( const Bond &bnd ) const
{
    return moldata.bond(bnd);
}

/** Return the geometric triangle representing the angle 'ang'

    \throw SireMol::missing_atom
*/
SireMaths::Triangle Molecule::angle( const Angle &ang ) const
{
    return moldata.angle(ang);
}

/** Return the geometric torsion representing the dihedral 'dih'

    \throw SireMol::missing_atom
*/
SireMaths::Torsion Molecule::dihedral( const Dihedral &dih ) const
{
    return moldata.dihedral(dih);
}

/** Return the geometric torsion representing the improper angle 'imp'

    \throw SireMol::missing_atom
*/
SireMaths::Torsion Molecule::improper( const Improper &imp ) const
{
    return moldata.improper(imp);
}

/** Return the length of the bond 'bnd'

    \throw SireMol::missing_atom
*/
double Molecule::measure( const Bond &bnd ) const
{
    return measure(bnd);
}

/** Return the size of the angle 'ang'

    \throw SireMol::missing_atom
*/
SireMaths::Angle Molecule::measure( const Angle &ang ) const
{
    return measure(ang);
}

/** Return the size of the dihedral 'dih'

    \throw SireMol::missing_atom
*/
SireMaths::Angle Molecule::measure( const Dihedral &dih ) const
{
    return measure(dih);
}

/** Return the size of the improper angle 'imp'

    \throw SireMol::missing_atom
*/
SireMaths::Angle Molecule::measure( const Improper &imp ) const
{
    return measure(imp);
}

/////////////////////////////////////////////////////////


///// Moving the molecule ///////////////////////////////

/** Translate the whole molecule by 'delta' */
void Molecule::translate(const Vector &delta)
{
    moldata.translate(delta);
}

/** Translate the atom group 'group' by 'delta'

    \throw SireMol::missing_atom
    \throw SireMol::missing_residue
*/
void Molecule::translate(const AtomIDGroup &group, const Vector &delta)
{
    moldata.translate(group,delta);
}

/** Translate the atoms whose indexes are in 'atoms' by 'delta'

    \throw SireMol::missing_atom
*/
void Molecule::translate(const AtomIndexSet &atoms, const Vector &delta)
{
    moldata.translate(atoms,delta);
}

/** Translate the whole of the residue 'resnum' by 'delta'

    \throw SireMol::missing_residue
*/
void Molecule::translate(ResNum resnum, const Vector &delta)
{
    moldata.translate(resnum,delta);
}

/** Rotate the whole molecule by the quaternion 'quat' about the point 'point' */
void Molecule::rotate(const Quaternion &quat, const Vector &point)
{
    moldata.rotate(quat,point);
}

/** Rotate the atoms in 'group' by 'quat' about 'point'

    \throw SireMol::missing_atom
    \throw SireMol::missing_residue
*/
void Molecule::rotate(const AtomIDGroup &group, const Quaternion &quat, const Vector &point)
{
    moldata.rotate(group,quat,point);
}

/** Rotate the atoms whose indexes are in 'atoms' by 'quat' about 'point'

    \throw SireMol::missing_atom
*/
void Molecule::rotate(const AtomIndexSet &atoms, const Quaternion &quat, const Vector &point)
{
    moldata.rotate(atoms,quat,point);
}

/** Rotate the atoms whose names are in 'atoms' and that are in residue 'resnum' by
    'quat' about 'point'

    \throw SireMol::missing_atom
    \throw SireMol::missing_residue
*/
void Molecule::rotate(ResNum resnum, const Quaternion &quat, const Vector &point)
{
    moldata.rotate(resnum,quat,point);
}

/////////////////////////////////////////////////////////


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
                      const WeightFunction &weightfunc, const AtomIndexSet &anchors)
{
    //Split the molecule into two groups based on the bond
    boost::tuple<AtomIDGroup,AtomIDGroup> groups = splitMolecule(bnd, connectivity());

    AtomIDGroup group0 = groups.get<0>();
    AtomIDGroup group1 = groups.get<1>();

    moldata.change(bnd, delta, group0, group1, weightfunc, anchors);
}

/** Overloaded function that changes the bond length using the RelFromMass weight function

    \throw SireMol::missing_atom
    \throw SireMol::anchor_error
    \throw SireMol::ring_error
*/
void Molecule::change(const Bond &bond, double delta, const AtomIndexSet &anchors)
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
                      const WeightFunction &weightfunc, const AtomIndexSet &anchors)
{
    //split the molecule into two parts based on the angle
    boost::tuple<AtomIDGroup,AtomIDGroup> groups = splitMolecule(ang, connectivity());

    AtomIDGroup group0 = groups.get<0>();
    AtomIDGroup group1 = groups.get<1>();

    moldata.change(ang, delta, group0, group1, weightfunc, anchors);
}

/** Overload of the function that uses a default weight function

    \throw SireMol::missing_atom
    \throw SireMol::anchor_error
    \throw SireMol::ring_error
*/
void Molecule::change(const SireMol::Angle &ang, const SireMaths::Angle &delta,
                      const AtomIndexSet &anchors)
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
                      const WeightFunction &weightfunc, const AtomIndexSet &anchors)
{
    //split the molecule into two parts based on the dihedral
    boost::tuple<AtomIDGroup,AtomIDGroup> groups = splitMolecule(dih, connectivity());

    AtomIDGroup group0 = groups.get<0>();
    AtomIDGroup group1 = groups.get<1>();

    moldata.change(Bond(dih.atom1(),dih.atom2()), delta, group0, group1, weightfunc, anchors);
}

/** Overload of the function that uses a default weight function

    \throw SireMol::missing_atom
    \throw SireMol::anchor_error
    \throw SireMol::ring_error
*/
void Molecule::change(const Dihedral &dih, const SireMaths::Angle &delta,
                      const AtomIndexSet &anchors)
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
                      const WeightFunction &weightfunc, const AtomIndexSet &anchors)
{
    //split the molecule into two parts based on the bond
    boost::tuple<AtomIDGroup,AtomIDGroup> groups = splitMolecule(bnd, connectivity());

    AtomIDGroup group0 = groups.get<0>();
    AtomIDGroup group1 = groups.get<1>();

    moldata.change(bnd, delta, group0, group1, weightfunc, anchors);
}

/** Overload of the function that uses a default WeightFunction */
void Molecule::change(const Bond &dih, const SireMaths::Angle &delta, const AtomIndexSet &anchors)
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
                      const WeightFunction &weightfunc, const AtomIndexSet &anchors)
{
    //split the molecule into two parts based on the improper
    boost::tuple<AtomIDGroup,AtomIDGroup> groups = splitMolecule(improper, connectivity());

    AtomIDGroup group0 = groups.get<0>();
    AtomIDGroup group1 = groups.get<1>();

    moldata.change(improper, delta, group0, group1, weightfunc, anchors);
}

/** Overload of the function that uses a default weight function

    \throw SireMol::missing_atom
    \throw SireMol::anchor_error
    \throw SireMol::ring_error
*/
void Molecule::change(const Improper &improper, const SireMaths::Angle &delta,
                      const AtomIndexSet &anchors)
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
                   const WeightFunction &weightfunc, const AtomIndexSet &anchors)
{
    change(bnd, val-measure(bnd), weightfunc, anchors);
}

/** Overload of the function that uses a default weight function

    \throw SireMol::missing_atom
    \throw SireMol::anchor_error
    \throw SireMol::ring_error
*/
void Molecule::set(const Bond &bnd, double val, const AtomIndexSet &anchors)
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
                   const WeightFunction &weightfunc, const AtomIndexSet &anchors)
{
    change(ang, val-measure(ang), weightfunc, anchors);
}

/** Overload of the function that uses a default weight function

    \throw SireMol::missing_atom
    \throw SireMol::anchor_error
    \throw SireMol::ring_error
*/
void Molecule::set(const SireMol::Angle &ang, const SireMaths::Angle &val,
                   const AtomIndexSet &anchors)
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
                   const WeightFunction &weightfunc, const AtomIndexSet &anchors)
{
    change(dih, val-measure(dih), weightfunc, anchors);
}

/** Overload of the function that uses a default weight function

    \throw SireMol::missing_atom
    \throw SireMol::anchor_error
    \throw SireMol::ring_error
*/
void Molecule::set(const Dihedral &dih, const SireMaths::Angle &val,
                   const AtomIndexSet &anchors)
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
                      const WeightFunction &weightfunc, const AtomIndexSet &anchors)
{
    change(dih, val-measure(dih), weightfunc, anchors);
}

/** Overload of the function that uses a default weight function

    \throw SireMol::missing_atom
    \throw SireMol::anchor_error
    \throw SireMol::ring_error
*/
void Molecule::setAll(const Dihedral &dih, const SireMaths::Angle &val,
                      const AtomIndexSet &anchors)
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
                   const WeightFunction &weightfunc, const AtomIndexSet &anchors)
{
    change(improper, size-measure(improper), weightfunc, anchors);
}

/** Overload of the function that uses a default weight function

    \throw SireMol::missing_atom
    \throw SireMol::anchor_error
    \throw SireMol::ring_error
*/
void Molecule::set(const Improper &improper, const SireMaths::Angle &size,
                   const AtomIndexSet &anchors)
{
    change(improper, size-measure(improper), anchors);
}

/////////////////////////////////////////////////////////
