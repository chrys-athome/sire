/** 
  * @file
  *
  * C++ Implementation: MoleculeInfo and MoleculeInfoPvt
  *
  * Description: 
  * Implementation of the public MoleculeInfo class, and the 
  * private MoleculeInfoPvt class.
  *
  * @author Christopher Woods, (C) 2006
  *
  * Copyright: See COPYING file that comes with this distribution
  *
  */
  
#include <QSharedData>
  
#include "moleculeinfo.h"

#include "SireStream/datastream.h"
#include "SireStream/implicitstreamer.h"

using namespace SireStream;

namespace SireMol
{

/** Private class used to implement MoleculeInfo.

    @author Christopher Woods
*/
class MoleculeInfoPvt : public QSharedData
{
public:
    MoleculeInfoPvt();
    
    MoleculeInfoPvt(const MoleculeInfoPvt &other);

    ~MoleculeInfoPvt();

    MoleculeInfoPvt& operator=(const MoleculeInfoPvt &other);

    bool operator==(const MoleculeInfoPvt &other) const;
    bool operator!=(const MoleculeInfoPvt &other) const;

    const ResidueInfo& operator[](ResID resid) const;
    const ResidueInfo& operator[](ResNum resnum) const;

    const CGAtomID& operator[](AtomID atmid) const;
    const CGAtomID& operator[](const AtomIndex &atomindex) const;
    const CGAtomID& operator[](const ResNumAtomID &resatomid) const;
    const CGAtomID& operator[](const ResIDAtomID &resatomid) const;

    const ResidueInfo& at(ResID resid) const;
    const ResidueInfo& at(ResNum resnum) const;
    
    const CGAtomID& at(AtomID atmid) const;
    const CGAtomID& at(const AtomIndex &atomindex) const;
    const CGAtomID& at(const ResNumAtomID &resatomid) const;
    const CGAtomID& at(const ResIDAtomID &resatomid) const;
    
    const AtomInfo& atom(AtomID atomid) const;
    const AtomInfo& atom(const AtomIndex &atomindex) const;
    const AtomInfo& atom(const ResNumAtomID &rsid) const;
    const AtomInfo& atom(const ResIDAtomID &rsid) const;
    const AtomInfo& atom(const CGAtomID &cgid) const;
    
    const ResidueInfo& residue(AtomID atmid) const;
    const ResidueInfo& residue(ResID resid) const;
    const ResidueInfo& residue(ResNum resnum) const;
    
    QString name() const;
    
    bool isEmpty() const;
    bool isNull() const;
    
    QString toString() const;
    
    int nResidues() const;

    int nAtoms() const;
    int nAtoms(ResNum resnm) const;

    QVector<ResNum> residueNumbers() const;
    QVector<ResNum> residueNumbers(const QString &resname) const;
    
    QStringList residueNames() const;
    
    QHash<CutGroupID,AtomInfoGroup> atomGroups() const;
    
    bool contains(ResNum resnum) const;
    bool contains(ResID resid) const;
    bool contains(CutGroupID cgid) const;
    
    bool contains(AtomID atmid) const;
    bool contains(const AtomIndex &atm) const;
    bool contains(const CGAtomID &cgid) const;
    bool contains(const ResNumAtomID &resid) const;
    bool contains(const ResIDAtomID &resid) const;
    
private:
    /** The name of this molecule */
    QString molname;

    /** Vector of residue numbers of the residues in this molecule, in the
        order that they were added to this molecule. */
    QVector<ResNum> resnums;

    /** Metainfo for all of the atoms in this molecule, indexed by
        CGAtomID (use a hash so that we save memory by copying this
        hash directly to the residues for single-cutgroup molecules
        - e.g. most solvents!) */
    QHash<CutGroupID, AtomInfoGroup> atominfos;
    
    /** Metainfo for all of the residues, indexed by residue number */
    QHash<ResNum, ResidueInfo> resinfos;

    /** QMap mapping the highest index of the atom in each residue
        to the residue number. This is used to quickly find the 'ith' 
        atom in the molecule. */
    QMap<AtomID, ResNum> idx2resnum;
    
