#ifndef SIREN_TIME_H
#define SIREN_TIME_H
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
    /** This class provides functions to query time. */
    class SIREN_EXPORT Time : public Object
    {
        SIREN_CLASS(Time,Object,1)
    
    public:
        Time();
        Time(int h, int m, int s=0, int ms=0);
        
        #ifdef SIREN_QT_SUPPORT
            Time(const QTime &time);
            operator QTime() const;
        #endif
        
        Time(const Time &other);
        
        ~Time();

        int hour() const;
        int minute() const;
        int second() const;
        int millisecond() const;
        
        int secsTo(const Time &other) const;
        int msecsTo(const Time &other) const;
        
        String toString() const;
        
        bool operator<(const Time &other) const;
        bool operator<=(const Time &other) const;
        bool operator>=(const Time &other) const;
        bool operator>(const Time &other) const;
        
        static Time current();
        
    protected:
        void copy_object(const Time &other);
        bool compare_object(const Time &other) const;
    
    private:
        /** A Qt QTime is used to get and query the date */
        QTime d;
    
    }; // end of class Time

} // end of namespace Siren

SIREN_EXPOSE_CLASS( Siren::Time )

SIREN_END_HEADER

#endif // ifndef SIREN_TIME_H
