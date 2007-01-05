#ifndef SIREMM_INTERNALTABLEBASE_HPP
#define SIREMM_INTERNALTABLEBASE_HPP

#include <QSet>
#include <QVector>

#include "SireDB/tablebase.h"

#include "SireMol/molecule.h"
#include "SireMol/groupedvector.hpp"
#include "SireMol/resnumindexid.h"

SIRE_BEGIN_HEADER

namespace SireMM
{
template<class Internals>
class InternalTableBase;
}

class QDataStream;

template<class Internals>
QDataStream& operator<<(QDataStream&, const SireMM::InternalTableBase<Internals>&);
template<class Internals>
QDataStream& operator>>(QDataStream&, SireMM::InternalTableBase<Internals>&);

namespace SireMM
{

using SireMol::ResNum;
using SireMol::GroupID;
using SireMol::GroupIndexID;
using SireMol::ResNumIndexID;
using SireMol::Molecule;

using SireMol::GroupedVector;

using SireDB::TableBase;

/** This is the grand base class of all InternalTables...! This class is
    pretty much useless on its own - it is only intended for derivation
    by InternalTableBase<Internals>. The purpose of this class is
    merely to split the parameter-independent parts of InternalTable
    into a common base-class in the hope that this may reduce
    code-bloat caused by repeated instantiation of the same functions
    for different parameter types for the same internal.

    \param Internals   The type of the Info object used to hold the metadata
                       about the internals, e.g. MolBondInfo, MolAngleInfo etc.

    @author Christopher Woods
*/
template<class Internals>
class InternalTableBase : public TableBase
{

friend QDataStream& ::operator<<<>(QDataStream&, const InternalTableBase<Internals>&);
friend QDataStream& ::operator>><>(QDataStream&, InternalTableBase<Internals>&);

public:
    /** Expose the type of the internal from the metadata class */
    typedef typename Internals::internal_type internal_type;

    /** Get the type of the exception to throw if an internal is missing
        (can get this from the Internals type) */
    typedef typename Internals::missing_error_type missing_error_type;

    InternalTableBase();
    InternalTableBase(const Internals &molinfo);

    InternalTableBase(const InternalTableBase<Internals> &other);

    ~InternalTableBase();

    const Internals& info() const;

    bool isEmpty() const;

    int size() const;
    int count() const;

    bool isCompatibleWith(const Molecule &molecule) const;

    int nInternals() const;
    int nInternals(ResNum resnum) const;
    int nInternals(GroupID groupid) const;
    int nIntraInternals() const;
    int nInterInternals() const;
    int nIntraInternals(ResNum resnum) const;
    int nInterInternals(ResNum resnum) const;

    virtual int nParameters() const=0;
    virtual int nParameters(ResNum resnum) const=0;
    virtual int nParameters(GroupID groupid) const=0;
    virtual int nIntraParameters() const=0;
    virtual int nInterParameters() const=0;
    virtual int nIntraParameters(ResNum resnum) const=0;
    virtual int nInterParameters(ResNum resnum) const=0;

    QSet<GroupID> groupIDs() const;

    QVector<internal_type> internals() const;
    QVector<internal_type> internals(ResNum resnum) const;
    QVector<internal_type> internals(GroupID groupid) const;

    QVector<internal_type> interInternals() const;
    QVector<internal_type> intraInternals() const;
    QVector<internal_type> interInternals(ResNum resnum) const;
    QVector<internal_type> intraInternals(ResNum resnum) const;

    GroupedVector<GroupIndexID,internal_type> internalsByGroup() const;
    template<class C>
    GroupedVector<GroupIndexID,internal_type> internalsByGroup(
                                                    const C &gids) const;

    GroupedVector<ResNumIndexID,internal_type> internalsByResidue() const;
    template<class C>
    GroupedVector<ResNumIndexID,internal_type> internalsByResidue(
                                                    const C &resnums) const;

    GroupedVector<ResNumIndexID,internal_type> interInternalsByResidue() const;
    GroupedVector<ResNumIndexID,internal_type> intraInternalsByResidue() const;

    template<class C>
    GroupedVector<ResNumIndexID,internal_type> interInternalsByResidue(
                                                    const C &resnums) const;
    template<class C>
    GroupedVector<ResNumIndexID,internal_type> intraInternalsByResidue(
                                                    const C &resnums) const;

