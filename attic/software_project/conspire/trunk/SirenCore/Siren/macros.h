#ifndef SIREN_MACROS_H
#define SIREN_MACROS_H

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

#include <QtGlobal>
#include "Siren/third_party/boost/static_assert.hpp"

//// This file contains the set of C preprocessor macros
//// that are used and are global for Siren

/** Used to signify the beginning of a header file */
#define SIREN_BEGIN_HEADER QT_BEGIN_HEADER

/** Used to signify the end of a header file */
#define SIREN_END_HEADER QT_END_HEADER

/** The size of a pointer in bytes */
#define SIREN_POINTER_SIZE QT_POINTER_SIZE

/** Used to signify that we are compiling 32bit code */
#if SIREN_POINTER_SIZE == 8
  #define SIREN_64BIT 1
#elif SIREN_POINTER_SIZE == 4
  #define SIREN_32BIT 1
#else
  #warning Cannot work out if this is a 32bit or 64bit compile!
#endif


/** Macro used to assert that the passed argument is true at compile time */
#define SIREN_STATIC_ASSERT BOOST_STATIC_ASSERT

/** Use to signify that the following template function should be
    compiled inline */
#define SIREN_INLINE_TEMPLATE inline

/** Use to signify that the following template function should be 
    compiled out of line */
#define SIREN_OUTOFLINE_TEMPLATE

/** Used to signify that the passed class should be exposed
    as part of a public API to a scripting interface */
#define SIREN_EXPOSE_CLASS(classname)

/** Used to signify that the passed exception class should be
    exposed as part of a public API to a scripting interface */
#define SIREN_EXPOSE_EXCEPTION(classname)

/** Used to signify that the passed template should be exposed using
    the specified alias */
#define SIREN_EXPOSE_ALIAS(classname, alias)

#define PASTE_TOKENS2(arg1,arg2) arg1 ## arg2
#define PASTE_TOKENS(arg1,arg2) PASTE_TOKENS2(arg1,arg2)

#ifdef WIN32
#define WINBLOODY32
#endif

#ifdef WINBLOODY32
   #ifdef Siren_EXPORTS
      #define SIREN_EXPORT Q_DECL_EXPORT
      #define SIREN_IMPORT
   #else
      #define SIREN_EXPORT
      #define SIREN_IMPORT
   #endif
   #ifndef Q_MOC_RUN
       #define not !
       #define and &&
       #define or ||
   #endif
   #define strtok_r strtok_s
   #define PRT_SLEEP(s) Sleep((int)((s)*1000.0))
   #define strdup _strdup
#else
   #define PRT_SLEEP(s) sleep(s)
   #define SIREN_EXPORT
   #define SIREN_IMPORT
#endif

/** Used to register a new concrete Siren Object class */
#define REGISTER_SIREN_CLASS(classname)  \
    const char* classname::typeName(){ return #classname; } \
    const char* classname::what() const{ return classname::typeName(); } \
    classname * classname::ptr_clone() const{ return new classname(*this); } \
    static const Siren::detail::ConcreteClassData<classname> \
                                                PASTE_TOKENS(register_object_,__LINE__); \
    Siren::Class classname::typeClass() { \
        static Siren::Class r_class(#classname); \
        return r_class; \
    } \
    Siren::Class classname::getClass() const { \
        return classname::typeClass(); \
    } \
    void classname::pvt_copy_object(const Siren::Object &other) \
    { \
        classname::copy_object( *((const classname *) (&other)) ); \
    } \
    bool classname::pvt_compare_object(const Siren::Object &other) const \
    { \
        return classname::compare_object( *((const classname *) (&other)) ); \
    }

/** Used to register a new concrete Siren Object template class */
#define REGISTER_SIREN_TEMPLATE_CLASS(classname)  \
    template<> \
    const char* classname::typeName(){ return #classname; } \
    template<> \
    const char* classname::what() const{ return classname::typeName(); } \
    template<> \
    classname * classname::ptr_clone() const{ return new classname(*this); } \
    static const Siren::detail::ConcreteClassData<classname> \
                                                PASTE_TOKENS(register_object_,__LINE__); \
    template<> \
    Siren::Class classname::typeClass() { \
        static Siren::Class r_class(#classname); \
        return r_class; \
    } \
    template<> \
    Siren::Class classname::getClass() const { \
        return classname::typeClass(); \
    } \
    template<> \
    void classname::pvt_copy_object(const Siren::Object &other) \
    { \
        classname::copy_object( *((const classname *) (&other)) ); \
    } \
    template<> \
    bool classname::pvt_compare_object(const Siren::Object &other) const \
    { \
        return classname::compare_object( *((const classname *) (&other)) ); \
    }
        
/** Used to define, in the definition of Siren Object classes,
    the functions that are common to all Siren objects */
#define SIREN_CLASS(classname, baseclass, version) \
    public: \
        friend class Siren::detail::ConcreteClassData< classname >; \
        typedef baseclass super; \
        static const char* typeName(); \
        virtual const char* what() const; \
        static Siren::Class typeClass(); \
        Siren::Class getClass() const; \
        static int typeClassVersion() { return version; } \
        int getClassVersion() const { return version; } \
        classname& operator=(const classname &other) { \
                   classname::copy_object(other); return *this; } \
        bool operator==(const classname &other) const { \
                   return classname::compare_object(other); } \
        bool operator!=(const classname &other) const { \
                   return !(classname::operator==(other)); } \
    protected: \
        void pvt_copy_object(const Siren::Object &other); \
        bool pvt_compare_object(const Siren::Object &other) const; \
    private: \
        classname * ptr_clone() const;
    
/** Used to define, in the definition of Siren Object classes,
    the functions that are common to all Siren objects */
#define SIREN_VIRTUAL_CLASS(classname, baseclass, version) \
    public: \
        friend class Siren::detail::VirtualClassData< classname >; \
        typedef baseclass super; \
        static const char* typeName(); \
        static Siren::Class typeClass(); \
        static int typeClassVersion() { return version; } \
        classname& operator=(const classname &other) { \
                   Siren::Object::operator=(other); return *this; } \
        bool operator==(const classname &other) const { \
                   return Siren::Object::operator==(other); } \
        bool operator!=(const classname &other) const { \
                   return !(Object::operator==(other)); } \
    private:
    
/** Use to register a new virtual Siren Object class */
#define REGISTER_SIREN_VIRTUAL_CLASS(classname)  \
    const char* classname::typeName(){ return #classname; } \
    static const Siren::detail::VirtualClassData<classname> \
                                        register_object_##__LINE__; \
    Siren::Class classname::typeClass() { \
        static Siren::Class r_class(#classname); \
        return r_class; \
    } \

#endif // end of ifndef SIREN_MACROS_H
