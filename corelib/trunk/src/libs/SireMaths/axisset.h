#ifndef SIREMATHS_AXISSET_H
#define SIREMATHS_AXISSET_H

#include <QString>

#include "matrix.h"
#include "vector.h"

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireMaths
{
class AxisSet;
}

class QDataStream;
QDataStream& operator<<(QDataStream&, const SireMaths::AxisSet&);
QDataStream& operator>>(QDataStream&, SireMaths::AxisSet&);

namespace SireMaths
{

/**
This class provides a complete set of orthonormal axes that provide a frame of reference (origin+axes) for a coordinate system. 
 
@author Christopher Woods
*/
class SIREMATHS_EXPORT AxisSet
{

friend QDataStream& ::operator<<(QDataStream&, const AxisSet&);
friend QDataStream& ::operator>>(QDataStream&, AxisSet&);

public:
    AxisSet();
    AxisSet(const Matrix &matrx, Vector orign = Vector());
    AxisSet(const AxisSet &other);
    ~AxisSet();

    QString toString() const;
    
    const Matrix& matrix() const;
    const Matrix& invMatrix() const;
    const Vector& origin() const;

    Vector fromIdentity(const Vector &vec) const;
    Vector toIdentity(const Vector &vec) const;
    Vector toFrame(const AxisSet &frame, const Vector &vec) const;
    Vector fromFrame(const AxisSet &frame, const Vector &vec) const;
    
protected:

    /** The matrix that represents the coordinate frame */
    Matrix mat;
    
    /** The inverse of 'mat' */
    Matrix invmat;
    
    /** The origin of this AxisSet */
    Vector orgn;
    
};

/** Returns a reference to the matrix representing this AxisSet */
inline const Matrix& AxisSet::matrix() const
{
    return mat;
}

/** Returns a reference to the inverse of the matrix representing this AxisSet */
inline const Matrix& AxisSet::invMatrix() const
{
    return invmat;
}

/** Returns a reference to the vector representing the origin of this AxisSet */
inline const Vector& AxisSet::origin() const
{
    return orgn;
}

}

Q_DECLARE_METATYPE(SireMaths::AxisSet);
Q_DECLARE_TYPEINFO(SireMaths::AxisSet, Q_MOVABLE_TYPE);

SIRE_END_HEADER

#endif
