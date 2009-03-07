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

#ifndef SIREBASE_CHUNKEDVECTOR_HPP
#define SIREBASE_CHUNKEDVECTOR_HPP

#include <QVector>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireBase
{
template<class T, int N>
class ChunkedVector;
}

template<class T, int N>
QDataStream& operator<<(QDataStream&, const ChunkedVector<T,N>&);
template<class T, int N>
QDataStream& operator>>(QDataStream&, ChunkedVector<T,N>&);

namespace SireBase
{

/** This class provides a vector that is expected to hold a large
    number of objects, and which is expected to have individual
    objects changed (and so would need to be reallocated a lot)
    
    This breaks up the big vector into chunks of size 'N'
    
    @author Christopher Woods
*/
template<class T, int N=100>
class SIREBASE_EXPORT ChunkedVector
{

friend QDataStream& ::operator<<<>(QDataStream&, const ChunkedVector<T,N>&);
friend QDataStream& ::operator>><>(QDataStream&, ChunkedVector<T,N>&);

public:
    ChunkedVector();
    ChunkedVector(int size);
    ChunkedVector(int size, const T &value);
    
    ChunkedVector(const ChunkedVector<T,N> &other);
    
    ~ChunkedVector();

    ChunkedVector<T,N>& operator=(const ChunkedVector<T,N> &other);
    
    bool operator==(const ChunkedVector<T,N> &other) const;
    bool operator!=(const ChunkedVector<T,N> &other) const;

    T& operator[](int i);
    const T& operator[](int i) const;
    
    const T& at(int i) const;
    
    T value(int i) const;
    T value(int i, const T &default_value) const;
    
    void append(const T &value);
    
    int count() const;
    int count(const T &value) const;
    
    int size() const;
    
    int nChunks() const;
    static int chunkSize();

    T* data(int i);
    const T* data(int i) const;

    const T* constData(int i) const;

    void resize(int count);

    QList<T> toList() const;
    QVector<T> toVector() const;
    std::vector<T> toStdVector() const;

    void squeeze();
    void reserve(int count);

