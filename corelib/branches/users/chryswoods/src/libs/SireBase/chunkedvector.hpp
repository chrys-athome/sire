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

#include <QDebug>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireBase
{

template<class T, int N> class ChunkedVector<T,N>;

namespace detail
{
void ChunkedVector_throwOutOfRangeError(int i, int n);

template<class T, int N>
const void* get_shared_container_pointer(const SireBase::ChunkedVector<T,N>&);

}

/** This class provides a vector that is expected to hold a large
    number of objects, and which is expected to have individual
    objects changed (and so would need to be reallocated a lot)
    
    This breaks up the big vector into chunks of size 'N'
    
    @author Christopher Woods
*/
template<class T, int N=100>
class SIREBASE_EXPORT ChunkedVector
{

friend const void* 
SireBase::detail::get_shared_container_pointer<>(const ChunkedVector<T,N>&);

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

    void remove(int i);
    void remove(int i, int count);
    
    int count() const;
    int count(const T &value) const;
    
    int size() const;
    
    int capacity() const;
    
    void resize(int count);

    void clear();

    QList<T> toList() const;
    QVector<T> toVector() const;
    std::vector<T> toStdVector() const;

    void squeeze();
    void reserve(int count);

    const void* shareKey() const;

    static ChunkedVector<T,N> fromVector(const QVector<T> &vector);
    static ChunkedVector<T,N> fromList(const QList<T> &list);
    static ChunkedVector<T,N> fromStdVector(const std::vector<T> &vector);

private:
    void assertValidIndex(int i) const;

    int nChunks() const;
    int nFullChunks() const;
    int nRemainder() const;

    /** The array of arrays - the large vector is broken
        up into vectors of length 'N' */
    QVector< QVector<T> > _chunks;
};

#ifndef SIRE_SKIP_INLINE_FUNCTIONS

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
    
    QVector<T> full_chunk(N);
    full_chunk.squeeze();
    
    if (nremainder > 0)
    {
        this->_chunks = QVector< QVector<T> >(nchunks+1, full_chunk);
        this->_chunks[nchunks].resize(nremainder);
    }
    else
    {
        this->_chunks = QVector< QVector<T> >(nchunks, full_chunk);
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
    int nremainder = size % N;

    QVector<T> full_chunk(N, value);
    
    if (nremainder > 0)
    {
        full_chunk.squeeze();
    
        this->_chunks = QVector< QVector<T> >(nchunks+1, full_chunk);
        this->_chunks[nchunks].resize(nremainder);
    }
    else
    {
        this->_chunks = QVector< QVector<T> >(nchunks, full_chunk);
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

/** Internal function used to assert that an index is valid */
template<class T, int N>
SIRE_OUTOFLINE_TEMPLATE
void ChunkedVector<T,N>::assertValidIndex(int i) const
{
    int c = this->count();

    if (i < 0 or i >= c)
        detail::ChunkedVector_throwOutOfRangeError(i, c);
}

/** Return a modifiable reference to the ith element */
template<class T, int N>
SIRE_OUTOFLINE_TEMPLATE
T& ChunkedVector<T,N>::operator[](int i)
{
    this->assertValidIndex(i);

    return this->_chunks[ i / N ][ i % N ];
}

/** Return a const reference to the ith element */
template<class T, int N>
SIRE_OUTOFLINE_TEMPLATE
const T& ChunkedVector<T,N>::operator[](int i) const
{
    this->assertValidIndex(i);

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

/** Internal class used to get the number of chunks */
template<class T, int N>
SIRE_OUTOFLINE_TEMPLATE
int ChunkedVector<T,N>::nChunks() const
{
    return this->_chunks.count();
}

/** Internal class used to get the number of full chunks 
    (chunks that contain 'N' elements) */
template<class T, int N>
SIRE_OUTOFLINE_TEMPLATE
int ChunkedVector<T,N>::nFullChunks() const
{
    int nchunks = this->_chunks.count();
    const QVector<T> *chunks_array = this->_chunks.constData();
    
    for (int i=nchunks-1; i>=0; --i)
    {
        if (chunks_array[i].count() == N)
            return i+1;
    }

    return 0;
}

/** Internal class used to get the number of elements in the last chunk */
template<class T, int N>
SIRE_OUTOFLINE_TEMPLATE
int ChunkedVector<T,N>::nRemainder() const
{
    int nchunks = this->nChunks();
    int nfullchunks = this->nFullChunks();
    
    if (nfullchunks < nchunks)
        return this->_chunks[nfullchunks].count();
    else
        return 0;
}

/** Append the value 'value' onto the end of this vector */
template<class T, int N>
SIRE_OUTOFLINE_TEMPLATE
void ChunkedVector<T,N>::append(const T &value)
{
    int nchunks = this->nChunks();
    int nfullchunks = this->nFullChunks();
    
    if (nfullchunks < nchunks)
    {
        //there is some space left in the last chunk
        this->_chunks[nfullchunks].append(value);
    }
    else
    {
        //there is no space
        this->_chunks.append( QVector<T>() );
        this->_chunks[nchunks].reserve(N);
        this->_chunks[nchunks].append(value);
    }
}

/** Remove the item at index 'i' from this vector - this can be
    slow (O[n]) */
template<class T, int N>
SIRE_OUTOFLINE_TEMPLATE
void ChunkedVector<T,N>::remove(int i)
{
    if (i > this->count() or i < 0)
        //nothing to remove
        return;

    //get the chunk index of this object
    int chunk_idx = i / N;
    
    int nfullchunks = this->nFullChunks();
    
    if (chunk_idx == nfullchunks)
    {
        //we are removing an item from the last chunk - this is easy
        this->_chunks[chunk_idx].remove( i % N );
    }
    else
    {
        //we are removing from a middle chunk, so have to shift
        //everything along by one...
        int nchunks = this->nChunks();
        
        //remove the object
        this->_chunks[chunk_idx].remove( i % N );
        
        //now go through each chunk moving the first object from the next
        //chunk to the last object of this chunk
        for (int i=chunk_idx; i<nchunks-1; ++i)
        {
            this->_chunks[i].append( this->_chunks[i+1][0] );
            this->_chunks[i+1].remove(0);
        }
        
        //clear out the last chunk if it is now empty
        if (this->_chunks[nchunks-1].isEmpty())
            this->_chunks.remove(nchunks-1);
    }
}

/** Remove 'count' objects from this vector, starting at position 'i'.
    This can be slow (O[n]) */
template<class T, int N>
SIRE_OUTOFLINE_TEMPLATE
void ChunkedVector<T,N>::remove(int i, int count)
{
    for (int j=0; j<count; ++j)
    {
        this->remove(i);
    }
}

/** Return the number of items in this vector */
template<class T, int N>
SIRE_OUTOFLINE_TEMPLATE
int ChunkedVector<T,N>::count() const
{
    int nfullchunks = this->nFullChunks();
    int nremainder = this->nRemainder();
    
    return (nfullchunks * N) + nremainder;
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

/** Return the capacity of this vector (the number of items that can
    be held without requiring more memory) */
template<class T, int N>
SIRE_OUTOFLINE_TEMPLATE
int ChunkedVector<T,N>::capacity() const
{
    int nfullchunks = this->nFullChunks();
    int nchunks = this->nChunks();
    
    int c = nfullchunks * N;
    
    for (int i=nfullchunks; i<nchunks; ++i)
    {
        c += qMin( this->_chunks.constData()[i].capacity(), N );
    }
    
    return c;
}

/** Completely clear this ChunkedVector */
template<class T, int N>
SIRE_OUTOFLINE_TEMPLATE
void ChunkedVector<T,N>::clear()
{
    this->_chunks.clear();
}

/** Resize this vector so that it holds 'count' items */
template<class T, int N>
SIRE_OUTOFLINE_TEMPLATE
void ChunkedVector<T,N>::resize(int new_count)
{
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
        this->operator=( ChunkedVector<T,N>(new_count) );
        return;
    }

    //get the current dimensions of the chunked vector
    int old_nchunks = this->nChunks();
    int old_nremainder = this->nRemainder();

    //get the dimensions of the new chunked vector
    int new_nchunks = 1 + ((new_count-1) / N);
    int new_nremainder = new_count % N;
        
    if (new_nchunks < old_nchunks)
    {
        this->_chunks.resize(new_nchunks);
    }
    else if (new_nchunks > old_nchunks)
    {
        if (old_nremainder != 0)
            this->_chunks[old_nchunks-1].resize(N);
            
        this->_chunks.resize(new_nchunks);
        
        for (int i=old_nchunks; i<new_nchunks; ++i)
        {
            this->_chunks[i].resize( N );
            this->_chunks[i].squeeze();
        }
    }
    
    //resize the last chunk to the right size
    if (new_nremainder == 0)
        this->_chunks[new_nchunks-1].resize(N);
    else
        this->_chunks[new_nchunks-1].resize(new_nremainder);

    return;
}

/** Squeeze this vector - this minimises the amount of memory used */
template<class T, int N>
SIRE_OUTOFLINE_TEMPLATE
void ChunkedVector<T,N>::squeeze()
{
    int nchunks = this->nChunks();
    int nfullchunks = this->nFullChunks();

    for (int i=nchunks-1; i>nfullchunks; --i)
    {
        if (this->_chunks[i].isEmpty())
            this->_chunks.remove(i);
        else
            this->_chunks[i].squeeze();
    }

    this->_chunks.squeeze();
}

/** Reserve space for at least 'count' elements */
template<class T, int N>
SIRE_OUTOFLINE_TEMPLATE
void ChunkedVector<T,N>::reserve(int count)
{
    if (count > this->capacity())
    {
        int current_nchunks = this->nChunks();
        
        int new_nchunks = count / N;
        int new_nremainder = count % N;
        
        if (new_nremainder > 0)
        {
            new_nchunks += 1;
        }
        
        this->_chunks.resize( new_nchunks );
        
        for (int i=current_nchunks; i<new_nchunks; ++i)
        {
            this->_chunks[i].reserve(N);
        }
    }
}

/* Return this chunked vector as a large vector */
template<class T, int N>
SIRE_OUTOFLINE_TEMPLATE
QVector<T> ChunkedVector<T,N>::toVector() const
{
    int size = this->count();
    
    if (size == 0)
        return QVector<T>();

    QVector<T> vector(size);
    
    T *vector_array = vector.data();
    
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
    if (vector.isEmpty())
        return ChunkedVector<T,N>();

    int nvals = vector.count();
    const T *vector_array = vector.constData();

    ChunkedVector<T,N> ret(nvals);
    
    for (int i=0; i<nvals; ++i)
    {
        ret[i] = vector_array[i];
    }
    
    return ret;
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

/** Return a key that can be used to identify the data, used
    only by the shared streaming subsystem */
template<class T, int N>
SIRE_OUTOFLINE_TEMPLATE
const void* ChunkedVector<T,N>::shareKey() const
{
    if (this->isEmpty())
        return 0;
    else
        return this->_chunks.constData();
}

#endif // SIRE_SKIP_INLINE_FUNCTIONS

}

#ifdef SIRE_INSTANTIATE_TEMPLATES
template class 
SireBase::ChunkedVector<double,100>;
#endif

SIRE_EXPOSE_ALIAS( (SireBase::ChunkedVector<double, 100>), SireBase::ChunkedVector_double_ );

SIRE_END_HEADER

#endif
