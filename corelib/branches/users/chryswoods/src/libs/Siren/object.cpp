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

#include <QMutex>

#include "object.h"
#include "class.h"
#include "objref.h"
#include "objptr.hpp"
#include "logger.h"
#include "tester.h"
#include "stream.h"

#include "Siren/errors.h"

using namespace Siren;
using namespace Siren::detail;

////////
//////// Implementation of Object
////////

static const RegisterObject<Object> r_object( VIRTUAL_CLASS );

const Class* Object::class_typeinfo = 0;

/** Constructor */
Object::Object() : QSharedData()
{}

/** Copy constructor */
Object::Object(const Object &other) : QSharedData()
{}

/** Destructor */
Object::~Object()
{}

QString Object::typeName()
{
    return "Siren::Object";
}

/** Copy assignment operator */
Object& Object::operator=(const Object&)
{
    return *this;
}

/** Return a clone of this object. */
ObjRef Object::clone() const
{
    return ObjRef( this->ptr_clone() );
}

/** Save this object to the passed stream */
void Object::save(Stream &s) const
{
    s.assertIsSaving();
    const_cast<Object*>(this)->stream(s);
}

/** Load this object from the passed stream */
void Object::load(Stream &s)
{
    s.assertIsLoading();
    this->stream(s);
}

/** Stream this object to/from the passed stream
    (this loads the object if the stream is opened
     for loading, while it saves the object if the 
     stream is opened for saving) */
void Object::stream(Stream &s)
{
    s.assertVersion<Object>(1);
    s.item<Object>();
}

Q_GLOBAL_STATIC_WITH_ARGS( QMutex, objectGlobalMutex, (QMutex::Recursive) );

QMutex SIREN_EXPORT &Siren::globalRegistrationLock()
{
    QMutex *m = objectGlobalMutex();
    BOOST_ASSERT( m );
    return *m;
}

/** Return the mutex that can be used as a lock
    on all object registration */
QMutex& Object::globalLock()
{
    return globalRegistrationLock();
}

/** This function is (semi-)automatically overridden by
    Interfaces<..> to allow the full set of interfaces
    attached to a concrete object to be returned */
QStringList Object::listInterfaces()
{
    //we have no interfaces :-)
    return QStringList();
}

void Object::throwUnregisteredMetaTypeError(const QString &type_name)
{
    throw Siren::program_bug( QObject::tr(
            "Cannot create the Class object for %1 as the programmer does "
            "not appear to have created a RegisterMetaType<%1> object for "
            "this type.").arg(type_name), CODELOC );
}

/** Function called by 'Implements' to register a concrete type */
Class* Object::registerConcreteClass( const RegisterMetaType *r,
                                      const Class &base_class,
                                      const QStringList &interfaces )
{
    return new Class( r, base_class, interfaces );
}

/** Function called by 'Implements' to register a virtual type */
Class* Object::registerVirtualClass( const RegisterMetaType *r,
                                     const Class &base_class,
                                     const QStringList &interfaces )
{
    return new Class( r, base_class, interfaces );
}

/** Create and return the Class object for Siren::Object */
const Class& Object::createTypeInfo()
{
    if ( class_typeinfo == 0 )
    {
        QMutexLocker lkr( &(globalLock()) );
        
        if ( class_typeinfo == 0 )
        {
            class_typeinfo = new Class( &r_object );
        }
    }
    
    return *class_typeinfo;
}

/** This returns the default string representation of all objects */
QString Object::toString() const
{
    return QString("%1( %2 )")
                .arg( this->what() )
                .arg( toInt( this ) );
}

/** This is the default unit test - it fails as there aren't any  
    (and it is an error for the programmer to *NOT* provide any tests!).
    
    This should run all of the unit tests on this class, writing the 
    results to the passed logger, and only returning true if all
    of the tests passed */
bool Object::test(Logger &logger) const
{
    logger.write( QObject::tr(
            "Testing of %1 failed as no unit tests have been written "
            "for this class. Please ask the author to provide some tests.")
                .arg(this->what()) );
                
    return false;
}

/** This is an overloaded class provided to run the unit tests
    and write the results to the default logger */
bool Object::test() const
{
    Logger logger;
    return this->test(logger);
}

bool Object::private_implements(const QString &class_type) const
{
    return this->getClass().canCast(class_type);
}

void Object::private_assertCanCast(const QString &class_type) const
{
    this->getClass().assertCanCast(class_type);
}

//////////
////////// Implementation of None
//////////

static const RegisterObject<None> r_none;

/** Constructor */
None::None() : Implements<None,Object>()
{}

/** Copy constructor */
None::None(const None &other) : Implements<None,Object>(other)
{}

/** Destructor */
None::~None()
{}

/** Copy assignment operator */
None& None::operator=(const None &other)
{
    Object::operator=(other);
    return *this;
}

/** Comparison operator */
bool None::operator==(const None &other) const
{
    return true;
}

/** Comparison operator */
bool None::operator!=(const None &other) const
{
    return false;
}

uint None::hashCode() const
{
    return qHash( None::typeName() );
}

QString None::toString() const
{
    return "None";
}

void None::stream(Stream &s)
{
    s.assertVersion<None>(1);

    Schema schema = s.item<None>();
    Object::stream( schema.base() );
}

bool None::test(Logger &logger) const
{
    Tester tester(*this, logger);

    try
    {
        // Test 1
        {
            tester.nextTest();
            tester.expect_equal( QObject::tr("Test a clone is equal."),
                                 CODELOC,
                                 *this, this->clone() );
        }
        
        // Test 2
        {
            tester.nextTest();
            tester.expect_equal( QObject::tr("None.what() is \"Siren::None\""),
                                 CODELOC,
                                 this->what(), QLatin1String("Siren::None") );
        }
        
        // Test 3
        {
            tester.nextTest();
        
            QByteArray b;

            DataStream ds( &b, QIODevice::WriteOnly );
        
            ds << *this;
        
            DataStream ds2( b );
            
            None new_none;
            
            ds2 >> new_none;
            
            tester.expect_true( QObject::tr("Reloaded None object is equal."),
                                CODELOC, new_none.equals( *this ) );
        }
    }
    catch(const Siren::exception &e)
    {
        tester.unexpected_error(e);
    }
    catch(const std::exception &e)
    {
        tester.unexpected_error( std_exception(e) );
    }
    catch(...)
    {
        tester.unexpected_error( unknown_error(CODELOC) );
    }
    
    return tester.allPassed();
}

/** Stream function for all object */
QDataStream SIREN_EXPORT &operator<<(QDataStream &ds, const Object &object)
{
    DataStream sds(ds);
    object.save(sds);
    
    return ds;
}

/** Stream function for all object */
QDataStream SIREN_EXPORT &operator>>(QDataStream &ds, Object &object)
{
    DataStream sds(ds);
    object.load(sds);
    
    return ds;
}
