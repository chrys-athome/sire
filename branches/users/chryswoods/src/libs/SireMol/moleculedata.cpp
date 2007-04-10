/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2007  Christopher Woods
  *
  *  This program is free software; you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation; either version 2 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program; if not, write to the Free Software
  *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  *  For full details of the license please see the COPYING file
  *  that should have come with this distribution.
  *
  *  You can contact the authors via the developer's mailing list
  *  at http://siremol.org
  *
\*********************************************/

#include "qhash_siremol.h"

#include <QDataStream>
#include <QSharedData>

#include "moleculedata.h"
#include "residuebonds.h"
#include "cutgroup.h"
#include "molecule.h"
#include "residue.h"
#include "editmol.h"
#include "bond.h"
#include "angle.h"
#include "dihedral.h"
#include "improper.h"
#include "atomidgroup.h"
#include "weightfunction.h"
#include "moleculeinfo.h"
#include "residueinfo.h"
#include "atominfogroup.h"
#include "idmolatom.h"

#include "moleculeproperty.h"

#include "cutgroupid.h"
#include "resid.h"
#include "resnum.h"
#include "atomid.h"

#include "cgatomid.h"
#include "resnumatomid.h"
#include "residatomid.h"

#include "SireMol/errors.h"
#include "SireBase/errors.h"
#include "SireError/errors.h"

#include "SireMaths/angle.h"
#include "SireMaths/quaternion.h"
#include "SireMaths/matrix.h"
#include "SireMaths/line.h"
#include "SireMaths/triangle.h"
#include "SireMaths/torsion.h"
#include "SireMaths/rotate.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireBase;
using namespace SireMol;
using namespace SireVol;

static const RegisterMetaType<MoleculeData> r_pvt;

/** Serialise to a binary data stream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const MoleculeData &moldata)
{
    writeHeader(ds, r_pvt, 1) << moldata._id << moldata._molversion
                              << moldata._coords << moldata._molinfo
                              << moldata._molbonds << moldata._properties;
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, MoleculeData &moldata)
{
    VersionID v = readHeader(ds, r_pvt);

    if (v == 1)
    {
        ds >> moldata._id >> moldata._molversion
           >> moldata._coords >> moldata._molinfo
           >> moldata._molbonds >> moldata._properties;
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

/** Shared null MoleculeData */
static QSharedDataPointer<MoleculeData> shared_null( new MoleculeData() );

/** Return a QSharedDataPointer to the shared null MoleculeData object */
QSharedDataPointer<MoleculeData> MoleculeData::null()
{
    return shared_null;
}

/** Null constructor */
MoleculeData::MoleculeData()
                : QSharedData(),
                  _id(0), _molversion(0,0)
{}

/** Construct from some molecule data (which comes from an EditMol) */
MoleculeData::MoleculeData(const detail::MolData &moldata)
             : QSharedData(),
               _id( MoleculeData::getNewID() ), _molversion(1,0),
               _molinfo(moldata.info()), _molbonds(moldata.connectivity()),
               _coords(moldata.coordinates())
{}

/** Copy constructor */
MoleculeData::MoleculeData(const MoleculeData &other)
                : QSharedData(),
                  _id(other._id),
                  _molversion(other._molversion),
                  _molinfo(other._molinfo),
                  _molbonds(other._molbonds),
                  _coords(other._coords),
                  _properties(other._properties)
{}

/** Destructor */
MoleculeData::~MoleculeData()
{}

/** Assignment operator */
MoleculeData& MoleculeData::operator=(const MoleculeData &other)
{
    _id = other._id;
    _molversion = other._molversion;
    _molinfo = other._molinfo;
    _molbonds = other._molbonds;
    _coords = other._coords;
    _properties = other._properties;

    return *this;
}

/** Assign from some molecule data (which has come from an EditMol).
    This will increment the major version number of this molecule. */
MoleculeData& MoleculeData::operator=(const detail::MolData &moldata)
{
    this->incrementMajorVersion();

    _molinfo = moldata.info();
    _molbonds = moldata.connectivity();
    _coords = moldata.coordinates();

    return *this;
}

/** Comparison operator */
bool MoleculeData::operator==(const MoleculeData &other) const
{
    return (this == &other) or
           (_id == other._id and
            _molversion == other._molversion and
            _molinfo == other._molinfo and
            _molbonds == other._molbonds and
            _coords == other._coords and
            _properties == other._properties);
}

