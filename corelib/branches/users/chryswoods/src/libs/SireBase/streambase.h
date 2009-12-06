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

#ifndef SIREBASE_STREAMBASE_H
#define SIREBASE_STREAMBASE_H

#include "Siren/stream.h"
#include "Siren/streamqt.h"

#include "chunkedhash.hpp"
#include "chunkedvector.hpp"
#include "packedarray2d.hpp"
#include "sparsematrix.hpp"

SIRE_BEGIN_HEADER

namespace Siren
{
    namespace detail
    {
        template<class Key, class Value, int N>
        struct StreamHelper< SireBase::ChunkedHash<Key,Value,N> >
        {
            static QString typeName()
            {
                return QString("SireBase::ChunkedHash< %1, %2 >")
                        .arg( StreamHelper<Key>::typeName(),
                              StreamHelper<Value>::typeName() );
            }
            
            static const void* getKey(const SireBase::ChunkedHash<Key,Value,N> &object)
            {
                if (object.isEmpty())
                    return 0;
                else
                    return object.shareKey();
            }
            
            static SireBase::ChunkedHash<Key,Value,N> null()
            { 
                return SireBase::ChunkedHash<Key,Value,N>(); 
            }
        };

        template<class T, int N>
        struct StreamHelper< SireBase::ChunkedVector<T,N> >
        {
            static QString typeName()
            {
                return QString("SireBase::ChunkedVector< %1 >")
                        .arg( StreamHelper<T>::typeName() );
            }
            
            static const void* getKey(const SireBase::ChunkedVector<T,N> &object)
            {
                if (object.isEmpty())
                    return 0;
                else
                    return object.shareKey();
            }
            
            static SireBase::ChunkedVector<T,N> null()
            { 
                return SireBase::ChunkedVector<T,N>(); 
            }
        };
        
        template<class T>
        struct StreamHelper< SireBase::PackedArray2D<T> >
        {
            static QString typeName()
            {
                return QString("SireBase::PackedArray2D< %1 >")
                        .arg( StreamHelper<T>::typeName() );
            }
            
            static const void* getKey(const SireBase::PackedArray2D<T> &object)
            {
                if (object.isEmpty())
                    return 0;
                
                else
                    return object.constData();
            }
            
            static SireBase::PackedArray2D<T> null()
            {
                return SireBase::PackedArray2D<T>();
            }
        };
        
        template<class T>
        struct StreamHelper< typename SireBase::PackedArray2D<T>::Array >
        {
            static QString typeName()
            {
                return QString("SireBase::PackedArray2D< %1 >::Array")
                        .arg( StreamHelper<T>::typeName() );
            }
            
            static const void* getKey(
                        const typename SireBase::PackedArray2D<T>::Array &object)
            {
                if (object.isEmpty())
                    return 0;
                
                else
                    return object.constData();
            }
            
            static typename SireBase::PackedArray2D<T>::Array null()
            {
                return SireBase::PackedArray2D<T>::Array();
            }
        };

        template<class T>
        struct StreamHelper< SireBase::SparseMatrix<T> >
        {
            static QString typeName()
            {
                return QString("SireBase::SparseMatrix< %1 >")
                        .arg( StreamHelper<T>::typeName() );
            }
            
            static const void* getKey(const SireBase::SparseMatrix<T> &object)
            {
                if (object.isEmpty())
                {
                    if (T() == object.defaultValue())
                        //this is really empty
                        return 0;
                }
                
                return object.shareKey();
            }
            
            static SireBase::SparseMatrix<T> null()
            { 
                return SireBase::SparseMatrix<T>(); 
            }
        };
    }
}

/** Streaming operator for SparseMatrix<T> */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Siren::Stream& operator&(Siren::Stream &s, SireBase::SparseMatrix<T> &matrix)
{
    s.assertVersion("SireBase::SparseMatrix", 1);
    
    Siren::SharedSchema shared = s.shared(matrix);
    
    if (shared.mustStream())
    {
        shared.data("default") & matrix.def;
        shared.data("is_symmetric") & matrix.is_symmetric;
        shared.data("is_transpose") & matrix.is_transpose;
        shared.data("data") & matrix.data;
    }
    
    return s;
}

