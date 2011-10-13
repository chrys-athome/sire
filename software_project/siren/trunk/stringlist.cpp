/********************************************\
  *
  *  Siren - C++ metaobject library
  *
  *  Copyright (C) 2011  Christopher Woods
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

#include "Siren/stringlist.h"
#include "Siren/exceptions.h"
#include "Siren/siren.hpp"

using namespace Siren;

REGISTER_SIREN_CLASS( Siren::StringList )

/** Empty constructor */
StringList::StringList() : Object()
{}

/** Construct to hold just a single string */
StringList::StringList(const String &str) : Object()
{
    d.append(str);
}

/** Construct to hold the array of strings */
StringList::StringList(const String *strs, int count) : Object()
{
    SIREN_ASSERT( strs != 0 );

    for (int i=0; i<count; ++i)
    {
        d.append( strs[i] );
    }
}

/** Construct from the passed list of strings */
StringList::StringList(const List<String>::Type &strs) : Object(), d(strs)
{}

#ifdef SIREN_QT_SUPPORT
StringList::StringList(const QStringList &strlist) : Object()
{
    for (QStringList::const_iterator it = strlist.constBegin();
         it != strlist.constEnd();
         ++it)
    {
        d.append( String(*it) );
    }
}

StringList::operator QStringList() const
{
    QStringList strlist;
    
    for (List<String>::const_iterator it = d.constBegin();
         it != d.constEnd();
         ++it)
    {
        strlist.append( *it );
    }
    
    return strlist;
}
#endif

/** Copy constructor */
StringList::StringList(const StringList &other) : Object(other), d(other.d)
{}

/** Destructor */
StringList::~StringList()
{}

/** Copy assignment function */
void StringList::copy_object(const StringList &other)
{
    d = other.d;
    super::copy_object(other);
}

/** Comparison operator */
bool StringList::compare_object(const StringList &other) const
{
    return d == other.d and super::operator==(other);
}

/** Assert that "i" indexes a valid position in the list 

    \throw Siren::invalid_index
*/
void StringList::assertValidIndex(int i) const
{
    if (i >= d.count() or i < -d.count())
        throw Siren::invalid_index( String::tr(
                "Index %1 is not valid for a StringList containing "
                "a number of strings equal to %2.")
                    .arg(i).arg(d.count()), CODELOC );
}

/** Return the ith string in the list

    \throw Siren::invalid_index
*/
const String& StringList::at(int i) const
{
    assertValidIndex(i);
    return d.at(i);
}

/** Return the ith string in the list

    \throw Siren::invalid_index
*/
const String& StringList::operator[](int i) const
{
    assertValidIndex(i);
    return d[i];
}

/** Return the number of strings in the list */
int StringList::count() const
{
    return d.count();
}

/** Return the number of strings in the list */
int StringList::size() const
{
    return d.count();
}

/** Return whether or not the list is empty */
bool StringList::isEmpty() const
{
    return d.isEmpty();
}

/** Return whether or not this list contains the passed string */
bool StringList::contains(const String &str) const
{
    return d.contains(str);
}

/** Return the underlying, editable list of strings */
List<String>::Type StringList::toList() const
{
    return d;
}

/** Construct a StringList from an editable list of strings */
StringList StringList::fromList(const List<String>::Type &list)
{
    return StringList(list);
}

/** Allow automatic casting to an editable list of strings */
StringList::operator List<String>::Type() const
{
    return d;
}

/** Join the strings together, separated by the separator 'sep' */
String StringList::join(const String &sep) const
{
    if (d.isEmpty())
        return String();
    
    else if (d.count() == 1)
        return d.at(0);
        
    else
    {
        int sz = 0;
    
        for (List<String>::const_iterator it = d.constBegin();
             it != d.constEnd();
             ++it)
        {
            sz += it->length();
        }
    
        if (sz == 0)
            return String();
            
        QString s;
        s.reserve(sz);
        
        bool is_first = true;
        
        for (List<String>::const_iterator it = d.constBegin();
             it != d.constEnd();
             ++it)
        {
            if (it->isEmpty())
                continue;
        
            if (not is_first)
            {
                s += sep;
            }
            else
                is_first = false;
                
            s += *it;
        }

        return String(s);
    }
}
