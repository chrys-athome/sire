#ifndef SIREN_TIMER_H
#define SIREN_TIMER_H
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
    /** This class provides a timer that can be used 
        to time how long operations take */
    class SIREN_EXPORT Timer : public Object
    {
        SIREN_CLASS(Timer,Object,1)
        
    public:
        Timer();
        Timer(const Timer &other);
        
        ~Timer();
        
        static Timer start();
        
        int elapsed() const;
        
    protected:
        void copy_object(const Timer &other);
        bool compare_object(const Timer &other) const;
        
    private:
        /** A Qt QTime is used to provide a timer */
        QTime d;
    
    }; // end of class Timer

} // end of namespace Siren

SIREN_EXPOSE_CLASS( Siren::Timer )

SIREN_END_HEADER

#endif // ifndef SIREN_TIMER_H
