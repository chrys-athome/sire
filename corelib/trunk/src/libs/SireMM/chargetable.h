#ifndef SIREMM_CHARGETABLE_H
#define SIREMM_CHARGETABLE_H

#include "chargeparameter.h"

#include "SireDB/atomtable.hpp"

SIRE_BEGIN_HEADER

namespace SireMM
{
class ChargeTable;
}

QDataStream& operator<<(QDataStream&, const SireMM::ChargeTable&);
QDataStream& operator>>(QDataStream&, SireMM::ChargeTable&);

namespace SireMM
{

using SireMol::Molecule;
using SireMol::CutGroupID;
using SireMol::CGAtomID;
using SireMol::ResNum;
using SireMol::ResNumAtomID;
using SireMol::AtomIndex;

using SireMol::GroupedVector;

/**
This is the table of partial charge parameters of atoms in a SireMol::Molecule.

@author Christopher Woods
*/
class SIREMM_EXPORT ChargeTable : public SireDB::AtomTableT<ChargeParameter>
{
public:
    ChargeTable();
    ChargeTable(const Molecule &molecule);

    ChargeTable(const ChargeTable &other);

    ~ChargeTable();

    ChargeTable* clone() const
    {
        return new ChargeTable(*this);
    }
    
    ChargeTable* create(const Molecule &molecule) const
    {
        return new ChargeTable(molecule);
    }
    
    static const char* typeName()
    {
        return "SireMM::ChargeTable";
    }

    const char* what() const
    {
        return ChargeTable::typeName();
    }

    int nCharges() const;
    int nCharges(ResNum resnum) const;
    int nCharges(CutGroupID cgid) const;

    const ChargeParameter& getCharge(const CGAtomID &cgid) const;
    const ChargeParameter& getCharge(const ResNumAtomID &rsid) const;
    const ChargeParameter& getCharge(const AtomIndex &atom) const;
    
    void setCharge(const CGAtomID &cgid, const ChargeParameter &charge);
    void setCharge(const ResNumAtomID &rsid, const ChargeParameter &charge);
    void setCharge(const AtomIndex &atom, const ChargeParameter &charge);
    
    bool assignedCharge(const CGAtomID &cgid) const;
    bool assignedCharge(const ResNumAtomID &rsid) const;
    bool assignedCharge(const AtomIndex &atom) const;
    
    bool hasMissingCharges() const;
    bool hasMissingCharges(ResNum resnum) const;
    bool hasMissingCharges(CutGroupID cgid) const;
    
    QSet<AtomIndex> missingCharges() const;
    QSet<AtomIndex> missingCharges(ResNum resnum) const;
    QSet<AtomIndex> missingCharges(CutGroupID cgid) const;
    
    void clearCharges();
    void clearCharge(const CGAtomID &cgid);
    void clearCharge(const ResNumAtomID &rsid);
    void clearCharge(const AtomIndex &atom);
    void clearCharges(ResNum resnum);
    void clearCharges(CutGroupID cgid);
    
    template<class C>
    void clearCharges(const C &lots);
    
    QVector<ChargeParameter> charges() const;
    QVector<ChargeParameter> charges(ResNum resnum) const;
    QVector<ChargeParameter> charges(CutGroupID id) const;
    
    template<class C>
    uint charges( QVector<C> &param_vector, uint strt=0 ) const;
    template<class C>
    uint charges( ResNum resnum, QVector<C> &param_vector, uint strt=0 ) const;
    template<class C>
    uint charges( CutGroupID cgid, QVector<C> &param_vector, uint strt=0 ) const;
    
    GroupedVector<CGAtomID,ChargeParameter> chargesByCutGroup() const;
    GroupedVector<CGAtomID,ChargeParameter> chargesByCutGroup(
                                              const QSet<CutGroupID> &cgids) const;
    
    GroupedVector<ResNumAtomID,ChargeParameter> chargesByResidue() const;
    GroupedVector<ResNumAtomID,ChargeParameter> chargesByResidue(
                                              const QSet<ResNum> &resnums) const;

    template<class C>
    void chargesByCutGroup( GroupedVector<CGAtomID,C> &param_vector ) const;
    template<class C>
    void chargesByResidue( GroupedVector<ResNumAtomID,C> &param_vector ) const;
    template<class C>
    void chargesByCutGroup(const QSet<CutGroupID> &cgids, 
                             GroupedVector<CGAtomID,C> &param_vector) const;
    template<class C>
    void chargesByResidue(const QSet<ResNum> &resnums,
                            GroupedVector<ResNumAtomID,C> &param_vector) const;

};

/** Synonym for ChargeTable::clear() */
template<class C>
SIRE_INLINE_TEMPLATE
void ChargeTable::clearCharges(const C &lots)
{
    this->clear(lots);
}

/** Synonym for ChargeTable::parameters(...) */
template<class C>
SIRE_INLINE_TEMPLATE
uint ChargeTable::charges( QVector<C> &param_vector, uint strt ) const
{
    return this->parameters( param_vector, strt );
}

/** Synonym for ChargeTable::parameters(...) */
template<class C>
SIRE_INLINE_TEMPLATE
uint ChargeTable::charges( ResNum resnum, QVector<C> &param_vector, uint strt ) const
{
    return this->parameters( resnum, param_vector, strt );
}

/** Synonym for ChargeTable::parameters(...) */
template<class C>
SIRE_INLINE_TEMPLATE
uint ChargeTable::charges( CutGroupID cgid, QVector<C> &param_vector, uint strt ) const
{
    return this->parameters( cgid, param_vector, strt );
}

/** Synonym for ChargeTable::parametersByCutGroup(...) */
template<class C>
SIRE_INLINE_TEMPLATE
void ChargeTable::chargesByCutGroup( GroupedVector<CGAtomID,C> &param_vector ) const
{
    this->parametersByCutGroup(param_vector);
}

/** Synonym for ChargeTable::parametersByResidue(...) */
template<class C>
SIRE_INLINE_TEMPLATE
void ChargeTable::chargesByResidue( GroupedVector<ResNumAtomID,C> &param_vector ) const
{
    this->parametersByResidue(param_vector);
}

/** Synonym for ChargeTable::parametersByCutGroup(...) */
template<class C>
SIRE_INLINE_TEMPLATE
void ChargeTable::chargesByCutGroup(const QSet<CutGroupID> &cgids,
                                        GroupedVector<CGAtomID,C> &param_vector) const
{
    this->parametersByCutGroup(cgids, param_vector);
}

/** Synonym for ChargeTable::parametersByResidue(...) */
template<class C>
SIRE_INLINE_TEMPLATE
void ChargeTable::chargesByResidue(const QSet<ResNum> &resnums,
                                       GroupedVector<ResNumAtomID,C> &param_vector) const
{
    this->parametersByResidue(resnums, param_vector);
}

}

Q_DECLARE_METATYPE(SireMM::ChargeTable)

SIRE_END_HEADER

#endif
