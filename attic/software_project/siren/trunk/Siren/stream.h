#ifndef SIREN_STREAM_H
#define SIREN_STREAM_H
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

#include "Siren/siren.h"
//#include "schemas.h"

#include "Siren/detail/boostsupport.h"

SIREN_BEGIN_HEADER

namespace Siren
{
    /** This is the base class of all stream objects. A virtual 
        base class is used to provide a common interface that
        the other derived classes can use for streaming
        (so that we don't end up with a profusion of different
        streaming functions!)
        
        @author Christopher Woods
    */
    class SIREN_EXPORT Stream : public noncopyable
    {
    public:
        Stream();
        virtual ~Stream();
        
        Stream& operator&(Object &object);
        Stream& operator&(Obj &object);
        
        Obj loadNextObject();

        bool isSaving() const;
        bool isLoading() const;

        void assertIsSaving() const;
        void assertIsLoading() const;

        int checkVersion(const String &type_name, int version);
        void assertVersion(const String &type_name, int version);

        template<class T>
        int checkVersion(int version);
        
        template<class T>
        void assertVersion(int version);

        Schema getSchema(const Class &type, int version=1);

        template<class T>
        Schema getSchema(int version=1);

        Stream& operator<<(const Object &data);
        Stream& operator>>(Object &data);

    protected:
        friend class Number;
        friend class String;
        friend class ByteArray;

        virtual void writeNumber(const Number &number)=0;
        virtual void writeString(const String &string)=0;
        virtual void writeBinary(const ByteArray &bytearray)=0;
        
        virtual void readNumber(Number &number)=0;
        virtual void readString(String &string)=0;
        virtual void readBinary(ByteArray &bytearray)=0;

        void saveNeedsDecoration(bool needs_decoration);
        
        void saveSavingMode();
        void saveLoadingMode();

        /** Called to signify the start of a new object of type 'type_name'.
            If this is a decorated stream, then it is up to you to verify that
            the type in the stream is the same as the type.
        */
        virtual void startObject(const String &type_name)=0;
        
        /** Called to signify the next base class item */
        virtual void nextBase()=0;

        /** Called to signify the end of an object of type 'type_name' */
        virtual void endObject(const String &type_name)=0;

        /** Read a reference number and return it */
        virtual int readReference()=0;

        /** Write the reference number 'id' */
        virtual void writeReference(int id)=0;
        
        /** Called to signify the start of the reference target with ID 'id' */
        virtual void startTarget(int id)=0;

        /** Called to signify the end of the reference target with ID 'id' */
        virtual void endTarget(int id)=0;
        
        /** Read the string with reference ID 'id' and return it */
        virtual QString readString(int id)=0;
        /** Write the string with reference 'ID' and value 'text' */
        virtual void writeString(int id, const QString &text)=0;

        /** Read a binary large object (Blob) reference and return it */
        virtual int readBlobReference()=0;
        /** Write the Blob reference ID 'id' */
        virtual void createBlobReference(int id)=0;

        /** Read and return the Blob with reference ID 'id' */
        virtual QByteArray readBlob(int id)=0;
        /** Write the Blob 'blob' with reference ID 'id' */
        virtual void writeBlob(int id, const QByteArray &blob)=0;

        /** Read the class ID and data format version for the class with
            name 'class_name' - the class ID is returned and 'version' updated */
        virtual int readClassID(const QString &class_name, int &version)=0;
        
        /** Write the class ID 'id' and data format version 'version' for the class 
            with type name 'class_name' */
        virtual void writeClassID(const QString &class_name, int id, int version)=0;
        
        /** Peek into the stream and return the class name of the type of
            the next object. This does not advance the stream */
        virtual QString peekNextType()=0;

        /** For non-decorated streams (which rely on magic numbers), read and
            return the next magic number. This number will always be 
            less than 2^30, so will fit into a 32 bit signed integer. */
        virtual int readMagic()=0;
        
        /** For non-decorated streams, write the magic number 'magic'.
            This number will always be less than 2^30, so will fit into a 
            32 bit signed integer. */
        virtual void writeMagic(int magic)=0;

        QString getClassType(int magic) const;

    private:
        friend class Schema;
        friend class ArraySchema;
        friend class SetSchema;
        friend class MapSchema;
        friend class SharedSchema;

        bool haveStreamed(const void *key) const;
        bool haveStreamed(int id) const;
        
        int getID(const void *key);
        
        const detail::SharedHelperBase& getReference(int id) const;
        
        void startTarget(int id, detail::SharedHelperBase *helper);
        void finaliseObject(int id);

        void throwNotSavingError() const;
        void throwNotLoadingError() const;

