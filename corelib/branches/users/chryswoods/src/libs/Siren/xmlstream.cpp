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

#include <QTextStream>
#include <QTextCodec>

#include "xmlstream.h"
#include "logger.h"
#include "tester.h"

#include "Siren/errors.h"

using namespace Siren;

static const RegisterMetaType<XMLStream> r_xmlstream;

/////////
///////// Implementation of XMLStream::Document
/////////

XMLStream::Document::Document() : device(0), string(0)
{}

XMLStream::Document::~Document()
{
    if (device or string)
    {
        QString doc = dom.toString(2);
        
        if (device)
        {
            QTextStream ts(device);
            ts.setCodec( QTextCodec::codecForName("UTF-8") );
            
            ts << doc;
        }
        else
        {
            *string = doc;
        }
    }
}

/** Push the current and last tag information onto a stack. This
    is necessary when we jump to a different part of the document,
    e.g. when we switch to writing a target, or a string */
void XMLDocument::Document::pushStack()
{
    current_element_stack.push( current_element );
    last_element_stack.push( last_element );
}

/** Pop an item off of the stack - this lets us return to our
    last position after we have finished writing a target, string etc. */
void XMLDocument::Document::popStack()
{
    if (current_element_stack.isEmpty())
        throw Siren::program_bug( QObject::tr(
                "We are trying to pop the document stack when it is empty!"),
                    CODELOC );
                    
    current_element = current_element_stack.pop();
    last_element = last_element_stack.pop();
}

/** Start writing a new target */
void XMLDocument::Document::startTarget(int id, bool is_saving)
{
    //save the current position
    this->pushStack();
    
    //now create a new target as a child of <targets> (create if necessary)
    
    //set this as the current and last element
}

/** Finish writing a target */
void XMLDocument::Document::endTarget(int id)
{
    //the current element must be the target with ID 'id'
    
    //go back to the previous position in the document
    this->popStack();
}

/** Create a new child as a child of the current node, using the tag
    'tag'. This will automatically add the "name" or "index" 
    attributes if these have been set. This does not change
    the current element. 
    
    This will save this element as 'last_element'
*/
QDomElement XMLDocument::Document::createChild(const QString &tag)
{
    QDomElement new_element = dom.createElement(tag);
    
    if (not next_name.isEmpty())
    {
        new_element.setAttribute("name", next_name);
        next_name = QString::null;
    }
    
    if (next_index >= 0)
    {
        new_element.setAttribute("index", next_index);
        next_index = -1;
    }
    
    current_element.addChild(new_element);
    last_element = new_element;
    
    return new_element;
}

/** Get the next child of the current element with the tag 'tag'. 
    If the "name" or "index" attributes have been set then this
    will automatically find the child that has the correct values
    of these attributes. This raises an exception if there is
    no matching child. This does not change the current element.

    This will save this element as 'last_element'
    
    \throw Siren::corrupted_data
*/
QDomElement XMLDocument::Document::getChild(const QString &tag_name, bool from_last)
{
    QDomElement found_element;

    if (from_last and (last_element.parentNode() == current_element))
    {
        //search siblings from here - this should be faster than searching
        //from the first element each time if we are looking for elements
        //in a large container
        found_element = 
        
        if (found_element.isNull())
            //start again from scratch
            return this->getChild(tag_name, false);
    }
    else
    {
        found_element = current_element.firstChild(tag_name);
        
        while (not found_element.isNull())
        {
            bool ok = true;
        
            if (not next_name.isEmpty())
            {
                //check the name
                ok = false;
            }
            
            if (next_index >= 0)
            {
                //check the index
                ok = false;
            }
            
            if (ok)
                //we've found the child
                break;
            
            //advance to the next child
            found_element = found_element.nextSiblingElement(tag_name);
        }
        
        if (found_element.isNull())
        {
            QStringList attributes;
            
            if (not next_name.isEmpty())
                attributes.append( QObject::tr("name=\"%1\"").arg(next_name) );
                
            if (next_index >= 0)
                attributes.append( QObject::tr("index=\"%1\"").arg(next_index) );
            
            if (attributes.isEmpty())
                this->corruptedData( QObject::tr(
                    "Cannot find the required child node <%1>.").arg(tag_name) );
            else
                this->corruptedData( QObject::tr(
                    "Cannot find the required child node <%1 %2>.")
                            .arg(tag_name, attributes.join(" ")) );
        }
    }
    
    last_element = found_element;
    
    return found_element;
}

/////////
///////// Implementation of XMLStream
/////////

/** Null constructor */
XMLStream::XMLStream() : ImplementsHandle<XMLStream,Stream>()
{}

