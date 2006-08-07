#ifndef SIREDB_MULTIMATCH_HPP
#define SIREDB_MULTIMATCH_HPP

#include <QMap>
#include <QString>
#include <QStringList>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireDB
{

/** This class is used by MatchData to hold match data either for all atoms in a group,
    or for each individual atom in the group.
    
    @author Christopher Woods
*/
template<class T>
class MultiMatch
{
public:
    MultiMatch();
    MultiMatch(const T &matchdata);
    MultiMatch(uint atm, const T &matchdata);
    
    MultiMatch(const MultiMatch<T> &other);
    
    ~MultiMatch();
    
    MultiMatch<T> operator&&(const MultiMatch<T> &other) const;
    
    QString toString() const;
    
    bool isNull() const;
    bool isEmpty() const;
    
    T getMatch(uint atm) const;
    int nMatches() const;
    
    void set(uint atm, const T &value);
    void set(const T &value);
    
    void simplify(uint maxatm);
    
private:

    /** Map of the match data */
    QMap<int,T> data;
};

/** Create an empty match */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
MultiMatch<T>::MultiMatch()
{}
    
/** Create a match that matches all atoms using the same criteria */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
MultiMatch<T>::MultiMatch(const T &matchdata) 
{
    data.insert(-1, matchdata);
}
    
/** Create a match that matches atoms using different criteria, with atom number 'atm'
    matched using the criteria in 'matchdata' */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
MultiMatch<T>::MultiMatch(uint atm, const T &matchdata)
{
    data.insert(atm, matchdata);
}
    
/** Copy constructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
MultiMatch<T>::MultiMatch(const MultiMatch<T> &other)
              : data(other.data)
{}
    
/** Destructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
MultiMatch<T>::~MultiMatch()
{}

/** Return whether this object is null (contains no match criteria) */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool MultiMatch<T>::isNull() const
{
    return data.isEmpty();
}

/** Return whether or not this object is empty (contains no match criteria) */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
bool MultiMatch<T>::isEmpty() const
{
    return data.isEmpty();
}
    
/** Return a string representation of this data */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
QString MultiMatch<T>::toString() const
{
    QStringList parts;
        
    for ( typename QMap<int,T>::const_iterator it = data.begin();
          it != data.end();
          ++it )
    {
        if (not it.value().isEmpty())
        {
            if (it.key() == -1)
                parts.append(it.value().toString());
            else
                parts.append(it.value().toString(it.key()));
        }
    }
        
    if (parts.isEmpty())
        return "*";
    else
        return parts.join( QObject::tr(" and ") );
}
    
/** Return the number of matches - this will be 0 if this is null, 
    1 if this is a single match, 1 if it is a multi match that only 
    matches atm == 0, and n+1, where n is the largest atom number with
    specified match criteria. This means that if this only contains
    match criteria for atom3, then there are 4 matches, as atom0,atom1,atom2
    are implied by the presence of atom3, and atom0-2 will be given 
    default, empty matching criteria. */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
int MultiMatch<T>::nMatches() const
{
    if (data.isEmpty())
        return 0;
    else
    {
        //return the value of the last item in the map, + 1
        typename QMap<int,T>::const_iterator it = data.constEnd();
        --it;
        
        //this allows counting of -1 as a single match
        return qMax(1, it.key() + 1);
    }
}

/** Return the criteria matching atom 'atm'. Note that if there is a 
    contradiction between the global match and the specific match then
    the specific match is used, e.g. res_name == "ALA" and res0_name == "ASP"
    would return "ASP" for atm == 0 and "ALA" for all other atoms. */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
T MultiMatch<T>::getMatch(uint atm) const
{
    return data.value(-1,T()) and data.value(atm,T());
}

/** And-combine two MultiMatch objects together. */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
MultiMatch<T> MultiMatch<T>::operator&&(const MultiMatch<T> &other) const
{
    if (other.data.isEmpty())
        return *this;
    else if (data.isEmpty())
        return other;
    else
    {
        MultiMatch<T> ret(*this);
        
        for (typename QMap<int,T>::const_iterator it = other.data.begin();
             it != other.data.end();
             ++it)
        {
            ret.data.insert( it.key(), data.value(it.key(),T()) and it.value() );
        }
        
        return ret;
    }
}

/** Set the value of the nth atom to 'val' */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void MultiMatch<T>::set(uint n, const T &val)
{
    data.insert(n, val);
}

/** Set the whole match to be equal to 'val' */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void MultiMatch<T>::set(const T &val)
{
    data.clear();
    data.insert(-1, val);
}

/** Simplify the data - this will collect together all 
    of the data into the global value if all of the values 
    have the same data. Note that this will only work if
    there is no data for atoms with index greater than
    'maxatm', and if each entry from 0->maxatm-1 is
    populated */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
void MultiMatch<T>::simplify(uint maxatm)
{
    if (not data.contains(0))
        return;
    else
    {
        T val0 = data.value(0);
        
        for (uint i=1; i<maxatm; ++i)
        {
            if (not data.contains(i))
                return;
            else if (data.value(i) != val0)
                return;
        }
        
        if (data.contains(-1) and data.value(-1) != val0)
            return;
        
        //everything is the same - move val0 into the global
        //position
        data.clear();
        data.insert(-1, val0);
    }
}

}

SIRE_END_HEADER

#endif
