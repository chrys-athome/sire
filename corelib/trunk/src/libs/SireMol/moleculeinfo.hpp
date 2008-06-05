#ifndef SIREMOL_MOLECULEINFO_HPP
#define SIREMOL_MOLECULEINFO_HPP

#include <QMap>
#include <QDataStream>

#include "moleculesignature.h"
#include "residueinfo.hpp"

#include "cgatomid.h"
#include "resnumatomid.h"

#include "SireStream/datastream.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
template<class IDX>
class MoleculeInfo;
}

template<class IDX>
QDataStream& operator<<(QDataStream&, const SireMol::MoleculeInfo<IDX>&);
template<class IDX>
QDataStream& operator>>(QDataStream&, SireMol::MoleculeInfo<IDX>&);

namespace SireMol
{

/** This class provides a collection of ResidueInfo objects to provide
    all of the metainfo for a molecule, and to provide the indexing from AtomIndex
    to the index type of this MoleculeInfo.
    
    A MoleculeInfo provides only const access to the contained ResidueInfo 
    objects.
    
    @author Christopher Woods
*/
template<class IDX>
class SIREMOL_EXPORT MoleculeInfo
{

friend QDataStream& ::operator<<<>(QDataStream&, const MoleculeInfo<IDX>&);
friend QDataStream& ::operator>><>(QDataStream&, MoleculeInfo<IDX>&);

public:
    MoleculeInfo(const QString &name = QObject::tr("unnamed"));
    MoleculeInfo(const QList< ResidueInfo<IDX> > &resinfos);
    MoleculeInfo(const QString &name, const QList< ResidueInfo<IDX> > &resinfos);
    
    MoleculeInfo(const MoleculeInfo<IDX> &other);
    
    ~MoleculeInfo();
    
    MoleculeInfo<IDX>& operator=(const MoleculeInfo<IDX> &other);
    
    bool operator==(const MoleculeInfo<IDX> &other) const;
    bool operator!=(const MoleculeInfo<IDX> &other) const;
    
    const ResidueInfo<IDX>& at(ResID i) const;
    const IDX& at(const AtomIndex &atom) const;
    const IDX& at(const ResNumAtomID &rsid) const;
    
    const ResidueInfo<IDX>& operator[](ResID i) const;
    const IDX& operator[](const AtomIndex &atom) const;
    const IDX& operator[](const ResNumAtomID &rsid) const;
    
    const QString& name() const;
    
    bool isEmpty() const;
    bool isNull() const;
    
    const IDX& index(const AtomIndex &atm) const;
    const IDX& index(const ResNumAtomID &rsid) const;
    const IDX& index(ResNum resnum, AtomID i) const;
    const IDX& index(AtomID i) const;
    
    AtomIndex atom(AtomID i) const;
    AtomIndex atom(ResNum resnum, AtomID i) const;
    AtomIndex atom(const ResNumAtomID &rsid) const;
    
    QSet<AtomIndex> getAtoms( const QSet<ResNumAtomID>& rsids) const;
    QSet<AtomIndex> getAtoms( const QSet<AtomID> &idxs ) const;
    
    QString toString() const;
    
    MoleculeSignature signature() const;
    
    int size() const;
    int nResidues() const;
    int count() const;

    int nAtoms() const;
    int nAtoms(ResNum resnm) const;

    void add(const ResidueInfo<IDX> &resinfo);
    void remove(ResNum resnum);
    
    const ResNumList& residueNumbers() const;
    ResNumList residueNumbers(const QString &resname) const;
    QStringList residueNames() const;
    
    ResidueIDSet residueIDs() const;
    
    const ResidueInfo<IDX>& residue(ResNum resnum) const;
    
    bool contains(const AtomIndex &atm) const;
    bool contains(ResNum resnum) const;
    bool contains(const ResNumAtomID &idx) const;
    
private:

    void recalculate();
    void checkResidue(ResNum resnum) const;

    /** Hash mapping a residue number to the corresponding ResidueInfo object */
    QHash< ResNum, ResidueInfo<IDX> > residus;
    
