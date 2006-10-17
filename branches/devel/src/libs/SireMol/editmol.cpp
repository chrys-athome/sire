/**
  * @file
  *
  * C++ Implementation: EditMol
  *
  * Description:
  * Implementation of EditMol
  *
  * Author: Christopher Woods, (C) 2006
  *
  * Copyright: See COPYING file that comes with this distribution
  *
  */

#include "qhash_siremol.h"

#include <QHash>
#include <QString>

#include "editmol.h"

#include "cgatomid.h"
#include "cgnumatomid.h"
#include "resnumatomid.h"
#include "residatomid.h"

#include "atom.h"
#include "bond.h"
#include "angle.h"
#include "dihedral.h"
#include "improper.h"

#include "cutgroup.h"
#include "editres.h"

#include "splitmolecule.h"
#include "atomidgroup.h"
#include "weightfunction.h"

#include "molecule.h"
#include "moleculedata.h"

#include "SireMol/errors.h"
#include "SireError/errors.h"

#include "SireVol/coordgroup.h"

#include "SireMaths/quaternion.h"
#include "SireMaths/matrix.h"
#include "SireMaths/triangle.h"
#include "SireMaths/line.h"
#include "SireMaths/torsion.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMol;
using namespace SireStream;

static const RegisterMetaType<EditMol> r_editmol("SireMol::EditMol");

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const EditMol &editmol)
{
    writeHeader(ds, r_editmol, 1);

    SharedDataStream(ds) << editmol.d;

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, EditMol &editmol)
{
    VersionID v = readHeader(ds, r_editmol);

    if (v == 1)
    {
        SharedDataStream(ds) >> editmol.d;
    }
    else
        throw version_error(v, "1", r_editmol, CODELOC);

    return ds;
}

/** Hash an EditMol */
uint qHash(const EditMol &editmol)
{
    return qHash(editmol.name());
}

/** Construct an empty, unnamed molecule */
EditMol::EditMol() : d( EditMolData::shared_null() )
{}

/** Construct an empty molecule called 'name', and using default
    residue-cutting */
EditMol::EditMol(const QString &name) : d( new EditMolData(name) )
{}

/** Construct a new molecule called 'name' and using the cutting scheme
    provided by 'cuttingfunc' */
EditMol::EditMol(const QString &name, const CuttingFunction &cuttingfunc)
        : d( new EditMolData(name,cuttingfunc) )
{}

/** Construct an unnamed molecule that uses the cutting scheme provided
    by 'cuttingfunc' */
EditMol::EditMol(const CuttingFunction &cuttingfunc)
        : d( new EditMolData(cuttingfunc) )
{}

/** Construct a molecule that is a copy of the molecule that contains
    the residue 'res' */
EditMol::EditMol(const EditRes &res) : d(res.d)
{}

/** Copy constructor */
EditMol::EditMol(const EditMol &other)
        : d( other.d )
{}

/** Destructor */
EditMol::~EditMol()
{}

/** Assignment operator */
EditMol& EditMol::operator=(const EditMol &other)
{
    d = other.d;
    return *this;
}

/** Comparison operator */
bool EditMol::operator==(const EditMol &other) const
{
    return (d == other.d) or (*d == *(other.d));
}

/** Comparison operator */
bool EditMol::operator!=(const EditMol &other) const
{
    return (d != other.d) and (*d != *(other.d));
}

/** Return a copy of the CutGroup with ID == 'cgid'

    \throw SireMol::missing_cutgroup
*/
CutGroup EditMol::operator[](CutGroupID cgid) const
{
    return d->cutGroup( d->at(cgid) );
}

/** Return a copy of the CutGroup with number 'cgnum'

    \throw SireMol::missing_cutgroup
*/
CutGroup EditMol::operator[](CutGroupNum cgnum) const
{
    return d->cutGroup(cgnum);
}

/** Return a copy of the residue at index 'resid'

    \throw SireError::invalid_index
*/
EditRes EditMol::operator[](ResID resid) const
{
    return EditRes(*this, resid);
}

/** Return a copy of the residue at index 'resnum' */
EditRes EditMol::operator[](ResNum resnum) const
{
    return EditRes(*this, resnum);
}

/** Return a copy of the atom at index 'atomid'

    \throw SireError::invalid_index
*/
Atom EditMol::operator[](AtomID atomid) const
{
    return d->atom( d->at(atomid) );
}

/** Return a copy of the atom at index 'cgatomid'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
Atom EditMol::operator[](const CGAtomID &cgatomid) const
{
    return d->atom( d->at(cgatomid) );
}

/** Return a copy of the atom at 'cgatomid'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
Atom EditMol::operator[](const CGNumAtomID &cgatomid) const
{
    return d->atom( d->at(cgatomid) );
}

/** Return a copy of the atom at 'resatomid'

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
Atom EditMol::operator[](const ResNumAtomID &resatomid) const
{
    return d->atom( d->at(resatomid) );
}

/** Return a copy of the atom at 'resatomid'

    \throw SireError::invalid_index
*/
Atom EditMol::operator[](const ResIDAtomID &resatomid) const
{
    return d->atom( d->at(resatomid) );
}

/** Return a copy of the atom with AtomIndex 'atm'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
Atom EditMol::operator[](const AtomIndex &atm) const
{
    return d->atom(atm);
}

/** Return a copy of the residue with number 'resnum'

    \throw SireMol::missing_residue
*/
EditRes EditMol::residue(ResNum resnum) const
{
    return this->operator[](resnum);
}

/** Return a copy of the residue at index 'resid'

    \throw SireError::invalid_index
*/
EditRes EditMol::residue(ResID resid) const
{
    return this->operator[](resid);
}

/** Return a copy of the first residue with the name 'resname'

    \throw SireMol::missing_residue
*/
EditRes EditMol::residue(const QString &resname) const
{
    return EditRes(*this, resname);
}

/** Return a hash containing copies of all of the residues in this
    molecule, indexed by residue number */
QHash<ResNum,EditRes> EditMol::residues() const
{
    QVector<ResNum> resnums = d->residueNumbers();

    QHash<ResNum,EditRes> allres;
    allres.reserve(resnums.count());

    foreach (ResNum resnum, resnums)
    {
        allres.insert( resnum, this->at(resnum) );
    }

    return allres;
}

/** Return a hash of the residues whose numbers are in 'resnums'

    \throw SireMol::missing_residue
*/
QHash<ResNum,EditRes> EditMol::residues(const QSet<ResNum> &resnums) const
{
    QHash<ResNum,EditRes> allres;
    allres.reserve(resnums.count());

    foreach (ResNum resnum, resnums)
    {
        allres.insert( resnum, this->at(resnum) );
    }

    return allres;
}

/** Return a copy of the residue with number 'resnum'

    \throw SireMol::missing_residue
*/
EditRes EditMol::at(ResNum resnum) const
{
    return this->operator[](resnum);
}

/** Return a copy of the residue at index 'resid'

    \throw SireError::invalid_index
*/
EditRes EditMol::at(ResID resid) const
{
    return this->operator[](resid);
}

/** Convert this EditMol into the data necessary to build a Molecule
    (thus committing the changes in this EditMol back to a Molecule) */
detail::MolData EditMol::commit() const
{
    return d->commit();
}

/** Return a copy of the CutGroup with ID == cgid

    \throw SireMol::missing_cutgroup
*/
CutGroup EditMol::at(CutGroupID cgid) const
{
    return this->operator[](cgid);
}

/** Return a copy of the CutGroup with number 'cgnum'

    \throw SireMol::missing_cutgroup
*/
CutGroup EditMol::at(CutGroupNum cgnum) const
{
    return this->operator[](cgnum);
}

/** Return a copy of the atom at index 'atomid'

    \throw SireError::invalid_index
*/
Atom EditMol::at(AtomID atomid) const
{
    return this->operator[](atomid);
}

/** Return a copy of the atom at index 'resatomid'

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
Atom EditMol::at(const ResNumAtomID &resatomid) const
{
    return this->operator[](resatomid);
}

/** Return a copy of the atom at index 'resatomid'

    \throw SireError::invalid_index
*/
Atom EditMol::at(const ResIDAtomID &resatomid) const
{
    return this->operator[](resatomid);
}

/** Return a copy of the atom at index 'cgatomid'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
Atom EditMol::at(const CGAtomID &cgatomid) const
{
    return this->operator[](cgatomid);
}

/** Return a copy of the atom at index 'cgatomid'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
Atom EditMol::at(const CGNumAtomID &cgatomid) const
{
    return this->operator[](cgatomid);
}

/** Return a copy of the atom with AtomIndex 'atm'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
Atom EditMol::at(const AtomIndex &atm) const
{
    return this->operator[](atm);
}

/** Return the connectivity of this molecule */
MoleculeBonds EditMol::connectivity() const
{
    return d->connectivity();
}

/** Return the connectivity of the residue with number 'resnum'

    \throw SireMol::missing_residue
*/
ResidueBonds EditMol::connectivity(ResNum resnum) const
{
    return d->connectivity(resnum);
}

/** Return the connectivity of the residue at index 'resid'

    \throw SireError::invalid_index
*/
ResidueBonds EditMol::connectivity(ResID resid) const
{
    return d->connectivity( d->at(resid) );
}

/** Return a copy of the metainfo about the molecule */
MoleculeInfo EditMol::info() const
{
    return d->info();
}

/** Return array of copies of all of the CutGroups in the molecule,
    in CutGroupID order. */
QVector<CutGroup> EditMol::cutGroups() const
{
    return d->cutGroups();
}

/** Return copies of all of the CutGroups that contain atoms from the
    residue with number 'resnum', indexed by CutGroupID

    \throw SireMol::missing_residue
*/
QHash<CutGroupID,CutGroup> EditMol::cutGroupsByID(ResNum resnum) const
{
    QSet<CutGroupNum> cgnums = d->cutGroupNums(resnum);

    QHash<CutGroupID,CutGroup> cgroups;
    cgroups.reserve(cgnums.count());

    foreach (CutGroupNum cgnum, cgnums)
    {
        cgroups.insert( d->cutGroupID(cgnum), d->cutGroup(cgnum) );
    }

    return cgroups;
}

