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

#ifndef SIREDB_PARAMETERGROUP_HPP
#define SIREDB_PARAMETERGROUP_HPP

#include <QVector>
#include <QSet>

#include "atomparameter.hpp"

#include "SireMol/atominfogroup.h"
#include "SireMol/cgatomid.h"
#include "SireMol/cgnumatomid.h"
#include "SireMol/resnumatomid.h"
#include "SireMol/residatomid.h"

#include "SireMol/moleculeinfo.h"
#include "SireMol/residueinfo.h"

#include "SireError/errors.h"

SIRE_BEGIN_HEADER

namespace SireDB
{
template<class Param>
class ParameterGroup;
}

template<class Param>
QDataStream& operator<<(QDataStream&, const SireDB::ParameterGroup<Param>&);
template<class Param>
QDataStream& operator>>(QDataStream&, SireDB::ParameterGroup<Param>&);

namespace SireDB
{

using SireMol::AtomInfoGroup;

/** This class holds a group of AtomParameters. This class is the parameter
    equivalent of SireMol::CutGroup. SireMol::CutGroup holds a group of
    atoms, holding the metadata in an AtomInfoGroup, and the coordinates
    in a QVector<SireMaths::Vector>. This class is very similar, also
    holding the metadata for the atoms in an AtomInfoGroup, but now holding
    the parameters in a QVector<Param>.

    \param Param  The type of parameter that is stored in this group

    @author Christopher Woods
*/
template<class Param>
class ParameterGroup
{

friend QDataStream& ::operator<<<>(QDataStream&, const ParameterGroup<Param>&);
friend QDataStream& ::operator>><>(QDataStream&, ParameterGroup<Param>&);

public:
    ParameterGroup();
    ParameterGroup(const QVector< AtomParameter<Param> > &params);
    ParameterGroup(const QList< AtomParameter<Param> > &params);

    ParameterGroup(const AtomInfoGroup &infogroup);
    ParameterGroup(const AtomInfoGroup &infogroup,
                   const QVector<Param> &parameters);

    ParameterGroup(const ParameterGroup<Param> &other);

    ~ParameterGroup();

    AtomParameter<Param> at(AtomID i) const;
    AtomParameter<Param> operator[](AtomID i) const;

    ParameterGroup<Param>& operator=(const ParameterGroup<Param> &other);
    ParameterGroup<Param>& operator=(const QVector<Param> &params);

    bool operator==(const ParameterGroup<Param> &other) const;
    bool operator!=(const ParameterGroup<Param> &other) const;

    bool isEmpty() const;

    QString toString() const;

    QVector< AtomParameter<Param> > atomParameters() const;
    QVector< AtomParameter<Param> > atomParameters(AtomID strt, AtomID end) const;

    const QVector<Param>& parameters() const;
    QVector<Param> parameters(AtomID strt, AtomID end) const;

    AtomParameter<Param> atomParameter(AtomID i) const;
    Param parameter(AtomID i) const;

    int nAtoms() const;

    void setParameters(const QVector<Param> &newparams);

    const AtomInfoGroup& atomGroup() const;

private:
    void assertSane() const;

    /** The metainformation about the atoms whose parameters
        this group corresponds to */
    AtomInfoGroup atominfos;

