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

#ifndef SIREMATHS_NMATRIX_H
#define SIREMATHS_NMATRIX_H

#include <QVector>

#include <utility>

#include "sireglobal.h"

#include "Siren/object.h"
#include "Siren/mutable.h"

SIRE_BEGIN_HEADER

namespace SireBase
{
template<class T>
class Array2D;

template<class T>
class TrigArray2D;
}

namespace SireMaths
{

class Matrix;
class Vector;
class NVector;
class TrigMatrix;

/** This is a dense, double, general N*M 2-dimensional matrix.
    The data is stored in column-major order, suitable for
    use with Fortran BLAS or LAPACK functions. This is 
    designed for high speed.
    
    The data is implicitly shared (copy on write), so 
    copying a matrix is very fast.
    
    @author Christopher Woods
*/
class SIREMATHS_EXPORT NMatrix 
        : public Siren::Implements<NMatrix,Siren::Object>,
          public Siren::Interfaces<NMatrix,Siren::Mutable>
{
public:
    NMatrix();
    
    NMatrix(int nrows, int ncolumns);
    NMatrix(int nrows, int ncolumns, double initial_value);
    
    NMatrix(const Matrix &matrix);
    NMatrix(const SireBase::Array2D<double> &array);
    NMatrix(const QVector< QVector<double> > &array);

    NMatrix(const Vector &vector, bool transpose=false);
    NMatrix(const QVector<double> &vector, bool transpose=false);
    NMatrix(const NVector &vector, bool transpose=false);    
    
    NMatrix(const TrigMatrix &matrix);
    
    NMatrix(const NMatrix &other);
    
    ~NMatrix();
    
    static NMatrix createRowMajor(int nrows, int ncolumns);
    static NMatrix createColumnMajor(int nrows, int ncolumns);
    
    NMatrix& operator=(const NMatrix &other);
    
    bool operator==(const NMatrix &other) const;
    bool operator!=(const NMatrix &other) const;
    
    double& operator()(int i, int j);
    const double& operator()(int i, int j) const;
    
    NMatrix& operator+=(const NMatrix &other);
    NMatrix& operator-=(const NMatrix &other);
    NMatrix& operator*=(const NMatrix &other);
    NMatrix& operator/=(const NMatrix &other);
    
    NMatrix& operator*=(double scale);
    NMatrix& operator/=(double scale);

    NMatrix operator-() const;
    
    NMatrix operator+(const NMatrix &other) const;
    NMatrix operator-(const NMatrix &other) const;
    NMatrix operator*(const NMatrix &other) const;
    NMatrix operator/(const NMatrix &other) const;
    
    NMatrix operator*(double scale) const;
    NMatrix operator/(double scale) const;
    
    NVector operator*(const NVector &vector) const;
    NVector operator*(const Vector &vector) const;
    
    static QString typeName();
    
    QString toString() const;
    uint hashCode() const;
    void stream(Siren::Stream &s);
    
    int nRows() const;
    int nColumns() const;
    
    NVector row(int i) const;
    NVector column(int j) const;
    
    void set(int i, int j, double value);
    
    void setRow(int i, double value);
    void setRow(int i, const NVector &row);
    
    void setColumn(int j, double value);
    void setColumn(int j, const NVector &column);
    
    void setAll(double value);
    
    double* data();
    const double* data() const;
    const double* constData() const;
    
    QVector<double> memory() const;
    
    int offset(int i, int j) const;
    int checkedOffset(int i, int j) const;
    
    void redimension(int nrows, int ncolumns);

    void reflectTopToBottom();
    void reflectBottomToTop();
    void reflectLeftToRight();
    void reflectRightToLeft();

    void reflectTopLeftToBottomRight();
    void reflectTopRightToBottomLeft();
    void reflectBottomRightToTopLeft();
    void reflectBottomLeftToTopRight();

    double determinant() const;
    double trace() const;
    
    NVector diagonal() const;
    
    std::pair<NVector,NMatrix> diagonalise() const;

    NMatrix inverse() const;
    
    NMatrix transpose() const;
    NMatrix fullTranspose() const;

    bool isTransposed() const;

    void assertValidIndex(int i, int j) const;
    
    void assertValidRow(int i) const;
    void assertValidColumn(int j) const;
    
    void assertNRows(int nrows) const;
    void assertNColumns(int ncolumns) const;

    void assertSquare() const;

protected:
    friend class Siren::Implements<NMatrix,Siren::Object>;
    static QStringList listInterfaces()
    {
        return Siren::Interfaces<NMatrix,Siren::Mutable>::listInterfaces();
    }

private:
    /** The raw data for the matrix */
    QVector<double> array;
    
    /** The number of rows and columns in the matrix */
    qint32 nrows, ncolumns;
    
    /** Whether the transpose of this matrix is stored */
    bool is_transpose;
};

#ifndef SIRE_SKIP_INLINE_FUNCTIONS

/** Return the offset into the 1D array for the value
    at index [i,j] - note that this performs *NO* checking,
    and invalid input will result in invalid output. If you
    want to check the indicies, use checkOffset(int i, int j) */
inline int NMatrix::offset(int i, int j) const
{
    if (is_transpose)
        return (i*ncolumns) + j;
    else
        return i + (j*nrows);
}

#endif // SIRE_SKIP_INLINE_FUNCTIONS

}

#include "SireBase/array2d.hpp"
#include "SireBase/trigarray2d.hpp"

#include "vector.h"
#include "matrix.h"

Q_DECLARE_METATYPE( SireMaths::NMatrix )

Q_DECLARE_METATYPE( SireBase::Array2D<SireMaths::Vector> )
Q_DECLARE_METATYPE( SireBase::Array2D<SireMaths::Matrix> )

Q_DECLARE_METATYPE( SireBase::TrigArray2D<SireMaths::Vector> )
Q_DECLARE_METATYPE( SireBase::TrigArray2D<SireMaths::Matrix> )

SIRE_EXPOSE_CLASS( SireMaths::NMatrix )

SIRE_EXPOSE_ALIAS( SireBase::Array2D<SireMaths::Vector>, SireBase::Array2D_Vector_ )
SIRE_EXPOSE_ALIAS( SireBase::Array2D<SireMaths::Matrix>, SireBase::Array2D_Matrix_ )

SIRE_EXPOSE_ALIAS( SireBase::TrigArray2D<SireMaths::Vector>, 
                   SireBase::TrigArray2D_Vector_ )
SIRE_EXPOSE_ALIAS( SireBase::TrigArray2D<SireMaths::Matrix>, 
                   SireBase::TrigArray2D_Matrix_ )

#ifdef SIRE_INSTANTIATE_TEMPLATES
template class SireBase::Array2D<SireMaths::Vector>;
template class SireBase::Array2D<SireMaths::Matrix>;
template class SireBase::TrigArray2D<SireMaths::Vector>;
template class SireBase::TrigArray2D<SireMaths::Matrix>;
#endif


SIRE_END_HEADER

#endif
