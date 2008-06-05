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
#include <boost/scoped_array.hpp>
#include "matrix.h"
#include "vector.h"
#include "maths.h"

#include "SireMaths/errors.h"
#include "SireStream/datastream.h"

#include <gsl/gsl_eigen.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>

using namespace SireMaths;
using namespace SireStream;

using boost::tuple;

static const RegisterMetaType<Matrix> r_matrix;

/** Serialise to a binary data stream */
QDataStream SIREMATHS_EXPORT &operator<<(QDataStream &ds, const Matrix &matrix)
{
    writeHeader(ds, r_matrix, 1) << matrix.xx << matrix.xy << matrix.xz
                                 << matrix.yx << matrix.yy << matrix.yz
                                 << matrix.zx << matrix.zy << matrix.zz;

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMATHS_EXPORT &operator>>(QDataStream &ds, Matrix &matrix)
{
    VersionID v = readHeader(ds, r_matrix);

    if (v == 1)
    {
        ds >> matrix.xx >> matrix.xy >> matrix.xz
           >> matrix.yx >> matrix.yy >> matrix.yz
           >> matrix.zx >> matrix.zy >> matrix.zz;
    }
    else
        throw version_error(v, "1", r_matrix, CODELOC);

    return ds;
}

/** Construct a default Matrix (identity matrix) */
Matrix::Matrix()
       : xx(1), xy(0), xz(0),
         yx(0), yy(1), yz(0),
         zx(0), zy(0), zz(1)
{}

/** Construct a matrix whose diagonal elements equal 'diagonal_value'
    and whose off-diagonal elements equal zero */
Matrix::Matrix(double diagonal_value)
       : xx(diagonal_value), xy(0), xz(0),
         yx(0), yy(diagonal_value), yz(0),
         zx(0), zy(0), zz(diagonal_value)
{}

/** Construct a Matrix. Elements listed as row 1, then
    row 2, then row 3. */
Matrix::Matrix(double sxx, double sxy, double sxz,
               double syx, double syy, double syz,
               double szx, double szy, double szz)
             : xx(sxx),xy(sxy),xz(sxz),
               yx(syx),yy(syy),yz(syz),
               zx(szx),zy(szy),zz(szz)
{}

/** Copy constructor */
Matrix::Matrix(const Matrix &m)
       : xx(m.xx),xy(m.xy),xz(m.xz),
         yx(m.yx),yy(m.yy),yz(m.yz),
         zx(m.zx),zy(m.zy),zz(m.zz)
{}

/** Construct a matrix from three vectors - each vector is a row */
Matrix::Matrix(const Vector &r1, const Vector &r2, const Vector &r3)
      : xx(r1.x()),xy(r1.y()),xz(r1.z()),
        yx(r2.x()),yy(r2.y()),yz(r2.z()),
        zx(r3.x()),zy(r3.y()),zz(r3.z())
{}

/** Construct a matrix from a tuple of three vectors - each
    vector is a row */
Matrix::Matrix(const tuple<Vector,Vector,Vector> &rows)
{
    const Vector &r1 = rows.get<0>();
    const Vector &r2 = rows.get<1>();
    const Vector &r3 = rows.get<2>();
    
    xx = r1.x(); xy = r1.y(); xz = r1.z();
    yx = r2.x(); yy = r2.y(); yz = r2.z();
    zx = r3.x(); zy = r3.y(); zz = r3.z();
}

/** Destructor */
Matrix::~Matrix()
{}

/** Return the determinant of the matrix */
double Matrix::determinant() const
{
    double d = xx*(yy*zz-zy*yz) +
               yx*(zy*xz-xy*zz) +
               zx*(xy*yz-yy*xz);
    return d;
}

/** Return a QString representation of the matrix */
QString Matrix::toString() const
{
    return QObject::tr("( %1, %2, %3 | %4, %5, %6 | %7, %8, %9 )")
                  .arg(xx).arg(yx).arg(zx).arg(xy).arg(yy).arg(zy).arg(xz).arg(yz).arg(zz);
}

/** Return the trace of the matrix */
Vector Matrix::trace() const
{
    return Vector(xx,yy,zz);
}

/** Return each column */
Vector Matrix::column0() const
{
    return Vector(xx,xy,xz);
}

/** Return each column */
Vector Matrix::column1() const
{
    return Vector(yx,yy,yz);
}

/** Return each column */
Vector Matrix::column2() const
{
    return Vector(zx,zy,zz);
}

/** Return each row */
Vector Matrix::row0() const
{
    return Vector(xx,yx,zx);
}

/** Return each row */
Vector Matrix::row1() const
{
    return Vector(xy,yy,zy);
}

/** Return each row */
Vector Matrix::row2() const
{
    return Vector(xz,yz,zz);
}

/** Return the transpose of the matrix */
Matrix Matrix::transpose() const
{
    return Matrix(xx,yx,zx,xy,yy,zy,xz,yz,zz);
}

/** Set the matrix to identity */
void Matrix::setToIdentity()
{
    xx = 1;
    yy = 1;
    zz = 1;

    xy = 0;
    xz = 0;
    yx = 0;
    yz = 0;
    zx = 0;
    zy = 0;
}

/** Return whether or not this matrix is equal to the identity matrix */
bool Matrix::isIdentity() const
{
    return xx == 1 and yy == xx and zz == xx and
           xy == 0 and xz == xy and yx == xy and
           yz == xy and zx == xy and zy == xy;

}

bool Matrix::operator==(const Matrix& m)
{
    return xx==m.xx && xy==m.xy && xz==m.xz &&
           yx==m.yx && yy==m.yy && yx==m.yz &&
           zx==m.zx && zy==m.zy && zz==m.zz;
}

bool Matrix::operator!=(const Matrix& m)
{
    return xx!=m.xx | xy!=m.xy | xz!=m.xz |
           yx!=m.yx | yy!=m.yy | yz!=m.yz |
           zx!=m.zx | zy!=m.zy | zz!=m.zz;
}

bool SIREMATHS_EXPORT SireMaths::operator==(const Matrix &m1, const Matrix &m2)
{
    return m1.xx==m2.xx && m1.xy==m2.xy && m1.xz==m2.xz &&
           m1.yx==m2.yx && m1.yy==m2.yy && m1.yx==m2.yz &&
           m1.zx==m2.zx && m1.zy==m2.zy && m1.zz==m2.zz;
}

bool SIREMATHS_EXPORT SireMaths::operator!=(const Matrix &m1, const Matrix &m2)
{
    return m1.xx!=m2.xx | m1.xy!=m2.xy | m1.xz!=m2.xz |
           m1.yx!=m2.yx | m1.yy!=m2.yy | m1.yz!=m2.yz |
           m1.zx!=m2.zx | m1.zy!=m2.zy | m1.zz!=m2.zz;
}

Matrix& Matrix::operator+=(const Matrix &m)
{
    xx += m.xx;
    xy += m.xy;
    xz += m.xz;

    yx += m.yx;
    yy += m.yy;
    yz += m.yz;

    zx += m.zx;
    zy += m.zy;
    zz += m.zz;

    return *this;
}

Matrix& Matrix::operator-=(const Matrix &m)
{
    xx -= m.xx;
    xy -= m.xy;
    xz -= m.xz;

    yx -= m.yx;
    yy -= m.yy;
    yz -= m.yz;

    zx -= m.zx;
    zy -= m.zy;
    zz -= m.zz;

    return *this;
}

Matrix& Matrix::operator*=(double c)
{
    xx *= c;
    xy *= c;
    xz *= c;

    yx *= c;
    yy *= c;
    yz *= c;

    zx *= c;
    zy *= c;
    zz *= c;

    return *this;
}

Matrix& Matrix::operator/=(double c)
{
    if (isZero(c))
        throw SireMaths::math_error(QObject::tr(
                            "Cannot divide a matrix by 0"),CODELOC);

    xx /= c;
    xy /= c;
    xz /= c;

    yx /= c;
    yy /= c;
    yz /= c;

    zx /= c;
    zy /= c;
    zz /= c;

    return *this;
}

const Matrix SIREMATHS_EXPORT SireMaths::operator+(const Matrix &m1, const Matrix &m2)
{
    return Matrix( m1.xx+m2.xx, m1.xy+m2.xy, m1.xz+m2.xz,
                   m1.yx+m2.yx, m1.yy+m2.yy, m1.yz+m2.yz,
                   m1.zx+m2.zx, m1.zy+m2.zy, m1.zz+m2.zz);
}

const Matrix SIREMATHS_EXPORT SireMaths::operator-(const Matrix &m1, const Matrix &m2)
{
    return Matrix( m1.xx-m2.xx, m1.xy-m2.xy, m1.xz-m2.xz,
                   m1.yx-m2.yx, m1.yy-m2.yy, m1.yz-m2.yz,
                   m1.zx-m2.zx, m1.zy-m2.zy, m1.zz-m2.zz);
}

const Matrix SIREMATHS_EXPORT SireMaths::operator*(const Matrix &m, double c)
{
    return Matrix(m.xx*c, m.xy*c, m.xz*c,
                  m.yx*c, m.yy*c, m.yz*c,
                  m.zx*c, m.zy*c, m.zz*c);
}

const Matrix SIREMATHS_EXPORT SireMaths::operator*(double c, const Matrix &m)
{
    return Matrix(m.xx*c, m.xy*c, m.xz*c,
                  m.yx*c, m.yy*c, m.yz*c,
                  m.zx*c, m.zy*c, m.zz*c);
}

/** Return the inverse of this matrix. Throws a math_error if this
    matrix cannot be inverted. */
Matrix Matrix::inverse() const
{
    //calculate the determinant of the matrix
    double det = this->determinant();

    //if the determinant is zero then this matrix cannot be inverted
    if (isZero(det))
    {
        throw SireMaths::math_error(QObject::tr(
                    "Matrix '%1' cannot be inverted!").arg(toString()),CODELOC);
    }

    //take the inverse of the determinant
    det = double(1.0) / det;

    //form the elements of the inverse matrix
    Matrix inv;

    inv.xx = det * (yy*zz - zy*yz);
    inv.xy = det * (xz*zy - zz*xy);
    inv.xz = det * (xy*yz - yy*xz);

    inv.yx = det * (zx*yz - yx*zz);
    inv.yy = det * (xx*zz - zx*xz);
    inv.yz = det * (xz*yx - yz*xx);

    inv.zx = det * (yx*zy - zx*yy);
    inv.zy = det * (xy*zx - xx*zy);
    inv.zz = det * (xx*yy - yx*xy);

    return inv;
}

/** Return whether or not this is a symmetric matrix */
bool Matrix::isSymmetric() const
{
    return ( zx == xz and yx == xy and yz == zy );
}

/** Ensure that this matrix is symmetric - this is done by copying the upper-right
    diagonal to the lower-left diagonal. Note that you should only really use this
    function on matricies that you know are symmetric, but may have lost some of
    their symmetry due to numerical rounding error */
void Matrix::enforceSymmetric()
{
    /**
         xx yx zx
         xy yy zy
         xz yz zz  **/

    xz = zx;
    xy = yx;
    yz = zy;
}

Matrix convertGSLMatrix(gsl_matrix *mat)
{
    return Matrix( gsl_matrix_get(mat,0,0), gsl_matrix_get(mat,1,0), gsl_matrix_get(mat,2,0),
                   gsl_matrix_get(mat,0,1), gsl_matrix_get(mat,1,1), gsl_matrix_get(mat,2,1),
                   gsl_matrix_get(mat,0,2), gsl_matrix_get(mat,1,2), gsl_matrix_get(mat,2,2) );
}

/** Obtain the principal axes of this matrix. This can only be performed if this
    matrix is symmetric. You should only call this function for matricies that
    you know are symmetric, as this function will assume that the matrix is
    symmetric, and will thus only use the upper-right diagonal of values.
    The returned principal axes will be sorted from the highest eigenvalue to
    the lowest. */
Matrix Matrix::getPrincipalAxes() const
{
    //assume that this matrix is symmetrical - we will thus
    //only look at the values in the upper-right diagonal

    //we first need to copy the contents of this matrix into an array...
    double *sym_mtx = new double[9];

    sym_mtx[0] = xx;
    sym_mtx[1] = yx;
    sym_mtx[2] = zx;

    sym_mtx[3] = yx;
    sym_mtx[4] = yy;
    sym_mtx[5] = zy;

    sym_mtx[6] = zx;
    sym_mtx[7] = zy;
    sym_mtx[8] = zz;

    //now use the GNU Scientific Library to solve the eigenvalue
    //problem for this matrix

    gsl_matrix_view m = gsl_matrix_view_array(sym_mtx,3,3);

    //allocate space for the resulting eigenvectors and eigenvalues
    gsl_vector *eig_val = gsl_vector_alloc(3);
    gsl_matrix *eig_vec = gsl_matrix_alloc(3,3);

    //now allocate some workspace for the calculation...
    gsl_eigen_symmv_workspace *w = gsl_eigen_symmv_alloc(3);

    //perform the calculation
    gsl_eigen_symmv(&m.matrix, eig_val, eig_vec, w);

    //free the space used by the calculation
    gsl_eigen_symmv_free(w);

    //now sort the eigenvectors from the smallest eigenvalue to
    //the largest
    gsl_eigen_symmv_sort (eig_val, eig_vec, GSL_EIGEN_SORT_ABS_ASC);

    //now copy the results back into a new Matrix
    Matrix ret = convertGSLMatrix(eig_vec);

    //free up the memory used by the GSL data...
    gsl_vector_free(eig_val);
    gsl_matrix_free(eig_vec);

    //free up the memory held by the copy of this matrix
    delete[] sym_mtx;

    //finally, return the matrix of principal components
    return ret;
}
