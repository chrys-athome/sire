#ifndef SIRESYSTEM_MOLECULEGROUPS_H
#define SIRESYSTEM_MOLECULEGROUPS_H

#include "moleculegroup.h"

SIRE_BEGIN_HEADER

namespace SireSystem
{
class MoleculeGroups;
}

QDataStream& operator<<(QDataStream&, const SireSystem::MoleculeGroups&);
QDataStream& operator>>(QDataStream&, SireSystem::MoleculeGroups&);

namespace SireSystem
{

/** This class holds a collection of MoleculeGroups, together
    with information about which molecules are contained in this
    group, and which groups they are contained within.
    
    @author Christopher Woods
*/
class SIRESYSTEM_EXPORT MoleculeGroups
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
    
    void add(const MoleculeGroup &group);
    void change(const MoleculeGroup &group);
    void remove(const MoleculeGroup &group);
    
    bool remove(MoleculeGroupID groupid);
    bool remove(const MoleculeGroup &group);
    bool remove(const QString &groupname);
    
    bool add(const Molecule &molecule, MoleculeGroupID groupid);
    bool remove(const Molecule &molecule, MoleculeGroupID groupid);
    
    bool change(const Molecule &molecule);
    bool remove(const Molecule &molecule);
    
    QVector<Molecule> molecules() const;
    
    QVector<MoleculeGroup> groups() const;
    QVector<MoleculeGroup> groups(MoleculeID molid) const;
    
    const MoleculeGroup& group(MoleculeGroupID groupid) const;
    const MoleculeGroup& group(const QString &name) const;
    
    bool contains(MoleculeID molid) const;
    
    int count() const;
    
    int nMolecules() const;
    
private:
    void reindex();
    void synchronise(const Molecule &molecule);
    void reindexAndSynchronise();
    void reindexAndSynchronise(MoleculeGroupID molid);
    
    Molecule getLatest(MoleculeID molid) const;
    
    /** The collection of groups in this object, indexed
        by their MoleculeGroupID */
    QHash<MoleculeGroupID, MoleculeGroup> molgroups;
    
    /** Index of where each molecule resides */
    QHash< MoleculeID, QSet<MoleculeGroupID> > index;
};

}

Q_DECLARE_METATYPE(SireSystem::MoleculeGroups);

SIRE_END_HEADER

#endif