        void throwUnsupportedError(const QString &type_name, int got_version,
                                   const QString &supported_versions) const;

        /** Whether or not this stream requires decorators */
        bool needs_decoration;

        /** Whether or not this stream is saving or loading */
        bool is_saving;
    };

    #ifndef SIREN_SKIP_INLINE_FUNCTIONS

    //////////
    ////////// Implementation of Schema - inlining should
    ////////// hopefully speed up calls when method naming 
    ////////// is not used
    ////////// 

    /** Tell the stream that the next object will be 
        a member variable, with name 'name' */
    inline Stream& Schema::data(const char *name)
    {
        if (needs_decoration)
            stream->nextData(name);
            
        return *stream;
    }

    /** Tell the stream that the next object will be
        a base class of this object */
    inline Stream& Schema::base()
    {
        if (needs_decoration)
            stream->nextBase();
            
        return *stream;
    }

    //////////
    ////////// Implementation of SharedSchema
    //////////

    /** Return whether or not the shared object must now
        be streamed - if this is false then the object
        has been streamed already. */
    inline bool SharedSchema::mustStream() const
    {
        return must_stream;
    }

    /** Tell the stream that the next object will be 
        a member variable, with name 'name' */
    inline Stream& SharedSchema::data(const char *name)
    {
        if (needs_decoration)
            stream->nextData(name);
            
        return *stream;
    }

    /** Tell the stream that the next object will be
        a base class of this object */
    inline Stream& SharedSchema::base()
    {
        if (needs_decoration)
            stream->nextBase();
            
        return *stream;
    }

    //////////
    ////////// Implementation of ContainerSchema - inlining should
    ////////// hopefully speed up calls when method naming 
    ////////// is not used
    ////////// 

    /** Return the number of items to stream for this container */
    inline int ContainerSchema::count() const
    {
        return n_to_stream;
    }

    /** Tell that container that the next item in the container
        is about to be streamed */
    inline void ContainerSchema::nextItem()
    {
        if (n_streamed == n_to_stream)
            ContainerSchema::throwFullContainerError();

        n_streamed += 1;
    }

    //////////
    ////////// Implementation of ArraySchema - inlining should
    ////////// hopefully speed up calls when method naming 
    ////////// is not used
    ////////// 

    /** Tell the stream that the next object will be
        the array item at index 'i' */
    inline Stream& ArraySchema::index(int i)
    {
        ContainerSchema::nextItem();

        if (needs_decoration)
            stream->nextIndex(i);
            
        return *stream;
    }

    //////////
    ////////// Implementation of SetSchema - inlining should
    ////////// hopefully speed up calls when method naming 
    ////////// is not used
    ////////// 

    /** Tell the stream that the next object will
        be an entry into a set */
    inline Stream& SetSchema::entry()
    {
        ContainerSchema::nextItem();

        if (needs_decoration)
            stream->nextEntry();
            
        return *stream;
    }

    //////////
    ////////// Implementation of MapSchema - inlining should
    ////////// hopefully speed up calls when method naming 
    ////////// is not used
    ////////// 

    /** Tell the stream that the next object will
        be an entry into a map at the key 'key' */
    template<class Key>
    SIREN_INLINE_TEMPLATE
    Stream& MapSchema::entry(Key &key)
    {
        ContainerSchema::nextItem();

        if (needs_decoration)
            stream->nextKey();
            
        (*stream) & key;
        
        if (needs_decoration)
            stream->nextValue();
            
        return *stream;
    }

    /** Tell the stream that the next object will
        be an entry into a map at the key 'key' */
    template<class Key>
    SIREN_INLINE_TEMPLATE
    Stream& MapSchema::entry(const Key &key)
    {
        ContainerSchema::nextItem();

        if (needs_decoration)
            stream->nextKey();
            
        (*stream) << key;
        
        if (needs_decoration)
            stream->nextValue();

        return *stream;
    }

    //////////
    ////////// Implementation of Stream
    //////////

    /** Return whether or not this stream is being used to save data */
    inline bool Stream::isSaving() const
    {
        return is_saving;
    }

    /** Return whether or not this stream is being used to load data */
    inline bool Stream::isLoading() const
    {
        return not is_saving;
    }

    /** Assert that this stream is in saving mode

        \throw Siren::invalid_state
    */
    inline void Stream::assertIsSaving() const
    {
        if (not is_saving)
            Stream::throwNotSavingError();
    }

    /** Assert that this stream is in loading mode

        \throw Siren::invalid_state
    */
    inline void Stream::assertIsLoading() const
    {
        if (is_saving)
            Stream::throwNotLoadingError();
    }

