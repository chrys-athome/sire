#ifndef SIREDB_ATOMTABLE_H
#define SIREDB_ATOMTABLE_H

#include "tablebase.h"
#include "parameterstore.hpp"
#include "atomtabledata.h"

#include "SireMol/atomindex.h"
#include "SireMol/atomindexset.h"
#include "SireMol/cgatomid.h"
#include "SireMol/resnumatomid.h"
#include "SireMol/moleculecginfo.h"
#include "SireMol/groupedvector.hpp"

SIRE_BEGIN_HEADER

namespace SireDB
{
class AtomTable;
}

class QDataStream;
QDataStream& operator<<(QDataStream&, const SireDB::AtomTable&);
QDataStream& operator>>(QDataStream&, SireDB::AtomTable&);

namespace SireDB
{

using SireMol::AtomIndex;
using SireMol::AtomIndexSet;
using SireMol::AtomID;
using SireMol::ResNum;
using SireMol::CutGroupID;
using SireMol::CGAtomID;
using SireMol::ResNumAtomID;
using SireMol::MoleculeCGInfo;
using SireMol::ResidueCGInfo;
using SireMol::AtomIndexVector;
using SireMol::GroupedVector;

/**
This is the virtual base class of all tables that store atomic parameters (parameters that are assigned to individual atoms in the molecule). Classes derived from this include AtomTypeTable, ChargeTable and LJTable.

@author Christopher Woods
*/
class SIREDB_EXPORT AtomTable : public TableBase
{

friend QDataStream& ::operator<<(QDataStream&, const AtomTable&);
friend QDataStream& ::operator>>(QDataStream&, AtomTable&);

public:
    typedef CGAtomID index_type;

    AtomTable();
    AtomTable(const Molecule &molecule);

    AtomTable(const AtomTable &other);

    ~AtomTable();

    const MoleculeCGInfo& info() const;

    bool isEmpty() const;

    int size() const;
    int count() const;
    
    int nAtoms() const;
    int nAtoms(ResNum resnum) const;
    int nAtoms(CutGroupID cgid) const;
  
    QHash<CutGroupID,uint> nAtomsPerCutGroup() const;
  
    int nAtoms(const QSet<ResNum> &resnums) const;
    int nAtoms(const QSet<CutGroupID> &cgids) const;

    int nCutGroups() const;
    int nResidues() const;
    
    bool isCompatibleWith(const Molecule &molecule) const;
    
    bool contains(const AtomIndex &atom) const;
    bool contains(CutGroupID cgid) const;
    bool contains(ResNum resnum) const;
    bool contains(const CGAtomID &id) const;
    bool contains(const ResNumAtomID &id) const;

    AtomIndex atom(const CGAtomID &id) const;
    AtomIndex atom(const ResNumAtomID &rsid) const;
    
    QSet<AtomIndex> atoms(const QSet<CGAtomID> &cgids) const;
    QSet<AtomIndex> atoms(const QSet<ResNumAtomID> &rsids) const;
    
    QVector<AtomIndex> atoms() const;
    QVector<AtomIndex> atoms(ResNum resnum) const;
    QVector<AtomIndex> atoms(CutGroupID cgid) const;
    
    GroupedVector<CGAtomID,AtomIndex> atomsByCutGroup() const;
    GroupedVector<ResNumAtomID,AtomIndex> atomsByResidue() const;
    GroupedVector<CGAtomID,AtomIndex> atomsByCutGroup(const QSet<CutGroupID> &cgids) const;
    GroupedVector<ResNumAtomID,AtomIndex> atomsByResidue(const QSet<ResNum> &resnums) const;

    QSet<CutGroupID> cutGroupIDs() const;
    QSet<ResNum> residueNumbers() const;

    QVector<CutGroupID> orderedCutGroupIDs() const;

    virtual int nParameters() const=0;
    virtual int nParameters(ResNum resnum) const=0;
    virtual int nParameters(CutGroupID cgid) const=0;
    
    virtual bool assignedParameter(const CGAtomID &cgid) const=0;
    virtual bool assignedParameter(const ResNumAtomID &rsid) const=0;
    virtual bool assignedParameter(const AtomIndex &atom) const=0;
    
    virtual bool hasMissingParameters() const=0;
    virtual bool hasMissingParameters(ResNum resnum) const=0;
    virtual bool hasMissingParameters(CutGroupID cgid) const=0;
    
    virtual void clear()=0;
    virtual void clear(const CGAtomID &cgid)=0;
    virtual void clear(const ResNumAtomID &rsid)=0;
    virtual void clear(const AtomIndex &atom)=0;
    virtual void clear(ResNum resnum)=0;
    virtual void clear(CutGroupID cgid)=0;
    
    template<class C>
    void clear(const C &lots);

protected:
    void setMolecule(const Molecule &molecule);
    
private:

    /** The metadata needed by the atom tables */
    AtomTableData metadata;
};

/** Clear all of the parameters for the objects in 'lots'. Note that 
    all of the object must be in the molecule or else an exception 
    will be thrown. */
template<class C>
SIRE_OUTOFLINE_TEMPLATE
void AtomTable::clear(const C &lots)
{
    for (typename C::const_iterator it = lots.begin();
         it != lots.end();
         ++it)
    {
        this->clear( *it );
    }
}

}

SIRE_END_HEADER

#endif