/** Return copies of all of the CutGroups that contain atoms from the
    residue with index 'resid', indexed by CutGroupID

    \throw SireError::invalid_index
*/
QHash<CutGroupID,CutGroup> EditMol::cutGroupsByID(ResID resid) const
{
    return this->cutGroupsByID( d->at(resid) );
}

/** Return copies of all of the CutGroups that contain atoms in the residue
    with number 'resnum', indexed by CutGroupNum

    \throw SireMol::missing_residue
*/
QHash<CutGroupNum,CutGroup> EditMol::cutGroupsByNum(ResNum resnum) const
{
    return d->cutGroups(resnum);
}

/** Return copies of all of the CutGroups that contain atoms in the residue
    with index 'resid', indexed by CutGroupNum

    \throw SireError::invalid_index
*/
QHash<CutGroupNum,CutGroup> EditMol::cutGroupsByNum(ResID resid) const
{
    return this->cutGroupsByNum( d->at(resid) );
}

/** Return copies of the CutGroups with the passed IDs

    \throw SireMol::missing_cutgroup
*/
QHash<CutGroupID,CutGroup> EditMol::cutGroups(const QSet<CutGroupID> &cgids) const
{
    QHash<CutGroupID,CutGroup> cgroups;
    cgroups.reserve(cgids.count());

    foreach (CutGroupID cgid, cgids)
    {
        cgroups.insert( cgid, d->cutGroup( d->at(cgid) ) );
    }

    return cgroups;
}

/** Return copies of the CutGroups whose numbers are in 'cgnums'

    \throw SireMol::missing_cutgroup
*/
QHash<CutGroupNum,CutGroup> EditMol::cutGroups(const QSet<CutGroupNum> &cgnums) const
{
    return d->cutGroups(cgnums);
}

/** Return copies of the CutGroups whose atoms are in the residues whose numbers
    are in 'resnums', indexed by CutGroupID

    \throw SireMol::missing_residue
*/
QHash<CutGroupID,CutGroup> EditMol::cutGroupsByID(const QSet<ResNum> &resnums) const
{
    QSet<CutGroupID> cgids;

    foreach (ResNum resnum, resnums)
    {
        QSet<CutGroupNum> cgnums = d->cutGroupNums(resnum);

        foreach (CutGroupNum cgnum, cgnums)
        {
            cgids.insert( d->cutGroupID(cgnum) );
        }
    }

    return this->cutGroups(cgids);
}

/** Return copies of the CutGroups whose atoms are in the residues whose indicies
    are in 'resids', indexed by CutGroupID

    \throw SireError::invalid_index
*/
QHash<CutGroupID,CutGroup> EditMol::cutGroupsByID(const QSet<ResID> &resids) const
{
    QSet<CutGroupID> cgids;

    foreach (ResID resid, resids)
    {
        QSet<CutGroupNum> cgnums = d->cutGroupNums( d->at(resid) );

        foreach (CutGroupNum cgnum, cgnums)
        {
            cgids.insert( d->cutGroupID(cgnum) );
        }
    }

    return this->cutGroups(cgids);
}

/** Return copies of the CutGroups whose atoms are in the residues whose numbers
    are in 'resnums', indexed by CutGroupNum

    \throw SireMol::missing_residue
*/
QHash<CutGroupNum,CutGroup> EditMol::cutGroupsByNum(const QSet<ResNum> &resnums) const
{
    QSet<CutGroupNum> cgnums;

    foreach (ResNum resnum, resnums)
    {
        cgnums += d->cutGroupNums(resnum);
    }

    return this->cutGroups(cgnums);
}

/** Return copies of the CutGroups whose atoms are in the residues whose indicies
    are in 'resids', indexed by CutGroupNum

    \throw SireError::invalid_index
*/
QHash<CutGroupNum,CutGroup> EditMol::cutGroupsByNum(const QSet<ResID> &resids) const
{
    QSet<CutGroupNum> cgnums;

    foreach (ResID resid, resids)
    {
        cgnums += d->cutGroupNums( d->at(resid) );
    }

    return this->cutGroups(cgnums);
}

/** Return a copy of the CutGroup with ID 'cgid'

    \throw SireMol::missing_cutgroup
*/
CutGroup EditMol::cutGroup(CutGroupID cgid) const
{
    return d->cutGroup( d->at(cgid) );
}

/** Return a copy of the CutGroup with number 'cgnum'

    \throw SireMol::missing_cutgroup
*/
CutGroup EditMol::cutGroup(CutGroupNum cgnum) const
{
    return d->cutGroup(cgnum);
}

/** Return array of copies of all of the CoordGroups in the molecule,
    in CutGroupID order. */
QVector<CoordGroup> EditMol::coordGroups() const
{
    return d->coordGroups();
}

/** Return copies of all of the CoordGroups that contain atoms from the
    residue with number 'resnum', indexed by CutGroupID

    \throw SireMol::missing_residue
*/
QHash<CutGroupID,CoordGroup> EditMol::coordGroupsByID(ResNum resnum) const
{
    QSet<CutGroupNum> cgnums = d->cutGroupNums(resnum);

    QHash<CutGroupID,CoordGroup> cgroups;
    cgroups.reserve(cgnums.count());

    foreach (CutGroupNum cgnum, cgnums)
    {
        cgroups.insert( d->cutGroupID(cgnum), d->coordGroup(cgnum) );
    }

    return cgroups;
}

/** Return copies of all of the CoordGroups that contain atoms from the
    residue with index 'resid', indexed by CutGroupID

    \throw SireError::invalid_index
*/
QHash<CutGroupID,CoordGroup> EditMol::coordGroupsByID(ResID resid) const
{
    return this->coordGroupsByID( d->at(resid) );
}

/** Return copies of all of the CoordGroups that contain atoms in the residue
    with number 'resnum', indexed by CutGroupNum

    \throw SireMol::missing_residue
*/
QHash<CutGroupNum,CoordGroup> EditMol::coordGroupsByNum(ResNum resnum) const
{
    return d->coordGroups(resnum);
}

/** Return copies of all of the CoordGroups that contain atoms in the residue
    with index 'resid', indexed by CutGroupNum

    \throw SireError::invalid_index
*/
QHash<CutGroupNum,CoordGroup> EditMol::coordGroupsByNum(ResID resid) const
{
    return this->coordGroupsByNum( d->at(resid) );
}

/** Return copies of the CoordGroups with the passed IDs

    \throw SireMol::missing_cutgroup
*/
QHash<CutGroupID,CoordGroup> EditMol::coordGroups(const QSet<CutGroupID> &cgids) const
{
    QHash<CutGroupID,CoordGroup> cgroups;
    cgroups.reserve(cgids.count());

    foreach (CutGroupID cgid, cgids)
    {
        cgroups.insert( cgid, d->coordGroup( d->at(cgid) ) );
    }

    return cgroups;
}

/** Return copies of the CoordGroups whose numbers are in 'cgnums'

    \throw SireMol::missing_cutgroup
*/
QHash<CutGroupNum,CoordGroup> EditMol::coordGroups(const QSet<CutGroupNum> &cgnums) const
{
    return d->coordGroups(cgnums);
}

/** Return copies of the CoordGroups whose atoms are in the residues whose numbers
    are in 'resnums', indexed by CutGroupID

    \throw SireMol::missing_residue
*/
QHash<CutGroupID,CoordGroup> EditMol::coordGroupsByID(const QSet<ResNum> &resnums) const
{
    QSet<CutGroupID> cgids;

    foreach (ResNum resnum, resnums)
    {
        QSet<CutGroupNum> cgnums = d->cutGroupNums(resnum);

        foreach (CutGroupNum cgnum, cgnums)
        {
            cgids.insert( d->cutGroupID(cgnum) );
        }
    }

    return this->coordGroups(cgids);
}

/** Return copies of the CoordGroups whose atoms are in the residues whose indicies
    are in 'resids', indexed by CutGroupID

    \throw SireError::invalid_index
*/
QHash<CutGroupID,CoordGroup> EditMol::coordGroupsByID(const QSet<ResID> &resids) const
{
    QSet<CutGroupID> cgids;

    foreach (ResID resid, resids)
    {
        QSet<CutGroupNum> cgnums = d->cutGroupNums( d->at(resid) );

        foreach (CutGroupNum cgnum, cgnums)
        {
            cgids.insert( d->cutGroupID(cgnum) );
        }
    }

    return this->coordGroups(cgids);
}

/** Return copies of the CoordGroups whose atoms are in the residues whose numbers
    are in 'resnums', indexed by CutGroupNum

    \throw SireMol::missing_residue
*/
QHash<CutGroupNum,CoordGroup> EditMol::coordGroupsByNum(const QSet<ResNum> &resnums) const
{
    QSet<CutGroupNum> cgnums;

    foreach (ResNum resnum, resnums)
    {
        cgnums += d->cutGroupNums(resnum);
    }

    return this->coordGroups(cgnums);
}

/** Return copies of the CoordGroups whose atoms are in the residues whose indicies
    are in 'resids', indexed by CutGroupNum

    \throw SireError::invalid_index
*/
QHash<CutGroupNum,CoordGroup> EditMol::coordGroupsByNum(const QSet<ResID> &resids) const
{
    QSet<CutGroupNum> cgnums;

    foreach (ResID resid, resids)
    {
        cgnums += d->cutGroupNums( d->at(resid) );
    }

    return this->coordGroups(cgnums);
}

/** Return a copy of the CoordGroup with ID 'cgid'

    \throw SireMol::missing_cutgroup
*/
CoordGroup EditMol::coordGroup(CutGroupID cgid) const
{
    return d->coordGroup( d->at(cgid) );
}

