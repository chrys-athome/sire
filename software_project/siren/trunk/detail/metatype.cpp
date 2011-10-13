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

#include "Siren/stringlist.h"
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
                    ifaces(interfaces), 
                    type_name_string(0),
                    base_name_string(0),
                    ifaces_list(0)
        {}
          
        /** Copy constructor */
        ClassData::ClassData(const ClassData &other)
                  : type_name(other.type_name),
                    base_name(other.base_name),
                    ifaces(other.ifaces),
                    type_name_string(0),
                    base_name_string(0),
                    ifaces_list(0)
        {}
    
        /** Destructor */
        ClassData::~ClassData()
        {
            String *ptr = type_name_string;
            delete ptr;
            
            ptr = base_name_string;
            delete ptr;
            
            StringList *ptr2 = ifaces_list;
            delete ptr2;
        }

        /** Comparison operator - two classes are identical if they
            have the same type names */
        bool ClassData::operator==(const ClassData &other) const
        {
            return base_name == other.base_name or
                   std::strcmp(base_name, other.base_name);
        }

        /** Return whether or not this class has a super type (base class) */
        bool ClassData::hasSuper() const
        {
            return base_name != 0;
        }

        /** Return the type name of the class */
        const String& ClassData::typeName() const
        {
            if (type_name_string == 0)
            {
                String *tname = new String( type_name );
                
                while (type_name_string == 0)
                {
                    const_cast<ClassData*>(this)->type_name_string
                            .testAndSetAcquire(0, tname);
                }
                
                if (type_name_string != tname)
                    delete tname;
            }
            
            return *type_name_string;
        }
        
        /** Return the name of the base class of this type. This 
            is 0 if this class does not have a base type */
        const String& ClassData::baseTypeName() const
        {
            static String null_string;
            
            if (base_name == 0)
                return null_string;
                
            else if (base_name_string == 0)
            {
                String *bname = new String( base_name );
                
                while (base_name_string == 0)
                {
                    const_cast<ClassData*>(this)->base_name_string
                            .testAndSetAcquire(0, bname);
                }
                
                if (bname != base_name_string)
                    delete bname;
            }

            return *base_name_string;
        }
    
        /** Return the array of interfaces supported by this class.
            Note that these are just the interfaces of this class, and 
            do not include the interfaces inherited from base classes. */
        const StringList& ClassData::interfaces() const
        {
            static StringList null_list;
            
            if (ifaces == 0)
                return null_list;
                
            else
            {
                if (ifaces_list == 0)
                {
                    //create a new ifaces_list to hold the const char* 
                    //interface names as String names
                    List<String>::Type ilist;
                    
                    int i=0;
                    while (ifaces[i] != 0)
                    {
                        ilist.append( String(ifaces[i]) );
                        ++i;
                    }
                    
                    if (ifaces_list == 0)
                    {
                        StringList *il = new StringList(ilist);
                        
                        while (ifaces_list == 0)
                        {
                            const_cast<ClassData*>(this)->ifaces_list
                                .testAndSetAcquire(0, il);
                        }
                        
                        if (ifaces_list != il)
                            delete il;
                    }
                }
                
                return *ifaces_list;
            }
        }
        
        /** Return the number of interfaces associated with just
            this class (this does not include any base classes */
        int ClassData::nInterfaces() const
        {
            return this->interfaces().count();
        }
        
    } // end of namespace Siren::detail
} // end of namespace Siren
