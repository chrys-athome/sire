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

using namespace SireBase;

///////
/////// Implementation of CountFlops
///////

/** Singleton copy of the counter */
CountFlops CountFlops::global_counter;

/** Constructor */
CountFlops::CountFlops() : flop_count(0)
{
    flop_timer.start();
}

/** Destructor */
CountFlops::~CountFlops()
{}

/** Add 'nflops' to the total number of floating point operations */
void CountFlops::addFlops(int nflops)
{
    register int c = global_counter.flop_count;

    while (not global_counter.flop_count.testAndSetOrdered(c, c+nflops))
    {
        c = global_counter.flop_count;
    }
}

/** Return a mark for the current time and number of flops */
FlopsMark CountFlops::mark()
{
    return FlopsMark(global_counter.flop_count, 
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
FlopsMark::FlopsMark(int _nflops, int _ms)
          : nflops(_nflops), ms(_ms)
{}

/** Copy constructor */
FlopsMark::FlopsMark(const FlopsMark &other)
          : nflops(other.nflops), ms(other.ms)
{}

/** Destructor */
FlopsMark::~FlopsMark()
{}

/** Return the average number of floating point operations
    performed since the mark 'other' */
double FlopsMark::operator-(const FlopsMark &other) const
{
    int dnflops = nflops - other.nflops;
    int dms = ms - other.ms;
    
    if (dms == 0)
        //are lowest resolution is 1 ms
        dms = 1;
        
    return (1000.0 * dnflops) / dms;
}

#endif // #ifdef SIRE_TIME_ROUTINES
