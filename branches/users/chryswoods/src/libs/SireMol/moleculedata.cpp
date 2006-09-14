/**
  * @file
  *
  * C++ Implementation: MoleculeData and MoleculeDataPvt
  *
  * Description:
  * Implementation of public MoleculeData class,
  * and the private MoleculeDataPvt class.
  *
  * @author Christopher Woods, (C) 2006
  *
  * Copyright: See COPYING file that comes with this distribution
  *
  */

#include "qhash_siremol.h"

#include <QDataStream>
#include <QSharedData>
#include <QMutex>

#include "moleculedata.h"
#include "residuebonds.h"
#include "cutgroup.h"
#include "cutgroupset.h"
#include "molecule.h"
#include "residue.h"
#include "bond.h"
#include "angle.h"
#include "dihedral.h"
#include "improper.h"
#include "atomidgroup.h"
#include "weightfunction.h"
#include "convertfunction.h"
#include "moleculeinfo.h"

#include "SireMol/errors.h"

#include "SireMaths/angle.h"
#include "SireMaths/quaternion.h"
#include "SireMaths/matrix.h"
#include "SireMaths/line.h"
#include "SireMaths/triangle.h"
#include "SireMaths/torsion.h"
#include "SireMaths/rotate.h"

#include "SireStream/datastream.h"

using namespace SireStream;

QDataStream& operator<<(QDataStream&, const SireMol::MoleculeDataPvt&);
QDataStream& operator>>(QDataStream&, SireMol::MoleculeDataPvt&);

namespace SireMol
{

using SireMaths::Matrix;
using SireMaths::Quaternion;

/** This class is a simple holder for the molecule's metadata, allowing
    it to be implicitly shared. The reason for breaking off this data is
    because the normal situation will be that only the coordinates of the
    molecule will be changed. Changing the coordinates should not cause
    duplication of any other data in the molecule. By holding all of the
    other data in an implicitly shared class we ensure that it is not
    duplicated when the molecule is moved.

    @author Christopher Woods
*/
class MolMetaData : public QSharedData
{
public:
    /** Null constructor */
    MolMetaDataPvt()
            : QSharedData(),
              id(0)
    {}

    /** Copy constructor */
    MolMetaDataPvt(const MolMetaDataPvt &other)
            : QSharedData(),
              id( other.id ),
              molversion( other.molversion ),
              molinfo( other.molinfo ),
              molbonds( other.molbonds )
    {}

    /** Destructor */
    ~MolMetaDataPvt()
    {}

    /** Assignment operator */
    MolMetaData& operator=(const MolMetaData &other)
    {
        id = other.id;
        molversion = other.molversion;
        molinfo = other.molinfo;
        molbonds = other.molbonds;
        return *this;
    }

};

/**
This class is the private implementation of MoleculeData. You should not use
this class in your own code.

@author Christopher Woods
*/
class MoleculeDataPvt : public QSharedData
{

friend QDataStream& ::operator<<(QDataStream&, const MoleculeDataPvt&);
friend QDataStream& ::operator>>(QDataStream&, MoleculeDataPvt&);

public:
    MoleculeData();

    MoleculeData(const EditMol &editmol,
                 const ConvertFunction &convertfunc);

    MoleculeData(const MoleculeData &other);

    ~MoleculeData();

   ////// Dealing with the ID number and version ///////////
     MoleculeID ID() const;
     void setNewID();

     const MoleculeVersion& version() const;
     void incrementMajorVersion();
     void incrementMinorVersion();
   /////////////////////////////////////////////////////////


   ////// Operators ////////////////////////////////////////
    MoleculeData& operator=(const MoleculeData &other);

    bool operator==(const MoleculeData &other) const;
    bool operator!=(const MoleculeData &other) const;
   /////////////////////////////////////////////////////////


   ///// Interfacing with Molecule and Residue /////////////
    Molecule molecule() const;

    Residue residue(ResNum resnum) const;
    Residue residue(ResID resid) const;
    Residue residue(const QString &resname) const;

    void merge(const Residue &residue);

    QHash<ResNum,Residue> residues() const;

    Residue at(ResNum resnum) const;
    Residue at(ResID i) const;
   /////////////////////////////////////////////////////////


   ///// Querying the molecule /////////////////////////////
    const MoleculeBonds& connectivity() const;
    ResidueBonds connectivity(ResNum resnum) const;

    const MoleculeInfo& info() const;

    QList<Atom> atoms() const;
    QList<Atom> atoms(ResNum resnum) const;
    QList<Atom> atoms(ResID resid) const;

    QVector<Atom> atomVector() const;
    QVector<Atom> atomVector(ResNum resnum) const;
    QVector<Atom> atomVector(ResID resid) const;

    QVector<CutGroup> cutGroups() const;
    QVector<CutGroup> cutGroups(ResNum resnum) const;
    QVector<CutGroup> cutGroups(ResID resid) const;

    CutGroup cutGroup(CutGroupID id) const;

    const QVector<CoordGroup>& coordinates() const;
    QVector<CoordGroup> coordinates(ResNum resnum) const;
    QVector<CoordGroup> coordinates(ResID resid) const;

    CoordGroup coordinates(CutGroupID id) const;

    Atom atom(CutGroupID cgid, AtomID atomid) const;
    Atom atom(ResNum resnum, AtomID atomid) const;
    Atom atom(ResID resid, AtomID atomid) const;
    Atom atom(const AtomIndex &atm) const;

    Vector coordinates(CutGroupID cgid, AtomID atomid) const;
    Vector coordinates(ResNum resnum, AtomID atomid) const;
    Vector coordinates(ResID resid, AtomID atomid) const;
    Vector coordinates(const AtomIndex &atm) const;

    const QString& name() const;

    QString residueName(ResNum resnum) const;
    QString residueName(ResID resid) const;

    ResNum residueNumber(ResID resid) const;

    bool isEmpty() const;
    bool isEmpty(ResNum resnum) const;
    bool isEmpty(ResID resid) const;

    QList<ResNum> residueNumbers() const;
    QStringList residueNames() const;

    QList<ResNum> residueNumbers(const QString &resnam) const;

    QHash<ResNum,Residue> residuesBondedTo(ResNum resnum) const;
    QHash<ResNum,Residue> residuesBondedTo(ResID resid) const;

    bool contains(CutGroupID cgid) const;
    bool contains(ResNum resnum) const;
    bool contains(ResID resid) const;

    bool contains(const AtomIndex &atm) const;
    bool contains(CutGroupID cgid, AtomID atomid) const;
    bool contains(ResNum resnum, AtomID atomid) const;
    bool contains(ResID resid, AtomID atomid) const;

    bool contains(const Bond &bond) const;

    int nAtoms() const;
    int nAtoms(ResNum resnum) const;
    int nAtoms(ResID resid) const;
    int nAtoms(CutGroupID id) const;

    int nResidues() const;

    int nCutGroups() const;

    QStringList atomNames(ResNum resnum) const;
    QStringList atomNames(ResID resid) const;

    void checkResidue(ResNum) const;
    void checkResidue(ResID) const;

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
    void translate(ResNum resnum, const QStringList &atoms, const Vector &delta);
    void translate(ResNum resnum, const Vector &delta);
    void translate(ResID resid, const QStringList &atoms, const Vector &delta);
    void translate(ResID resid, const Vector &delta);

    void rotate(const Quaternion &quat, const Vector &point);
    void rotate(const AtomIDGroup &group, const Quaternion &quat, const Vector &point);
    void rotate(const AtomIndex &atom, const Quaternion &quat, const Vector &point);
    void rotate(ResNum resnum, const QStringList &atoms, const Quaternion &quat,
                const Vector &point);
    void rotate(ResNum resnum, const Quaternion &quat, const Vector &point);
    void rotate(ResID resid, const QStringList &atoms, const Quaternion &quat,
                const Vector &point);
    void rotate(ResID resid, const Quaternion &quat, const Vector &point);

