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

#ifndef SIREMATHS_MULTIINT_H
#define SIREMATHS_MULTIINT_H

#include "multifloat.h"

SIRE_BEGIN_HEADER

namespace SireMaths
{

/** This class provides a vectorised 32bit signed integer. This represents
    a single vector of integers on the compiled machine, e.g.
    4 integers if we use SSE2, 8 integers for AVX/AVX2
    (note that AVX represents it as two SSE vectors)
    
    @author Christopher Woods
*/
class SIREMATHS_EXPORT MultiInt
{
public:
    MultiInt();
    
    MultiInt(qint32 value);
    
    MultiInt(const qint32 *array, int size);
    MultiInt(const QVector<qint32> &array);
    
    MultiInt(const MultiInt &other);
    
    ~MultiInt();
    
    bool isAligned() const;
    
    static QVector<MultiInt> fromArray(const QVector<qint32> &array);
    static QVector<MultiInt> fromArray(const qint32 *array, int size);
    
    static QVector<qint32> toArray(const QVector<MultiInt> &array);
    
    MultiInt& operator=(const MultiInt &other);
    MultiInt& operator=(qint32 value);
    
    bool operator==(const MultiInt &other) const;
    bool operator!=(const MultiInt &other) const;
    
    bool operator<(const MultiInt &other) const;
    bool operator>(const MultiInt &other) const;
    
    bool operator<=(const MultiInt &other) const;
    bool operator>=(const MultiInt &other) const;
    
    MultiInt compareEqual(const MultiInt &other) const;
    MultiInt compareNotEqual(const MultiInt &other) const;

    MultiInt compareLess(const MultiInt &other) const;
    MultiInt compareGreater(const MultiInt &other) const;
    
    MultiInt compareLessEqual(const MultiInt &other) const;
    MultiInt compareGreaterEqual(const MultiInt &other) const;
    
    MultiFloat reinterpretCastToFloat() const;
    
    const char* what() const;
    static const char* typeName();
    
    QString toString() const;
    QString toBinaryString() const;
    
    static int size();
    static int count();
    
    qint32 operator[](int i) const;
    
    void set(int i, qint32 value);
    qint32 get(int i) const;
    
    MultiInt operator-() const;
    
    MultiInt operator+(const MultiInt &other) const;
    MultiInt operator-(const MultiInt &other) const;
    
    MultiInt& operator+=(const MultiInt &other);
    MultiInt& operator-=(const MultiInt &other);
    
    MultiInt operator!() const;
    MultiInt operator&(const MultiInt &other) const;
    MultiInt operator|(const MultiInt &other) const;
    MultiInt operator^(const MultiInt &other) const;

    MultiInt& operator&=(const MultiInt &other);
    MultiInt& operator|=(const MultiInt &other);
    MultiInt& operator^=(const MultiInt &other);

    MultiInt logicalNot() const;
    
    MultiInt logicalAnd(const MultiInt &other) const;
    MultiInt logicalAndNot(const MultiInt &other) const;
    
    MultiInt logicalOr(const MultiInt &other) const;
    MultiInt logicalXor(const MultiInt &other) const;
    
    MultiInt max(const MultiInt &other) const;
    MultiInt min(const MultiInt &other) const;
    
    MultiInt rotate() const;
    
    bool isBinaryZero() const;
    bool isNotBinaryZero() const;
    bool hasBinaryZero() const;
    
    bool isBinaryOne() const;
    bool isNotBinaryOne() const;
    bool hasBinaryOne() const;
    
    qint32 sum() const;
    qint64 doubleSum() const;

private:
    /* Make the other Multi?? classes friends, so that they
       can read the vector data directly */
    friend class MultiFloat;
    friend class MultiUInt;

    static void assertAligned(const void *ptr, size_t size);

    #ifndef SIRE_SKIP_INLINE_FUNCTIONS

        #ifndef MULTIFLOAT_CHECK_ALIGNMENT
            void assertAligned(){}
        #endif

        #ifdef MULTIFLOAT_AVX_IS_AVAILABLE
            union
            {
                __m128 x[2];
                float a[8];
            } v;
            #define MULTIFLOAT_SIZE 8
        
