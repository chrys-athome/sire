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

#ifndef SIREID_IDSET_HPP
#define SIREID_IDSET_HPP

#include <QSet>
#include <QString>
#include <QStringList>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireID
{
template<class ID>
class IDSet;
}

template<class ID>
QDataStream& operator<<(QDataStream&, const SireID::IDSet<ID>&);
template<class ID>
QDataStream& operator>>(QDataStream&, SireID::IDSet<ID>&);

namespace SireID
{

template<class T>
uint qHash(const T &obj)
{
    return obj.hash();
}

/** This class holds a set of IDs of parts of a molecule, thereby allowing for
    "or" matching of IDs 
    
    @author Christopher Woods
*/
template<class ID>
class SIREID_EXPORT IDSet : public ID
{

friend QDataStream& ::operator<<<>(QDataStream&, const IDSet<ID>&);
friend QDataStream& ::operator>><>(QDataStream&, IDSet<ID>&);

public:
    typedef typename ID::Identifier Identifier;
    typedef typename ID::Index Index;
    typedef typename ID::SearchObject SearchObject;

    IDSet();
    IDSet(const ID &id);
    IDSet(const ID &id0, const ID &id1);
    
    IDSet(const QList<typename ID::Identifier> &ids);
    
    template<class T>
    IDSet(const T &ids);
    
    IDSet(const IDSet<ID> &other);
    
    ~IDSet();
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId< IDSet<ID> >() );
    }
    
    const char* what() const
    {
        return IDSet<ID>::typeName();
    }
    
    IDSet<ID>* clone() const
    {
        return new IDSet<ID>(*this);
    }
    
    bool isNull() const;
    
    uint hash() const;
                
    QString toString() const;

    const QSet<Identifier>& IDs() const;
    
    IDSet<ID>& operator=(const IDSet<ID> &other);
    IDSet<ID>& operator=(const ID &other);
    
    bool operator==(const SireID::ID &other) const;
    using SireID::ID::operator!=;
   
    bool operator==(const IDSet<ID> &other) const;
    bool operator!=(const IDSet<ID> &other) const;
    
    bool operator==(const ID &other) const;
    bool operator!=(const ID &other) const;
    
    QList<Index> map(const SearchObject &obj) const;

private:
    void add(const ID &id);

    QSet<typename ID::Identifier> ids;
};

/** Null constructor */
template<class ID>
SIRE_OUTOFLINE_TEMPLATE
IDSet<ID>::IDSet() : ID()
{}

/** Add the passed ID to the list */
template<class ID>
SIRE_OUTOFLINE_TEMPLATE
void IDSet<ID>::add(const ID &id)
{
    if (id.isNull())
        return;

    else if (id.template isA<Identifier>())
    {
        this->add(id.template asA<Identifier>().base());
    }
    else if (id.template isA< IDSet<ID> >())
        ids += id.template asA< IDSet<ID> >().ids;
    else
        ids.insert( Identifier(id) );
}

/** Construct from the passed ID */
template<class ID>
SIRE_OUTOFLINE_TEMPLATE
IDSet<ID>::IDSet(const ID &id) : ID()
{
    this->add(id);
}

/** Construct from the passed IDs */
template<class ID>
SIRE_OUTOFLINE_TEMPLATE
IDSet<ID>::IDSet(const ID &id0, const ID &id1) : ID()
{
    this->add(id0);
    this->add(id1);
}

/** Construct from the passed list of IDs */
template<class ID>
SIRE_OUTOFLINE_TEMPLATE
IDSet<ID>::IDSet(const QList<typename ID::Identifier> &new_ids) : ID()
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
IDSet<ID>::IDSet(const T &new_ids) : ID()
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
IDSet<ID>::IDSet(const IDSet &other) : ID(other), ids(other.ids)
{}

/** Destructor */
template<class ID>
SIRE_OUTOFLINE_TEMPLATE
IDSet<ID>::~IDSet()
{}

/** Is this selection null? */
template<class ID>
SIRE_OUTOFLINE_TEMPLATE
bool IDSet<ID>::isNull() const
{
    return ids.isEmpty();
}

/** Return a hash of this identifier */
template<class ID>
SIRE_OUTOFLINE_TEMPLATE
uint IDSet<ID>::hash() const
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
            
/** Return a string representatio of this ID */
template<class ID>
SIRE_OUTOFLINE_TEMPLATE
QString IDSet<ID>::toString() const
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
    
        return idstrings.join( QObject::tr(" and ") );
    }
}

/** Return all of the IDs in this set */
template<class ID>
SIRE_OUTOFLINE_TEMPLATE
const QSet<typename ID::Identifier>& IDSet<ID>::IDs() const
{
    return ids;
}

/** Copy assignment operator */
template<class ID>
SIRE_OUTOFLINE_TEMPLATE
IDSet<ID>& IDSet<ID>::operator=(const IDSet<ID> &other)
{
    ids = other.ids;
    return *this;
}

/** Copy assignment operator */
template<class ID>
SIRE_OUTOFLINE_TEMPLATE
IDSet<ID>& IDSet<ID>::operator=(const ID &other)
{
    ids.clear();
    this->add(other);
    
    return *this;
}

/** Comparison operator */
template<class ID>
SIRE_OUTOFLINE_TEMPLATE
bool IDSet<ID>::operator==(const SireID::ID &other) const
{
    return SireID::ID::compare< IDSet<ID> >(*this, other);
}

/** Comparison operator */
template<class ID>
SIRE_OUTOFLINE_TEMPLATE
bool IDSet<ID>::operator==(const IDSet<ID> &other) const
{
    return ids == other.ids;
}

/** Comparison operator */
template<class ID>
SIRE_OUTOFLINE_TEMPLATE
bool IDSet<ID>::operator!=(const IDSet<ID> &other) const
{
    return ids != other.ids;
}

/** Comparison operator */
template<class ID>
SIRE_OUTOFLINE_TEMPLATE
bool IDSet<ID>::operator==(const ID &other) const
{
    return this->operator==( IDSet<ID>(other) );
}

/** Comparison operator */
template<class ID>
SIRE_OUTOFLINE_TEMPLATE
bool IDSet<ID>::operator!=(const ID &other) const
{
    return this->operator!=( IDSet<ID>(other) );
}

/** Map this ID to the list of indicies that match this ID */
template<class ID>
SIRE_OUTOFLINE_TEMPLATE
QList<typename ID::Index> IDSet<ID>::map(const typename ID::SearchObject &obj) const
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

}

#ifndef SIRE_SKIP_INLINE_FUNCTIONS
/** Serialise to a binary datastream */
template<class ID>
SIRE_OUTOFLINE_TEMPLATE
QDataStream& operator<<(QDataStream &ds, const SireID::IDSet<ID> &idset)
{
    ds << idset.ids;
    return ds;
}

/** Extract from a binary datastream */
template<class ID>
SIRE_OUTOFLINE_TEMPLATE
QDataStream& operator>>(QDataStream &ds, SireID::IDSet<ID> &idset)
{
    ds >> idset.ids;
    return ds;
}
#endif // SIRE_SKIP_INLINE_FUNCTIONS

SIRE_END_HEADER

#endif
