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

#include "SireMol/qhash_siremol.h"

#include "molecule.h"
#include "moleculedata.h"
#include "newatom.h"
#include "residue.h"
#include "cutgroup.h"
#include "editmol.h"

#include "moleculeproperty.h"

#include "moleculeinfo.h"
#include "moleculebonds.h"
#include "residuebonds.h"

#include "cgatomid.h"
#include "resnumatomid.h"
#include "residatomid.h"

#include "atomidgroup.h"

#include "bond.h"
#include "angle.h"
#include "dihedral.h"

#include "splitmolecule.h"
#include "weightfunction.h"

#include "SireMaths/line.h"
#include "SireMaths/triangle.h"
#include "SireMaths/torsion.h"

#include "SireError/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireStream;
using namespace SireBase;
using namespace SireMol;

static const RegisterMetaType<Molecule> r_molecule;

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const Molecule &mol)
{
    writeHeader(ds, r_molecule, 1);

    SharedDataStream(ds) << mol.d;

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, Molecule &mol)
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

uint SIREMOL_EXPORT qHash(const Molecule &molecule)
{
    return molecule.ID();
}

/** @name Constructors and Destructor */
/////////////////////////////////////////////////////////
//@{

/** Create an empty molecule */
Molecule::Molecule() : d( MoleculeData::null() )
{}

/** Construct a Molecule that is a copy of the molecule that contains the
    residue 'residue' */
Molecule::Molecule(const Residue &residue) : d( residue.d )
{}

/** Construct a Molecule that is a copy of the molecule that contains the
    atom 'atom' */
Molecule::Molecule(const NewAtom &atom) : d( atom.d )
{}

/** Copy constructor - this is fast as this class is implicitly shared */
Molecule::Molecule(const Molecule &other) : d( other.d )
{}

/** Destructor */
Molecule::~Molecule()
{}

/** @name ID Number and Version
    Dealing with the ID number and version
*/
/////////////////////////////////////////////////////////
//@{

/** Return the ID number of this molecule */
MoleculeID Molecule::ID() const
{
    return d->ID();
}

/** Give this molecule a new (unique) ID number.
    This will reset the molecule's version. */
void Molecule::setNewID()
{
    d->setNewID();
}

/** Return the version number of this molecule. */
const MoleculeVersion& Molecule::version() const
{
    return d->version();
}

/** Return an ID string for this molecule */
QString Molecule::idString() const
{
    return QObject::tr("\"%1\" : %2 : %3")
                .arg(this->name()).arg(this->ID())
                .arg(this->version().toString());
}

/////////////////////////////////////////////////////////
//@}

/////////////////////////////////////////////////////////
//@}

/** @name Operators
    Miscellaneous operators
*/
/////////////////////////////////////////////////////////
//@{

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

/** Assign from the MolData which is the result from a commit of an EditMol */
Molecule& Molecule::operator=(const detail::MolData &moldata)
{
    *d = moldata;
    return *this;
}

/////////////////////////////////////////////////////////
//@}

/** Return an EditMol representation of this molecule. */
EditMol Molecule::edit() const
{
    return d->edit();
}

/** Return the property called 'name'

    \throw SireMol::missing_property
*/
const Property& Molecule::getProperty(const QString &name) const
{
    return d->getProperty(name);
}

/** Set the value of the property called 'name' to 'value' - this will
    replace any existing property with that name. */
void Molecule::setProperty(const QString &name, const Property &value)
{
    //if this is a molecule property then this property
    //must be compatible with this molecule
    if (value.isA<MoleculeProperty>())
        value.asA<MoleculeProperty>().assertCompatibleWith(*this);

    d->setProperty(name, value);
}

/** Add a property called 'name' with value 'value'. This will only add the
    property if there is not an already existing property with that name.

    \throw SireMol::duplicate_property
*/
void Molecule::addProperty(const QString &name, const Property &value)
{
    //if this is a molecule property then this property
    //must be compatible with this molecule
    if (value.isA<MoleculeProperty>())
        value.asA<MoleculeProperty>().assertCompatibleWith(*this);

    d->addProperty(name, value);
}

/** Set the property called 'name' to the value 'value'. This will overwrite
    any existing property with this name. */
void Molecule::setProperty(const QString &name, const PropertyBase &value)
{
    this->setProperty( name, Property(value) );
}

/** Add a property called 'name' with value 'value'. This will only add the
    property if there is not an already existing property with that name.

    \throw SireMol::duplicate_property
*/
void Molecule::addProperty(const QString &name, const PropertyBase &value)
{
    this->addProperty( name, Property(value) );
}

/** Set the property called 'name' to the value 'value'. This will overwrite
    any existing property with this name. */
void Molecule::setProperty(const QString &name, const QVariant &value)
{
    this->setProperty( name, Property( new VariantProperty(value) ) );
}

/** Add a property called 'name' with value 'value'. This will only add the
    property if there is not an already existing property with that name.

    \throw SireMol::duplicate_property
*/
void Molecule::addProperty(const QString &name, const QVariant &value)
{
    this->addProperty( name, Property( new VariantProperty(value) ) );
}

/** Return a hash of all of the properties associated with this molecule */
const QHash<QString,Property>& Molecule::properties() const
{
    return d->properties();
}


/** @name Molecule::operator[...]
    Indexing operators used to return copies of the objects in the molecule
    at the passed index. See also Molecule::at(...)
*/
/////////////////////////////////////////////////////////
//@{

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
    return Residue(*this, resid);
}

/** Return a copy of the residue with number 'resnum'

    \throw SireMol::missing_residue
*/
Residue Molecule::operator[](ResNum resnum) const
{
    return Residue(*this, resnum);
}

/** Return a copy of the atom at index 'cgatomid'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
Atom Molecule::operator[](const CGAtomID &cgatomid) const
{
    return d->at(cgatomid);
}

/** Return a copy of the atom at 'atomid'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
Atom Molecule::operator[](const IDMolAtom &atomid) const
{
    return d->at(atomid);
}

/////////////////////////////////////////////////////////
//@}

/////////////////////////////////////////////////////////

/** @name Molecule::residue(...), Molecule::residues()
    Functions used to return copies of the residues in this molecule.
*/
/////////////////////////////////////////////////////////
//@{

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
    const QVector<ResNum> &resnums = residueNumbers();

    int nres = resnums.count();

    QHash<ResNum,Residue> residus;
    residus.reserve(nres);

    for (int i=0; i<nres; ++i)
    {
        residus.insert( resnums[i], Residue(*this, resnums[i]) );
    }

    return residus;
}

