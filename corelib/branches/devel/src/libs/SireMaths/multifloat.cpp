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

#include <QStringList>

#include "SireError/errors.h"

using namespace SireMaths;

/** Constructor. This initialises the float to 0 */
MultiFloat::MultiFloat()
{
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        v.x = _mm_set1_ps(0);
    #else
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            v.a[i] = 0;
        }
    #endif
}

/** Construct a MultiFloat with all values equal to 'val' */
MultiFloat::MultiFloat(float val)
{
    #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
        v.x = _mm_set1_ps(val);
    #else
        for (int i=0; i<MULTIFLOAT_SIZE; ++i)
        {
            v.a[i] = val;
        }
    #endif
}

/** Construct from the passed array. If size is greater than MultiFloat::size()
    then an error will be raised. If size is less than MultiFloat::size() then
    this float will be padded with zeroes */
MultiFloat::MultiFloat(const float *array, int size)
{
    if (size > MULTIFLOAT_SIZE)
        throw SireError::unsupported( QObject::tr(
                "Cannot fit an array of size %1 in this MultiFloat, as it is only "
                "capable of holding %2 values...").arg(size).arg(MULTIFLOAT_SIZE), CODELOC );

    if (size <= 0)
    {
        #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
            v.x = _mm_set1_ps(0);
        #else
            for (int i=0; i<MULTIFLOAT_SIZE; ++i)
            {
                v.a[i] = 0;
            }
        #endif
    }
    else if (size == MULTIFLOAT_SIZE)
    {
        #ifdef MULTIFLOAT_SSE_IS_AVAILABLE
            //note that SSE packs things the 'wrong' way around
            v.x = _mm_set_ps(array[3], array[2], array[1], array[0]);
        #else
            for (int i=0; i<MULTIFLOAT_SIZE; ++i)
            {
                v.a[i] = array[i];
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
            //note that sse packs things the 'wrong' way around
            v.x = _mm_set_ps(tmp[3], tmp[2], tmp[1], tmp[0]);
        #else
            for (int i=0; i<MULTIFLOAT_SIZE; ++i)
            {
                v.a[i] = tmp[i];
            }
        #endif
    }
}

/** Construct from the passed array - this must be the same size as the vector */
MultiFloat::MultiFloat(const QVector<float> &array)
{
    this->operator=( MultiFloat(array.constData(), array.size()) );
}

/** Construct from the passed array - this must be the same size as the vector */
MultiFloat::MultiFloat(const QVector<double> &array)
{
    QVector<float> farray;
    farray.reserve(array.count());
    
    for (int i=0; i<array.count(); ++i)
    {
        farray.append(array.constData()[i]);
    }

    this->operator=( MultiFloat(farray) );
}

/** Copy constructor */
MultiFloat::MultiFloat(const MultiFloat &other)
{
    for (int i=0; i<MULTIFLOAT_SIZE; ++i)
    {
        v.a[i] = other.v.a[i];
    }
}

/** Assignment operator */
MultiFloat& MultiFloat::operator=(const MultiFloat &other)
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

/** Destructor */
MultiFloat::~MultiFloat()
{}

/** Create an array of MultiFloats from the passed array of doubles. This
    will pad the end of the array with zeroes if necessary */
QVector<MultiFloat> MultiFloat::fromArray(const QVector<double> &array)
{
    if (array.isEmpty())
        return QVector<MultiFloat>();

    float tmp[MULTIFLOAT_SIZE];
    
    QVector<MultiFloat> marray;
    
    int nvecs = array.count() / MULTIFLOAT_SIZE;
    int nremain = array.count() % MULTIFLOAT_SIZE;
    
    marray.reserve(nvecs + ( (nremain == 1) ? 1 : 0 ));
    
    int idx = 0;
    
    for (int i=0; i<nvecs; ++i)
    {
        for (int j=0; j<MULTIFLOAT_SIZE; ++j)
        {
            tmp[j] = array.constData()[idx];
            ++idx;
        }
    
        marray.append( MultiFloat((float*)(&tmp), MULTIFLOAT_SIZE) );
    }
    
    if (nremain > 0)
    {
        for (int j=0; j<nremain; ++j)
        {
            tmp[j] = array.constData()[idx];
            ++idx;
        }
        
        marray.append( MultiFloat((float*)(&tmp), nremain) );
    }
    
    return marray;
}

/** Create an array of MultiFloats from the passed array of floats. This will
    pad the end of the array with zeroes if necessary */
QVector<MultiFloat> MultiFloat::fromArray(const QVector<float> &array)
{
    if (array.isEmpty())
        return QVector<MultiFloat>();

    QVector<MultiFloat> marray;
    
    int nvecs = array.count() / MULTIFLOAT_SIZE;
    int nremain = array.count() % MULTIFLOAT_SIZE;
    
    marray.reserve(nvecs + ( (nremain == 1) ? 1 : 0 ));
    
    int idx = 0;
    
    for (int i=0; i<nvecs; ++i)
    {
        marray.append( MultiFloat((float*)(&(array.constData()[idx])), MULTIFLOAT_SIZE) );
        idx += MULTIFLOAT_SIZE;
    }
    
    if (nremain > 0)
    {
        marray.append( MultiFloat((float*)(&(array.constData()[idx])), nremain) );
    }
    
    return marray;
}

/** Return the passed MultiFloat converted back into a normal array */
QVector<float> MultiFloat::toArray(const QVector<MultiFloat> &array)
{
    if (array.isEmpty())
        return QVector<float>();
    
    QVector<float> ret;
    ret.reserve( array.count() * MULTIFLOAT_SIZE );
    
    for (int i=0; i<array.count(); ++i)
    {
        const MultiFloat &f = array.constData()[i];
        
        for (int j=0; j<MULTIFLOAT_SIZE; ++j)
        {
            ret.append(f[j]);
        }
    }
    
    return ret;
}

/** Return the passed MultiFloat converted back into a normal array of doubles */
QVector<double> MultiFloat::toDoubleArray(const QVector<MultiFloat> &array)
{
    if (array.isEmpty())
        return QVector<double>();
    
    QVector<double> ret;
    ret.reserve( array.count() * MULTIFLOAT_SIZE );
    
    for (int i=0; i<array.count(); ++i)
    {
        const MultiFloat &f = array.constData()[i];
        
        for (int j=0; j<MULTIFLOAT_SIZE; ++j)
        {
            ret.append(f[j]);
        }
    }
    
    return ret;
}

/** Comparison operator - only returns true if all elements are equal */
bool MultiFloat::operator==(const MultiFloat &other) const
{
    for (int i=0; i<MULTIFLOAT_SIZE; ++i)
    {
        if (v.a[i] != other.v.a[i])
            return false;
    }

    return true;
}

/** Comparison operator - only returns true if all elements are not equal */
bool MultiFloat::operator!=(const MultiFloat &other) const
{
    for (int i=0; i<MULTIFLOAT_SIZE; ++i)
    {
        if (v.a[i] == other.v.a[i])
            return false;
    }

    return true;
}

/** Comparison operator - only returns true if all elements are less */
bool MultiFloat::operator<(const MultiFloat &other) const
{
    for (int i=0; i<MULTIFLOAT_SIZE; ++i)
    {
        if (v.a[i] >= other.v.a[i])
            return false;
    }

    return true;
}

/** Comparison operator - only returns true if all elements are greater */
bool MultiFloat::operator>(const MultiFloat &other) const
{
    for (int i=0; i<MULTIFLOAT_SIZE; ++i)
    {
        if (v.a[i] <= other.v.a[i])
            return false;
    }

    return true;
}

/** Comparison operator - only returns true if all elements are less or equal */
bool MultiFloat::operator<=(const MultiFloat &other) const
{
    for (int i=0; i<MULTIFLOAT_SIZE; ++i)
    {
        if (v.a[i] > other.v.a[i])
            return false;
    }

    return true;
}

/** Comparison operator - only returns true if all elements are greater or equal */
bool MultiFloat::operator>=(const MultiFloat &other) const
{
    for (int i=0; i<MULTIFLOAT_SIZE; ++i)
    {
        if (v.a[i] < other.v.a[i])
            return false;
    }

    return true;
}

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
    
    return v.a[i];
}

