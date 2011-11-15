#ifndef SIREN_DETAIL_THREADDATA_H
#define SIREN_DETAIL_THREADDATA_H

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
#include "Siren/waitcondition.h"
#include "Siren/mutex.h"

SIREN_BEGIN_HEADER

namespace Siren
{
    namespace detail
    {
        /** This internal class contains the set of information needed by the 
            ThreadPool and Thread classes
        */
        class ThreadPoolData : public noncopyable
        {
        public:
            static exp_shared_ptr<ThreadPoolData>::Type createCPU(int n=1);
            static exp_shared_ptr<ThreadPoolData>::Type createCPU(
                        const exp_shared_ptr<ThreadPoolData>::Type &parent, int n=1);
            
            ~ThreadPoolData();
        
            int nDesired();
            int nForeGround();
            int nBackGround();

            bool isCPU();
            bool isGPU();

            exp_shared_ptr<ThreadPoolData>::Type getChild(int n=1);
        
            Promise run(const WorkPacket &packet, int n=1);
            Promise run(const WorkPacket &packet, 
                        WorkSpace &workspace, int n=1);
        
            static exp_shared_ptr<ThreadPoolData>::Type systemCPUs();
        
        protected:
            ThreadPoolData();
        
            void addChild(const exp_shared_ptr<ThreadPoolData>::Type &child);
            void reparent(const exp_shared_ptr<ThreadPoolData>::Type &parent);
            
            void returnThreads(int n);
        
        private:
            Mutex m;
            WaitCondition w;

            /** Shared pointer to self */
            exp_weak_ptr<ThreadPoolData>::Type self;

            /** Parent pool, to which free resources should be
                returned when this pool is deleted (or from which
                more resources can be requested) */
            exp_weak_ptr<ThreadPoolData>::Type prnt;

            /** All of the child pools, which may have open requests */
            List<exp_weak_ptr<ThreadPoolData>::Type>::Type chldrn;
            
            int n_desired;
            int n_allocated;
            int n_foreground;
            int n_background;
        
        }; // end of class ThreadPoolData
    
    } // end of namespace detail


} // end of namespace Siren

SIREN_END_HEADER

#endif // ifndef SIREN_DETAIL_THREADDATA_H
