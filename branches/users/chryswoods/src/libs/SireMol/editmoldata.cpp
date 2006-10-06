/**
  * @file
  *
  * C++ Implementation: EditMolData
  *
  * Description:
  * Implementation of EditMolData, and private EditMolData classes,
  * EditMolData_AtomData and EditMolData_ResData
  *
  * Author: Christopher Woods, (C) 2006
  *
  * Copyright: See COPYING file that comes with this distribution
  *
  */

#include "qhash_siremol.h"

#include "atomnum.h"
#include "cgatomid.h"
#include "cgnumatomid.h"
#include "resnumatomid.h"
#include "residatomid.h"

#include "atom.h"
#include "cutgroup.h"
#include "moleculeinfo.h"

#include "bond.h"
#include "angle.h"
#include "dihedral.h"
#include "improper.h"

#include "atomidgroup.h"

#include "editmoldata.h"

#include "weightfunction.h"

#include "editmol.h"
#include "editres.h"

#include "SireVol/coordgroup.h"

#include "SireMaths/vector.h"
#include "SireMaths/quaternion.h"
#include "SireMaths/matrix.h"


namespace SireMol
{
    /** Small internal class used by EditMolData to hold the
        information about an individual atom in the molecule */
    class EditMolData_AtomData
    {
    public:
        /** Null constructor */
        EditMolData_AtomData() : atomnum(0), cgnum(0), element(0), coords(0)
        {}

        /** Construct from a passed atom and CutGroup number */
        EditMolData_AtomData(const Atom &atom, CutGroupNum num)
                               : atomnum(atom.number()), cgnum(num),
                                 element(atom.element()), coords(atom.vector())
        {}

        /** Copy constructor */
        EditMolData_AtomData(const EditMolData_AtomData &other)
                  : atomnum(other.atomnum),
                    cgnum(other.cgnum),
                    element(other.element),
                    coords(other.coords)
        {}

        /** Destructor */
        ~EditMolData_AtomData()
        {}

        /** Assignment operator */
        EditMolData_AtomData& operator=(const EditMolData_AtomData &other)
        {
            atomnum = other.atomnum;
            cgnum = other.cgnum;
            element = other.element;
            coords = other.coords;
            return *this;
        }

        /** Comparison operator */
        bool operator==(const EditMolData_AtomData &other) const
        {
            return atomnum == other.atomnum and
                   cgnum == other.cgnum and
                   element == other.element and
                   coords == other.coords;
        }

        /** Comparison operator */
        bool operator!=(const EditMolData_AtomData &other) const
        {
            return atomnum != other.atomnum or
                   cgnum != other.cgnum or
                   element != other.element or
                   coords != other.coords;
        }

        /** The user-supplied atom number */
        AtomNum atomnum;

        /** The user-supplied number of the CutGroup that this
            atom is within */
        CutGroupNum cgnum;

        /** The chemical element of this atom */
        Element element;

        /** The coordinates of this atom */
        Vector coords;
    };

    /** Small internal class used by EditMolData to hold information
        about each residue. */
    class EditMolData_ResData
    {
    public:
        /** Empty constructor */
        EditMolData_ResData()
        {}

        /** Copy constructor */
        EditMolData_ResData(const EditMolData_ResData &other)
                 : resname(other.resname),
                   atoms(other.atoms),
                   atomnames(other.atomnames)
        {}

        /** Destructor */
        ~EditMolData_ResData()
        {}

        /** Assignment operator */
        EditMolData_ResData& operator=(const EditMolData_ResData &other)
        {
            resname = other.resname;
            atoms = other.atoms;
            atomnames = other.atomnames;
            return *this;
        }

        /** Comparison operator */
        bool operator==(const EditMolData_ResData &other) const
        {
            return resname == other.resname and
                   atomnames == other.atomnames and
                   atoms == other.atoms;
        }

        /** Comparison operator */
        bool operator!=(const EditMolData_ResData &other) const
        {
            return resname != other.resname or
                   atomnames != other.atomnames or
                   atoms != other.atoms;
        }

        /** The name of the residue */
        QString resname;

        /** The data for the atoms in this residue, indexed
            by their name */
        QHash<QString,EditMolData_AtomData> atoms;

        /** The names of the atoms in this residue in the
            order in which they were added to this residue */
        QStringList atomnames;
    };
}

#include "SireError/errors.h"
#include "SireMol/errors.h"

#include "SireMaths/vector.h"
#include "SireMaths/quaternion.h"
#include "SireMaths/matrix.h"
#include "SireMaths/line.h"
#include "SireMaths/triangle.h"
#include "SireMaths/torsion.h"
#include "SireMaths/rotate.h"

#include "SireStream/datastream.h"
using namespace SireStream;
using namespace SireMol;

static const RegisterMetaType<EditMolData_AtomData> r_atomdata(
                                      "SireMol::EditMolData_AtomData", MAGIC_ONLY);

