
#include "moleculegroups.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireSystem;
using namespace SireStream;

static const RegisterMetaType<MoleculeGroups> r_molgroups;

/** Serialise to a binary data stream */
QDataStream SIRESYSTEM_EXPORT &operator<<(QDataStream &ds, 
                                          const MoleculeGroups &molgroups)
{
    writeHeader(ds, r_molgroups, 1);
    
    SharedDataStream sds(ds);
    
    //just save each of the MoleculeGroups in the set 
    //(prevents streaming lots of redundant information)
    sds << quint32(molgroups.count());
    
    for (QHash<MoleculeGroupID,MoleculeGroup>::const_iterator
                                      it = molgroups.molgroups.begin();
         it != molgroups.molgroups.end();
         ++it)
    {
        sds << *it;
    }
    
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIRESYSTEM_EXPORT &operator>>(QDataStream &ds, 
                                          MoleculeGroups &molgroups)
{
    VersionID v = readHeader(ds, r_molgroups);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        quint32 ngroups;
        sds >> ngroups;
        
        if (ngroups == 0)
        {
            molgroups.clear();
        }
        else
        {
            //read in all of the groups
            QHash<MoleculeGroupID,MoleculeGroup> groups;
            groups.reserve(ngroups);
            
            for (uint i=0; i<ngroups; ++i)
            {
                MoleculeGroup group;
                
                sds >> group;
                
                groups.insert(group.ID(), group);
            }
            
            //now index them all
            molgroups.molgroups = groups;
            molgroups.reindexAndSynchronise();
        }
    }
    else
        throw version_error(v, "1", r_molgroups, CODELOC);
    
    return ds;
}

/** Create an empty set of MoleculeGroups */
MoleculeGroups::MoleculeGroups()
{}

/** Create a set that only contains 'group' */
MoleculeGroups::MoleculeGroups(const MoleculeGroup &group)
{
    molgroups.insert(group.ID(), group);
    this->reindex();
}

/** Copy constructor */
MoleculeGroups::MoleculeGroups(const MoleculeGroups &other)
               : molgroups(other.molgroups), index(other.index)
{}

/** Destructor */
MoleculeGroups::~MoleculeGroups()
{}

/** Assignment operator */
MoleculeGroups& MoleculeGroups::operator=(const MoleculeGroups &other)
{
    molgroups = other.molgroups;
    index = other.index;
    
    return *this;
}

/** Comparison operator */
bool MoleculeGroups::operator==(const MoleculeGroups &other) const
{
    return molgroups == other.molgroups;
}

/** Comparison operator */
bool MoleculeGroups::operator!=(const MoleculeGroups &other) const
{
    return molgroups != other.molgroups;
}

/** Completely clear this set - this removes all groups */
void MoleculeGroups::clear()
{
    molgroups.clear();
    index.clear();
}

/** Add the MoleculeGroup 'group' - this throws an exception  
    if this group is already in this set.
    
    All of the molecules in this set will be synchronised 
    to their latest versions.
    
    \throw SireMol::duplicate_group
*/
void MoleculeGroups::add(const MoleculeGroup &group)
{
    if ( molgroups.contains(group.ID()) )
        throw SireMol::duplicate_group( QObject::tr(
            "Cannot add the MoleculeGroup \"%1\" (%2 %3) as "
            "it already exists in this set (version == %3)")
                .arg(group.name()).arg(group.ID())
                .arg(group.version().toString(),
                     molgroups.value(group.ID()).version().toString()),
                        CODELOC );

    molgroups.insert( group.ID(), group );
    
    //now index and synchronise this group...
    this->reindexAndSynchronise(group.ID());
}

/** Remove the group with ID == groupid */
bool MoleculeGroups::remove(MoleculeGroupID groupid)
{
    if (molgroups.contains(groupid))
    {
  
        MoleculeGroup group = molgroups.take(groupid);
    
        int nats = group.molecules().count();
        const Molecule *group_array = group.molecules().constData();
    
        for (int i=0; i<nats; ++i)
        {
            MoleculeID molid = group_array[i].ID();
    
            QSet<MoleculeGroupID> groups_containing_mol = index.value(molid);
          
            groups_containing_mol.remove(groupid);
        
            if (groups_containing_mol.isEmpty())
                index.remove(molid);
            else
                index[molid] = groups_containing_mol;
        }
    
        return true;
    }
    else
        return false;
}

/** Remove the group 'group' from this set. */
bool MoleculeGroups::remove(const MoleculeGroup &group)
{
    return this->remove(group.ID());
}

/** Remove all groups from this set that are called 'groupname' */
bool MoleculeGroups::remove(const QString &groupname)
{
    QSet<MoleculeGroupID> groupids;
    
    for (QHash<MoleculeGroupID,MoleculeGroup>::const_iterator 
                                                    it = molgroups.constBegin();
         it != molgroups.constEnd();
         ++it)
    {
        if (it->name() == groupname)
            groupids.insert(it.key());
    }
    
    foreach (MoleculeGroupID groupid, groupids)
    {
        this->remove(groupid);
    }
}

/** Add the molecule 'molecule' to the group with ID == groupid. This
    throws an exception if this molecule already exists in this group.
    
    \throw SireMol::duplicate_molecule
*/
bool MoleculeGroups::add(const Molecule &molecule, MoleculeGroupID groupid)
{
    
}

bool MoleculeGroups::remove(const Molecule &molecule, MoleculeGroupID groupid);

bool MoleculeGroups::change(const Molecule &molecule);
bool MoleculeGroups::remove(const Molecule &molecule);

QVector<Molecule> MoleculeGroups::molecules() const;

QVector<MoleculeGroup> MoleculeGroups::groups() const;
QVector<MoleculeGroup> MoleculeGroups::groups(MoleculeID molid) const;

const MoleculeGroup& MoleculeGroups::group(MoleculeGroupID groupid) const;
const MoleculeGroup& MoleculeGroups::group(const QString &name) const;

bool MoleculeGroups::contains(MoleculeID molid) const;

int MoleculeGroups::count() const;

int MoleculeGroups::nMolecules() const;
