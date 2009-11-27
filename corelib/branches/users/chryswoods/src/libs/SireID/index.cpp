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

#include "index.h"

#include "Siren/errors.h"
#include "Siren/stream.h"

using namespace SireID;
using namespace Siren;

////////
//////// Implementation of IndexBase
////////

static const RegisterObject<IndexBase> r_indexbase( VIRTUAL_CLASS );

IndexBase::IndexBase(qint32 idx) : Extends<IndexBase,ID>(), _idx(idx)
{}

IndexBase::IndexBase(const IndexBase &other) 
          : Extends<IndexBase,ID>(other), _idx(other._idx)
{}

IndexBase::~IndexBase()
{}

IndexBase& IndexBase::operator=(const IndexBase &other)
{
    _idx = other._idx;
    ID::operator=(other);
    
    return *this;
}

bool IndexBase::operator==(const IndexBase &other) const
{
    return _idx == other._idx;
}

bool IndexBase::operator!=(const IndexBase &other) const
{
    return _idx != other._idx;
}

/** Return the null index */
qint32 IndexBase::null()
{
    return std::numeric_limits<qint32>::min();
}

QString IndexBase::typeName()
{
    return "SireID::IndexBase";
}

/** Return whether this is a null index - a null
    index is one that equals std::numeric_limits<qint32>::min(),
    which should be -21474833648 for a 32bit integer */
bool IndexBase::isNull() const
{
    return _idx == IndexBase::null();
}

/** Hash this Index */
uint IndexBase::hashCode() const
{
    return quint32(_idx);
}

void IndexBase::stream(Stream &s)
{
    s.assertVersion<IndexBase>(1);
    
    Schema schema = s.item<IndexBase>();
    
    schema.data("index") & _idx;
    
    ID::stream( schema.base() );
}

/** Return the raw value of this index */
qint32 IndexBase::value() const
{
    return _idx;
}

/** Allow implicit conversion back to an int */
IndexBase::operator qint32() const
{
    return _idx;
}
    
void IndexBase::throwInvalidIndex(qint32 n) const
{
    if (n == 0)
        throw Siren::invalid_index(QObject::tr(
            "Cannot access item at index %1 as the container is empty!")
                .arg(_idx), CODELOC );

    else if (n == 1)
        throw Siren::invalid_index(QObject::tr(
            "Cannot access item at index %1 as there is only item in the "
            "container.").arg(_idx), CODELOC );
    
    else
        throw Siren::invalid_index( QObject::tr(
            "No item at index %1. Index range is from %2 to %3.")
                .arg(_idx).arg(-n).arg(n-1), CODELOC );
}

/** Map this index into the container of 'n' elements - this
    maps the index (with negative indexing, e.g. -1 is the last
    element), and throws an exception if the index is out 
    of the bounds of the array
    
    \throw SireError::invalid_index
*/
qint32 IndexBase::map(qint32 n) const
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

////////
//////// Implementation of Index
////////

static const RegisterObject<Index> r_index;

Index::Index(qint32 idx) : Implements< Index,IndexType<Index> >(idx)
{}

Index::Index(const Index &other) : Implements< Index,IndexType<Index> >(other)
{}

Index::~Index()
{}

Index Index::null()
{
    return Index();
}

QString Index::toString() const
{
    return QString("Index(%1)").arg(_idx);
}