            #ifndef SIRE_SKIP_INLINE_FUNCTIONS
            MultiInt(__m128 val0, __m128 val1)
            {
                v.x[0] = val0;
                v.x[1] = val1;
            }
                #ifdef MULTIFLOAT_CHECK_ALIGNMENT
                    void assertAligned()
                    {
                        if (quintptr(this) % 16 != 0)
                            assertAligned(this, 16);
                    }
                #endif
            #endif
        #else
        #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
            union
            {
                __m128i x;
                qint32 a[4];
            } v;

            #ifndef SIRE_SKIP_INLINE_FUNCTIONS
            MultiInt(__m128i sse_val)
            {
                v.x = sse_val;
            }
                #ifdef MULTIFLOAT_CHECK_ALIGNMENT
                    void assertAligned()
                    {
                        if (quintptr(this) % 16 != 0)
                            assertAligned(this, 16);
                    }
                #endif
            #endif
        #else
            union
            {
                qint32 a[MULTIFLOAT_SIZE];
            } v;
            #define MULTIINT_BINONE getBinaryOne()

            #ifndef SIRE_SKIP_INLINE_FUNCTIONS
            static qint32 getBinaryOne()
            {
                const quint32 x = 0xFFFFFFFF;
                return *(reinterpret_cast<const qint32*>(&x));
            }
                #ifdef MULTIFLOAT_CHECK_ALIGNMENT
                    void assertAligned()
                    {
                        if (quintptr(this) % 32 != 0)
                            assertAligned(this, 32);
                    }
                #endif
            #endif
        #endif
        #endif
    #endif

};

#ifndef SIRE_SKIP_INLINE_FUNCTIONS

/** Constructor. This creates a MultiInt with an undefined initial state */
inline
MultiInt::MultiInt()
{
    assertAligned();

    #ifdef MULTIFLOAT_AVX_IS_AVAILABLE
        v.x[0] = _mm_set1_epi32(0);
        v.x[1] = _mm_set1_epi32(0);
    #else
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        v.x = _mm_set1_epi32(0);
    #else
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            v.a[i] = 0;
        }
    #endif
    #endif
}

/** Construct a MultiFloat with all values equal to 'val' */
inline
MultiInt::MultiInt(qint32 val)
{
    assertAligned();

    #ifdef MULTIFLOAT_AVX_IS_AVAILABLE
        v.x[0] = _mm_set1_epi32(val);
        v.x[1] = _mm_set1_epi32(val);
    #else
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        v.x = _mm_set1_epi32(val);
    #else
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            v.a[i] = val;
        }
    #endif
    #endif
}

/** Copy constructor */
inline
MultiInt::MultiInt(const MultiInt &other)
{
    #ifdef MULTIFLOAT_AVX_IS_AVAILABLE
        v.x[0] = other.v.x[0];
        v.x[1] = other.v.x[1];
    #else
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        v.x = other.v.x;
    #else
       for (int i=0; i<MULTIFLOAT_SIZE; ++i)
       {
           v.a[i] = other.v.a[i];
       }
    #endif
    #endif
}

/** Assignment operator */
inline
MultiInt& MultiInt::operator=(const MultiInt &other)
{
    if (this != &other)
    {
        #ifdef MULTIFLOAT_AVX_IS_AVAILABLE
            v.x[0] = other.v.x[0];
            v.x[1] = other.v.x[1];
        #else
        #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
            v.x = other.v.x;
        #else
            for (int i=0; i<MULTIFLOAT_SIZE; ++i)
            {
                v.a[i] = other.v.a[i];
            }
        #endif
        #endif
    }
    
    return *this;
}

/** Assignment operator */
inline
MultiInt& MultiInt::operator=(qint32 value)
{
    #ifdef MULTIFLOAT_AVX_IS_AVAILABLE
        v.x[0] = _mm_set1_epi32(value);
        v.x[1] = _mm_set1_epi32(value);
    #else
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        v.x = _mm_set1_epi32(value);
    #else
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            v.a[i] = value;
        }
    #endif
    #endif
    
    return *this;
}

/** Destructor */
inline
MultiInt::~MultiInt()
{}

