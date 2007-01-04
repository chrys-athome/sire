#ifndef SIREMOL_GROUPEDVECTOR_HPP
#define SIREMOL_GROUPEDVECTOR_HPP

#include "indexinfo.h"

#include "SireStream/datastream.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
template<class IDX, class V>
class GroupedVector;
}

template<class IDX, class V>
QDataStream& operator<<(QDataStream&, const SireMol::GroupedVector<IDX,V>&);
template<class IDX, class V>
QDataStream& operator>>(QDataStream&, SireMol::GroupedVector<IDX,V>&);

namespace SireMol
{

template<class T>
class IndexInfo;

/** This class provides a GroupedVector - this is a collection
    of objects that are held in a set of vectors that are indexed
    by types like GroupIndexID. You must supply an overload
    of group_index_info that provides the extra information needed
    to use this class

    \param V      The type of the contents of the GroupedVector
    \param IDX    The type used to index the GroupedVector
                  (e.g. GroupIndexID, ResAtomID, CGAtomID etc.)

    @author Christopher Woods
*/
template< class IDX, class V >
class SIREMOL_EXPORT GroupedVector
{

friend QDataStream& ::operator<<<>(QDataStream&, const GroupedVector<IDX,V>&);
friend QDataStream& ::operator>><>(QDataStream&, GroupedVector<IDX,V>&);

public:
    typedef V value_type;
    typedef typename IndexInfo<IDX>::groupid_type groupid_type;
    typedef IDX index_type;

    typedef typename IndexInfo<IDX>::invalid_group_error invalid_group_error;
    typedef typename IndexInfo<IDX>::invalid_index_error invalid_index_error;

    typedef QHash< groupid_type, QVector<value_type> > hash_type;

    typedef typename hash_type::const_iterator const_iterator;
    typedef typename hash_type::iterator iterator;

    typedef const_iterator ConstIterator;
    typedef iterator Iterator;

    GroupedVector();

    GroupedVector(const GroupedVector<IDX,V> &other);

    ~GroupedVector();

    bool operator==(const GroupedVector<IDX,V> &other) const;
    bool operator!=(const GroupedVector<IDX,V> &other) const;

    GroupedVector<IDX,V>& operator=(const GroupedVector<IDX,V> &other) const;

    const_iterator begin() const;
    const_iterator end() const;

    iterator begin();
    iterator end();

    const_iterator constBegin() const;
    const_iterator constEnd() const;

    const_iterator find(const groupid_type &groupid) const;
    iterator find(const groupid_type &groupid);
    const_iterator constFind(const groupid_type &groupid) const;

    iterator erase(iterator pos);

    const V& at(const index_type &index) const;
    const QVector<V>& at(const groupid_type &groupid) const;

    const V& operator[](const index_type &index) const;
    V& operator[](const index_type &index);

    const QVector<V>& operator[](const groupid_type &groupid) const;
    QVector<V>& operator[](const groupid_type &groupid);

    V value(const index_type &index) const;
    V value(const index_type &index, const V &defaultValue) const;
    QVector<V> values(const groupid_type &groupid) const;
    QVector<V> values(const groupid_type &groupid, const QVector<V> &defaultValue) const;

    void clear();

    int count() const;
    int size() const;

    int count(const groupid_type &groupid) const;
    int size(const groupid_type &groupid) const;

    bool isEmpty() const;
    bool isEmpty(const groupid_type &groupid) const;

    bool contains(const groupid_type &groupid) const;
    bool contains(const index_type &index) const;

    const V* constData(const groupid_type &groupid) const;
    const V* data(const groupid_type &groupid) const;
    V* data(const groupid_type &groupid);

    const QVector<V>& vector(const groupid_type &groupid) const;
    const QVector<V>& constVector(const groupid_type &groupid) const;
    QVector<V>& vector(const groupid_type &groupid);

    void reserve(int ngroups);
    void reserve(const groupid_type &groupid, int n);

    void resize(const groupid_type &groupid, int n);

    void squeeze();
    void squeeze(const groupid_type &groupid);

    void insert(const groupid_type &groupid, const QVector<V> &values);
    void remove(const groupid_type &groupid);

    QVector<V> take(const groupid_type &groupid);

    void append(const groupid_type &groupid, const V &value);

