#ifndef SIREN_DATETIME_H
#define SIREN_DATETIME_H
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
#include "Siren/date.h"
#include "Siren/time.h"

SIREN_BEGIN_HEADER

namespace Siren
{

    /** This class is used to query and handle the combination
        of a time and a date */
    class SIREN_EXPORT DateTime : public Object
    {
        SIREN_CLASS(DateTime,Object,1)
    
    public:
        DateTime();
        DateTime(const Date &date, const Time &time);

        DateTime(const DateTime &other);

        ~DateTime();
        
        bool isNull() const;
        
        Date date() const;
        Time time() const;
        
        String toString() const;
        
        static DateTime current();
        
    protected:
        void copy_object(const DateTime &other);
        bool compare_object(const DateTime &other) const;
        
    private:
        /** The date */
        Date d;
        
        /** The time */
        Time t;
    
    }; // end of class DateTime

} // end of namespace Siren

SIREN_EXPOSE_CLASS( Siren::DateTime )

#endif // ifndef SIREN_DATETIME_H