/** Return copies of the residues whose residues numbers are in 'resnums'

    \throw SireMol::missing_residue
*/
QHash<ResNum,Residue> Molecule::residues(const QSet<ResNum> &resnums) const
{
    QHash<ResNum,Residue> residus;
    residus.reserve(resnums.count());

    for (QSet<ResNum>::const_iterator it = resnums.begin();
         it != resnums.end();
         ++it)
    {
        residus.insert( *it, this->residue(*it) );
    }

    return residus;
}

/////////////////////////////////////////////////////////
//@}

/** @name Molecule::at(...)
    Indexing type operators - return a copy of the object at the passed index.
*/
/////////////////////////////////////////////////////////
//@{

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

/** Return a copy of the CutGroup with ID == cgid

    \throw SireMol::missing_cutgroup
*/
CutGroup Molecule::at(CutGroupID cgid) const
{
    return d->at(cgid);
}

/** Return a copy of the atom at index 'cgatomid'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
Atom Molecule::at(const CGAtomID &cgatomid) const
{
    return d->at(cgatomid);
}

/** Return a copy of the atom at 'atomid'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
Atom Molecule::at(const IDMolAtom &atomid) const
{
    return d->at(atomid);
}

/////////////////////////////////////////////////////////
//@}

/** @name Molecule::connectivity(...)
    Functions that are used to return the connectivity of the molecule
    or of parts of the molecule.
*/
/////////////////////////////////////////////////////////
//@{

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

/////////////////////////////////////////////////////////
//@}

/** Return the MoleculeInfo that holds the metainfo for this
    molecule */
const MoleculeInfo& Molecule::info() const
{
    return d->info();
}

/** @name Molecule::atoms(...)
    Functions that are used to return copies of groups of atoms in the molecule.
*/
/////////////////////////////////////////////////////////
//@{

/** Return an array holding a copy of all of the atoms in this molecule */
QVector<Atom> Molecule::atoms() const
{
    return d->atoms();
}

/** Return an array holding a copy of all of the atoms in the residue
    with number 'resnum'

    \throw SireMol::missing_residue
*/
QVector<Atom> Molecule::atoms(ResNum resnum) const
{
    return d->atoms(resnum);
}

/** Return an array holding a copy of all of the atoms in the residue
    at index 'resid' in this molecule.

    \throw SireError::invalid_index
*/
QVector<Atom> Molecule::atoms(ResID resid) const
{
    return d->atoms(resid);
}

/** Return an array holding a copy of all of the atoms in the CutGroup
    with ID == cgid

    \throw SireMol::missing_cutgroup
*/
QVector<Atom> Molecule::atoms(CutGroupID cgid) const
{
    return d->atoms(cgid);
}

/** Return arrays containing copies of all of the atoms in the residues whose indicies
    are in 'resids'

    \throw SireError::invalid_index
*/
QHash< ResID,QVector<Atom> > Molecule::atoms(const QSet<ResID> &resids) const
{
    return d->atoms(resids);
}

/** Return arrays containing copies of all of the atoms in the residues whose numbers
    are in 'resnums'

    \throw SireMol::missing_residue
*/
QHash< ResNum,QVector<Atom> > Molecule::atoms(const QSet<ResNum> &resnums) const
{
    return d->atoms(resnums);
}

/** Return arrays containing copies of all of the atoms in the CutGroups
    whose IDs are in 'cgids'

    \throw SireMol::missing_cutgroup
*/
QHash< CutGroupID,QVector<Atom> > Molecule::atoms(const QSet<CutGroupID> &cgids) const
{
    return d->atoms(cgids);
}

/** Return copies of all of the atoms whose AtomIndex objects are in 'atms'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
QHash<AtomIndex,Atom> Molecule::atoms(const QSet<AtomIndex> &atms) const
{
    return d->atoms(atms);
}

/** Return copies of all of the atoms whose indicies are in 'resatomids'

    \throw SireError::invalid_index
*/
QHash<ResIDAtomID,Atom> Molecule::atoms(const QSet<ResIDAtomID> &resatomids) const
{
    return d->atoms(resatomids);
}

/** Return copies of all of the atoms whose indicies are in 'resatomids'

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
QHash<ResNumAtomID,Atom> Molecule::atoms(const QSet<ResNumAtomID> &resatomids) const
{
    return d->atoms(resatomids);
}

/** Return copies of all of the atoms whose indicies are in 'cgatomids'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
QHash<CGAtomID,Atom> Molecule::atoms(const QSet<CGAtomID> &cgatomids) const
{
    return d->atoms(cgatomids);
}

/** Return copies of all of the atoms whose indicies are in 'atomids'

    \throw SireError::invalid_index
*/
QHash<AtomID,Atom> Molecule::atoms(const QSet<AtomID> &atomids) const
{
    return d->atoms(atomids);
}

/////////////////////////////////////////////////////////
//@}

/** @name Molecule::cutGroups(...)
    Functions that are used to return copies of the CutGroups in the molecule.
*/
/////////////////////////////////////////////////////////
//@{

/** Return a copy of all of the CutGroups in the molecule, indexed
    by CutGroupID */
QVector<CutGroup> Molecule::cutGroups() const
{
    return d->cutGroups();
}

/** Return a copy of all of the CutGroups that contain atoms from
    the residue with number 'resnum', indexed by CutGroupID

    \throw SireMol::missing_residue
*/
QHash<CutGroupID,CutGroup> Molecule::cutGroups(ResNum resnum) const
{
    return d->cutGroups(resnum);
}

/** Return a copy of all of the CutGroups that contain atoms from
    the residue at index 'resid'

    \throw SireError::invalid_index
*/
QHash<CutGroupID,CutGroup> Molecule::cutGroups(ResID resid) const
{
    return d->cutGroups(resid);
}

/** Return a copy of the CutGroup with ID == id

    \throw SireMol::missing_cutgroup
*/
CutGroup Molecule::cutGroup(CutGroupID id) const
{
    return d->cutGroup(id);
}

