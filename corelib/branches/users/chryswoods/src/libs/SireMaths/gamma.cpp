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

#include "gamma.h"

#include "Siren/errors.h"

#ifdef HAVE_BOOST_GAMMA_HPP
    #include <boost/math/special_functions/gamma.hpp>   // CONDITIONAL_INCLUDE
#endif

namespace SireMaths
{

#ifndef HAVE_BOOST_GAMMA_HPP
    static void throwBoostUnsupported(const QString &codeloc)
    {
        throw Siren::unsupported( QObject::tr(
               "Sire is linked against a version of the boost math libraries "
               "that does not include the gamma function. Please upgrade to "
               "a newer version of the boost and recompile Sire."), codeloc );
    }
#endif

/** Return the value of the Gamma function 
    \Gamma(\alpha) = \int_0^{\infty} t^{\alpha-1} e^{-t} dt 
*/
double SIREMATHS_EXPORT Gamma(double alpha)
{
    #ifdef HAVE_BOOST_GAMMA_HPP
        return boost::math::tgamma(alpha);
    #else
        throwBoostUnsupported(CODELOC);
        return 0;
    #endif
}

/** Synonym for SireMaths::Gamma */
double SIREMATHS_EXPORT gamma(double alpha)
{
    return Gamma(alpha);
}

/** Return the incomplete gamma function
    \Gamma(\alpha) = \int_x^{\infty} t^{\alpha-1} e^{-t} dt */
double SIREMATHS_EXPORT Gamma(double alpha, double x)
{
    #ifdef HAVE_BOOST_GAMMA_HPP
        return boost::math::gamma_q(alpha, x) * boost::math::tgamma(alpha);
    #else
        throwBoostUnsupported(CODELOC);
        return 0;
    #endif
}

/** Return the incomplete gamma function
    \Gamma(\alpha) = \int_0^{x} t^{\alpha-1} e^{-t} dt */
double SIREMATHS_EXPORT gamma(double alpha, double x)
{
    #ifdef HAVE_BOOST_GAMMA_HPP
        return boost::math::gamma_p(alpha, x) * boost::math::tgamma(alpha);
    #else
        throwBoostUnsupported(CODELOC);
        return 0;
    #endif
}

/** Synonym for gamma(alpha,x) */
double SIREMATHS_EXPORT incomplete_gamma_lower(double alpha, double x)
{
    return gamma(alpha,x);
}

/** Synonym for Gamma(alpha,x) */
double SIREMATHS_EXPORT incomplete_gamma_higher(double alpha, double x)
{
    return Gamma(alpha,x);
}

} // end of namespace SireMaths
