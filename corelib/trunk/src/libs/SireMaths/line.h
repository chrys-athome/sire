#ifndef SIREMATHS_LINE_H
#define SIREMATHS_LINE_H

#include "vector.h"

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireMaths
{
class Line;
}

class QDataStream;
QDataStream& operator<<(QDataStream&, const SireMaths::Line&);
QDataStream& operator>>(QDataStream&, SireMaths::Line&);

namespace SireMaths
{

/**
This class represents a line in three-dimensional space. (or two points)
 
@author Christopher Woods
*/
class SIREMATHS_EXPORT Line
{

friend QDataStream& ::operator<<(QDataStream&, const Line&);
friend QDataStream& ::operator>>(QDataStream&, Line&);

public:
    Line();
    Line(const Vector &point0, const Vector &point1);
    ~Line();

    QString toString() const;

    double length() const;
    Vector vector() const;

    int count() const;
    
    const Vector& point(int i) const;
    const Vector& operator[](int i) const;
    const Vector& at(int i) const;

private:

    /** The two points that make up the line */
    Vector points[2];
};

/** Return the number of elements in this line (2) */
inline int Line::count() const
{
    return 2;
}

/** Return the i'th point */
inline const Vector& Line::point(int i) const
{
    return points[ i%2 ];
}

/** Return the i'th point */
inline const Vector& Line::at(int i) const
{
    return point(i);
}

/** Return the i'th point */
inline const Vector& Line::operator[](int i) const
{
    return point(i);
}

/** Return the vector that represents this line (goes from point 0 to point 1) */
inline Vector Line::vector() const
{
    return (points[1] - points[0]);
}

/** Return the length of the line */
inline double Line::length() const
{
    return vector().length();
}

}

Q_DECLARE_METATYPE(SireMaths::Line)
Q_DECLARE_TYPEINFO(SireMaths::Line, Q_MOVABLE_TYPE);

SIRE_END_HEADER

#endif
