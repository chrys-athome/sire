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
#include "Siren/obj.h"
#include "Siren/none.h"
#include "Siren/exceptions.h"
#include "Siren/siren.hpp"
#include "Siren/timer.h"
#include "Siren/testreport.h"

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
    a null pointer if a WorkSpace is not required for this WorkPacket */
WorkSpacePtr WorkPacket::createWorkspace() const
{
    return WorkSpacePtr();
}

/** Create the accompanying WorkSpace for this WorkPacket, passing the
    ID of the worker thread processing this WorkPacket. This allows a 
    group of worker threads to all process a WorkPacket together, with
    each one given its own ID, and with each one able to communicate
    with the others via the created WorkSpace. This returns a null pointer
    if a WorkSpace is not required for this WorkPacket */
WorkSpacePtr WorkPacket::createWorkspace(int) const
{
    return WorkSpacePtr();
}

/** Run a chunk of processing of this WorkPacket. This returns the processed
    WorkPacket, or a Siren::Exception if something went wrong, or a non-WorkPacket
    object that corresponds to the final result of the calculation */
Obj WorkPacket::run() const throw()
{
    try
    {
        return this->runChunk();
    }
    catch(const Siren::Exception &e)
    {
        return e;
    }
    catch(const std::exception &e)
    {
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
    result of the calculation */
Obj WorkPacket::run(WorkSpace &workspace) const throw()
{
    try
    {
        return this->runChunk(workspace);
    }
    catch(const Siren::Exception &e)
    {
        return e;
    }
    catch(const std::exception &e)
    {
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
    result of the calculation */
Obj WorkPacket::run(WorkSpace &workspace, int id) const throw()
{
    try
    {
        return this->runChunk(workspace, id);
    }
    catch(const Siren::Exception &e)
    {
        return e;
    }
    catch(const std::exception &e)
    {
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
Obj WorkPacket::runChunk(WorkSpace&) const
{
    return this->runChunk();
}

/** Over-ride this function to allow your WorkPacket to access and use
    a WorkSpace, and be processed by a team of work threads during
    WorkPacket processing */
Obj WorkPacket::runChunk(WorkSpace&, int id) const
{
    if (id == 0)
        return this->runChunk();
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
Obj TestPacket::runChunk() const
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

/** Return whether or not this WorkPacket is finished */
bool TestPacket::isFinished() const
{
    return current_val == target_val;
}

/** Return the progress towards a completed calculation. This is 
    the percentage of the calculation that has been completed,
    e.g. 0 means the calculation has not been started, 50 means
    it is half complete, and 100 means fully completed */
int TestPacket::progress() const
{
    return (100 * (target_val - current_val)) / target_val; 
}

/** Test this TestPacket */
void TestPacket::test(TestReportEditor &report) const
{
    TestPacket packet(2);
    packet.run();

    report.addPassed( String::tr("TestPacket passed as no exception was thrown.") );
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
