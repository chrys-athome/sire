/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2014  Christopher Woods
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

#ifndef SIREMATHS_MULTIUINT_H
#define SIREMATHS_MULTIUINT_H

#include "multifloat.h"

SIRE_BEGIN_HEADER

namespace SireMaths
{

/** This class provides a vectorised unsigned 32 bit integer. This represents
    a single vector of integers on the compiled machine, e.g.
    4 integers if we use SSE2, 8 integers for AVX
    
    @author Christopher Woods
*/
class SIREMATHS_EXPORT MultiUInt
{
public:
    MultiUInt();
    
    MultiUInt(quint32 value);
    
    MultiUInt(const quint32 *array, int size);
    MultiUInt(const QVector<quint32> &array);
    
    MultiUInt(const MultiUInt &other);
    
    ~MultiUInt();
    
    bool isAligned() const;
    
    static QVector<MultiUInt> fromArray(const QVector<quint32> &array);
    
    static QVector<MultiUInt> fromArray(const quint32 *array, int size);
    
    static QVector<quint32> toArray(const QVector<MultiUInt> &array);
    
    MultiUInt& operator=(const MultiUInt &other);
    MultiUInt& operator=(quint32 value);
    
    bool operator==(const MultiUInt &other) const;
    bool operator!=(const MultiUInt &other) const;
    
    bool operator<(const MultiUInt &other) const;
    bool operator>(const MultiUInt &other) const;
    
    bool operator<=(const MultiUInt &other) const;
    bool operator>=(const MultiUInt &other) const;
    
    MultiUInt compareEqual(const MultiUInt &other) const;
    MultiUInt compareNotEqual(const MultiUInt &other) const;

    MultiUInt compareLess(const MultiUInt &other) const;
    MultiUInt compareGreater(const MultiUInt &other) const;
    
    MultiUInt compareLessEqual(const MultiUInt &other) const;
    MultiUInt compareGreaterEqual(const MultiUInt &other) const;
    
    const char* what() const;
    static const char* typeName();
    
    QString toString() const;
    QString toBinaryString() const;
    
    static int size();
    static int count();
    
    quint32 operator[](int i) const;
    
    void set(int i, quint32 value);
    quint32 get(int i) const;
    
    MultiUInt operator+(const MultiUInt &other) const;
    MultiUInt operator-(const MultiUInt &other) const;
    MultiUInt operator*(const MultiUInt &other) const;
    MultiUInt operator/(const MultiUInt &other) const;
    MultiUInt operator%(const MultiUInt &other) const;
    
    MultiUInt& operator+=(const MultiUInt &other);
    MultiUInt& operator-=(const MultiUInt &other);
    MultiUInt& operator*=(const MultiUInt &other);
    MultiUInt& operator/=(const MultiUInt &other);
    
    MultiUInt operator!() const;
    MultiUInt operator&(const MultiUInt &other) const;
    MultiUInt operator|(const MultiUInt &other) const;
    MultiUInt operator^(const MultiUInt &other) const;

    MultiUInt& operator&=(const MultiUInt &other);
    MultiUInt& operator|=(const MultiUInt &other);
    MultiUInt& operator^=(const MultiUInt &other);

    MultiUInt logicalNot() const;
    
    MultiUInt logicalAnd(const MultiUInt &other) const;
    MultiUInt logicalAndNot(const MultiUInt &other) const;
    
    MultiUInt logicalOr(const MultiUInt &other) const;
    MultiUInt logicalXor(const MultiUInt &other) const;
    
    MultiUInt& multiplyAdd(const MultiUInt &val0, const MultiUInt &val1);
    
    MultiUInt max(const MultiUInt &other) const;
    MultiUInt min(const MultiUInt &other) const;
    
    MultiUInt rotate() const;
    
    bool isBinaryZero() const;
    bool isNotBinaryZero() const;
    bool hasBinaryZero() const;
    
    bool isBinaryOne() const;
    bool isNotBinaryOne() const;
    bool hasBinaryOne() const;
    
