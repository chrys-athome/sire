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
#include "Siren/detail/metatype.hpp"

using namespace Siren;

// SIREN_REGISTER_TEMPLATE( Siren::T<D>, Siren::MyClass )

////////
//////// Implementation of Object
////////

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

/** Copy assignment operator */
Object& Object::operator=(const Object&)
{
    return *this;
}

/** Return the list of interfaces supported by this type.
    "0" is returned if this class does not support any interfaces */
const char** Object::ifaces()
{
    return 0;
}

/** Return a clone of this object. */
/*Obj Object::clone() const
{
    return Obj( this->ptr_clone() );
}*/

/** Save this object to the passed stream */
/*void Object::save(Stream &s) const
{
    s.assertIsSaving();
    const_cast<Object*>(this)->stream(s);
}*/

/** Load this object from the passed stream */
/*void Object::load(Stream &s)
{
    s.assertIsLoading();
    this->stream(s);
}*/

/** Stream this object to/from the passed stream
    (this loads the object if the stream is opened
     for loading, while it saves the object if the 
     stream is opened for saving) */
/*void Object::stream(Stream &s)
{
    s.assertVersion<Object>(1);
    s.item<Object>();
}*/

/** This returns the default string representation of all objects */
/*String Object::toString() const
{
    return String("%1( %2 )")
                .arg( this->what() )
                .arg( toInt( this ) );
}*/

/** This is the default unit test - it fails as there aren't any  
    (and it is an error for the programmer to *NOT* provide any tests!).
    
    This should run all of the unit tests on this class, writing the 
    results to the passed logger, and only returning true if all
    of the tests passed */
/*bool Object::test(Logger &logger) const
{
    #ifndef SIREN_DISABLE_TESTS

    logger.write( QObject::tr(
            "Testing of %1 failed as no unit tests have been written "
            "for this class. Please ask the author to provide some tests.")
                .arg(this->what()) );
                
    return false;
    
    #else
    
    return true;
    
    #endif
}*/

/** This is an overloaded class provided to run the unit tests
    and write the results to the default logger */
/*bool Object::test() const
{
    Logger logger;
    return this->test(logger);
}*/

