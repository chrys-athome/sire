#ifndef SireMM_DETAIL_INTERNALGROUPITERATOR_HPP
#define SireMM_DETAIL_INTERNALGROUPITERATOR_HPP

#include "SireMM/detail/internalgroup.hpp"

SIRE_BEGIN_HEADER

namespace SireMM
{
namespace detail
{

/** This is the iterator class for InternalInfo<T>. This is a const
    iterator that may be used to move forwards or backwards through
    the set of internals. Note that this may be set to iterate over
    subsets of the internals, rather than all of them. Also note
    that this takes an implicit copy of the data, so if you modify
    the original InternalInfo<T> then that will not be reflected
    in this iterator, which will continue to iterate over the
    old version of the data.

    @author Christopher Woods
*/
template<class T>
class SIREMM_EXPORT InternalGroupIterator
{
public:
    /** Define the type of index used by this container */
    typedef typename internal_type_data<T>::residue_id_type residue_id_type;
    typedef typename internal_type_data<T>::atom_id_type atom_id_type;

    /** Define the type of exception to throw if we can't find
        an internal */
    typedef typename internal_type_data<T>::missing_error_type missing_error_type;

    /** Define the type of the group that holds the internals */
    typedef InternalGroup<T> group_type;

    /** The type of the internals data */
    typedef QHash<GroupID,group_type> internals_type;

    InternalGroupIterator();

    InternalGroupIterator(const QSet<GroupID> &groupids,
                          const internals_type &internals);

    InternalGroupIterator(const InternalGroupIterator<T> &other);

    ~InternalGroupIterator();

    bool operator==(const InternalGroupIterator<T> &other) const;
    bool operator!=(const InternalGroupIterator<T> &other) const;

    InternalGroupIterator<T>& operator++();
    InternalGroupIterator<T>& operator--();

    InternalGroupIterator<T> operator++(int);
    InternalGroupIterator<T> operator--(int);

    InternalGroupIterator<T>& operator+=(int jump);
    InternalGroupIterator<T>& operator-=(int jump);

    InternalGroupIterator<T> operator+(int jump) const;
    InternalGroupIterator<T> operator-(int jump) const;

    void next();
    void prev();

    void nextGroup();
    void prevGroup();

    InternalGroupIterator<T> currentGroup() const;

    QSet<ResNum> residueNumbers() const;

    bool isValid() const;

    T key() const;
    GroupIndexID value() const;

    void toBegin();
    void toEnd();

    void toFirst();
    void toLast();

    void find(const T &internal);

private:
    /** The set of IDs of the groups that will be iterated over
        by this iterator */
    QSet<GroupID> groups;

    /** The iterator over the set of GroupID numbers */
    QSet<GroupID>::const_iterator groupid_iterator;

    /** Pointer to the current group being iterated over */
    const group_type *current_group;

    /** The iterator over the internals within the group */
    typename QHash<atom_id_type,Index>::const_iterator current_internal;

