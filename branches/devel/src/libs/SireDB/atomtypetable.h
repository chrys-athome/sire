#ifndef SIREDB_ATOMTYPETABLE_H
#define SIREDB_ATOMTYPETABLE_H

#include "atomtype.h"
#include "atomtable.hpp"

SIRE_BEGIN_HEADER

namespace SireDB
{
class AtomTypeTable;
}

QDataStream& operator<<(QDataStream&, const SireDB::AtomTypeTable&);
QDataStream& operator>>(QDataStream&, SireDB::AtomTypeTable&);

namespace SireDB
{

/**
This is a table of AtomTypes of atoms in a SireMol::Molecule.
 
@author Christopher Woods
*/
class SIREDB_EXPORT AtomTypeTable : public AtomTableT<AtomType>
{

public:
    AtomTypeTable();
    AtomTypeTable(const Molecule &molecule);
    
    AtomTypeTable(const AtomTypeTable &other);
    
    ~AtomTypeTable();

    AtomTypeTable* clone() const
    {
        return new AtomTypeTable(*this);
    }

    AtomTypeTable* create(const Molecule &molecule) const
    {
        return new AtomTypeTable(molecule);
    }

    static const char* typeName()
    {
        return "SireDB::AtomTypeTable";
    }

    const char* what() const
    {
        return AtomTypeTable::typeName();
    }

    int nAtomTypes() const;
    int nAtomTypes(ResNum resnum) const;
    int nAtomTypes(CutGroupID cgid) const;

    const AtomType& getAtomType(const CGAtomID &cgid) const;
    const AtomType& getAtomType(const ResNumAtomID &rsid) const;
    const AtomType& getAtomType(const AtomIndex &atom) const;
    
    void setAtomType(const CGAtomID &cgid, const AtomType &atomtype);
    void setAtomType(const ResNumAtomID &rsid, const AtomType &atomtype);
    void setAtomType(const AtomIndex &atom, const AtomType &atomtype);
    
    bool assignedAtomType(const CGAtomID &cgid) const;
    bool assignedAtomType(const ResNumAtomID &rsid) const;
    bool assignedAtomType(const AtomIndex &atom) const;
    
    bool hasMissingAtomTypes() const;
    bool hasMissingAtomTypes(ResNum resnum) const;
    bool hasMissingAtomTypes(CutGroupID cgid) const;
    
    QSet<AtomIndex> missingAtomTypes() const;
    QSet<AtomIndex> missingAtomTypes(ResNum resnum) const;
    QSet<AtomIndex> missingAtomTypes(CutGroupID cgid) const;
    
    void clearAtomTypes();
    void clearAtomType(const CGAtomID &cgid);
    void clearAtomType(const ResNumAtomID &rsid);
    void clearAtomType(const AtomIndex &atom);
    void clearAtomTypes(ResNum resnum);
    void clearAtomTypes(CutGroupID cgid);
    
    template<class C>
    void clearAtomTypes(const C &lots);
    
    QVector<AtomType> atomTypes() const;
    QVector<AtomType> atomTypes(ResNum resnum) const;
    QVector<AtomType> atomTypes(CutGroupID id) const;
    
    template<class C>
    uint atomTypes( QVector<C> &param_vector, uint strt=0 ) const;
    template<class C>
    uint atomTypes( ResNum resnum, QVector<C> &param_vector, uint strt=0 ) const;
    template<class C>
    uint atomTypes( CutGroupID cgid, QVector<C> &param_vector, uint strt=0 ) const;
    
    GroupedVector<CGAtomID,AtomType> atomTypesByCutGroup() const;
    GroupedVector<CGAtomID,AtomType> atomTypesByCutGroup(
                                              const QSet<CutGroupID> &cgids) const;
    
    GroupedVector<ResNumAtomID,AtomType> atomTypesByResidue() const;
    GroupedVector<ResNumAtomID,AtomType> atomTypesByResidue(
                                              const QSet<ResNum> &resnums) const;

    template<class C>
    void atomTypesByCutGroup( GroupedVector<CGAtomID,C> &param_vector ) const;
    template<class C>
    void atomTypesByResidue( GroupedVector<ResNumAtomID,C> &param_vector ) const;
    template<class C>
    void atomTypesByCutGroup(const QSet<CutGroupID> &cgids, 
                             GroupedVector<CGAtomID,C> &param_vector) const;
    template<class C>
    void atomTypesByResidue(const QSet<ResNum> &resnums,
                            GroupedVector<ResNumAtomID,C> &param_vector) const;
};

/** Synonym for AtomTypeTable::clear() */
template<class C>
SIRE_INLINE_TEMPLATE
void AtomTypeTable::clearAtomTypes(const C &lots)
{
    this->clear(lots);
}

/** Synonym for AtomTypeTable::parameters(...) */
template<class C>
SIRE_INLINE_TEMPLATE
uint AtomTypeTable::atomTypes( QVector<C> &param_vector, uint strt ) const
{
    return this->parameters( param_vector, strt );
}

/** Synonym for AtomTypeTable::parameters(...) */
template<class C>
SIRE_INLINE_TEMPLATE
uint AtomTypeTable::atomTypes( ResNum resnum, QVector<C> &param_vector, uint strt ) const
{
    return this->parameters( resnum, param_vector, strt );
}

/** Synonym for AtomTypeTable::parameters(...) */
template<class C>
SIRE_INLINE_TEMPLATE
uint AtomTypeTable::atomTypes( CutGroupID cgid, QVector<C> &param_vector, uint strt ) const
{
    return this->parameters( cgid, param_vector, strt );
}

/** Synonym for AtomTypeTable::parametersByCutGroup(...) */
template<class C>
SIRE_INLINE_TEMPLATE
void AtomTypeTable::atomTypesByCutGroup( GroupedVector<CGAtomID,C> &param_vector ) const
{
    this->parametersByCutGroup(param_vector);
}

/** Synonym for AtomTypeTable::parametersByResidue(...) */
template<class C>
SIRE_INLINE_TEMPLATE
void AtomTypeTable::atomTypesByResidue( GroupedVector<ResNumAtomID,C> &param_vector ) const
{
    this->parametersByResidue(param_vector);
}

/** Synonym for AtomTypeTable::parametersByCutGroup(...) */
template<class C>
SIRE_INLINE_TEMPLATE
void AtomTypeTable::atomTypesByCutGroup(const QSet<CutGroupID> &cgids,
                                        GroupedVector<CGAtomID,C> &param_vector) const
{
    this->parametersByCutGroup(cgids, param_vector);
}

/** Synonym for AtomTypeTable::parametersByResidue(...) */
template<class C>
SIRE_INLINE_TEMPLATE
void AtomTypeTable::atomTypesByResidue(const QSet<ResNum> &resnums,
                                       GroupedVector<ResNumAtomID,C> &param_vector) const
{
    this->parametersByResidue(resnums, param_vector);
}

}

Q_DECLARE_METATYPE(SireDB::AtomTypeTable)

SIRE_END_HEADER

#endif
