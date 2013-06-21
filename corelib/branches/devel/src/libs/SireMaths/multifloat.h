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

#ifndef SIREMATHS_MULTIFLOAT_H
#define SIREMATHS_MULTIFLOAT_H

#include "sireglobal.h"
#include <cmath>

//#undef SIRE_USE_SSE

#ifdef SIRE_USE_SSE
    #ifdef __SSE__
        #include <emmintrin.h>   // CONDITIONAL_INCLUDE
        #define MULTIFLOAT_SSE_IS_AVAILABLE 1
    #else
        #undef SIRE_USE_SSE
        #undef MULTIFLOAT_SSE_IS_AVAILABLE
    #endif
#else
    #undef MULTIFLOAT_SSE_IS_AVAILABLE
#endif

SIRE_BEGIN_HEADER

namespace SireMaths
{
class MultiFloat;
}

namespace SireMaths
{

/** This class provides a vectorised float. This represents
    a single vector of floats on the compiled machine, e.g.
    4 floats if we use SSE2, 8 floats for AVX
    
    @author Christopher Woods
*/
class SIREMATHS_EXPORT MultiFloat
{
public:
    MultiFloat();
    MultiFloat(float *array, int size);
    MultiFloat(const MultiFloat &other);
    
    ~MultiFloat();
    
    MultiFloat& operator=(const MultiFloat &other);
    
    MultiFloat operator==(const MultiFloat &other) const;
    MultiFloat operator!=(const MultiFloat &other) const;
    
    MultiFloat operator<(const MultiFloat &other) const;
    MultiFloat operator>(const MultiFloat &other) const;
    
    MultiFloat operator<=(const MultiFloat &other) const;
    MultiFloat operator>=(const MultiFloat &other) const;
    
    static int size();
    static int count();
    
    float operator[](int i) const;
    
    void set(int i, float value);
    float get(int i) const;
    
    MultiFloat operator+(const MultiFloat &other) const;
    MultiFloat operator-(const MultiFloat &other) const;
    MultiFloat operator*(const MultiFloat &other) const;
    MultiFloat operator/(const MultiFloat &other) const;
    
    MultiFloat& operator+=(const MultiFloat &other);
    MultiFloat& operator-=(const MultiFloat &other);
    MultiFloat& operator*=(const MultiFloat &other);
    MultiFloat& operator/=(const MultiFloat &other);
    
    MultiFloat& multiplyAdd(const MultiFloat &val0, const MultiFloat &val1);
    
    MultiFloat max(const MultiFloat &other) const;
    MultiFloat min(const MultiFloat &other) const;
    
    MultiFloat reciprocal() const;
    
    MultiFloat sqrt() const;
    MultiFloat rsqrt() const;
    
    MultiFloat rotate() const;
    
    float sum() const;

private:
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        __m128 x;
        #define MULTIFLOAT_SIZE 4

        #ifndef SIRE_SKIP_INLINE_FUNCTIONS
        MultiFloat(__m128 v) : x(v){}
        #endif
    #else
        float x[4];
        #define MULTIFLOAT_SIZE 4
    #endif
};

#ifndef SIRE_SKIP_INLINE_FUNCTIONS

/** Copy constructor */
inline
MultiFloat::MultiFloat(const MultiFloat &other)
{
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        x = other.x;
    #else
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            x[i] = other.x[i];
        }
    #endif
}

/** Assignment operator */
inline
MultiFloat& MultiFloat::operator=(const MultiFloat &other)
{
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        x = other.x;
    #else
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            x[i] = other.x[i];
        }
    #endif
    
    return *this;
}

/** Comparison operator. This will return a MultiFloat with elements
    set to zero for each float that is not equal */
inline
MultiFloat MultiFloat::operator==(const MultiFloat &other) const
{
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        return MultiFloat( _mm_cmpeq_ps(x, other.x) );
    #else
        MultiFloat ret;

        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            ret.x[i] = (x[i] == other.x[i]) ? 1.0 : 0.0;
        }
    
        return ret;
    #endif
}

