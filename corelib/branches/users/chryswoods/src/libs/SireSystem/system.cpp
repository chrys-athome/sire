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

#include "monitorname.h"

#include "SireFF/ffidx.h"
#include "SireFF/forcefield.h"
#include "SireFF/ff.h"
#include "SireFF/ffmolgroup.h"
#include "SireFF/forcetable.h"

#include "SireMol/partialmolecule.h"
#include "SireMol/molecule.h"
#include "SireMol/molecules.h"
#include "SireMol/moleculegroup.h"

#include "SireMol/errors.h"
#include "SireError/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include <QDebug>

using namespace SireSystem;
using namespace SireFF;
using namespace SireMol;
using namespace SireCAS;
using namespace SireBase;
using namespace SireUnits::Dimension;
using namespace SireStream;

static VersionRegistry<QUuid> *system_registry(0);

static VersionRegistry<QUuid>& systemRegistry()
{
    if (system_registry == 0)
    {
        //need to make this thread-safe...
        system_registry = new VersionRegistry<QUuid>();
    }
    
    return *system_registry;
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
        << system.sysmonitors
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
            >> system.sysmonitors
            >> static_cast<MolGroupsBase&>(system);

        system.rebuildIndex();
            
        system.sysversion = systemRegistry().registerObject(system.uid);
    }
    else
        throw version_error(v, "1", r_system, CODELOC);
        
    return ds;
}

/** Construct an unnamed System */
System::System()
       : ConcreteProperty<System,MolGroupsBase>(),
         uid( QUuid::createUuid() ),
         sysversion( systemRegistry().registerObject(uid) )
{
    molgroups[0] = ForceFields();
    molgroups[1] = MoleculeGroups();
}

SharedPolyPointer<System> shared_null;

const System& System::null()
{
    if (shared_null.constData() == 0)
    {
        QMutexLocker lkr( SireBase::globalLock() );
        
        if (shared_null.constData() == 0)
            shared_null = new System();
    }
    
    return *(shared_null.constData());
}

/** Construct a named System */
System::System(const SysName &name)
       : ConcreteProperty<System,MolGroupsBase>(),
         uid( QUuid::createUuid() ),
         sysname(name),
         sysversion( systemRegistry().registerObject(uid) )
{
    molgroups[0] = ForceFields();
    molgroups[1] = MoleculeGroups();
}

/** Copy constructor */
System::System(const System &other)
       : ConcreteProperty<System,MolGroupsBase>(other),
         uid(other.uid), sysname(other.sysname), sysversion(other.sysversion),
         sysmonitors(other.sysmonitors),
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
MoleculeGroups& System::_pvt_moleculeGroups()
{
    BOOST_ASSERT( molgroups[1]->isA<MoleculeGroups>() );
    return molgroups[1].edit().asA<MoleculeGroups>();
}

/** Return a reference to all of the non-forcefield
    molecule groups in this system */
const MoleculeGroups& System::_pvt_moleculeGroups() const
{
    BOOST_ASSERT( molgroups[1]->isA<MoleculeGroups>() );
    return molgroups[1]->asA<MoleculeGroups>();
}

/** Return a reference to all of the non-forcefield
    molecule groups in this system */
const MoleculeGroups& System::_pvt_constMoleculeGroups() const
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
const MoleculeGroup& System::_pvt_moleculeGroup(MGNum mgnum) const
{
    int idx = mgroups_by_num.value(mgnum, -1);
    
    if (idx == -1)
        this->_pvt_throwMissingGroup(mgnum);
        
    return molgroups[idx]->at(mgnum);
}

/** Internal function used to get the group with number 'mgnum' */
const MoleculeGroup& System::getGroup(MGNum mgnum) const
{
    return this->_pvt_moleculeGroup(mgnum);
}

/** Internal function used to get lots of groups */
void System::getGroups(const QList<MGNum> &mgnums,
                       QVarLengthArray<const MoleculeGroup*,10> &groups) const
{
    groups.clear();
    
    foreach (MGNum mgnum, mgnums)
    {
        groups.append( &(this->_pvt_moleculeGroup(mgnum)) );
    }
}

