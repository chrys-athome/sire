/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2009  Christopher Woods
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

#include "nmatrix.h"
#include "matrix.h"
#include "nvector.h"
#include "vector.h"

#include "sire_blas.h"
#include "sire_lapack.h"

#include "SireBase/array2d.hpp"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include "SireError/errors.h"

using namespace SireMaths;
using namespace SireBase;
using namespace SireStream;

static const RegisterMetaType<NMatrix> r_nmatrix;

/** Serialise to a binary datastream */
QDataStream SIREMATHS_EXPORT &operator<<(QDataStream &ds, const NMatrix &matrix)
{
    writeHeader(ds, r_nmatrix, 1);
    
    SharedDataStream sds(ds);
    
    sds << matrix.array << matrix.nrows << matrix.ncolumns << matrix.is_transpose;
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMATHS_EXPORT &operator>>(QDataStream &ds, NMatrix &matrix)
{
    VersionID v = readHeader(ds, r_nmatrix);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> matrix.array >> matrix.nrows >> matrix.ncolumns >> matrix.is_transpose;
    }
    else
        throw version_error(v, "1", r_nmatrix, CODELOC);
        
    return ds;
}

/** Null constructor */
NMatrix::NMatrix() : nrows(0), ncolumns(0), is_transpose(false)
{}

/** Construct a matrix with 'nrows' rows and 'ncolumns' columns.
    The values in the matrix are not initialised */
NMatrix::NMatrix(int nr, int nc)
        : nrows(nr), ncolumns(nc), is_transpose(false)
{
    if (nr <= 0 or nc <= 0)
    {
        nrows = 0;
        ncolumns = 0;
    }
    else
    {
        array = QVector<double>(nrows*ncolumns);
        array.squeeze();
    }
}

/** Construct a matrix with 'nrows' rows and 'ncolumns' columns.
    The values in the matrix are initialised to 'initial_value' */
NMatrix::NMatrix(int nrows, int ncolumns, double initial_value)
        : nrows(nr), ncolumns(nc), is_transpose(false)
{
    if (nr <= 0 or nc <= 0)
    {
        nrows = 0;
        ncolumns = 0;
    }
    else
    {
        array = QVector<double>(nrows*ncolumns, initial_value);
        array.squeeze();
    }
}

/** Construct from the passed Matrix */
NMatrix::NMatrix(const Matrix &matrix)
        : nrows(3), ncolumns(3), is_transpose(false)
{
    array = QVector<double>(9);
    array.squeeze();
    
    double *data = array.data();
    
    qMemCopy(data, matrix.column(0).constData(), 3*sizeof(double));
    qMemCopy(data+3, matrix.column(1).constData(), 3*sizeof(double));
    qMemCopy(data+6, matrix.column(2).constData(), 3*sizeof(double));
}

/** Construct from the passed Array */
NMatrix::NMatrix(const SireBase::Array2D<double> &array2d)
        : nrows(array.nRows()), ncolumns(array.nColumns()), is_transpose(true)
{
    int sz = array2d.nRows() * array2d.nColumns();

    if (sz > 0)
    {
        array = QVector<double>(sz);
        array.squeeze();
        
        qMemCopy(array.data(), array2d.constData(), sz*sizeof(double));
    }
}

/** Construct from the passed vector - this is copied to a column
    matrix, unless 'transpose' is true, in which case this is 
    copied as a row matrix */
NMatrix::NMatrix(const Vector &vector, bool transpose)
        : is_transpose(false)
{
    array = QVector<double>(3);
    array.squeeze();
    
    qMemCopy(array.data(), vector.constData(), 3*sizeof(double));
    
    if (transpose)
    {
        nrows = 1;
        ncolumns = 3;
    }
    else
    {
        nrows = 3;
        ncolumns = 1;
    }
}

/** Construct from the passed vector - this is copied to a column
    matrix, unless 'transpose' is true, in which case this is 
    copied as a row matrix */
NMatrix::NMatrix(const NVector &vector, bool transpose)
        : nrows(0), ncolumns(0), is_transpose(false)
{
    if (vector.count() > 0)
    {
        array = QVector<double>(vector.count());
        array.squeeze();
        
        qMemCopy(array.data(), vector.constData(), vector.count()*sizeof(double));

        if (transpose)
        {
            nrows = 1;
            ncolumns = vector.count();
        }
        else
        {
            nrows = vector.count();
            ncolumns = 1;
        }
    }
}

/** Copy constructor */
NMatrix::NMatrix(const NMatrix &other)
        : array(other.array), nrows(other.nrows),
          ncolumns(other.ncolumns), is_transpose(other.is_transpose)
{}