/** Comparison operator. This will return a MultiFloat with elements
    set to zero for each float that is not equal */
inline
MultiInt MultiInt::compareEqual(const MultiInt &other) const
{
    #ifdef MULTIFLOAT_AVX_IS_AVAILABLE
        return MultiInt( _mm_cmpeq_epi32(v.x[0], other.v.x[0]),
                         _mm_cmpeq_epi32(v.x[1], other.v.x[1]) );
    #else
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        return MultiInt( _mm_cmpeq_epi32(v.x, other.v.x) );
    #else
        MultiInt ret;

        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            ret.v.a[i] = (v.a[i] == other.v.a[i]) ? MULTIINT_BINONE : 0x0;
        }
    
        return ret;
    #endif
    #endif
}

/** Not equals comparison operator */
inline
MultiInt MultiInt::compareNotEqual(const MultiInt &other) const
{
    #ifdef MULTIFLOAT_AVX_IS_AVAILABLE
        return MultiInt( reinterpret_cast<__m128i>(_mm_cmpneq_ps(
                            reinterpret_cast<__m128>(v.x[0]),
                            reinterpret_cast<__m128>(other.v.x[0]) )),
                         reinterpret_cast<__m128i>(_mm_cmpneq_ps(
                            reinterpret_cast<__m128>(v.x[1]),
                            reinterpret_cast<__m128>(other.v.x[1]) )) );
    #else
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        return MultiInt( reinterpret_cast<__m128i>(_mm_cmpneq_ps(
                            reinterpret_cast<__m128>(v.x),
                            reinterpret_cast<__m128>(other.v.x) )) );
    #else
        MultiInt ret;
    
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            ret.v.a[i] = (v.a[i] != other.v.a[i]) ? MULTIINT_BINONE : 0x0;
        }
    
        return ret;
    #endif
    #endif
}

/** Less than comparison operator */
inline
MultiInt MultiInt::compareLess(const MultiInt &other) const
{
    #ifdef MULTIFLOAT_AVX_IS_AVAILABLE
        return MultiInt( _mm_cmplt_epi32(v.x[0], other.v.x[0]),
                         _mm_cmplt_epi32(v.x[1], other.v.x[1]) );
    #else
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        return MultiInt( _mm_cmplt_epi32(v.x, other.v.x) );
    #else
        MultiInt ret;
    
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            ret.v.a[i] = (v.a[i] < other.v.a[i]) ? MULTIINT_BINONE : 0x0;
        }
    
        return ret;
    #endif
    #endif
}

/** Reintepret cast this MultiInt to a MultiFloat. This is only useful if you
    are going to use this to perform bitwise comparisons */
inline
MultiFloat MultiInt::reinterpretCastToFloat() const
{
    #ifdef MULTIFLOAT_AVX_IS_AVAILABLE
        return MultiFloat(0);
    #else
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        return MultiFloat( *(reinterpret_cast<const __m128*>(&v.x)) );
    #else
        MultiFloat ret;
    
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            ret.v.a[i] = *(reinterpret_cast<const float*>(&v.a[i]));
        }
    
        return ret;
    #endif
    #endif
}

/** Greater than comparison operator */
inline
MultiInt MultiInt::compareGreater(const MultiInt &other) const
{
    #ifdef MULTIFLOAT_AVX_IS_AVAILABLE
        return MultiInt( _mm_cmpgt_epi32(v.x[0], other.v.x[0]),
                         _mm_cmpgt_epi32(v.x[1], other.v.x[1]) );
    #else
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        return MultiInt( _mm_cmpgt_epi32(v.x, other.v.x) );
    #else
        MultiInt ret;
    
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            ret.v.a[i] = (v.a[i] > other.v.a[i]) ? MULTIINT_BINONE : 0x0;
        }
    
        return ret;
    #endif
    #endif
}

