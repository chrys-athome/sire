#ifndef SIREN_OBJECT_H
#define SIREN_OBJECT_H

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

#include "Siren/siren.h"

SIREN_BEGIN_HEADER

namespace Siren
{

    /** This is the base class of all Siren virtual objects.
        
        All Siren objects are inherited from either this class
        (if they have virtual functions or wish to use metatyping
         or serialization), or, if they are basic, they are inherited
        from Siren::Primitive
        
        A Siren object provides the single root of all non-primitive,
        non-handle objects, together with a .getClass() function that returns
        a Siren::Class object that contains information about the type.
        
        In addition, all Siren::Objects support in-place unit testing,
        with each object containing a .test() method, e.g.
        
        obj.test();
        
        or
        
        obj.test( logger );
        
        This returns true if all tests passed. This allows unit tests
        to be written with the object, in the same implementation
        file as the object (and to have full access to the private
        data of the object).
        
        Object derived classes have three constraints;
        
          (1) They must have a well-defined state. This means that their
              state must be savable, and can be restored at a later point.
              This means that temporal data cannot be placed into an object
              (as the state of a clock cannot be saved, then restored, as then
               its time would be wrong! - however, if the current time was not
               part of the Clocks state, then it could be saved and restored)
        
          (2) Their state must not rely on shared data. This is because the  
              state of an Object can only be changed via its public interface.
              If it could contain shared data, then its state could be changed
              by any other object that contains a reference to that shared data.
              If you want to hold shared data, then you need to use the 
              Handle classes. (note that an Object can hold a Handle to 
              some shared data if, and only if, this shared data is not
              part of the state of the object, e.g. if the handle was to a
              Node one which the object will be computed, or to a Logger
              to which output should be directed.
              
          (3) All Object derived classes *MUST* be able to be saved and restored
              to binary and XML.
        
        Finally, a full metatype system is used, so that objects can 
        be constructed, cloned, queried, and serialized and deserialized 
        to/from binary streams and saved/restored to XML without any 
        knowledge of the type. This is achieved using;

        obj.what();    // return the type name of this object

        obj.getClass();   // Return information about the type of this object

        obj.getClass().superClass();  // Return information about the superclass type
        
        obj.getClass().UID();  // Return the unique ID number for this class type
        
        obj.getClass().version();  // Return the version number for the data in 
                                   // this class (used for serialisation)
        
        ObjRef new_obj = obj.copy();  // clone the current object
        
        ObjRef new_obj = obj.getClass().create();  // create a new object
        
        obj.isA<T>();   // return whether 'obj' is of type 'T'
        
        obj.asA<T>();   // return 'obj' cast as an object of type 'T'
        
        DataStream ds;
        
        ds << obj;   // save 'obj' to a binary stream
        
        ObjRef new_obj;
        ds >> new_obj;   // read 'new_obj' from a binary stream
        
        XMLStream xds;
        
        xds << obj;   // save 'obj' to an XML stream
        
        xds >> new_obj;  // read 'new_obj' from an XML stream
        
        You may not multiply inherit from Object. There must be a single
        inheritance path back to Object. The only exception are that
        you can multiply inherit Object with types derived from 
        Siren::Interface. Siren::Interface provides the basis of
        interface types, which are pure virtual base classes with
        no member data (similar to Interfaces in Java).
        
        Siren::Object is a virtual class. You must inherit from
        Implements<D,B>, where D is your new concrete class and 
        B is the class from which it inherits.
        
        For example, if NewObj is a concrete class derived from Object,
        the it should inherit from Implements<NewObj,Object>. 
        The Implements class does all of the hard work of 
        filling in the virtual functions of Object.
        
        If you are deriving a new virtual class from Object, then 
        you must inherit from Extends<D,B>, e.g. if NewObj is a 
        virtual class derived from Object, then it should inherit
        from Extends<NewObj,Object>.
        
        Each Siren::Object is immutable (unchangeable). If you want
        to create a mutable object, you *must* inherit from the
        Mutable interface. This will provide you with functions
        that will allow you to save and restore the state of
        the mutable object (e.g. to protect an invariant if
        an exception is thrown).
        
        For example, let imagine Foo is a concrete class derived
        from Bar, and Foo implements the Mutable interface. Also, 
        Bar is a virtual class derived from Object. The inheritance
        hierarchy would look like this;
        
        class Bar : public Extends<Bar,Object>{ ... };
        
        class Foo : public Implements<Foo,Bar>, public Interfaces<Foo,Mutable> { ... };
        
        Each Siren::Object must also be non-shared. This means that
        as long as access to the object is protected by a mutex,
        then the object is thread-safe. This also means that the 
        object cannot be changed 'behind-the-back', e.g. a shared
        object can be changed using any of the references/handles to that object.

        If you want a shared object, then you will need to
        create a class that is based on Siren::Handle.
        
        @author Christopher Woods
    */
    class SIREN_EXPORT Object
    {
    public:
        Object();
        Object(const Object &other);
        
        virtual ~Object();

        ///////////////////////////////////////////////////////////
        static Class typeClass();
        virtual Class getClass() const=0;

        static const char* typeName();
        virtual const char* what() const=0;

        virtual void copy(const Object &other)=0;
        virtual bool equals(const Object &other) const=0;

        virtual Object& operator=(const Object &other)=0;
        virtual bool operator==(const Object &other) const=0;
        virtual bool operator!=(const Object &other) const=0;
        ///////////////////////////////////////////////////////////

        Obj clone() const;
        
        void save(Stream &s) const;
        void load(Stream &s);

        bool test() const;

        template<class T>
        bool isA() const;

        template<class T>
        const T& asA() const;

        //////////////////////////////////////////
        //                                      //
        // Functions that should be overridden  //
        // by all Siren::Object derived types   //
        //                                      //
        //////////////////////////////////////////

        virtual String toString() const;

        virtual bool test(Logger &logger) const;

        virtual String docString() const;
        virtual String docString(const String &function) const;
        
        virtual uint hashCode() const;
        
        virtual void stream(Stream &s);

    protected:
        static const char** ifaces();
    
        /** Return a clone of this object. */
        virtual const Object* ptr_clone() const=0;
    };

} // end of namespace Siren

SIREN_END_HEADER

#endif
