#ifndef SIREN_QT4SUPPORT_H
#define SIREN_QT4SUPPORT_H

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

// Definition of Qt classes that are required for Siren.
// These will (eventually!) be replaced with Siren's own classes,
// thereby removing the dependency on Qt

#include <QString>
#include <QStringList>
#include <QChar>
#include <QMutex>
#include <QMutexLocker>
#include <QReadWriteLock>
#include <QByteArray>
#include <QDataStream>
#include <QSharedData>
#include <QSharedDataPointer>

#include <QThread>
#include <QThreadStorage>

#include <QHash>
#include <QMutableHashIterator>
#include <QHashIterator>

#include <QVector>
#include <QMutableVectorIterator>
#include <QVectorIterator>

#include <QList>
#include <QMutableListIterator>
#include <QListIterator>

#include <QSet>
#include <QMutableSetIterator>
#include <QSetIterator>

#include <QCache>
#include <QAtomicInt>

#include <QDebug>

SIREN_BEGIN_HEADER

#define SIREN_QT_SUPPORT 1

namespace Siren
{
    typedef qint8 int8;
    typedef quint8 uint8;
    typedef qint32 int32;
    typedef quint32 uint32;
    typedef qint64 int64;
    typedef quint64 uint64;

    typedef float float32;
    typedef double float64;
    typedef long double float128;

    typedef QMutex Mutex;
    typedef QMutexLocker MutexLocker;
    typedef QReadWriteLock ReadWriteLock;
    typedef QReadLocker ReadLocker;
    typedef QWriteLocker WriteLocker;
    typedef QAtomicInt AtomicInt;
    typedef QDataStream DataStream;
    typedef QSharedData SharedData;
    typedef QChar Char;
    typedef QLatin1Char Latin1Char;
    typedef QLatin1String Latin1String;
    
    template<class T>
    struct imp_shared_ptr
    {
        typedef QSharedDataPointer<T> Type;
    };
    
    template<class T>
    struct Vector
    {
        typedef QVector<T> Type;
        typedef QMutableVectorIterator<T> MutableIterator;
        typedef QVectorIterator<T> Iterator;
        typedef typename QVector<T>::iterator iterator;
        typedef typename QVector<T>::const_iterator const_iterator;
    };

    template<class T>
    struct List
    {
        typedef QList<T> Type;
        typedef QMutableListIterator<T> MutableIterator;
        typedef QListIterator<T> Iterator;
        typedef typename QList<T>::iterator iterator;
        typedef typename QList<T>::const_iterator const_iterator;
    };

    template<class Key, class Value>
    struct Hash
    {
        typedef QHash<Key,Value> Type;
        typedef QMutableHashIterator<Key,Value> MutableIterator;
        typedef QHashIterator<Key,Value> Iterator;
        typedef typename QHash<Key,Value>::iterator iterator;
        typedef typename QHash<Key,Value>::const_iterator const_iterator;
    };

    template<class T>
    struct Set
    {
        typedef QSet<T> Type;
        typedef QMutableSetIterator<T> MutableIterator;
        typedef QSetIterator<T> Iterator;
        typedef typename QSet<T>::iterator iterator;
        typedef typename QSet<T>::const_iterator const_iterator;
    };

    template<class Key, class Value>
    struct Cache
    {
        typedef QCache<Key,Value> Type;
    };

    #if !defined(QT_NO_DEBUG_STREAM)
    inline QDebug sirenDebug() { return qDebug(); }
    #endif

} // end of namespace Siren

SIREN_END_HEADER

#endif // SIREN_QT4SUPPORT_H

