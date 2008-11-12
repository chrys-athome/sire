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

#include "backend.h"

using namespace SireCluster;

using boost::shared_ptr;

/////////
///////// Implementation of BackendBase
/////////

/** Constructor */
BackendBase::BackendBase() 
            : boost::noncopyable(),
              datamutex(QMutex::Recursive)
{}

/** Protected constructor used to set the UID */
BackendBase::BackendBase(const QUuid &node_uid) 
            : boost::noncopyable(),
              uid(node_uid), datamutex(QMutex::Recursive)
{}

/** Destructor */    
BackendBase::~BackendBase()
{}

/** Return whether or not this is a null backend */
bool BackendBase::isNull() const
{
    return uid.isNull();
}

/** Return the unique ID for the node connected to this backend */
const QUuid& BackendBase::UID() const
{
    return uid;
}

/////////
///////// Implementation of Backend
/////////

/** Construct a null backend */
Backend::Backend()
{}

/** Construct a backend from the passed pointer */
Backend::Backend( const boost::shared_ptr<BackendBase> &ptr )
        : d(ptr)
{}

/** Copy constructor */
Backend::Backend(const Backend &other)
        : d(other.d)
{}        

/** Destructor */
Backend::~Backend()
{}

/** Copy assignment operator */
Backend& Backend::operator=(const Backend &other)
{
    d = other.d;
    return *this;
}

/** Comparison operator */
bool Backend::operator==(const Backend &other) const
{
    return d.get() == other.d.get();
}

/** Comparison operator */
bool Backend::operator!=(const Backend &other) const
{
    return d.get() != other.d.get();
}

/** Return the UID of the node connected to the backend */
QUuid Backend::UID() const
{
    if (d.get() == 0)
        return QUuid();
    else
        return d->UID();
}

/** Return whether or not the backend's event loop is running */
bool Backend::isRunning()
{
    QMutexLocker lkr( &(d->datamutex) );
    return d->isRunning();
}

/** Start the event loop on the backend */
void Backend::start()
{
    QMutexLocker lkr( &(d->datamutex) );
    
    if (d->isRunning())
        return;
        
    d->start();
}

/** Stop the event loop on the backend */
void Backend::stop()
{
    QMutexLocker lkr( &(d->datamutex) );

    if (not d->isRunning())
        return;
        
    d->stop();
}

/** Wait for the event loop to finish */
void Backend::wait()
{
    QMutexLocker lkr( &(d->datamutex) );
    
    if (d->isRunning())
        d->wait();
}