/** Not equals comparison operator */
inline
MultiFloat MultiFloat::operator!=(const MultiFloat &other) const
{
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        return MultiFloat( _mm_cmpneq_ps(x, other.x) );
    #else
        MultiFloat ret;
    
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            ret.x[i] = (x[i] != other.x[i]) ? 1.0 : 0.0;
        }
    
        return ret;
    #endif
}

/** Less than comparison operator */
inline
MultiFloat MultiFloat::operator<(const MultiFloat &other) const
{
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        return MultiFloat( _mm_cmplt_ps(x, other.x) );
    #else
        MultiFloat ret;
    
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            ret.x[i] = (x[i] < other.x[i]) ? 1.0 : 0.0;
        }
    
        return ret;
    #endif
}

/** Greater than comparison operator */
inline
MultiFloat MultiFloat::operator>(const MultiFloat &other) const
{
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        return MultiFloat( _mm_cmpgt_ps(x, other.x) );
    #else
        MultiFloat ret;
    
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            ret.x[i] = (x[i] > other.x[i]) ? 1.0 : 0.0;
        }
    
        return ret;
    #endif
}

/** Less than or equal comparison */
inline
MultiFloat MultiFloat::operator<=(const MultiFloat &other) const
{
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        return MultiFloat( _mm_cmple_ps(x, other.x) );
    #else
        MultiFloat ret;
    
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            ret.x[i] = (x[i] <= other.x[i]) ? 1.0 : 0.0;
        }
    
        return ret;
    #endif
}

/** Greater than or equal comparison */
inline
MultiFloat MultiFloat::operator>=(const MultiFloat &other) const
{
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        return MultiFloat( _mm_cmpge_ps(x, other.x) );
    #else
        MultiFloat ret;
    
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            ret.x[i] = (x[i] >= other.x[i]) ? 1.0 : 0.0;
        }
    
        return ret;
    #endif
}

/** Return the number of values in the vector */
inline
int MultiFloat::size()
{
    return MULTIFLOAT_SIZE;
}

/** Return the number of values in the vector */
inline
int MultiFloat::count()
{
    return MULTIFLOAT_SIZE;
}

/** Addition operator */
inline
MultiFloat MultiFloat::operator+(const MultiFloat &other) const
{
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        return MultiFloat( _mm_add_ps(x, other.x) );
    #else
        MultiFloat ret;
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            ret.x[i] = x[i] + other.x[i];
        }
        return ret;
    #endif
}

/** Subtraction operator */
inline
MultiFloat MultiFloat::operator-(const MultiFloat &other) const
{
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        return MultiFloat( _mm_sub_ps(x, other.x) );
    #else
        MultiFloat ret;
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            ret.x[i] = x[i] - other.x[i];
        }
        return ret;
    #endif
}

/** Multiplication operator */
inline
MultiFloat MultiFloat::operator*(const MultiFloat &other) const
{
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        return MultiFloat( _mm_mul_ps(x, other.x) );
    #else
        MultiFloat ret;
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            ret.x[i] = x[i] * other.x[i];
        }
        return ret;
    #endif
}

/** Division operator */
inline
MultiFloat MultiFloat::operator/(const MultiFloat &other) const
{
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        return MultiFloat( _mm_div_ps(x, other.x) );
    #else
        MultiFloat ret;
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            ret.x[i] = x[i] / other.x[i];
        }
        return ret;
    #endif
}

/** In-place addition operator */
inline
MultiFloat& MultiFloat::operator+=(const MultiFloat &other)
{
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        x = _mm_add_ps(x, other.x);
    #else
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            x[i] += other.x[i];
        }
    #endif

    return *this;
}

/** In-place subtraction operator */
inline
MultiFloat& MultiFloat::operator-=(const MultiFloat &other)
{
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        x = _mm_sub_ps(x, other.x);
    #else
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            x[i] -= other.x[i];
        }
    #endif

    return *this;
}