/////////////////////////////////////////////////////////
//@}

/** @name Molecule::coordGroups(...)
    Functions that are used to return copies of the CoordGroups that
    contain the grouped coordinates of atoms in the molecule.
*/
/////////////////////////////////////////////////////////
//@{

/** Return a copy of all of the CoordGroups that contain the coordinates
    of the atoms in this molecule. This is very fast, as this data is
    implicitly shared and held directly by the molecule.
*/
QVector<CoordGroup> Molecule::coordGroups() const
{
    return d->coordGroups();
}

/** Return a copy of the CoordGroups that contain the coordinates of atoms
    in the residue with number 'resnum'.

    \throw SireMol::missing_residue
*/
QHash<CutGroupID,CoordGroup> Molecule::coordGroups(ResNum resnum) const
{
    return d->coordGroups(resnum);
}

/** Return a copy of the CoordGroups that contain the coordinates of atoms
    in the residue at index 'resid'

    \throw SireError::invalid_index
*/
QHash<CutGroupID,CoordGroup> Molecule::coordGroups(ResID resid) const
{
    return d->coordGroups(resid);
}

template<class IDX>
QHash<CutGroupID,CoordGroup> getCoordGroups(const Molecule &mol, const QSet<IDX> &idxs)
{
    QHash<CutGroupID,CoordGroup> cgroups;

    foreach (IDX idx, idxs)
    {
        cgroups.unite( mol.coordGroups(idx) );
    }

    return cgroups;
}

/** Return a copy of the CoordGroups that contain the coordinates of the
    atoms that are contained in the residues whose numbers are in 'resnums'.

    \throw SireMol::missing_residue
*/
QHash<CutGroupID,CoordGroup> Molecule::coordGroups(const QSet<ResNum> &resnums) const
{
    return getCoordGroups<ResNum>(*this, resnums);
}

/** Return a copy of the CoordGroups that contain the coordinates of the
    atoms that are contained in the residues whose indicies are in 'resids'.

    \throw SireError::invalid_index
*/
QHash<CutGroupID,CoordGroup> Molecule::coordGroups(const QSet<ResID> &resids) const
{
    return getCoordGroups<ResID>(*this, resids);
}

/** Return a copy of the CoordGroup that holds the coordinates for the
    atoms in the CutGroup with ID == id

    \throw SireMol::missing_cutgroup
*/
CoordGroup Molecule::coordGroup(CutGroupID id) const
{
    return d->coordGroup(id);
}

/** Return a copy of the CoordGroups with IDs in 'cgids'

    \throw SireMol::missing_cutgroup
*/
QHash<CutGroupID,CoordGroup> Molecule::coordGroups(const QSet<CutGroupID> &cgids) const
{
    QHash<CutGroupID,CoordGroup> cgroups;
    cgroups.reserve(cgids.count());

    foreach ( CutGroupID cgid, cgids )
    {
        cgroups.insert( cgid, this->coordGroup(cgid) );
    }

    return cgroups;
}

/////////////////////////////////////////////////////////
//@}

/** @name Molecule::atom(....)
    Functions that are used to return copies of atoms in the molecule.
*/
/////////////////////////////////////////////////////////
//@{

/** Return a copy of the atom at index 'cgatmid'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
Atom Molecule::atom(const CGAtomID &cgatmid) const
{
    return d->atom(cgatmid);
}

/** Return a copy of the atom at index 'atomid'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
Atom Molecule::atom(const IDMolAtom &atomid) const
{
    return d->atom(atomid);
}

/////////////////////////////////////////////////////////
//@}

/** @name Molecule::coordinates(....)
    Functions that are used to return the coordinates of atoms or groups
    of atoms in the molecule.
*/
/////////////////////////////////////////////////////////
//@{

/** Return a copy of the coordinates of the atom at index 'cgatmid'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
Vector Molecule::coordinates(const CGAtomID &cgatmid) const
{
    return d->coordinates(cgatmid);
}

/** Return a copy of the coordinates of the atom at index 'atomid'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
Vector Molecule::coordinates(const IDMolAtom &atomid) const
{
    return d->coordinates(atomid);
}

/** Return copies of the coordinates of the atoms indexed by the passed indicies,
    with the returned coordinates stored in a hash that is itself then indexed
    by those indicies.

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
QHash<CGAtomID,Vector> Molecule::coordinates(const QSet<CGAtomID> &cgatomids) const
{
    return d->coordinates(cgatomids);
}

/** Return copies of the coordinates of the atoms indexed by the passed indicies,
    with the returned coordinates stored in a hash that is itself then indexed
    by those indicies.

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
QHash<ResNumAtomID,Vector> Molecule::coordinates(
                                      const QSet<ResNumAtomID> &resatomids) const
{
    return d->coordinates(resatomids);
}

/** Return copies of the coordinates of the atoms indexed by the passed indicies,
    with the returned coordinates stored in a hash that is itself then indexed
    by those indicies.

    \throw SireError::invalid_index
*/
QHash<ResIDAtomID,Vector> Molecule::coordinates(
                                      const QSet<ResIDAtomID> &resatomids) const
{
    return d->coordinates(resatomids);
}

/** Return copies of the coordinates of the atoms indexed by the passed indicies,
    with the returned coordinates stored in a hash that is itself then indexed
    by those indicies.

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
QHash<AtomIndex,Vector> Molecule::coordinates(const QSet<AtomIndex> &atoms) const
{
    return d->coordinates(atoms);
}

/** Return copies of the coordinates of the atoms indexed by the passed indicies,
    with the returned coordinates stored in a hash that is itself then indexed
    by those indicies.

    \throw SireError::invalid_index
*/
QHash<AtomID,Vector> Molecule::coordinates(const QSet<AtomID> &atomids) const
{
    return d->coordinates(atomids);
}

/** Return a copy of the coordinates of all of the atoms in this molecule, in
    the order that the atoms appear in this molecule. */
QVector<Vector> Molecule::coordinates() const
{
    return d->coordinates();
}

/** Return an array of all of the coordinates in the CutGroup with ID == cgid,
    with the coordinates having the same order in the array as the atoms do in
    the CutGroup.

    \throw SireMol::missing_cutgroup
*/
QVector<Vector> Molecule::coordinates(CutGroupID cgid)
{
    return d->coordinates(cgid);
}

