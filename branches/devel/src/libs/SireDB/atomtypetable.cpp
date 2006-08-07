
#include "SireMol/qhash_siremol.h"

#include <QDebug>

#include "atomtypetable.h"

#include "SireMol/molecule.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireDB;
using namespace SireMol;

/** Register this class so that it can be used with QVariant */
static const RegisterMetaType<AtomTypeTable> r_atomtypetable("SireDB::AtomTypeTable");

/** Serialise to a binary data stream */
QDataStream SIREDB_EXPORT &operator<<(QDataStream &ds, const AtomTypeTable &table)
{
    writeHeader(ds, r_atomtypetable, 1) 
          << static_cast<const AtomTableT<AtomType>&>(table);
          
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREDB_EXPORT &operator>>(QDataStream &ds, AtomTypeTable &table)
{
    VersionID v = readHeader(ds, r_atomtypetable);
    
    if (v == 1)
    {
        ds >> static_cast<AtomTableT<AtomType>&>(table);
    }
    else
        throw version_error(v, "1", r_atomtypetable, CODELOC);
    
    return ds;
}

/** Construct an empty AtomTypeTable */
AtomTypeTable::AtomTypeTable() : AtomTableT<AtomType>()
{}

/** Construct a table to hold the parameters of the passed molecule */
AtomTypeTable::AtomTypeTable(const Molecule &molecule) 
              : AtomTableT<AtomType>(molecule)
{}

/** Copy constructor. As this class uses implicitly shared data, this copy will be
    very fast and very memory efficient.  */
AtomTypeTable::AtomTypeTable(const AtomTypeTable &other) 
              : AtomTableT<AtomType>(other)
{}

/** Destructor */
AtomTypeTable::~AtomTypeTable()
{}

/** Synonym for nParameters(...) */
int AtomTypeTable::nAtomTypes() const
{
    return nParameters();
}

/** Synonym for nParameters(...) */
int AtomTypeTable::nAtomTypes(ResNum resnum) const
{
    return nParameters(resnum);
}

/** Synonym for nParameters(...) */
int AtomTypeTable::nAtomTypes(CutGroupID cgid) const
{
    return nParameters(cgid);
}

/** Synonym for getParameter(...) */
const AtomType& AtomTypeTable::getAtomType(const CGAtomID &cgid) const
{
    return getParameter(cgid);
}

/** Synonym for getParameter(...) */
const AtomType& AtomTypeTable::getAtomType(const ResNumAtomID &rsid) const
{
    return getParameter(rsid);
}

/** Synonym for getParameter(...) */
const AtomType& AtomTypeTable::getAtomType(const AtomIndex &atom) const
{
    return getParameter(atom);
}

/** Synonym for setParameter(...) */
void AtomTypeTable::setAtomType(const CGAtomID &cgid, const AtomType &atomtype)
{
    setParameter(cgid,atomtype);
}

/** Synonym for setParameter(...) */
void AtomTypeTable::setAtomType(const ResNumAtomID &rsid, const AtomType &atomtype)
{
    setParameter(rsid, atomtype);
}

/** Synonym for setParameter(...) */
void AtomTypeTable::setAtomType(const AtomIndex &atom, const AtomType &atomtype)
{
    setParameter(atom, atomtype);
}

/** Synonym for assignedParameter(...) */
bool AtomTypeTable::assignedAtomType(const CGAtomID &cgid) const
{
    return assignedParameter(cgid);
}

/** Synonym for assignedParameter(...) */
bool AtomTypeTable::assignedAtomType(const ResNumAtomID &rsid) const
{
    return assignedParameter(rsid);
}

/** Synonym for assignedParameter(...) */
bool AtomTypeTable::assignedAtomType(const AtomIndex &atom) const
{
    return assignedParameter(atom);
}

/** Synonym for hasMissingParameter(...) */
bool AtomTypeTable::hasMissingAtomTypes() const
{
    return hasMissingParameters();
}

/** Synonym for hasMissingParameter(...) */
bool AtomTypeTable::hasMissingAtomTypes(ResNum resnum) const
{
    return hasMissingParameters(resnum);
}

/** Synonym for hasMissingParameter(...) */
bool AtomTypeTable::hasMissingAtomTypes(CutGroupID cgid) const
{
    return hasMissingParameters(cgid);
}

/** Synonym for missingParameters(...) */
QSet<AtomIndex> AtomTypeTable::missingAtomTypes() const
{
    return missingParameters();
}

/** Synonym for missingParameters(...) */
QSet<AtomIndex> AtomTypeTable::missingAtomTypes(ResNum resnum) const
{
    return missingParameters(resnum);
}

/** Synonym for missingParameters(...) */
QSet<AtomIndex> AtomTypeTable::missingAtomTypes(CutGroupID cgid) const
{
    return missingParameters(cgid);
}

/** Synonym for clear(...) */
void AtomTypeTable::clearAtomTypes()
{
    clear();
}

/** Synonym for clear(...) */
void AtomTypeTable::clearAtomType(const CGAtomID &cgid)
{
    clear(cgid);
}

/** Synonym for clear(...) */
void AtomTypeTable::clearAtomType(const ResNumAtomID &rsid)
{
    clear(rsid);
}

/** Synonym for clear(...) */
void AtomTypeTable::clearAtomType(const AtomIndex &atom)
{
    clear(atom);
}

/** Synonym for clear(...) */
void AtomTypeTable::clearAtomTypes(ResNum resnum)
{
    clear(resnum);
}

/** Synonym for clear(...) */
void AtomTypeTable::clearAtomTypes(CutGroupID cgid)
{
    clear(cgid);
}

/** Synonym for parameters(...) */
QVector<AtomType> AtomTypeTable::atomTypes() const
{
    return parameters();
}

/** Synonym for parameters(...) */
QVector<AtomType> AtomTypeTable::atomTypes(ResNum resnum) const
{
    return parameters(resnum);
}

/** Synonym for parameters(...) */
QVector<AtomType> AtomTypeTable::atomTypes(CutGroupID cgid) const
{
    return parameters(cgid);
}

/** Synonym for parametersByCutGroup(...) */
GroupedVector<CGAtomID,AtomType> AtomTypeTable::atomTypesByCutGroup() const
{
    return parametersByCutGroup();
}

/** Synonym for parametersByCutGroup(...) */
GroupedVector<CGAtomID,AtomType> AtomTypeTable::atomTypesByCutGroup(
                                                const QSet<CutGroupID> &cgids) const
{
    return parametersByCutGroup(cgids);
}

/** Synonym for parametersByResidue(...) */
GroupedVector<ResNumAtomID,AtomType> AtomTypeTable::atomTypesByResidue() const
{
    return parametersByResidue();
}

/** Synonym for parametersByResidue(...) */
GroupedVector<ResNumAtomID,AtomType> AtomTypeTable::atomTypesByResidue(
                                                const QSet<ResNum> &resnums) const
{
    return parametersByResidue(resnums);
}
