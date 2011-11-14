#ifndef SIREN_WORKSPACE_HPP
#define SIREN_WORKSPACE_HPP

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

#include "Siren/workspace.h"
#include "Siren/exceptions.h"

SIREN_BEGIN_HEADER

namespace Siren
{
    template<class T>
    SIREN_OUTOFLINE_TEMPLATE
    bool WorkSpace::isA() const
    {
        return dynamic_cast<const T*>(this) != 0;
    }

    template<class T>
    SIREN_OUTOFLINE_TEMPLATE
    const T& WorkSpace::asA() const
    {
        const T* ptr = dynamic_cast<const T*>(this);
        
        if (not ptr)
            throw Siren::invalid_cast( String::tr(
                    "Cannot cast from a WorkSpace of type %1 to a WorkSpace "
                    "of type %2.")
                        .arg( this->what() )
                        .arg( T::typeName() ), CODELOC );
    
        return *ptr;
    }

    template<class T>
    SIREN_OUTOFLINE_TEMPLATE
    T& WorkSpace::asA()
    {
        T* ptr = dynamic_cast<T*>(this);
        
        if (not ptr)
            throw Siren::invalid_cast( String::tr(
                    "Cannot cast from a WorkSpace of type %1 to a WorkSpace "
                    "of type %2.")
                        .arg( this->what() )
                        .arg( T::typeName() ), CODELOC );
    
        return *ptr;
    }

} // end of namespace Siren

#endif // ifndef SIREN_WORKSPACE_HPP
