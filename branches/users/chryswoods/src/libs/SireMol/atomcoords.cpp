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

static void assertCanConvert(const QVariant &value)
{
    if (not value.canConvert<Vector>())
    {
        throw SireError::invalid_cast( QObject::tr(
            "Cannot convert an object of type %1 to a SireMaths::Vector, "
            "as is required to allow this object to be part of an "
            "AtomCoords property.")
                .arg(value.typeName()), CODELOC );
    }
}

static CoordGroup makeCoordGroup(const QVector<QVariant> &values)
{
    if (values.isEmpty())
        return CoordGroup();
        
    int nvals = values.count();
    const QVariant *values_array = values.constData();
    
    CoordGroupEditor editor = CoordGroupEditor( CoordGroup(nvals) );
    
    Vector *editor_array = editor.data();
    
    for (int i=0; i<nvals; ++i)
    {
        const QVariant &value = values_array[i];
        assertCanConvert(value);
        
        editor_array[i] = value.value<Vector>();
    }
    
    return editor.commit();
}

/** Construct from an array of QVariants 

    \throw SireError::invalid_cast
*/
AtomProperty<Vector>::AtomProperty(const QVector<QVariant> &values)
                     : ConcreteProperty<AtomProperty<Vector>,AtomProp>()
{
    if (not values.isEmpty())
    {
        coords = QVector<CoordGroup>( 1, makeCoordGroup(values) );
        coords.squeeze();
    }
}

/** Construct from an array of an array of QVariants - each QVariant array
    contains the coordinates for one CutGroup
    
    \throw SireError::invalid_cast
*/
AtomProperty<Vector>::AtomProperty(const QVector< QVector<QVariant> > &values)
                     : ConcreteProperty<AtomProperty<Vector>,AtomProp>()
{
    if (values.isEmpty())
        return;
        
    int ngroups = values.count();
    
    coords = QVector<CoordGroup>(ngroups);
    coords.squeeze();
    
    CoordGroup *coords_array = coords.data();
    const QVector<QVariant> *values_array = values.constData();
    
    for (int i=0; i<ngroups; ++i)
    {
        coords_array[i] = makeCoordGroup( values_array[i] );
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

/** Convert the coordinates to an array of array of vectors - each
    array holds the coordinates of one CutGroup */
QVector< QVector<QVariant> > AtomProperty<Vector>::toVariant() const
{
    if (coords.isEmpty())
        return QVector< QVector<QVariant> >();
        
    int ngroups = coords.count();
    
    QVector< QVector<QVariant> > converted_coords(ngroups);
    converted_coords.squeeze();
    
    const CoordGroup *coords_array = coords.constData();
    QVector<QVariant> *converted_coords_array = converted_coords.data();
    
    for (int i=0; i<ngroups; ++i)
    {
        const CoordGroup &cgroup = coords_array[i];
        int nats = cgroup.count();
        
        if (nats == 0)
            converted_coords_array[i] = QVector<QVariant>();
        else
        {
            const Vector *cgroup_array = cgroup.constData();

            QVector<QVariant> converted_vals(nats);
            converted_vals.squeeze();
            QVariant *converted_vals_array = converted_vals.data();
            
            for (int j=0; j<nats; ++j)
            {
                converted_vals_array[j].setValue<Vector>(cgroup_array[j]);
            }
            
            converted_coords_array[i] = converted_vals;
        }
    }
    
    return converted_coords;
}

/** Assign this from an array of QVariants */
void AtomProperty<Vector>::assignFrom(const QVector<QVariant> &values)
{
    this->operator=( AtomProperty<Vector>(values) );
}

/** Assign this from an array of array of QVariants */
void AtomProperty<Vector>::assignFrom(const QVector< QVector<QVariant> > &values)
{
    this->operator=( AtomProperty<Vector>(values) );
}

/** Return whether or not the passed QVariant is compatible with
    this property */
bool AtomProperty<Vector>::canConvert(const QVariant &value) const
{
    return value.canConvert<Vector>();
}

/** Assert that the passed QVariant is compatible with this property */
void AtomProperty<Vector>::assertCanConvert(const QVariant &value) const
{
    return ::assertCanConvert(value);
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