/** Destructor */
NMatrix::~NMatrix()
{}

/** Copy assignment operator */
NMatrix& NMatrix::operator=(const NMatrix &other)
{
    if (this != &other)
    {
        array = other.array;
        nrows = other.nrows;
        ncolumns = other.ncolumns;
        is_transpose = other.is_transpose;
    }

    return *this;
}

/** Comparison operator */
bool NMatrix::operator==(const NMatrix &other) const
{
    return this == &other or
           (nrows == other.nrows and ncolumns == other.ncolumns and
            is_transpose == other.is_transpose and
            array == other.array);
}

/** Comparison operator */
bool NMatrix::operator!=(const NMatrix &other) const
{
    return not this->operator==(other);
}

/** Assert that the index [i,j] is valid for this matrix

    \throw SireError::invalid_index
*/
void NMatrix::assertValidIndex(int i, int j) const
{
    if ( i < 0 or i >= nrows or j < 0 or j >= ncolumns )
    {
        throw SireError::invalid_index( QObject::tr(
            "The index [%1,%2] is invalid for the matrix with dimension [%1,%2].")
                .arg(i).arg(j).arg(nrows).arg(ncolumns), CODELOC );
    }
}

/** Assert that this is a square matrix 

    \throw SireError::incompatible_error
*/
void NMatrix::assertSquare() const
{
    if (nrows == 0 or ncolumns == 0)
        throw SireError::incompatible_error( QObject::tr(
            "The null matrix of zero dimension is not a square matrix!"), CODELOC );
            
    else if (nrows != ncolumns)
        throw SireError::incompatible_error( QObject::tr(
            "The operation is only compatible with square matricies. This "
            "matrix, of dimension [%1,%2], is not a square matrix.")
                .arg(nrows).arg(ncolumns), CODELOC );
}

/** Return a modifiable reference to the value at [i,j]

    \throw SireError::invalid_index
*/
double& NMatrix::operator()(int i, int j)
{
    this->assertValidIndex(i,j);
    
    return array.data()[ this->offset(i,j) ];
}

/** Return a reference to the value at [i,j]

    \throw SireError::invalid_index
*/
const double& NMatrix::operator()(int i, int j) const
{
    this->assertValidIndex(i,j);
    
    return array.data()[ this->offset(i,j) ];
}

/** Assert that this matrix has 'nrows' rows

    \throw SireError::incompatible_error
*/
void NMatrix::assertNRows(int nr) const
{
    if (nr != nrows)
        throw SireError::incompatible_error( QObject::tr(
                "The number of rows in this matrix (dimension [%1,%2]) is not "
                "equal to %3.")
                    .arg(nrows).arg(ncolumns).arg(nr), CODELOC );
}

/** Assert that this matrix has 'ncolumns' columns

    \throw SireError::incompatible_error
*/
void NMatrix::assertNColumns(int nc) const
{
    if (nc != ncolumns)
        throw SireError::incompatible_error( QObject::tr(
                "The number of columns in this matrix (dimension [%1,%2]) is not "
                "equal to %3.")
                    .arg(nrows).arg(ncolumns).arg(nc), CODELOC );
}

/** Return the transpose of this matrix. This is fast, as this
    just toggles a flag to say whether or not the transpose is
    to be used. If you want to fully transpose the data (e.g.
    if you want to directly access the data) the call 'fullTranspose()' */
NMatrix NMatrix::transpose() const
{
    //   / a b c \      / a d g j \
    //   | d e f |  =>  | b e h k |
    //   | g h i |      \ c f i l /
    //   \ j k l /
    
    // in memory stored as column-major
    // (a d g j b e h k c f i l)
    //
    //  offset(i,j)   == i + (j*nrows)     [2,1] => 2+4 == 6 == 'h'
    //  offset(i,j)^T == (i*ncolumns) + j  [1,2] => 4+2 == 6 == 'h'

    NMatrix ret(*this);
    ret.is_transpose = not is_transpose;
    ret.nrows = ncolumns;
    ret.ncolumns = nrows;
    
    return ret;
}

/** Fully transpose the data of this matrix */
NMatrix NMatrix::fullTranspose() const
{
    NMatrix ret(*this);
    ret.is_transpose = false;
    ret.ncolumns = nrows;
    ret.nrows = ncolumns;

    if (not is_transpose)
    {
        //the data needs to be transposed
        double *ret_data = ret.array.data();
        double *this_data = array.constData();
        
        for (qint32 i=0; i<nrows; ++i)
        {
            for (qint32 j=0; j<ncolumns; ++j)
            {
                ret_data[ ret.offset(j,i) ] = this_data[ offset(i,j) ];
            }
        }
    }
     
    return ret;   
}