    virtual bool assignedParameter(const internal_type &internal) const=0;
    virtual bool assignedParameter(const GroupIndexID &id) const=0;

    virtual bool hasMissingParameters() const=0;
    virtual bool hasMissingParameters(ResNum resnum) const=0;
    virtual bool hasMissingParameters(GroupID id) const=0;

    virtual bool hasMissingIntraParameters() const=0;
    virtual bool hasMissingIntraParameters(ResNum resnum) const=0;

    virtual bool hasMissingInterParameters() const=0;
    virtual bool hasMissingInterParameters(ResNum resnum) const=0;

    virtual QSet<internal_type> missingParameters() const=0;
    virtual QSet<internal_type> missingParameters(ResNum resnum) const=0;
    virtual QSet<internal_type> missingParameters(const QSet<GroupID> &groupids) const=0;

    virtual QSet<internal_type> missingIntraParameters() const=0;
    virtual QSet<internal_type> missingIntraParameters(ResNum resnum) const=0;

    virtual QSet<internal_type> missingInterParameters() const=0;
    virtual QSet<internal_type> missingInterParameters(ResNum resnum) const=0;

    virtual void addInternal(const internal_type &internal)=0;

    template<class C>
    void addInternals(const C &internals);

    virtual void removeInternal(const internal_type &internal)=0;

    template<class C>
    void removeInternals(const C &internals);

    virtual void removeInternals()=0;
    virtual void removeInternals(ResNum resnum)=0;
    virtual void removeInternals(GroupID groupid)=0;
    virtual void removeInternals(const QSet<GroupID> &groupids)=0;

    virtual void removeIntraInternals()=0;
    virtual void removeIntraInternals(ResNum resnum)=0;

    virtual void removeInterInternals()=0;
    virtual void removeInterInternals(ResNum resnum)=0;

    virtual void removeMissingInternals()=0;
    virtual void removeMissingInternals(ResNum resnum)=0;
    virtual void removeMissingInternals(GroupID groupid)=0;
    virtual void removeMissingInternals(const QSet<GroupID> &groupids)=0;

    virtual void removeMissingIntraInternals()=0;
    virtual void removeMissingIntraInternals(ResNum resnum)=0;
    virtual void removeMissingInterInternals()=0;
    virtual void removeMissingInterInternals(ResNum resnum)=0;

    virtual void clear()=0;
    virtual void clear(ResNum resnum)=0;
    virtual void clear(GroupID groupid)=0;
    virtual void clear(const internal_type &internal)=0;

    template<class C>
    void clear(const C &bonds);

    virtual void clearIntra()=0;
    virtual void clearIntra(ResNum resnum)=0;

    virtual void clearInter()=0;
    virtual void clearInter(ResNum resnum)=0;

    template<class C>
    void clearIntra(const C &bonds);
    template<class C>
    void clearInter(const C &bonds);

protected:
    Internals& get_info();

private:
    QVector<internal_type> copyInternals(typename Internals::const_iterator &it,
                                         int ninternals) const;

