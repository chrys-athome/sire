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

class MPIBackends;
class MPINode;
class MPINodes;

namespace detail
{
class MPIFrontEndPvt;
class MPIFrontEndsPvt;
}

MPIFrontEnd getFrontEnd(const MPINode &node);
MPIFrontEnds getFrontEnds(const MPINodes &nodes);

/** This class provides a registry for all of the front ends
    for a particular MPI communicator. The front ends are only
    visible on the master node for the communicator
    
    @author Christopher Woods
*/
class SIREMPI_EXPORT MPIFrontEnds
{

friend MPIFrontEnds getFrontEnds(const MPINodes &nodes);

public:
    MPIFrontEnds();
    MPIFrontEnds(const MPINodes &nodes);
    
    MPIFrontEnds(const MPIFrontEnds &other);
    
    ~MPIFrontEnds();
    
    MPIFrontEnds& operator=(const MPIFrontEnds &other);
    
    bool operator==(const MPIFrontEnds &other) const;
    bool operator!=(const MPIFrontEnds &other) const;
    
    MPIFrontEnd getFrontEnd(const MPINode &node);

    void shutdown();

private:
    MPIFrontEnds(const MPINodes &nodes, bool);

    /** PIMPL pointer for this object */
    boost::shared_ptr<detail::MPIFrontEndsPvt> d;
};

/** This class provides a frontend that can be used to communicate
    with the backend running on a remote MPI node 
    
    @author Christopher Woods
*/
class SIREMPI_EXPORT MPIFrontEnd
{

friend class MPIBackends;

public:
    MPIFrontEnd();
    MPIFrontEnd(const MPINode &node);
    MPIFrontEnd(const MPIFrontEnd &other);
    
    ~MPIFrontEnd();
    
    MPIFrontEnd& operator=(const MPIFrontEnd &other);
    
    bool operator==(const MPIFrontEnd &other) const;
    bool operator!=(const MPIFrontEnd &other) const;
    
    bool isBusy();
    
    void performWork(MPIPromise &promise);
    
    void stopWork();
    void abortWork();
    
    void requestProgress();
    void requestInterimResult();
    
protected:
    MPIFrontEnd(const MPINode &node, bool);
    
private:
    /** PIMPL pointer for this object */
    boost::shared_ptr<detail::MPIFrontEndPvt> d;
};

}

SIRE_END_HEADER

#endif
