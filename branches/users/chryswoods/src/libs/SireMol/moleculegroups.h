#ifndef SIREMOL_MOLECULEGROUPS_H
#define SIREMOL_MOLECULEGROUPS_H

#include "moleculegroup.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class MoleculeGroups;
}

QDataStream& operator<<(QDataStream&, const SireMol::MoleculeGroups&);
QDataStream& operator>>(QDataStream&, SireMol::MoleculeGroups&);

namespace SireMol
{

/** This class holds a collection of MoleculeGroups, together
    with information about which molecules are contained in this
    group, and which groups they are contained within.
    
    @author Christopher Woods
*/
class SIREMOL_EXPORT MoleculeGroups
{

friend QDataStream& ::operator<<(QDataStream&, const MoleculeGroups&);
friend QDataStream& ::operator>>(QDataStream&, MoleculeGroups&);

public:
    MoleculeGroups();
    MoleculeGroups(const MoleculeGroup &group);
    
    MoleculeGroups(const MoleculeGroups &other);
    
    ~MoleculeGroups();
    
    MoleculeGroups& operator=(const MoleculeGroups &other);
    
    bool operator==(const MoleculeGroups &other) const;
    bool operator!=(const MoleculeGroups &other) const;
    
    void clear();
    
    bool add(const MoleculeGroup &group);
    bool change(const MoleculeGroup &group);
    bool remove(const MoleculeGroup &group);
    
    bool remove(MoleculeGroupID groupid);
    bool remove(const QString &groupname);
    
    bool add(const Molecule &molecule, MoleculeGroupID groupid);
    bool remove(const Molecule &molecule, MoleculeGroupID groupid);
    
    bool change(const Molecule &molecule);
    bool remove(const Molecule &molecule);
    
    QVector<Molecule> molecules() const;
    
    QSet<MoleculeID> moleculeIDs() const;
    
    QVector<MoleculeGroup> groups() const;
    QVector<MoleculeGroup> groups(MoleculeID molid) const;
    
    const MoleculeGroup& group(MoleculeGroupID groupid) const;
    const MoleculeGroup& group(const QString &name) const;
    
    QSet<MoleculeGroupID> groupsContaining(MoleculeID molid) const;
    
    bool contains(MoleculeID molid) const;
    
    int count() const;
    
    int nMolecules() const;
    
    void assertContains(MoleculeGroupID groupid) const;
    
private:
    void reindex();
    void _pvt_index(const MoleculeGroup &group);
    
    void synchronise(MoleculeGroupID groupid);
    void synchronise(MoleculeID molid);
    void synchronise();
    
    Molecule getLatest(MoleculeID molid) const;
    
    /** The collection of groups in this object, indexed
        by their MoleculeGroupID */
    QHash<MoleculeGroupID, MoleculeGroup> molgroups;
    
    /** Index of where each molecule resides */
    QHash< MoleculeID, QSet<MoleculeGroupID> > index;
};

}

Q_DECLARE_METATYPE(SireMol::MoleculeGroups);

SIRE_END_HEADER

#endif
