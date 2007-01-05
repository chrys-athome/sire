#ifndef SIREMM_DETAIL_INTERNALINFO_HPP
#define SIREMM_DETAIL_INTERNALINFO_HPP

#include "SireMol/molecule.h"
#include "SireMol/moleculeinfo.h"

#include "SireMM/detail/internalgroup.hpp"
#include "SireMM/detail/internalgroupiterator.hpp"

#include "SireError/errors.h"
#include "SireMol/errors.h"
#include "SireStream/datastream.h"

SIRE_BEGIN_HEADER

namespace SireMM{ namespace detail
{
template<class T>
class InternalInfo;
}}

class QDataStream;

template<class T>
QDataStream& operator<<(QDataStream&, const SireMM::detail::InternalInfo<T>&);
template<class T>
QDataStream& operator>>(QDataStream&, SireMM::detail::InternalInfo<T>&);

namespace SireMM
{

using SireMol::Molecule;
using SireMol::MoleculeInfo;
using SireMol::ResNum;

namespace detail
{

/** This internal class is used to form the common base of
    the classes that hold collections of 'internals' - e.g.
    bonds, angles and dihedrals. This class forms the direct
    base of MolBondInfo, ResBondInfo,  MolAngleInfo, ResAngleInfo,
    MolDihedralInfo and ResDihedralInfo.

    This class stores the internals in groups, with each group
    storing the internals for a different residue combination.

    e.g. all bonds between residues 3 and 4 are stored in the same
    group, all intra-residue bonds for residue 3 are stored in another
    group, while another group is used to store all of the bonds for
    residue 4. This allows rapid indexing and lookup to find all
    of the bonds acting between different residues, or all
    bonds in the group, or all bonds involving particular residues.

    This class comes with an iterator type which allows rapid
    iterating over all of the internals of the group, or
    internals in collections of groups. This removes the need
    for expensive copying of internals during energy calculations.

    @author Christopher Woods
*/
template<class T>
class SIREMM_EXPORT InternalInfo
{

friend QDataStream& ::operator<<<>(QDataStream&, const InternalInfo<T>&);
friend QDataStream& ::operator>><>(QDataStream&, InternalInfo<T>&);

public:
    /** Define the iterator types for this container */
    typedef InternalGroupIterator<T> const_iterator;
    typedef const_iterator iterator;
    typedef const_iterator constIterator;
    typedef iterator Iterator;

    /** Define the type of index used by this container */
    typedef typename internal_type_data<T>::residue_id_type residue_id_type;
    typedef typename internal_type_data<T>::atom_id_type atom_id_type;

    /** Define the type of exception to throw if we can't find
        an internal */
    typedef typename internal_type_data<T>::missing_error_type missing_error_type;

    /** Define the type of the group that holds the internals */
    typedef InternalGroup<T> group_type;

    /** Define the type of internal held in this set */
    typedef T internal_type;

    InternalInfo();

    InternalInfo(const MoleculeInfo &molinfo);

    InternalInfo(const InternalInfo &other);

    ~InternalInfo();

    InternalInfo<T>& operator=(const InternalInfo<T> &other);

    const MoleculeInfo& info() const;

    int nGroups() const;

    bool isEmpty() const;

    const QSet<GroupID> groupIDs() const;
    const QSet<GroupID> groupIDs(ResNum resnum) const;

    const QSet<GroupID> intraGroupIDs() const;
    const QSet<GroupID> intraGroupIDs(ResNum resnum) const;

    const QSet<GroupID> interGroupIDs() const;
    const QSet<GroupID> interGroupIDs(ResNum resnum) const;

    internal_type getInternal(const GroupIndexID &id) const;
    const QSet<internal_type> getInternals(const QSet<GroupIndexID> &ids) const;

    bool contains(const T &internal) const;
    bool contains(ResNum resnum) const;
    bool contains(const AtomIndex &atom) const;
    bool contains(const residue_id_type &resid) const;
    bool contains(GroupID id) const;
    bool contains(const GroupIndexID &id) const;

    GroupIndexID at(const T &internal) const;
    GroupIndexID index(const T &internal) const;
    GroupIndexID operator[](const T &internal) const;

    size_t count() const;
    size_t size() const;

    QHash<GroupID,uint> groupSizes() const;

    const_iterator begin() const;
    const_iterator end() const;

    const_iterator find(const T &internal) const;

    //Generic interface - these should be specialised by derived classes,
    //e.g. MolBondInfo should supply nBonds(), nCommonBonds(), intraBonds() etc.
    int nInternals() const;
    int nInternals(GroupID group) const;
    int nInternals(ResNum resnum) const;

    int nIntraInternals() const;
    int nInterInternals() const;

    int nIntraInternals(ResNum resnum) const;
    int nInterInternals(ResNum resnum) const;

    const_iterator internals() const;
    const_iterator internals(ResNum resnum) const;
    const_iterator internals(GroupID groupid) const;

    const_iterator intraInternals() const;
    const_iterator interInternals() const;

    const_iterator intraInternals(ResNum resnum) const;
    const_iterator interInternals(ResNum resnum) const;

    template<class C>
    const_iterator internals(const C &resnums) const;
    template<class C>
    const_iterator intraInternals(const C &resnums) const;
    template<class C>
    const_iterator interInternals(const C &resnums) const;

protected:

    void checkResidue(ResNum resnum) const;
    void checkResidue(const residue_id_type &resid) const;
    void checkInternal(const internal_type &internal) const;

    int nInternals(const residue_id_type &resid) const;

    const_iterator internals(const residue_id_type &resid) const;

    int nCommonInternals(const QSet<ResNum> &resnums) const;
    bool hasCommonInternal(const QSet<ResNum> &resnums) const;
    const_iterator commonInternals(const QSet<ResNum> &resnums) const;

    InternalInfo extractResidue(ResNum resnum) const;

    GroupIndexID addInternal(const T &internal);
    void removeInternal(const T &internal);
    void removeInternal(const GroupIndexID &groupid);

    void removeInternals();
    void removeInternals(GroupID id);

    const group_type& getGroup(GroupID id) const;
    group_type& getGroup(GroupID id);

private:

