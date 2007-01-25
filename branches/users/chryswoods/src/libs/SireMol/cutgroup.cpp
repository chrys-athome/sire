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

#include "cutgroup.h"
#include "atomid.h"
#include "atom.h"

#include "SireError/errors.h"
#include "SireStream/datastream.h"

using namespace SireMol;
using namespace SireVol;
using namespace SireStream;

static const RegisterMetaType<SireMol::CutGroup> r_cutgroup;

/** Serialise to a binary data stream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const CutGroup &cgroup)
{
    writeHeader(ds, r_cutgroup, 1) << cgroup.atominfos << cgroup.coords;
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, CutGroup &cgroup)
{
    VersionID v = readHeader(ds, r_cutgroup);

    if (v == 1)
    {
        ds >> cgroup.atominfos >> cgroup.coords;
    }
    else
        throw version_error(v, "1", r_cutgroup, CODELOC);

    return ds;
}

/** Null constructor */
CutGroup::CutGroup()
{}

/** Construct a CutGroup containing the 'atoms'. This does not check
    for duplicate atoms, though you would be wise not to include
    duplicate atoms! */
CutGroup::CutGroup(const QVector<Atom> atoms)
{
    int sz = atoms.count();

    if (sz > 0)
    {
        //get a pointer to the array containing the atoms
        const Atom *atomarray = atoms.constData();

        //make space for the AtomInfo objects
        QVector<AtomInfo> atominfovector;
        atominfovector.resize(sz);

        //edit a new CoordGroup
        CoordGroupEditor editor = CoordGroup(sz).edit();

        //get a mutable pointer to the coordinate array, and the info array
        AtomInfo *infoarray = atominfovector.data();
        Vector *coordarray = editor.data();

        //copy the information from the Atom array into the info and coordinate arrays
        for (int i=0; i<sz; ++i)
        {
            coordarray[i] = atomarray[i];
            infoarray[i] = atomarray[i];
        }

        //save the coordinates
        coords = editor.commit();
        //save the metadata
        atominfos = atominfovector;
    }
}

/** Construct a CutGroup containing the 'atoms'. This does not check
    for duplicate atoms, though you would be wise not to include
    duplicate atoms! */
CutGroup::CutGroup(const QList<Atom> atoms)
{
    int sz = atoms.count();

    if (sz > 0)
    {
        QVector<AtomInfo> atominfovector;
        atominfovector.resize(sz);

        //edit a new CoordGroup
        CoordGroupEditor editor = CoordGroup(sz).edit();

        //get mutable pointers to the coordinate and info arrays
        AtomInfo *infoarray = atominfovector.data();
        Vector *coordarray = editor.data();

        //copy the information from the list
        int i = 0;
        for (QList<Atom>::const_iterator it = atoms.begin();
             it != atoms.end();
             ++it)
        {
            coordarray[i] = *it;
            infoarray[i] = *it;
            ++i;
        }

        //save the coordinates
        coords = editor.commit();
        //save the metadata
        atominfos = atominfovector;
    }
}

/** This function checks that the AtomInfoGroup and CoordGroup that comprise
    this CutGroup are compatible - if not then an exception if thrown

    \throw SireError::incompatible_error
*/
void CutGroup::assertCompatible() const
{
    if (atominfos.nAtoms() != coords.count())
        throw SireError::incompatible_error( QObject::tr(
              "You cannot create a CutGroup from an AtomInfo containing "
              "%1 atoms, and a CoordGroup containing %2 sets of coordinates!")
                  .arg(atominfos.nAtoms()).arg(coords.count()), CODELOC );
}

/** Construct a CutGroup using the atom information from 'infos' and
    the coordinates from 'coordinates'. The number of coordinates in 'coordinates'
    must be the same as the number of atoms in 'infos' or an
    exception will be thrown.

    \throw SireError::incompatible_error
*/
CutGroup::CutGroup(const AtomInfoGroup &infos, const CoordGroup &coordinates)
         : atominfos(infos), coords(coordinates)
{
    this->assertCompatible();
}