    quint32 sum() const;
    quint64 doubleSum() const;

private:
    friend class MultiFloat;

    static void assertAligned(const void *ptr, size_t size);

    #ifndef SIRE_SKIP_INLINE_FUNCTIONS

        #ifndef MULTIFLOAT_CHECK_ALIGNMENT
            void assertAligned(){}
        #endif

        union
        {
            quint32 a[MULTIFLOAT_SIZE];
        } v;
        #define MULTIUINT_BINONE getBinaryOne()

        #ifndef SIRE_SKIP_INLINE_FUNCTIONS
        static quint32 getBinaryOne()
        {
            const quint32 x = 0xFFFFFFFF;
            return x;
        }
            #ifdef MULTIFLOAT_CHECK_ALIGNMENT
                void assertAligned()
                {
                    /*if (quintptr(this) % 32 != 0)
                        assertAligned(this, 32);*/
                }
            #endif
        #endif
    #endif
};

#ifndef SIRE_SKIP_INLINE_FUNCTIONS

/** Constructor. This creates a MultiUInt with an undefined initial state */
inline
MultiUInt::MultiUInt()
{
    assertAligned();

    for (int i=0; i<MULTIFLOAT_SIZE; ++i)
    {
        v.a[i] = 0;
    }
}

/** Construct a MultiUInt with all values equal to 'val' */
inline
MultiUInt::MultiUInt(quint32 val)
{
    assertAligned();

    for (int i=0; i<MULTIFLOAT_SIZE; ++i)
    {
        v.a[i] = val;
    }
}

/** Copy constructor */
inline
MultiUInt::MultiUInt(const MultiUInt &other)
{
   for (int i=0; i<MULTIFLOAT_SIZE; ++i)
   {
       v.a[i] = other.v.a[i];
   }
}

/** Assignment operator */
inline
MultiUInt& MultiUInt::operator=(const MultiUInt &other)
{
    if (this != &other)
    {
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            v.a[i] = other.v.a[i];
        }
    }
    
    return *this;
}

/** Assignment operator */
inline
MultiUInt& MultiUInt::operator=(quint32 value)
{
    for (int i=0; i<MULTIFLOAT_SIZE; ++i)
    {
        v.a[i] = value;
    }
    
    return *this;
}

/** Destructor */
inline
MultiUInt::~MultiUInt()
{}

/** Comparison operator. This will return a MultiUInt with elements
    set to zero for each integer that is not equal */
inline
MultiUInt MultiUInt::compareEqual(const MultiUInt &other) const
{
    MultiUInt ret;

    for (int i=0; i<MULTIFLOAT_SIZE; ++i)
    {
        ret.v.a[i] = (v.a[i] == other.v.a[i]) ? MULTIUINT_BINONE : 0x0;
    }

    return ret;
}

/** Not equals comparison operator */
inline
MultiUInt MultiUInt::compareNotEqual(const MultiUInt &other) const
{
    MultiUInt ret;

    for (int i=0; i<MULTIFLOAT_SIZE; ++i)
    {
        ret.v.a[i] = (v.a[i] != other.v.a[i]) ? MULTIUINT_BINONE : 0x0;
    }

    return ret;
}

/** Less than comparison operator */
inline
MultiUInt MultiUInt::compareLess(const MultiUInt &other) const
{
    MultiUInt ret;

    for (int i=0; i<MULTIFLOAT_SIZE; ++i)
    {
        ret.v.a[i] = (v.a[i] < other.v.a[i]) ? MULTIUINT_BINONE : 0x0;
    }

    return ret;
}

/** Greater than comparison operator */
inline
MultiUInt MultiUInt::compareGreater(const MultiUInt &other) const
{
    MultiUInt ret;

    for (int i=0; i<MULTIFLOAT_SIZE; ++i)
    {
        ret.v.a[i] = (v.a[i] > other.v.a[i]) ? MULTIUINT_BINONE : 0x0;
    }

    return ret;
}