/** Matrix addition 

    \throw SireError::incompatible_error
*/
NMatrix& NMatrix::operator+=(const NMatrix &other)
{
    assertNRows(other.nRows());
    assertNColumns(other.nColumns());
    
    if (is_transpose == other.is_transpose)
    {
        double *data = array.data();
        const double *other_data = other.array.constData();
        const int sz = array.count();
    
        for (int i=0; i<sz; ++i)
        {
            data[i] += other_data[i];
        }
    }
    else
    {
        this->operator+=(other.fullTranspose());
    }
    
    return *this;
}

/** Matrix subtraction 

    \throw SireError::incompatible_error
*/
NMatrix& NMatrix::operator-=(const NMatrix &other)
{
    assertNRows(other.nRows());
    assertNColumns(other.nColumns());
    
    if (is_transpose == other.is_transpose)
    {
        double *data = array.data();
        const double *other_data = other.array.constData();
        const int sz = array.count();
    
        for (int i=0; i<sz; ++i)
        {
            data[i] -= other_data[i];
        }
    }
    else
    {
        this->operator-=(other.fullTranspose());
    }
    
    return *this;
}

/** Multiply all elements of this matrix by 'scale' */
NMatrix& NMatrix::operator*=(double scale)
{
    double *data = array.data();
    const int sz = array.count();

    if (scale == 0)
    {
        for (int i=0; i<sz; ++i)
        {
            data[i] = 0;
        }
    }
    else
    {
        for (int i=0; i<sz; ++i)
        {
            data[i] *= sz;
        }
    }
    
    return *this;
}

/** Divide all elements of this matrix by 'scale' */
NMatrix& NMatrix::operator/=(double scale)
{
    if (scale == 0)
        throw SireMaths::domain_error( QObject::tr(
            "This code does not support dividing a matrix by zero!"), CODELOC );
            
    return this->operator*=( 1/scale );
}

/** Matrix multiplication - this uses dgemm under the hood
    for speed 
    
    \throw SireError::incompatible_error
*/  
NMatrix& NMatrix::operator*=(const NMatrix &other)
{
    if (other.nRows() == 1 and other.nColumns() == 1)
        return this->operator*=(other(0,0));
        
    this->operator=( dgemm(*this, other) );
    
    return *this;
}

/** Return the inverse of this matrix
    
    This uses LAPACK under the hood, for speed
    
    \throw SireError::incompatible_error
    \throw SireMaths::domain_error
*/
NMatrix NMatrix::inverse() const
{
    this->assertSquare();
    NMatrix factors = dgeco(*this);
    return dgedi_inverse(factors);
}

/** Matrix division - this multiplies this matrix with the inverse of 'other' 

    \throw SireMaths::domain_error
    \throw SireError::incompatible_error
*/
NMatrix& NMatrix::operator/=(const NMatrix &other)
{
    return this->operator*=(other.inverse());
}

/** Return the negative of this matrix */
NMatrix NMatrix::operator-() const
{
    NMatrix ret(*this);

    const int sz = array.count();
    double *ret_data = ret.array.data();
    
    for (int i=0; i<sz; ++i)
    {
        ret_data[i] = -ret_data[i];
    }
    
    return ret;
}

/** Matrix addition 

    \throw SireError::incompatible_error
*/
NMatrix NMatrix::operator+(const NMatrix &other) const
{
    NMatrix ret(*this);
    ret += other;
    
    return ret;
}

/** Matrix subtraction 

    \throw SireError::incompatible_error
*/
NMatrix NMatrix::operator-(const NMatrix &other) const
{
    NMatrix ret(*this);
    ret -= other;
    
    return ret;
}

/** Matrix multiplication - this uses dgemm under the hood
    for speed 
    
    \throw SireError::incompatible_error
*/  
NMatrix NMatrix::operator*(const NMatrix &other) const
{
    NMatrix ret(*this);
    
    ret *= other;
    
    return ret;
}

/** Matrix division - this multiplies this matrix with the inverse of 'other' 

    \throw SireMaths::domain_error
    \throw SireError::incompatible_error
*/
NMatrix NMatrix::operator/(const NMatrix &other) const
{
    NMatrix ret(*this);
    
    ret /= other;
    
    return ret;
}

/** Multiply all elements of this matrix by 'scale' */
NMatrix NMatrix::operator*(double scale) const
{
    NMatrix ret(*this);
    
    ret *= scale;
    
    return ret;
}

/** Divide all elements of this matrix by 'scale' */
NMatrix NMatrix::operator/(double scale) const
{
    NMatrix ret(*this);
    
    ret /= scale;
    
    return ret;
}

