#ifndef SIREDB_PARAMETERSTORE_HPP
#define SIREDB_PARAMETERSTORE_HPP

#include <QHash>
#include <QDebug>
#include <QVector>
#include <QSet>
#include <QDataStream>

#include <boost/tuple/tuple.hpp>

#include "SireMol/groupedvector.hpp"

#include "SireDB/errors.h"
#include "SireError/errors.h"

#include "SireStream/datastream.h"

SIRE_BEGIN_HEADER

namespace SireDB
{
template<class IDX, class Param>
class ParameterStore;
}

template<class IDX, class Param>
QDataStream& operator<<(QDataStream&, const SireDB::ParameterStore<IDX,Param>&);
template<class IDX, class Param>
QDataStream& operator>>(QDataStream&, SireDB::ParameterStore<IDX,Param>&);

namespace SireDB
{

using SireMol::Index;
using SireMol::IndexInfo;
using SireMol::GroupIndexID;

/** This is a store of parameters, that records not only the parameters
    mapped to the index type 'IDX' (a double index, e.g. GroupIndexID,
    CGAtomID etc.), but also whether or not particular parameters
    have been assigned.

    This class is closely designed with InternalTable and AtomTable,
    and is not intended for use by any other parts of the code.

    \param  Param      The type of parameter stored in this ParameterStore

    \param  IDX        The type of the index. This must be a double index,
                       with the second index being of type 'Index'.
                       There must be an IndexInfo<IDX> class available
                       for this index type.

    @author Christopher Woods
*/
template<class IDX, class Param>
class SIREDB_EXPORT ParameterStore
{

friend QDataStream& ::operator<<<>(QDataStream&, const ParameterStore<IDX,Param>&);
friend QDataStream& ::operator>><>(QDataStream&, ParameterStore<IDX,Param>&);

public:
    typedef Param parameter_type;
    typedef Param value_type;

    typedef typename IndexInfo<IDX>::groupid_type groupid_type;
    typedef IDX index_type;
    typedef typename IndexInfo<IDX>::invalid_group_error invalid_group_error;
    typedef typename IndexInfo<IDX>::invalid_index_error invalid_index_error;

    ParameterStore();
    ParameterStore(const QHash<groupid_type,quint32> &nparams);

    ParameterStore(const ParameterStore<IDX,Param> &other);

    ~ParameterStore();

    bool operator==(const ParameterStore<IDX,Param> &other) const;
    bool operator!=(const ParameterStore<IDX,Param> &other) const;

    ParameterStore<IDX,Param>& operator=(const ParameterStore<IDX,Param> &other);

    int nParameters() const;
    int nParameters(const groupid_type &groupid) const;
    int nParameters(const QSet<groupid_type> &groupids) const;

    bool contains(const groupid_type &groupid) const;

    bool hasMissingParameters() const;
    bool hasMissingParameters(const groupid_type &groupid) const;
    bool hasMissingParameters(const QSet<groupid_type> &groupids) const;

    QSet<index_type> missingParameters() const;
    QSet<index_type> missingParameters(const groupid_type &groupid) const;
    QSet<index_type> missingParameters(const QSet<groupid_type> &groupids) const;

    void reserve(int ngroups);
    void squeeze();

    void resize(const groupid_type &index, int nparams);

    bool assignedParameter(const index_type &index) const;

    void setParameter(const index_type &index, const Param &param);

    const Param& getParameter(const index_type &index) const;

    const Param& at(const index_type &index) const;

    Param& operator[](const index_type &index);

    const Param& operator[](const index_type &index) const;

    Param value(const index_type &index) const;
    Param value(const index_type &index, const Param &defaultValue) const;

    void clear();
    void clear(const groupid_type &groupid);
    void clear(const QSet<groupid_type> &groupid);
    void clear(const index_type &index);

    void remove(const index_type &index);
    void remove(const groupid_type &groupid);
    void remove(const QSet<groupid_type> &groupids);

    void removeAll();

    void removeMissing(const groupid_type &groupid);
    void removeMissing(const QSet<groupid_type> &groupids);

    void removeAllMissing();

    void addParameter(const index_type &index);
    void addParameter(const index_type &index, const Param &param);

    QVector<Param> parameters(const groupid_type &groupid) const;

    template<class C>
    uint parameters(const groupid_type &groupid,
                    QVector<C> &param_vector, uint strt=0) const;

private:
    typedef QHash< groupid_type, QHash<Index,Param> > hash_type;
    typedef QHash< groupid_type, quint32 > nhash_type;

    QString missingString(const index_type &index) const;

    /** Double-hash containing the parameters */
    hash_type params;

