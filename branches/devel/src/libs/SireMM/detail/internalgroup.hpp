#ifndef SIREMM_DETAIL_INTERNALGROUP_HPP
#define SIREMM_DETAIL_INTERNALGROUP_HPP

#include <qhash.h>
#include <qset.h>

#include <boost/assert.hpp>

#include "SireMol/idtypes.h"

#include "SireMol/groupindexid.h"

#include "SireError/errors.h"
#include "SireStream/datastream.h"

SIRE_BEGIN_HEADER

namespace SireMM { namespace detail
{
template<class T>
class InternalGroup;
}}

class QDataStream;

template<class T>
QDataStream& operator<<(QDataStream&, const SireMM::detail::InternalGroup<T>&);
template<class T>
QDataStream& operator>>(QDataStream&, SireMM::detail::InternalGroup<T>&);

namespace SireMM
{

using SireMol::GroupID;
using SireMol::Index;
using SireMol::GroupIndexID;

/** This class is used to supply type information to 
    InternalInfo<T> - see MolBondInfo to see how to use it */
template<class T>
class internal_type_data;

namespace detail
{

/** This class holds information about a group of internals (bonds/angles/dihedrals).
    All of the internals are sorted into groups according to the residues that
    are involved in the internal. This class holds all of the internals of a group,
    holding them in a vector, and assigning each one an index based on its 
    location within the vector. The combination of the ID number of the group
    (the GroupID) and the index into the vector (the Index) gives a unique
    identification for each internal in a molecule (the GroupIndexID)
     
    @author Christopher Woods
*/
template<class T>
class SIREMM_EXPORT InternalGroup
{

friend QDataStream& ::operator<<<>(QDataStream&, const InternalGroup<T>&);
friend QDataStream& ::operator>><>(QDataStream&, InternalGroup<T>&);

public:
    /** Define the type of index used by this container */
    typedef typename internal_type_data<T>::residue_id_type residue_id_type;
    typedef typename internal_type_data<T>::atom_id_type atom_id_type;
    
    /** Define the type of exception to throw if we can't find
        an internal */
    typedef typename internal_type_data<T>::missing_error_type missing_error_type;
    
    /** Type holding the hash of atom_id_types */
    typedef QHash<atom_id_type,Index> atomid_to_index_type;
    
    InternalGroup();
    
    InternalGroup(const residue_id_type &id);
    
    InternalGroup(const InternalGroup<T> &other);
    
    ~InternalGroup();
    
    Index addInternal(const atom_id_type &atomid);
    void removeInternal(const atom_id_type &atomid);
    void removeInternal(Index index);
    
    bool isEmpty() const;
    
    T getInternal(const atom_id_type &id) const;
    T getInternal(Index idx) const;
    
    Index getIndex(const atom_id_type &id) const;
    
    QSet<ResNum> residueNumbers() const;
    
    int nInternals() const;
    int nInterInternals() const;
    int nIntraInternals() const;
    
    bool contains(const T &internal) const;
    bool contains(const atom_id_type &atomid) const;
    bool contains(const AtomIndex &atom) const;
    bool contains(Index idx) const;
    
    const residue_id_type& residueID() const;
    
    typename atomid_to_index_type::const_iterator begin() const;
    typename atomid_to_index_type::const_iterator end() const;
    
    typename atomid_to_index_type::const_iterator constBegin() const;
    typename atomid_to_index_type::const_iterator constEnd() const;
    
    typename atomid_to_index_type::const_iterator find(const atom_id_type &id) const;
    typename atomid_to_index_type::const_iterator constFind(const atom_id_type &id) const;
    
private:
    /** The ID number of the residue-combination of this group */
    residue_id_type resid;
    
    /** Hash mapping the atom ID types to the index */
    atomid_to_index_type atomid_to_index;

