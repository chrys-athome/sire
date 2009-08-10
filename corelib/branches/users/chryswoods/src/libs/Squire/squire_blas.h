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

#ifndef SQUIRE_BLAS_H
#define SQUIRE_BLAS_H

#ifdef SQUIRE_HAVE_BLAS_AND_LAPACK

#include <QVector>

#include "SireBase/array2d.hpp"

SIRE_BEGIN_HEADER

namespace Squire
{

typedef SireBase::Array2D<double> MatrixType;
typedef QVector<double> VectorType;

MatrixType dgemm(char TRANSA, char TRANSB,
                 const MatrixType &A, const MatrixType &B);

MatrixType dgemm(char TRANSA, char TRANSB,
                 double ALPHA, const MatrixType &A, const MatrixType &B,
                 double BETA, const MatrixType &C);
                      

void dgemm(char TRANSA, char TRANSB,
           const MatrixType &A, const MatrixType &B,
           MatrixType &result);

void dgemm(char TRANSA, char TRANSB,
           double ALPHA, const MatrixType &A, const MatrixType &B,
           double BETA, const MatrixType &C,
           MatrixType &result);

}

SIRE_END_HEADER

#endif // SQUIRE_HAVE_BLAS_AND_LAPACK

#endif
