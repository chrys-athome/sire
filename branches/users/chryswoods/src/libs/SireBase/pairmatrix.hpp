#ifndef SIREBASE_PAIRMATRIX_HPP
#define SIREBASE_PAIRMATRIX_HPP

#include <QVarLengthArray>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireBase
{

/**
This provides a 2D matrix that contains information about all pairs of two groups of atoms. This matrix is designed to be used in a pair-pair loop, where each element is accessed sequentially, and it is thus highly optimised for such use. It does provide random access via indicies, but this will be slightly slower (though still quite quick!).

@author Christopher Woods
*/
template<class T, int N=1024>
class PairMatrix
{
public:
    /** Construct a null PairMatrix */
    PairMatrix();

    /** Construct a PairMatrix that holds n_outer elements on the outer loop,
        and n_inner elements on the inner loop. If either or these are 0 then
        an invalid PairMatrix will be created. Also note that you can redimension
        this PairMatrix at any time using a potentially very quick function. */
    PairMatrix(unsigned int n_outer, unsigned int n_inner);
    ~PairMatrix();

    /** Return a reference to the element at index 'i,j', where
        'i' is the outer loop, and j is the inner loop. As this is optimised
        for speed, there is no checking to ensure that this array position
        is valid! */
    const T& operator()(unsigned int i, unsigned int j) const;
    T& operator()(unsigned int i, unsigned int j);

    /** Return a reference to the element at index 'j', where 'j'
        refers to the inner loop, and the outer loop index has been
        set via 'setOuterIndex()' */
    const T& operator()(unsigned int j) const;
    T& operator()(unsigned int j);

    const T& operator[](unsigned int j) const;
    T& operator[](unsigned int j);

    /** Return a const reference to the element at index 'i,j'. This is used by
        the Python wrapping... */
    const T& at(unsigned int i, unsigned int j) const;

    /** Redimension this PairMatrix to n_outer by n_inner elements. This
        function is very quick if the total number of elements does not change
        as the existing array is just reinterpreted. Reallocation will only
        occur if the number of elements becomes greater than n_elements. Note
        that you should clear or repopulate the PairMatrix after it has been
        redimensioned. Also note that both i and j should be greater than 0,
        and that any index set using 'setOuterIndex' will now be invalid. */
    void redimension(unsigned int i, unsigned int j);

    /** Return the dimensions of the array */
    unsigned int nOuter() const;
    unsigned int nInner() const;

    /** Set the index of the outer loop. This is used in combination with
        operator[](unsigned int j) which then only varies the index of the
        inner loop */
    void setOuterIndex(unsigned int i);

    /** Clear the PairMatrix. Sets all values to 0.0 */
    void clear();

private:
    /** Pointer to the array of entries in this PairMatrix */
    QVarLengthArray<T,N> array;

    /** The current outer index (multiplied by n_outer) */
    unsigned int outer_index;

    /** The dimensions of the array */
    unsigned int n_outer, n_inner;

    /** The number of elements in the array */
    unsigned int n_elements;
};

template<class T, int N>
SIRE_OUTOFLINE_TEMPLATE
PairMatrix<T,N>::PairMatrix() : array(0), n_outer(0), n_inner(0), n_elements(0)
{}

template<class T, int N>
SIRE_OUTOFLINE_TEMPLATE
PairMatrix<T,N>::PairMatrix(unsigned int i, unsigned int j)
              : n_outer(i), n_inner(j), n_elements(0)
{
    n_elements = n_outer * n_inner;
    array.resize(n_elements);
}

template<class T, int N>
SIRE_OUTOFLINE_TEMPLATE
PairMatrix<T,N>::~PairMatrix()
{}

template<class T, int N>
SIRE_OUTOFLINE_TEMPLATE
unsigned int PairMatrix<T,N>::nOuter() const
{
    return n_outer;
}

template<class T, int N>
SIRE_OUTOFLINE_TEMPLATE
unsigned int PairMatrix<T,N>::nInner() const
{
    return n_inner;
}

template<class T, int N>
SIRE_OUTOFLINE_TEMPLATE
void PairMatrix<T,N>::clear()
{
    array.resize(0);
    array.resize(n_elements);
}

template<class T, int N>
SIRE_INLINE_TEMPLATE
const T& PairMatrix<T,N>::operator()(unsigned int i, unsigned int j) const
{
    return array.constData()[i*n_outer + j];
}

template<class T, int N>
SIRE_INLINE_TEMPLATE
T& PairMatrix<T,N>::operator()(unsigned int i, unsigned int j)
{
    return array.data()[i*n_outer + j];
}

template<class T, int N>
SIRE_INLINE_TEMPLATE
const T& PairMatrix<T,N>::operator()(unsigned int j) const
{
    return array.constData()[outer_index + j];
}

template<class T, int N>
SIRE_INLINE_TEMPLATE
T& PairMatrix<T,N>::operator()(unsigned int j)
{
    return array.data()[outer_index + j];
}

template<class T, int N>
SIRE_INLINE_TEMPLATE
const T& PairMatrix<T,N>::operator[](unsigned int j) const
{
    return operator()(j);
}

template<class T, int N>
SIRE_INLINE_TEMPLATE
T& PairMatrix<T,N>::operator[](unsigned int j)
{
    return operator()(j);
}

template<class T, int N>
SIRE_INLINE_TEMPLATE
void PairMatrix<T,N>::setOuterIndex(unsigned int i)
{
    outer_index = n_outer * i;
}

template<class T, int N>
SIRE_INLINE_TEMPLATE
const T& PairMatrix<T,N>::at(unsigned int i, unsigned int j) const
{
    return PairMatrix::operator()(i,j);
}

template<class T, int N>
SIRE_INLINE_TEMPLATE
void PairMatrix<T,N>::redimension(unsigned int i, unsigned int j)
{
    if (i != n_inner or j != n_outer)
    {
        n_inner = i;
        n_outer = j;

        unsigned int new_n_elements = n_inner * n_outer;
        if (new_n_elements > n_elements)
        {
            n_elements = new_n_elements;
            array.resize(n_elements);
        }
    }
}

}

SIRE_END_HEADER

#endif
