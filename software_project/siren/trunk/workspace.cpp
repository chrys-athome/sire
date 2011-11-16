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

#include "Siren/workspace.h"
#include "Siren/obj.h"
#include "Siren/none.h"
#include "Siren/exceptions.h"

using namespace Siren;

//////////
////////// Implementation of WSpace
//////////

/** Constructor */
WSpace::WSpace() : noncopyable()
{}

/** Construct, passing in the Worker ID of this WorkSpace */
WSpace::WSpace(int id) : noncopyable()
{}

/** Destructor */
WSpace::~WSpace()
{}

/** Return the lock that can be used to serialise access to the data in 
    this workspace */
ReadWriteLock& WSpace::lock()
{
    return lk;
}

/** Flush all save operations. This makes sure that all saves performed
    in this WorkSpace are shared with other workers. This blocks until 
    those saves have been successfully communicated */
void WSpace::flush()
{}

/** Turn on or off auto-flushing. If auto-flushing is on, then all saves
    are automatically shared with other workers immediately. This slows
    down communication, as it means that every save has to block for 
    confirmation */
void WSpace::setAutoFlush(bool)
{}

/** Return whether auto-flusing is turned on */
bool WSpace::isAutoFlush()
{
    return false;
}

/** Return the size of the array associated with the key 'key' */
int WSpace::arraySize(const String &key)
{
    ReadLocker lkr(&lk);
    
    Hash<String,Vector<Obj>::Type>::const_iterator it = obj_arrays.constFind(key);
    
    if (it == obj_arrays.constEnd())
    {
        return 1;
    }
    else
    {
        return it->count();
    }
}

/** Save the passed object using the key 'key'. This overwrites the
    current value of the object with the passed value */
void WSpace::save(const String &key, const Object &object)
{
    this->save(key, 0, object);
}

/** Load and return the current value of the object with key 'key'. This
    returns Siren::None if there is no value associated with this key,
    or if this key does not exist. */
Obj WSpace::load(const String &key)
{
    return this->load(key, 0);
}

/** Save the current value of the object with key 'key' into the array
    with index 'idx' 
        
    \throw Siren::invalid_index
    \throw Siren::invalid_key
*/
void WSpace::save(const String &key, int idx, const Object &object)
{
    WriteLocker lkr(&lk);
    
    Hash<String,Vector<Obj>::Type>::iterator it = obj_arrays.find(key);
    
    if (it == obj_arrays.end())
    {
        if (idx == 0)
        {
            obj_arrays.insert(key, Vector<Obj>::Type());
            it = obj_arrays.find(key);
        }
        else
            throw Siren::invalid_index( String::tr(
                    "There is no space for an object with key %1, index %2.")
                        .arg(key).arg(idx), CODELOC );
    }
    
    idx = wrap_index(idx, it->count(), CODELOC);
    
    it->data()[idx] = object;
}

/** Load the current value of the object with key 'key' from index 'idx'
    of the array
    
    \throw Siren::invalid_index
    \throw Siren::invalid_key
*/
Obj WSpace::load(const String &key, int idx)
{
    ReadLocker lkr(&lk);
    
    Hash<String,Vector<Obj>::Type>::const_iterator it = obj_arrays.constFind(key);
    
    if (it == obj_arrays.constEnd())
    {
        if (idx == 0)
            return None();
    
        throw Siren::invalid_key( String::tr(
                "There is no array with key %1 in the workspace!")
                    .arg(key), CODELOC );
    }

    idx = wrap_index(idx, it->count(), CODELOC);
    
    return it->constData()[idx];
}

/** Save the values of the passed array of objects with the key 'key'
    into the array from index 'idx'
    
    \throw Siren::invalid_index
    \throw Siren::invalid_key
*/
void WSpace::save(const String &key, int idx, const List<Obj>::Type &objects)
{
    WriteLocker lkr(&lk);
    
    Hash<String,Vector<Obj>::Type>::iterator it = obj_arrays.find(key);
    
    if (it == obj_arrays.end())
        throw Siren::invalid_key( String::tr(
                "There is no array with key %1 in the workspace!")
                    .arg(key), CODELOC );
    
    idx = wrap_index(idx, it->count(), CODELOC);

    if (idx + objects.count() >= it->count())
        throw Siren::invalid_index( String::tr(
                "You cannot copy an array %1 objects into the array for key %2 "
                "at index %3, as the maximum number of objects that can be "
                "fitted into this array is %4.")
                    .arg(objects.count())
                    .arg(key)
                    .arg(idx)
                    .arg(it->count()), CODELOC );
    
    int i = 0;
    
    for (List<Obj>::const_iterator it2 = objects.constBegin();
         it2 != objects.constEnd();
         ++it2)
    {
        it->data()[idx+i] = *it2;
        ++i;
    }
}