/** Comparison operator */
bool MoleculeData::operator!=(const MoleculeData &other) const
{
    return (this != &other) and
           (_id != other._id or
            _molversion != other._molversion or
            _molinfo != other._molinfo or
            _molbonds != other._molbonds or
            _coords != other._coords or
            _properties != other._properties);
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

/** Edit this Molecule - we do this by returning an EditMol editor */
EditMol MoleculeData::edit() const
{
    #warning Editing is broken!
    throw SireError::incomplete_code( QObject::tr(
                  "Editing a molecule is not yet supported!"), CODELOC );

    return EditMol();
}

/** Return the property called 'name'

    \throw SireBase::missing_property
*/
const Property& MoleculeData::getProperty(const QString &name) const
{
    QHash<QString,Property>::const_iterator it = _properties.find(name);

    if (it == _properties.constEnd())
        throw SireBase::missing_property( QObject::tr(
                "There is no property called \"%1\" in the molecule \"%2\" (%3:%4)")
                    .arg(name, info().name()).arg(ID()).arg(version().toString()),
                        CODELOC );

    return *it;
}

/** Set the value of the property called 'name' to 'value' - this will
    replace any existing property with that name. */
void MoleculeData::setProperty(const QString &name, const Property &value)
{
    _properties.insert( name, value );

    //this has changed the major version of the molecule
    this->incrementMajorVersion();
}

/** Add a property called 'name' with value 'value'. This will only add the
    property if there is not an already existing property with that name.

    \throw SireBase::duplicate_property
*/
void MoleculeData::addProperty(const QString &name, const Property &value)
{
    if (_properties.contains(name))
        throw SireBase::duplicate_property( QObject::tr(
              "Cannot add the property \"%1\" to the molecule \"%2\" (%3:%4) "
              "as this molecule already has a property with this name.")
                  .arg(name, info().name()).arg(ID()).arg(version().toString()),
                      CODELOC );

    _properties.insert( name, value );

    //this has changed the major version of the molecule
    this->incrementMajorVersion();
}

/** Return a hash of all of the properties associated with this molecule */
const QHash<QString,Property>& MoleculeData::properties() const
{
    return _properties;
}

/** Return the connectivity of this molecule */
const MoleculeBonds& MoleculeData::connectivity() const
{
    return _molbonds;
}

/** Return the connectivity of the residue with number 'resnum'

    \throw SireMol::missing_residue
*/
ResidueBonds MoleculeData::connectivity(ResNum resnum) const
{
    info().assertResidueExists(resnum);
    return _molbonds.residue(resnum);
}

/** Return the connectivity of the residue at index 'resid'

    \throw SireError::invalid_index
*/
ResidueBonds MoleculeData::connectivity(ResID resid) const
{
    return _molbonds.residue( info().residueNumber(resid) );
}

/** Return a const reference to the molecule info */
const MoleculeInfo& MoleculeData::info() const
{
    return _molinfo;
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
    info().assertCutGroupExists(id);
    return _coords.at(id);
}

/** Return a copy of the CutGroup with ID == cgid

    \throw SireMol::missing_cutgroup
*/
CutGroup MoleculeData::at(CutGroupID cgid) const
{
    return cutGroup(cgid);
}

/** Return a copy of the atom at index 'cgatomid'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
Atom MoleculeData::at(const CGAtomID &cgatomid) const
{
    return atom(cgatomid);
}

/** Return a copy of the atom at 'atomid'

    \throw SireMol::missing_residue
    \throw SireMol::missing_cutgroup
    \throw SireMol::missing_atom
    \throw SireMol::missing_atom
*/
Atom MoleculeData::at(const IDMolAtom &atomid) const
{
    return atom(atomid);
}

/** @name MoleculeData::coordinates(....)
    Functions that are used to return the coordinates of atoms or groups
    of atoms in the molecule.
*/
/////////////////////////////////////////////////////////
//@{

/** Internal function used to return a copy of the coordinates of
    the atom at index 'cgatomid' - this does not check whether this
    index is valid! */
Vector MoleculeData::_unsafe_coordinates(const CGAtomID &cgatomid) const
{
    return _coords.constData()[cgatomid.cutGroupID()] // QVector<Vector>
                  .constData()[cgatomid.atomID()];    // Vector
}

/** Return a copy of the coordinates of the atom at index 'cgatomid'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
Vector MoleculeData::coordinates(const CGAtomID &cgatomid) const
{
    info().assertAtomExists(cgatomid);
    return this->_unsafe_coordinates(cgatomid);
}

/** Return a copy of the coordinates of the atom at index 'atomid'

    \throw SireMol::missing_residue
    \throw SireMol::missing_cutgroup
    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
Vector MoleculeData::coordinates(const IDMolAtom &atomid) const
{
    return this->_unsafe_coordinates( atomid.index(info()) );
}

template<class T>
QHash<T,Vector> copyCoords(const MoleculeData &moldata,
                           const QSet<T> &indicies)
{
    QHash<T,Vector> coords;
    coords.reserve(indicies.count());

    for (typename QSet<T>::const_iterator it = indicies.begin();
         it != indicies.end();
         ++it)
    {
          coords.insert( *it, moldata.coordinates(*it) );
    }

    return coords;
}

/** Return the coordinates of the atoms whose indicies are in
    'cgatomids', in a hash indexed by CGAtomID

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
QHash<CGAtomID,Vector> MoleculeData::coordinates(
                                const QSet<CGAtomID> &cgatomids) const
{
    return copyCoords<CGAtomID>(*this, cgatomids);
}

/** Return the coordinates of the atoms whose indicies are in
    'resatomids', in a hash indexed by ResNumAtomID

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
QHash<ResNumAtomID,Vector> MoleculeData::coordinates(
                            const QSet<ResNumAtomID> &resatomids) const
{
    return copyCoords<ResNumAtomID>(*this, resatomids);
}

/** Return the coordinates of the atoms whose indicies are in
    'resatomids', in a hash indexed by ResNumAtomID

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
QHash<ResIDAtomID,Vector> MoleculeData::coordinates(
                            const QSet<ResIDAtomID> &resatomids) const
{
    return copyCoords<ResIDAtomID>(*this, resatomids);
}

/** Return the coordinates of the atoms whose indicies are in
    'resatomids', in a hash indexed by ResNumAtomID

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
QHash<AtomIndex,Vector> MoleculeData::coordinates(
                            const QSet<AtomIndex> &atoms) const
{
    return copyCoords<AtomIndex>(*this, atoms);
}

/** Return the coordinates of the atoms whose indicies are in
    'atomids', in a hash indexed by AtomID

    \throw SireError::invalid_index
*/
QHash<AtomID,Vector> MoleculeData::coordinates(
                            const QSet<AtomID> &atomids) const
{
    return copyCoords<AtomID>(*this, atomids);
}

template<class T>
QHash< T, QVector<Vector> > copyGroupedCoords(const MoleculeData &moldata,
                                              const QSet<T> &indicies)
{
    QHash< T, QVector<Vector> > coords;
    coords.reserve(indicies.count());

    for (typename QSet<T>::const_iterator it = indicies.begin();
         it != indicies.end();
         ++it)
    {
        coords.insert( *it, moldata.coordinates(*it) );
    }

    return coords;
}

/** Return an array of the coordinates of the atoms in the CutGroup
    with ID == cgid, in the same order as the Atoms appear in that
    CutGroup

    \throw SireMol::missing_cutgroup
*/
QVector<Vector> MoleculeData::coordinates(CutGroupID cgid) const
{
    return cutGroup(cgid).coordinates();
}

/** Return an array of the coordinates for the atoms in the CutGroups
    whose IDs are in 'cgids', in a hash indexed by CutGroupID.

    \throw SireMol::missing_cutgroup
*/
QHash< CutGroupID,QVector<Vector> > MoleculeData::coordinates(
                                        const QSet<CutGroupID> &cgids) const
{
    return copyGroupedCoords<CutGroupID>(*this, cgids);
}

/** Return an array of the coordinates of the atoms in the residue with
    number 'resnum', in the same order as the Atoms appear in that residue

    \throw SireMol::missing_residue
*/
QVector<Vector> MoleculeData::coordinates(ResNum resnum) const
{
    //get the info for the residue
    const ResidueInfo &resinfo = info().residue(resnum);

    int nats = resinfo.nAtoms();
    QVector<Vector> coords;
    coords.reserve(nats);

    for (AtomID i(0); i<nats; ++i)
        coords.append( coordinates(resinfo[i]) );

    return coords;
}

/** Return an array of the coordinates of the atoms in the residues whose
    residue numbers are in 'resnums', indexed by 'resnum'

    \throw SireMol::missing_residue
*/
QHash< ResNum,QVector<Vector> > MoleculeData::coordinates(
                                        const QSet<ResNum> &resnums) const
{
    return copyGroupedCoords<ResNum>(*this, resnums);
}

/** Return an array containing the copy of the coordinates of all of
    the atoms in this molecule, in the same order as the atoms appear
    in this molecule. */
QVector<Vector> MoleculeData::coordinates() const
{
    int nres = info().nResidues();

    QVector<Vector> allatms;
    allatms.reserve( info().nAtoms() );

    for (ResID i(0); i < nres; ++i)
        allatms += this->coordinates(i);

    return allatms;
}

/** Return an array of the coordinates of the atoms in the residue at index
    'resid', in the same order as the Atoms appear in that residue

    \throw SireError::invalid_index
*/
QVector<Vector> MoleculeData::coordinates(ResID resid) const
{
    return coordinates( info().residueNumber(resid) );
}

/** Return an array of the coordinates of the atoms in the residues with
    index numbers in 'resids', indexed by 'ResID'

    \throw SireError::invalid_index
*/
QHash< ResID,QVector<Vector> > MoleculeData::coordinates(
                                       const QSet<ResID> &resids) const
{
    return copyGroupedCoords<ResID>(*this, resids);
}

/////////////////////////////////////////////////////////
//@}

/** @name Molecule::atoms(...)
    Functions that are used to return copies of groups of atoms in the molecule.
*/
/////////////////////////////////////////////////////////
//@{

template<class T>
QHash<T,Atom> copyAtoms(const MoleculeData &moldata,
                           const QSet<T> &indicies)
{
    QHash<T,Atom> coords;
    coords.reserve(indicies.count());

    for (typename QSet<T>::const_iterator it = indicies.begin();
         it != indicies.end();
         ++it)
    {
          coords.insert( *it, moldata.atom(*it) );
    }

    return coords;
}

/** Return copies of the atoms whose indicies are in
    'cgatomids', in a hash indexed by CGAtomID

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
QHash<CGAtomID,Atom> MoleculeData::atoms(
                                const QSet<CGAtomID> &cgatomids) const
{
    return copyAtoms<CGAtomID>(*this, cgatomids);
}

/** Return copies of the atoms whose indicies are in
    'resatomids', in a hash indexed by ResNumAtomID

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
QHash<ResNumAtomID,Atom> MoleculeData::atoms(
                            const QSet<ResNumAtomID> &resatomids) const
{
    return copyAtoms<ResNumAtomID>(*this, resatomids);
}

/** Return copies of the atoms whose indicies are in
    'resatomids', in a hash indexed by ResNumAtomID

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
QHash<ResIDAtomID,Atom> MoleculeData::atoms(
                            const QSet<ResIDAtomID> &resatomids) const
{
    return copyAtoms<ResIDAtomID>(*this, resatomids);
}

/** Return copies of the atoms whose indicies are in
    'resatomids', in a hash indexed by ResNumAtomID

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
QHash<AtomIndex,Atom> MoleculeData::atoms(
                            const QSet<AtomIndex> &atoms) const
{
    return copyAtoms<AtomIndex>(*this, atoms);
}

/** Return copies of the atoms whose indicies are in 'atomids',
    in a hash indexed by AtomID

    \throw SireError::invalid_index
*/
QHash<AtomID,Atom> MoleculeData::atoms(
                            const QSet<AtomID> &atomids) const
{
    return copyAtoms<AtomID>(*this, atomids);
}

template<class T>
QHash< T, QVector<Atom> > copyGroupedAtoms(const MoleculeData &moldata,
                                              const QSet<T> &indicies)
{
    QHash< T, QVector<Atom> > coords;
    coords.reserve(indicies.count());

    for (typename QSet<T>::const_iterator it = indicies.begin();
         it != indicies.end();
         ++it)
    {
        coords.insert( *it, moldata.atoms(*it) );
    }

    return coords;
}

/** Return an array of the atoms in the CutGroup
    with ID == cgid, in the same order as the Atoms appear in that
    CutGroup

    \throw SireMol::missing_cutgroup
*/
QVector<Atom> MoleculeData::atoms(CutGroupID cgid) const
{
    return cutGroup(cgid).atoms();
}

/** Return an array of the atoms in the CutGroups
    whose IDs are in 'cgids', in a hash indexed by CutGroupID.

    \throw SireMol::missing_cutgroup
*/
QHash< CutGroupID,QVector<Atom> > MoleculeData::atoms(
                                        const QSet<CutGroupID> &cgids) const
{
    return copyGroupedAtoms<CutGroupID>(*this, cgids);
}

/** Return an array of the atoms in the residue with
    number 'resnum', in the same order as the Atoms appear in that residue

    \throw SireMol::missing_residue
*/
QVector<Atom> MoleculeData::atoms(ResNum resnum) const
{
    //get the info for the residue
    const ResidueInfo &resinfo = info().residue(resnum);

    int nats = resinfo.nAtoms();
    QVector<Atom> coords;
    coords.reserve(nats);

    for (AtomID i(0); i<nats; ++i)
        coords += atom(resinfo[i]);

    return coords;
}

/** Return an array of the atoms in the residues whose
    residue numbers are in 'resnums', indexed by 'resnum'

    \throw SireMol::missing_residue
*/
QHash< ResNum,QVector<Atom> > MoleculeData::atoms(
                                        const QSet<ResNum> &resnums) const
{
    return copyGroupedAtoms<ResNum>(*this, resnums);
}

/** Return an array containing the copy of all of
    the atoms in this molecule, in the same order as the atoms appear
    in this molecule. */
QVector<Atom> MoleculeData::atoms() const
{
    int nres = info().nResidues();

    QVector<Atom> allatms;
    allatms.reserve( info().nAtoms() );

    for (ResID i(0); i < nres; ++i)
        allatms += this->atoms(i);

    return allatms;
}

/** Return an array of the atoms in the residue at index
    'resid', in the same order as the Atoms appear in that residue

    \throw SireError::invalid_index
*/
QVector<Atom> MoleculeData::atoms(ResID resid) const
{
    return atoms( info().residueNumber(resid) );
}

/** Return an array of the atoms in the residues with
    index numbers in 'resids', indexed by 'ResID'

    \throw SireError::invalid_index
*/
QHash< ResID,QVector<Atom> > MoleculeData::atoms(
                                       const QSet<ResID> &resids) const
{
    return copyGroupedAtoms<ResID>(*this, resids);
}

/////////////////////////////////////////////////////////
//@}

/** Return an array of all of the CutGroups in the molecule, indexed by their
    CutGroupID
*/
QVector<CutGroup> MoleculeData::cutGroups() const
{
    QVector<CutGroup> cgroups;

    int ncg = info().nCutGroups();
    cgroups.reserve(ncg);

    //the CutGroups are numbered sequentially from 0 to nCutGroups()-1
    for (CutGroupID i(0); i<ncg; ++i)
    {
        cgroups.append( cutGroup(i) );
    }

    return cgroups;
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

    return cgroups;
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
QVector<CoordGroup> MoleculeData::coordGroups() const
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
        coordgroups.insert( it.key(), coordGroup(it.key()) );
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
    return coordGroups( info().residue(resnum) );
}

/** Return the CoordGroups that contain the coordinates of the atoms in the
    residue at index 'resid'. The CoordGroups are returned as a hash,
    indexed by CutGroupID. Note that the returned CoordGroups may also contain
    coordinates for atoms that are not in this residue.

    \throw SireError::invalid_index
*/
QHash<CutGroupID,CoordGroup> MoleculeData::coordGroups(ResID resid) const
{
    return coordGroups( info().residue(resid) );
}

/** @name MoleculeData::atom(....)
    Functions that are used to return copies of atoms in the molecule.
*/
/////////////////////////////////////////////////////////
//@{

/** Internal function to return a copy of the atom at index
    'cgatomid' - this does not check whether or not this is a
    valid index! */
Atom MoleculeData::_unsafe_atom(const CGAtomID &cgatomid) const
{
    //get the atom by combining the AtomInfo object contained
    //in the molecule's MoleculeInfo with the Vector object
    //holding the atom's coordinates - both of these are best
    //indexed via CGAtomID

    return Atom( _molinfo.atomGroups()        // QHash<CutGroupID,AtomInfoGroup>
                         .constFind(cgatomid.cutGroupID()).value() // QVector<AtomInfo>
                         .constData()[cgatomid.atomID()],  //AtomInfo

                 _coords.constData()[cgatomid.cutGroupID()] //QVector<Vector>
                        .constData()[cgatomid.atomID()]     //Vector
               );
}

/** Return a copy of the atom at index 'cgatomid'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
Atom MoleculeData::atom(const CGAtomID &cgatomid) const
{
    info().assertAtomExists(cgatomid);
    return this->_unsafe_atom(cgatomid);
}

/** Return a copy of the atom at index 'atomid'

    \throw SireMol::missing_residue
    \throw SireMol::missing_cutgroup
    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
Atom MoleculeData::atom(const IDMolAtom &atomid) const
{
    return this->_unsafe_atom( atomid.index(info()) );
}

/////////////////////////////////////////////////////////
//@}

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

namespace SireMol
{
namespace detail
{

/** Small private class used to provide a workspace for moving lots of parts
    of the molecule. This class ensures that meta-data is only updated at the
    end of the move (e.g. AABoxes) and that the state of the molecule is maintained
    (i.e. either the whole move is commited, or none of the move is commited)

    @author Christopher Woods
*/
class MoveWorkspace
{
public:
    MoveWorkspace( QVector<CoordGroup> &coords )
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
            BOOST_ASSERT( cgid < _coords.count() );
            CoordGroupEditor editor = _coords[cgid];

            edited_groups.insert( cgid, editor );
            return edited_groups[cgid];
        }
    }

    /** Commit all of the moves. This will update the molecule's coordinates,
        and will also trigger the recalculation of all of CoordGroup's AABoxes. */
    bool commit()
    {
        for (QHash<CutGroupID,CoordGroupEditor>::iterator it = edited_groups.begin();
             it != edited_groups.end();
             ++it)
        {
            _coords[ it.key() ] = it->commit();
        }

        return not edited_groups.isEmpty();
    }