    /** List of residue numbers of the residues in this molecule, in the 
        order that they were added to this molecule */
    ResNumList resnums;
    
    /** QMap mapping the highest index of the atom in each residue
        to the residue number. This is used to quickly find the 'ith' 
        atom in the molecule. */
    QMap<AtomID, ResNum> idx2resnum;
    
    /** The name of this molecule */
    QString molname;
    
    /** The number of atoms in this molecule */
    quint32 nats;
};

/** Construct an empty MoleculeInfo for the molecule called 'name' */
template<class IDX>
SIRE_OUTOFLINE_TEMPLATE
MoleculeInfo<IDX>::MoleculeInfo(const QString &name)
                  : molname(name), nats(0)
{}

/** Construct a MoleculeInfo for an unnamed molecule containing the supplied
    list of residues */
template<class IDX>
SIRE_OUTOFLINE_TEMPLATE
MoleculeInfo<IDX>::MoleculeInfo(const QList< ResidueInfo<IDX> > &resinfos)
                  : molname(QObject::tr("unnamed")), nats(0)
{
    int nres = resinfos.count();
    for (int i=0; i<nres; ++i)
        add( resinfos.at(i) );
}

/** Construct a MoleculeInfo for the molecule called 'name' containing the supplied
    list of residues. */
template<class IDX>
SIRE_OUTOFLINE_TEMPLATE
MoleculeInfo<IDX>::MoleculeInfo(const QString &name, 
                                const QList< ResidueInfo<IDX> > &resinfos)
                  : molname(name), nats(0)
{
    int nres = resinfos.count();
    for (int i=0; i<nres; ++i)
        add( resinfos.at(i) );    
}
    
/** Copy constructor - should be quite fast as the object is mainly
    implicitly shared */
template<class IDX>
SIRE_OUTOFLINE_TEMPLATE
MoleculeInfo<IDX>::MoleculeInfo(const MoleculeInfo<IDX> &other)
                  : residus(other.residus), resnums(other.resnums),
                    idx2resnum(other.idx2resnum), molname(other.molname),
                    nats(other.nats)
{}
    
/** Destructor */
template<class IDX>
SIRE_OUTOFLINE_TEMPLATE
MoleculeInfo<IDX>::~MoleculeInfo()
{}
    
/** Assignment operator - should be quite fast as the object is mainly 
    implicitly shared */
template<class IDX>
SIRE_OUTOFLINE_TEMPLATE
MoleculeInfo<IDX>& MoleculeInfo<IDX>::operator=(const MoleculeInfo<IDX> &other)
{
    residus = other.residus;
    resnums = other.resnums;
    idx2resnum = other.idx2resnum;
    molname = other.molname;
    nats = other.nats;
    
    return *this;
}
    
/** Comparison operator */
template<class IDX>
SIRE_OUTOFLINE_TEMPLATE
bool MoleculeInfo<IDX>::operator==(const MoleculeInfo<IDX> &other) const
{
    return molname == other.molname and nats == other.nats and
           resnums == other.resnums and residus == other.residus;
}

/** Comparison operator */
template<class IDX>
SIRE_OUTOFLINE_TEMPLATE
bool MoleculeInfo<IDX>::operator!=(const MoleculeInfo<IDX> &other) const
{
    return molname != other.molname or nats != other.nats or
           resnums != other.resnums or residus != other.residus;
}

/** Return the name of the molecule */
template<class IDX>
SIRE_INLINE_TEMPLATE
const QString& MoleculeInfo<IDX>::name() const
{
    return molname;
}

/** Return whether or not this molecule is empty (contains no residues
    or atoms) */
template<class IDX>
SIRE_INLINE_TEMPLATE
bool MoleculeInfo<IDX>::isEmpty() const
{
    return nats == 0 and resnums.count() == 0;
}

/** Return whether or not this molecule is null (e.g. == MoleculeInfo()) */
template<class IDX>
SIRE_INLINE_TEMPLATE
bool MoleculeInfo<IDX>::isNull() const
{
    return *this == MoleculeInfo<IDX>();
}

/** Return the ResidueInfo for the 'ith' residue in this molecule

    \throw SireError::invalid_index
*/
template<class IDX>
SIRE_OUTOFLINE_TEMPLATE
const ResidueInfo<IDX>& MoleculeInfo<IDX>::operator[](ResID i) const
{
    if (i > residus.count())
        throw SireError::invalid_index(QObject::tr(
            "Index error in MoleculeInfo: i == %1,  size() == %2")
                .arg(i.index()).arg(size()), CODELOC);
                
    return *(residus.find( resnums.at(i.index()) ));
}
    
/** Return the index of the atom with AtomIndex 'atom'.
    
    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
template<class IDX>
SIRE_OUTOFLINE_TEMPLATE
const IDX& MoleculeInfo<IDX>::operator[](const AtomIndex &atom) const
{
    return index(atom);
}

    
/** Return the index of the atom at index 'rsid'
    
    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
template<class IDX>
SIRE_OUTOFLINE_TEMPLATE
const IDX& MoleculeInfo<IDX>::operator[](const ResNumAtomID &rsid) const
{
    return index(rsid);
}

/** Synonym for operator[] 

    \throw SireError::invalid_index
*/
template<class IDX>
SIRE_INLINE_TEMPLATE
const ResidueInfo<IDX>& MoleculeInfo<IDX>::at(ResID i) const
{
    return operator[](i);
}

/** Synonym for operator[]

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
template<class IDX>
SIRE_INLINE_TEMPLATE
const IDX& MoleculeInfo<IDX>::at(const AtomIndex &atom) const
{
    return operator[](atom);
}

/** Synonym for operator[]

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
template<class IDX>
SIRE_INLINE_TEMPLATE
const IDX& MoleculeInfo<IDX>::at(const ResNumAtomID &rsid) const
{
    return operator[](rsid);
}

/** Check that this molecule contains a residue with number 'resnum'. Throw
    an exception if it doesn't.
    
    \throw SireMol::missing_residue
*/
template<class IDX>
SIRE_OUTOFLINE_TEMPLATE
void MoleculeInfo<IDX>::checkResidue(ResNum resnum) const
{
    if (not residus.contains(resnum))
        throw SireMol::missing_residue(QObject::tr(
            "No residue with number %1 in molecule \"%2\"")
                .arg(resnum.toString(),name()), CODELOC);
                
}
    
/** Return the index for the atom with index 'atm'

    \throw SireMol::missing_atom
*/
template<class IDX>
SIRE_OUTOFLINE_TEMPLATE
const IDX& MoleculeInfo<IDX>::index(const AtomIndex &atm) const
{
    if (not residus.contains(atm.resNum()))
        throw SireMol::missing_atom(QObject::tr(
            "No atom \"%1\" in molecule \"%2\" as there is no residue with "
            "number %3 in this molecule!")
                .arg(atm.toString(),name(),atm.resNum().toString()), CODELOC);
                
    return residus.value(atm.resNum()).index(atm.name());
}

/** Return the index for the 'ith' atom in the molecule.

    \throw SireError::invalid_index
*/
template<class IDX>
SIRE_OUTOFLINE_TEMPLATE
const IDX& MoleculeInfo<IDX>::index(AtomID i) const
{
    if (i >= nats)
        throw SireError::invalid_index(QObject::tr(
            "Invalid index in MoleculeInfo. i == %1 while nAtoms() == %2")
                .arg(i.index()).arg(nats), CODELOC);
                
    //find the nearest index to 'i'
    QMap<AtomID,ResNum>::const_iterator it = idx2resnum.lowerBound(i);

    BOOST_ASSERT( it != idx2resnum.end() ); //this should be impossible...
    
    //get the residueinfo that contains this index...
    const ResidueInfo<IDX> &res = residus.value(it.value());
    
    //ok - it.key() contains the highest index for this residue
    //If we subtract it.key() from i that will convert i into a reversed
    //index into the residueinfo
    i -= it.key().index();
    
    //convert the reversed index into a real index by adding
    //it to 'natoms' in the residue (remembering that i is negative)
    i += res.nAtoms();
    
    return res.index(i);
}
    
/** Return the index for the atom with index 'resid'

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
template<class IDX>
SIRE_OUTOFLINE_TEMPLATE
const IDX& MoleculeInfo<IDX>::index(const ResNumAtomID &resid) const
{
    checkResidue( resid.resNum() );
    return residus[resid.resNum()].index(resid.atomID());
}

/** Return the index for the 'ith' atom in the residue with number 'resnum'

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
template<class IDX>
SIRE_INLINE_TEMPLATE
const IDX& MoleculeInfo<IDX>::index(ResNum resnum, AtomID i) const
{
    return this->at( ResNumAtomID(resnum,i) );
}

/** Return the AtomIndex of the 'ith' atom in the Molecule 

    \throw SireError::invalid_index
*/
template<class IDX>
SIRE_OUTOFLINE_TEMPLATE
AtomIndex MoleculeInfo<IDX>::atom(AtomID i) const
{
    if (i >= nats)
        throw SireError::invalid_index(QObject::tr(
            "Invalid index in MoleculeInfo. i == %1 while size() == %2")
                .arg(i.index()).arg(size()), CODELOC);
                
    //find the nearest index to 'i'
    QMap<AtomID,ResNum>::const_iterator it = idx2resnum.lowerBound(i);

    BOOST_ASSERT( it != idx2resnum.end() ); //this should be impossible...
    
    //get the residueinfo that contains this index...
    const ResidueInfo<IDX> &res = residus.value(it.value());
    
    //ok - it.key() contains the highest index for this residue
    //If we subtract it.key() from i that will convert i into a reversed
    //index into the residueinfo
    i -= it.key().index();

    return AtomIndex(res.atomNames().at(i.index()), res.resNum());
}
    
/** Return the AtomIndex of the ith atom in the residue with residue number 
    'resnum'.
    
    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
template<class IDX>
SIRE_OUTOFLINE_TEMPLATE
AtomIndex MoleculeInfo<IDX>::atom(ResNum resnum, AtomID id) const
{
    return residue(resnum).atom(id);
}
    
/** Return the AtomIndex of the atom with index 'rsid'

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
template<class IDX>
SIRE_INLINE_TEMPLATE
AtomIndex MoleculeInfo<IDX>::atom(const ResNumAtomID &rsid) const
{
    return atom(rsid.resNum(), rsid.atomID());
}
    
/** Return the AtomIndex objects for a whole load of indexes 

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
template<class IDX>
SIRE_OUTOFLINE_TEMPLATE
QSet<AtomIndex> MoleculeInfo<IDX>::getAtoms( const QSet<ResNumAtomID>& rsids) const
{
    QSet<AtomIndex> atms;
    atms.reserve(rsids.count());
    
    for (QSet<ResNumAtomID>::const_iterator it = rsids.begin();
         it != rsids.end();
         ++it)
    {
        atms.insert( atom(*it) );
    }
    
    return atms;
}

/** Return the AtomIndex objects fro a whole load of indexes

    \throw SireError::invalid_index
*/
template<class IDX>
SIRE_OUTOFLINE_TEMPLATE
QSet<AtomIndex> MoleculeInfo<IDX>::getAtoms( const QSet<AtomID> &idxs ) const
{
    QSet<AtomIndex> atms;
    atms.reserve(idxs.count());
    
    for (QSet<AtomID>::const_iterator it = idxs.begin();
         it != idxs.end();
         ++it)
    {
        atms.insert( atom(*it) );
    }
    
    return atms;
}
   
/** Return a string representation of this MoleculeInfo */
template<class IDX>
SIRE_OUTOFLINE_TEMPLATE
QString MoleculeInfo<IDX>::toString() const
{
    return QObject::tr("Molecule(\"%1\") : nAtoms() == %2, nResidues() == %3")
                        .arg(name()).arg(nAtoms()).arg(nResidues());
}
    
/** Return the signature for this molecule - this can be used to see if 
    two info objects describe the same molecule (have the same molecule,
    residue and atom names, and same residues and atoms) */
template<class IDX>
SIRE_OUTOFLINE_TEMPLATE
MoleculeSignature MoleculeInfo<IDX>::signature() const
{
    QHash<ResidueID,QStringSet> atmnames;
    
    //get the atom names for each residue
    QHashIterator< ResNum,ResidueInfo<IDX> > it(residus);
    
    while(it.hasNext())
    {
        it.next();
        atmnames.insert(it.value().ID(), it.value().atomNames().toSet());
    }
    
    return MoleculeSignature(molname, atmnames);
}
    
/** Return the number of residues in the Molecule */
template<class IDX>
SIRE_INLINE_TEMPLATE
int MoleculeInfo<IDX>::nResidues() const
{
    return residus.count();
}

/** Synonym for nResidues() */
template<class IDX>
SIRE_INLINE_TEMPLATE
int MoleculeInfo<IDX>::size() const
{
    return nResidues();
}

/** Synonym for nResidues() */
template<class IDX>
SIRE_INLINE_TEMPLATE
int MoleculeInfo<IDX>::count() const
{
    return nResidues();
}

/** Return the number of atoms in the molecule */
template<class IDX>
SIRE_INLINE_TEMPLATE
int MoleculeInfo<IDX>::nAtoms() const
{
    return nats;
}

/** Return the number of atoms in the residue with number 'resnum' */
template<class IDX>
SIRE_INLINE_TEMPLATE
int MoleculeInfo<IDX>::nAtoms(ResNum resnum) const
{
    checkResidue(resnum);
    return residus.value(resnum).nAtoms();
}

/** Add a new residue to the molecule. Note that you can't
    add duplicate residues!
     
    \throw SireMol::duplicate_residue
*/
template<class IDX>
SIRE_OUTOFLINE_TEMPLATE
void MoleculeInfo<IDX>::add(const ResidueInfo<IDX> &resinfo)
{
    if (residus.contains(resinfo.resNum()))
        throw SireMol::duplicate_residue(QObject::tr(
            "Cannot add residue \"%1(%2)\" to molecule \"%3\" as "
            "it already contains a residue with this number, \"%4(%5)\"")
                .arg(resinfo.name(),resinfo.resNum().toString(),name())
                .arg(residus[resinfo.resNum()].name(),resinfo.resNum().toString()), 
                        CODELOC);
                
    resnums.append(resinfo.resNum());
    residus.insert(resinfo.resNum(), resinfo);

    nats += resinfo.nAtoms();
    idx2resnum.insert(nats, resinfo.resNum());
}

/** Recalculate the indexing from 'i' to a specific atom in the molecule. This
    function is order(n) over the n residues. */
template<class IDX>
SIRE_OUTOFLINE_TEMPLATE
void MoleculeInfo<IDX>::recalculate()
{
    idx2resnum.clear();
    nats = 0;
    
    foreach( ResNum resnum, resnums )
    {
        nats += residus[resnum].nAtoms();
        idx2resnum.insert(nats,resnum);
    }
}

/** Remove the residue with number 'resnum' from this molecule. This can be a little
    slow as indexes will need to be recalculated. This does nothing if there is  
    no such residue in the molecule. */
template<class IDX>
SIRE_OUTOFLINE_TEMPLATE
void MoleculeInfo<IDX>::remove(ResNum resnum)
{
    if (residus.contains(resnum))
    {
        residus.remove(resnum);
        resnums.removeAll(resnum);
        
        //recalculate the indexes...
        recalculate();
    }
}
    
/** Return the list of residue numbers of residues in this molecule. The residue
    numbers are ordered in the same order as the residues were added to the
    molecule. */
template<class IDX>
SIRE_INLINE_TEMPLATE
const ResNumList& MoleculeInfo<IDX>::residueNumbers() const
{
    return resnums;
}

/** Return the list of residue numbers of residues called 'resname'. The residue
    numbers are ordered in the same order as the residues were added to the molecule.
    This returns an empty list if there are no residues with this residue name. */
template<class IDX>
SIRE_INLINE_TEMPLATE
ResNumList MoleculeInfo<IDX>::residueNumbers(const QString &resname) const
{
    ResNumList rnums;
    
    foreach( ResNum resnum, resnums )
    {
        if (residus.value(resnum).name() == resname)
            rnums.append(resnum);
    }
    
    return rnums;
}

/** Return the list of residue names of residues in this molecule. The residue
    names are returned in the same order as the numbers of residues are returned
    by the residueNumbers() function. */
template<class IDX>
SIRE_OUTOFLINE_TEMPLATE
QStringList MoleculeInfo<IDX>::residueNames() const
{
    QStringList resnames;
    foreach( ResNum resnum, resnums )
        resnames.append( residus[resnum].resName() );
        
    return resnames;
}

/** Return the set of ResName residue IDs in the molecule. This 
    returns the IDs in the same order as the residues were added to 
    this Molecule */
template<class IDX>
SIRE_OUTOFLINE_TEMPLATE
ResidueIDSet MoleculeInfo<IDX>::residueIDs() const
{
    ResidueIDSet resids;

    foreach( ResNum resnum, resnums )
    {
        resids.insert( residus.value(resnum).ID() );
    }
    
    return resids;
}

/** Return the ResidueInfo for the residue with number 'resnum' 

    \throw SireMol::missing_residue
*/
template<class IDX>
SIRE_OUTOFLINE_TEMPLATE
const ResidueInfo<IDX>& MoleculeInfo<IDX>::residue(ResNum resnum) const
{
    checkResidue(resnum);
    return *(residus.find(resnum));
}

/** Return whether or not the molecule contains the atom 'atm' */
template<class IDX>
SIRE_INLINE_TEMPLATE
bool MoleculeInfo<IDX>::contains(const AtomIndex &atm) const
{
    return residus.contains(atm.resNum()) 
                and residus[atm.resNum()].contains(atm.name());
}

/** Return whether or not the molecule contains the residue with number 'resnum' */
template<class IDX>
SIRE_INLINE_TEMPLATE
bool MoleculeInfo<IDX>::contains(ResNum resnum) const
{
    return residus.contains(resnum);
}

/** Return whether or not the molecule contains the atom with index 'idx' */
template<class IDX>
SIRE_OUTOFLINE_TEMPLATE
bool MoleculeInfo<IDX>::contains(const ResNumAtomID &idx) const
{
    return residus.contains( idx.resNum() ) and
           residus[idx.resNum()].contains( idx.atomID() );
}

const SireStream::MagicID molinfo_magic = SireStream::getMagic("SireMol::MoleculeInfo");

}

/** Serialise to a binary datastream */
template<class IDX>
SIRE_OUTOFLINE_TEMPLATE
QDataStream& operator<<(QDataStream &ds, const SireMol::MoleculeInfo<IDX> &minfo)
{
    SireStream::writeHeader(ds, SireMol::molinfo_magic, 1)
              << minfo.residus << minfo.resnums
              << minfo.idx2resnum << minfo.molname << minfo.nats;
       
    return ds;
}

/**Deserialise from a binary datastream */
template<class IDX>
SIRE_OUTOFLINE_TEMPLATE
QDataStream& operator>>(QDataStream &ds, SireMol::MoleculeInfo<IDX> &minfo)
{
    SireStream::VersionID v = SireStream::readHeader(ds, SireMol::molinfo_magic,
                                                     "SireMol::MoleculeInfo");
                                                     
    if (v == 1)
    {                                     
        ds >> minfo.residus >> minfo.resnums >> minfo.idx2resnum
           >> minfo.molname >> minfo.nats;
    }
    else
        throw SireStream::version_error(v, "1", "SireMol::MoleculeInfo", CODELOC);
       
    return ds;
}

SIRE_END_HEADER

#endif
