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

#include "handle.h"
#include "hanref.h"
#include "class.h"
#include "logger.h"
#include "waitcondition.h"

#include "Siren/errors.h"

using namespace Siren;
using namespace Siren::detail;

/////////
///////// Implementation of Handle
/////////

static const RegisterHandle<Handle> r_handle( VIRTUAL_CLASS );

/** Return the mutex that can be used as a lock
    on all registration */
Mutex& Handle::globalLock()
{
    return globalRegistrationLock();
}

/** This function is (semi-)automatically overridden by
    Interfaces<..> to allow the full set of interfaces
    attached to a concrete Handle to be returned */
QStringList Handle::listInterfaces()
{
    //we have no interfaces :-)
    return QStringList();
}

void Handle::throwUnregisteredMetaTypeError(const QString &type_name)
{
    throw Siren::program_bug( QObject::tr(
            "Cannot create the Class object for %1 as the programmer does "
            "not appear to have created a RegisterMetaType<%1> object for "
            "this type.").arg(type_name), CODELOC );
}

/** Function called by 'ImplementsHandle' to register a concrete type */
Class* Handle::registerConcreteClass( const RegisterMetaType *r,
                                      const Class &base_class,
                                      const QStringList &interfaces )
{
    return new Class( r, base_class, interfaces );
}

/** Function called by 'ExtendsHandle' to register a virtual type */
Class* Handle::registerVirtualClass( const RegisterMetaType *r,
                                     const Class &base_class,
                                     const QStringList &interfaces )
{
    return new Class( r, base_class, interfaces );
}

/** Create and return the Class object for Siren::Handle */
const Class& Handle::createTypeInfo()
{
    if ( class_typeinfo == 0 )
    {
        MutexLocker lkr( &(globalLock()) );
        
        if ( class_typeinfo == 0 )
        {
            class_typeinfo = new Class( &r_handle );
        }
    }
    
    return *class_typeinfo;
}

const Class* Handle::class_typeinfo = 0;

/** Null constructor */
Handle::Handle()
{}

/** Copy constructor */
Handle::Handle(const Handle &other) : resource_lock(other.resource_lock)
{}

/** Destructor */
Handle::~Handle()
{}

QString Handle::typeName()
{
    return "Siren::Handle";
}

/** Copy assignment operator */
Handle& Handle::operator=(const Handle &other)
{
    if (this != &other)
        resource_lock = other.resource_lock;

    return *this;
}

/** Comparison operator */
bool Handle::operator==(const Handle &other) const
{
    return resource_lock.get() == other.resource_lock.get();
}

/** Comparison operator */
bool Handle::operator!=(const Handle &other) const
{
    return not Handle::operator==(other);
}

/** Internal function used in the construct of Handles<T> to 
    tell Handle to create the resource lock */
void Handle::setValidResource()
{
    // MUST NOT be a recursive Mutex as otherwise .sleep() won't work!
    resource_lock.reset( new Mutex() );
}

/** Internal function used by WeakHandle to neuter (invalidate)
    the shared pointers in this Handle */
void Handle::neuter()
{
    resource_lock.reset();
}

/** Internal function used to check if this class implements 'class_type' */
bool Handle::private_implements(const QString &class_type) const
{
    return this->getClass().canCast(class_type);
}

/** Internal function used to assert that this class can be 
    cast to 'class_type' 
    
    \throw Siren::invalid_cast
*/
void Handle::private_assertCanCast(const QString &class_type) const
{
    this->getClass().assertCanCast(class_type);
}

/** Return whether or not this handle is null (not connected
    to the shared resource) */
bool Handle::isNull() const
{
    return resource_lock.get() == 0;
}

/** Assert that this handle is not null */
void Handle::assertNotNull() const
{
    if (this->isNull())
        throw Siren::nullptr_error( QObject::tr(
                "The Handle %1 is null!").arg(this->what()), CODELOC );
}

/** Return a copy of this handle - note this copies the
    handle, not the object being handled! */
HanRef Handle::clone() const
{
    return HanRef( this->ptr_clone() );
}

/** Return a string representation of this handle */
QString Handle::toString() const
{
    if (this->isNull())
        return QString("%1()").arg(this->what());

    else
        return QObject::tr("%1( Handling %2 )")
                .arg(this->what()).arg( toInt(resource_lock.get()) );
}

/** This is the default unit test - it fails as there aren't any  
    (and it is an error for the programmer to *NOT* provide any tests!).
    
    This should run all of the unit tests on this class, writing the 
    results to the passed logger, and only returning true if all
    of the tests passed */
bool Handle::test(Logger &logger) const
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
}

/** This is an overloaded class provided to run the unit tests
    and write the results to the default logger */
bool Handle::test() const
{
    Logger logger;
    return this->test(logger);
}

/** Internal function used to return the lock */
Mutex* Handle::resourceLock()
{
    return resource_lock.get();
}

/** Internal function used to return the lock */
Mutex* Handle::resourceLock() const
{
    return const_cast<Mutex*>(resource_lock.get());
}

void Handle::dropResource()
{
    resource_lock.reset();
}

/** Lock this handle */
void Handle::lock() const
{
    if (not this->isNull())
        const_cast<Mutex*>(resource_lock.get())->lock();
}

/** Unlock this handle */
void Handle::unlock() const
{
    if (not this->isNull())
        const_cast<Mutex*>(resource_lock.get())->unlock();
}

