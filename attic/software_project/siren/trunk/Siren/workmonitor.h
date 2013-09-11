#ifndef SIREN_WORKMONITOR_H
#define SIREN_WORKMONITOR_H

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
    class Percentage;

    namespace detail { class WorkMonitorData; }

    /** This is a lightweight class that can be used to record the progress, 
        speed, energy consumption, and health of a calculation 
        
        @author Christopher Woods
    */
    class SIREN_EXPORT WorkMonitor
    {
    public:
        WorkMonitor();
        WorkMonitor(const WorkMonitor &other);
        
        ~WorkMonitor();
        
        WorkMonitor& operator=(const WorkMonitor &other);
        
        bool operator==(const WorkMonitor &other) const;
        bool operator!=(const WorkMonitor &other) const;
        
        void addFLOPs(int32 nflops);
        void addEnergy(int32 joules);
        
        void setProgress(Percentage progress);
        void setHealth(Percentage health);
        
    private:
        exp_shared_ptr<detail::WorkMonitorData>::Type d;
    
    }; // end class WorkMonitor

} //end of namespace Siren

SIREN_EXPOSE_CLASS( Siren::WorkMonitor )

SIREN_END_HEADER

#endif // ifndef SIREN_WORKMONITOR_H