    void insert(const index_type &index, const V &value);
    void remove(const index_type &index);

private:
    /** All of the values in QVectors, indexed by group ID */
    hash_type vecdata;
};

/** Empty constructor */
template<class IDX, class V>
SIRE_OUTOFLINE_TEMPLATE
GroupedVector<IDX,V>::GroupedVector()
{}

/** Copy constructor */
template<class IDX, class V>
SIRE_OUTOFLINE_TEMPLATE
GroupedVector<IDX,V>::GroupedVector(const GroupedVector<IDX,V> &other)
                     : vecdata(other.vecdata)
{}

/** Destructor */
template<class IDX, class V>
SIRE_OUTOFLINE_TEMPLATE
GroupedVector<IDX,V>::~GroupedVector()
{}

/** Comparison operator */
template<class IDX, class V>
SIRE_INLINE_TEMPLATE
bool GroupedVector<IDX,V>::operator==(const GroupedVector<IDX,V> &other) const
{
    return vecdata == other.vecdata;
}

/** Comparison operator */
template<class IDX, class V>
SIRE_INLINE_TEMPLATE
bool GroupedVector<IDX,V>::operator!=(const GroupedVector<IDX,V> &other) const
{
    return vecdata != other.vecdata;
}

/** Assignment operator */
template<class IDX, class V>
SIRE_INLINE_TEMPLATE
GroupedVector<IDX,V>& GroupedVector<IDX,V>::operator=(const GroupedVector<IDX,V> &other) const
{
    vecdata = other.vecdata;
    return *this;
}

/** Return an iterator to the first group of the GroupedVector */
template<class IDX, class V>
SIRE_INLINE_TEMPLATE
typename GroupedVector<IDX,V>::const_iterator GroupedVector<IDX,V>::begin() const
{
    return vecdata.begin();
}

/** Return an iterator one-past the last group of the GroupedVector */
template<class IDX, class V>
SIRE_INLINE_TEMPLATE
typename GroupedVector<IDX,V>::const_iterator GroupedVector<IDX,V>::end() const
{
    return vecdata.end();
}

/** Return an iterator to the first group of the GroupedVector */
template<class IDX, class V>
SIRE_INLINE_TEMPLATE
typename GroupedVector<IDX,V>::iterator GroupedVector<IDX,V>::begin()
{
    return vecdata.begin();
}

/** Return an iterator one-past the last group of the GroupedVector */
template<class IDX, class V>
SIRE_INLINE_TEMPLATE
typename GroupedVector<IDX,V>::iterator GroupedVector<IDX,V>::end()
{
    return vecdata.end();
}

/** Return an iterator to the first group of the GroupedVector */
template<class IDX, class V>
SIRE_INLINE_TEMPLATE
typename GroupedVector<IDX,V>::const_iterator GroupedVector<IDX,V>::constBegin() const
{
    return vecdata.constBegin();
}

/** Return an iterator one-past the last group of the GroupedVector */
template<class IDX, class V>
SIRE_INLINE_TEMPLATE
typename GroupedVector<IDX,V>::const_iterator GroupedVector<IDX,V>::constEnd() const
{
    return vecdata.constEnd();
}

/** Return an iterator pointing to the group with ID 'groupid' - or end()
    if there is no group with this ID in this GroupedVector */
template<class IDX, class V>
typename GroupedVector<IDX,V>::const_iterator
SIRE_INLINE_TEMPLATE
GroupedVector<IDX,V>::find(const groupid_type &groupid) const
{
    return vecdata.find(groupid);
}

/** Return an iterator pointing to the group with ID 'groupid' - or end()
    if there is no group with this ID in this GroupedVector */
template<class IDX, class V>
typename GroupedVector<IDX,V>::iterator
SIRE_INLINE_TEMPLATE
GroupedVector<IDX,V>::find(const groupid_type &groupid)
{
    return vecdata.find(groupid);
}

/** Return an iterator pointing to the group with ID 'groupid' - or end()
    if there is no group with this ID in this GroupedVector */
template<class IDX, class V>
typename GroupedVector<IDX,V>::const_iterator
SIRE_INLINE_TEMPLATE
GroupedVector<IDX,V>::constFind(const groupid_type &groupid) const
{
    return vecdata.constFind(groupid);
}

/** Erase the group pointed to by the iterator 'pos', and return an iterator
    to the next group in the GroupedVector */
template<class IDX, class V>
SIRE_INLINE_TEMPLATE
typename GroupedVector<IDX,V>::iterator GroupedVector<IDX,V>::erase(iterator pos)
{
    return vecdata.erase(pos);
}

/** Return the object at index 'index'. This will throw an exception if
    this is an invalid index.

    \throw invalid_group_error
    \throw invalid_index_error
*/
template<class IDX, class V>
SIRE_INLINE_TEMPLATE
const V& GroupedVector<IDX,V>::at(const IDX &idx) const
{
    typename hash_type::const_iterator it = vecdata.find( IndexInfo<IDX>::groupID(idx) );

    if (it == vecdata.end())
        IndexInfo<IDX>::invalidGroup(idx);

    const QVector<V> &vec = *it;

    Index i = IndexInfo<IDX>::index(idx);
    if (i >= vec.count())
        IndexInfo<IDX>::invalidIndex(idx, vec.count());

    return vec.data()[i];
}

/** Return the vector object objects in the group with ID 'groupid'. This will
    throw an exception if this is an invalid group ID

    \throw invalid_group_error
*/
template<class IDX, class V>
SIRE_INLINE_TEMPLATE
const QVector<V>&
GroupedVector<IDX,V>::at(const typename IndexInfo<IDX>::groupid_type &groupid) const
{
    typename hash_type::const_iterator it = vecdata.find(groupid);

    if (it == vecdata.end())
        IndexInfo<IDX>::invalidGroup(groupid);

    return *it;
}

/** Return the object at index 'index'. This will throw an exception if
    this is an invalid index.

    \throw invalid_group_error
    \throw invalid_index_error
*/
template<class IDX, class V>
SIRE_INLINE_TEMPLATE
const V& GroupedVector<IDX,V>::operator[](const IDX &idx) const
{
    return this->at(idx);
}

/** Return a modifiable reference to the object at index 'index'. This will
    throw an exception if this is an invalid index.

    \throw invalid_group_error
    \throw invalid_index_error
*/
template<class IDX, class V>
SIRE_INLINE_TEMPLATE
V& GroupedVector<IDX,V>::operator[](const IDX &idx)
{
    typename hash_type::iterator it = vecdata.find( IndexInfo<IDX>::groupID(idx) );

    if (it == vecdata.end())
        IndexInfo<IDX>::invalidGroup(idx);

    QVector<V> &vec = *it;

    Index i = IndexInfo<IDX>::index(idx);

    if (i >= vec.count())
        IndexInfo<IDX>::invalidIndex(idx, vec.count());

    return vec.data()[i];
}

/** Return the vector object objects in the group with ID 'groupid'. This will
    throw an exception if this is an invalid group ID

    \throw invalid_group_error
*/
template<class IDX, class V>
SIRE_INLINE_TEMPLATE
const QVector<V>&
GroupedVector<IDX,V>::operator[](const typename IndexInfo<IDX>::groupid_type &groupid) const
{
    return this->at(groupid);
}

/** Return a modifiable reference to the QVector holding the objects of the
    group at index 'groupid'. This will throw an exception for an invalid
    group.

    \throw invalid_group_error
*/
template<class IDX, class V>
QVector<V>&
SIRE_OUTOFLINE_TEMPLATE
GroupedVector<IDX,V>::operator[](const typename IndexInfo<IDX>::groupid_type &groupid)
{
    typename hash_type::iterator it = vecdata.find(groupid);

    if (it == vecdata.end())
        IndexInfo<IDX>::invalidGroup(groupid);

    return *it;
}

/** Return the value of the item at index 'idx'. This will return a default-constructed
    value if this is an invalid index */
template<class IDX, class V>
SIRE_OUTOFLINE_TEMPLATE
V GroupedVector<IDX,V>::value(const IDX &idx) const
{
    typename hash_type::const_iterator it = vecdata.find( IndexInfo<IDX>::groupID(idx) );

    if (it == vecdata.end())
        return V();

    Index i = IndexInfo<IDX>::index(idx);

    const QVector<V> &vec = *it;

    if (i < vec.count())
        return vec.data()[i];
    else
        return V();
}

/** Return the value of the item at index 'idx'. This will return 'defaultValue'
    if this is an invalid index */
template<class IDX, class V>
SIRE_OUTOFLINE_TEMPLATE
V GroupedVector<IDX,V>::value(const IDX &idx, const V &defaultValue) const
{
    typename hash_type::const_iterator it = vecdata.find( IndexInfo<IDX>::groupID(idx) );

    if (it == vecdata.end())
        return defaultValue;

    Index i = IndexInfo<IDX>::index(idx);

    const QVector<V> &vec = *it;

    if (i < vec.count())
        return vec.data()[i];
    else
        return defaultValue;
}

/** Return a vector of values in the group 'groupid', or an empty vector if this
    is an invalid group */
template<class IDX, class V>
SIRE_OUTOFLINE_TEMPLATE
QVector<V>
GroupedVector<IDX,V>::values(const typename IndexInfo<IDX>::groupid_type &groupid) const
{
    typename hash_type::const_iterator it = vecdata.find( groupid );

    if (it == vecdata.end())
        return QVector<V>();
    else
        return *it;
}

/** Return a vector of values in the group 'groupid' or 'defaultValues' if this
    is an invalid group */
template<class IDX, class V>
SIRE_OUTOFLINE_TEMPLATE
QVector<V>
GroupedVector<IDX,V>::values(const typename IndexInfo<IDX>::groupid_type &groupid,
                             const QVector<V> &defaultValues) const
{
    typename hash_type::const_iterator it = vecdata.find( groupid );

    if (it == vecdata.end())
        return defaultValues;
    else
        return *it;
}

/** Completely clear this GroupedVector */
template<class IDX, class V>
void GroupedVector<IDX,V>::clear()
{
    vecdata.clear();
}

/** Return the total number of objects in this GroupedVector */
template<class IDX, class V>
SIRE_OUTOFLINE_TEMPLATE
int GroupedVector<IDX,V>::count() const
{
    int n = 0;

    for (typename hash_type::const_iterator it = vecdata.begin();
         it != vecdata.end();
         ++it)
    {
        n += it->count();
    }

    return n;
}

/** Return the total number of objects in this GroupedVector */
template<class IDX, class V>
SIRE_INLINE_TEMPLATE
int GroupedVector<IDX,V>::size() const
{
    return this->count();
}

/** Return the total number of objects in the group with group ID 'groupid'.
    This will throw an exception if this is an invalid group.

    \throw invalid_group_error
*/
template<class IDX, class V>
SIRE_OUTOFLINE_TEMPLATE
int GroupedVector<IDX,V>::count(const typename IndexInfo<IDX>::groupid_type &groupid) const
{
    typename hash_type::const_iterator it = vecdata.find( groupid );

    if (it == vecdata.end())
        IndexInfo<IDX>::invalidGroup(groupid);

    return it->count();
}

/** Return the total number of objects in the group with group ID 'groupid'.
    This will throw an exception if this is an invalid group.

    \throw invalid_group_error
*/
template<class IDX, class V>
SIRE_INLINE_TEMPLATE
int GroupedVector<IDX,V>::size(const typename IndexInfo<IDX>::groupid_type &groupid) const
{
    return this->count(groupid);
}

/** Return whether or not this is an empty GroupedVector. Note that
    this is only empty if it has no groups - a GroupedVector filled
    with empty groups is not in itself empty. Use count() == 0 if
    you wish to test whether or not this has no contents */
template<class IDX, class V>
SIRE_INLINE_TEMPLATE
bool GroupedVector<IDX,V>::isEmpty() const
{
    return vecdata.isEmpty();
}

/** Return whether or not the group with ID 'groupid' is empty. This will
    throw an exception for an invalid group ID

    \throw invalid_group_error
*/
template<class IDX, class V>
SIRE_OUTOFLINE_TEMPLATE
bool GroupedVector<IDX,V>::isEmpty(
                      const typename IndexInfo<IDX>::groupid_type &groupid) const
{
    typename hash_type::const_iterator it = vecdata.find( groupid );

    if (it == vecdata.end())
        IndexInfo<IDX>::invalidGroup(groupid);

    return it->isEmpty();
}

/** Return whether or not this GroupedVector contains a group with ID 'groupid' */
template<class IDX, class V>
SIRE_INLINE_TEMPLATE
bool GroupedVector<IDX,V>::contains(
                      const typename IndexInfo<IDX>::groupid_type &groupid) const
{
    return vecdata.contains(groupid);
}

/** Return whether or not this GroupedVector contains an object at index 'idx'. */
template<class IDX, class V>
SIRE_INLINE_TEMPLATE
bool GroupedVector<IDX,V>::contains(const IDX &idx) const
{
    typename hash_type::const_iterator it = vecdata.find( IndexInfo<IDX>::groupID(idx) );

    return it != vecdata.end() and ( IndexInfo<IDX>::index(idx) < it->count() );
}

/** Return a pointer to the data contained in the vector of objects in the group
    'groupid'. This throws an exception if this is an invalid group ID

    \throw invalid_group_error
*/
template<class IDX, class V>
SIRE_OUTOFLINE_TEMPLATE
const V* GroupedVector<IDX,V>::constData(
                      const typename IndexInfo<IDX>::groupid_type &groupid) const
{
    typename hash_type::const_iterator it = vecdata.find( groupid );

    if (it == vecdata.end())
        IndexInfo<IDX>::invalidGroup(groupid);

    return it->constData();
}

/** Return a pointer to the data contained in the vector of objects in the group
    'groupid'. This throws an exception if this is an invalid group ID

    \throw invalid_group_error
*/
template<class IDX, class V>
SIRE_INLINE_TEMPLATE
const V* GroupedVector<IDX,V>::data(
                      const typename IndexInfo<IDX>::groupid_type &groupid) const
{
    return this->constData(groupid);
}

/** Return a raw, modifiable pointer to the vector of objects in the group
    'groupid'. This throws an exception if this is an invalid group ID

    \throw invalid_group_error
*/
template<class IDX, class V>
SIRE_OUTOFLINE_TEMPLATE
V* GroupedVector<IDX,V>::data(
                      const typename IndexInfo<IDX>::groupid_type &groupid)
{
    typename hash_type::index it = vecdata.find(groupid);

    if (it == vecdata.end())
        IndexInfo<IDX>::invalidGroup(groupid);

    return it->data();
}

/** Return the QVector holding the objects in the group indexed by ID 'groupid'.
    This throws an exception if this is an invalid ID

    \throw invalid_group_error
*/
template<class IDX, class V>
SIRE_INLINE_TEMPLATE
const QVector<V>& GroupedVector<IDX,V>::vector(
                      const typename IndexInfo<IDX>::groupid_type &groupid) const
{
    return this->at(groupid);
}

/** Return the QVector holding the objects in the group indexed by ID 'groupid'.
    This throws an exception if this is an invalid ID

    \throw invalid_group_error
*/
template<class IDX, class V>
SIRE_INLINE_TEMPLATE
const QVector<V>& GroupedVector<IDX,V>::constVector(
                      const typename IndexInfo<IDX>::groupid_type &groupid) const
{
    return this->at(groupid);
}

/** Return a modifiable reference to the vector that holds the objects indexed
    by the ID 'groupid'. This throws an exception if this is an invalid ID.

    \throw invalid_group_error
*/
template<class IDX, class V>
SIRE_INLINE_TEMPLATE
QVector<V>& GroupedVector<IDX,V>::vector(
                      const typename IndexInfo<IDX>::groupid_type &groupid)
{
    return this->operator[](groupid);
}

/** Reserve sufficient space for 'ngroups' groups. This may speed up
    allocation, though is mostly unnecessary. */
template<class IDX, class V>
SIRE_INLINE_TEMPLATE
void GroupedVector<IDX,V>::reserve(int ngroups)
{
    vecdata.reserve(ngroups);
}

/** Reserve sufficient space for 'n' objects in the group with group ID
    'groupid' - this will throw an exception if this is an invalid ID.

    \throw invalid_group_error
*/
template<class IDX, class V>
SIRE_OUTOFLINE_TEMPLATE
void GroupedVector<IDX,V>::reserve(
                      const typename IndexInfo<IDX>::groupid_type &groupid, int n)
{
    typename hash_type::iterator it = vecdata.find(groupid);

    if (it == vecdata.end())
        IndexInfo<IDX>::invalidGroup(groupid);

    it->reserve(n);
}

/** Resize the vector for the group at index 'id' to hold 'n' elements. This
    will delete elements if 'n' is less than the current size of the vector.

    This will throw an exception if this is an invalid ID.

    \throw invalid_group_error
*/
template<class IDX, class V>
SIRE_OUTOFLINE_TEMPLATE
void GroupedVector<IDX,V>::resize(
                      const typename IndexInfo<IDX>::groupid_type &groupid, int n)
{
    typename hash_type::iterator it = vecdata.find(groupid);

    if (it == vecdata.end())
        IndexInfo<IDX>::invalidGroup(groupid);

    it->resize(n);
}

/** Squeeze this GroupedVector - this will minimise the memory usage of this
    GroupedVector */
template<class IDX, class V>
SIRE_OUTOFLINE_TEMPLATE
void GroupedVector<IDX,V>::squeeze()
{
    for (typename hash_type::iterator it = vecdata.begin();
         it != vecdata.end();
         ++it)
    {
        it->squeeze();
    }

    vecdata.squeeze();
}

/** Squeeze the QVector holding the objects in the group indexed by group
    with ID 'groupid'. This will throw an exception if this is an invalid ID.

    \throw invalid_group_error
*/
template<class IDX, class V>
SIRE_OUTOFLINE_TEMPLATE
void GroupedVector<IDX,V>::squeeze(
                      const typename IndexInfo<IDX>::groupid_type &groupid)
{
    typename hash_type::iterator it = vecdata.find(groupid);

    if (it == vecdata.end())
        IndexInfo<IDX>::invalidGroup(groupid);

    it->squeeze();
}

/** Insert the vector of values into this GroupedVector, indexed by ID 'groupid'.
    This will replace any existing values with this ID */
template<class IDX, class V>
SIRE_INLINE_TEMPLATE
void GroupedVector<IDX,V>::insert(
                      const typename IndexInfo<IDX>::groupid_type &groupid,
                      const QVector<V> &values)
{
    vecdata.insert(groupid,values);
}

/** Remove the vector of values indexed by the group ID 'groupid'. This does nothing
    if there isn't such a group in this GroupedVector. */
template<class IDX, class V>
SIRE_INLINE_TEMPLATE
void GroupedVector<IDX,V>::remove(
                      const typename IndexInfo<IDX>::groupid_type &groupid)
{
    vecdata.remove(groupid);
}

/** Remove the vector of values indexed by the group ID 'groupid' and return it.
    This will throw an exception if this is an invalid ID number.

    \throw invalid_group_error
*/
template<class IDX, class V>
SIRE_OUTOFLINE_TEMPLATE
QVector<V> GroupedVector<IDX,V>::take(
                      const typename IndexInfo<IDX>::groupid_type &groupid)
{
    if ( not vecdata.contains(groupid) )
        IndexInfo<IDX>::invalidGroup(groupid);

    return vecdata.take(groupid);
}

/** Append the value 'value' onto the end of the group with ID 'groupid'.
    This will automatically create the group if it doesn't already exist. */
template<class IDX, class V>
SIRE_INLINE_TEMPLATE
void GroupedVector<IDX,V>::append(
                      const typename IndexInfo<IDX>::groupid_type &groupid,
                      const V &value)
{
    vecdata[groupid].append(value);
}

/** Insert the item 'value' into the GroupedVector at index 'idx'. */
template<class IDX, class V>
SIRE_INLINE_TEMPLATE
void GroupedVector<IDX,V>::insert(const IDX &idx, const V &value)
{
    vecdata[ IndexInfo<IDX>::groupID(idx) ]
                  .insert( IndexInfo<IDX>::index(idx), value );
}

/** Remove the item at index 'idx' - this does nothing if this index is invalid.

    This removes the group if this was the last item in the group.
*/
template<class IDX, class V>
SIRE_OUTOFLINE_TEMPLATE
void GroupedVector<IDX,V>::remove(const IDX &idx)
{
    typename hash_type::iterator it = vecdata.find( IndexInfo<IDX>::groupID(idx) );

    if (it != vecdata.end())
    {
        it->remove( IndexInfo<IDX>::index(idx) );
        if (it->count() == 0)
            vecdata.erase(it);
    }
}

static const MagicID grouped_vector_magic =
                                    getMagic("SireMol::GroupedVector");

}

/** Serialise a GroupedVector to a QDataStream */
template<class IDX, class V>
SIRE_OUTOFLINE_TEMPLATE
QDataStream& operator<<(QDataStream &ds, const SireMol::GroupedVector<IDX,V> &vec)
{
    SireStream::writeHeader(ds, SireMol::grouped_vector_magic, 1) << vec.vecdata;

    return ds;
}

/** Deserialise a GroupedVector from a QDataStream */
template<class IDX, class V>
SIRE_OUTOFLINE_TEMPLATE
QDataStream& operator>>(QDataStream &ds, SireMol::GroupedVector<IDX,V> &vec)
{
    SireStream::VersionID v = SireStream::readHeader(ds, SireMol::grouped_vector_magic,
                                                     "SireMol::GroupedVector");

    if (v == 1)
    {
        ds >> vec.vecdata;
    }
    else
        throw SireStream::version_error(v, "1", "SireMol::GroupedVector", CODELOC);

    return ds;
}

SIRE_END_HEADER

#endif
