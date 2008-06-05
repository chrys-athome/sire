
#include <QDataStream>

#include "triangle.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireMaths;

static const RegisterMetaType<Triangle> r_triangle("SireMaths::Triangle");

/** Serialise to a binary data stream */
QDataStream SIREMATHS_EXPORT &operator<<(QDataStream &ds, const Triangle &triangle)
{
    writeHeader(ds, r_triangle, 1) 
          << triangle.points[0] << triangle.points[1] << triangle.points[2];
       
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMATHS_EXPORT &operator>>(QDataStream &ds, Triangle &triangle)
{
    VersionID v = readHeader(ds, r_triangle);
        
    if (v == 1)
    {
        ds >> triangle.points[0] >> triangle.points[1] >> triangle.points[2];
    }
    else
        throw version_error(v, "1", r_triangle, CODELOC);
        
    return ds;
}

/** Create a zero triangle */
Triangle::Triangle()
{}

/** Create a triangle from point 0 -> 1 -> 2 */
Triangle::Triangle(const Vector &point0, const Vector &point1, const Vector &point2)
{
    points[0] = point0;
    points[1] = point1;
    points[2] = point2;
}

/** Copy constructor */
Triangle::Triangle(const Triangle &other)
{
    for (int i=0; i<3; ++i)
        points[i] = other.points[i];
}

/** Destructor */
Triangle::~Triangle()
{}

/** Return a string representation of the triangle */
QString Triangle::toString() const
{
    return QObject::tr("Triangle: Angles %1, %2, %3")
                .arg(angle0().toString()).arg(angle1().toString()).arg(angle2().toString());
}
