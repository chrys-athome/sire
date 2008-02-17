/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2008  Christopher Woods
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

#ifndef SIREVOL_COORDGROUP2_H
#define SIREVOL_COORDGROUP2_H

#include <QSharedData>

#include "SireMaths/vector.h"

#include "aabox.h"

SIRE_BEGIN_HEADER

namespace SireVol
{
class CoordGroup2Base;
class CoordGroup2;
class CoordGroup2Editor;
class CoordGroup2Array;
class CoordGroup2ArrayArray;
}

namespace SireVol
{

namespace detail
{
class CGArrayArrayData;
class CGArrayData;
class CGData;
};

/** This is the base class of all CoordGroup-like classes
    (e.g. CoordGroup and CoordGroupEditor). CoordGroup classes
    hold a group of coordinates, together with an axis-aligned
    box that completely encloses all of those points. The
    class is implicitly shared, and, since it is used in the
    most performance-sensitive parts of the code, has
    a slightly more complex implicit-sharing design.

    @author Christopher Woods
*/
class SIREVOL_EXPORT CoordGroup2Base
{

public:
    ~CoordGroup2Base();

    bool operator==(const CoordGroup2Base &other) const;
    bool operator!=(const CoordGroup2Base &other) const;

    bool maybeDifferent(const CoordGroup2Base &other) const;

    const Vector& at(quint32 i) const;
    const Vector& operator[](quint32 i) const;

    const AABox& aaBox() const;

    const Vector* constData() const;
    const Vector* data() const;

    bool isEmpty() const;

    int count() const;
    int size() const;

    void assertValidIndex(quint32 i) const;

    void assertSameSize(const QVector<Vector> &coordinates) const;
    void assertSameSize(const CoordGroup2Base &other) const;

protected:
    CoordGroup2Base();

    CoordGroup2Base(quint32 size, const Vector &value = Vector());
    CoordGroup2Base(quint32 size, const Vector *values);

    CoordGroup2Base(const QVector<Vector> &coordinates);

    CoordGroup2Base(const CoordGroup2Base &other);

    CoordGroup2Base& operator=(const CoordGroup2Base &other);

    void setNeedsUpdate();
    bool needsUpdate() const;
    void update();

    const char* memory() const;
    char* memory();

    const char* constMemory() const;

    const detail::CoordGroupData& _pvt_group() const;
    detail::CoordGroupData& _pvt_group();

    const detail::CoordGroupData& _pvt_constGroup() const;

    const Vector* _pvt_data() const;
    Vector* _pvt_data();

    const Vector* _pvt_constData() const;

private:
    /** Pointer to the CGData object that describes
        this CoordGroup */
    detail::CGData *d;
};


}

SIRE_END_HEADER

#endif
