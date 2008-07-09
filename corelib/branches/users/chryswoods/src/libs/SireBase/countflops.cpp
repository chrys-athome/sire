/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2008  Christopher Woods
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

#include "countflops.h"

#ifdef SIRE_TIME_ROUTINES

#include <QDebug>
#include <cmath>

#include "SireError/errors.h"

#ifdef SIRE_USE_SSE
#include <emmintrin.h>
#endif

using namespace SireBase;

///////
/////// Implementation of CountFlops
///////

QList<CountFlops::ThreadFlops*> CountFlops::ThreadFlops::thread_flops;
QMutex CountFlops::ThreadFlops::thread_flops_mutex;

CountFlops::ThreadFlops::ThreadFlops() : nflops(0)
{
    QMutexLocker lkr(&thread_flops_mutex);
    thread_flops.append(this);
}

CountFlops::ThreadFlops::~ThreadFlops()
{
    QMutexLocker lkr(&thread_flops_mutex);
    thread_flops.removeAll(this);
}

/** Singleton copy of the counter */
CountFlops CountFlops::global_counter;

/** Constructor */
CountFlops::CountFlops()
{
    pthread_key_create(&thread_key, NULL);
    flop_timer.start();
}

/** Destructor */
CountFlops::~CountFlops()
{}

/** Return a mark for the current time and number of flops */
FlopsMark CountFlops::mark()
{
    QMutexLocker lkr( &ThreadFlops::thread_flops_mutex );

    int nthreads = ThreadFlops::thread_flops.count();
    QVector<int> thread_flops(nthreads);
    int *thread_flops_array = thread_flops.data();

    int i = 0;
    foreach (const ThreadFlops *ptr, ThreadFlops::thread_flops)
    {
        thread_flops_array[i] = ptr->nflops;
        ++i;
    }

    return FlopsMark(thread_flops, 
                     global_counter.flop_timer.elapsed());
}

///////
/////// Implementation of FlopsMark
///////

/** Copy assignment operator */
FlopsMark& FlopsMark::operator=(const FlopsMark &other)
{
    nflops = other.nflops;
    ms = other.ms;
    
    return *this;
}

/** Constructor - this creates a mark for NOW */
FlopsMark::FlopsMark()
{
    this->operator=( CountFlops::mark() );
}

/** Constructor used by CountFlops */
FlopsMark::FlopsMark(const QVector<int> &_nflops, int _ms)
          : nflops(_nflops), ms(_ms)
{}

/** Copy constructor */
FlopsMark::FlopsMark(const FlopsMark &other)
          : nflops(other.nflops), ms(other.ms)
{}

/** Destructor */
FlopsMark::~FlopsMark()
{}

/** Return the total number of flops from all threads up to this point */
int FlopsMark::nFlops() const
{
    int total_nflops = 0;
    
    foreach (int thread_nflops, nflops)
    {
        total_nflops += thread_nflops;
    }
    
    return total_nflops;
}

/** Return the total number of threads that contributed to this 
    flop count */
int FlopsMark::nThreads() const
{
    return nflops.count();
}

/** Return a FlopsMark object that contains just the information
    for the ith thread
    
    \throw SireError::invalid_index
*/
FlopsMark FlopsMark::threadFlops(int i) const
{
    if (i < 0 or i >= this->nThreads())
        throw SireError::invalid_index( QObject::tr(
            "Invalid index %1. The number of available threads == %2.")
                .arg(i).arg(this->nThreads()), CODELOC );

    QVector<int> thread_nflops(1, nflops.at(i));
    
    return FlopsMark(thread_nflops, ms);
}

/** Return a FlopsMark object that contains just the information
    for the ith thread
    
    \throw SireError::invalid_index
*/
FlopsMark FlopsMark::operator[](int i) const
{
    return this->threadFlops(i);
}

/** Return the average number of floating point operations
    performed since the mark 'other' */
