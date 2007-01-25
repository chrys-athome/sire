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

#ifndef SIREMATHS_MATRIX_H
#define SIREMATHS_MATRIX_H

#include <QString>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireMaths
{
class Matrix;
}

class QDataStream;
QDataStream& operator<<(QDataStream&, const SireMaths::Matrix&);
QDataStream& operator>>(QDataStream&, SireMaths::Matrix&);

namespace SireMaths
{

class Vector;
class Matrix4;

/**
This class represents a 3x3 square matrix, used to represent 3D transformations.

@author Christopher Woods
*/
class SIREMATHS_EXPORT Matrix
{

friend class Quaternion;
friend class Matrix4;

friend QDataStream& ::operator<<(QDataStream&, const Matrix&);
friend QDataStream& ::operator>>(QDataStream&, Matrix&);

public:
    Matrix(double sxx=1.0, double sxy=0.0, double sxz=0.0,
           double syx=0.0, double syy=1.0, double syz=0.0,
           double szx=0.0, double szy=0.0, double szz=1.0);

    Matrix(const Vector& c1, const Vector& c2, const Vector& c3);
    Matrix(double val);

    Matrix(const Matrix& m);

    ~Matrix();

    Matrix transpose() const;
    Matrix inverse() const;
    Vector trace() const;

    QString toString() const;

    bool isSymmetric() const;
    void enforceSymmetric();
    Matrix getPrincipalAxes() const;

    Vector column0() const;
    Vector column1() const;
    Vector column2() const;

    Vector row0() const;
    Vector row1() const;
    Vector row2() const;

    double determinant() const;

    void setToIdentity();

    bool isIdentity() const;

    static Matrix identity();
    static Matrix zero();

    bool operator==(const Matrix& m);
    bool operator!=(const Matrix& m);

    friend bool operator==(const Matrix &m1, const Matrix &m2);
    friend bool operator!=(const Matrix &m1, const Matrix &m2);

    Matrix& operator+=(const Matrix &m);
    Matrix& operator-=(const Matrix &m);
    Matrix& operator*=(const Matrix &m);
    Matrix& operator*=(double c);
    Matrix& operator/=(double c);

    friend const Matrix operator+(const Matrix &m1, const Matrix &m2);
    friend const Matrix operator-(const Matrix &m1, const Matrix &m2);
    friend const Matrix operator*(const Matrix &m1, const Matrix &m2);
    friend const Vector operator*(const Matrix &m, const Vector &p);
    friend const Matrix operator*(const Matrix &m, double c);
    friend const Matrix operator*(double c, const Matrix &m);

protected:
    /** The components of the matrix */

    //First column0, (xx,xy,xz)
    double xx,xy,xz;
    //Now column1, (yx,yy,yz)
    double yx,yy,yz;
    //Now column2, (zx,zy,zz)
    double zx,zy,zz;
};

/** Return the identiy matrix */
inline Matrix Matrix::identity()
{
    return Matrix();
}

/** Return the null matrix */
inline Matrix Matrix::zero()
{
    return Matrix(0.0,0.0,0.0, 0.0,0.0,0.0, 0.0,0.0,0.0 );
}

inline Matrix& Matrix::operator*=(const Matrix &m)
{
    double sxx = m.xx*xx + m.yx*xy + m.zx*xz;
    double syx = m.xx*yx + m.yx*yy + m.zx*yz;
    double szx = m.xx*zx + m.yx*zy + m.zx*zz;

    double sxy = m.xy*xx + m.yy*xy + m.zy*xz;
    double syy = m.xy*yx + m.yy*yy + m.zy*yz;
    double szy = m.xy*zx + m.yy*zy + m.zy*zz;

    double sxz = m.zx*xx + m.yz*xy + m.zz*xz;
    double syz = m.zx*yx + m.yz*yy + m.zz*yz;
    double szz = m.zx*zx + m.yz*zy + m.zz*zz;

    xx = sxx;
    xy = sxy;
    xz = sxz;

    yx = syx;
    yy = syy;
    yz = syz;

    zx = szx;
    zy = szy;
    zz = szz;

    return *this;
}

inline const Matrix operator*(const Matrix &m1, const Matrix &m2)
{
    double sxx = m1.xx*m2.xx + m1.yx*m2.xy + m1.zx*m2.xz;
    double syx = m1.xx*m2.yx + m1.yx*m2.yy + m1.zx*m2.yz;
    double szx = m1.xx*m2.zx + m1.yx*m2.zy + m1.zx*m2.zz;

    double sxy = m1.xy*m2.xx + m1.yy*m2.xy + m1.zy*m2.xz;
    double syy = m1.xy*m2.yx + m1.yy*m2.yy + m1.zy*m2.yz;
    double szy = m1.xy*m2.zx + m1.yy*m2.zy + m1.zy*m2.zz;

    double sxz = m1.zx*m2.xx + m1.yz*m2.xy + m1.zz*m2.xz;
    double syz = m1.zx*m2.yx + m1.yz*m2.yy + m1.zz*m2.yz;
    double szz = m1.zx*m2.zx + m1.yz*m2.zy + m1.zz*m2.zz;

    return Matrix(sxx,sxy,sxz,
                  syx,syy,syz,
                  szx,szy,szz);
}

}

Q_DECLARE_METATYPE(SireMaths::Matrix)
Q_DECLARE_TYPEINFO(SireMaths::Matrix, Q_MOVABLE_TYPE);

SIRE_END_HEADER

#endif
