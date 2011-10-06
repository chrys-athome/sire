#ifndef SIREN_METATYPE_HPP
#define SIREN_METATYPE_HPP

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

#include "Siren/detail/metatype.h"

SIREN_BEGIN_HEADER

#ifndef SIREN_EXCLUDE_TEMPLATE_CODE
namespace Siren
{
    namespace detail
    {

        /////////////////
        ///////////////// Implementation of the template functions
        ///////////////// of ConcreteClassData<T>
        /////////////////

        /** Create a metadata object to register the class T */
        template<class T>
        SIREN_OUTOFLINE_TEMPLATE
        ConcreteClassData<T>::ConcreteClassData() 
                             : ClassData( T::typeName(), 
                                          T::super::typeName(), 
                                          T::ifaces() )
        {
            Siren::detail::registerObject(*this);
        }

        /** Destructor */
        template<class T>
        SIREN_OUTOFLINE_TEMPLATE
        ConcreteClassData<T>::~ConcreteClassData()
        {}

        /** Create an object of the specified type - note that you are
            responsible for deleting the object */
        template<class T>
        SIREN_OUTOFLINE_TEMPLATE
        Object* ConcreteClassData<T>::createObject() const
        {
            return new T();
        }

        /** Returns that the registered object is concrete (non-virtual) */
        template<class T>
        SIREN_OUTOFLINE_TEMPLATE
        bool ConcreteClassData<T>::isConcrete() const
        {
            return true;
        }

        /////////////////
        ///////////////// Implementation of the template functions
        ///////////////// of VirtualClassData<T>
        /////////////////

        /** Create a metadata object to register the class T */
        template<class T>
        SIREN_OUTOFLINE_TEMPLATE
        VirtualClassData<T>::VirtualClassData() 
                              : ClassData( T::typeName(), 
                                           T::super::typeName(), 
                                           T::interfaces() )
        {
            Siren::detail::registerObject(*this);
        }

        /** Destructor */
        template<class T>
        SIREN_OUTOFLINE_TEMPLATE
        VirtualClassData<T>::~VirtualClassData()
        {}

        /** This returns "0" as it is not possible to create virtual objects */
        template<class T>
        SIREN_OUTOFLINE_TEMPLATE
        Object* VirtualClassData<T>::createObject() const
        {
            return 0;
        }

        /** Returns that the registered object is not concrete (it is virtual) */
        template<class T>
        SIREN_OUTOFLINE_TEMPLATE
        bool VirtualClassData<T>::isConcrete() const
        {
            return false;
        }

    } // end of namespace Siren::detail
} // end of namespace Siren
#endif // SIREN_EXCLUDE_TEMPLATE_CODE

SIREN_END_HEADER

#endif // end of ifndef SIREN_METATYPE_H
