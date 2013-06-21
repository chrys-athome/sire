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

#include <QVector>

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
    
    MultiFloat(float value);
    
    MultiFloat(const float *array, int size);
    MultiFloat(const QVector<float> &array);
    MultiFloat(const QVector<double> &array);
    
    MultiFloat(const MultiFloat &other);
    
    ~MultiFloat();
    
    MultiFloat& operator=(const MultiFloat &other);
    
    bool operator==(const MultiFloat &other) const;
    bool operator!=(const MultiFloat &other) const;
    
    bool operator<(const MultiFloat &other) const;
    bool operator>(const MultiFloat &other) const;
    
    bool operator<=(const MultiFloat &other) const;
    bool operator>=(const MultiFloat &other) const;
    
    MultiFloat compareEqual(const MultiFloat &other) const;
    MultiFloat compareNotEqual(const MultiFloat &other) const;

    MultiFloat compareLess(const MultiFloat &other) const;
    MultiFloat compareGreater(const MultiFloat &other) const;
    
    MultiFloat compareLessEqual(const MultiFloat &other) const;
    MultiFloat compareGreaterEqual(const MultiFloat &other) const;
    
    const char* what() const;
    static const char* typeName();
    
    QString toString() const;
    QString toBinaryString() const;
    
    static int size();
    static int count();
    
    float operator[](int i) const;
    
    void set(int i, float value);
    float get(int i) const;
    
    MultiFloat operator-() const;
    
    MultiFloat operator+(const MultiFloat &other) const;
    MultiFloat operator-(const MultiFloat &other) const;
    MultiFloat operator*(const MultiFloat &other) const;
    MultiFloat operator/(const MultiFloat &other) const;
    
    MultiFloat& operator+=(const MultiFloat &other);
    MultiFloat& operator-=(const MultiFloat &other);
    MultiFloat& operator*=(const MultiFloat &other);
    MultiFloat& operator/=(const MultiFloat &other);
    
    MultiFloat operator!() const;
    MultiFloat operator&(const MultiFloat &other) const;
    MultiFloat operator|(const MultiFloat &other) const;
    MultiFloat operator^(const MultiFloat &other) const;

    MultiFloat& operator&=(const MultiFloat &other);
    MultiFloat& operator|=(const MultiFloat &other);
    MultiFloat& operator^=(const MultiFloat &other);

    MultiFloat logicalNot() const;
    
    MultiFloat logicalAnd(const MultiFloat &other) const;
    MultiFloat logicalAndNot(const MultiFloat &other) const;
    
    MultiFloat logicalOr(const MultiFloat &other) const;
    MultiFloat logicalXor(const MultiFloat &other) const;
    
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
        union
        {
            __m128 x;
            float a[4];
        } v;
        #define MULTIFLOAT_SIZE 4

        #ifndef SIRE_SKIP_INLINE_FUNCTIONS
        MultiFloat(__m128 sse_val)
        {
            v.x = sse_val;
        }
        #endif
    #else
        union
        {
            float a[4];
        } v;
        #define MULTIFLOAT_SIZE 4
        #define MULTIFLOAT_BINONE getBinaryOne()

        #ifndef SIRE_SKIP_INLINE_FUNCTIONS
        static float getBinaryOne()
        {
            const unsigned int x = 0xFFFFFFFF;
            return *(reinterpret_cast<const float*>(&x));
        }
        #endif
    #endif
};

#ifndef SIRE_SKIP_INLINE_FUNCTIONS

/** Comparison operator. This will return a MultiFloat with elements
    set to zero for each float that is not equal */
inline
MultiFloat MultiFloat::compareEqual(const MultiFloat &other) const
{
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        return MultiFloat( _mm_cmpeq_ps(v.x, other.v.x) );
    #else
        MultiFloat ret;

        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            ret.v.a[i] = (v.a[i] == other.v.a[i]) ? MULTIFLOAT_BINONE : 0x0;
        }
    
        return ret;
    #endif
}

/** Not equals comparison operator */
inline
MultiFloat MultiFloat::compareNotEqual(const MultiFloat &other) const
{
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        return MultiFloat( _mm_cmpneq_ps(v.x, other.v.x) );
    #else
        MultiFloat ret;
    
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            ret.v.a[i] = (v.a[i] != other.v.a[i]) ? MULTIFLOAT_BINONE : 0x0;
        }
    
        return ret;
    #endif
}

