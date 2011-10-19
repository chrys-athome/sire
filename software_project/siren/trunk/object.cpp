/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2009  Christopher Woods
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

#include "Siren/object.h"
#include "Siren/object.hpp"
#include "Siren/string.h"
#include "Siren/obj.h"
#include "Siren/testreport.h"
#include "Siren/siren.hpp"
#include "Siren/detail/metatype.hpp"

#include "Siren/exceptions.h"

using namespace Siren;

// SIREN_REGISTER_TEMPLATE( Siren::T<D>, Siren::MyClass )

////////
//////// Implementation of Object
////////

static const Siren::detail::VirtualClassData<Object> r_object;

Object::Object()
{}

/** Copy constructor */
Object::Object(const Object&)
{}

/** Destructor */
Object::~Object()
{}

const char* Object::typeName()
{
    return "Siren::Object";
}

/** Return the list of interfaces supported by this type.
    "0" is returned if this class does not support any interfaces */
const char** Object::ifaces()
{
    return 0;
}

/** Copy the object 'other' into this object. Note that 
    both objects must be of the same type, or an invalid
    cast exception will be raised
    
    \throw Siren::invalid_cast
*/
Object& Object::operator=(const Object &other)
{
    if (this == &other)
        return *this;
        
    if (this->what() != other.what())
        throw Siren::invalid_cast( String::tr(
                "It is not possible to copy the contents of %1, of type %2, "
                "into an object of type %3.")
                    .arg(other.toString(), 
                         String(other.what()), 
                         String(this->what())), CODELOC );
                    
    this->pvt_copy_object(other);
    
    return *this;
}

/** Compare this object with 'other'. This returns true if this
    object is of the same type and has the same contents as 'other' */
bool Object::operator==(const Object &other) const
{
    if (this == &other)
        return true;
        
    else if (this->what() != other.what())
        return false;
        
    else
        return this->pvt_compare_object(other);
}

/** Compare this object with 'other'. This returns true if this object
    is of a different type or has different contents to 'other' */
bool Object::operator!=(const Object &other) const
{
    return not Object::operator==(other);
}

/** Copy the object 'other' into this object. Note that both
    objects must be of the same type, or an invalid
    cast exception will be raised
    
    \throw Siren::invalid_cast
*/
void Object::copy(const Object &other)
{
    this->operator=(other);
}

/** Return whether or not this object equals 'other' */
bool Object::equals(const Object &other) const
{
    return this->operator==(other);
}

/** Copy the contents of other into this object */
void Object::copy_object(const Object&)
{}

/** Compare this object with 'other' */
bool Object::compare_object(const Object&) const
{
    return true;
}

/** Return the complete documentation for this class. This 
    is gathered from the documentation files that are supplied
    with the header file for this class (using documentation strings
    such as here :-)) */
String Object::docString() const
{
    return String::tr("No documentation available.");
}

/** Return the documentation for the function 'function'. This
    is gathered from the documentation files that are supplied 
    with the header file for this class (using documentation strings
    such as here :-)) */
String Object::docString(const String &function) const
{
    return String::tr("No documentation available for %1.").arg(function);
}

/** Internal function used to raise an invalid_cast exception if an attempt
    is made to cast this object to an invalid type 
    
    \throw Siren::invalid_cast
*/
void Object::throwInvalidCast(const char *type_name) const
{
    throw Siren::invalid_cast( String::tr(
            "It is not possible to cast from an object of type %1 "
            "to an object of type %2.")
                .arg(this->what()).arg(type_name), CODELOC );
}

/** Return a clone of this object. */
Obj Object::clone() const
{
    return Obj( this->ptr_clone() );
}

/** Save this object to the passed stream */
void Object::save(Stream &s) const
{
    //s.assertIsSaving();
    const_cast<Object*>(this)->stream(s);
}

/** Load this object from the passed stream */
void Object::load(Stream &s)
{
    //s.assertIsLoading();
    this->stream(s);
}

/** Stream this object to/from the passed stream
    (this loads the object if the stream is opened
     for loading, while it saves the object if the 
     stream is opened for saving) */
void Object::stream(Stream &s)
{
    throw Siren::incomplete_code( String::tr(
            "The streaming function for class %1 has not been written. "
            "Please ensure that this function has been written or else "
            "it will not be possible to save or load objects of this type!")
                .arg(this->what()), CODELOC );
}

/** This returns the default string representation of all objects */
String Object::toString() const
{
    return String(this->what());
}

/** Return a hashcode for this object - note that you must supply
    your own copy of this function or else it will not be possible
    to add objects of this type to a hash, dictionary, map or set */
uint Object::hashCode() const
{
    throw Siren::incomplete_code( String::tr(
            "The .hashCode() function for class %1 has not been written. "
            "Please ensure that this function has been written or else "
            "it will not be possible to add objects of this class to "
            "dictionaries, maps, hashes or sets.")
                .arg(this->what()), CODELOC );
                
    return 0;
}

/** This is the default unit test - it fails as there aren't any  
    (and it is an error for the programmer to *NOT* provide any tests!).
    
    This should run all of the unit tests on this class, writing the 
    results and returning them in the passed TestReport object
*/
TestReport Object::test() const throw()
{
    #ifndef SIREN_DISABLE_TESTS

    return TestReport(this->getClass());
    
    #else
    
    return TestReport();
    
    #endif
}
