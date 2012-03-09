#ifndef CONSPIRE_CONSPIRE_H
#define CONSPIRE_CONSPIRE_H
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

#include "Siren/siren.h"
#include "Siren/object.h"
#include "Siren/obj.h"
#include "Siren/none.h"

#include "Conspire/qtsupport.h"

//Forward declaration of key classes
namespace Conspire
{
    namespace Boost
    {
        using namespace Siren::Boost;
    }

    using Siren::Object;
    using Siren::None;
    using Siren::Obj;

    using Siren::tr;

    class Option;
    class Options;
    
    class Value;
    class Values;

    #define CONSPIRE_EXPORT SIREN_EXPORT
    #define CONSPIRE_BEGIN_HEADER SIREN_BEGIN_HEADER
    #define CONSPIRE_END_HEADER SIREN_END_HEADER
    #define CONSPIRE_INLINE_TEMPLATE SIREN_INLINE_TEMPLATE
    #define CONSPIRE_OUTOFLINE_TEMPLATE SIREN_OUTOFLINE_TEMPLATE

    #ifdef SIREN_INSTANTIATE_TEMPLATES
        #define CONSPIRE_INSTANTIATE_TEMPLATES 1
    #endif


} // end of namespace Conspire

#endif // CONSPIRE_CONSPIRE_H
