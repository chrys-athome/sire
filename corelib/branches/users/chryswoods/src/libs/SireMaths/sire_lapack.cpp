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

#include "sire_lapack.h"
#include "nvector.h"
#include "nmatrix.h"

#include <cmath>

#include "SireError/errors.h"
#include "SireMaths/errors.h"

typedef int LAPACK_INT;

#ifdef SIRE_HAVE_LAPACK
    #include "sire_lapack_f.h" // CONDITIONAL_INCLUDE

    // Here are all of the prototypes of the LAPACK functions used
    // by sire_lapack
    extern "C"
    {
        /** This is dsyev - see LAPACK API for documentation */
        void SireDSYEV(const char *JOBZ, const char *UPLO, 
                       const LAPACK_INT *N, double *A, 
                       const LAPACK_INT *LDA, 
                       double *W, double *WORK, 
                       const LAPACK_INT *LWORK, 
                       LAPACK_INT *INFO);

        /** This is dgeco - see LAPACK API for documentation */
        void SireDGECO(double *A, const LAPACK_INT *LDA, 
                       const LAPACK_INT *N, LAPACK_INT *IPVT, 
                       double *RCOND, double *Z);
    
        /** This is dgedi - see LAPACK API for documentation */
        void SireDGEDI(double *A, const LAPACK_INT *LDA, 
                       const LAPACK_INT *N, const LAPACK_INT *IPVT, 
                       double *DET, double *WORK, const LAPACK_INT *JOB);
    
    } // end of extern "C"

#else  // SIRE_HAVE_LAPACK

static void not_implemented(const char *routine, const QString &codeloc)
{
    throw SireError::incomplete_code( QObject::tr(
                "The local BLAS routine (%1) has not been implemented.")
                    .arg(routine), codeloc );
}

#endif // SIRE_HAVE_BLAS

