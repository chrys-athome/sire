#ifndef SIREMATHS_VECTOR_H
#define SIREMATHS_VECTOR_H

class QDataStream;
class QString;

#include <QObject>

#include <boost/tuple/tuple.hpp>

#include "SireMaths/errors.h"

#include "maths.h"
#include "matrix.h"

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireMaths
{
class Vector;
}

class QDataStream;
QDataStream& operator<<(QDataStream&, const SireMaths::Vector&);
QDataStream& operator>>(QDataStream&, SireMaths::Vector&);

namespace SireMaths
{

using boost::tuple;

/**
This is a simple 3D vector. 

A vector is implemented as a very simple collection of 3 doubles. It should
thus be very efficient in terms of speed and size as there is no array. This
does mean that the .x(), .y() and .z() functions provide faster access
than the [] operator. 

There is a length/distance and a fastLength/fastDistance. The normal
version uses the standard library's sqrt function, while the fast versions
use a numerical approximation that is quite accurate (to within 1-2%).

These have been benchmarked on my development system using the 'testSpeed'
static function. The results are;

100,000,000 evaluations of vectors (1.0,2.0,3.0), (3.0,2.0,1.0)

distance took on average 4498 ms
distance2 took on average 148 ms
fastDistance took on average 350 ms

This shows that you should normally use 'distance2' for distance checking,
with fastDistance if you are not concerned with total accuracy. 

The error with fastDistance has an oscillating value, with the error having
a maximum overestimate of 1*10-5, and maximum underestimate of 0.00065 between
the distances of 0.0 and 150.0

Within this range the percentage error also oscillates, with the oscillations
having a random frequency, but almost constant amplitude of 0.0005%. See the
graph in techdocs/fastLengthError.png and the script techdocs/fastLengthError.py

@author Christopher Woods
*/

class Angle;
class Quaternion;

class SIREMATHS_EXPORT Vector
{

friend QDataStream& ::operator<<(QDataStream&, const Vector&);
friend QDataStream& ::operator>>(QDataStream&, Vector&);

public:
    typedef double value_type;

    /** Create the vector (xpos,ypos,zpos) */
    Vector(double xpos, double ypos, double zpos);
    /** Construct from a tuple of three values */
    Vector( const tuple<double,double,double> &pos );
    /** Create the vector (val,val,val) */
    Vector(double val=0.0);
    /** Copy constructor */
    Vector(const Vector& p1);
    ~Vector();

    /** Return the x/y/z components of the vector */
    double x() const;
    double y() const;
    double z() const;
        
    /** Return the components via rgb */
    double r() const;
    double g() const;
    double b() const;
    
    /** Assignment operator */
    const Vector& operator=(const Vector &other);
    
    /** Increment, decrement, negate etc. */
    const Vector& operator+=(const Vector &other);
    const Vector& operator-=(const Vector &other);
    const Vector& operator*=(const double &other);
    const Vector& operator/=(const double &other);
    Vector operator-() const;

    /** Set individual values of the vector */
    void set(double x, double y, double z);
    void setX(double x);
    void setY(double y);
    void setZ(double z);
    void setR(double x);
    void setG(double y);
    void setB(double z);
        
    /** Set component 'i' to the value 'v' */
    void set(int i, const double &v);
        
    /** Access the elements of the array via an index operator */
    const double& operator[](unsigned int i) const;
    
    unsigned int count() const;
    const double& at(unsigned int i) const;
    
    /** Return the length of the vector */
    double length() const;
    /** Return the manhattan length of the vector */
    double manhattanLength() const;
    /** Return the length^2 of the vector */
    double length2() const;
    /** Return the inverse of the length of the vector */
    double invLength() const;
    /** Return the inverse length squared */
    double invLength2() const;
    /** Return a normalised form of the vector */
    Vector normalise() const;

    /** Return whether or not this is a zero length vector */
    bool isZero() const;

    /** Return a QString representation of the vector */
    QString toString() const;
    
    /** Construct a Vector from the QString representation returned by 'toString()' */
    static Vector fromString(const QString &str);

