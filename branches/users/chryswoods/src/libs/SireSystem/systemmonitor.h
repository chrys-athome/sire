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

#ifndef SIRESYSTEM_SYSTEMMONITOR_H
#define SIRESYSTEM_SYSTEMMONITOR_H

#include "SireBase/property.h"

SIRE_BEGIN_HEADER

namespace SireSystem
{
class SystemMonitorBase;
class SystemMonitor;
}

QDataStream& operator<<(QDataStream&, const SireSystem::SystemMonitorBase&);
QDataStream& operator>>(QDataStream&, SireSystem::SystemMonitorBase&);

QDataStream& operator<<(QDataStream&, const SireSystem::SystemMonitor&);
QDataStream& operator>>(QDataStream&, SireSystem::SystemMonitor&);

namespace SireSystem
{

class QuerySystem;

/** This is the base class of all monitor classes

    @author Christopher Woods
*/
class SIRESYSTEM_EXPORT SystemMonitorBase : public SireBase::PropertyBase
{

friend QDataStream& ::operator<<(QDataStream&, const SystemMonitorBase&);
friend QDataStream& ::operator>>(QDataStream&, SystemMonitorBase&);

public:
    SystemMonitorBase();

    SystemMonitorBase(const SystemMonitorBase &other);

    virtual ~SystemMonitorBase();

    SystemMonitorBase& operator=(const SystemMonitorBase &other);

    virtual SystemMonitorBase* clone() const=0;

    virtual void monitor(QuerySystem &system);

    /** Convert the monitor's result into a single value - this
        will only be meaningful for some monitors */
    virtual double value() const=0;

    /** Return the number of times that this monitor has been updated */
    quint64 nUpdates() const
    {
        return nupdates;
    }

    /** Completely clear all of the monitor's statistics */
    virtual void clear()
    {
        nupdates = 0;
    }

    /** Assert that this monitor is compatible with the passed system */
    virtual void assertCompatibleWith(const QuerySystem &system) const=0;

private:
    quint64 nupdates;
};

/** This is the holder of SystemMonitor objects */
class SIRESYSTEM_EXPORT SystemMonitor
{

friend QDataStream& ::operator<<(QDataStream&, const SystemMonitor&);
friend QDataStream& ::operator>>(QDataStream&, SystemMonitor&);

public:
    SystemMonitor();

    SystemMonitor(const SystemMonitorBase &monitor);
    SystemMonitor(const SystemMonitor &other);

    ~SystemMonitor();

    SystemMonitor& operator=(const SystemMonitor &other);

    bool operator==(const SystemMonitor &other) const;
    bool operator!=(const SystemMonitor &other) const;

    const SystemMonitorBase& base() const;
    const char* what() const;

    void monitor(QuerySystem &system);

    double value() const;

    quint64 nUpdates() const;

    void clear();

    void assertCompatibleWith(const QuerySystem &system) const;

    template<class T>
    bool isA() const
    {
        return d->isA<T>();
    }

    template<class T>
    const T& asA() const
    {
        return d->asA<T>();
    }

private:
    /** Implicitly shared pointer to the monitor */
    SireBase::SharedPolyPointer<SystemMonitorBase> d;
};

}

Q_DECLARE_METATYPE(SireSystem::SystemMonitor);

SIRE_END_HEADER

#endif
