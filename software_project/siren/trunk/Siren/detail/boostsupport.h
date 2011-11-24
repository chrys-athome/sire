#ifndef SIREN_BOOSTSUPPORT_H
#define SIREN_BOOSTSUPPORT_H

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

// Definition of boost classes that are required for Siren.
// These will (eventually!) be replaced with Siren's own classes,
// thereby removing the dependency on boost

#include "Siren/third_party/boost/shared_ptr.hpp"
#include "Siren/third_party/boost/weak_ptr.hpp"
#include "Siren/third_party/boost/noncopyable.hpp"
#include "Siren/third_party/boost/function.hpp"
#include "Siren/third_party/boost/bind.hpp"

#ifndef GCCXML_PARSE
   // GCCXML chokes on these files
   #include "Siren/third_party/boost/type_traits/is_pod.hpp"
   #include "Siren/third_party/boost/mpl/if.hpp"
#endif

namespace Siren
{
    template<class T>
    struct exp_shared_ptr
    {
        typedef boost::shared_ptr<T> Type;
    };

    template<class T>
    struct exp_weak_ptr
    {
        typedef boost::weak_ptr<T> Type;
    };

    using boost::noncopyable;

    using boost::bind;
    using boost::function;

} // end of namespace Siren

#endif // SIREN_QTSUPPORT_H
