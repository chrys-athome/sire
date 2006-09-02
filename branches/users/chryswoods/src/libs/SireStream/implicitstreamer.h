#ifndef SIRESTREAM_IMPLICITSTREAMER_H
#define SIRESTREAM_IMPLICITSTREAMER_H
/**
  * @file
  *
  * C++ Interface: ImplicitStreamer
  *
  * Description: 
  * 
  * 
  * Author: Christopher Woods, (C) 2006
  *
  * Copyright: See COPYING file that comes with this distribution
  *
  */

#include <boost/shared_ptr.hpp>

#include <QSharedDataPointer>

#include "implicitstore.h"

SIRE_BEGIN_HEADER

namespace SireStream 
{

/**
This class is used to improve the streaming of implicit shared objects. This class allows only a single copy of the implicitly shared object to be written to the stream, with other copies being recorded as references. This also preserves the shared nature of the data when the stream is read, as this allows the object to be shared when the data is read back out from the stream.

@author Christopher Woods
*/
class SIRESTREAM_EXPORT ImplicitStreamer
{
public:
    ImplicitStreamer();

    ~ImplicitStreamer();

    template<class T>
    void saveQSharedData( const QSharedDataPointer<T> &sharedptr );

private:
    //functions used to save to the stream
    bool haveStreamed( void *objptr ) const;
    void streamReference( void *objptr );
    void streamHeader( void *objptr );

    //functions used to load from the stream
    void* readStream();
    void saveObject( void *

    //accessing the datastream itself
    QDataStream& datastream();

    /** Shared pointer to the worker class */
    boost::shared_ptr<ImplicitStreamerPvt> d;
};

template<class T>
void ImplicitStreamer::saveQSharedData(const QSharedDataPointer<T> &sharedptr)
{
    //has the object pointed to by sharedptr been saved to the stream already?
    if ( this->haveStreamed(sharedptr.constData()) )
        //stream a reference to this item
        this->streamReference(sharedptr.constData());
    else
    {
        //stream a header for this object
        this->streamHeader(sharedptr.constData());
        
        //now stream the object itself
        datastream() << *sharedptr;
    }
}

template<class T>
void ImplicitStreamer::loadQSharedData(QSharedDataPointer<T> &sharedptr)
{
    //load the header for this object
    StreamedObject streamobj = this->readStream();
    
    if (obj.alreadyLoaded())
        //the object has already been loaded - take an implicit copy
        sharedptr = obj.asA< QSharedDataPointer<T> >();
    else
    {
        //we need to load the object ourselves
        datastream() >> *sharedptr;
        
        //save an implicit copy of this object
        this->saveObject( new QSharedDataPointer<T>( sharedptr ) );
    }
}

}

SIRE_END_HEADER

#endif
