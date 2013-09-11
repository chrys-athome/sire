#ifndef SIREN_ASSERT_H
#define SIREN_ASSERT_H

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
#include "Siren/exception.h"

SIREN_BEGIN_HEADER

namespace Siren
{
    /** This exception is written to accompany the "SIREN_ASSERT"
        macro, which is used to perform compile-time assert tests.
        
        @author Christopher Woods
    */
    class SIREN_EXPORT assertation_error : public Exception
    {
        SIREN_CLASS(assertation_error,Exception,1)
        
    public:
        assertation_error();
        assertation_error(const char *condition, CODELOC_ARGS);
        
        assertation_error(const assertation_error &other);
        
        ~assertation_error() throw();
        
        void throwSelf() const;
    
    }; // end of class assertation_error

    #define SIREN_ASSERT( COND ) \
                do { if (not (COND)) \
                    throw Siren::assertation_error( #COND, CODELOC ); } while(0);
}

SIREN_EXPOSE_EXCEPTION( Siren::assertation_error )

SIREN_END_HEADER

#endif // ifndef SIREN_ASSERT_H