    /** List mapping the index to the ID types */
    QList<atom_id_type> index_to_atomid;
};

/** Null constructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
InternalGroup<T>::InternalGroup()
{}
    
/** Construct a group that contains the internals for the residue-combination 
    whose ID number is 'id' */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
InternalGroup<T>::InternalGroup(const residue_id_type &id)
                 : resid( id )
{}
    
/** Copy constructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
InternalGroup<T>::InternalGroup(const InternalGroup<T> &other)
                 : resid(other.resid), 
                   atomid_to_index(other.atomid_to_index),
                   index_to_atomid(other.index_to_atomid)
{}
    
/** Destructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
InternalGroup<T>::~InternalGroup()
{}
    
/** Add an internal to this group - this will do nothing if this internal
    is already in this group. This returns the index of the added internal. */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Index InternalGroup<T>::addInternal(const typename InternalGroup<T>::atom_id_type &atomid)
{
    if ( not atomid_to_index.contains(atomid) )
    {
        //the internals are numbered sequentially from 0 to nInternals() - 1
        //(so that the indicies can be used with an array)
        Index newindex( index_to_atomid.count() );
        atomid_to_index.insert( atomid, newindex );
        index_to_atomid.append( atomid );
        
        return newindex;
    }
    else
        return atomid_to_index.value(atomid);
}

/** Remove an internal from this group - this will do nothing if this internal
    is not in this group. This function can be slow as it may need to reindex
    all of the internals in this group, which is an O(n) operation on the 
    number of internals. */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void InternalGroup<T>::removeInternal(const typename InternalGroup<T>::atom_id_type &atomid)
{
    if ( atomid_to_index.contains(atomid) )
    {
        //find the internal...
        typename atomid_to_index_type::iterator it = atomid_to_index.find(atomid);
        
        //get its index
        Index atomidx = it.value();
        
        //remove the internal from the hash
        atomid_to_index.erase(it);
        
        //do we need to reindex the the internals - we will have to reindex
        //unless the index of the internal is equal to the new size of the hash
        Index n( index_to_atomid.count() );
        if ( atomidx + 1 < n )
        {
            for (Index i( atomidx+1 ); i<n; ++i)
            {
                //change the index of this internal
                atomid_to_index[ index_to_atomid[i] ] -= 1;
            }
        }
    
        //remove the index from the reverse-lookup hash
        index_to_atomid.removeAt(atomidx);
    }
}

/** Remove the internal with index 'idx' - this will do nothing if there 
    is no internal with this index. */
template<class T>
SIRE_INLINE_TEMPLATE
void InternalGroup<T>::removeInternal(Index idx)
{
    if ( idx < Index(index_to_atomid.count()) )
        this->removeInternal( index_to_atomid[idx] );
}
    
/** Return whether or not this group is empty */
template<class T>
SIRE_INLINE_TEMPLATE
bool InternalGroup<T>::isEmpty() const
{
    return atomid_to_index.isEmpty();
}
    
/** Convert the atom_id_type and the contained resid back into 
    an internal object of type T. This internal does not need to 
    be in this group for this to work - this works in all circumstances. */
template<class T>
SIRE_INLINE_TEMPLATE
T InternalGroup<T>::getInternal(const atom_id_type &id) const
{
    return internal_type_data<T>::convertToInternal(resid, id);
}

/** Return the internal at index 'id' - this will throw an exception 
    if this is an invalid index.
    
    \throw SireError::invalid_index
*/
template<class T>
T InternalGroup<T>::getInternal(Index idx) const
{
    if ( index_to_atomid.isEmpty() )
        throw SireError::invalid_index( QObject::tr(
                    "Invalid index: No internals in this object!"), CODELOC );
    else if ( idx >= Index(index_to_atomid.count()) )
        throw SireError::invalid_index( QObject::tr(
                    "Invalid index: requested %1, but only 0 -> %2 are valid")
                        .arg(idx.toString(),index_to_atomid.count()-1), CODELOC );
                        
    return internal_type_data<T>::convertToInternal( resid, index_to_atomid[idx] );
}

/** Return the index for the the atom_id_type 'id'. This throws 
    an exception if there is no index for this ID value
    
    \throw missing_error_type
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Index InternalGroup<T>::getIndex(const atom_id_type &id) const
{
    typename atomid_to_index_type::const_iterator it = atomid_to_index.find(id);
    
    if ( it == atomid_to_index.end() )
        throw missing_error_type( QObject::tr("Cannot find %1")
                                .arg( getInternal(id).toString() ), CODELOC );
                                
    return *it;
}

/** Return the set of residue numbers of the atoms involved in the 
    internals in this group */
template<class T>
SIRE_INLINE_TEMPLATE
QSet<ResNum> InternalGroup<T>::residueNumbers() const
{
    return resid.residueNumbers();
}
    
/** Return the total number of internals in this group */
template<class T>
SIRE_INLINE_TEMPLATE
int InternalGroup<T>::nInternals() const
{
    return atomid_to_index.count();
}

/** Return the number of inter-residue internals in this group. */
template<class T>
SIRE_INLINE_TEMPLATE
int InternalGroup<T>::nInterInternals() const
{
    if (resid.isInterResidue())
        return atomid_to_index.count();
    else
        return 0;
}

/** Return the number of intra-residue internals in this group. */
template<class T>
SIRE_INLINE_TEMPLATE
int InternalGroup<T>::nIntraInternals() const
{
    if (resid.isIntraResidue())
        return atomid_to_index.count();
    else
        return 0;
}
    
/** Return whether or not this group contains the internal 'internal' */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool InternalGroup<T>::contains(const T &internal) const
{
    //calculate a residue-combination ID for this internal
    residue_id_type new_resid = internal;
    
    if (resid == new_resid)
    {
        //calculate the atom-combination ID for this internal
        atom_id_type new_atmid = internal;
        return atomid_to_index.contains(new_atmid);
    }
    else
        return false;
}

/** Return whether or not this group contains the internal with atom-combination 
    ID number 'atomid' */
template<class T>
SIRE_INLINE_TEMPLATE
bool InternalGroup<T>::contains(const typename InternalGroup<T>::atom_id_type &atomid) const
{
    return atomid_to_index.contains(atomid);
}
    
/** Return whether or not this group contains the index 'idx'. */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool InternalGroup<T>::contains(Index idx) const
{
    return idx < Index(index_to_atomid.count());
}
    
/** Return a const-reference to the ID number that identifies
    the residue-combination of the internals stored in this group. */
template<class T>
SIRE_INLINE_TEMPLATE
const typename InternalGroup<T>::residue_id_type& InternalGroup<T>::residueID() const
{
    return resid;
}

/** Return an iterator pointing at the first internal in this group */
template<class T>
SIRE_INLINE_TEMPLATE
typename InternalGroup<T>::atomid_to_index_type::const_iterator 
InternalGroup<T>::begin() const
{
    return atomid_to_index.begin();
}

/** Return an iterator pointing at the imaginary item one past the last
    internal in this group */
template<class T>
SIRE_INLINE_TEMPLATE
typename InternalGroup<T>::atomid_to_index_type::const_iterator 
InternalGroup<T>::end() const
{
    return atomid_to_index.end();
}
    
/** Return an iterator pointing at the first internal in this group */
template<class T>
SIRE_INLINE_TEMPLATE
typename InternalGroup<T>::atomid_to_index_type::const_iterator 
InternalGroup<T>::constBegin() const
{
    return atomid_to_index.constBegin();
}

/** Return an iterator pointing at the imaginary item one past the last
    internal in this group */
template<class T>
SIRE_INLINE_TEMPLATE
typename InternalGroup<T>::atomid_to_index_type::const_iterator 
InternalGroup<T>::constEnd() const
{
    return atomid_to_index.constEnd();
}

/** Return an iterator pointing to the item with atom-id 'id', or to 'end()'
    if there is no such item in this set. */
template<class T>
SIRE_INLINE_TEMPLATE
typename InternalGroup<T>::atomid_to_index_type::const_iterator 
InternalGroup<T>::find(const atom_id_type &id) const
{
    return atomid_to_index.find(id);
}

/** Return an iterator pointing to the item with atom-id 'id', or to 'end()'
    if there is no such item in this set. */
template<class T>
SIRE_INLINE_TEMPLATE
typename InternalGroup<T>::atomid_to_index_type::const_iterator 
InternalGroup<T>::constFind(const atom_id_type &id) const
{
    return atomid_to_index.find(id);
}

/** Return whether or not this group contains the atom with index 'atom'. 
    This function can be slow as it requires the scanning of all of the 
    internals in this group */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool InternalGroup<T>::contains(const AtomIndex &atom) const
{
    //if the residue number of the atom is not in this 
    //group then the atom can't be either...
    if ( residueNumbers().contains(atom.resNum()) )
    {
        for (typename atomid_to_index_type::const_iterator it = atomid_to_index.begin();
             it != atomid_to_index.end();
             ++it)
        {
            if ( getInternal(it.key()).contains(atom) )
                return true;
        }
    }
    
    return false;
}

const SireStream::MagicID intgroup_magic = 
                              SireStream::getMagic("SireMM::detail::InternalGroup");

} // namespace detail

} // namespace SireMM

/** Serialise to a binary data stream */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
QDataStream &operator<<(QDataStream &ds, const SireMM::detail::InternalGroup<T> &group)
{
    SireStream::writeHeader(ds, SireMM::detail::intgroup_magic, 1) 
                << group.resid << group.atomid_to_index
                << group.index_to_atomid;
    return ds;
}

/** Deserialise from a binary data stream */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
QDataStream &operator>>(QDataStream &ds, SireMM::detail::InternalGroup<T> &group)
{
    SireStream::VersionID v = SireStream::readHeader(ds, SireMM::detail::intgroup_magic, 
                                                     "SireMM::detail::InternalGroup");
    
    if (v == 1)
    {
        ds >> group.resid >> group.atomid_to_index >> group.index_to_atomid;
    }
    else
        throw SireStream::version_error(v, "1", "SireMM::detail::InternalGroup", CODELOC);
        
    return ds;
}

SIRE_END_HEADER

#endif

