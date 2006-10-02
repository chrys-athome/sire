/**
  * @file
  *
  * C++ Implementation: EditMol
  *
  * Description:
  * Implementation of EditMol, and private EditMol classes,
  * EditMolData_AtomData and EditMolData_ResData
  *
  * Author: Christopher Woods, (C) 2006
  *
  * Copyright: See COPYING file that comes with this distribution
  *
  */

#include "qhash_siremol.h"

#include "editmoldata.h"

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
QDataStream& operator<<(QDataStream &ds, const EditMolDataPvt &mol)
{
    writeHeader(ds, r_editmoldata, 1) << mol.molnme << mol.molid << mol.molversion
                                      << mol.atms << mol.resnums << mol.cgatoms
                                      << mol.cgnums << mol.molbnds;

    return ds;
}

/** Deserialise an EditMolData from a datastream */
QDataStream& operator>>(QDataStream &ds, EditMolDataPvt &mol)
{
    VersionID v = readHeader(ds, r_editmoldata);

    if (v == 1)
    {
        ds >> mol.molnme >> mol.molid >> mol.molversion
           >> mol.atms >> mol.resnums >> mol.cgatoms
           >> mol.cgnums >> mol.molbnds;
    }
    else
        throw version_error(v, "1", r_editmoldata, CODELOC);

    return ds;
}

/** Create a new EditMol called 'molname' */
EditMolData::EditMolData(const QString &molname = QObject::tr("Unnamed"))
            : molnme(molname), molid(0), molversion(0,0)
{}

/** Copy constructor */
EditMolData::EditMolData(const EditMolData &other)
            : molnme(other.molnme), molid(other.molid),
              molversion(other.molversion), atms(other.atms),
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
EditMolData_ResData& EditMolData::_unsafe_atomdata(const AtomIndex &atom)
{
    return this->_unsafe_resdata(atom.resNum()).atoms.find(atom.name()).value();
}

/** Return a reference to the AtomData for the atom 'atom' - this does
    not check whether there is such an atom in this molecule */
const EditMolData_ResData& EditMolData::_unsafe_atomdata(const AtomIndex &atom) const
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
                    .arg(resid).arg(molnme).arg(resnums.count), CODELOC );
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

/** Return the ID number of this molecule */
MoleculeID EditMolData::ID() const
{
    return molid;
}

/** Give this molecule a new, unique ID number */
void EditMolData::setNewID()
{
    molid = Molecule::getNewID();
}

/** Return the version number of this molecule */
const MoleculeVersion& EditMolData::version() const
{
    return molversion;
}

/** Assignment operator */
EditMolData& EditMolData::operator=(const EditMolData &other)
{
    molnme = other.molnme;
    molid = other.molid;
    molversion = other.molversion;
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
             molid == other.molid and
             molversion == other.molversion and
             atms == other.atms and
             resnums == other.resnum and
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
    foreach (ResNum resnum, resnums)
    {
        //get the data for this residue
        const EditMolData_ResData &resdata = this->_unsafe_resdata(resnum);

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
                              resnum );
        }
    }

    //we should never get here!
    BOOST_ASSERT( bool we_should_never_get_here(false) );

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

    return cgatoms.find(cgnum).at(cgatomid.atomID());
}

