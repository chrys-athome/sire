/**
  * @file
  *
  * C++ Implementation: ResidueInfo and ResidueInfoPvt
  *
  * Description:
  * Implementation of the public ResidueInfo class, and the
  * private ResidueInfoPvt class.
  *
  * @author Christopher Woods, (C) 2006
  *
  * Copyright: See COPYING file that comes with this distribution
  *
  */

#include <QSharedData>

#include "residueinfo.h"

#include "atominfo.h"
#include "atominfogroup.h"

#include "resnum.h"
#include "resid.h"

#include "cgatomid.h"

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

    ResidueInfoPvt(const ResidueInfoPvt &other);

    ~ResidueInfoPvt();

    ResidueInfoPvt& operator=(const ResidueInfoPvt &other);

    bool operator==(const ResidueInfoPvt &other) const;
    bool operator!=(const ResidueInfoPvt &other) const;

    const CGAtomID& operator[](AtomID atmid) const;
    const CGAtomID& operator[](const QString &atmname) const;

    const CGAtomID& at(AtomID atmid) const;
    const CGAtomID& at(const QString &atmname) const;

    QString toString() const;

    int nAtoms() const;

    QString name() const;
    ResNum number() const;

    const AtomInfo& atom(AtomID i) const;
    const AtomInfo& atom(const QString &atmname) const;
    const AtomInfo& atom(const CGAtomID &cgatomid) const;

    QStringList atomNames() const;

    QVector<CutGroupID> cutGroupIDs() const;
    QHash<CutGroupID,AtomInfoGroup> atomGroups() const;

    QVector<CGAtomID> indicies() const;

    bool contains(const QString &atmname) const;
    bool contains(const AtomIndex &atm) const;
    bool contains(AtomID atm) const;
    bool contains(CutGroupID cgid) const;
    bool contains(const CGAtomID &cgid) const;

private:
    const AtomInfo& _unsafe_atom(const CGAtomID &cgid) const;

    /** The residue's name */
    QString resname;

    /** The residue's number */
    ResNum resnum;

    /** Hash mapping atom names to residue-based indicies */
    QHash<QString,AtomID> atomname2atomid;

    /** The names of the atoms, in the order that
        they were added to the residue (AtomID order) */
    QStringList atmnames;

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

using namespace SireMol;

static const RegisterMetaType<ResidueInfoPvt> r_respvt("SireMol::ResidueInfo",
                                                       MAGIC_ONLY);