/** Less than or equal comparison */
inline
MultiUInt MultiUInt::compareLessEqual(const MultiUInt &other) const
{
    MultiUInt ret;

    for (int i=0; i<MULTIFLOAT_SIZE; ++i)
    {
        ret.v.a[i] = (v.a[i] <= other.v.a[i]) ? MULTIUINT_BINONE : 0x0;
    }

    return ret;
}

/** Greater than or equal comparison */
inline
MultiUInt MultiUInt::compareGreaterEqual(const MultiUInt &other) const
{
    MultiUInt ret;

    for (int i=0; i<MULTIFLOAT_SIZE; ++i)
    {
        ret.v.a[i] = (v.a[i] >= other.v.a[i]) ? MULTIUINT_BINONE : 0x0;
    }

    return ret;
}

/** Return the number of values in the vector */
inline
int MultiUInt::size()
{
    return MULTIFLOAT_SIZE;
}

/** Return the number of values in the vector */
inline
int MultiUInt::count()
{
    return MULTIFLOAT_SIZE;
}

/** Addition operator */
inline
MultiUInt MultiUInt::operator+(const MultiUInt &other) const
{
    MultiUInt ret;
    for (int i=0; i<MULTIFLOAT_SIZE; ++i)
    {
        ret.v.a[i] = v.a[i] + other.v.a[i];
    }
    return ret;
}

/** Subtraction operator */
inline
MultiUInt MultiUInt::operator-(const MultiUInt &other) const
{
    MultiUInt ret;
    for (int i=0; i<MULTIFLOAT_SIZE; ++i)
    {
        ret.v.a[i] = v.a[i] - other.v.a[i];
    }
    return ret;
}

/** Multiplication operator */
inline
MultiUInt MultiUInt::operator*(const MultiUInt &other) const
{
    MultiUInt ret;
    for (int i=0; i<MULTIFLOAT_SIZE; ++i)
    {
        ret.v.a[i] = v.a[i] * other.v.a[i];
    }
    return ret;
}

/** Division operator */
inline
MultiUInt MultiUInt::operator/(const MultiUInt &other) const
{
    MultiUInt ret;
    for (int i=0; i<MULTIFLOAT_SIZE; ++i)
    {
        ret.v.a[i] = v.a[i] / other.v.a[i];
    }
    return ret;
}

/** Remainder operator */
inline
MultiUInt MultiUInt::operator%(const MultiUInt &other) const
{
    MultiUInt ret;
    for (int i=0; i<MULTIFLOAT_SIZE; ++i)
    {
        ret.v.a[i] = v.a[i] % other.v.a[i];
    }
    return ret;
}

/** In-place addition operator */
inline
MultiUInt& MultiUInt::operator+=(const MultiUInt &other)
{
    for (int i=0; i<MULTIFLOAT_SIZE; ++i)
    {
        v.a[i] += other.v.a[i];
    }

    return *this;
}

/** In-place subtraction operator */
inline
MultiUInt& MultiUInt::operator-=(const MultiUInt &other)
{
    for (int i=0; i<MULTIFLOAT_SIZE; ++i)
    {
        v.a[i] -= other.v.a[i];
    }

    return *this;
}

/** In-place multiplication operator */
inline
MultiUInt& MultiUInt::operator*=(const MultiUInt &other)
{
    for (int i=0; i<MULTIFLOAT_SIZE; ++i)
    {
        v.a[i] *= other.v.a[i];
    }

    return *this;
}

/** In-place division operator */
inline
MultiUInt& MultiUInt::operator/=(const MultiUInt &other)
{
    for (int i=0; i<MULTIFLOAT_SIZE; ++i)
    {
        v.a[i] /= other.v.a[i];
    }

    return *this;
}

/** Bitwise logical "and" comparison */
inline
MultiUInt MultiUInt::logicalAnd(const MultiUInt &other) const
{
    MultiUInt ret;

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
}