/** Return arrays of all of the coordinates in the CutGroups whose ID numbers are
    passed in 'cgids', returning the arrays in a hash indexed by CutGroupID.

    \throw SireMol::missing_cutgroup
*/
QHash< CutGroupID,QVector<Vector> > Molecule::coordinates(
                                        const QSet<CutGroupID> &cgids) const
{
    return d->coordinates(cgids);
}

/** Return an array of the coordinates of the atoms in the residue with number
    'resnum'. The coordinates are stored in the array in the same order as the
    atoms are stored in the residue.

    \throw SireMol::missing_residue
*/
QVector<Vector> Molecule::coordinates(ResNum resnum)
{
    return d->coordinates(resnum);
}

/** Return arrays of the coordinates of the atoms in the residues with numbers
    passed in 'resnums'. The coordinates are returned in a hash also indexed
    by ResNum.

    \throw SireMol::missing_residue
*/
QHash< ResNum,QVector<Vector> > Molecule::coordinates(
                                      const QSet<ResNum> &resnums) const
{
    return d->coordinates(resnums);
}

/** Return an array of the coordinates of the atoms in the residue at index
    'resid'. The coordinates are stored in the array in the same order as the
    atoms are stored in the residue.

    \throw SireError::invalid_index
*/
QVector<Vector> Molecule::coordinates(ResID resid)
{
    return d->coordinates(resid);
}

/** Return arrays of the coordinates of the atoms in the residues at indicies
    passed in 'resids'. The coordinates are returned in a hash also indexed
    by ResID.

    \throw SireError::invalid_index
*/
QHash< ResID,QVector<Vector> > Molecule::coordinates(
                                      const QSet<ResID> &resids) const
{
    return d->coordinates(resids);
}

/////////////////////////////////////////////////////////
//@}

/** Return the name of the molecule. */
QString Molecule::name() const
{
    return info().name();
}

/** Return the name of the residue at number 'resnum'

    \throw SireMol::missing_residue
*/
QString Molecule::residueName(ResNum resnum) const
{
    return info().residueName(resnum);
}

/** Return the name of the residue at index 'resid'

    \throw SireError::invalid_index
*/
QString Molecule::residueName(ResID resid) const
{
    return info().residueName(resid);
}

/** Return the number of the residue at index 'resid'

    \throw SireError::invalid_index
*/
ResNum Molecule::residueNumber(ResID resid) const
{
    return info().residueNumber(resid);
}

/** Return the number of the first residue that has the
    name 'resname'

    \throw SireMol::missing_residue
*/
ResNum Molecule::residueNumber(const QString &resname) const
{
    return info().residueNumber(resname);
}

/** @name Molecule::isEmpty(...)
    Functions used to query if parts of the molecule are empty.
*/
/////////////////////////////////////////////////////////
//@{

/** Return whether or not the entire molecule is empty */
bool Molecule::isEmpty() const
{
    return info().isEmpty();
}

/** Return whether or not the residue with number 'resnum' is empty.

    \throw SireMol::missing_residue
*/
bool Molecule::isEmpty(ResNum resnum) const
{
    return info().isEmpty(resnum);
}

/** Return whether or not the residue at index 'resid' is empty.

    \throw SireError::invalid_index
*/
bool Molecule::isEmpty(ResID resid) const
{
    return info().isEmpty(resid);
}

/** Return whether or not the CutGroup with ID == cgid is empty.

    \throw SireMol::missing_cutgroup
*/
bool Molecule::isEmpty(CutGroupID cgid) const
{
    return info().isEmpty(cgid);
}

/////////////////////////////////////////////////////////
//@}

/** Return the list of the names of the residues in this molecule, in
    the same order that they exist in this molecule. (i.e. the same
    order as the residue numbers are returned by Molecule::residueNumbers() )
*/
QStringList Molecule::residueNames() const
{
    return info().residueNames();
}

/** @name Molecule::residueNumbers(...)
    Functions to return the numbers of groups of residues.
*/
/////////////////////////////////////////////////////////
//@{

/** Return an array of the numbers of all of the residues in this molecule.
    This is in the same order as the residues exist in this molecule,
    (i.e. the same as the residue names returned by Molecule::residueNames() )
*/
QVector<ResNum> Molecule::residueNumbers() const
{
    return info().residueNumbers();
}

/** Return an array of the numbers of all of the residues that are called
    'resnam'. This returns an empty array if there are no residues with
    this name in this molecule. */
QVector<ResNum> Molecule::residueNumbers(const QString &resnam) const
{
    return info().residueNumbers(resnam);
}

/** Return an array of the numbers of all of the residues who have atoms
    that are in the CutGroup with ID == cgid

    \throw SireMol::missing_cutgroup
*/
QVector<ResNum> Molecule::residueNumbers(CutGroupID cgid) const
{
    return info().residueNumbers(cgid);
}

/////////////////////////////////////////////////////////
//@}

/** Return copies of the residues bonded to the residue with number 'resnum',
    indexed by residue number.

    \throw SireMol::missing_residue
*/
QHash<ResNum,Residue> Molecule::residuesBondedTo(ResNum resnum) const
{
    return residues( connectivity().resNumsBondedTo(resnum) );
}

/** Return copies of the residues bonded to the residue at index 'resid',
    indexed by residue number.

    \throw SireError::invalid_index
*/
QHash<ResNum,Residue> Molecule::residuesBondedTo(ResID resid) const
{
    return residuesBondedTo( residueNumber(resid) );
}

/** @name Molecule::contains(...)
    Functions used to query whether or not the molecule contains various objects.
*/
/////////////////////////////////////////////////////////
//@{

/** Return whether or not this molecule contains a CutGroup with ID == cgid */
bool Molecule::contains(CutGroupID cgid) const
{
    return info().contains(cgid);
}

/** Return whether or not this molecule contains a residue with number 'resnum' */
bool Molecule::contains(ResNum resnum) const
{
    return info().contains(resnum);
}

/** Return whether or not this molecule contains a residue at index 'resid' */
bool Molecule::contains(ResID resid) const
{
    return info().contains(resid);
}

/** Return whether or not this molecule contains an atom at index 'cgatomid' */
bool Molecule::contains(const CGAtomID &cgatomid) const
{
    return info().contains(cgatomid);
}