/** Perform matrix-vector multiplication - the number of 
    rows of the vector must equal to the number of rows
    of this matrix

    This uses dgemv under the hood for speed

    \throw SireError::incompatible_error
*/
NVector NMatrix::operator*(const NVector &vector) const
{
    return dgemv(*this, vector);
}

/** Perform matrix-vector multiplication - the number of 
    rows of the vector must equal to the number of rows
    of this matrix

    This uses dgemv under the hood for speed

    \throw SireError::incompatible_error
*/
NVector NMatrix::operator*(const Vector &vector) const
{
    if (this->nRows() == 3 and this->nColumns() == 3)
    {
        //use hand-written code
        const double *d = array.constData();
        
        if (is_transpose)
        {
            Matrix m( d[0], d[1], d[2],
                      d[3], d[4], d[5],
                      d[6], d[7], d[8] );   // Matrix uses row-major ordering
                      
            return m*vector;
        }
        else
        {
            Matrix m( d[0], d[3], d[6],
                      d[1], d[4], d[7],
                      d[2], d[5], d[8] );   // Matrix uses row-major ordering
    
            return m*vector;
        }
    }
    else
    {
        this->assertNColumns(3);
        
        NVector v(nrows);
        
        const double *d = array.data();
        
        for (int i=0; i<nrows; ++i)
        {
            double sum = 0;
            
            for (int j=0; j<3; ++i)
            {
                sum += vector[j]*d[offset(i,j)]
            }
        }
        
        return v;
    }
}

/** Return the number of rows in this matrix */
int NMatrix::nRows() const
{
    return nrows;
}

/** Return the number of columns in this matrix */
int NMatrix::nColumns() const
{
    return ncolumns;
}

/** Assert that there is an ith row! 

    \throw SireError::invalid_index
*/
void NMatrix::assertValidRow(int i) const
{
    if (i < 0 or i >= nrows)
        throw SireError::invalid_index( QObject::tr(
                "The matrix with dimension [%1,%2] does not have a row with index %3.")
                    .arg(nrows).arg(ncolumns).arg(i), CODELOC );
}

/** Assert that there is an jth column! 

    \throw SireError::invalid_index
*/
void NMatrix::assertValidColumn(int j) const
{
    if (j < 0 or j >= ncolumns)
        throw SireError::invalid_index( QObject::tr(
                "The matrix with dimension [%1,%2] does not have a column with index %3.")
                    .arg(nrows).arg(ncolumns).arg(j), CODELOC );
}

/** Return a vector containing the contents of the ith row 

    \throw SireError::invalid_index
*/
NVector NMatrix::row(int i) const
{
    this->assertValidRow(i);

    NVector v(ncolumns);
    
    if (is_transpose)
    {
        //row-major storage
        qMemCopy( v.data(), array.constData()+i*ncolumns, ncolumns*sizeof(double) );
    }
    else
    {
        //column-major storage
        double *d = v.data();
        const double *row = array.constData();
        
        for (int j=0; j<ncolumns; ++j)
        {
            d[j] = row[ j*nrows + i ];
        }
    }

    return v;
}

/** Return a vector containing the contents of the ith row 

    \throw SireError::invalid_index
*/
NVector NMatrix::column(int j) const
{
    this->assertValidColumn(j);

    NVector v(nrows);
    
    if (is_transpose)
    {
        //row-major storage
        double *d = v.data();
        const double *column = array.constData();
        
        for (int i=0; i<nrows; ++i)
        {
            d[i] = row[ i*ncolumns + j ];
        }
    }
    else
    {
        //column-major storage
        qMemCopy( v.data(), array.constData()+j*nrows, nrows*sizeof(double) );
    }

    return v;
}

/** Set the value of [i,j] to 'value'

    \throw SireError::invalid_index
*/
void NMatrix::set(int i, int j, double value)
{
    array.data()[offset(i,j)] = value;
}

/** Set the values of all data in the row 'i' to 'value'

    \throw SireError::invalid_index
*/
void NMatrix::setRow(int i, double value)
{
    this->assertValidRow(i);
    
    double *d = array.data();
    
    for (int j=0; j<ncolumns; ++j)
    {
        d[offset(i,j)] = value;
    }
}

/** Copy the vector 'row' to row 'i'

    \throw SireError::invalid_index
    \throw SireError::incompatible_error
*/
void NMatrix::setRow(int i, const NVector &row)
{
    this->assertValidRow(i);
    this->assertNColumns(row.count());
    
    double *d = array.data();
    const double *v = row.constData();
    
    for (int j=0; j<ncolumns; ++j)
    {
        d[offset(i,j)] = v[j];
    }
}

