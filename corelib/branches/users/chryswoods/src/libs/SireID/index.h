/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2007  Christopher Woods
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

#ifndef SIREID_INDEX_H
#define SIREID_INDEX_H

#include "id.h"

#include "Siren/stream.h"

#include <limits>

SIRE_BEGIN_HEADER

namespace SireID
{

/** This is the base class of all Index objects. An Index object
    provides the index of an object in an indexable list or array (or indeed
    any container that holds objects in a linear, numerical indexed
    manner (e.g. atoms in a Molecule, Molecules in a group)
    
    This class cannot be instantiated on its own - it must be 
    inherited by a derived class to be used, e.g.
    
    class AtomIdx : public Implements<AtomIdx, IndexType<AtomIdx> >,
                    public Interfaces<AtomIdx, AtomID>
    
    @author Christopher Woods
*/
class SIREID_EXPORT IndexBase : public Siren::Extends<IndexBase,ID>
{
public:
    IndexBase(qint32 idx = IndexBase::null());
    
    IndexBase(const IndexBase &other);
    
    ~IndexBase();

    static QString typeName();

    uint hashCode() const;
    void stream(Siren::Stream &s);

    static qint32 null();

    bool isNull() const;

    operator qint32() const;

    qint32 value() const;

    qint32 map(qint32 n) const;
    
protected:
    IndexBase& operator=(const IndexBase &other);

    bool operator==(const IndexBase &other) const;
    bool operator!=(const IndexBase &other) const;
    
    void throwInvalidIndex(qint32 n) const;
    
    /** The actual index value */
    qint32 _idx;
};

/** This derived version of index provides all of the
    standard operators that you would expect.
    
    @author Christopher Woods
*/
template<class T>
class SIREID_EXPORT IndexType : public IndexBase
{
public:
    IndexType(qint32 idx=IndexBase::null());
    
    IndexType(const IndexType<T> &other);

    ~IndexType();
    
    void stream(Siren::Stream &s);
    
    T& operator=(qint32 idx);
    
    T& operator+=(qint32 val);
    T& operator++();
    T operator++(qint32);
    
    T& operator-=(qint32 val);
    T& operator--();
    T operator--(qint32);
};

class SIREID_EXPORT Index : public Siren::Implements< Index, IndexType<Index> >
{
public:
    explicit Index(qint32 idx = IndexBase::null());
    
    Index(const Index &other);
    
    ~Index();
    
    static Index null();
    
    QString toString() const;
};

#ifndef SIRE_SKIP_INLINE_FUNCTIONS

template<class T>
SIRE_OUTOFLINE_TEMPLATE
IndexType<T>::IndexType(qint32 idx) : IndexBase(idx)
{}

template<class T>
SIRE_OUTOFLINE_TEMPLATE
IndexType<T>::IndexType(const IndexType<T> &other) : IndexBase(other)
{}

template<class T>
SIRE_OUTOFLINE_TEMPLATE
IndexType<T>::~IndexType()
{}

/** Assignment operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
T& IndexType<T>::operator=(qint32 idx)
{
    _idx = idx;
    return static_cast<T&>(*this);
}

/** Increment operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
T& IndexType<T>::operator+=(qint32 val)
{
    _idx += val;
    return static_cast<T&>(*this);
}

/** Increment operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
T& IndexType<T>::operator++()
{
    ++_idx;
    return static_cast<T&>(*this);
}

/** Increment operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
T IndexType<T>::operator++(qint32)
{
    T orig(*this);
    ++_idx;
    
    return orig;
}

/** Decrement operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
T& IndexType<T>::operator-=(qint32 val)
{
    _idx -= val;
    return static_cast<T&>(*this);
}

/** Decrement operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
T& IndexType<T>::operator--()
{
    --_idx;
    return static_cast<T&>(*this);
}

/** Decrement operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
T IndexType<T>::operator--(qint32)
{
    T orig(*this);
    --_idx;
    return orig;
}

template<class T>
SIRE_OUTOFLINE_TEMPLATE
void IndexType<T>::stream(Siren::Stream &s)
{
    s.assertVersion<T>(1);
    
    Siren::Schema schema = s.item<T>();
    
    schema.data("index") & _idx;
    
    ID::stream( schema.base() );
}

#endif // SIRE_SKIP_INLINE_FUNCTIONS

}

Q_DECLARE_METATYPE( SireID::Index )

SIRE_EXPOSE_CLASS( SireID::IndexBase )
SIRE_EXPOSE_CLASS( SireID::Index )

SIRE_END_HEADER

#endif
