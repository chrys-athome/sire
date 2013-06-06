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

#ifndef SIREBASE_ARRAYPROPERTY_HPP
#define SIREBASE_ARRAYPROPERTY_HPP

#include "SireBase/property.h"

#include "tostring.h"

SIRE_BEGIN_HEADER

namespace SireBase
{
template<class T>
class ArrayProperty;
}

template<class T>
QDataStream& operator<<(QDataStream&, const SireBase::ArrayProperty<T>&);
template<class T>
QDataStream& operator>>(QDataStream&, SireBase::ArrayProperty<T>&);

namespace SireBase
{

namespace detail
{
    int checkIndex(int i, int count);
}

/** This class provides a thin Property wrapper around an array.
    This is useful when you want to store a large array of a basic
    type without transforming all of the elements into property objects
    
    @author Christopher Woods
*/
template<class T>
class SIREBASE_EXPORT ArrayProperty : public ConcreteProperty<ArrayProperty<T>,Property>
{

friend QDataStream& ::operator<<<>(QDataStream&, const ArrayProperty<T>&);
friend QDataStream& ::operator>><>(QDataStream&, ArrayProperty<T>&);

public:
    ArrayProperty();
    ArrayProperty(const QList<T> &array);
    ArrayProperty(const QVector<T> &array);
    ArrayProperty(const ArrayProperty<T> &other);
    
    ~ArrayProperty();
    
    static const char* typeName();
    
    ArrayProperty<T>& operator=(const ArrayProperty &other);
    
    bool operator==(const ArrayProperty &other) const;
    bool operator!=(const ArrayProperty &other) const;

    ArrayProperty<T> operator+(const ArrayProperty<T> &other) const;
    ArrayProperty<T>& operator+=(const ArrayProperty<T> &other);
    const T& operator[](int i) const;

    QString toString() const;

    QVector<T> array() const;

    int count() const;
    int size() const;

    void append(const T &value);
    void append(const ArrayProperty<T> &values);
    
    const T& at(int i) const;
    
    void clear();
    
    bool empty() const;
    bool isEmpty() const;

    void insert(int i, const T &value);
    
    ArrayProperty<T> mid(int pos, int length=-1) const;
    
    void move(int from, int to);
    
    void pop_back();
    void pop_front();
    void prepend(const T &value);
    void push_back(const T &value);
    void push_front(const T &value);
    
    void removeAt(int i);
    void removeFirst();
    void removeLast();
    
    void replace(int i, const T &value);

    void swap(ArrayProperty<T> &other);
    
    void swap(int i, int j);
    
    T takeAt(int i);
    T takeFirst();
    T takeLast();
    
    QList<T> toList() const;
    QVector<T> toVector() const;
    
    T value(int i) const;
    T value(int i, const T &default_value) const;
    
