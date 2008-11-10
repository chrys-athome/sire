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

#ifndef SIREMPI_DETAIL_MPIDETAIL_H
#define SIREMPI_DETAIL_MPIDETAIL_H


namespace detail
{

/** Private class used by the MPINodes class */
class MPINodesData
{

friend class MPINodeData;   // so can see the MPI::Comm pointer

public:
    static boost::shared_ptr<MPINodesData> construct();
    
    ~MPINodesData();
    
    int count() const;
    
    int nFreeNodes() const;
    int nBusyNodes() const;
    
    void waitUntilAllFree();
    bool waitUntilAllFree(int timeout);
    
    MPINode getFreeNode();
    MPINode getFreeNode(int time);
    
    QList<MPINode> getNFreeNodes(int count);
    QList<MPINode> getNFreeNodes(int count, int time);

    void returnNode(const MPINode &node);

    #ifdef __SIRE_USE_MPI__
        MPI::Comm* mpiCommunicator();
        int mpiTag();
        int masterRank();
    #endif

    MPINodesData(int num_nodes);

    MPINode _pvt_getNode();

    /** Weak pointer to this object */
    boost::weak_ptr<MPINodesData> this_ptr;

    /** Mutex to protect access to the data of this class */
    QMutex data_mutex;
    
    /** Semaphore used to control access to the nodes */
    QSemaphore sem;

    /** All of the available MPI nodes in this communicator */
    QList<MPINode> free_nodes;
    
    /** All of the busy nodes in this communicator */
    QList< boost::weak_ptr<MPINodeData> > busy_nodes;

    #ifdef __SIRE_USE_MPI__
        /** The MPI communicator used to send message to 
            the nodes of this group */
        MPI::Intracomm send_mpicomm;

        /** The MPI communicator used to receive messages from
            the nodes of this group */
        MPI::Intracomm recv_mpicomm;

        /** The tag number that is used for all communications
            by nodes using this communicator */
        int mpitag;

        /** The total number of nodes in this communicator */
        int nnodes;
    #endif
};

}

}

SIRE_END_HEADER

#endif