double FlopsMark::operator-(const FlopsMark &other) const
{
    int dnflops = this->nFlops() - other.nFlops();
    int dms = ms - other.ms;
    
    if (dms == 0)
        //are lowest resolution is 1 ms
        dms = 1;
        
    return (1000.0 * dnflops) / dms;
}

double FlopsMark::benchmark_sum(0);
QMutex FlopsMark::benchmark_mutex;

/** Perform a simple benchmark to work out what the realistic maximum
    FLOPS count for this processor (compiled with this compiler)
    if only additions are used */
double FlopsMark::benchmarkSum()
{
    QMutexLocker lkr( &benchmark_mutex );

    const int nvals = 1000;
    const int nloops = 100000;
    
    double values[nvals];
    
    //fill this up with random rubbish
    for (int i=0; i<nvals; ++i)
    {
        values[i] = 5 * std::rand();
    }
    
    //now calculate the sum of the square root of each value with
    //the previous value
    FlopsMark before_sum;
    
    double sum = 0;
    
    #pragma omp parallel
    {
        const double *my_values = values;
        double my_sum = 0;

        #pragma omp for schedule(static)
        for (int j=0; j<nloops; ++j)
        {
            #ifdef SIRE_USE_SSE
            {
                for (int i=2; i<nvals; i+=2)
                {
                    __m128d low_pair = { my_values[i-1], my_values[i-2] };
                    __m128d high_pair = { my_values[i], my_values[i-1] };
                    
                    //__m128d sse_sum = _mm_sqrt_pd( low_pair * high_pair );
                    __m128d sse_sum = low_pair + high_pair;
                    
                    my_sum += ( *((const double*)(&sse_sum)) +
                                *( ((const double*)(&sse_sum)) + 1 ) );
                }
            }
            #else
            {
                for (int i=1; i<nvals; ++i)
                {
                    my_sum += my_values[i] + my_values[i-1];
                }
            }
            #endif
        }
    
        #pragma omp critical
        {
            sum += my_sum;
        }
    }
    
    int nflops = nloops * (2 * (nvals-1));
    
    ADD_FLOPS(nflops);
    
    FlopsMark after_sum;
    
    //save the sum - this prevents the compiler from optimising 
    //away the benchmark
    benchmark_sum = sum;
    
    return after_sum - before_sum;
}

/** Perform a simple benchmark to work out what the realistic maximum
    FLOPS count for this processor (compiled with this compiler)
    if a mixture of additions and products are used */
double FlopsMark::benchmarkProduct()
{
    QMutexLocker lkr( &benchmark_mutex );

    const int nvals = 1000;
    const int nloops = 100000;
    
    double values[nvals];
    
    //fill this up with random rubbish
    for (int i=0; i<nvals; ++i)
    {
        values[i] = 5 * std::rand();
    }
    
    //now calculate the sum of the square root of each value with
    //the previous value
    FlopsMark before_sum;
    
    double sum = 0;
    
    #pragma omp parallel
    {
        const double *my_values = values;
        double my_sum = 0;

        #pragma omp for schedule(static)
        for (int j=0; j<nloops; ++j)
        {
            #ifdef SIRE_USE_SSE
            {
                for (int i=2; i<nvals; i+=2)
                {
                    __m128d low_pair = { my_values[i-1], my_values[i-2] };
                    __m128d high_pair = { my_values[i], my_values[i-1] };
                    
                    //__m128d sse_sum = _mm_sqrt_pd( low_pair * high_pair );
                    __m128d sse_sum = low_pair * high_pair;
                    
                    my_sum += ( *((const double*)(&sse_sum)) +
                                *( ((const double*)(&sse_sum)) + 1 ) );
                }
            }
            #else
            {
                for (int i=1; i<nvals; ++i)
                {
                    my_sum += my_values[i] * my_values[i-1];
                }
            }
            #endif
        }
    
        #pragma omp critical
        {
            sum += my_sum;
        }
    }
    
    int nflops = nloops * (2 * (nvals-1));
    
    ADD_FLOPS(nflops);
    
    FlopsMark after_sum;
    
    //save the sum - this prevents the compiler from optimising 
    //away the benchmark
    benchmark_sum = sum;
    
    return after_sum - before_sum;
}