    /** Array of the parameters in this group */
    QVector<Param> params;
};

#ifndef SKIP_TEMPLATE_DEFINITIONS

/** Null constructor */
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
ParameterGroup<Param>::ParameterGroup()
{}

/** Construct from the passed array of AtomParameters */
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
ParameterGroup<Param>::ParameterGroup(
                          const QVector< AtomParameter<Param> > &params)
{
    int nats = params.count();

    if (nats > 0)
    {
        QVector<AtomInfo> atominfovector;
        atominfovector.reserve(nats);
        params.reserve(nats);

        const AtomParameter<Param> *array = params.constData();

        for (int i=0; i<nats; ++i)
        {
            atominfovector.append(array[i]);
            params.append(array[i]);
        }

        atominfos = atominfovector;
    }
}

/** Construct from the list of AtomParameters */
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
ParameterGroup<Param>::ParameterGroup(const QList< AtomParameter<Param> > &params)
{
    int nats = params.count();

    if (nats > 0)
    {
        QVector<AtomInfo> atominfovector;
        atominfovector.reserve(nats);
        params.reserve(nats);

        for (typename QList< AtomParameter<Param> >::const_iterator it = params.begin();
             it != params.end();
             ++it)
        {
            atominfovector.append(*it);
            params.append(*it);
        }

        atominfos = atominfovector;
    }
}

/** Construct using default-constructed parameters for each of the
    atoms in the passed AtomInfoGroup */
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
ParameterGroup<Param>::ParameterGroup(const AtomInfoGroup &infogroup)
                      : atominfos(infogroup), params( infogroup.nAtoms() )
{}

/** Check the sanity of this ParameterGroup (that the number of parameters
    equals the number of atoms)

    \throw SireError::incompatible_error
*/
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
void ParameterGroup<Param>::assertSane() const
{
    if (atominfos.nAtoms() != params.count())
        throw SireError::incompatible_error( QObject::tr(
                "The parameters are incompatible with the atoms, because the "
                "number of parameters equals %1 while the number of atoms "
                "equals %2.")
                    .arg(atominfos.nAtoms()).arg(params.count()), CODELOC );
}

/** Construct giving the parameters in 'parameters' to each of the
    atoms in the passed AtomInfoGroup - there must be the same number
    of parameters as atoms or else an exception will be thrown

    \throw SireError::incompatible_error
*/
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
ParameterGroup<Param>::ParameterGroup(const AtomInfoGroup &infogroup,
                                      const QVector<Param> &parameters)
                      : atominfos(infogroup), params(parameters)
{
    this->assertSane();
}

/** Copy constructor */
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
ParameterGroup<Param>::ParameterGroup(const ParameterGroup<Param> &other)
                      : atominfos(other.atominfos), params(other.params)
{}

/** Destructor */
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
ParameterGroup<Param>::~ParameterGroup()
{}

/** Return the AtomParameter at index 'i'

    \throw SireError::invalid_index
*/
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
AtomParameter<Param> ParameterGroup<Param>::at(AtomID i) const
{
    //try AtomInfoGroup first, as this will throw an exception
    //for an invalid index
    return AtomParameter<Param>( atominfos.at(i), params.constData()[i] );
}

/** Return the AtomParameter at index 'i'

    \throw SireError::invalid_index
*/
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
AtomParameter<Param> ParameterGroup<Param>::operator[](AtomID i) const
{
    return this->at(i);
}

/** Assignment operator */
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
ParameterGroup<Param>& ParameterGroup<Param>::operator=(const ParameterGroup<Param> &other)
{
    atominfos = other.atominfos;
    params = other.params;
    return *this;
}

/** Assign the parameters for the atoms from 'params' - this must have
    the same number of parameters in it as there are numbers of atoms
    in this group

    \throw SireError::incompatible_error
*/
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
ParameterGroup<Param>& ParameterGroup<Param>::operator=(const QVector<Param> &parameters)
{
    if (parameters.count() != atominfos.nAtoms())
        throw SireError::incompatible_error( QObject::tr(
                  "You cannot assign the parameters to this group because the number of "
                  "parameters (%1) is not equal to the number of atoms (%2).")
                      .arg(parameters.count()).arg(atominfos.nAtoms()), CODELOC );


    params = parameters;

    return *this;
}

/** Comparison operator */
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
bool ParameterGroup<Param>::operator==(const ParameterGroup<Param> &other) const
{
    return params == other.params and atominfos == other.atominfos;
}

/** Comparison operator */
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
bool ParameterGroup<Param>::operator!=(const ParameterGroup<Param> &other) const
{
    return params != other.params or atominfos != other.atominfos;
}

/** Return whether or not this group is empty (has no atoms) */
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
bool ParameterGroup<Param>::isEmpty() const
{
    return atominfos.isEmpty();
}

/** Return a string representation of this ParameterGroup */
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
QString ParameterGroup<Param>::toString() const
{
    int nats = atominfos.nAtoms();

    QStringList lines;

    for (AtomID i(0); i<nats; ++i)
    {
        lines.append( QString("%1: %2").arg(atominfos[i].index().toString())
                                       .arg(params[i].toString()) );
    }

    return lines.join("\n");
}

/** Return the AtomParameters in this group in an array, ordered in the
    same order as this group. */
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
QVector< AtomParameter<Param> > ParameterGroup<Param>::atomParameters() const
{
    QVector< AtomParameter<Param> > atomparams;

    int nats = atominfos.nAtoms();

    if (nats > 0)
    {
        atomparams.reserve(nats);

        const AtomInfo *infoarray = atominfos.constData();
        const Param *paramarray = params.constData();

        for (int i; i<nats; ++i)
        {
            atomparams.append( AtomParameter<Param>( infoarray[i],
                                                     paramarray[i] ) );
        }
    }

    return atomparams;
}

/** Return the AtomParameters in this group from indicies 'strt' to 'end'.
    If 'strt' is less than 'end' then the order is reversed. If strt or end refer
    to invalid indexes then an exception is thrown.

    \throw SireError::invalid_index
*/
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
QVector< AtomParameter<Param> > ParameterGroup<Param>::atomParameters(AtomID strt,
                                                                      AtomID end) const
{
    int nats = atominfos.nAtoms();

    if (strt >= nats or end >= nats)
        throw SireError::invalid_index( QObject::tr(
                "Invalid slice indicies for a ParameterGroup: %1:%2 when nAtoms() == %3")
                    .arg(strt).arg(end).arg(nats), CODELOC );

    QVector< AtomParameter<Param> > atomparams;

    if (strt < end)
    {
        const AtomInfo *infoarray = atominfos.constData();
        const Param *paramarray = params.constData();

        atomparams.resize( end - strt );
        AtomParameter<Param> *atomarray = atomparams.data();

        int idx = 0;
        for (int i=strt; i<end; ++i)
        {
            AtomParameter<Param> &atom = atomarray[idx];
            atom = infoarray[i];
            atom = paramarray[i];
            ++idx;
        }
    }
    else if (strt > end)
    {
        const AtomInfo *infoarray = atominfos.constData();
        const AtomParameter<Param> *paramarray = params.constData();

        atomparams.resize( strt - end );
        AtomParameter<Param> *atomarray = atomparams.data();

        int idx = 0;
        for (int i=end; i>strt; --i)
        {
            AtomParameter<Param> &atom = atomarray[idx];
            atom = infoarray[i];
            atom = paramarray[i];
            ++idx;
        }
    }

    return atomparams;
}

/** Return the array of all of the parameters, in the same order
    as they are in this group. This is fast as an implicit copy
    of the parameters are returned. */
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
const QVector<Param>& ParameterGroup<Param>::parameters() const
{
    return params;
}

/** Return the parameters in this group from indicies 'strt' to 'end'.
    If 'strt' is less than 'end' then the order is reversed. If strt or end refer
    to invalid indexes then an exception is thrown.

    \throw SireError::invalid_index
*/
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
QVector<Param> ParameterGroup<Param>::parameters(AtomID strt, AtomID end) const
{
    int nats = atominfos.nAtoms();

    if (strt >= nats or end >= nats)
        throw SireError::invalid_index( QObject::tr(
                "Invalid slice indicies for a ParameterGroup: %1:%2 when nAtoms() == %3")
                    .arg(strt).arg(end).arg(nats), CODELOC );

    QVector<Param> paramslice;

    if (strt < end)
    {
        const Param *paramarray = params.constData();

        paramslice.resize( end - strt );
        Param *paramslicearray = paramslice.data();

        int idx = 0;
        for (int i=strt; i<end; ++i)
        {
            paramslicearray[idx] = paramarray[i];
            ++idx;
        }
    }
    else if (strt > end)
    {
        const Param *paramarray = params.constData();

        paramslice.resize( strt - end );
        Param *paramslicearray = paramslice.data();

        int idx = 0;
        for (int i=end; i>strt; --i)
        {
            paramslicearray[idx] = paramarray[i];
            ++idx;
        }
    }

    return paramslice;
}

/** Return the AtomParameter at index 'i'

    \throw SireError::invalid_index
*/
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
AtomParameter<Param> ParameterGroup<Param>::atomParameter(AtomID i) const
{
    return this->at(i);
}

/** Return the parameter at index 'i' */
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
Param ParameterGroup<Param>::parameter(AtomID i) const
{
    atominfos.assertAtomExists(i);
    return params.constData()[i];
}

/** Return the number of atom parameters in this group */
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
int ParameterGroup<Param>::nAtoms() const
{
    return atominfos.nAtoms();
}

/** Set the parameters for this group to 'newparams' - the number of
    parameters must be the same as the number of atoms.

    \throw SireError::incompatible_error
*/
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
void ParameterGroup<Param>::setParameters(const QVector<Param> &newparams)
{
    this->operator=(newparams);
}

/** Return the AtomInfoGroup containing the metadata of the atoms
    in this group */
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
const AtomInfoGroup& ParameterGroup<Param>::atomGroup() const
{
    return atominfos;
}

static const MagicID parametergroup_magic = getMagic("SireDB::ParameterGroup");

#endif // SKIP_TEMPLATE_DEFINITIONS

}

#ifndef SKIP_TEMPLATE_DEFINITIONS

/** Serialise to a binary datastream */
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
QDataStream &operator<<(QDataStream &ds, const SireDB::ParameterGroup<Param> &group)
{
    SireStream::writeHeader(ds, SireDB::parametergroup_magic, 1)
                  << group.atominfos
                  << group.params;

    return ds;
}

/** Deserialise from a binary datastream */
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
QDataStream &operator>>(QDataStream &ds, SireDB::ParameterGroup<Param> &group)
{
    SireStream::VersionID v = SireStream::readHeader(ds, SireDB::parametergroup_magic,
                                                     "SireDB::ParameterGroup");

    if (v == 1)
    {
        ds >> group.atominfos >> group.params;
    }
    else
        throw SireStream::version_error(v, "1", "SireDB::ParameterGroup", CODELOC);

    return ds;
}

#endif // SKIP_TEMPLATE_DEFINITIONS

SIRE_END_HEADER

#endif