/** Less than comparison operator */
inline
MultiFloat MultiFloat::compareLess(const MultiFloat &other) const
{
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        return MultiFloat( _mm_cmplt_ps(v.x, other.v.x) );
    #else
        MultiFloat ret;
    
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            ret.v.a[i] = (v.a[i] < other.v.a[i]) ? MULTIFLOAT_BINONE : 0x0;
        }
    
        return ret;
    #endif
}

/** Greater than comparison operator */
inline
MultiFloat MultiFloat::compareGreater(const MultiFloat &other) const
{
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        return MultiFloat( _mm_cmpgt_ps(v.x, other.v.x) );
    #else
        MultiFloat ret;
    
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            ret.v.a[i] = (v.a[i] > other.v.a[i]) ? MULTIFLOAT_BINONE : 0x0;
        }
    
        return ret;
    #endif
}

/** Less than or equal comparison */
inline
MultiFloat MultiFloat::compareLessEqual(const MultiFloat &other) const
{
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        return MultiFloat( _mm_cmple_ps(v.x, other.v.x) );
    #else
        MultiFloat ret;
    
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            ret.v.a[i] = (v.a[i] <= other.v.a[i]) ? MULTIFLOAT_BINONE : 0x0;
        }
    
        return ret;
    #endif
}

/** Greater than or equal comparison */
inline
MultiFloat MultiFloat::compareGreaterEqual(const MultiFloat &other) const
{
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        return MultiFloat( _mm_cmpge_ps(v.x, other.v.x) );
    #else
        MultiFloat ret;
    
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            ret.v.a[i] = (v.a[i] >= other.v.a[i]) ? MULTIFLOAT_BINONE : 0x0;
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
        return MultiFloat( _mm_add_ps(v.x, other.v.x) );
    #else
        MultiFloat ret;
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            ret.v.a[i] = v.a[i] + other.v.a[i];
        }
        return ret;
    #endif
}

/** Subtraction operator */
inline
MultiFloat MultiFloat::operator-(const MultiFloat &other) const
{
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        return MultiFloat( _mm_sub_ps(v.x, other.v.x) );
    #else
        MultiFloat ret;
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            ret.v.a[i] = v.a[i] - other.v.a[i];
        }
        return ret;
    #endif
}

/** Multiplication operator */
inline
MultiFloat MultiFloat::operator*(const MultiFloat &other) const
{
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        return MultiFloat( _mm_mul_ps(v.x, other.v.x) );
    #else
        MultiFloat ret;
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            ret.v.a[i] = v.a[i] * other.v.a[i];
        }
        return ret;
    #endif
}

/** Division operator */
inline
MultiFloat MultiFloat::operator/(const MultiFloat &other) const
{
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        return MultiFloat( _mm_div_ps(v.x, other.v.x) );
    #else
        MultiFloat ret;
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            ret.v.a[i] = v.a[i] / other.v.a[i];
        }
        return ret;
    #endif
}

/** In-place addition operator */
inline
MultiFloat& MultiFloat::operator+=(const MultiFloat &other)
{
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        v.x = _mm_add_ps(v.x, other.v.x);
    #else
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            v.a[i] += other.v.a[i];
        }
    #endif

    return *this;
}

/** In-place subtraction operator */
inline
MultiFloat& MultiFloat::operator-=(const MultiFloat &other)
{
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        v.x = _mm_sub_ps(v.x, other.v.x);
    #else
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            v.a[i] -= other.v.a[i];
        }
    #endif

    return *this;
}

/** In-place multiplication operator */
inline
MultiFloat& MultiFloat::operator*=(const MultiFloat &other)
{
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        v.x = _mm_mul_ps(v.x, other.v.x);
    #else
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            v.a[i] *= other.v.a[i];
        }
    #endif

    return *this;
}

/** In-place division operator */
inline
MultiFloat& MultiFloat::operator/=(const MultiFloat &other)
{
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        v.x = _mm_div_ps(v.x, other.v.x);
    #else
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            v.a[i] /= other.v.a[i];
        }
    #endif

    return *this;
}

