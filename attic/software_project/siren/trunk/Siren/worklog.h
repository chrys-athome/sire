#ifndef SIREN_WORKLOG_H
#define SIREN_WORKLOG_H

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
#include "Siren/mutable.h"
#include "Siren/interfaces.h"
#include "Siren/editor.h"

SIREN_BEGIN_HEADER

namespace Siren
{
    class WorkLogEditor;
    
    namespace detail
    {
        class WorkLogData;
    }

    /** This class holds a record (log) of metadata about a WorkPacket
        as it is being processed by a WorkQueue. This is used by the WorkQueue
        to monitor the health of the calculation, the time taken and progress
        made during the calculation, and to hold metadata that may be useful
        for job scheduling
        
        @author Christopher Woods
    */
    class SIREN_EXPORT WorkLog : public Object, 
                                 public Interfaces< Mutable<WorkLogEditor,
                                                            WorkLog> >
    {
        SIREN_CLASS(WorkLog,Object,1)
    
    public:
        WorkLog();
        WorkLog(const WorkLog &other);
        ~WorkLog();
        
        DateTime submissionTime() const;
        DateTime startTime() const;
        DateTime finishTime() const;
        
        bool hasStarted() const;
        bool hasFinished() const;
        
        int nChunks() const;
        
        DateTime chunkStartTime(int i) const;
        DateTime chunkFinishTime(int i) const;
        
        bool chunkHasStarted(int i) const;
        bool chunkHasFinished(int i) const;
        
        int chunkDuration(int i) const;
        int averageChunkDuration() const;
        
        int progress() const;
        
        static WorkLog submitted();
        
    protected:
        void copy_object(const WorkLog &other);
        bool compare_object(const WorkLog &other) const;
        
    private:
        friend class WorkLogEditor;
        imp_shared_ptr<detail::WorkLogData>::Type d;
    
    }; // end of class WorkLog

    /** This class provide the editor for a Siren::WorkLog */
    class SIREN_EXPORT WorkLogEditor : public Editor<WorkLogEditor,WorkLog>
    {
    public:
        WorkLogEditor();
        WorkLogEditor(const WorkLog &log);
        WorkLogEditor(const WorkLogEditor &other);
        
        ~WorkLogEditor();
        
        WorkLogEditor& operator=(const WorkLogEditor &other);
        
        bool operator==(const WorkLogEditor &other) const;

        WorkLogEditor& chunkStarted();
        WorkLogEditor& chunkFinished();
        WorkLogEditor& chunkHadError();
        
        WorkLogEditor& chunkStarted(int worker_id, int nworkers);
        WorkLogEditor& chunkFinished(int worker_id, int nworkers);
        WorkLogEditor& chunkHadError(int worker_id, int nworkers);

        WorkLogEditor& jobAborted();
        WorkLogEditor& jobToBG();
        WorkLogEditor& jobToFG();

    private:
        imp_shared_ptr<detail::WorkLogData>::Type d;
    
    }; // end of class WorkLogEditor

} // end of namespace Siren

SIREN_EXPOSE_CLASS( Siren::WorkLog )
SIREN_EXPOSE_CLASS( Siren::WorkLogEditor )

SIREN_END_HEADER

#endif // ifndef SIREN_WORKLOG_H
