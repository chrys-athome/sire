/**
  * @file
  *
  * C++ Implementation: MoleculeData
  *
  * Description: Implementation of MoleculeData
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
using namespace SireMol;

static const RegisterMetaType<MoleculeData> r_pvt("SireMol::MoleculeData",
                                                  MAGIC_ONLY);

/** Serialise to a binary data stream */
QDataStream &operator<<(QDataStream &ds, const MoleculeData &moldata)
{
    writeHeader(ds, r_pvt, 1) << moldata.id() << moldata.molversion()
                              << moldata.coords() << moldata.molinfo()
                              << moldata.molbonds();
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream &operator>>(QDataStream &ds, MoleculeData &moldata)
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
QMutex MoleculeData::idmutex;

/** The last allocated ID number */
MoleculeID MoleculeData::lastid(0);

/** Get a new, unique, ID number */
MoleculeID MoleculeData::getNewID()
{
    QMutexLocker lkr(&idmutex);
    ++lastid;
    return lastid;
}

/** Null constructor */
MoleculeData::MoleculeData()
                : QSharedData(),
                  _id(0), _molversion(0,0)
{}

/** Construct the molecule data from an EditMol using the conversion function
    'converter'
*/
MoleculeData::MoleculeData(const EditMol &editmol,
                                 const ConvertFunction &converter)
                : QSharedData(),
                  _id(0), _molversion(0,0)
{
    //ok - lets build everything.
    this->buildMolecule(editmol, converter);

    //ok - we're done. Just have to give the molecule a new ID number :-)
    this->setNewID();
}

/** Copy constructor */
MoleculeData::MoleculeData(const MoleculeData &other)
                : QSharedData(),
                  _id(other._id),
                  _molversion(other._molversion),
                  _molinfo(other._molinfo),
                  _molbonds(other._molbonds),
                  _coords(other._coords)
{}

/** Destructor */
MoleculeData::~MoleculeData()
{}

/** Comparison operator */
bool MoleculeData::operator==(const MoleculeData &other) const
{
    return _id == other._id and
           _molversion == other._molversion and
           _molinfo == other._molinfo and
           _molbonds == other._molbonds and
           _coords == other._coords;
}

/** Comparison operator */
bool MoleculeData::operator!=(const MoleculeData &other) const
{
    return _id != other._id or
           _molversion != other._molversion or
           _molinfo != other._molinfo or
           _molbonds != other._molbonds or
           _coords != other._coords;
}

/** Build this molecule from the passed EditMol using the supplied
    ConvertFunction. This will increment the major version number
    of the molecule. */
void MoleculeData::buildMolecule(const EditMol &editmol,
                                    const ConvertFunction &convertfunction)
{
#warning Need to write MoleculeData::buildMolecule()!

    //increment the major version number of the molecule
    incrementMajorVersion();
}

/** Return the name of this molecule */
const QString& MoleculeData::name() const
{
    return info().name();
}

/** Return the number of atoms in this molecule */
int MoleculeData::nAtoms() const
{
    return info().nAtoms();
}

/** Return the number of atoms in residue 'resnum'

    \throw SireMol::missing_residue
*/
int MoleculeData::nAtoms(ResNum resnum) const
{
    return info().residue(resnum).nAtoms();
}

/** Return the number of residues in the molecule. */
int MoleculeData::nResidues() const
{
    return info().nResidues();
}

/** Return the number of bonds in the molecule */
int MoleculeData::nBonds() const
{
    return connectivity().nBonds();
}

/** Check that the residue 'resnum' exists - else throw an exception */
void MoleculeData::checkResidue(ResNum resnum) const
{
    if (not info().contains(resnum))
        throw SireMol::missing_residue(QObject::tr(
            "Molecule \"%1\" does not contain a residue with number \"%2\"")
                .arg(name(),resnum.toString()), CODELOC);
}

/** Check that the residue with index 'resid' exists - else throw
    an exception */
void MoleculeData::checkResidue(ResID resid) const
{
    if (not info().contains(resid))
        throw SireError::invalid_index(QObject::tr(
            "Molecule \"%1\" has no residue with index \"%2\" (nResidues() == %3)")
                .arg(name()).arg(resid).arg(nResidues()), CODELOC);
}

/** Return the ID number of the molecule */
MoleculeID MoleculeData::ID() const
{
    return _id;
}

/** Give this molecule a new ID number. This will also give a new
    ID number to all contained (identified) objects, e.g. CutGroups */
void MoleculeData::setNewID()
{
    //get a new ID number
    _id = getNewID();

    //now reset the version number to 1.0
    _molversion = MoleculeVersion(1,0);
}

/** Return the version number of the molecule */
const MoleculeVersion& MoleculeData::version() const
{
    return _molversion;
}

/** Increment the major version of the molecule */
void MoleculeData::incrementMajorVersion()
{
    _molversion.incrementMajor();
}

/** Increment the minor version number of the molecule */
void MoleculeData::incrementMinorVersion()
{
    _molversion.incrementMinor();
}

/** Return the connectivity of this molecule */
MoleculeBonds MoleculeData::connectivity() const
{
    return _molbonds;
}

/** Return the connectivity of residue 'resnum'

    \throw SireMol::missing_residue
*/
ResidueBonds MoleculeData::connectivity(ResNum resnum) const
{
    checkResidue(resnum);
    return connectivity().residue(resnum);
}

/** Return a const reference to the molecule info */
const MoleculeInfo& MoleculeData::info() const
{
    return _info;
}

/** Return the number of CutGroups in the molecule */
int MoleculeData::nCutGroups() const
{
    return _coords.count();
}

/** Check the CutGroupID is valid */
void MoleculeData::checkCutGroup(CutGroupID id) const
{
    if ( id >= nCutGroups() )
        throw SireMol::missing_cutgroup(QObject::tr(
            "There is no CutGroup with ID == %1 in Molecule \"%2\" (nCutGroups() == %3)")
                .arg(id),arg(name()).arg(nCutGroups()), CODELOC);
}

/** Return the CutGroup with CutGroupID == id

    \throw SireMol::missing_cutgroup
*/
CutGroup MoleculeData::cutGroup(CutGroupID id) const
{
    return CutGroup( info().atomGroup(id), coordinates(id) );
}

/** Return the coordinates for the CutGroup with CutGroupID == id

    \throw SireMol::missing_cutgroup
*/
CoordGroup MoleculeData::coordGroup(CutGroupID id) const
{
    checkCutGroup(id);
    return _coords.at(id);
}

/** Return the Atom with CGAtomID 'cgatomid'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
Atom MoleculeData::atom(const CGAtomID &cgatomid) const
{
    return Atom( info().atom(cgatomid), coordinates(cgatomid) );
}

/** Return the Atom at index 'atmid' in the CutGroup with ID 'cgid'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
Atom MoleculeData::atom(CutGroupID cgid, AtomID atmid) const
{
    return atom( CGAtomID(cgid,atmid) );
}

/** Return the coordinates of the atom with CGAtomID 'cgatomid'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
Vector MoleculeData::coordinates(const CGAtomID &cgatomid) const
{
    return coordinates(cgatomid.cutGroupID()).at(cgatomid.atomID());
}

/** Return the coordinates of the Atom at index 'atmid' in the CutGroup
    with ID 'cgid'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
Vector MoleculeData::coordinates(CutGroupID cgid, AtomID atmid) const
{
    return coordinates(cgid).at(atmid);
}

/** Return an array of all of the atoms in residue 'resnum', in the same
    order as they were added to the residue.

    \throw SireMol::missing_residue
*/
QVector<Atom> MoleculeData::atoms(ResNum resnum) const
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
QVector<Atom> MoleculeData::atoms() const
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
QHash<CutGroupID,CutGroup> MoleculeData::cutGroups() const
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
QHash<CutGroupID,CutGroup>
MoleculeData::cutGroups(const ResidueInfo &resinfo) const
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
QHash<CutGroupID,CutGroup>
MoleculeData::cutGroups(ResNum resnum) const
{
    return cutGroups( info().residue(resnum) );
}

/** Return a hash of all CutGroups in the residue with index 'resid', indexed
    by CutGroupID

    \throw SireError::invalid_index
*/
QHash<CutGroupID,CutGroup> MoleculeData::cutGroups(ResID resid) const
{
    return cutGroups( info().residue(resid) );
}

/** Return a hash of all of the coordinates of atoms in this molecule,
    organised by CoordGroup, and indexed by CutGroupID */
QHash<CutGroupID,CoordGroup> MoleculeData::coordGroups() const
{
    return _coords;
}

/** Internal function used to return a hash of the CoordGroups containing the
    coordinates of the residue described by 'resinfo' */
QHash<CutGroupID,CoordGroup> MoleculeData::coordGroups(const ResidueInfo &resinfo) const
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
QHash<CutGroupID,CoordGroup> MoleculeData::coordGroups(ResNum resnum) const
{
    return coordinates( info().residue(resnum) );
}

/** Return the CoordGroups that contain the coordinates of the atoms in the
    residue at index 'resid'. The CoordGroups are returned as a hash,
    indexed by CutGroupID. Note that the returned CoordGroups may also contain
    coordinates for atoms that are not in this residue.

    \throw SireError::invalid_index
*/
QHash<CutGroupID,CoordGroup> MoleculeData::coordGroups(ResID resid) const
{
    return coordinates( info().residue(resid) );
}

/** Return the name of residue 'resnum'

    \throw SireMol::missing_residue
*/
QString MoleculeData::residueName(ResNum resnum) const
{
    return info().residue(resnum).name();
}

/** Return the name of the residue at index 'resid'

    \throw SireError::invalid_index
*/
QString MoleculeData::residueName(ResID resid) const
{
    return info().residue(resid).name();
}

/** Return whether or not this is an empty molecule (no residues or atoms) */
bool MoleculeData::isEmpty() const
{
    return nResidues() == 0;
}

/** Return whether residue 'resnum' is empty (no atoms)

    \throw SireMol::missing_residue
*/
bool MoleculeData::isEmpty(ResNum resnum) const
{
    return nAtoms(resnum) == 0;
}

/** Return whether or not the residue at index 'resid' is empty

    \throw SireError::invalid_index
*/
bool MoleculeData::isEmpty(ResID resid) const
{
    return nAtoms(resid) == 0;
}

/** Return the list of residue numbers in this molecule */
QVector<ResNum> MoleculeData::residueNumbers() const
{
    return info().residueNumbers();
}

/** Return the list of residue names in this molecule */
QStringList MoleculeData::residueNames() const
{
    return info().residueNames();
}

/** Return the list of residue numbers of residues called 'resnam'.
    Returns an empty list if there are no residues called 'resnam' */
QVector<ResNum> MoleculeData::residueNumbers(const QString &resnam) const
{
    return info().residueNumbers(resnam);
}

/** Return the atom with index 'atm'

    \throw SireMol::missing_atom
*/
Atom MoleculeData::atom(const AtomIndex &atm) const
{
    return atom( info().at(atm) );
}

/** Return the atom at index 'atmid' in the residue with number 'resnum'

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
Atom MoleculeData::atom(ResNum resnum, AtomID atmid) const
{
    return atom( info().at(ResNumAtomID(resnum,atmid)) );
}

/** Return the atom at index 'atmid' in the residue at index 'resid'

    \throw SireError::invalid_index
*/
Atom MoleculeData::atom(ResID resid, AtomID atmid) const
{
    return atom( info().at(ResIDAtomID(resid,atmid)) );
}

/** Return the coordinates of the atom with ID 'atm'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
Vector MoleculeData::coordinates(const AtomIndex &atm) const
{
    return coordinates( info().at(atm) );
}

/** Return the coordinates of the atom at index 'atmid' in the residue
    with number 'resnum'

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
Vector MoleculeData::coordinates(ResNum resnum, AtomID atomid) const
{
    return coordinates( info().at(ResNumAtomID(resnum,atomid)) );
}

/** Return the coordinates of the atom at index 'atmid' in the
    residue at index 'resid'

    \throw SireError::invalid_index
*/
Vector MoleculeData::coordinates(ResID resid, AtomID atomid) const
{
    return coordinates( info().at(ResIDAtomID(resid,atomid)) );
}

/** Return whether this molecule contains a residue with number 'resnum' */
bool MoleculeData::contains(ResNum resnum) const
{
    return info().contains(resnum);
}

/** Return whether this molecule contains an atom with index 'atm' */
bool MoleculeData::contains(const AtomIndex &atm) const
{
    return info().contains(atm);
}

/** Return whether this molecule contains the bond 'bnd' */
bool MoleculeData::contains(const Bond &bond) const
{
    return connectivity().contains(bond);
}

/** Return the number of atoms in the CutGroup with ID = id

    \throw SireMol::missing_cutgroup
*/
int MoleculeData::nAtoms(CutGroupID id) const
{
    return info().nAtoms( id );
}

/** Return the name of all of the atoms in residue 'resnum' */
QStringList MoleculeData::atomNames(ResNum resnum) const
{
    return info().residue(resnum).atomNames();
}

/** Return the geometric line representing the bond 'bnd'

    \throw SireMol::missing_atom
*/
SireMaths::Line MoleculeData::bond(const Bond &bnd) const
{
    return SireMaths::Line( coordinates(bnd.atom0()),
                            coordinates(bnd.atom1()) );
}

/** Return the geometric triangle representing the angle 'ang'

    \throw SireMol::missing_atom
*/
SireMaths::Triangle MoleculeData::angle(const SireMol::Angle &ang) const
{
    return SireMaths::Triangle( coordinates(ang.atom0()),
                                coordinates(ang.atom1()),
                                coordinates(ang.atom2()) );
}

/** Return the geometric torsion representing the dihedral 'dih'

    \throw SireMol::missing_atom
*/
SireMaths::Torsion MoleculeData::dihedral(const Dihedral &dih) const
{
    return SireMaths::Torsion( coordinates(dih.atom0()),
                               coordinates(dih.atom1()),
                               coordinates(dih.atom2()),
                               coordinates(dih.atom3()) );
}

/** Return the geometric line representing the improper angle 'imp'

    \throw SireMol::missing_atom
*/
SireMaths::Torsion MoleculeData::improper(const Improper &imp) const
{
    return SireMaths::Torsion( coordinates(imp.atom0()),
                               coordinates(imp.atom1()),
                               coordinates(imp.atom2()),
                               coordinates(imp.atom3()) );
}

/** Return the length of the bond 'bnd'

    \throw SireMol::missing_atom
*/
double MoleculeData::measure(const Bond &bnd) const
{
    return bond(bnd).length();
}

/** Return the size of the angle 'ang'

    \throw SireMol::missing_atom
*/
SireMaths::Angle MoleculeData::measure(const SireMol::Angle &ang) const
{
    return angle(ang).angle1();
}

/** Return the size of the dihedral 'dih'

    \throw SireMol::missing_atom
*/
SireMaths::Angle MoleculeData::measure(const Dihedral &dih) const
{
    return dihedral(dih).angle();
}

/** Return the size of the improper angle 'imp'

    \throw SireMol::missing_atom
*/
SireMaths::Angle MoleculeData::measure(const Improper &improper) const
{
    throw SireError::incomplete_code("Need to write this!", CODELOC);

    return 0.0;
}

/** Return the relative weights of 'group0' and 'group1' using the weight function
    'weightfunc'.

    \throw SireMol::missing_atom
    \throw SireMol::missing_residue
*/
double MoleculeData::getWeight(const AtomIDGroup &group0, const AtomIDGroup &group1,
                                  const WeightFunction &weightfunc) const
{
    WeightCalculator calc(weightfunc);

#warning MoleculeData::getWeight() needs refactoring as it is inefficient!

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
    bool commit()
    {
        for (QHash<CutGroupID,CoordGroupEditor>::const_iterator it =
                                                     edited_groups.constBegin();
             it != edited_groups.constEnd();
             ++it)
        {
            _coords[ it.key() ] = it->commit();
        }

        return not edited_groups.isEmpty();
    }

private:
    /** Reference to the molecule's coordinate groups. */
    QHash<CutGroupID, CoordGroup> &_coords;
};

/** Internal function used to translate the whole molecule by 'delta',
    as part of a larger move */
void MoleculeData::translate(const Vector &delta, MoveWorkspace &workspace) const
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
void MoleculeData::translate(const Vector &delta)
{
    //create a workspace for this move
    MoveWorkspace workspace(_coords);

    //perform the move
    translate(delta, workspace);

    //commmit the move
    if (workspace.commit())
        incrementMinorVersion();
}

/** Internal function used to translate a load of atoms by 'delta' within
    a workspace

    \throw SireMol::missing_atom
*/
void MoleculeData::translate(const QSet<AtomIndex> &atoms, const Vector &delta,
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
void MoleculeData::translate(const QSet<AtomIndex> &atoms, const Vector &delta)
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
    if (workspace.commit())
        incrementMinorVersion();
}

/** Internal function used to translate all of the atoms in the residue
    represented by 'resinfo' whose names are in 'atomnames' by the
    vector 'delta' (as part of a larger move)

    \throw SireMol::missing_atom
*/
void MoleculeData::translate(const ResidueInfo &resinfo,
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
void MoleculeData::translate(const ResidueInfo &resinfo,
                                const QStringList &atomnames,
                                const Vector &delta)
{
    //create a workspace for the move
    MoveWorkspace workspace(_coords);

    //make the move
    translate(resinfo, atomnames, delta, workspace);

    //commit the move
    if (workspace.commit())
        incrementMinorVersion();
}

/** Translate all of the atoms in residue 'resnum' whose names are in
    the string list 'atoms' by 'delta'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void MoleculeData::translate(ResNum resnum, const QStringList &atoms,
                                const Vector &delta)
{
    translate( info().residue(resnum), atoms, delta );
}

/** Translate all of the atoms in the residue at index 'resid' whose
    names are in the string list 'atoms' by 'delta'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void MoleculeData::translate(ResID resid, const QStringList &atoms,
                                const Vector &delta)
{
    translate( info().residue(resid), atoms, delta );
}

/** Internal function used to translate all of the atoms in the residue
    represented by 'resinfo' by the vector 'delta' */
void MoleculeData::translate(const ResidueInfo &resinfo, const Vector &delta,
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
void MoleculeData::translate(const ResidueInfo &resinfo, const Vector &delta)
{
    //create a workspace for the move
    MoveWorkspace workspace(_coords);

    //make the move
    translate(resinfo, delta, workspace);

    //commit the move
    if (workspace.commit())
        incrementMinorVersion();
}

/** Internal function used to move all of the atoms in residue 'resnum' by 'delta',
    but as part of a larger move.

    \throw SireMol::missing_residue
*/
void MoleculeData::translate(ResNum resnum, const Vector &delta,
                                MoveWorkspace &workspace) const
{
    translate( info().residue(resnum), delta, workspace );
}

/** Translate all of the atoms in residue 'resnum' by 'delta'

    \throw SireMol::missing_residue
*/
void MoleculeData::translate(ResNum resnum, const Vector &delta)
{
    translate( info().residue(resnum), delta );
}

/** Internal function used to move all of the atoms in the residue
    at index 'resid' by 'delta', but as part of a larger move.

    \throw SireError::invalid_index
*/
void MoleculeData::translate(ResID resid, const Vector &delta,
                                MoveWorkspace &workspace) const
{
    translate( info().residue(resid), delta, workspace );
}

/** Translate all of the atoms in the residue at index
    'resid' by 'delta'

    \throw SireError::invalid_index
*/
void MoleculeData::translate(ResID resnum, const Vector &delta)
{
    translate( info().residue(resid), delta );
}

/** Internal function used to translate a single atom by 'delta' as part
    of a larger move

    \throw SireMol::missing_atom
*/
void MoleculeData::translate(const AtomIndex &atm, const Vector &delta,
                                MoveWorkspace &workspace) const
{
    //find the index of this atom in the molecule
    const CGAtomID &cgatomid = info().at(atm);

    //get an editor for the CoordGroup that holds this atom
    CoordGroupEditor &editor = workspace.edit(cgatomid.cutGroupID());

    //move the atom
    editor[ cgatomid.atomID() ].translate( delta );
}

/** Internal function used to translate an individual atom by 'delta'

    \throw SireMol::missing_atom
*/
void MoleculeData::translate(const AtomIndex &atm, const Vector &delta)
{
    //create a workspace for this move
    MoveWorkspace workspace(_coords);

    //perform the move
    translate(atm, delta, workspace);

    //commit the move
    if (workspace.commit())
        incrementMinorVersion();
}

/** Internal function used to translate an AtomIDGroup 'group' by 'delta'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void MoleculeData::translate(const AtomIDGroup &group, const Vector &delta,
                                MoveWorkspace &workspace) const
{
    if (delta.isZero())
        return;

    //move all of the residues
    for (QSet<ResNum>::const_iterator it = group.residues().begin();
         it != group.residues().end();
         ++it)
    {
        translate( *it, delta, workspace );
    }

    //move all of the atoms
    translate( group.atoms(), delta, workspace );
}

/** Translate the AtomIDGroup 'group' by 'delta'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void MoleculeData::translate(const AtomIDGroup &group, const Vector &delta)
{
    //create a workspace for the move
    MoveWorkspace workspace(_coords);

    //perform the move
    translate( group, delta, workspace );

    //commit the move
    if (workspace.commit())
        incrementMinorVersion();
}

/** Internal function used to rotate the entire molecule by the matrix 'matrix'
    (which may not actually be a rotation matrix!) as part of an existing move */
void MoleculeData::rotate(const Matrix &matrix, const Vector &point,
                             MoveWorkspace &workspace) const
{
    if (matrix.isIdentity())
        return;

    //loop over all CoordGroups and rotate them
    for (QHash<CutGroupID,CoordGroup>::const_iterator it = _coords.begin();
         it != coords.end();
         ++it)
    {
        //get an editor for this CoordGroup
        CoordGroupEditor &editor = workspace.edit(it.key());

        //rotate this group
        editor.rotate( matrix, point );
    }
}

/** Rotate the entire molecule by the matrix 'matrix' (which may not actually be a
    rotation matrix) about the point 'point'. */
void MoleculeData::rotate(const Matrix &matrix, const Vector &point)
{
    //create a workspace for the move
    MoveWorkspace workspace(_coords);

    //perform the move
    rotate( matrix, point, workspace );

    //commit the move
    if (workspace.commit())
        incrementMinorVersion();
}

/** Internal function used to rotate the entire molecule by the quaternion 'quat'
    about the point 'point', as part of a larger move */
void MoleculeData::rotate(const Quaternion &quat, const Vector &point,
                             MoveWorkspace &workspace) const
{
    if (quat.isIdentity())
        return;

    rotate( quat.toMatrix(), point, workspace );
}

/** Rotate the entire molecule by the quaternion 'quat' about the point 'point' */
void MoleculeData::rotate(const Quaternion &quat, const Vector &point)
{
    if (quat.isIdentity())
        return;

    rotate( quat.toMatrix(), point );
}

/** Internal function used to rotate the residue described by 'resinfo' by the
    matrix 'matrix' about the point 'point', as part of an existing move */
void MoleculeData::rotate(const ResidueInfo &resinfo, const Matrix &matrix,
                             const Vector &point, MoveWorkspace &workspace) const
{
    //loop over all atoms in the residue
    int nats = resinfo.nAtoms();

    for (int i=0; i<nats; ++i)
    {
        //get the index for this atom
        const CGAtomID cgatomid = resinfo.at(i);

        //get an editor for the CoordGroup that contains this atom
        CoordGroupEditor &editor = workspace.edit( cgatomid.cutGroupID() );

        //rotate the atom
        editor.rotate(cgatomid.atomID(), matrix, point);
    }
}

/** Internal function to rotate all of the atoms in the residue represented by 'resinfo'
    by the matrix 'matrix' about the point 'point'. */
void MoleculeData::rotate(const ResidueInfo &resinfo, const Matrix &matrix,
                             const Vector &point)
{
    //create the workspace
    MoveWorkspace &workspace(_coords);

    //make the move
    rotate( resinfo, matrix, point, workspace );

    //commit the move
    if (workspace.commit())
        incrementMinorVersion();
}

/** Internal function to rotate the residue with number 'resnum' using the matrix
    'matrix' about the point 'point' as part of an existing move

    \throw SireMol::missing_residue
*/
void MoleculeData::rotate(ResNum resnum, const Matrix &matrix,
                             const Vector &point, MoveWorkspace &workspace) const
{
    rotate( info().residue(resnum), matrix, point, workspace );
}

/** Rotate the residue with number 'resnum' using the matrix 'matrix' about
    the point 'point'

    \throw SireMol::missing_residue
*/
void MoleculeData::rotate(ResNum resnum, const Matrix &matrix,
                             const Vector &point)
{
    rotate( info().residue(resnum), matrix, point );
}

/** Internal function used to rotate the residue at index 'resid' using the matrix
    'matrix' about the point 'point' as part of an existing move

    \throw SireError::invalid_index
*/
void MoleculeData::rotate(ResID resid, const Matrix &matrix,
                             const Vector &point, MoveWorkspace &workspace) const
{
    rotate( info().residue(resid), matrix, point, workspace );
}

/** Rotate the residue at index 'resid' using the matrix 'matrix' about the point
    'point' as part of an existing move.

    \throw SireError::invalid_index
*/
void MoleculeData::rotate(ResID resid, const Matrix &matrix,
                             const Vector &point)
{
    rotate( info().residue(resid), matrix, point );
}

/** Internal function used to rotate a residue represented by 'resinfo' about
    the quaternion 'quat' about the point 'point' as part of an existing move */
void MoleculeData::rotate(const ResidueInfo &resinfo, const Quaternion &quat,
                             const Vector &point, MoveWorkspace &workspace) const
{
    if (quat.isIdentity())
        return;

    rotate( resinfo, quat.toMatrix(), point, workspace );
}

/** Internal function used to rotate a residue represented by 'resinfo' about
    the quaternion 'quat' about the point 'point' */
void MoleculeData::rotate(const ResidueInfo &resinfo, const Quaternion &quat,
                             const Vector &point)
{
    if (quat.isIdentity())
        return;

    rotate( resinfo, quat.toMatrix(), point, workspace );
}

/** Internal function used to rotate the residue with number 'resnum' by the
    quaternion 'quat' about the point 'point' as part of an existing move

    \throw SireMol::missing_residue
*/
void MoleculeData::rotate(ResNum resnum, const Quaternion &quat,
                             const Vector &point, MoveWorkspace &workspace) const
{
    rotate( info().residue(resnum), quat, point, workspace );
}

/** Rotate the residue with number 'resnum' by the quaternion 'quat' about the
    point 'point'

    \throw SireMol::missing_residue
*/
void MoleculeData::rotate(ResNum resnum, const Quaternion &quat,
                             const Vector &point)
{
    rotate( info().residue(resnum), quat, point );
}

/** Internal function used to rotate the residue at index 'resid' by the
    quaternion 'quat' about the point 'point' as part of an existing move

    \throw SireError::invalid_index
*/
void MoleculeData::rotate(ResID resid, const Quaternion &quat,
                             const Vector &point, MoveWorkspace &workspace) const
{
    rotate( info().residue(resid), quat, point, workspace );
}

/** Rotate the residue at index 'resid' by the quaternion 'quat' about the
    point 'point'

    \throw SireError::invalid_index
*/
void MoleculeData::rotate(ResID resid, const Quaternion &quat,
                             const Vector &point)
{
    rotate( info().residue(resid), quat, point );
}

/** Internal function used to rotate the named atoms in the residue described
    by 'resinfo' using the matrix 'matrix' about the point 'point' as part
    of a larger move.

    \throw SireMol::missing_atom
*/
void MoleculeData::rotate(const ResidueInfo &resinfo, const QStringList &atomnames,
                             const Matrix &matrix, const Vector &point,
                             MoveWorkspace &workspace) const
{
    //loop over all of the atoms
    foreach( QString atomname, atomnames )
    {
        //get the index of this atom
        const CGAtomID &cgatomid = resinfo.at(atomname);

        //get an editor for the CoordGroup that contains the coordinates
        //of this atom
        CoordGroupEditor &editor = workspace.edit( cgatomid.cutGroupID() );

        //rotate the atom
        editor.rotate( cgatomid.atomID(), matrix, point );
    }
}

/** Internal function used to rotate the atoms named in 'atomnames' that are
    in the residue described by 'resinfo' by the matrix 'matrix' about the point
    'point'.

    \throw SireMol::missing_atom
*/
void MoleculeData::rotate(const ResidueInfo &resinfo, const QStringList &atomnames,
                             const Matrix &matrix, const Vector &point)
{
    if (matrix.isIdentity())
        return;

    //create the workspace
    MoveWorkspace workspace(_coords);

    //make the move
    rotate(resinfo, atomnames, matrix, point, workspace);

    //commit the move
    if (workspace.commit())
        incrementMinorVersion();
}

/** Internal function to rotate the named atoms in the residue described by
    'resinfo' by the quaternion 'quat' about the point 'point' as part of a larger
    move

    \throw SireMol::missing_atom
*/
void MoleculeData::rotate(const ResidueInfo &resinfo, const QStringList &atomnames,
                             const Quaternion &quat, const Vector &point,
                             MoveWorkspace &workspace) const
{
    rotate( resinfo, atomnames, quat.toMatrix(), point, workspace );
}

/** Internal function to rotate the named atoms in the residue described by
    resinfo by the quaternion 'quat' about the point 'point'

    \throw SireMol::missing_atom
*/
void MoleculeData::rotate(const ResidueInfo &resinfo, const QStringList &atomnames,
                             const Quaternion &quat, const Vector &point)
{
    if (quat.isIdentity())
        return;

    //create the workspace
    MoveWorkspace workspace(_coords);

    //perform the move
    rotate( resinfo, atomnames, quat.toMatrix(), point, workspace );

    //commit the move
    if (workspace.commit())
        incrementMinorVersion();
}

/** Internal function used to rotate the atoms named in 'atomnames' in the residue
    with number 'resnum' about the matrix 'matrix' about the point 'point'
    as part of a larger move

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void MoleculeData::rotate(ResNum resnum, const QStringList &atomnames,
                             const Matrix &matrix, const Vector &point,
                             MoveWorkspace &workspace) const
{
    rotate( info().residue(resnum), atomnames, matrix, point, workspace );
}

/** Rotate the atoms named in 'atomnames' in the residue with number 'resnum'
    by the matrix 'matrix' about the point 'point'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void MoleculeData::rotate(ResNum resnum, const QStringList &atomnames,
                             const Matrix &matrix, const Vector &point)
{
    rotate( info().residue(resnum), atomnames, matrix, point );
}

/** Internal function used to rotate the atoms named in 'atomnames' in the
    residue with number 'resnum' by the quaternion 'quat' about the point
    'point' as part of a larger move

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void MoleculeData::rotate(ResNum resnum, const QStringList &atomnames,
                             const Quaternion &quat, const Vector &point,
                             MoveWorkspace &workspace) const
{
    rotate( info().residue(resnum), atomnames, quat, point, workspace );
}

/** Rotate the atoms named in 'atomnames' in the residue with number 'resnum'
    by the quaternion 'quat' about the point 'point'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void MoleculeData::rotate(ResNum resnum, const QStringList &atomnames,
                             const Quaternion &quat, const Vector &point)
{
    rotate( info().residue(resnum), atomnames, quat, point );
}

/** Internal function used to rotate the atoms named in 'atomnames' in the residue
    with index 'resid' about the matrix 'matrix' about the point 'point'
    as part of a larger move

    \throw SireError::invalid_index
    \throw SireMol::missing_atom
*/
void MoleculeData::rotate(ResID resid, const QStringList &atomnames,
                             const Matrix &matrix, const Vector &point,
                             MoveWorkspace &workspace) const
{
    rotate( info().residue(resid), atomnames, matrix, point, workspace );
}

/** Rotate the atoms named in 'atomnames' in the residue with index 'resid'
    by the matrix 'matrix' about the point 'point'

    \throw SireError::invalid_index
    \throw SireMol::missing_atom
*/
void MoleculeData::rotate(ResID resid, const QStringList &atomnames,
                             const Matrix &matrix, const Vector &point)
{
    rotate( info().residue(resid), atomnames, matrix, point );
}

/** Internal function used to rotate the atoms named in 'atomnames' in the
    residue with index 'resid' by the quaternion 'quat' about the point
    'point' as part of a larger move

    \throw SireError::invalid_index
    \throw SireMol::missing_atom
*/
void MoleculeData::rotate(ResID resid, const QStringList &atomnames,
                             const Quaternion &quat, const Vector &point,
                             MoveWorkspace &workspace) const
{
    rotate( info().residue(resid), atomnames, quat, point, workspace );
}

/** Rotate the atoms named in 'atomnames' in the residue with index 'resid'
    by the quaternion 'quat' about the point 'point'

    \throw SireError::invalid_index
    \throw SireMol::missing_atom
*/
void MoleculeData::rotate(ResID resid, const QStringList &atomnames,
                             const Quaternion &quat, const Vector &point)
{
    rotate( info().residue(resid), atomnames, quat, point );
}

/** Internal function used to rotate the set of atoms in 'atoms' by the rotation
    matrix 'rotmat' about the point 'point' as part of a large move.

    \throw SireMol::missing_atom
*/
void MoleculeData::rotate(const QSet<AtomIndex> &atoms, const Matrix &matrix,
                             const Vector &point, MoveWorkspace &workspace) const
{
    //loop over all of the atoms
    for (QSet<AtomIndex>::const_iterator it = atoms.begin();
         it != atoms.end();
         ++it)
    {
        //get the index of this atom
        CGAtomID &cgatomid = info().at(*it);

        //get an editor for the CoordGroup that contains this atom
        CoordGroupEditor &editor = workspace.edit( cgatomid.cutGroupID() );

        //rotate this atom
        editor.rotate( cgatomid.atomID(), matrix, point );
    }
}

/** Rotate the set of atoms in 'atoms' by the rotation matrix 'rotmat' about
    the point 'point'

    \throw SireMol::missing_atom
*/
void MoleculeData::rotate(const QSet<AtomIndex> &atoms, const Matrix &matrix,
                             const Vector &point)
{
    if (matrix.isIdentity())
        return;

    //create the workspace
    MoveWorkspace workspace(_coords);

    //make the move
    rotate( atoms, matrix, point, workspace );

    //commit the move
    if (workspace.commit())
        incrementMinorVersion();
}

/** Internal function used to rotate the set of atoms in 'atoms' by the quaternion
    'quat' about the point 'point' as part of a larger move

    \throw SireMol::missing_atom
*/
void MoleculeData::rotate(const QSet<Atom> &atoms, const Quaternion &quat,
                             const Vector &point, MoveWorkspace &workspace) const
{
    if (quat.isIdentity())
        return;

    rotate( atoms, quat.toMatrix(), point, workspace );
}

/** Rotate the set of atoms in 'atoms' by the quaternion 'quat' about the
    point 'point'

    \throw SireMol::missing_atom
*/
void MoleculeData::rotate(const QSet<Atom> &atoms, const Quaternion &quat,
                             const Vector &point)
{
    //create the workspace
    MoveWorkspace workspace(_coords);

    //make the move
    rotate( atoms, quat, point, workspace );

    //commit the move
    if (workspace.commit())
        incrementMinorVersion();
}

/** Internal function used to rotate the atom 'atom' by the matrix 'matrix'
    about the point 'point' as part of a larger move

    \throw SireMol::missing_atom
*/
void MoleculeData::rotate(const AtomIndex &atm, const Matrix &matrix,
                             const Vector &point, MoveWorkspace &workspace) const
{
    //get the index for this atom
    const CGAtomID &cgatomid = info().at(atm);

    //get an editor for the CoordGroup that contains this atom
    CoordGroupEditor &editor = workspace.edit( cgatomid.cutGroupID() );

    //move this atom
    editor.rotate( cgatomid.atomID(), matrix, point );
}

/** Rotate the atom 'atm' by the matrix 'matrix' about the point 'point'

    \throw SireMol::missing_atom
*/
void MoleculeData::rotate(const AtomIndex &atm, const Matrix &matrix,
                             const Vector &point)
{
    //create a workspace for the move
    MoveWorkspace workspace(_coords);

    //make the move
    rotate( atm, matrix, point, workspace );

    //commit the move
    if (workspace.commit())
        incrementMinorVersion();
}

/** Internal function used to rotate the atoms described in the
    AtomIDGroup 'group' using the matrix 'matrix' about the point 'point'
    as part of a larger move.

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void MoleculeData::rotate(const AtomIDGroup &group, const Matrix &matrix,
                             const Vector &point, MoveWorkspace &workspace) const
{
    //loop over all of the residues
    for (QSet<ResNum>::const_iterator it = group.residues().begin();
         it != group.residues().end();
         ++it)
    {
        rotate( *it, matrix, point, workspace );
    }

    //rotate all of the atoms
    rotate( group.atoms(), matrix, point, workspace );
}

/** Rotate all of the atoms in the AtomIDGroup 'group' using the matrix
    'matrix' about the point 'point'.

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void MoleculeData::rotate(const AtomIDGroup &group, const Matrix &matrix,
                             const Vector &point)
{
    if (matrix.isIdentity())
        return;

    //create a workspace for the move
    MoveWorkspace workspace(_coords);

    //perform the move
    rotate(group, matrix, point, workspace);

    //commit the move
    if (workspace.commit())
        incrementMinorVersion();
}

/** Internal function used to rotate the atoms in the AtomIDGroup 'group'
    using the quaternion 'quat' about the point 'point' as part of a
    larger move.

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void MoleculeData::rotate(const AtomIDGroup &group, const Quaternion &quat,
                             const Vector &point, MoveWorkspace &workspace) const
{
    if (quat.isIdentity())
        return;

    rotate( group, quat.toMatrix(), point, workspace );
}

/** Rotate the atoms in the AtomIDGroup 'group' using the quaternion 'quat'
    about the point 'point'.

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void MoleculeData::rotate(const AtomIDGroup &group, const Quaternion &quat,
                             const Vector &point)
{
    //create a workspace for this move
    MoveWorkspace workspace(_coords);

    //perform the move
    rotate( group, quat, point, workspace );

    //commit the move
    if (workspace.commit())
        incrementMinorVersion();
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
void MoleculeData::change(const Bond &bnd, double delta,
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

    //create a workspace for the move
    MoveWorkspace workspace(_coords);

    //translate group0 by (weight-1) * delta * bondvec
    this->translate(group0, (weight-1.0)*delta*bondvec, workspace);

    //translate group1 by weight * delta * bondvec
    this->translate(group1, weight*delta*bondvec, workspace);

    //commit the move
    if (workspace.commit())
        incrementMinorVersion();
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
void MoleculeData::change(const SireMol::Angle &ang, const SireMaths::Angle &delta,
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

    //create a workspace for the move
    MoveWorkspace workspace(_coords);

    //rotate group0 by (weight-1)*delta around angvec, about the central atom
    //of the angle
    this->rotate( group0, Quaternion((weight-1.0)*delta,angvec),
                  trig[1], workspace );

    //rotate group1 by weight*delta around angvec, about the central atom of the angle
    this->rotate( group1, Quaternion(weight*delta,angvec),
                  trig[1], workspace );

    //commit the move
    if (workspace.commit())
        incrementMinorVersion();
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
void MoleculeData::change(const Bond &dih, const SireMaths::Angle &delta,
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

    //create a workspace for the move
    MoveWorkspace workspace(_coords);

    //rotate group0 by (weight-1)*delta around dihvec, about the first atom of the bond
    this->rotate( group0, Quaternion((weight-1.0)*delta,dihvec),
                  line[0], workspace );

    //rotate group1 by weight*delta around dihvec, about the second atom of the bond
    this->rotate( group1, Quaternion(weight*delta,dihvec),
                  line[1], workspace );

    //commit the move
    if (workspace.commit())
        incrementMinorVersion();
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
void MoleculeData::change(const Improper &improper, const SireMaths::Angle &delta,
                const AtomIDGroup &group0, const AtomIDGroup &group1,
                const WeightFunction &weightfunc, const AtomIndexSet &anchors)
{
    throw SireError::incomplete_code("Need to write this!", CODELOC);
}
