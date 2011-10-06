#ifndef SIREN_EXCEPTIONS_H
#define SIREN_EXCEPTIONS_H
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

#include "siren.h"
#include "exception.h"

SIREN_BEGIN_HEADER

namespace Siren
{
    /** This exception is thrown when an attempt is made
        to cast an object to a class of the wrong type
        
        @author Christopher Woods
    */
    class SIREN_EXPORT invalid_cast : public Exception
    {
        SIREN_CLASS( invalid_cast, Exception )
    
    public:
        invalid_cast();
        invalid_cast(const String &error, CODELOC_ARGS);
        
        invalid_cast(const invalid_cast &other);
        
        ~invalid_cast() throw();
        
        invalid_cast& operator=(const invalid_cast &other);
        
        bool operator==(const invalid_cast &other) const;
        bool operator!=(const invalid_cast &other) const;

        void throwSelf() const;
        
    }; // end of class invalid_cast
    
    /** This exception is thrown when a numeric value
        held in one type would overflow when placed into
        another type 
        
        @author Christopher Woods
    */
    class SIREN_EXPORT numeric_overflow : public Exception
    {
        SIREN_CLASS( numeric_overflow, Exception )
        
    public:
        numeric_overflow();
        numeric_overflow(const String &error, CODELOC_ARGS);
        
        numeric_overflow(const numeric_overflow &other);
        
        ~numeric_overflow() throw();
        
        numeric_overflow& operator=(const numeric_overflow &other);
        
        bool operator==(const numeric_overflow &other) const;
        bool operator!=(const numeric_overflow &other) const;
        
        void throwSelf() const;

    }; // end of class numeric_overflow

} // end of namespace Siren

SIREN_EXPOSE_EXCEPTION( Siren::invalid_cast )
SIREN_EXPOSE_EXCEPTION( Siren::numeric_overflow )

SIREN_END_HEADER

#endif // ifndef SIREN_EXCEPTIONS_H