/** Try to lock this handle. This returns whether or not 
    this succeeds - if it does, then you must unlock the 
    handle once you have finished with it */
bool Handle::tryLock() const
{
    if (not this->isNull())
        return const_cast<Mutex*>(resource_lock.get())->tryLock();
    else
        return true;
}

/** Try to lock this handle, waiting a maximum of 'ms' milliseconds
    to get the lock. This returns whether or not 
    this succeeds - if it does, then you must unlock the 
    handle once you have finished with it */
bool Handle::tryLock(int ms) const
{
    if (not this->isNull())
        return const_cast<Mutex*>(resource_lock.get())->tryLock(ms);
    else
        return true;
}

/** Sleep on this handle, using the passed WaitCondition. Note that
    you must hold the lock on this resource */
void Handle::sleep(WaitCondition &waiter) const
{
    if (not this->isNull())
        waiter.wait( const_cast<Mutex*>(resource_lock.get()) );
}

/** Sleep on this handle, using the passed WaitCondition, waiting
    for at most 'ms' milliseconds to be woken up. This returns
    whether or not this was woken up. Note that you must hold
    the lock on this resource */
bool Handle::sleep(WaitCondition &waiter, int ms) const
{
    if (not this->isNull())
        return waiter.wait( const_cast<Mutex*>(resource_lock.get()), ms);
    else
        return true;
}

/////////
///////// Implementation of HandleLocker;
/////////

/** Null constructor */
HandleLocker::HandleLocker() : resource_mutex(0), is_locked(false)
{}

/** Construct from the passed Handle, in so doing locking that handle */
HandleLocker::HandleLocker(const Handle &handle) 
             : resource_mutex( const_cast<Mutex*>(handle.resource_lock.get()) ),
               is_locked(false)
{
    if (resource_mutex)
    {
        resource_mutex->lock();
        is_locked = true;
    }
}

/** Destructor - this will release the lock */
HandleLocker::~HandleLocker()
{
    if (is_locked)
        resource_mutex->unlock();
}

/** Manually release the lock */
void HandleLocker::unlock()
{
    if (is_locked)
    {
        resource_mutex->unlock();
        is_locked = false;
    }
}

/** Manually re-lock the lock */
void HandleLocker::relock()
{
    if (not is_locked)
    {
        if (resource_mutex)
        {
            resource_mutex->lock();
            is_locked = true;
        }
    }
}

/////////
///////// Implementation of WeakHandle
/////////

/** Null constructor */
WeakHandle::WeakHandle() : weak_handle(0)
{}

/** Construct a weak handle to 'handle' */
WeakHandle::WeakHandle(const Handle &handle) : weak_handle(0)
{
    neutered_handle.reset( handle.ptr_clone() );

    weak_handle = neutered_handle->createWeakHandle();
    weak_lock = neutered_handle->resource_lock;
    neutered_handle->neuter();
}

/** Copy constructor */
WeakHandle::WeakHandle(const WeakHandle &other) : weak_handle(0)
{
    if (other.weak_handle)
    {
        neutered_handle = other.neutered_handle;
        weak_handle = neutered_handle->copyWeakHandle(weak_handle);
        weak_lock = other.weak_lock;
    }
}

/** Destructor */
WeakHandle::~WeakHandle()
{
    if (weak_handle)
        neutered_handle->deleteWeakHandle(weak_handle);
}

/** Copy assignment operator */
WeakHandle& WeakHandle::operator=(const WeakHandle &other)
{
    if (this != &other)
    {
        if (weak_handle)
        {
            neutered_handle->deleteWeakHandle(weak_handle);
            weak_handle = 0;
        }
        
        neutered_handle = other.neutered_handle;
        weak_lock = other.weak_lock;
        weak_handle  = neutered_handle->copyWeakHandle(other.weak_handle);
    }
    
    return *this;
}

/** Comparison operator */
bool WeakHandle::operator==(const WeakHandle &other) const
{
    return weak_lock.lock().get() == other.weak_lock.lock().get();
}

/** Comparison operator */
bool WeakHandle::operator!=(const WeakHandle &other) const
{
    return not WeakHandle::operator==(other);
}

/** Return whether or not this weak handle has expired */
bool WeakHandle::expired() const
{
    return weak_lock.expired();
}

/** Return a handle to the resource being locked. This will return 
    a null reference if the resource is no longer available */
HanRef WeakHandle::lock() const
{
    if (not weak_handle)
        return HanRef();

    WeakHandle *nonconst_this = const_cast<WeakHandle*>(this);

    if (not nonconst_this->neutered_handle->restoreFromWeakHandle(weak_handle))
    {
        nonconst_this->neutered_handle->deleteWeakHandle(weak_handle);
        nonconst_this->weak_handle = 0;
        nonconst_this->neutered_handle.reset();
        nonconst_this->weak_lock.reset();
        
        return HanRef();
    }
    
    nonconst_this->neutered_handle->resource_lock = weak_lock.lock();
    
    if (nonconst_this->neutered_handle->resource_lock.get() == 0)
        nonconst_this->neutered_handle->setValidResource();
        
    HanRef new_handle = nonconst_this->neutered_handle->clone();
    
    //re-neuter the handle
    nonconst_this->neutered_handle->neuter();
    
    return new_handle;
}
