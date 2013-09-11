#ifndef SIREN_STATIC_H
#define SIREN_STATIC_H
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
#include "Siren/assert.h"

SIREN_BEGIN_HEADER

namespace Siren
{
    namespace detail
    {
        /** A simple class that registers the passed function
            with the global Siren init and fini functions */
        class Static
        {
        public:
            Static( void (*create_function)(), void (*delete_function)(),
                    const char *func, const char *type);
                    
            ~Static();
            
            static bool beingCreated();
            static bool beingDeleted();
            
            static void createAll();
            static void deleteAll();
        };
    
    } // end of namespace detail

} // end of namespace Siren

#define SIREN_STATIC(TYPE,FUNC) \
     namespace Siren_Static_ ## FUNC { \
         \
         static exp_shared_ptr<TYPE>::Type foo; \
         static exp_weak_ptr<TYPE>::Type weak_foo; \
         \
         void createFoo() \
         { \
             if (not foo) \
             { \
                 foo.reset(new TYPE()); \
                 weak_foo = foo; \
             } \
         } \
         \
         void deleteFoo() \
         { \
             foo.reset(); \
         } \
         \
         static Siren::detail::Static foo_static( &createFoo, &deleteFoo, \
                                                  #TYPE, #FUNC ); \
    } \
    \
    exp_shared_ptr<TYPE>::Type FUNC() \
    { \
        exp_shared_ptr<TYPE>::Type ptr = Siren_Static_ ## FUNC::weak_foo.lock(); \
        \
        if (not ptr) \
        { \
            if (Siren::detail::Static::beingCreated()) \
                Siren_Static_ ## FUNC::createFoo(); \
        } \
        \
        return Siren_Static_ ## FUNC::weak_foo.lock(); \
    }         

SIREN_END_HEADER

#endif // ifndef SIREN_STATIC_H