    /** The metadata regarding the internals in the molecule */
    Internals molinfo;

};

/** Empty constructor */
template<class Internals>
SIRE_OUTOFLINE_TEMPLATE
InternalTableBase<Internals>::InternalTableBase() : TableBase()
{}

/** Construct a table to hold the internals in 'minfo' */
template<class Internals>
SIRE_OUTOFLINE_TEMPLATE
InternalTableBase<Internals>::InternalTableBase(const Internals &minfo)
                             : TableBase(), molinfo(minfo)
{}

/** Copy constructor */
template<class Internals>
SIRE_OUTOFLINE_TEMPLATE
InternalTableBase<Internals>::InternalTableBase(const InternalTableBase<Internals> &other)
                             : TableBase(other), molinfo(other.molinfo)
{}

/** Destructor */
template<class Internals>
SIRE_OUTOFLINE_TEMPLATE
InternalTableBase<Internals>::~InternalTableBase()
{}

/** Return a const reference to the object that holds the metainfo
    about the internals stored in this table. This object is used
    to map an internal to a unique GroupIndexID, which is then
    used to look up the corresponding parameter in the ParameterStore
    (returned by the parameters() function) */
template<class Internals>
SIRE_INLINE_TEMPLATE
const Internals& InternalTableBase<Internals>::info() const
{
    return molinfo;
}

/** Allow derived classes to get non-const access to the info object */
template<class Internals>
SIRE_INLINE_TEMPLATE
Internals& InternalTableBase<Internals>::get_info()
{
    return molinfo;
}

/** Return whether or not this table is compatible with the molecule 'mol',
    i.e. that mol.info() == info().info() */
template<class Internals>
SIRE_INLINE_TEMPLATE
bool InternalTableBase<Internals>::isCompatibleWith(const Molecule &mol) const
{
    return mol.info() == molinfo.info();
}

/** Return whether or not this parameter table is empty
    (has no internals, rather than no parameters) */
template<class Internals>
SIRE_INLINE_TEMPLATE
bool InternalTableBase<Internals>::isEmpty() const
{
    return molinfo.isEmpty();
}

/** Return the number of internals in this table */
template<class Internals>
SIRE_INLINE_TEMPLATE
int InternalTableBase<Internals>::size() const
{
    return molinfo.nInternals();
}

/** Return the number of internals in this table */
template<class Internals>
SIRE_INLINE_TEMPLATE
int InternalTableBase<Internals>::count() const
{
    return molinfo.nInternals();
}

/** Return the number of internals in this table. This will equal
    nParameters() if all of the parameters have been assigned */
template<class Internals>
SIRE_INLINE_TEMPLATE
int InternalTableBase<Internals>::nInternals() const
{
    return molinfo.nInternals();
}

/** Return the number of internals in residue 'resnum' in this table.
    This will equal nParameters(resnum) if all of the parameters in
    this residue have been assigned. */
template<class Internals>
SIRE_INLINE_TEMPLATE
int InternalTableBase<Internals>::nInternals(ResNum resnum) const
{
    return molinfo.nInternals(resnum);
}

/** Return the number of internals in the group with ID 'groupid'.
    This will equal nParameters(groupid) if all of the parameters
    in this group have been assigned. */
template<class Internals>
SIRE_INLINE_TEMPLATE
int InternalTableBase<Internals>::nInternals(GroupID groupid) const
{
    return molinfo.nInternals(groupid);
}

/** Return the total number of intra-residue internals.
    This will equal nIntraParameters() if all of the parameters
    for the intra-residue internals have been assigned. */
template<class Internals>
SIRE_INLINE_TEMPLATE
int InternalTableBase<Internals>::nIntraInternals() const
{
    return molinfo.nIntraInternals();
}

/** Return the total number of inter-residue internals.
    This will equal nIntraParameters() if all of the parameters
    for the intra-residue internals have been assigned. */
template<class Internals>
SIRE_INLINE_TEMPLATE
int InternalTableBase<Internals>::nInterInternals() const
{
    return molinfo.nInterInternals();
}

/** Return the number of intra-residue internals in residue 'resnum'.
    This will equal nIntraParameters(resnum) if all of the parameters
    for the intra-residue internals have been assigned. */
template<class Internals>
SIRE_INLINE_TEMPLATE
int InternalTableBase<Internals>::nIntraInternals(ResNum resnum) const
{
    return molinfo.nIntraInternals(resnum);
}

/** Return the number of inter-residue internals in residue 'resnum'.
    This will equal nIntraParameters(resnum) if all of the parameters
    for the intra-residue internals have been assigned. */
template<class Internals>
SIRE_INLINE_TEMPLATE
int InternalTableBase<Internals>::nInterInternals(ResNum resnum) const
{
    return molinfo.nInterInternals(resnum);
}

/** Return the set of ID numbers for all of the groups in this table */
template<class Internals>
SIRE_INLINE_TEMPLATE
QSet<GroupID> InternalTableBase<Internals>::groupIDs() const
{
    return molinfo.groupIDs();
}

/** Internal function used to copy all of the internals from the
    passed iterator */
template<class Internals>
SIRE_OUTOFLINE_TEMPLATE
QVector<typename Internals::internal_type>
InternalTableBase<Internals>::copyInternals(typename Internals::const_iterator &it,
                                            int ninternals) const
{
    if (ninternals > 0)
    {
        QVector<internal_type> allints;
        allints.reserve(ninternals);

        for (it.toBegin(); it.isValid(); ++it)
        {
            allints.append( it.key() );
        }

        return allints;
    }
    else
        return QVector<internal_type>();
}

/** Return all of the internals in one large vector. This involves copying
    all of the internals so may be slow. The order of the internals in
    this vector is guaranteed to be the same as the order of the
    corresponding parameters as returned by InternalTable::parameters() */
template<class Internals>
SIRE_INLINE_TEMPLATE
QVector<typename Internals::internal_type>
InternalTableBase<Internals>::internals() const
{
    typename Internals::const_iterator it = molinfo.internals();
    return copyInternals( it, molinfo.nInternals() );
}

/** Return all of the internals in residue 'resnum' in one large vector. This
    involves copying the internals, so may be slow. The order of the internals in
    this vector is guaranteed to be the same as the order of the
    corresponding parameters as returned by InternalTable::parameters(resnum) */
template<class Internals>
SIRE_INLINE_TEMPLATE
QVector<typename Internals::internal_type>
InternalTableBase<Internals>::internals(ResNum resnum) const
{
    typename Internals::const_iterator it = molinfo.internals(resnum);
    return copyInternals( it, molinfo.nInternals(resnum) );
}

/** Return all of the internals in the group with ID 'groupid' in one vector.
    This involves copying the internals, so may be slow. The order of the internals
    is guaranteed to be the same as the order of the corresponding parameters
    as returned by InternalTable::parameters(groupid) */
template<class Internals>
SIRE_INLINE_TEMPLATE
QVector<typename Internals::internal_type>
InternalTableBase<Internals>::internals(GroupID groupid) const
{
    typename Internals::const_iterator it = molinfo.internals(groupid);
    return copyInternals( it, molinfo.nInternals(groupid) );
}

/** Return all of the inter-residue internals in one large vector. This involves copying
    all of the internals so may be slow. The order of the internals in
    this vector is guaranteed to be the same as the order of the
    corresponding parameters as returned by InternalTable::interParameters() */
template<class Internals>
SIRE_INLINE_TEMPLATE
QVector<typename Internals::internal_type>
InternalTableBase<Internals>::interInternals() const
{
    typename Internals::const_iterator it = molinfo.interInternals();
    return copyInternals( it, molinfo.nInterInternals() );
}

/** Return all of the intra-residue internals in one large vector. This involves copying
    all of the internals so may be slow. The order of the internals in
    this vector is guaranteed to be the same as the order of the
    corresponding parameters as returned by InternalTable::intraParameters() */
template<class Internals>
SIRE_INLINE_TEMPLATE
QVector<typename Internals::internal_type>
InternalTableBase<Internals>::intraInternals() const
{
    typename Internals::const_iterator it = molinfo.intraInternals();
    return copyInternals( it, molinfo.nIntraInternals() );
}

/** Return all of the inter-residue internals in residue 'resnum' in one large vector. This
    involves copying the internals, so may be slow. The order of the internals in
    this vector is guaranteed to be the same as the order of the
    corresponding parameters as returned by InternalTable::interParameters(resnum) */
template<class Internals>
SIRE_OUTOFLINE_TEMPLATE
QVector<typename Internals::internal_type>
InternalTableBase<Internals>::interInternals(ResNum resnum) const
{
    typename Internals::const_iterator it = molinfo.interInternals(resnum);
    return copyInternals( it, molinfo.nInterInternals(resnum) );
}

/** Return all of the intra-residue internals in residue 'resnum' in one large vector. This
    involves copying the internals, so may be slow. The order of the internals in
    this vector is guaranteed to be the same as the order of the
    corresponding parameters as returned by InternalTable::intraParameters(resnum) */
template<class Internals>
SIRE_OUTOFLINE_TEMPLATE
QVector<typename Internals::internal_type>
InternalTableBase<Internals>::intraInternals(ResNum resnum) const
{
    typename Internals::const_iterator it = molinfo.intraInternals(resnum);
    return copyInternals( it, molinfo.nIntraInternals(resnum) );
}

/** Return a copy of all of the internals in the groups with IDs in 'groupids'.
    The returned GroupedVector can be accessed using the GroupIndexID
    numbers contained in MolInfo, and stores the internals in the same order
    as the corresponding parameters returned by
    InternalTable::parametersByGroup(groupids) */
template<class Internals>
template<class C>
SIRE_OUTOFLINE_TEMPLATE
GroupedVector<GroupIndexID, typename Internals::internal_type>
InternalTableBase<Internals>::internalsByGroup(const C &groupids) const
{
    if (not groupids.isEmpty())
    {
        GroupedVector<GroupIndexID,internal_type> allints;

        allints.reserve( groupids.count() );

        for (typename C::const_iterator it = groupids.begin();
             it != groupids.end();
             ++it)
        {
            QVector<internal_type> ints = this->internals(*it);

            if ( not ints.isEmpty() )
                //only add the internals if there actually are some!
                allints.insert( *it, ints );
        }

        if (not allints.isEmpty())
            return allints;
    }

    return GroupedVector<GroupIndexID,internal_type>();
}

/** Return a copy of all of the internals in this table as a GroupedVector organised
    by group. The returned GroupedVector can be accessed using the GroupIndexID
    numbers contained in MolInfo, and stores the internals in the same order
    as the corresponding parameters returned by
    InternalTable::parametersByGroup() */
template<class Internals>
SIRE_OUTOFLINE_TEMPLATE
GroupedVector<GroupIndexID, typename Internals::internal_type>
InternalTableBase<Internals>::internalsByGroup() const
{
    return internalsByGroup( molinfo.groupIDs() );
}

/** Return a copy of all of the internals in the residues whose residue numbers
    are in 'resnums'. The returned GroupedVector will store the internals
    in the same order as the parameters returned by
    InternalTable::parametersByResidue(resnums) */
template<class Internals>
template<class C>
SIRE_OUTOFLINE_TEMPLATE
GroupedVector<ResNumIndexID, typename Internals::internal_type>
InternalTableBase<Internals>::internalsByResidue(const C &resnums) const
{
    if ( not resnums.isEmpty() )
    {
        GroupedVector<ResNumIndexID,internal_type> allints;
        allints.reserve( resnums.count() );

        for (typename C::const_iterator it = resnums.begin();
             it != resnums.end();
             ++it)
        {
            QVector<internal_type> ints = this->internals(*it);

            if ( not ints.isEmpty() )
                allints.insert( *it, ints );
        }

        if (not allints.isEmpty())
            return allints;
    }

    return GroupedVector<ResNumIndexID,internal_type>();
}

/** Return a copy of all of the internals in this table as a GroupedVector
    organised by residue number. The returned GroupedVector will store
    the internals in the same order as the parameters returned by
    InternalTable::parametersByResidue() */
template<class Internals>
SIRE_OUTOFLINE_TEMPLATE
GroupedVector<ResNumIndexID, typename Internals::internal_type>
InternalTableBase<Internals>::internalsByResidue() const
{
    return internalsByResidue( molinfo.info().residueNumbers() );
}

/** Return a copy of all of the inter-residue internals in the residues whose
    residue numbers are in 'resnums'. The returned GroupedVector will store the
    internals in the same order as the parameters returned by
    InternalTable::interParametersByResidue(resnums) */
template<class Internals>
template<class C>
SIRE_OUTOFLINE_TEMPLATE
GroupedVector<ResNumIndexID, typename Internals::internal_type>
InternalTableBase<Internals>::interInternalsByResidue(const C &resnums) const
{
    if ( not resnums.isEmpty() )
    {
        GroupedVector<ResNumIndexID,internal_type> allints;
        allints.reserve( resnums.count() );

        for (typename C::const_iterator it = resnums.begin();
             it != resnums.end();
             ++it)
        {
            QVector<internal_type> ints = this->interInternals(*it);

            if ( not ints.isEmpty() )
                allints.insert( *it, ints );
        }

        if (not allints.isEmpty())
            return allints;
    }

    return GroupedVector<ResNumIndexID,internal_type>();
}

/** Return a copy of all of the intra-residue internals in the residues whose
    residue numbers are in 'resnums'. The returned GroupedVector will store the
    internals in the same order as the parameters returned by
    InternalTable::intraParametersByResidue(resnums) */
template<class Internals>
template<class C>
SIRE_OUTOFLINE_TEMPLATE
GroupedVector<ResNumIndexID, typename Internals::internal_type>
InternalTableBase<Internals>::intraInternalsByResidue(const C &resnums) const
{
    if ( not resnums.isEmpty() )
    {
        GroupedVector<ResNumIndexID,internal_type> allints;
        allints.reserve( resnums.count() );

        for (typename C::const_iterator it = resnums.begin();
             it != resnums.end();
             ++it)
        {
            QVector<internal_type> ints = this->intraInternals(*it);

            if ( not ints.isEmpty() )
                allints.insert( *it, ints );
        }

        if (not allints.isEmpty())
            return allints;
    }

    return GroupedVector<ResNumIndexID,internal_type>();
}

/** Return a copy of all of the inter-residue internals in this table as
    a GroupedVector organised by residue number. The returned GroupedVector
    will store the internals in the same order as the parameters returned by
    InternalTable::interParametersByResidue() */
template<class Internals>
SIRE_OUTOFLINE_TEMPLATE
GroupedVector<ResNumIndexID, typename Internals::internal_type>
InternalTableBase<Internals>::interInternalsByResidue() const
{
    return interInternalsByResidue( molinfo.info().residueNumbers() );
}

/** Return a copy of all of the intra-residue internals in this table as
    a GroupedVector organised by residue number. The returned GroupedVector
    will store the internals in the same order as the parameters returned by
    InternalTable::intraParametersByResidue() */
template<class Internals>
SIRE_OUTOFLINE_TEMPLATE
GroupedVector<ResNumIndexID, typename Internals::internal_type>
InternalTableBase<Internals>::intraInternalsByResidue() const
{
    return intraInternalsByResidue( molinfo.info().residueNumbers() );
}

/** Add a whole load of internals to this table - this makes space for their
    parameters, which are initially unassiged. */
template<class Internals>
template<class C>
SIRE_OUTOFLINE_TEMPLATE
void InternalTableBase<Internals>::addInternals(const C &internals)
{
    for (typename C::const_iterator it = internals.begin();
         it != internals.end();
         ++it)
    {
        this->addInternal(*it);
    }
}

/** Remove all of the internals in 'internals', and their associated
    parameters from this table */
template<class Internals>
template<class C>
SIRE_OUTOFLINE_TEMPLATE
void InternalTableBase<Internals>::removeInternals(const C &internals)
{
    for (typename C::const_iterator it = internals.begin();
         it != internals.end();
         ++it)
    {
        this->removeInternal(*it);
    }
}

/** This clears all of the parameters in 'internals' */
template<class Internals>
template<class C>
SIRE_OUTOFLINE_TEMPLATE
void InternalTableBase<Internals>::clear(const C &internals)
{
    for (typename C::const_iterator it = internals.begin();
         it != internals.end();
         ++it)
    {
        this->clear(*it);
    }
}

/** This clears all of the intra-internal parameters in 'internals' */
template<class Internals>
template<class C>
SIRE_OUTOFLINE_TEMPLATE
void InternalTableBase<Internals>::clearIntra(const C &internals)
{
    for (typename C::const_iterator it = internals.begin();
         it != internals.end();
         ++it)
    {
        this->clearIntra(*it);
    }
}

/** This clears all of the inter-internal parameters in 'internals' */
template<class Internals>
template<class C>
SIRE_OUTOFLINE_TEMPLATE
void InternalTableBase<Internals>::clearInter(const C &internals)
{
    for (typename C::const_iterator it = internals.begin();
         it != internals.end();
         ++it)
    {
        this->clearInter(*it);
    }
}

const MagicID internaltablebase_magic = getMagic("SireMM::InternalTableBase");

}

/** Serialise to a binary data stream */
template<class Internals>
SIRE_OUTOFLINE_TEMPLATE
QDataStream& operator<<(QDataStream &ds,
                        const SireMM::InternalTableBase<Internals> &table)
{
    SireStream::writeHeader(ds, SireMM::internaltablebase_magic, 1)
             << table.molinfo
             << static_cast<const SireDB::TableBase&>(table);

    return ds;
}

/** Deserialise from a binary data stream */
template<class Internals>
SIRE_OUTOFLINE_TEMPLATE
QDataStream& operator>>(QDataStream &ds,
                        SireMM::InternalTableBase<Internals> &table)
{
    SireStream::VersionID v = SireStream::readHeader(ds, SireMM::internaltablebase_magic,
                                                     "SireMM::InternalTableBase");

    if (v == 1)
    {
        ds >> table.molinfo
           >> static_cast<SireDB::TableBase&>(table);
    }
    else
        throw SireStream::version_error(v, "1", "SireMM::InternalTableBase", CODELOC);

    return ds;
}

SIRE_END_HEADER

#endif