/** Return a copy of the CoordGroup with number 'cgnum'

    \throw SireMol::missing_cutgroup
*/
CoordGroup EditMol::coordGroup(CutGroupNum cgnum) const
{
    return d->coordGroup(cgnum);
}

/** Return a copy of the atom at index 'atomid'

    \throw SireError::invalid_index
*/
Atom EditMol::atom(AtomID atomid) const
{
    return this->operator[](atomid);
}

/** Return a copy of the atom at index 'atomid' in the CutGroup
    with ID == cgid

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
Atom EditMol::atom(CutGroupID cgid, AtomID atomid) const
{
    return this->operator[]( CGAtomID(cgid,atomid) );
}

/** Return a copy of the atom at index 'atomid' in the CutGroup
    with number 'cgnum'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
Atom EditMol::atom(CutGroupNum cgnum, AtomID atomid) const
{
    return this->operator[]( CGNumAtomID(cgnum,atomid) );
}

/** Return a copy of the atom at index 'cgatomid'

    \throw SireMol::missing_cutgroup
    \throw SireMol::missing_atom
*/
Atom EditMol::atom(const CGAtomID &cgatomid) const
{
    return this->operator[](cgatomid);
}

/** Return a copy of the atom at index 'cgatomid'

    \throw SireMol::missing_cutgroup
    \throw SireMol::missing_atom
*/
Atom EditMol::atom(const CGNumAtomID &cgatomid) const
{
    return this->operator[](cgatomid);
}

/** Return a copy of the atom at index 'atomid' in the
    residue with number 'resnum'

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
Atom EditMol::atom(ResNum resnum, AtomID atomid) const
{
    return this->operator[]( ResNumAtomID(resnum,atomid) );
}

/** Return a copy of the atom at index 'resatomid'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
Atom EditMol::atom(const ResNumAtomID &resatomid) const
{
    return this->operator[](resatomid);
}

/** Return a copy of the atom at index 'atomid' in the
    residue at index 'resid'

    \throw SireError::invalid_index
*/
Atom EditMol::atom(ResID resid, AtomID atomid) const
{
    return this->operator[]( ResIDAtomID(resid,atomid) );
}

/** Return a copy of the atom at index 'resatomid'

    \throw SireError::invalid_index
*/
Atom EditMol::atom(const ResIDAtomID &resatomid) const
{
    return this->operator[](resatomid);
}

/** Return a copy of the atom with AtomIndex 'atm'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
Atom EditMol::atom(const AtomIndex &atm) const
{
    return this->operator[](atm);
}

/** Return a copy of the atom with name 'atomname' in
    the residue with number 'resnum'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
Atom EditMol::atom(ResNum resnum, const QString &atomname) const
{
    return this->operator[]( AtomIndex(atomname,resnum) );
}

/** Return a copy of the coordinates of the atom at index 'atomid'

    \throw SireError::invalid_index
*/
Vector EditMol::coordinates(AtomID atomid) const
{
    return d->coordinates( d->at(atomid) );
}

/** Return a copy of the coordinates of the atom at index 'atomid'
    in the CutGroup with ID == cgid

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
Vector EditMol::coordinates(CutGroupID cgid, AtomID atomid) const
{
    return this->coordinates( CGAtomID(cgid,atomid) );
}

/** Return a copy of the coordinates of the atom at index 'atomid'
    in the CutGroup with number 'cgnum'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
Vector EditMol::coordinates(CutGroupNum cgnum, AtomID atomid) const
{
    return this->coordinates( CGNumAtomID(cgnum,atomid) );
}

/** Return a copy of the coordinates of the atom at index 'cgatomid'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
Vector EditMol::coordinates(const CGAtomID &cgatomid) const
{
    return d->coordinates( d->at(cgatomid) );
}

/** Return a copy of the coordinates of the atom at index 'cgatomid'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
Vector EditMol::coordinates(const CGNumAtomID &cgatomid) const
{
    return d->coordinates( d->at(cgatomid) );
}

/** Return a copy of the coordinates of the atom at index 'atomid'
    in the residue with number 'resnum'

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
Vector EditMol::coordinates(ResNum resnum, AtomID atomid) const
{
    return this->coordinates( ResNumAtomID(resnum,atomid) );
}

/** Return a copy of the coordinates of the atom at index 'resatomid'

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
Vector EditMol::coordinates(const ResNumAtomID &resatomid) const
{
    return d->coordinates( d->at(resatomid) );
}

/** Return a copy of the coordinates of the atom at index 'atomid'
    in the residue with index 'resid'

    \throw SireError::invalid_index
*/
Vector EditMol::coordinates(ResID resid, AtomID atomid) const
{
    return this->coordinates( ResIDAtomID(resid,atomid) );
}

/** Return a copy of the coordinates of the atom at index 'resatomid'

    \throw SireError::invalid_index
*/
Vector EditMol::coordinates(const ResIDAtomID &resatomid) const
{
    return d->coordinates( d->at(resatomid) );
}

/** Return a copy of the coordinates of the atom with AtomIndex 'atm'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
Vector EditMol::coordinates(const AtomIndex &atm) const
{
    return d->coordinates(atm);
}

/** Return a copy of the coordinates of the atom called 'atomname'
    in the residue with number 'resnum'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
Vector EditMol::coordinates(ResNum resnum, const QString &atomname) const
{
    return d->coordinates( AtomIndex(atomname,resnum) );
}

/** Return copies of all of the atoms in the molecule (in AtomID order) */
QVector<Atom> EditMol::atoms() const
{
    return d->atoms();
}

/** Return copies of the coordinates of all of the atoms in the molecule
    (in AtomID order) */
QVector<Vector> EditMol::coordinates() const
{
    return d->coordinates();
}

template<class T>
QHash<T,Atom> getAtoms(const EditMol &mol, const QSet<T> &idxs)
{
    QHash<T,Atom> atoms;
    atoms.reserve(idxs.count());

    for (typename QSet<T>::const_iterator it = idxs.begin();
         it != idxs.end();
         ++it)
    {
        atoms.insert( *it, mol.atom(*it) );
    }

    return atoms;
}

/** Return copies of the atoms whose indicies are in 'atomids'

    \throw SireError::invalid_index
*/
QHash<AtomID,Atom> EditMol::atoms(const QSet<AtomID> &atomids) const
{
    return getAtoms<AtomID>(*this, atomids);
}

/** Return copies of the atoms whose indicies are in 'cgatomids'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
QHash<CGAtomID,Atom> EditMol::atoms(const QSet<CGAtomID> &cgatomids) const
{
    return getAtoms<CGAtomID>(*this, cgatomids);
}

/** Return copies of the atoms whose indicies are in 'cgatomids'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
QHash<CGNumAtomID,Atom> EditMol::atoms(const QSet<CGNumAtomID> &cgatomids) const
{
    return getAtoms<CGNumAtomID>(*this, cgatomids);
}

/** Return copies of the atoms whose indicies are in 'resatomids'

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
QHash<ResNumAtomID,Atom> EditMol::atoms(const QSet<ResNumAtomID> &resatomids) const
{
    return getAtoms<ResNumAtomID>(*this, resatomids);
}

/** Return copies of the atoms whose indicies are in 'resatomids'

    \throw SireError::invalid_index
*/
QHash<ResIDAtomID,Atom> EditMol::atoms(const QSet<ResIDAtomID> &resatomids) const
{
    return getAtoms<ResIDAtomID>(*this, resatomids);
}

/** Return copies of the atoms whose AtomIndex objects are in 'atoms'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
QHash<AtomIndex,Atom> EditMol::atoms(const QSet<AtomIndex> &atoms) const
{
    return getAtoms<AtomIndex>(*this, atoms);
}

/** Return copies of the atoms in the CutGroup with number 'cgnum'
    (in AtomID order)

    \throw SireMol::missing_cutgroup
*/
QVector<Atom> EditMol::atoms(CutGroupNum cgnum) const
{
    return d->atoms(cgnum);
}

template<class T>
QHash< T,QVector<Atom> > getAtomArrays(const EditMol &mol, const QSet<T> &idxs)
{
    QHash< T,QVector<Atom> > atoms;
    atoms.reserve(idxs.count());

    for  (typename QSet<T>::const_iterator it = idxs.begin();
          it != idxs.end();
          ++it)
    {
        atoms.insert( *it, mol.atoms(*it) );
    }

    return atoms;
}

/** Return copies of the atoms for the CutGroups with numbers in 'cgnums'
    (in AtomID order)

    \throw SireMol::missing_cutgroup
*/
QHash< CutGroupNum,QVector<Atom> > EditMol::atoms(const QSet<CutGroupNum> &cgnums) const
{
    return getAtomArrays<CutGroupNum>(*this, cgnums);
}

/** Return copies of the atoms in the CutGroup with ID == cgid (in AtomID order)

    \throw SireMol::missing_cutgroup
*/
QVector<Atom> EditMol::atoms(CutGroupID cgid) const
{
    return this->atoms( d->at(cgid) );
}

/** Return copies of the atoms for the CutGroups with numbers in 'cgnums'
    (in AtomID order)

    \throw SireMol::missing_cutgroup
*/
QHash< CutGroupID,QVector<Atom> > EditMol::atoms(const QSet<CutGroupID> &cgids) const
{
    return getAtomArrays<CutGroupID>(*this, cgids);
}

/** Return copies of the atoms for the residue with number 'resnum'
    (in AtomID order)

    \throw SireMol::missing_residue
*/
QVector<Atom> EditMol::atoms(ResNum resnum) const
{
    return d->atoms(resnum);
}

/** Return copies of the atoms for the residues whose numbers are in
    'resnums' (in AtomID order)

    \throw SireMol::missing_residue
*/
QHash< ResNum,QVector<Atom> > EditMol::atoms(const QSet<ResNum> &resnums) const
{
    return getAtomArrays<ResNum>(*this, resnums);
}