    static ChunkedVector<T,N> fromVector(const QVector<T> &vector);
    static ChunkedVector<T,N> fromList(const QList<T> &list);
    static ChunkedVector<T,N> fromStdVector(const std::vector<T> &vector);

private:
    /** The array of arrays - the large vector is broken
        up into vectors of length 'N' */
    QVector< QVector<T> > _chunks;
};

/** Empty constructor */
template<class T, int N>
SIRE_OUTOFLINE_TEMPLATE
ChunkedVector<T,N>::ChunkedVector()
{}

/** Construct a chunked vector of size 'size' */
template<class T, int N>
SIRE_OUTOFLINE_TEMPLATE
ChunkedVector<T,N>::ChunkedVector(int size)
{
    if (size <= 0)
        return;

    int nchunks = size / N;
    int nremainder = size % N;
    
    if (remainder > 0)
    {
        this->_chunks = QVector< QVector<T> >(nchunks+1, QVector<T>(N));
        this->_chunks[nchunks].resize(nremainder);
    }
    else
    {
        this->_chunks = QVector< QVector<T> >(nchunks, QVector<T>(N));
    }
}

/** Construct a chunked vector of size 'size', with all constructed
    values having value 'value' */
template<class T, int N>
SIRE_OUTOFLINE_TEMPLATE
ChunkedVector<T,N>::ChunkedVector(int size, const T &value)
{
    if (size <= 0)
        return;
        
    int nchunks = size / N;
    int remainder = size % N;
    
    if (remainder > 0)
    {
        this->_chunks = QVector< QVector<T> >(nchunks+1, QVector<T>(N,value));
        this->_chunks[nchunks].resize(nremainder);
    }
    else
    {
        this->_chunks = QVector< QVector<T> >(nchunks, QVector<T>(N,value));
    }
}

/** Copy constructor */
template<class T, int N>
SIRE_OUTOFLINE_TEMPLATE
ChunkedVector<T,N>::ChunkedVector(const ChunkedVector<T,N> &other)
                 : _chunks(other._chunks)
{}

/** Destructor */
template<class T, int N>
SIRE_OUTOFLINE_TEMPLATE
ChunkedVector<T,N>::~ChunkedVector()
{}

/** Copy assignment operator */
template<class T, int N>
SIRE_OUTOFLINE_TEMPLATE
ChunkedVector<T,N>& ChunkedVector<T,N>::operator=(const ChunkedVector<T,N> &other)
{
    this->_chunks = other._chunks;
    return *this;
}

/** Comparison operator */
template<class T, int N>
SIRE_OUTOFLINE_TEMPLATE
bool ChunkedVector<T,N>::operator==(const ChunkedVector<T,N> &other) const
{
    return this->_chunks == other._chunks;
}

/** Comparison operator */
template<class T, int N>
SIRE_OUTOFLINE_TEMPLATE
bool ChunkedVector<T,N>::operator!=(const ChunkedVector<T,N> &other) const
{
    return this->_chunks != other._chunks;
}

/** Return a modifiable reference to the ith element */
template<class T, int N>
SIRE_OUTOFLINE_TEMPLATE
T& ChunkedVector<T,N>::operator[](int i)
{
    return this->_chunks[ i / N ][ i % N ];
}

/** Return a const reference to the ith element */
template<class T, int N>
SIRE_OUTOFLINE_TEMPLATE
const T& ChunkedVector<T,N>::operator[](int i) const
{
    return this->_chunks[ i / N ][ i % N ];
}

/** Return a const reference to the ith element */
template<class T, int N>
SIRE_OUTOFLINE_TEMPLATE
const T& ChunkedVector<T,N>::at(int i) const
{
    return this->operator[](i);
}

/** Return the ith element - if i is out of bounds, then it returns
    a default constructed value */
template<class T, int N>
SIRE_OUTOFLINE_TEMPLATE
T ChunkedVector<T,N>::value(int i) const
{
    return this->_chunks.value( i / N ).value( i % N );
}

/** Return the ith element - if i is out of bounds, then it 
    returns 'default_value' */
template<class T, int N>
SIRE_OUTOFLINE_TEMPLATE
T ChunkedVector<T,N>::value(int i, const T &default_value) const
{
    return this->_chunks.value( i / N ).value( i % N, default_value );
}

/** Append the value 'value' onto the end of this vector */
template<class T, int N>
SIRE_OUTOFLINE_TEMPLATE
void ChunkedVector<T,N>::append(const T &value)
{
    int count = this->_chunks.count();
    QVector<T> *chunks_array = this->_chunks.data();
    
    for (int i=0; i<count; ++i)
    {
        if (chunks_array[i].count() < N)
        {
            //this chunk has some space left
            chunks_array[i].append(value);
            return;
        }
    }
    
    //we ran out of chunks - add a new one
    QVector<T> new_chunk;
    new_chunk.reserve(N);
    new_chunk.append(value);
    
    this->_chunks.append( new_chunk );
}

/** Return the number of items in this vector */
template<class T, int N>
SIRE_OUTOFLINE_TEMPLATE
int ChunkedVector<T,N>::count() const
{
    int size = 0;
    int nchunks = this->_chunks.count();
    const QVector<T> *chunks_array = this->_chunks.constData();
    
    for (int i=0; i<nchunks; ++i)
    {
        size += chunks_array[i].count();
    }
    
    return size;
}

/** Return the number of items in this vector that are equal to 'value' */
template<class T, int N>
SIRE_OUTOFLINE_TEMPLATE
int ChunkedVector<T,N>::count(const T &value) const
{
    int size = 0;
    int nchunks = this->_chunks.count();
    const QVector<T> *chunks_array = this->_chunks.constData();
    
    for (int i=0; i<nchunks; ++i)
    {
        size += chunks_array[i].count(value);
    }
    
    return size;
}

/** Return the number of items in this vector */
template<class T, int N>
SIRE_OUTOFLINE_TEMPLATE
int ChunkedVector<T,N>::size() const
{
    return this->count();
}

/** Return the number of chunks in this vector */
template<class T, int N>
SIRE_OUTOFLINE_TEMPLATE
int ChunkedVector<T,N>::nChunks() const
{
    return this->_chunks.count();
}

/** Return the size of a chunk */
template<class T, int N>
SIRE_OUTOFLINE_TEMPLATE
int ChunkedVector<T,N>::chunkSize()
{
    return N;
}

/** Return the raw pointer to the array of the ith chunk */
template<class T, int N>
SIRE_OUTOFLINE_TEMPLATE
T* ChunkedVector<T,N>::data(int i)
{
    return this->_chunks.data()[i].data();
}

/** Return the raw pointer to the array of the ith chunk */
template<class T, int N>
SIRE_OUTOFLINE_TEMPLATE
const T* ChunkedVector<T,N>::data(int i) const
{
    return this->_chunks.data()[i].data();
}

/** Return the raw pointer to the array of the ith chunk */
template<class T, int N>
SIRE_OUTOFLINE_TEMPLATE
const T* ChunkedVector<T,N>::constData(int i) const
{
    return this->_chunks.constData()[i].constData();
}

/** Resize this vector so that it holds 'count' items */
template<class T, int N>
SIRE_OUTOFLINE_TEMPLATE
void ChunkedVector<T,N>::resize(int new_count)
{
    #warning I am getting confused writing this!!!

    int old_count = this->count();

    if (new_count == old_count)
    {
        return;
    }
    else if (new_count <= 0)
    {
        this->operator=( ChunkedVector<T,N>() );
        return;
    }
    else if (this->_chunks.isEmpty() or this->_chunks[0].isEmpty())
    {
        this->operator=( ChunkedVector<T,N>(count) );
        return;
    }

    int new_size = new_count / N;
    int new_remainder = new_count % N;

    int new_nchunks = new_size + 1;
    
    if (new_remainder == 0)
    {
        new_nchunks = new_size;
    }

    int old_nchunks = this->_chunks.count();
    int old_size = nchunks - 1;
    int old_remainder = this->_chunks.last().count();
    
    if (old_remainder == N)
    {
        old_size = nchunks;
        old_remainder = 0;
    }
    
    if (new_remainder == 0)
    {
        this->_chunks.resize(new_size)
        
        if (new_size > old_size)
        {
            for (int i=old_size; i<new_size; ++i)
            {
                this->_chunks[i] = QVector<T>(N);
            }
        }
        else if (new_size == old_size)
        {
            this->_chunks[ this->_chunks.count() - 1 ].resize(N);
        }
    }
    else
    {
        int old_remainder = this->_chunks[old_size-1].count();
        
        if (new_size > old_size)
        {
            this->_chunks[old_size-1].resize(N);
            
            this->_chunks.resize(new_size+1);
            
            for (int i=old_chunks; i<new_size; ++i)
            {
                this->_chunks[i] = QVector<T>(N);
            }
            
            this->_chunks[new_size] = QVector<T>(new_remainder);
        }
        else if (new_size == old_size)
        {
            this->_chunks[old_size-1].resize(new_remainder);
        }
        else
        {
            this->_chunks.resize(new_size+1)
        }
    }
}

template<class T, int N>
SIRE_OUTOFLINE_TEMPLATE
void ChunkedVector<T,N>::squeeze();

template<class T, int N>
SIRE_OUTOFLINE_TEMPLATE
void ChunkedVector<T,N>::reserve(int count);

/* Return this chunked vector as a large vector */
template<class T, int N>
SIRE_OUTOFLINE_TEMPLATE
QVector<T> ChunkedVector<T,N>::toVector() const
{
    int size = this->count();
    
    if (size == 0)
        return QVector<T>();

    QVector<T> vector(size);
    
    const T *vector_array = vector.data();
    
    int k = 0;
    
    int nchunks = this->_chunks.count();
    const QVector<T> *chunks_array = this->_chunks.constData();
    
    for (int i=0; i<nchunks; ++i)
    {
        int n_to_copy = qMin( N, size - k );
        
        const T *old_array = chunks_array[i].constData();
        
        for (int j=0; j<n_to_copy; ++j)
        {
            vector_array[k] = old_array[j];
            ++k;
        }
    }
    
    return vector;
}

/** Return this chunked vector as a list */
template<class T, int N>
SIRE_OUTOFLINE_TEMPLATE
QList<T> ChunkedVector<T,N>::toList() const
{
    return this->toVector().toList();
}

/** Return thus chunked vector as a std::vector */
template<class T, int N>
SIRE_OUTOFLINE_TEMPLATE
std::vector<T> ChunkedVector<T,N>::toStdVector() const
{
    return this->toVector().toStdVector();
}

/** Return a chunked vector that has been constructed from a QVector */
template<class T, int N>
SIRE_OUTOFLINE_TEMPLATE
ChunkedVector<T,N> ChunkedVector<T,N>::fromVector(const QVector<T> &vector)
{
    
}

/** Return a chunked vector that has been constructed from a QList */
template<class T, int N>
SIRE_OUTOFLINE_TEMPLATE
ChunkedVector<T,N> ChunkedVector<T,N>::fromList(const QList<T> &list)
{
    return ChunkedVector<T,N>::fromVector( QVector<T>::fromList(list) );
}

/** Return a chunked vector that has been constructed from a std::vector */
template<class T, int N>
SIRE_OUTOFLINE_TEMPLATE
ChunkedVector<T,N> ChunkedVector<T,N>::fromStdVector(const std::vector<T> &vector)
{
    return ChunkedVector<T,N>::fromVector( QVector<T>::fromStdVector(vector) );
}

}

SIRE_END_HEADER

#endif