/** Streaming operator for ChunkedVector<T> */
template<class T, int N>
SIRE_OUTOFLINE_TEMPLATE
Siren::Stream& operator&(Siren::Stream &s, SireBase::ChunkedVector<T,N> &vector)
{
    s.assertVersion("SireBase::ChunkedVector", 1);

    Siren::SharedSchema shared = s.shared(vector);

    if (shared.mustStream())
    {
        //this vector has not yet been streamed
        Siren::ArraySchema schema = s.array<T>(vector.count());
    
        if (s.isSaving())
        {
            for (int i=0; i<schema.count(); ++i)
            {
                schema.index(i) << vector.at(i);
            }
        }
        else
        {
            vector.resize(schema.count());
        
            for (int i=0; i<schema.count(); ++i)
            {
                schema.index(i) >> vector[i];
            }
        }  
    }
    
    return s;
}

/** Streaming operator for PackedArray2D<T>::Array */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Siren::Stream& operator&(Siren::Stream &s, 
                         typename SireBase::PackedArray2D<T>::Array &array)
{
    s.assertVersion("SireBase::PackedArray2D::Array", 1);
    
    Siren::SharedSchema shared = s.shared(array);
    
    if (shared.mustStream())
    {
        Siren::ArraySchema schema = s.array<T>( array.count() );
        
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
            SireBase::PackedArray2D<T> new_array
                                = SireBase::PackedArray2D<T>::create(schema.count());
            
            T *data = new_array.valueData();
            
            for (int i=0; i<schema.count(); ++i)
            {
                s >> data[i];
            }
            
            array = new_array[0];
        }
    }
    
    return s;
}

/** Streaming operator for PackedArray2D<T> */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Siren::Stream& operator&(Siren::Stream &s, SireBase::PackedArray2D<T> &array)
{
    s.assertVersion("SireBase::PackedArray2D", 1);
    
    Siren::SharedSchema shared = s.shared(array);
    
    if (shared.mustStream())
    {
        quint32 narrays = array.nArrays();

        QVarLengthArray<quint32> array_sizes( narrays );
        
        if (s.isSaving())
        {
            for (quint32 i=0; i<narrays; ++i)
                array_sizes[i] = array.nValues(i);
        }

        shared.data("array_sizes") & array_sizes;
        
        shared.data("array");
        Siren::ArraySchema arrayschema = s.array<T>(array.nValues());

        if (s.isSaving())
        {
            const T *data = array.constValueData();
            
            for (quint32 i=0; i<arrayschema.count(); ++i)
            {
                arrayschema.index(i) << data[i];
            }
        }
        else
        {
            SireBase::PackedArray2D<T> new_array 
                            = SireBase::PackedArray2D<T>::create(array_sizes);
            
            T *data = array.valueData();
            
            for (quint32 i=0; i<arrayschema.count(); ++i)
            {
                arrayschema.index(i) >> data[i];
            }
            
            array = new_array;
        }
    }
    
    return s;
}

/** Streaming operator for ChunkedHash<Key,Value> */
template<class Key, class Value, int N>
SIRE_OUTOFLINE_TEMPLATE
Siren::Stream& operator&(Siren::Stream &s, SireBase::ChunkedHash<Key,Value,N> &hash)
{
    s.assertVersion("SireBase::ChunkedHash", 1);

    Siren::SharedSchema shared = s.shared(hash);

    if (shared.mustStream())
    {
        Siren::MapSchema schema = s.map<Key,Value>(hash.count());
        
        if (s.isSaving())
        {
            for (typename SireBase::ChunkedHash<Key,Value>::const_iterator 
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

SIRE_END_HEADER

#endif
