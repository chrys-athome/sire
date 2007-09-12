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

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireID
{
class Index;
}

QDataStream& operator<<(QDataStream&, const SireID::Index&);
QDataStream& operator>>(QDataStream&, SireID::Index&);

namespace SireID
{

/** This is the base class of all Index objects. An Index object
    provides the index of an object in an indexable list or array (or indeed
    any container that holds objects in a linear, numerical indexed
    manner (e.g. atoms in a Molecule, Molecules in a group)
    
    This class cannot be instantiated on its own - it must be 
    inherited by a derived class to be used.
    
    Note that this is a 32bit index, so can only index from 
    0 to (2^32)-1
    
    @author Christopher Woods
*/
class SIREID_EXPORT Index
{

friend QDataStream& ::operator<<(QDataStream&, const Index&);
friend QDataStream& ::operator>>(QDataStream&, Index&);

public:
    ~Index()
    {}

    /** Allow implicit conversion back to an int */
    operator quint32() const
    {
        return _idx;
    }

protected:
    Index(quint32 idx) : _idx(idx)
    {}
    
    Index(const Index &other) : _idx(other._idx)
    {}
    
    Index& operator=(const Index &other)
    {
        _idx = other._idx;
        return *this;
    }
    
    /** The actual index value */
    quint32 _idx;
};

/** This derived version of index provides all of the
    standard operators that you would expect.
    
    @author Christopher Woods
*/
template<class T>
class SIREID_EXPORT Index_T_ : public Index
{
public:
    ~Index_T_()
    {}

    /** Comparison operator */
    bool operator==(const T &other) const
    {
        return _idx == other._idx;
    }
    
    /** Comparison operator */
    bool operator==(quint32 val) const
    {
        return _idx == val;
    }
    
    /** Comparison operator */
    bool operator!=(const T &other) const
    {
        return _idx != other._idx;
    }
    
    /** Comparison operator */
    bool operator!=(quint32 val) const
    {
        return _idx != val;
    }
    
    /** Assignment operator */
    T& operator=(quint32 idx)
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
    T operator++(qint)
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
    
protected:
    Index_T_(quint32 idx=0) : Index(idx)
    {}
    
    Index_T_(const Index_T_<T> &other) : Index(other)
    {}
};

}

/** Return a hash of this index */
uint qHash(const SireID::Index &index)
{
    return quint32(index);
}

/** Serialise an Idx class */
inline QDataStream& operator<<(QDataStream &ds, const SireID::Index &idx)
{
    ds << idx._idx;
    return ds;
}

/** Deserialise an Idx class */
inline QDataStream& operator>>(QDataStream &ds, SireID::Index &idx)
{
    ds >> idx._idx;
    return ds;
}

SIRE_END_HEADER

#endif
