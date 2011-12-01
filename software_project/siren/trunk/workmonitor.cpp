/********************************************\
  *
  *  Siren - C++ metaobject library
  *
  *  Copyright (C) 2011  Christopher Woods
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

#include "Siren/workmonitor.h"
#include "Siren/datetime.h"
#include "Siren/mutex.h"
#include "Siren/percentage.h"

using namespace Siren;
using namespace Siren::detail;

namespace Siren
{
    namespace detail
    {
        class WorkMonitorData : public noncopyable
        {
        public:
            WorkMonitorData() : flops(0), energy(0), health(100), progress(0)
            {}
            
            ~WorkMonitorData()
            {}
            
            /** Mutex to protect access to the data */
            Mutex m;
            
            /** The total number of flops recorded */
            int64 flops;
            
            /** The total amount of energy consumed, in joules */
            int64 energy;
            
            /** The current health of the calculation */
            Percentage health;
            
            /** The current progress of the calculation */
            Percentage progress;
            
            /** Record of when flops were reported */
            Vector< std::pair<DateTime,int> >::Type flops_record;
            
            /** Record of when energy was consumed */
            Vector< std::pair<DateTime,int> >::Type energy_record;
            
            /** The record of the health of the calculation */
            Vector< std::pair<DateTime,Percentage> >::Type health_record;
            
            /** The record of the progress of the calculation */
            Vector< std::pair<DateTime,Percentage> >::Type progress_record;
            
        }; // end of class WorkMonitorData
    
    } // end of namespace detail

} // end of namespace Siren

///////////
/////////// Implementation of WorkMonitor
///////////

/** Constructor */
WorkMonitor::WorkMonitor() : d( new WorkMonitorData() )
{}

/** Copy constructor */
WorkMonitor::WorkMonitor(const WorkMonitor &other) : d(other.d)
{}

/** Destructor */
WorkMonitor::~WorkMonitor()
{}

/** Copy assignment operator */
WorkMonitor& WorkMonitor::operator=(const WorkMonitor &other)
{
    d = other.d;
    return *this;
}

/** Comparison operator */
bool WorkMonitor::operator==(const WorkMonitor &other) const
{
    return d.get() == other.d.get();
}

/** Comparison operator */
bool WorkMonitor::operator!=(const WorkMonitor &other) const
{
    return not operator==(other);
}

/** Record that an additional 'nflops' floating point operations have been computed */
void WorkMonitor::addFLOPs(int32 nflops)
{
    if (d)
    {
        MutexLocker lkr( &(d->m) );
        
        d->flops += nflops;
        d->flops_record.append( std::pair<DateTime,int32>( 
                                        DateTime::current(), nflops ) );
    }
}

/** Record that an addition 'joules' of energy have been consumed */
void WorkMonitor::addEnergy(int32 joules)
{
    if (d)
    {
        MutexLocker lkr( &(d->m) );
        
        d->energy += joules;
        d->energy_record.append( std::pair<DateTime,int32>(
                                        DateTime::current(), joules ) );
    }
}

/** Record that the current progress of the calculation is 'progress'.
    Note that it is possible that the progress may decrease, e.g. if
    an error was detected */
void WorkMonitor::setProgress(Percentage progress)
{
    if (d)
    {
        MutexLocker lkr( &(d->m) );
        
        d->progress = progress;
        d->progress_record.append( std::pair<DateTime,Percentage>(
                                        DateTime::current(), progress ) );
    }
}

/** Record that the current health of the calculation is 'health'. Note that
    the health of the calculation may go up and down, as errors are detected
    and fixed */
void WorkMonitor::setHealth(Percentage health)
{
    if (d)
    {
        MutexLocker lkr( &(d->m) );
        
        d->health = health;
        d->health_record.append( std::pair<DateTime,Percentage>(
                                        DateTime::current(), health ) );
    }
}
