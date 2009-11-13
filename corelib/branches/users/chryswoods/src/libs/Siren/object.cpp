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

/** Copy assignment operator */
Object& Object::operator=(const Object&)
{
    return *this;
}

/** Comparison operator - this is blocked by default - use .equals()
    to do a value comparison of two different objects of unknown type */
bool Object::operator==(const Object&) const
{
    return true;
}

/** Comparison operator - this is blocked by default - use .equals()
    to do a value comparison of two different objects of unknown type */
bool Object::operator!=(const Object&) const
{
    return false;
}

/** Return a copy of this object. This only performs
    a shallow copy if this type implements the Shared 
    interface */
ObjRef Object::copy() const
{
    return ObjRef( this->clone() );
}
    
/** Return a copy of this object. This performs a deep 
    copy if this type implements the Shared interface */
ObjRef Object::deepCopy() const
{
    //we need to detach this object from shared storage
    //if it implements the 'Shared' interface
    //
    //(note that any object that can hold Shared objects
    // must inherit from the Shared interface!)
    const Shared *shared_iface = dynamic_cast<const Shared*>(this);
    
    if (shared_iface)
    {
        shared_iface->detach();
    }
    
    return ObjRef( this->clone() );
}

/** Deep-copy 'other' into this object */
void Object::deepCopy(const Object &other)
{
    this->copy( other.deepCopy() );
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
    LoggerPtr logger = Logger::getDefault();

    return this->test( logger.edit() );
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

////////
//////// Implementation of ObjPtrBase
////////

/** Serialise to a binary datastream */
DataStream SIREN_EXPORT &operator<<(DataStream &ds, const ObjPtrBase &objptr)
{
    ds << objptr.ptr;
    return ds;
}

/** Extract from a binary datastream */
DataStream SIREN_EXPORT &operator>>(DataStream &ds, ObjPtrBase &objptr)
{
    ds >> objptr.ptr;
    return ds;
}

/** Serialise to a binary datastream */
QDataStream SIREN_EXPORT &operator<<(QDataStream &ds, const ObjPtrBase &objptr)
{
    DataStream sds(ds);
    sds << objptr;
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREN_EXPORT &operator>>(QDataStream &ds, ObjPtrBase &objptr)
{
    DataStream sds(ds);
    sds >> objptr;
    
    return ds;
}

/** Serialise to XML */
XMLStream SIREN_EXPORT &operator<<(XMLStream &xml, const ObjPtrBase &objptr)
{
    xml << objptr.ptr;
    return xml;
}

/** Extract from XML */
XMLStream SIREN_EXPORT &operator>>(XMLStream &xml, ObjPtrBase &objptr)
{
    xml >> objptr.ptr;
    return xml;
}

/** Construct the pointer to the passed object */
ObjPtrBase::ObjPtrBase(const Object &object) : ptr(object)
{}

/** Construct the pointer to the passed object
     - this takes over ownership of 'object' */
ObjPtrBase::ObjPtrBase(Object *object) : ptr(object)
{}

/** Copy constructor */
ObjPtrBase::ObjPtrBase(const ObjPtrBase &other) : ptr(other.ptr)
{}

/** Destructor */
ObjPtrBase::~ObjPtrBase()
{}

/** Copy assignment operator */
ObjPtrBase& ObjPtrBase::operator=(const ObjPtrBase &other)
{
    ptr = other.ptr;
    return *this;
}

/** Comparison operator - this just performs pointer based
    comparison (so compares memory addresses) */
bool ObjPtrBase::operator==(const Object &object) const
{
    return ptr.constData() == &object;
}

/** Comparison operator - this just performs pointer based
    comparison (so compares memory addresses) */
bool ObjPtrBase::operator!=(const Object &object) const
{
    return not ObjPtrBase::operator==(object);
}

/** Comparison operator - this just performs pointer based
    comparison (so compares memory addresses) */
bool ObjPtrBase::operator==(const ObjPtrBase &other) const
{
    return ptr.constData() == other.ptr.constData();
}

/** Comparison operator - this just performs pointer based
    comparison (so compares memory addresses) */
bool ObjPtrBase::operator!=(const ObjPtrBase &other) const
{
    return not ObjPtrBase::operator==(other);
}

/** Detach this pointer from shared storage */
void ObjPtrBase::detach()
{
    ptr.detach();
}

/** Return whether or not this is the only pointer
    to the object */
bool ObjPtrBase::unique() const
{
    return ptr.unique();
}

/** Return whether or not this pointer is null */
bool ObjPtrBase::isNull() const
{
    return ptr.constData() == 0;
}

Q_GLOBAL_STATIC_WITH_ARGS( SharedPolyPointer<Object>, getNone, (None()) );

/** Allow automatic casting to an Object - this returns None
    if this is a null pointer */
ObjPtrBase::operator const Object&() const
{
    if (ptr.constData() != 0)
        return *ptr;
    else
        return *( getNone()->constData() );
}

/** Assert that this is not a null pointer 

    \throw Siren::nullptr_error
*/
void ObjPtrBase::assertNotNull() const
{
    if (ptr.constData() == 0)
        throw Siren::nullptr_error( QObject::tr(
            "You cannot dereference a null pointer."), CODELOC );
}

/** Return a read-only reference to this object

    \throw Siren::nullptr_error
*/
const Object& ObjPtrBase::read() const
{
    assertNotNull();
    return *ptr;
}

/** Return the raw pointer */
const Object* ObjPtrBase::data() const
{
    return ptr.data();
}

/** Return the raw pointer */
const Object* ObjPtrBase::constData() const
{
    return ptr.constData();
}

/** Return the raw pointer */
Object* ObjPtrBase::data()
{
    return ptr.data();
}

/** Return an editable reference to the object

    \throw Siren::nullptr_error
*/
Object& ObjPtrBase::write()
{
    assertNotNull();
    return *ptr;
}

/** Return an editable reference to the object

    \throw Siren::nullptr_error
*/
Object& ObjPtrBase::edit()
{
    return this->write();
}

void ObjPtrBase::throwCastingError(QString got_type, QString want_type)
{
    throw Siren::invalid_cast( QObject::tr(
            "You cannot case from an Object of type %1 to an Object of type %2.")
                .arg(got_type, want_type), CODELOC );
}

////////
//////// Implementation of GlobalObjPtrBase
////////

/** Serialise to a binary datastream */
DataStream SIREN_EXPORT &operator<<(DataStream &ds, const GlobalObjPtrBase &objptr)
{
    ds << objptr.ptr;
    return ds;
}

/** Extract from a binary datastream */
DataStream SIREN_EXPORT &operator>>(DataStream &ds, GlobalObjPtrBase &objptr)
{
    ds >> objptr.ptr;
    return ds;
}

/** Serialise to a binary datastream */
QDataStream SIREN_EXPORT &operator<<(QDataStream &ds, const GlobalObjPtrBase &objptr)
{
    DataStream sds(ds);
    sds << objptr;
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREN_EXPORT &operator>>(QDataStream &ds, GlobalObjPtrBase &objptr)
{
    DataStream sds(ds);
    sds >> objptr;
    
    return ds;
}

/** Serialise to XML */
XMLStream SIREN_EXPORT &operator<<(XMLStream &xml, const GlobalObjPtrBase &objptr)
{
    xml << objptr.ptr;
    return xml;
}

/** Extract from XML */
XMLStream SIREN_EXPORT &operator>>(XMLStream &xml, GlobalObjPtrBase &objptr)
{
    xml >> objptr.ptr;
    return xml;
}

/** Construct to hold a pointer to 'object' */
GlobalObjPtrBase::GlobalObjPtrBase(const Object &object)
                 : ptr(object)
{}

/** Construct to hold a pointer to 'object' - this takes over
    ownership of the pointer */
GlobalObjPtrBase::GlobalObjPtrBase(Object *object)
                 : ptr(object)
{}

/** Copy constructor */
GlobalObjPtrBase::GlobalObjPtrBase(const GlobalObjPtrBase &other)
                 : ptr(other.ptr)
{}

/** Destructor */
GlobalObjPtrBase::~GlobalObjPtrBase()
{}

/** Copy assignment operator */
GlobalObjPtrBase& GlobalObjPtrBase::operator=(const GlobalObjPtrBase &other)
{
    ptr = other.ptr;
    return *this;
}

/** Comparison operator */
bool GlobalObjPtrBase::operator==(const GlobalObjPtrBase &other) const
{
    return ptr.constData() == other.ptr.constData();
}

/** Comparison operator */
bool GlobalObjPtrBase::operator!=(const GlobalObjPtrBase &other) const
{
    return not GlobalObjPtrBase::operator==(other);
}

/** Comparison operator */
bool GlobalObjPtrBase::operator==(const Object &other) const
{
    return ptr.constData() == &other;
}

/** Comparison operator */
bool GlobalObjPtrBase::operator!=(const Object &other) const
{
    return not GlobalObjPtrBase::operator==(other);
}

/** Is this a unique pointer to the object? */
bool GlobalObjPtrBase::unique() const
{
    return ptr.unique();
}

/** Allow automatic casting to a Object */
GlobalObjPtrBase::operator const Object&() const
{
    if (ptr.constData() != 0)
        return *ptr;
    else
        return *( getNone()->constData() );
}

/** Return whether or not this pointer is null */
bool GlobalObjPtrBase::isNull() const
{
    return ptr.constData() == 0;
}

/** Assert that this is not a null pointer 

    \throw Siren::nullptr_error
*/
void GlobalObjPtrBase::assertNotNull() const
{
    if (ptr.constData() == 0)
        throw Siren::nullptr_error( QObject::tr(
            "You cannot dereference a null pointer."), CODELOC );
}

/** Return a read-only reference to this object

    \throw Siren::nullptr_error
*/
const Object& GlobalObjPtrBase::read() const
{
    assertNotNull();
    return *ptr;
}

/** Return the raw pointer */
const Object* GlobalObjPtrBase::data() const
{
    return ptr.data();
}

/** Return the raw pointer */
const Object* GlobalObjPtrBase::constData() const
{
    return ptr.constData();
}

void GlobalObjPtrBase::throwCastingError(QString got_type, QString want_type)
{
    throw Siren::invalid_cast( QObject::tr(
            "You cannot case from an Object of type %1 to an Object of type %2.")
                .arg(got_type, want_type), CODELOC );
}

////////
//////// Implementation of ObjRef
////////

/** Serialise to a binary datastream */
DataStream SIREN_EXPORT &operator<<(DataStream &ds, const ObjRef &obj)
{
    ds << obj.d;
    return ds;
}

/** Extract from a binary datastream */
DataStream SIREN_EXPORT &operator>>(DataStream &ds, ObjRef &obj)
{
    ds >> obj.d;
    return ds;
}

/** Serialise to a binary datastream */
QDataStream SIREN_EXPORT &operator<<(QDataStream &ds, const ObjRef &obj)
{
    DataStream sds(ds);
    sds << obj;
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREN_EXPORT &operator>>(QDataStream &ds, ObjRef &obj)
{
    DataStream sds(ds);
    sds >> obj;
    
    return ds;
}

/** Serialise to XML */
XMLStream SIREN_EXPORT &operator<<(XMLStream &xml, const ObjRef &obj)
{
    xml << obj.d;
    return xml;
}

/** Extract from XML */
XMLStream SIREN_EXPORT &operator>>(XMLStream &xml, ObjRef &obj)
{
    xml >> obj.d;
    return xml;
}

/** Construct a null reference - this points to 'None' */
ObjRef::ObjRef() : d( *(getNone()) )
{}

/** Construct a reference to the object 'obj' - this creates
    a reference to a shallow copy of 'obj' */
ObjRef::ObjRef(const Object &obj) : d(obj)
{}

/** Construct a reference to the object pointed to by 'obj'. This
    takes over ownership of this pointer and can delete the object
    whenever it likes */
ObjRef::ObjRef(Object *obj) : d(obj)
{
    if (obj == 0)
        d = *(getNone());
}

/** Copy constructor */
ObjRef::ObjRef(const ObjRef &other) : d(other.d)
{}

/** Destructor */
ObjRef::~ObjRef()
{}

/** Copy assignment operator */
ObjRef& ObjRef::operator=(const ObjRef &other)
{
    d = other.d;
    return *this;
}

/** Copy assignment operator */
ObjRef& ObjRef::operator=(const Object &other)
{
    d = other;
    return *this;
}

/** Comparison operator */
bool ObjRef::operator==(const ObjRef &other) const
{
    return d.constData() == other.d.constData() or
           d->equals(other);
}

/** Comparison operator */
bool ObjRef::operator!=(const ObjRef &other) const
{
    return not ObjRef::operator==(other);
}

/** Comparison operator */
bool ObjRef::operator==(const Object &other) const
{
    return d->equals(other);
}

/** Comparison operator */
bool ObjRef::operator!=(const Object &other) const
{
    return not ObjRef::operator==(other);
}

/** Return the Class type for the object */
Class ObjRef::getClass() const
{
    return d->getClass();
}

/** Return the type name for the object */
QString ObjRef::what() const
{
    return d->what();
}

/** Return a shallow copy of the object */
ObjRef ObjRef::copy() const
{
    return d->copy();
}

/** Return a deep copy of the object */
ObjRef ObjRef::deepCopy() const
{
    return d->deepCopy();
}

/** Return a string representation of this object */
QString ObjRef::toString() const
{
    return d->toString();
}

/** Unit test the class type of this object, returning 
    whether or not all of the tests passed. The output of
    the tests will be printed to the default Logger */
bool ObjRef::test() const
{
    return d->test();
}

/** Unit test the class type of this object, returning
    whether or not all of the tsts passed. The output
    will be printed to the passed logger */
bool ObjRef::test(Logger &logger) const
{
    return d->test(logger);
}

/** Copy the passed object 'other' - note that this
    copies into the object referred to by this reference */
void ObjRef::copy(const Object &other)
{
    d->copy(other);
}

/** Deep copy the passed object 'other' - note that this
    copies into the object referred to by this reference */
void ObjRef::deepCopy(const Object &other)
{
    d->deepCopy(other);
}

/** Return whether or not this object equals 'other' */
bool ObjRef::equals(const Object &other) const
{
    return d->equals(other);
}

/** Return the hash code for this object */
HASH_CODE ObjRef::hashCode() const
{
    return d->hashCode();
}

/** Allow automatic casting to an Object */
ObjRef::operator const Object&() const
{
    return *d;
}

//////////
////////// Implementation of None
//////////

static const RegisterMetaType<None> r_none( 13600120236270837364UL, 1 );

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

void None::save(DataStream &ds) const
{
    writeHeader(ds, r_none);
    Object::save(ds);
}

void None::load(DataStream &ds)
{
    VERSION_ID v = readHeader(ds, r_none);
    
    if (v != r_none.version())
        throw version_error(v, r_none, CODELOC);

    Object::load(ds);
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
                                 *this, this->copy() );
        }
        
        // Test 2
        {
            tester.nextTest();
            tester.expect_equal( QObject::tr("None.what() is \"Siren::None\""),
                                 CODELOC,
                                 this->what(), "Siren::None" );
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

/////////
///////// Instantiate template classes
/////////

template class ObjPtr<Object>;
template class GlobalObjPtr<Object>;
