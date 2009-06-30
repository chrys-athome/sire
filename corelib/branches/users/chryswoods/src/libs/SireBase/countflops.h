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

#ifndef SIREBASE_COUNTFLOPS_H
#define SIREBASE_COUNTFLOPS_H

#include "sireglobal.h"

#include <QTime>
#include <QVector>
#include <QMutex>

#include <pthread.h>

SIRE_BEGIN_HEADER

namespace SireBase
{

class CountFlops;

/** This class contains a marker that may be
    used to get the flops per second
    
    @author Christopher Woods
*/
class SIREBASE_EXPORT FlopsMark
{

friend class CountFlops;

public:
    FlopsMark();
    FlopsMark(const FlopsMark &other);
    
    ~FlopsMark();
    
    FlopsMark& operator=(const FlopsMark &other);
    
    double operator-(const FlopsMark &other) const;
    
    FlopsMark operator[](int i) const;
    
    int nFlops() const;
    int nThreads() const;
    
    FlopsMark threadFlops(int i) const;
    
    static double benchmark();
    static double benchmarkSum();
    static double benchmarkProduct();
    static double benchmarkQuotient();
    
protected:
    FlopsMark(const QVector<int> &nflops, int ms);
    
private:
    /** The number of flops carried out by at this mark
        for each thread */
    QVector<int> nflops;
    
    /** The time that this mark was taken (in ms) */
    int ms;
};

#ifdef SIRE_TIME_ROUTINES

/** This is the singleton class that can be used
    to count floating point operations in the program
    
    @author Christopher Woods
*/
class SIREBASE_EXPORT CountFlops
{
public:
    class ThreadFlops
    {
    public:
        ThreadFlops();
        ~ThreadFlops();
        
        int nflops;
    };

    CountFlops();
    ~CountFlops();
    
    static void addFlops(int nflops);
    static FlopsMark mark();

private:
    pthread_key_t thread_key;
    
    /** The timer used to get the flop rate */
    QTime flop_timer;
};

#endif //SIRE_TIME_ROUTINES

} // end of namespace SireBase

#ifdef ADD_FLOPS
#undef ADD_FLOPS
#endif

#ifdef SIRE_TIME_ROUTINES
#define ADD_FLOPS(n)  SireBase::CountFlops::addFlops(n);
#else
#define ADD_FLOPS(n) /* Not adding n flops */
#endif

SIRE_EXPOSE_CLASS( SireBase::FlopsMark )

SIRE_END_HEADER

#endif