/** Less than or equal comparison */
inline
MultiInt MultiInt::compareLessEqual(const MultiInt &other) const
{
    #ifdef MULTIFLOAT_AVX_IS_AVAILABLE
        return MultiInt( _mm_or_si128( _mm_cmplt_epi32(v.x[0], other.v.x[0]),
                                       _mm_cmpeq_epi32(v.x[0], other.v.x[0]) ),
                         _mm_or_si128( _mm_cmplt_epi32(v.x[1], other.v.x[1]),
                                       _mm_cmpeq_epi32(v.x[1], other.v.x[1]) ));
    #else
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        return MultiInt( _mm_or_si128( _mm_cmplt_epi32(v.x, other.v.x),
                                       _mm_cmpeq_epi32(v.x, other.v.x) ) );
    #else
        MultiInt ret;
    
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            ret.v.a[i] = (v.a[i] <= other.v.a[i]) ? MULTIINT_BINONE : 0x0;
        }
    
        return ret;
    #endif
    #endif
}

/** Greater than or equal comparison */
inline
MultiInt MultiInt::compareGreaterEqual(const MultiInt &other) const
{
    #ifdef MULTIFLOAT_AVX_IS_AVAILABLE
        return MultiInt( _mm_or_si128( _mm_cmpgt_epi32(v.x[0], other.v.x[0]),
                                       _mm_cmpeq_epi32(v.x[0], other.v.x[0]) ),
                         _mm_or_si128( _mm_cmpgt_epi32(v.x[1], other.v.x[1]),
                                       _mm_cmpeq_epi32(v.x[1], other.v.x[1]) ));
    #else
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        return MultiInt( _mm_or_si128( _mm_cmpgt_epi32(v.x, other.v.x),
                                       _mm_cmpeq_epi32(v.x, other.v.x) ) );
    #else
        MultiInt ret;
    
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            ret.v.a[i] = (v.a[i] >= other.v.a[i]) ? MULTIINT_BINONE : 0x0;
        }
    
        return ret;
    #endif
    #endif
}

/** Return the number of values in the vector */
inline
int MultiInt::size()
{
    return MULTIFLOAT_SIZE;
}

/** Return the number of values in the vector */
inline
int MultiInt::count()
{
    return MULTIFLOAT_SIZE;
}

/** Addition operator */
inline
MultiInt MultiInt::operator+(const MultiInt &other) const
{
    #ifdef MULTIFLOAT_AVX_IS_AVAILABLE
        return MultiInt( _mm_add_epi32(v.x[0], other.v.x[0]),
                         _mm_add_epi32(v.x[1], other.v.x[1]) );
    #else
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        return MultiInt( _mm_add_epi32(v.x, other.v.x) );
    #else
        MultiInt ret;
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            ret.v.a[i] = v.a[i] + other.v.a[i];
        }
        return ret;
    #endif
    #endif
}

/** Subtraction operator */
inline
MultiInt MultiInt::operator-(const MultiInt &other) const
{
    #ifdef MULTIFLOAT_AVX_IS_AVAILABLE
        return MultiInt( _mm_sub_epi32(v.x[0], other.v.x[0]),
                         _mm_sub_epi32(v.x[1], other.v.x[1]) );
    #else
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        return MultiInt( _mm_sub_epi32(v.x, other.v.x) );
    #else
        MultiInt ret;
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            ret.v.a[i] = v.a[i] - other.v.a[i];
        }
        return ret;
    #endif
    #endif
}

/** In-place addition operator */
inline
MultiInt& MultiInt::operator+=(const MultiInt &other)
{
    #ifdef MULTIFLOAT_AVX_IS_AVAILABLE
        v.x[0] = _mm_add_epi32(v.x[0], other.v.x[0]);
        v.x[1] = _mm_add_epi32(v.x[1], other.v.x[1]);
    #else
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        v.x = _mm_add_epi32(v.x, other.v.x);
    #else
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            v.a[i] += other.v.a[i];
        }
    #endif
    #endif

    return *this;
}

/** In-place subtraction operator */
inline
MultiInt& MultiInt::operator-=(const MultiInt &other)
{
    #ifdef MULTIFLOAT_AVX_IS_AVAILABLE
        v.x[0] = _mm_sub_epi32(v.x[0], other.v.x[0]);
        v.x[1] = _mm_sub_epi32(v.x[1], other.v.x[1]);
    #else
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        v.x = _mm_sub_epi32(v.x, other.v.x);
    #else
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            v.a[i] -= other.v.a[i];
        }
    #endif
    #endif

    return *this;
}

