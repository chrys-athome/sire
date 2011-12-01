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

#include "Siren/workpacket.h"
#include "Siren/workspace.h"
#include "Siren/workmonitor.h"
#include "Siren/percentage.h"
#include "Siren/obj.h"
#include "Siren/none.h"
#include "Siren/exceptions.h"
#include "Siren/siren.hpp"
#include "Siren/timer.h"
#include "Siren/testreport.h"
#include "Siren/thread.h"
#include "Siren/forages.h"
#include "Siren/workqueue.h"
#include "Siren/promise.h"

#include <cmath>

using namespace Siren;

//////////////
////////////// Implementation of WorkPacket
//////////////

REGISTER_SIREN_VIRTUAL_CLASS( Siren::WorkPacket )

/** Constructor */
WorkPacket::WorkPacket() : Object()
{}

/** Copy constructor */
WorkPacket::WorkPacket(const WorkPacket &other) : Object(other)
{}

/** Destructor */
WorkPacket::~WorkPacket()
{}

/** Return whether processing of this WorkPacket requires the creation
    and supply of an accompanying WorkSpace. If the WorkPacket requires 
    a WorkSpace, then you must use "WorkPacket::createWorkspace()" to 
    create that WorkSpace, and then supply that WorkSpace to the WorkPacket
    via the "WorkPacket::runChunk(WorkSpace &workspace...) functions */
bool WorkPacket::needsWorkspace() const
{
    return false;
}

/** Create the accompanying WorkSpace for this WorkPacket. This returns
    a null WorkSpace if one is not required for this WorkPacket */
WorkSpace WorkPacket::createWorkspace() const
{
    return WorkSpace();
}

/** Run a chunk of processing of this WorkPacket. This returns the processed
    WorkPacket, or a Siren::Exception if something went wrong, or a non-WorkPacket
    object that corresponds to the final result of the calculation 
    
    The (optionally supplied) WorkMonitor allows the in-progress calculation
    to be monitored, with near-real-time recording of the progress, energy
    consumption, speed and health of the job
*/
Obj WorkPacket::run(WorkMonitor monitor) const throw()
{
    try
    {
        return this->runChunk(monitor);
    }
    catch(const Siren::Exception &e)
    {
        //catastrophic failure!
        monitor.setHealth(0);

        return e;
    }
    catch(const std::exception &e)
    {
        //catastrophic failure!
        monitor.setHealth(0);
    
        try
        {
            return standard_exception(String::tr(
                "A C++ standard exception was thrown during processing "
                "of the WorkPacket %1.").arg(this->toString()), e, CODELOC);
        }
        catch(...)
        {
            return standard_exception(e,CODELOC);
        }
    }
    catch(...)
    {
        //catastrophic failure!
        monitor.setHealth(0);

        try
        {
            return unknown_exception(String::tr(
                "An unknown exception was thrown during processing "
                "of the WorkPacket %1.").arg(this->toString()), CODELOC);
        }
        catch(...)
        {
            return unknown_exception(CODELOC);
        }
    }
}

/** Run a chunk of processing of this WorkPacket, using the passed WorkSpace
    as an area for inter-process communication, or as a temporary work area.
    This returns the processed WorkPacket, or a Siren::Exception if something
    went wrong, or a non-WorkPacket object that corresponds to the final 
    result of the calculation 
    
    The (optionally supplied) WorkMonitor allows the in-progress calculation
    to be monitored, with near-real-time recording of the progress, energy
    consumption, speed and health of the job

*/
Obj WorkPacket::run(WorkSpace workspace, WorkMonitor monitor) const throw()
{
    try
    {
        return this->runChunk(workspace, monitor);
    }
    catch(const Siren::Exception &e)
    {
        //catastrophic failure!
        monitor.setHealth(0);

        return e;
    }
    catch(const std::exception &e)
    {
        //catastrophic failure!
        monitor.setHealth(0);

        try
        {
            return standard_exception(String::tr(
                "A C++ standard exception was thrown during processing "
                "of the WorkPacket %1.").arg(this->toString()), e, CODELOC);
        }
        catch(...)
        {
            return standard_exception(e,CODELOC);
        }
    }
    catch(...)
    {
        //catastrophic failure!
        monitor.setHealth(0);

        try
        {
            return unknown_exception(String::tr(
                "An unknown exception was thrown during processing "
                "of the WorkPacket %1.").arg(this->toString()), CODELOC);
        }
        catch(...)
        {
            return unknown_exception(CODELOC);
        }
    }
}

