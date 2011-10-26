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
#include "Siren/siren.hpp"

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
    Siren::None if a WorkSpace is not required for this WorkPacket */
Obj WorkPacket::createWorkspace() const
{
    return None();
}

/** Create the accompanying WorkSpace for this WorkPacket, passing the
    ID of the worker thread processing this WorkPacket. This allows a 
    group of worker threads to all process a WorkPacket together, with
    each one given its own ID, and with each one able to communicate
    with the others via the created WorkSpace. This returns Siren::None
    if a WorkSpace is not required for this WorkPacket */
Obj WorkPacket::createWorkspace(int) const
{
    return None();
}

/** Run a chunk of processing of this WorkPacket, using the passed WorkSpace
    as an area for inter-process communication, or as a temporary work area.
    This returns the processed WorkPacket, or a Siren::Exception if something
    went wrong, or a non-WorkPacket object that corresponds to the final 
    result of the calculation */
Obj WorkPacket::runChunk(WorkSpace&) const
{
    return this->runChunk();
}

/** Run a chunk of processing of this WorkPacket, using the passed WorkSpace
    as an area for inter-process communication, or as a temporary work area.
    This function is used to allow a group of worker threads to process 
    a WorkPacket together, with each thread given a unique worker_id
    (from 0 to n_workers-1).
    
    This returns the processed WorkPacket, or a Siren::Exception if something
    went wrong, or a non-WorkPacket object that corresponds to the final 
    result of the calculation */
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
