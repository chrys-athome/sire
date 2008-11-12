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

#ifndef SIREMPI_MPIFRONTENDS_H
#define SIREMPI_MPIFRONTENDS_H

#include "sireglobal.h"

#include <boost/shared_ptr.hpp>

SIRE_BEGIN_HEADER

namespace SireMPI
{

class MPIFrontend;
class MPIFrontends;

class MPIBackends;
class MPINode;
class MPINodes;

class MPIPromise;

namespace detail
{
class MPIFrontendPvt;
class MPIFrontendsPvt;
}

MPIFrontend getFrontend(const MPINode &node);
MPIFrontends getFrontends(const MPINodes &nodes);

/** This class provides a registry for all of the front ends
    for a particular MPI communicator. The front ends are only
    visible on the master node for the communicator
    
    @author Christopher Woods
*/
class SIREMPI_EXPORT MPIFrontends
{

friend MPIFrontends getFrontends(const MPINodes &nodes);

public:
    MPIFrontends();
    MPIFrontends(const MPINodes &nodes);
    
    MPIFrontends(const MPIFrontends &other);
    
    ~MPIFrontends();
    
    MPIFrontends& operator=(const MPIFrontends &other);
    
    bool operator==(const MPIFrontends &other) const;
    bool operator!=(const MPIFrontends &other) const;
    
    MPIFrontend getFrontEnd(const MPINode &node);

    void shutdown();

private:
    MPIFrontends(MPINodes nodes, bool);

    /** PIMPL pointer for this object */
    boost::shared_ptr<detail::MPIFrontendsPvt> d;
};

/** This class provides a frontend that can be used to communicate
    with the backend running on a remote MPI node 
    
    @author Christopher Woods
*/
class SIREMPI_EXPORT MPIFrontend
{

friend class MPIBackends;

public:
    MPIFrontend();
    MPIFrontend(const MPINode &node);
    MPIFrontend(const MPIFrontend &other);
    
    ~MPIFrontend();
    
    MPIFrontend& operator=(const MPIFrontend &other);
    
    bool operator==(const MPIFrontend &other) const;
    bool operator!=(const MPIFrontend &other) const;
    
    bool isBusy();
    
    void performWork(MPIPromise &promise);
    
    void stopWork();
    void abortWork();
    
    void requestProgress();
    void requestInterimResult();
    
protected:
    MPIFrontend(const MPINode &node, bool);
    
private:
    /** PIMPL pointer for this object */
    boost::shared_ptr<detail::MPIFrontendPvt> d;
};

}

SIRE_END_HEADER

#endif
