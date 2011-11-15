#ifndef SIREN_PROMISE_H
#define SIREN_PROMISE_H

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

#include "Siren/siren.h"
#include "Siren/obj.h"

SIREN_BEGIN_HEADER

namespace Siren
{
    class WorkPacket;
    class WorkQueue;
    class WorkSpace;

    namespace detail
    { 
        class PromiseData;
        class WorkQueueData;
        class WorkQueueItemData;
    
    } // end of namespace detail

    /** This class holds a promised result from calling a function.
        It provides a place-holder into which a result can be placed,
        and will block access to the result until it is available
        
        @author Christopher Woods
    */
    class SIREN_EXPORT Promise
    {
    public:
        Promise();
        Promise(const None &none);
        Promise(const Object &object);
        
        Promise(const Promise &other);
        
        ~Promise();
        
        Promise& operator=(const Promise &other);
        
        bool operator==(const Promise &other) const;
        bool operator!=(const Promise &other) const;
        
        static const char* typeName(){ return "Siren::Promise";}
        const char* what() const{ return typeName(); }
        
        Obj workPacket() const;
        
        bool available() const;
        
        void wait() const;
        bool wait(int ms) const;
        
        Obj result() const;
        Obj result(const Object &def) const;
        Obj result(const Object &def, int ms) const;
        
        void abort() const;
        void abort(int ms) const;
        
        void pause() const;
        
        Promise resubmit() const;
        
    protected:
        friend class detail::WorkQueueData;
        friend class detail::WorkQueueItemData;
        Promise(const WorkQueueItem &item);

        void cancel() const;  // called by the queueing system (abort is user)
    
        void jobStarted();
        void jobCancelled();
        void jobFinished(const Obj &result);
    
    private:
        /** Copy of the final result */
        Obj reslt;
        
        /** Shared pointer to the placeholder for the result,
            which contains a copy of the original WorkPacket used for 
            the computation */
        exp_shared_ptr<detail::PromiseData>::Type d;
    
    }; // end of class Promise

} // end of namespace Siren

SIREN_EXPOSE_CLASS( Siren::Promise )

SIREN_END_HEADER

#endif // ifndef SIREN_PROMISE_H
