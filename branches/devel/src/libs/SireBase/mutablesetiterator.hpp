#ifndef SIREBASE_MUTABLESETITERATOR_HPP
#define SIREBASE_MUTABLESETITERATOR_HPP

#include "set.hpp"

SIRE_BEGIN_HEADER

namespace SireBase
{

/**
  * This class provides a Java-style mutable iterator over a Set<K,V>.
  *
  * This is very similar in behaviour to the QMutableListIterator class.
  *
  * Only one mutable iterator can be active on a given Set at any time. 
  * Furthermore, no changes should be done directly to the Set while the 
  * iterator is active (as opposed to through the iterator), since this 
  * could invalidate the iterator and lead to undefined behavior.  
  *
  * @author Christopher Woods
  */
template<class K, class V>
class MutableSetIterator
{
public:
    MutableSetIterator();
    MutableSetIterator(Set<K,V> &set);
    MutableSetIterator(const MutableSetIterator<K,V> &other);
    
    ~MutableSetIterator();
    
    MutableSetIterator<K,V>& operator=(const MutableSetIterator<K,V> &other);
    
    const V& value() const;
    K key() const;
    
    bool isValid() const;
    
    V& value();
    void setValue(const V &obj);
    
    void remove();
    
    const V& next();
    const V& peekNext() const;
    
    const V& previous();
    const V& peekPrevious() const;
    
    const V& jumpTo(int i);
    const V& peekAt(int i) const;
    
    int index() const;
    
    bool hasNext() const;
    bool hasPrevious() const;
    
    bool find(const V &obj);
    bool findKey(const K &key);

    void toBack();
    void toFront();

    MutableSetIterator<K,V>& operator=( Set<K,V> &set );

private:

    void checkIndexStatus();
    void indexChanged();

    /** Static, empty set used to create a null mutable iterator */
    static Set<K,V> null_set;

    /** Reference to the set being modified */
    Set<K,V> &set;
    
    /** Non-const pointer to the array containing the object data */
    V *array;
    
    /** Non-const pointer to the current object */
    V *current;
    
    /** The current size of the set */
    int sz;
    
    /** The current index */
    int i;
    