    /** The expected number of parameters in each group */
    nhash_type nparams;
};

/** Empty constructor */
template<class IDX, class Param>
SIRE_OUTOFLINE_TEMPLATE
ParameterStore<IDX,Param>::ParameterStore()
{}

/** Construct a ParameterStore that holds the parameters for the groups described
    in 'n' - this is a hash of the IDs of all of the groups, and the number
    of parameters in each group */
template<class IDX, class Param>
SIRE_OUTOFLINE_TEMPLATE
ParameterStore<IDX,Param>::ParameterStore(
                      const QHash<ParameterStore<IDX,Param>::groupid_type,quint32> &n)
      : nparams(n)
{
    //create space for all of the parameters
    for (typename nhash_type::const_iterator it = n.begin();
         it != n.end();
         ++it)
    {
        QHash<Index,Param> groupparams;
        groupparams.reserve(*it);

        params.insert( it.key(), groupparams );
    }
}

/** Copy constructor */
template<class IDX, class Param>
SIRE_OUTOFLINE_TEMPLATE
ParameterStore<IDX,Param>::ParameterStore(const ParameterStore<IDX,Param> &other)
                          : params(other.params), nparams(other.nparams)
{}

/** Destructor */
template<class IDX, class Param>
SIRE_OUTOFLINE_TEMPLATE
ParameterStore<IDX,Param>::~ParameterStore()
{}

/** Comparison operator */
template<class IDX, class Param>
SIRE_INLINE_TEMPLATE
bool ParameterStore<IDX,Param>::operator==(const ParameterStore<IDX,Param> &other) const
{
    return params == other.params and nparams == other.nparams;
}

/** Comparison operator */
template<class IDX, class Param>
SIRE_INLINE_TEMPLATE
bool ParameterStore<IDX,Param>::operator!=(const ParameterStore<IDX,Param> &other) const
{
    return params != other.params or nparams != other.nparams;
}

/** Assignment operator */
template<class IDX, class Param>
SIRE_OUTOFLINE_TEMPLATE
ParameterStore<IDX,Param>&
ParameterStore<IDX,Param>::operator=(const ParameterStore<IDX,Param> &other)
{
    params = other.params;
    nparams = other.nparams;
    return *this;
}

/** Return a string that describes the missing index 'idx' */
template<class IDX, class Param>
SIRE_OUTOFLINE_TEMPLATE
QString ParameterStore<IDX,Param>::missingString(const IDX &idx) const
{
    return QString("Missing index %1:%2")
              .arg( IndexInfo<IDX>::groupID(idx).toString(),
                    IndexInfo<IDX>::index(idx).toString() );
}

/** Return the number of assigned parameters in this store */
template<class IDX, class Param>
SIRE_OUTOFLINE_TEMPLATE
int ParameterStore<IDX,Param>::nParameters() const
{
    int n = 0;

    for (typename hash_type::const_iterator it = params.begin();
         it != params.end();
         ++it)
    {
        n += it->count();
    }

    return n;
}

/** Return the number of assigned parameters in the group with ID 'groupid'.

    This will throw an exception if this is an invalid group ID.

    \throw invalid_group_error
*/
template<class IDX, class Param>
SIRE_OUTOFLINE_TEMPLATE
int ParameterStore<IDX,Param>::nParameters(
                    const ParameterStore<IDX,Param>::groupid_type &groupid) const
{
    typename hash_type::const_iterator it = params.find(groupid);

    if (it == params.end())
        IndexInfo<IDX>::invalidGroup(groupid);

    return it->count();
}

/** Return the number of assigned parameters in the groups with IDs in 'groupids'.

    This will throw an exception if this is an invalid group ID.

    \throw invalid_group_error
*/
template<class IDX, class Param>
SIRE_OUTOFLINE_TEMPLATE
int ParameterStore<IDX,Param>::nParameters(
                   const QSet<ParameterStore<IDX,Param>::groupid_type> &groupids) const
{
    int n = 0;

    typename hash_type::const_iterator group_it;

    for (typename QSet<groupid_type>::const_iterator it = groupids.begin();
         it != groupids.end();
         ++it)
    {
        group_it = params.find(*it);

        if (group_it != params.end())
            n += group_it->count();
    }

    return n;
}

/** Return whether or not this store contains the group with ID 'groupid' */
template<class IDX, class Param>
SIRE_INLINE_TEMPLATE
bool ParameterStore<IDX,Param>::contains(
                const ParameterStore<IDX,Param>::groupid_type &groupid) const
{
    return params.find(groupid) != params.end();
}

/** Return whether or not there are any missing parameters in this store */
template<class IDX, class Param>
SIRE_OUTOFLINE_TEMPLATE
bool ParameterStore<IDX,Param>::hasMissingParameters() const
{
    //loop over all groups...
    for (typename hash_type::const_iterator it = params.begin();
         it != params.end();
         ++it)
    {
        //compare the number of assigned parameters against the
        //expected number of assigned parameters - these should match
        //up when all of the parameters have been assigned...
        if ( uint(it->count()) != nparams.value(it.key()) )
            return true;
    }

    return false;
}

/** Return whether or not there are any missing parameters in the group
    with ID 'groupid'. This will throw an exception if there is no
    group with this ID.

    \throw invalid_group_error
*/
template<class IDX, class Param>
SIRE_OUTOFLINE_TEMPLATE
bool ParameterStore<IDX,Param>::hasMissingParameters(
                            const ParameterStore<IDX,Param>::groupid_type &groupid) const
{
    typename hash_type::const_iterator it = params.find(groupid);

    if (it == params.end())
        IndexInfo<IDX>::invalidGroup(groupid);

    //compare the number of assigned parameters to the expected
    //number of assigned parameters
    return uint(it->count()) == nparams.value(it.key());
}

/** Return whether or not there are any missing parameters in the groups
    whose IDs are in 'groupids'. This will throw an exception if there is no
    group with these IDs.

    \throw invalid_group_error
*/
template<class IDX, class Param>
SIRE_OUTOFLINE_TEMPLATE
bool ParameterStore<IDX,Param>::hasMissingParameters(
                    const QSet<ParameterStore<IDX,Param>::groupid_type> &groupids) const
{
    foreach( groupid_type groupid, groupids )
    {
        if (this->hasMissingParameters(groupid))
            return true;
    }

    return false;
}

/** Return the set of indexes of all of the missing parameters in the group with
    group ID 'groupid'. This will be an empty set if there are no missing
    parameters. This will throw an exception if the group ID is invalid.

    \throw invalid_group_error
*/
template<class IDX, class Param>
SIRE_OUTOFLINE_TEMPLATE
QSet<IDX> ParameterStore<IDX,Param>::missingParameters(
                          const ParameterStore<IDX,Param>::groupid_type &groupid) const
{
    typename hash_type::const_iterator it = params.find(groupid);

    if (it == params.end())
        IndexInfo<IDX>::invalidGroup(groupid);

    int ngot = it->count();
    int nexpect = nparams.value(it.key());

    BOOST_ASSERT(ngot <= nexpect);

    //are there the expected number of assigned parameters?
    if ( ngot == nexpect )
        //there are no missing parameters
        return QSet<IDX>();
    else
    {
        //how many missing parameters are there?
        int nmissing = nexpect - ngot;

        QSet<IDX> missing;

        //The parameters are indexed from 0 -> nexpect-1.
        //Loop through these indexes and record those that
        //are missing
        const QHash<Index,Param> &paramhash = *it;

        for (Index i(0); i<nexpect; ++i)
        {
            if ( not paramhash.contains(i) )
            {
                missing.insert( IDX(it.key(),i) );
                nmissing --;

                if (nmissing == 0)
                    //we have found all of the missing parameters
                    //shortcut the loop :-)
                    return missing;
            }
        }

        //we should never get here!
        BOOST_ASSERT( nmissing == 0 );
        return missing;
    }
}

/** Return the set of indexes of all of the missing parameters. This will
    be an empty set if there are no missing parameters */
template<class IDX, class Param>
SIRE_OUTOFLINE_TEMPLATE
QSet<IDX> ParameterStore<IDX,Param>::missingParameters() const
{
    QSet<IDX> idxs;

    //loop over all of the groups
    for (typename hash_type::const_iterator it = params.begin();
         it != params.end();
         ++it)
    {
        //add the missing parameters to the set
        idxs += this->missingParameters(it.key());
    }

    return idxs;
}

/** Return the set of indexes of all of the missing parameters in the groups
    with IDs in 'groupids'. This will be an empty set if there are no missing
    parameters. This will throw an exception if any of the group IDs are invalid.

    \throw invalid_group_error
*/
template<class IDX, class Param>
SIRE_OUTOFLINE_TEMPLATE
QSet<IDX> ParameterStore<IDX,Param>::missingParameters(
                    const QSet<ParameterStore<IDX,Param>::groupid_type> &groupids) const
{
    QSet<IDX> idxs;

    for (typename QSet<groupid_type>::const_iterator it = groupids.begin();
         it != groupids.end();
         ++it)
    {
        idxs += this->missingParameters(*it);
    }

    return idxs;
}

/** Reserve space for ngroups groups. */
template<class IDX, class Param>
SIRE_INLINE_TEMPLATE
void ParameterStore<IDX,Param>::reserve(int ngroups)
{
    params.reserve(ngroups);
}

/** Squeeze the store so that it uses the minimum amount of memory necessary */
template<class IDX, class Param>
SIRE_INLINE_TEMPLATE
void ParameterStore<IDX,Param>::squeeze()
{
    params.squeeze();
}

/** Resize the store so that the group indexed by ID 'groupid' uses 'n' parameters.
    This will delete any parameters that were greater than 'n'.

    This throws an exception if 'groupid' is invalid.

    \throw invalid_group_error
*/
template<class IDX, class Param>
SIRE_OUTOFLINE_TEMPLATE
void ParameterStore<IDX,Param>::resize(
                    const ParameterStore<IDX,Param>::groupid_type &groupid, int n)
{
    typename hash_type::iterator it = params.find(groupid);

    if (it == params.end())
        IndexInfo<IDX>::invalidGroup(groupid);

    int current_size = nparams.value(it.key());

    if (n == 0)
    {
        //remove this group
        params.erase(it);
        nparams.remove(groupid);
    }
    else if (current_size < n)
    {
        //we will be adding some parameters - reserve some extra space
        it->reserve(n);
        nparams.insert(groupid,n);
    }
    else
    {
        //get a reference to the group
        QHash<Index,Param> &grouphash = *it;

        //we are removing parameters with indexes greater than n-1
        typename QHash<Index,Param>::iterator group_it = grouphash.begin();

        while ( group_it != grouphash.end() )
        {
            if (group_it.key() >= n)
                //remove this parameter
                group_it = grouphash.erase(group_it);
            else
                ++group_it;
        }

        nparams.insert(groupid,n);
    }
}

/** Return whether or not the parameter for index 'idx' has been assigned.

    This will throw an exception if this is an invalid index.

    \throw invalid_group_error
    \throw invalid_index_error
*/
template<class IDX, class Param>
SIRE_OUTOFLINE_TEMPLATE
bool ParameterStore<IDX,Param>::assignedParameter(const IDX &idx) const
{
    typename hash_type::const_iterator it = params.find( IndexInfo<IDX>::groupID(idx) );

    if (it == params.end())
        IndexInfo<IDX>::invalidGroup(idx);

    return it->contains( IndexInfo<IDX>::index(idx) );
}

/** Set the value of the parameter at index 'idx' to 'param'. This will throw
    an exception if this is an invalid index. This will overwrite the value
    of the parameter if it had already been assigned.

    \throw invalid_group_error
    \throw invalid_index_error
*/
template<class IDX, class Param>
SIRE_OUTOFLINE_TEMPLATE
void ParameterStore<IDX,Param>::setParameter(const IDX &idx, const Param &param)
{
    groupid_type groupid = IndexInfo<IDX>::groupID(idx);

    typename hash_type::iterator it = params.find(groupid);

    if (it == params.end())
        IndexInfo<IDX>::invalidGroup(idx);

    //is this a valid index?
    Index i = IndexInfo<IDX>::index(idx);

    if (i >= nparams.value(groupid))
        IndexInfo<IDX>::invalidIndex(idx, nparams.value(groupid));

    //set the parameter
    it->insert( i, param );
}

/** Return the parameter at index 'idx'. This will throw an exception if
    either the parameter has not yet been assigned, or if this is an invalid index.

    \throw SireDB::missing_parameter
    \throw invalid_group_error
    \throw invalid_index_error
*/
template<class IDX, class Param>
SIRE_OUTOFLINE_TEMPLATE
const Param& ParameterStore<IDX,Param>::getParameter(const IDX &idx) const
{
    groupid_type groupid = IndexInfo<IDX>::groupID(idx);

    typename hash_type::const_iterator it = params.find(groupid);

    if (it == params.end())
        IndexInfo<IDX>::invalidGroup(idx);

    Index i = IndexInfo<IDX>::index(idx);

    if (i >= nparams.value(groupid))
        IndexInfo<IDX>::invalidIndex(idx, nparams.value(groupid));

    //has the parameter been assigned?
    typename QHash<Index,Param>::const_iterator group_it = it->find(i);

    if (group_it == it->end())
        throw SireDB::missing_parameter(missingString(idx), CODELOC);

    return *group_it;
}

/** Return the parameter at index 'idx'. This will throw an exception if
    either the parameter has not yet been assigned, or if this is an invalid index.

    \throw SireDB::missing_parameter
    \throw invalid_group_error
    \throw invalid_index_error
*/
template<class IDX, class Param>
SIRE_INLINE_TEMPLATE
const Param& ParameterStore<IDX,Param>::at(const IDX &idx) const
{
    return this->getParameter(idx);
}

/** Return a modifiable reference to the parameter at index 'idx'. This will
    create a default parameter if it hasn't already been assigned. This will
    throw an exception if idx is invalid.

    \throw invalid_group_error
    \throw invalid_index_error
*/
template<class IDX, class Param>
SIRE_OUTOFLINE_TEMPLATE
Param& ParameterStore<IDX,Param>::operator[](const IDX &idx)
{
    groupid_type groupid = IndexInfo<IDX>::groupID(idx);

    typename hash_type::iterator it = params.find(groupid);

    if (it == params.end())
        IndexInfo<IDX>::invalidGroup(idx);

    Index i = IndexInfo<IDX>::index(idx);

    if (i >= nparams.value(groupid))
        IndexInfo<IDX>::invalidIndex(idx, nparams.value(groupid));

    return (*it)[i];
}

/** Return the parameter at index 'idx'. This will throw an exception if
    either the parameter has not yet been assigned, or if this is an invalid index.

    \throw SireDB::missing_parameter
    \throw invalid_group_error
    \throw invalid_index_error
*/
template<class IDX, class Param>
SIRE_INLINE_TEMPLATE
const Param& ParameterStore<IDX,Param>::operator[](const IDX &idx) const
{
    return this->getParameter(idx);
}

/** Return the parameter at index 'idx'. This will return a default-constructed
    parameter if one has not yet been assigned to this index. This will throw
    an exception if this is an invalid index.

    \throw invalid_group_error
    \throw invalid_index_error
*/
template<class IDX, class Param>
SIRE_OUTOFLINE_TEMPLATE
Param ParameterStore<IDX,Param>::value(const IDX &idx) const
{
    groupid_type groupid = IndexInfo<IDX>::groupID(idx);

    typename hash_type::const_iterator it = params.find(groupid);

    if (it == params.end())
        IndexInfo<IDX>::invalidGroup(idx);

    Index i = IndexInfo<IDX>::index(idx);

    if (i >= nparams.value(groupid))
        IndexInfo<IDX>::invalidIndex(idx, nparams.value(groupid));

    return it->value(i);
}

/** Return the parameter at index 'idx'. This will return defaultValue
    if one has not yet been assigned to this index. This will throw
    an exception if this is an invalid index.

    \throw invalid_group_error
    \throw invalid_index_error
*/
template<class IDX, class Param>
SIRE_OUTOFLINE_TEMPLATE
Param ParameterStore<IDX,Param>::value(const IDX &idx, const Param &defaultValue) const
{
    groupid_type groupid = IndexInfo<IDX>::groupID(idx);

    typename hash_type::const_iterator it = params.find(groupid);

    if (it == params.end())
        IndexInfo<IDX>::invalidGroup(idx);

    Index i = IndexInfo<IDX>::index(idx);

    if (i >= nparams.value(groupid))
        IndexInfo<IDX>::invalidIndex(idx, nparams.value(groupid));

    return it->value(i, defaultValue);
}

/** Clear all of the parameters that have been assigned. This only
    clears the parameters - it doesn't remove the space for the parameters.
    Use ParameterStore::removeAll() to completely clear this object.
*/
template<class IDX, class Param>
SIRE_OUTOFLINE_TEMPLATE
void ParameterStore<IDX,Param>::clear()
{
    for (typename hash_type::iterator it = params.begin();
         it != params.end();
         ++it)
    {
        it->clear();
        it->reserve( nparams.value(it.key()) );
    }
}

/** Clear all of the parameters for the group with ID 'groupid'. This only
    clears the parameters - it doesn' remove the space for the parameters.
    Use ParameterStore::remove(groupid) to completely clear this group.

    This will throw an exception if this is an invalid group

    \throw invalid_group_error
*/
template<class IDX, class Param>
SIRE_OUTOFLINE_TEMPLATE
void ParameterStore<IDX,Param>::clear(
                      const ParameterStore<IDX,Param>::groupid_type &groupid)
{
    typename hash_type::iterator it = params.find(groupid);

    if (it == params.end())
        IndexInfo<IDX>::invalidGroup(groupid);

    it->clear();
    it->reserve( nparams.value(it.key()) );
}

/** Clear the parameters for the groups whose IDs are in 'groupids'. This
    only clears the parameters - it doesn' remove the space for the parameters.
    Use ParameterStore::remove(groupids) to completely clear this group.

    This will throw an exception if this is an invalid group

    \throw invalid_group_error
*/
template<class IDX, class Param>
SIRE_OUTOFLINE_TEMPLATE
void ParameterStore<IDX,Param>::clear(
                  const QSet<ParameterStore<IDX,Param>::groupid_type> &groupids)
{
    for (typename QSet<groupid_type>::const_iterator it = groupids.begin();
         it != groupids.end();
         ++it)
    {
        this->clear(*it);
    }
}

/** Clear the parameter for the index 'idx'. This only clears the parameter,
    it doesn't remove the place for the parameter. To completely clear the
    parameter use ParameterStore::remove(idx).

    This will throw an exception if this is an invalid index

    \throw invalid_group_error
    \throw invalid_index_error
*/
template<class IDX, class Param>
SIRE_OUTOFLINE_TEMPLATE
void ParameterStore<IDX,Param>::clear(const IDX &idx)
{
    groupid_type groupid = IndexInfo<IDX>::groupID(idx);

    typename hash_type::iterator it = params.find(groupid);

    if (it == params.end())
        IndexInfo<IDX>::invalidGroup(idx);

    Index i = IndexInfo<IDX>::index(idx);

    if (i >= nparams.value(groupid))
        IndexInfo<IDX>::invalidIndex(idx, nparams.value(groupid));

    it->remove(i);
}

/** Completely remove the parameter at index 'idx'. This will cause all of
    the parameters in the same group as this parameter to be reindexed.

    This will throw an exception if this is an invalid index.

    \throw invalid_group_error
    \throw invalid_index_error
*/
template<class IDX, class Param>
SIRE_OUTOFLINE_TEMPLATE
void ParameterStore<IDX,Param>::remove(const IDX &idx)
{
    groupid_type groupid = IndexInfo<IDX>::groupID(idx);

    typename hash_type::iterator it = params.find(groupid);

    if (it == params.end())
        IndexInfo<IDX>::invalidGroup(idx);

    Index i = IndexInfo<IDX>::index(idx);

    if (i >= nparams.value(groupid))
        IndexInfo<IDX>::invalidIndex(idx, nparams.value(groupid));

    //remove this parameter and reindex all of the parameters
    //with indexes that are greater than this
    QHash<Index,Param> &group_hash = *it;

    //remove the parameter
    group_hash.remove(i);

    //reindex the remaining parameters
    int n = nparams.value(groupid);
    for (i = Index(i+1) ; i < n; ++i)
    {
        if (group_hash.contains(i))
        {
            //remove the item from its old position
            Param param = group_hash.take(i);

            //insert it at the new index
            group_hash.insert( Index(i-1), param );
        }
    }

    //if this was the last parameter then remove the entire group
    if (n == 1)
    {
        params.remove(groupid);
        nparams.remove(groupid);
    }
    else
        //reduce the number of parameters in the group
        nparams.insert( groupid, n-1 );
}

/** Completely remove all of the parameters and space for the group
    with ID 'groupid'. This will throw an exception if this is
    an invalid ID

    \throw invalid_group_error
*/
template<class IDX, class Param>
SIRE_OUTOFLINE_TEMPLATE
void ParameterStore<IDX,Param>::remove(
                    const ParameterStore<IDX,Param>::groupid_type &groupid)
{
    typename hash_type::iterator it = params.find(groupid);

    if (it == params.end())
        IndexInfo<IDX>::invalidGroup(groupid);

    params.erase(it);
    nparams.remove(groupid);
}

/** Completely remove the parameters from the groups with IDs in 'groupids'.
    This will throw an exception if any of the IDs are invalid (in which
    case no groups would be removed)

    \throw invalid_group_error
*/
template<class IDX, class Param>
SIRE_OUTOFLINE_TEMPLATE
void ParameterStore<IDX,Param>::remove(
                  const QSet<ParameterStore<IDX,Param>::groupid_type> &groupids)
{
    //check that all of the groups are valid...
    for (typename QSet<groupid_type>::const_iterator it = groupids.begin();
         it != groupids.end();
         ++it)
    {
        if (params.constFind(*it) == params.constEnd())
            IndexInfo<IDX>::invalidGroup(*it);
    }

    //now remove all of the groups
    for (typename QSet<groupid_type>::const_iterator it = groupids.begin();
         it != groupids.end();
         ++it)
    {
        params.remove( *it );
        nparams.remove( *it );
    }
}

/** Remove all parameters, and all spaces for parameters. This will now
    equal the empty, null ParameterStore */
template<class IDX, class Param>
SIRE_OUTOFLINE_TEMPLATE
void ParameterStore<IDX,Param>::removeAll()
{
    params.clear();
    nparams.clear();
}

/** Add space for the parameter at index 'index'. This does nothing if there
    is already space for this parameter. Note that this will increase the number
    of parameters needed until they are equal to the Index of idx, and will
    also automatically insert the group. */
template<class IDX, class Param>
SIRE_OUTOFLINE_TEMPLATE
void ParameterStore<IDX,Param>::addParameter(const IDX &idx)
{
    groupid_type groupid = IndexInfo<IDX>::groupID(idx);
    Index i = IndexInfo<IDX>::index(idx);

    //get the hash containing the parameters for this group -
    // this is automatically created if it doesn't already exist
    QHash<Index,Param> &group_hash = params[groupid];

    //only need to do something if space for the parameter
    //doesn't already exist
    if (i >= nparams.value(groupid))
    {
        //increase 'nparams' to equal i+1
        nparams.insert( groupid, i+1 );

        //reserve sufficient space for the parameters in this group
        group_hash.reserve(i+1);
    }
}

/** Add space for the parameter at index 'index'. This does nothing if there
    is already space for this parameter. Note that this will increase the number
    of parameters needed until they are equal to the Index of idx, and will
    also automatically insert the group. Once space has been made, then
    assign it to have parameter 'param' */
template<class IDX, class Param>
SIRE_INLINE_TEMPLATE
void ParameterStore<IDX,Param>::addParameter(const IDX &idx, const Param &param)
{
    this->addParameter(idx);
    this->setParameter(idx, param);
}

/** Remove all missing parameters from the group with ID 'groupid. This will
    contract the store by removing the space for each missing parameter
    and reindexing, e.g. if parameters a->g currently have indexes;

    1==a, 5==b, 6==c, 7==d, 9==e, 12==f, 13==g

    then after removing the missing parameters (2,3,4,8,10,11) then the
    parameters will now have the indexes;

    1==a, 2==b, 3==c, 4==d, 5==e, 6==f, 7==g

    This will throw an exception if this is an invalid groupid.

    \throw invalid_group_error
*/
template<class IDX, class Param>
SIRE_OUTOFLINE_TEMPLATE
void ParameterStore<IDX,Param>::removeMissing(
                      const ParameterStore<IDX,Param>::groupid_type &groupid)
{
    //get the missing parameter indexes...
    QSet<IDX> missing = this->missingParameters(groupid);

    if (missing.isEmpty())
        //no parameters are missing :-)
        return;

    //sort the missing parameter indexes in increasing order...
    QList<quint32> idxs;

    for (typename QSet<IDX>::const_iterator it = missing.constBegin();
         it != missing.constEnd();
         ++it)
    {
        idxs.append( IndexInfo<IDX>::index( *it ).index() );
    }

    //actually sort the indexes...
    qSort( idxs.begin(), idxs.end() );

    //get the hash containing the parameters for this group
    QHash<Index,Param> &group_hash = params[groupid];

    //append the number of objects onto the list of missing indexes -
    //this is necessary for the below algorithm - before doing so,
    //record the number of missing parameters
    int nmissing = idxs.count();

    idxs.append( nparams.value(groupid) );

    for (int i=0; i<nmissing; ++i)
    {
        //reindex the items from idxs[i]+1 to idxs[i+1], by reducing
        //their index by i+1
        for (quint32 j=idxs[i]+1; j<idxs[j+1]; ++j)
        {
            BOOST_ASSERT(group_hash.contains(j));

            Param param = group_hash.take(j);
            group_hash.insert( j-i-1, param );
        }
    }

    //how many parameters are now left in the group
    int nremaining = idxs[nmissing] - nmissing;

    if (nremaining == 0)
        //remove this group
        params.remove(groupid);
    else
        //record the new amount
        nparams.insert( groupid, nremaining );
}

/** Remove all of the missing parameters from the groups with IDs in 'groupids'.
    This will throw an exception if any of the group IDs are invalid
    (in which case none of the missing parameters will be removed)

    \throw invalid_group_error
*/
template<class IDX, class Param>
SIRE_OUTOFLINE_TEMPLATE
void ParameterStore<IDX,Param>::removeMissing(
                        const QSet<ParameterStore<IDX,Param>::groupid_type> &groupids)
{
    //check that there are no invalid group ID numbers
    for (typename QSet<groupid_type>::const_iterator it = groupids.begin();
         it != groupids.end();
         ++it)
    {
        if (not params.contains(*it))
            IndexInfo<IDX>::invalidGroup(*it);
    }

    //now remove the missing parameters
    for (typename QSet<groupid_type>::const_iterator it = groupids.begin();
         it != groupids.end();
         ++it)
    {
        this->removeMissing( *it );
    }
}

/** Remove all missing parameters from this store */
template<class IDX, class Param>
SIRE_OUTOFLINE_TEMPLATE
void ParameterStore<IDX,Param>::removeAllMissing()
{
    QList<groupid_type> groupids = params.keys();

    for (typename QList<groupid_type>::const_iterator it = groupids.constBegin();
         it != groupids.constEnd();
         ++it)
    {
        this->removeMissing( *it );
    }
}

/** Return a vector of all of the parameters in the group with ID 'groupid'.
    This will throw an exception if this is an invalid ID, or if there are
    any missing parameters in this group.

    \throw SireDB::missing_parameter
    \throw invalid_group_error
*/
template<class IDX, class Param>
SIRE_OUTOFLINE_TEMPLATE
QVector<Param> ParameterStore<IDX,Param>::parameters(
                        const ParameterStore<IDX,Param>::groupid_type &groupid) const
{
    //are there any missing parameters in this group?
    if (this->hasMissingParameters(groupid))
        //throw an exception containing all of the missing parameters...
        throw SireDB::missing_parameter( missingString(groupid), CODELOC );

    QVector<Param> allparams;
    quint32 n = nparams.value(groupid);
    allparams.resize(n);

    //get the group containing the parameters
    const QHash<Index,Param> &group_hash = *(params.find(groupid));

    //iterate over all of the parameters and place them into their correct
    //position in the vector
    for (typename QHash<Index,Param>::const_iterator it = group_hash.begin();
         it != group_hash.end();
         ++it)
    {
        //copy the parameter
        BOOST_ASSERT(it.key() < n);
        allparams[it.key()] = it.value();
    }

    return allparams;
}

/** Copy the parameters for the group with ID 'groupid' into the vector
    'param_vector', starting at index 'strt'. This vector can be of any type,
    as long as the objects contain an operator=(const Param&) operator.

    This will throw an exception if param_vector has the wrong size for
    the parameters in the group,
    e.g. if param_vector.size() < nParameters(groupid) + strt
    If this is the case then a SireError::invalid_arg exception will
    be thrown.

    This will also throw an exception if groupid is invalid, or if there are
    any missing parameters in the group. param_vector will not be modified
    if any exception is thrown.

    This returns the index that is one past the last parameter copied
    (e.g. the index that could be used to copy the next group of
    parameters.

    \throw SireError::invalid_arg
    \throw SireDB::missing_parameter
    \throw invalid_group_error
*/
template<class IDX, class Param>
template<class C>
SIRE_OUTOFLINE_TEMPLATE
uint ParameterStore<IDX,Param>::parameters(
                        const ParameterStore<IDX,Param>::groupid_type &groupid,
                        QVector<C> &param_vector, uint strt) const
{
    //are there any missing parameters in this group?
    if (this->hasMissingParameters(groupid))
        //throw an exception containing all of the missing parameters...
        throw SireDB::missing_parameter( missingString(groupid), CODELOC );

    //check that the supplied vector has the right size
    uint n = nparams.value(groupid);
    uint last_index = n + strt;

    if (uint(param_vector.size()) < last_index)
        throw SireError::invalid_arg( QObject::tr(
              "You must supply a vector of the right size. I need a vector that "
              "has at least size() == %1, but I got a vector of size() == %2")
                  .arg(last_index).arg(param_vector.size()), CODELOC );

    //get the group containing the parameters
    const QHash<Index,Param> &group_hash = *(params.find(groupid));

    //get the pointer to the raw array
    C *array = param_vector.data();

    //iterate over all of the parameters and place them into their correct
    //position in the vector
    for (typename QHash<Index,Param>::const_iterator it = group_hash.begin();
         it != group_hash.end();
         ++it)
    {
        //get the index at which to place the parameter
        uint i = strt + it.key().index();

        //copy the parameter
        BOOST_ASSERT(i < last_index);
        array[i] = it.value();
    }

    //return the index of one past the last copied parameter
    return last_index;
}

const SireStream::MagicID parameterstore_magic =
                              SireStream::getMagic("SireDB::ParameterStore");

}

/** Serialise to a binary data stream */
template<class IDX, class Param>
SIRE_OUTOFLINE_TEMPLATE
QDataStream& operator<<(QDataStream &ds,
                        const SireDB::ParameterStore<IDX,Param> &store)
{
    SireStream::writeHeader(ds, SireDB::parameterstore_magic, 1)
          << store.params << store.nparams;

    return ds;
}

/** Deserialise from a binary data stream */
template<class IDX, class Param>
SIRE_OUTOFLINE_TEMPLATE
QDataStream& operator>>(QDataStream &ds, SireDB::ParameterStore<IDX,Param> &store)
{
    SireStream::VersionID v = SireStream::readHeader(ds, SireDB::parameterstore_magic,
                                                     "SireDB::ParameterStore");

    if (v == 1)
    {
        ds >> store.params >> store.nparams;
    }
    else
        throw SireStream::version_error(v, "1", "SireDB::ParameterStore", CODELOC);

    return ds;
}

SIRE_END_HEADER

#endif
