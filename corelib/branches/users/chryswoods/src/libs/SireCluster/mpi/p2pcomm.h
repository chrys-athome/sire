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

#ifndef SIRECLUSTER_MPI_P2PCOMM_H
#define SIRECLUSTER_MPI_P2PCOMM_H

#ifdef __SIRE_USE_MPI__

#include <mpi.h> // must be first to satisfy mpich

#include <QUuid>

#include "sireglobal.h"

#include <boost/shared_ptr.hpp>

SIRE_BEGIN_HEADER

namespace SireCluster
{

class Frontend;

namespace MPI
{

class MPICluster;

namespace detail
{
class P2PCommPvt;
}

/** This class holds a point-to-point communicator, which is
    used to allow direct, private communication between two
    processes in the MPI cluster
    
    @author Christopher Woods
*/
class P2PComm
{

friend class MPICluster;

public:
    P2PComm();
    P2PComm(int master_rank, int slave_rank);
    
    P2PComm(const P2PComm &other);
    
    ~P2PComm();
    
    P2PComm& operator=(const P2PComm &other);
    
    bool involves(int rank);
    
    bool isMaster();
    bool isSlave();
    
    bool isLocal();
    
    bool isNull() const;
    
    void setBackend(const Frontend &backend);
    
protected:
    static P2PComm createLocal();
    static P2PComm create(::MPI::Intracomm private_comm, 
                          int master_rank, int slave_rank);
    
private:
    /** Shared pointer to the implementation */
    boost::shared_ptr<detail::P2PCommPvt> d;
};

} // end of namespace MPI

} // end of namespace SireCluster

SIRE_END_HEADER

#endif // __SIRE_USE_MPI__
#endif