/** Construct the stream for the passed QIODevice */
XMLStream::XMLStream(QIODevice *d) : ImplementsHandle<XMLStream,Stream>(true)
{
    if (d == 0)
        XMLStream::operator=( XMLStream() );
        
    xml.reset( new Document() );

    if (d->isReadable())
    {
        Stream::saveLoadingMode();
        
        QString err;
        int errline, errcol;
        
        if (not xml->dom.setContent(d, true, &err, &errline, &errcol))
            throwLoadingError(err, errline, errcol);
    }
    else
    {
        Stream::saveSavingMode();
        xml->device = d;
    }
}

/** Construct the stream for the passed QString */
XMLStream::XMLStream(QString *s, QIODevice::OpenMode mode)
          : ImplementsHandle<XMLStream,Stream>(true)
{
    if (s == 0)
        XMLStream::operator=( XMLStream() );
        
    xml.reset( new Document() );
    
    if (mode & QIODevice::ReadOnly)
    {
        Stream::saveLoadingMode();
        
        QString err;
        int errline, errcol;
        
        if (not xml->dom.setContent(*s, true, &err, &errline, &errcol))
            throwLoadingError(err, errline, errcol);
    }
    else
    {
        Stream::saveSavingMode();
        xml->string = s;
    }
}

/** Construct to read from the passed string */
XMLStream::XMLStream(const QString &s) : ImplementsHandle<XMLStream,Stream>(true)
{
    xml.reset( new Document() );
    
    Stream::saveLoadingMode();
    
    QString err;
    int errline, errcol;
    
    if (not xml->dom.setContent(s, true, &err, &errline, &errcol))
        throwLoadingError(err, errline, errcol);
}

/** Copy constructor */
XMLStream::XMLStream(const XMLStream &other) 
          : ImplementsHandle<XMLStream,Stream>(other), xml(other.xml)
{}
                               
/** Destructor */
XMLStream::~XMLStream()
{}

/** Copy assignment operator */
XMLStream& XMLStream::operator=(const XMLStream &other)
{
    xml = other.xml;
    Stream::operator=(other);
    
    return *this;
}

/** Comparison operator */
bool XMLStream::operator==(const XMLStream &other) const
{
    return xml.get() == other.xml.get();
}

/** Comparison operator */
bool XMLStream::operator!=(const XMLStream &other) const
{
    return not XMLStream::operator==(other);
}

uint XMLStream::hashCode() const
{
    return qHash(XMLStream::typeName());
}

QString XMLStream::toString() const
{
    if (isNull())
        return QObject::tr("XMLStream::null");
    else if (isSaving())
        return QObject::tr("XMLStream(=saving=)");
    else
        return QObject::tr("XMLStream(=loading=)");
}

bool XMLStream::test(Logger &logger) const
{
    return false;
}

/** Stream a logical value. These are represented using;

    <flag value="true"/> or <flag value="false"/>
*/
XMLStream& XMLStream::operator&(bool &b)
{
    if (not isNull())
    {
        if (isSaving())
        {
            QDomElement flag = xml->createChild("flag");
            
            if (b)
                flag.setAttribute("value", QLatin1String("true"));
            else
                flag.setAttribute("value", QLatin1String("false"));
        }
        else
        {
            QDomElement flag = xml->getChild("flag");
            
            QString val = flag.attribute("value");
            
            xml->assertAttributeExists(val, "flag", "value");
            
            if (val == QLatin1String("true"))
            {
                b = true;
            }
            else if (val == QLatin1String("false"))
            {
                b = false;
            }
            else
                xml->corruptedData( QObject::tr(
                    "The \"value\" attribute for a <flag> should be "
                    "\"true\" or \"false\".") );
        }
    }
    
    return *this;
}

template<class T>
static T getSignedInteger(const QString &string, bool *ok)
{
    qint64 val = string.toLongLong(ok, 0);
    
    if (not *ok)
        return 0;
    
    if (val > std::numeric_limits<T>::max() or
        val < std::numeric_limits<T>::min())
    {
        *ok = false;
        return 0;
    }
    else
    {
        return val;
    }
}

template<class T>
static T getUnsignedInteger(const QString &string, bool *ok)
{
    quint64 val = string.toULongLong(ok, 0);
    
    if (not *ok)
        return 0;
        
    if (val > std::numeric_limits<T>::max())
    {
        *ok = false;
        return 0;
    }
    else
    {
        return val;
    }
}

template<class T>
static T getInteger(const QString &string, bool *ok)
{
    return getSignedInteger<T>(string, ok);
}

