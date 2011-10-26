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

/** Constructor */
WorkSpace::WorkSpace() : noncopyable()
{}

/** Destructor */
WorkSpace::~WorkSpace()
{}

/** Flush all save operations. This makes sure that all saves performed
    in this WorkSpace are shared with other workers. This blocks until 
    those saves have been successfully communicated */
void WorkSpace::flush()
{}

/** Turn on or off auto-flushing. If auto-flushing is on, then all saves
    are automatically shared with other workers immediately. This slows
    down communication, as it means that every save has to block for 
    confirmation */
void WorkSpace::setAutoFlush(bool)
{}

/** Return whether auto-flusing is turned on */
bool WorkSpace::isAutoFlush()
{
    return false;
}

/** Save the passed object using the key 'key'. This overwrites the
    current value of the object with the passed value */
void WorkSpace::save(const String &key, const Object &object)
{
    WriteLocker lkr(&lk);
    objs.insert(key, object);
}

/** Load and return the current value of the object with key 'key'. This
    returns Siren::None if there is no value associated with this key,
    or if this key does not exist. */
Obj WorkSpace::load(const String &key)
{
    ReadLocker lkr(&lk);
    return objs.value(key, None());
}

/** Save the current value of the object with key 'key' into the array
    with index 'idx' 
        
    \throw Siren::invalid_index
    \throw Siren::invalid_key
*/
void WorkSpace::save(const String &key, int idx, const Object &object)
{
    WriteLocker lkr(&lk);
    
    Hash<String,Vector<Obj>::Type>::iterator it = obj_arrays.find(key);
    
    if (it == obj_arrays.end())
        throw Siren::invalid_key( String::tr(
                "There is no array with key %1 in the workspace!")
                    .arg(key), CODELOC );
    
    idx = wrap_index(idx, it->count(), CODELOC);
    
    it->data()[idx] = object;
}

/** Load the current value of the object with key 'key' from index 'idx'
    of the array
    
    \throw Siren::invalid_index
    \throw Siren::invalid_key
*/
Obj WorkSpace::load(const String &key, int idx)
{
    ReadLocker lkr(&lk);
    
    Hash<String,Vector<Obj>::Type>::const_iterator it = obj_arrays.constFind(key);
    
    if (it == obj_arrays.constEnd())
        throw Siren::invalid_key( String::tr(
                "There is no array with key %1 in the workspace!")
                    .arg(key), CODELOC );

    idx = wrap_index(idx, it->count(), CODELOC);
    
    return it->constData()[idx];
}

/** Save the values of the passed array of objects with the key 'key'
    into the array from index 'idx'
    
    \throw Siren::invalid_index
    \throw Siren::invalid_key
*/
void WorkSpace::save(const String &key, int idx, const List<Obj>::Type &objects)
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
List<Obj>::Type WorkSpace::load(const String &key, int idx, int count)
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
Obj WorkSpace::receive(int worker_id, const String &key)
{
    return None();
}

/** Send the passed object to the worker with ID 'worker_id' with the key 'key'.
    This function returns immediately, returning a receipt that can be used
    to find out when the worker received the message (or that contains the 
    exception detailing whether the worker thread doesn't exist, or if it went
    offline before receiving the message) */
Receipt WorkSpace::send(int worker_id, const String &key, const Object &object)
{
    return Receipt();
}

/** Broadcast the passed object to all of the workers who are currently in 
    the group. This function returns immediately, returning a receipt that
    can be used to track which workers were sent the message, and when the
    messages were received and read, and any exceptions that were generated
    during the sending of the messages, e.g. if a worker died or went offline */
Receipts WorkSpace::broadcast(const String &key, const Object &object)
{
    return Receipts();
}

/** Internal function called by derived classes to supply an object
    that has been received from the specified worker, with specified key */
void WorkSpace::receivedFrom(int worker_id, const String &key, const Obj &object)
{
    WriteLocker lkr(&lk);
    received_objs[worker_id].insert(key, object);
}

/** Return whether or not we have a message object from the worker with ID
    'worker_id' and with key 'key' */
bool WorkSpace::hasObjectFrom(int worker_id, const String &key)
{
    ReadLocker lkr(&lk);
    return received_objs.value(worker_id).contains(key);
}

/** Wait for up to 'ms' milliseconds for a message from the worker with ID
    'worker_id' with key 'key'. This returns whether or not a message was
    received */
bool WorkSpace::waitForObjectFrom(int worker_id, const String &key, int ms)
{
    return false;
}