    /** The current key */
    K currentkey;
};

/** Global null Set, used by the default constructor to allow holding a reference */
template<class K, class V>
Set<K,V> MutableSetIterator<K,V>::null_set;

/** Construct a null iterator */
template<class K, class V>
SIRE_OUTOFLINE_TEMPLATE
MutableSetIterator<K,V>::MutableSetIterator()
                        : set(null_set), array(0), current(0), sz(0), i(-1)
{}
    
/** Construct a mutable iterator to operator on set 's' */
template<class K, class V>
SIRE_OUTOFLINE_TEMPLATE
MutableSetIterator<K,V>::MutableSetIterator(Set<K,V> &s)
                        : set(s), array(set.objs.data()), current(0), sz(set.size()), i(-1)
{}
        
/** Copy constructor - necessary to allow an iterator to be passed back from a function */
template<class K, class V>
SIRE_OUTOFLINE_TEMPLATE
MutableSetIterator<K,V>::MutableSetIterator(const MutableSetIterator<K,V> &other)
                        : set(null_set), array(0), current(0), sz(0), i(-1)
{
    *this = other;
}
        
/** Return whether or not this iterator is working on a valid Set, and is at 
    a valid position */
template<class K, class V>
SIRE_INLINE_TEMPLATE
bool MutableSetIterator<K,V>::isValid() const
{
    return array != 0 and i >= 0 and i < sz;
}
        
/** This function checks to see if editing the last object caused its index to change.
    If it did, then the Set has to be reconstructed (which can be slow!). This is 
    why it is quite important to not change the index of an object when you edit it! */
template<class K, class V>
SIRE_INLINE_TEMPLATE
void MutableSetIterator<K,V>::checkIndexStatus()
{
    //is the current item valid? and has its key changed?
    if ( isValid() and currentkey != Set<K,V>::indexOf(*current) )
        indexChanged();
}

/** The index of the object has changed - this means that we have to reindex the set! */
template<class K, class V>
SIRE_OUTOFLINE_TEMPLATE
void MutableSetIterator<K,V>::indexChanged()
{
    K newkey = Set<K,V>::indexOf(*current);
    
    //is there already an item with this index? If so, we will need to remove
    //that item from the Set...
    if (set.has_key(newkey))
        set.remove_key(newkey);
        
    //now we need to update the inventory with the new key for this object...
    int idx = set.inventory.value(currentkey,-1);
    BOOST_ASSERT(idx != -1);
    
    //remove the old key and insert the new one
    set.inventory.remove(currentkey);
    set.inventory.insert(newkey, idx);
    
    //update the current key for the object
    currentkey = newkey;
}

/** Destructor */
template<class K, class V>
SIRE_OUTOFLINE_TEMPLATE
MutableSetIterator<K,V>::~MutableSetIterator()
{
    //need to check the index status before we exit
    checkIndexStatus();
}
    
/** Reset the iterator to work on set 's'. The iterator will be placed at the front
    of the set */    
template<class K, class V>
SIRE_OUTOFLINE_TEMPLATE
MutableSetIterator<K,V>& MutableSetIterator<K,V>::operator=( Set<K,V> &s )
{
    //check the index status of the previous set
    checkIndexStatus();

    set = s;
    array = set.objs.data();
    sz = set.size();
    i = -1;
    current = 0;
    
    return *this;
}

/** Copy assignment - this is necessary to allow MutableSetIterators to be passed
    back by functions. Note that you should only ever use a single iterator on
    the set at a time, or else undefined things will happen! */
template<class K, class V>
SIRE_OUTOFLINE_TEMPLATE
MutableSetIterator<K,V>& MutableSetIterator<K,V>::operator=(const MutableSetIterator<K,V> &other)
{
    //need to const_cast as can't copy an iterator returned from a function without
    //a const copy function...
    MutableSetIterator<K,V> &nonconst = const_cast<MutableSetIterator<K,V>&>(other);

    //first, ensure that the indexes are all correct, and that 'other'
    //is up to date...
    nonconst.checkIndexStatus();
    
    //now copy all of the data
    set = nonconst.set;
    array = nonconst.array;
    current = nonconst.current;
    sz = nonconst.sz;
    i = nonconst.i;
    currentkey = nonconst.currentkey;
    
    return *this;
}

/** Return a const reference to the current value of the iterator. Returns undefined
    results unless isValid() returns true. */
template<class K, class V>
SIRE_INLINE_TEMPLATE
const V& MutableSetIterator<K,V>::value() const
{
    return *current;
}

/** Return the key of the current value. Returns undefined results unless isValid() 
    returns true. */
template<class K, class V>
SIRE_INLINE_TEMPLATE
K MutableSetIterator<K,V>::key() const
{
    return Set<K,V>::indexOf( *current );
}

/** Return a modifiable reference to the current object. This is the only two way
    to get a modifiable reference to an object in a set. You can change the object
    in any way you want, but note that if you change the object in a way that changes
    its index, then the Set will need to be reindexed, which can be a slow operation.
    Note also that if you change the object so that its index is the same as another
    object in the set, then this object will overwrite the other object. Note that
    this has undefined results unless isValid() is true. */
template<class K, class V>
SIRE_INLINE_TEMPLATE
V& MutableSetIterator<K,V>::value()
{
    return *current;
}
    
/** Set the value of the current object. This has the same caveats as value(), and is
    the only other way that you can change an object that is in a Set. This has undefined
    results unless isValid() returns true. */
template<class K, class V>
SIRE_INLINE_TEMPLATE
void MutableSetIterator<K,V>::setValue(const V &obj)
{
    *current = obj;
    checkIndexStatus();
}
    
/** Remove the current item from the set. Does nothing if isValid() returns false. This
    will move the iterator over the next item in the Set, or to the back if we are already
    at the last item. Note that this can be a slow process as removing an item will 
    necessitate that the Set is reindexed, and potentially all of the objects will
    need to be reordered and copied. */
template<class K, class V>
SIRE_OUTOFLINE_TEMPLATE
void MutableSetIterator<K,V>::remove()
{
    if (isValid())
    {
        set.removeAt(i);
        
        array = set.objs.data();
        sz = set.size();
        if (i < sz)
        {
            current = &(array[i]);
            currentkey = Set<K,V>::indexOf(*current);
        }
        else
            current = 0;
    }
}

/** Move over the next item in the set and return it. Has undefined results unless
    hasNext() returns true */
template<class K, class V>
SIRE_INLINE_TEMPLATE
const V& MutableSetIterator<K,V>::next()
{
    //we need to check the indexes before moving onto the next item...
    checkIndexStatus();

    i++;
    current = &(array[i]);
    currentkey = Set<K,V>::indexOf(*current);
    
    return *current;
}

/** Peek at the next item in the set. Has undefined results unless hasNext() returns true */
template<class K, class V>
SIRE_INLINE_TEMPLATE
const V& MutableSetIterator<K,V>::peekNext() const
{
    return array[i+1];
}
    
/** Move over the previous item in the set and return it. Has undefined results unless
    hasPrevious() returns true */
template<class K, class V>
SIRE_INLINE_TEMPLATE
const V& MutableSetIterator<K,V>::previous()
{
    //we need to check the indexes before moving onto the next item...
    checkIndexStatus();
    
    current = &(array[i]);
    currentkey = Set<K,V>::indexOf(*current);
    
    i--;
    
    return *current;
}

/** Peek at the previous item in the set. Has undefined results unless hasPrevious()
    returns true. */
template<class K, class V>
SIRE_INLINE_TEMPLATE
const V& MutableSetIterator<K,V>::peekPrevious() const
{
    return array[i];
}
    
/** Jump past item 'idx' in the Set. Has undefined results if there is no such item! */
template<class K, class V>
SIRE_INLINE_TEMPLATE
const V& MutableSetIterator<K,V>::jumpTo(int idx)
{
    //no need to do anything if we are already here...
    if (i != idx)
    {
        //we need to check the indexes before moving onto the next item...
        checkIndexStatus();

        i = idx;
        current = &(array[i]);
        currentkey = Set<K,V>::indexOf(*current);
    }
    
    return *current;
}

/** Peek at item 'idx' in the Set. Has undefined results if here is no such item! */
template<class K, class V>
SIRE_INLINE_TEMPLATE
const V& MutableSetIterator<K,V>::peekAt(int idx) const
{
    return array[idx];
}
    
/** Return the current index of the iterator */
template<class K, class V>
SIRE_INLINE_TEMPLATE
int MutableSetIterator<K,V>::index() const
{
    return i;
}
    
/** Return whether or not there is a 'next' item in the Set */
template<class K, class V>
SIRE_INLINE_TEMPLATE
bool MutableSetIterator<K,V>::hasNext() const
{
    return array != 0 and i < sz-1;
}

/** Return whether or not there is a 'previous' item in the Set */
template<class K, class V>
SIRE_INLINE_TEMPLATE
bool MutableSetIterator<K,V>::hasPrevious() const
{
    return array != 0 and i >= 0;
}

/** Move the iterator to the back of the Set (past the last item) */
template<class K, class V>
SIRE_INLINE_TEMPLATE
void MutableSetIterator<K,V>::toBack()
{
    checkIndexStatus();
    
    i = sz;
    current = 0;
}

/** Move the iterator to the front of the set (before the first item) */
template<class K, class V>
SIRE_INLINE_TEMPLATE
void MutableSetIterator<K,V>::toFront()
{
    checkIndexStatus();
    
    i = -1;
    current = 0;
}
    
/** Return whether the set contains an object with key 'key'. Positions the
    iterator just after the object if it is found, at the front of the set
    if it isn't */
template<class K, class V>
SIRE_OUTOFLINE_TEMPLATE
bool MutableSetIterator<K,V>::findKey(const K &key)
{
    int idx = set.inventory.value(key,-1);
    
    if (idx != -1)
    {
        jumpTo(idx);
        return true;
    }
    else
    {
        toFront();
        return false;
    }
}

/** Return whether the object 'obj' is in the Set. Positions the iterator
    just after the object if it is found, or at the front of the set if 
    it isn't */
template<class K, class V>
SIRE_OUTOFLINE_TEMPLATE
bool MutableSetIterator<K,V>::find(const V &obj)
{
    if ( findKey(Set<K,V>::indexOf(obj)) )
    {
        if (obj == value())
            return true;
        else
        {
            toFront();
            return false;
        }
    }
    else
        return false;
}

}

SIRE_END_HEADER

#endif
