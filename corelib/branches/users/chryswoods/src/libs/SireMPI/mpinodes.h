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

#ifndef SIREMPI_MPINODES_H
#define SIREMPI_MPINODES_H

#include "sireglobal.h"

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

SIRE_BEGIN_HEADER

namespace SireMPI
{

namespace detail
{
class MPINodesPvt;
}

class MPINode;
class MPINodesPtr;

/** This class represents the groups of MPI nodes that are available
    within a single communicator
    
    @author Christopher Woods
*/
class SIREMPI_EXPORT MPINodes
{

friend class MPINodesPtr;

public:
    MPINodes();
    
    MPINodes(const MPINodes &other);
    
    ~MPINodes();

    MPINodes& operator=(const MPINodes &other);
    
    bool operator==(const MPINodes &other) const;
    bool operator!=(const MPINodes &other) const;
    
    MPINode getFreeNode();
    QList<MPINode> getNFreeNodes(int count);
    
    MPINode getFreeNode(int time);
    QList<MPINode> getNFreeNodes(int count, int time);
    
    int nFreeNodes() const;
    int nBusyNodes() const;
    int nNodes() const;
    int count() const;
    
    bool isMaster() const;
    
    static MPINodes COMM_WORLD();

    const void* communicator() const;

private:
    MPINodes(const boost::shared_ptr<detail::MPINodesPvt> &ptr);

    /** PIMPL pointer */
    boost::shared_ptr<detail::MPINodesPvt> d;
};

/** This is a weak pointer to an MPINodes object - this is automatically
    set to null when the MPINodes object is destroyed
    
    @author Christopher Woods
*/
class SIREMPI_EXPORT MPINodesPtr
{
public:
    MPINodesPtr();
    MPINodesPtr(const MPINodes &node);
    MPINodesPtr(const MPINodesPtr &other);
    
    ~MPINodesPtr();
    
    MPINodesPtr& operator=(const MPINodesPtr &other);
    
    bool operator==(const MPINodesPtr &other) const;
    bool operator!=(const MPINodesPtr &other) const;
    
    MPINodes operator*() const;
    
    bool isNull() const;

private:
    /** Weak pointer to the PIMPL data of MPINode */
    boost::weak_ptr<detail::MPINodesPvt> d;
};

}

SIRE_EXPOSE_CLASS( SireMPI::MPINodes )

SIRE_END_HEADER

#endif
