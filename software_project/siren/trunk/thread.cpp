/********************************************\
  *
  *  Siren - C++ metaobject library
  *
  *  Copyright (C) 2011  Christopher Woods
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

#include "Siren/thread.h"
#include "Siren/siren.hpp"

namespace Siren
{
    namespace detail
    {
        /** This internal class represents a real, physical CPU thread. */
        class CPUThread : public QThread
        {
        public:
            CPUThread();
            ~CPUThread();
            
            void process(const WorkPacket &workpacket);
            
            void process(const WorkPacket &workpacket, 
                         WorkSpace &workspace,
                         int id);
            
            void pause();
            void play();
            void abort();
            
            int ID() const;
            String description() const;
            
        protected:
            void run();
        
        private:
            /** The WorkPacket to process */
            const WorkPacket *workpacket;
            
            /** The WorkSpace that may be used during processing */
            WorkSpace *workspace;
            
            /** The ID of the thread in the group processing
                the WorkPacket */
            int packet_id;

            /** The name of the thread - this comes from the WorkPacket
                being processed */
            String thread_name;
            
            /** The ID of the thread */
            int thread_id;
        };

    } // end of namespace detail
    
} // end of namespace Siren
