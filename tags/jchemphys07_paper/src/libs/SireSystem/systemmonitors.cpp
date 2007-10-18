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

#include "SireCAS/qhash_sirecas.h"

#include "systemmonitors.h"
#include "querysystem.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include "tostring.h"

#include "SireSystem/errors.h"

#include <boost/assert.hpp>

using namespace SireStream;
using namespace SireSystem;

static const RegisterMetaType<SystemMonitors> r_sysmons;

/** Serialise to a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator<<(QDataStream &ds,
                                          const SystemMonitors &sysmons)
{
    writeHeader(ds, r_sysmons, 1);

    SharedDataStream sds(ds);

    sds << sysmons.montrs << sysmons.deltas << sysmons.nupdates;

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIRE_EXPORT &operator>>(QDataStream &ds,
                                    SystemMonitors &sysmons)
{
    VersionID v = readHeader(ds, r_sysmons);

    if (v == 1)
    {
        SharedDataStream sds(ds);

        sds >> sysmons.montrs >> sysmons.deltas >> sysmons.nupdates;
    }
    else
        throw version_error(v, "1", r_sysmons, CODELOC);

    return ds;
}

/** Null constructor */
SystemMonitors::SystemMonitors() : nupdates(0)
{}

/** Copy constructor */
SystemMonitors::SystemMonitors(const SystemMonitors &other)
               : montrs(other.montrs), deltas(other.deltas),
                 nupdates(other.nupdates)
{}

/** Destructor */
SystemMonitors::~SystemMonitors()
{}

/** Copy assignment operator */
SystemMonitors& SystemMonitors::operator=(const SystemMonitors &other)
{
    if (this != &other)
    {
        montrs = other.montrs;
        deltas = other.deltas;
        nupdates = other.nupdates;
    }

    return *this;
}

/** Return the number of monitors in this set */
bool SystemMonitors::nMonitors() const
{
    return montrs.count();
}

/** Return the number of monitors in this set */
bool SystemMonitors::count() const
{
    return montrs.count();
}

/** Return whether or not this is empty (has no monitors) */
bool SystemMonitors::isEmpty() const
{
    return montrs.isEmpty();
}

/** Return whether or not this contains a monitor for the
    symbol 'symbol' */
bool SystemMonitors::contains(const Symbol &symbol) const
{
    return montrs.contains(symbol);
}

/** Assert that this set contains a monitor for the symbol 'symbol'

    \throw SireSystem::missing_monitor
*/
void SystemMonitors::assertContains(const Symbol &symbol) const
{
    if (not montrs.contains(symbol))
        throw SireSystem::missing_monitor( QObject::tr(
            "There is no monitor associated with the symbol \"%1\". "
            "Available monitors are %2.")
                .arg(symbol.toString(), Sire::toString(montrs.keys())), CODELOC );
}

/** Update all of the monitors with the current state
    of the system in 'system' */
void SystemMonitors::update(QuerySystem &system)
{
    //increment the number of updates...
    ++nupdates;

    //now see which monitors need updating...
    for (QHash< quint64, QSet<Symbol> >::const_iterator it = deltas.constBegin();
         it != deltas.constEnd();
         ++it)
    {
        if ( nupdates % it.key() == 0 )
        {
            //the monitors associated with this key need to be updated
            foreach (Symbol sym, *it)
            {
                BOOST_ASSERT(montrs.contains(sym));
                montrs.find(sym)->monitor(system);
            }
        }
    }
}

/** Synonym for 'update' */
void SystemMonitors::monitor(QuerySystem &system)
{
    this->update(system);
}

/** Force the update of all contained monitors, even if they aren't
    due to be updated! */
void SystemMonitors::forceUpdate(QuerySystem &system)
{
    ++nupdates;

    for (QHash<Symbol,SystemMonitor>::iterator it = montrs.begin();
         it != montrs.end();
         ++it)
    {
        it->monitor(system);
    }
}

/** Set the delta value of the monitor associated with the
    symbol 'symbol'.

    \throw SireSystem::missing_monitor
*/
void SystemMonitors::setDelta(const Symbol &symbol, quint64 delta)
{
    this->assertContains(symbol);

    if (deltas.value(delta).contains(symbol))
    {
        //nothing to change :-)
        return;
    }

    //remove any existing value
    for (QHash< quint64,QSet<Symbol> >::iterator it = deltas.begin();
         it != deltas.end();
         ++it)
    {
        it->remove(symbol);
    }

    //now add the new value
    if (delta != 0)
    {
        deltas[delta].insert(symbol);
    }
}

/** Set the monitor associated with the symbol 'symbol', and set
    it to be updated every 'delta' updates. This will replace
    any existing monitor that is associated with this symbol. */
void SystemMonitors::set(const Symbol &symbol, const SystemMonitor &monitor,
                         quint64 delta)
{
    montrs.insert(symbol, monitor);
    this->setDelta(symbol, delta);
}

/** Get the delta value for the monitor associated with the symbol 'symbol'

    \throw SireSystem::missing_monitor
*/
quint64 SystemMonitors::getDelta(const Symbol &symbol) const
{
    this->assertContains(symbol);

    for (QHash< quint64,QSet<Symbol> >::const_iterator it = deltas.begin();
         it != deltas.end();
         ++it)
    {
        if (it->contains(symbol))
            return it.key();
    }

    return 0;
}

/** Remove the monitor with symbol 'symbol' from this set of monitors,
    and return it.

    \throw SireSystem::missing_monitor
*/
SystemMonitor SystemMonitors::take(const Symbol &symbol)
{
    this->setDelta(symbol,0);
    return montrs.take(symbol);
}

/** Remove any monitor that is associated with the symbol 'symbol' */
void SystemMonitors::remove(const Symbol &symbol)
{
    if (this->contains(symbol))
    {
        this->setDelta(symbol,0);
        montrs.remove(symbol);
    }
}

/** Return the monitor associated with the symbol 'symbol'

    \throw SireStream::missing_monitor
*/
const SystemMonitor& SystemMonitors::monitor(const Symbol &symbol) const
{
    this->assertContains(symbol);
    return *(montrs.constFind(symbol));
}

/** Return all of the monitors, indexed by their symbol */
const QHash<Symbol,SystemMonitor>& SystemMonitors::monitors() const
{
    return montrs;
}

/** Return the current value of the monitor associated with the
    symbol 'symbol'

    \throw SireSystem::missing_monitor
*/
double SystemMonitors::value(const Symbol &symbol) const
{
    this->assertContains(symbol);
    return montrs.constFind(symbol)->value();
}

/** Return the number of updates that have been applied to this
    set of monitors */
quint64 SystemMonitors::nUpdates() const
{
    return nupdates;
}

/** Completely clear all of the statistics in all of the monitors */
void SystemMonitors::resetStatistics()
{
    if (nupdates != 0)
    {
        nupdates = 0;

        for (QHash<Symbol,SystemMonitor>::iterator it = montrs.begin();
             it != montrs.end();
             ++it)
        {
            it->clear();
        }
    }
}

/** Completely clear this set of monitors - this removes all
    of the monitors and all of their statistics */
void SystemMonitors::clear()
{
    montrs.clear();
    deltas.clear();
    nupdates = 0;
}

/** Assert that the monitors in this set are compatible with
    the passed system */
void SystemMonitors::assertCompatibleWith(const QuerySystem &system) const
{
    for (QHash<Symbol,SystemMonitor>::const_iterator it = montrs.constBegin();
         it != montrs.constEnd();
         ++it)
    {
        it->assertCompatibleWith(system);
    }
}