/** Return copies of the atoms for the residue at index 'resid'
    (in AtomID order)

    \throw SireError::invalid_index
*/
QVector<Atom> EditMol::atoms(ResID resid) const
{
    return this->atoms( d->at(resid) );
}

/** Return copies of the atoms for the residues whose indicies are in
    'resids' (in AtomID order)

    \throw SireError::invalid_index
*/
QHash< ResID,QVector<Atom> > EditMol::atoms(const QSet<ResID> &resids) const
{
    return getAtomArrays<ResID>(*this, resids);
}

template<class T>
QHash<T,Vector> getCoords(const EditMol &mol, const QSet<T> &idxs)
{
    QHash<T,Vector> coords;
    coords.reserve(idxs.size());

    for (typename QSet<T>::const_iterator it = idxs.begin();
         it != idxs.end();
         ++it)
    {
        coords.insert( *it, mol.coordinates(*it) );
    }

    return coords;
}

/** Return copies of the coordinates of the atoms whose indicies are in 'atomids'

    \throw SireError::invalid_index
*/
QHash<AtomID,Vector> EditMol::coordinates(const QSet<AtomID> &atomids) const
{
    return getCoords<AtomID>(*this, atomids);
}

/** Return copies of the coordinates of the atoms whose indicies are in
    'cgatomids'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
QHash<CGAtomID,Vector> EditMol::coordinates(const QSet<CGAtomID> &cgatomids) const
{
    return getCoords<CGAtomID>(*this, cgatomids);
}

/** Return copies of the coordinates of the atoms whose indicies are in
    'cgatomids'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
QHash<CGNumAtomID,Vector> EditMol::coordinates(const QSet<CGNumAtomID> &cgatomids) const
{
    return getCoords<CGNumAtomID>(*this, cgatomids);
}

/** Return copies of the coordinates of the atoms whose indicies are in
    'resatomids'

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
QHash<ResNumAtomID,Vector> EditMol::coordinates(const QSet<ResNumAtomID> &resatomids) const
{
    return getCoords<ResNumAtomID>(*this, resatomids);
}

/** Return copies of the coordinates of the atoms whose indicies are in
    'resatomids'

    \throw SireError::invalid_index
*/
QHash<ResIDAtomID,Vector> EditMol::coordinates(const QSet<ResIDAtomID> &resatomids) const
{
    return getCoords<ResIDAtomID>(*this, resatomids);
}

/** Return copies of the coordinates of the atoms whose AtomIndex objects are in
    'atoms'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
QHash<AtomIndex,Vector> EditMol::coordinates(const QSet<AtomIndex> &atoms) const
{
    return getCoords<AtomIndex>(*this, atoms);
}

/** Return copies of the coordinates of all of the atoms in the CutGroup
    with number 'cgnum' (in AtomID order)

    \throw SireMol::missing_cutgroup
*/
QVector<Vector> EditMol::coordinates(CutGroupNum cgnum) const
{
    return d->coordinates(cgnum);
}

template<class T>
QHash< T,QVector<Vector> > getCoordArrays(const EditMol &mol, const QSet<T> &idxs)
{
    QHash< T,QVector<Vector> > coords;
    coords.reserve(idxs.count());

    for (typename QSet<T>::const_iterator it = idxs.begin();
         it != idxs.end();
         ++it)
    {
        coords.insert( *it, mol.coordinates(*it) );
    }

    return coords;
}

/** Return copies of the coordinates of the atoms that are in the
    CutGroups whose numbers are in 'cgnums' (in AtomID order)

    \throw SireMol::missing_cutgroup
*/
QHash< CutGroupNum,QVector<Vector> > EditMol::coordinates(
                                          const QSet<CutGroupNum> &cgnums) const
{
    return getCoordArrays<CutGroupNum>(*this, cgnums);
}

/** Return copies of the coordinates of all of the atoms in the CutGroup
    with ID == cgid (in AtomID order)

    \throw SireMol::missing_cutgroup
*/
QVector<Vector> EditMol::coordinates(CutGroupID cgid) const
{
    return this->coordinates( d->at(cgid) );
}

/** Return copies of the coordinates of the atoms that are in the
    CutGroups whose IDs are in 'cgnums' (in AtomID order)

    \throw SireMol::missing_cutgroup
*/
QHash< CutGroupID,QVector<Vector> > EditMol::coordinates(
                                          const QSet<CutGroupID> &cgids) const
{
    return getCoordArrays<CutGroupID>(*this, cgids);
}

/** Return copies of the coordinates of all of the atoms in the residue
    with number 'resnum' (in AtomID order)

    \throw SireMol::missing_residue
*/
QVector<Vector> EditMol::coordinates(ResNum resnum) const
{
    return d->coordinates(resnum);
}

/** Return copies of the coordinates of the atoms that are in the
    residues whose numbers are in 'resnums' (in AtomID order)

    \throw SireMol::missing_residue
*/
QHash< ResNum,QVector<Vector> > EditMol::coordinates(const QSet<ResNum> &resnums) const
{
    return getCoordArrays<ResNum>(*this, resnums);
}

/** Return copies of the coordinates of all of the atoms in the residue
    with index 'resid' (in AtomID order)

    \throw SireError::invalid_index
*/
QVector<Vector> EditMol::coordinates(ResID resid) const
{
    return this->coordinates( d->at(resid) );
}

/** Return copies of the coordinates of the atoms that are in the
    residues whose indicies are in 'resids' (in AtomID order)

    \throw SireError::invalid_index
*/
QHash< ResID,QVector<Vector> > EditMol::coordinates(const QSet<ResID> &resids) const
{
    return getCoordArrays<ResID>(*this, resids);
}

/** Return the name of the molecule */
QString EditMol::name() const
{
    return d->name();
}

/** Return the name of the residue with number 'resnum'

    \throw SireMol::missing_residue
*/
QString EditMol::residueName(ResNum resnum) const
{
    return d->residueName(resnum);
}

/** Return the name of the residue at index 'resid'

    \throw SireError::invalid_index
*/
QString EditMol::residueName(ResID resid) const
{
    return d->residueName( d->at(resid) );
}

/** Return the number of the residue at index 'resid'

    \throw SireError::invalid_index
*/
ResNum EditMol::residueNumber(ResID resid) const
{
    return d->residueNumber(resid);
}

/** Return the index of the residue with the number 'resnum'

    \throw SireMol::missing_residue
*/
ResID EditMol::residueID(ResNum resnum) const
{
    return d->residueID(resnum);
}

/** Return the number of the first residue that is called 'resname'

    \throw SireMol::missing_residue
*/
ResNum EditMol::residueNumber(const QString &resname) const
{
    const QStringList resnames = d->residueNames();

    for (ResID i(0); i<resnames.count(); ++i)
    {
        if (resnames[i] == resname)
            return d->at(i);
    }

    throw SireMol::missing_residue( QObject::tr(
              "There is no residue called \"%1\" in the molecule \"%2\"")
                  .arg(resname, this->name()), CODELOC );

    return ResNum(0);
}

/** Return whether or not this molecule is empty (has no atoms) */
bool EditMol::isEmpty() const
{
    return d->isEmpty();
}

/** Return whether or not the residue with number 'resnum'
    is empty (has no atoms)

    \throw SireMol::missing_residue
*/
bool EditMol::isEmpty(ResNum resnum) const
{
    return d->isEmpty(resnum);
}

/** Return whether or not the residue with index 'resid'
    is empty (has no atoms)

    \throw SireError::invalid_index
*/
bool EditMol::isEmpty(ResID resid) const
{
    return d->isEmpty( d->at(resid) );
}

/** Return whether or not the CutGroup with ID == cgid
    is empty

    \throw SireMol::missing_cutgroup
*/
bool EditMol::isEmpty(CutGroupID cgid) const
{
    return d->isEmpty( d->at(cgid) );
}

/** Return whether or not the CutGroup with number 'cgnum'
    is empty

    \throw SireMol::missing_cutgroup
*/
bool EditMol::isEmpty(CutGroupNum cgnum) const
{
    return d->isEmpty(cgnum);
}

/** Return the list of the numbers of the residues (in ResID order) */
QVector<ResNum> EditMol::residueNumbers() const
{
    return d->residueNumbers();
}

/** Return the list of the names of the residues (in ResID order) */
QStringList EditMol::residueNames() const
{
    return d->residueNames();
}

/** Return the numbers of the residues in this molecule that are
    called 'resnam' - this returns an empty list if there are
    residues with this name in this molecule. */
QVector<ResNum> EditMol::residueNumbers(const QString &resnam) const
{
    const QStringList resnames = d->residueNames();

    QVector<ResNum> resnums;

    for (ResID i(0); i<resnames.count(); ++i)
    {
        if (resnames[i] == resnam)
            resnums.append( d->at(i) );
    }

    return resnums;
}

/** Return the numbers of the residues that contain atoms
    that are in the CutGroup with ID == cgid

    \throw SireMol::missing_cutgroup
*/
QVector<ResNum> EditMol::residueNumbers(CutGroupID cgid) const
{
    return d->residueNumbers( d->at(cgid) );
}

/** Return the numbers of the residues that contain atoms
    that are in the CutGroup with number 'cgnum'

    \throw SireMol::missing_cutgroup
*/
QVector<ResNum> EditMol::residueNumbers(CutGroupNum cgnum) const
{
    return d->residueNumbers(cgnum);
}

/** Return copies of the residues that are bonded to the residue
    with number 'resnum', indexed by residue number

    \throw SireMol::missing_residue
*/
QHash<ResNum,EditRes> EditMol::residuesBondedTo(ResNum resnum) const
{
    QSet<ResNum> resnums = d->connectivity(resnum).bondedResidues();

    return this->residues(resnums);
}

/** Return copies of the residues that are bonded to the residue
    with index 'resid', indexed by residue number

    \throw SireError::invalid_index
*/
QHash<ResNum,EditRes> EditMol::residuesBondedTo(ResID resid) const
{
    return this->residuesBondedTo( d->at(resid) );
}

