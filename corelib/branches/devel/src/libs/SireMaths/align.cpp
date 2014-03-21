/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2014  Christopher Woods
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

#include "SireMaths/align.h"
#include "SireMaths/accumulator.h"

#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_linalg.h>

namespace SireMaths
{
    /** Return the centroid of the points in 'p'. If n != -1 then
        only calculate the centroid of the first n points */
    Vector getCentroid(const QVector<Vector> &p, int n=-1)
    {
        if (p.isEmpty())
            return Vector(0);
    
        Average x, y, z;
    
        if (n < 0)
        {
            n = p.count();
        }
        else if (n > p.count())
        {
            n = p.count();
        }
        
        for (int i=0; i<n; ++i)
        {
            x.accumulate(p[i].x());
            y.accumulate(p[i].y());
            z.accumulate(p[i].z());
        }
        
        return Vector(x.average(), y.average(), z.average());
    }

    /** Return the RMSD between the two sets of points. If n != -1 then
        only calculate the RMSD using the first n points */
    double getRMSD(const QVector<Vector> &p, const QVector<Vector> &q, int n=-1)
    {
        if (p.isEmpty() or q.isEmpty())
            return 0;
        
        if (n < 0)
        {
            n = qMin(p.count(), q.count());
        }
        else if (n > qMin(p.count(),q.count()))
        {
            n = qMin(p.count(),q.count());
        }
        
        Average msd;
        
        for (int i=0; i<n; ++i)
        {
            msd.accumulate( pow_2(p[i].x()-q[i].x()) +
                            pow_2(p[i].y()-q[i].y()) +
                            pow_2(p[i].z()-q[i].z()) );
        }
        
        return std::sqrt( msd.average() );
    }

    double determinant(gsl_matrix *m, int nrow, int ncol)
    {
        
    }

    /** Use the kabasch algorithm (http://en.wikipedia.org/wiki/Kabsch_algorithm)
        to calculate the rotation matrix needed to align
        'q' on top of 'p'. This will match p[i] against
        q[i], and only matches the first N points (where N is the minimum
        of the number of points in 'p' and the number of points in 'q'.
        
        Note that this code assumes that q has been already translated on top of p.
     
        This code is inspired by the calculate_rmsd python script written by
        Jimmy Charnley Kromann and Lars Bratholm, available
        https://github.com/charnley/rmsd, and under license;
        
        =====================
        Copyright (c) 2013, Jimmy Charnley Kromann <jimmy@charnley.dk> & Lars Bratholm
        All rights reserved.

        Redistribution and use in source and binary forms, with or without
        modification, are permitted provided that the following conditions are met:

        1. Redistributions of source code must retain the above copyright notice, this
           list of conditions and the following disclaimer.
        2. Redistributions in binary form must reproduce the above copyright notice,
           this list of conditions and the following disclaimer in the documentation
           and/or other materials provided with the distribution.

        THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
        ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
        WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
        DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
        ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
        (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
        LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
        ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
        (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
        SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
        =======================
     
        (note that the C++ implement that I have here is licensed under the GPL,
         as stated at the top of this file)
    */
    Matrix SIREMATHS_EXPORT kabasch(const QVector<Vector> &p,
                                    const QVector<Vector> &q)
    {
        if (p.isEmpty() or q.isEmpty())
            return Matrix::identity();
    
        gsl_matrix *P = 0;
        gsl_matrix *Q = 0;
        gsl_matrix *A = 0;
        
        gsl_vector *S = 0;
        gsl_matrix *V = 0;
        gsl_matrix *W = 0;
        
        const int n = qMin(p.count(), q.count());
        
        try
        {
            //convert the two vectors of points into GSL matrices
            P = gsl_matrix_alloc(n, 3);
            Q = gsl_matrix_alloc(n, 3);
            
            for (int i=0; i<n; ++i)
            {
                gsl_matrix_set(P, i, 0, p[i].x());
                gsl_matrix_set(P, i, 1, p[i].y());
                gsl_matrix_set(P, i, 2, p[i].z());
                
                gsl_matrix_set(Q, i, 0, q[i].x());
                gsl_matrix_set(Q, i, 1, q[i].y());
                gsl_matrix_set(Q, i, 2, q[i].z());
            }
            
            //create space to hold the covariance matrix
            A = gsl_matrix_alloc(3, 3);
            
            for (int i=0; i<3; ++i)
            {
                for (int j=0; j<3; ++j)
                {
                    gsl_matrix_set(A, i, j, 0);
                }
            }
            
            //compute the covariance matrix P^T Q
            int ok = gsl_blas_dgemm(CblasTrans, CblasNoTrans, 1.0, P, Q, 0.0, A);
            
            qDebug() << "gsl_blas_dgemm" << ok;
            
            //create space to hold the matrices for single value decomposition
            S = gsl_vector_alloc(3);
            W = gsl_matrix_alloc(3, 3);
            
            // calculate single value decomposition of A into V S W^T
            ok = gsl_linalg_SV_decomp_jacobi(A, W, S);
            
            //above has decomposed in-place (so A has been overwritten to make V)
            gsl_matrix *V = A;
            
            qDebug() << "gsl_linalg_SV_decomp_jacobi" << ok;
            
            //calculate the product of determinants of V and W
            //double detVW =
        }
        catch(...)
        {
            gsl_matrix_free(P);
            gsl_matrix_free(Q);
            gsl_matrix_free(A);
            gsl_vector_free(S);
            gsl_matrix_free(V);
            throw;
        }
    
        return Matrix::identity();
    }
    