/** Construct a CutGroup using the atom information from 'infos' and
    the coordinates in 'coordinates'. The number of coordinates in
    'coordinates' must be the same as the number of atoms in 'infos'
    or an exception will be thrown

    \throw SireError::incompatible_error
*/
CutGroup::CutGroup(const AtomInfoGroup &infos,
                   const QVector<Vector> &coordinates)
         : atominfos(infos), coords(coordinates)
{
    this->assertCompatible();
}

/** Construct a CutGroup using the atom infomation from 'infos'. The
    atoms are all initially placed at the origin. */
CutGroup::CutGroup(const AtomInfoGroup &infos)
         : atominfos(infos)
{
    if (atominfos.nAtoms() > 0)
        coords = CoordGroup(atominfos.nAtoms(), Vector(0));
}

/** Copy constructor - fast as the data for this object is implicitly shared */
CutGroup::CutGroup(const CutGroup &other)
         : atominfos(other.atominfos), coords(other.coords)
{}

/** Destructor */
CutGroup::~CutGroup()
{}

/** Comparison operator */
bool CutGroup::operator==(const CutGroup &other) const
{
    return atominfos == other.atominfos and coords == other.coords;
}

/** Comparison operator */
bool CutGroup::operator!=(const CutGroup &other) const
{
    return atominfos != other.atominfos or coords != other.coords;
}

/** Return a copy of the 'ith' atom - this will throw an exception
    if 'i' refers to an invalid index

    \throw SireError::invalid_index
*/
Atom CutGroup::at(AtomID i) const
{
    //try 'coords' first, as this will throw an exception of the
    //right type
    const Vector &coord = coords[i];

    //now get the info
    const AtomInfo &info = atominfos[i];

    return Atom(info,coord);
}

/** Return a copy of the 'ith' atom - this will throw an exception
    if 'i' refers to an invalid index

    \throw SireError::invalid_index
*/
Atom CutGroup::atom(AtomID i) const
{
    return this->at(i);
}

/** Return a copy of the 'ith' atom - this will throw an exception
    if 'i' refers to an invalid index

    \throw SireError::invalid_index
*/
Atom CutGroup::operator[](AtomID i) const
{
    return this->at(i);
}

/** Assignment operator - fast as the data for this object is implicitly shared */
CutGroup& CutGroup::operator=(const CutGroup &other)
{
    atominfos = other.atominfos;
    coords = other.coords;
    return *this;
}

/** Set the coordinates for the atoms in the CutGroup. The coordinates must be
    in the same order as the atoms in this CutGroup, and must have the same size,
    else an exception will be thrown.

    \throw SireError::incompatible_error
*/
void CutGroup::setCoordGroup(const CoordGroup &newcoords)
{
    if (newcoords.size() == coords.size())
        coords = newcoords;
    else
        throw SireError::incompatible_error( QObject::tr(
                      "Cannot use a CoordGroup of size() == %1 to update the coordinates "
                      "of a CutGroup with size() == %2")
                          .arg(newcoords.size()).arg(coords.size()), CODELOC );
}

/** Set the coordinates of the atoms in the CutGroup. The coordinates must
    be in the same order as the atoms in this CutGroup, and must have the
    same size else an exception will be thrown.

    \throw SireError::incompatible_error
*/
void CutGroup::setCoordinates(const QVector<Vector> &newcoords)
{
    this->setCoordGroup( CoordGroup(newcoords) );
}

/** Assign from a CoordGroup - use this to update the coordinates only

    \throw SireError::incompatible_error
*/
CutGroup& CutGroup::operator=(const CoordGroup &coordgroup)
{
    this->setCoordGroup(coordgroup);
    return *this;
}

/** Assign from an array of coordinates - use this to update the coordinates
    only

    \throw SireError::incompatible_error
*/
CutGroup& CutGroup::operator=(const QVector<Vector> &coords)
{
    this->setCoordinates(coords);
    return *this;
}

/** Return a string representation of this CutGroup */
QString CutGroup::toString() const
{
    return QObject::tr("CutGroup: nAtoms() == %1").arg(nAtoms());
}