/** Serialise to a binary data stream */
QDataStream &operator<<(QDataStream &ds, const ResidueInfoPvt &resinfo)
{
    writeHeader(ds, r_respvt, 1)
            << resinfo.resname << resinfo.resnum << resinfo.atmnames
            << resinfo.cgidxs << resinfo.cgids << resinfo.atominfos;

    //no need to save atomname2atomid as this can be rebuilt from the
    //above data

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream &operator>>(QDataStream &ds, ResidueInfoPvt &resinfo)
{
    VersionID v = readHeader(ds, r_respvt);

    if (v == 1)
    {
        ds >> resinfo.resname >> resinfo.resnum >> resinfo.atmnames
           >> resinfo.cgidxs >> resinfo.cgids >> resinfo.atominfos;

        //rebuild atomname2atomid
        int nats = resinfo.atmnames.count();
        QHash<QString,AtomID> atomname2atomid;

        if (nats > 0)
        {
            atomname2atomid.reserve(nats);

            for (AtomID i(0); i<nats; ++i)
                atomname2atomid.insert(resinfo.atmnames[i], i);
        }

        resinfo.atomname2atomid = atomname2atomid;
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

/** Copy constructor */
ResidueInfoPvt::ResidueInfoPvt(const ResidueInfoPvt &other)
               : QSharedData(),
                 resname(other.resname), resnum(other.resnum),
                 atomname2atomid(other.atomname2atomid),
                 atmnames(other.atmnames), cgidxs(other.cgidxs),
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
        atmnames = other.atmnames;
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
            atmnames == other.atmnames and cgidxs == other.cgidxs);
}

/** Comparison operator */
bool ResidueInfoPvt::operator!=(const ResidueInfoPvt &other) const
{
    return (this != &other) and
           (resname != other.resname or resnum != other.resnum or
            atmnames != other.atmnames or cgidxs != other.cgidxs);
}

/** Return the number of atoms in this residue */
int ResidueInfoPvt::nAtoms() const
{
    return cgidxs.count();
}

/** Return the name of this residue */
QString ResidueInfoPvt::name() const
{
    return resname;
}

/** Return the number of this residue */
ResNum ResidueInfoPvt::number() const
{
    return resnum;
}

/** Return a string representation of this ResidueInfo */
QString ResidueInfoPvt::toString() const
{
    return QObject::tr("%1(%2): nAtoms() == %3")
                  .arg(name()).arg(number()).arg(nAtoms());
}

/** Return the CGAtomID of the 'ith' atom in this residue. This will throw
    an exception if 'i' is an invalid index.

    \throw SireError::invalid_index
*/
const CGAtomID& ResidueInfoPvt::operator[](AtomID i) const
{
    if (i < 0 or i >= nAtoms())
        throw SireError::invalid_index( QObject::tr(
                  "Invalid index in residue %1(%2). i == %3 while nAtoms() == %4")
                      .arg(name()).arg(number()).arg(i).arg(nAtoms()),
                          CODELOC );

    //access the index via 'constData()' as we have already checked that
    //this is a valid index
    return cgidxs.constData()[i];
}

/** Return the CGAtomID of the atom with name 'atmname' in this residue. This
    will throw an exception if there is no such atom in this residue.

    \throw SireMol::missing_atom
*/
const CGAtomID& ResidueInfoPvt::operator[](const QString &atmname) const
{
    QHash<QString,AtomID>::const_iterator it = atomname2atomid.find(atmname);

    if (it == atomname2atomid.constEnd())
        throw SireMol::missing_atom( QObject::tr(
                "No atom called '%1' in residue %2(%3)")
                    .arg(atmname, name()).arg(number()), CODELOC );

    //access the index via 'constData()' as we have already checked that
    //it is valid
    return cgidxs.constData()[*it];
}

/** Synonym for operator[](AtomID)

    \throw SireError::invalid_index
*/
const CGAtomID& ResidueInfoPvt::at(AtomID atmid) const
{
    return operator[](atmid);
}

/** Synonym for operator[](QString)

    \throw SireMol::missing_atom
*/
const CGAtomID& ResidueInfoPvt::at(const QString &atmname) const
{
    return operator[](atmname);
}

/** Private function used to return the AtomInfo for the atom
    with CGAtomID 'cgid' - this function assumes that 'cgid' is
    valid, so will not perform any validity checks. This means
    that passing an invalid CGAtomID to this function will lead
    to undefined results! */
inline const AtomInfo& ResidueInfoPvt::_unsafe_atom(const CGAtomID &cgid) const
{
    return atominfos.find(cgid.cutGroupID())->constData()[cgid.atomID()];
}

/** Return the AtomInfo of the ith atom in this residue

    \throw SireError::invalid_index
*/
const AtomInfo& ResidueInfoPvt::atom(AtomID i) const
{
    //get the CGAtomID of this atom - this will throw
    //an exception if this is an invalid atom
    const CGAtomID &cgid = this->at(i);

    //return the AtomInfo - can use unsafe access as
    //we know that cgid is valid
    return this->_unsafe_atom(cgid);
}

/** Return the AtomInfo for the atom with name 'atmname'.

    \throw SireMol::missing_atom
*/
const AtomInfo& ResidueInfoPvt::atom(const QString &atmname) const
{
    //get the CGAtomID of this atom - this will throw
    //an exception if this is an invalid atom
    const CGAtomID &cgid = this->at(atmname);

    //return the AtomInfo - can use unsafe access as
    //we know that 'cgid' is valid
    return this->_unsafe_atom(cgid);
}

/** Return the AtomInfo for the atom with CGAtomID 'cgid'.
    This will throw an exception if there is no atom in
    this residue with this 'cgid'.

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
const AtomInfo& ResidueInfoPvt::atom(const CGAtomID &cgid) const
{
    //find the AtomInfoGroup corresponding to the CutGroupID
    QHash<CutGroupID,AtomInfoGroup>::const_iterator it = atominfos.find(cgid.cutGroupID());

    if (it == atominfos.end())
        throw SireMol::missing_cutgroup( QObject::tr(
                "There is no CutGroup with ID == %1 in residue %2(%3)")
                    .arg(cgid.cutGroupID()).arg(name()).arg(number()),
                        CODELOC );

    const AtomInfo &atominfo = it->at(cgid.atomID());

    if (atominfo.number() != this->number())
        //this atom is not in this residue!
        throw SireError::invalid_index( QObject::tr(
                "The atom '%1' at index %2 in CutGroup with ID == %3 is not in the residue "
                "%4(%5)")
                    .arg(atominfo.toString()).arg(cgid.atomID()).arg(cgid.cutGroupID())
                    .arg(name()).arg(number()), CODELOC );

    return atominfo;
}

/** Return the names of all of the atoms in the residue, in the same order that they
    appear in the residue (AtomID order) */
QStringList ResidueInfoPvt::atomNames() const
{
    return atmnames;
}

/** Return the CutGroupIDs of all of the CutGroups that contain atoms from this residue. */
QVector<CutGroupID> ResidueInfoPvt::cutGroupIDs() const
{
    return cgids;
}

/** Return all of the AtomInfoGroups that contain atoms that come from this residue -
    note that these groups may also contain atoms that come from other residues. */
QHash<CutGroupID,AtomInfoGroup> ResidueInfoPvt::atomGroups() const
{
    return atominfos;
}

/** Return the CGAtomID indicies of all of the atoms in this residue, in the same order
    that they appear in this residue (AtomID order) */
QVector<CGAtomID> ResidueInfoPvt::indicies() const
{
    return cgidxs;
}

/** Return whether or not this residue contains an atom called 'atmname' */
bool ResidueInfoPvt::contains(const QString &atmname) const
{
    return atomname2atomid.contains(atmname);
}

/** Return whether or not this residue contains the atom with AtomIndex 'atm' */
bool ResidueInfoPvt::contains(const AtomIndex &atm) const
{
    return atm.resNum() == number() and this->contains(atm.name());
}

/** Return whether or not this residue contains the atom with AtomID 'atmid' */
bool ResidueInfoPvt::contains(AtomID atmid) const
{
    return atmid >= 0 and atmid < this->nAtoms();
}

/** Return whether or not this residue contains atoms that are present in the
    CutGroup with CutGroupID == cgid */
bool ResidueInfoPvt::contains(CutGroupID cgid) const
{
    return atominfos.contains(cgid);
}

/** Return whether or not this residue contains the atom with CGAtomID index 'cgid' */
bool ResidueInfoPvt::contains(const CGAtomID &cgid) const
{
    QHash<CutGroupID,AtomInfoGroup>::const_iterator it = atominfos.find(cgid.cutGroupID());

    return ( it != atominfos.end() and
             cgid.atomID() < it->count() and
             it->at(cgid.atomID()).resNum() == this->number() );
}

///////////
/////////// Implementation of ResidueInfo
///////////

static const RegisterMetaType<SireMol::ResidueInfo> r_resinfo("SireMol::ResidueInfo");

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

/** Copy constructor - this class is implicitly shared, so this is fast */
ResidueInfo::ResidueInfo(const ResidueInfo &other)
            : d(other.d)
{}

/** Destructor */
ResidueInfo::~ResidueInfo()
{}

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

/** Return the CGAtomID of the 'ith' atom in this residue. This will throw
    an exception if 'i' is an invalid index.

    \throw SireError::invalid_index
*/
const CGAtomID& ResidueInfo::operator[](AtomID i) const
{
    return d->operator[](i);
}

/** Return the CGAtomID of the atom with name 'atmname' in this residue. This
    will throw an exception if there is no such atom in this residue.

    \throw SireMol::missing_atom
*/
const CGAtomID& ResidueInfo::operator[](const QString &atmname) const
{
    return d->operator[](atmname);
}

/** Synonym for operator[](AtomID)

    \throw SireError::invalid_index
*/
const CGAtomID& ResidueInfo::at(AtomID atmid) const
{
    return d->at(atmid);
}

/** Synonym for operator[](QString)

    \throw SireMol::missing_atom
*/
const CGAtomID& ResidueInfo::at(const QString &atmname) const
{
    return d->at(atmname);
}

/** Return a string representation of this ResidueInfo */
QString ResidueInfo::toString() const
{
    return d->toString();
}

/** Return the number of atoms in this residue */
int ResidueInfo::nAtoms() const
{
    return d->nAtoms();
}

/** Return the name of this residue */
QString ResidueInfo::name() const
{
    return d->name();
}

/** Return the name of this residue */
QString ResidueInfo::resName() const
{
    return d->name();
}

/** Return the number of this residue */
ResNum ResidueInfo::number() const
{
    return d->number();
}

/** Return the number of this residue */
ResNum ResidueInfo::resNum() const
{
    return d->number();
}

/** Return the AtomInfo of the ith atom in this residue

    \throw SireError::invalid_index
*/
const AtomInfo& ResidueInfo::atom(AtomID i) const
{
    return d->atom(i);
}

/** Return the AtomInfo for the atom with name 'atmname'.

    \throw SireMol::missing_atom
*/
const AtomInfo& ResidueInfo::atom(const QString &atmname) const
{
    return d->atom(atmname);
}

/** Return the AtomInfo for the atom with CGAtomID 'cgid'.
    This will throw an exception if there is no atom in
    this residue with this 'cgid'.

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
const AtomInfo& ResidueInfo::atom(const CGAtomID &cgid) const
{
    return d->atom(cgid);
}

/** Return the names of all of the atoms in the residue, in the same order that they
    appear in the residue (AtomID order) */
QStringList ResidueInfo::atomNames() const
{
    return d->atomNames();
}

/** Return the CutGroupIDs of all of the CutGroups that contain atoms from this residue. */
QVector<CutGroupID> ResidueInfo::cutGroupIDs() const
{
    return d->cutGroupIDs();
}

/** Return all of the AtomInfoGroups that contain atoms that come from this residue -
    note that these groups may also contain atoms that come from other residues. */
QHash<CutGroupID,AtomInfoGroup> ResidueInfo::atomGroups() const
{
    return d->atomGroups();
}

/** Return the CGAtomID indicies of all of the atoms in this residue, in the same order
    that they appear in this residue (AtomID order) */
QVector<CGAtomID> ResidueInfo::indicies() const
{
    return d->indicies();
}

/** Return whether or not this residue contains an atom called 'atmname' */
bool ResidueInfo::contains(const QString &atmname) const
{
    return d->contains(atmname);
}

/** Return whether or not this residue contains the atom with AtomIndex 'atm' */
bool ResidueInfo::contains(const AtomIndex &atm) const
{
    return d->contains(atm);
}

/** Return whether or not this residue contains the atom with AtomID 'atmid' */
bool ResidueInfo::contains(AtomID atmid) const
{
    return d->contains(atmid);
}

/** Return whether or not this residue contains atoms that are present in the
    CutGroup with CutGroupID == cgid */
bool ResidueInfo::contains(CutGroupID cgid) const
{
    return d->contains(cgid);
}

/** Return whether or not this residue contains the atom with CGAtomID index 'cgid' */
bool ResidueInfo::contains(const CGAtomID &cgid) const
{
    return d->contains(cgid);
}
