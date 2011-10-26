#ifndef SIREN_WORKPACKET_H
#define SIREN_WORKPACKET_H

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

SIREN_BEGIN_HEADER

namespace Siren
{
    class WorkSpace;

    /** A WorkPacket is the base class of objects that contain
        packets of computational work that need to be processed.
        
        @author Christopher Woods
    */
    class SIREN_EXPORT WorkPacket : public Object
    {
        SIREN_VIRTUAL_CLASS(WorkPacket,Object,1)
    
    public:
        WorkPacket();
        WorkPacket(const WorkPacket &other);
        
        ~WorkPacket();
        
        virtual bool needsWorkspace() const;
        virtual Obj createWorkspace() const;
        virtual Obj createWorkspace(int worker_id) const;
        
        virtual bool isFinished() const=0;
        virtual int progress() const=0;

        Obj run() const throw();
        Obj run(WorkSpace &workspace) const throw();
        Obj run(WorkSpace &workspace, int worker_id) const throw();
        
    protected:
        virtual Obj runChunk() const=0;
        virtual Obj runChunk(WorkSpace &workspace) const;
        virtual Obj runChunk(WorkSpace &workspace, int worker_id) const;

        void copy_object(const WorkPacket &other);
        bool compare_object(const WorkPacket &other) const;
        
    }; // end of class WorkPacket


    /** This is a simple WorkPacket that is used for testing */
    class SIREN_EXPORT TestPacket : public WorkPacket
    {
        SIREN_CLASS(TestPacket,WorkPacket,1)
        
    public:
        TestPacket();
        TestPacket(int n);
        TestPacket(const TestPacket &other);
        
        ~TestPacket();
        
        bool isFinished() const;
        int progress() const;
        
    protected:
        Obj runChunk() const;
        
        void test(TestReportEditor &report) const;

        void copy_object(const TestPacket &other);
        bool compare_object(const TestPacket &other) const;
    
    private:
        /** The number to which we must count */
        int target_val;
        
        /** The current value */
        int current_val;
    
    }; // end of class TestPacket

} // end of namespace Siren

SIREN_EXPOSE_CLASS( Siren::WorkPacket )
SIREN_EXPOSE_CLASS( Siren::TestPacket )

SIREN_END_HEADER

#endif // ifndef SIREN_WORKPACKET_H