/** Bitwise logical "and" comparison */
inline
MultiFloat MultiFloat::logicalAnd(const MultiFloat &other) const
{
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        return MultiFloat( _mm_and_ps(v.x, other.v.x) );
    #else
        MultiFloat ret;
    
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            unsigned char *ret_char_v = reinterpret_cast<unsigned char*>(&(ret.v.a[i]));
            const unsigned char *char_v = reinterpret_cast<const unsigned char*>(&(v.a[i]));
            const unsigned char *other_char_v
                        = reinterpret_cast<const unsigned char*>(&(other.v.a[i]));

            for (unsigned int j=0; j<sizeof(float); ++j)
            {
                ret_char_v[j] = char_v[j] & other_char_v[j];
            }
        }
    
        return ret;
    #endif
}

/** Bitwise logical "and not" */
inline
MultiFloat MultiFloat::logicalAndNot(const MultiFloat &other) const
{
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        return MultiFloat( _mm_andnot_ps(v.x, other.v.x) );
    #else
        MultiFloat ret;
    
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            unsigned char *ret_char_v = reinterpret_cast<unsigned char*>(&(ret.v.a[i]));
            const unsigned char *char_v = reinterpret_cast<const unsigned char*>(&(v.a[i]));
            const unsigned char *other_char_v
                        = reinterpret_cast<const unsigned char*>(&(other.v.a[i]));

            for (unsigned int j=0; j<sizeof(float); ++j)
            {
                ret_char_v[j] = !(char_v[j] & other_char_v[j]);
            }
        }
    
        return ret;
    #endif
}

/** Bitwise logical or operator */
inline
MultiFloat MultiFloat::logicalOr(const MultiFloat &other) const
{
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        return MultiFloat( _mm_or_ps(v.x, other.v.x) );
    #else
        MultiFloat ret;
    
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            unsigned char *ret_char_v = reinterpret_cast<unsigned char*>(&(ret.v.a[i]));
            const unsigned char *char_v = reinterpret_cast<const unsigned char*>(&(v.a[i]));
            const unsigned char *other_char_v
                        = reinterpret_cast<const unsigned char*>(&(other.v.a[i]));

            for (unsigned int j=0; j<sizeof(float); ++j)
            {
                ret_char_v[j] = char_v[j] | other_char_v[j];
            }
        }
    
        return ret;
    #endif
}

/** Bitwise logical xor */
inline
MultiFloat MultiFloat::logicalXor(const MultiFloat &other) const
{
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        return MultiFloat( _mm_xor_ps(v.x, other.v.x) );
    #else
        MultiFloat ret;
    
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            unsigned char *ret_char_v = reinterpret_cast<unsigned char*>(&(ret.v.a[i]));
            const unsigned char *char_v = reinterpret_cast<const unsigned char*>(&(v.a[i]));
            const unsigned char *other_char_v
                        = reinterpret_cast<const unsigned char*>(&(other.v.a[i]));

            for (unsigned int j=0; j<sizeof(float); ++j)
            {
                ret_char_v[j] = char_v[j] ^ other_char_v[j];
            }
        }
    
        return ret;
    #endif
}

/** Logical not operator */
inline
MultiFloat MultiFloat::logicalNot() const
{
    MultiFloat ret;

    for (int i=0; i<MULTIFLOAT_SIZE; ++i)
    {
        unsigned char *ret_char_v = reinterpret_cast<unsigned char*>(&(ret.v.a[i]));
        const unsigned char *char_v = reinterpret_cast<const unsigned char*>(&(v.a[i]));

        for (unsigned int j=0; j<sizeof(float); ++j)
        {
            ret_char_v[j] = !char_v[j];
        }
    }

    return ret;
}

/** Logical not operator */
inline
MultiFloat MultiFloat::operator!() const
{
    return this->logicalNot();
}

/** Logical and operator */
inline
MultiFloat MultiFloat::operator&(const MultiFloat &other) const
{
    return this->logicalAnd(other);
}

/** Logical or operator */
inline
MultiFloat MultiFloat::operator|(const MultiFloat &other) const
{
    return this->logicalOr(other);
}

/** Logical xor operator */
inline
MultiFloat MultiFloat::operator^(const MultiFloat &other) const
{
    return this->logicalXor(other);
}

