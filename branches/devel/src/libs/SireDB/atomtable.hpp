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

#ifndef SIREDB_ATOMTABLE_HPP
#define SIREDB_ATOMTABLE_HPP

#include <QSet>
#include <QVector>

#include "atomtable.h"
#include "parametergroup.hpp"

#include "SireMol/cgatomid.h"
#include "SireMol/cgnumatomid.h"
#include "SireMol/resnumatomid.h"
#include "SireMol/residatomid.h"

#include "SireError/errors.h"
#include "SireDB/errors.h"

#include "SireStream/datastream.h"

SIRE_BEGIN_HEADER

namespace SireDB
{
template<class Param>
class AtomTableT;
}

template<class Param>
QDataStream& operator<<(QDataStream &ds, const SireDB::AtomTableT<Param>&);
template<class Param>
QDataStream& operator>>(QDataStream &ds, SireDB::AtomTableT<Param>&);

namespace SireDB
{

using SireMol::CutGroupID;

/** This template class provides the additional functionality for
    an AtomTable that is used to store parameters of type 'T'.

    @author Christopher Woods
*/
template<class Param>
class SIREDB_EXPORT AtomTableT : public AtomTable
{

friend QDataStream& ::operator<<<>(QDataStream&, const AtomTableT<Param>&);
friend QDataStream& ::operator>><>(QDataStream&, AtomTableT<Param>&);

public:
    typedef Param parameter_type;

    AtomTableT();
    AtomTableT(const MoleculeInfo &molinfo);

    AtomTableT(const AtomTableT<Param> &other);

    ~AtomTableT();

    AtomParameter<Param> at(const CGAtomID &cgatomid) const;
    AtomParameter<Param> at(const CGNumAtomID &cgatomid) const;
    AtomParameter<Param> at(const ResNumAtomID &resatomid) const;
    AtomParameter<Param> at(const ResIDAtomID &resatomid) const;
    AtomParameter<Param> at(const AtomIndex &atom) const;
    AtomParameter<Param> at(AtomID atomid) const;

    AtomParameter<Param> operator[](const CGAtomID &cgatomid) const;
    AtomParameter<Param> operator[](const CGNumAtomID &cgatomid) const;
    AtomParameter<Param> operator[](const ResNumAtomID &resatomid) const;
    AtomParameter<Param> operator[](const ResIDAtomID &resatomid) const;
    AtomParameter<Param> operator[](const AtomIndex &atom) const;
    AtomParameter<Param> operator[](AtomID atomid) const;

    AtomParameter<Param> value(const CGAtomID &cgatomid) const;
    AtomParameter<Param> value(const CGNumAtomID &cgatomid) const;
    AtomParameter<Param> value(const ResNumAtomID &resatomid) const;
    AtomParameter<Param> value(const ResIDAtomID &resatomid) const;
    AtomParameter<Param> value(const AtomIndex &atom) const;
    AtomParameter<Param> value(AtomID atomid) const;

    AtomParameter<Param> value(const CGAtomID &cgatomid, const Param &defaultValue) const;
    AtomParameter<Param> value(const CGNumAtomID &cgatomid, const Param &defaultValue) const;
    AtomParameter<Param> value(const ResNumAtomID &resatomid, const Param &defaultValue) const;
    AtomParameter<Param> value(const ResIDAtomID &resatomid, const Param &defaultValue) const;
    AtomParameter<Param> value(const AtomIndex &atom, const Param &defaultValue) const;
    AtomParameter<Param> value(AtomID atomid, const Param &defaultValue) const;

    Param parameter(const CGAtomID &cgatomid) const;
    Param parameter(const CGNumAtomID &cgatomid) const;
    Param parameter(const ResNumAtomID &resatomid) const;
    Param parameter(const ResIDAtomID &resatomid) const;
    Param parameter(const AtomIndex &atom) const;
    Param parameter(AtomID atomid) const;

    void setParameter(const CGAtomID &cgatomid, const Param &param);
    void setParameter(const CGNumAtomID &cgatomid, const Param &param);
    void setParameter(const ResNumAtomID &resatomid, const Param &param);
    void setParameter(const ResIDAtomID &resatomid, const Param &param);
    void setParameter(const AtomIndex &atom, const Param &param);
    void setParameter(AtomID atomid, const Param &param);

    void add(const TableBase &table);

    ParameterGroup<Param> parameterGroup(CutGroupID cgid) const;

    QVector< ParameterGroup<Param> > parameterGroups() const;

    QHash< CutGroupID,ParameterGroup<Param> >
        parameterGroups( const QSet<CutGroupID> &cgids ) const;
    QHash< CutGroupNum,ParameterGroup<Param> >
        parameterGroups( const QSet<CutGroupNum> &cgnums) const;

    QVector<Param> parameters() const;
    QVector<Param> parameters(ResNum resnum) const;
    QVector<Param> parameters(ResID resid) const;
    QVector<Param> parameters(CutGroupID cgid) const;
    QVector<Param> parameters(CutGroupNum cgnum) const;

    QHash< CutGroupID,QVector<Param> >
          parameters(const QSet<CutGroupID> &cgids) const;
    QHash< CutGroupNum,QVector<Param> >
          parameters(const QSet<CutGroupNum> &cgnums) const;

    QHash< ResNum,QVector<Param> >
          parameters(const QSet<ResNum> &resnums) const;
    QHash< ResID,QVector<Param> >
          parameters(const QSet<ResID> &resids) const;

    QVector< AtomParameter<Param> > atomParameters() const;
    QVector< AtomParameter<Param> > atomParameters(ResNum resnum) const;
    QVector< AtomParameter<Param> > atomParameters(ResID resid) const;
    QVector< AtomParameter<Param> > atomParameters(CutGroupID cgid) const;
    QVector< AtomParameter<Param> > atomParameters(CutGroupNum cgnum) const;