    void rotate(const Matrix &matrix, const Vector &point);
    void rotate(const AtomIDGroup &group, const Matrix &matrix, const Vector &point);
    void rotate(const AtomIndex &atom, const Matrix &matrix, const Vector &point);
    void rotate(ResNum resnum, const QStringList &atoms, const Matrix &matrix,
                const Vector &point);
    void rotate(ResNum resnum, const Matrix &matrix, const Vector &point);
    void rotate(ResID resid, const QStringList &atoms, const Matrix &matrix,
                const Vector &point);
    void rotate(ResID resid, const Matrix &matrix, const Vector &point);
   /////////////////////////////////////////////////


   //////// Internal geometry moves ////////////////
    void change(const Bond &bnd, double delta,
                const AtomIDGroup &group0, const AtomIDGroup &group1,
                const WeightFunction &weightfunc, const AtomIndexSet &anchors);

    void change(const SireMol::Angle &ang, const SireMaths::Angle &delta,
                const AtomIDGroup &group0, const AtomIDGroup &group1,
                const WeightFunction &weightfunc, const AtomIndexSet &anchors);

    void change(const Bond &bnd, const SireMaths::Angle &delta,
                const AtomIDGroup &group0, const AtomIDGroup &group1,
                const WeightFunction &weightfunc, const AtomIndexSet &anchors);

    void change(const Improper &improper, const SireMaths::Angle &delta,
                const AtomIDGroup &group0, const AtomIDGroup &group1,
                const WeightFunction &weightfunc, const AtomIndexSet &anchors);
   /////////////////////////////////////////////////

private:

    static MoleculeID getNewID();
    static QMutex idmutex;
    static MoleculeID lastid;

    /** ID number used to identify the molecule */
    MoleculeID _id;

    /** The version number of the metadata */
    MoleculeVersion _molversion;

    /** The metainfo about the molecule - this contains the names of the molecule,
        residue and all atoms, and additional metainfo about all of the residues
        and atoms. This object may also be used to map from atom or residue IDs
        to CGAtomIDs (which are used to lookup the coordinates) */
    MoleculeInfo _molinfo;

    /** The connectivity of this molecule */
    MoleculeBonds _molbonds;

    /** The coordinates of all of the atoms in this molecule, arranged into
        CoordGroups, with the index into the vector being the CutGroupID
        of the CutGroup to which these coordinates belong. */
    QVector<CoordGroup> _coords;

};

} // End of namespace SireMol

using namespace SireMol;

////////////
//////////// Implementation of MoleculeDataPvt
////////////

static const RegisterMetaType<MoleculeDataPvt> r_pvt("SireMol::MoleculeDataPvt",
                                                     MAGIC_ONLY);

