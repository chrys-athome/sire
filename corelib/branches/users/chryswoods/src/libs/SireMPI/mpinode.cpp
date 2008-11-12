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

#include <boost/tuple/tuple.hpp>

#include <QTextStream>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QUuid>

#include "mpinode.h"
#include "mpinodes.h"
#include "mpipromise.h"
#include "mpiworker.h"
#include "mpifrontends.h"
#include "mpibackends.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include "SireError/errors.h"

#include "tostring.h"

#include <QDebug>

using namespace SireMPI;
using namespace SireStream;

using boost::shared_ptr;
using boost::weak_ptr;
using boost::tuple;

namespace SireMPI 
{
namespace detail
{

/** Private implementation of MPINode */
class MPINodePvt : public QThread
{
public:
    MPINodePvt();
    ~MPINodePvt();
    
    /** Mutex to protect access to the data of this node */
    QMutex datamutex;

    /** Mutex and wait condition used to start this node */
    QMutex start_mutex;

    /** Mutex and wait condition used to start this node */
    QWaitCondition start_waiter;

    /** Weak pointer to the parent MPINodes */
    MPINodesPtr parent_ptr;
    
    /** Unique ID of this node */
    QUuid uid;
    
    /** The current calculation, or the results of the last calculation */
    MPIPromise mpipromise;
    
    /** The rank of this node in the MPINodes object */
    int rank;
    
    /** Was the last job aborted? */
    bool is_aborted;
    
    /** Is this the master node of the communicator? */
    bool is_master;

protected:
    void run();
};

} // end of namespace detail
} // end of namespace SireMPI

using namespace SireMPI::detail;

//////////
////////// Implementation of MPINodePvt
//////////

/** Null constructor */
MPINodePvt::MPINodePvt() 
           : QThread(), datamutex(QMutex::Recursive),
             rank(0), is_aborted(false), is_master(false)
{}

/** Destructor - this blocks until the node is not busy, and then
    it either completely deletes the node, or posts a new copy
    back to the list of free nodes in the communicator */
MPINodePvt::~MPINodePvt()
{
    //wait until this node is no longer busy...
    qDebug() << MPINode::globalRank() << CODELOC;
    this->wait();

    qDebug() << MPINode::globalRank() << CODELOC;

    if (not parent_ptr.isNull())
    {
        qDebug() << MPINode::globalRank() << CODELOC;
        (*parent_ptr).returnedNode();
        qDebug() << MPINode::globalRank() << CODELOC;
    }
}

/** Small private class used to clean up MPINode objects that have finished
    their jobs. This is used so that the destructors are called in a dedicated
    thread (thus preventing the possibility that the destructor for a node
    will be called in its own thread - thereby leading to deadlock) */
class NodeDeleter : private QThread
{
public:
    NodeDeleter() : QThread()
    {
        keep_running = true;
        this->start();
    }
    
    ~NodeDeleter()
    {
        keep_running = false;
        this->wait();
    }
    
    static void scheduleForDeletion(const MPINode &node);
     
protected:
    void writeErrorString(const QString &location,
                          const SireError::exception &e) const
    {
        QTextStream ts(stdout);
    
        ts << QObject::tr(" ** FATAL ERROR ON NODE %1 in NodeDeleter **\n"
                          " ** LOCATION %2 **\n%3\n")
                    .arg(MPINode::globalRank())
                    .arg(location, e.toString());
    }

    void run()
    {
        try
        {
            while (keep_running)
            {
                QMutexLocker lkr(&datamutex);
                
                //delete all of the nodes - this calls all 
                //of their destructors
                delete_nodes.clear();

                lkr.unlock();
                
                //only do this once every 5 seconds
                QThread::sleep(5);
            }
            
            //ok, we are exiting - clear any final nodes
            QMutexLocker lkr(&datamutex);
            delete_nodes.clear();
        }
        catch(const SireError::exception &e)
        {
            writeErrorString(CODELOC, e);
        }
        catch(const std::exception &e)
        {
            writeErrorString(CODELOC, SireError::std_exception(e));
        }
        catch(...)
        {
            writeErrorString(CODELOC, 
                    SireError::unknown_exception("An unknown error occured!", CODELOC));
        }
    }
    
private:
    QMutex datamutex;
    QList<MPINode> delete_nodes;
    bool keep_running;
};

