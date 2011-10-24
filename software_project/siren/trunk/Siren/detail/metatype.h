#ifndef SIREN_METATYPE_H
#define SIREN_METATYPE_H

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

#include "Siren/macros.h"
#include "Siren/detail/qt4support.h"

namespace Siren
{
    class Object;
    class Class;
    class String;
    class StringList;
}

namespace Siren
{
    namespace detail
    {
        class ClassData;
    
        /** This is an internal class used to provide the registry 
            of all Siren types held in the program 
            
            @author Christopher Woods
        */
        class ClassRegistry
        {
        public:
            static Class getClass(const char *type_name);
            static Class getClass(const String &type_name);
            
            static StringList registeredClasses();
            
        protected:
            friend class ClassData;
            static void registerClass(const ClassData &data);
            static void unregisterClass(const ClassData &data);
        
        private:
            ClassRegistry();
            ~ClassRegistry();
            
            static void createSingleton();
            
            //static AtomicPointer<ClassRegistry>::Type singleton;
            static ClassRegistry *singleton;

            ReadWriteLock *lock;
            List<const ClassData*>::Type *registry;
            
        }; // end of class ClassRegistry
    
        /** This is the base class of the objects that are used
            internally by Siren to hold basic metadata about Siren
            objects.
            
            This is an internal class and is not part of the public API
            
            @author Christopher Woods
        */
        class SIREN_EXPORT ClassData
        {
        public:
            ClassData(const char* class_type_name, 
                      const char* base_type_name,
                      const char** interfaces,
                      int version);
        
            virtual ~ClassData();
        
            virtual Object* createObject() const=0;
        
            bool operator==(const ClassData &other) const;
            bool operator!=(const ClassData &other) const;
        
            bool hasSuper() const;
            
            bool isClass(const char *type_name) const;
            bool isClass(const ClassData &other) const;
        
            const String& typeName() const;
            const String& baseTypeName() const;
        
            int version() const;
        
            const StringList& interfaces() const;
            int nInterfaces() const;
        
            virtual bool isConcrete() const=0;
        
        private:
            /** Noncopyable class */
            ClassData(const ClassData&){}
            ClassData& operator=(const ClassData&){ return *this;}

            /** The fully qualified name of the class */
            const char *type_name;
            
            /** The fully qualified name of any base type
                (is equal to 0 if there is no base type */
            const char *base_name;
            
            /** The list of interfaces supported by this class.
                This is a 0-terminated array, so 0 indicates that
                there are no interfaces supported by this class */
            const char **ifaces;
            
            /** This is a cache of the string name of the class */
            AtomicPointer<String>::Type type_name_string;
            
            /** This is a cache of the base name of the class */
            AtomicPointer<String>::Type base_name_string;
            
            /** This is a cache of the interfaces supported by this 
                class - this is 0 if there are no interfaces */
            AtomicPointer<StringList>::Type ifaces_list;
            
            /** The version number of the data in this class */
            int version_number;
            
        }; // end of class ClassData

        /** This template class is used to hold metadata
            about concrete (non-virtual) Siren objects. This
            class is created automatically via the SIREN_REGISTER_CLASS
            macro.
            
            This is an internal class and not part of the public API
            
            @author Christopher Woods
        */
        template<class T>
        class SIREN_EXPORT ConcreteClassData : public ClassData
        {
        public:
            ConcreteClassData();
            ~ConcreteClassData();
        
            Object* createObject() const;
            
            bool isConcrete() const;
            
        }; // end of class ConcreteClassData<T>

        /** This template class is used to hold metadata
            about virtual Siren objects. This class is
            created automatically via the SIREN_REGISTER_VIRTUAL_CLASS
            macro.
            
            This is an internal class and not part of the public API
            
            @author Christopher Woods
        */
        template<class T>
        class SIREN_EXPORT VirtualClassData : public ClassData
        {
        public:
            VirtualClassData();
            ~VirtualClassData();
        
            Object* createObject() const;
            
            bool isConcrete() const;
        
        }; // end of class VirtualClassData<T>

    } // end of namespace Siren::detail

} // end of namespace Siren

#endif // end of ifndef SIREN_METATYPE_H