/** Bitwise logical "and" comparison */
inline
MultiInt MultiInt::logicalAnd(const MultiInt &other) const
{
    #ifdef MULTIFLOAT_AVX_IS_AVAILABLE
        return MultiInt( _mm_and_si128(v.x[0], other.v.x[1]),
                         _mm_and_si128(v.x[0], other.v.x[1]) );
    #else
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        return MultiInt( _mm_and_si128(v.x, other.v.x) );
    #else
        MultiInt ret;
    
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            unsigned char *ret_char_v = reinterpret_cast<unsigned char*>(&(ret.v.a[i]));
            const unsigned char *char_v = reinterpret_cast<const unsigned char*>(&(v.a[i]));
            const unsigned char *other_char_v
                        = reinterpret_cast<const unsigned char*>(&(other.v.a[i]));

            for (unsigned int j=0; j<sizeof(qint32); ++j)
            {
                ret_char_v[j] = char_v[j] & other_char_v[j];
            }
        }
    
        return ret;
    #endif
    #endif
}

/** Bitwise logical "and" comparison */
inline
MultiFloat MultiFloat::logicalAnd(const MultiInt &other) const
{
    #ifdef MULTIFLOAT_AVX_IS_AVAILABLE
        return MultiFloat(0);
    #else
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        return MultiFloat( _mm_and_ps(v.x, *(reinterpret_cast<const __m128*>(&(other.v.x)))) );
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
    #endif
}

/** Bitwise logical "and not" */
inline
MultiInt MultiInt::logicalAndNot(const MultiInt &other) const
{
    #ifdef MULTIFLOAT_AVX_IS_AVAILABLE
        return MultiInt( _mm_andnot_si128(v.x[0], other.v.x[0]),
                         _mm_andnot_si128(v.x[0], other.v.x[0]) );
    #else
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        return MultiInt( _mm_andnot_si128(v.x, other.v.x) );
    #else
        MultiInt ret;
    
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            unsigned char *ret_char_v = reinterpret_cast<unsigned char*>(&(ret.v.a[i]));
            const unsigned char *char_v = reinterpret_cast<const unsigned char*>(&(v.a[i]));
            const unsigned char *other_char_v
                        = reinterpret_cast<const unsigned char*>(&(other.v.a[i]));

            for (unsigned int j=0; j<sizeof(qint32); ++j)
            {
                ret_char_v[j] = !(char_v[j] & other_char_v[j]);
            }
        }
    
        return ret;
    #endif
    #endif
}

/** Bitwise logical "and not" (this is *this and (not other)) */
inline
MultiFloat MultiFloat::logicalAndNot(const MultiInt &other) const
{
    #ifdef MULTIFLOAT_AVX_IS_AVAILABLE
        const __m256 val = *(reinterpret_cast<const __m256*>(&other.v.x));
        return MultiFloat( _mm256_andnot_ps(val, v.x) );
    #else
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        const __m128 val = *(reinterpret_cast<const __m128*>(&other.v.x));
        return MultiFloat( _mm_andnot_ps(val, v.x) );
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
    #endif
}

/** Bitwise logical or operator */
inline
MultiInt MultiInt::logicalOr(const MultiInt &other) const
{
    #ifdef MULTIFLOAT_AVX_IS_AVAILABLE
        return MultiInt( _mm_or_si128(v.x[0], other.v.x[0]),
                         _mm_or_si128(v.x[1], other.v.x[1]) );
    #else
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        return MultiInt( _mm_or_si128(v.x, other.v.x) );
    #else
        MultiInt ret;
    
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            unsigned char *ret_char_v = reinterpret_cast<unsigned char*>(&(ret.v.a[i]));
            const unsigned char *char_v = reinterpret_cast<const unsigned char*>(&(v.a[i]));
            const unsigned char *other_char_v
                        = reinterpret_cast<const unsigned char*>(&(other.v.a[i]));

            for (unsigned int j=0; j<sizeof(qint32); ++j)
            {
                ret_char_v[j] = char_v[j] | other_char_v[j];
            }
        }
    
        return ret;
    #endif
    #endif
}

