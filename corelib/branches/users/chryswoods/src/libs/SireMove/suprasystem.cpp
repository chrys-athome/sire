/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2009  Christopher Woods
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

#include "suprasystem.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMove;
using namespace SireSystem;
using namespace SireBase;
using namespace SireStream;

static const RegisterMetaType<SupraSystem> r_suprasystem;

/** Serialise to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, const SupraSystem &suprasystem)
{
    writeHeader(ds, r_suprasystem, 1);
    
    SharedDataStream sds(ds);
    
    sds << suprasystem.subsystems
        << static_cast<const Property&>(suprasystem);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, SupraSystem &suprasystem)
{
    VersionID v = readHeader(ds, r_suprasystem);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> suprasystem.subsystems
            >> static_cast<Property&>(suprasystem);
    }
    else
        throw version_error(v, "1", r_suprasystem, CODELOC);
        
    return ds;
}

/** Null Constructor */
SupraSystem::SupraSystem() : ConcreteProperty<SupraSystem,Property>()
{}

/** Construct a supra-system consisting of 'n' sub systems */
SupraSystem::SupraSystem(int n)
            : ConcreteProperty<SupraSystem,Property>(),
              subsystems(n)
{
    subsystems.squeeze();
}

/** Construct a supra-system that consists of 'n' sub systems,
    which are initially created to be equal to 'system' */
SupraSystem::SupraSystem(const System &system, int n)
            : ConcreteProperty<SupraSystem,Property>(),
              subsystems(n)
{
    subsystems.squeeze();
    
    if (n > 0)
    {
        SupraSubSystem &subsys = subsystems[0];
        
        subsys.setSubSystem(system);
        
        for (int i=1; i<subsystems.count(); ++i)
        {
            subsystems[i] = subsys;
        }
    }
}

/** Construct a supra-system that consists of the sub systems
    that are copies of those in 'systems' */
SupraSystem::SupraSystem(const QVector<System> &systems)
            : ConcreteProperty<SupraSystem,Property>(),
              subsystems(systems.count())
{
    subsystems.squeeze();
    
    int nsystems = systems.count();
    
    for (int i=0; i<nsystems; ++i)
    {
        subsystems[i].setSystem( systems.at(i) );
    }
}

/** Copy constructor */
SupraSystem::SupraSystem(const SupraSystem &other)
            : ConcreteProperty<SupraSystem,Property>(other), 
              subsystems(other.subsystems)
{}

/** Destructor */
SupraSystem::~SupraSystem()
{}

/** Copy assignment operator */
SupraSystem& SupraSystem::operator=(const SupraSystem &other)
{
    Property::operator=(other);
    subsystems = other.subsystems;
    
    return *this;
}

/** Comparison operator */
bool SupraSystem::operator==(const SupraSystem &other) const
{
    return (this == &other) or
           (subsystems.constData() == other.subsystems.constData()) or
           (subsystems == other.subsystems);
}

/** Comparison operator */
bool SupraSystem::operator!=(const SupraSystem &other) const
{
    return not this->operator==(other);
}

Q_GLOBAL_STATIC( SupraSystem, supraSystem )

/** Return the global null SupraSystem */
const SupraSystem& SupraSystem::null()
{
    return *(supraSystem());
}

/** This function is called just before all of the sub-systems are packed */
void SupraSystem::_pre_pack()
{}

/** This function is called just after all of the sub-systems are packed */
void SupraSystem::_post_pack()
{}

/** This function is called just before all of the sub-systems are unpacked */
void SupraSystem::_pre_unpack()
{}

/** This function is called just after all of the sub-systems are unpacked */
void SupraSystem::_post_unpack()
{}

/** Internal function used to return the 'ith' sub-system. No bounds
    checking is performed in 'i' */
const SupraSubSystem& SupraSystem::_pvt_subSystem(int i) const
{
    if (i < 0 or i >= subsystems.count())
        throw SireError::program_bug( QObject::tr(
            "Should not access system %1 as count == %2")
                .arg(i).arg(subsystems.count()), CODELOC );

    return subsystems.constData()[i];
}

/** Internal function used to return the 'ith' sub-system. No bounds
    checking is performed in 'i' */
SupraSubSystem& SupraSystem::_pvt_subSystem(int i)
{
    if (i < 0 or i >= subsystems.count())
        throw SireError::program_bug( QObject::tr(
            "Should not access system %1 as count == %2")
                .arg(i).arg(subsystems.count()), CODELOC );

    return subsystems.data()[i];
}

/** Return the ith sub-system in this supra-system

    \throw SireError::invalid_index
*/
const SupraSubSystem& SupraSystem::operator[](int i) const
{
    return this->_pvt_subSystem( Index(i).map( subsystems.count() ) );
}

/** Return the ith sub-system in this supra-system

    \throw SireError::invalid_index
*/
const SupraSubSystem& SupraSystem::at(int i) const
{
    return this->operator[](i);
}

