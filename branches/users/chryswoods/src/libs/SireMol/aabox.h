#ifndef SIREMOL_AABOX_H
#define SIREMOL_AABOX_H

#include "SireMaths/vector.h"

#include "atomvector.h"
#include "atomset.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class AABox;
}

class QDataStream;
QDataStream& operator<<(QDataStream&, const SireMol::AABox&);
QDataStream& operator>>(QDataStream&, SireMol::AABox&);

namespace SireMol
{

using SireMaths::Vector;

/**
An AABox is an axis-aligned bounding box that is the smallest box that is aligned with the three cartesian axes that completely encases a CutGroup. It is trivial to obtain the bounding sphere from the AABox. The AABox is used by the distance calculators to quickly determine whether two CutGroups are within the cutoff radius, and to obtain all CutGroups that are within particular regions of space.
 
@author Christopher Woods
*/
class SIREMOL_EXPORT AABox
{

friend QDataStream& ::operator<<(QDataStream&, const AABox&);
friend QDataStream& ::operator>>(QDataStream&, AABox&);

public:
    AABox();
    AABox(const Vector &cent, const Vector &extents);
    AABox(const AtomVector &atoms);
    AABox(const AtomSet &atoms);
    AABox(const AtomList &atoms);
    
    ~AABox();

    bool operator==(const AABox &other) const;
    bool operator!=(const AABox &other) const;

    void recalculate(const AtomVector &atoms);
    void recalculate(const AtomSet &atoms);
    void recalculate(const AtomList &atoms);

    void translate(const Vector &delta);

    const AABox& operator=(const AtomSet &atoms);
    const AABox& operator=(const AtomVector &atoms);
    
    const AABox& operator=(const AABox &other);
    
    const Vector& center() const;
    const Vector& halfExtents() const;
    Vector maxCoords() const;
    Vector minCoords() const;

    double radius() const;

    bool withinDistance(double dist, const AABox &box) const;
    bool intersects(const AABox &other) const;
    
private:
    
    /** The coordinates of the center of this box */
    Vector cent;
    
    /** The positive half-extents of this box along the x/y/z axes.
        The volume of this box runs from cent-halfextent to cent+halfextent */
    Vector halfextents;
    
    /** The radius of the smallest sphere that completely contains this box */
    double rad;
};

/** Copy operator */
inline const AABox& AABox::operator=(const AABox &box)
{
    cent = box.cent;
    halfextents = box.halfextents;
    rad = box.rad;
    return *this;
}

/** Return the center of the box */
inline const Vector& AABox::center() const
{
    return cent;
}

/** Return the positive half extents of the box */
inline const Vector& AABox::halfExtents() const
{
    return halfextents;
}

/** Return the maximum coordinates of the box */
inline Vector AABox::maxCoords() const
{
    return cent + halfextents;
}

/** Return the minimum coordinates of the box */
inline Vector AABox::minCoords() const
{
    return cent - halfextents;
}

/** Return the radius of the smallest sphere that contains this box
    (the sphere is centered at 'center()', just as the box is) */
inline double AABox::radius() const
{
    return rad;
}

}

Q_DECLARE_METATYPE(SireMol::AABox)

SIRE_END_HEADER

#endif
