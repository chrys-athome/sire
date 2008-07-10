/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2007  Christopher Woods
  *
  *  This program is free software; you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation; either version 2 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program; if not, write to the Free Software
  *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  *  For full details of the license please see the COPYING file
  *  that should have come with this distribution.
  *
  *  You can contact the authors via the developer's mailing list
  *  at http://siremol.org
  *
\*********************************************/

#include <QHash>
#include <QMutex>

#include "system.h"

#include "SireFF/ffidx.h"

#include "SireMol/errors.h"
#include "SireError/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireSystem;
using namespace SireFF;
using namespace SireMol;
using namespace SireCAS;
using namespace SireBase;
using namespace SireUnits::Dimension;
using namespace SireStream;

////////
//////// Implementation of SystemRegistry
////////

class SystemRegistry
{
public:
    ~SystemRegistry();
    
    static const MajorMinorVersion& getVersion(const QUuid &uid);

private:
    SystemRegistry();
    
    static SystemRegistry global_registry;
    
    QMutex registry_mutex;
    QHash<QUuid,MajorMinorVersion> versions;
};

SystemRegistry::SystemRegistry()
{}

SystemRegistry::~SystemRegistry()
{}

SystemRegistry SystemRegistry::global_registry;

const MajorMinorVersion& SystemRegistry::getVersion(const QUuid &uid)
{
    QMutexLocker lkr(&global_registry.registry_mutex);
    
    if (not global_registry.versions.contains(uid))
        global_registry.versions.insert(uid, MajorMinorVersion());
     
    QHash<QUuid,MajorMinorVersion>::iterator it = global_registry.versions.find(uid);
    it->incrementMajor();
              
    return *it;
}

////////
//////// Implementation of SystemData
////////

/** Rebuild the index of molecule groups in this System */
void System::rebuildIndex()
{
    //rebuild our index...
    mgroups_by_num.clear();
    
    QList<MGNum> mgnums0 = molgroups[0]->mgNums();
    QList<MGNum> mgnums1 = molgroups[1]->mgNums();
    
    mgroups_by_num.reserve( mgnums0.count() + mgnums1.count() );
    
    foreach (MGNum mgnum, mgnums0)
    {
        mgroups_by_num.insert( mgnum, 0 );
    }
    
    foreach (MGNum mgnum, mgnums1)
    {
        if (mgroups_by_num.contains(mgnum))
            throw SireError::program_bug( QObject::tr(
                "It should not be possible for a System to contain two molecule "
                "groups that both contain the same number... (%1)")
                    .arg(mgnum), CODELOC );
    
        mgroups_by_num.insert( mgnum, 1 );
    }
    
    //now rebuild the index of molecules
    MolGroupsBase::clearIndex();
    
    foreach (MGNum mgnum, mgnums0)
    {
        MolGroupsBase::addToIndex(molgroups[0]->at(mgnum));
    }

    foreach (MGNum mgnum, mgnums1)
    {
        MolGroupsBase::addToIndex(molgroups[1]->at(mgnum));
    }
}

static const RegisterMetaType<System> r_system;

/** Serialise to a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator<<(QDataStream &ds, const System &system)
{
    writeHeader(ds, r_system, 1);
    
    SharedDataStream sds(ds);
    
    sds << system.uid << system.sysname
        << system.molgroups[0] << system.molgroups[1] 
        << system.sysmonitors << system.sysspace
        << static_cast<const MolGroupsBase&>(system);
        
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator>>(QDataStream &ds, System &system)
{
    VersionID v = readHeader(ds, r_system);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> system.uid >> system.sysname
            >> system.molgroups[0] >> system.molgroups[1] 
            >> system.sysmonitors >> system.sysspace
            >> static_cast<MolGroupsBase&>(system);

        system.rebuildIndex();
            
        system.sysversion = SystemRegistry::getVersion(system.uid);
    }
    else
        throw version_error(v, "1", r_system, CODELOC);
        
    return ds;
}

/** Construct an unnamed System */
System::System()
       : ConcreteProperty<System,MolGroupsBase>(),
         uid( QUuid::createUuid() ),
         sysversion( SystemRegistry::getVersion(uid) )
{
    molgroups[0] = ForceFields();
    molgroups[1] = MolGroups();
}

