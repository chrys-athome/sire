
#include "plane.h"

#include "SireStream/datastream.h"

using namespace SireMaths;
using namespace SireStream;

static const RegisterMetaType<Plane> r_plane;

/** Serialise to a binary data stream */
QDataStream SIREMATHS_EXPORT &operator<<(QDataStream &ds, const Plane &plane)
{
    writeHeader(ds, r_plane, 1) << plane.norm << plane.dist;

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMATHS_EXPORT &operator>>(QDataStream &ds, Plane &plane)
{
    VersionID v = readHeader(ds, r_plane);

    if (v == 1)
    {
        ds >> plane.norm >> plane.dist;
    }
    else
        throw version_error(v, "1",  r_plane, CODELOC);

    return ds;
}

/** Construct a default plane - this is the x/y plane, lying at the origin */
Plane::Plane() : norm(0.0,0.0,1.0), dist(0.0)
{}

/** Construct the plane lying perpendicular to 'normal' and at a distance of 'dist'
    from the origin */
Plane::Plane(const Vector &normal, const double &distance)
      : dist(distance)
{
    //normalise the normal
    try
    {
        norm = normal.normalise();
    }
    catch( SireMaths::math_error& )
    {
        //trying to normalise a zero vector
        norm.set(0.0,0.0,1.0);
    }
}

/** Construct a plane that lies perpendicular to 'normal' and that also contains
    the point 'contains_point' */
Plane::Plane(const Vector &normal, const Vector &contains_point)
{
    //normalise the normal
    try
    {
        norm = normal.normalise();
    }
    catch( SireMaths::math_error& )
    {
        //trying to normalise a zero vector
        norm.set(0.0,0.0,1.0);
    }

    //the equation of the plane is 'ax + by + cz + d = 0'
    //If the plane contains 'contains_point', then we can just
    //plug this into the equation to find 'd'
    dist = -( Vector::dot(norm, contains_point) );
}

/** Construct the plane that fulfills the equation "ax + by + cz + d = 0" */
Plane::Plane(const double &a, const double &b, const double &c, const double &d)
      : norm(a,b,c), dist(d)
{
    double lgth = norm.length();

    //we need to ensure that the plane normal is normalised
    try
    {
        norm /= lgth;
        dist /= lgth;
    }
    catch( SireMaths::math_error& )
    {
        //trying to normalise a zero vector
        norm.set(0.0,0.0,1.0);
        dist = 0.0;
    }
}

/** Copy constructor */
Plane::Plane(const Plane &other) : norm(other.norm), dist(other.dist)
{}

/** Destructor */
Plane::~Plane()
{}
