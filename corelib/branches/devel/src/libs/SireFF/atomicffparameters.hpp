/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2010  Christopher Woods
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

#ifndef SIREFF_ATOMICFFPARAMETERS_HPP
#define SIREFF_ATOMICFFPARAMETERS_HPP

#include "ffparameters.h"

#include "SireBase/packedarray2d.hpp"

SIRE_BEGIN_HEADER

namespace SireFF
{
template<class T>
class AtomicFFParameters;
}

template<class T>
QDataStream& operator<<(QDataStream&, const SireFF::AtomicFFParameters<T>&);
template<class T>
QDataStream& operator>>(QDataStream&, SireFF::AtomicFFParameters<T>&);

namespace SireFF
{

/** This template class holds the FFParameters of type T, where
    there is a parameter for each atom in the bead
    
    @author Christopher Woods
*/
template<class T>
class SIREFF_EXPORT AtomicFFParameters
        : public SireBase::ConcreteProperty<AtomicFFParameters<T>,FFParameters>
{

friend QDataStream& ::operator<<<>(QDataStream&, const AtomicFFParameters<T>&);
friend QDataStream& ::operator>><>(QDataStream&, AtomicFFParameters<T>&);

public:
    AtomicFFParameters();
    AtomicFFParameters(const QVector<T> &params);
    AtomicFFParameters(const typename SireBase::PackedArray2D<T>::Array &params);
    
    AtomicFFParameters(const AtomicFFParameters<T> &other);
    
    ~AtomicFFParameters();
    
    static const char* typeName();
    
    AtomicFFParameters<T>& operator=(const AtomicFFParameters<T> &other);
    
    bool operator==(const AtomicFFParameters<T> &other) const;
    bool operator!=(const AtomicFFParameters<T> &other) const;
    
    const typename SireBase::PackedArray2D<T>::Array& array() const;
    
    operator typename SireBase::PackedArray2D<T>::Array() const;
    
private:
    /** The actual parameters */
    typename SireBase::PackedArray2D<T>::Array params;
};

/** This class holds the array of AtomicFFParameters<T> 

    @author Christopher Woods
*/
class SIREFF_EXPORT AtomicFFParametersArray
    : public SireBase::ConcreteProperty<AtomicFFParametersArray<T>,FFParametersArray>
{

friend QDataStream& ::operator<<<>(QDataStream&, const AtomicFFParametersArray<T>&);
friend QDataStream& ::operator>><>(QDataStream&, AtomicFFParametersArray<T>&);

public:
    AtomicFFParametersArray();
    AtomicFFParametersArray(const PackedArray2D<T> &params);
    AtomicFFParametersArray(const QVector<T> &params);
    AtomicFFParametersArray(const QVector< QVector<T> > &params);
    
    ~AtomicFFParametersArray();
    
    static const char* typeName();
    
    AtomicFFParametersArray<T>& operator=(const AtomicFFParametersArray<T> &other);
    
    bool operator==(const AtomicFFParametersArray<T> &other) const;
    bool operator!=(const AtomicFFParametersArray<T> &other) const;
    
    const SireBase::PackedArray2D<T>& array() const;
    
    operator SireBase::PackedArray2D<T>() const;
    
    void append(const QVector<T> &params);
    void append(const QVector< QVector<T> > &params);
    
    void append(const AtomicFFParameters<T> &params);
    void append(const AtomicFFParametersArray<T> &params);
    
    void append(const FFParameters &params);
    void append(const FFParametersArray &params);
    
    void update(int idx, const QVector<T> &params);
    void update(const QVector<int> &idxs, const QVector< QVector<T> > &params);
    
    void update(int idx, const AtomicFFParameters<T> &params);
    void update(const QVector<int> &idxs, const AtomicFFParametersArray<T> &params);
    
    void update(int idx, const FFParameters &params);
    void update(const QVector<int> &idxs, const FFParametersArray &params);
    
    void remove(int idx);
    void remove(const QVector<int> &idxs);
    
private:
    /** The packed array of all of the bead's parameters */
    SireBase::PackedArray2D<T> params;
};

}

SIRE_END_HEADER

#endif
