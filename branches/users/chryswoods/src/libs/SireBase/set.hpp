#ifndef SIREBASE_SET_H
#define SIREBASE_SET_H

#include <QHash>
#include <QMutableHashIterator>
#include <QVector>
#include <QObject>
#include <QDataStream>

#include <boost/tuple/tuple.hpp>

#include "SireError/errors.h"

#include "SireStream/datastream.h"

SIRE_BEGIN_HEADER

namespace SireBase
{
template<class K, class V>
class Set;
template<class K, class V>
class SetIterator;
template<class K, class V>
class MutableSetIterator;
}

template<class K, class V>
QDataStream& operator<<(QDataStream&, const SireBase::Set<K,V>&);
template<class K, class V>
QDataStream& operator>>(QDataStream&, SireBase::Set<K,V>&);

/** Prototype for the indexing function. Return the key for the 
    passed value. Note that this must always return the same 
    key for the passed value. */
template<class K, class V>
K set_indexer(const V &value);

namespace SireBase
{

/**
  * This class provides a Set of objects, that are indexed by an index type
  * that is obtained via the "K set_indexer(const V &value)" function.
  *
  * @author Christopher Woods
  *
  */

template<class K, class V>
class Set
{

friend QDataStream& ::operator<<<>(QDataStream&, const Set<K,V>&);
friend QDataStream& ::operator>><>(QDataStream&, Set<K,V>&);

friend class SetIterator<K,V>;
friend class MutableSetIterator<K,V>;

public:
    Set();
    Set(const Set<K,V> &other);
    Set(const QVector<V> &objs);
    ~Set();

    bool operator==(const Set<K,V> &other) const;
    bool operator!=(const Set<K,V> &other) const;

    const Set<K,V>& operator=(const Set<K,V> &other);
        
    const V& at(int i) const;
    const V& at_key(const K &key) const;

    const V& operator[](int i) const;
    const V& operator()(const K &key) const;

    int count() const;
    int size() const;

    bool isEmpty() const;
    void clear();
    
    QString toString() const;

    int indexOfObject(const V &obj) const;
    int indexOfKey(const K &key) const;

    QList<K> keys() const;
    const QVector<V>& values() const;
    
    const V* constData() const;
    
    bool contains(const V &obj) const;
    bool has_key(const K &key) const;

    void insert(const V &obj);
    
    void remove(const V &obj);
    void remove_key(const K &key);
    void removeAt(int i);

    void reserve(int n);
    void squeeze();
    
    Set<K,V> sum(const Set<K,V> &other) const;
    Set<K,V> intersection(const Set<K,V> &other) const;
    Set<K,V> leftDifference(const Set<K,V> &other) const;
    Set<K,V> rightDifference(const Set<K,V> &other) const;
    Set<K,V> symmetricDifference(const Set<K,V> &other) const;

    boost::tuple< QVector<V>, QVector<V> > splitIntoVectors(const Set<K,V> &other) const;
    boost::tuple< Set<K,V>, Set<K,V> > splitIntoSets(const Set<K,V> &other) const;
    
    bool intersects(const Set<K,V> &other) const;
    bool isSubSet(const Set<K,V> &other) const;
    bool isSuperSet(const Set<K,V> &other) const;

    bool isStrictSubSet(const Set<K,V> &other) const;
    bool isStrictSuperSet(const Set<K,V> &other) const;

    static K indexOf(const V &obj);

private:

    void insert(const K &key, const V &obj);

    /** The contents of this Set */
    QVector<V> objs;