    /** The total number of atoms in this molecule */
    quint32 nats;
};

}

using namespace SireMol;

static const RegisterMetaType<MoleculeInfoPvt> r_molpvt("SireMol::MoleculeInfoPvt", 
                                                        MAGIC_ONLY);

/** Serialise to a binary data stream */
QDataStream &operator<<(QDataStream &ds, const MoleculeInfoPvt &molinfo)
{
    writeHeader(ds, r_molpvt, 1) 
          << molinfo.molname << molinfo.resnums << molinfo.atominfos
          << molinfo.resinfos;
        
    //don't need to save idx2resnum and nats as these can be regenerated
          
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream &operator>>(QDataStream &ds, MoleculeInfoPvt &molinfo)
{
    VersionID v = readHeader(ds, r_molpvt);
    
    if (v == 1)
    {
        ds >> molinfo.molname >> molinfo.resnums >> molinfo.atominfos
           >> molinfo.resinfos;
           
        //regenerate idx2resnums and nats
        molinfo.regenerateIndex();
    }
    else
        throw version_error(v, "1", r_molpvt, CODELOC);
    
    return ds;
}

////////////
//////////// Implementation of MoleculeInfoPvt
////////////

/** Null constructor */
MoleculeInfoPvt::MoleculeInfoPvt()
                : QSharedData(),
                  molname(QObject::tr("null")), nats(0)
{}

/** Copy constructor */
MoleculeInfoPvt::MoleculeInfoPvt(const MoleculeInfoPvt &other)
                : QSharedData(),
                  molname(other.molname), resnums(other.resnums),
                  atominfos(other.atominfos), resinfos(other.resinfos),
                  idx2resnum(other.idx2resnum), nats(other.nats)
{}

/** Destructor */
MoleculeInfoPvt::~MoleculeInfoPvt()
{}

/** Assignment operator */
MoleculeInfoPvt& MoleculeInfoPvt::operator=(const MoleculeInfoPvt &other)
{
    if (&other != this)
    {
        molname = other.molname;
        resnums = other.resnums;
        atominfos = other.atominfos;
        resinfos = other.resinfos;
        idx2resnum = other.idx2resnum;
        nats = other.nats;
    }
    
    return *this;
}

/** Comparison operator */
bool MoleculeInfoPvt::operator==(const MoleculeInfoPvt &other) const
{
    return (this == &other) or
           (molname == other.molname and resinfos == other.resinfos);
}

/** Comparison operator */
bool MoleculeInfoPvt::operator!=(const MoleculeInfoPvt &other) const
{
    return (this != &other) and
           (molname != other.molname or resinfos != other.resinfos);
}

/** Return the name of this molecule */
QString MoleculeInfoPvt::name() const
{
    return molname;
}

/** Internal function used to return the ResidueInfo for the 'ith' residue.
    This assumes that 'i' is valid, so will have undefined results if that
    is not the case! */
const ResidueInfo& MoleculeInfoPvt::_unsafe_residue(ResID i) const
{
    return resinfos.find( resnums.constData()[i.index()] ).value();
}

/** Internal function used to return the ResidueInfo for residue with number
    'resnum'. This assumes that there is a residue with this number, so will 
    have undefined results if that is not the case! */
const ResidueInfo& MoleculeInfoPvt::_unsafe_residue(ResNum resnum) const
{
    return resinfos.find( resnum ).value();
}

/** Return the ResidueInfo for the 'ith' residue in this molecule. This will 
    throw an exception if 'i' is an invalid index. 
    
    \throw SireError::invalid_index
*/
const ResidueInfo& MoleculeInfoPvt::operator[](ResID i) const
{
    checkResidue(i);
    return this->_unsafe_residue(i);
}

/** Return the ResidueInfo for the residue with residue number 'resnum'. This
    will throw an exception if there is no such residue.
    
    \throw SireMol::missing_residue
*/
const ResidueInfo& MoleculeInfoPvt::operator[](ResNum resnum) const
{
    checkResidue(resnum);
    return this->_unsafe_residue(resnum);
}

/** Throw an exception if passed an invalid AtomID 

    \throw SireError::invalid_index
*/
inline void MoleculeInfoPvt::checkAtom(AtomID atmid) const
{
    if (atmid < 0 or atmid >= nAtoms())
        throw SireError::invalid_index( QObject::tr(
                "Invalid index in molecule '%1', no AtomID index '%2'")
                    .arg(this->toString()).arg(atmid.index()), CODELOC );
}

/** Return the CGAtomID of the 'ith' atom in this molecule. This will throw an 
    exception if 'i' is an invalid index.
    
    \throw SireError::invalid_index
*/
const CGAtomID& MoleculeInfoPvt::operator[](AtomID i) const
{
    checkAtom(i);
                    
    //find the nearest index to 'i'
    QMap<AtomID,ResNum>::const_iterator it = idx2resnum.lowerBound(i.index());

    BOOST_ASSERT( it != idx2resnum.end() ); //this should be impossible...
    
    //get the residueinfo that contains this index...
    const ResidueInfo &resinfo = this->_unsafe_residue( it.value() );
    
    //ok - it.key() contains the highest index for this residue
    //If we subtract it.key() from i that will convert i into a reversed
    //index into the residueinfo
    i -= it.key().index();
    
    //convert the reversed index into a real index by adding
    //it to 'natoms' in the residue (remembering that i is negative)
    i += resinfo.nAtoms();
    
    return resinfo[i];
}

/** Return the CGAtomID of the atom with AtomIndex 'atm'. This will throw an
    exception if there is no such atom in this molecule.
    
    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
const CGAtomID& MoleculeInfoPvt::operator[](const AtomIndex &atm) const
{
    return residue(atm.resNum()).atom(atm.name());
}

/** Return the CGAtomID of the atom with index 'resatomid'. This will throw
    an exception if there is no such atom in this molecule.
    
    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
const CGAtomID& MoleculeInfoPvt::operator[](const ResNumAtomID &resatomid) const
{
    return residue(resatomid.resNum()).atom(resatomid.atomID());
}

/** Return the CGAtomID of the atom with index 'resatomid'. This will throw
    an exception if thereis no such atom in this molecule.
    
    \throw SireError::invalid_index
*/
const CGAtomID& MoleculeInfoPvt::operator[](const ResIDAtomID &resatomid) const
{
    return residue(resatomid.resID()).atom(resatomid.atomID());
}

/** Synonym for operator[](ResID)

    \throw SireError::invalid_index
*/
const ResidueInfo& MoleculeInfoPvt::at(ResID resid) const
{
    return operator[](resid);
}

/** Synonym for operator[](ResNum)

    \throw SireMol::missing_residue
*/
const ResidueInfo& MoleculeInfoPvt::at(ResNum resnum) const
{
    return operator[](resnum);
}

/** Synonym for operator[](AtomID)

    \throw SireError::invalid_index
*/
const CGAtomID& MoleculeInfoPvt::at(AtomID atmid) const
{
    return operator[](atmid);
}

/** Synonym for operator[](AtomIndex)

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
const CGAtomID& MoleculeInfoPvt::at(const AtomIndex &atomindex) const
{
    return operator[](atomindex);
}

/** Synonym for operator[](ResID)

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
const CGAtomID& MoleculeInfoPvt::at(const ResNumAtomID &resatomid) const
{
    return operator[](resatomid);
}

/** Synonym for operator[](ResID)

    \throw SireError::invalid_index
*/
const CGAtomID& MoleculeInfoPvt::at(const ResIDAtomID &resatomid) const
{
    return operator[](resatomid);
}

/** Internal function used to return the AtomInfo for the atom with CGAtomID
    'cgid'. This assumes that 'cgid' is valid, so this will have undefined
    consequences if cgid is not valid! */
const AtomInfo& MoleculeInfoPvt::_unsafe_atom(const CGAtomID &cgid) const
{
    return atominfos.find(cgid.cutGroupID())->constData()[cgid.atomID()];
}

/** Return the AtomInfo for the 'ith' atom of this molecule. This throws
    an index if 'i' is invalid.
    
    \throw SireError::invalid_index
*/
const AtomInfo& MoleculeInfoPvt::atom(AtomID i) const
{
    //get the CGAtomID of this atom - this will throw an 
    //exception if this is an invalid index
    const CGAtomID &cgid = this->at(i);

    //get the AtomInfo - can use unsafe function as we know that
    //cgid is valid
    return this->_unsafe_atom(cgid);
}

/** Return the AtomInfo for the atom with index 'atomindex'. This will throw
    an exception if there is no such atom in this molecule.
    
    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
const AtomInfo& MoleculeInfoPvt::atom(const AtomIndex &atomindex) const
{
    //get the CGAtomID of this atom - this will throw an 
    //exception if this is an invalid index
    const CGAtomID &cgid = this->at(atomindex);

    //get the AtomInfo - can use unsafe function as we know that
    //cgid is valid
    return this->_unsafe_atom(cgid);
}

/** Return the AtomInfo for the atom with index 'rsid'. This will throw
    an exception if there is no such atom in this molecule.
    
    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
const AtomInfo& MoleculeInfoPvt::atom(const ResNumAtomID &rsid) const
{
    //get the CGAtomID of this atom - this will throw an 
    //exception if this is an invalid index
    const CGAtomID &cgid = this->at(rsid);

    //get the AtomInfo - can use unsafe function as we know that
    //cgid is valid
    return this->_unsafe_atom(cgid);
}

/** Return the AtomInfo for the atom with index 'rsid'. This will throw
    an exception if there is no such atom in this molecule.
    
    \throw SireError::invalid_index
*/
const AtomInfo& MoleculeInfoPvt::atom(const ResIDAtomID &rsid) const
{
    //get the CGAtomID of this atom - this will throw an 
    //exception if this is an invalid index
    const CGAtomID &cgid = this->at(rsid);

    //get the AtomInfo - can use unsafe function as we know that
    //cgid is valid
    return this->_unsafe_atom(cgid);
}

/** Return the AtomInfo for the atom with CGAtomID 'cgid'. This will throw
    an exception if there is no such atom in this molecule.
    
    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
const AtomInfo& MoleculeInfoPvt::atom(const CGAtomID &cgid) const
{
    QHash<CutGroupID,AtomInfoGroup>::const_iterator it = atominfos.find(cgid.cutGroupID());
    
    if (it == atominfos.end())
        throw SireMol::missing_cutgroup( QObject::tr(
                "There is no CutGroup with CutGroupID == %1 in molecule %2 "
                "(nCutGroups() == %3)")
                    .arg(cgid.cutGroupID()).arg(this->name()).arg(this->nCutGroups()),
                        CODELOC );
                        
    return it.value().atom(cgid.atomID());
}

/** Return the residue that contains the atom with AtomID 'atmid' 

    \throw SireError::invalid_index
*/
const ResidueInfo& MoleculeInfoPvt::residue(AtomID atmid) const
{
    checkAtom(atmid);

    //find the nearest index to 'i'
    QMap<AtomID,ResNum>::const_iterator it = idx2resnum.lowerBound(i.index());

    BOOST_ASSERT( it != idx2resnum.end() ); //this should be impossible...
    
    //get the residueinfo that contains this index...
    return this->_unsafe_residue( it.value() );
}

/** Return the 'ith' residue in this molecule. This will throw an exception
    if 'i' refers to an invalid residue. 
    
    \throw SireError::invalid_index
*/
const ResidueInfo& MoleculeInfoPvt::residue(ResID i) const
{
    return this->at(i);
}

/** Return the residue with number 'resnum'. This will throw an exception
    if there is no such residue. 
    
    \throw SireMol::missing_residue
*/
const ResidueInfo& MoleculeInfoPvt::residue(ResNum resnum) const
{
    return this->at(resnum);
}

/** Return whether or not this molecule is empty (contains no atoms) */
bool MoleculeInfoPvt::isEmpty() const
{
    return nAtoms() == 0;
}

/** Return whether this is a null molecule (no atoms) */
bool MoleculeInfoPvt::isNull() const
{
    return nAtoms() == 0;
}

/** Return a string representation of the molecule info */
QString MoleculeInfoPvt::toString() const
{
    return QObject::tr("Molecule('%1'): nAtoms() == %2, nResidues() == %3, "
                       "nCutGroups() == %4")
                          .arg(this->name()).arg(nAtoms()).arg(nResidues())
                          .arg(nCutGroups());
}

/** Return the number of residues in this molecule */
int MoleculeInfoPvt::nResidues() const
{
    return resnums.count();
}

/** Return the number of atoms in this molecule */
int MoleculeInfoPvt::nAtoms() const
{
    return nats;
}

/** Return the number of atoms in the residue with number 'resnum'

    \throw SireMol::missing_residue
*/
int MoleculeInfoPvt::nAtoms(ResNum resnm) const
{
    return residue(resnum).nAtoms();
}

/** Return the numbers of all of the residues, in the order that the residues 
    appear in the molecule (ResID order) */
QVector<ResNum> MoleculeInfoPvt::residueNumbers() const
{
    return resnums;
}

/** Return the numbers of all of the residues that are called 'resname'. */
QVector<ResNum> MoleculeInfoPvt::residueNumbers(const QString &resname) const
{
    QVector<ResNum> ret;
    
    for (QHash<ResNum, ResidueInfo>::const_iterator it = resinfos.begin();
         it != resinfos.end();
         ++it)
    {
        if (it->resName() == resname)
            ret.append( it->resNum() );
    }
    
    return ret;
}

/** Return the list of residue names of the residues, in the order that they appear
    in the molecule (ResID order) */
QStringList MoleculeInfoPvt::residueNames() const
{
    QStringList ret;
    
    for (QHash<ResNum, ResidueInfo>::const_iterator it = resinfos.begin();
         it != resinfos.end();
         ++it)
    {
         ret.append( it->resName() );
    }
    
    return ret;
}

/** Return all of the AtomInfoGroups of this molecule, indexed by CutGroupID */
QHash<CutGroupID,AtomInfoGroup> MoleculeInfoPvt::atomGroups() const
{
    return atominfos;
}

/** Return whether or not this molecule contains a residue with number 'resnum' */
bool MoleculeInfoPvt::contains(ResNum resnum) const
{
    return resinfos.contains(resnum);
}

/** Return whether or not this molecule contains a residue with ResID 'resid' */
bool MoleculeInfoPvt::contains(ResID resid) const
{
    return resid >= 0 and resid < nResidues();
}

/** Return whether or not this molecule contains a CutGroup with ID 'cgid' */
bool MoleculeInfoPvt::contains(CutGroupID cgid) const
{
    return atominfos.contains(cgid);
}

/** Return whether or not this molecule contains an atom with ID == 'atmid' */
bool MoleculeInfoPvt::contains(AtomID atmid) const
{
    return atmid >= 0 and atmid < nAtoms();
}

/** Return whether or not this molecule contains an atom with index 'atm' */
bool MoleculeInfoPvt::contains(const AtomIndex &atm) const
{
    QHash<ResNum, ResidueInfo>::const_iterator it = resinfos.find(atm.resNum());

    return it != resinfos.end() and 
           it.value().contains(atm.name());
}

/** Return whether or not this molecule contains an atom with CGAtomID 'cgid' */
bool MoleculeInfoPvt::contains(const CGAtomID &cgid) const
{
    QHash<CutGroupID, AtomInfoGroup>::const_iterator it = atominfos.find(cgid.cutGroupID());
    
    return it != atominfos.end() and 
           it->contains(cgid.atomID());
}

/** Return whether or not this molecule contains an atom with index 'resid' */
bool MoleculeInfoPvt::contains(const ResNumAtomID &resid) const
{
    QHash<ResNum, ResidueInfo>::const_iterator it = resinfos.find(resid.resNum());

    return it != resinfos.end() and 
           it.value().contains(resid.atomID());
}

/** Return whether or not this molecule contains an atom with index 'resid' */
bool MoleculeInfoPvt::contains(const ResIDAtomID &resid) const
{
    return (resid.resID() > 0 and resid.resID() < nResidues()) and 
           _unsafe_residue(resid.resID()).contains(resid.atomID());
}

////////////
//////////// Implementation of MoleculeInfo
////////////

static const RegisterMetaType<SireMol::MoleculeInfo> r_molinfo("SireMol::MoleculeInfo");

/** Serialise to a binary data stream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const MoleculeInfo &molinfo)
{
    //get a streamer object to help stream this implicitly shared class
    ImplicitStreamer streamer = ImplicitStore::getStreamer(ds);

    writeHeader(ds, r_molinfo, 1);
    streamer.saveQSharedData(molinfo.d);
    
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, MoleculeInfo &molinfo)
{
    VersionID v = readHeader(ds, r_molinfo);
    
    if (v == 1)
    {
        //get a streamer object to help stream this implicitly shared class
        ImplicitStreamer streamer = ImplicitStore::getStreamer(ds);
        
        streamer.loadQSharedData(molinfo.d);
    }
    else
        throw version_error(v, "1", r_molinfo, CODELOC);
    
    return ds;
}

MoleculeInfo::MoleculeInfo()
{
}


MoleculeInfo::~MoleculeInfo()
{
}


/** Return the set of AtomInfos for the atoms whose indicies are in 'idxs'.

    \throw SireError::invalid_index
*/
QSet<AtomInfo> MoleculeInfo::getAtoms( const QSet<AtomID> &idxs ) const
{
    QSet<AtomInfo> ret;
    
    ret.reserve( idxs.count() );
    
    for (QSet<AtomID>::const_iterator it = idxs.begin();
         it != idxs.end();
         ++it)
    {
        ret.insert( this->at(*it) );
    }
    
    return ret;
}

/** Return the set of AtomInfos for the atoms whose AtomIndexes are in 'atms'.

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
QSet<AtomInfo> MoleculeInfo::getAtoms( const QSet<AtomIndex> &atms ) const
{
    QSet<AtomInfo> ret;
    
    ret.reserve( idxs.count() );
    
    for (QSet<AtomIndex>::const_iterator it = idxs.begin();
         it != idxs.end();
         ++it)
    {
        ret.insert( this->at(*it) );
    }
    
    return ret;
}

/** Return the set of AtomInfos for the atoms whose indicies are in 'rsids'.

    \throw SireError::invalid_index
*/
QSet<AtomInfo> MoleculeInfo::getAtoms( const QSet<ResNumAtomID> &rsids) const
{
    QSet<AtomInfo> ret;
    
    ret.reserve( idxs.count() );
    
    for (QSet<ResNumAtomID>::const_iterator it = idxs.begin();
         it != idxs.end();
         ++it)
    {
        ret.insert( this->at(*it) );
    }
    
    return ret;
}

/** Return the set of AtomInfos for the atoms whose indicies are in 'rsids'.

    \throw SireError::invalid_index
*/
QSet<AtomInfo> MoleculeInfo::getAtoms( const QSet<ResIDAtomID> &rsids) const
{
    QSet<AtomInfo> ret;
    
    ret.reserve( idxs.count() );
    
    for (QSet<ResIDAtomID>::const_iterator it = idxs.begin();
         it != idxs.end();
         ++it)
    {
        ret.insert( this->at(*it) );
    }
    
    return ret;
}

/** Return the set of AtomInfos for the atoms whose indicies are in 'cgids'.

    \throw SireError::invalid_index
*/
QSet<AtomInfo> MoleculeInfo::getAtoms( const QSet<CGAtomID> &cgids ) const
{
    QSet<AtomInfo> ret;
    
    ret.reserve( idxs.count() );
    
    for (QSet<CGAtomID>::const_iterator it = idxs.begin();
         it != idxs.end();
         ++it)
    {
        ret.insert( this->at(*it) );
    }
    
    return ret;
}
