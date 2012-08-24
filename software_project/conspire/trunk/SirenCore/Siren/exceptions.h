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
    namespace Exceptions
    {
        /** This exception is thrown when a problem is detected
            with a file or directory
            
            @author Christopher Woods
        */
        class SIREN_EXPORT file_error : public Exception
        {
            SIREN_CLASS(file_error,Exception,1)
        
        public:
            file_error();
            file_error(const Qt::String &error, CODELOC_ARGS);
            file_error(const Qt::File &f,
                       const Qt::String &error, CODELOC_ARGS);
            
            file_error(const file_error &other);
            
            ~file_error() throw();

            void throwSelf() const;
            
        }; // end of class file_error
    
        /** This exception is thrown when an attempt is made
            to cast an object to a class of the wrong type
            
            @author Christopher Woods
        */
        class SIREN_EXPORT invalid_cast : public Exception
        {
            SIREN_CLASS(invalid_cast,Exception,1)
        
        public:
            invalid_cast();
            invalid_cast(const Qt::String &error, CODELOC_ARGS);
            
            invalid_cast(const invalid_cast &other);
            
            ~invalid_cast() throw();

            void throwSelf() const;
            
        }; // end of class invalid_cast

        /** This exception is thrown when an attempt is made
            to access a list or array with an invalid index
            
            @author Christopher Woods
        */
        class SIREN_EXPORT invalid_index : public Exception
        {
            SIREN_CLASS(invalid_index,Exception,1)
        
        public:
            invalid_index();
            invalid_index(const Qt::String &error, CODELOC_ARGS);
            
            invalid_index(const invalid_index &other);
            
            ~invalid_index() throw();

            void throwSelf() const;
            
        }; // end of class invalid_index

        /** This exception is thrown when an attempt is made
            to access a hash, dictionary or map with an invalid key
            
            @author Christopher Woods
        */
        class SIREN_EXPORT invalid_key : public Exception
        {
            SIREN_CLASS(invalid_key,Exception,1)
        
        public:
            invalid_key();
            invalid_key(const Qt::String &error, CODELOC_ARGS);
            
            invalid_key(const invalid_key &other);
            
            ~invalid_key() throw();

            void throwSelf() const;
            
        }; // end of class invalid_key
        
        /** This exception is thrown when a numeric value
            held in one type would overflow when placed into
            another type 
            
            @author Christopher Woods
        */
        class SIREN_EXPORT numeric_overflow : public Exception
        {
            SIREN_CLASS(numeric_overflow,Exception,1)
            
        public:
            numeric_overflow();
            numeric_overflow(const Qt::String &error, CODELOC_ARGS);
            
            numeric_overflow(const numeric_overflow &other);
            
            ~numeric_overflow() throw();
            
            void throwSelf() const;

        }; // end of class numeric_overflow

        /** This exception is thrown when a call is made of some
            code that has not yet been written! 
        */
        class SIREN_EXPORT incomplete_code : public Exception
        {
            SIREN_CLASS(incomplete_code,Exception,1)
            
        public:
            incomplete_code();
            incomplete_code(const Qt::String &error, CODELOC_ARGS);
            
            incomplete_code(const incomplete_code &other);
            
            ~incomplete_code() throw();
            
            void throwSelf() const;
        
        }; // end of class incomplete_code

        /** This exception is thrown when a bug in the program is detected 
        */
        class SIREN_EXPORT program_bug : public Exception
        {
            SIREN_CLASS(program_bug,Exception,1)
            
        public:
            program_bug();
            program_bug(const Qt::String &error, CODELOC_ARGS);
            
            program_bug(const program_bug &other);
            
            ~program_bug() throw();
            
            void throwSelf() const;
        
        }; // end of class program_bug

        /** This exception is thrown when a request is made
            of a class that is not registered with Siren
        */
        class SIREN_EXPORT unavailable_class : public Exception
        {
            SIREN_CLASS(unavailable_class,Exception,1)
            
        public:
            unavailable_class();
            unavailable_class(const Qt::String &error, CODELOC_ARGS);
            
            unavailable_class(const unavailable_class &other);
            
            ~unavailable_class() throw();
            
            void throwSelf() const;
        
        }; // end of class unavailable_class

        /** This exception is thrown when a standard C++ exception is caught 
            (it acts to translate standard C++ exceptions into Siren exceptions) */
        class SIREN_EXPORT standard_exception : public Exception
        {
            SIREN_CLASS(standard_exception,Exception,1)
        
        public:
            standard_exception();
            standard_exception(const std::exception &e, CODELOC_ARGS);
            standard_exception(const Qt::String &error, const std::exception &e,
                               CODELOC_ARGS);
            
            standard_exception(const standard_exception &other);
            
            ~standard_exception() throw();
            
            void throwSelf() const;
            
        }; // end of class standard_exception

        /** This exception is thrown when an unknown exception is caught
            (it acts to translate unknown exceptions into Siren exceptions) */
        class SIREN_EXPORT unknown_exception : public Exception
        {
            SIREN_CLASS(unknown_exception,Exception,1)
            
        public:
            unknown_exception();
            unknown_exception(CODELOC_ARGS);
            unknown_exception(const Qt::String &error, CODELOC_ARGS);
        
            unknown_exception(const unknown_exception &other);
            
            ~unknown_exception() throw();
            
            void throwSelf() const;
        
        }; // end of class unknown_exception

        /** This exception is thrown when the end of for_ages is
            signalled, and any blocked threads are interupted
        */
        class SIREN_EXPORT interupted_thread : public Exception
        {
            SIREN_CLASS(interupted_thread,Exception,1)
            
        public:
            interupted_thread();
            interupted_thread(const Qt::String &error, CODELOC_ARGS);
            
            interupted_thread(const interupted_thread &other);
            
            ~interupted_thread() throw();
            
            void throwSelf() const;
        
        }; // end of class interupted_thread

        /** This exception is thrown when a function or constructor
            is called using an invalid argument, e.g. passing a 
            negative number to a function that is expecting
            positive numbers.
        */
        class SIREN_EXPORT invalid_arg : public Exception
        {
            SIREN_CLASS(invalid_arg,Exception,1)
            
        public:
            invalid_arg();
            invalid_arg(const Qt::String &error, CODELOC_ARGS);
            
            invalid_arg(const invalid_arg &other);
            
            ~invalid_arg() throw();
            
            void throwSelf() const;
        
        }; // end of class invalid_arg

        /** This exception is thrown when a problem with the  
            computer system running this program is detected 
        */
        class SIREN_EXPORT system_error : public Exception
        {
            SIREN_CLASS(system_error,Exception,1)
            
        public:
            system_error();
            system_error(const Qt::String &error, CODELOC_ARGS);
            
            system_error(const system_error &other);
            
            ~system_error() throw();
            
            void throwSelf() const;
        
        }; // end of class system_error

        /** This exception is thrown when the program is 
            detected to be in an invalid state 
        */
        class SIREN_EXPORT invalid_state : public Exception
        {
            SIREN_CLASS(invalid_state,Exception,1)
            
        public:
            invalid_state();
            invalid_state(const Qt::String &error, CODELOC_ARGS);
            
            invalid_state(const invalid_state &other);
            
            ~invalid_state() throw();
            
            void throwSelf() const;
        
        }; // end of class invalid_state

        /** This exception is thrown when a request is made
            of a resource that does not exist
        */
        class SIREN_EXPORT unavailable_resource : public Exception
        {
            SIREN_CLASS(unavailable_resource,Exception,1)
            
        public:
            unavailable_resource();
            unavailable_resource(const Qt::String &error, CODELOC_ARGS);
            
            unavailable_resource(const unavailable_resource &other);
            
            ~unavailable_resource() throw();
            
            void throwSelf() const;
        
        }; // end of class system_error

    } // end of namespace Exceptions

    using namespace Siren::Exceptions;

} // end of namespace Siren

SIREN_EXPOSE_EXCEPTION( Siren::file_error )
SIREN_EXPOSE_EXCEPTION( Siren::invalid_cast )
SIREN_EXPOSE_EXCEPTION( Siren::invalid_index )
SIREN_EXPOSE_EXCEPTION( Siren::invalid_key )
SIREN_EXPOSE_EXCEPTION( Siren::numeric_overflow )
SIREN_EXPOSE_EXCEPTION( Siren::incomplete_code )
SIREN_EXPOSE_EXCEPTION( Siren::program_bug )
SIREN_EXPOSE_EXCEPTION( Siren::unavailable_class )
SIREN_EXPOSE_EXCEPTION( Siren::standard_exception )
SIREN_EXPOSE_EXCEPTION( Siren::unknown_exception )
SIREN_EXPOSE_EXCEPTION( Siren::interupted_thread )
SIREN_EXPOSE_EXCEPTION( Siren::invalid_arg )
SIREN_EXPOSE_EXCEPTION( Siren::system_error )
SIREN_EXPOSE_EXCEPTION( Siren::invalid_state )
SIREN_EXPOSE_EXCEPTION( Siren::unavailable_resource )

SIREN_END_HEADER

#endif // ifndef SIREN_EXCEPTIONS_H