/** Return the AtomIndex of the atom at index 'cgatomid'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
AtomIndex EditMolData::operator[](const CGNumAtomID &cgatomid) const
{
    this->assertAtomExists(cgatomid);

    return cgatoms.find(cgatomid.cutGroupNum()).at(cgatomid.atomID());
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

/** Return copies of all CoordGroups that contain coordinates from atoms
    with residue number 'resnum'

    \throw SireMol::missing_residue
*/
QHash<CutGroupNum,CoordGroup> EditMolData::coordGroups(ResNum resnum) const
{
    return this->cutGroups( this->cutGroupNums(resnum) );
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

        foreach( QString atmname, resdata.atomnames )
        {
            const EditMolData_AtomData &atomdata = resdata.atoms.find(atmname).value();

            allatoms.append( Atom( atomdata.atomnum, AtomIndex(atmname,resnum),
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

        foreach( QString atmname, resdata.atomnames )
        {
            const EditMolData_AtomData &atomdata = resdata.atoms.find(atmname).value();

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
QVector<Vector> EditMolData::coordinates(CutGroupID cgid) const
{
    this->assertCutGroupExists(cgid);

    const QList<AtomIndex> &cgatms = cgatoms.find(cgnum).value();

    QVector<Vector> allcoords;
    allcoords.reserve( cgatms.count() );

    for (QList<AtomIndex>::const_iterator it = cgatms.begin();
         it != cgatms.end();
         ++it)
    {
        allatoms.append( this->_unsafe_coordinates(*it) );
    }

    return allcoords;
}

/** Return an array of all of the coordinates of all of the atoms in the
    Residue with number 'resnum', in the same order as the atoms appear
    in this residue (AtomID order)

    \throw SireMol::missing_residue
*/
QVector<Atom> EditMolData::coordinates(ResNum resnum) const
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
    foreach( AtomIndex atm, group0.atoms() )
    {
        calc.addToA( atom(atm) );
    }

    foreach( AtomIndex atm, group1.atoms() )
    {
        calc.addToB( atom(atm) );
    }

    //now add all of the residues
    foreach( ResNum resnum, group0.residues() )
    {
        calc.addToA( atoms(resnum) );
    }

    foreach( ResNum resnum, group1.residues() )
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
    this->_unsafe_resdata.resname = name;
}

/** Clean this molecule - this removes any empty residues */
void EditMolData::clean()
{
    QList<ResNum> oldnums = resnums;

    for (QList<ResNum>::const_iterator it = oldnums.begin();
         it != oldnums.end();
         ++it)
    {
        if (this->isEmpty(resnum))
            this->remove(resnum);
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
    molbnds.renumberResidue(oldnum, newnum);
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

    resdata.atoms.insert( atom.name(), EditMolData_AtomData(atom) );
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
    this->add( atom, cgfunc.getNumber(atom) );
}

/** Remove the atom 'atom' from this molecule. Note that removing
    the last atom from a residue or CutGroup *will not* remove
    the Residue or CutGroup. Use EditMolData::clean() if you
    want to remove any empty residues or CutGroups.

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
// void EditMolData::remove(const AtomIndex &atom)
// {
// }

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
    this->add(resnum, tmpl.name(), tmpl);
}

/** Remove the residue with number 'resnum' from this molecule.

    \throw SireMol::missing_residue
*/
void EditMolData::remove(ResNum resnum)
{
    this->assertResidueExists(resnum);

    molbnds.remove(resnum);
    atms.remove(resnum);
    resnums.removeAll(resnum);
}

/** Translate all of the atoms in the molecule by 'delta' */
void EditMolData::translate(const Vector &delta)
{
    MoveWorkspace workspace(atms);

    foreach( ResNum resnum, resnums )
    {
        workspace.translate(*it, delta);
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
        workspace.translate( resnum, delta );
    }

    foreach( AtomIndex atom, group.atoms() )
    {
        workspace.translate( atom, delta );
    }

    atms = workspace.commit();
}

void EditMolData::translate(const QSet<AtomIndex> &atoms, const Vector &delta);
void EditMolData::translate(ResNum resnum, const QStringList &atoms, const Vector &delta);
void EditMolData::translate(ResNum resnum, const Vector &delta);
void EditMolData::translate(const QSet<ResNum> &resnums, const Vector &delta);
void EditMolData::translate(ResID resid, const QStringList &atoms, const Vector &delta);
void EditMolData::translate(ResID resid, const Vector &delta);
void EditMolData::translate(const QSet<ResID> &resids, const Vector &delta);
void EditMolData::translate(CutGroupID cgid, const Vector &delta);
void EditMolData::translate(const QSet<CutGroupID> &cgids, const Vector &delta);
void EditMolData::translate(ResNum resnum, AtomID atomid, const Vector &delta);
void EditMolData::translate(ResNum resnum, const QSet<AtomID> &atomids, const Vector &delta);

void EditMolData::rotate(const Quaternion &quat, const Vector &point);
void EditMolData::rotate(const AtomIDGroup &group, const Quaternion &quat, const Vector &point);
void EditMolData::rotate(const AtomIndex &atom, const Quaternion &quat, const Vector &point);
void EditMolData::rotate(const QSet<AtomIndex> &atoms, const Quaternion &quat, const Vector &point);
void EditMolData::rotate(ResNum resnum, const QStringList &atoms, const Quaternion &quat,
            const Vector &point);
void EditMolData::rotate(ResNum resnum, const Quaternion &quat, const Vector &point);
void EditMolData::rotate(const QSet<ResNum> &resnums, const Quaternion &quat, const Vector &point);
void EditMolData::rotate(ResID resid, const QStringList &atoms, const Quaternion &quat,
            const Vector &point);
void EditMolData::rotate(ResID resid, const Quaternion &quat, const Vector &point);
void EditMolData::rotate(const QSet<ResID> &resids, const Quaternion &quat, const Vector &point);
void EditMolData::rotate(CutGroupID cgid, const Quaternion &quat, const Vector &point);
void EditMolData::rotate(const QSet<CutGroupID> &cgids, const Quaternion &quat, const Vector &point);
void EditMolData::rotate(ResNum resnum, AtomID atomid, const Quaternion &quat, const Vector &point);
void EditMolData::rotate(ResNum resnum, const QSet<AtomID> &atomids,
            const Quaternion &quat, const Vector &point);

void EditMolData::rotate(const Matrix &matrix, const Vector &point);
void EditMolData::rotate(const AtomIDGroup &group, const Matrix &matrix, const Vector &point);
void EditMolData::rotate(const AtomIndex &atom, const Matrix &matrix, const Vector &point);
void EditMolData::rotate(const QSet<AtomIndex> &atoms, const Matrix &matrix, const Vector &point);
void EditMolData::rotate(ResNum resnum, const QStringList &atoms, const Matrix &matrix,
            const Vector &point);
void EditMolData::rotate(ResNum resnum, const Matrix &matrix, const Vector &point);
void EditMolData::rotate(const QSet<ResNum> &resnums, const Matrix &matrix, const Vector &point);
void EditMolData::rotate(ResID resid, const QStringList &atoms, const Matrix &matrix,
            const Vector &point);
void EditMolData::rotate(ResID resid, const Matrix &matrix, const Vector &point);
void EditMolData::rotate(const QSet<ResID> &resids, const Matrix &matrix, const Vector &point);
void EditMolData::rotate(CutGroupID cgid, const Matrix &matrix, const Vector &point);
void EditMolData::rotate(const QSet<CutGroupID> &cgids, const Matrix &matrix, const Vector &point);
void EditMolData::rotate(ResNum resnum, AtomID atomid, const Matrix &rotmat, const Vector &point);
void EditMolData::rotate(ResNum resnum, const QSet<AtomID> &atomids,
            const Matrix &rotmat, const Vector &point);

void EditMolData::setCoordinates(CutGroupID cgid, const CoordGroup &newcoords);
void EditMolData::setCoordinates(const QHash<CutGroupID,CoordGroup> &newcoords);

void EditMolData::setCoordinates(const QVector<Vector> &newcoords);

void EditMolData::setCoordinates(CutGroupID cgid, const QVector<Vector> &newcoords);
void EditMolData::setCoordinates(const QHash< CutGroupID,QVector<Vector> > &newcoords);

void EditMolData::setCoordinates(ResNum resnum, const QVector<Vector> &newcoords);
void EditMolData::setCoordinates(const QHash< ResNum,QVector<Vector> > &newcoords);

void EditMolData::setCoordinates(ResID resid, const QVector<Vector> &newcoords);
void EditMolData::setCoordinates(const QHash< ResID,QVector<Vector> > &newcoords);

void EditMolData::setCoordinates(const AtomIndex &atom, const Vector &newcoords);
void EditMolData::setCoordinates(const QHash<AtomIndex,Vector> &newcoords);

void EditMolData::setCoordinates(const CGAtomID &cgatomid, const Vector &newcoords);
void EditMolData::setCoordinates(const QHash<CGAtomID,Vector> &newcoords);

void EditMolData::setCoordinates(const ResNumAtomID &resatomid, const Vector &newcoords);
void EditMolData::setCoordinates(const QHash<ResNumAtomID,Vector> &newcoords);

void EditMolData::setCoordinates(const ResIDAtomID &resatomid, const Vector &newcoords);
void EditMolData::setCoordinates(const QHash<ResIDAtomID,Vector> &newcoords);

void EditMolData::setCoordinates(ResNum resnum, AtomID atomid, const Vector &newcoords);
void EditMolData::setCoordinates(ResNum resnum, const QHash<AtomID,Vector> &newcoords);

void EditMolData::setCoordinates(ResNum resnum, const QHash<QString,Vector> &newcoords);
/////////////////////////////////////////////////


//////// Internal geometry moves ////////////////
void EditMolData::change(const Bond &bnd, double delta,
            const AtomIDGroup &group0, const AtomIDGroup &group1,
            const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors);

void EditMolData::change(const SireMol::Angle &ang, const SireMaths::Angle &delta,
            const AtomIDGroup &group0, const AtomIDGroup &group1,
            const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors);

void EditMolData::change(const Bond &bnd, const SireMaths::Angle &delta,
            const AtomIDGroup &group0, const AtomIDGroup &group1,
            const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors);

void EditMolData::change(const Improper &improper, const SireMaths::Angle &delta,
            const AtomIDGroup &group0, const AtomIDGroup &group1,
            const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors);
/////////////////////////////////////////////////
















/** Construct an EditMolPvt for the molecule called 'molname' */
EditMolDataPvt::EditMolDataPvt(const QString &molname) : QSharedData(), molnme(molname)
{}

/** Construct an EditMolDataPvt that contains the data that represents the molecule 'mol' */
EditMolDataPvt::EditMolDataPvt(const Molecule &mol) : QSharedData(), molnme(mol.name())
{
    //loop over each residue in the molecule and add it in turn...
    int nres = mol.nResidues();
    QVector<ResNum> resnums = mol.residueNumbers();
    QStringList resnames = mol.residueNames();

    for (int i=0; i<nres; ++i)
    {
        this->addResidue(resnums.at(i),resnames.at(i));

        QVector<Atom> atms = mol.atoms(resnums.at(i));
        int nats = atms.count();

        for (int j=0; j<nats; ++j)
            this->add(atms.at(j));
    }

    molbnds = mol.connectivity();
}

/** Copy constructor */
EditMolDataPvt::EditMolDataPvt(const EditMolDataPvt &other)
               : QSharedData(),
                 molnme(other.molnme), atms(other.atms),
                 resnmes(other.resnmes), resnums(other.resnums), molbnds(other.molbnds)
{}

/** Destructor */
EditMolDataPvt::~EditMolDataPvt()
{}

/** Comparison operator */
bool EditMolDataPvt::operator==(const EditMolDataPvt &other) const
{
    return (this == &other) or
           (molnme == other.molnme and atms == other.atms and resnmes == other.resnmes and
            resnums == other.resnums and molbnds == other.molbnds);
}

bool EditMolDataPvt::operator!=(const EditMolDataPvt &other) const
{
    return (this != &other) and
           (molnme != other.molnme or atms != other.atms or resnmes != other.resnmes or
            resnums != other.resnums or molbnds != other.molbnds);
}

/** Return the name of the molecule */
inline const QString& EditMolDataPvt::name() const
{
    return molnme;
}

/** Check that the residue with number 'resnum' is in this molecule. Throw
    an exception if it isn't.

    \throw SireMol::missing_residue
*/
inline void EditMolDataPvt::checkResidue(ResNum resnum) const
{
    if (not resnmes.contains(resnum))
        throw SireMol::missing_residue(QObject::tr(
            "EditMol \"%1\" does not contain a residue with number \"%2\"")
                .arg(name(),resnum.toString()), CODELOC);
}

/** Return a reference to the EditResAtomSet for the residue with number 'resnum'.
    This automatically creates the residue if it doesn't already exist. */
EditResAtomSet& EditMolDataPvt::residue(ResNum resnum)
{
    if (not resnmes.contains(resnum))
    {
        addResidue(resnum, QObject::tr("UNK"));
    }

    return atms[resnum];
}

/** Return the list of atoms in the residue 'resnum', sorted in the same order
    as they were added to this molecule. This will throw an exception if there
    is no such residue in this molecule.

    \throw SireMol::missing_residue
*/
QVector<Atom> EditMolDataPvt::atoms(ResNum resnum) const
{
    checkResidue(resnum);
    return AtomSet(atms[resnum].values());
}

/** Return a vector of atoms in the residue 'resnum', sorted in the same order
    as they were added to this molecule. This will throw an exception if there
    is no such residue in this molecule.

    \throw SireMol::missing_residue
*/
AtomVector EditMolDataPvt::atomVector(ResNum resnum) const
{
    checkResidue(resnum);
    return atms[resnum].values();
}

/** Return a const reference to the bond data of the molecule */
inline const MoleculeBonds& EditMolDataPvt::connectivity() const
{
    return molbnds;
}

/** Return the connectivity of the residue with number 'resnum'. This will
    throw an exception if there is no such residue */
inline ResidueBonds EditMolDataPvt::connectivity(ResNum resnum) const
{
    checkResidue(resnum);
    return molbnds.residue(resnum);
}

/** Return a signature for the EditMol. This can be used to compare
    this EditMol with another (or with Molecule) to see if the
    two objects describe the same molecules (i.e. contain the same
    residues, atoms and bonds) */
inline MoleculeSignature EditMolDataPvt::signature() const
{
    QHash<ResidueID,QStringSet> atmnames;

    //loop over all of the residues
    QMapIterator<ResNum,QString> it(resnmes);
    while(it.hasNext())
    {
        it.next();

        //get the atom names in this residue
        atmnames.insert( ResidueID(it.value(),it.key()), atms[it.key()].keys().toSet() );
    }

    return MoleculeSignature(molnme, atmnames, molbnds);
}

/** Set the name of the molecule */
inline void EditMolDataPvt::setName(const QString &name)
{
    molnme = name;
}

/** Return whether or not this molecule contains a residue with residue
    number 'resnum' */
inline bool EditMolDataPvt::contains(ResNum resnum) const
{
    return resnmes.contains(resnum);
}

/** Return whether or not this molecule contains an atom with index 'atm'.
    Note that this returns true only if there is an exact match (case-sensitive
    match) */
inline bool EditMolDataPvt::contains(const AtomIndex &atm) const
{
    return atms.contains(atm.resNum()) and atms[atm.resNum()].has_key(atm.name());
}

/** Return whether or not this molecule contains the bond 'bond'. */
inline bool EditMolDataPvt::contains(const Bond &bond) const
{
    return molbnds.contains(bond);
}

/** Return the atom in this residue with the name 'atomname'. This will first
    try to find an atom that matches the name exactly - i.e. a case sensitive
    match. If that fails, it will then try to match an all upper-case version
    of the name, then an all lower-case version of the name. If these fail,
    then it will try and match the first any-case version of the name that it
    can find. It is always much better and faster to match the exact name.

    This 'sloppy' matching is to make it easier for the user working with
    the code. It may however lead to confusion if you really want to use
    different atoms with the same name but with different case. I strongly
    urge against this, but I know that some people do like to use case
    to distinguish atoms, so I will not prevent it.

    \throw SireMol::missing_atom
*/
inline const Atom& EditMolDataPvt::atom(const AtomIndex &atm) const
{
    if (not atms.contains(atm.resNum()))
        throw SireMol::missing_atom(QObject::tr(
            "Molecule \"%1\" does not contain atom %2 as it doesn't contain the residue!")
                .arg(name()).arg(atm.toString()), CODELOC);
    else
    {
        //get the atoms in the residue
        EditResAtomSet resatms = atms[atm.resNum()];

        if (resatms.has_key(atm.name()))
            //exact match!
            return resatms.at_key(atm.name());
        else
        {
            //try an all uppercase match
            QString upper = atm.name().toUpper();
            if (resatms.has_key(upper))
                return resatms.at_key(upper);

            //try an all lowercase match
            QString lower = atm.name().toLower();
            if (resatms.has_key(lower))
                return resatms.at_key(lower);

            //that hasn't worked. We will now scan all atoms in the residue
            //and see if there is an any-case match.
            int nats = resatms.count();
            for (int i=0; i<nats; ++i)
            {
                if (resatms[i].name().toLower() == lower)
                    return resatms[i];
            }

            throw SireMol::missing_atom(QObject::tr(
                "There is no atom called \"%1\" in residue %2 in molecule \"%3\"")
                    .arg(atm.name(),atm.resNum().toString(),name()), CODELOC);
        }
    }
}

/** Internal function used to get a non-const reference to the atom
    with atomindex 'atm'. This throws an exception if there is no such
    atom in this molecule.

    \throw SireMol::missing_atom
*/
inline ERAtomAccessor EditMolDataPvt::atom(const AtomIndex &atm)
{
    if (not this->contains(atm))
        throw SireMol::missing_atom(QObject::tr(
            "There is no atom called \"%1\" in residue %2 in molecule \"%3\"")
                .arg(atm.name(),atm.resNum().toString(),name()), CODELOC);

    ERAtomAccessor it(atms[atm.resNum()]);
    it.findKey(atm.name());

    return it;
}

/** Return the name of the residue with number 'resnum'. This throws an
    exception if there is no residue with this number.

    \throw SireMol::missing_residue
*/
QString EditMolDataPvt::residueName(ResNum resnum) const
{
    if (not resnmes.contains(resnum))
        throw SireMol::missing_residue(QObject::tr(
            "Molecule %1 is missing a residue numbered %2")
                .arg(name(),resnum.toString()), CODELOC);

    return resnmes[resnum];
}

/** Return the list of all of the residue numbers in this molecule */
QList<ResNum> EditMolDataPvt::residueNumbers() const
{
    return resnums;
}

/** Return the list of all residue names */
QStringList EditMolDataPvt::residueNames() const
{
    return resnmes.values();
}

/** Return the set of residue IDs of all of the residues in this molecule */
ResidueIDSet EditMolDataPvt::residueIDs() const
{
    ResidueIDSet resids;
    int nres = resnums.count();
    resids.reserve(nres);

    for (int i=0; i<nres; ++i)
    {
        ResNum resnum = resnums[i];
        resids.insert( ResidueID(resnmes[resnum], resnum) );
    }

    return resids;
}

/** Return the list of residue numbers of residues called 'resnam'. This will
    return an empty list if there are no residues with this name */
QList<ResNum> EditMolDataPvt::residueNumbers(const QString &resnam) const
{
    QList<ResNum> rnums;

    QMapIterator<ResNum, QString> it(resnmes);

    while( it.hasNext() )
    {
        it.next();

        if (it.value() == resnam)
            rnums.append(it.key());
    }

    return rnums;
}

/** Completely clear this molecule */
void EditMolDataPvt::clear()
{
    atms.clear();
    molbnds.clear();
    resnmes.clear();
}

/** Clean up the EditMolData - this removes all empty residues (those that don't
    contain any atoms) */
void EditMolDataPvt::clean()
{
    //delete all empty residues
    QMutableMapIterator< ResNum, EditResAtomSet > it(atms);

    while (it.hasNext())
    {
        if (it.value().isEmpty())
        {
            //remove the residue from the list of residue names
            resnmes.remove(it.key());
            //remove the residue
            it.remove();
        }
    }
}

/** Finalise a molecule - this cleans up the molecule by removing empty residues
    and squeezing memory usage so that as the molecule consumes as little memory as
    possible. */
void EditMolDataPvt::finalise()
{
    this->clean();

    molbnds.finalise();

    QMutableMapIterator<ResNum,EditResAtomSet> it(atms);

    while(it.hasNext())
    {
        it.next();
        it.value().squeeze();
    }
}

/** Add the atom 'atom' to the molecule. This will automatically create
    the residue if it is missing. This will throw an exception if an atom
    with this name already exists.

    \throw SireMol::duplicate_atom
*/
void EditMolDataPvt::add(const Atom &atom)
{
    EditResAtomSet &resatms = residue(atom.resNum());

    if (resatms.has_key(atom.name()))
        throw SireMol::duplicate_atom(QObject::tr(
            "Cannot add another atom called \"%1\" to molecule \"%2\" (atom = %3)")
                .arg(atom.name(),name(),atom.toString()), CODELOC);

    resatms.insert(atom);
}

/** Remove the atom 'atom' from this molecule. This does nothing if this
    atom is not present in the molecule. */
void EditMolDataPvt::remove(const AtomIndex &atom)
{
    if (atms.contains(atom.resNum()))
    {
        EditResAtomSet &resatms = atms[atom.resNum()];

        if (resatms.has_key(atom.name()))
        {
            resatms.remove_key(atom.name());
            this->removeAllBonds(atom);
        }
    }
}

/** Add a bond - this will throw an exception if either of the atoms in the
    bond are missing

    \throw SireMol::missing_atom
*/
void EditMolDataPvt::add(const Bond &bond)
{
     if ( not (this->contains(bond.atom0()) and this->contains(bond.atom1())) )
     {
        QString err;
        if (not this->contains(bond.atom0()))
            err = QObject::tr("No atom %1 (%2) in molecule \"%3\". ")
                .arg(bond.atom0().name(),bond.atom0().resNum().toString(),name());

        if (not this->contains(bond.atom1()))
            err += QObject::tr("No atom %1 (%2) in molecule \"%3\".")
                .arg(bond.atom1().name(),bond.atom1().resNum().toString(),name());

        throw SireMol::missing_atom(err, CODELOC);
     }

     molbnds.add(bond);
}

/** Remove a bond - this will do nothing if the bond doesn't exist */
void EditMolDataPvt::remove(const Bond &bond)
{
    molbnds.remove(bond);
}

/** Remove all bonds involving the residue with residue number 'resnum' - this
    does nothing if this residue doesn't exist, or if it already has no bonds. */
void EditMolDataPvt::removeAllBonds(ResNum resnum)
{
    molbnds.removeAll(resnum);
}

/** Remove all bonds involving the atom 'atom'. This does nothing if this
    atom is not in this molecule, or if it doesn't have any bonds */
void EditMolDataPvt::removeAllBonds(const AtomIndex &atom)
{
    molbnds.removeAll(atom);
}

/** Remove all bonding information from this molecule */
void EditMolDataPvt::removeAllBonds()
{
    molbnds.clear();
}

/** Update the atom information for 'atom'. This will throw an exception
    if this atom doesn't already exist.

    \throw SireMol::missing_atom
*/
void EditMolDataPvt::update(const Atom &atm)
{
    ERAtomAccessor it = atom(atm);

    it.value().element() = atm.element();
    it.value().vector() = atm.vector();
}

/** Update the atom 'atm' with the contents of 'details' */
void EditMolDataPvt::update(const AtomIndex &atm, const Atom &details)
{
    atom(atm).value() = Atom(details.number(), atm, details.element(), details.vector());
}

/** Update the information for atom 'atm' - change the element */
void EditMolDataPvt::update(const AtomIndex &atm, const Element &element)
{
    atom(atm).value().element() = element;
}

/** Update the information for atom 'atm' - change the coordinates */
void EditMolDataPvt::update(const AtomIndex &atm, const Vector &coords)
{
    atom(atm).value().vector() = coords;
}

/** Add a residue with residue number 'resnum' and name 'resnam' to this molecule.
    This will throw an exception if a residue with this number already exists

    \throw SireMol::duplicate_residue
*/
void EditMolDataPvt::addResidue(ResNum resnum, const QString &resnam)
{
    if (resnmes.contains(resnum))
        throw SireMol::duplicate_residue(QObject::tr(
            "A residue with number %1 already exists. Cannot replace %2(%1) with %4(%1)")
                .arg(resnum.toString(),resnmes[resnum],resnam), CODELOC);

    resnmes.insert(resnum, resnam);
    resnums.append(resnum);
    qSort(resnums);
    atms.insert(resnum, EditResAtomSet());
}

/** Remove the residue with number 'resnum' from this molecule. This remove all of the
    residue's atoms and all of the bonds to the residue. This does nothing if there
    is no residue with this number in the molecule. */
void EditMolDataPvt::removeResidue(ResNum resnum)
{
    if (resnmes.contains(resnum))
    {
        //remove all bonding involving this residue
        molbnds.removeAll(resnum);

        //remove the residue's atoms
        atms.remove(resnum);

        //remove the residue
        resnmes.remove(resnum);
    }
}

/** Remove the first residue with name 'resnam'. Does nothing if there are no residues
    called 'resnam'. */
void EditMolDataPvt::removeResidue(const QString &resnam)
{
    QList<ResNum> resnums = residueNumbers(resnam);

    if (resnums.count() > 0)
        removeResidue(resnums[0]);
}

/** Remove all residues called 'resnam'. Does nothing if there are no residues with this
    name. */
void EditMolDataPvt::removeAllResidues(const QString &resnam)
{
    QList<ResNum> resnums = residueNumbers(resnam);

    foreach( ResNum resnum, resnums )
        removeResidue(resnum);
}

/** Renumber the residue with current number 'resnum' to the new number 'newnum'.

    This will throw an exception if either there is no current residue with number
    'resnum' or if there is an existing residue with number 'newnum'

    \throw SireMol::missing_residue
    \throw SireMol::duplicate_residue
*/
void EditMolDataPvt::renumberResidue(ResNum resnum, ResNum newnum)
{
    throw SireError::incomplete_code(QObject::tr(
          "Need to write code to renumber residues..."), CODELOC);
}

/** Return the vector of all of the atoms that are in this molecule. The atoms
    are sorted according to their residue number, then in the order that
    they were added to the residue. */
AtomVector EditMolDataPvt::atomVector() const
{
    //get the number of atoms in the molecule
    int nats = nAtoms();

    //reserve sufficient space in the vector
    AtomVector allatoms;
    allatoms.reserve(nats);

    QMapIterator<ResNum, EditResAtomSet> it(atms);

    while( it.hasNext() )
    {
        it.next();

        foreach( Atom atm, it.value().values() )
            allatoms.append(atm);
    }

    return allatoms;
}

/** Return the set of all of the atoms that are in this molecule. The atoms
    are sorted according to their residue number, then in the order that
    they were added to the residue. */
AtomSet EditMolDataPvt::atoms() const
{
    return AtomSet(atomVector());
}

/** Translate the atom 'atm' by 'delta'. This throws an exception if there is
    no such atom in this molecule.

    \throw SireMol::missing_atom
*/
inline void EditMolDataPvt::translate(const AtomIndex &atm, const Vector &delta)
{
    atom(atm).value() += delta;
}

/** Rotate the atom 'atm' by the rotation matrix 'rotmat' around the point 'point'. This
    will throw an exception if there is no such atom.

    \throw SireMol::missing_atom
*/
inline void EditMolDataPvt::rotate(const AtomIndex &atm, const Matrix &rotmat, const Vector &point)
{
    ERAtomAccessor it = atom(atm);
    Atom &rotatm = it.value();
    rotatm = SireMaths::rotate(rotatm, rotmat, point);
}

/** Rotate the atom 'atm' by the quaternion 'quat' around the point 'point'. This
    will throw an exception if there is no such atom.

    \throw SireMol::missing_atom
*/
inline void EditMolDataPvt::rotate(const AtomIndex &atm, const Quaternion &quat, const Vector &point)
{
    rotate(atm, quat.toMatrix(), point);
}

/** Translate all of the atoms in the IDGroup 'group' by 'delta'. This will
    throw an exception if any of the atoms are missing from this molecule.

    \throw SireMol::missing_atom
    \throw SireMol::missing_residue
*/
void EditMolDataPvt::translate(const AtomIDGroup &group, const Vector &delta)
{
    if (delta.isZero())
        return;

    //translate all of the residues
    for (ResNumSet::const_iterator it = group.residues().begin();
         it != group.residues().end(); ++it)
    {
        this->translate(*it, delta);
    }

    //now translate all of the atoms...
    for (QSet<AtomIndex>::const_iterator it = group.atoms().begin();
         it != group.atoms().end(); ++it)
    {
        this->translate(*it, delta);
    }
}

/** Rotate all of the atoms in the IDGroup 'group' by 'delta'. This will
    throw an exception if any of the atoms are missing from this molecule.

    \throw SireMol::missing_atom
    \throw SireMol::missing_residue
*/
void EditMolDataPvt::rotate(const AtomIDGroup &group, const Quaternion &quat, const Vector &point)
{
    if (quat.isIdentity())
        return;

    //as an optimisation, rotate using the matrix form of the quaternion
    Matrix rotmat = quat.toMatrix();

    //rotate all of the residues
    for (ResNumSet::const_iterator it = group.residues().begin();
         it != group.residues().end(); ++it)
    {
        this->rotate(*it, rotmat, point);
    }

    //now rotate all of the atoms...
    for (QSet<AtomIndex>::const_iterator it = group.atoms().begin();
         it != group.atoms().end(); ++it)
    {
        this->rotate(*it, rotmat, point);
    }
}

/** Translate all of the atoms in residue 'resnum'. This will throw an exception if
    there is no residue with this number in this molecule.

    \throw SireMol::missing_residue
*/
void EditMolDataPvt::translate(ResNum resnum, const Vector &delta)
{
    if (not contains(resnum))
        throw SireMol::missing_residue(QObject::tr(
            "There is no residue with number \"%1\" in molecule \"%2\"")
                .arg(resnum.toString(),name()), CODELOC);

    if (delta.isZero())
        return;

    EditResAtomSet &resatms = atms[resnum];

    ERAtomAccessor it(resatms);
    while (it.hasNext())
    {
        it.next();
        it.value() += delta;
    }
}

/** Translate the atoms with names in 'atoms' in residue 'resnum' by delta. This will
    throw an exception if there is no residue with this number, or if any of the
    atoms are not in the residue.

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void EditMolDataPvt::translate(ResNum resnum, const QStringList &atoms, const Vector &delta)
{
    if (not contains(resnum))
        throw SireMol::missing_residue(QObject::tr(
            "There is no residue with number \"%1\" in molecule \"%2\"")
                .arg(resnum.toString(),name()), CODELOC);

    if (delta.isZero())
        return;

    EditResAtomSet &resatms = atms[resnum];

    ERAtomAccessor it(resatms);

    foreach( QString atmname, atoms )
    {
        it.findKey(atmname);

        if (not it.isValid())
            throw SireMol::missing_atom(QObject::tr(
                "No atom called \"%1\" in residue \"%2(%3)\" in molecule \"%4\"")
                    .arg(atmname,residueName(resnum),resnum.toString(),name()), CODELOC);

        it.value() += delta;
    }
}

/** Rotate all of the atoms in the residue 'resnum' using the rotation matrix 'rotmat'
    about the point 'point'. This will throw an exception if there is no such
    residue in the molecule.

    \throw SireMol::missing_residue
*/
void EditMolDataPvt::rotate(ResNum resnum, const Matrix &rotmat, const Vector &point)
{
    if (not contains(resnum))
        throw SireMol::missing_residue(QObject::tr(
            "There is no residue with number \"%1\" in molecule \"%2\"")
                .arg(resnum.toString(),name()), CODELOC);

    EditResAtomSet &resatms = atms[resnum];

    ERAtomAccessor it(resatms);
    while( it.hasNext() )
    {
        it.next();
        Atom &rotatm = it.value();
        rotatm = SireMaths::rotate(rotatm, rotmat, point);
    }
}

/** Rotate all of the atoms in the residue 'resnum' using the quaternion 'quat'
    about the point 'point'. This will throw an exception if there is no such
    residue in the molecule.

    \throw SireMol::missing_residue
*/
inline void EditMolDataPvt::rotate(ResNum resnum, const Quaternion &quat, const Vector &point)
{
    if (not quat.isIdentity())
        this->rotate(resnum, quat.toMatrix(), point);
}

/** Rotate the atoms with names in 'atoms' and in residue 'resnum' using the rotation
    matrix 'rotmat' about the point 'point'. This will throw an exception if any of the atoms
    are not in the residue, or if there is no such residue in this molecule.

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
inline void EditMolDataPvt::rotate(ResNum resnum, const QStringList &atoms,
                                   const Matrix &rotmat, const Vector &point)
{
    if (not contains(resnum))
        throw SireMol::missing_residue(QObject::tr(
            "There is no residue with number \"%1\" in molecule \"%2\"")
                .arg(resnum.toString(),name()), CODELOC);

    EditResAtomSet &resatms = atms[resnum];
    ERAtomAccessor it( resatms );

    foreach( QString atmnam, atoms )
    {
        it.findKey(atmnam);
        if (not it.isValid())
            throw SireMol::missing_atom(QObject::tr(
                "No atom called \"%1\" in residue \"%2(%3)\" in molecule \"%4\"")
                    .arg(atmnam,residueName(resnum),resnum.toString(),name()), CODELOC);

        Atom &rotatm = it.value();
        rotatm = SireMaths::rotate(rotatm, rotmat, point);
    }
}

/** Rotate the atoms with names in 'atoms' and in residue 'resnum' using the quaternion
    'quat' about the point 'point'. This will throw an exception if any of the atoms
    are not in the residue, or if there is no such residue in this molecule.

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
inline void EditMolDataPvt::rotate(ResNum resnum, const QStringList &atoms,
                                   const Quaternion &quat, const Vector &point)
{
    if (not quat.isIdentity())
        this->rotate(resnum, atoms, quat.toMatrix(), point);
}

/** Translate the entire molecule by 'delta' */
void EditMolDataPvt::translate(const Vector &delta)
{
    if (not delta.isZero())
    {
        QMutableMapIterator<ResNum,EditResAtomSet> it(atms);

        while( it.hasNext() )
        {
            it.next();
            EditResAtomSet &resatms = it.value();

            ERAtomAccessor it2(resatms);
            while (it2.hasNext())
            {
                it2.next();
                it2.value() += delta;
            }
        }
    }
}

/** Rotate the entire molecule by the rotation matrix 'rotmat' about the point 'point'. */
void EditMolDataPvt::rotate(const Matrix &rotmat, const Vector &point)
{
    QMutableMapIterator<ResNum,EditResAtomSet> it(atms);

    while( it.hasNext() )
    {
        it.next();

        EditResAtomSet &resatms = it.value();

        ERAtomAccessor it2(resatms);
        while( it2.hasNext() )
        {
            it2.next();
            Atom &rotatm = it2.value();
            rotatm = SireMaths::rotate(rotatm, rotmat, point);
        }
    }
}

/** Rotate the entire molecule by the rotation matrix implied by the quaternion 'quat'
    about the point 'point' */
inline void EditMolDataPvt::rotate(const Quaternion &quat, const Vector &point)
{
    if (not quat.isIdentity())
        rotate(quat.toMatrix(), point);
}

/** Set the name of residue 'resnum' to 'resname'. This will throw an
    exception if there is no residue with this number in the molecule.

    \throw SireMol::missing_residue
*/
void EditMolDataPvt::setResidueName(ResNum resnum, const QString &resname)
{
    checkResidue(resnum);
    resnmes[resnum] = resname;
}

/** Clear the residue 'resnum'. Note that this doesn't delete the residue,
    it just removes all atoms and bonds in the residue, and all inter-residue
    bonds to the residue. This does nothing if there is no such residue
    in this molecule. */
void EditMolDataPvt::clearResidue(ResNum resnum)
{
    molbnds.removeAll(resnum);

    if (atms.contains(resnum))
        atms[resnum].clear();
}

/** Return whether or not this molecule is empty (has no atoms or residues)
    (note that a molecule with empty residues is not itself empty) */
bool EditMolDataPvt::isEmpty() const
{
    return atms.isEmpty();
}

/** Return whether or not the residue with residue number 'resnum' is empty.
    This returns true if there is no such residue with this name, or if there
    are no atoms in this residue. */
bool EditMolDataPvt::isEmptyResidue(ResNum resnum) const
{
    return (not atms.contains(resnum)) or atms[resnum].isEmpty();
}

/** Return the total number of residues in this molecule - this includes
    empty residues */
int EditMolDataPvt::nResidues() const
{
    return resnmes.count();
}

/** Return the total number of atoms in the molecule */
int EditMolDataPvt::nAtoms() const
{
    QMapIterator<ResNum, EditResAtomSet> it(atms);

    int nats = 0;

    while (it.hasNext())
    {
        it.next();
        nats += it.value().count();
    }

    return nats;
}

/** Return the number of atoms in the residue with number 'resnum'. This will
    throw an exception if there is no such residue in this molecule.

    \throw SireMol::missing_residue
*/
inline int EditMolDataPvt::nAtoms(ResNum resnum) const
{
    checkResidue(resnum);
    return atms[resnum].count();
}

/** Return the 'ith' atom in the residue 'resnum'. This will throw an exception
    if there is no such residue with this number, or if i < 0 or i >= natoms(resnum)

    \throw SireMol::missing_residue
    \throw SireMol::invalid_index
*/
const Atom& EditMolDataPvt::at(ResNum resnum, int i) const
{
    if (i < 0 or i >= nAtoms(resnum))
        throw SireError::invalid_index(QObject::tr(
            "Invalid index (%1) for EditRes[i] (%2:%3 in molecule \"%4\") - natoms() == %5")
                .arg(i).arg(residueName(resnum),resnum.toString(),name())
                    .arg(nAtoms(resnum)), CODELOC);

    return atms[resnum].at(i);
}

/** Return the names of all of the atoms in the residue, in an arbitrary order.
    This will throw an exception if there is no such residue in this molecule.

    \throw SireMol::missing_residue
*/
inline QStringList EditMolDataPvt::atomNames(ResNum resnum) const
{
    checkResidue(resnum);
    return atms[resnum].keys();
}

/** Return the geometric line representing the bond 'bnd'. This will throw
    an exception if the bond refers to atoms or residues that are not in this
    molecule.

    \throw SireMol::missing_atom
*/
SireMaths::Line EditMolDataPvt::bond(const Bond &bnd) const
{
    return SireMaths::Line( atom(bnd.atom0()), atom(bnd.atom1()) );
}

/** Return the geometric triangle representing the angle 'ang'. This will throw
    an exception if the angle refers to atoms or residues that are not in this
    molecule.

    \throw SireMol::missing_atom
*/
SireMaths::Triangle EditMolDataPvt::angle(const SireMol::Angle &ang) const
{
    return SireMaths::Triangle( atom(ang.atom0()), atom(ang.atom1()), atom(ang.atom2()) );
}

/** Return the geometric torsion representing the dihedral 'dih'. This will throw
    an exception if the dihedral refers to atoms or residues that are not in this
    molecule.

    \throw SireMol::missing_atom
*/
SireMaths::Torsion EditMolDataPvt::dihedral(const Dihedral &dih) const
{
    return SireMaths::Torsion( atom(dih.atom0()), atom(dih.atom1()),
                               atom(dih.atom2()), atom(dih.atom3()) );
}

/** Return the geometric torsion representing the improper angle 'improper'. This will throw
    an exception if the improper angle refers to atoms or residues that are not in this
    molecule.

    \throw SireMol::missing_atom
*/
SireMaths::Torsion EditMolDataPvt::improper(const Improper &improper) const
{
    return SireMaths::Torsion( atom(improper.atom0()), atom(improper.atom1()),
                               atom(improper.atom2()), atom(improper.atom3()) );
}

/** Return the relative weights of group0 and group1 of this molecule. An exception
    will be thrown if the groups contains any atoms that are not in this molecule.

    The returned weight will lie between 0 and 1. If the value is 0, then all
    of the weight is on group0. If the weight is 1, then all of the weight is
    on group1. If the weight is 0.5, then the weight is shared evenly between
    group0 and group1. If the weight is 0.25, then group0 has 75% of the weight
    while group1 has 25%. If the weight if 0.75 then group0 has 25% of the
    weight while group1 has 75%. (so in essence, it is the percentage of the
    weight that group1 has)

    \throw SireMol::missing_atom
    \throw SireMol::missing_residue
*/
double EditMolDataPvt::getWeight(const AtomIDGroup &group0, const AtomIDGroup &group1,
                                 const WeightFunction &weightfunc) const
{
    //create a calculator workspace for the weight calculation
    WeightCalculator calc(weightfunc);

    //add all of atoms in the groups to the calculator
    foreach( AtomIndex atm, group0.atoms() )
    {
        calc.addToA( atom(atm) );
    }

    foreach( AtomIndex atm, group1.atoms() )
    {
        calc.addToB( atom(atm) );
    }

    //now add all of the residues
    foreach( ResNum resnum, group0.residues() )
    {
        calc.addToA( atomVector(resnum) );
    }

    foreach( ResNum resnum, group1.residues() )
    {
        calc.addToB( atomVector(resnum) );
    }

    return calc.weight();
}

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
void EditMolDataPvt::change(const Bond &bnd, double delta,
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
void EditMolDataPvt::change(const SireMol::Angle &ang, const SireMaths::Angle &delta,
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
void EditMolDataPvt::change(const Bond &dih, const SireMaths::Angle &delta,
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
void EditMolDataPvt::change(const Improper &improper, const SireMaths::Angle &delta,
                            const AtomIDGroup &group0, const AtomIDGroup &group1,
                            const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors)
{
    throw SireError::incomplete_code(QObject::tr(
            "Need to write code to change improper angles!"), CODELOC);
}

///////////////
/////////////// Implementation of EditMolData
///////////////

/** Serialise an EditMolData to a datastream */
QDataStream& operator<<(QDataStream &ds, const EditMolData &mol)
{
    ds << mol.d();
    return ds;
}

/** Deserialise an EditMolData from a datastream - create a new
    copy to deserialise into... */
QDataStream& operator>>(QDataStream &ds, EditMolData &mol)
{
    mol.detach();
    ds >> mol.d();
    return ds;
}

/** Construct an empty molecule called 'molname' */
EditMolData::EditMolData(const QString &molname)
            : ptr( new QSharedDataPointer<EditMolDataPvt>(new EditMolDataPvt(molname)) )
{}

/** Construct an EditMol that represents the same molecule as 'mol' */
EditMolData::EditMolData(const Molecule &mol)
            : ptr( new QSharedDataPointer<EditMolDataPvt>(new EditMolDataPvt(mol)) )
{}

/** Copy constructor - This makes a shallow copy of other. Beware, as this may not be
    what you expect. However, it is a consequence of this class being explicitly
    shared. If you wish to get your own copy then ensure that you 'detach()' after
    making this copy. */
EditMolData::EditMolData(const EditMolData &other)
            : ptr( other.ptr )
{}

/** Destructor */
EditMolData::~EditMolData()
{}

/** Make a shallow copy of 'other' - see shallowCopy() */
EditMolData& EditMolData::operator=(const EditMolData &other)
{
    ptr = other.ptr;
    return *this;
}

/** Private function used to easily retrieve a const reference to the
    underlying EditMolDataPvt object */
const EditMolDataPvt& EditMolData::d() const
{
    return *(*ptr);
}

/** Private function used to easily retrieve a reference to the
    underlying EditMolDataPvt object */
EditMolDataPvt& EditMolData::d()
{
    return *(*ptr);
}

/** Comparison operator */
bool EditMolData::operator==(const EditMolData &other) const
{
    //two editmols are equal if they have the same atoms,
    //residues and bonds...
    return d() == other.d();
}

/** Comparison operator */
bool EditMolData::operator!=(const EditMolData &other) const
{
    return d() != other.d();
}

/** Detach this copy of the EditMolData from shared storage. You are guarnteed
    to be the only holder of the data after this call. */
void EditMolData::detach()
{
    ptr.reset( new QSharedDataPointer<EditMolDataPvt>( *ptr ) );
}

/** Return a deep copy of this EditMolData. The deep copy will have its own
    copy of the data, and changing it will not change the original. */
EditMolData EditMolData::deepCopy() const
{
    EditMolData mol(*this);
    mol.detach();
    return mol;
}

/** Return a shallow copy of this EditMolData. The shallow copy will share
    data with the original, and changing the copy will change the original. */
EditMolData EditMolData::shallowCopy()
{
    return EditMolData(*this);
}

/** Return an EditMol view of this data */
EditMol EditMolData::editmol()
{
    return EditMol(*this);
}

/** Return an EditRes view of residue 'resnum' in this molecule. This will throw
    an exception if there is no such residue in this molecule.

    \throw SireMol::missing_residue
*/
EditRes EditMolData::editres(ResNum resnum)
{
    //ensure that there is a residue with this number
    d().checkResidue(resnum);
    return EditRes(EditMolData(*this), resnum);
}

/** Return an EditRes view of the first residue called 'resnam'. This will throw
    an exception if there is no such residue in this molecule.

    \throw SireMol::missing_residue
*/
EditRes EditMolData::editres(const QString &resnam)
{
    QList<ResNum> resnums = residueNumbers(resnam);

    if (resnums.count() == 0)
        throw SireMol::missing_residue(QObject::tr(
            "There is no residue called \"%1\" in molecule \"%2\"")
                .arg(resnam).arg(name()), CODELOC);

    return editres(resnums[0]);
}

/** Return a list of EditRes that are bonded to residue 'rnum'. This throws an exception
    if there is no such residue. This returns a list of residues that are on a shallow
    copy of the molecule.

    \throw SireMol::missing_residue
*/
EditResSet EditMolData::residuesBondedTo(ResNum resnum)
{
    QList<ResNum> resnums = connectivity(resnum).residuesBondedTo();

    EditResSet residus;
    foreach( ResNum rnum, resnums )
        residus.insert( editres(rnum) );

    return residus;
}

/** Extract the residue with residue number 'resnum' - this returns an editres
    that is a deep copy of the residue in this molecule, but the returned
    editres is alone in the deep copy */
EditRes EditMolData::extract(ResNum resnum) const
{
    EditMolData newres = deepCopy();

    QList<ResNum> resnums = newres.residueNumbers();

    foreach( ResNum rnum, resnums )
    {
        if (rnum != resnum)
            newres.removeResidue(resnum);
    }

    return EditRes(newres, resnum);
}

/** Renumber the residue 'resnum' to 'newnum'.

    An exception will be thrown if either there isn't a residue with number 'resnum',
    or if a residue with number 'newnum' already exists.

    \throw SireMol::missing_residue
    \throw SireMol::duplicate_residue
*/
void EditMolData::renumberResidue(ResNum resnum, ResNum newnum)
{
    d().renumberResidue(resnum,newnum);
}

/** Return the 'ith' residue in the molecule as an EditRes. This will
    return a shallow copy EditRes (changing it will change this molecule).
    An exception will be thrown if i<0 or i>=nresidues

    \throw SireError::invalid_index
*/
EditRes EditMolData::at(int i)
{
    QList<ResNum> resnums = d().residueNumbers();

    if (i < 0 or i >= resnums.count())
        throw SireError::invalid_index(QObject::tr(
            "Index error accessing i=%1 for molecule \"%2\" (nresidues == %3)")
                .arg(i).arg(name()).arg(resnums.count()), CODELOC);

    return editres(resnums.at(i));
}

void EditMolData::setName(const QString &name)
{
    d().setName(name);
}

void EditMolData::setResidueName(ResNum resnum, const QString &name)
{
    d().setResidueName(resnum,name);
}

void EditMolData::clear()
{
    d().clear();
}

void EditMolData::clean()
{
    d().clean();
}

void EditMolData::clearResidue(ResNum resnum)
{
    d().clearResidue(resnum);
}

void EditMolData::finalise()
{
    d().finalise();
}

void EditMolData::add(const Atom &atom)
{
    d().add(atom);
}

EditRes EditMolData::addResidue(ResNum resnum, const QString &resnam)
{
    d().addResidue(resnum,resnam);
    return editres(resnum);
}

EditRes EditMolData::addResidue(ResNum resnum, const EditRes &tmpl)
{
    //add the residue
    EditRes newres = addResidue(resnum, tmpl.name());

    //now add the atoms and bonds of the template to the new residue
    newres.add(tmpl);

    return newres;
}

EditRes EditMolData::addResidue(ResNum resnum, const QString &resnam, const EditRes &tmpl)
{
    //add the residue
    EditRes newres = addResidue(resnum, resnam);

    //now add the atoms and bonds of the template to the new residue
    newres.add(tmpl);

    return newres;
}

void EditMolData::remove(const AtomIndex &atom)
{
    d().remove(atom);
}

void EditMolData::removeResidue(ResNum resnum)
{
    d().removeResidue(resnum);
}

void EditMolData::removeResidue(const QString &resnam)
{
    d().removeResidue(resnam);
}

void EditMolData::removeAllResidues(const QString &resnam)
{
    d().removeAllResidues(resnam);
}

void EditMolData::add(const Bond &bond)
{
    d().add(bond);
}

void EditMolData::remove(const Bond &bond)
{
    d().remove(bond);
}

void EditMolData::removeAllBonds(ResNum resnum)
{
    d().removeAllBonds(resnum);
}

void EditMolData::removeAllBonds(const AtomIndex &atom)
{
    d().removeAllBonds(atom);
}

void EditMolData::removeAllBonds()
{
    d().removeAllBonds();
}

void EditMolData::update(const Atom &atom)
{
    d().update(atom);
}

void EditMolData::update(const AtomIndex &atm, const Atom &details)
{
    d().update(atm,details);
}

void EditMolData::update(const AtomIndex &atm, const Element &element)
{
    d().update(atm,element);
}

void EditMolData::update(const AtomIndex &atm, const Vector &coords)
{
    d().update(atm,coords);
}

MoleculeBonds EditMolData::connectivity() const
{
    return d().connectivity();
}

ResidueBonds EditMolData::connectivity(ResNum resnum) const
{
    return d().connectivity(resnum);
}

MoleculeSignature EditMolData::signature() const
{
    return d().signature();
}

AtomSet EditMolData::atoms(ResNum resnum) const
{
    return d().atoms(resnum);
}

AtomVector EditMolData::atomVector(ResNum resnum) const
{
    return d().atomVector(resnum);
}

const QString& EditMolData::name() const
{
    return d().name();
}

bool EditMolData::isEmpty() const
{
    return d().isEmpty();
}

bool EditMolData::isEmptyResidue(ResNum resnum) const
{
    return d().isEmptyResidue(resnum);
}

QString EditMolData::residueName(ResNum resnum) const
{
    return d().residueName(resnum);
}

QList<ResNum> EditMolData::residueNumbers() const
{
    return d().residueNumbers();
}

QList<ResNum> EditMolData::residueNumbers(const QString &resnam) const
{
    return d().residueNumbers(resnam);
}

QStringList EditMolData::residueNames() const
{
    return d().residueNames();
}

ResidueIDSet EditMolData::residueIDs() const
{
    return d().residueIDs();
}

const Atom& EditMolData::atom(const AtomIndex &atm) const
{
    return d().atom(atm);
}

bool EditMolData::contains(ResNum resnum) const
{
    return d().contains(resnum);
}

bool EditMolData::contains(const AtomIndex &atm) const
{
    return d().contains(atm);
}

bool EditMolData::contains(const Bond &bond) const
{
    return d().contains(bond);
}

const Atom& EditMolData::at(ResNum resnum, int i) const
{
    return d().at(resnum,i);
}

int EditMolData::nResidues() const
{
    return d().nResidues();
}

int EditMolData::nAtoms() const
{
    return d().nAtoms();
}

int EditMolData::nAtoms(ResNum resnum) const
{
    return d().nAtoms(resnum);
}

QStringList EditMolData::atomNames(ResNum resnum) const
{
    return d().atomNames(resnum);
}

AtomSet EditMolData::atoms() const
{
    return d().atoms();
}

AtomVector EditMolData::atomVector() const
{
    return d().atomVector();
}

void EditMolData::translate(const AtomIndex &atom, const Vector &delta)
{
    d().translate(atom, delta);
}

void EditMolData::rotate(const AtomIndex &atom, const Quaternion &quat, const Vector &point)
{
    d().rotate(atom, quat, point);
}

void EditMolData::translate(const AtomIDGroup &group, const Vector &delta)
{
    d().translate(group, delta);
}

void EditMolData::rotate(const AtomIDGroup &group, const Quaternion &quat, const Vector &point)
{
    d().rotate(group, quat, point);
}

void EditMolData::translate(ResNum resnum, const Vector &delta)
{
    d().translate(resnum, delta);
}

void EditMolData::rotate(ResNum resnum, const Quaternion &quat, const Vector &point)
{
    d().rotate(resnum, quat, point);
}

void EditMolData::translate(ResNum resnum, const QStringList &atoms, const Vector &delta)
{
    d().translate(resnum,atoms,delta);
}

void EditMolData::rotate(ResNum resnum, const QStringList &atoms,
                         const Quaternion &quat, const Vector &point)
{
    d().rotate(resnum,atoms,quat,point);
}

void EditMolData::translate(const Vector &delta)
{
    d().translate(delta);
}

void EditMolData::rotate(const Quaternion &quat, const Vector &point)
{
    d().rotate(quat,point);
}

double EditMolData::getWeight(const AtomIDGroup &group0, const AtomIDGroup &group1,
                            const WeightFunction &weightfunc) const
{
    return d().getWeight(group0,group1,weightfunc);
}

/** Return the geometric line representing the bond 'bnd'. This will throw
    an exception if the bond refers to atoms or residues that are not in this
    molecule.

    \throw SireMol::missing_atom
*/
SireMaths::Line EditMolData::bond(const Bond &bnd) const
{
    return d().bond(bnd);
}

/** Return the geometric triangle representing the angle 'ang'. This will throw
    an exception if the angle refers to atoms or residues that are not in this
    molecule.

    \throw SireMol::missing_atom
*/
SireMaths::Triangle EditMolData::angle(const SireMol::Angle &ang) const
{
    return d().angle(ang);
}

/** Return the geometric torsion representing the dihedral 'dih'. This will throw
    an exception if the dihedral refers to atoms or residues that are not in this
    molecule.

    \throw SireMol::missing_atom
*/
SireMaths::Torsion EditMolData::dihedral(const Dihedral &dih) const
{
    return d().dihedral(dih);
}

/** Return the geometric torsion representing the improper angle 'improper'. This will throw
    an exception if the improper angle refers to atoms or residues that are not in this
    molecule.

    \throw SireMol::missing_atom
*/
SireMaths::Torsion EditMolData::improper(const Improper &improper) const
{
    return d().improper(improper);
}

/** Return the length of the bond 'bnd'. This will throw an exception if this
    bond refers to atoms that are not in this molecule.

    \throw SireMol::missing_atom
*/
double EditMolData::measure(const Bond &bnd) const
{
    return bond(bnd).length();
}

/** Return the size of the angle 'ang'. This will throw an exception if this
    angle refers to atoms that are not in this molecule.

    \throw SireMol::missing_atom
*/
SireMaths::Angle EditMolData::measure(const SireMol::Angle &ang) const
{
    return angle(ang).angle1();
}

/** Return the size of the dihedral 'dih'. This will throw an exception if this
    dihedral refers to atoms that are not in this molecule.

    \throw SireMol::missing_atom
*/
SireMaths::Angle EditMolData::measure(const Dihedral &dih) const
{
    return dihedral(dih).angle();
}

/** Return the size of the improper angle 'ang'. This will throw an exception if this
    improper angle refers to atoms that are not in this molecule.

    \throw SireMol::missing_atom
*/
SireMaths::Angle EditMolData::measure(const Improper &improper) const
{
    throw SireError::incomplete_code(QObject::tr(
        "Need to write code to measure an improper angle..."), CODELOC);

    return 0.0;
}

void EditMolData::change(const Bond &bnd, double delta,
                         const AtomIDGroup &group0, const AtomIDGroup &group1,
                         const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors)
{
    d().change(bnd, delta, group0, group1, weightfunc, anchors);
}

void EditMolData::change(const SireMol::Angle &ang, const SireMaths::Angle &delta,
                         const AtomIDGroup &group0, const AtomIDGroup &group1,
                         const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors)
{
    d().change(ang, delta, group0, group1, weightfunc, anchors);
}

void EditMolData::change(const Bond &bnd, const SireMaths::Angle &delta,
                         const AtomIDGroup &group0, const AtomIDGroup &group1,
                         const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors)
{
    d().change(bnd, delta, group0, group1, weightfunc, anchors);
}

void EditMolData::change(const Improper &improper, const SireMaths::Angle &delta,
                         const AtomIDGroup &group0, const AtomIDGroup &group1,
                         const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors)
{
    d().change(improper, delta, group0, group1, weightfunc, anchors);
}