    const group_type& _unsafe_getGroup(GroupID id) const;
    group_type& _unsafe_getGroup(GroupID id);

    group_type& getGroup(const T &internal);

    const_iterator _unsafe_internals(const QSet<GroupID> &groups) const;
    const_iterator _unsafe_internals(GroupID groupid) const;

    QSet<GroupID> getCommonGroups(const QSet<ResNum> &resnums) const;

    /** typedef the types of the member data - prevents excess
        typing and obscure errors... */
    typedef QHash< ResNum, QSet<GroupID> > resnum_to_groupid_type;
    typedef QHash<residue_id_type, GroupID> resid_to_groupid_type;
    typedef QHash<GroupID, group_type> groupid_to_group_type;

    /** This hash maps residue numbers to the set of
        GroupID numbers of the groups that contain
        internals that involve the residue with that
        residue number */
    resnum_to_groupid_type resnum_to_groupid;

    /** This hash maps a residue-identifier to the
        ID number of the group that contains the internals
        for that group of residues */
    resid_to_groupid_type resid_to_groupid;

    /** This hash maps the InternalGroup to its
        GroupID number */
    groupid_to_group_type groupid_to_group;

    /** The set of the ID numbers of all of the groups
        in this object */
    QSet<GroupID> groupids;

    /** The MoleculeInfo object that holds the metadata about
        the molecule */
    MoleculeInfo molinfo;

