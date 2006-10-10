#ifndef SIREDB_ATOMTABLE_H
#define SIREDB_ATOMTABLE_H

#include "tablebase.h"

#include "SireMol/moleculeinfo.h"

SIRE_BEGIN_HEADER

namespace SireDB
{
class AtomTable;
}

class QDataStream;
QDataStream& operator<<(QDataStream&, const SireDB::AtomTable&);
QDataStream& operator>>(QDataStream&, SireDB::AtomTable&);

namespace SireMol
{
class AtomIndex;
}

namespace SireDB
{

using SireMol::MoleculeInfo;
using SireMol::ResidueInfo;
using SireMol::AtomIndex;

using SireMol::CutGroupID;
using SireMol::ResNum;
using SireMol::ResID;
using SireMol::AtomID;
using SireMol::CGAtomID;
using SireMol::ResNumAtomID;
using SireMol::ResIDAtomID;

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
    AtomTable(const MoleculeInfo &molinfo);

    AtomTable(const AtomTable &other);

    ~AtomTable();

    const MoleculeInfo& info() const;

    bool isEmpty() const;

    virtual int nParameters() const;
    int nParameters(ResNum resnum) const;
    int nParameters(ResID resid) const;
    int nParameters(CutGroupID cgid) const;
    int nParameters(CutGroupNum cgnum) const;

    virtual int nAssignedParameters() const;
    int nAssignedParameters(ResNum resnum) const;
    int nAssignedParameters(ResID resid) const;
    int nAssignedParameters(CutGroupID cgid) const;
    int nAssignedParameters(CutGroupNum cgnum) const;

    virtual int nMissingParameters() const;
    int nMissingParameters(ResNum resnum) const;
    int nMissingParameters(ResID resid) const;
    int nMissingParameters(CutGroupID cgid) const;
    int nMissingParameters(CutGroupNum cgnum) const;

    bool assignedParameter(const CGAtomID &cgatomid) const;
    bool assignedParameter(const CGNumAtomID &cgatomid) const;
    bool assignedParameter(const ResNumAtomID &resatomid) const;
    bool assignedParameter(const ResIDAtomID &resatomid) const;
    bool assignedParameter(const AtomIndex &atom) const;
    bool assignedParameter(AtomID atomid) const;

    virtual bool hasMissingParameters() const;
    bool hasMissingParameters(ResNum resnum) const;
    bool hasMissingParameters(ResID resid) const;
    bool hasMissingParameters(CutGroupID cgid) const;
    bool hasMissingParameters(CutGroupNum cgnum) const;

    virtual QSet<AtomIndex> missingParameters() const;
    QSet<AtomIndex> missingParameters(ResNum resnum) const;
    QSet<AtomIndex> missingParameters(ResID resid) const;
    QSet<AtomIndex> missingParameters(CutGroupID cgid) const;
    QSet<AtomIndex> missingParameters(CutGroupNum cgnum) const;

    template<class C>
    QSet<AtomIndex> missingParameters(const C &lots) const;

    virtual void clear();

    void clear(const CGAtomID &cgatomid);
    void clear(const CGNumAtomID &cgatomid);
    void clear(const ResNumAtomID &resatomid);
    void clear(const ResIDAtomID &resatomid);
    void clear(const AtomIndex &atom);
    void clear(AtomID atomid);

    void clear(ResNum resnum);
    void clear(ResID resid);

    void clear(CutGroupNum cgnum);
    void clear(CutGroupID cgid);

    template<class C>
    void clear(const C &lots);

private:
    virtual int _unsafe_nAssignedParameters(const ResidueInfo &resinfo) const=0;
    virtual int _unsafe_nAssignedParameters(CutGroupID cgid) const=0;

    virtual int _unsafe_nMissingParameters(const ResidueInfo &resinfo) const=0;
    virtual int _unsafe_nMissingParameters(CutGroupID cgid) const=0;

    virtual bool _unsafe_assignedParameter(const CGAtomID &cgatomid) const=0;

    virtual bool _unsafe_hasMissingParameters(const ResidueInfo &resinfo) const=0;
    virtual bool _unsafe_hasMissingParameters(CutGroupID cgid) const=0;

    virtual QSet<AtomIndex> _unsafe_missingParameters(
                                            const ResidueInfo &resinfo) const=0;
    virtual QSet<AtomIndex> _unsafe_missingParameters(CutGroupID cgid) const=0;

    virtual void _unsafe_clear(const ResidueInfo &resinfo)=0;
    virtual void _unsafe_clear(CutGroupID cgid)=0;
    virtual void _unsafe_clear(const CGAtomID &cgatomid)=0;

    /** The metadata for the molecule whose parameters
        are in this table */
    MoleculeInfo molinfo;
};

/** Return the metadata for the molecule whose parameters are in this
    table */
inline const MoleculeInfo& AtomTable::info() const
{
    return molinfo;
}

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

/** Return the AtomIndexes of atoms that are missing parameters. */
template<class C>
SIRE_OUTOFLINE_TEMPLATE
QSet<AtomIndex> AtomTable::missingParameters(const C &lots) const
{
    QSet<AtomIndex> missing_atoms;

    for (typename C::const_iterator it = lots.begin();
         it != lots.end();
         ++it)
    {
        missing_atoms += this->missingParameters(*it);
    }

    return missing_atoms;
}

}

SIRE_END_HEADER

#endif