/** Internal function used to get pointers to all of the groups in this system */
QHash<MGNum,const MoleculeGroup*> System::getGroups() const
{
    QHash<MGNum,const MoleculeGroup*> groups;
    
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

/** Return the monitor at ID 'monid'

    \throw SireSystem::missing_monitor
    \throw SireSystem::duplicate_monitor
    \throw SireError::invalid_index
*/
const SystemMonitor& System::operator[](const MonitorID &monid) const
{
    return sysmonitors[monid];
}

/** Convienient syntax for System::add */
System& System::operator+=(const FF &forcefield)
{
    this->add(forcefield);
    return *this;
}

/** Convienient syntax for System::add */
System& System::operator+=(const MoleculeGroup &molgroup)
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
System& System::operator-=(const MoleculeGroup &molgroup)
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

/** Collect statistics about the current configuration */
void System::collectStats()
{
    sysmonitors.monitor(*this);
    sysversion.incrementMinor();
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

/** Return the monitor at ID 'monid'

    \throw SireSystem::missing_monitor
    \throw SireSystem::duplicate_monitor
    \throw SireError::invalid_index
*/
const SystemMonitor& System::at(const MonitorID &monid) const
{
    return sysmonitors.at(monid);
}

/** Return the monitor at ID 'monid'

    \throw SireSystem::missing_monitor
    \throw SireSystem::duplicate_monitor
    \throw SireError::invalid_index
*/
const SystemMonitor& System::monitor(const MonitorID &monid) const
{
    return sysmonitors.monitor(monid);
}

/** Return the monitors with ID 'monid'

    \throw SireSystem::missing_monitor
    \throw SireError::invalid_index
*/
QList<SysMonPtr> System::monitors(const MonitorID &monid) const
{
    return sysmonitors.monitors(monid);
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

/** Return the number of monitors in this system */
int System::nMonitors() const
{
    return sysmonitors.count();
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

/** Return the name of the monitor at ID 'monid'

    \throw SireSystem::missing_monitor
    \throw SireSystem::duplicate_monitor
    \throw SireError::invalid_index
*/
const MonitorName& System::monitorName(const MonitorID &monid) const
{
    return sysmonitors.monitorName(monid);
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
    return QString("System( name=%1, nForceFields=%2, nMolecules=%3 "
                           "nMonitors()=%4 )")
                .arg(this->name())
                .arg(this->nForceFields())
                .arg(this->nMolecules())
                .arg(this->nMonitors());
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

/** Return the value of the component in this system
    that is identified by the symbol 'component'
    
    \throw SireFF::missing_component
*/
double System::componentValue(const Symbol &component)
{
    return this->energy(component);
}

/** Return the values of the components of this system whose
    symbols are in 'components'
    
    \throw SireFF::missing_component
*/
Values System::componentValues(const QSet<Symbol> &components)
{
    return this->energies(components);
}

/** Return the values of all components, constants and 
    expressions in this system */
Values System::componentValues()
{
    return this->energies();
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

/** Set the value of the property called 'name' to the value 'value' in 
    all forcefields that have this property
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void System::setProperty(const QString &name, const Property &value)
{
    this->_pvt_forceFields().setProperty(name, value);
    sysversion.incrementMajor();
}

/** Set the value of the property called 'name' in the forcefields identified
    by the ID 'ffid' to the value 'value'
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void System::setProperty(const FFID &ffid, const QString &name, const Property &value)
{
    this->_pvt_forceFields().setProperty(ffid, name, value);
    sysversion.incrementMajor();
}

/** Set the energy component equal to the constant value 'value'. This will
    replace any existing component with this value, although it is an error
    to try to replace an energy component of one of the constituent forcefields
    
    \throw SireFF::duplicate_component
*/
void System::setComponent(const Symbol &symbol, double value)
{
    this->_pvt_forceFields().setComponent(symbol, value);
    sysversion.incrementMajor();
}

/** Set the energy component equal to the expression 'expression'. This will
    replace any existing component with this expression, although it is an error
    to try to replace an energy component of one of the constituent forcefields
    
    \throw SireFF::duplicate_component
*/
void System::setComponent(const Symbol &symbol, const SireCAS::Expression &expression)
{
    this->_pvt_forceFields().setComponent(symbol, expression);
    sysversion.incrementMajor();
}

/** Return the energy component represented by the symbol 'symbol'

    \throw SireFF::missing_component
*/
Expression System::getComponent(const Symbol &symbol) const
{
    return this->_pvt_forceFields().getComponent(symbol);
}

/** Return whether or not this system has an energy component represented
    by the symbol 'symbol' */
bool System::hasComponent(const Symbol &symbol) const
{
    return this->_pvt_forceFields().hasComponent(symbol);
}

/** Return the symbols representing all of the energy components of
    the system */
Symbols System::components() const
{
    return this->_pvt_forceFields().components();
}

/** Return the values of the property called 'name' in all of the 
    forcefields that contain this property
    
    \throw SireBase::missing_property
*/
QHash<FFName,PropertyPtr> System::property(const QString &name) const
{
    return this->_pvt_forceFields().property(name);
}

/** Return the value of the property 'name' in the forcefield identified 
    by the ID 'ffid'
    
    \throw SireFF::missing_forcefield
    \throw SireFF::duplicate_forcefield
    \throw SireBase::missing_property
    \throw SireError::invalid_index
*/
const Property& System::property(const FFID &ffid, const QString &name) const
{
    return this->_pvt_forceFields().property(ffid, name);
}

/** Return whether or not any of the forcefields contain a property called 'name' */
bool System::containsProperty(const QString &name) const
{
    return this->_pvt_forceFields().containsProperty(name);
}

/** Return whether or not any of the forcefields identified by the ID 'ffid'
    contain a property called 'name' */
bool System::containsProperty(const FFID &ffid, const QString &name) const
{
    return this->_pvt_forceFields().containsProperty(ffid, name);
}

/** Return the values of all of the properties of all of the forcefields,
    indexed by the name of the forcefield */
QHash<FFName,Properties> System::properties() const
{
    return this->_pvt_forceFields().properties();
}

/** Return the list of all monitors of this system */
const SystemMonitors& System::monitors() const
{
    return sysmonitors;
}

/** Return an array of all of the forcefields in this system */
const ForceFields& System::forceFields() const
{
    return this->_pvt_forceFields();
}

/** Return all of the extra non-forcefield molecule groups in this system */
const MoleculeGroups& System::extraGroups() const
{
    return this->_pvt_moleculeGroups();
}

/** Tell all of the forcefields that they will need to recalculate
    their energies from scratch. This can speed up calculations where
    you know that the majority (or all) of the molecules will be
    changing */
void System::mustNowRecalculateFromScratch()
{
    this->_pvt_forceFields().mustNowRecalculateFromScratch();
}

/** Return whether or not any of the forcefields are dirty */
bool System::isDirty() const
{
    return this->_pvt_forceFields().isDirty();
}

/** Return whether or not all of the forcefields are clean */
bool System::isClean() const
{
    return this->_pvt_forceFields().isClean();
}

/** Add a system monitor 'monitor', identified by the name 'name', which
    will be updated every 'frequency' steps.
    
    \throw SireSystem::duplicate_monitor
*/
void System::add(const QString &name, const SystemMonitor &monitor, int frequency)
{
    sysmonitors.add(name, monitor, frequency);
    sysversion.incrementMajor();
}

/** Add the monitors in 'monitors' to this system 

    \throw SireSystem::duplicate_monitor
*/
void System::add(const SystemMonitors &monitors)
{
    sysmonitors.add(monitors);
    sysversion.incrementMajor();
}

/** Add the monitors in 'monitors', setting the frequency of the 
    new monitors to 'frequency'
    
    \throw SireSystem::duplicate_monitor
*/
void System::add(const SystemMonitors &monitors, int frequency)
{
    sysmonitors.add(monitors, frequency);
    sysversion.incrementMajor();
}

/** Set the monitors of this system to 'monitors' */
void System::setMonitors(const SystemMonitors &monitors)
{
    if (sysmonitors != monitors)
    {
        sysmonitors = monitors;
        sysversion.incrementMajor();
    }
}

/** Set the monitors of the system to 'monitors', and reset the 
    frequency of all of the monitors so that they are triggered
    every 'frequency' steps */
void System::setMonitors(const SystemMonitors &monitors, int frequency)
{
    SystemMonitors new_monitors(monitors);
    new_monitors.setAllFrequency(frequency);
    this->setMonitors(new_monitors);
}

/** Add the forcefield 'forcefield' to this system. This will raise
    an exception if this forcefield (or one with the same name)
    is already present in this set. Note that if the added
    forcefield will be updated to contain the versions of 
    any molecules that are already present in any of the 
    other forcefields.

    \throw SireFF::duplicate_forcefield
    \throw SireMol::duplicate_group
*/
void System::add(const FF &forcefield)
{
    FFPtr ff( forcefield );
    ff.edit().update( this->matchToExistingVersion(forcefield.molecules()) );

    System old_system(*this);
    
    try
    {
        this->_pvt_forceFields().add(ff.read());
        this->rebuildIndex();
        sysversion.incrementMajor();
    }
    catch(...)
    {
        this->operator=(old_system);
        throw;
    }
}

/** Add the molecule group 'molgroup' to this system. If this is 
    a molecule group that is part of a forcefield, then the entire
    forcefield will be added to this system. This will raise
    an exception if this molecule group is already present in 
    this system. Note that the added molecule group will be 
    updated to contain the version of the any molecules that
    are already present in this system
    
    \throw SireFF::duplicate_forcefield
    \throw SireMol::duplicate_group
*/
void System::add(const MoleculeGroup &molgroup)
{
    if (molgroup.isA<FFMolGroup>())
    {
        this->add( molgroup.asA<FFMolGroup>().forceField() );
    }
    else if (molgroup.isA<SireFF::detail::FFMolGroupPvt>())
    {
        this->add( molgroup.asA<SireFF::detail::FFMolGroupPvt>().forceField() );
    }
    else
    {
        MolGroupPtr mgroup(molgroup);
        mgroup.edit().update( this->matchToExistingVersion(molgroup.molecules()) );

        System old_system(*this);
        
        try
        {
            this->_pvt_moleculeGroups().add(mgroup);
            this->rebuildIndex();
            sysversion.incrementMajor();
        }
        catch(...)
        {
            this->operator=(old_system);
            throw;
        }
    }
}

/** Remove all monitors that match the ID 'monid'

    \throw SireSystem::missing_monitor
    \throw SireError::invalid_index
*/
void System::remove(const MonitorID &monid)
{
    sysmonitors.remove(monid);
    sysversion.incrementMajor();
}

/** Remove the forcefield(s) that match the ID 'ffid'

    \throw SireError::invalid_index
*/
void System::remove(const FFID &ffid)
{
    System old_system(*this);
    
    try
    {
        this->_pvt_forceFields().remove(ffid);
        this->rebuildIndex();
        sysversion.incrementMajor();
    }
    catch(...)
    {
        this->operator=(old_system);
        throw;
    }
}

/** Remove the forcefield 'ff'. Note that this removes the forcefield
    in this system that has the same name as 'ff'
    
    \throw SireFF::missing_forcefield
*/
void System::remove(const FF &ff)
{
    this->remove(ff.name());
}

/** Remove the molecule group(s) that match the ID 'mgid'. 
    Note that you can't remove molecule groups that are part
    of a forcefield
    
    \throw SireMol::missing_group
    \throw SireError::invalid_index
    \throw SireError::invalid_arg
*/
void System::remove(const MGID &mgid)
{
    QList<MGNum> mgnums = mgid.map(*this);
    
    System old_system(*this);
    
    try
    {
        foreach (MGNum mgnum, mgnums)
        {
            if (mgroups_by_num.value(mgnum) == 0)
            {
                const MoleculeGroup &molgroup = this->_pvt_moleculeGroup(mgnum);
                BOOST_ASSERT( molgroup.isA<SireFF::detail::FFMolGroupPvt>() );
            
                const FF &ff = molgroup.asA<SireFF::detail::FFMolGroupPvt>().forceField();
            
                throw SireError::invalid_arg( QObject::tr(
                    "You cannot remove the molecule group with number %1 "
                    "(that matches the ID %2) as it is part of the forcefield "
                    "%3 in this system.")
                        .arg(mgnum).arg(mgid.toString())
                        .arg(ff.name()), CODELOC );
            }
            
            this->_pvt_moleculeGroups().remove(mgnum);
            this->removeFromIndex(mgnum);
            mgroups_by_num.remove(mgnum);
        }
        
        sysversion.incrementMajor();
    }
    catch(...)
    {
        this->operator=(old_system);
        throw;
    }
}

/** Remove the molecule group 'molgroup'. 

    \throw SireMol::missing_group
    \throw SireError::invalid_arg
*/
void System::remove(const MoleculeGroup &molgroup)
{
    this->remove(molgroup.number());
}

/** Remove all molecules from this system that match the ID 'molid'

    \throw SireMol::missing_molecule
*/
void System::remove(const MolID &molid)
{
    QList<MolNum> molnums = molid.map(*this);
    
    System old_system(*this);
    
    try
    {
        foreach (MolNum molnum, molnums)
        {
            foreach (MGNum mgnum, this->groupsContaining(molnum))
            {
                molgroups[mgroups_by_num.value(mgnum)].edit().remove(molnum, mgnum);
            }
            
            this->removeFromIndex(molnum);
        }
        
        sysversion.incrementMinor();
    }
    catch(...)
    {
        this->operator=(old_system);
        throw;
    }
}

/** Completely remove all molecules from this system */
void System::removeAllMolecules()
{
    this->removeAll();
}

/** Completely remove all non-forcefield molecule groups
    from this system */
void System::removeAllMoleculeGroups()
{
    QList<MGNum> mgnums = this->_pvt_constMoleculeGroups().mgNums();
    
    if (not mgnums.isEmpty())
    {
        this->_pvt_moleculeGroups().remove( MGNumList(mgnums) );
        
        foreach (MGNum mgnum, mgnums)
        {
            this->removeFromIndex(mgnum);
            mgroups_by_num.remove(mgnum);
        }
    }
    
    sysversion.incrementMajor();
}

/** Completely remove all monitors from this system */
void System::removeAllMonitors()
{
    if (not sysmonitors.isEmpty())
    {
        sysmonitors.removeAll();
        sysversion.incrementMajor();
    }
}

/** Completely remove all of the forcefields (and their contained
    molecule groups) from this system */
void System::removeAllForceFields()
{
    if (this->nForceFields() == 0)
        return;
        
    QList<MGNum> mgnums = this->_pvt_forceFields().mgNums();
    this->_pvt_forceFields().removeAllForceFields();

    foreach (MGNum mgnum, mgnums)
    {
        this->removeFromIndex(mgnum);
        mgroups_by_num.remove(mgnum);
    }
    
    sysversion.incrementMajor();
}

/** Return the molecule group with number 'mgnum'

    \throw SireMol::missing_group
*/
const MoleculeGroup& System::at(MGNum mgnum) const
{
    return this->_pvt_moleculeGroup(mgnum);
}

/** Add the molecule viewed in 'molview' to the molecule groups
    identified by the ID 'mgid'. The supplied property map
    is used to find the properties required by any forcefields
    that this molecule may be added to. The version of the molecule
    already present in this system is used, if such a molecule exists.
    
    \throw SireMol::missing_group
    \throw SireBase::missing_property
    \throw SireError::invalid_index
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void System::add(const MoleculeView &molview, const MGID &mgid,
                 const PropertyMap &map)
{
    QList<MGNum> mgnums = mgid.map(*this);
    
    PartialMolecule view(molview);
    view.update( this->matchToExistingVersion(molview.data()) );
    
    System old_system(*this);
    
    try
    {
        foreach (MGNum mgnum, mgnums)
        {
            if (mgroups_by_num.value(mgnum) == 0)
                this->_pvt_forceFields().add(view, mgnum, map);
            else
                this->_pvt_moleculeGroups().add(view, mgnum);
                
            this->addToIndex(mgnum, view.data().number());
        }
        
        sysversion.incrementMinor();
    }
    catch(...)
    {
        this->operator=(old_system);
        throw;
    }
}

/** Add the views of the molecule in 'molviews' to the molecule groups
    identified by the ID 'mgid'. The supplied property map
    is used to find the properties required by any forcefields
    that this molecule may be added to. The version of the molecule
    already present in this system is used, if such a molecule exists.
    
    \throw SireMol::missing_group
    \throw SireBase::missing_property
    \throw SireError::invalid_index
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void System::add(const ViewsOfMol &molviews, const MGID &mgid,
                 const PropertyMap &map)
{
    QList<MGNum> mgnums = mgid.map(*this);
    
    ViewsOfMol views(molviews);
    views.update( this->matchToExistingVersion(molviews.data()) );
    
    System old_system(*this);
    
    try
    {
        foreach (MGNum mgnum, mgnums)
        {
            if (mgroups_by_num.value(mgnum) == 0)
                this->_pvt_forceFields().add(views, mgnum, map);
            else
                this->_pvt_moleculeGroups().add(views, mgnum);
                
            this->addToIndex(mgnum, views.number());
        }
        
        sysversion.incrementMinor();
    }
    catch(...)
    {
        this->operator=(old_system);
        throw;
    }
}

/** Add the molecules viewed in 'molecules' to the molecule groups
    identified by the ID 'mgid'. The supplied property map
    is used to find the properties required by any forcefields
    that this molecule may be added to. The version of the molecule
    already present in this system is used, if such a molecule exists.
    
    \throw SireMol::missing_group
    \throw SireBase::missing_property
    \throw SireError::invalid_index
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void System::add(const Molecules &molecules, const MGID &mgid,
                 const PropertyMap &map)
{
    QList<MGNum> mgnums = mgid.map(*this);
    
    Molecules mols = this->matchToExistingVersion(molecules);
    
    System old_system(*this);
    
    try
    {
        foreach (MGNum mgnum, mgnums)
        {
            if (mgroups_by_num.value(mgnum) == 0)
                this->_pvt_forceFields().add(mols, mgnum, map);
            else
                this->_pvt_moleculeGroups().add(mols, mgnum);
                
            this->addToIndex(mgnum, mols.molNums());
        }
        
        sysversion.incrementMinor();
    }
    catch(...)
    {
        this->operator=(old_system);
        throw;
    }
}

/** Add the molecules in the molecule group 'molgroup' to the molecule groups
    identified by the ID 'mgid'. The supplied property map
    is used to find the properties required by any forcefields
    that this molecule may be added to. The version of the molecule
    already present in this system is used, if such a molecule exists.
    
    \throw SireMol::missing_group
    \throw SireBase::missing_property
    \throw SireError::invalid_index
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void System::add(const MoleculeGroup &molgroup, const MGID &mgid,
                 const PropertyMap &map)
{
    QList<MGNum> mgnums = mgid.map(*this);
    
    MolGroupPtr group(molgroup);
    group.edit().update( this->matchToExistingVersion(molgroup.molecules()) );
    
    System old_system(*this);
    
    try
    {
        foreach (MGNum mgnum, mgnums)
        {
            if (mgroups_by_num.value(mgnum) == 0)
                this->_pvt_forceFields().add(group, mgnum, map);
            else
                this->_pvt_moleculeGroups().add(group, mgnum);
                
            this->addToIndex(mgnum, molgroup.molNums());
        }
        
        sysversion.incrementMinor();
    }
    catch(...)
    {
        this->operator=(old_system);
        throw;
    }
}

/** Add the view of the molecule in 'molview' to the groups
    identified by 'mgid'. This only adds the view to a group
    if it doesn't already exist in the group. The version
    of the molecule already present in this set is used if 
    such a molecule already exists. The supplied property map
    is used to find the properties required by any forcefields
    that this molecule may be added to.
    
    \throw SireMol::missing_group
    \throw SireBase::missing_property
    \throw SireError::invalid_index
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void System::addIfUnique(const MoleculeView &molview, const MGID &mgid,
                         const PropertyMap &map)
{
    QList<MGNum> mgnums = mgid.map(*this);
    
    PartialMolecule view(molview);
    view.update( this->matchToExistingVersion(molview.data()) );
    
    System old_system(*this);
    
    try
    {
        foreach (MGNum mgnum, mgnums)
        {
            if (mgroups_by_num.value(mgnum) == 0)
                this->_pvt_forceFields().addIfUnique(view, mgnum, map);
            else
                this->_pvt_moleculeGroups().addIfUnique(view, mgnum);
                
            this->addToIndex(mgnum, view.data().number());
        }
        
        sysversion.incrementMinor();
    }
    catch(...)
    {
        this->operator=(old_system);
        throw;
    }
}

/** Add the views of the molecule in 'molviews' to the groups
    identified by 'mgid'. This only adds the view to a group
    if it doesn't already exist in the group. The version
    of the molecule already present in this set is used if 
    such a molecule already exists. The supplied property map
    is used to find the properties required by any forcefields
    that this molecule may be added to.
    
    \throw SireMol::missing_group
    \throw SireBase::missing_property
    \throw SireError::invalid_index
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void System::addIfUnique(const ViewsOfMol &molviews, const MGID &mgid,
                         const PropertyMap &map)
{
    QList<MGNum> mgnums = mgid.map(*this);
    
    ViewsOfMol views(molviews);
    views.update( this->matchToExistingVersion(molviews.data()) );
    
    System old_system(*this);
    
    try
    {
        foreach (MGNum mgnum, mgnums)
        {
            if (mgroups_by_num.value(mgnum) == 0)
                this->_pvt_forceFields().addIfUnique(views, mgnum, map);
            else
                this->_pvt_moleculeGroups().addIfUnique(views, mgnum);
                
            this->addToIndex(mgnum, views.number());
        }
        
        sysversion.incrementMinor();
    }
    catch(...)
    {
        this->operator=(old_system);
        throw;
    }
}

/** Add the views of the molecules in 'molecules' to the groups
    identified by 'mgid'. This only adds the view to a group
    if it doesn't already exist in the group. The version
    of the molecule already present in this set is used if 
    such a molecule already exists. The supplied property map
    is used to find the properties required by any forcefields
    that this molecule may be added to.
    
    \throw SireMol::missing_group
    \throw SireBase::missing_property
    \throw SireError::invalid_index
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void System::addIfUnique(const Molecules &molecules, const MGID &mgid,
                         const PropertyMap &map)
{
    QList<MGNum> mgnums = mgid.map(*this);
    
    Molecules mols = this->matchToExistingVersion(molecules);
    
    System old_system(*this);
    
    try
    {
        foreach (MGNum mgnum, mgnums)
        {
            if (mgroups_by_num.value(mgnum) == 0)
                this->_pvt_forceFields().addIfUnique(mols, mgnum, map);
            else
                this->_pvt_moleculeGroups().addIfUnique(mols, mgnum);
                
            this->addToIndex(mgnum, mols.molNums());
        }
        
        sysversion.incrementMinor();
    }
    catch(...)
    {
        this->operator=(old_system);
        throw;
    }
}

/** Add the view of the molecules in the group 'molgroup' to the groups
    identified by 'mgid'. This only adds the view to a group
    if it doesn't already exist in the group. The version
    of the molecule already present in this set is used if 
    such a molecule already exists. The supplied property map
    is used to find the properties required by any forcefields
    that this molecule may be added to.
    
    \throw SireMol::missing_group
    \throw SireBase::missing_property
    \throw SireError::invalid_index
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void System::addIfUnique(const MoleculeGroup &molgroup, const MGID &mgid,
                         const PropertyMap &map)
{
    QList<MGNum> mgnums = mgid.map(*this);
    
    MolGroupPtr group(molgroup);
    group.edit().update( this->matchToExistingVersion(molgroup.molecules()) );
    
    System old_system(*this);
    
    try
    {
        foreach (MGNum mgnum, mgnums)
        {
            if (mgroups_by_num.value(mgnum) == 0)
                this->_pvt_forceFields().addIfUnique(group, mgnum, map);
            else
                this->_pvt_moleculeGroups().addIfUnique(group, mgnum);
                
            this->addToIndex(mgnum, molgroup.molNums());
        }
        
        sysversion.incrementMinor();
    }
    catch(...)
    {
        this->operator=(old_system);
        throw;
    }
}

/** Convenient overload of System::add that uses the default locations
    to find any necessary properties. 
    
    \throw SireMol::missing_group
    \throw SireBase::missing_property
    \throw SireError::invalid_index
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void System::add(const MoleculeView &molview, const MGID &mgid)
{
    this->add(molview, mgid, PropertyMap());
}

/** Convenient overload of System::add that uses the default locations
    to find any necessary properties. 
    
    \throw SireMol::missing_group
    \throw SireBase::missing_property
    \throw SireError::invalid_index
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void System::add(const ViewsOfMol &molviews, const MGID &mgid)
{
    this->add(molviews, mgid, PropertyMap());
}

/** Convenient overload of System::add that uses the default locations
    to find any necessary properties. 
    
    \throw SireMol::missing_group
    \throw SireBase::missing_property
    \throw SireError::invalid_index
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void System::add(const Molecules &molecules, const MGID &mgid)
{
    this->add(molecules, mgid, PropertyMap());
}

/** Convenient overload of System::add that uses the default locations
    to find any necessary properties. 
    
    \throw SireMol::missing_group
    \throw SireBase::missing_property
    \throw SireError::invalid_index
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void System::add(const MoleculeGroup &molgroup, const MGID &mgid)
{
    this->add(molgroup, mgid, PropertyMap());
}

/** Convenient overload of System::addIfUnique that uses the default locations
    to find any necessary properties. 
    
    \throw SireMol::missing_group
    \throw SireBase::missing_property
    \throw SireError::invalid_index
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void System::addIfUnique(const MoleculeView &molview, const MGID &mgid)
{
    this->addIfUnique(molview, mgid, PropertyMap());
}

/** Convenient overload of System::addIfUnique that uses the default locations
    to find any necessary properties. 
    
    \throw SireMol::missing_group
    \throw SireBase::missing_property
    \throw SireError::invalid_index
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void System::addIfUnique(const ViewsOfMol &molviews, const MGID &mgid)
{
    this->addIfUnique(molviews, mgid, PropertyMap());
}

/** Convenient overload of System::addIfUnique that uses the default locations
    to find any necessary properties. 
    
    \throw SireMol::missing_group
    \throw SireBase::missing_property
    \throw SireError::invalid_index
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void System::addIfUnique(const Molecules &molecules, const MGID &mgid)
{
    this->addIfUnique(molecules, mgid, PropertyMap());
}

/** Convenient overload of System::addIfUnique that uses the default locations
    to find any necessary properties. 
    
    \throw SireMol::missing_group
    \throw SireBase::missing_property
    \throw SireError::invalid_index
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void System::addIfUnique(const MoleculeGroup &molgroup, const MGID &mgid)
{
    this->addIfUnique(molgroup, mgid, PropertyMap());
}

/** Remove all molecules from the molecule groups identified by the ID 'mgid'

    \throw SireMol::missing_group
    \throw SireError::invalid_index
*/
void System::removeAll(const MGID &mgid)
{
    QList<MGNum> mgnums = mgid.map(*this);
    
    if (mgnums.count() == 1)
    {
        MGNum mgnum = mgnums.at(0);
        
        this->_pvt_moleculeGroups(mgnum).removeAll(mgnum);
        this->clearIndex(mgnum);
        sysversion.incrementMinor();
    }
    else
    {
        System old_system(*this);
    
        try
        {
            foreach (MGNum mgnum, mgnums)
            {
                this->_pvt_moleculeGroups(mgnum).removeAll(mgnum);
                this->clearIndex(mgnum);
            }
            
            sysversion.incrementMinor();
        }
        catch(...)
        {
            this->operator=(old_system);
            throw;
        }
    }
}

/** Remove the view 'molview' from the specified groups in this
    forcefield. Note that this only removes the specific view
    (and indeed only the first copy of this view if there 
    are duplicates) - it does not remove the atoms in this
    view from all of the other views
    
    \throw SireMol::missing_group
    \throw SireError::invalid_index
*/
void System::remove(const MoleculeView &molview, const MGID &mgid)
{
    QList<MGNum> mgnums = mgid.map(*this);
    
    System old_system(*this);
    
    try
    {
        foreach (MGNum mgnum, mgnums)
        {
            MolGroupsBase &molgroups = this->_pvt_moleculeGroups(mgnum);
            
            molgroups.remove(molview, mgnum);
            
            if (not molgroups.at(mgnum).contains(molview.data().number()))
                this->removeFromIndex(mgnum, molview.data().number());
        }
        
        sysversion.incrementMinor();
    }
    catch(...)
    {
        this->operator=(old_system);
        throw;
    }
}

/** Remove the views in 'molviews' from the specified groups in this
    forcefield. Note that this only removes the specific views
    (and indeed only the first copy of this view if there 
    are duplicates) - it does not remove the atoms in this
    view from all of the other views
    
    \throw SireMol::missing_group
    \throw SireError::invalid_index
*/
void System::remove(const ViewsOfMol &molviews, const MGID &mgid)
{
    QList<MGNum> mgnums = mgid.map(*this);
    
    System old_system(*this);
    
    try
    {
        foreach (MGNum mgnum, mgnums)
        {
            MolGroupsBase &molgroups = this->_pvt_moleculeGroups(mgnum);
            
            molgroups.remove(molviews, mgnum);
            
            if (not molgroups.at(mgnum).contains(molviews.number()))
                this->removeFromIndex(mgnum, molviews.number());
        }
        
        sysversion.incrementMinor();
    }
    catch(...)
    {
        this->operator=(old_system);
        throw;
    }
}

/** Remove them molecules in 'molecules' from the specified groups in this
    forcefield. Note that this only removes the specific views
    (and indeed only the first copy of this view if there 
    are duplicates) - it does not remove the atoms in this
    view from all of the other views
    
    \throw SireMol::missing_group
    \throw SireError::invalid_index
*/
void System::remove(const Molecules &molecules, const MGID &mgid)
{
    QList<MGNum> mgnums = mgid.map(*this);
    
    System old_system(*this);
    
    try
    {
        foreach (MGNum mgnum, mgnums)
        {
            MolGroupsBase &molgroups = this->_pvt_moleculeGroups(mgnum);
            
            molgroups.remove(molecules, mgnum);
            
            const MoleculeGroup &molgroup = molgroups.at(mgnum);
            
            for (Molecules::const_iterator it = molecules.constBegin();
                 it != molecules.constEnd();
                 ++it)
            {
                if (not molgroup.contains(it->number()))
                    this->removeFromIndex(mgnum, it->number());
            }
        }
        
        sysversion.incrementMinor();
    }
    catch(...)
    {
        this->operator=(old_system);
        throw;
    }
}

/** Remove the views in the molecule group 'molgroup' from the specified 
    groups in this forcefield. Note that this only removes the specific views
    (and indeed only the first copy of this view if there 
    are duplicates) - it does not remove the atoms in this
    view from all of the other views
    
    \throw SireMol::missing_group
    \throw SireError::invalid_index
*/
void System::remove(const MoleculeGroup &molgroup, const MGID &mgid)
{
    this->remove(molgroup.molecules(), mgid);
}

/** Remove the all copies of the view in 'molview' from the specified 
    groups in this forcefield. Note that this only removes the specific views
    - it does not remove the atoms in this view from all of the other views
    
    \throw SireMol::missing_group
    \throw SireError::invalid_index
*/
void System::removeAll(const MoleculeView &molview, const MGID &mgid)
{
    QList<MGNum> mgnums = mgid.map(*this);
    
    System old_system(*this);
    
    try
    {
        foreach (MGNum mgnum, mgnums)
        {
            MolGroupsBase &molgroups = this->_pvt_moleculeGroups(mgnum);
            
            molgroups.removeAll(molview, mgnum);
            
            if (not molgroups.at(mgnum).contains(molview.data().number()))
                this->removeFromIndex(mgnum, molview.data().number());
        }
        
        sysversion.incrementMinor();
    }
    catch(...)
    {
        this->operator=(old_system);
        throw;
    }
}

/** Remove the all copies of the views in 'molviews' from the specified 
    groups in this forcefield. Note that this only removes the specific views
    - it does not remove the atoms in this view from all of the other views
    
    \throw SireMol::missing_group
    \throw SireError::invalid_index
*/
void System::removeAll(const ViewsOfMol &molviews, const MGID &mgid)
{
    QList<MGNum> mgnums = mgid.map(*this);
    
    System old_system(*this);
    
    try
    {
        foreach (MGNum mgnum, mgnums)
        {
            MolGroupsBase &molgroups = this->_pvt_moleculeGroups(mgnum);
            
            molgroups.removeAll(molviews, mgnum);
            
            if (not molgroups.at(mgnum).contains(molviews.number()))
                this->removeFromIndex(mgnum, molviews.number());
        }
        
        sysversion.incrementMinor();
    }
    catch(...)
    {
        this->operator=(old_system);
        throw;
    }
}

/** Remove the all copies of the molecules in 'molecules' from the specified 
    groups in this forcefield. Note that this only removes the specific views
    - it does not remove the atoms in this view from all of the other views
    
    \throw SireMol::missing_group
    \throw SireError::invalid_index
*/
void System::removeAll(const Molecules &molecules, const MGID &mgid)
{
    QList<MGNum> mgnums = mgid.map(*this);
    
    System old_system(*this);
    
    try
    {
        foreach (MGNum mgnum, mgnums)
        {
            MolGroupsBase &molgroups = this->_pvt_moleculeGroups(mgnum);
            
            molgroups.removeAll(molecules, mgnum);
            
            const MoleculeGroup &molgroup = molgroups.at(mgnum);
            
            for (Molecules::const_iterator it = molecules.constBegin();
                 it != molecules.constEnd();
                 ++it)
            {
                if (not molgroup.contains(it->number()))
                    this->removeFromIndex(mgnum, it->number());
            }
        }
        
        sysversion.incrementMinor();
    }
    catch(...)
    {
        this->operator=(old_system);
        throw;
    }
}

/** Remove the all copies of the molecules in the molecule group 'molgroup' 
    from the specified groups in this forcefield. Note that this only removes
    the specific views - it does not remove the atoms in this view from all 
    of the other views
    
    \throw SireMol::missing_group
    \throw SireError::invalid_index
*/
void System::removeAll(const MoleculeGroup &molgroup, const MGID &mgid)
{
    this->removeAll(molgroup.molecules(), mgid);
}

/** Remove all views of the molecule with number 'molnum' from the molecule
    groups identified by 'mgid'
    
    \throw SireMol::missing_group
    \throw SireError::invalid_index
*/
void System::remove(MolNum molnum, const MGID &mgid)
{
    QList<MGNum> mgnums = mgid.map(*this);
    
    System old_system(*this);
    
    try
    {
        foreach (MGNum mgnum, mgnums)
        {
            this->_pvt_moleculeGroups(mgnum).remove(molnum, mgnum);
            this->removeFromIndex(mgnum, molnum);
        }
        
        sysversion.incrementMinor();
    }
    catch(...)
    {
        this->operator=(old_system);
        throw;
    }
}

/** Remove all of the molecules whose numbers are in 'molnums' from
    all of the molecule groups identified by the ID 'mgid'
    
    \throw SireMol::missing_group
    \throw SireError::invalid_index
*/
void System::remove(const QSet<MolNum> &molnums, const MGID &mgid)
{
    QList<MGNum> mgnums = mgid.map(*this);
    
    System old_system(*this);
    
    try
    {
        foreach (MGNum mgnum, mgnums)
        {
            this->_pvt_moleculeGroups(mgnum).remove(molnums, mgnum);
            this->removeFromIndex(mgnum, molnums);
        }
        
        sysversion.incrementMinor();
    }
    catch(...)
    {
        this->operator=(old_system);
        throw;
    }
}

/** Update this system so that it uses the version of the molecule
    available in 'moldata'
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void System::update(const MoleculeData &moldata)
{
    if (this->_pvt_constMoleculeGroups().contains(moldata.number()))
    {
        if (this->_pvt_constForceFields().contains(moldata.number()))
        {
            System old_system(*this);
            
            try
            {
                this->_pvt_forceFields().update(moldata);
                this->_pvt_moleculeGroups().update(moldata);
            }
            catch(...)
            {
                this->operator=(old_system);
                throw;
            }
        }
        else
        {
            this->_pvt_moleculeGroups().update(moldata);
        }
    }
    else
    {
        this->_pvt_forceFields().update(moldata);
    }

    sysversion.incrementMinor();
}

/** Update this system so that it uses the same version of the molecules
    present in 'molecules'
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void System::update(const Molecules &molecules)
{
    if (molecules.isEmpty())
        return;
    else if (molecules.count() == 1)
    {
        this->update( molecules.constBegin()->data() );
        return;
    }

    if (this->_pvt_constMoleculeGroups().isEmpty())
    {
        this->_pvt_forceFields().update(molecules);
    }
    else
    {
        System old_system(*this);
        
        try
        {
            this->_pvt_forceFields().update(molecules);
            this->_pvt_moleculeGroups().update(molecules);
        }
        catch(...)
        {
            this->operator=(old_system);
            throw;
        }
    }
    
    sysversion.incrementMinor();
}

/** Update this system so that it uses the same version of the molecules
    present in the molecule group 'molgroup'
    
    \throw SireBase::missing_property
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/
void System::update(const MoleculeGroup &molgroup)
{
    this->update(molgroup.molecules());
}

/** Set the contents of the molecule group(s) identified by the ID 'mgid'
    so that they contain just the view of the molecule in 'molview'.
    The version of the molecule already present in this set is used if 
    such a molecule already exists. The supplied property map
    is used to find the properties required by any forcefields
    that this molecule may be added to.
    
    \throw SireMol::missing_group
    \throw SireBase::missing_property
    \throw SireError::invalid_index
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/    
void System::setContents(const MGID &mgid, const MoleculeView &molview,
                         const PropertyMap &map)
{
    QList<MGNum> mgnums = mgid.map(*this);
    
    PartialMolecule view(molview);
    view.update( this->matchToExistingVersion(molview.data()) );
    
    System old_system(*this);
    
    try
    {
        foreach (MGNum mgnum, mgnums)
        {
            if (mgroups_by_num.value(mgnum) == 0)
                this->_pvt_forceFields().setContents(mgnum, view, map);
            else
                this->_pvt_moleculeGroups(mgnum).setContents(mgnum, view);

            this->clearIndex(mgnum);
            this->addToIndex(mgnum, view.data().number());
        }
        
        sysversion.incrementMinor();
    }
    catch(...)
    {
        this->operator=(old_system);
        throw;
    }
}

/** Set the contents of the molecule group(s) identified by the ID 'mgid'
    so that they contain just the views of the molecule in 'molviews'.
    The version of the molecule already present in this set is used if 
    such a molecule already exists. The supplied property map
    is used to find the properties required by any forcefields
    that this molecule may be added to.
    
    \throw SireMol::missing_group
    \throw SireBase::missing_property
    \throw SireError::invalid_index
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/    
void System::setContents(const MGID &mgid, const ViewsOfMol &molviews,
                         const PropertyMap &map)
{
    QList<MGNum> mgnums = mgid.map(*this);
    
    ViewsOfMol views(molviews);
    views.update( this->matchToExistingVersion(molviews.data()) );
    
    System old_system(*this);
    
    try
    {
        foreach (MGNum mgnum, mgnums)
        {
            if (mgroups_by_num.value(mgnum) == 0)
                this->_pvt_forceFields().setContents(mgnum, views, map);
            else
                this->_pvt_moleculeGroups(mgnum).setContents(mgnum, views);

            this->clearIndex(mgnum);
            this->addToIndex(mgnum, views.number());
        }
        
        sysversion.incrementMinor();
    }
    catch(...)
    {
        this->operator=(old_system);
        throw;
    }
}

/** Set the contents of the molecule group(s) identified by the ID 'mgid'
    so that they contain just the views of the molecules in 'molecules'.
    The version of the molecule already present in this set is used if 
    such a molecule already exists. The supplied property map
    is used to find the properties required by any forcefields
    that this molecule may be added to.
    
    \throw SireMol::missing_group
    \throw SireBase::missing_property
    \throw SireError::invalid_index
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/    
void System::setContents(const MGID &mgid, const Molecules &molecules,
                         const PropertyMap &map)
{
    QList<MGNum> mgnums = mgid.map(*this);
    
    Molecules mols = this->matchToExistingVersion(molecules);
    
    System old_system(*this);
    
    try
    {
        foreach (MGNum mgnum, mgnums)
        {
            if (mgroups_by_num.value(mgnum) == 0)
                this->_pvt_forceFields().setContents(mgnum, mols, map);
            else
                this->_pvt_moleculeGroups(mgnum).setContents(mgnum, mols);

            this->clearIndex(mgnum);
            this->addToIndex(mgnum, mols.molNums());
        }
        
        sysversion.incrementMinor();
    }
    catch(...)
    {
        this->operator=(old_system);
        throw;
    }
}

/** Set the contents of the molecule group(s) identified by the ID 'mgid'
    so that they contain just the molecules in the group 'molgroup'.
    The version of the molecule already present in this set is used if 
    such a molecule already exists. The supplied property map
    is used to find the properties required by any forcefields
    that this molecule may be added to.
    
    \throw SireMol::missing_group
    \throw SireBase::missing_property
    \throw SireError::invalid_index
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/    
void System::setContents(const MGID &mgid, const MoleculeGroup &molgroup,
                         const PropertyMap &map)                         
{
    QList<MGNum> mgnums = mgid.map(*this);
    
    MolGroupPtr group(molgroup);
    group.edit().update( this->matchToExistingVersion(molgroup.molecules()) );
    
    System old_system(*this);
    
    try
    {
        foreach (MGNum mgnum, mgnums)
        {
            if (mgroups_by_num.value(mgnum) == 0)
                this->_pvt_forceFields().setContents(mgnum, group, map);
            else
                this->_pvt_moleculeGroups(mgnum).setContents(mgnum, group);

            this->clearIndex(mgnum);
            this->addToIndex(mgnum, group->molNums());
        }
        
        sysversion.incrementMinor();
    }
    catch(...)
    {
        this->operator=(old_system);
        throw;
    }
}

/** Convenient overload of System::setContents that uses the default
    property locations to find the properties required by the forcefields
    
    \throw SireMol::missing_group
    \throw SireBase::missing_property
    \throw SireError::invalid_index
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/    
void System::setContents(const MGID &mgid, const MoleculeView &molview)
{
    this->setContents(mgid, molview, PropertyMap());
}

/** Convenient overload of System::setContents that uses the default
    property locations to find the properties required by the forcefields
    
    \throw SireMol::missing_group
    \throw SireBase::missing_property
    \throw SireError::invalid_index
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/    
void System::setContents(const MGID &mgid, const ViewsOfMol &molviews)
{
    this->setContents(mgid, molviews, PropertyMap());
}

/** Convenient overload of System::setContents that uses the default
    property locations to find the properties required by the forcefields
    
    \throw SireMol::missing_group
    \throw SireBase::missing_property
    \throw SireError::invalid_index
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/    
void System::setContents(const MGID &mgid, const Molecules &molecules)
{
    this->setContents(mgid, molecules, PropertyMap());
}

/** Convenient overload of System::setContents that uses the default
    property locations to find the properties required by the forcefields
    
    \throw SireMol::missing_group
    \throw SireBase::missing_property
    \throw SireError::invalid_index
    \throw SireError::invalid_cast
    \throw SireError::incompatible_error
*/    
void System::setContents(const MGID &mgid, const MoleculeGroup &molgroup)
{
    this->setContents(mgid, molgroup, PropertyMap());
}
