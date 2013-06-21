/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2013  Christopher Woods
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

#include "multifloat.h"

#include "SireError/errors.h"

using namespace SireMaths;
using namespace SireError;

/** Constructor. This initialises the float to 0 */
MultiFloat::MultiFloat()
{
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        x = _mm_set1_ps(0);
    #else
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            x[i] = 0;
        }
    #endif
}

/** Construct from the passed array. If size is greater than MultiFloat::size()
    then an error will be raised. If size is less than MultiFloat::size() then
    this float will be padded with zeroes */
MultiFloat::MultiFloat(float *array, int size)
{
    if (size > MULTIFLOAT_SIZE)
        throw SireError::unsupported( QObject::tr(
                "Cannot fit an array of size %1 in this MultiFloat, as it is only "
                "capable of holding %2 values...").arg(size).arg(MULTIFLOAT_SIZE), CODELOC );

    if (size <= 0)
    {
        #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
            x = _mm_set1_ps(0);
        #else
            for (int i=0; i<MULTIFLOAT_SIZE; ++i)
            {
                x[i] = 0;
            }
        #endif
    }
    else if (size == MULTIFLOAT_SIZE)
    {
        #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
            x = _mm_set_ps(array[0], array[1], array[2], array[3]);
        #else
            for (int i=0; i<MULTIFLOAT_SIZE; ++i)
            {
                x[i] = array[i];
            }
        #endif
    }
    else
    {
        float tmp[MULTIFLOAT_SIZE];
        
        for (int i=0; i<size; ++i)
        {
            tmp[i] = array[i];
        }
        
        for (int i=size; i<MULTIFLOAT_SIZE; ++i)
        {
            tmp[i] = 0;
        }
        
        #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
            x = _mm_set_ps(tmp[0], tmp[1], tmp[2], tmp[3]);
        #else
            for (int i=0; i<MULTIFLOAT_SIZE; ++i)
            {
                x[i] = tmp[i];
            }
        #endif
    }
}

/** Destructor */
MultiFloat::~MultiFloat()
{}

/** Return the ith value in the multifloat */
float MultiFloat::operator[](int i) const
{
    if (i < 0)
        i = MULTIFLOAT_SIZE + i;
    
    if (i < 0 or i >= MULTIFLOAT_SIZE)
    {
        throw SireError::invalid_index( QObject::tr(
                "Cannot access element %1 of MultiFloat (holds only %2 values)")
                    .arg(i).arg(MULTIFLOAT_SIZE), CODELOC );
    }
    
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        return *( ((const float*)&x) + i );
    #else
        return x[i];
    #endif
}

/** Set the ith value of the multifloat to 'value' */
void MultiFloat::set(int i, float value)
{
    if (i < 0)
        i = MULTIFLOAT_SIZE + i;
    
    if (i < 0 or i >= MULTIFLOAT_SIZE)
    {
        throw SireError::invalid_index( QObject::tr(
                "Cannot access element %1 of MultiFloat (holds only %2 values)")
                    .arg(i).arg(MULTIFLOAT_SIZE), CODELOC );
    }

    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        float tmp[4];
    
        tmp[0] = this->operator[](0);
        tmp[1] = this->operator[](1);
        tmp[2] = this->operator[](2);
        tmp[3] = this->operator[](3);
    
        tmp[i] = value;
    
        x = _mm_set_ps(tmp[0], tmp[1], tmp[2], tmp[3]);
    #else
        x[i] = value;
    #endif
}

/** Return the ith value in the multifloat */
float MultiFloat::get(int i) const
{
    return this->operator[](i);
}