/** Construct a named System */
System::System(const SysName &name)
       : ConcreteProperty<System,MolGroupsBase>(),
         uid( QUuid::createUuid() ),
         sysname(name),
         sysversion( SystemRegistry::getVersion(uid) )
{
    molgroups[0] = ForceFields();
    molgroups[1] = MolGroups();
}

/** Copy constructor */
System::System(const System &other)
       : ConcreteProperty<System,MolGroupsBase>(other),
         uid(other.uid), sysname(other.sysname), sysversion(other.sysversion),
         sysmonitors(other.sysmonitors), sysspace(other.sysspace),
         mgroups_by_num(other.mgroups_by_num)
{
    molgroups[0] = other.molgroups[0];
    molgroups[1] = other.molgroups[1];
}

/** Destructor */
System::~System()
{}

/** Copy assignment operator */
System& System::operator=(const System &other)
{
    if (this != &other)
    {
        uid = other.uid;
        sysname = other.sysname;
        sysversion = other.sysversion;
        molgroups[0] = other.molgroups[0];
        molgroups[1] = other.molgroups[1];
        sysmonitors = other.sysmonitors;
        sysspace = other.sysspace;
        mgroups_by_num = other.mgroups_by_num;
        
        MolGroupsBase::operator=(other);
    }
    
    return *this;
}

/** Comparison operator - two systems are equal if they have the 
    same UID and version */
bool System::operator==(const System &other) const
{
    return uid == other.uid and sysversion == other.sysversion;
}

/** Comparison operator - two systems are equal if they have the 
    same UID and version */
bool System::operator!=(const System &other) const
{
    return not this->operator==(other);
}

/** Return a modifiable reference to all of the forcefields in
    this system */
ForceFields& System::_pvt_forceFields()
{
    BOOST_ASSERT( molgroups[0]->isA<ForceFields>() );
    return molgroups[0].edit().asA<ForceFields>();
}

/** Return a reference to all of the forcefields in this system */ 
const ForceFields& System::_pvt_forceFields() const
{
    BOOST_ASSERT( molgroups[0]->isA<ForceFields>() );
    return molgroups[0]->asA<ForceFields>();
}

/** Return a reference to all of the forcefields in this system */ 
const ForceFields& System::_pvt_constForceFields() const
{
    return this->_pvt_forceFields();
}

/** Return a modifiable reference to all of the non-forcefield
    molecule groups in this system */
MolGroups& System::_pvt_moleculeGroups()
{
    BOOST_ASSERT( molgroups[1]->isA<MolGroups>() );
    return molgroups[1].edit().asA<MolGroups>();
}

/** Return a reference to all of the non-forcefield
    molecule groups in this system */
const MolGroups& System::_pvt_moleculeGroups() const
{
    BOOST_ASSERT( molgroups[1]->isA<MolGroups>() );
    return molgroups[1]->asA<MolGroups>();
}

/** Return a reference to all of the non-forcefield
    molecule groups in this system */
const MolGroups& System::_pvt_constMoleculeGroups() const
{
    return this->_pvt_moleculeGroups();
}

void System::_pvt_throwMissingGroup(MGNum mgnum) const
{
    throw SireMol::missing_group( QObject::tr(
        "There is no molecule group with number %1 in this system. "
        "Available groups have numbers %2.")
            .arg(mgnum).arg( Sire::toString(this->mgNums()) ),
                CODELOC );
}

/** Return the set of molecule groups that contains the molecule group
    with number 'mgnum'
    
    \throw SireMol::missing_group
*/
MolGroupsBase& System::_pvt_moleculeGroups(MGNum mgnum)
{
    int idx = mgroups_by_num.value(mgnum, -1);
    
    if (idx == -1)
        this->_pvt_throwMissingGroup(mgnum);
        
    return molgroups[idx].edit();
}

