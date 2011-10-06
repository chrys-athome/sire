#ifndef SIREN_OBJ_HPP
#define SIREN_OBJ_HPP
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

#include "Siren/obj.h"

SIREN_BEGIN_HEADER

#ifndef SIREN_EXCLUDE_TEMPLATE_CODE

namespace Siren
{

/** Return whether the contained object can be cast to type T */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
bool Obj::isA() const
{
    return d->isA<T>();
}

/** Return the contained object cast to type T

    \throw Siren::invalid_cast
*/
template<class T>
const T& Obj::asA() const
{
    return d->asA<T>();
}


} // end of namespace Siren

#endif //SIREN_EXCLUDE_TEMPLATE_CODE

SIREN_END_HEADER

#endif // ifndef SIREN_OBJ_HPP
