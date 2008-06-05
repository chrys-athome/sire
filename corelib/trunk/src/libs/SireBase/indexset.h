#ifndef SIREBASE_INDEXSET_H
#define SIREBASE_INDEXSET_H

#include <QSet>
#include <QSetIterator>
#include <QList>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireBase
{

typedef qint32 Index;
typedef QSet<Index> IndexSet;
typedef QList<Index> IndexList;

/** Return whether or not two QSets are intersecting */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool intersect(const QSet<T> &set0, const QSet<T> &set1)
{
    if (set0.count() <= set1.count())
    {
        QSetIterator<T> it(set0);
        while (it.hasNext())
            if (set1.contains( it.next() ))
                return true;
    }
    else
    {
        QSetIterator<T> it(set1);
        while (it.hasNext())
            if (set0.contains( it.next() ))
                return true;
    }
    
    return false;
}

/** Return the set of indexes in 'container' of the objects contained in 'tofind', with 
    the returned IndexSet ordered by the order in tofind. If 'tofind' contains objects that
    are not in 'container', then those objects are ignored. Note that container *must*
    implement the 'indexOfKey' and 'has_key' functions (e.g. it could be derived from SireBase::Set),
    while 'tofind' just have to provide an indexing function (tofind::operator[]).
    
    Note also that the type held in tofind must be compatible with the type used by
    the container to index itself, e.g. an AtomSet uses AtomIndexes to index itself,
    so tofind must be a list or set of Atoms or AtomIndexes.
    
    To give an example, if container contains the objects;
        
    index:    0   1   2   3   4   5   6   7
    object:   c   a   t   s   l   i   k   e
    
    and tofind contains
    
    index:    0   1   2   3   4
    object:   g   k   s   c   l
    
    then the returned IndexSet will contain;
    
    index:    0   1   2   3 
    object:   6   3   0   4
    
    ( 'g' is not in container, so it is ignored. 'k' is at index 6, 's' is at index 3,
      'c' is at index 0 and 'l' is at index 4 )
*/
template<class C, class I>
SIRE_OUTOFLINE_TEMPLATE
IndexSet getIndexes(const C &container, const I &tofind)
{
    IndexSet indexes;
    
    int sz = tofind.count();
    for (int i=0; i<sz; i++)
    {
        if (container.has_key(tofind[i]))
            indexes.insert( container.indexOfKey(tofind[i]) );
    }
    
    return indexes;
}

}

SIRE_END_HEADER

#endif