/** Return the set of molecule groups that contains the molecule group
    with number 'mgnum'
    
    \throw SireMol::missing_group
*/
const MolGroupsBase& System::_pvt_moleculeGroups(MGNum mgnum) const
{
    int idx = mgroups_by_num.value(mgnum, -1);
    
    if (idx == -1)
        this->_pvt_throwMissingGroup(mgnum);
        
    return molgroups[idx].read();
}

/** Return the set of molecule groups that contains the molecule group
    with number 'mgnum'
    
    \throw SireMol::missing_group
*/
const MolGroupsBase& System::_pvt_constMoleculeGroups(MGNum mgnum) const
{
    return this->_pvt_moleculeGroups(mgnum);
}

/** Return the molecule group with number 'mgnum'

    \throw SireMol::missing_group
*/
const MolGroup& System::_pvt_moleculeGroup(MGNum mgnum) const
{
    int idx = mgroups_by_num.value(mgnum, -1);
    
    if (idx == -1)
        this->_pvt_throwMissingGroup(mgnum);
        
    return molgroups[idx]->at(mgnum);
}

/** Internal function used to get the group with number 'mgnum' */
const MolGroup& System::getGroup(MGNum mgnum) const
{
    return this->_pvt_moleculeGroup(mgnum);
}

/** Internal function used to get lots of groups */
void System::getGroups(const QList<MGNum> &mgnums,
                       QVarLengthArray<const MolGroup*,10> &groups) const
{
    groups.clear();
    
    foreach (MGNum mgnum, mgnums)
    {
        groups.append( &(this->_pvt_moleculeGroup(mgnum)) );
    }
}

/** Internal function used to get pointers to all of the groups in this system */
QHash<MGNum,const MolGroup*> System::getGroups() const
{
    QHash<MGNum,const MolGroup*> groups;
    
    QList<MGNum> mgnums0 = molgroups[0]->mgNums();
    QList<MGNum> mgnums1 = molgroups[1]->mgNums();

    groups.reserve(mgnums0.count() + mgnums1.count());
    
    foreach (MGNum mgnum, mgnums0)
    {
        groups.insert( mgnum, &(molgroups[0]->at(mgnum)) );
    }
    
    foreach (MGNum mgnum, mgnums1)
    {
        groups.insert( mgnum, &(molgroups[1]->at(mgnum)) );
    }
    
    return groups;
}

/** Return the forcefield that matches the ID 'ffid'

    \throw SireFF::missing_forcefield
    \throw SireFF::duplicate_forcefield
    \throw SireError::invalid_index
*/
const FF& System::operator[](const FFID &ffid) const
{
    return this->_pvt_forceFields()[ffid];
}

/** Convienient syntax for System::add */
System& System::operator+=(const FF &forcefield)
{
    this->add(forcefield);
    return *this;
}

/** Convienient syntax for System::add */
System& System::operator+=(const MolGroup &molgroup)
{
    this->add(molgroup);
    return *this;
}

/** Convienient syntax for System::remove */
System& System::operator-=(const FF &forcefield)
{
    this->remove(forcefield);
    return *this;
}

/** Convienient syntax for System::remove */
System& System::operator-=(const MolGroup &molgroup)
{
    this->remove(molgroup);
    return *this;
}

/** Convienient syntax for System::remove */
System& System::operator-=(const FFID &ffid)
{
    this->remove(ffid);
    return *this;
}

/** Convienient syntax for System::remove */
System& System::operator-=(const MGID &mgid)
{
    this->remove(mgid);
    return *this;
}

/** Convienient syntax for System::remove */
System& System::operator-=(const MolID &molid)
{
    this->remove(molid);
    return *this;
}

/** Set the name of this system */
void System::setName(const SysName &newname)
{
    if (sysname != newname)
    {
        sysname = newname;
        sysversion.incrementMajor();
    }
}

/** Return the forcefield with ID 'ffid' in this system

    \throw SireFF::missing_forcefield
    \throw SireFF::duplicate_forcefield
    \throw SireError::invalid_index
*/
const FF& System::at(const FFID &ffid) const
{
    return this->_pvt_forceFields().at(ffid);
}