    /** Return the dot product of v0 and v1 */
    static double dot(const Vector &v0, const Vector &v1);
    /** Return the cross product of v0 and v1 */
    static Vector cross(const Vector &v0, const Vector &v1);
    
    /** Set this Vector so that it has the maximum x/y/z components out of 
        this and 'other' (e.g. this->x = max(this->x(),other.x() etc.) */
    void setMax(const Vector &other);
    /** Set this Vector so that it has the minimum x/y/z components */
    void setMin(const Vector &other);

    /** Return a vector that has the maximum x/y/z components out of this
        and 'other' */
    Vector max(const Vector &other) const;
    /** Return a vector that has the minimum components */
    Vector min(const Vector &other) const;
        
    /** == operator */
    bool operator==(const Vector &p1) const;
    /** != operator */
    bool operator!=(const Vector &p1) const;

    /** Return the bearing of this vector against (0,1,0) (north) on the xy plane */
    Angle bearing() const;
    /** Return the bearing of this vector against 'v' on the xy plane */
    Angle bearingXY(const Vector &v) const;
    /** Return the bearing of this vector against 'v' on the xz plane */
    Angle bearingXZ(const Vector &v) const;
    /** Return the bearing of this vector against 'v' on the yz plane */
    Angle bearingYZ(const Vector &v) const;
    
    /** Return the metric tensor of a vector, e.g.
           
             | y*y + z*z,    -x*y    -x*z      |
             |    -y*x,   x*x + z*z  -y*z      |
             |    -z*x       -z*y    x*x + y*y |
             
      */
    Matrix metricTensor() const;
    
    /** Return the distance squared between two vectors */
    static double distance2(const Vector &v1, const Vector &v2);
    /** Return the distance between two vectors */
    static double distance(const Vector &v1, const Vector &v2);
    /** Return the 1 / distance between two vectors */
    static double invDistance(const Vector &v1, const Vector &v2);
    /** Return 1 / distance2 between two vectors */
    static double invDistance2(const Vector &v1, const Vector &v2);

    /** Return the angle between vectors 'v0' and 'v1' - this is the smallest
        angle, and will always lie between 0 and 180 degrees */
    static Angle angle(const Vector &v0, const Vector &v1);
    /** Return the angle between v0-v1-v2 (treating the vectors as points in space) */
    static Angle angle(const Vector &v0, const Vector &v1, const Vector &v2);
    
    /** Return the dihedral angle between v0-v1-v2-v3 (treating the vectors as points) */
    static Angle dihedral(const Vector &v0, const Vector &v1, 
                          const Vector &v2, const Vector &v3);
                
    /** Generate a vector, v0, that has distance 'dst' v0-v1, angle 'ang' v0-v1-v2,
        and dihedral 'dih' v0-v1-v2-v3 */
    static Vector generate(double dst, const Vector &v1, const Angle &ang, 
                           const Vector &v2, const Angle &dih, const Vector &v3);
                          
