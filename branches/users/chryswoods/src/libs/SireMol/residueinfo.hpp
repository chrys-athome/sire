#ifndef SIREMOL_RESIDUEINFO_HPP
#define SIREMOL_RESIDUEINFO_HPP

#include <QDataStream>

#include "residueinfo.h"

#include "SireError/errors.h"
#include "SireStream/datastream.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
template<class IDX>
class ResidueInfo;
}

template<class IDX>
QDataStream& operator<<(QDataStream&, const SireMol::ResidueInfo<IDX>&);
template<class IDX>
QDataStream& operator>>(QDataStream&, SireMol::ResidueInfo<IDX>&);

namespace SireMol
{

/**
  * This is a template class that contains metainformation about a residue 
  * (name, number and atom names) and allows that information to be used to 
  * obtain an index into data for each of the atoms. For example, this class, 
  * in the guise of ResidueInfo<CGAtomID>, is used by MoleculeData to hold the 
  * metainformation about each residue, and to provide a rapid search from the 
  * AtomIndex to the CutGroup containing the Atom's data. 
  *
  * @author Christopher Woods
  *
  */
template<class IDX>
class SIREMOL_EXPORT ResidueInfo : public ResidueInfoBase
{

friend QDataStream& ::operator<<<>(QDataStream&, const ResidueInfo<IDX>&);
friend QDataStream& ::operator>><>(QDataStream&, ResidueInfo<IDX>&);

public:
    ResidueInfo();
    ResidueInfo(const QString &resnam, ResNum resnum);
    
    ResidueInfo(const QString &resnam, ResNum resnum,
                const QHash<QString, IDX> &atmidxs);
    
    ResidueInfo(const ResidueInfo<IDX> &other);
    
    ~ResidueInfo();

    ResidueInfo<IDX>& operator=(const ResidueInfo<IDX> &other);

    bool operator==(const ResidueInfo<IDX> &other) const;
    bool operator!=(const ResidueInfo<IDX> &other) const;

    void add(const QString &atmname, const IDX &index);

    const QVector<IDX>& indicies() const;

    const IDX& operator[](uint i) const;
    const IDX& operator[](AtomID i) const;
    const IDX& operator[](const QString &atmnam) const;
    
    const IDX& at(AtomID i) const;
    const IDX& at(uint i) const;
    
    const IDX& index(AtomID i) const;
    const IDX& index(uint i) const;
    const IDX& index(const QString &atmnam) const;
    
    void clear();
    void squeeze();
    
    void reserve(int n);
    
private:

    /** Array of indexes to the atom data. 
        The ResidueInfoBase class provides the name hash which
        maps atom names to indexes into this array. */
    QVector<IDX> idxs;

};

/** Construct an empty ResidueInfo object */
template<class IDX>
SIRE_OUTOFLINE_TEMPLATE
ResidueInfo<IDX>::ResidueInfo() : ResidueInfoBase()
{}

/** Construct an empty ResidueInfo object for the residue called 'resnam'
    with number 'resnum' */
template<class IDX>
SIRE_OUTOFLINE_TEMPLATE
ResidueInfo<IDX>::ResidueInfo(const QString &resnam, ResNum resnum) 
                 : ResidueInfoBase(resnam,resnum)
{}

/** Construct a ResidueInfo object for the residue called 'resnam' with 
    number 'resnum' and containing atoms in 'atmidxs' with data held in
    the place according to the contained indicies in the hash */
template<class IDX>
SIRE_OUTOFLINE_TEMPLATE
ResidueInfo<IDX>::ResidueInfo(const QString &resnam, ResNum resnum,
                              const QHash<QString, IDX> &atmidxs)
                 : ResidueInfoBase(resnam,resnum)
{
    this->reserve(atmidxs.count());

    QHashIterator<QString,IDX> it(atmidxs);
    
    while( it.hasNext() )
    {
        it.next();
        this->add(it.key(),it.value());
    }
}
    
/** Copy constructor - quite fast as the data in this class is implicitly
    shared. */
template<class IDX>
SIRE_OUTOFLINE_TEMPLATE
ResidueInfo<IDX>::ResidueInfo(const ResidueInfo<IDX> &other)
                 : ResidueInfoBase(other), idxs(other.idxs)
{}
    
/** Destructor */
template<class IDX>
SIRE_OUTOFLINE_TEMPLATE
ResidueInfo<IDX>::~ResidueInfo()
{}

/** Assignment operator - quite fast as the data in this class is 
    implicitly shared. */
template<class IDX>
SIRE_OUTOFLINE_TEMPLATE
ResidueInfo<IDX>& ResidueInfo<IDX>::operator=(const ResidueInfo<IDX> &other)
{
    ResidueInfo::operator=(other);
    idxs = other.idxs;
    return *this;
}

/** Comparison operator */
template<class IDX>
SIRE_OUTOFLINE_TEMPLATE
bool ResidueInfo<IDX>::operator==(const ResidueInfo<IDX> &other) const
{
    return ResidueInfo::operator==(other) and idxs == other.idxs;
}

/** Comparison operator */
template<class IDX>
SIRE_OUTOFLINE_TEMPLATE
bool ResidueInfo<IDX>::operator!=(const ResidueInfo<IDX> &other) const
{
    return ResidueInfo::operator!=(other) or idxs != other.idxs;
}

/** Add a new atom name and index to this residue. An exception will
    be thrown if there is already an atom with this name in the residue.
    
    \throw SireMol::duplicate_atom
*/
template<class IDX>
SIRE_OUTOFLINE_TEMPLATE
void ResidueInfo<IDX>::add(const QString &atmname, const IDX &index)
{
    ResidueInfoBase::addAtom(atmname, idxs.count());
    idxs.append(index);
}

/** Return all of the indicies of atoms in this residue. */
template<class IDX>
SIRE_INLINE_TEMPLATE
const QVector<IDX>& ResidueInfo<IDX>::indicies() const
{
    return idxs;
}

/** Return the index of the 'ith' atom in this residue. 

    \throw SireError::invalid_index
*/
template<class IDX>
SIRE_INLINE_TEMPLATE
const IDX& ResidueInfo<IDX>::operator[](uint i) const
{
    if ( i >= uint(idxs.count()) )
        throw SireError::invalid_index(QObject::tr(
            "Invalid index in ResidueInfo: i == %1, but size() == %2")
                .arg(i).arg(idxs.count()), CODELOC);
                
    return idxs.constData()[i];
}

/** Return the index of the 'ith' atom in this residue. 

    \throw SireError::invalid_index
*/
template<class IDX>
SIRE_INLINE_TEMPLATE
const IDX& ResidueInfo<IDX>::operator[](AtomID i) const
{
    return this->operator[](i.index());
}

/** Synonym for operator[](int)

    \throw SireError::invalid_index
*/
template<class IDX>
SIRE_INLINE_TEMPLATE
const IDX& ResidueInfo<IDX>::at(uint i) const
{
    return operator[](i);
}

/** Synonym for operator[](int)

    \throw SireError::invalid_index
*/
template<class IDX>
SIRE_INLINE_TEMPLATE
const IDX& ResidueInfo<IDX>::at(AtomID i) const
{
    return operator[](i);
}

/** Return the index of the atom called 'atmnam'. This will throw 
    an exception if there is no atom with this name in this residue.
    
    \throw SireMol::missing_atom
*/
template<class IDX>
SIRE_INLINE_TEMPLATE
const IDX& ResidueInfo<IDX>::operator[](const QString &atmnam) const
{
    int i = name2idx.value(atmnam, -1);
    
    if (i < 0)
        throw SireMol::missing_atom(QObject::tr(
            "No atom called \"%1\" in residue \"%2(%3)\"")
                .arg(atmnam,resName(),resNum().toString()), CODELOC);
                
    return idxs.constData()[i];
}
    
/** Convienient version of operator[](int i)

    \throw SireError::invalid_index
*/
template<class IDX>
SIRE_INLINE_TEMPLATE
const IDX& ResidueInfo<IDX>::index(uint i) const
{
    return operator[](i);
}

/** Convienient version of operator[](int i)

    \throw SireError::invalid_index
*/
template<class IDX>
SIRE_INLINE_TEMPLATE
const IDX& ResidueInfo<IDX>::index(AtomID i) const
{
    return operator[](i);
}

/** Convienient version of operator[](const QString&)

    \throw SireMol::missing_atom
*/
template<class IDX>
SIRE_INLINE_TEMPLATE
const IDX& ResidueInfo<IDX>::index(const QString &atmnam) const
{
    return operator[](atmnam);
}

/** Completely clear this ResidueInfo */
template<class IDX>
SIRE_OUTOFLINE_TEMPLATE
void ResidueInfo<IDX>::clear()
{
    ResidueInfoBase::clear();
    idxs.clear();
}

/** Minimise the memory usage of this object */
template<class IDX>
SIRE_OUTOFLINE_TEMPLATE
void ResidueInfo<IDX>::squeeze()
{
    ResidueInfoBase::squeeze();
    idxs.squeeze();
}

/** Reserve space for 'n' atoms. This can improve the efficiency of 
    adding atoms to the ResidueInfo *if* you know how many atoms will
    be added ahead of time. */
template<class IDX>
SIRE_OUTOFLINE_TEMPLATE
void ResidueInfo<IDX>::reserve(int n)
{
    ResidueInfoBase::reserve(n);
    idxs.reserve(n);
}

const SireStream::MagicID resinfo_magic = SireStream::getMagic("SireMol::ResidueInfo");

}

/** Serialise to a binary data stream */
template<class IDX>
SIRE_OUTOFLINE_TEMPLATE
QDataStream& operator<<(QDataStream &ds, const SireMol::ResidueInfo<IDX> &rinfo)
{
    SireStream::writeHeader(ds, SireMol::resinfo_magic, 1)
            << rinfo.idxs << static_cast<const SireMol::ResidueInfoBase&>(rinfo);
       
    return ds;
}

/** Deserialise from a binary data stream */
template<class IDX>
SIRE_OUTOFLINE_TEMPLATE
QDataStream& operator>>(QDataStream &ds, SireMol::ResidueInfo<IDX> &rinfo)
{
    SireStream::VersionID v = SireStream::readHeader(ds, SireMol::resinfo_magic,
                                                     "SireMol::ResidueInfo");
                                                     
    if (v == 1)
    {
        ds >> rinfo.idxs >> static_cast<SireMol::ResidueInfoBase&>(rinfo);
    }
    else
        throw SireStream::version_error(v, "1", "SireMol::ResidueInfo", CODELOC);
    
    return ds;
}

SIRE_END_HEADER

#endif