/** Return whether or not this molecule contains the CutGroup
    with ID == cgid */
bool EditMol::contains(CutGroupID cgid) const
{
    return d->contains(cgid);
}

/** Return whether or not this molecule contains the CutGroup
    with number 'cgnum' */
bool EditMol::contains(CutGroupNum cgnum) const
{
    return d->contains(cgnum);
}

/** Return whether or not this molecule contains the residue
    with number 'resnum' */
bool EditMol::contains(ResNum resnum) const
{
    return d->contains(resnum);
}

/** Return whether or not this molecule contains a residue
    at index 'resid' */
bool EditMol::contains(ResID resid) const
{
    return d->contains(resid);
}

/** Return whether or not this molecule contains an atom
    called 'atomname' in a residue with number 'resnum' */
bool EditMol::contains(ResNum resnum, const QString &atomname) const
{
    return this->contains( AtomIndex(atomname,resnum) );
}

/** Return whether or not this molecule contains the
    atom 'atm' */
bool EditMol::contains(const AtomIndex &atm) const
{
    return d->contains(atm);
}

/** Return whether or not this molecule contains an atom
    with index 'atomid' in a CutGroup with ID == cgid */
bool EditMol::contains(CutGroupID cgid, AtomID atomid) const
{
    return this->contains( CGAtomID(cgid,atomid) );
}

/** Return whether or not this molecule contains an atom
    with index 'atomid' in a CutGroup with number 'cgnum' */
bool EditMol::contains(CutGroupNum cgnum, AtomID atomid) const
{
    return this->contains( CGNumAtomID(cgnum,atomid) );
}

/** Return whether or not this molecule contains an atom
    with index 'cgatomid' */
bool EditMol::contains(const CGAtomID &cgatomid) const
{
    return d->contains(cgatomid);
}

/** Return whether or not this molecule contains an atom
    with index 'cgatomid' */
bool EditMol::contains(const CGNumAtomID &cgatomid) const
{
    return d->contains(cgatomid);
}

/** Return whether or not this molecule contains an atom
    with index 'atomid' in a residue with number 'resnum' */
bool EditMol::contains(ResNum resnum, AtomID atomid) const
{
    return this->contains( ResNumAtomID(resnum,atomid) );
}

/** Return whether or not this molecule contains an atom
    with index 'resatomid' */
bool EditMol::contains(const ResNumAtomID &resatomid) const
{
    return d->contains(resatomid);
}

/** Return whether or not this molecule contains an atom
    with index 'atomid' in a residue with index 'resid' */
bool EditMol::contains(ResID resid, AtomID atomid) const
{
    return this->contains( ResIDAtomID(resid,atomid) );
}

/** Return whether or not this molecule contains an atom
    with index 'resatomid' */
bool EditMol::contains(const ResIDAtomID &resatomid) const
{
    return d->contains(resatomid);
}

/** Return whether or not this molecule contains the bond
    'bond' */
bool EditMol::contains(const Bond &bond) const
{
    return connectivity().contains(bond);
}

/** Return the total number of atoms in this molecule */
int EditMol::nAtoms() const
{
    return d->nAtoms();
}

/** Return the total number of atoms in the residue with
    number 'resnum'

    \throw SireMol::missing_residue
*/
int EditMol::nAtoms(ResNum resnum) const
{
    return d->nAtoms(resnum);
}

/** Return the total number of atoms in the residue with
    index 'resid'

    \throw SireError::invalid_index
*/
int EditMol::nAtoms(ResID resid) const
{
    return d->nAtoms(d->at(resid));
}

/** Return the total number of atoms in the CutGroup
    with ID == cgid

    \throw SireMol::missing_cutgroup
*/
int EditMol::nAtoms(CutGroupID cgid) const
{
    return d->nAtoms(d->at(cgid));
}

/** Return the total number of atoms in the CutGroup
    with number 'cgnum'

    \throw SireMol::missing_cutgroup
*/
int EditMol::nAtoms(CutGroupNum cgnum) const
{
    return d->nAtoms(cgnum);
}

/** Return the total number of residues in this molecule */
int EditMol::nResidues() const
{
    return d->nResidues();
}

/** Return the total number of CutGroups in this molecule */
int EditMol::nCutGroups() const
{
    return d->nCutGroups();
}

/** Return the total number of bonds in this molecule */
int EditMol::nBonds() const
{
    return connectivity().nBonds();
}

/** Return the total number of bonds in the residue with number
    'resnum'

    \throw SireMol::missing_residue
*/
int EditMol::nBonds(ResNum resnum) const
{
    return connectivity(resnum).nBonds();
}

/** Return the total number of bonds in the residue at index
    'resid'

    \throw SireError::invalid_index
*/
int EditMol::nBonds(ResID resid) const
{
    return connectivity(resid).nBonds();
}

/** Return the total number of inter-residue bonds in this molecule */
int EditMol::nInterBonds() const
{
    return connectivity().nInterBonds();
}

/** Return the total number of inter-residue bonds involving
    the residue with number 'resnum'

    \throw SireMol::missing_residue
*/
int EditMol::nInterBonds(ResNum resnum) const
{
    return connectivity(resnum).nInterBonds();
}

/** Return the total number of inter-residue bonds involving
    the residue with index 'resid'

    \throw SireError::invalid_index
*/
int EditMol::nInterBonds(ResID resid) const
{
    return connectivity(resid).nInterBonds();
}

/** Return the total number of intra-residue bonds in this molecule */
int EditMol::nIntraBonds() const
{
    return connectivity().nIntraBonds();
}

/** Return the total number of intra-residue bonds involving
    the residue with number 'resnum'

    \throw SireMol::missing_residue
*/
int EditMol::nIntraBonds(ResNum resnum) const
{
    return connectivity(resnum).nIntraBonds();
}

/** Return the total number of intra-residue bonds involving
    the residue with index 'resid'

    \throw SireError::invalid_index
*/
int EditMol::nIntraBonds(ResID resid) const
{
    return connectivity(resid).nIntraBonds();
}

/** Return the list of the names of the atoms in the residue
    with number 'resnum' (in AtomID order)

    \throw SireMol::missing_residue
*/
QStringList EditMol::atomNames(ResNum resnum) const
{
    return d->atomNames(resnum);
}

/** Return the list of the names of the atoms in the
    residue at index 'resid' (in AtomID order)

    \throw SireError::invalid_index
*/
QStringList EditMol::atomNames(ResID resid) const
{
    return this->atomNames( d->at(resid) );
}

/** Return the geometric line representing the bond 'bnd'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
SireMaths::Line EditMol::bond(const Bond &bnd) const
{
    return SireMaths::Line( coordinates(bnd.atom0()),
                            coordinates(bnd.atom1()) );
}

/** Return the geometric triangle representing the angle 'ang'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
SireMaths::Triangle EditMol::angle(const SireMol::Angle &ang) const
{
    return SireMaths::Triangle( coordinates(ang.atom0()),
                                coordinates(ang.atom1()),
                                coordinates(ang.atom2()) );
}

/** Return the geometric torsion representing the dihedral 'dih'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
SireMaths::Torsion EditMol::dihedral(const Dihedral &dih) const
{
    return SireMaths::Torsion( coordinates(dih.atom0()),
                               coordinates(dih.atom1()),
                               coordinates(dih.atom2()),
                               coordinates(dih.atom3()) );

}

/** Return the geometric torsion representing the improper angle 'imp'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
SireMaths::Torsion EditMol::improper(const Improper &imp) const
{
    return SireMaths::Torsion( coordinates(imp.atom0()),
                               coordinates(imp.atom1()),
                               coordinates(imp.atom2()),
                               coordinates(imp.atom3()) );
}

/** Return the length of the bond 'bnd'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
double EditMol::measure(const Bond &bnd) const
{
    return bond(bnd).length();
}

/** Return the size of the angle 'ang'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
SireMaths::Angle EditMol::measure(const SireMol::Angle &ang) const
{
    return angle(ang).angle();
}

/** Return the size of the dihedral 'dih'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
SireMaths::Angle EditMol::measure(const Dihedral &dih) const
{
    return dihedral(dih).angle();
}

/** Return the size of the improper angle 'imp'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
SireMaths::Angle EditMol::measure(const Improper &imp) const
{
    throw SireError::incomplete_code( QObject::tr("Need to write this!"), CODELOC );
    return 0.0;
}

/** Return the relative weights of the two groups 'group0' and 'group1',
    using the weighting function 'weightfunc'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
double EditMol::getWeight(const AtomIDGroup &group0, const AtomIDGroup &group1,
                          const WeightFunction &weightfunc) const
{
    return d->getWeight(group0, group1, weightfunc);
}

/** Set the name of this molecule to 'name' */
void EditMol::setName(const QString &name)
{
    d->setName(name);
}

/** Set the name of the residue with number 'resnum' to
    'name'

    \throw SireMol::missing_residue
*/
void EditMol::setResidueName(ResNum resnum, const QString &name)
{
    d->setResidueName(resnum, name);
}

/** Clean the molecule - this removes all empty residues and CutGroups */
void EditMol::clean()
{
    d->clean();
}

/** Renumber the residue with number 'oldnum' to 'newnum'

    \throw SireMol::missing_residue
    \throw SireMol::duplicate_residue
*/
void EditMol::renumberResidue(ResNum oldnum, ResNum newnum)
{
    d->renumberResidue(oldnum, newnum);
}

/** Add the atom 'atom' to this molecule.

    \throw SireMol::duplicate_atom
*/
void EditMol::add(const Atom &atom)
{
    d->add(atom);
}

/** Add the atom 'atom' to the molecule, and place
    it into the CutGroup with number 'cgnum'

    \throw SireMol::duplicate_atom
*/
void EditMol::add(const Atom &atom, CutGroupNum cgnum)
{
    d->add(atom, cgnum);
}

