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

#include "systemmonitor.h"
#include "system.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireSystem;
using namespace SireBase;
using namespace SireStream;

/////////
///////// Implementation of SysMonBase
/////////

static const RegisterMetaType<SysMonBase> r_sysmonbase( MAGIC_ONLY,
                                                        "SireSystem::SysMonBase" );
                                                        
/** Serialise to a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator<<(QDataStream &ds, const SysMonBase &sysmonbase)
{
    writeHeader(ds, r_sysmonbase, 1);
    
    ds << static_cast<const PropertyBase&>(sysmonbase);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator>>(QDataStream &ds, SysMonBase &sysmonbase)
{
    VersionID v = readHeader(ds, r_sysmonbase);
    
    if (v == 1)
    {
        ds >> static_cast<PropertyBase&>(sysmonbase);
    }
    else
        throw version_error(v, "1", r_sysmonbase, CODELOC);
        
    return ds;
}

/** Constructor */
SysMonBase::SysMonBase() : PropertyBase()
{}

/** Copy constructor */
SysMonBase::SysMonBase(const SysMonBase &other)
           : PropertyBase(other)
{}

/** Destructor */
SysMonBase::~SysMonBase()
{}

/////////
///////// Implementation of NullMonitor
/////////

static const RegisterMetaType<NullMonitor> r_nullmonitor;
                                                        
/** Serialise to a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator<<(QDataStream &ds, 
                                          const NullMonitor &nullmonitor)
{
    writeHeader(ds, r_nullmonitor, 1);
    
    ds << static_cast<const SysMonBase&>(nullmonitor);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator>>(QDataStream &ds, NullMonitor &nullmonitor)
{
    VersionID v = readHeader(ds, r_nullmonitor);
    
    if (v == 1)
    {
        ds >> static_cast<SysMonBase&>(nullmonitor);
    }
    else
        throw version_error(v, "1", r_nullmonitor, CODELOC);
        
    return ds;
}

/** Constructor */
NullMonitor::NullMonitor() : ConcreteProperty<NullMonitor,SysMonBase>()
{}

/** Copy constructor */
NullMonitor::NullMonitor(const NullMonitor &other)
            : ConcreteProperty<NullMonitor,SysMonBase>(other)
{}

/** Destructor */
NullMonitor::~NullMonitor()
{}

/** Copy assignment operator */
NullMonitor& NullMonitor::operator=(const NullMonitor &other)
{
    SysMonBase::operator=(other);
    return *this;
}

/** Comparison operator */
bool NullMonitor::operator==(const NullMonitor &other) const
{
    return true;
}

/** Comparison operator */
bool NullMonitor::operator!=(const NullMonitor &other) const
{
    return false;
}

/** A null monitor doesn't monitor anything! */
void NullMonitor::monitor(System &system)
{
    return;
}

/////////
///////// Implementation of SystemMonitor
/////////

static const RegisterMetaType<SystemMonitor> r_monitor;

/** Serialise a SystemMonitor to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, const SystemMonitor &monitor)
{
    writeHeader(ds, r_monitor, 1);
    
    SharedDataStream sds(ds);
    
    sds << static_cast<const Property&>(monitor);
    
    return ds;
}

/** Deserialise a SystemMonitor from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, SystemMonitor &monitor)
{
    VersionID v = readHeader(ds, r_monitor);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        sds >> static_cast<Property&>(monitor);
    }
    else
        throw version_error(v, "1", r_monitor, CODELOC);
        
    return ds;
}

static SystemMonitor *_pvt_shared_null = 0;

const SystemMonitor& SystemMonitor::shared_null()
{
    if (_pvt_shared_null == 0)
        _pvt_shared_null = new SystemMonitor( NullMonitor() );
        
    return *_pvt_shared_null;
}

/** Null constructor - constructs a simple, empty SysMonBase */
SystemMonitor::SystemMonitor() : Property(SystemMonitor::shared_null())
{}

/** Construct from a passed property

    \throw SireError::invalid_cast
*/
SystemMonitor::SystemMonitor(const PropertyBase &property) 
              : Property(property.asA<SysMonBase>())
{}

/** Construct from passed SysMonBase */
SystemMonitor::SystemMonitor(const SysMonBase &monitor) : Property(monitor)
{}

/** Copy constructor */
SystemMonitor::SystemMonitor(const SystemMonitor &other) : Property(other)
{}

/** Destructor */
SystemMonitor::~SystemMonitor()
{}

/** Copy assignment operator from a Property object

    \throw SireError::invalid_cast
*/
SystemMonitor& SystemMonitor::operator=(const PropertyBase &property)
{
    Property::operator=(property.asA<SysMonBase>());
    return *this;
}

/** Copy assignment operator from another SysMonBase */
SystemMonitor& SystemMonitor::operator=(const SysMonBase &other)
{
    Property::operator=(other);
    return *this;
}

/** Dereference this pointer */
const SysMonBase* SystemMonitor::operator->() const
{
    return static_cast<const SysMonBase*>(&(d()));
}

/** Dereference this pointer */
const SysMonBase& SystemMonitor::operator*() const
{
    return static_cast<const SysMonBase&>(d());
}

/** Return a read-only reference to the contained object */
const SysMonBase& SystemMonitor::read() const
{
    return static_cast<const SysMonBase&>(d());
}

/** Return a modifiable reference to the contained object.
    This will trigger a copy of the object if more than
    one pointer is pointing to it. */
SysMonBase& SystemMonitor::edit()
{
    return static_cast<SysMonBase&>(d());
}
    
/** Return a raw pointer to the SysMonBase object */
const SysMonBase* SystemMonitor::data() const
{
    return static_cast<const SysMonBase*>(&(d()));
}

/** Return a raw pointer to the SysMonBase object */
const SysMonBase* SystemMonitor::constData() const
{
    return static_cast<const SysMonBase*>(&(d()));
}
    
/** Return a raw pointer to the SysMonBase object */
SysMonBase* SystemMonitor::data()
{
    return static_cast<SysMonBase*>(&(d()));
}

/** Automatic casting operator */
SystemMonitor::operator const SysMonBase&() const
{
    return static_cast<const SysMonBase&>(d());
}
