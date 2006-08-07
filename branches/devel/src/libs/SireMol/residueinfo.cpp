
#include "residueinfo.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireMol;

static const RegisterMetaType<ResidueInfoBase> 
                    r_resinfobase("SireMol::ResidueInfoBase", MAGIC_ONLY);

/** Serialise to a binary data stream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const ResidueInfoBase &rinfo)
{
    writeHeader(ds, r_resinfobase, 1) << rinfo.resid << rinfo.name2idx 
                                      << rinfo.atmnames;
                                      
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, ResidueInfoBase &rinfo)
{
    VersionID v = readHeader(ds, r_resinfobase);
    
    if (v == 1)
    {
        ds >> rinfo.resid >> rinfo.name2idx >> rinfo.atmnames;
    }
    else
        throw version_error(v, "1", r_resinfobase, CODELOC);
    
    return ds;
}

/** Construct an empty ResidueInfoBase */
ResidueInfoBase::ResidueInfoBase() : resid("NULL",0)
{}

/** Construct a ResidueInfoBase for a residue called 'resnam' with number 'resnum' */
ResidueInfoBase::ResidueInfoBase(const QString &resnam, ResNum resnum)
                : resid(resnam,resnum)
{}
    
/** Copy constructor - quite fast as mainly implicitly shared */
ResidueInfoBase::ResidueInfoBase(const ResidueInfoBase &other)
                : resid(other.resid), name2idx(other.name2idx), atmnames(other.atmnames)
{}
    
/** Destructor */
ResidueInfoBase::~ResidueInfoBase()
{}
    
/** Assignment operator - quite fast as mainly implicitly shared */
ResidueInfoBase& ResidueInfoBase::operator=(const ResidueInfoBase &other)
{
    resid = other.resid;
    name2idx = other.name2idx;
    atmnames = other.atmnames;
    return *this;
}
    
/** Comparison operator */
bool ResidueInfoBase::operator==(const ResidueInfoBase &other) const
{
    return resid == other.resid and name2idx == other.name2idx;
}
    
/** Comparison operator */
bool ResidueInfoBase::operator!=(const ResidueInfoBase &other) const
{
    return resid != other.resid or name2idx != other.name2idx;
}

/** Return the number of atoms in the residue */
int ResidueInfoBase::nAtoms() const
{
    return atmnames.count();
}

/** Synonym for nAtoms() */
int ResidueInfoBase::size() const
{
    return nAtoms();
}

/** Synonym for nAtoms() */
int ResidueInfoBase::count() const
{
    return nAtoms();
}

/** Return a string representation of this residue */
QString ResidueInfoBase::toString() const
{
    int nats = nAtoms();
    return QObject::tr("ResidueInfo: \"%1\", nAtoms() == %2")
                        .arg(resid.toString()).arg(nats);
}
    
/** Return the index of the 'ith' atom in the residue 

    \throw SireError::invalid_index
*/
AtomIndex ResidueInfoBase::atom(AtomID i) const
{
    if (i >= nAtoms())
        throw SireError::invalid_index(QObject::tr(
            "Invalid index \"%1\" in residue \"%2-%3\" when nAtoms() == %4")
                .arg(i.index()).arg(resName(),resNum().toString()).arg(nAtoms()), CODELOC);
                
    return AtomIndex(atmnames.at(i.index()), resNum());
}
    
/** Set the name of the residue */
void ResidueInfoBase::setName(const QString &name)
{
    resid.setName(name);
}

/** Set the residue number */
void ResidueInfoBase::setNumber(ResNum resnum)
{
    resid.setNumber(resnum);
}
    
/** Clear this residue */
void ResidueInfoBase::clear()
{
    name2idx.clear();
    atmnames.clear();
}

/** Minimise the memory usage of this ResidueInfoBase */
void ResidueInfoBase::squeeze()
{
    name2idx.squeeze();
}
    
/** Reserve space for up to 'n' atoms. */
void ResidueInfoBase::reserve(int n)
{
    name2idx.reserve(n);
}

/** Add the atom called 'atmname' to the ResidueInfoBase, with the index
    in the ResidueInfo object at array index 'i' 
    
    \throw SireMol::duplicate_atom
*/
void ResidueInfoBase::addAtom(const QString &atmname, quint32 i)
{
    if (name2idx.contains(atmname))
        throw SireMol::duplicate_atom(QObject::tr(
            "Cannot add atom \"%1\" to residue \"%2(%3)\" as an atom by this "
            "name already exists!")
                .arg(atmname).arg(resName(),resNum().toString()), CODELOC);

    name2idx.insert(atmname, i);
    atmnames.append(atmname);
}
