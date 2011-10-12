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
#include "Siren/detail/qt4support.h"

#include <cstring>

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

        /** Construct a new ClassData object for the passed class,
            specifying the base class name and the list of supported
            interfaces */
        ClassData::ClassData(const char* class_type_name, 
                             const char* base_type_name,
                             const char** interfaces)
                  : type_name(class_type_name),
                    base_name(base_type_name),
                    ifaces(interfaces)
        {}
          
        /** Copy constructor */
        ClassData::ClassData(const ClassData &other)
                  : type_name(other.type_name),
                    base_name(other.base_name),
                    ifaces(other.ifaces)
        {}
    
        /** Destructor */
        ClassData::~ClassData()
        {}

        /** Comparison operator - two classes are identical if they
            have the same type names */
        bool ClassData::operator==(const ClassData &other) const
        {
            return base_name == other.base_name or
                   std::strcmp(base_name, other.base_name);
        }

        /** Return the type name of the class */
        const char* ClassData::typeName() const
        {
            return type_name;
        }
        
        /** Return the name of the base class of this type. This 
            is 0 if this class does not have a base type */
        const char* ClassData::baseTypeName() const
        {
            return base_name;
        }
    
        /** Return the array of interfaces supported by this class.
            Note that these are just the interfaces of this class, and 
            do not include the interfaces inherited from base classes.
            This returns 0 if there are no interfaces associated with
            this class */
        const char** ClassData::interfaces() const
        {
            return ifaces;
        }
        
        /** Return the number of interfaces associated with just
            this class (this does not include any base classes */
        int ClassData::nInterfaces() const
        {
            if (ifaces == 0)
                return 0;
                
            int i = 0;
            
            while (ifaces[i] != 0)
            {
                ++i;
            }
            
            return i;
        }
        
    } // end of namespace Siren::detail
} // end of namespace Siren
