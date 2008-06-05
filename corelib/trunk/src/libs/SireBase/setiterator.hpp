#ifndef SIREBASE_SETITERATOR_HPP
#define SIREBASE_SETITERATOR_HPP

#include "set.hpp"

SIRE_BEGIN_HEADER

namespace SireBase
{

/**
  * This class provides a Java-style const iterator over a Set<K,V>.
  *
  * This is very similar in behaviour to the QListIterator class.
  *
  * Multiple iterators can be used on the same set. If the set is modified 
  * while a SetIterator is active, the SetIterator will continue iterating 
  * over the original set, ignoring the modified copy.
  *
  * @author Christopher Woods
  */
template<class K, class V>
class SetIterator
{
public:
    SetIterator();
    SetIterator(const Set<K,V> &set);
    SetIterator(const SetIterator<K,V> &other);
    
    ~SetIterator();
    
    SetIterator<K,V>& operator=(const SetIterator<K,V> &other);
    SetIterator<K,V>& operator=( const Set<K,V> &set );
    
    const V& value() const;
    K key() const;
    
    const V& next();
    const V& peekNext() const;
    
    const V& previous();
    const V& peekPrevious() const;
    
    const V& jumpTo(int i);
    const V& peekAt(int i) const;
    
    bool find(const V &obj);
    bool findKey(const K &key);
    
    int index() const;
    
    bool hasNext() const;
    bool hasPrevious() const;
    
    bool isValid() const;
    
    void toBack();
    void toFront();

private:

    /** Implicit copy of the set being iterated over */
    Set<K,V> set;
    
    /** Array containing the object data */
    const V *array;
    
    /** Pointer to the current item */
    const V *current;
    
    /** The size of the set */
    const int sz;
    
