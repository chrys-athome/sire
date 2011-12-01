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

#include "Siren/worklog.h"
#include "Siren/siren.hpp"
#include "Siren/datetime.h"
#include "Siren/mutable.hpp"
#include "Siren/editor.hpp"

using namespace Siren;
using namespace Siren::detail;

namespace Siren
{
    namespace detail
    {
        /** This class holds the data for a WorkLog */
        class WorkLogData : public imp_shared_data
        {
        public:
            WorkLogData();
            WorkLogData(const WorkLogData &other);
            ~WorkLogData();
            
            WorkLogData& operator=(const WorkLogData &other);
            
            bool operator==(const WorkLogData &other) const;
            
            /** The time the job was submitted (this WorkQueueItem was created) */
            DateTime submission_time;
            
            /** The time the queue told us the job was running */
            DateTime start_time;
            
            /** The time the queue told us that the job had finished.
                The result is added to the Promise attached to this item */
            DateTime finish_time;
     
        }; // end of class WorkLogData
    
    } // end of namespace detail

} // end of namespace Siren

/////////
///////// Implementation of WorkLogData
/////////



/////////
///////// Implementation of WorkLog
/////////

/** Function used to construct a WorkLog that has the time the job was
    submitted as the current data and time */
WorkLog WorkLog::submitted()
{
    WorkLog log;
    log.d->submission_time = DateTime::current();
    return log;
}
