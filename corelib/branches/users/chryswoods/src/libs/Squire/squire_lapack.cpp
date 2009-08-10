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

#ifdef SQUIRE_HAVE_BLAS_AND_LAPACK

#include "squire_lapack.h"
#include "squire_lapack_f.h"


#define SquireINTEGER int

//prototypes for all of the LAPACK functions used by squire_lapack
extern "C"
{
    /** This is dsyev - see LAPACK API for documentation */
    void SquireDSYEV(const char *JOBZ, const char *UPLO, 
                     const SquireINTEGER *N,
                     double *A, 
                     const SquireINTEGER *LDA, 
                     double *W, double *WORK,
                     SquireINTEGER *LWORK, SquireINTEGER *INFO);
}

namespace Squire
{

void SQUIRE_EXPORT dsyev(char JOBZ, char UPLO, MatrixType &A, VectorType &W, int &info)
{
    SquireINTEGER N = A.nRows();
    SquireINTEGER LDA = A.nColumns();

    if (N == 0 or LDA == 0)
    {
        W = VectorType();
        info = 0;
        return;
    }

    BOOST_ASSERT( SquireINTEGER(W.count()) == N );

    VectorType WORK(5*N);

    SquireINTEGER LWORK = WORK.count();
   
    SquireINTEGER INFO;
    
    SquireDSYEV( &JOBZ, &UPLO, &N, A.data(), &LDA, W.data(), WORK.data(), &LWORK, &INFO );

    info = INFO;
}

void SQUIRE_EXPORT dsyev(char JOBZ, char UPLO, MatrixType &A, VectorType &W)
{
    int info;
    dsyev(JOBZ, UPLO, A, W, info);
}

VectorType SQUIRE_EXPORT dsyev(char JOBZ, char UPLO, MatrixType &A)
{
    VectorType W(A.nRows());
    
    dsyev(JOBZ, UPLO, A, W);
    
    return W;
}

VectorType SQUIRE_EXPORT dsyev(char JOBZ, char UPLO, MatrixType &A, int &INFO)
{
    VectorType W(A.nRows());
    
    dsyev(JOBZ, UPLO, A, W, INFO);
    
    return W;
}


} // end of namespace Squire

#endif // SQUIRE_HAVE_BLAS_AND_LAPACK