    /** The current index */
    int i;
};

/** Construct a null iterator */
template<class K, class V>
SIRE_OUTOFLINE_TEMPLATE
SetIterator<K,V>::SetIterator() : array(0), current(0), sz(0), i(-1)
{}

/** Construct an iterator over the Set 's'. The iterator is positioned
    before the first item */
template<class K, class V>
SIRE_OUTOFLINE_TEMPLATE
SetIterator<K,V>::SetIterator(const Set<K,V> &s)
                 : set(s), array(set.objs.constData()), current(0), sz(s.size()), i(-1)
{}
    
/** Copy constructor */
template<class K, class V>
SIRE_OUTOFLINE_TEMPLATE
SetIterator<K,V>::SetIterator(const SetIterator<K,V> &other)
                 : set(other.set), array(set.objs.constData()), sz(set.size()), i(other.i)
{
    if (i >= 0 and i < sz)
        current = &(array[i]);
    else
        current = 0;
}
    
/** Destructor */
template<class K, class V>
SIRE_OUTOFLINE_TEMPLATE
SetIterator<K,V>::~SetIterator()
{}
    
/** Copy assignment */
template<class K, class V>
SIRE_OUTOFLINE_TEMPLATE
SetIterator<K,V>& SetIterator<K,V>::operator=( const SetIterator<K,V> &other )
{
    set = other.set;
    array = set.objs.constData();
    sz = set.size();
    i = other.i;
    
    if (i >= 0 and i < sz)
        current = &(array[i]);
    else
        current = 0;

    return *this;
}
    
/** Reset the iterator to work on the set 's'. The iterator will be placed
    at the front. */
template<class K, class V>
SIRE_OUTOFLINE_TEMPLATE
SetIterator<K,V>& SetIterator<K,V>::operator=( const Set<K,V> &s )
{
    set = s;
    array = set.objs.constData();
    sz = set.size();
    current = 0;
    i = -1;
    
    return *this;
}

/** Return whether or not this is a valid iterator at a valid position  
    in a Set */
template<class K, class V>
SIRE_OUTOFLINE_TEMPLATE
bool SetIterator<K,V>::isValid() const
{
    return array != 0 and i >= 0 and i < sz;
}

/** Return the value of the last item jumped over - this will return undefined
    results if this iterator it not valid */
template<class K, class V>
SIRE_INLINE_TEMPLATE
const V& SetIterator<K,V>::value() const
{
    return *current;
}

/** Return the key of the last item jumped over - this will return undefined
    results if this iterator is not valid. */
template<class K, class V>
SIRE_INLINE_TEMPLATE
K SetIterator<K,V>::key() const
{
    return Set<K,V>::indexOf( *current );
}
    
/** Jump to the next value - return the next value. This will return undefined
    results if we are already at the end of the Set */
template<class K, class V>
SIRE_INLINE_TEMPLATE
const V& SetIterator<K,V>::next()
{
    i++;
    current = &(array[i]);
    return *current;
}

/** Take a peek at the next value in the Set. This will return undefined 
    results if the current value is at the end of the set, or if we are
    beyond the end of the Set */
template<class K, class V>
SIRE_INLINE_TEMPLATE
const V& SetIterator<K,V>::peekNext() const
{
    return array[i+1];
}
    
/** Jump to the previous value - return the previous value. This will return
    undefined results if we are at the beginning of the set */
template<class K, class V>
SIRE_INLINE_TEMPLATE
const V& SetIterator<K,V>::previous()
{
    current = &(array[i]);
    i--;
    return *current;
}

/** Take a peek at the previous value in the Set. This will return undefined
    results if the current value is at the beginning of the set, or if we are
    beyond the beginning of the Set */
template<class K, class V>
SIRE_INLINE_TEMPLATE
const V& SetIterator<K,V>::peekPrevious() const
{
    return array[i];
}
    
/** Jump past item 'idx' and return it. This will return undefined results if 
    'i' is not a valid index in the Set */
template<class K, class V>
SIRE_INLINE_TEMPLATE
const V& SetIterator<K,V>::jumpTo(int idx)
{
    if (i != idx)
    {
        i = idx;
        current = &(array[i]);
    }
    
    return *current;
}
    
/** Peek at the item with index 'idx'. This will return undefined results if 
    'i' is not a valid index in the Set */
template<class K, class V>
SIRE_INLINE_TEMPLATE
const V& SetIterator<K,V>::peekAt(int idx) const
{
    return array[idx];
}
    
/** Return the current index of the iterator. This will be '-1' if the iterator
    is at the front of the Set, and size() if the iterator is at the back. */
template<class K, class V>
SIRE_INLINE_TEMPLATE
int SetIterator<K,V>::index() const
{
    return i;
}

/** Return whether or not there is a next item in the Set */
template<class K, class V>
SIRE_INLINE_TEMPLATE
bool SetIterator<K,V>::hasNext() const
{
    return array != 0 and i < sz-1;
}

/** Return whether or not the set has a previous value */
template<class K, class V>
SIRE_INLINE_TEMPLATE
bool SetIterator<K,V>::hasPrevious() const
{
    return array != 0 and i >= 0;
}

/** Move the iterator to the back of the set (past the last item) */
template<class K, class V>
SIRE_INLINE_TEMPLATE
void SetIterator<K,V>::toBack()
{
    i = sz;
    current = 0;
}

/** Move the iterator to the front of the set (before the first item) */
template<class K, class V>
SIRE_INLINE_TEMPLATE
void SetIterator<K,V>::toFront()
{
    i = -1;
    current = 0;
}

/** Return whether the set contains an object with key 'key'. Positions the
    iterator just after the object if it is found, at the front of the set
    if it isn't */
template<class K, class V>
SIRE_INLINE_TEMPLATE
bool SetIterator<K,V>::findKey(const K &key)
{
    int idx = set.inventory.value(key,-1);
    
    if (idx != -1)
    {
        jumpTo(i);
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
SIRE_INLINE_TEMPLATE
bool SetIterator<K,V>::find(const V &obj)
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