/** Serialise to a binary data stream */
QDataStream &operator<<(QDataStream &ds, const EditMolData_AtomData &atomdata)
{
    writeHeader(ds, r_atomdata, 1)
              << atomdata.atomnum << atomdata.cgnum
              << atomdata.element << atomdata.coords;

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream &operator>>(QDataStream &ds, EditMolData_AtomData &atomdata)
{
    VersionID v = readHeader(ds, r_atomdata);

    if (v == 1)
    {
        ds >> atomdata.atomnum >> atomdata.cgnum
           >> atomdata.element >> atomdata.coords;
    }
    else
        throw version_error(v, "1", r_atomdata, CODELOC);

    return ds;
}

static const RegisterMetaType<EditMolData_ResData> r_resdata(
                                      "SireMol::EditMolData_ResData", MAGIC_ONLY);

/** Serialise to a binary data stream */
QDataStream &operator<<(QDataStream &ds, const EditMolData_ResData &resdata)
{
    writeHeader(ds, r_resdata, 1)
              << resdata.resname << resdata.atomnames << resdata.atoms;

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream &operator>>(QDataStream &ds, EditMolData_ResData &resdata)
{
    VersionID v = readHeader(ds, r_resdata);

    if (v == 1)
    {
        ds >> resdata.resname >> resdata.atomnames >> resdata.atoms;
    }
    else
        throw version_error(v, "1", r_resdata, CODELOC);

    return ds;
}

static const RegisterMetaType<EditMolData> r_editmoldata("SireMol::EditMolData");

/** Serialise an EditMolData to a datastream */
QDataStream& operator<<(QDataStream &ds, const EditMolData &mol)
{
    writeHeader(ds, r_editmoldata, 1) << mol.molnme
                                      << mol.atms << mol.resnums << mol.cgatoms
                                      << mol.cgnums << mol.molbnds;

    return ds;
}

/** Deserialise an EditMolData from a datastream */
QDataStream& operator>>(QDataStream &ds, EditMolData &mol)
{
    VersionID v = readHeader(ds, r_editmoldata);

    if (v == 1)
    {
        ds >> mol.molnme
           >> mol.atms >> mol.resnums >> mol.cgatoms
           >> mol.cgnums >> mol.molbnds;
    }
    else
        throw version_error(v, "1", r_editmoldata, CODELOC);

    return ds;
}

static const QSharedDataPointer<EditMolData> shared_null_ptr( new EditMolData() );

QSharedDataPointer<EditMolData> shared_null()
{
    return shared_null_ptr;
}

/** Create a new EditMol called 'molname' */
EditMolData::EditMolData(const QString &molname)
            : QSharedData(), molnme(molname)
{}

/** Copy constructor */
EditMolData::EditMolData(const EditMolData &other)
            : QSharedData(),
              molnme(other.molnme), atms(other.atms),
              resnums(other.resnums), cgatoms(other.cgatoms),
              cgnums(other.cgnums), molbnds(other.molbnds)
{}

/** Destructor */
EditMolData::~EditMolData()
{}


//////// Unsafe functions ///////////////////////
/** Return a reference to the ResData that contains the information for
    the residue with number 'resnum' - this does not check whether this is
    a valid residue number. */
EditMolData_ResData& EditMolData::_unsafe_resdata(ResNum resnum)
{
    return atms.find(resnum).value();
}

/** Return a reference to the ResData that contains the information for
    the residue with number 'resnum' - this does not check whether this is
    a valid residue number. */
const EditMolData_ResData& EditMolData::_unsafe_resdata(ResNum resnum) const
{
    return atms.find(resnum).value();
}

/** Return a reference to the AtomData for the atom 'atom' - this does
    not check whether there is such an atom in this molecule */
EditMolData_AtomData& EditMolData::_unsafe_atomdata(const AtomIndex &atom)
{
    return this->_unsafe_resdata(atom.resNum()).atoms.find(atom.name()).value();
}

/** Return a reference to the AtomData for the atom 'atom' - this does
    not check whether there is such an atom in this molecule */
const EditMolData_AtomData& EditMolData::_unsafe_atomdata(const AtomIndex &atom) const
{
    return this->_unsafe_resdata(atom.resNum()).atoms.find(atom.name()).value();
}

/** Return a copy of the atom at index 'atm' - this does not check
    if this is a valid index */
Atom EditMolData::_unsafe_atom(const AtomIndex &atm) const
{
    const EditMolData_AtomData &atomdata = this->_unsafe_atomdata(atm);

    return Atom(atomdata.atomnum, atm, atomdata.element, atomdata.coords);
}

/** Return a reference to the coordinates of the atom 'atom' - this
    does not check whether this atom is actually in this molecule! */
const Vector& EditMolData::_unsafe_coordinates(const AtomIndex &atom) const
{
    const EditMolData_AtomData &atomdata = this->_unsafe_atomdata(atom);

    return atomdata.coords;
}

/////////////////////////////////////////////////


/** @name Assertations
    Assertations that will throw an exception if not true
*/
/////////////////////////////////////////////////////////
//@{

/** Assert that the residue with number 'resnum' exists in this molecule

    \throw SireMol::missing_residue
*/
void EditMolData::assertResidueExists(ResNum resnum) const
{
    if (not resnums.contains(resnum))
        throw SireMol::missing_residue( QObject::tr(
                "There is no residue with number \"%1\" in the molecule \"%2\"")
                    .arg(resnum).arg(molnme), CODELOC );
}

/** Assert that the residue with index 'resid' exists in this molecule

    \throw SireError::invalid_index
*/
void EditMolData::assertResidueExists(ResID resid) const
{
    if (resid >= resnums.count())
        throw SireError::invalid_index( QObject::tr(
                "There is no residue with index \"%1\" in the molecule \"%2\" "
                "(nResidues() == %3)")
                    .arg(resid).arg(molnme).arg(resnums.count()), CODELOC );
}

/** Assert that the residue with number 'resnum' does *not* exist.

    \throw SireMol::duplicate_residue
*/
void EditMolData::assertResidueNotExists(ResNum resnum) const
{
    if (resnums.contains(resnum))
        throw SireMol::duplicate_residue( QObject::tr(
                "The molecule \"%1\" already contains a residue with number \"%2\" "
                "(called \"%3\")")
                    .arg(molnme).arg(resnum).arg(residueName(resnum)), CODELOC );
}

/** Assert that the CutGroup with ID == 'cgid' exists in this molecule

    \throw SireMol::missing_cutgroup
*/
void EditMolData::assertCutGroupExists(CutGroupID cgid) const
{
    if (cgid >= cgnums.count())
        throw SireMol::missing_cutgroup( QObject::tr(
              "There is no CutGroup with CutGroupID == \"%1\" in the molecule "
              "\"%2\" (nCutGroups() == %3)")
                  .arg(cgid).arg(molnme).arg(cgnums.count()), CODELOC );
}

/** Assert that the CutGroup with number 'cgnum' exists in this molecule

    \throw SireMol::missing_cutgroup
*/
void EditMolData::assertCutGroupExists(CutGroupNum cgnum) const
{
    if (not cgatoms.contains(cgnum))
        throw SireMol::missing_cutgroup( QObject::tr(
              "There is no CutGroup with CutGroupNum == \"%1\" in the molecule "
              "\"%2\"")
                  .arg(cgnum).arg(molnme), CODELOC );
}

/** Assert that the EditMol contains the atom with index 'atom'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void EditMolData::assertAtomExists(const AtomIndex &atom) const
{
    this->assertResidueExists(atom.resNum());

    if ( not this->_unsafe_resdata(atom.resNum()).atoms.contains(atom.name()) )
        throw SireMol::missing_atom( QObject::tr(
                "There is no atom with name \"%1\" in residue \"%2\" in molecule \"%3\"")
                    .arg(atom.name()).arg(atom.resNum()).arg(molnme), CODELOC );
}

/** Assert that all of the named atoms in 'atoms' exist in the residue with
    number 'resnum'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void EditMolData::assertAtomsExist(ResNum resnum, const QStringList &atoms) const
{
    this->assertResidueExists(resnum);

    const EditMolData_ResData &resdata = this->_unsafe_resdata(resnum);

    foreach (QString atom, atoms)
    {
        if (not resdata.atoms.contains(atom))
            throw SireMol::missing_atom( QObject::tr(
                "There is no atom with name \"%1\" in residue \"%2\" in molecule \"%3\"")
                    .arg(atom).arg(resnum).arg(molnme), CODELOC );
    }
}

/** Assert that all of the indexed atoms in 'atoms' exist in the residue with
    number 'resnum'

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
void EditMolData::assertAtomsExist(ResNum resnum, const QSet<AtomID> &atoms) const
{
    int nats = this->nAtoms(resnum);

    for (QSet<AtomID>::const_iterator it = atoms.begin();
         it != atoms.end();
         ++it)
    {
        if (*it >= nats)
            throw SireError::invalid_index( QObject::tr(
                    "There is no atom with index \"%1\" in the residue %2(%3) "
                    "in the molecule \"%4\" (nAtoms(%3) == %5)")
                        .arg(*it).arg(residueName(resnum)).arg(resnum)
                        .arg(molnme).arg(nats), CODELOC );
    }
}

/** Return the name of the residue with number 'resnum'

    \throw SireMol::missing_residue
*/
QString EditMolData::residueName(ResNum resnum) const
{
    this->assertResidueExists(resnum);
    return this->_unsafe_resdata(resnum).resname;
}

/** Return the residue number of the residue at index 'resid'

    \throw SireError::invalid_index
*/
ResNum EditMolData::residueNumber(ResID resid) const
{
    this->assertResidueExists(resid);
    return resnums[resid];
}

/** Return the CutGroupNum of the CutGroup with ID == 'cgid'

    \throw SireMol::missing_cutgroup
*/
CutGroupNum EditMolData::cutGroupNum(CutGroupID cgid) const
{
    this->assertCutGroupExists(cgid);
    return cgnums[cgid];
}

/** Return the numbers of all CutGroups that contain atoms from
    the residue with number 'resnum'

    \throw SireMol::missing_residue
*/
QSet<CutGroupNum> EditMolData::cutGroupNums(ResNum resnum) const
{
    this->assertResidueExists(resnum);

    const EditMolData_ResData &resdata = this->_unsafe_resdata(resnum);

    QSet<CutGroupNum> cgnums;

    for (QHash<QString,EditMolData_AtomData>::const_iterator it = resdata.atoms.begin();
         it != resdata.atoms.end();
         ++it)
    {
        cgnums.insert( it->cgnum );
    }

    return cgnums;
}

/** Return the total number of atoms in this molecule */
int EditMolData::nAtoms() const
{
    int nats = 0;

    foreach (ResNum resnum, resnums)
    {
        nats += atms.find(resnum)->atoms.count();
    }

    return nats;
}

/** Return the number of atoms in the residue with number 'resnum'

    \throw SireMol::missing_residue
*/
int EditMolData::nAtoms(ResNum resnum) const
{
    this->assertResidueExists(resnum);

    return this->_unsafe_resdata(resnum).atoms.count();
}

/** Return the number of atoms in the CutGroup with number 'resnum'

    \throw SireMol::missing_cutgroup
*/
int EditMolData::nAtoms(CutGroupNum cgnum) const
{
    this->assertCutGroupExists(cgnum);

    return cgatoms.find(cgnum)->count();
}

/** Return the total number of residues in this molecule */
int EditMolData::nResidues() const
{
    return resnums.count();
}

/** Return the total number of CutGroups in this molecule */
int EditMolData::nCutGroups() const
{
    return cgnums.count();
}

/** Assert that the total number of atoms in this molecule equals 'nats'

    \throw SireError::incompatible_error
*/
void EditMolData::assertNAtoms(int nats) const
{
    if (this->nAtoms() != nats)
        throw SireError::incompatible_error( QObject::tr(
                "The number of atoms in the molecule \"%1\" is equal to %2, while "
                "the requested number of atoms is %3.")
                    .arg(molnme).arg(nAtoms()).arg(nats), CODELOC );
}

/** Assert that the total number of atoms in the residue with number 'resnum'
    is 'nats'

    \throw SireMol::missing_residue
    \throw SireError::incompatible_error
*/
void EditMolData::assertNAtoms(ResNum resnum, int nats) const
{
    if (this->nAtoms(resnum) != nats)
        throw SireError::incompatible_error( QObject::tr(
                "The requested number of atoms in the residue %1(%2), in molecule "
                "\"%3\", is %4, while the requested number of atoms is %5.")
                    .arg(residueName(resnum)).arg(resnum).arg(molnme)
                    .arg(nAtoms(resnum)).arg(nats), CODELOC );
}

/** Assert that the total number of atoms in the CutGroup with number 'cgnum'
    is equal to 'nats'

    \throw SireMol::missing_cutgroup
*/
void EditMolData::assertNAtoms(CutGroupNum cgnum, int nats) const
{
    if (this->nAtoms(cgnum) != nats)
        throw SireError::incompatible_error( QObject::tr(
                "The requested number of atoms in the CutGroup with number '%1', "
                "in molecule \"%2\", is %3, while the requested number of atoms is %4.")
                    .arg(cgnum).arg(molnme).arg(nAtoms(cgnum)).arg(nats),
                        CODELOC );
}

/** Return whether or not this is an empty molecule
    (no atoms) */
bool EditMolData::isEmpty() const
{
    if (not atms.isEmpty())
    {
        for (QHash<ResNum,EditMolData_ResData>::const_iterator it = atms.begin();
             it != atms.end();
             ++it)
        {
            if (not it->atoms.isEmpty())
                return false;
        }
    }

    return true;
}

/** Return whether or not the residue with number 'resnum' is empty

    \throw SireMol::missing_residue
*/
bool EditMolData::isEmpty(ResNum resnum) const
{
    this->assertResidueExists(resnum);
    return this->_unsafe_resdata(resnum).atoms.isEmpty();
}

/** Return whether or not the CutGroup with number 'cgnum'
    is empty

    \throw SireMol::missing_cutgroup
*/
bool EditMolData::isEmpty(CutGroupNum cgnum) const
{
    this->assertCutGroupExists(cgnum);
    return cgatoms.find(cgnum)->isEmpty();
}

/** Assert that the EditMol contains an atom at index 'atomid'

    \throw SireError::invalid_index
*/
void EditMolData::assertAtomExists(AtomID atomid) const
{
    int nats = this->nAtoms();

    if ( atomid >= nats )
        throw SireError::invalid_index( QObject::tr(
                "There is no atom at index \"%1\" in the molecule \"%2\" "
                "(nAtoms() == %3)")
                    .arg(atomid).arg(molnme).arg(nats), CODELOC );
}

/** Assert that the EditMol contains an atom at index 'cgatomid'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
void EditMolData::assertAtomExists(const CGAtomID &cgatomid) const
{
    int nats = cgatoms.find(this->cutGroupNum(cgatomid.cutGroupID()))->count();

    if (cgatomid.atomID() >= nats)
        throw SireError::invalid_index( QObject::tr(
                "There is no atom at index \"%1\" in the CutGroup with "
                "CutGroupID == \"%2\" in the molecule \"%3\" "
                "(nAtoms(%2) == %4)")
                    .arg(cgatomid.atomID()).arg(cgatomid.cutGroupID())
                    .arg(molnme).arg(nats), CODELOC );
}

/** Assert that the EditMol contains an atom at index 'cgatomid'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
void EditMolData::assertAtomExists(const CGNumAtomID &cgatomid) const
{
    this->assertCutGroupExists(cgatomid.cutGroupNum());

    int nats = cgatoms.find(cgatomid.cutGroupNum())->count();

    if (cgatomid.atomID() >= nats)
        throw SireError::invalid_index( QObject::tr(
                "There is no atom at index \"%1\" in the CutGroup with "
                "CutGroupNum == \"%2\" in the molecule \"%3\" "
                "(nAtoms(%2) == %4)")
                    .arg(cgatomid.atomID()).arg(cgatomid.cutGroupNum())
                    .arg(molnme).arg(nats), CODELOC );
}

/** Assert that the EditMol contains an atom at index 'resatomid'

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
void EditMolData::assertAtomExists(const ResNumAtomID &resatomid) const
{
    this->assertResidueExists(resatomid.resNum());

    int nats = this->_unsafe_resdata(resatomid.resNum()).atomnames.count();

    if (resatomid.atomID() >= nats)
        throw SireError::invalid_index( QObject::tr(
              "There is no atom at index \"%1\" in the residue with "
              "number \"%2\" in the molecule \"%3\" "
              "(nAtoms(%2) == %4)")
                  .arg(resatomid.atomID()).arg(resatomid.resNum())
                  .arg(molnme).arg(nats), CODELOC );
}

/** Assert tha the EditMol contains an atom at index 'resatomid'

    \throw SireError::invalid_index
*/
void EditMolData::assertAtomExists(const ResIDAtomID &resatomid) const
{
    int nats = this->_unsafe_resdata(this->residueNumber(resatomid.resID()))
                                                                  .atomnames.count();

    if (resatomid.atomID() >= nats)
        throw SireError::invalid_index( QObject::tr(
              "There is no atom at index \"%1\" in the residue at "
              "index \"%2\" in the molecule \"%3\" "
              "(nAtoms(%2) == %4)")
                  .arg(resatomid.atomID()).arg(resatomid.resID())
                  .arg(molnme).arg(nats), CODELOC );
}

/////////////////////////////////////////////////////////
//@}

/** Assignment operator */
EditMolData& EditMolData::operator=(const EditMolData &other)
{
    molnme = other.molnme;
    atms = other.atms;
    resnums = other.resnums;
    cgatoms = other.cgatoms;
    cgnums =  other.cgnums;
    molbnds = other.molbnds;

    return *this;
}

/** Comparison operator */
bool EditMolData::operator==(const EditMolData &other) const
{
    return (this == &other) or
           ( molnme == other.molnme and
             atms == other.atms and
             resnums == other.resnums and
             cgatoms == other.cgatoms and
             cgnums == other.cgnums and
             molbnds == other.molbnds );
}

/** Comparison operator */
bool EditMolData::operator!=(const EditMolData &other) const
{
    return not (*this == other);
}

// Molecule EditMolData::commit() const
// {
// }

/** @name Indexing operators
    These indexing operators allow conversion from general indicies
    to those used internally by EditMolData (namely AtomIndex,
    ResNum, AtomName, CutGroupNum)
*/
/////////////////////////////////////////////////////////
//@{

/** Return the AtomIndex associated with the AtomIndex 'atm'.
    This just checks that 'atm' is valid in this molecule
    and then returns it - this is a useful function that
    allows template functions to work with AtomIndex as
    well as all of the other index types

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
AtomIndex EditMolData::operator[](const AtomIndex &atm) const
{
    this->assertAtomExists(atm);
    return atm;
}

/** Return the AtomIndex of the atom at index 'atomid'

    \throw SireError::invalid_index
*/
AtomIndex EditMolData::operator[](AtomID atomid) const
{
    this->assertAtomExists(atomid);

    //the total number of atoms passed so far...
    int nats = 0;

    //loop through each residue, in order
    for (QList<ResNum>::const_iterator it = resnums.begin();
         it != resnums.end();
         ++it)
    {
        //get the data for this residue
        const EditMolData_ResData &resdata = this->_unsafe_resdata(*it);

        const QStringList &atomnames = resdata.atomnames;

        //add the number of atoms in this residue onto the total
        nats += atomnames.count();

        //is 'nats' greater than 'atomid' - if so then the atom
        //with this index must be in this residue
        if (atomid < nats)
        {
            //the atom is in this residue - convert 'nats' into an
            //index into this residue
            return AtomIndex( atomnames[ atomnames.count() - nats + atomid ],
                              *it );
        }
    }

    //we should never get here!
    throw SireError::program_bug( QObject::tr(
                                    "We should never get here!"), CODELOC );

    return AtomIndex();
}

/** Return the AtomIndex of the atom at index 'resatomid'

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
AtomIndex EditMolData::operator[](const ResNumAtomID &resatomid) const
{
    this->assertAtomExists(resatomid);

    return AtomIndex( this->_unsafe_resdata(resatomid.resNum())
                                .atomnames[resatomid.atomID()],
                      resatomid.resNum() );
}

/** Return the AtomIndex of the atom at index 'resatomid'

    \throw SireError::invalid_index
*/
AtomIndex EditMolData::operator[](const ResIDAtomID &resatomid) const
{
    ResNum resnum = this->residueNumber(resatomid.resID());

    return AtomIndex( this->_unsafe_resdata(resnum).atomnames[resatomid.atomID()],
                      resnum );
}

/** Return the AtomIndex of the atom at index 'cgatomid'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
AtomIndex EditMolData::operator[](const CGAtomID &cgatomid) const
{
    this->assertAtomExists(cgatomid);

    CutGroupNum cgnum = this->cutGroupNum(cgatomid.cutGroupID());

    return cgatoms.find(cgnum)->at(cgatomid.atomID());
}

/** Return the AtomIndex of the atom at index 'cgatomid'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
AtomIndex EditMolData::operator[](const CGNumAtomID &cgatomid) const
{
    this->assertAtomExists(cgatomid);

    return cgatoms.find(cgatomid.cutGroupNum())->at(cgatomid.atomID());
}

/** Return the residue number of the residue at index 'resid'

    \throw SireError::invalid_index
*/
ResNum EditMolData::operator[](ResID resid) const
{
    return this->residueNumber(resid);
}

/** Return the ResNum associated with the ResNum 'resnum'.
    This just checks that 'resnum' is valid in this molecule
    and then returns it - this is a useful function that
    allows template functions to work with ResNum as
    well as all of the other index types

    \throw SireMol::missing_residue
*/
ResNum EditMolData::operator[](ResNum resnum) const
{
    this->assertResidueExists(resnum);
    return resnum;
}

/** Return the CutGroupNum of the CutGroup with ID == cgid

    \throw SireMol::missing_cutgroup
*/
CutGroupNum EditMolData::operator[](CutGroupID cgid) const
{
    return this->cutGroupNum(cgid);
}

/** Return the CutGroupNum associated with the CutGroupNum 'cgnum'.
    This just checks that 'cgnum' is valid in this molecule
    and then returns it - this is a useful function that
    allows template functions to work with CutGroupNum as
    well as all of the other index types

    \throw SireMol::missing_cutgroup
*/
CutGroupNum EditMolData::operator[](CutGroupNum cgnum) const
{
    this->assertCutGroupExists(cgnum);
    return cgnum;
}

/////////////////////////////////////////////////////////
//@}


///// Querying the molecule /////////////////////////////

/** Synonym for operator[](CutGroupID)

    \throw SireMol::missing_cutgroup
*/
CutGroupNum EditMolData::at(CutGroupID cgid) const
{
    return this->operator[](cgid);
}

/** Synonym for operator[](CutGroupNum)

    \throw SireMol::missing_cutgroup
*/
CutGroupNum EditMolData::at(CutGroupNum cgnum) const
{
    return this->operator[](cgnum);
}

/** Synonym for operator[](ResID)

    \throw SireError::invalid_index
*/
ResNum EditMolData::at(ResID resid) const
{
    return this->operator[](resid);
}

/** Synonym for operator[](ResNum)

    \throw SireMol::missing_residue
*/
ResNum EditMolData::at(ResNum resnum) const
{
    return this->operator[](resnum);
}

/** Synonym for operator[](AtomIndex)

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
AtomIndex EditMolData::at(const AtomIndex &atom) const
{
    return this->operator[](atom);
}

/** Synonym for operator[](AtomID)

    \throw SireError::invalid_index
*/
AtomIndex EditMolData::at(AtomID atomid) const
{
    return this->operator[](atomid);
}

/** Synonym for operator[](ResNumAtomID)

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
AtomIndex EditMolData::at(const ResNumAtomID &resatomid) const
{
    return this->operator[](resatomid);
}

/** Synonym for operator[](ResIDAtomID)

    \throw SireError::invalid_index
*/
AtomIndex EditMolData::at(const ResIDAtomID &resatomid) const
{
    return this->operator[](resatomid);
}

/** Synonym for operator[](CGAtomID)

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
AtomIndex EditMolData::at(const CGAtomID &cgatomid) const
{
    return this->operator[](cgatomid);
}

/** Synonym for operator[](CGNumAtomID)

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
AtomIndex EditMolData::at(const CGNumAtomID &cgatomid) const
{
    return this->operator[](cgatomid);
}

/** Return whether or not the molecule contains a CutGroup with
    number 'cgnum' */
bool EditMolData::contains(CutGroupNum cgnum) const
{
    return cgatoms.contains(cgnum);
}

/** Return whether or not the molecule contains a CutGroup at
    index 'cgid' */
bool EditMolData::contains(CutGroupID cgid) const
{
    return cgnums.count() < cgid;
}

/** Return whether or not the molecule contains a residue
    with number 'resnum' */
bool EditMolData::contains(ResNum resnum) const
{
    return atms.contains(resnum);
}

/** Return whether or not the molecule contains a residue
    at index 'resid' */
bool EditMolData::contains(ResID resid) const
{
    return resnums.count() < resid;
}

/** Return whether or not the molecule contains the atom 'atom' */
bool EditMolData::contains(const AtomIndex &atom) const
{
    return this->contains(atom.resNum()) and
                this->_unsafe_resdata(atom.resNum()).atoms.contains(atom.name());
}

/** Return whether or not the molecule contains the atom 'atom' */
bool EditMolData::contains(const CGAtomID &atom) const
{
    return this->contains(atom.cutGroupID()) and
           cgatoms.find( cgnums[atom.cutGroupID()] )->count() < atom.atomID();
}

/** Return whether or not the molecule contains the atom 'atom' */
bool EditMolData::contains(const CGNumAtomID &atom) const
{
    return this->contains(atom.cutGroupNum()) and
           cgatoms.find( atom.cutGroupNum() )->count() < atom.atomID();
}

/** Return whether or not the molecule contains the atom 'atom' */
bool EditMolData::contains(const ResNumAtomID &atom) const
{
    return this->contains(atom.resNum()) and
           this->_unsafe_resdata(atom.resNum()).atomnames.count() < atom.atomID();
}

/** Return whether or not the molecule contains the atom 'atom' */
bool EditMolData::contains(const ResIDAtomID &atom) const
{
    return this->contains(atom.resID()) and
           this->_unsafe_resdata(resnums[atom.resID()])
                              .atomnames.count() < atom.atomID();
}

/** Return the connectivity of this molecule */
const MoleculeBonds& EditMolData::connectivity() const
{
    return molbnds;
}

/** Return the connectivity of the residue with number 'resnum' */
ResidueBonds EditMolData::connectivity(ResNum resnum) const
{
    this->assertResidueExists(resnum);
    return molbnds.residue(resnum);
}

/** Return the metadata for this molecule - this will be slow
    as it involves copying all of the data for this molecule */
// MoleculeInfo EditMolData::info() const
// {
// }

/** Internal function used to return a copy of the CutGroup with number
    'cgnum' */
CutGroup EditMolData::_unsafe_cutGroup(CutGroupNum cgnum) const
{
    const QList<AtomIndex> &atoms = cgatoms.find(cgnum).value();

    //how many atoms are in this CutGroup?
    int nats = atoms.count();

    QVector<AtomInfo> atominfos;
    QVector<Vector> coords;

    atominfos.reserve(nats);
    coords.reserve(nats);

    for (int i=0; i<nats; ++i)
    {
        //get a copy of the atom
        Atom atom = this->_unsafe_atom(atoms[i]);

        //split it into the info group and coord group
        atominfos.append(atom);
        coords.append(atom);
    }

    return CutGroup( AtomInfoGroup(atominfos), CoordGroup(coords) );
}

/** Internal function used to return a copy of the CoordGroup containing
    the coordinates of the CutGroup with number 'cgnum' */
CoordGroup EditMolData::_unsafe_coordGroup(CutGroupNum cgnum) const
{
    const QList<AtomIndex> &atoms = cgatoms.find(cgnum).value();

    //how many atoms are in this CutGroup?
    int nats = atoms.count();

    QVector<Vector> coords;
    coords.reserve(nats);

    for (int i=0; i<nats; ++i)
    {
        coords.append( this->_unsafe_coordinates(atoms[i]) );
    }

    return CoordGroup(coords);
}

/** Return an array of the CutGroups in this molecule, in the order
    that they appear in this molecule (CutGroupID order) */
QVector<CutGroup> EditMolData::cutGroups() const
{
    int ncg = cgnums.count();

    QVector<CutGroup> cgroups;
    cgroups.reserve(ncg);

    for (int i=0; i<ncg; ++i)
    {
        cgroups.append( this->_unsafe_cutGroup( cgnums[i] ) );
    }

    return cgroups;
}

/** Return copies of the CutGroups with numbers 'cgnums'

    \throw SireMol::missing_cutgroup
*/
QHash<CutGroupNum,CutGroup> EditMolData::cutGroups(const QSet<CutGroupNum> &cgnums) const
{
    QHash<CutGroupNum,CutGroup> cgroups;
    cgroups.reserve(cgnums.count());

    for (QSet<CutGroupNum>::const_iterator it = cgnums.begin();
         it != cgnums.end();
         ++it)
    {
        cgroups.insert( *it, this->cutGroup(*it) );
    }

    return cgroups;
}

/** Return a copy of the CutGroups that contain atoms in the residue with
    number 'resnum'

    \throw SireMol::missing_residue
*/
QHash<CutGroupNum,CutGroup> EditMolData::cutGroups(ResNum resnum) const
{
    QSet<CutGroupNum> cgnums = this->cutGroupNums(resnum);

    return this->cutGroups(cgnums);
}

/** Return a copy of the CutGroup with number 'cgnum'

    \throw SireMol::missing_cutgroup
*/
CutGroup EditMolData::cutGroup(CutGroupNum cgnum) const
{
    this->assertCutGroupExists(cgnum);
    return this->_unsafe_cutGroup(cgnum);
}

/** Return an array containing a copy of all of the CoordGroups
    for all of the CutGroups in this molecule, in the same order
    as the CutGroups appear in this molecule (CutGroupID order) */
QVector<CoordGroup> EditMolData::coordGroups() const
{
    int ncg = cgnums.count();

    QVector<CoordGroup> cgroups;
    cgroups.reserve(ncg);

    for (int i=0; i<ncg; ++i)
        cgroups.append( this->_unsafe_coordGroup(cgnums[i]) );

    return cgroups;
}

/** Return copies of the CoordGroups that hold the coordinates of the
    CutGroups whose numbers are in 'cgnums'

    \throw SireMol::missing_cutgroup
*/
QHash<CutGroupNum,CoordGroup> EditMolData::coordGroups(
                                      const QSet<CutGroupNum> &cgnums) const
{
    QHash<CutGroupNum,CoordGroup> cgroups;
    cgroups.reserve(cgnums.count());

    for (QSet<CutGroupNum>::const_iterator it = cgnums.begin();
         it != cgnums.end();
         ++it)
    {
        cgroups.insert( *it, this->coordGroup(*it) );
    }

    return cgroups;
}

/** Return copies of all CoordGroups that contain coordinates from atoms
    with residue number 'resnum'

    \throw SireMol::missing_residue
*/
QHash<CutGroupNum,CoordGroup> EditMolData::coordGroups(ResNum resnum) const
{
    return this->coordGroups( this->cutGroupNums(resnum) );
}

/** Return a copy of the CoordGroup for the CutGroup with number 'cgnum'

    \throw SireMol::missing_cutgroup
*/
CoordGroup EditMolData::coordGroup(CutGroupNum cgnum) const
{
    this->assertCutGroupExists(cgnum);
    return this->_unsafe_coordGroup(cgnum);
}

/** Return a copy of the atom with index 'atm'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
Atom EditMolData::atom(const AtomIndex &atm) const
{
    this->assertAtomExists(atm);
    return this->_unsafe_atom(atm);
}

/** Return a copy of the coordinates of the atom at index 'atm'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
Vector EditMolData::coordinates(const AtomIndex &atm) const
{
    this->assertAtomExists(atm);
    return this->_unsafe_coordinates(atm);
}

/** Return an array containing copies of all of the atoms in this molecule,
    in the same order as they appear in this molecule (AtomID order) */
QVector<Atom> EditMolData::atoms() const
{
    int nats = this->nAtoms();

    QVector<Atom> allatoms;
    allatoms.reserve(nats);

    int nres = this->nResidues();
    for (int i=0; i<nres; ++i)
    {
        ResNum resnum = resnums[i];

        const EditMolData_ResData &resdata = this->_unsafe_resdata(resnum);

        for (QStringList::const_iterator it = resdata.atomnames.begin();
             it != resdata.atomnames.end();
             ++it)
        {
            const EditMolData_AtomData &atomdata = resdata.atoms.find(*it).value();

            allatoms.append( Atom( atomdata.atomnum, AtomIndex(*it,resnum),
                                   atomdata.element, atomdata.coords ) );
        }
    }

    return allatoms;
}

/** Return an array containing copies of all of the coordinates of the
    atoms in this molecule, in the same order as they appear in this
    molecule (AtomID order) */
QVector<Vector> EditMolData::coordinates() const
{
    int nats = this->nAtoms();

    QVector<Vector> coords;
    coords.reserve(nats);

    int nres = this->nResidues();
    for (int i=0; i<nres; ++i)
    {
        ResNum resnum = resnums[i];

        const EditMolData_ResData &resdata = this->_unsafe_resdata(resnum);

        for (QStringList::const_iterator it = resdata.atomnames.begin();
             it != resdata.atomnames.end();
             ++it)
        {
            const EditMolData_AtomData &atomdata = resdata.atoms.find(*it).value();

            coords.append( atomdata.coords );
        }
    }

    return coords;
}

/** Return an array of all of the atoms in the CutGroup with number
    'cgnum', in the same order as the atoms appear in this CutGroup
    (AtomID order)

    \throw SireMol::missing_cutgroup
*/
QVector<Atom> EditMolData::atoms(CutGroupNum cgnum) const
{
    this->assertCutGroupExists(cgnum);

    const QList<AtomIndex> &cgatms = cgatoms.find(cgnum).value();

    QVector<Atom> allatoms;
    allatoms.reserve( cgatms.count() );

    for (QList<AtomIndex>::const_iterator it = cgatms.begin();
         it != cgatms.end();
         ++it)
    {
        allatoms.append( this->_unsafe_atom(*it) );
    }

    return allatoms;
}

/** Return an array of all of the atoms in the Residue with number
    'resnum', in the same order as the atoms appear in this residue
    (AtomID order)

    \throw SireMol::missing_residue
*/
QVector<Atom> EditMolData::atoms(ResNum resnum) const
{
    this->assertResidueExists(resnum);

    const EditMolData_ResData &resdata = this->_unsafe_resdata(resnum);

    QVector<Atom> allatoms;
    allatoms.reserve( resdata.atomnames.count() );

    for (QStringList::const_iterator it = resdata.atomnames.begin();
         it != resdata.atomnames.end();
         ++it)
    {
        const EditMolData_AtomData &atomdata =
                                  resdata.atoms.find(*it).value();

        allatoms.append( Atom(atomdata.atomnum,
                              AtomIndex(*it, resnum),
                              atomdata.element,
                              atomdata.coords) );
    }

    return allatoms;
}

/** Return an array of all of the coordinates of the atoms in the
    CutGroup with number 'cgnum', in the same order as the atoms
    appear in this CutGroup (AtomID order)

    \throw SireMol::missing_cutgroup
*/
QVector<Vector> EditMolData::coordinates(CutGroupNum cgnum) const
{
    this->assertCutGroupExists(cgnum);

    const QList<AtomIndex> &cgatms = cgatoms.find(cgnum).value();

    QVector<Vector> allcoords;
    allcoords.reserve( cgatms.count() );

    for (QList<AtomIndex>::const_iterator it = cgatms.begin();
         it != cgatms.end();
         ++it)
    {
        allcoords.append( this->_unsafe_coordinates(*it) );
    }

    return allcoords;
}

/** Return an array of all of the coordinates of all of the atoms in the
    Residue with number 'resnum', in the same order as the atoms appear
    in this residue (AtomID order)

    \throw SireMol::missing_residue
*/
QVector<Vector> EditMolData::coordinates(ResNum resnum) const
{
    this->assertResidueExists(resnum);

    const EditMolData_ResData &resdata = this->_unsafe_resdata(resnum);

    QVector<Vector> allcoords;
    allcoords.reserve( resdata.atomnames.count() );

    for (QStringList::const_iterator it = resdata.atomnames.begin();
         it != resdata.atomnames.end();
         ++it)
    {
        const EditMolData_AtomData &atomdata =
                                  resdata.atoms.find(*it).value();

        allcoords.append( atomdata.coords );
    }

    return allcoords;
}

/** Return the geometric line corresponding to the bond 'bnd'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
SireMaths::Line EditMolData::bond(const Bond &bnd) const
{
    return SireMaths::Line( this->coordinates(bnd.atom0()),
                            this->coordinates(bnd.atom1()) );
}

/** Return the geometric triangle corresponding to the angle 'ang'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
SireMaths::Triangle EditMolData::angle(const SireMol::Angle &ang) const
{
    return SireMaths::Triangle( this->coordinates(ang.atom0()),
                                this->coordinates(ang.atom1()),
                                this->coordinates(ang.atom2()) );
}

/** Return the geometric torsion corresponding to the dihedral 'dih'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
SireMaths::Torsion EditMolData::dihedral(const Dihedral &dih) const
{
    return SireMaths::Torsion( this->coordinates(dih.atom0()),
                               this->coordinates(dih.atom1()),
                               this->coordinates(dih.atom2()),
                               this->coordinates(dih.atom3()) );
}

/** Return the geometric torsion corresponding to the improper angle 'imp'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
SireMaths::Torsion EditMolData::improper(const Improper &imp) const
{
    return SireMaths::Torsion( this->coordinates(imp.atom0()),
                               this->coordinates(imp.atom1()),
                               this->coordinates(imp.atom2()),
                               this->coordinates(imp.atom3()) );
}

/** Return the length of the bond 'bnd'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
double EditMolData::measure(const Bond &bnd) const
{
    return bond(bnd).length();
}

/** Return the size of the angle 'ang'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
SireMaths::Angle EditMolData::measure(const SireMol::Angle &ang) const
{
    return angle(ang).angle();
}

/** Return the size of the dihedral 'dih'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
SireMaths::Angle EditMolData::measure(const Dihedral &dih) const
{
    return dihedral(dih).angle();
}

/** Return the size of the improper angle 'imp'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
SireMaths::Angle EditMolData::measure(const Improper &imp) const
{
    throw SireError::incomplete_code("Need to write this!", CODELOC);

    return 0.0;
}

/** Return the relative weights of 'group0' and 'group1' in this
    molecule.

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
double EditMolData::getWeight(const AtomIDGroup &group0, const AtomIDGroup &group1,
                              const WeightFunction &weightfunc) const
{
    //create a calculator workspace for the weight calculation
    WeightCalculator calc(weightfunc);

#warning EditMolData::getWeight() needs refactoring as it is inefficient!

    //add all of atoms in the groups to the calculator
    foreach (AtomIndex atm, group0.atoms())
    {
        calc.addToA( atom(atm) );
    }

    foreach (AtomIndex atm, group1.atoms())
    {
        calc.addToB( atom(atm) );
    }

    //now add all of the residues
    foreach (ResNum resnum, group0.residues())
    {
        calc.addToA( atoms(resnum) );
    }

    foreach (ResNum resnum, group1.residues())
    {
        calc.addToB( atoms(resnum) );
    }

    return calc.weight();
}

/** Set the name of this molecule */
void EditMolData::setName(const QString &name)
{
    molnme = name;
}

/** Set the name of the residue with number 'resnum'

    \throw SireMol::missing_residue
*/
void EditMolData::setResidueName(ResNum resnum, const QString &name)
{
    this->assertResidueExists(resnum);
    this->_unsafe_resdata(resnum).resname = name;
}

/** Clean this molecule - this removes any empty residues */
void EditMolData::clean()
{
    QList<ResNum> oldnums = resnums;

    foreach (ResNum resnum, oldnums)
    {
        if (this->isEmpty(resnum))
            this->remove(resnum);
    }

    QList<CutGroupNum> oldcgnums = cgnums;

    foreach (CutGroupNum cgnum, oldcgnums)
    {
        if (this->isEmpty(cgnum))
            this->remove(cgnum);
    }
}

/** Renumber the residue with current number 'resnum' to the new number
    'newresnum'. This will fail if there is already a residue with this
    number.

    \throw SireMol::missing_residue
    \throw SireMol::duplicate_residue
*/
void EditMolData::renumberResidue(ResNum oldnum, ResNum newnum)
{
    this->assertResidueExists(oldnum);
    this->assertResidueNotExists(newnum);

    //move the residue in the hash...
    EditMolData_ResData resdata = atms.take(oldnum);
    atms.insert( newnum, resdata );

    //renumber the residue in the list of residue numbers
    resnums.replace( resnums.indexOf(oldnum), newnum );

    //renumber the residue in the molecule's connectivity
    molbnds.renumber(oldnum, newnum);
}

/** Add the atom 'atom' to this molecule, and place it into the
    CutGroup with number 'cgnum'. This will add the residue and
    CutGroup if necessary. This will throw an exception if this
    atom already exists.

    \throw SireMol::duplicate_atom
*/
void EditMolData::add(const Atom &atom, CutGroupNum cgnum)
{
    ResNum resnum = atom.resNum();

    if ( atms.contains(resnum) )
    {
        const EditMolData_ResData &resdata = this->_unsafe_resdata(resnum);

        if ( resdata.atoms.contains(atom.name()) )
            throw SireMol::duplicate_atom( QObject::tr(
                    "Cannot add the atom %1 to the molecule \"%2\" as it "
                    "already contains an atom with this name in the residue "
                    "%3(%4)")
                        .arg(atom.toString(), molnme, resdata.resname)
                        .arg(resnum), CODELOC );
    }
    else
    {
        //the residue containing this atom doesn't exist -
        //add the residue
        atms.insert( resnum, EditMolData_ResData() );
        resnums.append(resnum);
    }

    //add the atom to the residue data
    EditMolData_ResData &resdata = this->_unsafe_resdata(resnum);

    resdata.atoms.insert( atom.name(), EditMolData_AtomData(atom, cgnum) );
    resdata.atomnames.append( atom.name() );

    //add this to the desired CutGroup
    if (not cgatoms.contains(cgnum))
    {
        cgatoms.insert(cgnum, QList<AtomIndex>());
        cgnums.append(cgnum);
    }

    cgatoms[cgnum].append(atom);
}

/** Add the atom 'atom' to this molecule. This will add the residue if
    necessary. This will throw an exception if this atom already exists.

    \throw SireMol::duplicate_atom
*/
void EditMolData::add(const Atom &atom)
{
    this->add( atom, cuttingfunc(atom, *this) );
}

/** Remove the atom 'atom' from this molecule. Note that removing
    the last atom from a residue or CutGroup *will not* remove
    the Residue or CutGroup. Use EditMolData::clean() if you
    want to remove any empty residues or CutGroups.

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void EditMolData::remove(const AtomIndex &atom)
{
    this->assertAtomExists(atom);

    //get the data for the residue in which this atom exists
    EditMolData_ResData &resdata = this->_unsafe_resdata(atom.resNum());

    //remove this atom from the CutGroup
    cgatoms[ resdata.atoms.find(atom.name())->cgnum ].removeAll(atom.name());

    //remove this atom from the bonding
    molbnds.remove(atom);

    //remove this atom
    resdata.atomnames.removeAll(atom.name());
    resdata.atoms.remove(atom.name());
}

/** Add the bond 'bnd' to this molecule - the atoms referred
    to in the bond must exist or else an exception will be thrown.
    This does nothing if this bond already exists.

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void EditMolData::add(const Bond &bond)
{
    this->assertAtomExists(bond.atom0());
    this->assertAtomExists(bond.atom1());

    molbnds.add(bond);
}

/** Remove the bond 'bond' from this molecule - this does
    nothing if the bond doesn't exist, but will throw an
    exception if either of the atoms referenced in the bond
    are not in this molecule

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void EditMolData::remove(const Bond &bond)
{
    this->assertAtomExists(bond.atom0());
    this->assertAtomExists(bond.atom1());

    molbnds.remove(bond);
}

/** Remove all bonds that involve the residue with number 'resnum'.

    \throw SireMol::missing_residue
*/
void EditMolData::removeAllBonds(ResNum resnum)
{
    this->assertResidueExists(resnum);
    molbnds.remove(resnum);
}

/** Remove all bonds that involve the atom 'atom'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void EditMolData::removeAllBonds(const AtomIndex &atom)
{
    this->assertAtomExists(atom);
    molbnds.remove(atom);
}

/** Remove all intra-residue bonds in the molecule with number 'resnum'

    \throw SireMol::missing_residue
*/
void EditMolData::removeAllIntraBonds(ResNum resnum)
{
    this->assertResidueExists(resnum);
    molbnds.removeIntra(resnum);
}

/** Remove all inter-residue bonds in the molecule with number 'resnum'

    \throw SireMol::missing_residue
*/
void EditMolData::removeAllInterBonds(ResNum resnum)
{
    this->assertResidueExists(resnum);
    molbnds.removeInter(resnum);
}

/** Remove all bonds from this molecule */
void EditMolData::removeAllBonds()
{
    molbnds.clear();
}

/** Add the residue with number 'resnum' and name 'resnam'. This
    will throw an exception if a residue with this number already
    exists.

    \throw SireMol::duplicate_residue
*/
void EditMolData::add(ResNum resnum, const QString &resnam)
{
    this->assertResidueNotExists(resnum);

    EditMolData_ResData resdata;
    resdata.resname = resnam;

    atms.insert( resnum, resdata );
    resnums.append(resnum);
}

/** Add the residue with number 'resnum' and name 'resnam' using
    the residue in 'tmpl' as a template.

    \throw SireMol::duplicate_residue
*/
void EditMolData::add(ResNum resnum, const QString &resnam, const EditRes &tmpl)
{
}

/** Add the residue with number 'resnum' to this molecule, using the
    residue in 'tmpl' as a template.

    \throw SireMol::duplicate_residue
*/
void EditMolData::add(ResNum resnum, const EditRes &tmpl)
{
    #warning Commented out here!
    //this->add(resnum, tmpl.name(), tmpl);
}

/** Remove the residue with number 'resnum' from this molecule.

    \throw SireMol::missing_residue
*/
void EditMolData::remove(ResNum resnum)
{
    //get the CutGroups that have atoms involving this residue
    QSet<CutGroupNum> rescgs = this->cutGroupNums(resnum);

    //remove the atoms from all of the affected CutGroups
    foreach (CutGroupNum cgnum, rescgs)
    {
        QList<AtomIndex> &cgroup = cgatoms.find(cgnum).value();

        QMutableListIterator<AtomIndex> it(cgroup);

        while (it.hasNext())
        {
            const AtomIndex &atm = it.next();

            if (atm.resNum() == resnum)
                it.remove();
        }
    }

    molbnds.remove(resnum);
    atms.remove(resnum);
    resnums.removeAll(resnum);
}

/** Remove the CutGroup with number 'cgnum' from this molecule
    (and all of the atoms that are contained in this CutGroup)

    \throw SireMol::missing_cutgroup
*/
void EditMolData::remove(CutGroupNum cgnum)
{
    this->assertCutGroupExists(cgnum);

    //remove all of the atoms in the CutGroup
    QList<AtomIndex> cgroup = cgatoms.find(cgnum).value();

    foreach (AtomIndex atom, cgroup)
    {
        this->remove(atom);
    }

    //now remove the CutGroup itself
    cgatoms.remove(cgnum);
    cgnums.removeAll(cgnum);
}

namespace SireMol { namespace detail {

/** Small internal class that is used to move the EditMolData atoms
    in a way that ensures that each move function is atomic
    (it either completes or it does nothing - it doesn't half-work)
*/
class MoveWorkspace
{
public:
    MoveWorkspace(const QHash<ResNum, EditMolData_ResData> &atoms)
            : atms(atoms)
    {}

    ~MoveWorkspace()
    {}

    const QHash<ResNum,EditMolData_ResData>& commit() const
    {
        return atms;
    }

    void setCoordinates(const QList<ResNum> resnums, const Vector *newcoords)
    {
        int i = 0;

        for (QList<ResNum>::const_iterator it = resnums.begin();
             it != resnums.end();
             ++it)
        {
            EditMolData_ResData &resdata = atms.find(*it).value();

            for (QStringList::const_iterator atom_it = resdata.atomnames.begin();
                 atom_it != resdata.atomnames.end();
                 ++atom_it)
            {
                resdata.atoms.find(*atom_it)->coords = newcoords[i];
                ++i;
            }
        }
    }

    void translate(const AtomIndex &atom, const Vector &delta)
    {
        atms.find(atom.resNum())     //ResData
                    ->atoms.find(atom.name())   //AtomData
                    ->coords += delta;    // Vector
    }

    void rotate(const AtomIndex &atom, const Matrix &rotmat,
                const Vector &point)
    {
        Vector &coords = atms.find(atom.resNum())
                             ->atoms.find(atom.name())
                             ->coords;

        coords = SireMaths::rotate(coords, rotmat, point);
    }

    void setCoordinates(const AtomIndex &atom, const Vector &newcoords)
    {
        atms.find(atom.resNum())    //ResData
                   ->atoms.find(atom.name())   //AtomData
                   ->coords = newcoords;     //Vector
    }

    void translate(ResNum resnum, const Vector &delta)
    {
        EditMolData_ResData &resdata = atms.find(resnum).value();

        for (QHash<QString,EditMolData_AtomData>::iterator it = resdata.atoms.begin();
             it != resdata.atoms.end();
             ++it)
        {
            it->coords += delta;
        }
    }

    void rotate(ResNum resnum, const Matrix &matrix, const Vector &point)
    {
        EditMolData_ResData &resdata = atms.find(resnum).value();

        for (QHash<QString,EditMolData_AtomData>::iterator it = resdata.atoms.begin();
             it != resdata.atoms.end();
             ++it)
        {
            Vector &coords = it->coords;
            coords = SireMaths::rotate(coords, matrix, point);
        }
    }

    void setCoordinates(ResNum resnum, const Vector *newcoords)
    {
        EditMolData_ResData &resdata = atms.find(resnum).value();

        int i=0;

        for (QStringList::const_iterator it = resdata.atomnames.begin();
             it != resdata.atomnames.end();
             ++it)
        {
            resdata.atoms.find(*it)->coords = newcoords[i];
            ++i;
        }
    }

    void translate(ResNum resnum, const QStringList &atomnames,
                   const Vector &delta)
    {
        EditMolData_ResData &resdata = atms.find(resnum).value();

        for (QStringList::const_iterator it = atomnames.begin();
             it != atomnames.end();
             ++it)
        {
            resdata.atoms.find(*it)->coords += delta;
        }
    }

    void rotate(ResNum resnum, const QStringList &atomnames,
                const Matrix &rotmat, const Vector &point)
    {
        EditMolData_ResData &resdata = atms.find(resnum).value();

        for (QStringList::const_iterator it = atomnames.begin();
             it != atomnames.end();
             ++it)
        {
            Vector &coords = resdata.atoms.find(*it)->coords;
            coords = SireMaths::rotate(coords, rotmat, point);
        }
    }

    void translate(ResNum resnum, const QSet<AtomID> &atomids,
                   const Vector &delta)
    {
        EditMolData_ResData &resdata = atms.find(resnum).value();

        for (QSet<AtomID>::const_iterator it = atomids.begin();
             it != atomids.end();
             ++it)
        {
            resdata.atoms.find( resdata.atomnames[*it] )->coords += delta;
        }
    }

    void rotate(ResNum resnum, const QSet<AtomID> &atomids,
                const Matrix &rotmat, const Vector &point)
    {
        EditMolData_ResData &resdata = atms.find(resnum).value();

        for (QSet<AtomID>::const_iterator it = atomids.begin();
             it != atomids.end();
             ++it)
        {
            Vector &coords = resdata.atoms.find( resdata.atomnames[*it] )
                                      ->coords;

            coords = SireMaths::rotate(coords, rotmat, point);
        }
    }

private:
    /** Copy of all of the atoms in the molecule that is being moved */
    QHash<ResNum, EditMolData_ResData> atms;
};

}  //end of namespace detail
}  //end of namespace SireMol

using SireMol::detail::MoveWorkspace;

/** @name EditMolData::translate(...)
    These functions are used to translate parts of the molecule.
    Each of these function are guaranteed to either fully complete,
    or to do nothing. These function will not leave the molecule
    in a partially moved state.
*/
/////////////////////////////////////////////////////////
//@{

/** Translate all of the atoms in the molecule by 'delta' */
void EditMolData::translate(const Vector &delta)
{
    MoveWorkspace workspace(atms);

    foreach( ResNum resnum, resnums )
    {
        workspace.translate(resnum, delta);
    }

    atms = workspace.commit();
}

/** Translate the atom with AtomIndex 'atom' by 'delta'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void EditMolData::translate(const AtomIndex &atom, const Vector &delta)
{
    this->assertAtomExists(atom);

    MoveWorkspace workspace(atms);

    workspace.translate(atom, delta);

    atms = workspace.commit();
}

/** Translate all of the atoms in the group 'group' by 'delta'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void EditMolData::translate(const AtomIDGroup &group, const Vector &delta)
{
    MoveWorkspace workspace(atms);

    foreach( ResNum resnum, group.residues() )
    {
        this->assertResidueExists(resnum);
        workspace.translate( resnum, delta );
    }

    foreach( AtomIndex atom, group.atoms() )
    {
        this->assertAtomExists(atom);
        workspace.translate( atom, delta );
    }

    atms = workspace.commit();
}

/** Translate all of the atoms in 'atoms' by 'delta'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void EditMolData::translate(const QSet<AtomIndex> &atoms, const Vector &delta)
{
    MoveWorkspace workspace(atms);

    foreach( AtomIndex atom, atoms )
    {
        this->assertAtomExists(atom);
        workspace.translate(atom, delta);
    }

    atms = workspace.commit();
}

/** Translate all of the atoms that are in the residue with number 'resnum'
    and that also have their names in 'atoms', by 'delta'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void EditMolData::translate(ResNum resnum, const QStringList &atoms,
                            const Vector &delta)
{
    this->assertAtomsExist(resnum,atoms);

    MoveWorkspace workspace(atms);

    workspace.translate(resnum, atoms, delta);

    atms = workspace.commit();
}

/** Translate all of the atoms in the residue with number 'resnum'
    by 'delta'

    \throw SireMol::missing_residue
*/
void EditMolData::translate(ResNum resnum, const Vector &delta)
{
    this->assertResidueExists(resnum);

    MoveWorkspace workspace(atms);

    workspace.translate(resnum, delta);

    atms = workspace.commit();
}

/** Translate the set of residues whose numbers are in 'resnums' by
    'delta'

    \throw SireMol::missing_residue
*/
void EditMolData::translate(const QSet<ResNum> &resnums, const Vector &delta)
{
    MoveWorkspace workspace(atms);

    foreach (ResNum resnum, resnums)
    {
        this->assertResidueExists(resnum);
        workspace.translate(resnum, delta);
    }

    atms = workspace.commit();
}

/** Translate the atoms in residue with index 'resid' whose names are in 'atoms'
    by 'delta'

    \throw SireError::invalid_index
    \throw SireMol::missing_atom
*/
void EditMolData::translate(ResID resid, const QStringList &atoms,
                            const Vector &delta)
{
    ResNum resnum = this->at(resid);

    this->assertAtomsExist(resnum, atoms);

    MoveWorkspace workspace(atms);
    workspace.translate(resnum, atoms, delta);

    atms = workspace.commit();
}

/** Translate all of the atoms in the residue with index 'resid'
    by 'delta'

    \throw SireError::invalid_index
*/
void EditMolData::translate(ResID resid, const Vector &delta)
{
    ResNum resnum = this->at(resid);

    MoveWorkspace workspace(atms);
    workspace.translate(resnum, delta);

    atms = workspace.commit();
}

/** Translate all of the residues whose indicies are in 'resids'
    by 'delta'

    \throw SireError::invalid_index
*/
void EditMolData::translate(const QSet<ResID> &resids, const Vector &delta)
{
    MoveWorkspace workspace(atms);

    foreach (ResID resid, resids)
    {
        ResNum resnum = this->at(resid);
        workspace.translate(resnum, delta);
    }

    atms = workspace.commit();
}

/** Internal function used to translate all of the atoms in the
    CutGroup with number 'cgnum' by 'delta' */
void EditMolData::_pvt_translate(CutGroupNum cgnum, const Vector &delta,
                                 MoveWorkspace &workspace) const
{
    foreach (AtomIndex atm, cgatoms[cgnum])
    {
        workspace.translate(atm, delta);
    }
}

/** Translate all of the atoms in the CutGroup with number 'cgnum'
    by 'delta'

    \throw SireMol::missing_cutgroup
*/
void EditMolData::translate(CutGroupNum cgnum, const Vector &delta)
{
    this->assertCutGroupExists(cgnum);

    MoveWorkspace workspace(atms);
    this->_pvt_translate(cgnum, delta, workspace);

    atms = workspace.commit();
}

/** Translate all of the atoms in the CutGroups with numbers in 'cgnums'
    by 'delta'

    \throw SireMol::missing_cutgroup
*/
void EditMolData::translate(const QSet<CutGroupNum> &cgnums, const Vector &delta)
{
    MoveWorkspace workspace(atms);

    foreach (CutGroupNum cgnum, cgnums)
    {
        this->assertCutGroupExists(cgnum);
        this->_pvt_translate(cgnum, delta, workspace);
    }

    atms = workspace.commit();
}

/** Translate the atom with index 'atomid' in the residue with number
    'resnum' by 'delta'

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
void EditMolData::translate(ResNum resnum, AtomID atomid, const Vector &delta)
{
    MoveWorkspace workspace(atms);
    workspace.translate(this->at(ResNumAtomID(resnum,atomid)), delta);

    atms = workspace.commit();
}

/** Translate the atoms whose indicies are in 'atomids' and who are in
    the residue with number 'resnum' by 'delta'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void EditMolData::translate(ResNum resnum, const QSet<AtomID> &atomids,
                            const Vector &delta)
{
    this->assertAtomsExist(resnum, atomids);

    MoveWorkspace workspace(atms);
    workspace.translate(resnum, atomids, delta);

    atms = workspace.commit();
}

/////////////////////////////////////////////////////////
//@}

/** @name EditMolData::rotate(...)
    These functions are used to rotate parts of the molecule.
    Each of these function are guaranteed to either fully complete,
    or to do nothing. These function will not leave the molecule
    in a partially moved state.
*/
/////////////////////////////////////////////////////////
//@{

/** Rotate the entire molecule using the quaternion 'quat' about the
    point 'point' */
void EditMolData::rotate(const Quaternion &quat, const Vector &point)
{
    if (quat.isIdentity())
        return;

    this->rotate(quat.toMatrix(), point);
}

/** Rotate the group of atoms in 'group' using the quaternion 'quat' about
    the point 'point'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void EditMolData::rotate(const AtomIDGroup &group,
                         const Quaternion &quat, const Vector &point)
{
    if (quat.isIdentity())
        return;

    this->rotate(group, quat.toMatrix(), point);
}

/** Rotate the atom 'atom' using the quaternion 'quat' about the
    point 'point'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void EditMolData::rotate(const AtomIndex &atom,
                         const Quaternion &quat, const Vector &point)
{
    if (quat.isIdentity())
        return;

    this->rotate(atom, quat.toMatrix(), point);
}

/** Rotate the atoms in 'atoms' using the quaternion 'quat'
    about the point 'point'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void EditMolData::rotate(const QSet<AtomIndex> &atoms,
                         const Quaternion &quat, const Vector &point)
{
    if (quat.isIdentity())
        return;

    this->rotate(atoms, quat.toMatrix(), point);
}

/** Rotate the atoms whose names are in 'atoms' and are also in the residue
    with number 'resnum' using the quaternion 'quat' about the point
    'point'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void EditMolData::rotate(ResNum resnum, const QStringList &atoms,
                         const Quaternion &quat, const Vector &point)
{
    if (quat.isIdentity())
        return;

    this->rotate(resnum, atoms, quat.toMatrix(), point);
}

/** Rotate all of the atoms in the residue with number 'resnum' using
    the quaternion 'quat' about the point 'point'

    \throw SireMol::missing_residue
*/
void EditMolData::rotate(ResNum resnum,
                         const Quaternion &quat, const Vector &point)
{
    if (quat.isIdentity())
        return;

    this->rotate(resnum, quat.toMatrix(), point);
}

/** Rotate all of the atoms in the residues whose numbers are in 'resnums'
    using the quaternion 'quat' about the point 'point'

    \throw SireMol::missing_residue
*/
void EditMolData::rotate(const QSet<ResNum> &resnums,
                         const Quaternion &quat, const Vector &point)
{
    if (quat.isIdentity())
        return;

    this->rotate(resnums, quat.toMatrix(), point);
}

/** Rotate the atoms whose names are in 'atoms' that are in the
    residue at index 'resid' using the quaternion 'quat' about
    the point 'point'

    \throw SireError::invalid_index
    \throw SireMol::missing_atom
*/
void EditMolData::rotate(ResID resid, const QStringList &atoms,
                         const Quaternion &quat, const Vector &point)
{
    if (quat.isIdentity())
        return;

    this->rotate(resid, atoms, quat.toMatrix(), point);
}

/** Rotate all of the atoms in the residue at index 'resid' using the
    quaternion 'quat' about the point 'point'

    \throw SireError::invalid_index
*/
void EditMolData::rotate(ResID resid,
                         const Quaternion &quat, const Vector &point)
{
    if (quat.isIdentity())
        return;

    this->rotate(resid, quat.toMatrix(), point);
}

/** Rotate all of the atoms in the residues whose indicies are in
    'resids' using the quaternion 'quat' about the point 'point'

    \throw SireError::invalid_index
*/
void EditMolData::rotate(const QSet<ResID> &resids,
                         const Quaternion &quat, const Vector &point)
{
    if (quat.isIdentity())
        return;

    this->rotate(resids, quat.toMatrix(), point);
}

/** Rotate all of the atoms in the CutGroup with number 'cgnum' using the
    quaternion 'quat' about the point 'point'

    \throw SireMol::missing_cutgroup
*/
void EditMolData::rotate(CutGroupNum cgnum,
                         const Quaternion &quat, const Vector &point)
{
    if (quat.isIdentity())
        return;

    this->rotate(cgnum, quat.toMatrix(), point);
}

/** Rotate all of the CutGroups whose numbers are in 'cgnums' using the
    quaternion 'quat' about the point 'point'

    \throw SireMol::missing_cutgroup
*/
void EditMolData::rotate(const QSet<CutGroupNum> &cgnums,
                         const Quaternion &quat, const Vector &point)
{
    if (quat.isIdentity())
        return;

    this->rotate(cgnums, quat.toMatrix(), point);
}

/** Rotate the atom at index 'atomid' in the residue with number
    'resnum' using the quaternion 'quat' about the point 'point'.

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
void EditMolData::rotate(ResNum resnum, AtomID atomid,
                         const Quaternion &quat, const Vector &point)
{
    if (quat.isIdentity())
        return;

    this->rotate(resnum, atomid, quat.toMatrix(), point);
}

/** Rotate the atoms whose indicies in the residue with number 'resnum'
    are in 'atomids', using the quaternion 'quat' about the point 'point'

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
void EditMolData::rotate(ResNum resnum, const QSet<AtomID> &atomids,
                         const Quaternion &quat, const Vector &point)
{
    if (quat.isIdentity())
        return;

    this->rotate(resnum, atomids, quat, point);
}

/** Rotate the entire molecule using the matrix 'matrix' about
    the point 'point' */
void EditMolData::rotate(const Matrix &matrix, const Vector &point)
{
    MoveWorkspace workspace(atms);

    foreach( ResNum resnum, resnums )
    {
        workspace.rotate(resnum, matrix, point);
    }

    atms = workspace.commit();
}

/** Rotate the atoms in the group 'group' using the matrix 'matrix'
    about the point 'point'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void EditMolData::rotate(const AtomIDGroup &group,
                         const Matrix &matrix, const Vector &point)
{
    MoveWorkspace workspace(atms);

    foreach (ResNum resnum, group.residues())
    {
        this->assertResidueExists(resnum);
        workspace.rotate(resnum, matrix, point);
    }

    foreach (AtomIndex atom, group.atoms())
    {
        this->assertAtomExists(atom);
        workspace.rotate(atom, matrix, point);
    }

    atms = workspace.commit();
}

/** Rotate the atom 'atom' using the matrix 'matrix' about the point
    'point'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void EditMolData::rotate(const AtomIndex &atom,
                         const Matrix &matrix, const Vector &point)
{
    this->assertAtomExists(atom);

    MoveWorkspace workspace(atms);
    workspace.rotate(atom, matrix, point);

    atms = workspace.commit();
}

/** Rotate the atoms in 'atoms' using the matrix 'matrix' about the point
    'point'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void EditMolData::rotate(const QSet<AtomIndex> &atoms,
                         const Matrix &matrix, const Vector &point)
{
    MoveWorkspace workspace(atms);

    foreach (AtomIndex atom, atoms)
    {
        this->assertAtomExists(atom);
        workspace.rotate(atom, matrix, point);
    }

    atms = workspace.commit();
}

/** Rotate the atoms whose names are in 'atoms' and that are in the residue
    with number 'resnum' using the matrix 'matrix' about the point 'point'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void EditMolData::rotate(ResNum resnum, const QStringList &atoms,
                         const Matrix &matrix, const Vector &point)
{
    this->assertAtomsExist(resnum, atoms);

    MoveWorkspace workspace(atms);

    workspace.rotate(resnum, atoms, matrix, point);

    atms = workspace.commit();
}

/** Rotate all of the atoms in the residue with number 'resnum' using
    the matrix 'matrix' about the point 'point'.

    \throw SireMol::missing_residue
*/
void EditMolData::rotate(ResNum resnum,
                         const Matrix &matrix, const Vector &point)
{
    this->assertResidueExists(resnum);

    MoveWorkspace workspace(atms);

    workspace.rotate(resnum, matrix, point);

    atms = workspace.commit();
}

/** Rotate all of the atoms in the residues whose numbers are in 'resnums'
    using the matrix 'matrix' about the point 'point'

    \throw SireMol::missing_residue
*/
void EditMolData::rotate(const QSet<ResNum> &resnums,
                         const Matrix &matrix, const Vector &point)
{
    MoveWorkspace workspace(atms);

    foreach (ResNum resnum, resnums)
    {
        this->assertResidueExists(resnum);
        workspace.rotate(resnum, matrix, point);
    }

    atms = workspace.commit();
}

/** Rotate the atoms whose names are in 'atoms' and that are also in the
    residue with index 'resid' using the matrix 'matrix' about the point
    'point'

    \throw SireError::invalid_index
    \throw SireMol::missing_atom
*/
void EditMolData::rotate(ResID resid, const QStringList &atoms,
                         const Matrix &matrix, const Vector &point)
{
    ResNum resnum = this->at(resid);

    this->assertAtomsExist(resnum, atoms);

    MoveWorkspace workspace(atms);

    workspace.rotate(resnum, matrix, point);

    atms = workspace.commit();
}

/** Rotate all of the atoms in the residue at index 'resid' using the
    matrix 'matrix' about the point 'point'

    \throw SireError::invalid_index
*/
void EditMolData::rotate(ResID resid,
                         const Matrix &matrix, const Vector &point)
{
    ResNum resnum = this->at(resid);

    MoveWorkspace workspace(atms);
    workspace.rotate(resnum, matrix, point);

    atms = workspace.commit();
}

/** Rotate all of the atoms in the residues whose indicies are in
    'resids' using the matrix 'matrix' about the point 'point'

    \throw SireError::invalid_index
*/
void EditMolData::rotate(const QSet<ResID> &resids,
                         const Matrix &matrix, const Vector &point)
{
    MoveWorkspace workspace(atms);

    foreach (ResID resid, resids)
    {
        ResNum resnum = this->at(resid);
        workspace.rotate(resnum, matrix, point);
    }

    atms = workspace.commit();
}

/** Internal function used to rotate the CutGroup with number 'cgnum'
    using the matrix 'matrix' about the point 'point' */
void EditMolData::_pvt_rotate(CutGroupNum cgnum, const Matrix &matrix,
                              const Vector &point, MoveWorkspace &workspace) const
{
    const QList<AtomIndex> &atoms = cgatoms.find(cgnum).value();

    for (QList<AtomIndex>::const_iterator it = atoms.begin();
         it != atoms.end();
         ++it)
    {
        workspace.rotate(*it, matrix, point);
    }
}

/** Rotate all of the atoms in the CutGroup with number 'cgnum'
    using the matrix 'matrix' about the point 'point'

    \throw SireMol::missing_cutgroup
*/
void EditMolData::rotate(CutGroupNum cgnum,
                         const Matrix &matrix, const Vector &point)
{
    this->assertCutGroupExists(cgnum);

    MoveWorkspace workspace(atms);

    this->_pvt_rotate(cgnum, matrix, point, workspace);

    atms = workspace.commit();
}

/** Rotate all of the atoms in the CutGroups whose numbers are in cgnums

    \throw SireMol::missing_cutgroup
*/
void EditMolData::rotate(const QSet<CutGroupNum> &cgnums,
                         const Matrix &matrix, const Vector &point)
{
    MoveWorkspace workspace(atms);

    foreach (CutGroupNum cgnum, cgnums)
    {
        this->assertCutGroupExists(cgnum);
        this->_pvt_rotate(cgnum, matrix, point, workspace);
    }

    atms = workspace.commit();
}

/** Rotate the atom at index 'atomid' in the residue with number 'resnum'
    using the matrix 'matrix' about the point 'point'

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
void EditMolData::rotate(ResNum resnum, AtomID atomid,
                         const Matrix &rotmat, const Vector &point)
{
    MoveWorkspace workspace(atms);

    workspace.rotate( this->at(ResNumAtomID(resnum,atomid)), rotmat, point );

    atms = workspace.commit();
}

/** Rotate the atoms with indicies in 'atomids' in the residue with
    number 'resnum' using the matrix 'matrix' about the point 'point'

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
void EditMolData::rotate(ResNum resnum, const QSet<AtomID> &atomids,
                         const Matrix &rotmat, const Vector &point)
{
    this->assertAtomsExist(resnum, atomids);

    MoveWorkspace workspace(atms);

    workspace.rotate(resnum, atomids, rotmat, point);

    atms = workspace.commit();
}

/////////////////////////////////////////////////////////
//@}

/** @name EditMolData::setCoordinates(...)
    These functions are used to set the coordinates of parts of
    the molecule. Each of these function are guaranteed to
    either fully complete, or to do nothing. These function will
    not leave the molecule in a partially changed state.
*/
/////////////////////////////////////////////////////////
//@{

/** Internal function used to set the coordinates of the atoms in the CutGroup
    with number 'cgnum' to the coordinates in the array 'newcoords' */
void EditMolData::_pvt_setCoordinates(CutGroupNum cgnum, const Vector *newcoords,
                                      MoveWorkspace &workspace) const
{
    const QList<AtomIndex> &atoms = cgatoms.find(cgnum).value();

    int i = 0;

    for (QList<AtomIndex>::const_iterator it = atoms.begin();
         it != atoms.end();
         ++it)
    {
        workspace.setCoordinates(*it, newcoords[i]);
        ++i;
    }
}

/** Set the coordinates of the atoms in the CutGroup with number 'cgnum'
    to 'newcoords'

    \throw SireMol::missing_cutgroup
    \throw SireError::incompatible_error
*/
void EditMolData::setCoordinates(CutGroupNum cgnum, const CoordGroup &newcoords)
{
    this->assertCutGroupExists(cgnum);
    this->assertNAtoms(cgnum, newcoords.size());

    MoveWorkspace workspace(atms);

    this->_pvt_setCoordinates(cgnum, newcoords.constData(), workspace);

    atms = workspace.commit();
}

/** Set the coordinates of all of the specified CutGroups to the
    values in 'newcoords'

    \throw SireMol::missing_cutgroup
    \throw SireError::incompatible_error
*/
void EditMolData::setCoordinates(const QHash<CutGroupNum,CoordGroup> &newcoords)
{
    MoveWorkspace workspace(atms);

    for (QHash<CutGroupNum,CoordGroup>::const_iterator it = newcoords.begin();
         it != newcoords.end();
         ++it)
    {
        this->assertCutGroupExists(it.key());
        this->assertNAtoms(it.key(), it.value().size());

        this->_pvt_setCoordinates(it.key(), it->constData(), workspace);
    }

    atms = workspace.commit();
}

/** Set the coordinates of all atoms in the molecule to 'newcoords'

    \throw SireError::incompatible_error
*/
void EditMolData::setCoordinates(const QVector<Vector> &newcoords)
{
    this->assertNAtoms(newcoords.size());

    MoveWorkspace workspace(atms);

    workspace.setCoordinates( resnums, newcoords.constData() );

    atms = workspace.commit();
}

/** Set the coordinates of the atoms in the CutGroup with number 'cgnum'
    to 'newcoords'

    \throw SireMol::missing_cutgroup
    \throw SireError::incompatible_error
*/
void EditMolData::setCoordinates(CutGroupNum cgnum,
                                 const QVector<Vector> &newcoords)
{
    this->assertCutGroupExists(cgnum);
    this->assertNAtoms(cgnum, newcoords.size());

    MoveWorkspace workspace(atms);

    this->_pvt_setCoordinates(cgnum, newcoords.constData(), workspace);

    atms = workspace.commit();
}

/** Set the coordinates of the atoms in the specified CutGroups to the
    values in 'newcoords'

    \throw SireMol::missing_cutgroup
    \throw SireError::incompatible_error
*/
void EditMolData::setCoordinates(
                    const QHash< CutGroupNum,QVector<Vector> > &newcoords)
{
    MoveWorkspace workspace(atms);

    for (QHash< CutGroupNum,QVector<Vector> >::const_iterator it = newcoords.begin();
         it != newcoords.end();
         ++it)
    {
        this->assertCutGroupExists(it.key());
        this->assertNAtoms(it.key(), it.value().size());

        this->_pvt_setCoordinates(it.key(), it->constData(), workspace);
    }

    atms = workspace.commit();
}

/** Set the coordinates of all of the atoms in the residue with number 'resnum'
    to 'newcoords'

    \throw SireMol::missing_residue
    \throw SireError::incompatible_error
*/
void EditMolData::setCoordinates(ResNum resnum, const QVector<Vector> &newcoords)
{
    this->assertResidueExists(resnum);
    this->assertNAtoms(resnum, newcoords.count());

    MoveWorkspace workspace(atms);

    workspace.setCoordinates(resnum, newcoords.constData());

    atms = workspace.commit();
}

/** Set the coordinates of the specified residues to the values
    in 'newcoords'

    \throw SireMol::missing_residue
    \throw SireError::incompatible_error
*/
void EditMolData::setCoordinates(const QHash< ResNum,QVector<Vector> > &newcoords)
{
    MoveWorkspace workspace(atms);

    for (QHash< ResNum,QVector<Vector> >::const_iterator it = newcoords.begin();
         it != newcoords.end();
         ++it)
    {
        ResNum resnum = it.key();
        this->assertResidueExists(resnum);
        this->assertNAtoms(resnum, it.value().size());

        workspace.setCoordinates(resnum, it.value().constData());
    }

    atms = workspace.commit();
}

/** Set the coordinates of the residue with index 'resid' to 'newcoords'

    \throw SireError::invalid_index
    \throw SireError::incompatible_error
*/
void EditMolData::setCoordinates(ResID resid, const QVector<Vector> &newcoords)
{
    ResNum resnum = this->at(resid);

    this->assertNAtoms(resnum, newcoords.size());

    MoveWorkspace workspace(atms);

    workspace.setCoordinates(resnum, newcoords.constData());

    atms = workspace.commit();
}

/** Set the coordinates of the specified residues to the values in 'newcoords'

    \throw SireError::invalid_index
    \throw SireError::incompatible_error
*/
void EditMolData::setCoordinates(const QHash< ResID,QVector<Vector> > &newcoords)
{
    MoveWorkspace workspace(atms);

    for (QHash< ResID,QVector<Vector> >::const_iterator it = newcoords.begin();
         it != newcoords.end();
         ++it)
    {
        ResNum resnum = this->at(it.key());

        this->assertNAtoms(resnum, it->size());

        workspace.setCoordinates(resnum, it->constData());
    }

    atms = workspace.commit();
}

/** Set the coordinates of the atom 'atom' to 'newcoords'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void EditMolData::setCoordinates(const AtomIndex &atom,
                                 const Vector &newcoords)
{
    this->assertAtomExists(atom);

    MoveWorkspace workspace(atms);
    workspace.setCoordinates(atom, newcoords);

    atms = workspace.commit();
}

/** Set the coordinates of the specified atoms to the values in
    'newcoords'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void EditMolData::setCoordinates(const QHash<AtomIndex,Vector> &newcoords)
{
    MoveWorkspace workspace(atms);

    for (QHash<AtomIndex,Vector>::const_iterator it = newcoords.begin();
         it != newcoords.end();
         ++it)
    {
        this->assertAtomExists(it.key());
        workspace.setCoordinates(it.key(), it.value());
    }

    atms = workspace.commit();
}

template<class T>
void EditMolData::_pvt_setCoords(const T &idx, const Vector &newcoords)
{
    AtomIndex atom = this->at(idx);

    MoveWorkspace workspace(atms);

    workspace.setCoordinates(atom, newcoords);

    atms = workspace.commit();
}

template<class T>
void EditMolData::_pvt_setCoords(const QHash<T,Vector> &newcoords)
{
    MoveWorkspace workspace(atms);

    for (typename QHash<T,Vector>::const_iterator it = newcoords.begin();
         it != newcoords.end();
         ++it)
    {
        AtomIndex atom = this->at(it.key());
        workspace.setCoordinates(atom, it.value());
    }

    atms = workspace.commit();
}

/** Set the coordinates of the atom at index 'cgatomid' to 'newcoords'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
void EditMolData::setCoordinates(const CGAtomID &cgatomid, const Vector &newcoords)
{
    this->_pvt_setCoords<CGAtomID>(cgatomid, newcoords);
}

/** Set the coordinates of the specified atoms to the values in 'newcoords'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
void EditMolData::setCoordinates(const QHash<CGAtomID,Vector> &newcoords)
{
    this->_pvt_setCoords<CGAtomID>(newcoords);
}

/** Set the coordinates of the atom at index 'resatomid' to 'newcoords'

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
void EditMolData::setCoordinates(const ResNumAtomID &resatomid,
                                 const Vector &newcoords)
{
    this->_pvt_setCoords<ResNumAtomID>(resatomid, newcoords);
}

/** Set the coordinates of the specified atoms to the values in 'newcoords'

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
void EditMolData::setCoordinates(const QHash<ResNumAtomID,Vector> &newcoords)
{
    this->_pvt_setCoords<ResNumAtomID>(newcoords);
}

/** Set the coordinates of the atom at index 'resatomid' to 'newcoords'

    \throw SireError::invalid_index
*/
void EditMolData::setCoordinates(const ResIDAtomID &resatomid,
                                 const Vector &newcoords)
{
    this->_pvt_setCoords<ResIDAtomID>(resatomid, newcoords);
}

/** Set the coordinates of the specified atoms to the values in 'newcoords'

    \throw SireError::invalid_index
*/
void EditMolData::setCoordinates(const QHash<ResIDAtomID,Vector> &newcoords)
{
    this->_pvt_setCoords<ResIDAtomID>(newcoords);
}

/** Set the coordinates of the atom at index 'atomid' in the residue with
    number 'resnum' to 'newcoords'

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
void EditMolData::setCoordinates(ResNum resnum, AtomID atomid,
                                 const Vector &newcoords)
{
    this->setCoordinates( ResNumAtomID(resnum, atomid), newcoords );
}

/** Set the coordinates of the specified atoms in the residue with number
    'resnum' to the values in 'newcoords'

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
void EditMolData::setCoordinates(ResNum resnum,
                                 const QHash<AtomID,Vector> &newcoords)
{
    MoveWorkspace workspace(atms);

    for (QHash<AtomID,Vector>::const_iterator it = newcoords.begin();
         it != newcoords.end();
         ++it)
    {
        AtomIndex atom = this->at( ResNumAtomID(resnum,it.key()) );
        workspace.setCoordinates(atom, it.value());
    }

    atms = workspace.commit();
}

/** Set the coordinates of the named atoms in the residue with number 'resnum'
    to the values in 'newcoords'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void EditMolData::setCoordinates(ResNum resnum,
                                 const QHash<QString,Vector> &newcoords)
{
    MoveWorkspace workspace(atms);

    for (QHash<QString,Vector>::const_iterator it = newcoords.begin();
         it != newcoords.end();
         ++it)
    {
        AtomIndex atom(it.key(),resnum);
        this->assertAtomExists(atom);

        workspace.setCoordinates(atom, it.value());
    }

    atms = workspace.commit();
}

/////////////////////////////////////////////////////////
//@}

/** Translate the atoms in group0 and group1 along the vector of the bond 'bnd', ensuring
    weighting the translation of the two sides by 'weight' and ensuring that the
    atoms in 'anchors' are not moved.

    This will throw an exception if either the atoms in 'bnd' or any of the atoms in
    group0 or group1 are not in this molecule.

    An exception will also be thrown if there are anchor atoms in both group0 and group1.

    \throw SireMol::missing_atom
    \throw SireMol::missing_residue
    \throw SireMol::anchor_error
*/
void EditMolData::change(const Bond &bnd, double delta,
                         const AtomIDGroup &group0, const AtomIDGroup &group1,
                         const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors)
{
    //get the anchor status
    bool anchor0 = group0.isEmpty() or group0.intersects(anchors);
    bool anchor1 = group1.isEmpty() or group1.intersects(anchors);

    double weight;

    if (anchor0 and anchor1)
        //this bond is immovable
        throw SireMol::anchor_error(QObject::tr(
            "Cannot change bond \"%1\" as both groups are either anchored or empty.\n%2\n%3")
                .arg(bnd.toString(),group0.inventory(),group1.inventory()), CODELOC);
    else if (anchor0)
        weight = 0.0;
    else if (anchor1)
        weight = 1.0;
    else
        weight = getWeight(group0, group1, weightfunc);

    //get the vector of the bond...
    SireMaths::Line line = this->bond(bnd);
    Vector bondvec = line.vector();

    if (bondvec.isZero())
        //if we have a zero bondvector, then translate along the x axis
        bondvec = Vector(1.0,0.0,0.0);
    else
        bondvec = bondvec.normalise();

    //translate group0 by (weight-1) * delta * bondvec
    this->translate(group0, (weight-1.0)*delta*bondvec);

    //translate group1 by weight * delta * bondvec
    this->translate(group1, weight*delta*bondvec);
}

/** Rotate the atoms in group0 and group1 around the angle 'ang', ensuring
    weighting the rotation of the two sides by 'weight' and ensuring that the
    atoms in 'anchors' are not moved.

    This will throw an exception if any of the atoms in 'ang' or any of the atoms in
    group0 or group1 are not in this molecule.

    An exception will also be thrown if there are anchor atoms in both group0 and group1.

    \throw SireMol::missing_atom
    \throw SireMol::missing_residue
    \throw SireMol::anchor_error
*/
void EditMolData::change(const SireMol::Angle &ang, const SireMaths::Angle &delta,
                         const AtomIDGroup &group0, const AtomIDGroup &group1,
                         const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors)
{
    //get the anchor status - this will override the passed weight
    bool anchor0 = group0.isEmpty() or group0.intersects(anchors);
    bool anchor1 = group1.isEmpty() or group1.intersects(anchors);

    double weight;

    if (anchor0 and anchor1)
        //this bond is immovable
        throw SireMol::anchor_error(QObject::tr(
            "Cannot change angle \"%1\" as both groups are either anchored or empty.\n%2\n%3")
                .arg(ang.toString(),group0.inventory(),group1.inventory()), CODELOC);
    else if (anchor0)
        weight = 0.0;
    else if (anchor1)
        weight = 1.0;
    else
        weight = getWeight(group0, group1, weightfunc);

    //get the geometry of the angle...
    SireMaths::Triangle trig = angle(ang);
    Vector angvec = trig.vector();

    if (angvec.isZero())
        //if we have a zero angle vector, then rotate about the z axis
        angvec = Vector(0.0,0.0,1.0);
    else
        angvec = angvec.normalise();

    //rotate group0 by (weight-1)*delta around angvec, about the central atom
    //of the angle
    this->rotate( group0, Quaternion((weight-1.0)*delta,angvec), trig[1] );

    //rotate group1 by weight*delta around angvec, about the central atom of the angle
    this->rotate( group1, Quaternion(weight*delta,angvec), trig[1] );
}

/** Rotate the atoms in group0 and group1 around the bond 'dih', ensuring
    weighting the rotation of the two sides by 'weight' and ensuring that the
    atoms in 'anchors' are not moved.

    This will throw an exception if any of the atoms in 'dih' or any of the atoms in
    group0 or group1 are not in this molecule.

    An exception will also be thrown if there are anchor atoms in both group0 and group1.

    \throw SireMol::missing_atom
    \throw SireMol::missing_residue
    \throw SireMol::anchor_error
*/
void EditMolData::change(const Bond &dih, const SireMaths::Angle &delta,
                         const AtomIDGroup &group0, const AtomIDGroup &group1,
                         const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors)
{
    //get the anchor status - this will override the passed weight
    bool anchor0 = group0.isEmpty() or group0.intersects(anchors);
    bool anchor1 = group1.isEmpty() or group1.intersects(anchors);

    double weight;

    if (anchor0 and anchor1)
        //this bond is immovable
        throw SireMol::anchor_error(QObject::tr(
            "Cannot change dihedral centered about \"%1\" as both groups are "
            "either anchored or empty.\n%2\n%3")
                .arg(dih.toString(),group0.inventory(),group1.inventory()), CODELOC);
    else if (anchor0)
        weight = 0.0;
    else if (anchor1)
        weight = 1.0;
    else
        weight = getWeight(group0, group1, weightfunc);

    //get the geometry of the dihedral...
    SireMaths::Line line = bond(dih);
    Vector dihvec = line.vector();

    if (dihvec.isZero())
        //if we have a zero dihedral vector, then rotate about the x axis
        dihvec = Vector(1.0,0.0,0.0);
    else
        dihvec = dihvec.normalise();

    //rotate group0 by (weight-1)*delta around dihvec, about the first atom of the bond
    this->rotate( group0, Quaternion((weight-1.0)*delta,dihvec), line[0] );

    //rotate group1 by weight*delta around dihvec, about the second atom of the bond
    this->rotate( group1, Quaternion(weight*delta,dihvec), line[1] );
}

/** Rotate the atoms in group0 and group1 around the improper angle 'improper', ensuring
    weighting the rotation of the two sides by 'weight' and ensuring that the
    atoms in 'anchors' are not moved.

    This will throw an exception if any of the atoms in 'improper' or any of the atoms in
    group0 or group1 are not in this molecule.

    An exception will also be thrown if there are anchor atoms in both group0 and group1.

    \throw SireMol::missing_atom
    \throw SireMol::missing_residue
    \throw SireMol::anchor_error
*/
void EditMolData::change(const Improper &improper, const SireMaths::Angle &delta,
                         const AtomIDGroup &group0, const AtomIDGroup &group1,
                         const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors)
{
    throw SireError::incomplete_code(QObject::tr(
            "Need to write code to change improper angles!"), CODELOC);
}