Q_GLOBAL_STATIC( NodeDeleter, globalNodeDeleter );

/** Schedule the passed node to be deleted */
void NodeDeleter::scheduleForDeletion(const MPINode &node)
{
    NodeDeleter *global_deleter = globalNodeDeleter();
        
    if (global_deleter == 0)
        //we are being destroyed!
        return;
   
    QMutexLocker lkr( &(global_deleter->datamutex) );
    global_deleter->delete_nodes.append(node);
}

/** This is what is run in the background to send a job to the backend 
    node and to wait for a response */
void MPINodePvt::run()
{
    //tell the parent thread that we are now definitely running
    start_mutex.lock();
    start_waiter.wakeAll();
    start_mutex.unlock();
    
    //copy the promise from the parent thread into a thread-local copy
    MPIPromise local_promise;
    
    {
        QMutexLocker lkr(&datamutex);
        is_aborted = false;
        
        if (mpipromise.isNull())
            return;
            
        local_promise = mpipromise;
        
        if (is_aborted)
        {
            //we've been aborted before even starting!
            local_promise.setAborted();
        }
    }

    //the job is running the right node, isn't it...?
    BOOST_ASSERT( local_promise.node().UID() == this->uid );

    //take a copy of this node to ensure that it is not deleted during
    //the job
    MPINode this_node = local_promise.node();
    
    //get a front end for this node
    MPIFrontend frontend = getFrontend(this_node);

    //tell the frontend to perform the work on the backend
    frontend.performWork(local_promise);

    {
        QMutexLocker lkr(&datamutex);
        is_aborted = local_promise.isAborted();
    }

    //we've now finished - we currently hold a pointer to ourselves,
    //so this will need to be scheduled to be freed (otherwise
    //we will deadlock as the destructor is called, waiting for the
    //node to delete itself!)
    NodeDeleter::scheduleForDeletion(this_node);
}

//////////
////////// Implementation of MPINode
//////////

/** Null constructor */
MPINode::MPINode() : d( new MPINodePvt() )
{}

/** Construct from the passed pointer */
MPINode::MPINode(const shared_ptr<MPINodePvt> &ptr) : d(ptr)
{}

/** Construct an MPINode which is part of the passed communicator,
    with the specified rank, and saying whether or not this is 
    the master node of this communicator */
MPINode::MPINode(const MPINodes &communicator, int rank, bool is_master)
        : d( new MPINodePvt() )
{
    d->parent_ptr = communicator;
    d->uid = communicator.getUID(rank);
    d->rank = rank;
}

/** Copy constructor */
MPINode::MPINode(const MPINode &other)
        : d(other.d)
{}

/** Destructor */
MPINode::~MPINode()
{}

/** Copy assignment operator */
MPINode& MPINode::operator=(const MPINode &other)
{
    d = other.d;
    return *this;
}

/** Comparison operator */
bool MPINode::operator==(const MPINode &other) const
{
    return d.get() == other.d.get();
}

/** Comparison operator */
bool MPINode::operator!=(const MPINode &other) const
{
    return d.get() != other.d.get();
}

/** Return the unique ID for this MPI node */
const QUuid& MPINode::UID() const
{
    return d->uid;
}

/** Return whether or not this node is null */
bool MPINode::isNull() const
{
    return d->parent_ptr.isNull() or d->uid.isNull();
}

/** Return this node to the pool of unused threads - this returns
    immediately, but the node will only be returned once it is no 
    longer busy, and no other copies of this MPINode exist */
void MPINode::returnNode()
{
    this->operator=( MPINode() );
}

/** Return the communicator in which this node belongs 

    \throw SireError::unavailable_resource
*/
MPINodes MPINode::communicator() const
{
    if (d->parent_ptr.isNull())
        throw SireError::unavailable_resource( QObject::tr(
            "A null MPINode doesn't have a communicator!"), CODELOC );
            
    return *(d->parent_ptr);
}

/** Return the rank of this node in its communicator */
int MPINode::rank() const
{
    return d->rank;
}

/** Return the global rank of this node in MPI::COMM_WORLD */
int MPINode::globalRank()
{
    return MPIBackends::globalRank();
}

