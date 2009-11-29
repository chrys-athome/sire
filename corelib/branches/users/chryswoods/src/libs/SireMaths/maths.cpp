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

#include "SireMaths/maths.h"

#include "third_party/GSL/fcmp.h" // CONDITIONAL_INCLUDE

namespace SireMaths
{
    /** Return true if two numbers are equal. This uses gsl_fcmp
        for the comparison, and the values must be equal to within
        a range of 2*delta, where delta = 2^k * epsilon, where
        k equals the maximum base-2 exponent of val0 and val1 as
        calculated via frexp(). This should normally be a value
        around 1e-6 */
    bool SIREMATHS_EXPORT areEqual(double val0, double val1, double epsilon)
    {
        return not ( third_party::gsl_fcmp(val0,val1,epsilon) );
    }

    /** Return true if two numbers are equal */
    bool SIREMATHS_EXPORT areEqual(double val0, double val1)
    {
        return not ( third_party::gsl_fcmp(val0,val1,1e-6) );
    }
}
