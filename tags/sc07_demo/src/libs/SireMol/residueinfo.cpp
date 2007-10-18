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

#include <QSharedData>
#include <QSet>
#include <QHash>
#include <QVector>

#include "residueinfo.h"

#include "atominfo.h"
#include "atominfogroup.h"

#include "resnum.h"
#include "resid.h"

#include "cgatomid.h"
#include "cgnumatomid.h"

#include "editmoldata.h"

#include "SireMol/errors.h"
#include "SireError/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireStream;

QDataStream& operator<<(QDataStream&, const SireMol::ResidueInfoPvt&);
QDataStream& operator>>(QDataStream&, SireMol::ResidueInfoPvt&);

namespace SireMol
{

/** Private implementation of the ResidueInfo class.

    @author Christopher Woods
*/
class ResidueInfoPvt : public QSharedData
{

friend QDataStream& ::operator<<(QDataStream&, const ResidueInfoPvt&);
friend QDataStream& ::operator>>(QDataStream&, ResidueInfoPvt&);

public:
    ResidueInfoPvt();

    ResidueInfoPvt(ResNum resnum, const EditMolData &moldata,
                   const QHash<CutGroupID,AtomInfoGroup> &atominfos);

    ResidueInfoPvt(const ResidueInfoPvt &other);

    ~ResidueInfoPvt();

    ResidueInfoPvt& operator=(const ResidueInfoPvt &other);

    bool operator==(const ResidueInfoPvt &other) const;
    bool operator!=(const ResidueInfoPvt &other) const;

    const AtomInfo& _unsafe_atom(const CGAtomID &cgid) const;

    const CGAtomID& _unsafe_index(AtomID atomid) const;
    const CGAtomID& _unsafe_index(const QString &atomname) const;

    const AtomInfoGroup& _unsafe_atomInfoGroup(CutGroupID cgid) const;

    void assertAtomExists(AtomID atomid) const;
    void assertAtomExists(const CGAtomID &cgatomid) const;
    void assertAtomExists(const QString &atomname) const;

    void assertCutGroupExists(CutGroupID cgid) const;

    void assertNAtoms(int n) const;

    /** The residue's name */
    QString resname;

    /** The residue's number */
    ResNum resnum;

    /** Hash mapping atom names to residue-based indicies */
    QHash<QString,AtomID> atomname2atomid;

    /** The CGAtomID indicies of the atoms in the residue, in the
        order that they were added to the residue (AtomID order) */
    QVector<CGAtomID> cgidxs;

    /** The CutGroupIDs of all groups that contain atoms from this
        residue */
    QVector<CutGroupID> cgids;

    /** Metainfo for all of the atoms in this residue, indexed by CGAtomID */
    QHash<CutGroupID, AtomInfoGroup> atominfos;
};

}

Q_DECLARE_METATYPE(SireMol::ResidueInfoPvt);

using namespace SireMol;

static const RegisterMetaType<ResidueInfoPvt> r_respvt;

