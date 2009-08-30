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

const int start_m = 50;

double SIREMATHS_EXPORT boys_f0(double x)
{
    double two_sqrt_x = 2 * std::sqrt(x);
    
    if (two_sqrt_x == 0)
        return 1;
    else
        return gamma(0.5, x) / two_sqrt_x;
}

double SIREMATHS_EXPORT boys_f1(double x)
{
    if (x == 0)
        return 1.0/3.0;

    //use upward recurrance
    const double f_0_x = boys_f0(x);
    const double e_to_minus_x = std::exp(-x);

    return (1 / (2*x)) * (f_0_x - e_to_minus_x);
}

double SIREMATHS_EXPORT boys_f2(double x)
{
    if (x == 0)
        return 1.0/5.0;

    //use upward recurrance
    const double f_0_x = boys_f0(x);
    const double e_to_minus_x = std::exp(-x);
    const double one_over_two_x = 1 / (2*x);

    const double f_1_x = one_over_two_x * (f_0_x - e_to_minus_x);
    
    return one_over_two_x * (3*f_1_x - e_to_minus_x);
}

double SIREMATHS_EXPORT boys_f3(double x)
{
    if (x == 0)
        return 1.0/7.0;

    //use upward recurrance
    const double f_0_x = boys_f0(x);
    const double e_to_minus_x = std::exp(-x);
    const double one_over_two_x = 1 / (2*x);

    const double f_1_x = one_over_two_x * (f_0_x - e_to_minus_x);
    const double f_2_x = one_over_two_x * (3*f_1_x - e_to_minus_x);
    
    return one_over_two_x * (5*f_2_x - e_to_minus_x);
}

double SIREMATHS_EXPORT boys(int m, double x)
{
    switch( m )
    {
        case 0:
            return boys_f0(x);
        case 1:
            return boys_f1(x);
        case 2:
            return boys_f2(x);
        case 3:
            return boys_f3(x);
        default:
        {
            //use downward recurrence relationship - start from
            //an estimate of boys[m+start_m,x] and work down
            //(the error will become insignificant)
            
            //boys[0,x] is of the same order of magnitude as boys[m,x],
            //so it makes a good estimate
            const double f_0_x = boys_f0(x);
            
            const double e_to_minus_x = std::exp(-x);
            
            double f_m_x = f_0_x;
                
            for (int i=m+start_m; i>=m; --i)
            {
                f_m_x = (1.0 / (2.0*i + 1.0)) * (2*x*f_m_x + e_to_minus_x);
            }

            return f_m_x;
        }
    }
}

double SIREMATHS_EXPORT boys(double m, double x)
{
    return boys( int(m), x );
}

void SIREMATHS_EXPORT multi_boys_2(double x, double boys[2])
{    
    if (x != 0)
    {
        boys[0] = boys_f0(x);
    
        //use upward recursion to get boysf1 and boysf2
        const double one_over_two_x = 1 / (2*x);
        const double exp_minus_x = std::exp(-x);
        
        boys[1] = one_over_two_x * (boys[0] - exp_minus_x);
    }
    else
    {
        boys[0] = 1;
        boys[1] = 1.0/3.0;
    }
}

void SIREMATHS_EXPORT multi_boys_3(double x, double boys[3])
{    
    if (x != 0)
    {
        boys[0] = boys_f0(x);
    
        //use upward recursion to get boysf1 and boysf2
        const double one_over_two_x = 1 / (2*x);
        const double exp_minus_x = std::exp(-x);
        
        boys[1] = one_over_two_x * (boys[0] - exp_minus_x);
        boys[2] = one_over_two_x * (3*boys[1] - exp_minus_x);
    }
    else
    {
        boys[0] = 1;
        boys[1] = 1.0/3.0;
        boys[2] = 1.0/5.0;
    }
}

void SIREMATHS_EXPORT multi_boys_4(double x, double boys[4])
{    
    if (x != 0)
    {
        boys[0] = boys_f0(x);
    
        //use upward recursion to get boysf1, boysf2 and boysf3
        const double one_over_two_x = 1 / (2*x);
        const double exp_minus_x = std::exp(-x);
        
        boys[1] = one_over_two_x * (boys[0] - exp_minus_x);
        boys[2] = one_over_two_x * (3*boys[1] - exp_minus_x);
        boys[3] = one_over_two_x * (5*boys[2] - exp_minus_x);
    }
    else
    {
        boys[0] = 1;
        boys[1] = 1.0/3.0;
        boys[2] = 1.0/5.0;
        boys[3] = 1.0/7.0;
    }
}

void SIREMATHS_EXPORT multi_boys_n(double x, double b[], int n)
{
    //use downward recursion from boys_n
    b[n-1] = boys(n-1, x);
    
    const double two_x = 2*x;
    const double exp_minus_x = std::exp(-x);
    
    for (int i=n-2; i>=0; --i)
    {
        b[i] = (1 / (2*i + 1.0)) * (two_x * b[i+1] + exp_minus_x);
    }
}

QVector<double> SIREMATHS_EXPORT multi_boys(double x, int n)
{
    QVector<double> boys(n, 0);
    boys.squeeze();
    
    multi_boys_n(x, boys.data(), n);
    
    return boys;
}

} // end of namespace SireMaths
