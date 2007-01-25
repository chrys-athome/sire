/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
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

#ifndef SIREFF_WINDOW_HPP
#define SIREFF_WINDOW_HPP

#include "SireError/errors.h"

#include <QVector>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireFF
{

/** A Window is a read-only view into a parameter QVector. This uses the fact that the QVector
    class is implicitly shared to save memory by not creating a copy of a subsection of 
    the vector, but instead to hold an implicit copy of the vector, and to index
    it using new indexes based on the position of this view.
    
    @author Christopher Woods
*/  
template<class T>
class Window
{

typedef QVector<T> VectorT;

public:
    Window();
    Window(const VectorT &table, UInt strt, UInt end);
    Window(const Window<T> &other);
    
    ~Window();
    
    const T& at(int i) const;
    const T& operator[](int i) const;
    
    int size() const;
    int count() const;
    
    const VectorT& vector() const;
    
    const T* constData() const;
private:
  
    /** The table containing the reference data */
    VectorT reftable;
    
    /** The start index for this window */
    UInt strt;
    
    /** The number of elements in this window */
    UInt sz;
    
};

/** Create a null window */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Window<T>::Window() : strt(0), sz(0)
{}

/** Create a window over the range strt to end inclusive of the data in the 
    table 'table'. Both strt and end must be valid indexes in 'table' or
    else an exception will be thrown. If strt > end then they will be
    swapped.
     
    \throw SireError::invalid_index
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Window<T>::Window(const VectorT &table, UInt start, UInt end)
          : reftable(table)
{
    if (start > end)
        qSwap(start,end);
        
    if (end >= (UInt)table.count())
        throw SireError::invalid_index(QObject::tr(
            "Cannot get a window over the range %1-%2 in an array with range 0-%3")
                .arg(start).arg(end).arg( table.count() - 1 ), CODELOC);

    strt = start;
    
    //save the size of the window
    sz = end - start + 1;
}

/** Copy constructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Window<T>::Window(const Window<T> &other)
          : reftable(other.reftable), strt(other.strt), sz(other.sz)
{}

/** Destructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Window<T>::~Window()
{}
    
/** Return the ith element in this table - this must be a valid index */
template<class T>
SIRE_INLINE_TEMPLATE
const T& Window<T>::at(int i) const
{
    return reftable.at( strt + i );
}
    
/** Return the ith element in this table - this must be a valid index */
template<class T>
SIRE_INLINE_TEMPLATE
const T& Window<T>::operator[](int i) const
{
    return this->at(i);
}
    
/** Return a const pointer to the start of the array that holds this data */
template<class T>
SIRE_INLINE_TEMPLATE
const T* Window<T>::constData() const
{
    return &(this->at(0));
}

/** Return a reference to the actual QVector that holds the data in this window */
template<class T>
SIRE_INLINE_TEMPLATE
const QVector<T>& Window<T>::vector() const
{
    return reftable;
}

/** Return the size of the window */
template<class T>
SIRE_INLINE_TEMPLATE
int Window<T>::size() const
{
    return sz;
}

/** Return the size of the window */
template<class T>
SIRE_INLINE_TEMPLATE
int Window<T>::count() const
{
    return sz;
}

}

SIRE_END_HEADER

#endif
