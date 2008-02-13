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

#ifndef SIREMOL_ATOMCOORDS_H
#define SIREMOL_ATOMCOORDS_H

#include "atomproperty.hpp"

#include "SireVol/coordgroup.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
template<>
class AtomProperty<SireMaths::Vector>;
}

QDataStream& operator<<(QDataStream&, const SireMol::AtomProperty<SireMaths::Vector>&);
QDataStream& operator>>(QDataStream&, SireMol::AtomProperty<SireMaths::Vector>&);

namespace SireMol
{

using SireMaths::Vector;
using SireVol::CoordGroup;

/** This is an explicit specialisation of AtomProperty<T> for the Vector
    class, as the Vector implies coordinates, which are arranged into
    CoordGroups (so that bounding boxes are calculated automatically)

    @author Christopher Woods
*/
template<>
class AtomProperty<Vector>
       : public SireBase::ConcreteProperty<AtomProperty<Vector>,AtomProp>
{

friend QDataStream& ::operator<<(QDataStream&, const AtomProperty<SireMaths::Vector>&);
friend QDataStream& ::operator>>(QDataStream&, AtomProperty<SireMaths::Vector>&);

public:
    AtomProperty();

    AtomProperty(const MoleculeInfoData &molinfo);

    AtomProperty(const QVector<Vector> &coordinates);
    AtomProperty(const QVector< QVector<Vector> > &coordinates);

    AtomProperty(const CoordGroup &cgroup);
    AtomProperty(const QVector<CoordGroup> &cgroups);

    AtomProperty(const QVector<QVariant> &values);
    AtomProperty(const QVector< QVector<QVariant> > &values);

    AtomProperty(const AtomProperty<Vector> &other);

    ~AtomProperty();

    AtomProperty<Vector>& operator=(const AtomProperty<Vector> &other);

    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId< AtomProperty<Vector> >() );
    }

    AtomProperty<Vector>* clone() const
    {
        return new AtomProperty<Vector>(*this);
    }

    bool isCompatibleWith(const MoleculeInfoData &molinfo) const;

    bool operator==(const AtomProperty<Vector> &other) const;
    bool operator!=(const AtomProperty<Vector> &other) const;

    bool canConvert(const QVariant &value) const;

    void assignFrom(const QVector<QVariant> &values);
    void assignFrom(const QVector< QVector<QVariant> > &values);

    QVector< QVector<QVariant> > toVariant() const;

    const CoordGroup& operator[](CGIdx cgidx) const;
    CoordGroup& operator[](CGIdx cgidx);

    const CoordGroup& at(CGIdx cgidx) const;
    const CoordGroup& get(CGIdx cgidx) const;

    const Vector& operator[](const CGAtomIdx &cgatomidx) const;
    const Vector& at(const CGAtomIdx &cgatomidx) const;
    const Vector& get(const CGAtomIdx &cgatomidx) const;

    AtomProperty<Vector>& set(const CGAtomIdx &cgatomidx, const Vector &value);

    AtomProperty<Vector>& set(CGIdx cgidx, const QVector<Vector> &values);
    AtomProperty<Vector>& set(CGIdx cgidx, const CoordGroup &cgroup);

    const CoordGroup* data() const;
    const CoordGroup* constData() const;

    CoordGroup* data();

    const Vector* data(CGIdx cgidx) const;
    const Vector* constData(CGIdx cgidx) const;

    int size() const;
    int count() const;

    int nCutGroups() const;

    int nAtoms() const;
    int nAtoms(CGIdx cgidx) const;

    void assertCanConvert(const QVariant &value) const;

private:
    /** The actual atomic coordinates, arranged into CoordGroups */
    QVector<CoordGroup> coords;
};

typedef AtomProperty<Vector> AtomCoords;

}

Q_DECLARE_METATYPE( SireMol::AtomCoords );

SIRE_END_HEADER

#endif