    QHash< CutGroupID,QVector< AtomParameter<Param> > >
          atomParameters(const QSet<CutGroupID> &cgids) const;
    QHash< CutGroupNum,QVector< AtomParameter<Param> > >
          atomParameters(const QSet<CutGroupNum> &cgnums) const;

    QHash< ResNum,QVector< AtomParameter<Param> > >
          atomParameters(const QSet<ResNum> &resnums) const;
    QHash< ResID,QVector< AtomParameter<Param> > >
          atomParameters(const QSet<ResID> &resids) const;

protected:
    int _unsafe_nAssignedParameters(const ResidueInfo &resinfo) const;
    int _unsafe_nAssignedParameters(CutGroupID cgid) const;

    int _unsafe_nMissingParameters(const ResidueInfo &resinfo) const;
    int _unsafe_nMissingParameters(CutGroupID cgid) const;

    bool _unsafe_assignedParameter(const CGAtomID &cgatomid) const;

    bool _unsafe_hasMissingParameters(const ResidueInfo &resinfo) const;
    bool _unsafe_hasMissingParameters(CutGroupID cgid) const;

    QSet<AtomIndex> _unsafe_missingParameters(const ResidueInfo &resinfo) const;
    QSet<AtomIndex> _unsafe_missingParameters(CutGroupID cgid) const;

    Param _unsafe_parameter(const CGAtomID &cgatomid) const;
    AtomParameter<Param> _unsafe_atomParameter(const CGAtomID &cgatomid) const;

    AtomParameter<Param> _unsafe_atomParameter(const CGAtomID &cgatomid,
                                               const Param &defaultValue) const;

    QVector<Param> _unsafe_parameters(const ResidueInfo &resinfo) const;
    QVector<Param> _unsafe_parameters(CutGroupID cgid) const;

    QVector< AtomParameter<Param> > _unsafe_atomParameters(
                                          const ResidueInfo &resinfo) const;
    QVector< AtomParameter<Param> > _unsafe_atomParameters(
                                          CutGroupID cgid) const;

    ParameterGroup<Param> _unsafe_parameterGroup(CutGroupID cgid) const;

    void _unsafe_clear(const ResidueInfo &resinfo);
    void _unsafe_clear(CutGroupID cgid);
    void _unsafe_clear(const CGAtomID &cgatomid);

    void _unsafe_setParameter(const CGAtomID &cgatomid, const Param &param);

    void clearCache(CutGroupID cgid);
    void clearCache(ResNum resnum);

private:
    template<class C>
    QHash< C, QVector<Param> > _pvt_parameters(const QSet<C> &idxs) const;

    template<class C>
    QHash< C, QVector< AtomParameter<Param> > >
                _pvt_atomParameters(const QSet<C> &idxs) const;

    /** Array of all of the parameter hashes. The first index in
        the CutGroupID, the second index is the AtomID (thus this
        pair of arrays is indexed via CGAtomID) */
    QVector< QHash<AtomID,Param> > params;

    /** Cache of the completed parameter groups - these become populated
        as the table is populated. */
    QHash< CutGroupID, QVector<Param> > groupcache;