/** Remove the atom 'atom'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void EditMol::remove(const AtomIndex &atom)
{
    d->remove(atom);
}

/** Add the bond 'bond'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void EditMol::add(const Bond &bond)
{
    d->add(bond);
}

/** Remove the bond 'bond'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void EditMol::remove(const Bond &bond)
{
    d->remove(bond);
}

/** Remove all bonds from the residue with number 'resnum'

    \throw SireMol::missing_residue
*/
void EditMol::removeAllBonds(ResNum resnum)
{
    d->removeAllBonds(resnum);
}

/** Remove all bonds from the residue with number 'resid'

    \throw SireError::invalid_index
*/
void EditMol::removeAllBonds(ResID resid)
{
    this->removeAllBonds( d->at(resid) );
}

/** Remove all bonds that involve the atom 'atom'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void EditMol::removeAllBonds(const AtomIndex &atom)
{
    d->removeAllBonds(atom);
}

/** Remove all bonds from the molecule */
void EditMol::removeAllBonds()
{
    d->removeAllBonds();
}

/** Remove all intra-residue bonds from the residue with
    number 'resnum'

    \throw SireMol::missing_residue
*/
void EditMol::removeAllIntraBonds(ResNum resnum)
{
    d->removeAllIntraBonds(resnum);
}

/** Remove all intra-residue bonds from the residue at
    index 'resid'

    \throw SireError::invalid_index
*/
void EditMol::removeAllIntraBonds(ResID resid)
{
    this->removeAllIntraBonds( d->at(resid) );
}

/** Remove all intra-residue bonds that involve the atom 'atom'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void EditMol::removeAllIntraBonds(const AtomIndex &atom)
{
    d->removeAllIntraBonds(atom);
}

/** Remove all intra-residue bonds from this molecule */
void EditMol::removeAllIntraBonds()
{
    d->removeAllIntraBonds();
}

/** Remove all inter-residue bonds from the residue with
    number 'resnum'

    \throw SireMol::missing_residue
*/
void EditMol::removeAllInterBonds(ResNum resnum)
{
    d->removeAllInterBonds(resnum);
}

/** Remove all inter-residue bonds from the residue at
    index 'resid'

    \throw SireError::invalid_index
*/
void EditMol::removeAllInterBonds(ResID resid)
{
    this->removeAllInterBonds( d->at(resid) );
}

/** Remove all inter-residue bonds that involve the atom 'atom'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void EditMol::removeAllInterBonds(const AtomIndex &atom)
{
    d->removeAllInterBonds(atom);
}

/** Remove all inter-residue bonds from this molecule */
void EditMol::removeAllInterBonds()
{
    d->removeAllInterBonds();
}

/** Automatically add bonds to this molecule based on the
    vdw radii of the atoms in this molecule. */
void EditMol::addAutoBonds()
{
    d->addAutoBonds();
}

/** Automatically add bonds to this molecule using the
    bond adding function 'bondfunc' */
void EditMol::addAutoBonds(BondAddingFunction &bondfunc)
{
    d->addAutoBonds(bondfunc);
}

/** Add the residue with number 'resnum' and called 'resnam' to
    this molecule.

    \throw SireMol::dupilicate_residue
*/
void EditMol::add(ResNum resnum, const QString &resnam)
{
    d->add(resnum, resnam);
}

/** Remove the residue with number 'resnum'

    \throw SireMol::missing_residue
*/
void EditMol::remove(ResNum resnum)
{
    d->remove(resnum);
}

/** Remove the residue at index 'resid'

    \throw SireError::invalid_index
*/
void EditMol::remove(ResID resid)
{
    d->remove(d->at(resid));
}

/** Apply the residue 'tmpl' as a template for the residue with number
    'resnum', using the templatefunction 'tmplfunc'

    \throw SireMol::missing_residue
*/
void EditMol::applyTemplate(const EditRes &tmpl, ResNum resnum,
                            const TemplateFunction &tmplfunc)
{
    d->applyTemplate(tmpl, resnum, tmplfunc);
}

/** Apply the residue 'tmpl' as a template for the residue with index
    'resid', using the template function 'tmplfunc'

    \throw SireError::invalid_index
*/
void EditMol::applyTemplate(const EditRes &tmpl, ResID resid,
                            const TemplateFunction &tmplfunc)
{
    this->applyTemplate(tmpl, d->at(resid), tmplfunc);
}

/** Apply the molecule 'tmpl' as a template for this whole molecule, using
    the template function 'tmplfunc' */
void EditMol::applyTemplate(const EditMol &tmpl, const TemplateFunction &tmplfunc)
{
    d->applyTemplate(tmpl, tmplfunc);
}

/** Translate the entire molecule by 'delta' */
void EditMol::translate(const Vector &delta)
{
    d->translate(delta);
}

/** Translate the group 'group' by 'delta'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void EditMol::translate(const AtomIDGroup &group, const Vector &delta)
{
    d->translate(group, delta);
}

/** Translate the atom 'atom' by 'delta'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void EditMol::translate(const AtomIndex &atom, const Vector &delta)
{
    d->translate(atom, delta);
}

/** Translate the atoms 'atoms' by 'delta'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void EditMol::translate(const QSet<AtomIndex> &atoms, const Vector &delta)
{
    d->translate(atoms, delta);
}

/** Translate the atoms whose names are in 'atoms' that are in the residue
    with number 'resnum' by 'delta'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void EditMol::translate(ResNum resnum, const QStringList &atoms, const Vector &delta)
{
    d->translate(resnum, atoms, delta);
}

/** Translate all of the atoms in the residue with number 'resnum' by 'delta'

    \throw SireMol::missing_residue
*/
void EditMol::translate(ResNum resnum, const Vector &delta)
{
    d->translate(resnum, delta);
}

/** Translate all of the atoms whose residue numbers are in 'resnums'
    by 'delta'

    \throw SireMol::missing_residue
*/
void EditMol::translate(const QSet<ResNum> &resnums, const Vector &delta)
{
    d->translate(resnums, delta);
}

/** Translate the atoms whose names are in 'atoms' and that are in the
    residue at index 'resid' by 'delta'

    \throw SireError::invalid_index
    \throw SireMol::missing_atom
*/
void EditMol::translate(ResID resid, const QStringList &atoms, const Vector &delta)
{
    this->translate( d->at(resid), atoms, delta );
}

/** Translate all of the atoms in the residue at index 'resid' by 'delta'

    \throw SireError::invalid_index
*/
void EditMol::translate(ResID resid, const Vector &delta)
{
    this->translate( d->at(resid), delta );
}

/** Translate all of the atoms in the residues whose indicies are in
    'resids' by 'delta'

    \throw SireError::invalid_index
*/
void EditMol::translate(const QSet<ResID> &resids, const Vector &delta)
{
    QSet<ResNum> resnums;
    resnums.reserve(resids.count());

    foreach (ResID resid, resids)
    {
        resnums.insert( d->at(resid) );
    }

    this->translate(resnums, delta);
}

/** Translate all of the atoms in the CutGroup with number 'cgnum'
    by 'delta'

    \throw SireMol::missing_cutgroup
*/
void EditMol::translate(CutGroupNum cgnum, const Vector &delta)
{
    d->translate(cgnum, delta);
}

/** Translate all of the atoms in the CutGroups whose numbers are
    in 'cgnums' by 'delta'

    \throw SireMol::missing_cutgroup
*/
void EditMol::translate(const QSet<CutGroupNum> &cgnums, const Vector &delta)
{
    d->translate(cgnums, delta);
}

/** Translate all of the atoms in the CutGroup with ID == cgid
    by 'delta'

    \throw SireMol::missing_cutgroup
*/
void EditMol::translate(CutGroupID cgid, const Vector &delta)
{
    this->translate( d->at(cgid), delta );
}

/** Translate all of the atoms in the CutGroups whose IDs are in
    'cgids' by 'delta'

    \throw SireMol::missing_cutgroup
*/
void EditMol::translate(const QSet<CutGroupID> &cgids, const Vector &delta)
{
    QSet<CutGroupNum> cgnums;
    cgnums.reserve(cgids.count());

    foreach (CutGroupID cgid, cgids)
    {
        cgnums.insert( d->at(cgid) );
    }

    this->translate(cgnums, delta);
}

/** Rotate the entire molecule using the quaternion 'quat' about the point 'point' */
void EditMol::rotate(const Quaternion &quat, const Vector &point)
{
    d->rotate(quat, point);
}

/** Rotate the group 'group' using the quaternion 'quat' about the point 'point'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void EditMol::rotate(const AtomIDGroup &group, const Quaternion &quat, const Vector &point)
{
    d->rotate(group, quat, point);
}

/** Rotate the atom 'atom' using the quaternion 'quat' about the point 'point'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void EditMol::rotate(const AtomIndex &atom, const Quaternion &quat, const Vector &point)
{
    d->rotate(atom, quat, point);
}

/** Rotate the atoms in 'atoms' using the quaternion 'quat' about the point 'point'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void EditMol::rotate(const QSet<AtomIndex> &atoms, const Quaternion &quat,
                     const Vector &point)
{
    d->rotate(atoms, quat, point);
}

/** Rotate the atoms whose names are in 'atoms', that are in the residue
    with number 'resnum' using the quaternion 'quat' about the point 'point'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void EditMol::rotate(ResNum resnum, const QStringList &atoms, const Quaternion &quat,
                     const Vector &point)
{
    d->rotate(resnum, atoms, quat, point);
}

/** Rotate all of the atoms in the residue with number 'resnum'
    using the quaternion 'quat' about the point 'point'

    \throw SireMol::missing_residue
*/
void EditMol::rotate(ResNum resnum, const Quaternion &quat, const Vector &point)
{
    d->rotate(resnum, quat, point);
}

/** Rotate all of the atoms that are in the residues whose numbers are
    in 'resnums' using the quaternion 'quat' about the point 'point'

    \throw SireMol::missing_residue
*/
void EditMol::rotate(const QSet<ResNum> &resnums, const Quaternion &quat,
                     const Vector &point)
{
    d->rotate(resnums, quat, point);
}