/** Set the values of all data in the column 'j' to 'value'

    \throw SireError::invalid_index
*/
void NMatrix::setColumn(int j, double value)
{
    this->assertValidColumn(j);
    
    double *d = array.data();
    
    for (int i=0; i<nrows; ++i)
    {
        d[offset(i,j)] = value;
    }
}

/** Copy the vector 'column' to column 'j'

    \throw SireError::invalid_index
    \throw SireError::incompatible_error
*/
void NMatrix::setColumn(int j, const NVector &column)
{
    this->assertValidColumn(j);
    this->assertNRows(column.count());
    
    double *d = array.data();
    const double *v = column.constData();
    
    for (int i=0; i<nrows; ++i)
    {
        d[offset(i,j)] = v[i];
    }
}

/** Set all entries in the matrix to the value 'value' */
void NMatrix::setAll(double value)
{
    double *d = array.data();
    int sz = array.count();
    
    for (int i=0; i<sz; ++i)
    {
        d[i] = value;
    }
}

/** Return a raw pointer to the data of this matrix. The data is
    stored in column-major order (same as Fortran - not same as C++ or C).
    To be safe, use the 'offset' function to get the offset of 
    the value at [i,j] in this array */
double* NMatrix::data()
{
    return array.data();
}

/** Return a raw pointer to the data of this matrix. The data is
    stored in column-major order (same as Fortran - not same as C++ or C).
    To be safe, use the 'offset' function to get the offset of 
    the value at [i,j] in this array */
const double* NMatrix::data() const
{
    return array.constData();
}

/** Return a raw pointer to the data of this matrix. The data is
    stored in column-major order (same as Fortran - not same as C++ or C).
    To be safe, use the 'offset' function to get the offset of 
    the value at [i,j] in this array */
const double* NMatrix::constData() const
{
    return array.constData();
}

/** Calculate the offset in the 1D array of the value
    at index [i,j]
    
    \throw SireError::invalid_index
*/
int NMatrix::checkedOffset(int i, int j) const
{
    this->assertValidIndex(i,j);
    return this->offset(i,j);
}

/** Return a string representation of this matrix */
QString NMatrix::toString() const
{
    if (nrows == 0)
        return "( )";
        
    else if (nrows == 1)
    {
        const double *d = array.constData();
    
        QStringList row;
        for (qint32 j=0; j<ncolumns; ++j)
        {
            row.append( QString::number(d[j], 'g', 6) );
        }
        
        return QString("( %1 )").arg( row.join(", ") );
    }

    QStringList rows;
    
    const double *d = array.constData();
    
    for (qint32 i=0; i<nrows; ++i)
    {
        QStringList row;
        
        for (qint32 j=0; j<ncolumns; ++j)
        {
            row.append( QString::number(d[offset(i,j)], 'g', 6) );
        }
        
        if (i == 0)
            rows.append( QString("/ %1 \\").arg( row.join(", ") ) );
        else if (i == quint32(this->nRows() - 1))
            rows.append( QString("\\ %1 /").arg( row.join(", ") ) );
        else
            rows.append( QString("| %1 |").arg( row.join(", ") ) );
    }
    
    return rows.join("\n");
}

/** Return the determinant of this matrix

    This uses LAPACK under the hood, for speed

    \throw SireError::incompatible_error
*/
double NMatrix::determinant() const
{
    this->assertSquare();
    
    NMatrix factors = dgeco(*this);
    return dgedi_determinant(factors);
}

/** Return the trace of this matrix - this is only valid for a square matrix

    \throw SireError::incompatible_error
*/
double NMatrix::trace() const
{
    this->assertSquare();
    
    const double *d = array.constData();
    double sum = 0;
    
    for (int i=0; i<nrows; ++i)
    {
        sum += d[i*nrows + i];
    }
    
    return sum;
}
    
/** Return a vector containing the diagonal of this matrix - this is only
    valid for a square matrix
    
    \throw SireError::incompatible_error
*/
NVector NMatrix::diagonal() const
{
    this->assertSquare();
    
    const double *d = array.constData();

    NVector vector(nrows);
    double *v = vector.data();
    
    for (int i=0; i<nrows; ++i)
    {
        v[i] = d[i*nrows + i];
    }
    
    return vector;
}

/** Return the eigenvalues and eigenvectors of this matrix. This
    uses LAPACK under the hood for speed

    \throw SireError::incompatible_error
    \throw SireMaths::domain_error
*/
std::pair<NVector,NMatrix> NMatrix::diagonalise() const
{
    this->assertSquare();
    return dsyev(*this);
}
