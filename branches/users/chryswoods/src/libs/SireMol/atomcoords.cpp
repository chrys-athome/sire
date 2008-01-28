/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2007  Christopher Woods
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

#include "atomcoords.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireStream;
using namespace SireVol;
using namespace SireMaths;
using namespace SireMol;
using namespace SireBase;

RegisterMetaType< AtomProperty<Vector> > r_atomcoords;

/** Serialise to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds,
                                       const AtomProperty<Vector> &atomcoords)
{
    writeHeader(ds, r_atomcoords, 1);

    SharedDataStream sds(ds);

    sds << atomcoords.coords
        << static_cast<const MolViewProperty&>(atomcoords);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds,
                                       AtomProperty<Vector> &atomcoords)
{
    VersionID v = readHeader(ds, r_atomcoords);

    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> atomcoords.coords
            >> static_cast<MolViewProperty&>(atomcoords);
    }
    else
        throw version_error(v, "1", r_atomcoords, CODELOC);

    return ds;
}

/** Null constructor */
AtomProperty<Vector>::AtomProperty()
                     : ConcreteProperty<AtomProperty<Vector>,AtomProp>()
{}

/** Create space for the coordinates for all of the atoms described in
    'molinfo' - the coordinates are all initially (0,0,0) */
AtomProperty<Vector>::AtomProperty(const MoleculeInfoData &molinfo)
                     : ConcreteProperty<AtomProperty<Vector>,AtomProp>()
{
    int ncg = molinfo.nCutGroups();

    if (ncg > 0)
    {
        coords = QVector<CoordGroup>(ncg);
        CoordGroup *coords_array = coords.data();

        for (CGIdx i(0); i<ncg; ++i)
        {
            coords_array[i] = CoordGroup(molinfo.nAtoms(i));
        }

        coords.squeeze();
    }
}

/** Construct from the passed set of coordinates (arranged into
    a single CutGroup */
AtomProperty<Vector>::AtomProperty(const QVector<Vector> &coordinates)
                     : ConcreteProperty<AtomProperty<Vector>,AtomProp>()
{
    int nats = coordinates.count();

    if (nats > 0)
    {
        coords = QVector<CoordGroup>(1);
        coords[0] = CoordGroup(coordinates);
        coords.squeeze();
    }
}

/** Construct from the passed set of coordinates (arranged into
    CutGroups) */
AtomProperty<Vector>::AtomProperty(const QVector< QVector<Vector> > &coordinates)
                     : ConcreteProperty<AtomProperty<Vector>,AtomProp>()
{
    int ncg = coordinates.count();

    if (ncg > 0)
    {
        const QVector<Vector> *coords_array = coordinates.constData();

        coords.reserve(ncg);

        for (int i=0; i<ncg; ++i)
        {
            if (coords_array[i].count() > 0)
                coords.append( CoordGroup(coords_array[i]) );
        }

        coords.squeeze();
    }
}

/** Construct from the single passed CoordGroup */
AtomProperty<Vector>::AtomProperty(const CoordGroup &cgroup)
                     : ConcreteProperty<AtomProperty<Vector>,AtomProp>()
{
    if (cgroup.count() > 0)
    {
        coords = QVector<CoordGroup>(1);
        coords[0] = cgroup;
        coords.squeeze();
    }
}

/** Construct from the array of CoordGroups */
AtomProperty<Vector>::AtomProperty(const QVector<CoordGroup> &cgroups)
                     : ConcreteProperty<AtomProperty<Vector>,AtomProp>()
{
    int ncg = cgroups.count();

    if (ncg > 0)
    {
        const CoordGroup *cgroups_array = cgroups.constData();

        coords.reserve(ncg);

        for (int i=0; i<ncg; ++i)
        {
            if (cgroups_array[i].count() > 0)
                coords.append(cgroups_array[i]);
        }

        coords.squeeze();
    }
}

/** Copy constructor */
AtomProperty<Vector>::AtomProperty(const AtomProperty<Vector> &other)
                     : ConcreteProperty<AtomProperty<Vector>,AtomProp>(other),
                       coords(other.coords)
{}

/** Destructor */
AtomProperty<Vector>::~AtomProperty()
{}

/** Copy assignment operator */
AtomProperty<Vector>& AtomProperty<Vector>::operator=(const AtomProperty<Vector> &other)
{
    MolViewProperty::operator=(other);
    coords = other.coords;
    return *this;
}

/** Comparison operator */
bool AtomProperty<Vector>::operator==(const AtomProperty<Vector> &other) const
{
    return coords == other.coords;
}

/** Comparison operator */
bool AtomProperty<Vector>::operator!=(const AtomProperty<Vector> &other) const
{
    return coords != other.coords;
}

/** Return the CoordGroup for the CutGroup at index 'cgidx' */
const CoordGroup& AtomProperty<Vector>::operator[](CGIdx cgidx) const
{
    return coords.constData()[cgidx.map(coords.count())];
}