/** Serialise to a binary data stream */
QDataStream &operator<<(QDataStream &ds, const MoleculeDataPvt &moldata)
{
    writeHeader(ds, r_pvt, 1) << moldata.id() << moldata.molversion()
                              << moldata.coords() << moldata.molinfo()
                              << moldata.molbonds();
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream &operator>>(QDataStream &ds, MoleculeDataPvt &moldata)
{
    VersionID v = readHeader(ds, r_pvt);

    if (v == 1)
    {
        ds >> moldata.id() >> moldata.molversion()
           >> moldata.coords() >> moldata.molinfo()
           >> moldata.molbonds();
    }
    else
        throw version_error(v, "1", r_pvt, CODELOC);

    return ds;
}

/** Mutex to serialise the aquisition of new ID numbers */
QMutex MoleculeDataPvt::idmutex;

/** The last allocated ID number */
MoleculeID MoleculeDataPvt::lastid(0);

/** Get a new, unique, ID number */
MoleculeID MoleculeDataPvt::getNewID()
{
    QMutexLocker lkr(&idmutex);
    ++lastid;
    return lastid;
}

/** Null constructor */
MoleculeDataPvt::MoleculeDataPvt() : QSharedData()
{}

/** Construct the molecule data from an EditMol using the conversion function
    'converter'
*/
MoleculeDataPvt::MoleculeDataPvt(const EditMol &editmol,
                                 const ConvertFunction &converter)
                : QSharedData()
{
    //ok - lets build everything.


    //ok - we're done. Just have to give the molecule a new ID number :-)
    this->setNewID();
}

/** Copy constructor */
MoleculeDataPvt::MoleculeDataPvt(const MoleculeDataPvt &other)
                : QSharedData(),
                  _id(other._id),
                  _molversion(other._molversion),
                  _molinfo(other._molinfo),
                  _molbonds(other._molbonds),
                  _coords(other._coords)
{}

/** Destructor */
MoleculeDataPvt::~MoleculeDataPvt()
{}

/** Comparison operator */
bool MoleculeDataPvt::operator==(const MoleculeDataPvt &other) const
{
    return _id == other._id and
           _molversion == other._molversion and
           _molinfo == other._molinfo and
           _molbonds == other._molbonds and
           _coords == other._coords;
}

/** Comparison operator */
bool MoleculeDataPvt::operator!=(const MoleculeDataPvt &other) const
{
    return _id != other._id or
           _molversion != other._molversion or
           _molinfo != other._molinfo or
           _molbonds != other._molbonds or
           _coords != other._coords;
}

/** Return the name of this molecule */
inline const QString& MoleculeDataPvt::name() const
{
    return info().name();
}

/** Return the number of atoms in this molecule */
inline int MoleculeDataPvt::nAtoms() const
{
    return info().nAtoms();
}

/** Return the number of atoms in residue 'resnum'

    \throw SireMol::missing_residue
*/
inline int MoleculeDataPvt::nAtoms(ResNum resnum) const
{
    return info().residue(resnum).nAtoms();
}

/** Return the number of residues in the molecule. */
inline int MoleculeDataPvt::nResidues() const
{
    return info().nResidues();
}

/** Return the number of bonds in the molecule */
inline int MoleculeDataPvt::nBonds() const
{
    return connectivity().nBonds();
}

/** Check that the residue 'resnum' exists - else throw an exception */
inline void MoleculeDataPvt::checkResidue(ResNum resnum) const
{
    if (not info().contains(resnum))
        throw SireMol::missing_residue(QObject::tr(
            "Molecule \"%1\" does not contain a residue with number \"%2\"")
                .arg(name(),resnum.toString()), CODELOC);
}

/** Check that the residue with index 'resid' exists - else throw
    an exception */
inline void MoleculeDataPvt::checkResidue(ResID resid) const
{
    if (not info().contains(resid))
        throw SireError::invalid_index(QObject::tr(
            "Molecule \"%1\" has no residue with index \"%2\" (nResidues() == %3)")
                .arg(name()).arg(resid).arg(nResidues()), CODELOC);
}

/** Return the ID number of the molecule */
inline MoleculeID MoleculeDataPvt::ID() const
{
    return _id;
}

/** Give this molecule a new ID number. This will also give a new
    ID number to all contained (identified) objects, e.g. CutGroups */
void MoleculeDataPvt::setNewID()
{
    //get a new ID number
    _id = getNewID();

    //now reset the version number to 1.0
    _molversion = MoleculeVersion(1,0);
}

/** Return the version number of the molecule */
inline const MoleculeVersion& MoleculeDataPvt::version() const
{
    return _molversion;
}

/** Increment the major version of the molecule */
inline void MoleculeDataPvt::incrementMajorVersion()
{
    _molversion.incrementMajor();
}

/** Increment the minor version number of the molecule */
inline void MoleculeDataPvt::incrementMinorVersion()
{
    _molversion.incrementMinor();
}

/** Return the connectivity of this molecule */
inline MoleculeBonds MoleculeDataPvt::connectivity() const
{
    return _molbonds;
}

/** Return the connectivity of residue 'resnum'

    \throw SireMol::missing_residue
*/
inline ResidueBonds MoleculeDataPvt::connectivity(ResNum resnum) const
{
    checkResidue(resnum);
    return connectivity().residue(resnum);
}

/** Return a const reference to the molecule info */
inline const MoleculeInfo& MoleculeDataPvt::info() const
{
    return _info;
}

/** Return the number of CutGroups in the molecule */
inline int MoleculeDataPvt::nCutGroups() const
{
    return _coords.count();
}

/** Check the CutGroupID is valid */
inline void MoleculeDataPvt::checkCutGroup(CutGroupID id) const
{
    if ( id >= nCutGroups() )
        throw SireMol::missing_cutgroup(QObject::tr(
            "There is no CutGroup with ID == %1 in Molecule \"%2\" (nCutGroups() == %3)")
                .arg(id),arg(name()).arg(nCutGroups()), CODELOC);
}

/** Return the CutGroup with CutGroupID == id

    \throw SireMol::missing_cutgroup
*/
inline CutGroup MoleculeDataPvt::cutGroup(CutGroupID id) const
{
    return CutGroup( info().atomGroup(id), coordinates(id) );
}

/** Return the coordinates for the CutGroup with CutGroupID == id

    \throw SireMol::missing_cutgroup
*/
inline CoordGroup MoleculeDataPvt::coordinates(CutGroupID id) const
{
    checkCutGroup(id);
    return _coords.at(id);
}

/** Return the Atom with CGAtomID 'cgatomid'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
inline Atom MoleculeDataPvt::atom(const CGAtomID &cgatomid) const
{
    return Atom( info().atom(cgatomid), coordinates(cgatomid) );
}

/** Return the Atom at index 'atmid' in the CutGroup with ID 'cgid'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
inline Atom MoleculeDataPvt::atom(CutGroupID cgid, AtomID atmid) const
{
    return atom( CGAtomID(cgid,atmid) );
}

/** Return the coordinates of the atom with CGAtomID 'cgatomid'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
inline Vector MoleculeDataPvt::coordinates(const CGAtomID &cgatomid) const
{
    return coordinates(cgatomid.cutGroupID()).at(cgatomid.atomID());
}

/** Return the coordinates of the Atom at index 'atmid' in the CutGroup
    with ID 'cgid'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
inline Vector MoleculeDataPvt::coordinates(CutGroupID cgid, AtomID atmid) const
{
    return coordinates(cgid).at(atmid);
}

/** Return an array of all of the atoms in residue 'resnum', in the same
    order as they were added to the residue.

    \throw SireMol::missing_residue
*/
inline QVector<Atom> MoleculeDataPvt::atoms(ResNum resnum) const
{
    //get the metainfo for this residue
    ResidueInfo resinfo = info().residue(resnum);

    //create space for all of the atoms
    QVector<Atom> atms;
    int nats = resinfo.nAtoms();
    atms.reserve(nats);

    //copy all of the atoms
    for (AtomID i=0; i<nats; ++i)
    {
        atms += atom( resinfo[i] );
    }

    return atms;
}

/** Return an array of all of the atoms in this molecule. This
    returns the atoms in the order of the residues (ordered in
    the order the residues were added to the molecule) then
    the atoms ordered in the order that they were added to the
    residue. This function can be slow as it copies all of the
    atoms that are in the molecule.
*/
inline QVector<Atom> MoleculeDataPvt::atoms() const
{
    QVector<Atom> atms;
    int nats = info().nAtoms();
    atms.reserve(nats);

    //loop over all residues in order...
    int nres = info().nResidues();
    for (ResID i=0; i < nres; ++i)
    {
        ResidueInfo resinfo = info().residue(i);

        int nresatms = resinfo.nAtoms();

        //loop over all atoms in order, and add them to the vector
        for (AtomID j=0; j<nresatms; ++j)
            atms += atom( resinfo[j] );
    }

    return atms;
}

/** Return a hash of all of the CutGroups in the molecule, indexed by their
    CutGroupID
*/
inline QHash<CutGroupID,CutGroup> MoleculeDataPvt::cutGroups() const
{
    QHash<CutGroupID,CutGroup> cgroups;

    int ncg = info().nCutGroups();
    cgroups.reserve(ncg);

    //the CutGroups are numbered sequentially from 0 to nCutGroups()-1
    for (int CutGroupID i=0; i<ncg; ++i)
    {
        cgroups.insert( i, cutGroup(i) );
    }

    return cgset;
}

/** Internal function used to return the hash of all CutGroups in the
    residue described by 'resinfo' */
inline QHash<CutGroupID,CutGroup>
MoleculeDataPvt::cutGroups(const ResidueInfo &resinfo) const
{
    QHash<CutGroupID,AtomInfoGroup> resgroups = resinfo.atomGroups();

    //create space for the CutGroups
    QHash<CutGroupID,CutGroup> cgroups;
    int ncg = resgroups.count();
    cgroups.reserve(ncg);

    for (QHash<CutGroupID,AtomInfoGroup>::const_iterator it = resgroups.constBegin();
         it != resgroups.constEnd();
         ++it)
    {
        cgroups.insert( it.key(), CutGroup(*it, coordinates(it.key())) );
    }

    return resgroups;
}

/** Return the hash of all CutGroups in the residue 'resnum', indexed
    by CutGroupID

    \throw SireMol::missing_residue
*/
inline QHash<CutGroupID,CutGroup>
MoleculeDataPvt::cutGroups(ResNum resnum) const
{
    return cutGroups( info().residue(resnum) );
}

/** Return a hash of all CutGroups in the residue with index 'resid', indexed
    by CutGroupID

    \throw SireError::invalid_index
*/
inline QHash<CutGroupID,CutGroup>
MoleculeDataPvt::cutGroups(ResID resid) const
{
    return cutGroups( info().residue(resid) );
}

/** Return a hash of all of the coordinates of atoms in this molecule,
    organised by CoordGroup, and indexed by CutGroupID */
inline QHash<CutGroupID,CoordGroup>
MoleculeDataPvt::coordinates() const
{
    return _coords;
}

/** Internal function used to return a hash of the CoordGroups containing the
    coordinates of the residue described by 'resinfo' */
inline QHash<CutGroupID,CoordGroup>
MoleculeDataPvt::coordinates(const ResidueInfo &resinfo) const
{
    QHash<CutGroupID, AtomInfoGroup> resgroups = resinfo.atomGroups();

    QHash<CutGroupID, CoordGroup> coordgroups;
    int ncg = resgroups.count();
    coordgroups.reserve(ncg);

    for (QHash<CutGroupID,AtomInfoGroup>::const_iterator it = resgroups.constBegin();
         it != resgroups.constEnd();
         ++it)
    {
        coordgroups.insert( it.key(), coordinates(it.key()) );
    }

    return coordgroups;
}

/** Return the CoordGroups that contain the coordinates of the atoms in the
    residue with number 'resnum'. The CoordGroups are returned as a hash,
    indexed by CutGroupID. Note that the returned CoordGroups may also contain
    coordinates for atoms that are not in this residue.

    \throw SireMol::missing_residue
*/
inline QHash<CutGroupID,CoordGroup>
MoleculeDataPvt::coordinates(ResNum resnum) const
{
    return coordinates( info().residue(resnum) );
}

/** Return the CoordGroups that contain the coordinates of the atoms in the
    residue at index 'resid'. The CoordGroups are returned as a hash,
    indexed by CutGroupID. Note that the returned CoordGroups may also contain
    coordinates for atoms that are not in this residue.

    \throw SireError::invalid_index
*/
inline QHash<CutGroupID,CoordGroup>
MoleculeDataPvt::coordinates(ResID resid) const
{
    return coordinates( info().residue(resid) );
}

/** Return the name of residue 'resnum'

    \throw SireMol::missing_residue
*/
inline QString MoleculeDataPvt::residueName(ResNum resnum) const
{
    return info().residue(resnum).name();
}

/** Return the name of the residue at index 'resid'

    \throw SireError::invalid_index
*/
inline QString MoleculeDataPvt::residueName(ResID resid) const
{
    return info().residue(resid).name();
}

/** Return whether or not this is an empty molecule (no residues or atoms) */
inline bool MoleculeDataPvt::isEmpty() const
{
    return nResidues() == 0;
}

/** Return whether residue 'resnum' is empty (no atoms)

    \throw SireMol::missing_residue
*/
inline bool MoleculeDataPvt::isEmpty(ResNum resnum) const
{
    return nAtoms(resnum) == 0;
}

/** Return whether or not the residue at index 'resid' is empty

    \throw SireError::invalid_index
*/
inline bool MoleculeDataPvt::isEmpty(ResID resid) const
{
    return nAtoms(resid) == 0;
}

/** Return the list of residue numbers in this molecule */
inline QVector<ResNum> MoleculeDataPvt::residueNumbers() const
{
    return info().residueNumbers();
}

/** Return the list of residue names in this molecule */
inline QStringList MoleculeDataPvt::residueNames() const
{
    return info().residueNames();
}

/** Return the list of residue numbers of residues called 'resnam'.
    Returns an empty list if there are no residues called 'resnam' */
inline QVector<ResNum> MoleculeDataPvt::residueNumbers(const QString &resnam) const
{
    return info().residueNumbers(resnam);
}

/** Return the atom with index 'atm'

    \throw SireMol::missing_atom
*/
inline Atom MoleculeDataPvt::atom(const AtomIndex &atm) const
{
    return atom( info().at(atm) );
}

/** Return the atom at index 'atmid' in the residue with number 'resnum'

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
inline Atom MoleculeDataPvt::atom(ResNum resnum, AtomID atmid) const
{
    return atom( info().at(ResNumAtomID(resnum,atmid)) );
}

/** Return the atom at index 'atmid' in the residue at index 'resid'

    \throw SireError::invalid_index
*/
inline Atom MoleculeDataPvt::atom(ResID resid, AtomID atmid) const
{
    return atom( info().at(ResIDAtomID(resid,atmid)) );
}

/** Return the coordinates of the atom with ID 'atm'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
inline Vector MoleculeDataPvt::coordinates(const AtomIndex &atm) const
{
    return coordinates( info().at(atm) );
}

/** Return the coordinates of the atom at index 'atmid' in the residue
    with number 'resnum'

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
inline Vector MoleculeDataPvt::coordinates(ResNum resnum, AtomID atomid) const
{
    return coordinates( info().at(ResNumAtomID(resnum,atomid)) );
}

/** Return the coordinates of the atom at index 'atmid' in the
    residue at index 'resid'

    \throw SireError::invalid_index
*/
inline Vector MoleculeDataPvt::coordinates(ResID resid, AtomID atomid) const
{
    return coordinates( info().at(ResIDAtomID(resid,atomid)) );
}

/** Return whether this molecule contains a residue with number 'resnum' */
inline bool MoleculeDataPvt::contains(ResNum resnum) const
{
    return info().contains(resnum);
}

/** Return whether this molecule contains an atom with index 'atm' */
inline bool MoleculeDataPvt::contains(const AtomIndex &atm) const
{
    return info().contains(atm);
}

/** Return whether this molecule contains the bond 'bnd' */
inline bool MoleculeDataPvt::contains(const Bond &bond) const
{
    return connectivity().contains(bond);
}

/** Return the number of atoms in the CutGroup with ID = id

    \throw SireMol::missing_cutgroup
*/
inline int MoleculeDataPvt::nAtoms(CutGroupID id) const
{
    return info().nAtoms( id );
}

/** Return the name of all of the atoms in residue 'resnum' */
inline QStringList MoleculeDataPvt::atomNames(ResNum resnum) const
{
    return info().residue(resnum).atomNames();
}

/** Return the geometric line representing the bond 'bnd'

    \throw SireMol::missing_atom
*/
inline SireMaths::Line MoleculeDataPvt::bond(const Bond &bnd) const
{
    return SireMaths::Line( coordinates(bnd.atom0()),
                            coordinates(bnd.atom1()) );
}

/** Return the geometric triangle representing the angle 'ang'

    \throw SireMol::missing_atom
*/
inline SireMaths::Triangle MoleculeDataPvt::angle(const SireMol::Angle &ang) const
{
    return SireMaths::Triangle( coordinates(ang.atom0()),
                                coordinates(ang.atom1()),
                                coordinates(ang.atom2()) );
}

/** Return the geometric torsion representing the dihedral 'dih'

    \throw SireMol::missing_atom
*/
inline SireMaths::Torsion MoleculeDataPvt::dihedral(const Dihedral &dih) const
{
    return SireMaths::Torsion( coordinates(dih.atom0()),
                               coordinates(dih.atom1()),
                               coordinates(dih.atom2()),
                               coordinates(dih.atom3()) );
}

/** Return the geometric line representing the improper angle 'imp'

    \throw SireMol::missing_atom
*/
inline SireMaths::Torsion MoleculeDataPvt::improper(const Improper &imp) const
{
    return SireMaths::Torsion( coordinates(imp.atom0()),
                               coordinates(imp.atom1()),
                               coordinates(imp.atom2()),
                               coordinates(imp.atom3()) );
}

/** Return the length of the bond 'bnd'

    \throw SireMol::missing_atom
*/
inline double MoleculeDataPvt::measure(const Bond &bnd) const
{
    return bond(bnd).length();
}

/** Return the size of the angle 'ang'

    \throw SireMol::missing_atom
*/
inline SireMaths::Angle MoleculeDataPvt::measure(const SireMol::Angle &ang) const
{
    return angle(ang).angle1();
}

/** Return the size of the dihedral 'dih'

    \throw SireMol::missing_atom
*/
inline SireMaths::Angle MoleculeDataPvt::measure(const Dihedral &dih) const
{
    return dihedral(dih).angle();
}

/** Return the size of the improper angle 'imp'

    \throw SireMol::missing_atom
*/
inline SireMaths::Angle MoleculeDataPvt::measure(const Improper &improper) const
{
    throw SireError::incomplete_code("Need to write this!", CODELOC);

    return 0.0;
}

/** Return the relative weights of 'group0' and 'group1' using the weight function
    'weightfunc'.

    \throw SireMol::missing_atom
    \throw SireMol::missing_residue
*/
double MoleculeDataPvt::getWeight(const AtomIDGroup &group0, const AtomIDGroup &group1,
                                  const WeightFunction &weightfunc) const
{
    WeightCalculator calc(weightfunc);

#warning MoleculeDataPvt::getWeight() needs refactoring as it is inefficient!

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

/** Small private class used to provide a workspace for moving lots of parts
    of the molecule. This class ensures that meta-data is only updated at the
    end of the move (e.g. AABoxes) and that the state of the molecule is maintained
    (i.e. either the whole move is commited, or none of the move is commited)

    @author Christopher Woods
*/
class MoveWorkspace
{
public:
    MoveWorkspace( QHash<CutGroupID,CoordGroup> &coords )
                : _coords(coords)
    {}

    ~MoveWorkspace()
    {}

    /** Return the editor for the CoordGroup at CutGroupID 'cgid'. This
        must be a valid ID number or bad things will happen :-) */
    CoordGroupEditor& edit(CutGroupID cgid)
    {
        QHash<CutGroupID,CoordGroupEditor>::iterator it = edited_groups.find(cgid);

        if (it != edited_groups.end())
            return it.value();
        else
        {
            BOOST_ASSERT( cgid < coords.count() );
            CoordGroupEditor editor = _coords[cgid];

            edited_groups.insert( cgid, editor );
            return edited_groups[editor];
        }
    }

    /** Commit all of the moves. This will update the molecule's coordinates,
        and will also trigger the recalculation of all of CoordGroup's AABoxes. */
    void commit()
    {
        for (QHash<CutGroupID,CoordGroupEditor>::const_iterator it =
                                                     edited_groups.constBegin();
             it != edited_groups.constEnd();
             ++it)
        {
            _coords[ it.key() ] = it->commit();
        }
    }

private:
    /** Reference to the molecule's coordinate groups. */
    QHash<CutGroupID, CoordGroup> &_coords;
};

/** Internal function used to translate the whole molecule by 'delta',
    as part of a larger move */
void MoleculeDataPvt::translate(const Vector &delta, MoveWorkspace &workspace) const
{
    if (delta.isZero())
        return;

    for (QHash<CutGroupID,CoordGroup>::const_iterator it = _coords.begin();
         it != _coords.end();
         ++it)
    {
        workspace.edit(it.key()).translate(delta);
    }
}

/** Translate the whole molecule by 'delta' */
void MoleculeDataPvt::translate(const Vector &delta)
{
    //create a workspace for this move
    MoveWorkspace workspace(_coords);

    //perform the move
    translate(delta, workspace);

    //commmit the move
    workspace.commit();
    incrementMinorVersion();
}

/** Internal function used to translate a load of atoms by 'delta' within
    a workspace

    \throw SireMol::missing_atom
*/
void MoleculeDataPvt::translate(const QSet<AtomIndex> &atoms, const Vector &delta,
                                MoveWorkspace &workspace) const
{
    if (delta.isZero())
        return;

    for (QSet<AtomIndex>::const_iterator it = atoms.constBegin();
         it != atoms.constEnd();
         ++it)
    {
        //get the index for this atom
        CGAtomID cgatomid = info().at(atm);

        //get an editor for the CoordGroup that contains the coordinates
        //of this atom
        CoordGroupEditor &editor = workspace.edit(cgatomid.cutGroupID());

        //move the right atom
        editor[ cgatomid.atomID() ].translate(delta);
    }
}

/** Translate the Atom with index 'index' by 'delta'

    \throw SireMol::missing_atom
*/
void MoleculeDataPvt::translate(const QSet<AtomIndex> &atoms, const Vector &delta)
{
    // Key to improving efficiency is to ensure that we only recalculate
    // metadata for the moved atoms once *all* of them have been moved.
    // To achieve this, we use CoordGroupEditors to edit the coordinates,
    // and only commit the edits once all of the atoms have been moved.
    // A benefit of this is that we maintain state - namely we either
    // commit all of the atom moves or none. This is important
    // to ensure consistency if an exception is thrown.

    MoveWorkspace workspace(_coords);

    //actually translate the atoms
    translate( atoms, delta, workspace );

    //commit the move
    workspace.commit();

    //increment the version number
    incrementMinorVersion();
}

/** Internal function used to translate all of the atoms in the residue
    represented by 'resinfo' whose names are in 'atomnames' by the
    vector 'delta' (as part of a larger move)

    \throw SireMol::missing_atom
*/
void MoleculeDataPvt::translate(const ResidueInfo &resinfo,
                                const QStringList &atomnames,
                                const Vector &delta,
                                MoveWorkspace &workspace) const
{
    if (delta.isZero())
        return;

    foreach( QString atomname, atomnames )
    {
        const CGAtomID &cgatomid = resinfo.at(atomname);

        CoordGroupEditor editor = workspace.edit(cgatomid.cutGroupID());

        editor[ cgatomid.atomID() ].translate(delta);
    }
}

/** Internal function used to translate all of the atoms in the residue
    represented by 'resinfo' whose names are in 'atomnames' by the
    vector 'delta'

    \throw SireMol::missing_atom
*/
void MoleculeDataPvt::translate(const ResidueInfo &resinfo,
                                const QStringList &atomnames,
                                const Vector &delta)
{
    //create a workspace for the move
    MoveWorkspace workspace(_coords);

    //make the move
    translate(resinfo, atomnames, delta, workspace);

    //commit the move
    workspace.commit();
    incrementMinorVersion();
}

/** Translate all of the atoms in residue 'resnum' whose names are in
    the string list 'atoms' by 'delta'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void MoleculeDataPvt::translate(ResNum resnum, const QStringList &atoms,
                                const Vector &delta)
{
    translate( info().residue(resnum), atoms, delta );
}

/** Translate all of the atoms in the residue at index 'resid' whose
    names are in the string list 'atoms' by 'delta'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void MoleculeDataPvt::translate(ResID resid, const QStringList &atoms,
                                const Vector &delta)
{
    translate( info().residue(resid), atoms, delta );
}

/** Internal function used to translate all of the atoms in the residue
    represented by 'resinfo' by the vector 'delta' */
void MoleculeDataPvt::translate(const ResidueInfo &resinfo, const Vector &delta,
                                MoveWorkspace &workspace) const
{
    if (delta.isZero())
        return;

    //get the CGAtomIDs of all of the atoms in this residue
    QVector<CGAtomID> indicies = resinfo.indicies();

    //move all of the atoms
    int nats = indicies.count();
    for (int i=0; i<nats; ++i)
    {
        const CGAtomID &cgatomid = indicies[i];
        CoordGroupEditor &editor = workspace.edit(cgatomid.cutGroupID());

        editor[ cgatomid.atomID() ].translate(delta);
    }
}

/** Internal function used to translate all of the atoms in the residue
    represented by 'resinfo' by the vector 'delta' */
void MoleculeDataPvt::translate(const ResidueInfo &resinfo, const Vector &delta)
{
    //create a workspace for the move
    MoveWorkspace workspace(_coords);

    //make the move
    translate(resinfo, delta, workspace);

    //commit the move
    workspace.commit();
    incrementMinorVersion();
}

/** Internal function used to move all of the atoms in residue 'resnum' by 'delta',
    but as part of a larger move.

    \throw SireMol::missing_residue
*/
void MoleculeDataPvt::translate(ResNum resnum, const Vector &delta,
                                MoveWorkspace &workspace) const
{
    translate( info().residue(resnum), delta, workspace );
}

/** Translate all of the atoms in residue 'resnum' by 'delta'

    \throw SireMol::missing_residue
*/
void MoleculeDataPvt::translate(ResNum resnum, const Vector &delta)
{
    translate( info().residue(resnum), delta );
}

/** Internal function used to move all of the atoms in the residue
    at index 'resid' by 'delta', but as part of a larger move.

    \throw SireError::invalid_index
*/
void MoleculeDataPvt::translate(ResID resid, const Vector &delta,
                                MoveWorkspace &workspace) const
{
    translate( info().residue(resid), delta, workspace );
}

/** Translate all of the atoms in the residue at index
    'resid' by 'delta'

    \throw SireError::invalid_index
*/
void MoleculeDataPvt::translate(ResID resnum, const Vector &delta)
{
    translate( info().residue(resid), delta );
}

#warning Got to here today!!!!
#warning Need to continue down this line!

/** Internal function used to translate an individual atom by 'delta'

    \throw SireMol::missing_atom
*/
inline void MoleculeDataPvt::translate(const AtomIndex &atm, const Vector &delta)
{
    CGAtomID id = molinfo.index(atm);

    CutGroup &cgroup = cgroups[id.cutGroupID().index()];

    Vector coords = cgroup.coordinates(id.atomID());
    coords += delta;

    cgroup.setCoordinates(id.atomID(),coords);

    //increment the version number
    incrementMinorVersion();
}

/** Translate the AtomIDGroup 'group' by 'delta'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void MoleculeDataPvt::translate(const AtomIDGroup &group, const Vector &delta)
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
    for (AtomIndexSet::const_iterator it = group.atoms().begin();
         it != group.atoms().end(); ++it)
    {
        this->translate(*it, delta);
    }
}

/** Rotate the entire molecule by the quaternion 'quat' about the point 'point' */
void MoleculeDataPvt::rotate(const Quaternion &quat, const Vector &point)
{
    if (quat.isIdentity())
        return;

    Matrix rotmat = quat.toMatrix();

    int sz = cgroups.count();
    CutGroup *cgarray = cgroups.data();

    for (int i=0; i<sz; ++i)
    {
        CutGroup &cgroup = cgarray[i];

        VectorVector coords = cgroup.coordinates();

        int nats = coords.count();
        Vector *coordsarray = coords.data();

        for (int j=0; j<nats; ++j)
        {
            Vector &coord = coordsarray[j];
            coord = SireMaths::rotate(coord,rotmat,point);
        }

        cgroup.setCoordinates(coords);
    }

    //increment the version number
    incrementMinorVersion();
}

/** Rotate the atom 'atm' by the rotation matrix 'rotmat' about the point 'point'

    \throw SireMol::missing_atom
*/
inline void MoleculeDataPvt::rotate(const AtomIndex &atm,
                                    const Matrix &rotmat, const Vector &point)
{
    CGAtomID id = molinfo.index(atm);

    CutGroup &cgroup = cgroups[id.cutGroupID().index()];

    Vector coords = cgroup.coordinates(id.atomID());

    coords = SireMaths::rotate(coords,rotmat,point);

    cgroup.setCoordinates(id.atomID(),coords);

    //increment the version number
    incrementMinorVersion();
}

/** Rotate the atoms in residue 'resnum' whose names are in 'atoms' by the
    rotation matrix 'rotmat' about the point 'point'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
inline void MoleculeDataPvt::rotate(ResNum resnum, const QStringList &atoms,
                                    const Matrix &rotmat, const Vector &point)
{
    ResidueCGInfo resinfo = molinfo.residue(resnum);

    CutGroup *cgarray = cgroups.data();

    foreach( QString atmname, atoms )
    {
        CGAtomID id = resinfo[atmname];

        CutGroup &cgroup = cgarray[id.cutGroupID().index()];

        Vector coords = cgroup.coordinates(id.atomID());
        coords = SireMaths::rotate(coords,rotmat,point);

        cgroup.setCoordinates(id.atomID(),coords);
    }

    //increment the version number
    incrementMinorVersion();
}

/** Rotate the residue 'resnum' by the rotation matrix 'rotmat' about the
    point 'point'

    \throw SireMol::missing_residue
*/
inline void MoleculeDataPvt::rotate(ResNum resnum,
                                    const Matrix &rotmat, const Vector &point)
{
    ResidueCGInfo resinfo = molinfo.residue(resnum);
    const CGAtomID *indicies = resinfo.indicies().constData();
    CutGroup *cgarray = cgroups.data();

    int nats = resinfo.nAtoms();
    for (int i=0; i<nats; ++i)
    {
        CGAtomID id = indicies[i];

        CutGroup &cgroup = cgarray[id.cutGroupID().index()];

        Vector coords = cgroup.coordinates(id.atomID());
        coords = SireMaths::rotate(coords,rotmat,point);

        cgroup.setCoordinates(id.atomID(),coords);
    }

    //increment the version number
    incrementMinorVersion();
}

/** Rotate the atoms in residue 'resnum' whose names are in 'atoms'
    by the quaternion 'quat' about the point 'point'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void MoleculeDataPvt::rotate(ResNum resnum, const QStringList &atoms,
                             const Quaternion &quat, const Vector &point)
{
    if (quat.isIdentity())
        return;

    Matrix rotmat = quat.toMatrix();
    rotate(resnum, atoms, rotmat, point);
}

/** Rotate the atoms in residue 'resnum' by the quaternion 'quat' about the point 'point'

    \throw SireMol::missing_residue
*/
void MoleculeDataPvt::rotate(ResNum resnum,
                             const Quaternion &quat, const Vector &point)
{
    if (quat.isIdentity())
        return;

    Matrix rotmat = quat.toMatrix();
    rotate(resnum, rotmat, point);
}

/** Rotate the group 'group' by the quaternion 'quat' about the point 'point'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void MoleculeDataPvt::rotate(const AtomIDGroup &group,
                             const Quaternion &quat, const Vector &point)
{
    if (quat.isIdentity())
        return;

    Matrix rotmat = quat.toMatrix();

    //translate all of the residues
    for (ResNumSet::const_iterator it = group.residues().begin();
         it != group.residues().end(); ++it)
    {
        this->rotate(*it, rotmat, point);
    }

    //now translate all of the atoms...
    for (AtomIndexSet::const_iterator it = group.atoms().begin();
         it != group.atoms().end(); ++it)
    {
        this->rotate(*it, rotmat, point);
    }
}

/** Rotate the atoms in 'atoms' by the quaternion 'quat' about the point 'point'

    \throw SireMol::missing_atom
*/
void MoleculeDataPvt::rotate(const AtomIndexSet &atoms,
                             const Quaternion &quat, const Vector &point)
{
    if (quat.isIdentity())
        return;

    Matrix rotmat = quat.toMatrix();

    for( AtomIndexSet::const_iterator it = atoms.begin();
         it != atoms.end(); ++it)
    {
        this->rotate(*it, rotmat, point);
    }
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
void MoleculeDataPvt::change(const Bond &bnd, double delta,
                const AtomIDGroup &group0, const AtomIDGroup &group1,
                const WeightFunction &weightfunc, const AtomIndexSet &anchors)
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
void MoleculeDataPvt::change(const SireMol::Angle &ang, const SireMaths::Angle &delta,
                const AtomIDGroup &group0, const AtomIDGroup &group1,
                const WeightFunction &weightfunc, const AtomIndexSet &anchors)
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
void MoleculeDataPvt::change(const Bond &dih, const SireMaths::Angle &delta,
                const AtomIDGroup &group0, const AtomIDGroup &group1,
                const WeightFunction &weightfunc, const AtomIndexSet &anchors)
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
void MoleculeDataPvt::change(const Improper &improper, const SireMaths::Angle &delta,
                const AtomIDGroup &group0, const AtomIDGroup &group1,
                const WeightFunction &weightfunc, const AtomIndexSet &anchors)
{
    throw SireError::incomplete_code("Need to write this!", CODELOC);
}

////////////
//////////// Implementation of MoleculeData
////////////

static const RegisterMetaType<MoleculeData> r_moldata("SireMol::MoleculeData", MAGIC_ONLY);

/** Serialise the molecule data to a binary datastream */
QDataStream& operator<<(QDataStream &ds, const MoleculeData &mol)
{
    writeHeader(ds, r_moldata, 1);

    SharedDataStream(ds) << mol.d;

    return ds;
}

/** Deserialise the molecule data from a binary datastream - this will detach
    this data from any other shared copies before reading in the data. */
QDataStream& operator>>(QDataStream &ds, MoleculeData &mol)
{
    VersionID v = readHeader(ds, r_moldata);

    if (v == 1)
    {
        SharedDataStream(ds) >> mol.d;
    }
    else
        throw version_error(v, "1", r_moldata, CODELOC);

    return ds;
}

/** Construct a new, empty molecule. */
MoleculeData::MoleculeData()
             : ptr( new QSharedDataPointer<MoleculeDataPvt>(new MoleculeDataPvt()) )
{}

/** Construct the MoleculeData for a molecule called 'molname', with identified residues
    in 'resids', atoms arranged into the CutGroups in 'cutgroups', and molecular bonding
    described in 'bonds'. Exceptions will be thrown if either the same atom appears in
    two different CutGroups, or if a bond is passed that refers to an atom that isn't
    in the CutGroups, or if an atom refers to a residue that is not in 'resids'.

    \throw SireMol::duplicate_atom
    \throw SireMol::missing_atom
    \throw SireMol::missing_residue
*/
MoleculeData::MoleculeData(const QString &molname, const ResidueIDSet &resids,
                           const CutGroupVector &cutgroups, const MoleculeBonds &bonds)
             : ptr( new QSharedDataPointer<MoleculeDataPvt>(
                          new MoleculeDataPvt(molname,resids,cutgroups,bonds)) )
{}

/** Shallow copy constructor. This makes a shallow copy of the molecule data. This may
    not be what you expect so beware. If you wish to get your own personal copy, then
    either use this constructor with 'deepCopy()' or use the 'detach()' function. */
MoleculeData::MoleculeData(const MoleculeData &other) : ptr(other.ptr)
{}

/** Destructor */
MoleculeData::~MoleculeData()
{}

/** Shallow copy operator */
MoleculeData& MoleculeData::operator=(const MoleculeData &other)
{
    ptr = other.ptr;
    return *this;
}

/** Private function used to easily retrieve a const reference to the
    underlying MoleculeDataPvt object */
const MoleculeDataPvt& MoleculeData::d() const
{
    return *(*ptr);
}

/** Private function used to easily retrieve a reference to the
    underlying MoleculeDataPvt object */
MoleculeDataPvt& MoleculeData::d()
{
    return *(*ptr);
}

/** Comparison operator - two molecules are equal if their molecule IDs are
    the same (even if the actual data is different) */
bool MoleculeData::operator==(const MoleculeData &other) const
{
    return d() == other.d();
}

/** Comparison operator - two molecules are different if they have different
    molecule ID numbers. */
bool MoleculeData::operator!=(const MoleculeData &other) const
{
    return d() != other.d();
}

/** Detach this MoleculeData from the shared data storage. Now any changes
    to this data will not affect any other copies */
void MoleculeData::detach()
{
    ptr.reset( new QSharedDataPointer<MoleculeDataPvt>(*ptr) );
}

/** Return a deep copy of this MoleculeData. This is an exact copy
    of this MoleculeData, but changing it will not change this copy. */
MoleculeData MoleculeData::deepCopy() const
{
    MoleculeData mol(*this);
    mol.detach();
    return mol;
}

/** Return a shallow copy of this MoleculeData - modifying the returned
    copy will also modify this original */
MoleculeData MoleculeData::shallowCopy() const
{
    return MoleculeData(*this);
}

/** Return a Molecule view of this data - this is a shallow copy. Modifying
    this molecule will change this data. */
Molecule MoleculeData::molecule()
{
    return Molecule(*this);
}

/** Return a Residue view of this data - this is a shallow copy. Modifying
    this residue will change this data.

    \throw SireMol::missing_residue
*/
Residue MoleculeData::residue(ResNum resnum)
{
    d().checkResidue(resnum);

    return Residue(*this, resnum);
}

/** Return a Residue view of the first residue called 'resname'.

    \throw SireMol::missing_residue
*/
Residue MoleculeData::residue(const QString &resname)
{
    ResNumList resnums = d().residueNumbers(resname);

    if (resnums.count() == 0)
        throw SireMol::missing_residue(QObject::tr(
            "There is no residue called \"%1\" in molecule \"%2\"")
                .arg(resname, name()), CODELOC);

    return residue(resnums[0]);
}

/** Return the set of all Residues in the molecule. The residues are
    all shallow copies on this MoleculeData. Thus changing the residues
    can change this data, and may change the other residues in the set. */
ResidueSet MoleculeData::residues()
{
    ResNumList resnums = d().residueNumbers();

    ResidueSet residus;
    foreach( ResNum resnum, resnums )
        residus.insert( Residue(*this, resnum) );

    return residus;
}

/** Return a Residue view of the ith residue in the molecule.

    \throw SireError::invalid_index
*/
Residue MoleculeData::at(int i)
{
    ResNumList resnums = d().residueNumbers();

    if (i < 0 or i >= resnums.count())
        throw SireError::invalid_index(QObject::tr(
            "Index error accessing index %1 for molecule \"%2\" (nResidues == %3)")
                .arg(i).arg(name()).arg(resnums.count()), CODELOC);

    return residue(resnums[0]);
}

/** Return the ID number of this molecule */
MoleculeID MoleculeData::ID() const
{
    return d().ID();
}

/** Give this Molecule a new ID number. This detachs the molecule from shared
    storage. */
void MoleculeData::setNewID()
{
    this->detach();
    d().setNewID();
}

/** Return the version number of this molecule */
const MoleculeVersion& MoleculeData::version() const
{
    return d().version();
}

/** Increment the major version number of this molecule - this does not
    detach the molecule from shared storage (e.g. it increases the version
    number for the molecule and any residue views) */
void MoleculeData::incrementMajorVersion()
{
    d().incrementMajorVersion();
}

/** Increment the minor version number of this molecule - this does not
    detach the molecule from shared storage (e.g. it increases the version
    number for the molecule and any residue views) */
void MoleculeData::incrementMinorVersion()
{
    d().incrementMinorVersion();
}

/** Return a clone of this Molecule. A clone is a Molecule that has the same data,
    but has a different ID number, and different ID numbers for any contained
    objects, e.g. CutGroups */
MoleculeData MoleculeData::clone() const
{
    MoleculeData mol(*this);
    mol.setNewID();

    return mol;
}

/** Return the connectivity of the molecule */
MoleculeBonds MoleculeData::connectivity() const
{
    return d().connectivity();
}

/** Return the connectivity of the residue 'resnum' */
ResidueBonds MoleculeData::connectivity(ResNum resnum) const
{
    return d().connectivity(resnum);
}

/** Return a const reference to the moleculeinfo for this molecule */
const MoleculeCGInfo& MoleculeData::info() const
{
    return d().info();
}

/** Return a signature for the molecule. This can be used to compare
    this molecule with another (or with an EditMol) to see if the
    two objects describe the same molecules (i.e. contain the same
    residues, atoms and bonds) */
MoleculeSignature MoleculeData::signature() const
{
    return d().signature();
}

/** Return a set of all of the atoms in residue 'resnum'

    \throw SireMol::missing_residue
*/
AtomSet MoleculeData::atoms(ResNum resnum) const
{
    return d().atoms(resnum);
}

/** Return a set of all atoms in the molecule */
AtomSet MoleculeData::atoms() const
{
    return d().atoms();
}

/** Return a vector of all of the atoms in residue 'resnum'

    \throw SireMol::missing_residue
*/
AtomVector MoleculeData::atomVector(ResNum resnum) const
{
    return d().atomVector(resnum);
}

/** Return a vector of all atoms in the molecule */
AtomVector MoleculeData::atomVector() const
{
    return d().atomVector();
}

/** Return the set of all CutGroups in the molecule */
CutGroupSet MoleculeData::cutGroups() const
{
    return d().cutGroups();
}

/** Return the set of all CutGroups that contain atoms that are in
    residue 'resnum'. If there are no CutGroups with atoms in this
    residue, or this residue does not exist, then an empty
    set is returned */
CutGroupSet MoleculeData::cutGroups(ResNum resnum) const
{
    return d().cutGroups(resnum);
}

/** Return the CutGroup with ID == id

    \throw SireMol::missing_cutgroup
*/
const CutGroup& MoleculeData::cutGroup(CutGroupID id) const
{
    return d().cutGroup(id);
}

/** Return the atom in CutGroup with ID == id

    \throw SireMol::missing_cutgroup
    \throw SireMol::missing_atom
*/
const Atom& MoleculeData::atom(const CGAtomID &id) const
{
    return d().atom(id);
}

/** Return the name of the molecule */
const QString& MoleculeData::name() const
{
    return d().name();
}

/** Return the name of residue 'resnum'

    \throw SireMol::missing_residue
*/
QString MoleculeData::residueName(ResNum resnum) const
{
    return d().residueName(resnum);
}

/** Return whether or not this is an empty molecule (contains no atoms or bonds) */
bool MoleculeData::isEmpty() const
{
    return d().isEmpty();
}

/** Return whether the residue 'resnum' is empty

    \throw SireMol::missing_residue
*/
bool MoleculeData::isEmpty(ResNum resnum) const
{
    return d().isEmpty(resnum);
}

/** Return the list of residue numbers in this molecule */
ResNumList MoleculeData::residueNumbers() const
{
    return d().residueNumbers();
}

/** Return the list of residue names in this molecule */
QStringList MoleculeData::residueNames() const
{
    return d().residueNames();
}

/** Return the list of residue numbers of residues called 'resname'. This
    returns an empty list if there are no residues called 'resname'. */
ResNumList MoleculeData::residueNumbers(const QString &resnam) const
{
    return d().residueNumbers(resnam);
}

/** Return the list of residues that are bonded to residue 'resnum'. This returns
    an empty list if there are no other residues bonded to residue 'resnum'

    \throw SireMol::missing_residue
*/
ResidueSet MoleculeData::residuesBondedTo(ResNum resnum)
{
    d().checkResidue(resnum);
    ResNumList resnums = connectivity().resNumsBondedTo(resnum);

    ResidueSet residus;
    foreach( ResNum rnum,  resnums )
        residus.insert( Residue(*this, rnum) );

    return residus;
}

/** Return a const reference to the atom with index 'atm'

    \throw SireMol::missing_atom
*/
const Atom& MoleculeData::atom(const AtomIndex &atm) const
{
    return d().atom(atm);
}

/** Return whether or not the molecule contains a residue with number 'resnum' */
bool MoleculeData::contains(ResNum resnum) const
{
    return d().contains(resnum);
}

/** Return whether or not the molecule contains an atom with index 'atm' */
bool MoleculeData::contains(const AtomIndex &atm) const
{
    return d().contains(atm);
}

/** Return whether or not the molecule contains the bond 'bond' */
bool MoleculeData::contains(const Bond &bond) const
{
    return d().contains(bond);
}

/** Return the ith atom of residue 'resnum'

    \throw SireMol::missing_residue
    \throw SireMol::invalid_index
*/
const Atom& MoleculeData::at(ResNum resnum, int i) const
{
    return d().at(resnum,i);
}

/** Return the total number of atoms in the molecule */
int MoleculeData::nAtoms() const
{
    return d().nAtoms();
}

/** Return the total number of atoms in residue 'resnum'

    \throw SireMol::missing_residue
*/
int MoleculeData::nAtoms(ResNum resnum) const
{
    return d().nAtoms(resnum);
}

/** Return the total number of atoms in CutGroup 'id' */
int MoleculeData::nAtoms(CutGroupID id) const
{
    return d().nAtoms(id);
}

/** Return the total number of residues in the molecule */
int MoleculeData::nResidues() const
{
    return d().nResidues();
}

/** Return the total number of CutGroups in the molecule */
int MoleculeData::nCutGroups() const
{
    return d().nCutGroups();
}

/** Return the list of atom names in residue 'resnum'

    \throw SireMol::missing_residue
*/
QStringList MoleculeData::atomNames(ResNum resnum) const
{
    return d().atomNames(resnum);
}

/** Check that the residue with number 'resnum' exists - if not, then
    throw an exception.

    \throw SireMol::missing_residue
*/
void MoleculeData::checkResidue(ResNum resnum) const
{
    d().checkResidue(resnum);
}

/** Return the geometric line representing the bond 'bnd'

    \throw SireMol::missing_atom
*/
SireMaths::Line MoleculeData::bond(const Bond &bnd) const
{
    return d().bond(bnd);
}

/** Return the geometric triangle representing the angle 'ang'

    \throw SireMol::missing_atom
*/
SireMaths::Triangle MoleculeData::angle(const SireMol::Angle &ang) const
{
    return d().angle(ang);
}

/** Return the geometric torsion representing the dihedral 'dih'

    \throw SireMol::missing_atom
*/
SireMaths::Torsion MoleculeData::dihedral(const Dihedral &dih) const
{
    return d().dihedral(dih);
}

/** Return the geometric torsion representing the improper angle 'imp'

    \throw SireMol::missing_atom
*/
SireMaths::Torsion MoleculeData::improper(const Improper &imp) const
{
    return d().improper(imp);
}

/** Return the length of the bond 'bnd'

    \throw SireMol::missing_atom
*/
double MoleculeData::measure(const Bond &bnd) const
{
    return d().measure(bnd);
}

/** Return the size of the angle 'ang'

    \throw SireMol::missing_atom
*/
SireMaths::Angle MoleculeData::measure(const SireMol::Angle &ang) const
{
    return d().measure(ang);
}

/** Return the size of the dihedral 'dih'

    \throw SireMol::missing_atom
*/
SireMaths::Angle MoleculeData::measure(const Dihedral &dih) const
{
    return d().measure(dih);
}

/** Return the size of the improper angle 'imp'

    \throw SireMol::missing_atom
*/
SireMaths::Angle MoleculeData::measure(const Improper &imp) const
{
    return d().measure(imp);
}

/** Return the relative weight of the two atom groups 'group0' and 'group1' using
    the supplied weight function

    \throw SireMol::missing_atom
    \throw SireMol::missing_residue
*/
double MoleculeData::getWeight(const AtomIDGroup &group0, const AtomIDGroup &group1,
                               const WeightFunction &weightfunc) const
{
    return d().getWeight(group0,group1,weightfunc);
}

/** Translate the whole molecule by 'delta' */
void MoleculeData::translate(const Vector &delta)
{
    d().translate(delta);
}

/** Translate the atom group 'group' by 'delta'

    \throw SireMol::missing_atom
    \throw SireMol::missing_residue
*/
void MoleculeData::translate(const AtomIDGroup &group, const Vector &delta)
{
    d().translate(group,delta);
}

/** Translate the atoms whose indexes are in 'atoms' by 'delta'

    \throw SireMol::missing_atom
*/
void MoleculeData::translate(const AtomIndexSet &atoms, const Vector &delta)
{
    d().translate(atoms,delta);
}

/** Translate atoms whose names are in 'atoms' in residue 'resnum' by 'delta'

    \throw SireMol::missing_atom
    \throw SireMol::missing_residue
*/
void MoleculeData::translate(ResNum resnum, const QStringList &atoms, const Vector &delta)
{
    d().translate(resnum,atoms,delta);
}

/** Translate the whole of the residue 'resnum' by 'delta'

    \throw SireMol::missing_residue
*/
void MoleculeData::translate(ResNum resnum, const Vector &delta)
{
    d().translate(resnum,delta);
}

/** Rotate the whole molecule by the quaternion 'quat' about the point 'point' */
void MoleculeData::rotate(const Quaternion &quat, const Vector &point)
{
    d().rotate(quat,point);
}

/** Rotate the atoms in 'group' by 'quat' about 'point'

    \throw SireMol::missing_atom
    \throw SireMol::missing_residue
*/
void MoleculeData::rotate(const AtomIDGroup &group, const Quaternion &quat, const Vector &point)
{
    d().rotate(group,quat,point);
}

/** Rotate the atoms whose indexes are in 'atoms' by 'quat' about 'point'

    \throw SireMol::missing_atom
*/
void MoleculeData::rotate(const AtomIndexSet &atoms, const Quaternion &quat, const Vector &point)
{
    d().rotate(atoms,quat,point);
}

/** Rotate the atoms whose names are in 'atoms' and that are in residue 'resnum' by
    'quat' about 'point'

    \throw SireMol::missing_atom
    \throw SireMol::missing_residue
*/
void MoleculeData::rotate(ResNum resnum, const QStringList &atoms, const Quaternion &quat,
                          const Vector &point)
{
    d().rotate(resnum,atoms,quat,point);
}

/** Rotate the whole of residue 'resnum' by 'quat' about 'point'

    \throw SireMol::missing_residue
*/
void MoleculeData::rotate(ResNum resnum, const Quaternion &quat, const Vector &point)
{
    d().rotate(resnum,quat,point);
}

/** Translate 'group0' and 'group1' by 'delta' along the vector of bond 'bnd', weighting the motion
    using the weight function 'weightfunc' and ensuring that the atoms in 'anchors' are
    anchored.

    \throw SireMol::missing_atom
    \throw SireMol::missing_residue
    \throw SireMol::anchor_error
*/
void MoleculeData::change(const Bond &bnd, double delta,
                          const AtomIDGroup &group0, const AtomIDGroup &group1,
                          const WeightFunction &weightfunc, const AtomIndexSet &anchors)
{
    d().change(bnd,delta,group0,group1,weightfunc,anchors);
}

/** Rotate 'group0' and 'group1' by 'delta' about the vector perpendicular to 'ang' and
    about the point of atom1 in the angle, with the motion weighted using 'weightfunc' and
    ensuring that the atoms in 'anchors' are anchored.

    \throw SireMol::missing_atom
    \throw SireMol::missing_residue
    \throw SireMol::anchor_error
*/
void MoleculeData::change(const SireMol::Angle &ang, const SireMaths::Angle &delta,
                          const AtomIDGroup &group0, const AtomIDGroup &group1,
                          const WeightFunction &weightfunc, const AtomIndexSet &anchors)
{
    d().change(ang,delta,group0,group1,weightfunc,anchors);
}

/** Rotate 'group0' and 'group1' by delta about the vector of the bond 'bnd' and
    the point in the centre of the vector, with the motion weighted using 'weightfunc'
    and ensuring that the atoms in 'anchors' are anchored.

    \throw SireMol::missing_atom
    \throw SireMol::missing_residue
    \throw SireMol::anchor_error
*/
void MoleculeData::change(const Bond &bnd, const SireMaths::Angle &delta,
                          const AtomIDGroup &group0, const AtomIDGroup &group1,
                          const WeightFunction &weightfunc, const AtomIndexSet &anchors)
{
    d().change(bnd,delta,group0,group1,weightfunc,anchors);
}

/** Rotate 'group0' and 'group1' about the improper 'imp' by 'delta', weighting the
    motion of the two groups by 'weightfunc' and ensuring that the atoms in
    'anchors' are anchored.

    \throw SireMol::missing_atom
    \throw SireMol::missing_residue
    \throw SireMol::anchor_error
*/
void MoleculeData::change(const Improper &imp, const SireMaths::Angle &delta,
                          const AtomIDGroup &group0, const AtomIDGroup &group1,
                          const WeightFunction &weightfunc, const AtomIndexSet &anchors)
{
    d().change(imp,delta,group0,group1,weightfunc,anchors);
}