/** Bitwise logical xor */
inline
MultiInt MultiInt::logicalXor(const MultiInt &other) const
{
    #ifdef MULTIFLOAT_AVX_IS_AVAILABLE
        return MultiInt( _mm_xor_si128(v.x[0], other.v.x[1]),
                         _mm_xor_si128(v.x[0], other.v.x[1]) );
    #else
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        return MultiInt( _mm_xor_si128(v.x, other.v.x) );
    #else
        MultiInt ret;
    
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            unsigned char *ret_char_v = reinterpret_cast<unsigned char*>(&(ret.v.a[i]));
            const unsigned char *char_v = reinterpret_cast<const unsigned char*>(&(v.a[i]));
            const unsigned char *other_char_v
                        = reinterpret_cast<const unsigned char*>(&(other.v.a[i]));

            for (unsigned int j=0; j<sizeof(qint32); ++j)
            {
                ret_char_v[j] = char_v[j] ^ other_char_v[j];
            }
        }
    
        return ret;
    #endif
    #endif
}

/** Logical not operator */
inline
MultiInt MultiInt::logicalNot() const
{
    MultiInt ret;

    for (int i=0; i<MULTIFLOAT_SIZE; ++i)
    {
        unsigned char *ret_char_v = reinterpret_cast<unsigned char*>(&(ret.v.a[i]));
        const unsigned char *char_v = reinterpret_cast<const unsigned char*>(&(v.a[i]));

        for (unsigned int j=0; j<sizeof(qint32); ++j)
        {
            ret_char_v[j] = !char_v[j];
        }
    }

    return ret;
}

/** Logical not operator */
inline
MultiInt MultiInt::operator!() const
{
    return this->logicalNot();
}

/** Logical and operator */
inline
MultiInt MultiInt::operator&(const MultiInt &other) const
{
    return this->logicalAnd(other);
}

/** Logical or operator */
inline
MultiInt MultiInt::operator|(const MultiInt &other) const
{
    return this->logicalOr(other);
}

/** Logical xor operator */
inline
MultiInt MultiInt::operator^(const MultiInt &other) const
{
    return this->logicalXor(other);
}

/** In place logical and */
inline
MultiInt& MultiInt::operator&=(const MultiInt &other)
{
    #ifdef MULTIFLOAT_AVX_IS_AVAILABLE
        v.x[0] = _mm_and_si128(v.x[0], other.v.x[0]);
        v.x[1] = _mm_and_si128(v.x[1], other.v.x[1]);
    #else
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        v.x = _mm_and_si128(v.x, other.v.x);
    #else
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            unsigned char *char_v = reinterpret_cast<unsigned char*>(&(v.a[i]));
            const unsigned char *other_char_v
                        = reinterpret_cast<const unsigned char*>(&(other.v.a[i]));

            for (unsigned int j=0; j<sizeof(qint32); ++j)
            {
                char_v[j] &= other_char_v[j];
            }
        }
    #endif
    #endif

    return *this;
}

/** In place logical and */
inline
MultiFloat& MultiFloat::operator&=(const MultiInt &other)
{
    #ifdef MULTIFLOAT_AVX_IS_AVAILABLE
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
    #else
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        v.x = _mm_and_ps( v.x, *(reinterpret_cast<const __m128*>(&(other.v.x))) );
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
    #endif

    return *this;
}

/** In-place logical or */
inline
MultiInt& MultiInt::operator|=(const MultiInt &other)
{
    #ifdef MULTIFLOAT_AVX_IS_AVAILABLE
        v.x[0] = _mm_or_si128(v.x[0], other.v.x[0]);
        v.x[1] = _mm_or_si128(v.x[1], other.v.x[1]);
    #else
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        v.x = _mm_or_si128(v.x, other.v.x);
    #else
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            unsigned char *char_v = reinterpret_cast<unsigned char*>(&(v.a[i]));
            const unsigned char *other_char_v
                        = reinterpret_cast<const unsigned char*>(&(other.v.a[i]));

            for (unsigned int j=0; j<sizeof(qint32); ++j)
            {
                char_v[j] |= other_char_v[j];
            }
        }
    #endif
    #endif

    return *this;
}