    /** Implicit copy of the data being iterated over */
    QHash<GroupID,group_type> internals;
};

#ifndef SKIP_TEMPLATE_DEFINITIONS

/** Null constructor - do not use a null iterator as you will get undefined results :-) */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
InternalGroupIterator<T>::InternalGroupIterator() : current_group(0)
{}

/** Construct an iterator to iterate over the internals contained in the groups
    contained in 'all_internals', whose ID numbers are stored in group_ids */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
InternalGroupIterator<T>::InternalGroupIterator(const QSet<GroupID> &group_ids,
                        const typename InternalGroupIterator<T>::internals_type &all_internals)
                         : groups(group_ids), current_group(0), internals(all_internals)
{}

/** Copy constructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
InternalGroupIterator<T>::InternalGroupIterator( const InternalGroupIterator<T> &other )
                         : groups(other.groups), groupid_iterator(other.groupid_iterator),
                           current_group(0), current_internal(other.current_internal),
                           internals(other.internals)
{
    //get the pointer to the current group
    if (other.current_group)
        current_group = internals.constFind(*groupid_iterator).operator->();
}

/** Destructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
InternalGroupIterator<T>::~InternalGroupIterator()
{}

/** Return whether or not the iterator is currently pointing
    to a valid position. Attempts to dereference an invalid
    iterator will lead to undefined results! */
template<class T>
SIRE_INLINE_TEMPLATE
bool InternalGroupIterator<T>::isValid() const
{
    return current_group != 0;
}

/** Comparison operator - this returns true if this iterator is pointing at the same
    internal as 'other', or if both iterators are invalid */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool InternalGroupIterator<T>::operator==(const InternalGroupIterator<T> &other) const
{
    return (current_group == 0 and current_group == other.current_group) or
              current_internal == other.current_internal;
}

/** Comparison operator - returns true if this iterator is pointing to a different internal
    as 'other', or if one of the iterators is invalid and the other isn't */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool InternalGroupIterator<T>::operator!=(const InternalGroupIterator<T> &other) const
{
    return (current_group == 0 and current_group != other.current_group) or
              (other.current_group == 0 and current_group != other.current_group) or
                  current_internal != other.current_internal;
}

/** Advance the operator to the next internal, and return an iterator to the next internal.
    This will return an invalid iterator if this is already an invalid iterator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
InternalGroupIterator<T>& InternalGroupIterator<T>::operator++()
{
    //can't go any further forward if we are invalid
    if (current_group == 0)
        return *this;

    //increment the internal iterator
    ++current_internal;

    //is the internal iterator now invalid?
    if ( current_internal == current_group->constEnd() )
    {
        //we need to move onto the next group
        ++groupid_iterator;

        //is this is a valid groupid?
        if ( groupid_iterator != groups.constEnd() )
        {
            //get the next group over which to iterate
            typename internals_type::const_iterator it =
                                        internals.constFind(*groupid_iterator);
            BOOST_ASSERT( it != internals.constEnd() );

            //get a pointer to the current group
            current_group = it.operator->();

            //reset the internal iterator to iterator over
            //the internals of the new group
            current_internal = current_group->constBegin();

            //ensure that this is valid...
            BOOST_ASSERT( current_internal != current_group->constEnd() );
        }
        else
        {
            //zero the pointer to the current group
            current_group = 0;
        }
    }

    return *this;
}

/** Advance the operator to the next internal, and return an iterator to the current internal. */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
InternalGroupIterator<T> InternalGroupIterator<T>::operator++(int)
{
    //copy the current iterator
    InternalGroupIterator<T> it = *this;

    //only advance this iterator if it is not at the end
    if (this->isValid())
        this->operator++();

    return it;
}

/** Move the iterator back to the first item in the list. If this
    is empty, then the iterator will remain invalid. */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void InternalGroupIterator<T>::toBegin()
{
    if ( groups.count() > 0 )
    {
        //move to the first group
        groupid_iterator = groups.constBegin();

        typename internals_type::const_iterator it = internals.constFind(*groupid_iterator);
        BOOST_ASSERT( it != internals.constEnd() );

        //get a pointer to the current group
        current_group = it.operator->();

        BOOST_ASSERT( current_group->nInternals() != 0 );

        current_internal = current_group->constBegin();
    }
    else
        current_group = 0;
}

/** Move this iterator to the end of the list of internals - this moves
    the iterator past the last internal, thus making the iterator invalid. */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void InternalGroupIterator<T>::toEnd()
{
    current_group = 0;
}

/** Move this iterator back to the first internal in the list - this does
    the same as toBegin() */
template<class T>
SIRE_INLINE_TEMPLATE
void InternalGroupIterator<T>::toFirst()
{
    toBegin();
}

/** Move the iterator to the last internal in the list - note that this
    will make the iterator invalid if there are no internals in the list! */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void InternalGroupIterator<T>::toLast()
{
    //we can only move to the last item if there is
    //an item to move to!
    if ( groups.count() > 0 )
    {
        //move to the last internal
        groupid_iterator = groups.constEnd();
        --groupid_iterator;

        typename internals_type::const_iterator it = internals.constFind(*groupid_iterator);
        BOOST_ASSERT( it != internals.constEnd() );

        current_group = it.operator->();

        BOOST_ASSERT(current_group->nInternals() != 0);

        current_internal = current_group->constEnd();
        --current_internal;
    }
    else
        current_group = 0;
}

/** Move the iterator to the internal 'internal', or make this iterator invalid
    if this internal is not available. */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void InternalGroupIterator<T>::find(const T &internal)
{
    residue_id_type resid = internal;

    //iterate over all of the groups...
    for (groupid_iterator = groups.constBegin();
         groupid_iterator != groups.constEnd();
         ++groupid_iterator)
    {
        //get the group...
        typename internals_type::const_iterator it = internals.constFind(*groupid_iterator);
        BOOST_ASSERT( it != internals.constEnd() );

        current_group = it.operator->();
        BOOST_ASSERT(current_group->nInternals() != 0);

        //does the group's residue-ID match resid?
        if (current_group->residueID() == resid)
        {
            //the internal must be in this group, if it is in this set...
            atom_id_type atmid = internal;

            current_internal = current_group->find(atmid);

            if (current_internal == current_group->constEnd())
                //we didn't find the internal - move this iterator to the end
                this->toEnd();

            return;
        }
    }

    //move the iterator to the end as we didn't find the internal
    this->toEnd();
}

/** Move the iterator back to the previous internal, and return an iterator to the
    previous internal. Note that this will return an iterator to the first item
    in the list if this is called on the first item */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
InternalGroupIterator<T>& InternalGroupIterator<T>::operator--()
{
    if ( current_group == 0 )
    {
        //we are at the end, so move back to the last
        //internal in the list
        toLast();
        return *this;
    }
    else if ( current_internal == current_group->constBegin() )
    {
        //move to the previous group
        if (groupid_iterator == groups.constBegin())
            //we are at the first item already
            return *this;
        else
        {
            groupid_iterator--;
        }

        //get the new current group
        typename internals_type::const_iterator it = internals.constFind(*groupid_iterator);
        BOOST_ASSERT( it != internals.constEnd() );

        current_group = it.operator->();
        BOOST_ASSERT( current_group->nInternals() != 0 );

        current_internal = current_group->constEnd();
        --current_internal;
    }
    else
        --current_internal;

    return *this;
}

/** Move the iterator back to the previous internal, and return an iterator to
    the current internal. */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
InternalGroupIterator<T> InternalGroupIterator<T>::operator--(int)
{
    //copy the current iterator
    InternalGroupIterator<T> it = *this;

    //move the iterator back
    this->operator--();

    return it;
}

/** Move the iterator forward by 'jump' - moving past the last internal lead
    to this iterator becoming invalid */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
InternalGroupIterator<T>& InternalGroupIterator<T>::operator+=(int jump)
{
    if (jump < 0)
        return operator-=( -jump );
    else if (jump == 0)
        return *this;
    else
    {
        for (int i=0; i<jump-1; ++i)
            this->operator++();

        return this->operator++();
    }
}

/** Move the iterator back by 'jump' - moving past the first internal will lead to
    an iterator that points to the first internal */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
InternalGroupIterator<T>& InternalGroupIterator<T>::operator-=(int jump)
{
    if (jump < 0)
        return operator+=( -jump );
    else if (jump == 0)
        return *this;
    else
    {
        for (int i=0; i<jump-1; ++i)
            this->operator--();

        return this->operator--();
    }
}

/** Return an iterator that is moved 'jump' positions past this iterator. This
    will return an invalid iterator if this jumps past the last internal */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
InternalGroupIterator<T> InternalGroupIterator<T>::operator+(int jump) const
{
    InternalGroupIterator<T> it = *this;
    it += jump;
    return it;
}

/** Return an iterator that is moved 'jump' positions back past this iterator. This
    will return an iterator to the first internal if this jumps past the first internal */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
InternalGroupIterator<T> InternalGroupIterator<T>::operator-(int jump) const
{
    InternalGroupIterator<T> it = *this;
    it -= jump;
    return it;
}

/** Return the current key - this is the internal that is at this position.
    Note that this will have undefined results if called on an invalid iterator */
template<class T>
SIRE_INLINE_TEMPLATE
T InternalGroupIterator<T>::key() const
{
    BOOST_ASSERT( current_group != 0 );
    return current_group->getInternal( current_internal.key() );
}

/** Return the current value of the iterator - this is the GroupIndexID of the
    internal in the set. Calling this on an invalid iterator will yield undefined
    results. */
template<class T>
SIRE_INLINE_TEMPLATE
GroupIndexID InternalGroupIterator<T>::value() const
{
    BOOST_ASSERT( current_group != 0 );

    return GroupIndexID( *groupid_iterator,
                         current_internal.value() );
}

/** Advance the iterator to the next item - synonym for operator++() */
template<class T>
SIRE_INLINE_TEMPLATE
void InternalGroupIterator<T>::next()
{
    this->operator++();
}

/** Move the iterator back to the previous item - synonym for operator--() */
template<class T>
SIRE_INLINE_TEMPLATE
void InternalGroupIterator<T>::prev()
{
    this->operator--();
}

/** Move the iterator forward to the first internal of the next group.
    Makes the iterator invalid if we are already at the last group. */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void InternalGroupIterator<T>::nextGroup()
{
    //move onto the next group
    ++groupid_iterator;

    //is this is a valid groupid?
    if ( groupid_iterator != groups.constEnd() )
    {
        //get the next group over which to iterate
        typename internals_type::const_iterator it =
                                    internals.constFind(*groupid_iterator);
        BOOST_ASSERT( it != internals.constEnd() );

        //get a pointer to the current group
        current_group = it.operator->();

        //reset the internal iterator to iterator over
        //the internals of the new group
        current_internal = current_group->constBegin();

        //ensure that this is valid...
        BOOST_ASSERT( current_internal != current_group->constEnd() );
    }
    else
    {
        //zero the pointer to the current group
        current_group = 0;
    }
}

/** Move the iterator back to the first internal of the previous group.
    This moves back to the first internal to be iterated over if
    we are already in the first group. */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void InternalGroupIterator<T>::prevGroup()
{
    if ( current_group == 0 )
    {
        toLast();

        if (this->isValid())
        {
            //we are at the last internal in the last group -
            //move to the first internal of the last group
            current_internal = current_group->constBegin();
        }
    }
    else
    {
        //move to the previous group
        if (groupid_iterator == groups.constBegin())
        {
            //we are at the first group already - move to the
            //first internal in the first group
            current_internal = current_group->constBegin();
        }
        else
        {
            groupid_iterator--;

            //get the new current group
            typename internals_type::const_iterator it = internals.constFind(*groupid_iterator);
            BOOST_ASSERT( it != internals.constEnd() );

            current_group = it.operator->();
            BOOST_ASSERT( current_group->nInternals() != 0 );

            //move to the first internal in the new group
            current_internal = current_group->constBegin();
        }
    }
}

/** Return an iterator that iterates over only the internals in the current group.

    The returned iterator is positioned on the first internal of the group,
    or is invalid if this iterator is currently invalid. */
template<class T>
SIRE_INLINE_TEMPLATE
InternalGroupIterator<T> InternalGroupIterator<T>::currentGroup() const
{
    if (not this->isValid())
        return InternalGroupIterator<T>();
    else
    {
        //get the current group's ID number
        QSet<GroupID> gid;
        gid.insert( *groupid_iterator );

        InternalGroupIterator<T> it(gid, internals);
        it.toBegin();

        return it;
    }
}

/** Return the set of residue numbers of the current group. If you want
    to know the order of the residue numbers then look at one of
    the internals in the group, as they will all have exactly the
    same order of residue numbers. */
template<class T>
SIRE_INLINE_TEMPLATE
QSet<ResNum> InternalGroupIterator<T>::residueNumbers() const
{
    if ( this->isValid() )
        return current_group->residueNumbers();
    else
        return QSet<ResNum>();
}

#endif // SKIP_TEMPLATE_DEFINITIONS

}

}

SIRE_END_HEADER

#endif
