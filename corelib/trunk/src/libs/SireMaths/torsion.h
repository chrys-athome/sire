#ifndef SIREMATHS_TORSION_H
#define SIREMATHS_TORSION_H

#include "vector.h"
#include "line.h"
#include "triangle.h"
#include "angle.h"

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireMaths
{
class Torsion;
}

class QDataStream;
QDataStream& operator<<(QDataStream&, const SireMaths::Torsion&);
QDataStream& operator>>(QDataStream&, SireMaths::Torsion&);

namespace SireMaths
{

/**
This class represents a torsion in three dimensional space, e.g. four points in space, not necessarily lying in a plane. A torsion is used to calculate dihedral angles (imagine each point is an atom). I am not happy with the name of this class, and welcome suggestions :-)
 
@author Christopher Woods
*/
class SIREMATHS_EXPORT Torsion
{

friend QDataStream& ::operator<<(QDataStream&, const Torsion&);
friend QDataStream& ::operator>>(QDataStream&, Torsion&);

public:
    Torsion();
    Torsion( const Vector &point0, const Vector &point1,
             const Vector &point2, const Vector &point3 );
    ~Torsion();

    QString toString() const;

    Angle angle() const;

    Line line03() const;
    Line line12() const;

    Vector vector03() const;
    Vector vector12() const;

    Triangle triangle1() const;
    Triangle triangle2() const;

    int count() const;

    const Vector& point( int i ) const;
    const Vector& operator[] ( int i ) const;
    const Vector& at( int i ) const;

private:

    /** The four points that make up the torsion */
    Vector points[4];
};

/** Return the torsion angle of this torsion (the torsion angle 0-1-2-3 around the 1-2 line) */
inline Angle Torsion::angle() const
{
    return Vector::dihedral(points[0], points[1], points[2], points[3]);
}

/** Return the line from point 0 to point 3 */
inline Line Torsion::line03() const
{
    return Line(points[0], points[3]);
}

/** Return the line from point 1 to point 2 */
inline Line Torsion::line12() const
{
    return Line(points[1], points[2]);
}

/** Return the vector from point 0 to point 3 */
inline Vector Torsion::vector03() const
{
    return line03().vector();
}

/** Return the vector from point 1 to point 2 */
inline Vector Torsion::vector12() const
{
    return line12().vector();
}

/** Return the triangle around point 1, i.e. point0-point1-point2 */
inline Triangle Torsion::triangle1() const
{
    return Triangle(points[0], points[1], points[2]);
}

/** Return the triangle around point 2, i.e. point1-point2-point3 */
inline Triangle Torsion::triangle2() const
{
    return Triangle(points[1], points[2], points[3]);
}

/** Return the number of points in a torsion (4) */
inline int Torsion::count() const
{
    return 4;
}

/** Return the point at index 'i' */
inline const Vector& Torsion::point( int i ) const
{
    return points[ i % 4 ];
}

/** Return the point at index 'i' */
inline const Vector& Torsion::operator[] ( int i ) const
{
    return this->point(i);
}

/** Return the point at index 'i' */
inline const Vector& Torsion::at( int i ) const
{
    return this->point(i);
}

}

Q_DECLARE_METATYPE(SireMaths::Torsion)
Q_DECLARE_TYPEINFO(SireMaths::Torsion, Q_MOVABLE_TYPE);

SIRE_END_HEADER

#endif