private:
    /** Reference to the molecule's coordinate groups. */
    QVector<CoordGroup> &_coords;

    /** The set of edited CoordGroups, indexed by CutGroupID */
    QHash<CutGroupID, CoordGroupEditor> edited_groups;
};

} // end of namespace detail
} // end of namespace SireMol

using SireMol::detail::MoveWorkspace;

/** Internal function used to translate the whole molecule by 'delta',
    as part of a larger move */
void MoleculeData::translate(const Vector &delta, MoveWorkspace &workspace) const
{
    if (delta.isZero())
        return;

    int ncg = info().nCutGroups();

    for (CutGroupID i(0); i<ncg; ++i)
    {
        workspace.edit(i).translate(delta);
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

/** Translate the CutGroup with ID == 'cgid' by 'delta' as part of an existing
    move

    \throw SireMol::missing_cutgroup
*/
void MoleculeData::translate(CutGroupID cgid, const Vector &delta,
                             MoveWorkspace &workspace) const
{
    workspace.edit( cgid ).translate(delta);
}

/** Translate the CutGroup with ID == 'cgid' by 'delta'

    \throw SireMol::missing_cutgroup
*/
void MoleculeData::translate(CutGroupID cgid, const Vector &delta)
{
    //create a workspace for the move
    MoveWorkspace workspace(_coords);

    //perform the move
    translate(cgid, delta, workspace);

    //commit the move
    if (workspace.commit())
        incrementMinorVersion();
}

/** Translate a whole load of CutGroups by 'delta', as part of an existing
    move

    \throw SireMol::missing_cutgroup
*/
void MoleculeData::translate(const QSet<CutGroupID> &cgids, const Vector &delta,
                             MoveWorkspace &workspace) const
{
    for (QSet<CutGroupID>::const_iterator it = cgids.begin();
         it != cgids.end();
         ++it)
    {
        this->translate(*it, delta, workspace);
    }
}

/** Translate a whole load of CutGroups by 'delta'

    \throw SireMol::missing_cutgroup
*/
void MoleculeData::translate(const QSet<CutGroupID> &cgids, const Vector &delta)
{
    //create a workspace for the move
    MoveWorkspace workspace(_coords);

    //perform the move
    this->translate(cgids, delta, workspace);

    //commit the move
    if (workspace.commit())
        this->incrementMinorVersion();
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
        CGAtomID cgatomid = info().at(*it);

        //get an editor for the CoordGroup that contains the coordinates
        //of this atom
        CoordGroupEditor &editor = workspace.edit(cgatomid.cutGroupID());

        //move the right atom
        editor[ cgatomid.atomID() ] += delta;
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

        editor[ cgatomid.atomID() ] += delta;
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

        editor[ cgatomid.atomID() ] += delta;
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

/** Internal function used to move a whole load of residues by 'delta'
    as part of a larger move

    \throw SireMol::missing_residue
*/
void MoleculeData::translate(const QSet<ResNum> &resnums, const Vector &delta,
                             MoveWorkspace &workspace) const
{
    for (QSet<ResNum>::const_iterator it = resnums.begin();
         it != resnums.end();
         ++it)
    {
        this->translate(*it, delta, workspace);
    }
}

/** Translate all of the atoms in the residues whose numbers are in
    'resnums' by 'delta'

    \throw SireMol::missing_residue
*/
void MoleculeData::translate(const QSet<ResNum> &resnums, const Vector &delta)
{
    //create a workspace for the move
    MoveWorkspace workspace(_coords);

    //perform the move
    this->translate(resnums, delta, workspace);

    //commit the move
    if (workspace.commit())
        this->incrementMinorVersion();
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
void MoleculeData::translate(ResID resid, const Vector &delta)
{
    translate( info().residue(resid), delta );
}

/** Internal function used to move a whole load of residues by 'delta'
    as part of a larger move

    \throw SireError::invalid_index
*/
void MoleculeData::translate(const QSet<ResID> &resids, const Vector &delta,
                             MoveWorkspace &workspace) const
{
    for (QSet<ResID>::const_iterator it = resids.begin();
         it != resids.end();
         ++it)
    {
        this->translate(*it, delta, workspace);
    }
}

/** Translate all of the atoms in the residues whose indicies are
    in 'resids' by 'delta'

    \throw SireError::invalid_index
*/
void MoleculeData::translate(const QSet<ResID> &resids, const Vector &delta)
{
    //create a workspace for the move
    MoveWorkspace workspace(_coords);

    //perform the move
    this->translate(resids, delta, workspace);

    //commit the move
    if (workspace.commit())
        this->incrementMinorVersion();
}

/** Translate the atom with index 'atomid' in the residue with number 'resnum'
    by 'delta' as part of a larger move

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
void MoleculeData::translate(ResNum resnum, AtomID atomid, const Vector &delta,
                             MoveWorkspace &workspace) const
{
    //get the index for this atom
    CGAtomID cgatomid = info().at( ResNumAtomID(resnum,atomid) );

    //get an editor for the CoordGroup that contains the coordinates
    //of this atom
    CoordGroupEditor &editor = workspace.edit(cgatomid.cutGroupID());

    //move the right atom
    editor[ cgatomid.atomID() ] += delta;
}

/** Translate the atom with index 'atomid' in the residue with number 'resnum'
    by 'delta'

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
void MoleculeData::translate(ResNum resnum, AtomID atomid, const Vector &delta)
{
    //create a workspace for the move
    MoveWorkspace workspace(_coords);

    //perform the move
    this->translate(resnum, atomid, delta, workspace);

    //commit the move
    if (workspace.commit())
        this->incrementMinorVersion();
}

/** Internal function to translate a whole load of atoms whose indicies in the
    residue with number 'resnum' are in 'atomids' - translate by 'delta' as
    part of a larger move

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
void MoleculeData::translate(ResNum resnum, const QSet<AtomID> &atomids,
                             const Vector &delta, MoveWorkspace &workspace) const
{
    //get the residue info for this residue
    const ResidueInfo &resinfo = info().at(resnum);

    //go through all of the atoms...
    for (QSet<AtomID>::const_iterator it = atomids.begin();
         it != atomids.end();
         ++it)
    {
        const CGAtomID &cgatomid = resinfo.at(*it);

        //get an editor for the CutGroup that contains this atom
        CoordGroupEditor &editor = workspace.edit(cgatomid.cutGroupID());

        //translate the atom
        editor[cgatomid.atomID()] += delta;
    }
}

/** Translate all of the atoms in the residue with number 'resnum' whose
    indicies in that residue are in 'atomids', by 'delta'

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
void MoleculeData::translate(ResNum resnum, const QSet<AtomID> &atomids,
                             const Vector &delta)
{
    //create a workspace for the move
    MoveWorkspace workspace(_coords);

    //perform the move
    this->translate(resnum, atomids, delta, workspace);

    //commit the move
    if (workspace.commit())
        this->incrementMinorVersion();
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
    editor[ cgatomid.atomID() ] += delta;
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

    int ncg = info().nCutGroups();

    //loop over all CoordGroups and rotate them
    for (CutGroupID i(0); i<ncg; ++i)
    {
        workspace.edit(i).rotate(matrix, point);
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

/** Internal function used to rotate all of the atoms in the CutGroup with
    ID == cgid using the matrix 'mat' about the point 'point' as part of
    a larger move

    \throw SireMol::missing_cutgroup
*/
void MoleculeData::rotate(CutGroupID cgid, const Matrix &matrix,
                          const Vector &point, MoveWorkspace &workspace) const
{
    workspace.edit(cgid).rotate(matrix, point);
}

/** Rotate all of the atoms in the CutGroup with ID == cgid using the
    matrix 'matrix' about the point 'point'

    \throw SireMol::missing_cutgroup
*/
void MoleculeData::rotate(CutGroupID cgid, const Matrix &matrix,
                          const Vector &point)
{
    //create a workspace for the move
    MoveWorkspace workspace(_coords);

    //perform the move
    this->rotate(cgid, matrix, point, workspace);

    //commit the move
    if (workspace.commit())
        this->incrementMinorVersion();
}

/** Rotate all of the atoms in the CutGroups whose IDs are in 'cgids'
    using the matrix 'matrix' about the point 'point' as part of a larger
    move

    \throw SireMol::missing_cutgroup
*/
void MoleculeData::rotate(const QSet<CutGroupID> &cgid, const Matrix &matrix,
                          const Vector &point, MoveWorkspace &workspace) const
{
    for (QSet<CutGroupID>::const_iterator it = cgid.begin();
         it != cgid.end();
         ++it)
    {
        this->rotate(*it, matrix, point, workspace);
    }
}

/** Rotate all of the atoms in the CutGroups whose IDs are in 'cgids'
    using the matrix 'matrix' about the point 'point'

    \throw SireMol::missing_cutgroup
*/
void MoleculeData::rotate(const QSet<CutGroupID> &cgids, const Matrix &matrix,
                          const Vector &point)
{
    //create a workspace for the move
    MoveWorkspace workspace(_coords);

    //perform the move
    this->rotate(cgids, matrix, point, workspace);

    //commit the move
    if (workspace.commit())
        this->incrementMinorVersion();
}

/** Rotate all of the atoms in the CutGroup with ID == cgid using the
    quaternion 'quat' about the point 'point'

    \throw SireMol::missing_cutgroup
*/
void MoleculeData::rotate(CutGroupID cgid, const Quaternion &quat,
                          const Vector &point)
{
    if (not quat.isIdentity())
        this->rotate(cgid, quat.toMatrix(), point);
}

/** Rotate all of the atoms in the CutGroups whose IDs are in 'cgids'
    using the quaternion 'quat' about the point 'point'

    \throw SireMol::missing_cutgroup
*/
void MoleculeData::rotate(const QSet<CutGroupID> &cgids, const Quaternion &quat,
                          const Vector &point)
{
    if (not quat.isIdentity())
        this->rotate(cgids, quat.toMatrix(), point);
}

/** Internal function used to rotate the residue described by 'resinfo' by the
    matrix 'matrix' about the point 'point', as part of an existing move */
void MoleculeData::rotate(const ResidueInfo &resinfo, const Matrix &matrix,
                          const Vector &point, MoveWorkspace &workspace) const
{
    //loop over all atoms in the residue
    int nats = resinfo.nAtoms();

    for (AtomID i(0); i<nats; ++i)
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
    MoveWorkspace workspace(_coords);

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

/** Internal function used to rotate all of the atoms in the residues whose
    numbers are in 'resnums' by the matrix 'matrix' about the point 'point'
    as part of a larger move

    \throw SireMol::missing_residue
*/
void MoleculeData::rotate(const QSet<ResNum> &resnums, const Matrix &matrix,
                          const Vector &point, MoveWorkspace &workspace) const
{
    for (QSet<ResNum>::const_iterator it = resnums.begin();
         it != resnums.end();
         ++it)
    {
        this->rotate( *it, matrix, point, workspace );
    }
}

/** Rotate all of the atoms in the residues whose numbers are in 'resnums'
    by the matrix 'matrix' about the point 'point'

    \throw SireMol::missing_residue
*/
void MoleculeData::rotate(const QSet<ResNum> &resnums, const Matrix &matrix,
                          const Vector &point)
{
    //create a workspace for the move
    MoveWorkspace workspace(_coords);

    //perform the move
    this->rotate(resnums, matrix, point, workspace);

    //commit the move
    if (workspace.commit())
        this->incrementMinorVersion();
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

/** Internal function used to rotate all of the atoms in the residues whose
    indicies are in 'resids' using the matrix 'matrix' about the point
    'point' as part of a larger move.

    \throw SireError::invalid_index
*/
void MoleculeData::rotate(const QSet<ResID> &resids, const Matrix &matrix,
                          const Vector &point, MoveWorkspace &workspace) const
{
    for (QSet<ResID>::const_iterator it = resids.begin();
         it != resids.end();
         ++it)
    {
        this->rotate(*it, matrix, point, workspace);
    }
}

/** Rotate all of the atoms in the residues whose indicies are in 'resids'
    using the matrix 'matrix' about the point 'point'

    \throw SireError::invalid_index
*/
void MoleculeData::rotate(const QSet<ResID> &resids, const Matrix &matrix,
                          const Vector &point)
{
    //create a workspace for the move
    MoveWorkspace workspace(_coords);

    //perform the move
    this->rotate(resids, matrix, point, workspace);

    //commit the move
    if (workspace.commit())
        this->incrementMinorVersion();
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

    rotate( resinfo, quat.toMatrix(), point );
}

/** Internal function used to rotate the atom at index 'atomid'
    in the residue described by 'resinfo' using the matrix 'matrix'
    about the point 'point' as part of a larger move

    \throw SireError::invalid_index
*/
void MoleculeData::rotate(const ResidueInfo &resinfo, AtomID atomid,
                          const Matrix &matrix, const Vector &point,
                          MoveWorkspace &workspace) const
{
    const CGAtomID &cgatomid = resinfo.at(atomid);

    CoordGroupEditor &editor = workspace.edit(cgatomid.cutGroupID());

    editor.rotate(cgatomid.atomID(), matrix, point);
}

/** Internal function used to rotate the atoms whose indicies are in
    'atomids' in the residue described by 'resinfo' using the matrix
    'matrix' about the point 'point' as part of a larger move.

    \throw SireError::invalid_index
*/
void MoleculeData::rotate(const ResidueInfo &resinfo, const QSet<AtomID> &atomids,
                          const Matrix &matrix, const Vector &point,
                          MoveWorkspace &workspace) const
{
    for (QSet<AtomID>::const_iterator it = atomids.begin();
         it != atomids.end();
         ++it)
    {
        const CGAtomID &cgatomid = resinfo.at(*it);

        CoordGroupEditor &editor = workspace.edit(cgatomid.cutGroupID());

        editor.rotate(cgatomid.atomID(), matrix, point);
    }
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

/** Rotate the atom with index 'atomid' in the residue with number 'resnum'
    by the matrix 'matrix' about the point 'point'

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
void MoleculeData::rotate(ResNum resnum, AtomID atomid,
                          const Matrix &matrix, const Vector &point)
{
    MoveWorkspace workspace(_coords);
    this->rotate( info().residue(resnum), atomid, matrix, point, workspace );

    if (workspace.commit())
        this->incrementMinorVersion();
}

/** Rotate the atoms whose indicies are in 'resids' that are in the residue
    with number 'resnum' using the matrix 'matrix' about the point 'point'

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
void MoleculeData::rotate(ResNum resnum, const QSet<AtomID> &atomids,
                          const Matrix &matrix, const Vector &point)
{
    MoveWorkspace workspace(_coords);
    this->rotate( info().residue(resnum), atomids, matrix, point, workspace );

    if (workspace.commit())
        this->incrementMinorVersion();
}

/** Rotate the atom with index 'atomid' in the residue with number 'resnum'
    by the quaternion 'quat' about the point 'point'

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
void MoleculeData::rotate(ResNum resnum, AtomID atomid,
                          const Quaternion &quat, const Vector &point)
{
    if (not quat.isIdentity())
        this->rotate(resnum, atomid, quat.toMatrix(), point);
}

/** Rotate the atoms whose indicies are in 'resids' that are in the residue
    with number 'resnum' using the quaternion 'quat' about the point 'point'

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
void MoleculeData::rotate(ResNum resnum, const QSet<AtomID> &atomids,
                          const Quaternion &quat, const Vector &point)
{
    if (not quat.isIdentity())
        this->rotate(resnum, atomids, quat.toMatrix(), point);
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

/** Rotate all of the atoms in the residues whose numbers are in 'resnums'
    using the quaternion 'quat' about the point 'point'

    \throw SireMol::missing_residue
*/
void MoleculeData::rotate(const QSet<ResNum> &resnums, const Quaternion &quat,
                          const Vector &point)
{
    if (quat.isIdentity())
        return;

    //create a workspace for the move
    MoveWorkspace workspace(_coords);

    //perform the move
    this->rotate(resnums, quat.toMatrix(), point, workspace);

    //commit the move
    if (workspace.commit())
        this->incrementMinorVersion();
}

/** Rotate all of the atoms in the residues whose indicies are in 'resids'
    using the quaterion 'quat' about the point 'point'

    \throw SireError::invalid_index
*/
void MoleculeData::rotate(const QSet<ResID> &resids, const Quaternion &quat,
                          const Vector &point)
{
    if (quat.isIdentity())
        return;

    //create a workspace for the move
    MoveWorkspace workspace(_coords);

    //perform the move
    this->rotate(resids, quat.toMatrix(), point, workspace);

    //commit the move
    if (workspace.commit())
        this->incrementMinorVersion();
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
        const CGAtomID &cgatomid = info().at(*it);

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
void MoleculeData::rotate(const QSet<AtomIndex> &atoms, const Quaternion &quat,
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
void MoleculeData::rotate(const QSet<AtomIndex> &atoms, const Quaternion &quat,
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

/** Rotate the atom 'atm' by the quaternion 'quat' about the point 'point'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void MoleculeData::rotate(const AtomIndex &atm, const Quaternion &quat,
                          const Vector &point)
{
    if (not quat.isIdentity())
        this->rotate(atm, quat.toMatrix(), point);
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

/** @name MoleculeData::setCoordinates(....)
    Functions that are used to set the coordinates of
    the atoms in the molecule.
*/
/////////////////////////////////////////////////////////
//@{

/** Internal function used to set the coordinates of a CutGroup as part of
    a larger move

    \throw SireError::incompatible_error
    \throw SireMol::missing_cutgroup
*/
void MoleculeData::setCoordinates(CutGroupID cgid, const CoordGroup &newcoords,
                                  MoveWorkspace &workspace) const
{
    workspace.edit(cgid).setCoordinates(newcoords);
}

/** Set the coordinates of the CutGroup with ID == cgid to 'newcoords'

    \throw SireError::incompatible_error
    \throw SireMol::missing_cutgroup
*/
void MoleculeData::setCoordinates(CutGroupID cgid, const CoordGroup &newcoords)
{
    MoveWorkspace workspace(_coords);

    this->setCoordinates(cgid, newcoords, workspace);

    if (workspace.commit())
        this->incrementMinorVersion();
}

/** Set the coordinates of several CutGroups to the values contained
    in 'newcoords'

    \throw SireError::incompatible_error
    \throw SireMol::missing_cutgroup
*/
void MoleculeData::setCoordinates(const QHash<CutGroupID,CoordGroup> &newcoords)
{
    MoveWorkspace workspace(_coords);

    for (QHash<CutGroupID,CoordGroup>::const_iterator it = newcoords.begin();
         it != newcoords.end();
         ++it)
    {
        this->setCoordinates(it.key(), it.value(), workspace);
    }

    if (workspace.commit())
        this->incrementMinorVersion();
}

/** Set the coordinates of all of the CutGroups from the coordinates
    stored in the passed CoordGroups

    \throw SireError::incompatible_error
*/
void MoleculeData::setCoordinates(const QVector<CoordGroup> &newcoords)
{
    //if the coordinates are not different, then there is nothing to do!
    if (newcoords.constData() == _coords.constData())
        return;

    //check that there are the right number of CoordGroups, with the
    //right number of coordinates in each group...

    int ncg = newcoords.count();
    if (ncg != _coords.count())
        throw SireError::incompatible_error( QObject::tr(
                  "Cannot set the coordinates as the number of passed CoordGroups "
                  "(%1) does not equal the number of CutGroups (%2) in this "
                  "molecule (%3 : %4)")
                      .arg(ncg).arg(_coords.count())
                      .arg(info().name()).arg(_id), CODELOC );

    //check that there are the right number of points in each group...
    const CoordGroup *newcoords_array = newcoords.constData();
    const CoordGroup *oldcoords_array = _coords.constData();

    for (int i=0; i<ncg; ++i)
    {
        const CoordGroup &oldgroup = oldcoords_array[i];
        const CoordGroup &newgroup = newcoords_array[i];

        if (oldgroup.count() != newgroup.count())
            throw SireError::incompatible_error( QObject::tr(
                "Cannot set the coordinates as the number of points in the "
                "passed CoordGroup (index == %1, count() == %2) does not equal "
                "the number of atoms in the corresponding CutGroup "
                "(nAtoms() == %3) in this molecule (%4 : %5)")
                    .arg(i).arg(oldgroup.count()).arg(newgroup.count())
                    .arg(info().name()).arg(_id), CODELOC );
    }

    //everything is ok - copy the coordinates
    _coords = newcoords;
}

/** Internal function used to set the coordinates of the CutGroup with
    ID == 'cgid' to 'newcoords' as part of a larger move

    \throw SireError::incompatible_error
    \throw SireMol::missing_cutgroup
*/
void MoleculeData::setCoordinates(CutGroupID cgid, const QVector<Vector> &newcoords,
                                  MoveWorkspace &workspace) const
{
    workspace.edit(cgid).setCoordinates(newcoords);
}

/** Set the coordinates of the CutGroup with ID == cgid to 'newcoords'

    \throw SireError::incompatible_error
    \throw SireMol::missing_cutgroup
*/
void MoleculeData::setCoordinates(CutGroupID cgid,
                                  const QVector<Vector> &newcoords)
{
    MoveWorkspace workspace(_coords);

    this->setCoordinates(cgid, newcoords, workspace);

    if (workspace.commit())
        this->incrementMinorVersion();
}

/** Set the coordinates of lots of CutGroups to the values stored in
    'newcoords'

    \throw SireError::incompatible_error
    \throw SireMol::missing_cutgroup
*/
void MoleculeData::setCoordinates(const QHash< CutGroupID,QVector<Vector> > &newcoords)
{
    MoveWorkspace workspace(_coords);

    for (QHash< CutGroupID, QVector<Vector> >::const_iterator it = newcoords.begin();
         it != newcoords.end();
         ++it)
    {
        this->setCoordinates(it.key(), it.value(), workspace);
    }

    if (workspace.commit())
        this->incrementMinorVersion();
}

/** Internal function used to set the coordinates of the atoms in the residue
    described by 'resinfo' to 'newcoords'

    \throw SireError::incompatible_error
    \throw SireMol::missing_residue
*/
void MoleculeData::setCoordinates(const ResidueInfo &resinfo,
                                  const QVector<Vector> &newcoords,
                                  MoveWorkspace &workspace) const
{
    int nats = resinfo.nAtoms();

    resinfo.assertNAtoms(newcoords.size());

    const Vector *coordarray = newcoords.constData();

    for (AtomID i(0); i<nats; ++i)
    {
        const CGAtomID &cgatomid = resinfo.at(i);

        CoordGroupEditor &editor = workspace.edit(cgatomid.cutGroupID());

        editor[cgatomid.atomID()] = coordarray[i];
    }
}

/** Set the coordinates of the atoms in the residue with number 'resnum' to
    'newcoords'

    \throw SireError::incompatible_error
    \throw SireMol::missing_residue
*/
void MoleculeData::setCoordinates(ResNum resnum,
                                  const QVector<Vector> &newcoords)
{
    MoveWorkspace workspace(_coords);

    this->setCoordinates( info().residue(resnum), newcoords, workspace );

    if (workspace.commit())
        this->incrementMinorVersion();
}

/** Set the coordinates of the atoms in several residues to equal 'newcoords'

    \throw SireError::incompatible_error
    \throw SireMol::missing_residue
*/
void MoleculeData::setCoordinates(const QHash< ResNum,QVector<Vector> > &newcoords)
{
    MoveWorkspace workspace(_coords);

    for (QHash< ResNum,QVector<Vector> >::const_iterator it = newcoords.begin();
         it != newcoords.end();
         ++it)
    {
        this->setCoordinates( info().residue(it.key()),
                              it.value(), workspace );
    }

    if (workspace.commit())
        this->incrementMinorVersion();
}

/** Set the coordinates of all of the atoms in the residue at index 'resid'
    to 'newcoords'

    \throw SireError::incompatible_error
    \throw SireError::invalid_index
*/
void MoleculeData::setCoordinates(ResID resid,
                                  const QVector<Vector> &newcoords)
{
    MoveWorkspace workspace(_coords);

    this->setCoordinates( info().residue(resid), newcoords, workspace );

    if (workspace.commit())
        this->incrementMinorVersion();
}

/** Set the coordinates of lots of residues to the values in 'newcoords'

    \throw SireError::incompatible_error
    \throw SireError::invalid_index
*/
void MoleculeData::setCoordinates(const QHash< ResID,QVector<Vector> > &newcoords)
{
    MoveWorkspace workspace(_coords);

    for (QHash< ResID,QVector<Vector> >::const_iterator it = newcoords.begin();
         it != newcoords.end();
         ++it)
    {
        this->setCoordinates( info().residue(it.key()),
                              it.value(), workspace );
    }

    if (workspace.commit())
        this->incrementMinorVersion();
}

/** Internal function to set the coordinates of the atom with AtomIndex
    'atom' to 'newcoords'
*/
void MoleculeData::setCoordinates(const CGAtomID &cgatomid, const Vector &newcoords,
                                  MoveWorkspace &workspace) const
{
    CoordGroupEditor &editor = workspace.edit(cgatomid.cutGroupID());

    editor[cgatomid.atomID()] = newcoords;
}

/** Set the coordinates of all atoms in the Molecule to 'newcoords'

    \throw SireError::incompatible_error
*/
void MoleculeData::setCoordinates(const QVector<Vector> &newcoords)
{
    info().assertNAtoms(newcoords.size());

    MoveWorkspace workspace(_coords);

    int nats = newcoords.size();
    const Vector *coordarray = newcoords.constData();

    const MoleculeInfo &molinfo = info();

    for (AtomID i(0); i<nats; ++i)
    {
        this->setCoordinates(molinfo.at(i), coordarray[i], workspace);
    }

    if (workspace.commit())
        this->incrementMinorVersion();
}

/** Set the coordinates of the atom with AtomIndex 'atom' to 'newcoords'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void MoleculeData::setCoordinates(const AtomIndex &atom, const Vector &newcoords)
{
    MoveWorkspace workspace(_coords);

    this->setCoordinates( info().at(atom), newcoords, workspace );

    if (workspace.commit())
        this->incrementMinorVersion();
}

/** Set the coordinates of a large number of atoms to the values stored
    in 'newcoords'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void MoleculeData::setCoordinates(const QHash<AtomIndex,Vector> &newcoords)
{
    MoveWorkspace workspace(_coords);

    for (QHash<AtomIndex,Vector>::const_iterator it = newcoords.begin();
         it != newcoords.end();
         ++it)
    {
        this->setCoordinates( info().at(it.key()),
                              it.value(), workspace );
    }

    if (workspace.commit())
        this->incrementMinorVersion();
}

/** Set the coordinates of the atom with index 'cgatomid' to 'newcoords'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
void MoleculeData::setCoordinates(const CGAtomID &cgatomid,
                                  const Vector &newcoords)
{
    info().assertAtomExists(cgatomid);

    MoveWorkspace workspace(_coords);

    this->setCoordinates(cgatomid, newcoords, workspace);

    if (workspace.commit())
        this->incrementMinorVersion();
}

/** Set the coordinates of lots of atoms to the values in 'newcoords'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
void MoleculeData::setCoordinates(const QHash<CGAtomID,Vector> &newcoords)
{
    MoveWorkspace workspace(_coords);

    for (QHash<CGAtomID,Vector>::const_iterator it = newcoords.begin();
         it != newcoords.end();
         ++it)
    {
        info().assertAtomExists(it.key());

        this->setCoordinates(it.key(), it.value(), workspace);
    }

    if (workspace.commit())
        this->incrementMinorVersion();
}

/** Set the coordinates of the atom at index 'resatomid' to 'newcoords'

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
void MoleculeData::setCoordinates(const ResNumAtomID &resatomid,
                                  const Vector &newcoords)
{
    MoveWorkspace workspace(_coords);

    this->setCoordinates( info().at(resatomid), newcoords, workspace );

    if (workspace.commit())
        this->incrementMinorVersion();
}

/** Set the coordinates of lots of atoms to the values in 'newcoords'

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
void MoleculeData::setCoordinates(const QHash<ResNumAtomID,Vector> &newcoords)
{
    MoveWorkspace workspace(_coords);

    for (QHash<ResNumAtomID,Vector>::const_iterator it = newcoords.begin();
         it != newcoords.end();
         ++it)
    {
        this->setCoordinates( info().at(it.key()), it.value(), workspace );
    }

    if (workspace.commit())
        this->incrementMinorVersion();
}

/** Set the coordinates of the atom at index 'resatomid' to 'newcoords'

    \throw SireError::invalid_index
*/
void MoleculeData::setCoordinates(const ResIDAtomID &resatomid,
                                  const Vector &newcoords)
{
    MoveWorkspace workspace(_coords);

    this->setCoordinates( info().at(resatomid), newcoords, workspace );

    if (workspace.commit())
        this->incrementMinorVersion();
}

/** Set the coordinates of lots of atoms to 'newcoords'

    \throw SireError::invalid_index
*/
void MoleculeData::setCoordinates(const QHash<ResIDAtomID,Vector> &newcoords)
{
    MoveWorkspace workspace(_coords);

    for (QHash<ResIDAtomID,Vector>::const_iterator it = newcoords.begin();
         it != newcoords.end();
         ++it)
    {
        this->setCoordinates( info().at(it.key()),
                              it.value(), workspace );
    }

    if (workspace.commit())
        this->incrementMinorVersion();
}

/** Set the coordinates of the atom with index 'atomid' in the residue with
    number 'resnum' to 'newcoords'

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
void MoleculeData::setCoordinates(ResNum resnum, AtomID atomid,
                                  const Vector &newcoords)
{
    this->setCoordinates( ResNumAtomID(resnum,atomid), newcoords );
}

/** Set the coordinates of lots of atoms in the residue with number 'resnum'
    to the values in 'newcoords'

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
void MoleculeData::setCoordinates(ResNum resnum,
                                  const QHash<AtomID,Vector> &newcoords)
{
    const ResidueInfo &resinfo = info().at(resnum);

    MoveWorkspace workspace(_coords);

    for (QHash<AtomID,Vector>::const_iterator it = newcoords.begin();
         it != newcoords.end();
         ++it)
    {
        this->setCoordinates( resinfo.at(it.key()),
                              it.value(), workspace );
    }

    if (workspace.commit())
        this->incrementMinorVersion();
}

/** Set the coordinates of lots of atoms in the residue with number 'resnum'
    to the values stored in 'newcoords'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void MoleculeData::setCoordinates(ResNum resnum,
                                  const QHash<QString,Vector> &newcoords)
{
    const ResidueInfo &resinfo = info().at(resnum);

    MoveWorkspace workspace(_coords);

    for (QHash<QString,Vector>::const_iterator it = newcoords.begin();
         it != newcoords.end();
         ++it)
    {
        this->setCoordinates( resinfo.at(it.key()),
                              it.value(), workspace );
    }

    if (workspace.commit())
        this->incrementMinorVersion();
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
void MoleculeData::change(const Bond &bnd, double delta,
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
                const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors)
{
    throw SireError::incomplete_code("Need to write this!", CODELOC);
}