/** Negative operator */
MultiFloat MultiFloat::operator-() const
{
    MultiFloat ret;
    
    for (int i=0; i<MULTIFLOAT_SIZE; ++i)
    {
        ret.v.a[i] = -v.a[i];
    }
    
    return ret;
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

    v.a[i] = value;
}

/** Return the ith value in the multifloat */
float MultiFloat::get(int i) const
{
    return this->operator[](i);
}

const char* MultiFloat::what() const
{
    return MultiFloat::typeName();
}

const char* MultiFloat::typeName()
{
    return "SireMaths::MultiFloat";
}

QString MultiFloat::toString() const
{
    QStringList vals;
    
    for (int i=0; i<this->count(); ++i)
    {
        vals.append( QString::number(v.a[i]) );
    }
    
    return QObject::tr("{ %1 }").arg(vals.join(", "));
}

QString MultiFloat::toBinaryString() const
{
    QStringList vals;
    
    for (int i=0; i<this->count(); ++i)
    {
        const unsigned char *c = reinterpret_cast<const unsigned char*>(&(v.a[i]));
        
        QString val("0x");
        
        for (unsigned int j=0; j<sizeof(float); ++j)
        {
            val.append( QString("%1").arg((unsigned short)(c[j]), 2, 16, QChar('0')) );
        }
        
        vals.append(val);
    }
    
    return QObject::tr("{ %1 }").arg(vals.join(", "));
}