/** Rotate the atoms whose names are in 'atoms' and that are in the
    residue at index 'resid' using the quaternion 'quat' about the point 'point'

    \throw SireError::invalid_index
    \throw SireMol::missing_atom
*/
void EditMol::rotate(ResID resid, const QStringList &atoms, const Quaternion &quat,
                     const Vector &point)
{
    this->rotate( d->at(resid), atoms, quat, point );
}

/** Rotate all of the atoms of the residue at index 'resid'
    using the quaternion 'quat' about the point 'point'

    \throw SireError::invalid_index
*/
void EditMol::rotate(ResID resid, const Quaternion &quat, const Vector &point)
{
    this->rotate( d->at(resid), quat, point );
}

/** Rotate all of the atoms in the residues whose indicies are in 'resids'
    using the quaternion 'quat' about the point 'point'

    \throw SireError::invalid_index
*/
void EditMol::rotate(const QSet<ResID> &resids, const Quaternion &quat,
                     const Vector &point)
{
    QSet<ResNum> resnums;
    resnums.reserve(resids.count());

    foreach (ResID resid, resids)
    {
        resnums.insert( d->at(resid) );
    }

    this->rotate(resnums, quat, point);
}

/** Rotate all of the atoms in the CutGroup with number 'cgnum'
    using the quaternion 'quat' about the point 'point'

    \throw SireMol::missing_cutgroup
*/
void EditMol::rotate(CutGroupNum cgnum, const Quaternion &quat, const Vector &point)
{
    d->rotate(cgnum, quat, point);
}

/** Rotate all of the atoms in the CutGroups whose numbers are in
    'cgnums' using the quaternion 'quat' about the point 'point'

    \throw SireMol::missing_cutgroup
*/
void EditMol::rotate(const QSet<CutGroupNum> &cgnums, const Quaternion &quat,
                     const Vector &point)
{
    d->rotate(cgnums, quat, point);
}

/** Rotate all of the atoms in the CutGroup with ID == cgid
    using the quaternion 'quat' about the point 'point'

    \throw SireMol::missing_cutgroup
*/
void EditMol::rotate(CutGroupID cgid, const Quaternion &quat, const Vector &point)
{
    this->rotate( d->at(cgid), quat, point );
}

/** Rotate all of the atoms in the CutGroups whose IDs are in 'cgids'
    using the quaternion 'quat' about the point 'point'

    \throw SireMol::missing_cutgroup
*/
void EditMol::rotate(const QSet<CutGroupID> &cgids, const Quaternion &quat,
                     const Vector &point)
{
    QSet<CutGroupNum> cgnums;
    cgnums.reserve(cgids.count());

    foreach (CutGroupID cgid, cgids)
    {
        cgnums.insert( d->at(cgid) );
    }

    this->rotate(cgnums, quat, point);
}

/** Rotate the entire molecule using the matrix 'matrix' about the point 'point' */
void EditMol::rotate(const Matrix &matrix, const Vector &point)
{
    d->rotate(matrix, point);
}

/** Rotate the group 'group' using the matrix 'matrix' about the point 'point'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void EditMol::rotate(const AtomIDGroup &group, const Matrix &matrix, const Vector &point)
{
    d->rotate(group, matrix, point);
}

/** Rotate the atom 'atom' using the matrix 'matrix' about the point 'point'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void EditMol::rotate(const AtomIndex &atom, const Matrix &matrix, const Vector &point)
{
    d->rotate(atom, matrix, point);
}

/** Rotate the atoms in 'atoms' using the matrix 'matrix' about the point 'point'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void EditMol::rotate(const QSet<AtomIndex> &atoms, const Matrix &matrix,
                     const Vector &point)
{
    d->rotate(atoms, matrix, point);
}

/** Rotate the atoms whose names are in 'atoms', that are in the residue
    with number 'resnum' using the matrix 'matrix' about the point 'point'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void EditMol::rotate(ResNum resnum, const QStringList &atoms, const Matrix &matrix,
                     const Vector &point)
{
    d->rotate(resnum, atoms, matrix, point);
}

/** Rotate all of the atoms in the residue with number 'resnum'
    using the matrix 'matrix' about the point 'point'

    \throw SireMol::missing_residue
*/
void EditMol::rotate(ResNum resnum, const Matrix &matrix, const Vector &point)
{
    d->rotate(resnum, matrix, point);
}

/** Rotate all of the atoms that are in the residues whose numbers are
    in 'resnums' using the matrix 'matrix' about the point 'point'

    \throw SireMol::missing_residue
*/
void EditMol::rotate(const QSet<ResNum> &resnums, const Matrix &matrix,
                     const Vector &point)
{
    d->rotate(resnums, matrix, point);
}

/** Rotate the atoms whose names are in 'atoms' and that are in the
    residue at index 'resid' using the matrix 'matrix' about the point 'point'

    \throw SireError::invalid_index
    \throw SireMol::missing_atom
*/
void EditMol::rotate(ResID resid, const QStringList &atoms, const Matrix &matrix,
                     const Vector &point)
{
    this->rotate( d->at(resid), atoms, matrix, point );
}

/** Rotate all of the atoms of the residue at index 'resid'
    using the matrix 'matrix' about the point 'point'

    \throw SireError::invalid_index
*/
void EditMol::rotate(ResID resid, const Matrix &matrix, const Vector &point)
{
    this->rotate( d->at(resid), matrix, point );
}

/** Rotate all of the atoms in the residues whose indicies are in 'resids'
    using the matrix 'matrix' about the point 'point'

    \throw SireError::invalid_index
*/
void EditMol::rotate(const QSet<ResID> &resids, const Matrix &matrix,
                     const Vector &point)
{
    QSet<ResNum> resnums;
    resnums.reserve(resids.count());

    foreach (ResID resid, resids)
    {
        resnums.insert( d->at(resid) );
    }

    this->rotate(resnums, matrix, point);
}

/** Rotate all of the atoms in the CutGroup with number 'cgnum'
    using the matrix 'matrix' about the point 'point'

    \throw SireMol::missing_cutgroup
*/
void EditMol::rotate(CutGroupNum cgnum, const Matrix &matrix, const Vector &point)
{
    d->rotate(cgnum, matrix, point);
}

/** Rotate all of the atoms in the CutGroups whose numbers are in
    'cgnums' using the matrix 'matrix' about the point 'point'

    \throw SireMol::missing_cutgroup
*/
void EditMol::rotate(const QSet<CutGroupNum> &cgnums, const Matrix &matrix,
                     const Vector &point)
{
    d->rotate(cgnums, matrix, point);
}

/** Rotate all of the atoms in the CutGroup with ID == cgid
    using the matrix 'matrix' about the point 'point'

    \throw SireMol::missing_cutgroup
*/
void EditMol::rotate(CutGroupID cgid, const Matrix &matrix, const Vector &point)
{
    this->rotate( d->at(cgid), matrix, point );
}

/** Rotate all of the atoms in the CutGroups whose IDs are in 'cgids'
    using the matrix 'matrix' about the point 'point'

    \throw SireMol::missing_cutgroup
*/
void EditMol::rotate(const QSet<CutGroupID> &cgids, const Matrix &matrix,
                     const Vector &point)
{
    QSet<CutGroupNum> cgnums;
    cgnums.reserve(cgids.count());

    foreach (CutGroupID cgid, cgids)
    {
        cgnums.insert( d->at(cgid) );
    }

    this->rotate(cgnums, matrix, point);
}

/** Set the coordinates of the atoms in the CutGroup with number 'cgnum'
    to 'newcoords'

    \throw SireMol::missing_cutgroup
    \throw SireError::incompatible_error
*/
void EditMol::setCoordinates(CutGroupNum cgnum, const CoordGroup &newcoords)
{
    d->setCoordinates(cgnum, newcoords);
}

/** Set the coordinates of the specified CutGroups to the values
    in 'newcoords'

    \throw SireMol::missing_cutgroup
    \throw SireError::incompatible_error
*/
void EditMol::setCoordinates(const QHash<CutGroupNum,CoordGroup> &newcoords)
{
    d->setCoordinates(newcoords);
}

/** Set the coordinates of the atoms in the CutGroup with ID == cgid
    to 'newcoords'

    \throw SireMol::missing_cutgroup
    \throw SireError::incompatible_error
*/
void EditMol::setCoordinates(CutGroupID cgid, const CoordGroup &newcoords)
{
    this->setCoordinates( d->at(cgid), newcoords );
}

template<class Num, class Idx, class T>
QHash<Num,T> convertToNum(const QSharedDataPointer<EditMolData> &d,
                          const QHash<Idx,T> &coords)
{
    QHash<Num,T> convertedcoords;
    convertedcoords.reserve(coords.size());

    for (typename QHash<Idx,T>::const_iterator it = coords.begin();
         it != coords.end();
         ++it)
    {
        convertedcoords.insert( d->at(it.key()), it.value() );
    }

    return convertedcoords;
}

/** Set the coordinates of the specified CutGroups to the values
    in 'newcoords'

    \throw SireMol::missing_cutgroup
    \throw SireError::incompatible_error
*/
void EditMol::setCoordinates(const QHash<CutGroupID,CoordGroup> &newcoords)
{
    this->setCoordinates( convertToNum<CutGroupNum,CutGroupID,CoordGroup>(d, newcoords) );
}

/** Set the coordinates of the entire molecule to 'newcoords'

    \throw SireError::incompatible_error
*/
void EditMol::setCoordinates(const QVector<Vector> &newcoords)
{
    d->setCoordinates(newcoords);
}

/** Set the coordinates of the atoms in the CutGroup with number
    'cgnum' to 'newcoords'

    \throw SireMol::missing_cutgroup
    \throw SireError::incompatible_error
*/
void EditMol::setCoordinates(CutGroupNum cgnum, const QVector<Vector> &newcoords)
{
    d->setCoordinates(cgnum, newcoords);
}

