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

#ifndef SIRESYSTEM_MONITORS_H
#define SIRESYSTEM_MONITORS_H

#include "systemmonitor.h"

#include "SireCAS/symbol.h"

#include <QHash>

SIRE_BEGIN_HEADER

namespace SireSystem
{
class SystemMonitors;
}

QDataStream& operator<<(QDataStream&, const SireSystem::SystemMonitors&);
QDataStream& operator>>(QDataStream&, SireSystem::SystemMonitors&);

namespace SireSystem
{

class QuerySystem;

using SireCAS::Symbol;

/** This class will eventually be the container for the variety
    of monitors that will monitor properties of a running simulation
    system

    @author Christopher Woods
*/
class SIRESYSTEM_EXPORT SystemMonitors
{

friend QDataStream& ::operator<<(QDataStream&, const SystemMonitors&);
friend QDataStream& ::operator>>(QDataStream&, SystemMonitors&);

public:
    SystemMonitors();

    SystemMonitors(const SystemMonitors &other);

    ~SystemMonitors();

    SystemMonitors& operator=(const SystemMonitors &other);

    bool nMonitors() const;
    bool count() const;

    bool isEmpty() const;

    bool contains(const Symbol &symbol) const;

    void set(const Symbol &symbol, const SystemMonitor &monitor,
             quint64 delta = 1);

    void setDelta(const Symbol &symbol, quint64 delta);
    quint64 getDelta(const Symbol &symbol) const;

    SystemMonitor take(const Symbol &symbol);
    void remove(const Symbol &symbol);

    const SystemMonitor& monitor(const Symbol &symbol) const;

    double value(const Symbol &symbol) const;

    void update(QuerySystem &system);
    void forceUpdate(QuerySystem &system);

    void monitor(QuerySystem &system);

    const QHash<Symbol,SystemMonitor>& monitors() const;

    quint64 nUpdates() const;

    void resetStatistics();

    void clear();

    void assertContains(const Symbol &symbol) const;
    void assertCompatibleWith(const QuerySystem &system) const;

private:
    /** All of the system monitors, indexed by the symbol
        used to represent them */
    QHash<Symbol, SystemMonitor> montrs;

    /** All of the symbols that must be updated every
        n steps (n is the key) */
    QHash< quint64, QSet<Symbol> > deltas;

    /** The number of times this collection of monitors
        has been updated */
    quint64 nupdates;
};

}

Q_DECLARE_METATYPE(SireSystem::SystemMonitors);

SIRE_END_HEADER

#endif