    /** A hash of all the objects that are in this set.
        This is used to quickly search whether a particular
        object is in this set. The value of this hash is the index
        of the object in the vector */
    QHash<K,qint32> inventory;
};

/////////////////////////////////////
///////////////////////////////////// Implementation of the Set<K,V> class
/////////////////////////////////////

/** Create an empty set */
template<class K, class V>
SIRE_OUTOFLINE_TEMPLATE
Set<K,V>::Set()
{}

/** Copy constructor */
template<class K, class V>
SIRE_OUTOFLINE_TEMPLATE
Set<K,V>::Set(const Set<K,V> &other) : objs(other.objs), inventory(other.inventory)
{}

/** Construct a Set from a vector of elements */
template<class K, class V>
SIRE_OUTOFLINE_TEMPLATE
Set<K,V>::Set(const QVector<V> &objvector) : objs(objvector)
{
    int nobjs = objs.count();
    inventory.reserve(nobjs);
    
    for (int i=0; i<nobjs; ++i)
    {
        //get the index for the object...
        K idx = indexOf(objs.at(i));
        
        if (not inventory.contains(idx))
            inventory.insert(idx,i);
        else
        {
            //remove the duplicated item and move on...
            objs.remove(i);
            i--;
            nobjs--;
        }
    }
}

/** Destructor */
template<class K, class V>
SIRE_OUTOFLINE_TEMPLATE
Set<K,V>::~Set()
{}

/** Return the index of the object 'obj'. This is a shorthand function
    that calls the 'set_indexer' function on the object. Note that the 
    object does not need to be in a set. */
template<class K, class V>
SIRE_INLINE_TEMPLATE
K Set<K,V>::indexOf(const V &obj)
{
    return ::set_indexer<K,V>(obj);
}

/** Comparison operator */
template<class K, class V>
SIRE_OUTOFLINE_TEMPLATE
bool Set<K,V>::operator==(const Set<K,V> &other) const
{
    return inventory == other.inventory and objs == other.objs;
}

/** Comparison operator */
template<class K, class V>
SIRE_OUTOFLINE_TEMPLATE
bool Set<K,V>::operator!=(const Set<K,V> &other) const
{
    return inventory != other.inventory or objs != other.objs;
}

/** Assignment operator */
template<class K, class V>
SIRE_OUTOFLINE_TEMPLATE
const Set<K,V>& Set<K,V>::operator=(const Set<K,V> &other)
{
    inventory = other.inventory;
    objs = other.objs;

    return *this;
}

/** Return whether or not this Set is empty */
template<class K, class V>
SIRE_INLINE_TEMPLATE
bool Set<K,V>::isEmpty() const
{
    return objs.isEmpty();
}

/** Return a const reference to the item at index 'i'.  

    \throw SireError::invalid_index
*/
template<class K, class V>
SIRE_INLINE_TEMPLATE
const V& Set<K,V>::at(int i) const
{
    if (i < 0 or i >= objs.count())
        throw SireError::invalid_index(QObject::tr(
            "Invalid index in Set<K,V> - i == %1, size() == %2")
                .arg(i).arg(count()), CODELOC);
    
    return objs.constData()[i];
}

/** Return a const reference to the object at key 'key' 

    \throw SireError::invalid_key
*/
template<class K, class V>
SIRE_INLINE_TEMPLATE
const V& Set<K,V>::at_key(const K &key) const
{
    int idx = inventory.value(key, -1);
    
    if (idx == -1)
        throw SireError::invalid_key(QObject::tr(
          "Invalid key in Set<K,V>"), CODELOC);
                  
    return objs.constData()[idx];
}

/** Return a const reference to the item at index 'i'. 

    \throw SireError::invalid_index
*/
template<class K, class V>
SIRE_INLINE_TEMPLATE
const V& Set<K,V>::operator[](int i) const
{
    return at(i);
}

/** Return a reference to the item with key 'idx'. 

    \throw SireError::invalid_key
*/
template<class K, class V>
SIRE_INLINE_TEMPLATE
const V& Set<K,V>::operator()(const K &key) const
{
    return at_key(key);
}

/** Return a list of all of the keys of this set */
template<class K, class V>
SIRE_OUTOFLINE_TEMPLATE
QList<K> Set<K,V>::keys() const
{
    return inventory.keys();
}

/** Return a list of all of the values in this Set in the same order
    as they occur in the set. This is fast as this just returns a 
    reference to the underlying QVector of the Set. */
template<class K, class V>
SIRE_INLINE_TEMPLATE
const QVector<V>& Set<K,V>::values() const
{
    return objs;
}

/** Return a const pointer to the underlying array containing the 
    objects in this set. This pointer will remain valid until this
    Set is reallocated */
template<class K, class V>
SIRE_INLINE_TEMPLATE
const V* Set<K,V>::constData() const
{
    return objs.constData();
}

/** Return the number of items in the Set */
template<class K, class V>
SIRE_INLINE_TEMPLATE
int Set<K,V>::count() const
{
    return objs.count();
}

/** Return the number of items in the Set */
template<class K, class V>
SIRE_INLINE_TEMPLATE
int Set<K,V>::size() const
{
    return objs.count();
}

/** Clear the entire set */
template<class K, class V>
SIRE_OUTOFLINE_TEMPLATE
void Set<K,V>::clear()
{
    objs.clear();
    inventory.clear();
}

/** Print a string representation of this Set */
template<class K, class V>
SIRE_OUTOFLINE_TEMPLATE
QString Set<K,V>::toString() const
{
    return QObject::tr("Set<K,V> : size() == %1").arg(size());
}

/** Return the index of the key 'key' in this set, throw an exception if the key
    is not in this set.

    \throw SireError::invalid_key
*/
template<class K, class V>
SIRE_OUTOFLINE_TEMPLATE
int Set<K,V>::indexOfKey(const K &key) const
{
    qint32 idx = inventory.value(key, -1);
    
    if (idx < 0 or idx >= objs.count())
        throw SireError::invalid_key(QObject::tr(
                  "Invalid key in Set<K,V>"), CODELOC);
        
    return idx;
}    

/** Return the index of the object 'obj' in this set, throw an exception if the 
    key is not in this set. 
    
    \throw SireError::invalid_key
*/
template<class K, class V>
SIRE_OUTOFLINE_TEMPLATE
int Set<K,V>::indexOfObject(const V &obj) const
{
    return indexOfKey( indexOf(obj) );
}

/** Return whether or not this set contains the object 'obj' */
template<class K, class V>
SIRE_INLINE_TEMPLATE
bool Set<K,V>::contains(const V &obj) const
{
    return inventory.contains( indexOf(obj) );
}

/** Return whether or not this set contains the object index 'idx' */
template<class K, class V>
SIRE_INLINE_TEMPLATE
bool Set<K,V>::has_key(const K &idx) const
{
    return inventory.contains(idx);
}

/** Private function used to insert an object for which the key is already known.
    You *must* ensure that the value of 'key' is the result of set_indexer(obj)
    or else subtle bugs will result. If there is already an object with this 
    key then it is replaced */
template<class K, class V>
SIRE_INLINE_TEMPLATE
void Set<K,V>::insert(const K &key, const V &obj)
{
    qint32 idx = inventory.value(key,-1);
    
    if (idx == -1)
    {
        inventory.insert(key, objs.count());
        objs.append(obj);
    }
    else
        objs[idx] = obj;
}

/** Insert the object 'obj' into the set. If there
    is already an object with the same key as obj, 
    (obtained via the set_indexer() function) then this object is replaced. */
template<class K, class V>
SIRE_OUTOFLINE_TEMPLATE
void Set<K,V>::insert(const V &obj)
{
    //get the key of this object
    insert( indexOf(obj), obj );
}

/** Remove the object with key 'key'. This will be slow as the entire set
    may need to be copied and reordered. This will do nothing if there is no
    object in the set with this key.
*/
template<class K, class V>
SIRE_OUTOFLINE_TEMPLATE
void Set<K,V>::remove_key(const K &key)
{
    qint32 idx = inventory.value(key,-1);
    
    if (idx != -1)
    {
        //remove the item from the inventory
        inventory.remove(key);
        
        //remove the object itself - this is potentially slow
        //as it may need to copy and reorder the entire vector
        objs.remove(idx);
        
        //now reindex the inventory... (this is slow)
        QMutableHashIterator<K,qint32> it(inventory);
        
        while (it.hasNext())
        {
            it.next();
            if (it.value() >= idx)
                it.value() --;
        }
    }
}

/** Remove the object 'obj' from this set. Note that this will 
    be slow as the entire set will need to be copied and reordered. 
    This will do nothing if this object is not in the set.
*/
template<class K, class V>
SIRE_INLINE_TEMPLATE
void Set<K,V>::remove(const V &obj)
{
    //get the key for this object - we will remove the key!
    remove_key( indexOf(obj) );
}

/** Remove the object at index 'i'

    \throw SireError::invalid_index
*/
template<class K, class V>
SIRE_INLINE_TEMPLATE
void Set<K,V>::removeAt(int i)
{
    remove( at(i) );
}

/** Return the sum (union) of this Set with 'other'. This returns 
    a Set that has all of the keys of both this set and 'other'. If
    both sets have the same key, then the copy of the object from 
    this set is used. This means that this->sum(other) may return
    a different set to other.sum(*this) */
template<class K, class V>
SIRE_OUTOFLINE_TEMPLATE
Set<K,V> Set<K,V>::sum(const Set<K,V> &other) const
{
    int mysize = size();
    int othersize = other.size();
    
    const V *otherarray = other.objs.constData();
    const V *myarray = objs.constData();
    
    //which set is bigger?
    if (mysize >= othersize)
    {
        Set<K,V> set(*this);
        
        //the sum will have a maximum size of the sum of the sizes
        //of this set and other
        set.reserve( mysize + othersize );
        
        for( int i=0; i<othersize; ++i )
        {
            const V &obj = otherarray[i];
            K key = indexOf(obj);
            
            if (not set.has_key(key))
                set.insert(key,obj);
        }
        
        return set;
    }
    else
    {
        Set<K,V> set(other);
        
        //the sum will have a maximum size of the sum of the sizes
        //of this set and other
        set.reserve( mysize + othersize );
        
        for( int i=0; i<mysize; ++i )
        {
            set.insert( myarray[i] );
        }
        
        return set;
    }
}

/** Return the intersection (difference) of this set with 'other'. Note that as the
    test is based on the indexes, the actual values may be different. The returned
    set contains the values from this set. This means that this->intersection(other)
    may return a set that has different values to other->intersection(this). */
template<class K, class V>
SIRE_OUTOFLINE_TEMPLATE
Set<K,V> Set<K,V>::intersection(const Set<K,V> &other) const
{
    Set<K,V> set;

    int mysize = size();
    int othersize = other.size();
    
    const V *otherarray = other.objs.constData();
    const V *myarray = objs.constData();

    //loop through the elements of the smallest list..
    if (mysize <= othersize)
    {
        //reserve sufficient space in the set - the intersection
        //cannot be any larger than the smallest set
        set.reserve(mysize);

        for( int i=0; i<mysize; ++i )
        {
            const V &obj = myarray[i];
            K key = indexOf(obj);
            
            if (other.has_key(key))
                set.insert(key,obj);
        }
    }
    else
    {
        //reserve sufficient space in the set - the intersection
        //cannot be any larger than the smallest set
        set.reserve(othersize);
    
        for( int i=0; i<othersize; ++i)
        {
            const V &obj = otherarray[i];
            K key = indexOf(obj);
        
            int idx = inventory.value( key, -1 );
            if (idx != -1)
                set.insert( key, myarray[idx] );
        }
    }

    return set;
}

/** Return whether or not two sets intersect (have at least one common
    member) */
template<class K, class V>
SIRE_OUTOFLINE_TEMPLATE
bool Set<K,V>::intersects(const Set<K,V> &other) const
{
    if (size() <= other.size())
    {
        QHashIterator<K,qint32> it(inventory);
        
        while(it.hasNext())
        {
            it.next();
        
            if ( other.has_key(it.key()) )
                return true;
        }
    }
    else
    {
        QHashIterator<K,qint32> it(other.inventory);
        
        while(it.hasNext())
        {
            it.next();
            
            if (this->has_key(it.key()))
                return true;
        }
    }

    return false;
}

/** Return the left difference of this set with 'other'. This is the
    collection of items in this set that are not in 'other'. */
template<class K, class V>
SIRE_OUTOFLINE_TEMPLATE
Set<K,V> Set<K,V>::leftDifference(const Set<K,V> &other) const
{
    Set<K,V> set;

    int mysize = size();
    const V *myarray = objs.constData();

    //the left difference cannot be larger than this set
    set.reserve(mysize);

    for( int i=0; i<mysize; ++i)
    {
        const V &obj = myarray[i];
        K key = indexOf(obj);
        
        if (not other.has_key(key))
            set.insert(key,obj);
    }

    return set;
}

/** Return the right difference of this set with 'other'. This is the
    collection of items that are in 'other' that are not in this set. */
template<class K, class V>
SIRE_OUTOFLINE_TEMPLATE
Set<K,V> Set<K,V>::rightDifference(const Set<K,V> &other) const
{
    Set<K,V> set;

    int othersize = other.size();
    const V *otherarray = other.objs.constData();
    
    //the right difference cannot be larger than 'other'
    set.reserve(othersize);

    for( int i=0; i<othersize; ++i)
    {
        const V &obj = otherarray[i];
        K key = indexOf(obj);
        
        if (not this->has_key(key))
            set.insert(key,obj);
    }
    
    return set;
}

/** Return the symmetric difference of this set with 'other'. This
    is the collection of objects that are in this or other, but are not
    in both this and other */
template<class K, class V>
SIRE_OUTOFLINE_TEMPLATE
Set<K,V> Set<K,V>::symmetricDifference(const Set<K,V> &other) const
{
    Set<K,V> set;
    
    int mysize = size();
    int othersize = other.size();
    
    const V *otherarray = other.objs.constData();
    const V *myarray = objs.constData();
    
    //the symmetric difference can be in size anything up to the 
    //sum of the sizes of the two sets
    set.reserve( mysize + othersize );
    
    for( int i=0; i<mysize; ++i )
    {
        const V &obj = myarray[i];
        K key = indexOf(obj);
        
        if (not other.has_key(key))
            set.insert(key,obj);
    }
                  
    for( int i=0; i<othersize; ++i )
    {
        const V &obj = otherarray[i];
        K key = indexOf(obj);
        
        if (not this->has_key(key))
            set.insert(key,obj);
    }
    
    return set;
}

/** Split the set into two vectors. The first contains items that are 
    in both Sets, while the second contains those items that
    are only in this Set. */
template<class K, class V>
SIRE_OUTOFLINE_TEMPLATE
boost::tuple< QVector<V>, QVector<V> > Set<K,V>::splitIntoVectors(const Set<K,V> &other) const
{
    int mysize = size();
    int othersize = other.size();
    const V *myarray = objs.constData();
    
    //the first vector can only be as large as the smallest set...
    QVector<V> bothvec;
    bothvec.reserve( qMin(mysize,othersize) );
    
    //the second vector can only be as large as this set...
    QVector<V> thisvec;
    thisvec.reserve(mysize);
    
    for( int i=0; i<mysize; ++i )
    {
        const V &obj = myarray[i];
        
        if (other.contains(obj))
            bothvec.append(obj);
        else
            thisvec.append(obj);
    }
    
    return boost::tuple< QVector<V>, QVector<V> >(bothvec, thisvec);
}

/** Split the set into two sets. The first set contains items that are
    in both Sets, while the second set contains those items that are 
    only in this Set. (this is the Set version of splitIntoLists) */
template<class K, class V>
SIRE_OUTOFLINE_TEMPLATE
boost::tuple< Set<K,V>, Set<K,V> > Set<K,V>::splitIntoSets(const Set<K,V> &other) const
{
    int mysize = size();
    int othersize = other.size();
    const V *myarray = objs.constData();
    
    //the first set can only be as large as the smallest set...
    Set<K,V> bothset;
    bothset.reserve( qMin(mysize,othersize) );
    
    //the second set can only be as large as this set...
    Set<K,V> thisset;
    thisset.reserve(mysize);
    
    for( int i=0; i<mysize; ++i )
    {
        const V &obj = myarray[i];
        K key = indexOf(obj);
        
        if (other.has_key(key))
            bothset.insert(key,obj);
        else
            thisset.insert(key,obj);
    }
    
    return boost::tuple< Set<K,V>, Set<K,V> >(bothset, thisset);
}

/** Return whether or not this is a complete subset of 'other' */
template<class K, class V>
SIRE_OUTOFLINE_TEMPLATE
bool Set<K,V>::isSubSet(const Set<K,V> &other) const
{
    QHashIterator<K,qint32> it(inventory);
    
    while (it.hasNext())
    {
        it.next();
        
        if (not other.has_key(it.key()))
            return false;
    }

    return true;
}

/** Return whether or not this is a superset of 'other' */
template<class K, class V>
SIRE_INLINE_TEMPLATE
bool Set<K,V>::isSuperSet(const Set<K,V> &other) const
{
    return other.isSubSet(*this);
}

/** Return whether or not this is a strict subset of 'other' (a strict 
    subset is a subset that is also smaller) */
template<class K, class V>
SIRE_INLINE_TEMPLATE
bool Set<K,V>::isStrictSubSet(const Set<K,V> &other) const
{
    return this->size() < other.size() and this->isSubSet(other);
}

/** Return whether or not this is a strict superset of 'other' 
    (a strict superset is one that is also larger) */
template<class K, class V>
SIRE_INLINE_TEMPLATE
bool Set<K,V>::isStrictSuperSet(const Set<K,V> &other) const
{
    return this->size() > other.size() and this->isSuperSet(other);
}

/** Reserve space for at least 'n' entries */
template<class K, class V>
SIRE_OUTOFLINE_TEMPLATE
void Set<K,V>::reserve(int n)
{
    inventory.reserve(n);
    objs.reserve(n);
}
    
/** Squeeze the set so that it uses the minimum amount of memory possible.
    This can be slow as it will cause the contents of the set to be copied. */
template<class K, class V>
SIRE_OUTOFLINE_TEMPLATE
void Set<K,V>::squeeze()
{
    inventory.squeeze();
    objs.squeeze();
}

/** Magic for a Set */
static const SireStream::MagicID set_magic = SireStream::getMagic("SireBase::Set");

}

/** Serialise a Set to a binary datastream */
template<class K, class V>
SIRE_OUTOFLINE_TEMPLATE
QDataStream& operator<<(QDataStream &ds, const SireBase::Set<K,V> &set)
{
    SireStream::writeHeader(ds, SireBase::set_magic, 1) << set.objs;
    
    return ds;
}

/** Deserialise a Set from a binary datastream */
template<class K, class V>
SIRE_OUTOFLINE_TEMPLATE
QDataStream& operator>>(QDataStream &ds, SireBase::Set<K,V> &set)
{
    SireStream::VersionID v = SireStream::readHeader(ds, SireBase::set_magic, 
                                                     "SireBase::Set");

    if (v == 1)
    {
        ds >> set.objs;
    
        //recalculate the indexes...
        int sz = set.objs.count();
        for (int i=0; i<sz; ++i)
        {
            K key = ::set_indexer<K,V>(set.objs.at(i));
            BOOST_ASSERT( not set.inventory.contains(key) );        
            set.inventory.insert(key, i);
        }

        return ds;
    }
    else
        throw SireStream::version_error( v, "1", "SireBase::Set", CODELOC );
}

SIRE_END_HEADER

#endif
