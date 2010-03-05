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

#ifndef SIRECLUSTER_MPI_MPICLUSTER_H
#define SIRECLUSTER_MPI_MPICLUSTER_H

#ifdef SIRE_USE_MPI

#include <boost/function.hpp>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireCluster
{
    namespace MPI
    {

        /** This class provides the global interface that abstracts
            the sending of messages between nodes using MPI
    
            This is a private class which is only available internally
            to SireCluster if MPI is available

            @author Christopher Woods
        */
        class MPICluster
        {
        public:
            static void send(int rank, const QByteArray &message,
                             boost::function<void ()> acknowledge_send_function);

            static void exec(int argc, char **argv);
    
            static void start(int argc, char **argv);
            static void shutdown();
            
            static void sync();
            
            static int rank();
            static int count();

            static QString hostName();
            
        };

    } // end of namespace MPI

} // end of namespace SireCluster

SIRE_END_HEADER

#endif // SIRE_USE_MPI

#endif
