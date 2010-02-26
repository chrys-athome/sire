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
#include <QVarLengthArray>
#include <QUuid>

#include "stream.h"

SIREN_BEGIN_HEADER

Siren::Stream& operator&(Siren::Stream&, QDateTime&);
Siren::Stream& operator&(Siren::Stream&, QDate&);
Siren::Stream& operator&(Siren::Stream&, QTime&);
Siren::Stream& operator&(Siren::Stream&, QLocale&);
Siren::Stream& operator&(Siren::Stream&, QUuid&);

namespace Siren
{
    namespace detail
    {
        template<> 
        struct StreamHelper<QString>
        {
            static QString typeName(){ return "QString"; }
            
            static const void* getKey(const QString &object)
            {
                return object.constData();
            }
            
            static QString null(){ return QString::null; }
        };
        
        template<>
        struct StreamHelper<QUuid>
        {
            static QUuid typeName(){ return "QUuid"; }
            
            static const void* getKey(const QUuid &object)
            {
                return &object;
            }
            
            static QUuid null(){ return QUuid(); }
        };
        
        template<class T>
        struct StreamHelper< QList<T> >
        {
            static QString typeName()
            {
                return QString("QList< %1 >").arg( StreamHelper<T>::typeName() );
            }
            
            static const void* getKey(const QList<T> &object)
            {
                if (object.isEmpty())
                    return 0;
                else
                    return &(object.first());
            }
            
            static QList<T> null(){ return QList<T>(); }
        };
        
        template<class T>
        struct StreamHelper< QVector<T> >
        {
            static QString typeName()
            {
                return QString("QVector< %1 >").arg( StreamHelper<T>::typeName() );
            }
            
            static const void* getKey(const QVector<T> &object)
            {
                return object.constData();
            }
            
            static QVector<T> null(){ return QVector<T>(); }
        };
        
        template<class T>
        struct StreamHelper< QSet<T> >
        {
            static QString typeName()
            {
                return QString("QSet< %1 >").arg( StreamHelper<T>::typeName() );
            }
            
            static const void* getKey(const QSet<T> &object)
            {
                if (object.isEmpty())
                    return 0;
                else
                    return &(*(object.constBegin()));
            }
            
            static QSet<T> null(){ return QSet<T>(); }
        };
        
        template<class Key, class Value>
        struct StreamHelper< QHash<Key,Value> >
        {
            static QString typeName()
            {
                return QString("QHash< %1, %2 >")
                            .arg( StreamHelper<Key>::typeName(),
                                  StreamHelper<Value>::typeName() );
            }
            
            static const void* getKey(const QHash<Key,Value> &object)
            {
                if (object.isEmpty())
                    return 0;
                else
                    return &(object.constBegin().value());
            }
            
            static QHash<Key,Value> null() { return QHash<Key,Value>(); }
        };
    }
}

/** Streaming operator for QVarLengthArray<T> */
template<class T, int N>
SIREN_OUTOFLINE_TEMPLATE
Siren::Stream& operator&(Siren::Stream &s, QVarLengthArray<T,N> &array)
{
    s.assertVersion("QVarLengthArray", 1);
    
    Siren::ArraySchema schema = s.array<T>(array.count());
    
    if (s.isSaving())
    {
        const T *data = array.constData();
        
        for (int i=0; i<schema.count(); ++i)
        {
            s << data[i];
        }
    }
    else
    {
        array.resize(schema.count());
        
        T *data = array.data();
        
        for (int i=0; i<schema.count(); ++i)
        {
            s >> data[i];
        }
    }
    
    return s;
}

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
        Siren::ArraySchema schema = s.array<T>(vector.count());
    
        if (s.isSaving())
        {
            const T *array = vector.constData();
        
            for (int i=0; i<schema.count(); ++i)
            {
                schema.index(i) << array[i];
            }
        }
        else
        {
            vector.resize(schema.count());
        
            T *array = vector.data();
        
            for (int i=0; i<schema.count(); ++i)
            {
                schema.index(i) >> array[i];
            }
        }  
    }
    
    return s;
}

/** Streaming operator for QList<T> */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
Siren::Stream& operator&(Siren::Stream &s, QList<T> &list)
{
    s.assertVersion("QList", 1);
    
    Siren::SharedSchema shared = s.shared(list);
    
    if (shared.mustStream())
    {
        Siren::ArraySchema schema = s.array<T>(list.count());
        
        if (s.isSaving())
        {
            int i = 0;
        
            for (typename QList<T>::const_iterator it = list.constBegin();
                 it != list.constEnd();
                 ++it)
            {
                schema.index(i) << *it;
                ++i;
            }
        }
        else
        {
            list.clear();
            
            for (int i=0; i<schema.count(); ++i)
            {
                T val;
                schema.index(i) >> val;
                
                list.append(val);
            }
        }
    }
    
    return s;
}

/** Streaming operator for QSet<T> */
template<class T>
SIREN_OUTOFLINE_TEMPLATE
Siren::Stream& operator&(Siren::Stream &s, QSet<T> &set)
{
    s.assertVersion("QSet", 1);
    
    Siren::SharedSchema shared = s.shared(set);
    
    if (shared.mustStream())
    {
        Siren::SetSchema schema = s.set<T>(set.count());
        
        if (s.isSaving())
        {
            for (typename QSet<T>::const_iterator it = set.constBegin();
                 it != set.constEnd();
                 ++it)
            {
                schema.entry() << *it;
            }
        }
        else
        {
            set.clear();
            set.reserve(schema.count());
            
            for (int i=0; i<schema.count(); ++i)
            {
                T value;
                schema.entry() >> value;
                
                set.insert(value);
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
        Siren::MapSchema schema = s.map<Key,Value>(hash.count());
        
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
            hash.reserve(schema.count());
            
            for (int i=0; i<schema.count(); ++i)
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