template<>
static quint8 getInteger<quint8>(const QString &string, bool *ok)
{
    return getUnsignedInteger<quint8>(string, ok);
}

template<>
static quint16 getInteger<quint16>(const QString &string, bool *ok)
{
    return getUnsignedInteger<quint16>(string, ok);
}

template<>
static quint32 getInteger<quint32>(const QString &string, bool *ok)
{
    return getUnsignedInteger<quint32>(string, ok);
}

template<>
static quint64 getInteger<quint64>(const QString &string, bool *ok)
{
    return getUnsignedInteger<quint64>(string, ok);
}

template<class T>
void XMLStream::stream_integer(const QString &type, T &value)
{
    if (not isNull())
    {
        if (isSaving())
        {
            QDomElement number = xml->createChild("number");
            
            number.setAttribute("type", type);
            number.setAttribute("value", value);
        }
        else
        {
            QDomElement number = xml->getChild("number");

            QString read_type = number.attribute("type");
            QString read_value = number.attribute("value");
            
            xml->assertAttributeExists(read_type, "number", "type");
            xml->assertAttributeExists(read_value, "number", "value");
            
            if (read_type != type)
                xml->corruptedData( QObject::tr(
                        "The \"type\" attribute of this number was expected to be "
                        "\"%1\", but instead it is \"%2\".")
                            .arg(type, read_type) );
        
            bool ok;
            
            value = ::getInteger<T>(read_value, &ok);
            
            if (not ok)
                xml->corruptedData( QObject::tr(
                        "The \"value\" attribute of this number was expected to be "
                        "an integer between %1 and %2, but instead it was %3.")
                            .arg(std::numeric_limits<T>::min())
                            .arg(std::numeric_limits<T>::max())
                            .arg(read_value) );
        }
    }
}

XMLStream& XMLStream::operator&(qint8 &i)
{
    this->stream_integer<qint8>( "int8", i );
    return *this;
}

XMLStream& XMLStream::operator&(quint8 &i)
{
    this->stream_integer<quint8>( "uint8", i );
    return *this;
}

XMLStream& XMLStream::operator&(quint16 &i)
{
    this->stream_integer<quint16>( "uint16", i );
    return *this;
}

XMLStream& XMLStream::operator&(qint16 &i)
{
    this->stream_integer<qint16>( "int16", i );
    return *this;
}

XMLStream& XMLStream::operator&(qint32 &i)
{
    this->stream_integer<qint32>( "int32", i );
    return *this;
}

XMLStream& XMLStream::operator&(quint32 &i)
{
    this->stream_integer<quint32>( "uint32", i );
    return *this;
}

XMLStream& XMLStream::operator&(qint64 &i)
{
    this->stream_integer<qint64>( "int64", i );
    return *this;
}

XMLStream& XMLStream::operator&(quint64 &i)
{
    this->stream_integer<quint64>( "uint64", i );
    return *this;
}

template<class T>
T getFloat(const QString &value, bool *ok)
{
    double v = value.toDouble(ok);
    
    if (*ok)
        return v;
    else
        return 0;
}

template<>
float getFloat<float>(const QString &value, bool *ok)
{
    float v = value.toFloat(ok);
    
    if (*ok)
        return v;
    else
        return 0;
}

template<class T>
void XMLStream::stream_float( const QString &type, T &value )
{
    if (not isNull())
    {
        if (isSaving())
        {
            QDomElement number = xml->createChild("number");
            
            number.setAttribute("type", type);
            number.setAttribute("value", value);
        }
        else
        {
            QDomElement number = xml->getChild("number");

            QString read_type = number.attribute("type");
            QString read_value = number.attribute("value");
            
            xml->assertAttributeExists(read_type, "number", "type");
            xml->assertAttributeExists(read_value, "number", "value");
            
            if (read_type != type)
                xml->corruptedData( QObject::tr(
                        "The \"type\" attribute of this number was expected to be "
                        "\"%1\", but instead it is \"%2\".")
                            .arg(type, read_type) );
        
            bool ok;
            
            value = ::getFloat<T>(read_value, &ok);
            
            if (not ok)
                xml->corruptedData( QObject::tr(
                        "The \"value\" attribute of this number was expected to be "
                        "a number between -%1 and %1, but instead it was %2.")
                            .arg(std::numeric_limits<T>::max())
                            .arg(read_value) );
        }
    }
}

XMLStream& XMLStream::operator&(float &f)
{
    this->stream_float<float>("float", f);
    return *this;
}

XMLStream& XMLStream::operator&(double &f)
{
    this->stream_float<double>("double", f);
    return *this;
}
