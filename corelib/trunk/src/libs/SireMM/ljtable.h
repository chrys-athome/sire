#ifndef SIREMM_LJTABLE_H
#define SIREMM_LJTABLE_H

#include "ljparameter.h"

#include "SireDB/atomtable.hpp"

SIRE_BEGIN_HEADER

namespace SireMM
{
class LJTable;
}

QDataStream& operator<<(QDataStream&, const SireMM::LJTable&);
QDataStream& operator>>(QDataStream&, SireMM::LJTable&);

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
This is the table of Lennard Jones parameters of atoms in a SireMol::Molecule.

@author Christopher Woods
*/
class SIREMM_EXPORT LJTable : public SireDB::AtomTableT<LJParameter>
{
public:
    LJTable();
    LJTable(const Molecule &molecule);

    LJTable(const LJTable &other);

    ~LJTable();

    LJTable* clone() const
    {
        return new LJTable(*this);
    }
    
    LJTable* create(const Molecule &molecule) const
    {
        return new LJTable(molecule);
    }
    
    static const char* typeName()
    {
        return "SireMM::LJTable";
    }

    const char* what() const
    {
        return LJTable::typeName();
    }

    int nLJs() const;
    int nLJs(ResNum resnum) const;
    int nLJs(CutGroupID cgid) const;

    const LJParameter& getLJ(const CGAtomID &cgid) const;
    const LJParameter& getLJ(const ResNumAtomID &rsid) const;
    const LJParameter& getLJ(const AtomIndex &atom) const;
    
    void setLJ(const CGAtomID &cgid, const LJParameter &charge);
    void setLJ(const ResNumAtomID &rsid, const LJParameter &charge);
    void setLJ(const AtomIndex &atom, const LJParameter &charge);
    
    bool assignedLJ(const CGAtomID &cgid) const;
    bool assignedLJ(const ResNumAtomID &rsid) const;
    bool assignedLJ(const AtomIndex &atom) const;
    
    bool hasMissingLJs() const;
    bool hasMissingLJs(ResNum resnum) const;
    bool hasMissingLJs(CutGroupID cgid) const;
    
    QSet<AtomIndex> missingLJs() const;
    QSet<AtomIndex> missingLJs(ResNum resnum) const;
    QSet<AtomIndex> missingLJs(CutGroupID cgid) const;
    
    void clearLJs();
    void clearLJ(const CGAtomID &cgid);
    void clearLJ(const ResNumAtomID &rsid);
    void clearLJ(const AtomIndex &atom);
    void clearLJs(ResNum resnum);
    void clearLJs(CutGroupID cgid);
    
    template<class C>
    void clearLJs(const C &lots);
    
    QVector<LJParameter> ljs() const;
    QVector<LJParameter> ljs(ResNum resnum) const;
    QVector<LJParameter> ljs(CutGroupID id) const;
    
    template<class C>
    uint ljs( QVector<C> &param_vector, uint strt=0 ) const;
    template<class C>
    uint ljs( ResNum resnum, QVector<C> &param_vector, uint strt=0 ) const;
    template<class C>
    uint ljs( CutGroupID cgid, QVector<C> &param_vector, uint strt=0 ) const;
    
    GroupedVector<CGAtomID,LJParameter> ljsByCutGroup() const;
    GroupedVector<CGAtomID,LJParameter> ljsByCutGroup(
                                              const QSet<CutGroupID> &cgids) const;
    
    GroupedVector<ResNumAtomID,LJParameter> ljsByResidue() const;
    GroupedVector<ResNumAtomID,LJParameter> ljsByResidue(
                                              const QSet<ResNum> &resnums) const;

    template<class C>
    void ljsByCutGroup( GroupedVector<CGAtomID,C> &param_vector ) const;
    template<class C>
    void ljsByResidue( GroupedVector<ResNumAtomID,C> &param_vector ) const;
    template<class C>
    void ljsByCutGroup(const QSet<CutGroupID> &cgids, 
                             GroupedVector<CGAtomID,C> &param_vector) const;
    template<class C>
    void ljsByResidue(const QSet<ResNum> &resnums,
                            GroupedVector<ResNumAtomID,C> &param_vector) const;

};

/** Synonym for LJTable::clear() */
template<class C>
SIRE_INLINE_TEMPLATE
void LJTable::clearLJs(const C &lots)
{
    this->clear(lots);
}

/** Synonym for LJTable::parameters(...) */
template<class C>
SIRE_INLINE_TEMPLATE
uint LJTable::ljs( QVector<C> &param_vector, uint strt ) const
{
    return this->parameters( param_vector, strt );
}

/** Synonym for LJTable::parameters(...) */
template<class C>
SIRE_INLINE_TEMPLATE
uint LJTable::ljs( ResNum resnum, QVector<C> &param_vector, uint strt ) const
{
    return this->parameters( resnum, param_vector, strt );
}

/** Synonym for LJTable::parameters(...) */
template<class C>
SIRE_INLINE_TEMPLATE
uint LJTable::ljs( CutGroupID cgid, QVector<C> &param_vector, uint strt ) const
{
    return this->parameters( cgid, param_vector, strt );
}

/** Synonym for LJTable::parametersByCutGroup(...) */
template<class C>
SIRE_INLINE_TEMPLATE
void LJTable::ljsByCutGroup( GroupedVector<CGAtomID,C> &param_vector ) const
{
    this->parametersByCutGroup(param_vector);
}

/** Synonym for LJTable::parametersByResidue(...) */
template<class C>
SIRE_INLINE_TEMPLATE
void LJTable::ljsByResidue( GroupedVector<ResNumAtomID,C> &param_vector ) const
{
    this->parametersByResidue(param_vector);
}

/** Synonym for LJTable::parametersByCutGroup(...) */
template<class C>
SIRE_INLINE_TEMPLATE
void LJTable::ljsByCutGroup(const QSet<CutGroupID> &cgids,
                                        GroupedVector<CGAtomID,C> &param_vector) const
{
    this->parametersByCutGroup(cgids, param_vector);
}

/** Synonym for LJTable::parametersByResidue(...) */
template<class C>
SIRE_INLINE_TEMPLATE
void LJTable::ljsByResidue(const QSet<ResNum> &resnums,
                                       GroupedVector<ResNumAtomID,C> &param_vector) const
{
    this->parametersByResidue(resnums, param_vector);
}

}

Q_DECLARE_METATYPE(SireMM::LJTable)

SIRE_END_HEADER

#endif