/** In place logical and */
inline
MultiFloat& MultiFloat::operator&=(const MultiFloat &other)
{
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        v.x = _mm_and_ps(v.x, other.v.x);
    #else
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            unsigned char *char_v = reinterpret_cast<unsigned char*>(&(v.a[i]));
            const unsigned char *other_char_v
                        = reinterpret_cast<const unsigned char*>(&(other.v.a[i]));

            for (unsigned int j=0; j<sizeof(float); ++j)
            {
                char_v[j] &= other_char_v[j];
            }
        }
    #endif

    return *this;
}

/** In-place logical or */
inline
MultiFloat& MultiFloat::operator|=(const MultiFloat &other)
{
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        v.x = _mm_or_ps(v.x, other.v.x);
    #else
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            unsigned char *char_v = reinterpret_cast<unsigned char*>(&(v.a[i]));
            const unsigned char *other_char_v
                        = reinterpret_cast<const unsigned char*>(&(other.v.a[i]));

            for (unsigned int j=0; j<sizeof(float); ++j)
            {
                char_v[j] |= other_char_v[j];
            }
        }
    #endif

    return *this;
}

/** In-place logical xor */
inline
MultiFloat& MultiFloat::operator^=(const MultiFloat &other)
{
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        v.x = _mm_xor_ps(v.x, other.v.x);
    #else
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            unsigned char *char_v = reinterpret_cast<unsigned char*>(&(v.a[i]));
            const unsigned char *other_char_v
                        = reinterpret_cast<const unsigned char*>(&(other.v.a[i]));

            for (unsigned int j=0; j<sizeof(float); ++j)
            {
                char_v[j] ^= other_char_v[j];
            }
        }
    #endif

    return *this;
}

/** Multiply val0 and val1 and add it onto this value */
inline
MultiFloat& MultiFloat::multiplyAdd(const MultiFloat &v0, const MultiFloat &v1)
{
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        v.x = _mm_add_ps(v.x, _mm_mul_ps(v0.v.x, v1.v.x));
    #else
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            v.a[i] += v0.v.a[i] * v1.v.a[i];
        }
    #endif

    return *this;
}

/** Return the maximum vector between this and other */
inline
MultiFloat MultiFloat::max(const MultiFloat &other) const
{
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        return MultiFloat( _mm_max_ps(v.x, other.v.x) );
    #else
        MultiFloat ret;
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            ret.v.a[i] = std::max(v.a[i], other.v.a[i]);
        }
        return ret;
    #endif
}

/** Return the minimum vector between this and other */
inline
MultiFloat MultiFloat::min(const MultiFloat &other) const
{
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        return MultiFloat( _mm_min_ps(v.x, other.v.x) );
    #else
        MultiFloat ret;
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            ret.v.a[i] = std::min(v.a[i], other.v.a[i]);
        }
        return ret;
    #endif
}

/** Return the reciprocal of this vector */
inline
MultiFloat MultiFloat::reciprocal() const
{
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        return MultiFloat( _mm_rcp_ps(v.x) );
    #else
        MultiFloat ret;
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            ret.v.a[i] = float(1) / v.a[i];
        }
        return ret;
    #endif
}

/** Return the square root of this vector */
inline
MultiFloat MultiFloat::sqrt() const
{
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        return MultiFloat( _mm_sqrt_ps(v.x) );
    #else
        MultiFloat ret;
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            ret.v.a[i] = std::sqrt(v.a[i]);
        }
        return ret;
    #endif
}

/** Return the recipical square root of this vector */
inline
MultiFloat MultiFloat::rsqrt() const
{
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        return MultiFloat( _mm_rsqrt_ps(v.x) );
    #else
        MultiFloat ret;
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            ret.v.a[i] = float(1) / std::sqrt(v.a[i]);
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
        return MultiFloat( _mm_set_ps(v.a[0], v.a[3], v.a[2], v.a[1]) );
    #else
        MultiFloat ret;
        ret.v.a[0] = v.a[1];
        ret.v.a[1] = v.a[2];
        ret.v.a[2] = v.a[3];
        ret.v.a[3] = v.a[0];
        return ret;
    #endif
}

/** Return the sum of all elements of this vector */
inline
float MultiFloat::sum() const
{
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        return v.a[0] + v.a[1] + v.a[2] + v.a[3];
    #else
        float sum = 0;
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            sum += v.a[i];
        }
        return sum;
    #endif
}

#endif // #ifndef SIRE_SKIP_INLINE_FUNCTIONS

}

SIRE_EXPOSE_CLASS( SireMaths::MultiFloat )

SIRE_END_HEADER

#endif