    operator QVector<T>() const;
    
private:
    /** The actual array */
    QVector<T> a;
};

typedef ArrayProperty<qint64> IntegerArrayProperty;
typedef ArrayProperty<double> DoubleArrayProperty;
typedef ArrayProperty<QString> StringArrayProperty;

#ifndef SIRE_SKIP_INLINE_FUNCTIONS

/** Constructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
ArrayProperty<T>::ArrayProperty() : ConcreteProperty<ArrayProperty<T>,Property>()
{}

/** Construct from the passed list */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
ArrayProperty<T>::ArrayProperty(const QList<T> &array)
                 : ConcreteProperty<ArrayProperty<T>,Property>()
{
    if (not array.isEmpty())
    {
        a.reserve(array.count());
        
        for (typename QList<T>::const_iterator it = array.constBegin();
             it != array.constEnd();
             ++it)
        {
            a.append(*it);
        }
    }
}

/** Construct from the passed array */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
ArrayProperty<T>::ArrayProperty(const QVector<T> &array)
                 : ConcreteProperty<ArrayProperty<T>,Property>(),
                   a(array)
{}

/** Copy constructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
ArrayProperty<T>::ArrayProperty(const ArrayProperty<T> &other)
                 : ConcreteProperty<ArrayProperty<T>,Property>(other), a(other.a)
{}

/** Destructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
ArrayProperty<T>::~ArrayProperty()
{}

template<class T>
SIRE_OUTOFLINE_TEMPLATE
const char* ArrayProperty<T>::typeName()
{
    return QMetaType::typeName( qMetaTypeId< ArrayProperty<T> >() );
}

/** Copy assignment operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
ArrayProperty<T>& ArrayProperty<T>::operator=(const ArrayProperty &other)
{
    a = other.a;
    return *this;
}

/** Comparison operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool ArrayProperty<T>::operator==(const ArrayProperty &other) const
{
    return a == other.a;
}

/** Comparison operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool ArrayProperty<T>::operator!=(const ArrayProperty &other) const
{
    return a != other.a;
}

/** Return the sum of the two arrays (join them together) */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
ArrayProperty<T> ArrayProperty<T>::operator+(const ArrayProperty<T> &other) const
{
    return ArrayProperty<T>( a + other.a );
}

/** Add the contents of the other array onto this array */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
ArrayProperty<T>& ArrayProperty<T>::operator+=(const ArrayProperty<T> &other)
{
    a += other.a;
    return *this;
}

/** Return the ith element of the array */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const T& ArrayProperty<T>::operator[](int i) const
{
    return a.constData()[ detail::checkIndex(i, a.count()) ];
}

template<class T>
SIRE_OUTOFLINE_TEMPLATE
QString ArrayProperty<T>::toString() const
{
    return Sire::toString(a);
}

template<class T>
SIRE_OUTOFLINE_TEMPLATE
QVector<T> ArrayProperty<T>::array() const
{
    return a;
}

/** Return the number of elements in the array */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
int ArrayProperty<T>::count() const
{
    return a.count();
}

/** Return the size of the array */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
int ArrayProperty<T>::size() const
{
    return a.size();
}

/** Append the passed value onto the array */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void ArrayProperty<T>::append(const T &value)
{
    a.append(value);
}

/** Append the passed array on this array */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void ArrayProperty<T>::append(const ArrayProperty<T> &values)
{
    if (not values.isEmpty())
    {
        a.reserve( a.count() + values.count() );
        
        for (int i=0; i<values.count(); ++i)
        {
            a.append( values.a.constData()[i] );
        }
    }
}

/** Return the ith element of the array */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
const T& ArrayProperty<T>::at(int i) const
{
    return a.constData()[ detail::checkIndex(i,a.count()) ];
}

/** Clear the array */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void ArrayProperty<T>::clear()
{
    a.clear();
}

/** Return whether or not the array is empty */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool ArrayProperty<T>::empty() const
{
    return a.isEmpty();
}

/** Return whether or not the array is empty */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool ArrayProperty<T>::isEmpty() const
{
    return a.isEmpty();
}

/** Insert the passed value into the array at index i */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void ArrayProperty<T>::insert(int i, const T &value)
{
    a.insert(i, value);
}

/** Return the subset of the array from a[pos, pos+length] */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
ArrayProperty<T> ArrayProperty<T>::mid(int pos, int length) const
{
    return ArrayProperty<T>( a.mid( detail::checkIndex(pos,a.count()), length ) );
}

/** Move the item at index 'from' to index 'to' */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void ArrayProperty<T>::move(int from, int to)
{
    from = detail::checkIndex(from, a.count());
    to = detail::checkIndex(to, a.count());
    
    if (from != to)
    {
        QList<T> list = a.toList();
        list.move(from,to);
        a = list.toVector();
    }
}

/** Pop the last element of the array */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void ArrayProperty<T>::pop_back()
{
    a.pop_back();
}

/** Pop the first element of the array */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void ArrayProperty<T>::pop_front()
{
    a.pop_front();
}

/** Prepend the passed value onto the beginning of the array */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void ArrayProperty<T>::prepend(const T &value)
{
    a.prepend(value);
}

/** Push the passed value onto the back of the array */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void ArrayProperty<T>::push_back(const T &value)
{
    a.push_back(value);
}

/** Push the passed value onto the front of the array */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void ArrayProperty<T>::push_front(const T &value)
{
    a.push_front(value);
}

/** Remove the value at index 'i' */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void ArrayProperty<T>::removeAt(int i)
{
    i = detail::checkIndex(i,a.count());
    
    QList<T> list = a.toList();
    list.removeAt(i);
    a = list.toVector();
}

/** Remove the first item from the array */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void ArrayProperty<T>::removeFirst()
{
    QList<T> list = a.toList();
    list.removeFirst();
    a = list.toVector();
}

/** Remove the last item from the array */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void ArrayProperty<T>::removeLast()
{
    QList<T> list = a.toList();
    list.removeLast();
    a = list.toVector();
}

/** Replace the item at index 'i' with the passed value */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void ArrayProperty<T>::replace(int i, const T &value)
{
    a.replace( detail::checkIndex(i,a.count()), value );
}

/** Swap this array with 'other' */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void ArrayProperty<T>::swap(ArrayProperty<T> &other)
{
    QVector<T> tmp = a;
    a = other.a;
    other.a = tmp;
}

/** Swap the items at indicies i and j */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void ArrayProperty<T>::swap(int i, int j)
{
    i = detail::checkIndex(i,a.count());
    j = detail::checkIndex(j,a.count());

    if (i != j)
    {
        T tmp = a[i];
        a[i] = a[j];
        a[j] = tmp;
    }
}

/** Take the item at index i */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
T ArrayProperty<T>::takeAt(int i)
{
    i = detail::checkIndex(i,a.count());
    
    QList<T> list = a.toList();
    T val = list.takeAt(i);
    a = list.toVector();
    return val;
}

/** Take the first item from the array */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
T ArrayProperty<T>::takeFirst()
{
    QList<T> list = a.toList();
    T val = list.takeFirst();
    a = list.toVector();
    return val;
}

/** Take the last item from the array */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
T ArrayProperty<T>::takeLast()
{
    QList<T> list = a.toList();
    T val = list.takeLast();
    a = list.toVector();
    return val;
}

/** Return this array as a QList<T> */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
QList<T> ArrayProperty<T>::toList() const
{
    QList<T> list;
    
    for (int i=0; i<a.count(); ++i)
    {
        list.append( a.constData()[i] );
    }
    
    return list;
}

/** Return this array as a QVector<T> */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
QVector<T> ArrayProperty<T>::toVector() const
{
    return a;
}

/** Return the value at index 'i', or 'default_value' if this 
    is an invalid index */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
T ArrayProperty<T>::value(int i, const T &default_value) const
{
    int idx = i;
    
    if (i < 0)
        idx = a.count() + i;
    
    if (idx < 0 or idx >= a.count())
        return default_value;
    else
        return a.constData()[idx];
}

/** Return the value at index 'i' */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
T ArrayProperty<T>::value(int i) const
{
    return this->value(i, T());
}

/** Allow for automatic casting to QVector<T> */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
ArrayProperty<T>::operator QVector<T>() const
{
    return a;
}

#endif

}

Q_DECLARE_METATYPE( SireBase::IntegerArrayProperty )
Q_DECLARE_METATYPE( SireBase::DoubleArrayProperty )
Q_DECLARE_METATYPE( SireBase::StringArrayProperty )

SIRE_EXPOSE_ALIAS( SireBase::ArrayProperty<qint64>, IntegerArrayProperty )
SIRE_EXPOSE_ALIAS( SireBase::ArrayProperty<double>, DoubleArrayProperty )
SIRE_EXPOSE_ALIAS( SireBase::ArrayProperty<QString>, StringArrayProperty )

#ifdef SIRE_INSTANTIATE_TEMPLATES

template class SireBase::ArrayProperty<qint64>;
template class SireBase::ArrayProperty<double>;
template class SireBase::ArrayProperty<QString>;

#endif //SIRE_INSTANTIATE_TEMPLATES

SIRE_END_HEADER

#endif