/** Return whether or not this molecule contains an atom at index 'atomid' */
bool Molecule::contains(const IDMolAtom &atomid) const
{
    return info().contains(atomid);
}

/** Return whether or not this molecule contains the bond 'bond' */
bool Molecule::contains(const Bond &bond) const
{
    return connectivity().contains(bond);
}

/////////////////////////////////////////////////////////
//@}

/** @name Molecule::nAtoms(...)
    Functions used to returns the number of atoms in different parts of the molecule.
*/
/////////////////////////////////////////////////////////
//@{

/** Return the total number of atoms in the molecule. */
int Molecule::nAtoms() const
{
    return info().nAtoms();
}

/** Return the total number of atoms in the residue with number 'resnum'

    \throw SireMol::missing_residue
*/
int Molecule::nAtoms(ResNum resnum) const
{
    return info().nAtoms(resnum);
}

/** Return the total number of atoms in the residue at index 'resid'

    \throw SireError::invalid_index
*/
int Molecule::nAtoms(ResID resid) const
{
    return info().nAtoms(resid);
}

/** Return the total number of atoms in the CutGroup with ID == cgid

    \throw SireMol::missing_cutgroup
*/
int Molecule::nAtoms(CutGroupID cgid) const
{
    return info().nAtoms(cgid);
}

/////////////////////////////////////////////////////////
//@}

/** Return the total number of residues in the Molecule */
int Molecule::nResidues() const
{
    return info().nResidues();
}

/** Return the total number of CutGroups in the molecule */
int Molecule::nCutGroups() const
{
    return info().nCutGroups();
}

/** @name Molecule::nBonds(...)
    Functions used to return the number of bonds in different parts of the molecule.
*/
/////////////////////////////////////////////////////////
//@{

/** Return the total number of bonds in this molecule */
int Molecule::nBonds() const
{
    return connectivity().nBonds();
}

/** Return the total number of bonds in the residue with number 'resnum'

    \throw SireMol::missing_residue
*/
int Molecule::nBonds(ResNum resnum) const
{
    return connectivity(resnum).nBonds();
}

/** Return the total number of bonds in the residue with index 'resid'

    \throw SireError::invalid_index
*/
int Molecule::nBonds(ResID resid) const
{
    return connectivity(resid).nBonds();
}

/** Return the total number of inter-residue bonds in this molecule */
int Molecule::nInterBonds() const
{
    return connectivity().nInterBonds();
}

/** Return the total number of inter-bonds in the residue with number 'resnum'

    \throw SireMol::missing_residue
*/
int Molecule::nInterBonds(ResNum resnum) const
{
    return connectivity(resnum).nInterBonds();
}

/** Return the total number of inter-bonds in the residue with index 'resid'

    \throw SireError::invalid_index
*/
int Molecule::nInterBonds(ResID resid) const
{
    return connectivity(resid).nInterBonds();
}

/** Return the total number of intra-bonds in this molecule */
int Molecule::nIntraBonds() const
{
    return connectivity().nIntraBonds();
}

/** Return the total number of intra-bonds in the residue with number 'resnum'

    \throw SireMol::missing_residue
*/
int Molecule::nIntraBonds(ResNum resnum) const
{
    return connectivity(resnum).nIntraBonds();
}

/** Return the total number of intra-bonds in the residue with index 'resid'

    \throw SireError::invalid_index
*/
int Molecule::nIntraBonds(ResID resid) const
{
    return connectivity(resid).nIntraBonds();
}

/////////////////////////////////////////////////////////
//@}

/** Return the names of the atoms in the residue with number 'resnum',
    returned in the same order as the atoms in the residue.

    \throw SireMol::missing_residue
*/
QStringList Molecule::atomNames(ResNum resnum) const
{
    return info().atomNames(resnum);
}

/** Return the names of the atoms in the residue at index 'resid',
    returned in the same order as the atoms in the residue.

    \throw SireError::invalid_index
*/
QStringList Molecule::atomNames(ResID resid) const
{
    return info().atomNames(resid);
}

/** Return the geometric line that lies along the bond 'bnd'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
SireMaths::Line Molecule::bond(const Bond &bnd) const
{
    return d->bond(bnd);
}

/** Return the geometric triangle that represents the three atoms in the
    angle 'ang'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
SireMaths::Triangle Molecule::angle(const SireMol::Angle &ang) const
{
    return d->angle(ang);
}

/** Return the geometric torsion that represents the four atoms in the
    dihedral 'dih'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
SireMaths::Torsion Molecule::dihedral(const Dihedral &dih) const
{
    return d->dihedral(dih);
}

/** Return the geometric torsion that represents the four atoms of the
    improper angle 'imp'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
SireMaths::Torsion Molecule::improper(const Improper &imp) const
{
    return d->improper(imp);
}

/** @name Molecule::measure(...)
    Functions used to measure internal coordinates of the molecule.
*/
/////////////////////////////////////////////////////////
//@{

/** Return the length of the bond 'bnd'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
double Molecule::measure(const Bond &bnd) const
{
    return d->measure(bnd);
}

/** Return the size of the angle 'ang'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
SireMaths::Angle Molecule::measure(const SireMol::Angle &ang) const
{
    return d->measure(ang);
}

/** Return the size of the dihedral 'dih'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
SireMaths::Angle Molecule::measure(const Dihedral &dih) const
{
    return d->measure(dih);
}

/** Return the size of the improper angle 'imp'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
SireMaths::Angle Molecule::measure(const Improper &imp) const
{
    return d->measure(imp);
}

/////////////////////////////////////////////////////////
//@}

/** Return the relative weights of the two groups 'group0' and 'group1', using
    the weight function 'weightfunc'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
double Molecule::getWeight(const AtomIDGroup &group0, const AtomIDGroup &group1,
                           const WeightFunction &weightfunc) const
{
    return d->getWeight(group0, group1, weightfunc);
}

/** @name Molecule::translate(...)
    Functions used to translate all or part of the molecule.
*/
/////////////////////////////////////////////////////////
//@{

/** Translate all of the molecule by 'delta' */
void Molecule::translate(const Vector &delta)
{
    d->translate(delta);
}

