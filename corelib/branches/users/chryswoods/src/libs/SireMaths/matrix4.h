/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
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

#ifndef SIREMATHS_MATRIX4_H
#define SIREMATHS_MATRIX4_H

#include <QString>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireMaths
{
class Matrix4;
}

class QDataStream;
QDataStream& operator<<(QDataStream&, const SireMaths::Matrix4&);
QDataStream& operator>>(QDataStream&, SireMaths::Matrix4&);

namespace SireMaths
{

class Vector;
class Matrix;
class Quaternion;

/**
This class represents a 4x4 square matrix.

@author Christopher Woods
*/
class SIREMATHS_EXPORT Matrix4
{

friend class Quaternion;

friend QDataStream& ::operator<<(QDataStream&, const Matrix4&);
friend QDataStream& ::operator>>(QDataStream&, Matrix4&);

public:
    Matrix4();

    Matrix4(double diagonal_value);

    Matrix4(double xx, double xy, double xz, double xw,
            double yx, double yy, double yz, double yw,
            double zx, double zy, double zz, double zw,
            double wx, double wy, double wz, double ww);

    Matrix4(const Matrix &m);

    Matrix4(const Matrix4& m);

    ~Matrix4();

    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<Matrix4>() );
    }
    
    const char* what() const
    {
        return Matrix4::typeName();
    }

    Matrix4 transpose() const;

    QString toString() const;

    Matrix4 getPrincipalAxes() const;

    static Matrix4 identity();
    static Matrix4 zero();

    bool operator==(const Matrix4& m);
    bool operator!=(const Matrix4& m);

    Matrix4& operator+=(const Matrix4 &m);
    Matrix4& operator-=(const Matrix4 &m);

    Matrix4 operator+(const Matrix4 &m) const;
    Matrix4 operator-(const Matrix4 &m) const;

    Quaternion column0() const;
    Quaternion column1() const;
    Quaternion column2() const;
    Quaternion column3() const;

protected:
    /** The components of the matrix */

    //First column0, (xx,xy,xz,xw)
    double xx,xy,xz,xw;
    //Now column1, (yx,yy,yz,yw)
    double yx,yy,yz,yw;
    //Now column2, (zx,zy,zz,zw)
    double zx,zy,zz,zw;
    //Now column3, (wx,wy,wz,ww)
    double wx,wy,wz,ww;
};

#ifndef SIRE_SKIP_INLINE_FUNCTIONS

/** Return the identiy matrix */
inline Matrix4 Matrix4::identity()
{
    return Matrix4();
}

/** Return the null matrix */
inline Matrix4 Matrix4::zero()
{
    return Matrix4(0.0,0.0,0.0,0.0, 0.0,0.0,0.0,0.0,
                   0.0,0.0,0.0,0.0, 0.0,0.0,0.0,0.0 );
}

#endif //SIRE_SKIP_INLINE_FUNCTIONS

}

Q_DECLARE_METATYPE(SireMaths::Matrix4)
Q_DECLARE_TYPEINFO(SireMaths::Matrix4, Q_MOVABLE_TYPE);

SIRE_EXPOSE_CLASS( SireMaths::Matrix4 )

SIRE_END_HEADER

#endif
