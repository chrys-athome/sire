#ifndef SIREN_EXCEPTION_H
#define SIREN_EXCEPTION_H
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
#include "Siren/string.h"
#include "Siren/stringlist.h"
#include "Siren/detail/boostsupport.h"

SIREN_BEGIN_HEADER

#define CODELOC __FILE__, __LINE__, BOOST_CURRENT_FUNCTION
#define CODELOC_ARGS const char *file, int line, const char *current_function

namespace Siren
{

    /** This is the base class of all Siren exceptions.
        Siren exceptions provide additional information
        to standard C++ exceptions, e.g. a backtrace
        from where the exception is thrown, and the exception
        can be serialised and transferred between threads
        
        @author Christopher Woods
    */
    class SIREN_EXPORT Exception : public Object
    {
        SIREN_VIRTUAL_CLASS( Exception, Object )
    
    public:
        Exception();
        Exception(const String &error, CODELOC_ARGS);
        
        Exception(const Exception &other);
        
        ~Exception() throw();
        
        String error() const;
        String from() const;
        
        StringList backTrace() const;
        
        String where() const;
        String why() const;
        
        String pid() const;
        
        virtual void throwSelf() const=0;

    protected:
        void copy_object(const Exception &other);
        bool compare_object(const Exception &other) const;
        
    private:
        /** The error message associated with this exception */
        String err;
        
        /** The location (function, file and line number) from 
            where the exception was thrown */
        String plce;
        
        /** The backtrace associated with the error */
        StringList bt;
    };


} // end of namespace Siren

SIREN_END_HEADER

#endif // ifdef SIREN_EXCEPTION_H