    friend const Vector operator+(const Vector &p1, const Vector &p2);
    friend const Vector operator-(const Vector &p1, const Vector &p2);
    friend const Vector operator*(const Vector &p1, double c);
    friend const Vector operator*(double c, const Vector &p1);
    friend const Vector operator/(const Vector &p1, double c);
    friend const Quaternion operator*(const Vector &p1, const Quaternion &p2);
    friend const Quaternion operator*(const Quaternion &p1, const Vector &p2);
    friend const Vector operator*(const Matrix &m, const Vector &p);

protected:
    double sc[3];
};

inline const Vector& Vector::operator=(const Vector &other)
{
    for (int i=0; i<3; i++)
      sc[i] = other.sc[i];
    
    return *this;
}

inline double Vector::x() const
{
    return sc[0];
}

inline double Vector::y() const
{
    return sc[1];
}

inline double Vector::z() const
{
    return sc[2];
}

inline double Vector::r() const
{
    return std::max(0.0, std::min(1.0,sc[0]));
}

inline double Vector::g() const
{
    return std::max(0.0, std::min(1.0,sc[1]));
}

inline double Vector::b() const
{
    return std::max(0.0, std::min(1.0,sc[2]));
}

inline void Vector::set(double x, double y, double z)
{
    sc[0] = x;
    sc[1] = y;
    sc[2] = z;
}

inline void Vector::set(int i, const double &v)
{
    sc[i] = v;
}

inline void Vector::setX(double x)
{
    sc[0] = x;
}

inline void Vector::setY(double y)
{
    sc[1] = y;
}

inline void Vector::setZ(double z)
{
    sc[2] = z;
}

inline void Vector::setR(double x)
{
    sc[0] = x;
}

inline void Vector::setG(double y)
{
    sc[1] = y;
}

inline void Vector::setB(double z)
{
    sc[2] = z;
}

inline double Vector::distance2(const Vector &v1, const Vector &v2)
{
    return pow_2(v1.sc[0]-v2.sc[0]) + 
           pow_2(v1.sc[1]-v2.sc[1]) + 
           pow_2(v1.sc[2]-v2.sc[2]);
}

inline double Vector::distance(const Vector &v1, const Vector &v2)
{
    return std::sqrt( pow_2(v1.sc[0]-v2.sc[0]) +
                      pow_2(v1.sc[1]-v2.sc[1]) +
                      pow_2(v1.sc[2]-v2.sc[2]) );
}

inline double Vector::invDistance(const Vector &v1, const Vector &v2)
{
    double dist = pow_2(v1.sc[0]-v2.sc[0]) +
                  pow_2(v1.sc[1]-v2.sc[1]) +
                  pow_2(v1.sc[2]-v2.sc[2]);
                  
    if (SireMaths::isZero(dist))
    {
        QString err = QObject::tr("Zero distance in Vector::invDistance! %1 and %2")
                          .arg(v1.toString(),v2.toString());
        throw SireMaths::math_error(err,CODELOC);
    }
    
    return double(1.0) / std::sqrt(dist);
}

inline double Vector::invDistance2(const Vector &v1, const Vector &v2)
{
    return double(1.0) / ( pow_2(v1.sc[0]-v2.sc[0]) +
                           pow_2(v1.sc[1]-v2.sc[1]) +
                           pow_2(v1.sc[2]-v2.sc[2]) );
}

inline bool Vector::operator==(const Vector &p1) const
{
    return p1.sc[0] == sc[0] and p1.sc[1] == sc[1] and p1.sc[2] == sc[2];
}

inline bool Vector::operator!=(const Vector &p1) const
{
    return p1.sc[0] != sc[0] or p1.sc[1] != sc[1] or p1.sc[2] != sc[2];
}

inline const double& Vector::operator[](unsigned int i) const
{
    return sc[i];
}

inline const Vector& Vector::operator+=(const Vector &other)
{
    for (int i=0; i<3; i++)
        sc[i] += other.sc[i];

    return *this;
}

inline const Vector& Vector::operator-=(const Vector &other)
{
    for (int i=0; i<3; i++)
        sc[i] -= other.sc[i];

    return *this;
}

inline const Vector& Vector::operator*=(const double &val)
{
    for (int i=0; i<3; i++)
        sc[i] *= val;

    return *this;
}

inline const Vector& Vector::operator/=(const double &val)
{
    if ( SireMaths::isZero(val) )
        throw SireMaths::math_error(QObject::tr(
            "Cannot divide a vector by zero! %1 / 0 is a error!").arg(this->toString()),CODELOC);
            
    for (int i=0; i<3; i++)
        sc[i] /= val;

    return *this;
}

inline Vector Vector::operator-() const
{
    return Vector(-sc[0],-sc[1],-sc[2]);
}

inline unsigned int Vector::count() const
{
    return 3;
}

inline const double& Vector::at(unsigned int i) const
{
    return this->operator[](i);
}

inline const Vector operator+(const Vector &p1, const Vector &p2)
{
    return Vector(p1.sc[0]+p2.sc[0], p1.sc[1]+p2.sc[1], p1.sc[2]+p2.sc[2]);
}

inline const Vector operator-(const Vector &p1, const Vector &p2)
{
    return Vector(p1.sc[0]-p2.sc[0], p1.sc[1]-p2.sc[1], p1.sc[2]-p2.sc[2]);
}

inline const Vector operator*(const Vector &p1, double c)
{
    return Vector(p1.sc[0]*c, p1.sc[1]*c, p1.sc[2]*c);
}

inline const Vector operator*(double c, const Vector &p1)
{
    return Vector(p1.sc[0]*c, p1.sc[1]*c, p1.sc[2]*c);
}

inline const Vector operator/(const Vector &p1, double c)
{
    if (isZero(c))
        throw SireMaths::math_error(QObject::tr(
            "Cannot divide a vector by zero! %1 / 0 is a error!").arg(p1.toString()),CODELOC);

    return Vector(p1.sc[0]/c, p1.sc[1]/c, p1.sc[2]/c);
}

inline double Vector::length() const
{
    return sqrt(sc[0]*sc[0] + sc[1]*sc[1] + sc[2]*sc[2]);
}

inline double Vector::length2() const
{
    return (sc[0]*sc[0]+sc[1]*sc[1]+sc[2]*sc[2]);
}

inline double Vector::invLength() const
{
    double lgth = length();
    
    if (SireMaths::isZero(lgth))
        throw SireMaths::math_error(QObject::tr(
            "Cannot take the inverse length of a zero vector, %1").arg(this->toString()),CODELOC);
    
    return double(1.0) / lgth;
}

inline double Vector::invLength2() const
{
    double lgth2 = length2();
    
    if (SireMaths::isZero(lgth2))
        throw SireMaths::math_error(QObject::tr(
            "Cannot take the inverse length of a zero vector, %1").arg(this->toString()),CODELOC);
    
    return double(1.0) / lgth2;
}

inline bool Vector::isZero() const
{
    return SireMaths::isZero(sc[0]) and SireMaths::isZero(sc[1]) and SireMaths::isZero(sc[2]);
}

inline Vector Vector::normalise() const
{
    double l = length2();
    
    if (SireMaths::isZero(l))
        throw SireMaths::math_error(QObject::tr(
            "Cannot normalise a zero vector, %1").arg(this->toString()),CODELOC);
    
    l = 1.0 / sqrt(l);
    return Vector(sc[0]*l,sc[1]*l,sc[2]*l);
}

inline double Vector::dot(const Vector &v0, const Vector &v1)
{
    return (v0.sc[0]*v1.sc[0] + v0.sc[1]*v1.sc[1] + v0.sc[2]*v1.sc[2]);
}

inline void Vector::setMax(const Vector &other)
{
    for (int i=0; i<3; i++)
        sc[i] = SIRE_MAX( other.sc[i], sc[i] );
}

inline void Vector::setMin(const Vector &other)
{
    for (int i=0; i<3; i++)
        sc[i] = SIRE_MIN( other.sc[i], sc[i] );
}    

inline Vector Vector::max(const Vector &other) const
{
    Vector v(*this);
    v.setMax(other);
    return v;
}

inline Vector Vector::min(const Vector &other) const
{
    Vector v(*this);
    v.setMin(other);
    return v;
}

inline Matrix Vector::metricTensor() const
{
    double x2 = sc[0]*sc[0];
    double y2 = sc[1]*sc[1];
    double z2 = sc[2]*sc[2];
    
    double xy = sc[0]*sc[1];
    double xz = sc[0]*sc[2];
    double yz = sc[1]*sc[2];

    return Matrix( y2 + z2, -xy, -xz,
                   -xy, x2 + z2, -yz,
                   -xz, -yz, x2 + y2 );
}

inline const Vector operator*(const Matrix &m, const Vector &p)
{
    return Vector(m.xx*p.sc[0] + m.yx*p.sc[1] + m.zx*p.sc[2],
                  m.xy*p.sc[0] + m.yy*p.sc[1] + m.zy*p.sc[2],
                  m.xz*p.sc[0] + m.yz*p.sc[1] + m.zz*p.sc[2]);
}

}

Q_DECLARE_METATYPE(SireMaths::Vector)
Q_DECLARE_TYPEINFO(SireMaths::Vector, Q_MOVABLE_TYPE);

SIRE_END_HEADER

#endif

