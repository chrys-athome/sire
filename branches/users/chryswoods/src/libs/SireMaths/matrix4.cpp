/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2007  Christopher Woods
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

#include <QString>
#include "matrix4.h"
#include "vector.h"
#include "matrix.h"
#include "maths.h"
#include "quaternion.h"

#include "SireMaths/errors.h"
#include "SireStream/datastream.h"

#include <gsl/gsl_eigen.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>

using namespace SireMaths;
using namespace SireStream;

static const RegisterMetaType<Matrix4> r_matrix4;

/** Serialise to a binary data stream */
QDataStream SIREMATHS_EXPORT &operator<<(QDataStream &ds, const Matrix4 &matrix)
{
    writeHeader(ds, r_matrix4, 1)
       << matrix.xx << matrix.xy << matrix.xz << matrix.xw
       << matrix.yx << matrix.yy << matrix.yz << matrix.yw
       << matrix.zx << matrix.zy << matrix.zz << matrix.zw
       << matrix.wx << matrix.wy << matrix.wz << matrix.ww;

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMATHS_EXPORT &operator>>(QDataStream &ds, Matrix4 &matrix)
{
    VersionID v = readHeader(ds, r_matrix4);

    if (v == 1)
    {
        ds >> matrix.xx >> matrix.xy >> matrix.xz >> matrix.xw
           >> matrix.yx >> matrix.yy >> matrix.yz >> matrix.yw
           >> matrix.zx >> matrix.zy >> matrix.zz >> matrix.zw
           >> matrix.wx >> matrix.wy >> matrix.wz >> matrix.ww;
    }
    else
        throw version_error(v, "1", r_matrix4, CODELOC);

    return ds;
}

/** Construct a default Matrix (identity matrix) */
Matrix4::Matrix4()
        : xx(1), xy(0), xz(0), xw(0),
          yx(0), yy(1), yz(0), yw(0),
          zx(0), zy(0), zz(1), zw(0),
          wx(0), wy(0), wz(0), ww(1)
{}

/** Construct a matrix whose diagonal values are equal to
    'diagonal_value' and whose off-diagonal values are zero */
Matrix4::Matrix4(double diagonal_value)
        : xx(diagonal_value), xy(0), xz(0), xw(0),
          yx(0), yy(diagonal_value), yz(0), yw(0),
          zx(0), zy(0), zz(diagonal_value), zw(0),
          wx(0), wy(0), wz(0), ww(diagonal_value)
{}

/** Construct a Matrix. Elements listed as column 1, then
    column 2, then column 3, then column 4 */
Matrix4::Matrix4(double sxx, double sxy, double sxz, double sxw,
                 double syx, double syy, double syz, double syw,
                 double szx, double szy, double szz, double szw,
                 double swx, double swy, double swz, double sww)
             : xx(sxx),xy(sxy),xz(sxz),xw(sxw),
               yx(syx),yy(syy),yz(syz),yw(syw),
               zx(szx),zy(szy),zz(szz),zw(szw),
               wx(swx),wy(swy),wz(swz),ww(sww)
{}

/** Construct a matrix4 from a matrix (the extra elements are 0 for columns0-2,
    and 0,0,0,1 for column 3 */
Matrix4::Matrix4(const Matrix &m)
        : xx(m.xx),xy(m.xy),xz(m.xz),xw(0.0),
          yx(m.yx),yy(m.yy),yz(m.yz),yw(0.0),
          zx(m.zx),zy(m.zy),zz(m.zz),zw(0.0),
          wx(0.0),wy(0.0),wz(0.0),ww(1.0)
{}

/** Copy constructor */
Matrix4::Matrix4(const Matrix4 &m)
        : xx(m.xx),xy(m.xy),xz(m.xz),xw(m.xw),
          yx(m.yx),yy(m.yy),yz(m.yz),yw(m.yw),
          zx(m.zx),zy(m.zy),zz(m.zz),zw(m.zw),
          wx(m.wx),wy(m.wy),wz(m.wz),ww(m.ww)
{}

/** Destructor */
Matrix4::~Matrix4()
{}

/** Return a QString representation of the matrix */
QString Matrix4::toString() const
{
    return QObject::tr(
        "( %1, %2, %3  %4 | %5, %6, %7  %8 | %9, %10, %11  %12 | %13, %14, %15, %16 )")
                          .arg(xx).arg(yx).arg(zx).arg(wx)
                          .arg(xy).arg(yy).arg(zy).arg(wy)
                          .arg(xz).arg(yz).arg(zz).arg(wz)
                          .arg(xw).arg(yw).arg(zw).arg(ww);
}

/** Return the transpose of the matrix */
Matrix4 Matrix4::transpose() const
{
    return Matrix4(xx,yx,zx,wx,
                   xy,yy,zy,wy,
                   xz,yz,zz,wz,
                   xw,yw,zw,ww);
}

bool Matrix4::operator==(const Matrix4& m)
{
    return xx==m.xx && xy==m.xy && xz==m.xz && xw==m.xw &&
           yx==m.yx && yy==m.yy && yx==m.yz && yw==m.yw &&
           zx==m.zx && zy==m.zy && zz==m.zz && zw==m.zw &&
           wx==m.wx && wy==m.wy && wz==m.wz && ww==m.ww;
}

bool Matrix4::operator!=(const Matrix4& m)
{
    return not (*this == m);
}

Matrix4& Matrix4::operator+=(const Matrix4 &m)
{
    xx += m.xx;
    xy += m.xy;
    xz += m.xz;
    xw += m.xw;

    yx += m.yx;
    yy += m.yy;
    yz += m.yz;
    yw += m.yw;

    zx += m.zx;
    zy += m.zy;
    zz += m.zz;
    zw += m.zw;

    wx += m.wx;
    wy += m.wy;
    wz += m.wz;
    ww += m.ww;

    return *this;
}

Matrix4& Matrix4::operator-=(const Matrix4 &m)
{
    xx -= m.xx;
    xy -= m.xy;
    xz -= m.xz;
    xw -= m.xw;

    yx -= m.yx;
    yy -= m.yy;
    yz -= m.yz;
    yw -= m.yw;

    zx -= m.zx;
    zy -= m.zy;
    zz -= m.zz;
    zw -= m.zw;

    wx -= m.wx;
    wy -= m.wy;
    wz -= m.wz;
    ww -= m.ww;

    return *this;
}

Matrix4 Matrix4::operator+(const Matrix4 &other) const
{
    Matrix4 ret = *this;
    ret += other;
    return ret;
}

Matrix4 Matrix4::operator-(const Matrix4 &other) const
{
    Matrix4 ret = *this;
    ret -= other;
    return ret;
}

Matrix4 convertGSLMatrix4(gsl_matrix *mat)
{
    return Matrix4( gsl_matrix_get(mat,0,0), gsl_matrix_get(mat,1,0),
                    gsl_matrix_get(mat,2,0), gsl_matrix_get(mat,3,0),

                    gsl_matrix_get(mat,0,1), gsl_matrix_get(mat,1,1),
                    gsl_matrix_get(mat,2,1), gsl_matrix_get(mat,3,1),

                    gsl_matrix_get(mat,0,2), gsl_matrix_get(mat,1,2),
                    gsl_matrix_get(mat,2,2), gsl_matrix_get(mat,3,2),

                    gsl_matrix_get(mat,0,3), gsl_matrix_get(mat,1,3),
                    gsl_matrix_get(mat,2,3), gsl_matrix_get(mat,3,3));
}

/** Obtain the principal axes of this matrix. This can only be performed if this
    matrix is symmetric. You should only call this function for matricies that
    you know are symmetric, as this function will assume that the matrix is
    symmetric, and will thus only use the upper-right diagonal of values.
    The returned principal axes will be sorted from the highest eigenvalue to
    the lowest. */
Matrix4 Matrix4::getPrincipalAxes() const
{
    //assume that this matrix is symmetrical - we will thus
    //only look at the values in the upper-right diagonal

    //we first need to copy the contents of this matrix into an array...
    double sym_mtx[16];

    sym_mtx[0] = xx;
    sym_mtx[1] = yx;
    sym_mtx[2] = zx;
    sym_mtx[3] = wx;

    sym_mtx[4] = yx;
    sym_mtx[5] = yy;
    sym_mtx[6] = zy;
    sym_mtx[7] = wy;

    sym_mtx[8]  = zx;
    sym_mtx[9]  = zy;
    sym_mtx[10] = zz;
    sym_mtx[11] = wz;

    sym_mtx[12] = wx;
    sym_mtx[13] = wy;
    sym_mtx[14] = wz;
    sym_mtx[15] = ww;

    //now use the GNU Scientific Library to solve the eigenvalue
    //problem for this matrix

    gsl_matrix_view m = gsl_matrix_view_array(sym_mtx,4,4);

    //allocate space for the resulting eigenvectors and eigenvalues
    gsl_vector *eig_val = gsl_vector_alloc(4);
    gsl_matrix *eig_vec = gsl_matrix_alloc(4,4);

    //now allocate some workspace for the calculation...
    gsl_eigen_symmv_workspace *w = gsl_eigen_symmv_alloc(4);

    //perform the calculation
    gsl_eigen_symmv(&m.matrix, eig_val, eig_vec, w);

    //free the space used by the calculation
    gsl_eigen_symmv_free(w);

    //now sort the eigenvectors from the smallest eigenvalue to
    //the largest
    gsl_eigen_symmv_sort (eig_val, eig_vec, GSL_EIGEN_SORT_ABS_ASC);

    //now copy the results back into a new Matrix
    Matrix4 ret = convertGSLMatrix4(eig_vec);

    //free up the memory used by the GSL data...
    gsl_vector_free(eig_val);
    gsl_matrix_free(eig_vec);

    //finally, return the matrix of principal components
    return ret;
}

Quaternion Matrix4::column0() const
{
    return Quaternion(xx,xy,xz,xw);
}

Quaternion Matrix4::column1() const
{
    return Quaternion(yx,yy,yz,yw);
}

Quaternion Matrix4::column2() const
{
    return Quaternion(zx,zy,zz,zw);
}

Quaternion Matrix4::column3() const
{
    return Quaternion(wx,wy,wz,ww);
}