/** Set the coordinates of the atoms in the specified CutGroups to
    'newcoords'

    \throw SireMol::missing_cutgroup
    \throw SireError::incompatible_error
*/
void EditMol::setCoordinates(const QHash< CutGroupNum,QVector<Vector> > &newcoords)
{
    d->setCoordinates( newcoords );
}

/** Set the coordinates of the atoms in the CutGroup with ID == cgid
    to 'newcoords'

    \throw SireMol::missing_cutgroup
    \throw SireError::incompatible_error
*/
void EditMol::setCoordinates(CutGroupID cgid, const QVector<Vector> &newcoords)
{
    this->setCoordinates( d->at(cgid), newcoords );
}

/** Set the coordinates of the atoms in the specified CutGroups to
    'newcoords'

    \throw SireMol::missing_cutgroup
    \throw SireError::incompatible_error
*/
void EditMol::setCoordinates(const QHash< CutGroupID,QVector<Vector> > &newcoords)
{
    this->setCoordinates(
              convertToNum< CutGroupNum,CutGroupID,QVector<Vector> >(d, newcoords)
                        );
}

/** Set the coordinates of all of the atoms in the residue with number 'resnum'
    to 'newcoords'

    \throw SireMol::missing_residue
    \throw SireError::incompatible_error
*/
void EditMol::setCoordinates(ResNum resnum, const QVector<Vector> &newcoords)
{
    d->setCoordinates(resnum, newcoords);
}

/** Set the coordinates of all of the atoms in the specified residues to 'newcoords'

    \throw SireMol::missing_residue
    \throw SireError::incompatible_error
*/
void EditMol::setCoordinates(const QHash< ResNum,QVector<Vector> > &newcoords)
{
    d->setCoordinates( newcoords );
}

/** Set the coordinates of all of the atoms in the residue at index 'resid'
    to 'newcoords'

    \throw SireError::invalid_index
    \throw SireError::incompatible_error
*/
void EditMol::setCoordinates(ResID resid, const QVector<Vector> &newcoords)
{
    this->setCoordinates( d->at(resid), newcoords );
}

/** Set the coordiates of all of the atoms in the specified residues
    to 'newcoords'

    \throw SireError::invalid_index
    \throw SireError::incompatible_error
*/
void EditMol::setCoordinates(const QHash< ResID,QVector<Vector> > &newcoords)
{
    this->setCoordinates( convertToNum< ResNum,ResID,QVector<Vector> >(d, newcoords) );
}

/** Set the coordinates of the atom 'atom' to 'newcoords'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void EditMol::setCoordinates(const AtomIndex &atom, const Vector &newcoords)
{
    d->setCoordinates(atom, newcoords);
}

/** Set the coordinates for the specified atoms to 'newcoords'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
void EditMol::setCoordinates(const QHash<AtomIndex,Vector> &newcoords)
{
    d->setCoordinates(newcoords);
}

/** Set the coordinates of the atom with index 'cgatomid' to 'newcoords'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
void EditMol::setCoordinates(const CGNumAtomID &cgatomid, const Vector &newcoords)
{
    d->setCoordinates(cgatomid, newcoords);
}

/** Set the coordinates of the specified atoms to 'newcoords'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
void EditMol::setCoordinates(const QHash<CGNumAtomID,Vector> &newcoords)
{
    d->setCoordinates(newcoords);
}

/** Set the coordinates of the atom at index 'cgatomid' to 'newcoords'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
void EditMol::setCoordinates(const CGAtomID &cgatomid, const Vector &newcoords)
{
    d->setCoordinates(cgatomid, newcoords);
}

/** Set the coordinates of the specified atoms to 'newcoords'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
void EditMol::setCoordinates(const QHash<CGAtomID,Vector> &newcoords)
{
    d->setCoordinates(newcoords);
}

/** Set the coordinates of the atom at index 'resatomid' to 'newcoords'

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
void EditMol::setCoordinates(const ResNumAtomID &resatomid, const Vector &newcoords)
{
    d->setCoordinates(resatomid, newcoords);
}

/** Set the coordinates of the specified atoms to 'newcoords'

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
void EditMol::setCoordinates(const QHash<ResNumAtomID,Vector> &newcoords)
{
    d->setCoordinates(newcoords);
}

/** Set the coordinates of the atom at index 'resatomid' to 'newcoords'

    \throw SireError::invalid_index
*/
void EditMol::setCoordinates(const ResIDAtomID &resatomid, const Vector &newcoords)
{
    d->setCoordinates(resatomid, newcoords);
}

/** Set the coordinates of the specified atoms to 'newcoords'

    \throw SireError::invalid_index
*/
void EditMol::setCoordinates(const QHash<ResIDAtomID,Vector> &newcoords)
{
    d->setCoordinates(newcoords);
}

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
void EditMol::change(const Bond &bnd, double delta,
                     const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors)
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
void EditMol::change(const Bond &bond, double delta, const QSet<AtomIndex> &anchors)
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
void EditMol::change(const SireMol::Angle &ang, const SireMaths::Angle &delta,
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
void EditMol::change(const SireMol::Angle &ang, const SireMaths::Angle &delta,
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
void EditMol::change(const Dihedral &dih, const SireMaths::Angle &delta,
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
void EditMol::change(const Dihedral &dih, const SireMaths::Angle &delta,
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
void EditMol::change(const Bond &bnd, const SireMaths::Angle &delta,
                     const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors)
{
    //split the molecule into two parts based on the bond
    boost::tuple<AtomIDGroup,AtomIDGroup> groups = splitMolecule(bnd, connectivity());

    AtomIDGroup group0 = groups.get<0>();
    AtomIDGroup group1 = groups.get<1>();

    d->change(bnd, delta, group0, group1, weightfunc, anchors);
}

/** Overload of the function that uses a default WeightFunction */
void EditMol::change(const Bond &dih, const SireMaths::Angle &delta, const QSet<AtomIndex> &anchors)
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
void EditMol::change(const Improper &improper, const SireMaths::Angle &delta,
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
void EditMol::change(const Improper &improper, const SireMaths::Angle &delta,
                     const QSet<AtomIndex> &anchors)
{
    change(improper, delta, RelFromMass(), anchors);
}

/** This function is similar to EditMol::changeBond, except that it sets the specified
    bond to a particular length

    \throw SireMol::missing_atom
    \throw SireMol::anchor_error
    \throw SireMol::ring_error
*/
void EditMol::set(const Bond &bnd, double val,
                  const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors)
{
    change(bnd, val-measure(bnd), weightfunc, anchors);
}

/** Overload of the function that uses a default weight function

    \throw SireMol::missing_atom
    \throw SireMol::anchor_error
    \throw SireMol::ring_error
*/
void EditMol::set(const Bond &bnd, double val, const QSet<AtomIndex> &anchors)
{
    change(bnd, val-measure(bnd), anchors);
}

/** This function is similar to EditMol::changeAngle, except that it set the specified angle
    to a particular size. Note that this will probably change the size of other angles, so
    if you wish to set a whole series of angles, then you will need to think carefully about
    the order in which you set them.

    \throw SireMol::missing_atom
    \throw SireMol::anchor_error
    \throw SireMol::ring_error
*/
void EditMol::set(const SireMol::Angle &ang, const SireMaths::Angle &val,
                  const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors)
{
    change(ang, val-measure(ang), weightfunc, anchors);
}

/** Overload of the function that uses a default weight function

    \throw SireMol::missing_atom
    \throw SireMol::anchor_error
    \throw SireMol::ring_error
*/
void EditMol::set(const SireMol::Angle &ang, const SireMaths::Angle &val,
                  const QSet<AtomIndex> &anchors)
{
    change(ang, val-measure(ang), anchors);
}

/** This function is similar to EditMol::changeDihedral, except that it sets the specified
    dihedral to a particular size. Note that this will probably change the size of other
    dihedrals, so think about the order in which you apply multiple calls of this function

    \throw SireMol::missing_atom
    \throw SireMol::anchor_error
    \throw SireMol::ring_error
*/
void EditMol::set(const Dihedral &dih, const SireMaths::Angle &val,
                  const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors)
{
    change(dih, val-measure(dih), weightfunc, anchors);
}

/** Overload of the function that uses a default weight function

    \throw SireMol::missing_atom
    \throw SireMol::anchor_error
    \throw SireMol::ring_error
*/
void EditMol::set(const Dihedral &dih, const SireMaths::Angle &val,
                  const QSet<AtomIndex> &anchors)
{
    change(dih, val-measure(dih), anchors);
}

/** This function is similar to EditMol::change(const Bond&), except that it sets the specified
    dihedral to a particular size.

    \throw SireMol::missing_atom
    \throw SireMol::anchor_error
    \throw SireMol::ring_error
*/
void EditMol::setAll(const Dihedral &dih, const SireMaths::Angle &val,
                     const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors)
{
    change(dih, val-measure(dih), weightfunc, anchors);
}

/** Overload of the function that uses a default weight function

    \throw SireMol::missing_atom
    \throw SireMol::anchor_error
    \throw SireMol::ring_error
*/
void EditMol::setAll(const Dihedral &dih, const SireMaths::Angle &val,
                     const QSet<AtomIndex> &anchors)
{
    change(dih, val-measure(dih), anchors);
}

/** This function is similar to EditMol::change(const Improper&), except that it
    sets the specified improper angle to a particular size.

    \throw SireMol::missing_atom
    \throw SireMol::anchor_error
    \throw SireMol::ring_error
*/
void EditMol::set(const Improper &improper, const SireMaths::Angle &size,
                  const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors)
{
    change(improper, size-measure(improper), weightfunc, anchors);
}

/** Overload of the function that uses a default weight function

    \throw SireMol::missing_atom
    \throw SireMol::anchor_error
    \throw SireMol::ring_error
*/
void EditMol::set(const Improper &improper, const SireMaths::Angle &size,
                  const QSet<AtomIndex> &anchors)
{
    change(improper, size-measure(improper), anchors);
}
