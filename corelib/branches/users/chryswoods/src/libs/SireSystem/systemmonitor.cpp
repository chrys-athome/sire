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

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireSystem;
using namespace SireBase;
using namespace SireStream;

//////////////
////////////// Implementation of SystemMonitorBase
//////////////

static const RegisterMetaType<SystemMonitorBase> r_sysmonbase(MAGIC_ONLY,
                                                    "SireSystem::SystemMonitorBase");

/** Serialise to a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator<<(QDataStream &ds,
                                          const SystemMonitorBase &sysmonbase)
{
    writeHeader(ds, r_sysmonbase, 1)
        << sysmonbase.nupdates
        << static_cast<const PropertyBase&>(sysmonbase);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator>>(QDataStream &ds,
                                          SystemMonitorBase &sysmonbase)
{
    VersionID v = readHeader(ds, r_sysmonbase);

    if (v == 1)
    {
        ds >> sysmonbase.nupdates >> static_cast<PropertyBase&>(sysmonbase);
    }
    else
        throw version_error(v, "1", r_sysmonbase, CODELOC);

    return ds;
}

/** Null constructor */
SystemMonitorBase::SystemMonitorBase() : PropertyBase(), nupdates(0)
{}

/** Copy constructor */
SystemMonitorBase::SystemMonitorBase(const SystemMonitorBase &other)
                  : PropertyBase(other), nupdates(other.nupdates)
{}

/** Destructor */
SystemMonitorBase::~SystemMonitorBase()
{}

/** Copy assignment operator */
SystemMonitorBase& SystemMonitorBase::operator=(const SystemMonitorBase &other)
{
    PropertyBase::operator=(other);

    nupdates = other.nupdates;

    return *this;
}

/** Monitor the passed system - this will calculate the property
    that is being monitored for the current configuration of the system
    and will record it in the monitor */
void SystemMonitorBase::monitor(QuerySystem&)
{
    ++nupdates;
}

//////////////
////////////// Implementation of NullMonitor
//////////////

namespace SireSystem
{
class NullMonitor;
}

QDataStream& operator<<(QDataStream&, const SireSystem::NullMonitor&);
QDataStream& operator>>(QDataStream&, SireSystem::NullMonitor&);

namespace SireSystem
{

class SIRESYSTEM_EXPORT NullMonitor : public SystemMonitorBase
{

friend QDataStream& ::operator<<(QDataStream&, const NullMonitor&);
friend QDataStream& ::operator>>(QDataStream&, NullMonitor&);

public:
    NullMonitor() : SystemMonitorBase()
    {}

    NullMonitor(const NullMonitor &other) : SystemMonitorBase(other)
    {}

    ~NullMonitor()
    {}

    NullMonitor& operator=(const NullMonitor &other)
    {
        SystemMonitorBase::operator=(other);
        return *this;
    }

    NullMonitor* clone() const
    {
        return new NullMonitor(*this);
    }

    static const char* typeName()
    {
        return "SireSystem::NullMonitor";
    }

    const char* what() const
    {
        return NullMonitor::typeName();
    }

    double value() const
    {
        return 0;
    }

    void clear()
    {}

    void assertCompatibleWith(const QuerySystem&) const
    {}

protected:
    bool _pvt_isEqual(const PropertyBase &other) const
    {
        BOOST_ASSERT( other.isA<NullMonitor>() );
        return true;
    }
};

}

Q_DECLARE_METATYPE(SireSystem::NullMonitor);

static const RegisterMetaType<NullMonitor> r_nullmonitor;

/** Serialise to a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator<<(QDataStream &ds,
                                          const NullMonitor &nullmonitor)
{
    writeHeader(ds, r_nullmonitor, 1)
        << static_cast<const SystemMonitorBase&>(nullmonitor);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator>>(QDataStream &ds,
                                          NullMonitor &nullmonitor)
{
    VersionID v = readHeader(ds, r_nullmonitor);

    if (v == 1)
    {
        ds >> static_cast<SystemMonitorBase&>(nullmonitor);
    }
    else
        throw version_error(v, "1", r_nullmonitor, CODELOC);

    return ds;
}

//////////////
////////////// Implementation of SystemMonitor
//////////////

static const RegisterMetaType<SystemMonitor> r_sysmon;

/** Serialise to a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator<<(QDataStream &ds,
                                          const SystemMonitor &sysmon)
{
    writeHeader(ds, r_sysmon, 1);

    SharedDataStream sds(ds);

    sds << sysmon.d;

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator>>(QDataStream &ds,
                                          SystemMonitor &sysmon)
{
    VersionID v = readHeader(ds, r_sysmon);

    if (v == 1)
    {
        SharedDataStream sds(ds);
        sds >> sysmon.d;
    }
    else
        throw version_error(v, "1", r_sysmon, CODELOC);

    return ds;
}

static SharedPolyPointer<SystemMonitorBase> shared_null;

static SharedPolyPointer<SystemMonitorBase>& getSharedNull()
{
    if (shared_null.constData() == 0)
        shared_null = new NullMonitor();

    return shared_null;
}

/** Construct a null system monitor */
SystemMonitor::SystemMonitor() : d( getSharedNull() )
{}

/** Construct from the passed monitor */
SystemMonitor::SystemMonitor(const SystemMonitorBase &monitor)
              : d(monitor)
{}

/** Copy constructor */
SystemMonitor::SystemMonitor(const SystemMonitor &other)
              : d(other.d)
{}

/** Destructor */
SystemMonitor::~SystemMonitor()
{}

/** Copy assignment operator */
SystemMonitor& SystemMonitor::operator=(const SystemMonitor &other)
{
    d = other.d;
    return *this;
}

/** Comparison operator */
bool SystemMonitor::operator==(const SystemMonitor &other) const
{
    return d == other.d or *d == *(other.d);
}

/** Comparison operator */
bool SystemMonitor::operator!=(const SystemMonitor &other) const
{
    return d != other.d and *d != *(other.d);
}

/** Return the base class of this monitor */
const SystemMonitorBase& SystemMonitor::base() const
{
    return *d;
}

/** Return the class name of this monitor */
const char* SystemMonitor::what() const
{
    return d->what();
}

/** Monitor the current configuration of the passed system and
    add the result into the monitor */
void SystemMonitor::monitor(QuerySystem &system)
{
    d->monitor(system);
}

/** Return the current value of the monitor - some monitors
    may not have a valid value */
double SystemMonitor::value() const
{
    return d->value();
}

/** Return the number of times this monitor has been updated */
quint64 SystemMonitor::nUpdates() const
{
    return d->nUpdates();
}

/** Completely clear the statistics within this monitor */
void SystemMonitor::clear()
{
    d->clear();
}

/** Assert that this monitor is compatible with the passed system */
void SystemMonitor::assertCompatibleWith(const QuerySystem &system) const
{
    d->assertCompatibleWith(system);
}
