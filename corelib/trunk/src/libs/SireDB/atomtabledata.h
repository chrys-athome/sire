#ifndef SIREDB_ATOMTABLEDATA_H
#define SIREDB_ATOMTABLEDATA_H

#include <QSharedDataPointer>
#include <QVector>

#include "SireMol/atomindex.h"
#include "SireMol/atomindexset.h"
#include "SireMol/moleculecginfo.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class Molecule;
}

namespace SireDB
{
class AtomTableData;
}

class QDataStream;
QDataStream& operator<<(QDataStream&, const SireDB::AtomTableData&);
QDataStream& operator>>(QDataStream&, SireDB::AtomTableData&);

namespace SireDB
{

using SireMol::AtomIndex;
using SireMol::AtomIndexVector;
using SireMol::ResNum;
using SireMol::CutGroupID;
using SireMol::CGAtomID;
using SireMol::Molecule;
using SireMol::MoleculeCGInfo;

class AtomTableDataPvt;

/**
This class holds the (implicitly shared) metadata required by each of the AtomTable derived objects. This data is held by the single AtomTable that is virtually inherited by the derived objects.

@author Christopher Woods
*/
class AtomTableData
{

friend QDataStream& ::operator<<(QDataStream&, const AtomTableData&);
friend QDataStream& ::operator>>(QDataStream&, AtomTableData&);

public:
    AtomTableData();
    AtomTableData(const Molecule &molecule);
    
    AtomTableData(const AtomTableData &other);
    
    ~AtomTableData();

    AtomTableData& operator=(const AtomTableData &other);

    bool isEmpty() const;

    const MoleculeCGInfo& info() const;
    
    QSet<CutGroupID> cutGroupIDs() const;
    QVector<CutGroupID> orderedCutGroupIDs() const;

    bool contains(CutGroupID id) const;
    bool contains(const CGAtomID &idx) const;
    
    int nAtoms(CutGroupID id) const;
    int nCutGroups() const;
    
    const AtomIndex& atom(const CGAtomID &id) const;
    
    AtomIndexVector atoms() const;
    AtomIndexVector atoms(CutGroupID id) const;
    AtomIndexVector atoms(ResNum resnum) const;

private:

    /** Pointer to the implicitly shared data */
    QSharedDataPointer<AtomTableDataPvt> d;

};

}

SIRE_END_HEADER

#endif