namespace SireMaths
{

std::pair<NVector,NMatrix> SIREMATHS_EXPORT dsyev(const NMatrix &A, bool upper)
{
    #ifdef SIRE_HAVE_LAPACK
        if (A.isTransposed())
        {
            //we can only process a column-major ordered matrix...
            return dsyev( A.transpose().fullTranspose(), upper );
        }
    
        char JOBZ, UPLO;
        LAPACK_INT N, LDA, LWORK, INFO;
        
        JOBZ = 'V';
        
        if (upper)
            UPLO = 'U';
        else
            UPLO = 'L';
            
        N = A.nRows();
        
        BOOST_ASSERT( A.nColumns() == N );
        
        LDA = N;
        
        NVector EIGVAL(N);
        
        QVector<double> WORK( 5*N );
        LWORK = WORK.count();
        
        INFO = 0;
        
        NMatrix EIGVEC( A );
        
        SireDSYEV(&JOBZ, &UPLO, &N, EIGVEC.data(),
                  &LDA, EIGVAL.data(), WORK.data(), &LWORK, &INFO);
                  
        if (INFO != 0)
            throw SireMaths::domain_error( QObject::tr(
                    "There was a problem running dsyev - INFO == %1. A ==\n%2.")
                        .arg(INFO).arg(A.toString()), CODELOC );
        
        return std::pair<NVector,NMatrix>(EIGVAL, EIGVEC);
    
    #else // SIRE_HAVE_LAPACK
        not_implemented( "dsyev", CODELOC );
        return std::pair<NVector,NMatrix>();
    
    #endif // SIRE_HAVE_LAPACK
}

NVector SIREMATHS_EXPORT dsyev_eigenvalues(const NMatrix &A, bool upper)
{
    #ifdef SIRE_HAVE_LAPACK
        if (A.isTransposed())
        {
            //we can only process a column-major ordered matrix...
            return dsyev_eigenvalues( A.transpose().fullTranspose(), upper );
        }
    
        char JOBZ, UPLO;
        LAPACK_INT N, LDA, LWORK, INFO;
        
        JOBZ = 'N';
        
        if (upper)
            UPLO = 'U';
        else
            UPLO = 'L';
            
        N = A.nRows();
        
        BOOST_ASSERT( A.nColumns() == N );
        
        LDA = N;
        
        NVector EIGVAL(N);
        
        QVector<double> WORK( 5*N );
        LWORK = WORK.count();
        
        INFO = 0;
        
        NMatrix A_COPY( A );
        
        SireDSYEV(&JOBZ, &UPLO, &N, A_COPY.data(),
                  &LDA, EIGVAL.data(), WORK.data(), &LWORK, &INFO);
                  
        if (INFO != 0)
            throw SireMaths::domain_error( QObject::tr(
                    "There was a problem running dsyev - INFO == %1. A ==\n%2.")
                        .arg(INFO).arg(A.toString()), CODELOC );
        
        return EIGVAL;
    
    #else // SIRE_HAVE_LAPACK
        not_implemented( "dsyev", CODELOC );
        return NVector();
    
    #endif // SIRE_HAVE_LAPACK
}

std::pair< NMatrix,QVector<int> > SIREMATHS_EXPORT dgeco(const NMatrix &A)
{
    #ifdef SIRE_HAVE_LAPACK
        LAPACK_INT LDA, N;
        
        if (A.isTransposed())
            return dgeco( A.transpose().fullTranspose() );
            
        N = A.nRows();
        BOOST_ASSERT( A.nColumns() == N );
        
        LDA = N;
        
        QVector<LAPACK_INT> IPVT(N);
        
        double RCOND;
        QVector<double> Z(N);
        
        NMatrix A_OUT(A);
        
        SireDGECO(A_OUT.data(), &LDA, &N, IPVT.data(), &RCOND, Z.data());
        
        return std::pair< NMatrix,QVector<int> >(A_OUT, IPVT);
    
    #else // SIRE_HAVE_LAPACK
        not_implemented( "dsyev", CODELOC );
        return std::pair< NMatrix,QVector<int> >();
    
    #endif // SIRE_HAVE_LAPACK
}

NMatrix SIREMATHS_EXPORT dgedi_inverse(const NMatrix &A, const QVector<int> &IPVT)
{
    #ifdef SIRE_HAVE_LAPACK
        LAPACK_INT LDA, N, JOB;
    
        N = A.nRows();
        BOOST_ASSERT( A.nColumns() == N );
        BOOST_ASSERT( IPVT.count() == N );
        
        LDA = N;
        
        QVector<double> WORK(N);
        
        JOB = 01;
        
        NMatrix A_OUT( A );
        
        double DET[2];
        
        SireDGEDI(A_OUT.data(), &LDA, &N, IPVT.constData(), &(DET[0]), 
                  WORK.data(), &JOB);
                  
        return A_OUT;
        
    #else // SIRE_HAVE_LAPACK
        not_implemented( "dsyev", CODELOC );
        return NMatrix();
    
    #endif // SIRE_HAVE_LAPACK
}

double SIREMATHS_EXPORT dgedi_determinant(const NMatrix &A, const QVector<int> &IPVT)
{
    #ifdef SIRE_HAVE_LAPACK
        LAPACK_INT LDA, N, JOB;
    
        N = A.nRows();
        BOOST_ASSERT( A.nColumns() == N );
        BOOST_ASSERT( IPVT.count() == N );
        
        LDA = N;
        
        QVector<double> WORK(N);
        
        JOB = 10;
        
        NMatrix A_OUT( A );
        
        double DET[2];
        
        SireDGEDI(A_OUT.data(), &LDA, &N, IPVT.constData(), &(DET[0]), 
                  WORK.data(), &JOB);
                  
        // DET contains DET[0] * 10^DET[1]
        return DET[0] * std::pow(10,DET[1]);
    
    #else // SIRE_HAVE_LAPACK
        not_implemented( "dsyev", CODELOC );
        return 0;
    
    #endif // SIRE_HAVE_LAPACK
}

std::pair<double,NMatrix> SIREMATHS_EXPORT dgedi(const NMatrix &A, 
                                                 const QVector<int> &IPVT)
{
    #ifdef SIRE_HAVE_LAPACK
        LAPACK_INT LDA, N, JOB;
    
        N = A.nRows();
        BOOST_ASSERT( A.nColumns() == N );
        BOOST_ASSERT( IPVT.count() == N );
        
        LDA = N;
        
        QVector<double> WORK(N);
        
        JOB = 11;
        
        NMatrix A_OUT( A );
        
        double DET[2];
        
        SireDGEDI(A_OUT.data(), &LDA, &N, IPVT.constData(), &(DET[0]), 
                  WORK.data(), &JOB);
                  
        // DET contains DET[0] * 10^DET[1]
        return std::pair<double,NMatrix>( DET[0] * std::pow(10,DET[1]), A_OUT );
    
    #else // SIRE_HAVE_LAPACK
        not_implemented( "dsyev", CODELOC );
        return std::pair<double,NMatrix>();
    
    #endif // SIRE_HAVE_LAPACK
}

} // end of namespace SireMaths
