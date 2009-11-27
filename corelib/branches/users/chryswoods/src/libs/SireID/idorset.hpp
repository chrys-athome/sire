/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2008  Christopher Woods
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

#ifndef SIREID_IDORSET_HPP
#define SIREID_IDORSET_HPP

#include <QSet>
#include <QString>
#include <QStringList>

#include "SireID/id.h"

SIRE_BEGIN_HEADER

namespace SireID
{

/** This class holds a set of IDs, thereby allowing for
    "or" matching of IDs 
    
    @author Christopher Woods
*/
template<class ID>
class SIREID_EXPORT IDOrSet : public Siren::Implements< IDOrSet<ID>, ID >
{
public:
    typedef typename ID::Identifier Identifier;
    typedef typename ID::Index Index;
    typedef typename ID::SearchObject SearchObject;

    IDOrSet();
    IDOrSet(const ID &id);
    IDOrSet(const ID &id0, const ID &id1);
    
    IDOrSet(const QList<typename ID::Identifier> &ids);
    
    template<class T>
    IDOrSet(const T &ids);
    
    IDOrSet(const IDOrSet<ID> &other);
    
    ~IDOrSet();
    
    bool isNull() const;
    
    uint hashCode() const;
    QString toString() const;
    void stream(Siren::Stream &s);

    const QSet<Identifier>& IDs() const;
    
    IDOrSet<ID>& operator=(const IDOrSet<ID> &other);
    IDOrSet<ID>& operator=(const ID &other);
    
    bool operator==(const SireID::ID &other) const;
    bool operator!=(const SireID::ID &other) const;
   
    bool operator==(const IDOrSet<ID> &other) const;
    bool operator!=(const IDOrSet<ID> &other) const;
    
    bool operator==(const ID &other) const;
    bool operator!=(const ID &other) const;
    
    QList<Index> map(const SearchObject &obj) const;

private:
    void add(const ID &id);