/** Run a chunk of processing of this WorkPacket, using a group of worker threads 
    to process a WorkPacket together, with each thread given a unique worker_id
    (from 0 to n_workers-1).
    
    This returns the processed WorkPacket, or a Siren::Exception if something
    went wrong, or a non-WorkPacket object that corresponds to the final 
    result of the calculation.
    
    The (optionally supplied) WorkMonitor allows the in-progress calculation
    to be monitored, with near-real-time recording of the progress, energy
    consumption, speed and health of the job
*/
Obj WorkPacket::run(int id, int nworkers, WorkMonitor monitor) const throw()
{
    if (nworkers <= 0)
        nworkers = 1;
        
    if (id < 0)
        id = 0;
        
    if (id >= nworkers)
    {
        //catastrophic failure!
        monitor.setHealth(0);

        return Siren::program_bug( String::tr(
                "How can ID (%1) be greater than or equal to NWORKERS (%2)???")
                    .arg(id).arg(nworkers), CODELOC );
    }

    try
    {
        return this->runChunk(id, nworkers, monitor);
    }
    catch(const Siren::Exception &e)
    {
        //catastrophic failure!
        monitor.setHealth(0);

        return e;
    }
    catch(const std::exception &e)
    {
        //catastrophic failure!
        monitor.setHealth(0);

        try
        {
            return standard_exception(String::tr(
                "A C++ standard exception was thrown during processing "
                "of the WorkPacket %1.").arg(this->toString()), e, CODELOC);
        }
        catch(...)
        {
            return standard_exception(e,CODELOC);
        }
    }
    catch(...)
    {
        //catastrophic failure!
        monitor.setHealth(0);

        try
        {
            return unknown_exception(String::tr(
                "An unknown exception was thrown during processing "
                "of the WorkPacket %1.").arg(this->toString()), CODELOC);
        }
        catch(...)
        {
            return unknown_exception(CODELOC);
        }
    }
}

/** Run a chunk of processing of this WorkPacket, using the passed WorkSpace
    as an area for inter-process communication, or as a temporary work area.
    This function is used to allow a group of worker threads to process 
    a WorkPacket together, with each thread given a unique worker_id
    (from 0 to n_workers-1).
    
    This returns the processed WorkPacket, or a Siren::Exception if something
    went wrong, or a non-WorkPacket object that corresponds to the final 
    result of the calculation 
    
    The (optionally supplied) WorkMonitor allows the in-progress calculation
    to be monitored, with near-real-time recording of the progress, energy
    consumption, speed and health of the job
*/
Obj WorkPacket::run(WorkSpace workspace, int id, int nworkers,
                    WorkMonitor monitor) const throw()
{
    if (nworkers <= 0)
        nworkers = 1;
        
    if (id < 0)
        id = 0;
        
    if (id >= nworkers)
    {
        //catastrophic failure!
        monitor.setHealth(0);

        return Siren::program_bug( String::tr(
                "How can ID (%1) be greater than or equal to NWORKERS (%2)???")
                    .arg(id).arg(nworkers), CODELOC );
    }

    try
    {
        return this->runChunk(workspace, id, nworkers, monitor);
    }
    catch(const Siren::Exception &e)
    {
        //catastrophic failure!
        monitor.setHealth(0);

        return e;
    }
    catch(const std::exception &e)
    {
        //catastrophic failure!
        monitor.setHealth(0);

        try
        {
            return standard_exception(String::tr(
                "A C++ standard exception was thrown during processing "
                "of the WorkPacket %1.").arg(this->toString()), e, CODELOC);
        }
        catch(...)
        {
            return standard_exception(e,CODELOC);
        }
    }
    catch(...)
    {
        //catastrophic failure!
        monitor.setHealth(0);

        try
        {
            return unknown_exception(String::tr(
                "An unknown exception was thrown during processing "
                "of the WorkPacket %1.").arg(this->toString()), CODELOC);
        }
        catch(...)
        {
            return unknown_exception(CODELOC);
        }
    }
}

/** Over-ride this function to allow your WorkPacket to access and use
    a WorkSpace during WorkPacket processing */
Obj WorkPacket::runChunk(WorkSpace&, WorkMonitor monitor) const
{
    return this->runChunk(monitor);
}

/** Over-ride this function to allow your WorkPacket to access and use
    a WorkSpace, and be processed by a team of work threads during
    WorkPacket processing */
Obj WorkPacket::runChunk(WorkSpace&, int id, int, WorkMonitor monitor) const
{
    if (id == 0)
        return this->runChunk(monitor);
    else
        return None();
}

/** Over-ride this function to allow your WorkPacket to be 
    processed by a team of work threads during WorkPacket processing */
Obj WorkPacket::runChunk(int id, int, WorkMonitor monitor) const
{
    if (id == 0)
        return this->runChunk(monitor);
    else
        return None();
}

/** Reduce the set of results returned by each of the workers into the
    single result of the calculation */
Obj WorkPacket::reduce(const Vector<Obj>::Type &results,
                       WorkMonitor monitor) const throw()
{
    //if the workpacket does not know how to reduce the results, then
    //we are only interested in the first result
    if (results.count() >= 1)
        return results.at(0);
    else
        return None();
}

/** Copy assignment operator */
void WorkPacket::copy_object(const WorkPacket &other)
{
    super::copy_object(other);
}

/** Comparison operator */
bool WorkPacket::compare_object(const WorkPacket &other) const
{
    return super::compare_object(other);
}

//////////////
////////////// Implementation of TestPacket
//////////////

REGISTER_SIREN_CLASS( Siren::TestPacket )

