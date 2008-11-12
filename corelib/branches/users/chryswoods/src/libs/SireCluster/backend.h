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

#ifndef SIRECLUSTER_BACKEND_H
#define SIRECLUSTER_BACKEND_H

#include "sireglobal.h"

#include <QUuid>
#include <QMutex>

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

SIRE_BEGIN_HEADER

namespace SireCluster
{

class Backend;
class Backends;
class Frontend;

/** This is the virtual base class of all backends - these are held
    in a Backend object - and are responsible for actually receiving
    the work packet from the Frontend, performing the result, and then
    returning the completed work packet back to the frontend 
    
    @author Christopher Woods
*/
class SIRECLUSTER_EXPORT BackendBase : public boost::noncopyable
{

friend class Backend;

public:
    BackendBase();
    
    virtual ~BackendBase();

    bool isNull() const;
    
    const QUuid& UID() const;

protected:    
    /** Start the backend's event loop */
    virtual void start()=0;
    
    /** Stop the backend's event loop */
    virtual void stop()=0;
    
    /** Wait until the backend's event loop has stopped */
    virtual void wait()=0;

    /** Return whether or not the backend's event loop is running */
    virtual bool isRunning()=0;

protected:
    BackendBase(const QUuid &uid);
    
private:
    /** The unique ID for this backend */
    QUuid uid;
    
    /** Mutex used to protect access to this backend */
    QMutex datamutex;
};

/** This is the public interface for a Backend. A Backend is an object
    that can receive a WorkPacket from a FrontEnd, can process that
    WorkPacket, and then returns the processed WorkPacket back to
    the FrontEnd. A Backend is always created paired with a FrontEnd
    
    A Backend is explicitly shared and thread-safe.
    
    @author Christopher Woods
*/
class SIRECLUSTER_EXPORT Backend
{

friend class Backends;
friend class Frontend;

public:
    Backend();
    Backend(const Backend &other);
    
    ~Backend();
    
    Backend& operator=(const Backend &other);
    
    bool operator==(const Backend &other) const;
    bool operator!=(const Backend &other) const;
    
    QUuid UID() const;
    
protected:
    Backend( const boost::shared_ptr<BackendBase> &ptr );
    
    void start();
    void stop();
    void wait();
    bool isRunning();
    
private:
    /** Private implementation */
    boost::shared_ptr<BackendBase> d;
};

}

SIRE_END_HEADER

#endif