/** Translate the group of atoms 'group' by 'delta'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void Molecule::translate(const AtomIDGroup &group, const Vector &delta)
{
    d->translate(group, delta);
}

/** Translate the atom 'atom' by 'delta'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void Molecule::translate(const AtomIndex &atom, const Vector &delta)
{
    d->translate(atom, delta);
}

/** Translate the set of atoms in 'atoms' by 'delta'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void Molecule::translate(const QSet<AtomIndex> &atoms, const Vector &delta)
{
    d->translate(atoms, delta);
}

/** Translate the atoms in the residue with number 'resnum' whose names are
    in 'atoms' by 'delta'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void Molecule::translate(ResNum resnum, const QStringList &atoms, const Vector &delta)
{
    d->translate(resnum, atoms, delta);
}

/** Translate the residue with number 'resnum' by 'delta'

    \throw SireMol::missing_residue
*/
void Molecule::translate(ResNum resnum, const Vector &delta)
{
    d->translate(resnum, delta);
}

/** Translate the set of residues whose residue numbers are in 'resnums'
    by 'delta'

    \throw SireMol::missing_residue
*/
void Molecule::translate(const QSet<ResNum> &resnums, const Vector &delta)
{
    d->translate(resnums, delta);
}

/** Translate the atoms in the residue at index 'resid' whose names are in
    'atoms' by 'delta'

    \throw SireError::invalid_index
    \throw SireMol::missing_atom
*/
void Molecule::translate(ResID resid, const QStringList &atoms, const Vector &delta)
{
    d->translate(resid, atoms, delta);
}

/** Translate the residue at index 'resid' by 'delta'

    \throw SireError::invalid_index
*/
void Molecule::translate(ResID resid, const Vector &delta)
{
    d->translate(resid, delta);
}

/** Translate the set of residues whose indicies are in 'resids' by 'delta'

    \throw SireError::invalid_index
*/
void Molecule::translate(const QSet<ResID> &resids, const Vector &delta)
{
    d->translate(resids, delta);
}

/** Translate all of the atoms in the CutGroup with ID == cgid by 'delta'

    \throw SireMol::missing_cutgroup
*/
void Molecule::translate(CutGroupID cgid, const Vector &delta)
{
    d->translate(cgid, delta);
}

/** Translate all of the atoms in the CutGroups with IDs in 'cgids' by 'delta'

    \throw SireMol::missing_cutgroup
*/
void Molecule::translate(const QSet<CutGroupID> &cgids, const Vector &delta)
{
    d->translate(cgids, delta);
}

/////////////////////////////////////////////////////////
//@}

/** @name Molecule::rotate(...)
    Functions used to rotate part or all of the molecule using quaternions or matricies.
*/
/////////////////////////////////////////////////////////
//@{

/** Rotate the whole molecule using the quaternion 'quat' about the point 'point' */
void Molecule::rotate(const Quaternion &quat, const Vector &point)
{
    d->rotate(quat, point);
}

/** Rotate the atoms in the group 'group' using the quaternion 'quat' about the
    point 'point'.

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void Molecule::rotate(const AtomIDGroup &group, const Quaternion &quat,
                      const Vector &point)
{
    d->rotate(group, quat, point);
}

/** Rotate the atom 'atom' using the quaternion 'quat' about the point 'point'.

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void Molecule::rotate(const AtomIndex &atom, const Quaternion &quat,
                      const Vector &point)
{
    d->rotate(atom, quat, point);
}

/** Rotate the atoms in 'atoms' using the quaternion 'quat' about the point 'point'.

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void Molecule::rotate(const QSet<AtomIndex> &atoms, const Quaternion &quat,
                      const Vector &point)
{
    d->rotate(atoms, quat, point);
}

/** Rotate the atoms in the residue with number 'resnum' whose names are in 'atoms'
    using the quaternion 'quat' about the point 'point'.

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void Molecule::rotate(ResNum resnum, const QStringList &atoms,
                      const Quaternion &quat, const Vector &point)
{
    d->rotate(resnum, atoms, quat, point);
}

/** Rotate the atoms in the residue with number 'resnum' using the quaternion
    'quat' about the point 'point'.

    \throw SireMol::missing_residue
*/
void Molecule::rotate(ResNum resnum, const Quaternion &quat, const Vector &point)
{
    d->rotate(resnum, quat, point);
}

/** Rotate the atoms in the residues whose numbers are in 'resnums'
    using the quaternion 'quat' about the point 'point'.

    \throw SireMol::missing_residue
*/
void Molecule::rotate(const QSet<ResNum> &resnums, const Quaternion &quat,
                      const Vector &point)
{
    d->rotate(resnums, quat, point);
}

/** Rotate the atoms in the residue at index 'resid' whose names are in 'atoms'
    using the quaternion 'quat' about the point 'point'.

    \throw SireError::invalid_index
    \throw SireMol::missing_atom
*/
void Molecule::rotate(ResID resid, const QStringList &atoms,
                      const Quaternion &quat, const Vector &point)
{
    d->rotate(resid, atoms, quat, point);
}

/** Rotate the atoms in the residue at index 'resid' using the quaternion 'quat'
    about the point 'point'.

    \throw SireError::invalid_index
*/
void Molecule::rotate(ResID resid, const Quaternion &quat, const Vector &point)
{
    d->rotate(resid, quat, point);
}

/** Rotate the atoms in the residues whose indicies are in 'resids'
    using the quaternion 'quat' about the point 'point'.

    \throw SireError::invalid_index
*/
void Molecule::rotate(const QSet<ResID> &resids, const Quaternion &quat,
                      const Vector &point)
{
    d->rotate(resids, quat, point);
}

/** Rotate the atoms in the CutGroup with ID == cgid using the quaternion 'quat'
    about the point 'point'.

    \throw SireMol::missing_cutgroup
*/
void Molecule::rotate(CutGroupID cgid, const Quaternion &quat, const Vector &point)
{
    d->rotate(cgid, quat, point);
}

/** Rotate the atoms in the CutGroups whose IDs are in 'cgids'
    using the quaternion 'quat' about the point 'point'.

    \throw SireMol::missing_cutgroup
*/
void Molecule::rotate(const QSet<CutGroupID> &cgids, const Quaternion &quat,
                      const Vector &point)
{
    d->rotate(cgids, quat, point);
}