/** In-place multiplication operator */
inline
MultiFloat& MultiFloat::operator*=(const MultiFloat &other)
{
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        x = _mm_mul_ps(x, other.x);
    #else
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            x[i] *= other.x[i];
        }
    #endif

    return *this;
}

/** In-place division operator */
inline
MultiFloat& MultiFloat::operator/=(const MultiFloat &other)
{
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        x = _mm_div_ps(x, other.x);
    #else
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            x[i] /= other.x[i];
        }
    #endif

    return *this;
}

/** Multiply val0 and val1 and add it onto this value */
inline
MultiFloat& MultiFloat::multiplyAdd(const MultiFloat &v0, const MultiFloat &v1)
{
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        x = _mm_add_ps(x, _mm_mul_ps(v0.x, v1.x));
    #else
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            x[i] += v0.x[i] * v1.x[i];
        }
    #endif

    return *this;
}

/** Return the maximum vector between this and other */
inline
MultiFloat MultiFloat::max(const MultiFloat &other) const
{
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        return MultiFloat( _mm_max_ps(x, other.x) );
    #else
        MultiFloat ret;
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            ret.x[i] = std::max(x[i], other.x[i]);
        }
        return ret;
    #endif
}

/** Return the minimum vector between this and other */
inline
MultiFloat MultiFloat::min(const MultiFloat &other) const
{
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        return MultiFloat( _mm_min_ps(x, other.x) );
    #else
        MultiFloat ret;
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            ret.x[i] = std::min(x[i], other.x[i]);
        }
        return ret;
    #endif
}

/** Return the reciprocal of this vector */
inline
MultiFloat MultiFloat::reciprocal() const
{
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        return MultiFloat( _mm_rcp_ps(x) );
    #else
        MultiFloat ret;
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            ret.x[i] = float(1) / x[i];
        }
        return ret;
    #endif
}

/** Return the square root of this vector */
inline
MultiFloat MultiFloat::sqrt() const
{
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        return MultiFloat( _mm_sqrt_ps(x) );
    #else
        MultiFloat ret;
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            ret.x[i] = std::sqrt(x[i]);
        }
        return ret;
    #endif
}

/** Return the recipical square root of this vector */
inline
MultiFloat MultiFloat::rsqrt() const
{
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        return MultiFloat( _mm_rsqrt_ps(x) );
    #else
        MultiFloat ret;
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            ret.x[i] = float(1) / std::sqrt(x[i]);
        }
        return ret;
    #endif
}

/** Rotate this vector. This moves each element one space to the left, moving the
    first element to the last element */
inline
MultiFloat MultiFloat::rotate() const
{
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        // there must be an SSE intrinsic to rotate left...
        float tmp[4];
    
        tmp[0] = *( ((const float*)&x) + 1 );
        tmp[1] = *( ((const float*)&x) + 2 );
        tmp[2] = *( ((const float*)&x) + 3 );
        tmp[3] = *( ((const float*)&x) );
    
        return MultiFloat( _mm_set_ps(tmp[0], tmp[1], tmp[2], tmp[3]) );
    #else
        MultiFloat ret;
        ret.x[0] = x[1];
        ret.x[1] = x[2];
        ret.x[2] = x[3];
        ret.x[3] = x[0];
        return ret;
    #endif
}

/** Return the sum of all elements of this vector */
inline
float MultiFloat::sum() const
{
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        return *( ((const float*)&x) ) +
               *( ((const float*)&x) + 1 ) +
               *( ((const float*)&x) + 2 ) +
               *( ((const float*)&x) + 3 );
    #else
        float sum = 0;
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            sum += x[i];
        }
        return sum;
    #endif
}

#endif // #ifndef SIRE_SKIP_INLINE_FUNCTIONS

}

SIRE_END_HEADER

#endif
