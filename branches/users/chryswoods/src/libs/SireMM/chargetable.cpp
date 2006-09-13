
#include "SireMol/qhash_siremol.h"

#include "chargetable.h"

#include "SireMol/molecule.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireMM;
using namespace SireDB;
using namespace SireMol;

/** Register this as a concrete class */
static const RegisterMetaType<ChargeTable> r_chargetable("SireMM::ChargeTable");

/** Serialise to a binary data stream */
QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const ChargeTable &table)
{
    writeHeader(ds, r_chargetable, 1) 
            << static_cast<const AtomTableT<ChargeParameter>&>(table);
            
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, ChargeTable &table)
{
    VersionID v = readHeader(ds, r_chargetable);
    
    if (v == 1)
    {
        ds >> static_cast<AtomTableT<ChargeParameter>&>(table);
    }
    else
        throw version_error(v, "1", r_chargetable, CODELOC);
    
    return ds;
}

/** Null Constructor */
ChargeTable::ChargeTable() : AtomTableT<ChargeParameter>()
{}

/** Construct a table to hold the charges for the Molecule 'molecule' */
ChargeTable::ChargeTable(const Molecule &molecule)
            : AtomTableT<ChargeParameter>(molecule)
{}

/** Copy constructor - this is fast as implicit sharing is used */
ChargeTable::ChargeTable(const ChargeTable &other)
            : AtomTableT<ChargeParameter>(other)
{}

/** Destructor */
ChargeTable::~ChargeTable()
{}

/** Synonym for nParameters(...) */
int ChargeTable::nCharges() const
{
    return nParameters();
}

/** Synonym for nParameters(...) */
int ChargeTable::nCharges(ResNum resnum) const
{
    return nParameters(resnum);
}

/** Synonym for nParameters(...) */
int ChargeTable::nCharges(CutGroupID cgid) const
{
    return nParameters(cgid);
}

/** Synonym for getParameter(...) */
const ChargeParameter& ChargeTable::getCharge(const CGAtomID &cgid) const
{
    return getParameter(cgid);
}

/** Synonym for getParameter(...) */
const ChargeParameter& ChargeTable::getCharge(const ResNumAtomID &rsid) const
{
    return getParameter(rsid);
}

/** Synonym for getParameter(...) */
const ChargeParameter& ChargeTable::getCharge(const AtomIndex &atom) const
{
    return getParameter(atom);
}

/** Synonym for setParameter(...) */
void ChargeTable::setCharge(const CGAtomID &cgid, const ChargeParameter &charge)
{
    setParameter(cgid,charge);
}

/** Synonym for setParameter(...) */
void ChargeTable::setCharge(const ResNumAtomID &rsid, const ChargeParameter &charge)
{
    setParameter(rsid, charge);
}

/** Synonym for setParameter(...) */
void ChargeTable::setCharge(const AtomIndex &atom, const ChargeParameter &charge)
{
    setParameter(atom, charge);
}

/** Synonym for assignedParameter(...) */
bool ChargeTable::assignedCharge(const CGAtomID &cgid) const
{
    return assignedParameter(cgid);
}

/** Synonym for assignedParameter(...) */
bool ChargeTable::assignedCharge(const ResNumAtomID &rsid) const
{
    return assignedParameter(rsid);
}

/** Synonym for assignedParameter(...) */
bool ChargeTable::assignedCharge(const AtomIndex &atom) const
{
    return assignedParameter(atom);
}

/** Synonym for hasMissingParameter(...) */
bool ChargeTable::hasMissingCharges() const
{
    return hasMissingParameters();
}

/** Synonym for hasMissingParameter(...) */
bool ChargeTable::hasMissingCharges(ResNum resnum) const
{
    return hasMissingParameters(resnum);
}

/** Synonym for hasMissingParameter(...) */
bool ChargeTable::hasMissingCharges(CutGroupID cgid) const
{
    return hasMissingParameters(cgid);
}

/** Synonym for missingParameters(...) */
QSet<AtomIndex> ChargeTable::missingCharges() const
{
    return missingParameters();
}

/** Synonym for missingParameters(...) */
QSet<AtomIndex> ChargeTable::missingCharges(ResNum resnum) const
{
    return missingParameters(resnum);
}

/** Synonym for missingParameters(...) */
QSet<AtomIndex> ChargeTable::missingCharges(CutGroupID cgid) const
{
    return missingParameters(cgid);
}

/** Synonym for clear(...) */
void ChargeTable::clearCharges()
{
    clear();
}

/** Synonym for clear(...) */
void ChargeTable::clearCharge(const CGAtomID &cgid)
{
    clear(cgid);
}

/** Synonym for clear(...) */
void ChargeTable::clearCharge(const ResNumAtomID &rsid)
{
    clear(rsid);
}

/** Synonym for clear(...) */
void ChargeTable::clearCharge(const AtomIndex &atom)
{
    clear(atom);
}

/** Synonym for clear(...) */
void ChargeTable::clearCharges(ResNum resnum)
{
    clear(resnum);
}

/** Synonym for clear(...) */
void ChargeTable::clearCharges(CutGroupID cgid)
{
    clear(cgid);
}

/** Synonym for parameters(...) */
QVector<ChargeParameter> ChargeTable::charges() const
{
    return parameters();
}

/** Synonym for parameters(...) */
QVector<ChargeParameter> ChargeTable::charges(ResNum resnum) const
{
    return parameters(resnum);
}

/** Synonym for parameters(...) */
QVector<ChargeParameter> ChargeTable::charges(CutGroupID cgid) const
{
    return parameters(cgid);
}

/** Synonym for parametersByCutGroup(...) */
GroupedVector<CGAtomID,ChargeParameter> ChargeTable::chargesByCutGroup() const
{
    return parametersByCutGroup();
}

/** Synonym for parametersByCutGroup(...) */
GroupedVector<CGAtomID,ChargeParameter> ChargeTable::chargesByCutGroup(
                                                const QSet<CutGroupID> &cgids) const
{
    return parametersByCutGroup(cgids);
}

/** Synonym for parametersByResidue(...) */
GroupedVector<ResNumAtomID,ChargeParameter> ChargeTable::chargesByResidue() const
{
    return parametersByResidue();
}

/** Synonym for parametersByResidue(...) */
GroupedVector<ResNumAtomID,ChargeParameter> ChargeTable::chargesByResidue(
                                                const QSet<ResNum> &resnums) const
{
    return parametersByResidue(resnums);
}
