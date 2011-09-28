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

#include "metatype.h"
#include "qtsupport.h"

using namespace Siren;

namespace Siren
{
    namespace detail
    {

        /** Internal function used to register the ClassData object that
            contains the metadata information for a class.
    
            This is an internal function and is not part of the public API
        */
        void SIREN_EXPORT registerObject(const ClassData &object)
        {
            qDebug() << "Registering" << object.typeName();
        }
        
    } // end of namespace Siren::detail
} // end of namespace Siren