    /** Cache of the completed parameters with respect to residue */
    QHash< ResNum, QVector<Param> > rescache;
};

#ifndef SKIP_TEMPLATE_DEFINITIONS

/** Construct an empty AtomTableT */
template<class Param>
AtomTableT<Param>::AtomTableT() : AtomTable()
{}

/** Construct an empty AtomTableT that will hold the parameters for
    the atoms in 'molinfo' */
template<class Param>
AtomTableT<Param>::AtomTableT(const MoleculeInfo &molinfo)
                  : AtomTable(molinfo)
{
    //make space for the parameters
    uint ncg = this->info().nCutGroups();

    params.resize(ncg);
    groupcache.reserve(ncg);
    rescache.reserve(this->info().nResidues());

    for (CutGroupID i(0); i<ncg; ++i)
    {
        params[i].reserve( this->info().nAtoms(i) );
    }
}

/** Copy constructor - this is fast as the data for this object
    is implicitly shared */
template<class Param>
AtomTableT<Param>::AtomTableT(const AtomTableT<Param> &other)
                  : AtomTable(other),
                    params(other.params),
                    groupcache(other.groupcache),
                    rescache(other.rescache)
{}

/** Destructor */
template<class Param>
AtomTableT<Param>::~AtomTableT()
{}

/** Clear the cache of any extracted parameter groups associated with
    the CutGroup with ID == cgid */
template<class Param>
void AtomTableT<Param>::clearCache(CutGroupID cgid)
{
    groupcache.remove(cgid);
}

/** Clear the cache of any extracted parameter groups associated with
    the residue with number 'resnum' */
template<class Param>
void AtomTableT<Param>::clearCache(ResNum resnum)
{
    rescache.remove(resnum);
}

/** Internal function used to return whether or not the parameter for the
    atom with index 'cgatomid' has been assigned. This does not check whether
    or not this index is valid. */
template<class Param>
bool AtomTableT<Param>::_unsafe_assignedParameter(const CGAtomID &cgatomid) const
{
    return params.constData()[cgatomid.cutGroupID()]   // QHash<AtomID,Param>
                 .contains(cgatomid.atomID());
}

/** Internal function used to return the number of assigned parameters for
    the residue described by 'resinfo'. This does not check to see if
    this is a valid residue. */
template<class Param>
int AtomTableT<Param>::_unsafe_nAssignedParameters(const ResidueInfo &resinfo) const
{
    uint nats = resinfo.nAtoms();

    int nassigned = 0;

    for (AtomID i(0); i<nats; ++i)
    {
        if ( this->_unsafe_assignedParameter(resinfo[i]) )
            ++nassigned;
    }

    return nassigned;
}

/** Internal function used to return the total number of assigned parameters in
    the CutGroup with ID 'cgid'. This does not check whether or not this is
    a valid CutGroup ID number */
template<class Param>
int AtomTableT<Param>::_unsafe_nAssignedParameters(CutGroupID cgid) const
{
    return params.constData()[cgid].count();
}

/** Return the number of missing parameters for the residue described
    by 'resinfo' - this does not check to see whether this residue is valid. */
template<class Param>
int AtomTableT<Param>::_unsafe_nMissingParameters(const ResidueInfo &resinfo) const
{
    uint nats = resinfo.nAtoms();

    int nmissing = 0;

    for (AtomID i(0); i<nats; ++i)
    {
        if ( not this->_unsafe_assignedParameter(resinfo[i]) )
            ++nmissing;
    }

    return nmissing;
}

/** Return the number of missing parameters for the CutGroup with ID 'cgid'.
    This does not check whether or not this is a valid CutGroupID */
template<class Param>
int AtomTableT<Param>::_unsafe_nMissingParameters(CutGroupID cgid) const
{
    return info().nAtoms(cgid) - this->_unsafe_nAssignedParameters(cgid);
}

/** Return whether or not the residue represented by 'resinfo' has any missing
    parameters. This does not check whether or not this is a valid residue */
template<class Param>
bool AtomTableT<Param>::_unsafe_hasMissingParameters(const ResidueInfo &resinfo) const
{
    uint nats = resinfo.nAtoms();

    for (AtomID i(0); i<nats; ++i)
    {
        if ( not this->_unsafe_assignedParameter(resinfo[i]) )
            return true;
    }

    return false;
}

/** Return whether or not the CutGroup with ID == cgid has any missing parameters.
    This does not check whether or not this is a valid CutGroupID. */
template<class Param>
bool AtomTableT<Param>::_unsafe_hasMissingParameters(CutGroupID cgid) const
{
    return this->_unsafe_nMissingParameters(cgid) > 0;
}

/** Return the AtomIndexes of the atoms in the residue represented by 'resinfo'
    that are missing parameters. This does not check whether this is a valid residue */
template<class Param>
QSet<AtomIndex> AtomTableT<Param>::_unsafe_missingParameters(
                                                  const ResidueInfo &resinfo) const
{
    QSet<AtomIndex> missing;

    uint nats = resinfo.nAtoms();

    for (AtomID i(0); i<nats; ++i)
    {
        if ( not this->_unsafe_assignedParameter(resinfo[i]) )
            missing.insert( resinfo.atom(i) );
    }

    return missing;
}

/** Return the AtomIndexes of the atoms in the CutGroup with ID == cgid
    that are missing parameters. This does not check whether this is a valid
    CutGroupID */
template<class Param>
QSet<AtomIndex> AtomTableT<Param>::_unsafe_missingParameters(CutGroupID cgid) const
{
    uint nats = info().nAtoms(cgid);

    QSet<AtomIndex> missing;

    for (AtomID i(0); i<nats; ++i)
    {
        CGAtomID cgatomid(cgid, i);

        if (not this->_unsafe_assignedParameter(cgatomid))
            missing.insert( info().atom(cgatomid) );
    }

    return missing;
}

/** Internal function used to return the parameter for the atom at
    index 'cgatomid' - this does not check that this is a valid index,
    nor does it check whether or not the parameter actually exists! */
template<class Param>
Param AtomTableT<Param>::_unsafe_parameter(const CGAtomID &cgatomid) const
{
    return params.constData()[cgatomid.cutGroupID()]   //QHash<AtomID,Param>
                 .value(cgatomid.atomID());
}

/** Internal function used to return the AtomParameter for the atom at
    index 'cgatomid' - this does not check that this is a valid index,
    nor does it check whether or not the parameter actually exists! */
template<class Param>
AtomParameter<Param> AtomTableT<Param>::_unsafe_atomParameter(
                                              const CGAtomID &cgatomid) const
{
    return AtomParameter<Param>( info().atom(cgatomid),
                                 this->_unsafe_parameter(cgatomid) );
}

/** Internal function used to return the AtomParameter for the atom at
    index 'cgatomid' if it has been assigned, or 'defaultValue' if
    it has not. */
template<class Param>
AtomParameter<Param> AtomTableT<Param>::_unsafe_atomParameter(
                                              const CGAtomID &cgatomid,
                                              const Param &defaultValue) const
{
    const QHash<AtomID,Param> &cgparams = params.constData()[cgatomid.cutGroupID()];

    return AtomParameter<Param>( info().atom(cgatomid),
                                 cgparams.value(cgatomid.atomID(), defaultValue) );
}

/** Return an array of all of the parameters in the residue described by
    'resinfo'. This does not check if this residue is valid, nor if the
    parameters actually exist! The array is in AtomID order */
template<class Param>
QVector<Param> AtomTableT<Param>::_unsafe_parameters(
                                              const ResidueInfo &resinfo) const
{
    int nats = resinfo.nAtoms();

    QVector<Param> resparams;
    resparams.reserve(nats);

    for (AtomID i(0); i<nats; ++i)
        resparams.append( this->_unsafe_parameter(resinfo[i]) );

    return resparams;
}

/** Return an array of all of the parameters of the atoms in the CutGroup
    with ID == cgid. This does not check whether this is a valid CutGroupID,
    nor whether the parameters actually exist. The array is in AtomID order. */
template<class Param>
QVector<Param> AtomTableT<Param>::_unsafe_parameters(CutGroupID cgid) const
{
    int nats = info().nAtoms(cgid);

    const QHash<AtomID,Param> &paramhash = params.constData()[cgid];

    QVector<Param> cgparams;
    cgparams.reserve(nats);

    for (AtomID i(0); i<nats; ++i)
        cgparams.append( paramhash.value(i) );

    return cgparams;
}

/** Return an array of the AtomParameters of the atoms in the residue
    described by 'resinfo' - this does not check whether this residue is
    valid, nor does it check whether the parameters actually exist. The
    array is in AtomID order. */
template<class Param>
QVector< AtomParameter<Param> >
AtomTableT<Param>::_unsafe_atomParameters(const ResidueInfo &resinfo) const
{
    int nats = resinfo.nAtoms();

    QVector< AtomParameter<Param> > resparams;
    resparams.reserve(nats);

    for (AtomID i(0); i<nats; ++i)
        resparams.append( this->_unsafe_atomParameter(resinfo[i]) );

    return resparams;
}

/** Return an array of the Atom parameters for the atoms in the CutGroup
    with ID == cgid. This does not check whether this is a valid CutGroupID,
    nor does it check whether the parameters actually exist. The array
    is in AtomID order. */
template<class Param>
QVector< AtomParameter<Param> >
AtomTableT<Param>::_unsafe_atomParameters(CutGroupID cgid) const
{
    const AtomInfoGroup infogroup = info().atomGroup(cgid);

    const QHash<AtomID,Param> &paramhash = params.constData()[cgid];

    int nats = infogroup.nAtoms();

    QVector< AtomParameter<Param> > cgparams;
    cgparams.reserve(nats);

    for (AtomID i(0); i<nats; ++i)
        cgparams.append( AtomParameter<Param>(infogroup[i], paramhash[i]) );

    return cgparams;
}

/** Return the parameter group for the CutGroup with ID == cgid. This
    does not check whether or not this is a valid CutGroupID, nor does
    it check whether the parameters actually exist. */
template<class Param>
ParameterGroup<Param> AtomTableT<Param>::_unsafe_parameterGroup(CutGroupID cgid) const
{
    return ParameterGroup<Param>( this->info().atomGroup(cgid),
                                  this->_unsafe_parameters(cgid) );
}

/** Clear the parameter for the atom at index 'cgatomid'. This does
    not check whether this is a valid index. */
template<class Param>
void AtomTableT<Param>::_unsafe_clear(const CGAtomID &cgatomid)
{
    //check that the parameter is assigned to prevent making an
    //unnecessary copy if the parameter is not assigned.
    if ( this->_unsafe_assignedParameter(cgatomid) )
        params[cgatomid.cutGroupID()].remove(cgatomid.atomID());
}

/** Clear all parameters for the residue described by 'resinfo'. This does
    not check whether this is a valid residue. */
template<class Param>
void AtomTableT<Param>::_unsafe_clear(const ResidueInfo &resinfo)
{
    uint nats = resinfo.nAtoms();

    for (AtomID i(0); i<nats; ++i)
        this->_unsafe_clear( resinfo[i] );
}

/** Clear all of the parameters from the CutGroup with ID == cgid.
    This does not check whether this is valid CutGroupID */
template<class Param>
void AtomTableT<Param>::_unsafe_clear(CutGroupID cgid)
{
    if (not params.at(cgid).isEmpty())
        params[cgid].clear();
}

template<class Param>
void AtomTableT<Param>::_unsafe_setParameter(const CGAtomID &cgatomid,
                                             const Param &param)
{
    params.data()[cgatomid.cutGroupID()].insert(cgatomid.atomID(),param);

    //clear the cache of groups associated with this atom
    this->clearCache(cgatomid.cutGroupID());
    this->clearCache(this->info().atom(cgatomid).resNum());
}

/** Return the AtomParameter for the atom at index 'cgatomid'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
    \throw SireDB::missing_parameter
*/
template<class Param>
AtomParameter<Param> AtomTableT<Param>::at(const CGAtomID &cgatomid) const
{
    return this->_unsafe_atomParameter( this->assertHaveParameter(cgatomid) );
}

/** Return the parameter for the atom at index 'cgatomid'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
    \throw SireDB::missing_parameter
*/
template<class Param>
AtomParameter<Param> AtomTableT<Param>::at(const CGNumAtomID &cgatomid) const
{
    return this->_unsafe_atomParameter( this->assertHaveParameter(cgatomid) );
}

/** Return the parameter for the atom at index 'resatomid'

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
    \throw SireDB::missing_parameter
*/
template<class Param>
AtomParameter<Param> AtomTableT<Param>::at(const ResNumAtomID &resatomid) const
{
    return this->_unsafe_atomParameter( this->assertHaveParameter(resatomid) );
}

/** Return the parameter for the atom at index 'resatomid'

    \throw SireError::invalid_index
    \throw SireDB::missing_parameter
*/
template<class Param>
AtomParameter<Param> AtomTableT<Param>::at(const ResIDAtomID &resatomid) const
{
    return this->_unsafe_atomParameter( this->assertHaveParameter(resatomid) );
}

/** Return the parameter for the atom with AtomIndex 'atom'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
    \throw SireDB::missing_parameter
*/
template<class Param>
AtomParameter<Param> AtomTableT<Param>::at(const AtomIndex &atom) const
{
    return this->_unsafe_atomParameter( this->assertHaveParameter(atom) );
}

/** Return the parameter for the atom at index 'atomid'

    \throw SireError::invalid_index
    \throw SireDB::missing_parameter
*/
template<class Param>
AtomParameter<Param> AtomTableT<Param>::at(AtomID atomid) const
{
    return this->_unsafe_atomParameter( this->assertHaveParameter(atomid) );
}

/** Return the AtomParameter for the atom at index 'cgatomid'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
    \throw SireDB::missing_parameter
*/
template<class Param>
AtomParameter<Param> AtomTableT<Param>::operator[](const CGAtomID &cgatomid) const
{
    return this->at(cgatomid);
}

/** Return the parameter for the atom at index 'cgatomid'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
    \throw SireDB::missing_parameter
*/
template<class Param>
AtomParameter<Param> AtomTableT<Param>::operator[](const CGNumAtomID &cgatomid) const
{
    return this->at(cgatomid);
}

/** Return the parameter for the atom at index 'resatomid'

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
    \throw SireDB::missing_parameter
*/
template<class Param>
AtomParameter<Param> AtomTableT<Param>::operator[](const ResNumAtomID &resatomid) const
{
    return this->at(resatomid);
}

/** Return the parameter for the atom at index 'resatomid'

    \throw SireError::invalid_index
    \throw SireDB::missing_parameter
*/
template<class Param>
AtomParameter<Param> AtomTableT<Param>::operator[](const ResIDAtomID &resatomid) const
{
    return this->at(resatomid);
}

/** Return the parameter for the atom with AtomIndex 'atom'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
    \throw SireDB::missing_parameter
*/
template<class Param>
AtomParameter<Param> AtomTableT<Param>::operator[](const AtomIndex &atom) const
{
    return this->at(atom);
}

/** Return the parameter for the atom at index 'atomid'

    \throw SireError::invalid_index
    \throw SireDB::missing_parameter
*/
template<class Param>
AtomParameter<Param> AtomTableT<Param>::operator[](AtomID atomid) const
{
    return this->at(atomid);
}

/** Return the value of the parameter of the atom at index 'cgatomid'.
    This will return the default constructed value if it has not yet been
    assigned.

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
template<class Param>
AtomParameter<Param> AtomTableT<Param>::value(const CGAtomID &cgatomid) const
{
    return this->_unsafe_atomParameter(cgatomid);
}

/** Return the value of the parameter of the atom at index 'cgatomid'.
    This will return the default constructed value if it has not yet been
    assigned.

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
template<class Param>
AtomParameter<Param> AtomTableT<Param>::value(const CGNumAtomID &cgatomid) const
{
    return this->_unsafe_atomParameter( info().at(cgatomid) );
}

/** Return the value of the parameter of the atom at index 'resatomid'.
    This will return the default constructed value if it has not yet been
    assigned.

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
template<class Param>
AtomParameter<Param> AtomTableT<Param>::value(const ResNumAtomID &resatomid) const
{
    return this->_unsafe_atomParameter( info().at(resatomid) );
}

/** Return the value of the parameter of the atom at index 'resatomid'.
    This will return the default constructed value if it has not yet been
    assigned.

    \throw SireError::invalid_index
*/
template<class Param>
AtomParameter<Param> AtomTableT<Param>::value(const ResIDAtomID &resatomid) const
{
    return this->_unsafe_atomParameter( info().at(resatomid) );
}

/** Return the value of the parameter of the atom with AtomIndex 'atom'.
    This will return the default constructed value if it has not yet been
    assigned.

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
template<class Param>
AtomParameter<Param> AtomTableT<Param>::value(const AtomIndex &atom) const
{
    return this->_unsafe_atomParameter( info().at(atom) );
}

/** Return the value of the parameter of the atom at index 'atomid'.
    This will return the default constructed value if it has not yet been
    assigned.

    \throw SireError::invalid_index
*/
template<class Param>
AtomParameter<Param> AtomTableT<Param>::value(AtomID atomid) const
{
    return this->_unsafe_atomParameter( info().at(atomid) );
}

/** Return the value of the parameter of the atom at index 'cgatomid'.
    This will return the defaultValue if it has not yet been
    assigned.

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
template<class Param>
AtomParameter<Param> AtomTableT<Param>::value(const CGAtomID &cgatomid,
                                              const Param &defaultValue) const
{
    return this->_unsafe_atomParameter(cgatomid, defaultValue);
}

/** Return the value of the parameter of the atom at index 'cgatomid'.
    This will return the defaultValue if it has not yet been
    assigned.

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
template<class Param>
AtomParameter<Param> AtomTableT<Param>::value(const CGNumAtomID &cgatomid,
                                              const Param &defaultValue) const
{
    return this->_unsafe_atomParameter( info().at(cgatomid),
                                    defaultValue );
}

/** Return the value of the parameter of the atom at index 'resatomid'.
    This will return the defaultValue if it has not yet been
    assigned.

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
template<class Param>
AtomParameter<Param> AtomTableT<Param>::value(const ResNumAtomID &resatomid,
                                              const Param &defaultValue) const
{
    return this->_unsafe_atomParameter( info().at(resatomid),
                                    defaultValue );
}

/** Return the value of the parameter of the atom at index 'resatomid'.
    This will return the defaultValue if it has not yet been
    assigned.

    \throw SireError::invalid_index
*/
template<class Param>
AtomParameter<Param> AtomTableT<Param>::value(const ResIDAtomID &resatomid,
                                              const Param &defaultValue) const
{
    return this->_unsafe_atomParameter( info().at(resatomid),
                                    defaultValue );
}

/** Return the value of the parameter of the atom with AtomIndex 'atom'.
    This will return the defaultValue if it has not yet been
    assigned.

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
template<class Param>
AtomParameter<Param> AtomTableT<Param>::value(const AtomIndex &atom,
                                              const Param &defaultValue) const
{
    return this->_unsafe_atomParameter( info().at(atom),
                                    defaultValue );
}

/** Return the value of the parameter of the atom at index 'atomid'.
    This will return the defaultValue if it has not yet been
    assigned.

    \throw SireError::invalid_index
*/
template<class Param>
AtomParameter<Param> AtomTableT<Param>::value(AtomID atomid,
                                              const Param &defaultValue) const
{
    return this->_unsafe_atomParameter( info().at(atomid),
                                    defaultValue );
}

/** Return the parameter of the atom with index 'cgatomid'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
    \throw SireDB::missing_parameter
*/
template<class Param>
Param AtomTableT<Param>::parameter(const CGAtomID &cgatomid) const
{
    return this->_unsafe_parameter( this->assertHaveParameter(cgatomid) );
}

/** Return the parameter at index 'cgatomid'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
    \throw SireDB::missing_parameter
*/
template<class Param>
Param AtomTableT<Param>::parameter(const CGNumAtomID &cgatomid) const
{
    return this->_unsafe_parameter( this->assertHaveParameter(cgatomid) );
}

/** Return the parameter of the atom with index 'resatomid'

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
    \throw SireDB::missing_parameter
*/
template<class Param>
Param AtomTableT<Param>::parameter(const ResNumAtomID &resatomid) const
{
    return this->_unsafe_parameter( this->assertHaveParameter(resatomid) );
}

/** Return the parameter of the atom with index 'resatomid'

    \throw SireError::invalid_index
    \throw SireDB::missing_parameter
*/
template<class Param>
Param AtomTableT<Param>::parameter(const ResIDAtomID &resatomid) const
{
    return this->_unsafe_parameter( this->assertHaveParameter(resatomid) );
}

/** Return the parameter of the atom with AtomIndex 'atom'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
    \throw SireDB::missing_parameter
*/
template<class Param>
Param AtomTableT<Param>::parameter(const AtomIndex &atom) const
{
    return this->_unsafe_parameter( this->assertHaveParameter(atom) );
}

/** Return the parameter of the atom with index 'atomid'

    \throw SireError::invalid_index
    \throw SireDB::missing_parameter
*/
template<class Param>
Param AtomTableT<Param>::parameter(AtomID atomid) const
{
    return this->_unsafe_parameter( this->assertHaveParameter(atomid) );
}

/** Set the parameter for the atom with index 'cgatomid' to 'param'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
template<class Param>
void AtomTableT<Param>::setParameter(const CGAtomID &cgatomid,
                                     const Param &param)
{
    this->_unsafe_setParameter(cgatomid, param);
}

/** Set the parameter for the atom with index 'cgatomid' to 'param'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
template<class Param>
void AtomTableT<Param>::setParameter(const CGNumAtomID &cgatomid,
                                     const Param &param)
{
    this->_unsafe_setParameter( info().at(cgatomid), param );
}

/** Set the parameter for the atom with index 'resatomid' to 'param'

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
template<class Param>
void AtomTableT<Param>::setParameter(const ResNumAtomID &resatomid,
                                     const Param &param)
{
    this->_unsafe_setParameter( info().at(resatomid), param );
}

/** Set the parameter for the atom with index 'resatomid' to 'param'

    \throw SireError::invalid_index
*/
template<class Param>
void AtomTableT<Param>::setParameter(const ResIDAtomID &resatomid,
                                     const Param &param)
{
    this->_unsafe_setParameter( info().at(resatomid), param );
}

/** Set the parameter of the atom with AtomIndex 'atom' to 'param'

    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
template<class Param>
void AtomTableT<Param>::setParameter(const AtomIndex &atom,
                                     const Param &param)
{
    this->_unsafe_setParameter( info().at(atom), param );
}

/** Set the parameter of the atom at index 'atomid' to 'param'

    \throw SireError::invalid_index
*/
template<class Param>
void AtomTableT<Param>::setParameter(AtomID atomid,
                                     const Param &param)
{
    this->_unsafe_setParameter( info().at(atomid), param );
}

/** Add the contents of 'table' into this table. Note that the
    info() objects for both tables must be identical, and
    'table' must be of type AtomTableT<Param>.

    \throw SireError::incompatible_error
    \throw SireError::invalid_cast
*/
template<class Param>
void AtomTableT<Param>::add(const TableBase &table)
{
    //is the table of the right type?
    if (not table.isA< AtomTableT<Param> >())
        throw SireError::invalid_cast( QObject::tr(
                  "Cannot cast from a table of type \"%1\" to a table of "
                  "type \"%2\".")
                      .arg(table.what()).arg(this->what()), CODELOC );

    const AtomTableT<Param> &table_t = table.asA< AtomTableT<Param> >();

    //is this the same object?
    if (&table_t == this)
        return;

    //are the info() objects identical?
    if (info() != table_t.info())
        throw SireError::incompatible_error( QObject::tr(
                "Cannot combine the table of type \"%1\", for the molecule "
                "\"%2\", with the table of type \"%3\", for the molecule "
                "\"%4\", as the info() objects are not identical.")
                    .arg(this->what()).arg(info().name())
                    .arg(table_t.what()).arg(table_t.info().name()), CODELOC );


    //run through each of the parameters in 'table' and add them to
    //this table if they have been set
    uint ncg = this->info().nCutGroups();

    for (CutGroupID cgid(0); cgid<ncg; ++cgid)
    {
        uint nats = this->info().nAtoms(cgid);

        for (AtomID i(0); i<nats; ++i)
        {
            CGAtomID cgatomid(cgid,i);

            if (table_t.assignedParameter(cgatomid))
                this->setParameter(cgatomid, table_t.parameter(cgatomid));
        }
    }
}

/** Return the parameter group for the CutGroup with ID == cgid

    \throw SireMol::missing_cutgroup
    \throw SireDB::missing_parameter
*/
template<class Param>
ParameterGroup<Param> AtomTableT<Param>::parameterGroup(CutGroupID cgid) const
{
    const AtomInfoGroup &atomgroup = info().at(cgid);

    //is there a cached copy?
    if (groupcache.contains(cgid))
        return ParameterGroup<Param>( atomgroup, groupcache[cgid] );
    else
    {
        //we need to buid the group
        return ParameterGroup<Param>( atomgroup, this->parameters() );
    }
}

/** Return an array of all of the parameter groups in this molecule.

    \throw SireDB::missing_parameter
*/
template<class Param>
QVector< ParameterGroup<Param> > AtomTableT<Param>::parameterGroups() const
{
    uint ncg = info().nCutGroups();

    QVector< ParameterGroup<Param> > groups;
    groups.reserve(ncg);

    for (CutGroupID cgid(0); cgid < ncg; ++cgid)
        groups.append( this->parameterGroup(cgid) );

    return groups;
}

/** Return the parameter groups for the CutGroups whose IDs are in 'cgids'

    \throw SireMol::missing_cutgroup
    \throw SireDB::missing_parameter
*/
template<class Param>
QHash< CutGroupID,ParameterGroup<Param> >
AtomTableT<Param>::parameterGroups( const QSet<CutGroupID> &cgids ) const
{
    QHash< CutGroupID,ParameterGroup<Param> > groups;
    groups.reserve(cgids.count());

    for (QSet<CutGroupID>::const_iterator it = cgids.begin();
         it != cgids.end();
         ++it)
    {
        groups.insert(*it, this->parameterGroup(*it));
    }

    return groups;
}

/** Return the parameter groups for the CutGroups whose numbers are in 'cgnums'

    \throw SireMol::missing_cutgroup
    \throw SireDB::missing_parameter
*/
template<class Param>
QHash< CutGroupNum,ParameterGroup<Param> >
AtomTableT<Param>::parameterGroups( const QSet<CutGroupNum> &cgnums) const
{
    QHash< CutGroupNum,ParameterGroup<Param> > groups;
    groups.reserve(cgnums.count());

    for (QSet<CutGroupNum>::const_iterator it = cgnums.begin();
         it != cgnums.end();
         ++it)
    {
        groups.insert( *it, this->parameterGroup(info().cutGroupID(*it)) );
    }

    return groups;
}

/** Return an array of all of the parameters in this table, in AtomID order

    \throw SireDB::missing_parameter
*/
template<class Param>
QVector<Param> AtomTableT<Param>::parameters() const
{
    QVector<Param> allparams;

    uint nats = info().nAtoms();

    allparams.reserve(nats);

    for (AtomID i(0); i<nats; ++i)
        allparams.append( this->parameter(i) );

    return allparams;
}

/** Return an array of all of the parameter for the residue with number 'resnum',
    in AtomID order

    \throw SireMol::missing_residue
    \throw SireDB::missing_parameter
*/
template<class Param>
QVector<Param> AtomTableT<Param>::parameters(ResNum resnum) const
{
    const ResidueInfo &resinfo = info().at(resnum);

    if (rescache.contains(resnum))
        return rescache[resnum];
    else
    {
        QVector<Param> resparams;

        uint nats = resinfo.nAtoms();

        resparams.reserve(nats);

        for (AtomID i(0); i<nats; ++i)
        {
            resparams.append( this->parameter(resinfo[i]) );
        }

        //cache this result - breaks constness!
        const_cast<AtomTableT<Param>&>(*this).rescache.insert(resnum, resparams);

        return resparams;
    }
}

/** Return an array of the parameters for the residue at index
    'resid', in AtomID order

    \throw SireError::invalid_index
    \throw SireDB::missing_parameter
*/
template<class Param>
QVector<Param> AtomTableT<Param>::parameters(ResID resid) const
{
    return this->parameters( info().residueNumber(resid) );
}

/** Return an array of the parameters for the CutGroup with
    ID == 'cgid', in AtomID order.

    \throw SireMol::missing_cutgroup
    \throw SireDB::missing_parameter
*/
template<class Param>
QVector<Param> AtomTableT<Param>::parameters(CutGroupID cgid) const
{
    if (groupcache.contains(cgid))
        return groupcache[cgid];
    else
    {
        uint nats = info().nAtoms(cgid);

        QVector<Param> cgparams;
        cgparams.reserve(nats);

        for (AtomID i(0); i<nats; ++i)
            cgparams.append( this->parameter(CGAtomID(cgid,i)) );

        //cache this result (breaks constness of function!)
        const_cast<AtomTableT<Param>&>(*this).groupcache.insert( cgid, cgparams );

        return cgparams;
    }
}

/** Return an array of the parameters for the atoms in the CutGroup
    with number 'cgnum', in AtomID order.

    \throw SireMol::missing_cutgroup
    \throw SireDB::missing_parameter
*/
template<class Param>
QVector<Param> AtomTableT<Param>::parameters(CutGroupNum cgnum) const
{
    return this->parameters( info().cutGroupID(cgnum) );
}

/** Internal function used to return sets of parameters */
template<class Param>
template<class C>
QHash< C,QVector<Param> >
AtomTableT<Param>::_pvt_parameters( const QSet<C> &idxs ) const
{
    QHash< C,QVector<Param> > allparams;
    allparams.reserve(idxs.count());

    foreach ( C idx, idxs )
    {
        allparams.insert( idx, this->parameters(idx) );
    }

    return allparams;
}

/** Return the parameters for the atoms in the CutGroups whose IDs
    are in 'cgids'

    \throw SireMol::missing_cutgroup
    \throw SireDB::missing_parameter
*/
template<class Param>
QHash< CutGroupID,QVector<Param> >
AtomTableT<Param>::parameters(const QSet<CutGroupID> &cgids) const
{
    return this->_pvt_parameters<CutGroupID>(cgids);
}

/** Return the parameters for the atoms in the CutGroups whose numbers
    are in 'cgnums'

    \throw SireMol::missing_cutgroup
    \throw SireDB::missing_parameter
*/
template<class Param>
QHash< CutGroupNum,QVector<Param> >
AtomTableT<Param>::parameters(const QSet<CutGroupNum> &cgnums) const
{
    return this->_pvt_parameters<CutGroupNum>(cgnums);
}

/** Return arrays of the parameters of atoms that are in the residues
    whose numbers are in 'resnums'

    \throw SireMol::missing_residue
    \throw SireDB::missing_parameter
*/
template<class Param>
QHash< ResNum,QVector<Param> >
AtomTableT<Param>::parameters(const QSet<ResNum> &resnums) const
{
    return this->_pvt_parameters<ResNum>(resnums);
}

/** Return arrays of the parameters of atoms that are in the residues
    whose indicies are in 'resids'

    \throw SireError::invalid_index
    \throw SireDB::missing_parameter
*/
template<class Param>
QHash< ResID,QVector<Param> >
AtomTableT<Param>::parameters(const QSet<ResID> &resids) const
{
    return this->_pvt_parameters<ResID>(resids);
}

/** Return an array of all of the AtomParameters in this table, in
    AtomID order.

    \throw SireDB::missing_parameter
*/
template<class Param>
QVector< AtomParameter<Param> > AtomTableT<Param>::atomParameters() const
{
    QVector< AtomParameter<Param> > allparams;

    uint nats = info().nAtoms();

    allparams.reserve(nats);

    for (AtomID i(0); i<nats; ++i)
    {
        allparams.append( this->at(i) );
    }

    return allparams;
}

/** Return an array of all of the AtomParameters in this table for the
    residue with number 'resnum', in AtomID order

    \throw SireMol::missing_residue
    \throw SireDB::missing_parameter
*/
template<class Param>
QVector< AtomParameter<Param> >
AtomTableT<Param>::atomParameters(ResNum resnum) const
{
    const ResidueInfo &resinfo = info().at(resnum);

    uint nats = resinfo.nAtoms();

    //get the array of parameters (which could be cached)
    QVector<Param> resparams = this->parameters(resnum);
    const Param *paramarray = resparams.constData();

    //this should always be true!
    BOOST_ASSERT(resparams.count() == nats);

    //now go through and add the AtomInfo to each parameter
    QVector< AtomParameter<Param> > atomparams;
    atomparams.reserve(nats);

    for (AtomID i(0); i<nats; ++i)
    {
        atomparams.append( AtomParameter<Param>( resinfo.atom(i),
                                                 paramarray[i] ) );
    }

    return atomparams;
}

/** Return an array of the AtomParameters for the residue at index 'resid',
    returning the parameters in AtomID order

    \throw SireError::invalid_index
    \throw SireDB::missing_parameter
*/
template<class Param>
QVector< AtomParameter<Param> >
AtomTableT<Param>::atomParameters(ResID resid) const
{
    return this->atomParameters( info().residueNumber(resid) );
}

/** Return an array of the AtomParameters for the atoms in the CutGroup
    with ID == 'cgid', in AtomID order.

    \throw SireMol::missing_cutgroup
    \throw SireDB::missing_parameter
*/
template<class Param>
QVector< AtomParameter<Param> >
AtomTableT<Param>::atomParameters(CutGroupID cgid) const
{
    const AtomInfoGroup &atomgroup = info().at(cgid);

    uint nats = atomgroup.nAtoms();

    //get the array of parameters - as this may already be cached
    QVector<Param> cgparams = this->parameters(cgid);
    const Param *paramarray = cgparams.constData();

    BOOST_ASSERT(cgparams.count() == nats);

    QVector< AtomParameter<Param> > atomparams;
    atomparams.reserve(nats);

    //add these to the AtomInfo of each atom to create the
    //AtomParameters
    for (AtomID i(0); i<nats; ++i)
    {
        atomparams.append( AtomParameter<Param>( atomgroup[i], paramarray[i] ) );
    }

    return atomparams;
}

/** Return an array of the AtomParameters for the atoms in the CutGroup
    with number 'cgnum'

    \throw SireMol::missing_cutgroup
    \throw SireDB::missing_parameter
*/
template<class Param>
QVector< AtomParameter<Param> >
AtomTableT<Param>::atomParameters(CutGroupNum cgnum) const
{
    return this->atomParameters( info().cutGroupID(cgnum) );
}

/** Internal function used to return the AtomParameters of groups */
template<class Param>
template<class C>
QHash< C,QVector< AtomParameter<Param> > >
AtomTableT<Param>::_pvt_atomParameters( const QSet<C> &idxs ) const
{
    QHash< C,QVector<AtomParameter<Param> > > allparams;
    allparams.reserve(idxs.count());

    foreach (C idx, idxs)
    {
        allparams.insert( idx, this->atomParameters(idx) );
    }

    return allparams;
}

/** Return arrays of the AtomParameters for the CutGroups whose
    IDs are in 'cgids'

    \throw SireMol::missing_cutgroup
    \throw SireDB::missing_parameter
*/
template<class Param>
QHash< CutGroupID,QVector< AtomParameter<Param> > >
AtomTableT<Param>::atomParameters(const QSet<CutGroupID> &cgids) const
{
    return this->_pvt_atomParameters<CutGroupID>(cgids);
}

/** Return arrays of the AtomParameters for the CutGroups whose
    numbers are in 'cgnums'

    \throw SireMol::missing_cutgroup
    \throw SireDB::missing_parameter
*/
template<class Param>
QHash< CutGroupNum,QVector< AtomParameter<Param> > >
AtomTableT<Param>::atomParameters(const QSet<CutGroupNum> &cgnums) const
{
    return this->_pvt_atomParameters<CutGroupNum>(cgnums);
}

/** Return arrays of the AtomParameters for the residues whose
    numbers are in 'resnums'

    \throw SireMol::missing_residue
    \throw SireDB::missing_parameter
*/
template<class Param>
QHash< ResNum,QVector< AtomParameter<Param> > >
AtomTableT<Param>::atomParameters(const QSet<ResNum> &resnums) const
{
    return this->_pvt_atomParameters(resnums);
}

/** Return arrays of the AtomParameters for the residues whose
    indicies are in 'resids'

    \throw SireError::invalid_index
    \throw SireDB::missing_parameter
*/
template<class Param>
QHash< ResID,QVector< AtomParameter<Param> > >
AtomTableT<Param>::atomParameters(const QSet<ResID> &resids) const
{
    return this->_pvt_atomParameters(resids);
}

static const MagicID atomtablet_magic = getMagic("SireDB::AtomTableT");

#endif // SKIP_TEMPLATE_DEFINITIONS

}

#ifndef SKIP_TEMPLATE_DEFINITIONS

/** Serialise an AtomTable to a binary data stream */
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
QDataStream& operator<<(QDataStream &ds, const SireDB::AtomTableT<Param> &table)
{
    SireStream::writeHeader(ds, SireDB::atomtablet_magic, 1)
               << table.params << static_cast<const SireDB::AtomTable&>(table);

    return ds;
}

/** Deserialise an AtomTable from a binary data stream */
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
QDataStream& operator>>(QDataStream &ds, SireDB::AtomTableT<Param> &table)
{
    SireStream::VersionID v = SireStream::readHeader(ds, SireDB::atomtablet_magic,
                                                     "SireDB::AtomTableT");

    if (v == 1)
    {
        ds >> table.params >> static_cast<SireDB::AtomTable&>(table);
    }
    else
        throw SireStream::version_error(v, "1", "SireDB::AtomTableT", CODELOC);

    return ds;
}

#endif // SKIP_TEMPLATE_DEFINITIONS

SIRE_END_HEADER

#endif
