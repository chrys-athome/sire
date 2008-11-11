/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2008  Christopher Woods
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

#ifndef SIREMPI_MPINODE_H
#define SIREMPI_MPINODE_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

class QUuid;

namespace SireMPI
{

class MPINodes;
class MPIWorker;
class MPIPromise;

class MPINodePtr;
class MPINodesPtr;

namespace detail
{
class MPINodePvt;
}

/** This class provides a handle to represent a node that
    is made available via MPI. It provides information about
    the node (number of available processors and memory etc.)
    and provides the ID necessary to allow jobs to be assigned
    to it.
    
    @author Christopher Woods
*/
class SIREMPI_EXPORT MPINode
{

friend class MPINodes;
friend class MPINodePtr;

public:
    MPINode();
    
    MPINode(const MPINode &other);

    ~MPINode();
    
    MPINode& operator=(const MPINode &other);
    
    bool operator==(const MPINode &other) const;
    bool operator!=(const MPINode &other) const;
    
    const QUuid& UID() const;

    bool isMaster() const;

    bool isNull() const;

    bool isBusy() const;
        
    bool wasAborted() const;
        
    void wait();    
    bool wait(int time);

    void returnNode();

    MPINodes communicator() const;
    int rank() const;

    MPIPromise start(const MPIWorker &worker);
    void stop();
    void abort();

protected:
    MPINode(const MPINodes &communicator, int rank, bool is_master);

private:
    /** PIMPL pointer */
    boost::shared_ptr<detail::MPINodePvt> d;
};

/** This is a weak pointer to the node - this is automatically 
    set to null if the node is destroyed */
class SIREMPI_EXPORT MPINodePtr
{
public:
    MPINodePtr();
    MPINodePtr(const MPINode &node);
    MPINodePtr(const MPINodePtr &other);
    
    ~MPINodePtr();
    
    MPINodePtr& operator=(const MPINodePtr &other);
    
    bool operator==(const MPINodePtr &other) const;
    bool operator!=(const MPINodePtr &other) const;
    
    MPINode operator*() const;
    
    bool isNull() const;

private:
    /** Weak pointer to the PIMPL data of MPINode */
    boost::weak_ptr<detail::MPINodePvt> d;
};

}

SIRE_EXPOSE_CLASS( SireMPI::MPINode )

SIRE_END_HEADER

#endif