/** Return the set of 'count' objects from the array with key 'key'
    from index 'idx'
    
    \throw Siren::invalid_index
    \throw Siren::invalid_key
*/
List<Obj>::Type WSpace::load(const String &key, int idx, int count)
{
    ReadLocker lkr(&lk);
    
    Hash<String,Vector<Obj>::Type>::const_iterator it = obj_arrays.constFind(key);
    
    if (it == obj_arrays.constEnd())
        throw Siren::invalid_key( String::tr(
                "There is no array with key %1 in the workspace!")
                    .arg(key), CODELOC );
    
    idx = wrap_index(idx, it->count(), CODELOC);

    if (idx + count >= it->count())
        throw Siren::invalid_index( String::tr(
                "You cannot get %1 objects from the array for key %2 "
                "at index %3, as the maximum number of objects that can be "
                "got into this array is %4.")
                    .arg(count)
                    .arg(key)
                    .arg(idx)
                    .arg(it->count()), CODELOC );
    
    List<Obj>::Type objs;
    
    for (int i=0; i<count; ++i)
    {
        objs.append( it->constData()[idx+i] );
    }
    
    return objs;
}

/** Block until we have received an object from the worker with ID 'worker_id',
    with key 'key'. This will raise an error if the worker with ID 'worker_id'
    either does not exist, or if it goes offline while we are waiting
    
    \throw Siren::comms_error
*/
Obj WSpace::receive(int worker_id, const String &key)
{
    return None();
}

/** Send the passed object to the worker with ID 'worker_id' with the key 'key'.
    This function returns immediately, returning a receipt that can be used
    to find out when the worker received the message (or that contains the 
    exception detailing whether the worker thread doesn't exist, or if it went
    offline before receiving the message) */
Receipt WSpace::send(int worker_id, const String &key, const Object &object)
{
    return Receipt();
}

/** Broadcast the passed object to all of the workers who are currently in 
    the group. This function returns immediately, returning a receipt that
    can be used to track which workers were sent the message, and when the
    messages were received and read, and any exceptions that were generated
    during the sending of the messages, e.g. if a worker died or went offline */
Receipts WSpace::broadcast(const String &key, const Object &object)
{
    return Receipts();
}

/** Internal function called by derived classes to supply an object
    that has been received from the specified worker, with specified key */
void WSpace::receivedFrom(int worker_id, const String &key, const Obj &object)
{
    WriteLocker lkr(&lk);
    received_objs[worker_id].insert(key, object);
}

/** Return whether or not we have a message object from the worker with ID
    'worker_id' and with key 'key' */
bool WSpace::hasObjectFrom(int worker_id, const String &key)
{
    ReadLocker lkr(&lk);
    return received_objs.value(worker_id).contains(key);
}

/** Wait for up to 'ms' milliseconds for a message from the worker with ID
    'worker_id' with key 'key'. This returns whether or not a message was
    received */
bool WSpace::waitForObjectFrom(int worker_id, const String &key, int ms)
{
    return false;
}

/////////
///////// Implementation of WorkSpace
/////////

/** Create an empty WorkSpace of type WSpace */
WorkSpace::WorkSpace() : d(new WSpace())
{}

/** Construct from the passed pointer. Note that this will take over
    ownership of the data and will delete the workspace when it is no
    longer needed */
WorkSpace::WorkSpace(WSpace *workspace) : d(workspace)
{}

/** Construct an empty WorkSpace of type WSpace with worker_id 'worker_id' */
WorkSpace::WorkSpace(int worker_id) : d(new WSpace(worker_id))
{}

/** Copy constructor */
WorkSpace::WorkSpace(const WorkSpace &other) : d(other.d)
{}

/** Destructor - this will destroy the contained WSpace if this is the last reference */
WorkSpace::~WorkSpace()
{}

/** Copy assignment operator */
WorkSpace& WorkSpace::operator=(const WorkSpace &other)
{
    d = other.d;
    return *this;
}