    QSet<typename ID::Identifier> ids;
};

#ifndef SIRE_SKIP_INLINE_FUNCTIONS

/** Null constructor */
template<class ID>
SIRE_OUTOFLINE_TEMPLATE
IDOrSet<ID>::IDOrSet() 
            : Siren::Implements< IDOrSet<ID>, ID >()
{}

/** Add the passed ID to the list */
template<class ID>
SIRE_OUTOFLINE_TEMPLATE
void IDOrSet<ID>::add(const ID &id)
{
    if (id.isNull())
        return;

    else if (id.template isA<Identifier>())
    {
        this->add(id.template asA<Identifier>().base());
    }
    else if (id.template isA< IDOrSet<ID> >())
        ids += id.template asA< IDOrSet<ID> >().ids;
    else
        ids.insert( Identifier(id) );
}

/** Construct from the passed ID */
template<class ID>
SIRE_OUTOFLINE_TEMPLATE
IDOrSet<ID>::IDOrSet(const ID &id)
            : Siren::Implements< IDOrSet<ID>, ID >()
{
    this->add(id);
}

/** Construct from the passed IDs */
template<class ID>
SIRE_OUTOFLINE_TEMPLATE
IDOrSet<ID>::IDOrSet(const ID &id0, const ID &id1)
            : Siren::Implements< IDOrSet<ID>, ID >()
{
    this->add(id0);
    this->add(id1);
}

/** Construct from the passed list of IDs */
template<class ID>
SIRE_OUTOFLINE_TEMPLATE
IDOrSet<ID>::IDOrSet(const QList<typename ID::Identifier> &new_ids)
            : Siren::Implements< IDOrSet<ID>, ID >()
{
    for (typename QList<Identifier>::const_iterator it = new_ids.constBegin();
         it != new_ids.constEnd();
         ++it)
    {
        this->add(it->base());
    }
}

/** Construct from the passed list of IDs */
template<class ID>
template<class T>
SIRE_OUTOFLINE_TEMPLATE
IDOrSet<ID>::IDOrSet(const T &new_ids)
            : Siren::Implements< IDOrSet<ID>, ID >()
{
    for (typename T::const_iterator it = new_ids.constBegin();
         it != new_ids.constEnd();
         ++it)
    {
        this->add(*it);
    }
}

/** Copy constructor */
template<class ID>
SIRE_OUTOFLINE_TEMPLATE
IDOrSet<ID>::IDOrSet(const IDOrSet &other)
            : Siren::Implements< IDOrSet<ID>, ID >(others), ids(other.ids)
{}

/** Destructor */
template<class ID>
SIRE_OUTOFLINE_TEMPLATE
IDOrSet<ID>::~IDOrSet()
{}

/** Is this selection null? */
template<class ID>
SIRE_OUTOFLINE_TEMPLATE
bool IDOrSet<ID>::isNull() const
{
    return ids.isEmpty();
}

/** Return a hash of this identifier */
template<class ID>
SIRE_OUTOFLINE_TEMPLATE
uint IDOrSet<ID>::hashCode() const
{
    uint h = 0;
    
    for (typename QSet<Identifier>::const_iterator it = ids.constBegin();
         it != ids.constEnd();
         ++it)
    {
        h += it->hash();
    }
    
    return h;
}

template<class ID>
SIRE_OUTOFLINE_TEMPLATE
void IDOrSet<ID>::stream(Siren::Stream &s)
{
    s.assertVersion< IDOrSet<ID> >(1);
    
    Siren::Schema schema = s.item< IDOrSet<ID> >();
    
    schema.data("identifiers") & ids;
    
    ID::stream( schema.base() );
}
            
/** Return a string representatio of this ID */
template<class ID>
SIRE_OUTOFLINE_TEMPLATE
QString IDOrSet<ID>::toString() const
{
    if (ids.isEmpty())
        return QObject::tr("null");
    else
    {
        QStringList idstrings;
        
        for (typename QSet<Identifier>::const_iterator it = ids.constBegin();
             it != ids.constEnd();
             ++it)
        {
            idstrings.append( it->toString() );
        }
    
        return idstrings.join( QObject::tr(" or ") );
    }
}

/** Return all of the IDs in this set */
template<class ID>
SIRE_OUTOFLINE_TEMPLATE
const QSet<typename ID::Identifier>& IDOrSet<ID>::IDs() const
{
    return ids;
}

/** Copy assignment operator */
template<class ID>
SIRE_OUTOFLINE_TEMPLATE
IDOrSet<ID>& IDOrSet<ID>::operator=(const IDOrSet<ID> &other)
{
    ids = other.ids;
    return *this;
}

/** Copy assignment operator */
template<class ID>
SIRE_OUTOFLINE_TEMPLATE
IDOrSet<ID>& IDOrSet<ID>::operator=(const ID &other)
{
    ids.clear();
    this->add(other);
    
    return *this;
}

/** Comparison operator */
template<class ID>
SIRE_OUTOFLINE_TEMPLATE
bool IDOrSet<ID>::operator==(const SireID::ID &other) const
{
    return SireID::ID::compare< IDOrSet<ID> >(*this, other);
}

/** Comparison operator */
template<class ID>
SIRE_OUTOFLINE_TEMPLATE
bool IDOrSet<ID>::operator!=(const SireID::ID &other) const
{
    return not this->operator==(other);
}

/** Comparison operator */
template<class ID>
SIRE_OUTOFLINE_TEMPLATE
bool IDOrSet<ID>::operator==(const IDOrSet<ID> &other) const
{
    return ids == other.ids;
}

/** Comparison operator */
template<class ID>
SIRE_OUTOFLINE_TEMPLATE
bool IDOrSet<ID>::operator!=(const IDOrSet<ID> &other) const
{
    return ids != other.ids;
}

/** Comparison operator */
template<class ID>
SIRE_OUTOFLINE_TEMPLATE
bool IDOrSet<ID>::operator==(const ID &other) const
{
    return this->operator==( IDOrSet<ID>(other) );
}

/** Comparison operator */
template<class ID>
SIRE_OUTOFLINE_TEMPLATE
bool IDOrSet<ID>::operator!=(const ID &other) const
{
    return this->operator!=( IDOrSet<ID>(other) );
}

/** Map this ID to the list of indicies that match this ID */
template<class ID>
SIRE_OUTOFLINE_TEMPLATE
QList<typename ID::Index> IDOrSet<ID>::map(const typename ID::SearchObject &obj) const
{
    if (ids.isEmpty())
        return Identifier().map(obj);
        
    QList<Index> idxs;
        
    for (typename QSet<Identifier>::const_iterator it = ids.constBegin();
         it != ids.constEnd();
         ++it)
    {
        try
        {
            idxs += it->map(obj);
        }
        catch(...)
        {
            //no match
        }
    }
    
    QSet<Index> set;
    set.reserve(idxs.count());
    
    QMutableListIterator<Index> it(idxs);
    
    while (it.hasNext())
    {
        it.next();
    
        if (set.contains(it.value()))
            it.remove();
        else
            set.insert(it.value());
    }

    ID::processMatches(idxs, obj);

    return idxs;
}

#endif // SIRE_SKIP_INLINE_FUNCTIONS

}

SIRE_END_HEADER

#endif
