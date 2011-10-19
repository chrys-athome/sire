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
#include <boost/static_assert.hpp>

//// This file contains the set of C preprocessor macros
//// that are used and are global for Siren

/** Used to signify the beginning of a header file */
#define SIREN_BEGIN_HEADER

/** Used to signify the end of a header file */
#define SIREN_END_HEADER

/** Used to signify that the symbol for the class or function should be 
    visible outside of the Siren library. */
#define SIREN_EXPORT Q_DECL_EXPORT

/** Macro used to assert that the passed argument is true at compile time */
#define SIREN_STATIC_ASSERT BOOST_STATIC_ASSERT

/** Use to signify that the following template function should be
    compiled inline */
#define SIREN_INLINE_TEMPLATE

/** Use to signify that the following template function should be 
    compiled out of line */
#define SIREN_OUTOFLINE_TEMPLATE

/** Used to signify that the passed class should be exposed
    as part of a public API to a scripting interface */
#define SIREN_EXPOSE_CLASS(classname)

/** Used to signify that the passed exception class should be
    exposed as part of a public API to a scripting interface */
#define SIREN_EXPOSE_EXCEPTION(classname)

#define PASTE_TOKENS2(arg1,arg2) arg1 ## arg2
#define PASTE_TOKENS(arg1,arg2) PASTE_TOKENS2(arg1,arg2)

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
        
/** Used to define, in the definition of Siren Object classes,
    the functions that are common to all Siren objects */
#define SIREN_CLASS(classname, baseclass) \
    public: \
        friend class detail::ConcreteClassData< classname >; \
        typedef baseclass super; \
        static const char* typeName(); \
        virtual const char* what() const; \
        static Class typeClass(); \
        Class getClass() const; \
    protected: \
        void pvt_copy_object(const Siren::Object &other); \
        bool pvt_compare_object(const Siren::Object &other) const; \
    private: \
        classname * ptr_clone() const;
    
/** Used to define, in the definition of Siren Object classes,
    the functions that are common to all Siren objects */
#define SIREN_VIRTUAL_CLASS(classname, baseclass) \
    public: \
        friend class detail::VirtualClassData< classname >; \
        typedef baseclass super; \
        static const char* typeName(); \
        static Class typeClass(); \
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
