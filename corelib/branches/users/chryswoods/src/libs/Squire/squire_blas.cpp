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

#ifndef SQUIRE_SQUIRE_BLAS_H
#define SQUIRE_SQUIRE_BLAS_H

#ifdef SQUIRE_HAVE_BLAS_AND_LAPACK

#include "squire_blas.h"
#include "squire_blas_f.h"

#define SquireINTEGER int

//prototypes for all of the BLAS functions used by squire_blas
extern "C"
{
    /** This is dgemm - see BLAS API for documentation */
    void SquireDGEMM(const char *TRANSA, const char *TRANSB,
                     const SquireINTEGER *M, const SquireINTEGER *N,
                     const SquireINTEGER *K,
                     const double *ALPHA, 
                     const double *A, const SquireINTEGER *LDA,
                     const double *B, const SquireINTEGER *LDB,
                     const double *BETA,
                     const double *C, const SquireINTEGER *LDC);
                     
}

namespace Squire
{

static int getLD(const MatrixType &m, char TRANS)
{
    switch( TRANS )
    {
        case 'N':
        case 'n':
            return m.nRows();
      
        default:
            return m.nColumns();
    }
}

void SQUIRE_EXPORT dgemm(char TRANSA, char TRANSB,
                         double ALPHA, const MatrixType &A, const MatrixType &B,
                         double BETA, const MatrixType &C,
                         MatrixType &result)
{
    if (result.constData() != C.constData())
    {
        result = C;
    }
    
    SquireINTEGER M, N, K, LDA, LDB, LDC;
    
    M = A.nRows();
    N = B.nColumns();
    K = A.nColumns();
    
    BOOST_ASSERT( K == SquireINTEGER(B.nRows()) );
    
    LDA = getLD(A, TRANSA);
    LDB = getLD(B, TRANSB);
    LDC = C.nRows();
    
    BOOST_ASSERT( SquireINTEGER(C.nRows()) == LDA );
    BOOST_ASSERT( SquireINTEGER(C.nColumns()) == LDB );
    
    SquireDGEMM( &TRANSA, &TRANSB, &M, &N, &K, 
                 &ALPHA, A.constData(), &LDA, B.constData(), &LDB,
                 &BETA, result.data(), &LDC );
}
                      
void SQUIRE_EXPORT dgemm(char TRANSA, char TRANSB,
                         const MatrixType &A, const MatrixType &B,
                         MatrixType &result)
{
    Squire::dgemm(TRANSA, TRANSB, 1, A, B, 0, result, result);
}

MatrixType SQUIRE_EXPORT dgemm(char TRANSA, char TRANSB,
                               const MatrixType &A, const MatrixType &B)
{
    MatrixType result( getLD(A,TRANSA), getLD(B,TRANSB) );
    Squire::dgemm(TRANSA, TRANSB, A, B, result);
    return result;
}

MatrixType SQUIRE_EXPORT dgemm(char TRANSA, char TRANSB,
                               double alpha, 
                               const MatrixType &A, const MatrixType &B,
                               double beta, const MatrixType &C)
{
    MatrixType result = C;
    Squire::dgemm(TRANSA, TRANSB, alpha, A, B, beta, C, result);
    return result;
}

} // end of namespace Squire

#else  // SQUIRE_HAVE_BLAS_AND_LAPACK

#include "SireError/errors.h"
#include "squire_blas.h"

static void blas_not_supported()
{
    throw SireError::unsupported( QObject::tr(
            "BLAS is not available in this version of Squire. You "
            "need to recompile Squire, telling it where the BLAS "
            "libraries are located."), CODELOC );
}

namespace Squire
{

MatrixType SQUIRE_EXPORT Squire::dgemm(char TRANSA, char TRANSB,
                                       const MatrixType &A, const MatrixType &B)
{
    blas_not_supported();
    return MatrixType();
}

MatrixType SQUIRE_EXPORT Squire::dgemm(char TRANSA, char TRANSB,
                 double ALPHA, const MatrixType &A, const MatrixType &B,
                 double BETA, const MatrixType &C)
{
    blas_not_supported();
    return MatrixType();
}
                      
void SQUIRE_EXPORT Squire::dgemm(char TRANSA, char TRANSB,
           const MatrixType &A, const MatrixType &B,
           MatrixType &result)
{
    blas_not_supported();
}

void SQUIRE_EXPORT Squire::dgemm(char TRANSA, char TRANSB,
           double ALPHA, const MatrixType &A, const MatrixType &B,
           double BETA, const MatrixType &C,
           MatrixType &result)
{
    blas_not_supported();
}

} // end of namespace Squire

#endif // SQUIRE_HAVE_BLAS_AND_LAPACK

#endif
