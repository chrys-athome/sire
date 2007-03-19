/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
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

#ifndef SIREMM_INTERNALTABLE_HPP
#define SIREMM_INTERNALTABLE_HPP

#include "internaltablebase.hpp"
#include "SireDB/parameterstore.hpp"

#include "SireStream/datastream.h"

SIRE_BEGIN_HEADER

namespace SireMM
{
template<class Internals, class Param>
class InternalTable;
}

class QDataStream;

template<class Internals, class Param>
QDataStream& operator<<(QDataStream&, const SireMM::InternalTable<Internals,Param>&);
template<class Internals, class Param>
QDataStream& operator>>(QDataStream&, SireMM::InternalTable<Internals,Param>&);

namespace SireMM
{

using SireMol::ResNum;
using SireMol::GroupIndexID;
using SireMol::ResNumIndexID;

/** This is the base class of all of the InternalTables

    \param Internals   The type of the Info object used to hold the metadata
                       about the internals, e.g. MolBondInfo, MolAngleInfo etc.

    \param Param       The actual parameter type stored in the table

    @author Christopher Woods
*/
template<class Internals, class Param>
class InternalTable : public InternalTableBase<Internals>
{

friend QDataStream& ::operator<<<>(QDataStream&, const InternalTable<Internals,Param>&);
friend QDataStream& ::operator>><>(QDataStream&, InternalTable<Internals,Param>&);

public:
    /** Expose the type of parameter */
    typedef Param parameter_type;

    /** Expose the type of the internal from the metadata class */
    typedef typename Internals::internal_type internal_type;

    /** Get the type of the exception to throw if an internal is missing
        (can get this from the Internals type) */
    typedef typename Internals::missing_error_type missing_error_type;

    InternalTable();
    InternalTable(const Internals &molinfo);

    InternalTable(const InternalTable<Internals,Param> &other);

    ~InternalTable();

    int nParameters() const;
    int nParameters(ResNum resnum) const;
    int nParameters(GroupID groupid) const;
    int nIntraParameters() const;
    int nInterParameters() const;
    int nIntraParameters(ResNum resnum) const;
    int nInterParameters(ResNum resnum) const;

    const Param& at(const internal_type &internal) const;
    const Param& at(const GroupIndexID &id) const;

    const Param& operator[](const internal_type &internal) const;
    const Param& operator[](const GroupIndexID &id) const;

    Param& operator[](const internal_type &internal);
    Param& operator[](const GroupIndexID &id);

    Param value(const internal_type &internal) const;
    Param value(const internal_type &internal, const Param &defaultParam) const;

    Param value(const GroupIndexID &id) const;
    Param value(const GroupIndexID &id, const Param &defaultParam) const;

    const Param& getParameter(const internal_type &internal) const;
    const Param& getParameter(const GroupIndexID &id) const;

    void setParameter(const internal_type &internal, const Param &param);
    void setParameter(const GroupIndexID &id, const Param &param);

    bool assignedParameter(const internal_type &internal) const;
    bool assignedParameter(const GroupIndexID &id) const;

    bool hasMissingParameters() const;
    bool hasMissingParameters(ResNum resnum) const;
    bool hasMissingParameters(GroupID id) const;

    bool hasMissingIntraParameters() const;
    bool hasMissingIntraParameters(ResNum resnum) const;

    bool hasMissingInterParameters() const;
    bool hasMissingInterParameters(ResNum resnum) const;

    QSet<internal_type> missingParameters() const;
    QSet<internal_type> missingParameters(ResNum resnum) const;
    QSet<internal_type> missingParameters(const QSet<GroupID> &groupids) const;

    QSet<internal_type> missingIntraParameters() const;
    QSet<internal_type> missingIntraParameters(ResNum resnum) const;

    QSet<internal_type> missingInterParameters() const;
    QSet<internal_type> missingInterParameters(ResNum resnum) const;

    void addInternal(const internal_type &internal);
    void addInternal(const internal_type &internal, const Param &param);

    template<class C>
    void addInternals(const C &internals);

    void removeInternal(const internal_type &internal);

    template<class C>
    void removeInternals(const C &internals);

    void removeInternals();
    void removeInternals(ResNum resnum);
    void removeInternals(GroupID groupid);
    void removeInternals(const QSet<GroupID> &groupids);

    void removeIntraInternals();
    void removeIntraInternals(ResNum resnum);

    void removeInterInternals();
    void removeInterInternals(ResNum resnum);

    void removeMissingInternals();
    void removeMissingInternals(ResNum resnum);
    void removeMissingInternals(GroupID groupid);
    void removeMissingInternals(const QSet<GroupID> &groupids);

    void removeMissingIntraInternals();
    void removeMissingIntraInternals(ResNum resnum);
    void removeMissingInterInternals();
    void removeMissingInterInternals(ResNum resnum);

    void clear();
    void clear(ResNum resnum);
    void clear(GroupID groupid);
    void clear(const internal_type &internal);

    template<class C>
    void clear(const C &bonds);

    void clearIntra();
    void clearIntra(ResNum resnum);

    void clearInter();
    void clearInter(ResNum resnum);

    template<class C>
    void clearIntra(const C &bonds);
    template<class C>
    void clearInter(const C &bonds);