/** Serialise to a binary data stream */
QDataStream &operator<<(QDataStream &ds, const ResidueInfoPvt &resinfo)
{
    writeHeader(ds, r_respvt, 1)
            << resinfo.resname << resinfo.resnum << resinfo.cgidxs << resinfo.atominfos;

    //the remaining data can be regenerated :-)

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream &operator>>(QDataStream &ds, ResidueInfoPvt &resinfo)
{
    VersionID v = readHeader(ds, r_respvt);

    if (v == 1)
    {
        ResidueInfoPvt newinfo;

        ds >> newinfo.resname >> newinfo.resnum >> newinfo.cgidxs >> newinfo.atominfos;

        //rebuild other metadata
        int nats = newinfo.cgidxs.count();

        if (nats > 0)
        {
            newinfo.atomname2atomid.reserve(nats);
            QSet<CutGroupID> cgids;

            for (int i=0; i<nats; ++i)
            {
                const CGAtomID &cgatomid = newinfo.cgidxs.constData()[i];
                const AtomInfoGroup &atomgroup = newinfo.atominfos.
                                          constFind(cgatomid.cutGroupID()).value();

                const AtomInfo &atominfo = atomgroup.at(cgatomid.atomID());

                newinfo.atomname2atomid.insert( atominfo.name(), AtomID(i) );
                cgids.insert( cgatomid.cutGroupID() );
            }

            newinfo.cgids.reserve(cgids.count());

            for (QSet<CutGroupID>::const_iterator it = cgids.constBegin();
                 it != cgids.constEnd();
                 ++it)
            {
                newinfo.cgids.append(*it);
            }
        }

        resinfo = newinfo;
    }
    else
        throw version_error(v, "1", r_respvt, CODELOC);

    return ds;
}

///////////
/////////// Implementation of ResidueInfoPvt
///////////

/** Null constructor */
ResidueInfoPvt::ResidueInfoPvt() : QSharedData()
{}

/** Construct from the passed EditMolData and AtomInfos (arranged into CutGroups) */
ResidueInfoPvt::ResidueInfoPvt(ResNum rnum, const EditMolData &moldata,
                               const QHash<CutGroupID,AtomInfoGroup> &infogroups)
               : QSharedData(),
                 resname(moldata.residueName(rnum)), resnum(rnum),
                 atominfos(infogroups)
{
    int nats = moldata.nAtoms(resnum);

    atomname2atomid.reserve(nats);
    cgidxs.reserve(nats);

    //get the names of all of the atoms in the residue
    QStringList atomnames = moldata.atomNames(resnum);

    //add each atom name to the residue info (recording which CutGroup
    //they are in
    QSet<CutGroupID> atomcgroups;

    for (AtomID atomid(0); atomid < nats; ++atomid)
    {
        const QString &atomname = atomnames[atomid];
        atomname2atomid.insert(atomname, atomid);

        AtomIndex atom(atomname,resnum);

        CutGroupID cgid = moldata.cutGroupID(atom);
        atomcgroups.insert(cgid);

        cgidxs.append( CGAtomID(cgid, infogroups.find(cgid)->indexOf(atom)) );
    }

    cgids.reserve(atomcgroups.count());

    foreach (CutGroupID cgid, atomcgroups)
    {
        cgids.append(cgid);
    }
}

/** Copy constructor */
ResidueInfoPvt::ResidueInfoPvt(const ResidueInfoPvt &other)
               : QSharedData(),
                 resname(other.resname), resnum(other.resnum),
                 atomname2atomid(other.atomname2atomid),
                 cgidxs(other.cgidxs),
                 cgids(other.cgids), atominfos(other.atominfos)
{}

/** Destructor */
ResidueInfoPvt::~ResidueInfoPvt()
{}

/** Assignment operator */
ResidueInfoPvt& ResidueInfoPvt::operator=(const ResidueInfoPvt &other)
{
    if (&other != this)
    {
        resname = other.resname;
        resnum = other.resnum;
        atomname2atomid = other.atomname2atomid;
        cgidxs = other.cgidxs;
        cgids = other.cgids;
        atominfos = other.atominfos;
    }

    return *this;
}

/** Comparison operator */
bool ResidueInfoPvt::operator==(const ResidueInfoPvt &other) const
{
    return (this == &other) or
           (resname == other.resname and resnum == other.resnum and
            cgidxs == other.cgidxs and atomname2atomid == other.atomname2atomid);
}

/** Comparison operator */
bool ResidueInfoPvt::operator!=(const ResidueInfoPvt &other) const
{
    return (this != &other) and
           (resname != other.resname or resnum != other.resnum or
            cgidxs != other.cgidxs or atomname2atomid != other.atomname2atomid);
}

/** Return the AtomInfo of the atom at index 'cgatomid' - this does not
    check whether this is a valid index */
const AtomInfo& ResidueInfoPvt::_unsafe_atom(const CGAtomID &cgatomid) const
{
    return atominfos.find(cgatomid.cutGroupID()).value().constData()[cgatomid.atomID()];
}

/** Return the CGAtomID index of the atom with index 'AtomID' - this does
    not check whether this is a valid index */
const CGAtomID& ResidueInfoPvt::_unsafe_index(AtomID atomid) const
{
    return cgidxs.constData()[atomid];
}

/** Return the CGAtomID index of the atom with name 'atomname' - this
    does not check whether or not there is such an atom! */
const CGAtomID& ResidueInfoPvt::_unsafe_index(const QString &atomname) const
{
    return this->_unsafe_index( atomname2atomid.find(atomname).value() );
}

/** Return the AtomInfoGroup for the CutGroup with ID == cgid - this does not
    check whether or not this group exists */
const AtomInfoGroup& ResidueInfoPvt::_unsafe_atomInfoGroup(CutGroupID cgid) const
{
    return atominfos.find(cgid).value();
}

/** Assert that this residue contains an atom with index 'atomid'

    \throw SireError::invalid_index
*/
void ResidueInfoPvt::assertAtomExists(AtomID atomid) const
{
    if (atomid >= cgidxs.count())
        throw SireError::invalid_index( QObject::tr(
                "Residue \"%1\":%2 has no atom with index \"%3\" (nAtoms() == %4)")
                    .arg(resname).arg(resnum).arg(atomid).arg(cgidxs.count()), CODELOC );
}

/** Assert that this residue contains a CutGroup with ID == cgid */
void ResidueInfoPvt::assertCutGroupExists(CutGroupID cgid) const
{
    if (not atominfos.contains(cgid))
        throw SireMol::missing_cutgroup( QObject::tr(
            "There is no CutGroup with ID == %1 in Residue \"%2\":%3")
                .arg(cgid).arg(resname).arg(resnum), CODELOC);
}

/** Assert that this residue contains an atom with index 'cgatomid'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
void ResidueInfoPvt::assertAtomExists(const CGAtomID &cgatomid) const
{
    assertCutGroupExists(cgatomid.cutGroupID());

    if (not cgidxs.contains(cgatomid))
        throw SireError::invalid_index( QObject::tr(
            "Residue \"%1\":%2 has no atom with index \"%3\", despite having atoms "
            "from that CutGroup")
                .arg(resname).arg(resnum).arg(cgatomid.toString()), CODELOC );
}

/** Assert that this residue contains an atom with name 'atomname'

    \throw SireMol::missing_atom
*/
void ResidueInfoPvt::assertAtomExists(const QString &atomname) const
{
    if (not atomname2atomid.contains(atomname))
        throw SireMol::missing_atom( QObject::tr(
            "Residue \"%1\":%2 does not contain an atom called \"%3\"")
                .arg(resname).arg(resnum).arg(atomname), CODELOC );
}

/** Assert that there are 'n' atoms in this residue

    \throw SireError::incompatible_error
*/
void ResidueInfoPvt::assertNAtoms(int n) const
{
    if (cgidxs.count() != n)
        throw SireError::incompatible_error( QObject::tr(
                "The number of atoms in residue %1(%2), %3, is not equal "
                "to %4.")
                    .arg(resname).arg(resnum).arg(n).arg(cgidxs.count()),
                        CODELOC );
}

///////////
/////////// Implementation of ResidueInfo
///////////

static const RegisterMetaType<SireMol::ResidueInfo> r_resinfo;

/** Serialise to a binary data stream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const ResidueInfo &resinfo)
{
    writeHeader(ds, r_resinfo, 1);

    SharedDataStream(ds) << resinfo.d;

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, ResidueInfo &resinfo)
{
    VersionID v = readHeader(ds, r_resinfo);

    if (v == 1)
    {
        SharedDataStream(ds) >> resinfo.d;
    }
    else
        throw version_error(v, "1", r_resinfo, CODELOC);

    return ds;
}

/** Shared null object */
static QSharedDataPointer<ResidueInfoPvt> shared_null( new ResidueInfoPvt() );

/** Null constructor */
ResidueInfo::ResidueInfo() : d(shared_null)
{}

/** Construct from the passed EditMolData, together with the supplied
    atominfos arranged into CutGroups */
ResidueInfo::ResidueInfo(ResNum resnum, const EditMolData &moldata,
                         const QHash<CutGroupID,AtomInfoGroup> &atominfos)
            : d( new ResidueInfoPvt(resnum,moldata,atominfos) )
{}

/** Copy constructor - this class is implicitly shared, so this is fast */
ResidueInfo::ResidueInfo(const ResidueInfo &other)
            : d(other.d)
{}

/** Destructor */
ResidueInfo::~ResidueInfo()
{}

/** Assert that the AtomIndex 'atom' has the same residue number as
    this residue

    \throw SireError::invalid_arg
*/
void ResidueInfo::assertSameResidue(const AtomIndex &atom) const
{
    if (atom.resNum() != this->number())
        throw SireError::invalid_arg( QObject::tr(
                "You cannot use an AtomIndex with residue number \"%1\" "
                "with a residue with number \"%2\"")
                    .arg(atom.resNum()).arg(this->number()), CODELOC );
}

/** Assert that the AtomIndex objects in 'atoms' all have the same
    residue number as this residue

    \throw SireError::invalid_arg
*/
void ResidueInfo::assertSameResidue(const QSet<AtomIndex> &atom) const
{
    for (QSet<AtomIndex>::const_iterator it = atom.begin();
         it != atom.end();
         ++it)
    {
        this->assertSameResidue(*it);
    }
}

/** Assert that at least some of the atoms in this residue are in
    the CutGroup with ID == cgid

    \throw SireMol::missing_cutgroup
*/
void ResidueInfo::assertSameResidue(CutGroupID cgid) const
{
    if (not this->contains(cgid))
        throw SireMol::missing_cutgroup( QObject::tr(
              "The residue \"%1\":\"%2\" does not have any atoms in the "
              "CutGroup with ID == %3")
                  .arg(this->name()).arg(this->number()).arg(cgid), CODELOC );
}

/** Assert that at least some of the atoms in this residue are in
    the CutGroup with number 'cgnum'

    \throw SireMol::missing_cutgroup
*/
void ResidueInfo::assertSameResidue(CutGroupNum cgnum) const
{
    if (not this->contains(cgnum))
        throw SireMol::missing_cutgroup( QObject::tr(
              "The residue \"%1\":\"%2\" does not have any atoms in the "
              "CutGroup with number  \"%3\"")
                  .arg(this->name()).arg(this->number()).arg(cgnum), CODELOC );
}

/** Assignment operator - this class is implicitly shared, so this is fast */
ResidueInfo& ResidueInfo::operator=(const ResidueInfo &other)
{
    d = other.d;
    return *this;
}

/** Comparison operator */
bool ResidueInfo::operator==(const ResidueInfo &other) const
{
    return *d == *(other.d);
}

/** Comparison operator */
bool ResidueInfo::operator!=(const ResidueInfo &other) const
{
    return *d != *(other.d);
}

/** Return the CGAtomID index of the atom with index 'atmid'

    \throw SireError::invalid_index
*/
const CGAtomID& ResidueInfo::operator[](AtomID atmid) const
{
    d->assertAtomExists(atmid);
    return d->_unsafe_index(atmid);
}

/** Return the CGAtomID index of the atom with name 'atmname'

    \throw SireMol::missing_atom
*/
const CGAtomID& ResidueInfo::operator[](const QString &atmname) const
{
    d->assertAtomExists(atmname);
    return d->_unsafe_index(atmname);
}

/** Return the AtomInfoGroup for the CutGroup with ID == cgid - this CutGroup
    must contain atoms that are in this residue

    \throw SireMol::missing_cutgroup
*/
const AtomInfoGroup& ResidueInfo::operator[](CutGroupID cgid) const
{
    d->assertCutGroupExists(cgid);
    return d->_unsafe_atomInfoGroup(cgid);
}

/** Return the AtomInfoGroup for the CutGroup with number 'cgnum' - this CutGroup
    must contain atoms that are in this residue

    \throw SireMol::missing_cutgroup
*/
const AtomInfoGroup& ResidueInfo::operator[](CutGroupNum cgnum) const
{
    #warning CutGroupNum broken
    throw SireError::incomplete_code( QObject::tr(
                          "Need to update ResidueInfo to work with CutGroupNum"),
                              CODELOC );

    return AtomInfoGroup();
}

/** Return the CGAtomID index of the atom with index 'atmid'

    \throw SireError::invalid_index
*/
const CGAtomID& ResidueInfo::at(AtomID atmid) const
{
    return this->operator[](atmid);
}

/** Return the CGAtomID index of the atom with name 'atmname'

    \throw SireMol::missing_atom
*/
const CGAtomID& ResidueInfo::at(const QString &atmname) const
{
    return this->operator[](atmname);
}

/** Return the AtomInfoGroup for the CutGroup with ID == cgid - this CutGroup
    must contain atoms that are in this residue

    \throw SireMol::missing_cutgroup
*/
const AtomInfoGroup& ResidueInfo::at(CutGroupID cgid) const
{
    return this->operator[](cgid);
}

/** Return the AtomInfoGroup for the CutGroup with number 'cgnum' - this CutGroup
    must contain atoms that are in this residue

    \throw SireMol::missing_cutgroup
*/
const AtomInfoGroup& ResidueInfo::at(CutGroupNum cgnum) const
{
    return this->operator[](cgnum);
}

/** Return the AtomInfo for the atom at index 'atomid'

    \throw SireError::invalid_index
*/
const AtomInfo& ResidueInfo::atom(AtomID atomid) const
{
    return d->_unsafe_atom( this->at(atomid) );
}

/** Return the AtomInfo for the atom with name 'atomname'

    \throw SireMol::missing_atom
*/
const AtomInfo& ResidueInfo::atom(const QString &atomname) const
{
    return d->_unsafe_atom( this->at(atomname) );
}

/** Return the AtomInfo for the atom with index 'cgid'

    (note that this atom must exist in *this* residue)

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
const AtomInfo& ResidueInfo::atom(const CGAtomID &cgid) const
{
    d->assertAtomExists(cgid);
    return d->_unsafe_atom( cgid );
}

/** Return the AtomInfo for the atom with index 'cgid'

    (note that this atom must exist in *this* residue)

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
const AtomInfo& ResidueInfo::atom(const CGNumAtomID &cgid) const
{
    #warning CutGroupNum broken
    throw SireError::incomplete_code( QObject::tr(
                          "Need to update ResidueInfo to work with CutGroupNum"),
                              CODELOC );

    return AtomInfo();
}

/** Return the AtomInfoGroup for the CutGroup with ID == cgid - this CutGroup
    must contain atoms that are in this residue

    \throw SireMol::missing_cutgroup
*/
const AtomInfoGroup& ResidueInfo::atomGroup(CutGroupID cgid) const
{
    return this->operator[](cgid);
}

/** Return the AtomInfoGroup for the CutGroup with number 'cgnum' - this CutGroup
    must contain atoms that are in this residue

    \throw SireMol::missing_cutgroup
*/
const AtomInfoGroup& ResidueInfo::atomGroup(CutGroupNum cgnum) const
{
    return this->operator[](cgnum);
}

/** Return a copy of the complete set of AtomInfoGroups for all of the CutGroups that contain
    atoms that are in this residue. */
QHash<CutGroupID,AtomInfoGroup> ResidueInfo::atomGroups() const
{
    return d->atominfos;
}

/** Return copies of the AtomInfoGroups for the CutGroups whose IDs are in 'cgids' - these
    CutGroups must each contain atoms that are in this Residue

    \throw SireMol::missing_cutgroup
*/
QHash<CutGroupID,AtomInfoGroup> ResidueInfo::atomGroups(const QSet<CutGroupID> &cgids) const
{
    QHash<CutGroupID,AtomInfoGroup> groups;
    groups.reserve(cgids.count());

    for (QSet<CutGroupID>::const_iterator it = cgids.begin();
         it != cgids.end();
         ++it)
    {
        groups.insert( *it, this->atomGroup(*it) );
    }

    return groups;
}

/** Return copies of the AtomInfoGroups for the CutGroups whose numbers are in
    'cgnums' - these CutGroups must each contain atoms that are in this Residue

    \throw SireMol::missing_cutgroup
*/
QHash<CutGroupNum,AtomInfoGroup> ResidueInfo::atomGroups(
                                      const QSet<CutGroupNum> &cgnums) const
{
    QHash<CutGroupNum,AtomInfoGroup> groups;
    groups.reserve(cgnums.count());

    for (QSet<CutGroupNum>::const_iterator it = cgnums.begin();
         it != cgnums.end();
         ++it)
    {
        groups.insert( *it, this->atomGroup(*it) );
    }

    return groups;
}

/** Return an array of all of the AtomInfos of the atoms in this residue, in the
    order that they appear in this residue */
QVector<AtomInfo> ResidueInfo::atoms() const
{
    QVector<AtomInfo> atms;

    int nats = d->cgidxs.count();
    const CGAtomID *cgarray = d->cgidxs.constData();

    atms.reserve(nats);
    for (int i=0; i<nats; ++i)
        atms.append( d->_unsafe_atom( cgarray[i] ) );

    return atms;
}

template<class T>
QHash<T,AtomInfo> getAtoms(const ResidueInfo &resinfo, const QSet<T> &idxs)
{
    QHash<T,AtomInfo> atms;
    atms.reserve(idxs.count());

    for (typename QSet<T>::const_iterator it = idxs.begin();
         it != idxs.end();
         ++it)
    {
        atms.insert( *it, resinfo.atom(*it) );
    }

    return atms;
}

/** Return the AtomInfos of the atoms whose indicies are in 'idxs' - these
    are returned in a hash indexed by AtomID

    \throw SireError::invalid_index
*/
QHash<AtomID,AtomInfo> ResidueInfo::atoms( const QSet<AtomID> &idxs ) const
{
    return getAtoms<AtomID>(*this, idxs);
}

/** Return the AtomInfos of the atoms whose indicies are in 'idxs' - these
    are returned in a hash indexed by CGAtomID. Note that all of the atoms
    must be contained in this residue.

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
QHash<CGAtomID,AtomInfo> ResidueInfo::atoms( const QSet<CGAtomID> &cgids ) const
{
    return getAtoms<CGAtomID>(*this, cgids);
}

/** Return the AtomInfos of the atoms whose indicies are in 'idxs' - these
    are returned in a hash indexed by CGNumAtomID. Note that all of the atoms
    must be contained in this residue.

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
QHash<CGNumAtomID,AtomInfo> ResidueInfo::atoms( const QSet<CGNumAtomID> &cgnums ) const
{
    return getAtoms<CGNumAtomID>(*this, cgnums);
}

/** Return the AtomInfos of the atoms whose names are in 'atomnames' - these
    are returned in a hash indexed by atom name.

    \throw SireMol::missing_atom
*/
QHash<QString,AtomInfo> ResidueInfo::atoms( const QSet<QString> &atomnames ) const
{
    return getAtoms<QString>(*this, atomnames);
}

/** Return the vector of AtomInfos of the atoms that are in the CutGroup with ID == cgid.
    This CutGroup must contain atoms that are in this residue.

    \throw SireMol::missing_residue
*/
QVector<AtomInfo> ResidueInfo::atoms(CutGroupID cgid) const
{
    d->assertCutGroupExists(cgid);
    return d->_unsafe_atomInfoGroup(cgid).atoms();
}

/** Return the vector of AtomInfos of the atoms that are in the CutGroup
    with number 'cgnum'. This CutGroup must contain atoms that are in this residue.

    \throw SireMol::missing_cutgroup
*/
QVector<AtomInfo> ResidueInfo::atoms(CutGroupNum cgnum) const
{
    #warning CutGroupNum broken
    throw SireError::incomplete_code( QObject::tr(
                          "Need to update ResidueInfo to work with CutGroupNum"),
                              CODELOC );

    return QVector<AtomInfo>();
}

template<class T>
QHash< T, QVector<AtomInfo> > getAtomVectors(const ResidueInfo &resinfo,
                                             const QSet<T> &idxs)
{
    QHash< T, QVector<AtomInfo> > atms;
    atms.reserve( idxs.count() );

    for (typename QSet<T>::const_iterator it = idxs.begin();
         it != idxs.end();
         ++it)
    {
        atms.insert( *it, resinfo.atoms(*it) );
    }

    return atms;
}

/** Return the arrays of AtomInfos of the atoms in the CutGroups whose IDs are
    in 'cgids' - each of these CutGroups must contain atoms that are in this residue.

    \throw SireMol::missing_cutgroup
*/
QHash< CutGroupID, QVector<AtomInfo> > ResidueInfo::atoms(
                                            const QSet<CutGroupID> &cgids) const
{
    return getAtomVectors<CutGroupID>(*this, cgids);
}

/** Return the arrays of AtomInfos of the atoms in the CutGroups whose numbers are
    in 'cgnums' - each of these CutGroups must contain atoms that are in this residue.

    \throw SireMol::missing_cutgroup
*/
QHash< CutGroupNum, QVector<AtomInfo> > ResidueInfo::atoms(
                                            const QSet<CutGroupNum> &cgnums) const
{
    return getAtomVectors<CutGroupNum>(*this, cgnums);
}

/** Return an array of all of the CGAtomID indicies of all of the atoms in this
    residue, in the same order as the atoms in this residue. */
const QVector<CGAtomID>& ResidueInfo::indicies() const
{
    return d->cgidxs;
}

/** Return an array of the CutGroupIDs of all CutGroups that contain atoms
    that are in this residue. */
const QSet<CutGroupID>& ResidueInfo::cutGroupIDs() const
{
    #warning ResidueInfo::cutGroupIDs is broken!
    return QSet<CutGroupID>();
}

/** Return an array of the CutGroup numbers of all CutGroups that contain atoms
    that are in this residue. */
QSet<CutGroupNum> ResidueInfo::cutGroupNums() const
{
    #warning ResidueInfo::cutGroupNums is broken!
    return QSet<CutGroupNum>();
}

/** Return a string representation of this ResidueInfo */
QString ResidueInfo::toString() const
{
    return QObject::tr("Residue(\"%1\", %2): nAtoms() == %3, nCutGroups() == %4")
                    .arg(name()).arg(number()).arg(nAtoms()).arg(nCutGroups());
}

/** Return the name of this residue */
QString ResidueInfo::name() const
{
    return d->resname;
}

/** Return the name of this residue */
QString ResidueInfo::resName() const
{
    return d->resname;
}

/** Return the number of this residue */
ResNum ResidueInfo::number() const
{
    return d->resnum;
}

/** Return the number of this residue */
ResNum ResidueInfo::resNum() const
{
    return d->resnum;
}

/** Return the number of CutGroups that contain atoms from this residue */
int ResidueInfo::nCutGroups() const
{
    return d->cgids.count();
}

/** Return the number of atoms in this residue */
int ResidueInfo::nAtoms() const
{
    return d->cgidxs.count();
}

/** Return the number of atoms in CutGroup with ID == cgid
    *that are also in this residue*

    \throw SireMol::missing_cutgroup
*/
int ResidueInfo::nAtoms(CutGroupID cgid) const
{
    d->assertCutGroupExists(cgid);

    return d->_unsafe_atomInfoGroup(cgid).nAtoms(resNum());
}

/** Return the number of atoms in CutGroup with number 'cgnum'
    *that are also in this residue*

    \throw SireMol::missing_cutgroup
*/
int ResidueInfo::nAtoms(CutGroupNum cgnum) const
{
    #warning CutGroupNum broken
    throw SireError::incomplete_code( QObject::tr(
                          "Need to update ResidueInfo to work with CutGroupNum"),
                              CODELOC );

    return 0;
}

/** Return the name of the atom at index 'atomid'

    \throw SireError::invalid_index
*/
QString ResidueInfo::atomName(AtomID atomid) const
{
    return this->atom(atomid).name();
}

/** Return the names of the atoms whose indicies are in 'atomids',
    returning a hash of the names indexed by AtomID

    \throw SireError::invalid_index
*/
QHash<AtomID,QString> ResidueInfo::atomNames(const QSet<AtomID> &atomids) const
{
    QHash<AtomID,QString> names;
    names.reserve(atomids.count());

    for (QSet<AtomID>::const_iterator it = atomids.begin();
         it != atomids.end();
         ++it)
    {
        names.insert( *it, atomName(*it) );
    }

    return names;
}

/** Return the names of all of the atoms in this residue, in the order
    that they appear in this residue */
QStringList ResidueInfo::atomNames() const
{
    QStringList atmnames;

    int nats = d->cgidxs.count();
    const CGAtomID *cgarray = d->cgidxs.constData();

    for (int i=0; i<nats; ++i)
        atmnames.append( d->_unsafe_atom( cgarray[i] ).name() );

    return atmnames;
}

/** Return whether or not this residue contains an atom called 'atmname' */
bool ResidueInfo::contains(const QString &atmname) const
{
    return d->atomname2atomid.contains(atmname);
}

/** Return whether or not this residue contains an atom with index 'atomindex' */
bool ResidueInfo::contains(const AtomIndex &atomindex) const
{
    return atomindex.resNum() == this->resNum() and
           this->contains(atomindex.name());
}

/** Return whether or not this residue contains an atom with index 'atomid' */
bool ResidueInfo::contains(AtomID atomid) const
{
    return atomid < this->nAtoms();
}

/** Return whether or not this residue contains atoms that are in the
    CutGroup with ID == cgid */
bool ResidueInfo::contains(CutGroupID cgid) const
{
    return d->cgids.contains(cgid);
}

/** Return whether or not this residue contains atoms that are in the
    CutGroup with number 'cgnum' */
bool ResidueInfo::contains(CutGroupNum cgnum) const
{
    #warning CutGroupNum broken
    throw SireError::incomplete_code( QObject::tr(
                          "Need to update ResidueInfo to work with CutGroupNum"),
                              CODELOC );

    return false;
}

/** Return whether or not this residue contains an atom with
    index 'cgatomid' */
bool ResidueInfo::contains(const CGAtomID &cgatomid) const
{
    return d->cgidxs.contains(cgatomid);
}

/** Return whether or not this residue contains an atom with
    index 'cgatomid' */
bool ResidueInfo::contains(const CGNumAtomID &cgatomid) const
{
    #warning CutGroupNum broken
    throw SireError::incomplete_code( QObject::tr(
                          "Need to update ResidueInfo to work with CutGroupNum"),
                              CODELOC );

    return false;
}

/** Return whether or not this residue contains an atom with
    index 'atomid' in the CutGroup with ID == cgid */
bool ResidueInfo::contains(CutGroupID cgid, AtomID atomid) const
{
    return this->contains( CGAtomID(cgid,atomid) );
}

/** Return whether or not this residue contains an atom with
    index 'atomid' in the CutGroup with number 'cgnum' */
bool ResidueInfo::contains(CutGroupNum cgnum, AtomID atomid) const
{
    return this->contains( CGNumAtomID(cgnum,atomid) );
}

/** Return whether or not this is empty (has no atoms) */
bool ResidueInfo::isEmpty() const
{
    return nAtoms() == 0;
}

/** Return whether or not the CutGroup with ID == cgid is empty

    \throw SireMol::missing_cutgroup
*/
bool ResidueInfo::isEmpty(CutGroupID cgid) const
{
    return this->at(cgid).isEmpty();
}

/** Return whether or not the CutGroup with number 'cgnum' is empty

    \throw SireMol::missing_cutgroup
*/
bool ResidueInfo::isEmpty(CutGroupNum cgnum) const
{
    #warning CutGroupNum broken
    throw SireError::incomplete_code( QObject::tr(
                          "Need to update ResidueInfo to work with CutGroupNum"),
                              CODELOC );

    return false;
}

/** Assert that this residue contains an atom with name 'atomname'

    \throw SireMol::missing_atom
*/
void ResidueInfo::assertAtomExists(const QString &atomname) const
{
    d->assertAtomExists(atomname);
}

/** Assert that this residue contains an atom with index 'atomid'

    \throw SireError::invalid_index
*/
void ResidueInfo::assertAtomExists(AtomID atomid) const
{
    d->assertAtomExists(atomid);
}

/** Assert that the number of atoms in this residue is 'nats'

    \throw SireError::incompatible_error
*/
void ResidueInfo::assertNAtoms(int nats) const
{
    d->assertNAtoms(nats);
}