/** Return a vector of all of the atoms in this CutGroup, in the same order
    as the atoms in the CutGroup. */
QVector<Atom> CutGroup::atoms() const
{
    QVector<Atom> atomvector;

    int nats = coords.size();
    if (nats > 0)
    {
        //get const pointers to the info and coords arrays...
        const AtomInfo *infoarray = atominfos.constData();
        const Vector *coordarray = coords.constData();

        //resize the vector to hold the output
        atomvector.resize(nats);

        Atom *atomarray = atomvector.data();

        for (int i=0; i<nats; ++i)
        {
            Atom &atom = atomarray[i];

            atom = infoarray[i];
            atom = coordarray[i];
        }
    }

    return atomvector;
}

/** Return a vector of all of the atoms from 'strt' to 'end' in this CutGroup.
    If 'strt' is less than 'end' then the order is reversed. If strt or end refer
    to invalid indexes then an exception is thrown.

    \throw SireError::invalid_index
*/
QVector<Atom> CutGroup::atoms(AtomID strt, AtomID end) const
{
    int nats = coords.size();

    if (strt >= nats or end >= nats)
        throw SireError::invalid_index( QObject::tr(
                "Invalid slice indicies for a CutGroup: %1:%2 when size() == %3")
                    .arg(strt).arg(end).arg(nats), CODELOC );

    QVector<Atom> atomvector;

    if (strt < end)
    {
        const AtomInfo *infoarray = atominfos.constData();
        const Vector *coordarray = coords.constData();

        atomvector.resize( end - strt );
        Atom *atomarray = atomvector.data();

        int idx = 0;
        for (int i=strt; i<end; ++i)
        {
            Atom &atom = atomarray[idx];
            atom = infoarray[i];
            atom = coordarray[i];
            ++idx;
        }
    }
    else if (strt > end)
    {
        const AtomInfo *infoarray = atominfos.constData();
        const Vector *coordarray = coords.constData();

        atomvector.resize( strt - end );
        Atom *atomarray = atomvector.data();

        int idx = 0;
        for (int i=end; i>strt; --i)
        {
            Atom &atom = atomarray[idx];
            atom = infoarray[i];
            atom = coordarray[i];
            ++idx;
        }
    }

    return atomvector;
}

/** Return an array of all of the coordinates of the atoms in this CutGroup.
    This is in the same order as the array produced by CutGroup::atoms() */
QVector<Vector> CutGroup::coordinates() const
{
    QVector<Vector> coordvector;

    int nats = coords.size();
    if (nats > 0)
    {
        //get const pointers to the coords arrays...
        const Vector *coordarray = coords.constData();

        //resize the vector to hold the output
        coordvector.reserve(nats);

        for (int i=0; i<nats; ++i)
        {
            coordvector.append( coordarray[i] );
        }
    }

    return coordvector;
}

/** Return a vector of all of the coordinates from 'strt' to 'end' in this CutGroup.
    If 'strt' is less than 'end' then the order is reversed. If strt or end refer
    to invalid indexes then an exception is thrown. This returns the coordinates
    in the same order as CutGroup::atoms(int strt, int end).

    \throw SireError::invalid_index
*/
QVector<Vector> CutGroup::coordinates(AtomID strt, AtomID end) const
{
    int nats = coords.size();

    if (strt >= nats or end >= nats)
        throw SireError::invalid_index( QObject::tr(
                "Invalid slice indicies for a CutGroup: %1:%2 when size() == %3")
                    .arg(strt).arg(end).arg(nats), CODELOC );

    QVector<Vector> coordvector;

    if (strt < end)
    {
        const Vector *coordarray = coords.constData();

        coordvector.reserve( end - strt );

        for (int i=strt; i<end; ++i)
            coordvector.append( coordarray[i] );
    }
    else if (strt > end)
    {
        const Vector *coordarray = coords.constData();

        coordvector.reserve( strt - end );

        for (int i=end; i>strt; --i)
            coordvector.append(coordarray[i]);
    }

    return coordvector;
}
