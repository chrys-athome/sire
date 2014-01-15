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

#include "multiuint.h"

#include <QStringList>

#include "SireError/errors.h"

#include <QDebug>

using namespace SireMaths;

static inline bool isAligned32(const void *pointer)
{
    return (quintptr)pointer % size_t(32) == 0;
}

static void assertAligned32(const void *pointer, QString place)
{
/*    if (not isAligned32(pointer))
        throw SireError::program_bug( QObject::tr(
                "An unaligned MultiFloat has been created! %1")
                    .arg((quintptr)pointer % size_t(32)), place );
*/
}

void MultiUInt::assertAligned(const void *ptr, size_t size)
{
/*    if ( (quintptr)ptr % size != 0 )
        throw SireError::program_bug( QObject::tr(
                "An unaligned MultiFloat has been created! %1, %2, %3")
                    .arg((quintptr)ptr)
                    .arg((quintptr)ptr % size)
                    .arg(size), CODELOC );
*/
}

/** Construct from the passed array. If size is greater than MultiUInt::size()
    then an error will be raised. If size is less than MultiUInt::size() then
    this integer will be padded with zeroes */
MultiUInt::MultiUInt(const quint32 *array, int size)
{
    assertAligned();

    if (size > MULTIFLOAT_SIZE)
        throw SireError::unsupported( QObject::tr(
                "Cannot fit an array of size %1 in this MultiUInt, as it is only "
                "capable of holding %2 values...").arg(size).arg(MULTIFLOAT_SIZE), CODELOC );

    if (size <= 0)
    {
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            v.a[i] = 0;
        }
    }
    else if (size == MULTIFLOAT_SIZE)
    {
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            v.a[i] = array[i];
        }
    }
    else
    {
        for (int i=0; i<size; ++i)
        {
            v.a[i] = array[i];
        }
        
        for (int i=size; i<MULTIFLOAT_SIZE; ++i)
        {
            v.a[i] = 0;
        }
    }
}

/** Construct from the passed array - this must be the same size as the vector */
MultiUInt::MultiUInt(const QVector<quint32> &array)
{
    assertAligned();
    this->operator=( MultiUInt(array.constData(), array.size()) );
}

/** Return whether or not this MultiUInt is correctly aligned. If it is not,
    then any SSE operations will fail */
bool MultiUInt::isAligned() const
{
    return true;
}

QVector<MultiUInt> MultiUInt::fromArray(const quint32 *array, int size)
{
    if (size == 0)
        return QVector<MultiUInt>();

    int nvecs = size / MULTIFLOAT_SIZE;
    int nremain = size % MULTIFLOAT_SIZE;

    QVector<MultiUInt> marray(nvecs + ( (nremain > 0) ? 1 : 0 ));
    MultiUInt *a = marray.data();
    
    int idx = 0;
    
    quint32 tmp[MULTIFLOAT_SIZE];
    
    for (int i=0; i<nvecs; ++i)
    {
        for (int j=0; j<MULTIFLOAT_SIZE; ++j)
        {
            tmp[j] = array[idx];
            ++idx;
        }
    
        a[i] = MultiUInt((quint32*)(&tmp), MULTIFLOAT_SIZE);
    }
    
    if (nremain > 0)
    {
        for (int j=0; j<nremain; ++j)
        {
            tmp[j] = array[idx];
            ++idx;
        }
        
        a[marray.count()-1] = MultiUInt((quint32*)(&tmp), nremain);
    }

    assertAligned32(marray.constData(), CODELOC);

    return marray;
}

/** Create an array of MultiFloats from the passed array of doubles. This
    will pad the end of the array with zeroes if necessary */
QVector<MultiUInt> MultiUInt::fromArray(const QVector<quint32> &array)
{
    return MultiUInt::fromArray(array.constData(), array.count());
}

/** Return the passed MultiUInt array converted back into a normal array */
QVector<quint32> MultiUInt::toArray(const QVector<MultiUInt> &array)
{
    if (array.isEmpty())
        return QVector<quint32>();
    
    QVector<quint32> ret;
    ret.reserve( array.count() * MULTIFLOAT_SIZE );
    
    for (int i=0; i<array.count(); ++i)
    {
        const MultiUInt &f = array.constData()[i];
        
        for (int j=0; j<MULTIFLOAT_SIZE; ++j)
        {
            ret.append(f[j]);
        }
    }
    
    return ret;
}

/** Comparison operator - only returns true if all elements are equal */
bool MultiUInt::operator==(const MultiUInt &other) const
{
    for (int i=0; i<MULTIFLOAT_SIZE; ++i)
    {
        if (v.a[i] != other.v.a[i])
            return false;
    }

    return true;
}

/** Comparison operator - only returns true if all elements are not equal */
bool MultiUInt::operator!=(const MultiUInt &other) const
{
    return not operator==(other);
}

/** Return whether all of the elements of this MultiUInt are
    equal to 0x00000000 (e.g. every bit in the entire vector is 0) */
bool MultiUInt::isBinaryZero() const
{
    for (int i=0; i<MULTIFLOAT_SIZE; ++i)
    {
        static const quint32 bin_zero = 0x00000000;
    
        if (v.a[i] != bin_zero)
            return false;
    }
    
    return true;
}

