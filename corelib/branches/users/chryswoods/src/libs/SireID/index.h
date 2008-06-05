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

SIRE_BEGIN_HEADER

namespace SireID
{
class IndexBase;
}

QDataStream& operator<<(QDataStream&, const SireID::IndexBase&);
QDataStream& operator>>(QDataStream&, SireID::IndexBase&);

uint qHash(const SireID::IndexBase &index);

namespace SireID
{

/** This is the base class of all Index objects. An Index object
    provides the index of an object in an indexable list or array (or indeed
    any container that holds objects in a linear, numerical indexed
    manner (e.g. atoms in a Molecule, Molecules in a group)
    
    This class cannot be instantiated on its own - it must be 
    inherited by a derived class to be used.
    
    @author Christopher Woods
*/
class SIREID_EXPORT IndexBase
{

friend QDataStream& ::operator<<(QDataStream&, const IndexBase&);
friend QDataStream& ::operator>>(QDataStream&, IndexBase&);

public:
    ~IndexBase()
    {}

    /** Return the null index */
    static qint32 null()
    {
        return std::numeric_limits<qint32>::min();
    }

    /** Return whether this is a null index - a null
        index is one that equals std::numeric_limits<qint32>::min(),
        which should be -21474833648 for a 32bit integer */
    bool isNull() const
    {
        return _idx == IndexBase::null();
    }

    /** Allow implicit conversion back to an int */
    operator qint32() const
    {
        return _idx;
    }

    /** Map this index into the container of 'n' elements - this
        maps the index (with negative indexing, e.g. -1 is the last
        element), and throws an exception if the index is out 
        of the bounds of the array
        
        \throw SireError::invalid_index
    */
    qint32 map(qint32 n) const
    {
        if (_idx >= 0 and _idx < n)
            return _idx;
        else if (_idx < 0 and _idx >= -n)
            return n + _idx;
        else
        {
            throwInvalidIndex(n);
            return null();
        }
    }

protected:
    explicit IndexBase(qint32 idx = IndexBase::null()) : _idx(idx)
    {}
    
    IndexBase(const IndexBase &other) : _idx(other._idx)
    {}
    
    IndexBase& operator=(const IndexBase &other)
    {
        _idx = other._idx;
        return *this;
    }
    
    void throwInvalidIndex(qint32 n) const;
    
    /** The actual index value */
    qint32 _idx;
};

/** This derived version of index provides all of the
    standard operators that you would expect.
    
    @author Christopher Woods
*/
template<class T>
class SIREID_EXPORT Index_T_ : public IndexBase
{
public:
    explicit Index_T_(qint32 idx=IndexBase::null()) : IndexBase(idx)
    {}
    
    explicit Index_T_(const Index_T_<T> &other) : IndexBase(other)
    {}

    ~Index_T_()
    {}

    const char* what() const
    {
        return T::typeName();
    }
    
    IndexBase* clone() const
    {
        return new T( static_cast<const T&>(*this) );
    }
    
    uint hash() const
    {
        return qHash( static_cast<const IndexBase&>(*this) );
    }

    /** Comparison operator */
    bool operator==(const T &other) const
    {
        return _idx == other._idx;
    }
    
    /** Comparison operator */
    bool operator==(const ID &other) const
    {
        return ID::compare<T>(static_cast<const T&>(*this),other);
    }
    
    /** Comparison operator */
    bool operator==(qint32 val) const
    {
        return _idx == val;
    }
    
    /** Comparison operator */
    bool operator!=(const T &other) const
    {
        return _idx != other._idx;
    }
    
    /** Comparison operator */
    bool operator!=(qint32 val) const
    {
        return _idx != val;
    }
    
    /** Assignment operator */
    T& operator=(qint32 idx)
    {
        _idx = idx;
        return static_cast<T&>(*this);
    }
    
    /** Increment operator */
    T& operator+=(qint32 val)
    {
        _idx += val;
        return static_cast<T&>(*this);
    }
    
    /** Increment operator */
    T& operator++()
    {
        ++_idx;
        return static_cast<T&>(*this);
    }
    
    /** Increment operator */
    T operator++(qint32)
    {
        T orig(*this);
        ++_idx;
        
        return orig;
    }
    
    /** Decrement operator */
    T& operator-=(qint32 val)
    {
        _idx -= val;
        return static_cast<T&>(*this);
    }
    
    /** Decrement operator */
    T& operator--()
    {
        --_idx;
        return static_cast<T&>(*this);
    }
    
    /** Decrement operator */
    T operator--(qint32)
    {
        T orig(*this);
        --_idx;
        return orig;
    }
};

class Index : public Index_T_<Index>
{
public:
    explicit Index(qint32 idx = IndexBase::null()) : Index_T_<Index>(idx)
    {}
    
    Index(const Index &other) : Index_T_<Index>(other)
    {}
    
    ~Index()
    {}
    
    static const char* typeName()
    {
        return "SireID::Index";
    }
    
    static Index null()
    {
        return Index();
    }
    
    QString toString() const
    {
        return QString("Index(%1)").arg(_idx);
    }
};

}

/** Return a hash of this index */
inline uint qHash(const SireID::IndexBase &index)
{
    return qint32(index);
}

/** Serialise an Idx class */
inline QDataStream& operator<<(QDataStream &ds, const SireID::IndexBase &idx)
{
    ds << idx._idx;
    return ds;
}

/** Deserialise an Idx class */
inline QDataStream& operator>>(QDataStream &ds, SireID::IndexBase &idx)
{
    ds >> idx._idx;
    return ds;
}

SIRE_END_HEADER

#endif
