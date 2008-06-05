#ifndef SIREMATHS_QUATERNION_H
#define SIREMATHS_QUATERNION_H

class QString;

#include "maths.h"

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireMaths
{
class Quaternion;
}

class QDataStream;
QDataStream& operator<<(QDataStream&, const SireMaths::Quaternion&);
QDataStream& operator>>(QDataStream&, SireMaths::Quaternion&);

namespace SireMaths
{

class Vector;
class Matrix;
class Angle;

/**
This is a quaternion class that is used to handle 3D rotations and SLERP.
 
@author Christopher Woods
*/
class SIREMATHS_EXPORT Quaternion
{

friend QDataStream& ::operator<<(QDataStream&, const Quaternion&);
friend QDataStream& ::operator>>(QDataStream&, Quaternion&);

public:
    Quaternion();
    Quaternion(const Quaternion& p1);
    
    Quaternion(const Angle &angle, const Vector &axis);
    Quaternion(const Matrix &m);
    Quaternion(double x, double y, double z, double w);
    
    ~Quaternion();

    double x() const;
    double y() const;
    double z() const;
    double w() const;

    bool isIdentity() const;

    Quaternion inverse() const;
    Quaternion conjugate() const;

    double dot(const Quaternion &q) const;

    QString toString() const;
    static Quaternion fromString(const QString &str);

    Matrix toMatrix() const;
    void fromMatrix(const Matrix &m);
    Vector rotate(const Vector &p) const;

    Quaternion slerp(const Quaternion &q, double lambda) const;

    Quaternion pow(double n) const;

    static Quaternion identity();
    
    void renormalise();

    bool operator==(const Quaternion &p1);
    bool operator!=(const Quaternion &p1);

    friend bool operator==(const Quaternion &p1, const Quaternion &p2);
    friend bool operator!=(const Quaternion &p1, const Quaternion &p2);

    Quaternion& operator=(const Quaternion &p);

    friend const Quaternion operator+(const Quaternion &p1, const Quaternion &p2);
    friend const Quaternion operator-(const Quaternion &p1, const Quaternion &p2);
    friend const Quaternion operator*(const Quaternion &p1, const Quaternion &p2);
    friend const Quaternion operator*(const Quaternion &p1, const Vector &p2);
    friend const Quaternion operator*(const Vector &p1, const Quaternion &p2);

private:
    /** The x,y,z,w of the quaternion */
    double sc[4];

};

}

Q_DECLARE_METATYPE(SireMaths::Quaternion)
Q_DECLARE_TYPEINFO(SireMaths::Quaternion, Q_MOVABLE_TYPE);

SIRE_END_HEADER

#endif
