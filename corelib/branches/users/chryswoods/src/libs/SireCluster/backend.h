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

#include <boost/shared_ptr.hpp>

#include <QUuid>

SIRE_BEGIN_HEADER

namespace SireCluster
{

class WorkPacket;

namespace detail
{
class BackendPvt;
}

/** This is the public interface for a Backend. A Backend is an object
    that can receive a WorkPacket from a FrontEnd, can process that
    WorkPacket, and then returns the processed WorkPacket back to
    the FrontEnd. A Backend is always created paired with a FrontEnd
    
    A Backend is explicitly shared and thread-safe.
    
    @author Christopher Woods
*/
class SIRECLUSTER_EXPORT Backend
{
public:
    Backend();
    Backend(const Backend &other);
    
    ~Backend();
    
    Backend& operator=(const Backend &other);
    
    bool operator==(const Backend &other) const;
    bool operator!=(const Backend &other) const;
    
    bool isNull() const;
    
    QUuid UID() const;

    static Backend create();
    
    void startJob(const WorkPacket &workpacket);
    
    void stopJob();
    void abortJob();
    
    void wait();
    bool wait(int timeout);
    
    float progress();
    WorkPacket interimResult();
    
    WorkPacket result();
    
    void connect();
    bool tryConnect();
    void disconnect();
    
    void shutdown();
    
private:
    /** Private implementation */
    boost::shared_ptr<detail::BackendPvt> d;
};

}

SIRE_EXPOSE_CLASS( SireCluster::Backend )

SIRE_END_HEADER

#endif
