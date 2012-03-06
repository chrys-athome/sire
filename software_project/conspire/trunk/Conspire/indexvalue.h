#ifndef CONSPIRE_INDEXVALUE_H
#define CONSPIRE_INDEXVALUE_H
/********************************************\
  *
  *  Conspire
  *
  *  Copyright (C) 2012  Christopher Woods
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

#include "value.h"

CONSPIRE_BEGIN_HEADER

namespace Conspire
{
    /** This class represents a single index option. An index is the
        index of an item in the list, e.g. the first item has index 1, the
        second item has index 2 etc. A negative index refers to counting
        from the back of the list, e.g. -1 is the last item, -2 is the next
        to last item etc. An index of 0 is a null index, showing that
        no item has been selected
        
        @author Christopher Woods
    */
    class CONSPIRE_EXPORT IndexValue : public Value
    {
        SIREN_CLASS(IndexValue,Value,1)
    
    public:
        IndexValue();
        IndexValue(int index);
        IndexValue(Qt::String index);
        IndexValue(Qt::DomElement elem);
        
        IndexValue(const IndexValue &other);
        
        ~IndexValue();
        
        int value() const;

    protected:
        Qt::DomElement toDomElement(Qt::DomDocument doc) const;
        
        Qt::String toValueString() const;
        Obj fromValueString(Qt::String value) const;

    private:
        /** The current index */
        Qt::int32 idx;
    };

    /** This simple class is used with the IndexList class */
    class CONSPIRE_EXPORT Range : public Object
    {
        SIREN_CLASS(Range,Object,1)
    
    public:
        Range();
        Range(int index);
        Range(int start, int stop, int step=1);
        Range(Qt::String range);
        Range(const Range &other);
        
        ~Range();
        
        Qt::String toString() const;
        
        bool contains(int i, int nvalues) const;
        
        int start() const;
        int stop() const;
        int jump() const;
        
        Qt::List<int>::Type indicies(int nvalues) const;
        Qt::List<int>::Type indicies(int i, int nvalues, int batchsize=1000) const;
        
        int nBatches(int nvalues, int batchsize=1000) const;

        int count(int nvalues) const;
        int count(int i, int nvalues, int batchsize=1000) const;

        int at(int i, int nvalues) const;

    protected:
        void copy_object(const Range &other);
        bool compare_object(const Range &other) const;

    private:
        /** The start value of the range */
        Qt::int32 _start;
        
        /** The end value of the range */
        Qt::int32 _end;
        
        /** The step to take between elements in the range */
        Qt::int32 _step;
    };

    /** This class represent a list of indicies (IndexValues). This is
        used to allow multiple indicies to be specified. Indicies can
        be specified individually, e.g.
        
        indicies = [ 1, 2, 3 ]
        
        or ranges can be specified, using "..", e.g.
        
        indicies = [ 1..10 ]
        
        would be every index from 1 to 10 inclusive
        
        A range can specify the jump between values, e.g.
        
        indicies = [ 2..(2)..10 ]
        
        would be from 2 to 10, with a jump of 2, or
        
        indicies = [ 1000..(100)..2000 ]
        
        would be from 1000 to 2000 in steps of 100.
        
        If a range is missing the last value, then it ranges to the end of the list, e.g.
        
        indicies = [ 10.. ]
        
        would be indicies 10, 11, 12 etc. until the end of the list, while
        
        indicies = [ 2..(2).. ]
        
        would be every even index.
        
        Negative indicies can be used as well, e.g.
        
        indicies = [ 1000..(100)..-1000 ]
        
        would be from index 1000 to the index 1000 from the end, in steps of 100.
        
        Indicies can also be reversed, e.g.
        
        indicies = [ 10..1 ]
        
        would be 10, 9, 8, 7, 6, 5, 4, 3, 2, 1
        
        Each of these can also be combined together using a comma to separate indicies, e.g.
        
        indicies = [ 1, 2, 3, 4, 5..9, 10..(10)..100, 200..(100).. ]
        
        would be every index from 1 to 10, then every 10th index from 10 to 100, then
        every hundreth index for the rest of the list.
        
        If the index list allows duplicates (allow_duplicates is true) then an 
        index can appear in the list multiple times. Otherwise, repeated indicies
        are silently dropped.
        
        The square brackets around the list of indicies are optional, e.g.
        
        indicies = 1, 2, 3, 4, 5..9, 10..(10)..100, 200..(100).. 
        
        would be perfectly ok. The square brackets can help though if the index list
        is split over multiple lines, as the parser will ensure that linebreaks within
        square brackets are ignored, e.g. you could write
        
        indicies = [ 1, 2, 3, 4,       # indicies 1 to 4
                     5..9,             # indicies 5 to 9
                     10..(10)..100     # every tenth index from 10 to 100
                     200..(100)..      # every hundreth index from 200 to the end
                   ]
        
        @author Christopher Woods
    */
    class CONSPIRE_EXPORT IndexList : public Value
    {
        SIREN_CLASS(IndexList,Value,1)
    
    public:
        IndexList();
        IndexList(int index);
        IndexList(int start, int end, int jump=1);
        IndexList(Range range);
        IndexList(Qt::List<int>::Type indicies, bool allow_duplicates=true);
        IndexList(Qt::List<Range>::Type indices, bool allow_duplicates=true);
        
        IndexList(Qt::String indicies, bool allow_duplicates=true);
        IndexList(Qt::DomElement elem);
        
        IndexList(const IndexList &other);
        
        ~IndexList();

        Qt::List<int>::Type indicies(int nvalues) const;
        Qt::List<int>::Type indicies(int i, int nvalues, int batchsize=1000) const;
        
        int nBatches(int nvalues, int batchsize=1000) const;

        int count(int nvalues) const;
        int count(int i, int nvalues, int batchsize=1000) const;

        int at(int i, int nvalues) const;

        bool allowDuplicates() const;

    protected:
        void copy_object(const IndexList &other);
        bool compare_object(const IndexList &other) const;
    
        Qt::DomElement toDomElement(Qt::DomDocument doc) const;
        
        Qt::String toValueString() const;
        Obj fromValueString(Qt::String value) const;

    private:
        /** The indicies, packaged as ranges */
        Qt::List<Range>::Type idxs;
        
        /** Whether or not to allow duplicate indicies */
        bool allow_duplicates;
    };

}

SIREN_EXPOSE_CLASS( Conspire::Range )
SIREN_EXPOSE_CLASS( Conspire::IndexList )

CONSPIRE_END_HEADER

#endif
