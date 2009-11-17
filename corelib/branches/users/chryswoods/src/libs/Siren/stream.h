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

#ifndef SIREN_STREAM_H
#define SIREN_STREAM_H

#include <boost/noncopyable.hpp>

#include "sirenglobal.h"

SIREN_BEGIN_HEADER

namespace Siren
{

class Object;
class Stream;

/** This schema class is used to help with the streaming
    of objects - it provides object version checks, 
    allows naming of types and also helps provide
    any necessary closure (as the schema notifies the
    stream when it starts, and when it goes out of scope)
    
    @author Christopher Woods
*/
class SIREN_EXPORT Schema
{
public:
    Schema();
    Schema(const Schema &other);

    ~Schema();

    Schema& operator=(const Schema &other);

    Stream& data(const char *data_name);
    Stream& base();
    
    const QString& name() const;
    int version() const;
    
    void assertVersion(int version) const;

    void end();

private:
    friend class Stream;
    Schema(Stream *stream, const QString &class_name, int version);

    Stream *stream;
    const QString &class_name;
    bool needs_membernames;
};

/** This is the base class of all stream objects. A virtual 
    base class is used to provide a common interface that
    the other derived classes can use for streaming
    (so that we don't end up with a profusion of different
    streaming functions!)
    
    @author Christopher Woods
*/
class SIREN_EXPORT Stream : boost::noncopyable
{
public:
    Stream();
    
    virtual ~Stream();
    
    virtual Stream& operator&(qint8 &i)=0;
    virtual Stream& operator&(bool &i)=0; 
    virtual Stream& operator&(quint8 &i)=0;
    virtual Stream& operator&(quint16 &i)=0;
    virtual Stream& operator&(qint16 &i)=0;
    virtual Stream& operator&(qint32 &i)=0;
    virtual Stream& operator&(quint64 &i)=0;
    virtual Stream& operator&(qint64 &i)=0;
    virtual Stream& operator&(quint32 &i)=0;

    virtual Stream& operator&(float &f)=0;
    virtual Stream& operator&(double &f)=0;

    virtual Stream& operator&(char *&s)=0;

    void assertIsSaving() const;
    void assertIsLoading() const;

    template<class T>
    Schema schema();

protected:
    void setSavingMode();
    void setLoadingMode();

private:
    friend class Schema;
    void incref(const QString &type_name);
    void decref(const QString &type_name);

    QVarLengthArray<int,10> refcounts;

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
    a member variabe, with name 'name' */
inline Stream& Schema::data(const char *name)
{
    if (needs_membernames)
        stream->data(name);
        
    return *stream;
}

/** Tell the stream that the next object will be
    a base class of this object */
inline Stream& Schema::base()
{
    if (needs_membernames)
        stream->base();
        
    return *stream;
}

//////////
////////// Implementation of Stream
//////////

/** Return the schema for the type 'T', saying either that
    we will write version 'version', or that we will support
    the version up to a maximum of 'version'
    
    \throw Siren::corrupted_data
    \throw Siren::version_error
*/
template<class T>
SIREN_OUTOFLINE_TEMPLATE
Schema Stream::schema(int version)
{
    QString type_name = T::typeName();

    int got_version = this->getSchema( type_name, version );
    
    this->start(type_name);
    
    return Schema(this, type_name, got_version);
}

#endif // SIREN_SKIP_INLINE_FUNCTIONS

}

SIREN_END_HEADER

#endif