    /** The smallest unassigned GroupID value */
    GroupID newid;
};

/** Construct an empty InternalInfo object */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
InternalInfo<T>::InternalInfo() : newid(0)
{}

/** Construct an InternalInfo object that holds the internals for the
    molecule 'mol' */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
InternalInfo<T>::InternalInfo(const MoleculeInfo &moleculeinfo)
                : molinfo(moleculeinfo), newid(0)
{}

/** Copy constructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
InternalInfo<T>::InternalInfo(const InternalInfo &other)
                : resnum_to_groupid(other.resnum_to_groupid),
                  resid_to_groupid(other.resid_to_groupid),
                  groupid_to_group(other.groupid_to_group),
                  groupids(other.groupids),
                  molinfo(other.molinfo),
                  newid(other.newid)
{}

/** Destructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
InternalInfo<T>::~InternalInfo()
{}

/** Assignment operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
InternalInfo<T>& InternalInfo<T>::operator=(const InternalInfo<T> &other)
{
    //prevent self-copying
    if (&other == this)
        return *this;

    resnum_to_groupid = other.resnum_to_groupid;
    resid_to_groupid = other.resid_to_groupid;
    groupid_to_group = other.groupid_to_group;
    groupids = other.groupids;
    molinfo = other.molinfo;
    newid = other.newid;

    return *this;
}

/** Return a reference to the info object of the molecule whose internals
    are described by this object */
template<class T>
SIRE_INLINE_TEMPLATE
const MoleculeInfo& InternalInfo<T>::info() const
{
    return molinfo;
}

/** Internal function used to check that the residue with number 'resnum'
    is in the molecule. If not, then an exception is thrown.

    \throw SireMol::missing_residue
*/
template<class T>
SIRE_INLINE_TEMPLATE
void InternalInfo<T>::checkResidue(ResNum resnum) const
{
    if (not molinfo.contains(resnum))
        throw SireMol::missing_residue(QObject::tr(
                      "There is no residue with number '%1' in the molecule '%2'")
                          .arg(resnum.toString(), molinfo.toString()), CODELOC);
}

/** Internal function used to check that the residues whose numbers
    are in 'resid' are in the molecule. If not, then an exception is thrown.

    \throw SireMol::missing_residue
*/
template<class T>
SIRE_INLINE_TEMPLATE
void InternalInfo<T>::checkResidue(
                    const typename InternalInfo<T>::residue_id_type &resid) const
{
    QSet<ResNum> missing;

    for (int i=0; i<resid.size(); ++i)
    {
        if (not molinfo.contains(resid[i]))
            missing.insert(resid[i]);
    }

    if (not missing.isEmpty())
    {
        //convert the set of residue numbers to a string
        QStringList missing_strings;

        foreach( ResNum resnum, missing )
            missing_strings.append(resnum.toString());

        QString missing_string = missing_strings.join(" ");

        if (missing.count() == 1)
            throw SireMol::missing_residue(QObject::tr(
                  "There is no residue with number '%1' in the molecule '%2'")
                        .arg(missing_string, molinfo.toString()), CODELOC);
        else
            throw SireMol::missing_residue(QObject::tr(
                  "There are no residues with numbers '%1' in the molecule '%2'")
                        .arg(missing_string, molinfo.toString()), CODELOC);
    }
}

/** Internal function used to check that the atoms specified in the
    internal 'internal' are in this molecule. If not, then an exception
    is throw.

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
template<class T>
SIRE_INLINE_TEMPLATE
void InternalInfo<T>::checkInternal(
                  const typename InternalInfo<T>::internal_type &internal) const
{
    QSet<ResNum> missingres;
    QSet<AtomIndex> missingatm;

    for (int i=0; i<internal.size(); ++i)
    {
        const AtomIndex &atom = internal[i];

        if (not molinfo.contains( atom.resNum() ))
            missingres.insert(atom.resNum());
        else if (not molinfo.contains(atom))
            missingatm.insert(atom);
    }

    if (not missingres.isEmpty())
    {
        //convert the set of residue numbers to a string
        QStringList missing_strings;

        foreach( ResNum resnum, missingres )
            missing_strings.append(resnum.toString());

        QString missing_string = missing_strings.join(" ");

        if (missingres.count() == 1)
            throw SireMol::missing_residue( QObject::tr(
                    "Invalid internal: \"%1\". There is no residue with number %2 "
                    "in the molecule '%3'")
                        .arg(internal.toString(), missing_string, molinfo.toString()),
                            CODELOC );
        else
            throw SireMol::missing_residue( QObject::tr(
                    "Invalid internal: \"%1\". There are no residues with numbers %2 "
                    "in the molecule '%3'")
                        .arg(internal.toString(), missing_string, molinfo.toString()),
                            CODELOC );
    }
    else if (not missingatm.isEmpty())
    {
        //convert the set of atoms to a string
        QStringList missing_strings;

        foreach( AtomIndex atm, missingatm )
            missing_strings.append( QString("\"%1\"").arg(atm.toString()) );

        QString missing_string = missing_strings.join(" ");

        if (missingatm.count() == 1)
            throw SireMol::missing_atom( QObject::tr(
                    "Invalid internal: \"%1\". There is no atom \"%1\" "
                    "in the molecule '%3'")
                        .arg(internal.toString(), missing_string, molinfo.toString()),
                            CODELOC );

    }
}

/** Return whether or not this is empty (has no internals) */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool InternalInfo<T>::isEmpty() const
{
    return groupids.isEmpty();
}

/** Return the number of groups. The internals for each
    residue-combination are placed into a different group - this
    returns how many groups there are. */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
int InternalInfo<T>::nGroups() const
{
    return groupids.count();
}

/** Internal function used to return a reference to the group
    with GroupID 'id' - note that this will throw an exception
    if there is no such group in the list

    \throw SireMol::missing_group
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const typename InternalInfo<T>::group_type& InternalInfo<T>::getGroup(GroupID id) const
{
    typename groupid_to_group_type::const_iterator it = groupid_to_group.find(id);

    if ( it == groupid_to_group.end() )
        throw SireMol::missing_group(QObject::tr("Cannot find the group with GroupID %1")
                                            .arg(id.toString()), CODELOC);

    return *it;
}

/** Internal function used to return a reference to the group
    with GroupID 'id' - note that this will throw an exception
    if there is no such group in the list

    \throw SireMol::missing_group
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
typename InternalInfo<T>::group_type& InternalInfo<T>::getGroup(GroupID id)
{
    typename groupid_to_group_type::iterator it = groupid_to_group.find(id);

    if ( it == groupid_to_group.end() )
        throw SireMol::missing_group(QObject::tr("Cannot find the group with GroupID %1")
                                            .arg(id.toString()), CODELOC);

    return *it;
}

/** Internal function used to return a reference to the group
    with GroupID 'id' - note that there must be a group with
    this ID in the list or else this will have undefined
    results! */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const typename InternalInfo<T>::group_type& InternalInfo<T>::_unsafe_getGroup(GroupID id) const
{
    typename groupid_to_group_type::const_iterator it = groupid_to_group.find(id);
    BOOST_ASSERT( it != groupid_to_group.end() );

    return *it;
}

/** Internal function used to return a const reference to the group
    with GroupID 'id' - note that there must be a group with
    this ID in the list or else this will have undefined
    results! */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
typename InternalInfo<T>::group_type& InternalInfo<T>::_unsafe_getGroup(GroupID id)
{
    typename groupid_to_group_type::iterator it = groupid_to_group.find(id);
    BOOST_ASSERT( it != groupid_to_group.end() );

    return *it;
}

/** Return whether or not this list contains the residue-combination identified
    by 'resid' */
template<class T>
SIRE_INLINE_TEMPLATE
bool InternalInfo<T>::contains(const typename InternalInfo<T>::residue_id_type &resid) const
{
    return resid_to_groupid.contains(resid);
}

/** Return whether or not this list contains the internal 'internal' */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool InternalInfo<T>::contains(const T &internal) const
{
    //if the molecule doesn't contain the atoms of the internal
    //then we can't contain the internal
    for (int i=0; i<internal.count(); ++i)
    {
        if (not molinfo.contains(internal[i]))
            return false;
    }

    //get the residue_id for this internal
    residue_id_type resid(internal);

    //get the GroupID from the group
    typename resid_to_groupid_type::const_iterator it = resid_to_groupid.find(resid);

    if ( it != resid_to_groupid.end() )
    {
        return _unsafe_getGroup( *it ).contains( atom_id_type(internal) );
    }
    else
        return false;
}

/** Return whether or not there are any internals listed that involve 'resnum' */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool InternalInfo<T>::contains(ResNum resnum) const
{
    return molinfo.contains(resnum) and resnum_to_groupid.contains(resnum);
}

/** Return whether or not there are any internals listed that involve the
    atom with AtomIndex 'atom' */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool InternalInfo<T>::contains(const AtomIndex &atom) const
{
    //we can only contain this atom if the molecule contains the atom
    if (molinfo.contains(atom))
    {
        //get all of the groups that involve the atom's residue
        QSet<GroupID> resgroups = resnum_to_groupid.value( atom.resNum() );

        //iterate over all of these groups
        for (QSet<GroupID>::const_iterator it = resgroups.begin();
             it != resgroups.end();
             ++it)
        {
            //does this group contain the atom?
            if ( _unsafe_getGroup( *it ).contains(atom) )
                return true;
        }

    }

    return false;
}

/** Return whether or not this contains the object with GroupIndexID 'id' */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool InternalInfo<T>::contains(const GroupIndexID &id) const
{
    return groupid_to_group.contains(id.groupID()) and
           groupid_to_group.find(id.groupID())->contains(id.index());
}

/** Return the GroupIndexID of the passed internal. Note that this will throw
    an exception if this internal is not contained in the list.

    \throw InternalInfo<T>::missing_error_type
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
GroupIndexID InternalInfo<T>::at(const T &internal) const
{
    //get the resid
    residue_id_type resid = internal;

    //get the group that would contain this internal
    typename resid_to_groupid_type::const_iterator it = resid_to_groupid.find(resid);

    if ( it == resid_to_groupid.end() )
        throw missing_error_type( QObject::tr("Cannot find \"%1\".")
                                  .arg(internal.toString()), CODELOC );

    GroupID groupid = *it;
    Index index = _unsafe_getGroup(groupid).getIndex(internal);

    return GroupIndexID(groupid,index);
}

/** Return the GroupIndexID of the passed internal. Note that this will throw
    an exception if this internal is not contained in the list.

    \throw InternalInfo<T>::missing_error_type
*/
template<class T>
SIRE_INLINE_TEMPLATE
GroupIndexID InternalInfo<T>::index(const T &internal) const
{
    return at(internal);
}

/** Return the GroupIndexID of the passed internal. Note that this will throw
    an exception if this internal is not contained in the list.

    \throw InternalInfo<T>::missing_error_type
*/
template<class T>
SIRE_INLINE_TEMPLATE
GroupIndexID InternalInfo<T>::operator[](const T &internal) const
{
    return at(internal);
}

/** Return the number of internals in this set */
template<class T>
SIRE_INLINE_TEMPLATE
size_t InternalInfo<T>::size() const
{
    return nInternals();
}

/** Return the number of internals in this set */
template<class T>
SIRE_INLINE_TEMPLATE
size_t InternalInfo<T>::count() const
{
    return nInternals();
}

/** Return the size of each group, indexed by the GroupID number */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
QHash<GroupID,uint> InternalInfo<T>::groupSizes() const
{
    QHash<GroupID,uint> sizes;

    for (typename groupid_to_group_type::const_iterator it = groupid_to_group.begin();
         it != groupid_to_group.end();
         ++it)
    {
        sizes.insert( it.key(), it.value().nInternals() );
    }

    return sizes;
}

/** Internal function used to return an iterator over all of the internals
    in the groups whose GroupID numbers are listed in 'groups'. Note that
    'groups' must not refer to any groups that don't exist in this object,
    or else undefined things will happen! */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
typename InternalInfo<T>::const_iterator InternalInfo<T>::_unsafe_internals(
                                              const QSet<GroupID> &groups) const
{
    if (groups.isEmpty())
        return typename InternalInfo<T>::const_iterator();
    else
    {
        typename InternalInfo<T>::const_iterator it(groups, groupid_to_group);
        it.toFirst();
        return it;
    }
}

/** Internal function used to return an iterator over all of the internals
    in the group with GroupID 'id'. Note that this group must exist
    or else undefined things will happen! */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
typename InternalInfo<T>::const_iterator InternalInfo<T>::_unsafe_internals(
                                                                    GroupID id) const
{
    QSet<GroupID> groups;
    groups.insert(id);

    typename InternalInfo<T>::const_iterator it(groups, groupid_to_group);
    it.toFirst();
    return it;
}

/** Return an iterator over all of the internals in this object.

    The iterator is initially position on the first internal.
    Note that an invalid iterator is returned if there are no
    internals in this object
*/
template<class T>
SIRE_INLINE_TEMPLATE
typename InternalInfo<T>::const_iterator InternalInfo<T>::internals() const
{
    return _unsafe_internals( groupids );
}

/** Return an iterator over all of the internals in the group with group ID
    'groupid'

    The iterator is initially positioned on the first internal.
    Note that an invalid iterator is returned if there are no
    internals in this group.

    \throw SireMol::missing_group
*/
template<class T>
SIRE_INLINE_TEMPLATE
typename InternalInfo<T>::const_iterator InternalInfo<T>::internals(GroupID groupid) const
{
    if (not groupid_to_group.contains(groupid))
        throw SireMol::missing_group(QObject::tr("Cannot find the group with GroupID %1")
                                            .arg(groupid.toString()), CODELOC);

    return _unsafe_internals(groupid);
}

/** Return an iterator over all of the internals listed in this object for the
    residue with number 'resnum'.

    The iterator is initially position on the first internal.
    Note that an invalid iterator is returned if there are no
    internals in this object for this residue

    \throw SireMol::missing_residue
*/
template<class T>
SIRE_INLINE_TEMPLATE
typename InternalInfo<T>::const_iterator InternalInfo<T>::internals(ResNum resnum) const
{
    checkResidue(resnum);
    return _unsafe_internals( resnum_to_groupid.value(resnum) );
}

/** Return an iterator over all of the internals listed in this object for the
    residue-combination specified in 'resid'

    The iterator is initially position on the first internal.
    Note that an invalid iterator is returned if there are no
    internals in this object for this residue-combination

    \throw SireMol::missing_residue
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
typename InternalInfo<T>::const_iterator InternalInfo<T>::internals(
                                                const residue_id_type &resid) const
{
    checkResidue(resid);

    typename resid_to_groupid_type::const_iterator it = resid_to_groupid.find(resid);

    if ( it != resid_to_groupid.end() )
    {
        QSet<GroupID> groupids;
        groupids.insert( *it );
        return _unsafe_internals(groupids);
    }
    else
        return typename InternalInfo<T>::const_iterator();
}

/** Return an iterator over all of the intra-residue internals in the set.

    The iterator is initially position on the first internal.
    Note that an invalid iterator is returned if there are no
    internals in this object that match this criteria
*/
template<class T>
SIRE_INLINE_TEMPLATE
typename InternalInfo<T>::const_iterator InternalInfo<T>::intraInternals() const
{
    QSet<GroupID> groups;

    for (QSet<GroupID>::const_iterator it = groupids.begin();
         it != groupids.end();
         ++it)
    {
        if ( _unsafe_getGroup(*it).residueID().isIntraResidue() )
            groups.insert(*it);
    }

    return _unsafe_internals(groups);
}

/** Return an iterator over all of the inter-residue internals in the set.

    The iterator is initially position on the first internal.
    Note that an invalid iterator is returned if there are no
    internals in this object that match this criteria
*/
template<class T>
SIRE_INLINE_TEMPLATE
typename InternalInfo<T>::const_iterator InternalInfo<T>::interInternals() const
{
    QSet<GroupID> groups;

    for (QSet<GroupID>::const_iterator it = groupids.begin();
         it != groupids.end();
         ++it)
    {
        if ( _unsafe_getGroup(*it).residueID().isInterResidue() )
            groups.insert(*it);
    }

    return _unsafe_internals(groups);
}

/** Return an iterator over all of the internals listed in this object that
    are completly contained within the residue 'resnum'

    The iterator is initially position on the first internal.
    Note that an invalid iterator is returned if there are no
    internals in this object that match this criteria

    \throw SireMol::missing_residue
*/
template<class T>
SIRE_INLINE_TEMPLATE
typename InternalInfo<T>::const_iterator InternalInfo<T>::intraInternals(ResNum resnum) const
{
    checkResidue(resnum);

    //get the resid that represents the intra-residue
    residue_id_type resid = resnum;

    return internals(resid);
}

/** Return an iterator over all of the internals listed in this object that
    act over multiple residues, but that involve residue with number 'resnum'.

    The iterator is initially position on the first internal.
    Note that an invalid iterator is returned if there are no
    internals in this object that match this criteria

    \throw SireMol::missing_residue
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
typename InternalInfo<T>::const_iterator InternalInfo<T>::interInternals(ResNum resnum) const
{
    checkResidue(resnum);

    //get the set of all groups that reference this residue
    QSet<GroupID> resgroups = resnum_to_groupid.value(resnum);

    //is there a group that represents the intra-residue combination?
    residue_id_type intrares = resnum;
    if ( resid_to_groupid.contains(intrares) )
        resgroups.remove( resid_to_groupid.value(intrares) );

    //return an iterator over the internals in these groups
    return _unsafe_internals(resgroups);
}

/** Return an iterator over all of the internals that involve at least one
    of the residues whose number is listed in 'resnums'

    The iterator is initially position on the first internal.
    Note that an invalid iterator is returned if there are no
    internals in this object that match this criteria

    \throw SireMol::missing_residue
*/
template<class T>
template<class C>
SIRE_OUTOFLINE_TEMPLATE
typename InternalInfo<T>::const_iterator InternalInfo<T>::internals(const C &resnums) const
{
    //construct the set of groups that contain the internals
    //for these residues
    QSet<GroupID> resgroups;

    for (typename C::const_iterator it = resnums.begin();
         it != resnums.end();
         ++it)
    {
        checkResidue(*it);
        //insert into the set of groups the group that involve this residue
        resgroups += resnum_to_groupid.value(*it);
    }

    //return an iterator over all of these groups
    return _unsafe_internals(resgroups);
}

/** Return an iterator over all of the internals that act *within* the residues
    whose numbers are listed in 'resnums' (e.g. all intra-residue internals).

    The iterator is initially position on the first internal.
    Note that an invalid iterator is returned if there are no
    internals in this object that match this criteria

    \throw SireMol::missing_residue
*/
template<class T>
template<class C>
SIRE_OUTOFLINE_TEMPLATE
typename InternalInfo<T>::const_iterator InternalInfo<T>::intraInternals(const C &resnums) const
{
    //constrct the set of groups that contain the internals
    QSet<GroupID> resgroups;

    for (typename C::const_iterator it = resnums.begin();
         it != resnums.end();
         ++it)
    {
        checkResidue(*it);

        //get the ID for the intra-residue combination
        residue_id_type resid = *it;

        //get the GroupID of this residue-combination
        typename resid_to_groupid_type::const_iterator it = resid_to_groupid.find(resid);

        if (it != resid_to_groupid.end())
            resgroups.insert( *it );
    }

    //return an iterator over these groups
    return _unsafe_internals(resgroups);
}

/** Return an iterator over all of the internals that act over multipe residues
    (inter-residue internals) and that involve at least one residue whose residue
    number is listed in 'resnums'.

    The iterator is initially position on the first internal.
    Note that an invalid iterator is returned if there are no
    internals in this object that match this criteria

    \throw SireMol::missing_residue
*/
template<class T>
template<class C>
SIRE_OUTOFLINE_TEMPLATE
typename InternalInfo<T>::const_iterator InternalInfo<T>::interInternals(const C &resnums) const
{
    //construct the set of groups that contains the internals
    QSet<GroupID> resgroups;

    for (typename C::const_iterator it = resnums.begin();
         it != resnums.end();
         ++it)
    {
        checkResidue(*it);

        //get the ID for the intra-residue combination
        residue_id_type resid = *it;

        //get the GroupIDs of groups that involve this residue
        QSet<GroupID> groups = resnum_to_groupid.value( *it );

        //remove the intra-residue group from this set
        if ( resid_to_groupid.contains(resid) )
            groups.remove( resid_to_groupid.value(resid) );

        //now add the remaining groups to the main set
        resgroups += groups;
    }

    //return an iterator over the internals in these groups
    return _unsafe_internals(resgroups);
}

/** Return an iterator over all of the internals in this object,
    which is already pointing at the first internal. Note that this
    will return an invalid iterator if there are no internals
    in this object. */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
typename InternalInfo<T>::const_iterator InternalInfo<T>::begin() const
{
    typename InternalInfo<T>::const_iterator it( groupids, groupid_to_group );
    it.toBegin();
    return it;
}

/** Return an iterator over all of the internals in this object,
    which is already pointing at one past the last internal. */
template<class T>
SIRE_INLINE_TEMPLATE
typename InternalInfo<T>::const_iterator InternalInfo<T>::end() const
{
    typename InternalInfo<T>::const_iterator it( groupids, groupid_to_group );
    it.toEnd();
    return it;
}

/** Return an iterator over all of the internals in this object that
    is pointing to the internal 'internal'. Note that this will return
    an invalid iterator if there are no internals in this object, or
    will return an iterator that is equal to 'end()' if the internal
    cannot be found. */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
typename InternalInfo<T>::const_iterator InternalInfo<T>::find(const T &internal) const
{
    typename InternalInfo<T>::const_iterator it( groupids, groupid_to_group );
    it.find(internal);
    return it;
}

/** Return a copy of this object that only includes internals that involve
    the residue with number 'resnum'. This returns an empty object if
    there are no internals listed in this object that involve the residue.

    \throw SireMol::missing_residue
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
InternalInfo<T> InternalInfo<T>::extractResidue(ResNum resnum) const
{
    checkResidue(resnum);

    if ( contains(resnum) )
    {
        //get the set of GroupIDs of groups that involve this residue
        QSet<GroupID> resgroups = resnum_to_groupid.value(resnum);

        //create a new InternalInfo object to hold the internals of
        //this residue
        InternalInfo<T> ret;

        //save the GroupIDs of the groups that have this residue
        ret.resnum_to_groupid.insert( resnum, resgroups );

        //save the smallest unassigned GroupID number
        ret.newid = newid;

        //now add each group one-by-one
        for (QSet<GroupID>::const_iterator it = resgroups.begin();
             it != resgroups.end();
             ++it)
        {
            GroupID groupid = *it;
            const group_type &group = _unsafe_getGroup( groupid );

            //add the group indexed by the GroupID
            ret.groupid_to_group.insert(groupid, group);
            ret.groupids.insert(groupid);

            //now add the GroupID indexed by the residue-combination ID
            ret.resid_to_groupid.insert( group.residueID(), groupid );

            //get the set of residues involved in this group
            QSet<ResNum> group_resnums = group.residueNumbers();

            //remove the current residue from this set
            group_resnums.remove(resnum);

            //now add the GroupID indexed by the other residue number
            foreach (ResNum other_res, group_resnums)
            {
                ret.resnum_to_groupid[other_res].insert( groupid );
            }
        }

        int n = 0;
        for ( QSet<GroupID>::const_iterator it = ret.groupids.begin();
              it != ret.groupids.end();
              ++it )
        {
            //add the number of internals in the group
            n += _unsafe_getGroup( *it ).nInternals();
        }

        //return the completed object
        return ret;
    }
    else
    {
        //this doesn't contain any internals involving this residue,
        //so return an empty set
        return InternalInfo<T>();
    }
}

/** Return a reference to the group that can hold the internal 'internal'.
    Note that this group is created if it doesn't already exist.

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
typename InternalInfo<T>::group_type& InternalInfo<T>::getGroup(const T &internal)
{
    checkInternal(internal);

    //get the ID of the residues in the internal
    residue_id_type resid = internal;

    typename resid_to_groupid_type::iterator it = resid_to_groupid.find(resid);

    if ( it != resid_to_groupid.end() )
        //we have found the ID of the group!
        return _unsafe_getGroup( *it );
    else
    {
        //there is no group for this residue combination - create one
        GroupID groupid = newid;
        ++newid;

        groupid_to_group.insert( groupid, group_type(resid) );

    }
}

/** Add 'internal' to the set of internals. This does
    nothing if the internal is already in the set. This
    returns the GroupIndexID of the added internal

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
GroupIndexID InternalInfo<T>::addInternal(const T &internal)
{
    checkInternal(internal);

    //get the group into which this internal belongs

    //do this by getting the ID of the residues in the internal
    residue_id_type resid = internal;

    //find the group
    GroupID groupid;
    if ( not resid_to_groupid.contains(resid) )
    {
        //there is no group - we need to create a new one
        group_type new_group(resid);

        //get a new ID number for the group
        groupid = newid;
        groupids.insert(groupid);
        ++newid;

        //save the group, indexed by its ID number
        groupid_to_group.insert( groupid, new_group );

        //save the group, indexed by the resid
        resid_to_groupid.insert( resid, groupid );

        //get the set of residues involved in this internal
        QSet<ResNum> resnums = new_group.residueNumbers();

        foreach( ResNum resnum, resnums )
        {
            //index the group by the residues that it contains
            resnum_to_groupid[resnum].insert( groupid );
        }
    }
    else
        groupid = resid_to_groupid.value(resid);

    //calculate the ID for the atom combination
    atom_id_type atmid = internal;

    //add the internal to the group, getting the index ID
    Index index = _unsafe_getGroup(groupid).addInternal(atmid);

    return GroupIndexID(groupid, index);
}

/** Return the corresponding internal to the index 'id'. This
    will throw an exception if there is no internal with this ID.

    \throw SireMol::missing_group
    \throw SireError::invalid_index
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
T InternalInfo<T>::getInternal(const GroupIndexID &id) const
{
    return this->getGroup(id.groupID()).getInternal(id.index());
}

/** Return the corresponding internals for the indexes in 'ids'.
    This will throw an exception if there is no internal with
    this ID.

    \throw SireMol::missing_group
    \throw SireError::invalid_index
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const QSet<T> InternalInfo<T>::getInternals(const QSet<GroupIndexID> &ids) const
{
    if ( not ids.isEmpty() )
    {
        QSet<T> allinternals;
        allinternals.reserve(ids.count());

        for (QSet<GroupIndexID>::const_iterator it = ids.begin();
             it != ids.end();
             ++it)
        {
            allinternals.insert( this->getGroup(it->groupID())
                                      .getInternal(it->index()) );
        }

        return allinternals;
    }
    else
        return QSet<T>();
}

/** Remove the internal 'internal' from this set. This may be slow
    as it could involve the re-indexing of an entire group of internals.
    It shouldn't be too slow though, as each group should be reasonably
    small. This does nothing if there is no such internal in this set. */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void InternalInfo<T>::removeInternal(const T &internal)
{
    //only remove the internal if we contain it - this
    //avoids triggering a deep-copy
    if ( this->contains(internal) )
    {
        //get the GroupID of the group that contains this internal
        residue_id_type resid = internal;

        GroupID groupid = resid_to_groupid.value(resid); // this must work as
                                                         // contains(internal) is true

        //get the group that contains this internal
        group_type &group = _unsafe_getGroup(groupid);

        //remove the internal - this may cause the
        //group to re-index its contents
        atom_id_type atmid = internal;
        group.removeInternal(atmid);

        //is the group now empty?
        if (group.isEmpty())
        {
            //get the residue numbers of the residues in the group
            QSet<ResNum> resnums = group.residueNumbers();

            //remove the group
            groupids.remove(groupid);
            groupid_to_group.remove(groupid);
            resid_to_groupid.remove(resid);

            //now remove the metainformation used to relate
            //the group to the residue numbers of the internal
            foreach (ResNum resnum, resnums)
            {
                QSet<GroupID> &resgroups = resnum_to_groupid[resnum];
                resgroups.remove(groupid);

                //are there now no internals involving this residue?
                if (resgroups.isEmpty())
                    //remove this residue from the set
                    resnum_to_groupid.remove(resnum);
            }
        }
    }
}

/** Remove the internal with ID 'groupid' from this set. This may be slow
    as it could involve the re-indexing of an entire group of internals.
    It shouldn't be too slow though, as each group should be reasonably
    small. This does nothing if there is no such internal in this set. */
template<class T>
SIRE_INLINE_TEMPLATE
void InternalInfo<T>::removeInternal(const GroupIndexID &groupid)
{
    if (this->contains(groupid))
        this->_unsafe_getGroup(groupid.groupID()).removeInternal(groupid.index());
}

/** Remove all internals in the group with ID 'id' - does nothing if
    there is no such group */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void InternalInfo<T>::removeInternals(GroupID id)
{
    //do we have a group with this ID number?
    if (groupid_to_group.contains(id))
    {
        //get the actual group - this will contain the group's resid
        const group_type &group = groupid_to_group[id];

        const residue_id_type &resid = group.residueID();

        //remove this group for each of the residues
        foreach (ResNum resnum, resid.residueNumbers())
        {
            resnum_to_groupid[resnum].remove(id);
        }

        //remove the group from the list of resids...
        resid_to_groupid.remove(resid);

        //remove from the list of groupids
        groupids.remove(id);

        //now, finally, remove the group itself
        groupid_to_group.remove(id);
    }
}

/** Remove all internals */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void InternalInfo<T>::removeInternals()
{
    resnum_to_groupid.clear();
    resid_to_groupid.clear();
    groupid_to_group.clear();
    groupids.clear();
    newid = GroupID(0);
}

/** Returns the number of internals in the group with GroupID 'group'.
    This returns '0' if there is no group with this ID number */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
int InternalInfo<T>::nInternals(GroupID group) const
{
    typename groupid_to_group_type::const_iterator it = groupid_to_group.find(group);

    if (it != groupid_to_group.end())
        return it->nInternals();
    else
        return 0;
}

/** Return the total number of internals in this set */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
int InternalInfo<T>::nInternals() const
{
    int n = 0;

    //iterate over all of the groups
    for ( QSet<GroupID>::const_iterator it = groupids.begin();
          it != groupids.end();
          ++it )
    {
        //add the number of internals in the group
        n += _unsafe_getGroup( *it ).nInternals();
    }

    return n;
}

/** Return the number of internals in the list that involve the
    residue with number 'resnum'. This returns 0 if there are
    no internals in this list that involve this residue. */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
int InternalInfo<T>::nInternals(ResNum resnum) const
{
    checkResidue(resnum);

    int n = 0;

    //get the set of groups that contain internals that involve this
    //residue
    QSet<GroupID> resgroups = resnum_to_groupid.value(resnum);

    //iterate over all of the groups involving this residue
    for (QSet<GroupID>::const_iterator it = resgroups.begin();
         it != resgroups.end();
         ++it)
    {
        //add the number of internals in the group
        n += _unsafe_getGroup( *it ).nInternals();
    }

    return n;
}

/** Internal function used to return the number of internals in the group
    that holds the internals for the residue-combination with ID number 'resid'.
    This returns 0 if there are no internals for this residue-combination. */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
int InternalInfo<T>::nInternals(const typename InternalInfo<T>::residue_id_type &resid) const
{
    checkResidue(resid);

    typename resid_to_groupid_type::const_iterator it = resid_to_groupid.find(resid);

    if (it != resid_to_groupid.end())
        return _unsafe_getGroup(*it).nInternals();
    else
        return 0;
}

/** Return the total number of intra-residue internals in this set */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
int InternalInfo<T>::nIntraInternals() const
{
    int n = 0;

    //loop over all of the groups and sum up the
    //number of bonds in all of the intra-residue groups
    for ( QSet<GroupID>::const_iterator it = groupids.begin();
          it != groupids.end();
          ++it )
    {
        const group_type &group = _unsafe_getGroup( *it );

        if (group.residueID().isIntraResidue())
            n += group.nInternals();
    }

    return n;
}

/** Return the total number of inter-residue internals in this set */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
int InternalInfo<T>::nInterInternals() const
{
    int n = 0;

    //loop over all of the groups and sum up the
    //number of bonds in all of the inter-residue groups
    for ( QSet<GroupID>::const_iterator it = groupids.begin();
          it != groupids.end();
          ++it )
    {
        const group_type &group = _unsafe_getGroup( *it );

        if (group.residueID().isInterResidue())
            n += group.nInternals();
    }

    return n;
}

/** Return the number of intra-residue internals listed for the
    residue with number 'resnum'. This returns 0 if there are no
    intraresidue internals listed for this residue. */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
int InternalInfo<T>::nIntraInternals(ResNum resnum) const
{
    checkResidue(resnum);
    residue_id_type resid = resnum;
    return nInternals(resid);
}

/** Return the number of inter-residue internals listed for the residue
    with number 'resnum'. This returns 0 if there are no interresidue
    internals listed for this residue. */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
int InternalInfo<T>::nInterInternals(ResNum resnum) const
{
    checkResidue(resnum);

    //get the GroupIDs of all groups that involve this residue
    QSet<GroupID> resgroups = resnum_to_groupid.value(resnum);

    int n = 0;

    //iterate over all of the groups
    for (QSet<GroupID>::const_iterator it = resgroups.begin();
         it != resgroups.end();
         ++it)
    {
        //add the number of intermolecular internals to the total
        n += _unsafe_getGroup( *it ).nInterInternals();
    }

    return n;
}

/** Return the set of ID numbers of all of the groups in this object */
template<class T>
SIRE_INLINE_TEMPLATE
const QSet<GroupID> InternalInfo<T>::groupIDs() const
{
    return groupids;
}

/** Return the set of ID numbers of all of the groups that involve the
    residue with number 'resnum'. This will return an empty set if this
    residue has no internals, and will throw an exception if this
    residue is not in the molecule.

    \throw SireMol::missing_residue
*/
template<class T>
SIRE_INLINE_TEMPLATE
const QSet<GroupID> InternalInfo<T>::groupIDs(ResNum resnum) const
{
    checkResidue(resnum);
    return resnum_to_groupid.value(resnum);
}

/** Return the set of ID numbers of all of the intra-residue groups */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const QSet<GroupID> InternalInfo<T>::intraGroupIDs() const
{
    QSet<GroupID> ret;

    //loop over all of the groups and add the intra-residue groups
    for ( QSet<GroupID>::const_iterator it = groupids.begin();
          it != groupids.end();
          ++it )
    {
        const group_type &group = _unsafe_getGroup( *it );

        if (group.residueID().isIntraResidue())
            ret.insert(*it);
    }

    return ret;
}

/** Return the set of ID numbers of all of the intra-residue groups that
    involve the residue with number 'resnum'. This will return an empty
    set if there are no intra-residue internals within this residue,
    and will throw an exception if this residue is not in the molecule.

    \throw SireMol::missing_residue
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const QSet<GroupID> InternalInfo<T>::intraGroupIDs(ResNum resnum) const
{
    checkResidue(resnum);

    //convert the residue number into a residue ID type (??ResID, e.g. BondResID)
    residue_id_type resid = resnum;

    //try to find the GroupID of any group that has this residue ID
    typename resid_to_groupid_type::const_iterator it = resid_to_groupid.find(resid);

    QSet<GroupID> ret;

    //add the GroupID of any groups that have this residue ID
    if (it != resid_to_groupid.end())
        ret.insert(*it);

    return ret;
}

/** Return the set of ID numbers of all of the inter-residue groups */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const QSet<GroupID> InternalInfo<T>::interGroupIDs() const
{
    QSet<GroupID> ret;

    //loop over all of the groups and add the inter-residue groups
    for ( QSet<GroupID>::const_iterator it = groupids.begin();
          it != groupids.end();
          ++it )
    {
        const group_type &group = _unsafe_getGroup( *it );

        if (group.residueID().isInterResidue())
            ret.insert(*it);
    }

    return ret;
}

/** Return the set of ID numbers of all of the inter-residue groups that
    involve the residue with number 'resnum'. This will return an empty
    set if there are no inter-residue internals involving this residue,
    and will throw an exception if this residue is not in the molecule.

    \throw SireMol::missing_residue
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const QSet<GroupID> InternalInfo<T>::interGroupIDs(ResNum resnum) const
{
    checkResidue(resnum);

    //get the GroupIDs of all groups that involve this residue
    QSet<GroupID> resgroups = resnum_to_groupid.value(resnum);

    if (not resgroups.isEmpty())
    {
        //remove from this list the groupID of the intra-residue group
        residue_id_type resid = resnum;
        typename resid_to_groupid_type::const_iterator it = resid_to_groupid.find(resid);

        if (it != resid_to_groupid.end())
            resgroups.remove(*it);
    }

    return resgroups;
}

/** Small internal function used to return whether or not 'smallset'
    is a subset of 'bigset' */
inline bool isSubSet(const QSet<ResNum> &smallset, const QSet<ResNum> &bigset)
{
    if (smallset == bigset)
    {
        //an equal set is also a subset
        return true;
    }
    else if (smallset.count() > bigset.count())
    {
        //smallset cannot be a subset of bigset!
        return false;
    }
    else
    {
        //check the items one by one
        for (QSet<ResNum>::const_iterator it = smallset.begin();
             it != smallset.end();
             ++it)
        {
            if ( not bigset.contains(*it) )
                return false;
        }

        return true;
    }
}

/** Return the ID numbers of groups that contain internals that contain
    all of the residues in 'resnums'. This may be slow as it requires
    a search over all of the groups in this set. */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
QSet<GroupID> InternalInfo<T>::getCommonGroups(const QSet<ResNum> &resnums) const
{
    QSet<GroupID> ret;

    //iterate over all of the groups in the molecule
    for ( typename groupid_to_group_type::const_iterator it = groupid_to_group.constBegin();
          it != groupid_to_group.constEnd();
          ++it )
    {
        //get the set of residue numbers involved in this group
        QSet<ResNum> group_resnums = it->residueNumbers();

        //resnums must be a complete subset of group_resnums
        if ( isSubSet(resnums, group_resnums) )
            ret.insert( it.key() );
    }

    return ret;
}

/** Return the number of internals that involve all of the residues in
    'resnums' */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
int InternalInfo<T>::nCommonInternals(const QSet<ResNum> &resnums) const
{
    QSet<GroupID> groups = this->getCommonGroups(resnums);

    int n = 0;
    for ( QSet<GroupID>::const_iterator it = groups.constBegin();
          it != groups.constEnd();
          ++it )
    {
        n += nInternals( *it );
    }

    return n;
}

/** Return whether or not there are any internals that contain
    all of the residues in 'resnums' */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool InternalInfo<T>::hasCommonInternal(const QSet<ResNum> &resnums) const
{
    return this->getCommonGroups(resnums).count() > 0;
}

/** Return an iterator over all of the internals that involve all
    of the residues in 'resnums'.

    This returns an iterator initially pointed to the first internal,
    or an invalid iterator if there are no internals that match
    this criteria */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
typename InternalInfo<T>::const_iterator
InternalInfo<T>::commonInternals(const QSet<ResNum> &resnums) const
{
    QSet<GroupID> groups = this->getCommonGroups(resnums);

    return _unsafe_internals(groups);
}

const MagicID intinfo_magic = getMagic("SireMM::detail::InternalInfo");

}

}

/** Serialise to a binary data stream */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
QDataStream &operator<<(QDataStream &ds, const SireMM::detail::InternalInfo<T> &info)
{
    SireStream::writeHeader(ds, SireMM::detail::intinfo_magic, 1)
          << info.resnum_to_groupid << info.resid_to_groupid
          << info.groupid_to_group << info.groupids << info.molinfo << info.newid;

    return ds;
}

/** Deserialise from a binary data stream */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
QDataStream &operator>>(QDataStream &ds, SireMM::detail::InternalInfo<T> &info)
{
    SireStream::VersionID v = SireStream::readHeader(ds, SireMM::detail::intinfo_magic,
                                                     "SireMM::detail::InternalInfo");

    if (v == 1)
    {
        ds >> info.resnum_to_groupid >> info.resid_to_groupid
           >> info.groupid_to_group >> info.groupids >> info.molinfo >> info.newid;
    }
    else
        throw SireStream::version_error(v, "1", "SireMM::detail::InternalInfo", CODELOC);

    return ds;
}

SIRE_END_HEADER

#endif