/** Return whether or not this SupraSystem contains no subsystems */
bool SupraSystem::isEmpty() const
{
    return subsystems.isEmpty();
}

/** Return the number of sub-systems in this supra-system */
int SupraSystem::nSubSystems() const
{
    return subsystems.count();
}

/** Return the number of sub-systems in this supra-system */
int SupraSystem::count() const
{
    return this->nSubSystems();
}

/** Return the number of sub-systems in this supra-system */
int SupraSystem::size()
{
    return this->nSubSystems();
}

/** Clear the statistics that are collected between blocks of 
    sub-system moves */
void SupraSystem::clearStatistics()
{
    int n = subsystems.count();
    
    for (int i=0; i<n; ++i)
    {
        subsystems[i].edit().clearStatistics();
    }
}

/** Clear the statistics that are collected within blocks of
    sub-system moves */
void SupraSystem::clearSubStatistics()
{
    int n = subsystems.count();
    
    for (int i=0; i<n; ++i)
    {
        subsystems[i].edit().clearSubStatistics();
    }
}

/** Clear all statistics collected during the moves */
void SupraSystem::clearAllStatistics()
{
    int n = subsystems.count();
    
    for (int i=0; i<n; ++i)
    {
        subsystems[i].edit().clearAllStatistics();
    }
}

/** Tell all sub-systems that the next energy calculate must
    be performed from scratch - this is useful for debugging */
void SupraSystem::mustNowRecalculateFromScratch()
{
    int n = subsystems.count();
    
    for (int i=0; i<n; ++i)
    {
        subsystems[i].edit().mustNowRecalculateFromScratch();
    }
}

/** Return whether or not *all* sub-systems are packed */
bool SupraSystem::isPacked() const
{
    int n = subsystems.count();
    
    for (int i=0; i<n; ++i)
    {
        if (not subsystems.at(i)->isPacked())
            return false;
    }
    
    return true;
}

/** Return whether or not *all* sub-systems are packed to memory */
bool SupraSystem::isPackedToMemory() const
{
    int n = subsystems.count();
    
    for (int i=0; i<n; ++i)
    {
        if (not subsystems.at(i)->isPackedToMemory())
            return false;
    }
    
    return true;
}

/** Return whether or not *all* sub-systems are packed to disk */
bool SupraSystem::isPackedToDisk() const
{
    int n = subsystems.count();
    
    for (int i=0; i<n; ++i)
    {
        if (not subsystems.at(i)->isPackedToDisk())
            return false;
    }
    
    return true;
}

/** Pack all sub-systems */
void SupraSystem::pack()
{
    int n = subsystems.count();

    if (n == 0)
        return;

    bool being_packed = not this->unPacked();

    if (being_packed)
        this->_pre_pack();
    
    for (int i=0; i<n; ++i)
    {
        if (not subsystems.at(i)->isPacked())
            subsystems[i].edit().pack();
    }
    
    if (being_packed)
        this->_post_pack();
}

/** Unpack all sub-systems */
void SupraSystem::unpack()
{
    int n = subsystems.count();
    
    if (n == 0)
        return;
    
    bool being_unpacked = this->anyPacked();
    
    if (being_unpacked)
        this->_pre_unpack();
    
    for (int i=0; i<n; ++i)
    {
        if (subsystems.at(i)->isPacked())
            subsystems[i].edit().unpack();
    }
    
    if (being_unpacked)
        this->_post_unpack();
}

/** Pack all sub-systems to disk */
void SupraSystem::packToDisk()
{
    int n = subsystems.count();
    
    if (n == 0)
        return;
        
    bool being_packed = not this->isPacked();
    
    if (being_packed)
        this->_pre_pack();
    
    for (int i=0; i<n; ++i)
    {
        if (not subsystems.at(i)->isPackedToDisk())
            subsystems[i].edit().packToDisk();
    }
    
    if (being_packed)
        this->_post_pack();
}

/** Pack all sub-systems to disk, in the directory 'tempdir'. Note
    that this will not move sub-systems that are already packed into
    a different directory */
void SupraSystem::packToDisk(const QString &tempdir)
{
    int n = subsystems.count();
    
    if (n == 0)
        return;
        
    bool being_packed = not this->isPacked();
    
    if (being_packed)
        this->_pre_pack();
    
    for (int i=0; i<n; ++i)
    {
        if (not subsystems.at(i)->isPackedToDisk())
            subsystems[i].edit().packToDisk(tempdir);
    }
    
    if (being_packed)
        this->_post_pack();
}

/** Pack all sub-systems to memory */
void SupraSystem::packToMemory()
{
    int n = subsystems.count();
    
    if (n == 0)
        return;
    
    bool being_packed = not this->isPacked();
    
    if (being_packed)
        this->_pre_pack();
    
    for (int i=0; i<n; ++i)
    {
        if (not subsystems.at(i)->isPackedToMemory())
            subsystems[i].edit().packToMemory();
    }
    
    if (being_packed)
        this->_post_pack();
}