    /** Check the version of the object of type 'T', suggesting the 
        desired writing version, and returning the actual version */
    template<class T>
    int Stream::checkVersion(int version)
    {
        return this->checkVersion( detail::StreamHelper<T>::typeName(), version );
    }

    /** Assert that the version of the object of type 'T' being
        streamed is 'version' 
        
        \throw Siren::version_error
    */
    template<class T>
    void Stream::assertVersion(int version)
    {
        this->assertVersion( detail::StreamHelper<T>::typeName(), version );
    }

    /** Return whether or not this item has been shared streamed before */
    template<class T>
    SIREN_OUTOFLINE_TEMPLATE
    SharedSchema Stream::shared(const T &value)
    {
        this->assertIsSaving();
        
        const void* key = detail::StreamHelper<T>::getKey(value);

        if (key == 0)
        {
            this->createReference(0);
            return SharedSchema(this, 0, false, needs_decoration);
        }
        else if (this->haveStreamed(key))
        {
            int id = this->getID(key);
            this->createReference(id);
            return SharedSchema(this, id, false, needs_decoration);
        }
        else
        {
            int new_id = this->getID(key);
            this->createReference( new_id );
            
            this->startTarget(new_id);
            
            return SharedSchema(this, new_id, true, needs_decoration);
        }
    }

    /** Return whether or not this item has been shared streamed before,
        and it is has, and we are loading, then update the passed reference 
        so that it is equal to the shared object */
    template<class T>
    SIREN_OUTOFLINE_TEMPLATE
    SharedSchema Stream::shared(T &value)
    {
        if (this->isSaving())
        {
            const T &const_value = value;
            return this->shared<T>(const_value);
        }
        else
        {
            int id = this->readReference();
            
            if (id == 0)
            {
                //this is a null object
                value = detail::StreamHelper<T>::null();
                return SharedSchema(this, id, false, needs_decoration);
            }
            else if (this->haveStreamed(id))
            {
                value = this->getReference(id).asA<T>();
                return SharedSchema(this, id, false, needs_decoration);
            }
            else
            {
                this->startTarget(id, new detail::SharedHelper<T>(&value));
                return SharedSchema(this, id, true, needs_decoration);
            }
        }
    }

    /** Return the schema for a Siren::Object of type 'T', saying either that
        we will write version 'version', or that we will support
        the version up to a maximum of 'version'
        
        \throw Siren::corrupted_data
        \throw Siren::version_error
    */
    template<class T>
    SIREN_OUTOFLINE_TEMPLATE
    Schema Stream::item(int version)
    {
        const QString type_name = detail::StreamHelper<T>::typeName();
        
        this->startItem(type_name);
        return Schema(this, type_name, this->needs_decoration);
    }
        
    template<class T>
    SIREN_OUTOFLINE_TEMPLATE
    ArraySchema Stream::array(int count)
    {
        const QString type_name = detail::StreamHelper<T>::typeName();
        count = this->startArray( type_name, count );
        return ArraySchema(this, type_name, count, this->needs_decoration);
    }

    template<class T>
    SIREN_OUTOFLINE_TEMPLATE
    SetSchema Stream::set(int count)
    {
        const QString type_name = detail::StreamHelper<T>::typeName();
        count = this->startSet( type_name, count );
        return SetSchema(this, type_name, count, this->needs_decoration);
    }

    template<class Key, class Value>
    SIREN_OUTOFLINE_TEMPLATE
    MapSchema Stream::map(int count, bool allow_duplicates)
    {
        const QString key_type = detail::StreamHelper<Key>::typeName();
        const QString value_type = detail::StreamHelper<Value>::typeName();
        
        count = this->startMap( key_type, value_type, count, allow_duplicates );
        
        return MapSchema(this, key_type, value_type, count, 
                         allow_duplicates, this->needs_decoration);
    }

    /** Explicitly request saving of an object */
    template<class T>
    SIREN_INLINE_TEMPLATE
    Stream& Stream::operator<<(const T &data)
    {
        if (not this->is_saving)
            this->throwNotSavingError();

        return (*this) & ( const_cast<T&>(data) );
    }

    /** Explicitly request loading of an object */
    template<class T>
    SIREN_INLINE_TEMPLATE
    Stream& Stream::operator>>(T &data)
    {
        if (this->is_saving)
            this->throwNotLoadingError();

        return (*this) & data;
    }

    #endif // SIREN_SKIP_INLINE_FUNCTIONS

}

SIREN_EXPOSE_CLASS( Siren::Stream )

SIREN_END_HEADER

#endif