/** Return a modifiable reference to the CoordGroup for the CutGroup at
    index 'cgidx'

    \throw SireError::invalid_index
*/
CoordGroup& AtomProperty<Vector>::operator[](CGIdx cgidx)
{
    return coords.data()[cgidx.map(coords.count())];
}

/** Return the CoordGroup for the CutGroup at index 'cgidx' */
const CoordGroup& AtomProperty<Vector>::at(CGIdx cgidx) const
{
    return this->operator[](cgidx);
}

/** Return the CoordGroup for the CutGroup at index 'cgidx' */
const CoordGroup& AtomProperty<Vector>::get(CGIdx cgidx) const
{
    return this->operator[](cgidx);
}

/** Return the coordinates of the atom at index 'cgatomidx' */
const Vector& AtomProperty<Vector>::operator[](const CGAtomIdx &cgatomidx) const
{
    return this->operator[](cgatomidx.cutGroup()).at(cgatomidx.atom());
}

/** Return the coordinates of the atom at index 'cgatomidx' */
const Vector& AtomProperty<Vector>::at(const CGAtomIdx &cgatomidx) const
{
    return this->operator[](cgatomidx);
}

/** Return the coordinates of the atom at index 'cgatomidx' */
const Vector& AtomProperty<Vector>::get(const CGAtomIdx &cgatomidx) const
{
    return this->operator[](cgatomidx);
}

/** Set the coordinates of the atom at index 'cgatomidx' to the value 'value' */
AtomProperty<Vector>& AtomProperty<Vector>::set(const CGAtomIdx &cgatomidx,
                                                const Vector &value)
{
    CoordGroup &group = coords.data()[cgatomidx.cutGroup().map(coords.count())];
    group = group.edit().setCoordinates( cgatomidx.atom().map(group.count()), value );

    return *this;
}

/** Set the coordinates of the atoms in the CutGroup at index 'cgidx'
    to have the value 'values' - note that the number of values
    must equal the number of atoms!!! */
AtomProperty<Vector>& AtomProperty<Vector>::set(CGIdx cgidx,
                                                const QVector<Vector> &values)
{
    CoordGroup &group = coords.data()[cgidx.map(coords.count())];

    if (group.count() != values.count())
        throw SireError::incompatible_error( QObject::tr(
            "Cannot set the coordinates of the atoms in the CutGroup as "
            "the number of coordinates (%1) is not the same as the number "
            "of atoms! (%2)")
                .arg(values.count()).arg(group.count()), CODELOC );

    group = CoordGroup(values);

    return *this;
}

/** Set the coordinates of atoms in the CutGroup at index 'cgidx' from
    the passed CoordGroup - note that the number of values must
    equal the number of atoms!!! */
AtomProperty<Vector>& AtomProperty<Vector>::set(CGIdx cgidx, const CoordGroup &cgroup)
{
    CoordGroup &group = coords.data()[cgidx.map(coords.count())];

    if (group.count() != cgroup.count())
        throw SireError::incompatible_error( QObject::tr(
            "Cannot set the coordinates of the atoms in the CutGroup as "
            "the number of coordinates (%1) is not the same as the number "
            "of atoms! (%2)")
                .arg(cgroup.count()).arg(group.count()), CODELOC );

    group = cgroup;

    return *this;
}

/** Return a raw pointer to the array of CoordGroups */
const CoordGroup* AtomProperty<Vector>::data() const
{
    return coords.constData();
}

/** Return a modifiable raw pointer to the array of CoordGroups */
CoordGroup* AtomProperty<Vector>::data()
{
    return coords.data();
}

/** Return a raw pointer to the array of CoordGroups */
const CoordGroup* AtomProperty<Vector>::constData() const
{
    return coords.constData();
}

/** Return a raw pointer to the array of coordinates for the
    CoordGroup at index 'cgidx' */
const Vector* AtomProperty<Vector>::data(CGIdx cgidx) const
{
    return this->operator[](cgidx).constData();
}

/** Return a raw pointer to the array of coordinates for the
    CoordGroup at index 'cgidx' */
const Vector* AtomProperty<Vector>::constData(CGIdx cgidx) const
{
    return this->data(cgidx);
}

/** Return the number of CoordGroups in this set */
int AtomProperty<Vector>::size() const
{
    return coords.count();
}

/** Return the number of CoordGroups in this set */
int AtomProperty<Vector>::count() const
{
    return coords.count();
}

/** Return the number of CoordGroups in this set */
int AtomProperty<Vector>::nCutGroups() const
{
    return coords.count();
}

/** Return the number of atoms in this set */
int AtomProperty<Vector>::nAtoms() const
{
    int nats = 0;

    for (int i=0; i<coords.count(); ++i)
    {
        nats += coords.constData()[i].count();
    }

    return nats;
}

/** Return the number of atoms in the CutGroup at index 'cgidx' */
int AtomProperty<Vector>::nAtoms(CGIdx cgidx) const
{
    return this->operator[](cgidx).count();
}

////////
//////// Lets explicitly instantiate other AtomProperty types...
////////

#include "atommasses.h"
#include "atomelements.h"

template class AtomProperty<SireUnits::Dimension::Mass>;
template class AtomProperty<Element>;