/** Return the forcefield with ID 'ffid' in this system

    \throw SireFF::missing_forcefield
    \throw SireFF::duplicate_forcefield
    \throw SireError::invalid_index
*/
const FF& System::forceField(const FFID &ffid) const
{
    return this->_pvt_forceFields().forceField(ffid);
}

/** Return the forcefield that contains the molecule group 
    identified by the ID 'mgid'
    
    \throw SireMol::missing_group
    \throw SireMol::duplicate_group
    \throw SireError::invalid_index
*/
const FF& System::forceField(const MGID &mgid) const
{
    return this->_pvt_forceFields().forceField( this->getGroupNumber(mgid) );
}

/** Return the number of forcefields in this system */
int System::nForceFields() const
{
    return this->_pvt_forceFields().nForceFields();
}

/** Return the index of the forcefield with ID 'ffid'

    \throw SireFF::missing_forcefield
    \throw SireFF::duplicate_forcefield
    \throw SireError::invalid_index
*/
FFIdx System::ffIdx(const FFID &ffid) const
{
    return this->_pvt_forceFields().ffIdx(ffid);
}

/** Return the name of the forcefield with ID 'ffid'

    \throw SireFF::missing_forcefield
    \throw SireFF::duplicate_forcefield
    \throw SireError::invalid_index
*/
const FFName& System::ffName(const FFID &ffid) const
{
    return this->_pvt_forceFields().ffName(ffid);
}

/** Return a string representation of this system */
QString System::toString() const
{
    return QString("System( name=%1, nForceFields=%2, nMolecules=%3 )")
                .arg(this->name())
                .arg(this->nForceFields())
                .arg(this->nMolecules());
}

/** Return the symbol that represents the total energy component
    of the system */
const Symbol& System::totalComponent() const
{
    return this->_pvt_forceFields().totalComponent();
}

/** Return the total energy of this system. */
Energy System::energy()
{
    return this->_pvt_forceFields().energy();
}

/** Return the total energy of the energy component in this system
    that is identified by the symbol 'component'
    
    \throw SireFF::missing_component
*/
Energy System::energy(const Symbol &component)
{
    return this->_pvt_forceFields().energy(component);
}

/** Return the energies of the components of this system whose
    symbols are in 'components'
    
    \throw SireFF::missing_component
*/
Values System::energies(const QSet<Symbol> &components)
{
    return this->_pvt_forceFields().energies(components);
}

/** Return the energies of all energy components, constants and 
    expressions in this system */
Values System::energies()
{
    return this->_pvt_forceFields().energies();
}

/** Add the forces acting on the molecules in the forcetable 'forcetable'
    from this system onto this forcetable, scaled by the optionally 
    supplied 'scale_force' */
void System::force(ForceTable &forcetable, double scale_force)
{
    this->_pvt_forceFields().force(forcetable, scale_force);
}

/** Add the forces acting on the molecules in the forcetable 'forcetable'
    from the component of this system identified by 'component' onto 
    this forcetable, scaled by the optionally supplied 'scale_force' */
void System::force(ForceTable &forcetable, const Symbol &component,
                   double scale_force)
{
    this->_pvt_forceFields().force(forcetable, component, scale_force);
}

/** Set the space for this system. This is the space within which all of the 
    molecules in this system are mapped. The mapping function 'mapfunc' is
    used to map the molecules from one space to another (and is also
    saved as the function to map new molecules as well) */
void System::setSpace(const SpaceBase &space, const MappingFunction &mapfunc)
{
    if (sysspace != space)
    {
        System old_system(*this);
        
        try
        {
            //this will move *all* molecules!
            this->mustNowRecalculateFromScratch();
            
            Molecules all_mols = this->molecules();
        }
        catch(...)
        {
            this->operator=(old_system);
            throw;
        }
    }
}

/** Set the space for this system. This is the space within which all of the 
    molecules in this system are mapped. The existing mapping function is
    used to map the molecules from the current space to the new space. */
