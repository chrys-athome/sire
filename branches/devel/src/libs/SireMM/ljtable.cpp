
#include "qhash_siremol.h"

#include "ljtable.h"

#include "SireMol/molecule.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireMM;
using namespace SireDB;
using namespace SireMol;

/** Register this as a concrete class */
static const RegisterMetaType<LJTable> r_ljtable("SireMM::LJTable");

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const LJTable &table)
{
    writeHeader(ds, r_ljtable, 1) 
            << static_cast<const AtomTableT<LJParameter>&>(table);
            
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, LJTable &table)
{
    VersionID v = readHeader(ds, r_ljtable);
    
    if (v == 1)
    {
        ds >> static_cast<AtomTableT<LJParameter>&>(table);
    }
    else
        throw version_error(v, "1", r_ljtable, CODELOC);
    
    return ds;
}

/** Null Constructor */
LJTable::LJTable() : AtomTableT<LJParameter>()
{}

/** Construct a table to hold the ljs for the Molecule 'molecule' */
LJTable::LJTable(const Molecule &molecule)
            : AtomTableT<LJParameter>(molecule)
{}

/** Copy constructor - this is fast as implicit sharing is used */
LJTable::LJTable(const LJTable &other)
            : AtomTableT<LJParameter>(other)
{}

/** Destructor */
LJTable::~LJTable()
{}

/** Synonym for nParameters(...) */
int LJTable::nLJs() const
{
    return nParameters();
}

/** Synonym for nParameters(...) */
int LJTable::nLJs(ResNum resnum) const
{
    return nParameters(resnum);
}

/** Synonym for nParameters(...) */
int LJTable::nLJs(CutGroupID cgid) const
{
    return nParameters(cgid);
}

/** Synonym for getParameter(...) */
const LJParameter& LJTable::getLJ(const CGAtomID &cgid) const
{
    return getParameter(cgid);
}

/** Synonym for getParameter(...) */
const LJParameter& LJTable::getLJ(const ResNumAtomID &rsid) const
{
    return getParameter(rsid);
}

/** Synonym for getParameter(...) */
const LJParameter& LJTable::getLJ(const AtomIndex &atom) const
{
    return getParameter(atom);
}

/** Synonym for setParameter(...) */
void LJTable::setLJ(const CGAtomID &cgid, const LJParameter &lj)
{
    setParameter(cgid,lj);
}

/** Synonym for setParameter(...) */
void LJTable::setLJ(const ResNumAtomID &rsid, const LJParameter &lj)
{
    setParameter(rsid, lj);
}

/** Synonym for setParameter(...) */
void LJTable::setLJ(const AtomIndex &atom, const LJParameter &lj)
{
    setParameter(atom, lj);
}

/** Synonym for assignedParameter(...) */
bool LJTable::assignedLJ(const CGAtomID &cgid) const
{
    return assignedParameter(cgid);
}

/** Synonym for assignedParameter(...) */
bool LJTable::assignedLJ(const ResNumAtomID &rsid) const
{
    return assignedParameter(rsid);
}

/** Synonym for assignedParameter(...) */
bool LJTable::assignedLJ(const AtomIndex &atom) const
{
    return assignedParameter(atom);
}

/** Synonym for hasMissingParameter(...) */
bool LJTable::hasMissingLJs() const
{
    return hasMissingParameters();
}

/** Synonym for hasMissingParameter(...) */
bool LJTable::hasMissingLJs(ResNum resnum) const
{
    return hasMissingParameters(resnum);
}

/** Synonym for hasMissingParameter(...) */
bool LJTable::hasMissingLJs(CutGroupID cgid) const
{
    return hasMissingParameters(cgid);
}

/** Synonym for missingParameters(...) */
QSet<AtomIndex> LJTable::missingLJs() const
{
    return missingParameters();
}

/** Synonym for missingParameters(...) */
QSet<AtomIndex> LJTable::missingLJs(ResNum resnum) const
{
    return missingParameters(resnum);
}

/** Synonym for missingParameters(...) */
QSet<AtomIndex> LJTable::missingLJs(CutGroupID cgid) const
{
    return missingParameters(cgid);
}

/** Synonym for clear(...) */
void LJTable::clearLJs()
{
    clear();
}

/** Synonym for clear(...) */
void LJTable::clearLJ(const CGAtomID &cgid)
{
    clear(cgid);
}

/** Synonym for clear(...) */
void LJTable::clearLJ(const ResNumAtomID &rsid)
{
    clear(rsid);
}

/** Synonym for clear(...) */
void LJTable::clearLJ(const AtomIndex &atom)
{
    clear(atom);
}

/** Synonym for clear(...) */
void LJTable::clearLJs(ResNum resnum)
{
    clear(resnum);
}

/** Synonym for clear(...) */
void LJTable::clearLJs(CutGroupID cgid)
{
    clear(cgid);
}

/** Synonym for parameters(...) */
QVector<LJParameter> LJTable::ljs() const
{
    return parameters();
}

/** Synonym for parameters(...) */
QVector<LJParameter> LJTable::ljs(ResNum resnum) const
{
    return parameters(resnum);
}

/** Synonym for parameters(...) */
QVector<LJParameter> LJTable::ljs(CutGroupID cgid) const
{
    return parameters(cgid);
}

/** Synonym for parametersByCutGroup(...) */
GroupedVector<CGAtomID,LJParameter> LJTable::ljsByCutGroup() const
{
    return parametersByCutGroup();
}

/** Synonym for parametersByCutGroup(...) */
GroupedVector<CGAtomID,LJParameter> LJTable::ljsByCutGroup(
                                                const QSet<CutGroupID> &cgids) const
{
    return parametersByCutGroup(cgids);
}

/** Synonym for parametersByResidue(...) */
GroupedVector<ResNumAtomID,LJParameter> LJTable::ljsByResidue() const
{
    return parametersByResidue();
}

/** Synonym for parametersByResidue(...) */
GroupedVector<ResNumAtomID,LJParameter> LJTable::ljsByResidue(
                                                const QSet<ResNum> &resnums) const
{
    return parametersByResidue(resnums);
}
