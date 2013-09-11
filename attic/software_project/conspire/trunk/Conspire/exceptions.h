#ifndef CONSPIRE_EXCEPTIONS_H
#define CONSPIRE_EXCEPTIONS_H
/********************************************\
  *
  *  Conspire
  *
  *  Copyright (C) 2012  Christopher Woods
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

#include "Conspire/conspire.h"
#include "Siren/exceptions.h"

SIREN_BEGIN_HEADER

namespace Conspire
{
    //import all of the base Siren exceptions 
    //into the Conspire namespace
    using namespace Siren::Exceptions;
    using Siren::Exception;

    namespace Exceptions
    {
        /** This exception is thrown when there is an error 
            parsing a Conspire file
        */
        class SIREN_EXPORT parse_error : public Exception
        {
            SIREN_CLASS(parse_error,Exception,1)
            
        public:
            parse_error();
            parse_error(const Qt::String &error, CODELOC_ARGS);
            
            parse_error(const parse_error &other);
            
            ~parse_error() throw();
            
            void throwSelf() const;
        
        }; // end of class parse_error
    }

    using namespace Conspire::Exceptions;
}

SIREN_EXPOSE_EXCEPTION( Conspire::parse_error )

SIREN_END_HEADER

#endif // ifndef CONSPIRE_EXCEPTIONS_H
