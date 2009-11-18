/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2009  Christopher Woods
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

#ifndef SIREN_STREAMQT_H
#define SIREN_STREAMQT_H

#include <QVector>
#include <QHash>
#include <QSet>
#include <QMap>

#include "stream.h"

SIREN_BEGIN_HEADER

/** Streaming operator for QVector<T> */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
Siren::Stream& operator&(Siren::Stream &s, QVector<T> &vector)
{
    s.assertVersion("QVector", 1);

    Siren::SharedSchema shared = s.shared(vector);

    if (shared.mustStream())
    {
        //this vector has not yet been streamed
        int sz = vector.count();

        Siren::ArraySchema schema = s.array<T>(sz);
    
        if (s.isSaving())
        {
            const T *array = vector.constData();
        
            for (int i=0; i<sz; ++i)
            {
                schema.index(i) << array[i];
            }
        }
        else
        {
            vector.resize(sz);
        
            T *array = vector.data();
        
            for (int i=0; i<sz; ++i)
            {
                schema.index(i) >> array[i];
            }
        }  
    }
    
    return s;
}

/** Streaming operator for QHash<Key,Value> */
template<class Key, class Value>
SIREN_OUTOFLINE_TEMPLATE
Siren::Stream& operator&(Siren::Stream &s, QHash<Key,Value> &hash)
{
    s.assertVersion("QHash", 1);

    Siren::SharedSchema shared = s.shared(hash);

    if (shared.mustStream())
    {
        int sz = hash.count();
    
        Siren::MapSchema schema = s.map<Key,Value>(sz);
        
        if (s.isSaving())
        {
            for (typename QHash<Key,Value>::const_iterator 
                                it = hash.constBegin();
                 it != hash.constEnd();
                 ++it)
            {
                schema.entry(it.key()) << it.value();
            }
        }
        else
        {
            hash.clear();
            hash.reserve(sz);
            
            for (int i=0; i<sz; ++i)
            {
                Key key;
                Value value;
                
                schema.entry(key) >> value;
                
                hash.insert(key, value);
            }
        }
    }
    
    return s;
}

SIREN_END_HEADER

#endif