/** Return whether all of the elements of this MultiUInt are
    not equal to 0x00000000 (e.g. at least one bit in the entire vector is 1) */
bool MultiUInt::isNotBinaryZero() const
{
    return not isBinaryZero();
}

/** Return whether or not at least one of the elements of this vector
    is binary zero (the float is equal to 0x00000000) */
bool MultiUInt::hasBinaryZero() const
{
    for (int i=0; i<MULTIFLOAT_SIZE; ++i)
    {
        static const quint32 bin_zero = 0x00000000;
    
        if (v.a[i] == bin_zero)
            return true;
    }
    
    return false;
}

/** Return whether all of the elements of this MultiUInt are
    equal to 0xFFFFFFFF (e.g. every bit in the entire vector is 1) */
bool MultiUInt::isBinaryOne() const
{
    for (int i=0; i<MULTIFLOAT_SIZE; ++i)
    {
        static const quint32 bin_one = 0xFFFFFFFF;
    
        if (v.a[i] != bin_one)
            return false;
    }
    
    return true;
}

/** Return whether all of the elements of this MultiUInt are
    not equal to 0xFFFFFFFF (e.g. at least one bit in the entire vector is 0) */
bool MultiUInt::isNotBinaryOne() const
{
    return not isBinaryOne();
}

/** Return whether or not at least one of the elements of this vector
    is binary one (the float is equal to 0xFFFFFFFF) */
bool MultiUInt::hasBinaryOne() const
{
    for (int i=0; i<MULTIFLOAT_SIZE; ++i)
    {
        static const quint32 bin_one = 0xFFFFFFFF;
    
        if (v.a[i] == bin_one)
            return true;
    }
    
    return false;
}

/** Comparison operator - only returns true if all elements are less */
bool MultiUInt::operator<(const MultiUInt &other) const
{
    for (int i=0; i<MULTIFLOAT_SIZE; ++i)
    {
        if (v.a[i] >= other.v.a[i])
            return false;
    }

    return true;
}

/** Comparison operator - only returns true if all elements are greater */
bool MultiUInt::operator>(const MultiUInt &other) const
{
    for (int i=0; i<MULTIFLOAT_SIZE; ++i)
    {
        if (v.a[i] <= other.v.a[i])
            return false;
    }

    return true;
}

/** Comparison operator - only returns true if all elements are less or equal */
bool MultiUInt::operator<=(const MultiUInt &other) const
{
    for (int i=0; i<MULTIFLOAT_SIZE; ++i)
    {
        if (v.a[i] > other.v.a[i])
            return false;
    }

    return true;
}

/** Comparison operator - only returns true if all elements are greater or equal */
bool MultiUInt::operator>=(const MultiUInt &other) const
{
    for (int i=0; i<MULTIFLOAT_SIZE; ++i)
    {
        if (v.a[i] < other.v.a[i])
            return false;
    }

    return true;
}

/** Return the ith value in the multifloat */
quint32 MultiUInt::operator[](int i) const
{
    if (i < 0)
        i = MULTIFLOAT_SIZE + i;
    
    if (i < 0 or i >= MULTIFLOAT_SIZE)
    {
        throw SireError::invalid_index( QObject::tr(
                "Cannot access element %1 of MultiUInt (holds only %2 values)")
                    .arg(i).arg(MULTIFLOAT_SIZE), CODELOC );
    }
    
    return v.a[i];
}

/** Set the ith value of the MultiUInt to 'value' */
void MultiUInt::set(int i, quint32 value)
{
    if (i < 0)
        i = MULTIFLOAT_SIZE + i;
    
    if (i < 0 or i >= MULTIFLOAT_SIZE)
    {
        throw SireError::invalid_index( QObject::tr(
                "Cannot access element %1 of MultiFloat (holds only %2 values)")
                    .arg(i).arg(MULTIFLOAT_SIZE), CODELOC );
    }

    v.a[i] = value;
}

/** Return the ith value in the MultiUInt */
quint32 MultiUInt::get(int i) const
{
    return this->operator[](i);
}

const char* MultiUInt::what() const
{
    return MultiUInt::typeName();
}

const char* MultiUInt::typeName()
{
    return "SireMaths::MultiUInt";
}

QString MultiUInt::toString() const
{
    QStringList vals;
    
    for (int i=0; i<this->count(); ++i)
    {
        vals.append( QString::number(v.a[i]) );
    }
    
    return QObject::tr("{ %1 }").arg(vals.join(", "));
}

QString MultiUInt::toBinaryString() const
{
    QStringList vals;
    
    for (int i=0; i<this->count(); ++i)
    {
        const unsigned char *c = reinterpret_cast<const unsigned char*>(&(v.a[i]));
        
        QString val("0x");
        
        for (unsigned int j=0; j<sizeof(quint32); ++j)
        {
            val.append( QString("%1").arg((unsigned short)(c[j]), 2, 16, QChar('0')) );
        }
        
        vals.append(val);
    }
    
    return QObject::tr("{ %1 }").arg(vals.join(", "));
}