/** Set the ith sub-system equal to 'subsystem'

    \throw SireError::invalid_index
*/
void SupraSystem::setSubSystem(int i, const SupraSubSystem &subsystem)
{
    subsystems[ Index(i).map(subsystems.count()) ] = subsystem;
}

/** Set all sub-systems equal to 'subsystem' */
void SupraSystem::setSubSystem(const SupraSubSystem &subsystem)
{
    int n = subsystems.count();
    
    for (int i=0; i<n; ++i)
    {
        this->setSubSystem(i, subsystem);
    }
}

/** Set all sub-systems equal to a copy of those in 'system'. Note
    that both this SupraSystem and 'other' must have the same
    number of sub-systems 
    
    \throw SireError::incompatible_error
*/
void SupraSystem::setSubSystems(const SupraSystem &system)
{
    int n = subsystems.count();
    
    if (system.nSubSystems() != n)
    {
        throw SireError::incompatible_error( QObject::tr(
            "You cannot set the sub-systems for this supra-system from the "
            "passed supra-system, as they have different numbers of "
            "sub-systems (%1 vs. %2)")
                .arg(n).arg(system.nSubSystems()), CODELOC );
    }

    for (int i=0; i<n; ++i)
    {
        this->setSubSystem(i, system[i]);
    }
}

/** Set the SireSystem::System used by the ith sub-system equal to 'system'

    \throw SireError::invalid_index
*/
void SupraSystem::setSubSystem(int i, const System &system)
{
    subsystems[ Index(i).map(subsystems.count()) ].edit().setSubSystem(system);
}

/** Set the SireSystem::System used by all sub-systems equal to 'system' */
void SupraSystem::setSubSystem(const System &system)
{
    int n = subsystems.count();
    
    for (int i=0; i<n; ++i)
    {
        this->setSubSystem(i, system);
    }
}

/** Set the moves that will be applied to the ith sub-system during
    each sub-move
    
    \throw SireError::invalid_index
*/
void SupraSystem::setSubMoves(int i, const Moves &moves)
{
    subsystems[ Index(i).map(subsystems.count()) ].edit().setSubMoves(moves);
}

/** Set the moves that will be applied to all sub-systems during
    each sub-move
    
    \throw SireError::invalid_index
*/
void SupraSystem::setSubMoves(const Moves &moves)
{
    int n = subsystems.count();
    
    for (int i=0; i<n; ++i)
    {
        this->setSubMoves(i, moves);
    }
}

/** Set the system and moves used for the ith sub-system to those
    contained in 'simstore'
    
    \throw SireError::invalid_index
*/
void SupraSystem::setSubSystemAndMoves(int i, const SimStore &simstore)
{
    subsystems[ Index(i).map(subsystems.count()) ].edit().setSubSystemAndMoves(simstore);
}

/** Set the system and moves used for all sub-systems to those
    contained in 'simstore' */
void SupraSystem::setSubSystemAndMoves(const SimStore &simstore)
{
    int n = subsystems.count();
    
    for (int i=0; i<n; ++i)
    {
        this->setSubSystemAndMoves(i, simstore);
    }
}

/** Set the system and moves that will be applied to the ith sub-system
    to 'system' and 'moves' 
    
    \throw SireError::invalid_index
*/
void SupraSystem::setSubSystemAndMoves(int i, const System &system, const Moves &moves)
{
    this->setSystemAndMoves( i, SimStore(system,moves) );
}

/** Set the system and moves used by all sub-systems to 
    'system' and 'moves' */
void SupraSystem::setSubSystemAndMoves(const System &system, const Moves &moves)
{
    this->setSystemAndMoves( SimStore(system,moves) );
}

/** Set the monitors used for all sub-systems to 'monitors', and set
    them all to update with a frequency of 'frequency' */
void SupraSystem::setSubMonitors(const SystemMonitors &monitors, int frequency)
{
    SystemMonitors new_monitors = monitors;
    new_monitors.setAllFrequency(frequency);
    
    int n = subsystems.count();
    
    for (int i=0; i<n; ++i)
    {
        subsystems[i].edit().setMonitors(monitors, frequency);
    }
}

void SupraSystem::setSubMonitors(int i, const SystemMonitors &monitors,
                        int frequency=1);

void SupraSystem::add(const QString &name, const SystemMonitor &monitor,
             int frequency=1);
             
void SupraSystem::add(int i, const QString &name, const SystemMonitor &monitor,
             int frequency=1);
             
void SupraSystem::add(const SystemMonitors &monitors, int frequency=1);
void SupraSystem::add(int i, const SystemMonitors &monitors,
             int frequency=1);

void SupraSystem::setNSubMoves(int nmoves);
void SupraSystem::setNSubMoves(int i, int nmoves);

void SupraSystem::setRecordSubStatistics(bool record_stats);
void SupraSystem::setRecordSubStatistics(int i, bool record_stats);