/** Rotate the whole molecule using the matrix 'matrix' about the point 'point' */
void Molecule::rotate(const Matrix &matrix, const Vector &point)
{
    d->rotate(matrix, point);
}

/** Rotate the atoms in the group 'group' using the matrix 'matrix' about the
    point 'point'.

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void Molecule::rotate(const AtomIDGroup &group, const Matrix &matrix,
                      const Vector &point)
{
    d->rotate(group, matrix, point);
}

/** Rotate the atom 'atom' using the matrix 'matrix' about the point 'point'.

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void Molecule::rotate(const AtomIndex &atom, const Matrix &matrix,
                      const Vector &point)
{
    d->rotate(atom, matrix, point);
}

/** Rotate the atoms in 'atoms' using the matrix 'matrix' about the point 'point'.

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void Molecule::rotate(const QSet<AtomIndex> &atoms, const Matrix &matrix,
                      const Vector &point)
{
    d->rotate(atoms, matrix, point);
}

/** Rotate the atoms in the residue with number 'resnum' whose names are in 'atoms'
    using the matrix 'matrix' about the point 'point'.

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void Molecule::rotate(ResNum resnum, const QStringList &atoms,
                      const Matrix &matrix, const Vector &point)
{
    d->rotate(resnum, atoms, matrix, point);
}

/** Rotate the atoms in the residue with number 'resnum' using the matrixernion
    'matrix' about the point 'point'.

    \throw SireMol::missing_residue
*/
void Molecule::rotate(ResNum resnum, const Matrix &matrix, const Vector &point)
{
    d->rotate(resnum, matrix, point);
}

/** Rotate the atoms in the residues whose numbers are in 'resnums'
    using the matrix 'matrix' about the point 'point'.

    \throw SireMol::missing_residue
*/
void Molecule::rotate(const QSet<ResNum> &resnums, const Matrix &matrix,
                      const Vector &point)
{
    d->rotate(resnums, matrix, point);
}

/** Rotate the atoms in the residue at index 'resid' whose names are in 'atoms'
    using the matrix 'matrix' about the point 'point'.

    \throw SireError::invalid_index
    \throw SireMol::missing_atom
*/
void Molecule::rotate(ResID resid, const QStringList &atoms,
                      const Matrix &matrix, const Vector &point)
{
    d->rotate(resid, atoms, matrix, point);
}

/** Rotate the atoms in the residue at index 'resid' using the matrix 'matrix'
    about the point 'point'.

    \throw SireError::invalid_index
*/
void Molecule::rotate(ResID resid, const Matrix &matrix, const Vector &point)
{
    d->rotate(resid, matrix, point);
}

/** Rotate the atoms in the residues whose indicies are in 'resids'
    using the matrix 'matrix' about the point 'point'.

    \throw SireError::invalid_index
*/
void Molecule::rotate(const QSet<ResID> &resids, const Matrix &matrix,
                      const Vector &point)
{
    d->rotate(resids, matrix, point);
}

/** Rotate the atoms in the CutGroup with ID == cgid using the matrix 'matrix'
    about the point 'point'.

    \throw SireMol::missing_cutgroup
*/
void Molecule::rotate(CutGroupID cgid, const Matrix &matrix, const Vector &point)
{
    d->rotate(cgid, matrix, point);
}

/** Rotate the atoms in the CutGroups whose IDs are in 'cgids'
    using the matrix 'matrix' about the point 'point'.

    \throw SireMol::missing_cutgroup
*/
void Molecule::rotate(const QSet<CutGroupID> &cgids, const Matrix &matrix,
                      const Vector &point)
{
    d->rotate(cgids, matrix, point);
}

/////////////////////////////////////////////////////////
//@}

/** @name Molecule::setCoordinates(...)
    Functions used to set directly the coordinates of atoms in the molecule.
*/
/////////////////////////////////////////////////////////
//@{

/** Set the coordinates of the atoms in the CutGroup with ID == cgid to
    'newcoords'.

    \warning 'newcoords' must have the same number of points as there
             are atoms in the CutGroup or else an exception will be thrown.

    \throw SireError::incompatible_error
    \throw SireMol::missing_cutgroup
*/
void Molecule::setCoordinates(CutGroupID cgid, const CoordGroup &newcoords)
{
    d->setCoordinates(cgid, newcoords);
}

/** Set the coordinates of lots of CutGroups at once, with the new
    coordinates in 'newcoords', indexed by the ID of the CutGroup
    for which they are updating.

    \throw SireError::incompatible_error
    \throw SireMol::missing_cutgroup
*/
void Molecule::setCoordinates(const QHash<CutGroupID,CoordGroup> &newcoords)
{
    d->setCoordinates(newcoords);
}

/** Set the coordinates from the array of CoordGroups - these must be
    in the same order as they are in the molecule, and must have the
    same number of points in each CoordGroup as there are atoms in each
    CutGroup

    \throw SireError::incompatible_error
*/
void Molecule::setCoordinates(const QVector<CoordGroup> &newcoords)
{
    d->setCoordinates(newcoords);
}

/** Set the coordinates of all of the atoms in the whole molecule to the
    the coordinates stored in the array 'newcoords'.

    \warning The number of coordinates in 'newcoords' must be the same as
             the number of atoms in the molecule or else an exception will
             be thrown.

    \throw SireError::incompatible_error
*/
void Molecule::setCoordinates(const QVector<Vector> &newcoords)
{
    d->setCoordinates(newcoords);
}

/** Set the coordinates of the CutGroup with ID == cgid to the coordinates
    stored in the array 'newcoords'.

    \warning The number of coordinates in 'newcoords' must be the same as
             the number of atoms in the CutGroup or else an exception will
             be thrown.

    \throw SireError::incompatible_error
    \throw SireMol::missing_cutgroup
*/
void Molecule::setCoordinates(CutGroupID cgid, const QVector<Vector> &newcoords)
{
    d->setCoordinates(cgid, newcoords);
}

/** Set the coordinates for many CutGroups, using the arrays of coordinates
    stored in 'newcoords', indexed by the ID numbers of the CutGroups for
    which they are updating.

    \throw SireError::incompatible_error
    \throw SireMol::missing_cutgroup
*/
void Molecule::setCoordinates(const QHash< CutGroupID,QVector<Vector> > &newcoords)
{
    d->setCoordinates(newcoords);
}

