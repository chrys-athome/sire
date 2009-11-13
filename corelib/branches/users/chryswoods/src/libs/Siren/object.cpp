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
#include "logger.h"
#include "tester.h"
#include "xmlstream.h"

#include "Siren/errors.h"

using namespace Siren;
using namespace Siren::detail;

////////
//////// Implementation of Object
////////

static const RegisterMetaType<Object> r_object( VIRTUAL_CLASS,
                                                18292577219026618092UL, 1 );

/** Serialise to a binary datastream - this will share-stream objects
    (so repeated shared objects will be streamed just once, with 
    references to later copies used) */
DataStream SIREN_EXPORT &operator<<(DataStream &ds, const Object &object)
{
    object.save( ds );
    return ds;
}

/** Read this object from a binary datastream */
DataStream SIREN_EXPORT &operator>>(DataStream &ds, Object &object)
{
    object.load( ds );
    return ds;
}
                                  
/** Serialise to a binary QDataStream */
QDataStream SIREN_EXPORT &operator<<(QDataStream &ds, const Object &object)
{
    DataStream sds(ds);
    
    sds << object;
    
    return ds;
}

/** Extract from a binary QDataStream */
QDataStream SIREN_EXPORT &operator>>(QDataStream &ds, Object &object)
{
    DataStream sds(ds);
    
    sds >> object;
    
    return ds;
}

/** Serialise to XML */
XMLStream SIREN_EXPORT &operator<<(XMLStream &xml, const Object &object)
{
    QDomNode root_node = xml.createChild();

    object.save(root_node);
    
    xml.closeChild();
    
    return xml;
}

/** Extract from an XML document */
XMLStream SIREN_EXPORT &operator>>(XMLStream &xml, Object &object)
{
    QDomNode root_node = xml.createChild();

    object.load(root_node);
    
    xml.closeChild();
    
    return xml;
}

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

Q_GLOBAL_STATIC_WITH_ARGS( QMutex, objectGlobalMutex, (QMutex::Recursive) );

/** Return the mutex that can be used as a lock
    on all object registration */
QMutex& Object::globalLock()
{
    QMutex *m = objectGlobalMutex();
    
    BOOST_ASSERT( m );
    
    return *m;
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
Class* Object::registerConcreteClass( const RegisterMetaTypeBase *r,
                                      const Class &base_class,
                                      const QStringList &interfaces )
{
    return new Class( r, base_class, interfaces, true );
}

/** Function called by 'Implements' to register a virtual type */
Class* Object::registerVirtualClass( const RegisterMetaTypeBase *r,
                                     const Class &base_class,
                                     const QStringList &interfaces )
{
    return new Class( r, base_class, interfaces, false );
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
    return this->test( Logger::getDefault() );
}

/** Reimplement this function to load this object from 
    a XML DOM - the calls of derived classes should
    proceed in sequence, with Object::load() called last */
void Object::load(const QDomNode &dom)
{
    throw Siren::incomplete_code( QObject::tr(
                "Loading objects from XML has yet to be implemented..."), CODELOC );
}

/** Reimplement this function to save this object to
    a XML DOM - the calls of derived classes should
    proceed in sequence, with Object::save() called last */
void Object::save(QDomNode &dom) const
{
    throw Siren::incomplete_code( QObject::tr(
                "Saving objects to XML has yet to be implemented..."), CODELOC );
}

/** Reimplement this function to load this object from
    a binary datastream - the calls of derived classes
    should proceed in sequence, with Object::load() called last */
void Object::load(DataStream &ds)
{
    writeHeader(ds, r_object);
}

/** Reimplement this function to save this object to 
    a binary datastream - this calls of derived classes
    should proceed in sequence, with Object::save() called last */
void Object::save(DataStream &ds) const
{
    VERSION_ID v = readHeader(ds, r_object);
    
    if (v != r_object.version())
        throw Siren::version_error( v, r_object, CODELOC );
}

//////////
////////// Implementation of None
//////////

static const RegisterMetaType<None> r_none( 13600120236270837364UL, 1 );

/** Constructor */
None::None() : ConcreteObject<None,Object>()
{}

/** Copy constructor */
None::None(const None &other) : ConcreteObject<None,Object>(other)
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
    return Object::operator==(other);
}

/** Comparison operator */
bool None::operator!=(const None &other) const
{
    return Object::operator!=(other);
}

HASH_CODE None::hashCode() const
{
    return r_none.hashBase();
}

QString None::toString() const
{
    return "None";
}

void None::save(DataStream &ds)
{
    writeHeader(ds, r_none);
    Object::load(ds);
}

void None::load(DataStream &ds)
{
    VERSION_ID v = readHeader(ds, r_none);
    
    if (v != r_none.version())
        throw version_error(v, r_none, CODELOC);
}

bool None::test(Logger &logger) const
{
    Tester tester(logger, *this);

    try
    {
        // Test 1
        {
            tester.nextTest();
            tester.expect_equal( QObject::tr("Test a clone is equal."),
                                 *this, this->clone(),
                                 logger );
        }
        
        // Test 2
        {
            tester.nextTest();
            tester.expect_equal( QObject::tr("None.what() is \"Siren::None\""),
                                 this->what(), "Siren::None",
                                 logger );
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
                                new_none.equals( *this ) );
        }
    }
    catch(const Siren::exception &e)
    {
        tester.unexpected_error(e);
    }
    catch(const std::exception &e)
    {
        tester.unexpected_error( std_exception(e, CODELOC) );
    }
    catch(...)
    {
        tester.unexpected_error( unknown_error(CODELOC) );
    }
    
    return tester.allPassed();
}