/** Perform a simple benchmark to work out what the realistic maximum
    FLOPS count for this processor (compiled with this compiler)
    if a mixture of additions and divides are used */
double FlopsMark::benchmarkQuotient()
{
    QMutexLocker lkr( &benchmark_mutex );

    const int nvals = 1000;
    const int nloops = 100000;
    
    double values[nvals];
    
    //fill this up with random rubbish
    for (int i=0; i<nvals; ++i)
    {
        values[i] = 5 * std::rand();
    }
    
    //now calculate the sum of the square root of each value with
    //the previous value
    FlopsMark before_sum;
    
    double sum = 0;
    
    #pragma omp parallel
    {
        const double *my_values = values;
        double my_sum = 0;

        #pragma omp for schedule(static)
        for (int j=0; j<nloops; ++j)
        {
            #ifdef SIRE_USE_SSE
            {
                for (int i=2; i<nvals; i+=2)
                {
                    __m128d low_pair = { my_values[i-1], my_values[i-2] };
                    __m128d high_pair = { my_values[i], my_values[i-1] };
                    
                    //__m128d sse_sum = _mm_sqrt_pd( low_pair * high_pair );
                    __m128d sse_sum = low_pair / high_pair;
                    
                    my_sum += ( *((const double*)(&sse_sum)) +
                                *( ((const double*)(&sse_sum)) + 1 ) );
                }
            }
            #else
            {
                for (int i=1; i<nvals; ++i)
                {
                    my_sum += my_values[i] / my_values[i-1];
                }
            }
            #endif
        }
    
        #pragma omp critical
        {
            sum += my_sum;
        }
    }
    
    int nflops = nloops * (2 * (nvals-1));
    
    ADD_FLOPS(nflops);
    
    FlopsMark after_sum;
    
    //save the sum - this prevents the compiler from optimising 
    //away the benchmark
    benchmark_sum = sum;
    
    return after_sum - before_sum;
}

/** Perform a simple benchmark to work out what the realistic maximum
    FLOPS count for this processor (compiled with this compiler)
    if a mixture of additions, products and sqrts are used */
double FlopsMark::benchmark()
{
    QMutexLocker lkr( &benchmark_mutex );

    const int nvals = 1000;
    const int nloops = 100000;
    
    double values[nvals];
    
    //fill this up with random rubbish
    for (int i=0; i<nvals; ++i)
    {
        values[i] = 5 * std::rand();
    }
    
    //now calculate the sum of the square root of each value with
    //the previous value
    FlopsMark before_sum;
    
    double sum = 0;
    
    #pragma omp parallel
    {
        const double *my_values = values;
        double my_sum = 0;

        #pragma omp for schedule(static)
        for (int j=0; j<nloops; ++j)
        {
            #ifdef SIRE_USE_SSE
            {
                for (int i=2; i<nvals; i+=2)
                {
                    __m128d low_pair = { my_values[i-1], my_values[i-2] };
                    __m128d high_pair = { my_values[i], my_values[i-1] };
                    
                    //__m128d sse_sum = _mm_sqrt_pd( low_pair * high_pair );
                    __m128d sse_sum = _mm_sqrt_pd( low_pair * high_pair );
                    
                    my_sum += ( *((const double*)(&sse_sum)) +
                                *( ((const double*)(&sse_sum)) + 1 ) );
                }
            }
            #else
            {
                for (int i=1; i<nvals; ++i)
                {
                    my_sum += std::sqrt(my_values[i] * my_values[i-1]);
                }
            }
            #endif
        }
    
        #pragma omp critical
        {
            sum += my_sum;
        }
    }
    
    int nflops = nloops * (3 * (nvals-1));
    
    ADD_FLOPS(nflops);
    
    FlopsMark after_sum;
    
    //save the sum - this prevents the compiler from optimising 
    //away the benchmark
    benchmark_sum = sum;
    
    return after_sum - before_sum;
}

#endif // #ifdef SIRE_TIME_ROUTINES