    /** Use the kabasch algorithm (http://en.wikipedia.org/wiki/Kabsch_algorithm)
        to calculate the translation vector and rotation matrix needed to align
        'q' on top of 'p'. This will match p[i] against
        q[i], and only matches the first N points (where N is the minimum
        of the number of points in 'p' and the number of points in 'q'.
        This will provide additional fitting to find the optimum translation vector
        for the alignment (as opposed to merely using the difference between the 
        centroids, as used in the simple kabasch algorithm)
     
        This code is inspired by the calculate_rmsd python script written by
        Jimmy Charnley Kromann and Lars Bratholm, available
        https://github.com/charnley/rmsd, and under license;
        
        =====================
        Copyright (c) 2013, Jimmy Charnley Kromann <jimmy@charnley.dk> & Lars Bratholm
        All rights reserved.

        Redistribution and use in source and binary forms, with or without
        modification, are permitted provided that the following conditions are met:

        1. Redistributions of source code must retain the above copyright notice, this
           list of conditions and the following disclaimer.
        2. Redistributions in binary form must reproduce the above copyright notice,
           this list of conditions and the following disclaimer in the documentation
           and/or other materials provided with the distribution.

        THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
        ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
        WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
        DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
        ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
        (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
        LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
        ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
        (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
        SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
        =======================
     
        (note that the C++ implement that I have here is licensed under the GPL,
         as stated at the top of this file)
    */
    AxisSet SIREMATHS_EXPORT kabaschFit(const QVector<Vector> &p,
                                        const QVector<Vector> &q)
    {
        return AxisSet(Matrix::identity(), Vector(0));
    }

    /** Return AxisSet containing the translation vector and rotation
        matrix needed to optimally align the points in 'q' on top of the
        points in 'p'. If 'fit' is true, then this performs an RMSD
        fit to find the optimal translation vector (as opposed to merely
        taking the difference of centroids) */
    AxisSet SIREMATHS_EXPORT getAlignment(const QVector<Vector> &p,
                                          const QVector<Vector> &q,
                                          bool fit)
    {
        if (p.isEmpty() or q.isEmpty())
            return AxisSet(Matrix::identity(), Vector(0));
    
        if (fit)
            return kabaschFit(p, q);
        
        else
        {
            //calculate the difference in centroids of p and q
            const int n = qMin(p.count(), q.count());
            
            Vector delta = getCentroid(q,n) - getCentroid(p,n);
            
            QVector<Vector> r(n);
            for (int i=0; i<n; ++i)
            {
                r[i] = q[i] - delta;
            }
            
            return AxisSet(kabasch(p, r), delta);
        }
    }
    
    /** Return a copy of points 'q' aligned on top of points 'p'. If 'fit' 
        is true, then this performs an RMSD
        fit to find the optimal translation vector (as opposed to merely
        taking the difference of centroids) */
    QVector<Vector> SIREMATHS_EXPORT align(const QVector<Vector> &p,
                                           const QVector<Vector> &q,
                                           bool fit)
    {
        if (p.isEmpty() or q.isEmpty())
            return q;
        
        AxisSet a = getAlignment(p, q, fit);
        
        Vector c = getCentroid(q, qMin(p.count(),q.count()));
        
        QVector<Vector> r(q);
        
        for (int i=0; i<r.count(); ++i)
        {
            r[i] = c + a.fromIdentity(q[i] - c);
        }
        
        return r;
    }

} // end of namespace SireMaths