void System::setSpace(const SpaceBase &space)
{
    this->setSpace(space, sysmapfunc);
}
/*
SpaceBase& System::space() const;

void System::setProperty(const QString &name, const Property &value);
void System::setProperty(const FFID &ffid, const QString &name, const Property &value);

void System::setComponent(const Symbol &symbol, double value);
void System::setComponent(const Symbol &symbol, const SireCAS::Expression &expression);

Expression System::getComponent(const Symbol &symbol) const;

QHash<FFName,Property> System::property(const QString &name) const;

const Property& System::property(const FFID &ffid, const QString &name) const;

bool System::containsProperty(const QString &name) const;
bool System::containsProperty(const FFID &ffid, const QString &name) const;

QHash<FFName,Properties> System::properties() const;

const QVector<ForceField>& System::forceFields() const;
QList<FFName> System::ffNames() const;

void System::mustNowRecalculateFromScratch();

bool System::isDirty() const;
bool System::isClean() const;

void System::add(const FF &forcefield);
void System::add(const MolGroup &molgroup);

void System::remove(const FFIdx &ffidx);
void System::remove(const FFName &ffname);
void System::remove(const FFID &ffid);

void System::remove(MGNum mgnum);
void System::remove(const MolGroup &molgroup);
void System::remove(const MGID &mgid);
void System::remove(const MolID &molid);

const MolGroup& System::at(MGNum mgnum) const;

void System::add(const MoleculeView &molview, const MGID &mgid,
         const PropertyMap &map);
void System::add(const ViewsOfMol &molviews, const MGID &mgid,
         const PropertyMap &map);
void System::add(const Molecules &molecules, const MGID &mgid,
         const PropertyMap &map);
void System::add(const MolGroup &molgroup, const MGID &mgid,
         const PropertyMap &map);

void System::addIfUnique(const MoleculeView &molview, const MGID &mgid,
                 const PropertyMap &map);
void System::addIfUnique(const ViewsOfMol &molviews, const MGID &mgid,
                 const PropertyMap &map);
void System::addIfUnique(const Molecules &molecules, const MGID &mgid,
                 const PropertyMap &map);
void System::addIfUnique(const MolGroup &molgroup, const MGID &mgid,
                 const PropertyMap &map);

void System::add(const MoleculeView &molview, const MGID &mgid);
void System::add(const ViewsOfMol &molviews, const MGID &mgid);
void System::add(const Molecules &molecules, const MGID &mgid);
void System::add(const MolGroup &molgroup, const MGID &mgid);

void System::addIfUnique(const MoleculeView &molview, const MGID &mgid);
void System::addIfUnique(const ViewsOfMol &molviews, const MGID &mgid);
void System::addIfUnique(const Molecules &molecules, const MGID &mgid);
void System::addIfUnique(const MolGroup &molgroup, const MGID &mgid);

void System::removeAll(const MGID &mgid);
void System::remove(const MoleculeView &molview, const MGID &mgid);
void System::remove(const ViewsOfMol &molviews, const MGID &mgid);
void System::remove(const Molecules &molecules, const MGID &mgid);
void System::remove(const MolGroup &molgroup, const MGID &mgid);

void System::removeAll(const MoleculeView &molview, const MGID &mgid);
void System::removeAll(const ViewsOfMol &molviews, const MGID &mgid);
void System::removeAll(const Molecules &molecules, const MGID &mgid);
void System::removeAll(const MolGroup &molgroup, const MGID &mgid);

void System::remove(MolNum molnum, const MGID &mgid);
void System::remove(const QSet<MolNum> &molnums, const MGID &mgid);

void System::update(const MoleculeData &moldata);
void System::update(const Molecules &molecules);
void System::update(const MolGroup &molgroup);

void System::setContents(const MGID &mgid, const MoleculeView &molview,
                 const PropertyMap &map);
void System::setContents(const MGID &mgid, const ViewsOfMol &molviews,
                 const PropertyMap &map);
void System::setContents(const MGID &mgid, const Molecules &molecules,
                 const PropertyMap &map);
void System::setContents(const MGID &mgid, const MolGroup &molgroup,
                 const PropertyMap &map);

void System::setContents(const MGID &mgid, const MoleculeView &molview);
void System::setContents(const MGID &mgid, const ViewsOfMol &molviews);
void System::setContents(const MGID &mgid, const Molecules &molecules);
void System::setContents(const MGID &mgid, const MolGroup &molgroup);    
*/