/** Set the coordinates of the residue 'resnum' using the array of coordinates
    in 'newcoords'. The coordinates should be in the same order as the atoms
    are in the residue.

    \warning The number of coordinates in 'newcoords' must be the same as
             the number of atoms in the residue or else an exception will be
             thrown.

    \throw SireError::incompatible_error
    \throw SireMol::missing_residue
*/
void Molecule::setCoordinates(ResNum resnum, const QVector<Vector> &newcoords)
{
    d->setCoordinates(resnum, newcoords);
}

/** Set the coordinates of a large number of residues, using the arrays
    of coordinates in 'newcoords' indexed by the residue number of the
    residue for which they are updating.

    \throw SireError::incompatible_error
    \throw SireMol::missing_residue
*/
void Molecule::setCoordinates(const QHash< ResNum,QVector<Vector> > &newcoords)
{
    d->setCoordinates(newcoords);
}

/** Set the coordinates of the residue at index 'resid' using the array of
    coordinates 'newcoords'.

    \warning The number of coordinates in 'newcoords' must be the same as
             the number of atoms in the residue or else an exception will be
             thrown.

    \throw SireError::incompatible_error
    \throw SireError::invalid_index
*/
void Molecule::setCoordinates(ResID resid, const QVector<Vector> &newcoords)
{
    d->setCoordinates(resid, newcoords);
}

/** Set the coordinates of many residues, with the new coordinates stored
    as arrays indexed by the index (ResID) of the residue for which they
    are updating.

    \throw SireError::incompatible_error
    \throw SireError::invalid_index
*/
void Molecule::setCoordinates(const QHash< ResID,QVector<Vector> > &newcoords)
{
    d->setCoordinates(newcoords);
}

/** Set the coordinates of the individual atom 'atom' to 'newcoords'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void Molecule::setCoordinates(const AtomIndex &atom, const Vector &newcoords)
{
    d->setCoordinates(atom, newcoords);
}

/** Set the coordinates of many atoms to 'newcoords', with the new coordinates
    indexed by the AtomIndex of the atoms that they are updating.

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void Molecule::setCoordinates(const QHash<AtomIndex,Vector> &newcoords)
{
    d->setCoordinates(newcoords);
}

/** Set the coordinates of the atom at index 'cgatomid' to 'newcoords'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
void Molecule::setCoordinates(const CGAtomID &cgatomid, const Vector &newcoords)
{
    d->setCoordinates(cgatomid, newcoords);
}

/** Set the coordinates of many atoms, with the new coordinates indexed
    by the CGAtomID of the atoms that they are updating.

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
void Molecule::setCoordinates(const QHash<CGAtomID,Vector> &newcoords)
{
    d->setCoordinates(newcoords);
}

/** Set the coordinates of the atom with index 'resatomid' to 'newcoords'.

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
void Molecule::setCoordinates(const ResNumAtomID &resatomid, const Vector &newcoords)
{
    d->setCoordinates(resatomid, newcoords);
}

/** Set the coordinates of many atoms, with the new coordinates indexed
    by the ResNumAtomID of the atoms that they are updating.

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
void Molecule::setCoordinates(const QHash<ResNumAtomID,Vector> &newcoords)
{
    d->setCoordinates(newcoords);
}

/** Set the coordinates of the atom at index 'resatomid' to 'newcoords'

    \throw SireError::invalid_index
*/
void Molecule::setCoordinates(const ResIDAtomID &resatomid, const Vector &newcoords)
{
    d->setCoordinates(resatomid, newcoords);
}

/** Set the coordinates for many atoms, with the new coordinates indexed
    by the ResIDAtomID of the atoms that they are updating.

    \throw SireError::invalid_index
*/
void Molecule::setCoordinates(const QHash<ResIDAtomID,Vector> &newcoords)
{
    d->setCoordinates(newcoords);
}

/////////////////////////////////////////////////
//@}

/** @name Molecule::change(...)
    Functions used to change the length or size of internal degrees of freedom
    within this molecule.
*/
/////////////////////////////////////////////////
//@{

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

/////////////////////////////////////////////////
//@}

/** @name Molecule::set(...)
    Functions used to set the length or size of degrees of freedom
    within this molecule.
*/
/////////////////////////////////////////////////
//@{

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
//@}

/** Assert that this molecule is the same as 'other' - i.e. that
    they both have the same MoleculeID number.

    \throw SireError::incompatible_error
*/
void Molecule::assertSameMolecule(const Molecule &other) const
{
    if (this->ID() != other.ID())
        throw SireError::incompatible_error( QObject::tr(
            "This molecule (\"%1\", ID == %2, Version == %3) is not "
            "the different to the other molecule (\"%4\", ID == %5, "
            "Version == %6)")
                .arg(this->name()).arg(this->ID())
                .arg(this->version().toString())
                .arg(other.name()).arg(other.ID())
                .arg(other.version().toString()), CODELOC );
}

/** Assert that this molecule has the same major version as 'other'
    - this also asserts that both molecules have the same ID number.

    \throw SireError::incompatible_error
*/
void Molecule::assertSameMajorVersion(const Molecule &other) const
{
    this->assertSameMolecule(other);

    if (this->version().major() != other.version().major())
        throw SireError::incompatible_error( QObject::tr(
            "This molecule (\"%1\", ID == %2, Version == %3) has a "
            "different major version to the other molecule (\"%4\", "
            "ID == %5, Version == %6)")
                .arg(this->name()).arg(this->ID())
                .arg(this->version().toString())
                .arg(other.name()).arg(other.ID())
                .arg(other.version().toString()), CODELOC );
}

/** Assert that this molecule has the same version as 'other'
    - this also asserts that both molecules have the same ID number.

    \throw SireError::incompatible_error
*/
void Molecule::assertSameVersion(const Molecule &other) const
{
    this->assertSameMolecule(other);

    if (this->version() != other.version())
        throw SireError::incompatible_error( QObject::tr(
            "This molecule (\"%1\", ID == %2, Version == %3) has a "
            "different version to the other molecule (\"%4\", "
            "ID == %5, Version == %6)")
                .arg(this->name()).arg(this->ID())
                .arg(this->version().toString())
                .arg(other.name()).arg(other.ID())
                .arg(other.version().toString()), CODELOC );
}
