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

#include <cmath>

#include "boys.h"
#include "gamma.h"

#include <QDebug>

namespace SireMaths
{

const int start_m = 2000;

double SIREMATHS_EXPORT boys(int m, double x)
{
    //use downward recurrence relationship - start from
    //an estimate of boys[start_m,x] and work down
    //(the error will become insignificant)
    
    //boys[0,x] is of the same order of magnitude as boys[m,x],
    //so it makes a good estimate
    const double f_0_x = (1.0 / (2.0*std::sqrt(x))) * gamma(0.5, x);
    
    const double e_to_minus_x = std::exp(-x);
    
    double f_m_x = f_0_x;
        
    for (int i=start_m; i>=m; --i)
    {
        f_m_x = (1.0 / (2.0*i + 1.0)) * (2*x*f_m_x + e_to_minus_x);
    }

    return f_m_x;
}

double SIREMATHS_EXPORT boys(double m, double x)
{
    return boys( int(m), x );
}

} // end of namespace SireMaths