/** Construct a default TestPacket - this counts to 10 */
TestPacket::TestPacket() : WorkPacket(), target_val(10), current_val(0)
{}

/** Construct a TestPacket that will count up to 'n' */
TestPacket::TestPacket(int n) : WorkPacket(), target_val(n), current_val(0)
{
    if (n < 0)
        throw Siren::invalid_arg( String::tr(
                "You cannot create a TestPacket which asks to count up to "
                "a negative number (%1).")
                    .arg(n), CODELOC );
}

/** Copy constructor */
TestPacket::TestPacket(const TestPacket &other)
           : WorkPacket(other), 
             target_val(other.target_val),
             current_val(other.current_val)
{}

/** Destructor */
TestPacket::~TestPacket()
{}

/** Run a chunk of calculation. This will count from current_val to 
    target_val in blocks of 10, sleeping for one second between each
    value */
Obj TestPacket::runChunk(WorkMonitor monitor) const
{
    int n_remaining = target_val - current_val;
    
    if (n_remaining <= 0)
        return *this;
    
    if (n_remaining > 10)
        n_remaining = 10;
        
    for (int i=1; i<=n_remaining; ++i)
    {
        sirenDebug() << "TestPacket counting:" << (current_val+i)
                     << "of" << target_val << "...";
                     
        //busy sleep, so that we burn some CPU
        Timer t = Timer::start();
        
        double j = 1;
        int k = 0;
        
        while (t.elapsed() < 1000)
        {
            j += std::sqrt(j);
            k += 1;
        }
        
        sirenDebug() << "(performed" << k << "square roots per second!)";
    }
    
    TestPacket next_chunk(*this);
    next_chunk.current_val += n_remaining;
    
    return next_chunk;
}

/** Run a chunk of the calculation in parallel. This will count from current_val 
    to target_val in blocks of 10, divided between the various threads, sleeping
    for one second between blocks */
Obj TestPacket::runChunk(int worker_id, int nworkers, WorkMonitor monitor) const
{
    int n_remaining = target_val - current_val;
    
    if (n_remaining <= 0)
        return *this;

    const int n_per_thread = 10;
    const int max_n_to_count = n_per_thread * nworkers;
    
    if (n_remaining > max_n_to_count)
        n_remaining = max_n_to_count;

    for (int i=worker_id+1; i<=n_remaining; i += nworkers)
    {
        sirenDebug() << "TestPacket(" << worker_id << ") counting:" << (current_val+i)
                     << "of" << target_val << "...";
                     
        //busy sleep, so that we burn some CPU
        Timer t = Timer::start();
        
        double j = 1;
        int k = 0;
        
        while (t.elapsed() < 1000)
        {
            for (int l=0; l<1000; ++l)
            {
                j += std::sqrt(j);
                k += 1;
            }
        }
        
        sirenDebug() << "(" << worker_id << "performed" 
                     << k << "square roots per second!)";

        monitor.addFLOPs(k);
    }

    monitor.setHealth(100);

    if (current_val + n_remaining >= target_val)
    {
        //we have finished :-)
        monitor.setProgress(100);
        return None();
    }
    else
    {
        TestPacket next_chunk(*this);
        next_chunk.current_val += n_remaining;
        
        monitor.setProgress( 100.0 * (next_chunk.current_val / target_val) );
    
        return next_chunk;
    }
}

void run_function()
{
    for_ages::setThisThreadName("run_function");
    sirenDebug() << "HELLO...";
    for_ages::sleep(2);
    sirenDebug() << "...WORLD!";
}

/** Test this TestPacket */
void TestPacket::test(TestReportEditor &report) const
{
    TestPacket packet(5);

    WorkMonitor monitor;
    
    Obj result;
    
    while (for_ages::loop())
    {
        result = packet.run(monitor);
        
        if (not result.isA<TestPacket>())
            break;
            
        packet = result.asA<TestPacket>();
    }

    if (not result.isNone())
        report.addFailed( String::tr("Processing a TestPacket resulted in a non-None "
                                     "result! Result = %1").arg(result.toString()) );
    else
        report.addPassed( String::tr("TestPacket processing was successful.") );

    Thread t = Thread::run( &run_function );
    

    t = Thread::run( &run_function );
    

    Thread t2 = Thread::run( &run_function );
    
    for (int i=0; i<10; ++i)
    {
        Thread::run( &run_function );
    }
    
    WorkQueue queue(5);
    
    Promise promise = queue.submitBG( TestPacket(50), 5 );
    
    sirenDebug() << queue.toString();
    
    //queue = WorkQueue();
    
    sirenDebug() << promise.toString();
    
    promise.wait();
    
    sirenDebug() << promise.result().toString();
}

/** Copy assignment operator */
void TestPacket::copy_object(const TestPacket &other)
{
    target_val = other.target_val;
    current_val = other.current_val;
    super::copy_object(other);
}

/** Comparison operator */
bool TestPacket::compare_object(const TestPacket &other) const
{
    return target_val == other.target_val and
           current_val == other.current_val and
           super::compare_object(other);
}