/** Return whether this node is the master node for the communicator */
bool MPINode::isMaster() const
{
    return d->is_master;
}

/** Return whether or not this node is busy doing some work */
bool MPINode::isBusy() const
{
    return d->isRunning();
}
    
/** Wait until this node is no longer busy doing work */
void MPINode::wait()
{
    d->wait();
}

/** Wait until this node is no longer busy doing work, or until
    'time' milliseconds have passed. This returns whether or not
    the nodes is not busy */
bool MPINode::wait(int time)
{
    return d->wait(time);
}

/** Start and set the node working in the job contained in 'worker'. 
    This blocks until any other jobs running on this node have
    finished. This returns an MPIPromise that can be used to follow
    the progress of the work and get the result */

/** Start and run the job that is contained in the passed worker */
MPIPromise MPINode::start(const MPIWorker &worker)
{
    if (d->parent_ptr.isNull() or d->uid.isNull())
        throw SireError::invalid_state( QObject::tr(
            "You cannot run any work on the null MPI node."), CODELOC );

    QMutexLocker lkr( &(d->datamutex) );

    //wait until the node is no longer busy
    while (d->isRunning())
    {
        lkr.unlock();
        d->wait();
        lkr.relock();
    }

    //save the worker to be processed
    d->mpipromise = MPIPromise(worker, *this);

    //reset the aborted status
    d->is_aborted = false;

    //now start the thread that is used to start the job and wait
    //for responses from the backend node
    d->start_mutex.lock();
    d->start();
    d->start_waiter.wait( &(d->start_mutex) );
    d->start_mutex.unlock();
    
    return d->mpipromise;
}

/** This tells the node to stop performing any work that it may 
    be running. This stops the work at the next convienient moment
    and returns to the promise the result at the point of stopping.
    This does not block */
void MPINode::stop()
{
    QMutexLocker lkr( &(d->datamutex) );

    if (not d->isRunning())
    {
        //nothing to stop
        return;
    }

    //tell the node to stop
    MPIFrontend frontend = getFrontend(*this);
    frontend.stopWork();

    //wait for the node to finish
    lkr.unlock();
    d->wait();
}

/** This tells the node to abort any work it may be running. This
    stops the work at the next convienient moment and returns to
    the promise the work from before it started. This does not block. */
void MPINode::abort()
{
    QMutexLocker lkr( &(d->datamutex) );

    if (not d->isRunning())
    {
        //nothing to stop
        return;
    }

    //tell the node to stop
    MPIFrontend frontend = getFrontend(*this);
    frontend.abortWork();

    //wait for the node to finish
    lkr.unlock();
    d->wait();
}

/** Return whether the last (or current) job has been aborted */
bool MPINode::wasAborted() const
{
    QMutexLocker lkr( &(d->datamutex) );
    return d->is_aborted;
}

//////////
////////// Implementation of MPINodePtr
//////////

/** Null constructor */
MPINodePtr::MPINodePtr()
{}

/** Construct a pointer to the nodes 'nodes' */
MPINodePtr::MPINodePtr(const MPINode &nodes) : d(nodes.d)
{}

/** Copy constructor */
MPINodePtr::MPINodePtr(const MPINodePtr &other) : d(other.d)
{}

/** Destructor */
MPINodePtr::~MPINodePtr()
{}

/** Copy assignment operator */
MPINodePtr& MPINodePtr::operator=(const MPINodePtr &other)
{
    d = other.d;
    return *this;
}

/** Comparison operator */
bool MPINodePtr::operator==(const MPINodePtr &other) const
{
    return d.lock().get() == other.d.lock().get();
}

/** Comparison operator */
bool MPINodePtr::operator!=(const MPINodePtr &other) const
{
    return d.lock().get() != other.d.lock().get();
}

/** Dereference this pointer

    \throw SireError::nullptr_error
*/
MPINode MPINodePtr::operator*() const
{
    boost::shared_ptr<MPINodePvt> ptr = d.lock();
    
    if (ptr.get() == 0)
        throw SireError::nullptr_error( QObject::tr(
            "Attempting to dereference a null MPINode pointer"),   
                CODELOC );
                
    return MPINode(ptr);
}

/** Return whether or not the pointer is null */
bool MPINodePtr::isNull() const
{
    return d.expired();
}
