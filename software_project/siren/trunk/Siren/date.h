#ifndef SIREN_DATE_H
#define SIREN_DATE_H
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
    /** This class provides functions for manipulating and 
        querying dates */
    class SIREN_EXPORT Date : public Object
    {
        SIREN_CLASS( Date, Object )
    
    public:
        Date();
        Date(int y, int m, int d);
        
        Date(const Date &other);
        
        #ifdef SIREN_QT_SUPPORT
            Date(const QDate &date);
            operator QDate() const;
        #endif
        
        ~Date();
        
        String toString() const;
        
        int day() const;
        int month() const;
        int year() const;
        
        int dayOfWeek() const;
        int dayOfYear() const;
        
        int daysInMonth() const;
        int daysInYear() const;
        
        bool isValid() const;
        
        bool operator<(const Date &other) const;
        bool operator<=(const Date &other) const;
        bool operator>=(const Date &other) const;
        bool operator>(const Date &other) const;
        
        static Date current();
        
    protected:
        void copy_object(const Date &other);
        bool compare_object(const Date &other) const;
        
    private:
        /** The Qt QDate class is used to provide date functions */
        QDate d;
            
    }; // end of class Date

} // end of namespace Siren

SIREN_EXPOSE_CLASS( Siren::Date )

SIREN_END_HEADER

#endif // ifndef SIREN_TIME_H