/** In-place logical xor */
inline
MultiInt& MultiInt::operator^=(const MultiInt &other)
{
    #ifdef MULTIFLOAT_AVX_IS_AVAILABLE
        v.x[0] = _mm_xor_si128(v.x[0], other.v.x[0]);
        v.x[1] = _mm_xor_si128(v.x[1], other.v.x[1]);
    #else
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        v.x = _mm_xor_si128(v.x, other.v.x);
    #else
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            unsigned char *char_v = reinterpret_cast<unsigned char*>(&(v.a[i]));
            const unsigned char *other_char_v
                        = reinterpret_cast<const unsigned char*>(&(other.v.a[i]));

            for (unsigned int j=0; j<sizeof(qint32); ++j)
            {
                char_v[j] ^= other_char_v[j];
            }
        }
    #endif
    #endif

    return *this;
}

/** Return the maximum vector between this and other */
inline
MultiInt MultiInt::max(const MultiInt &other) const
{
    #ifdef MULTIFLOAT_AVX_IS_AVAILABLE
        MultiInt ret;
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            ret.v.a[i] = std::max(v.a[i], other.v.a[i]);
        }
        return ret;
    #else
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        #ifdef MULTIFLOAT_SSE4_IS_AVAILABLE
            return MultiInt( _mm_max_epi32(v.x, other.v.x) );
        #else
            MultiInt ret;
            for (int i=0; i<MULTIFLOAT_SIZE; ++i)
            {
                ret.v.a[i] = std::max(v.a[i], other.v.a[i]);
            }
            return ret;
        #endif
    #else
        MultiInt ret;
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            ret.v.a[i] = std::max(v.a[i], other.v.a[i]);
        }
        return ret;
    #endif
    #endif
}

/** Return the minimum vector between this and other */
inline
MultiInt MultiInt::min(const MultiInt &other) const
{
    #ifdef MULTIFLOAT_AVX_IS_AVAILABLE
        MultiInt ret;
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            ret.v.a[i] = std::min(v.a[i], other.v.a[i]);
        }
        return ret;
    #else
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        #ifdef MULTIFLOAT_SSE4_IS_AVAILABLE
            return MultiInt( _mm_min_epi32(v.x, other.v.x) );
        #else
            MultiInt ret;
            for (int i=0; i<MULTIFLOAT_SIZE; ++i)
            {
                ret.v.a[i] = std::min(v.a[i], other.v.a[i]);
            }
            return ret;
        #endif
    #else
        MultiInt ret;
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            ret.v.a[i] = std::min(v.a[i], other.v.a[i]);
        }
        return ret;
    #endif
    #endif
}

/** Rotate this vector. This moves each element one space to the left, moving the
    first element to the last element */
inline
MultiInt MultiInt::rotate() const
{
    #ifdef MULTIFLOAT_AVX_IS_AVAILABLE
        return MultiInt(0);
    #else
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        // there must be an SSE intrinsic to rotate left...
        return MultiInt( _mm_shuffle_epi32(v.x, _MM_SHUFFLE(0,3,2,1)) );
    #else
        MultiInt ret;
        
        for (int i=1; i<MULTIFLOAT_SIZE; ++i)
        {
            ret.v.a[i-1] = v.a[i];
        }
        
        ret.v.a[MULTIFLOAT_SIZE-1] = v.a[0];

        return ret;
    #endif
    #endif
}

/** Return the sum of all elements of this vector */
inline
qint32 MultiInt::sum() const
{
    qint32 sum = 0;
    for (int i=0; i<MULTIFLOAT_SIZE; ++i)
    {
        sum += v.a[i];
    }
    return sum;
}

/** Return the sum of all elements of this vector, using doubles for the sum */
inline
qint64 MultiInt::doubleSum() const
{
    qint64 sum = 0;
    for (int i=0; i<MULTIFLOAT_SIZE; ++i)
    {
        sum += v.a[i];
    }
    return sum;
}

#endif // #ifndef SIRE_SKIP_INLINE_FUNCTIONS

}

SIRE_EXPOSE_CLASS( SireMaths::MultiInt )

SIRE_END_HEADER

#endif