    QVector<Param> parameters() const;
    QVector<Param> parameters(ResNum resnum) const;
    QVector<Param> parameters(GroupID groupid) const;

    QVector<Param> interParameters() const;
    QVector<Param> intraParameters() const;
    QVector<Param> interParameters(ResNum resnum) const;
    QVector<Param> intraParameters(ResNum resnum) const;

    GroupedVector<GroupIndexID,Param> parametersByGroup() const;
    template<class C>
    GroupedVector<GroupIndexID,Param> parametersByGroup(const C &gids) const;

    GroupedVector<ResNumIndexID,Param> parametersByResidue() const;
    template<class C>
    GroupedVector<ResNumIndexID,Param> parametersByResidue(const C &resnums) const;

    GroupedVector<ResNumIndexID,Param> interParametersByResidue() const;
    GroupedVector<ResNumIndexID,Param> intraParametersByResidue() const;

    template<class C>
    GroupedVector<ResNumIndexID,Param> interParametersByResidue(
                                                    const C &resnums) const;
    template<class C>
    GroupedVector<ResNumIndexID,Param> intraParametersByResidue(
                                                    const C &resnums) const;

private:
    QVector<Param> copyParameters(typename Internals::const_iterator &it, int nparams) const;

    /** The parameters in the table */
    SireDB::ParameterStore<GroupIndexID,Param> params;
};

#ifndef SKIP_TEMPLATE_DEFINITIONS

/** Empty constructor */
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
InternalTable<Internals,Param>::InternalTable() : InternalTableBase<Internals>()
{}

/** Construct a table that holds the parameters for the internals described
    in 'info' */
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
InternalTable<Internals,Param>::InternalTable(const Internals &info)
                               : InternalTableBase<Internals>(info),
                                 params(info.groupSizes())
{}

/** Copy constructor - fast as this class is implicitly shared */
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
InternalTable<Internals,Param>::InternalTable(const InternalTable<Internals,Param> &other)
                               : InternalTableBase<Internals>(other),
                                 params(other.params)
{}

/** Destructor */
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
InternalTable<Internals,Param>::~InternalTable()
{}

/** Return the number of assigned parameters in this table.
    This will equal nInternals() if all of the parameters have
    been assigned */
template<class Internals, class Param>
SIRE_INLINE_TEMPLATE
int InternalTable<Internals,Param>::nParameters() const
{
    return params.nParameters();
}

/** Return the number of assigned parameters in the residue 'resnum'. This
    will equal nInternals(resnum) if all of the parameters have been assigned. */
template<class Internals, class Param>
SIRE_INLINE_TEMPLATE
int InternalTable<Internals,Param>::nParameters(ResNum resnum) const
{
    return params.nParameters( this->info().groupIDs(resnum) );
}

/** Return the number of assigned parameters in the group with ID 'groupid'.
    This will equal nInternals(groupid) if all of the parameters have been assigned. */
template<class Internals, class Param>
SIRE_INLINE_TEMPLATE
int InternalTable<Internals,Param>::nParameters(GroupID groupid) const
{
    return params.nParameters(groupid);
}

/** Return the total number of intra-residue parameters. This will equal nIntraInternals()
    if all of the parameters have been assigned. */
template<class Internals, class Param>
int InternalTable<Internals,Param>::nIntraParameters() const
{
    return params.nParameters( this->info().intraGroupIDs() );
}

/** Return the total number of inter-residue parameters. This will equal nInterInternals()
    if all of the parameters have been assigned. */
template<class Internals, class Param>
int InternalTable<Internals,Param>::nInterParameters() const
{
    return params.nParameters( this->info().interGroupIDs() );
}

/** Return the number of assigned intra-residue parameters in the residue with number 'resnum'.
    This will equal nInternals(resnum) if all of the parameters have been assigned. */
template<class Internals, class Param>
SIRE_INLINE_TEMPLATE
int InternalTable<Internals,Param>::nIntraParameters(ResNum resnum) const
{
    return params.nParameters( this->info().intraGroupIDs(resnum) );
}

/** Return the number of assigned inter-residue parameters in the residue with number 'resnum'.
    This will equal nInternals(resnum) if all of the parameters have been assigned. */
template<class Internals, class Param>
SIRE_INLINE_TEMPLATE
int InternalTable<Internals,Param>::nInterParameters(ResNum resnum) const
{
    return params.nParameters( this->info().interGroupIDs(resnum) );
}

/** Return the parameter for the internal 'internal'. This will throw
    an exception if either the parameter for this internal has not
    yet been assigned, or if this internal is not present in
    this table

    \throw SireDB::missing_parameter
    \throw missing_error_type
*/
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
const Param& InternalTable<Internals,Param>::at(const internal_type &internal) const
{
    return params.at( this->info().index(internal) );
}

/** Return the parameter for the index 'index'. This will throw an exception
    if either the parameter for this index has not yet been assigned, or
    if this is an invalid index

    \throw SireDB::missing_parameter
    \throw SireError::invalid_key
    \throw SireError::invalid_index
*/
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
const Param& InternalTable<Internals,Param>::at(const GroupIndexID &index) const
{
    return params.at( index );
}

/** Return the parameter for the internal 'internal'. This will throw
    an exception if either the parameter for this internal has not
    yet been assigned, or if this internal is not present in
    this table

    \throw SireDB::missing_parameter
    \throw missing_error_type
*/
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
const Param& InternalTable<Internals,Param>::operator[](const internal_type &internal) const
{
    return params[ this->info()[internal] ];
}

/** Return the parameter for the index 'index'. This will throw an exception
    if either the parameter for this index has not yet been assigned, or
    if this is an invalid index

    \throw SireDB::missing_parameter
    \throw SireError::invalid_key
    \throw SireError::invalid_index
*/
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
const Param& InternalTable<Internals,Param>::operator[](const GroupIndexID &id) const
{
    return params[id];
}

/** Return a modifiable reference to the parameter for the internal 'internal'.
    This will throw an exception if this internal is not in the table, but
    will silently create a default parameter if the parameter has not previously
    been assigned.

    \throw missing_error_type
*/
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
Param& InternalTable<Internals,Param>::operator[](const internal_type &internal)
{
    return params[ this->info()[internal] ];
}

/** Return a modifiable reference to the parameter for the internal 'internal'.
    This will throw an exception if this is an invalid index, but will silently
    create a default parameter if the parameter has not previously been assigned.

    \throw missing_error_type
*/
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
Param& InternalTable<Internals,Param>::operator[](const GroupIndexID &id)
{
    return params[id];
}

/** Return the value of the parameter for the internal 'internal', or return
    a default-constructed value if the parameter has not been set. This will
    throw an exception if this internal is not in this table.

    \throw missing_error_type
*/
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
Param InternalTable<Internals,Param>::value(
                      const typename Internals::internal_type &internal) const
{
    return params.value( this->info()[internal] );
}

/** Return the value of the parameter for the internal 'internal', or return
    'defaultParam' if the parameter has not been set. This will throw an exception
    if this this internal is not in this table.

    \throw missing_error_type
*/
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
Param InternalTable<Internals,Param>::value(
                                const typename Internals::internal_type &internal,
                                const Param &defaultParam) const
{
    return params.value( this->info()[internal], defaultParam );
}

/** Return the value of the parameter for the index 'id', or return
    a default-constructed value if the parameter has not been set. This will
    throw an exception if this index is invalid.

    \throw SireError::invalid_index
    \throw SireError::invalid_key
*/
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
Param InternalTable<Internals,Param>::value(const GroupIndexID &id) const
{
    return params.value( id );
}

/** Return the value of the parameter for the index 'id', or return
    'defaultParam' if the parameter has not been set. This will throw an exception
    if this this index is invalid.

    \throw missing_error_type
*/
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
Param InternalTable<Internals,Param>::value(const GroupIndexID &id,
                                            const Param &defaultParam) const
{
    return params.value( id, defaultParam );
}

/** Return the value of the parameter for the internal 'internal'. This will
    throw an exception if either the parameter has not yet been assigned, or
    if the internal is not in this table.

    \throw missing_error_type
    \throw SireDB::missing_parameter
*/
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
const Param& InternalTable<Internals,Param>::getParameter(
                                const typename Internals::internal_type &internal) const
{
    return params[ this->info()[internal] ];
}

/** Return the value of the parameter for the internal with index 'id'. This will
    throw an exception if either the parameter has not yet been assigned, or
    this index is invalid

    \throw SireDB::missing_parameter
    \throw SireError::invalid_key
    \throw SireError::invalid_index
*/
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
const Param& InternalTable<Internals,Param>::getParameter(const GroupIndexID &id) const
{
    return params[ id ];
}

/** Set the parameter for the internal 'internal' to 'param'. Note that this
    will throw an exception if this internal is not in this table.

    \throw missing_error_type
*/
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
void InternalTable<Internals,Param>::setParameter(
                                  const typename Internals::internal_type &internal,
                                  const Param &param)
{
    params.setParameter( this->info()[internal], param );
}

/** Set the parameter for the internal with index 'id' to 'param'. Note that this
    will throw an exception if this index is invalid.

    \throw SireError::invalid_key
    \throw SireError::invalid_index
*/
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
void InternalTable<Internals,Param>::setParameter(const GroupIndexID &id,
                                                  const Param &param)
{
    params.setParameter( id, param );
}

/** Return whether or not the parameter for the internal 'internal' has been
    assigned. Note that this will throw an exception if this internal is
    not in this table

    \throw missing_error_type
*/
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
bool InternalTable<Internals,Param>::assignedParameter(
                            const typename Internals::internal_type &internal) const
{
    return params.assignedParameter( this->info()[internal] );
}

/** Return whether or not the parameter for the index 'id' has been assigned.
    Note that this will throw an exception if this index is invalid.

    \throw SireError::invalid_key
    \throw SireError::invalid_index
*/
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
bool InternalTable<Internals,Param>::assignedParameter(const GroupIndexID &id) const
{
    return params.assignedParameter(id);
}

/** Return whether or not there are any missing parameters */
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
bool InternalTable<Internals,Param>::hasMissingParameters() const
{
    return params.hasMissingParameters();
}

/** Return whether or not all of the parameters for the internals listed in
    the residue 'resnum' have been assigned. Note that this will return 'true'
    if there are no internals from this residue in this table (as there
    are thus no parameters missing for this internal) */
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
bool InternalTable<Internals,Param>::hasMissingParameters(ResNum resnum) const
{
    return params.hasMissingParameters( this->info().groupIDs(resnum) );
}

/** Return whether or not all of the parameters for the internals listed
    in the group with ID 'id' have been assigned */
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
bool InternalTable<Internals,Param>::hasMissingParameters(GroupID id) const
{
    return params.hasMissingParameters(id);
}

/** Return whether or not all of the parameters for intra-residue internals
    have been assigned (if there are any missing) */
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
bool InternalTable<Internals,Param>::hasMissingIntraParameters() const
{
    return params.hasMissingParameters( this->info().intraGroupIDs() );
}

/** Return whether or not all of the parameters for the intra-residue internals
    in residue 'resnum' have been assigned (are there any missing?) */
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
bool InternalTable<Internals,Param>::hasMissingIntraParameters(ResNum resnum) const
{
    return params.hasMissingParameters( this->info().intraGroupIDs(resnum) );
}

/** Return whether or not all of the parameters for inter-residue internals
    have been assigned (if there are any missing) */
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
bool InternalTable<Internals,Param>::hasMissingInterParameters() const
{
    return params.hasMissingParameters( this->info().interGroupIDs() );
}

/** Return whether or not all of the parameters for the inter-residue internals
    in residue 'resnum' have been assigned (are there any missing?) */
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
bool InternalTable<Internals,Param>::hasMissingInterParameters(ResNum resnum) const
{
    return params.hasMissingParameters( this->info().interGroupIDs(resnum) );
}

/** Return the set of internals that have missing parameters. This will return
    an empty set unless hasMissingParameters() returns true */
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
QSet<typename Internals::internal_type>
InternalTable<Internals,Param>::missingParameters() const
{
    return this->info().getInternals( params.missingParameters() );
}

/** Return the set of internals in the groups whose ID numbers are
    in 'groupids' that have missing parameters */
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
QSet<typename Internals::internal_type>
InternalTable<Internals,Param>::missingParameters(const QSet<GroupID> &groupids) const
{
    return this->info().getInternals( params.missingParameters(groupids) );
}

/** Return the set of internals that have missing parameters in residue 'resnum'.
    This will return an empty set unless hasMissingParameters(ResNum) returns true */
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
QSet<typename Internals::internal_type>
InternalTable<Internals,Param>::missingParameters(ResNum resnum) const
{
    return this->missingParameters( this->info().groupIDs(resnum) );
}

/** Return the set of intra-residue internals that have missing parameters. This
    will return an empty set unless hasMissingIntraParameters() returns true */
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
QSet<typename Internals::internal_type>
InternalTable<Internals,Param>::missingIntraParameters() const
{
    return this->missingParameters( this->info().intraGroupIDs() );
}

/** Return the set of intra-residue internals that have missing parameters in residue 'resnum'.
    This will return an empty set unless hasMissingIntraParameters(ResNum) returns true */
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
QSet<typename Internals::internal_type>
InternalTable<Internals,Param>::missingIntraParameters(ResNum resnum) const
{
    return this->missingParameters( this->info().intraGroupIDs(resnum) );
}

/** Return the set of inter-residue internals that have missing parameters. This
    will return an empty set unless hasMissingInterParameters() returns true */
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
QSet<typename Internals::internal_type>
InternalTable<Internals,Param>::missingInterParameters() const
{
    return this->missingParameters( this->info().interGroupIDs() );
}

/** Return the set of inter-residue internals that have missing parameters in residue 'resnum'.
    This will return an empty set unless hasMissingInterParameters(ResNum) returns true */
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
QSet<typename Internals::internal_type>
InternalTable<Internals,Param>::missingInterParameters(ResNum resnum) const
{
    return this->missingParameters( this->info().interGroupIDs(resnum) );
}

/** Add an internal to the table - this will make a space for the parameter
    (which is initially unassigned). This does nothing if this internal
    is already in the table */
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
void InternalTable<Internals,Param>::addInternal(
                            const typename Internals::internal_type &internal)
{
    params.addParameter( this->get_info().addInternal(internal) );
}

/** Add an internal to the table and assign it the parameter 'param'. This overwrites
    the existing parameter if this internal is already in this table. */
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
void InternalTable<Internals,Param>::addInternal(
                            const typename Internals::internal_type &internal,
                            const Param &param)
{
    params.addParameter( this->get_info().addInternal(internal), param );
}

/** Add a whole load of internals to this table - this makes space for their
    parameters, which are initially unassiged. */
template<class Internals, class Param>
template<class C>
SIRE_OUTOFLINE_TEMPLATE
void InternalTable<Internals,Param>::addInternals(const C &internals)
{
    for (typename C::const_iterator it = internals.begin();
         it != internals.end();
         ++it)
    {
        this->addInternal(*it);
    }
}

/** Remove an internal, and its associated parameter, from this table.
    This does nothing if this parameter is not in this table. Note that
    this may be slow as it may involve reindexing all of the internals
    in a group */
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
void InternalTable<Internals,Param>::removeInternal(
                                  const typename Internals::internal_type &internal)
{
    if ( this->info().contains(internal) )
    {
        GroupIndexID id = this->info()[internal];
        this->get_info().removeInternal(id);
        params.remove(id);
    }
}

/** Remove all internals and their corresponding parameters from this table */
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
void InternalTable<Internals,Param>::removeInternals()
{
    this->get_info().removeInternals();
    params.removeAll();
}

/** Remove all of the internals in 'internals', and their associated
    parameters from this table */
template<class Internals, class Param>
template<class C>
SIRE_OUTOFLINE_TEMPLATE
void InternalTable<Internals,Param>::removeInternals(const C &internals)
{
    for (typename C::const_iterator it = internals.begin();
         it != internals.end();
         ++it)
    {
        this->removeInternal(*it);
    }
}

/** Remove all of the internals from the group with ID number 'id'.
    This does nothing if there is no such group in the table */
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
void InternalTable<Internals,Param>::removeInternals(GroupID groupid)
{
    this->get_info().removeInternals(groupid);
    params.remove(groupid);
}

/** Remove all of the internals from the groups whose ID numbers
    are in 'groupids' */
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
void InternalTable<Internals,Param>::removeInternals(const QSet<GroupID> &groupids)
{
    this->get_info().removeInternals(groupids);
    params.remove(groupids);
}

/** Remove all of the internals from residue 'resnum', and their
    associated parameters */
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
void InternalTable<Internals,Param>::removeInternals(ResNum resnum)
{
    this->removeInternals( this->info().groupIDs(resnum) );
}

/** Remove all of the intra-residue internals from this table, and
    their associated parameters. */
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
void InternalTable<Internals,Param>::removeIntraInternals()
{
    this->removeInternals( this->info().intraGroupIDs() );
}

/** Remove all of the intra-residue internals in residue 'resnum',
    and their associated parameters, from this table */
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
void InternalTable<Internals,Param>::removeIntraInternals(ResNum resnum)
{
    this->removeInternals( this->info().intraGroupIDs(resnum) );
}

/** Remove all of the inter-residue internals from this table, and
    their associated parameters. */
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
void InternalTable<Internals,Param>::removeInterInternals()
{
    this->removeInternals( this->info().interGroupIDs() );
}

/** Remove all of the inter-residue internals in residue 'resnum',
    and their associated parameters, from this table */
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
void InternalTable<Internals,Param>::removeInterInternals(ResNum resnum)
{
    this->removeInternals( this->info().interGroupIDs(resnum) );
}

/** Remove all of the internals in the passed groups that are missing
    parameters */
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
void InternalTable<Internals,Param>::removeMissingInternals(const QSet<GroupID> &groupids)
{
    //get the IDs of the missing parameters
    QSet<GroupIndexID> missing_ids = params.missingParameters(groupids);

    //remove these parameters from the parameter store
    params.remove(groupids);

    //now remove these internals from the list of internals...
    this->get_info().removeInternals(missing_ids);
}

/** Remove all of the internals in this group that are missing parameters */
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
void InternalTable<Internals,Param>::removeMissingInternals(GroupID groupid)
{
    QSet<GroupID> groupids;
    groupids.insert(groupid);
    this->removeMissingInternals(groupids);
}

/** Remove all of the internals that are missing parameters. */
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
void InternalTable<Internals,Param>::removeMissingInternals()
{
    this->removeMissingInternals( this->info().groupIDs() );
}

/** Remove all of the internals in the residue 'resnum' that are missing
    parameters */
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
void InternalTable<Internals,Param>::removeMissingInternals(ResNum resnum)
{
    this->removeMissingInternals( this->info().groupIDs(resnum) );
}

/** Remove all intra-residue internals that are missing parameters */
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
void InternalTable<Internals,Param>::removeMissingIntraInternals()
{
    this->removeMissingInternals( this->info().intraGroupIDs() );
}

/** Remove all inter-residue internals that are missing parameters */
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
void InternalTable<Internals,Param>::removeMissingInterInternals()
{
    this->removeMissingInternals( this->info().interGroupIDs() );
}

/** Remove all of the intra-residue internals in residue 'resnum' that are
    missing parameters */
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
void InternalTable<Internals,Param>::removeMissingIntraInternals(ResNum resnum)
{
    this->removeMissingInternals( this->info().intraGroupIDs(resnum) );
}

/** Remove all of the inter-residue internals in residue 'resnum' that are
    missing parameters */
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
void InternalTable<Internals,Param>::removeMissingInterInternals(ResNum resnum)
{
    this->removeMissingInternals( this->info().interGroupIDs(resnum) );
}

/** This clears all of the parameters from this table - this doesn't remove
    the internal - it just removes the parameters. */
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
void InternalTable<Internals,Param>::clear()
{
    params.clear();
}

/** This clears all of the parameters in the group with ID 'groupid' */
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
void InternalTable<Internals,Param>::clear(GroupID groupid)
{
    params.clear(groupid);
}

/** This clears the parameter for the individual internal 'internal' */
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
void InternalTable<Internals,Param>::clear(const typename Internals::internal_type &internal)
{
    params.clear( this->info()[internal] );
}

/** This clears all of the parameters in 'internals' */
template<class Internals, class Param>
template<class C>
SIRE_OUTOFLINE_TEMPLATE
void InternalTable<Internals,Param>::clear(const C &internals)
{
    for (typename C::const_iterator it = internals.begin();
         it != internals.end();
         ++it)
    {
        this->clear(*it);
    }
}

/** This clears all of the parameters for the internals in residue 'resnum' */
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
void InternalTable<Internals,Param>::clear(ResNum resnum)
{
    params.clear( this->info().groupIDs(resnum) );
}

/** This clears all of the parameters for the intra-residue internals
    in this table. */
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
void InternalTable<Internals,Param>::clearIntra()
{
    params.clear( this->info().intraGroupIDs() );
}

/** This clears all of the parameters for the intra-residue internals
    in residue 'resnum' in this table. */
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
void InternalTable<Internals,Param>::clearIntra(ResNum resnum)
{
    params.clear( this->info().intraGroupIDs(resnum) );
}

/** This clears all of the parameters for the inter-residue internals
    in this table. */
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
void InternalTable<Internals,Param>::clearInter()
{
    params.clear( this->info().interGroupIDs() );
}

/** This clears all of the parameters for the inter-residue internals
    in residue 'resnum' in this table. */
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
void InternalTable<Internals,Param>::clearInter(ResNum resnum)
{
    params.clear( this->info().interGroupIDs(resnum) );
}

/** This clears all of the inter-internal parameters in 'internals' */
template<class Internals, class Param>
template<class C>
SIRE_OUTOFLINE_TEMPLATE
void InternalTable<Internals,Param>::clearInter(const C &internals)
{
    for (typename C::const_iterator it = internals.begin();
         it != internals.end();
         ++it)
    {
        this->clearInter(*it);
    }
}

/** This clears all of the intra-internal parameters in 'internals' */
template<class Internals, class Param>
template<class C>
SIRE_OUTOFLINE_TEMPLATE
void InternalTable<Internals,Param>::clearIntra(const C &internals)
{
    for (typename C::const_iterator it = internals.begin();
         it != internals.end();
         ++it)
    {
        this->clearIntra(*it);
    }
}

/** Internal function used to copy the parameters for the internals
    in the iterator 'it' from the parameter store into a vector
    of parameters, in the same order as the internals in the iterator.

    This will throw an exception if any of the parameters are unassigned.

    \throw SireMol::missing_parameter
*/
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
QVector<Param>
InternalTable<Internals,Param>::copyParameters(typename Internals::const_iterator &it,
                                               int nparams) const
{
    if (nparams > 0)
    {
        QVector<Param> allparams;
        allparams.reserve(nparams);

        for (it.toBegin(); it.isValid(); ++it)
        {
            allparams.append( params[it.value()] );
        }

        return allparams;
    }
    else
        return QVector<Param>();
}

/** Copy all of the parameters in the table into a single large vector.
    An exception will be thrown if any of the parameters have not been assigned.
    The order of the parameters will be the same as the corresponding
    internals returned by InternalTable::internals()

    This will throw an exception if any of the parameters are unassigned.

    \throw SireMol::missing_parameter
*/
template<class Internals, class Param>
SIRE_INLINE_TEMPLATE
QVector<Param> InternalTable<Internals,Param>::parameters() const
{
    typename Internals::const_iterator it = this->info().internals();
    return copyParameters( it, this->info().nInternals() );
}

/** Copy all of the parameters in the table for the residue 'resnum'
    into a single large vector. An exception will be thrown if any of the
    parameters have not been assigned. The order of the parameters will
    be the same as the corresponding internals returned by
    InternalTable::internals(resnum)

    This will throw an exception if any of the parameters are unassigned.

    \throw SireMol::missing_parameter
*/
template<class Internals, class Param>
SIRE_INLINE_TEMPLATE
QVector<Param> InternalTable<Internals,Param>::parameters(ResNum resnum) const
{
    typename Internals::const_iterator it = this->info().internals(resnum);
    return copyParameters( it, this->info().nInternals(resnum) );
}

/** Copy all of the parameters in the table for the group with ID 'groupid'
    into a single large vector. An exception will be thrown if any of the
    parameters have not been assigned. The order of the parameters will
    be the same as the corresponding internals returned by
    InternalTable::internals(groupid)

    This will throw an exception if any of the parameters are unassigned.

    \throw SireMol::missing_parameter
*/
template<class Internals, class Param>
SIRE_INLINE_TEMPLATE
QVector<Param> InternalTable<Internals,Param>::parameters(GroupID groupid) const
{
    typename Internals::const_iterator it = this->info().internals(groupid);
    return copyParameters( it, this->info().nInternals(groupid) );
}

/** Copy all of the inter-residue parameters in the table
    into a single large vector. An exception will be thrown if any
    of the parameters have not been assigned. The order of the parameters
    will be the same as the corresponding internals returned by
    InternalTable::interInternals()

    This will throw an exception if any of the parameters are unassigned.

    \throw SireMol::missing_parameter
*/
template<class Internals, class Param>
SIRE_INLINE_TEMPLATE
QVector<Param> InternalTable<Internals,Param>::interParameters() const
{
    typename Internals::const_iterator it = this->info().interInternals();
    return copyParameters( it, this->info().nInterInternals() );
}

/** Copy all of the intra-residue parameters in the table
    into a single large vector. An exception will be thrown if any
    of the parameters have not been assigned. The order of the parameters
    will be the same as the corresponding internals returned by
    InternalTable::intraInternals()

    This will throw an exception if any of the parameters are unassigned.

    \throw SireMol::missing_parameter
*/
template<class Internals, class Param>
SIRE_INLINE_TEMPLATE
QVector<Param> InternalTable<Internals,Param>::intraParameters() const
{
    typename Internals::const_iterator it = this->info().intraInternals();
    return copyParameters( it, this->info().nIntraInternals() );
}

/** Copy all of the inter-residue parameters in the table for the residue
    'resnum' into a single large vector. An exception will be thrown if any
    of the parameters have not been assigned. The order of the parameters
    will be the same as the corresponding internals returned by
    InternalTable::interInternals(resnum)

    This will throw an exception if any of the parameters are unassigned.

    \throw SireMol::missing_parameter
*/
template<class Internals, class Param>
SIRE_INLINE_TEMPLATE
QVector<Param> InternalTable<Internals,Param>::interParameters(ResNum resnum) const
{
    typename Internals::const_iterator it = this->info().interInternals(resnum);
    return copyParameters( it, this->info().nInterInternals(resnum) );
}

/** Copy all of the intra-residue parameters in the table for the residue
    'resnum' into a single large vector. An exception will be thrown if any
    of the parameters have not been assigned. The order of the parameters
    will be the same as the corresponding internals returned by
    InternalTable::intraInternals(resnum)

    This will throw an exception if any of the parameters are unassigned.

    \throw SireMol::missing_parameter
*/
template<class Internals, class Param>
SIRE_INLINE_TEMPLATE
QVector<Param> InternalTable<Internals,Param>::intraParameters(ResNum resnum) const
{
    typename Internals::const_iterator it = this->info().intraInternals(resnum);
    return copyParameters( it, this->info().nInterInternals(resnum) );
}

/** Copy all of the parameters for the internals in the groups whose ID numbers
    are in 'groupids' into the GroupedVector that is indexed by GroupIndexID.
    The index is the same as that used by the metadata object returned
    by InternalTable::info() and will be in the same order as the
    GroupedVector of the corresponding internals returned by
    InternalTable::internalsByGroup(groupids)

    This will throw an exception if any of the parameters are unassigned.

    \throw SireMol::missing_parameter
*/
template<class Internals, class Param>
template<class C>
SIRE_INLINE_TEMPLATE
GroupedVector<GroupIndexID,Param>
InternalTable<Internals,Param>::parametersByGroup(const C &groupids) const
{
    if (not groupids.isEmpty())
    {
        GroupedVector<GroupIndexID,Param> allparams;

        allparams.reserve(groupids.count());

        for (typename C::const_iterator it = groupids.begin();
             it != groupids.end();
             ++it)
        {
            GroupID gid = *it;

            if (params.contains(gid))
                allparams.insert( gid, params.parameters(gid) );
        }

        if (not allparams.isEmpty())
            return allparams;
    }

    return GroupedVector<GroupIndexID,Param>();
}

/** Copy all of the parameters in the table into the GroupedVector that is indexed
    by GroupIndexID. The index is the same as that used by the metadata object
    returned by InternalTable::info(), and will be in the same order as the
    GroupedVector of the corresponding internals returned by
    InternalTable::internalsByGroup()

    This will throw an exception if any of the parameters are unassigned.

    \throw SireMol::missing_parameter
*/
template<class Internals, class Param>
SIRE_INLINE_TEMPLATE
GroupedVector<GroupIndexID,Param> InternalTable<Internals,Param>::parametersByGroup() const
{
    return parametersByGroup( this->info().groupIDs() );
}

/** Copy all of the parameters in the table for the internals in the
    residues whose residue numbers are in 'resnums' into the GroupedVector
    that is indexed by ResNumIndexID. The returned GroupedVector will be in
    the same order as the GroupedVector of the corresponding internals returned by
    InternalTable::internalsByResidue(resnums)

    This will throw an exception if any of the parameters are unassigned.

    \throw SireMol::missing_parameter
*/
template<class Internals, class Param>
template<class C>
SIRE_INLINE_TEMPLATE
GroupedVector<ResNumIndexID,Param>
InternalTable<Internals,Param>::parametersByResidue(const C &resnums) const
{
    if (not resnums.isEmpty())
    {
        GroupedVector<ResNumIndexID,Param> allparams;
        allparams.reserve(resnums.count());

        for (typename C::const_iterator it = resnums.begin();
             it != resnums.end();
             ++it)
        {
            QVector<Param> p = this->parameters(*it);

            if (not p.isEmpty())
                allparams.insert( *it, p );
        }

        if (not allparams.isEmpty())
            return allparams;
    }

    return GroupedVector<ResNumIndexID,Param>();
}

/** Copy all of the parameters in the table into the GroupedVector that is indexed
    by ResNumIndexID. The returned GroupedVector will be in the same order as the
    GroupedVector of the corresponding internals returned by
    InternalTable::internalsByResidue()

    This will throw an exception if any of the parameters are unassigned.

    \throw SireMol::missing_parameter
*/
template<class Internals, class Param>
SIRE_INLINE_TEMPLATE
GroupedVector<ResNumIndexID,Param> InternalTable<Internals,Param>::parametersByResidue() const
{
    return parametersByResidue( this->info().info().residueNumbers() );
}

/** Copy all of the inter-residue parameters in the table for the internals in the
    residues whose residue numbers are in 'resnums' into the GroupedVector
    that is indexed by ResNumIndexID. The returned GroupedVector will be in
    the same order as the GroupedVector of the corresponding internals returned by
    InternalTable::interInternalsByResidue(resnums)

    This will throw an exception if any of the parameters are unassigned.

    \throw SireMol::missing_parameter
*/
template<class Internals, class Param>
template<class C>
SIRE_OUTOFLINE_TEMPLATE
GroupedVector<ResNumIndexID,Param>
InternalTable<Internals,Param>::interParametersByResidue(const C &resnums) const
{
    if (not resnums.isEmpty())
    {
        GroupedVector<ResNumIndexID,Param> allparams;
        allparams.reserve(resnums.count());

        for (typename C::const_iterator it = resnums.begin();
             it != resnums.end();
             ++it)
        {
            QVector<Param> p = this->interParameters(*it);

            if (not p.isEmpty())
                allparams.insert( *it, p );
        }

        if (not allparams.isEmpty())
            return allparams;
    }

    return GroupedVector<ResNumIndexID,Param>();
}

/** Copy all of the intra-residue parameters in the table for the internals in the
    residues whose residue numbers are in 'resnums' into the GroupedVector
    that is indexed by ResNumIndexID. The returned GroupedVector will be in
    the same order as the GroupedVector of the corresponding internals returned by
    InternalTable::intraInternalsByResidue(resnums)

    This will throw an exception if any of the parameters are unassigned.

    \throw SireMol::missing_parameter
*/
template<class Internals, class Param>
template<class C>
SIRE_OUTOFLINE_TEMPLATE
GroupedVector<ResNumIndexID,Param>
InternalTable<Internals,Param>::intraParametersByResidue(const C &resnums) const
{
    if (not resnums.isEmpty())
    {
        GroupedVector<ResNumIndexID,Param> allparams;
        allparams.reserve(resnums.count());

        for (typename C::const_iterator it = resnums.begin();
             it != resnums.end();
             ++it)
        {
            QVector<Param> p = this->intraParameters(*it);

            if (not p.isEmpty())
                allparams.insert( *it, p );
        }

        if (not allparams.isEmpty())
            return allparams;
    }

    return GroupedVector<ResNumIndexID,Param>();
}

/** Copy all of the inter-residue parameters in the table into the GroupedVector
    that is indexed by ResNumIndexID. The returned GroupedVector will be in
    the same order as the GroupedVector of the corresponding internals returned by
    InternalTable::interInternalsByResidue()

    This will throw an exception if any of the parameters are unassigned.

    \throw SireMol::missing_parameter
*/
template<class Internals, class Param>
SIRE_INLINE_TEMPLATE
GroupedVector<ResNumIndexID,Param>
InternalTable<Internals,Param>::interParametersByResidue() const
{
    return interParametersByResidue( this->info().info().residueNumbers() );
}

/** Copy all of the intra-residue parameters in the table into the GroupedVector
    that is indexed by ResNumIndexID. The returned GroupedVector will be in
    the same order as the GroupedVector of the corresponding internals returned by
    InternalTable::intraInternalsByResidue()

    This will throw an exception if any of the parameters are unassigned.

    \throw SireMol::missing_parameter
*/
template<class Internals, class Param>
SIRE_INLINE_TEMPLATE
GroupedVector<ResNumIndexID,Param>
InternalTable<Internals,Param>::intraParametersByResidue() const
{
    return intraParametersByResidue( this->info().info().residueNumbers() );
}

const MagicID internaltable_magic = getMagic("SireMM::InternalTable");

#endif // SKIP_TEMPLATE_DEFINITIONS

}

#ifndef SKIP_TEMPLATE_DEFINITIONS

/** Serialise to a binary data stream */
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
QDataStream& operator<<(QDataStream &ds,
                        const SireMM::InternalTable<Internals,Param> &table)
{
    SireStream::writeHeader(ds, SireMM::internaltable_magic, 1)
             << table.params
             << static_cast<const SireMM::InternalTableBase<Internals>&>(table);

    return ds;
}

/** Deserialise from a binary data stream */
template<class Internals, class Param>
SIRE_OUTOFLINE_TEMPLATE
QDataStream& operator>>(QDataStream &ds,
                        SireMM::InternalTable<Internals,Param> &table)
{
    SireStream::VersionID v = SireStream::readHeader(ds, SireMM::internaltable_magic,
                                                     "SireMM::InternalTable");

    if (v == 1)
    {
        ds >> table.params
           >> static_cast<SireMM::InternalTableBase<Internals>&>(table);
    }
    else
        throw SireStream::version_error(v, "1", "SireMM::InternalTable", CODELOC);

    return ds;
}

#endif // SKIP_TEMPLATE_DEFINITIONS

SIRE_END_HEADER

#endif