/** Bitwise logical "and not" */
inline
MultiUInt MultiUInt::logicalAndNot(const MultiUInt &other) const
{
    MultiUInt ret;

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
}

/** Bitwise logical or operator */
inline
MultiUInt MultiUInt::logicalOr(const MultiUInt &other) const
{
    MultiUInt ret;

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
}

/** Bitwise logical xor */
inline
MultiUInt MultiUInt::logicalXor(const MultiUInt &other) const
{
    MultiUInt ret;

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
}

/** Logical not operator */
inline
MultiUInt MultiUInt::logicalNot() const
{
    MultiUInt ret;

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
MultiUInt MultiUInt::operator!() const
{
    return this->logicalNot();
}

/** Logical and operator */
inline
MultiUInt MultiUInt::operator&(const MultiUInt &other) const
{
    return this->logicalAnd(other);
}

/** Logical or operator */
inline
MultiUInt MultiUInt::operator|(const MultiUInt &other) const
{
    return this->logicalOr(other);
}

/** Logical xor operator */
inline
MultiUInt MultiUInt::operator^(const MultiUInt &other) const
{
    return this->logicalXor(other);
}

/** In place logical and */
inline
MultiUInt& MultiUInt::operator&=(const MultiUInt &other)
{
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

    return *this;
}

/** In-place logical or */
inline
MultiUInt& MultiUInt::operator|=(const MultiUInt &other)
{
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

    return *this;
}

/** In-place logical xor */
inline
MultiUInt& MultiUInt::operator^=(const MultiUInt &other)
{
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

    return *this;
}

/** Multiply val0 and val1 and add it onto this value */
inline
MultiUInt& MultiUInt::multiplyAdd(const MultiUInt &v0, const MultiUInt &v1)
{
    for (int i=0; i<MULTIFLOAT_SIZE; ++i)
    {
        v.a[i] += v0.v.a[i] * v1.v.a[i];
    }

    return *this;
}

/** Return the maximum vector between this and other */
inline
MultiUInt MultiUInt::max(const MultiUInt &other) const
{
    MultiUInt ret;
    for (int i=0; i<MULTIFLOAT_SIZE; ++i)
    {
        ret.v.a[i] = std::max(v.a[i], other.v.a[i]);
    }
    return ret;
}

/** Return the minimum vector between this and other */
inline
MultiUInt MultiUInt::min(const MultiUInt &other) const
{
    MultiUInt ret;
    for (int i=0; i<MULTIFLOAT_SIZE; ++i)
    {
        ret.v.a[i] = std::min(v.a[i], other.v.a[i]);
    }
    return ret;
}

/** Rotate this vector. This moves each element one space to the left, moving the
    first element to the last element */
inline
MultiUInt MultiUInt::rotate() const
{
    MultiUInt ret;
    
    for (int i=1; i<MULTIFLOAT_SIZE; ++i)
    {
        ret.v.a[i-1] = v.a[i];
    }
    
    ret.v.a[MULTIFLOAT_SIZE-1] = v.a[0];

    return ret;
}

/** Return the sum of all elements of this vector */
inline
quint32 MultiUInt::sum() const
{
    quint32 sum = 0;
    for (int i=0; i<MULTIFLOAT_SIZE; ++i)
    {
        sum += v.a[i];
    }
    return sum;
}

/** Return the sum of all elements of this vector, using 64 bit integers for the sum */
inline
quint64 MultiUInt::doubleSum() const
{
    quint64 sum = 0;
    for (int i=0; i<MULTIFLOAT_SIZE; ++i)
    {
        sum += v.a[i];
    }
    return sum;
}

inline MultiFloat MultiFloat::logicalAnd(const MultiUInt &other) const
{
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
}

#endif // #ifndef SIRE_SKIP_INLINE_FUNCTIONS

}

SIRE_EXPOSE_CLASS( SireMaths::MultiUInt )

SIRE_END_HEADER

#endif