/** Comparison operator */
bool WorkSpace::operator==(const WorkSpace &other) const
{
    return d.get() == other.d.get();
}

/** Comparison operator */
bool WorkSpace::operator!=(const WorkSpace &other) const
{
    return not operator==(other);
}

/** Return the type of the contained WSpace */
const char* WorkSpace::what() const
{
    return d->what();
}


/** Flush all save operations. This makes sure that all saves performed
    in this WorkSpace are shared with other workers. This blocks until 
    those saves have been successfully communicated */
void WorkSpace::flush()
{
    d->flush();
}

/** Turn on or off auto-flushing. If auto-flushing is on, then all saves
    are automatically shared with other workers immediately. This slows
    down communication, as it means that every save has to block for 
    confirmation */
void WorkSpace::setAutoFlush(bool on)
{
    d->setAutoFlush(on);
}

/** Return whether auto-flusing is turned on */
bool WorkSpace::isAutoFlush()
{
    return d->isAutoFlush();
}

/** Return the size of the array associated with the key 'key' */
int WorkSpace::arraySize(const String &key)
{
    return d->arraySize(key);
}

/** Save the passed object using the key 'key'. This overwrites the
    current value of the object with the passed value */
void WorkSpace::save(const String &key, const Object &object)
{
    d->save(key,object);
}

/** Load and return the current value of the object with key 'key'. This
    returns Siren::None if there is no value associated with this key,
    or if this key does not exist. */
Obj WorkSpace::load(const String &key)
{
    return d->load(key);
}

/** Save the current value of the object with key 'key' into the array
    with index 'idx' 
        
    \throw Siren::invalid_index
    \throw Siren::invalid_key
*/
void WorkSpace::save(const String &key, int idx, const Object &object)
{
    d->save(key, idx, object);
}

/** Load the current value of the object with key 'key' from index 'idx'
    of the array
    
    \throw Siren::invalid_index
    \throw Siren::invalid_key
*/
Obj WorkSpace::load(const String &key, int idx)
{
    return d->load(key,idx);
}

/** Save the values of the passed array of objects with the key 'key'
    into the array from index 'idx'
    
    \throw Siren::invalid_index
    \throw Siren::invalid_key
*/
void WorkSpace::save(const String &key, int idx, const List<Obj>::Type &objects)
{
    d->save(key, idx, objects);
}

/** Return the set of 'count' objects from the array with key 'key'
    from index 'idx'
    
    \throw Siren::invalid_index
    \throw Siren::invalid_key
*/
List<Obj>::Type WorkSpace::load(const String &key, int idx, int count)
{
    return d->load(key,idx,count);
}

/** Block until we have received an object from the worker with ID 'worker_id',
    with key 'key'. This will raise an error if the worker with ID 'worker_id'
    either does not exist, or if it goes offline while we are waiting
    
    \throw Siren::comms_error
*/
Obj WorkSpace::receive(int worker_id, const String &key)
{
    return d->receive(worker_id,key);
}

/** Send the passed object to the worker with ID 'worker_id' with the key 'key'.
    This function returns immediately, returning a receipt that can be used
    to find out when the worker received the message (or that contains the 
    exception detailing whether the worker thread doesn't exist, or if it went
    offline before receiving the message) */
Receipt WorkSpace::send(int worker_id, const String &key, const Object &object)
{
    return d->send(worker_id,key,object);
}

/** Broadcast the passed object to all of the workers who are currently in 
    the group. This function returns immediately, returning a receipt that
    can be used to track which workers were sent the message, and when the
    messages were received and read, and any exceptions that were generated
    during the sending of the messages, e.g. if a worker died or went offline */
Receipts WorkSpace::broadcast(const String &key, const Object &object)
{
    return d->broadcast(key, object);
}

/** Return whether or not we have a message object from the worker with ID
    'worker_id' and with key 'key' */
bool WorkSpace::hasObjectFrom(int worker_id, const String &key)
{
    return d->hasObjectFrom(worker_id,key);
}

/** Wait for up to 'ms' milliseconds for a message from the worker with ID
    'worker_id' with key 'key'. This returns whether or not a message was
    received */
bool WorkSpace::waitForObjectFrom(int worker_id, const String &key, int ms)
{
    return d->waitForObjectFrom(worker_id,key,ms);
}
