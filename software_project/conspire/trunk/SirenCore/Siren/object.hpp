#ifndef SIREN_OBJECT_HPP
#define SIREN_OBJECT_HPP

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

#include "Siren/object.h"
#include "Siren/class.h"

SIREN_BEGIN_HEADER

namespace Siren
{

    template<class T>
    SIREN_OUTOFLINE_TEMPLATE
    bool Object::isA() const
    {
        const T *ptr = dynamic_cast<const T*>(this);
        
        if (ptr)
            return true;
            
        return this->getClass().canCast( T::typeName() );
    }

    template<class T>
    SIREN_OUTOFLINE_TEMPLATE
    void Object::assertIsA() const
    {
        if (not this->isA<T>())
            this->throwInvalidCast( T::typeName() );
    }

    template<class T>
    SIREN_OUTOFLINE_TEMPLATE
    const T& Object::asA() const
    {
        const T *ptr = dynamic_cast<const T*>(this);
        
        if (ptr)
            return *ptr;
            
        if (not this->getClass().canCast( T::typeName() ))
            this->throwInvalidCast( T::typeName() );
            
        return *((const T*)(this));
    }

} // end of namespace Siren

SIREN_END_HEADER

#endif // ifndef SIREN_OBJECT_HPP
