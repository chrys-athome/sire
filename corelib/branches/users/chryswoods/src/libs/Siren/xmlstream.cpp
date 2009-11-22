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

#include "third_party/encode.h"   // CONDITIONAL_INCLUDE
#include "third_party/decode.h"   // CONDITIONAL_INCLUDE

#include "Siren/errors.h"

#include <QDebug>

using namespace Siren;

static const RegisterMetaType<XMLStream> r_xmlstream;

/////////
///////// Implementation of XMLStream::Document
/////////

XMLStream::Document::Document() 
          : next_name(QString::null), device(0), string(0), 
            sirenml_version(1),
            next_is_base(false), next_is_entry(false), 
            next_is_key(false), next_is_value(false)
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

/** Initialise the document, setting/reading the version number */
void XMLStream::Document::initialiseDocument(bool is_saving)
{
    if (is_saving)
    {
        document_root = dom.createElement("sirenml");
        document_root.setAttribute("version", sirenml_version);
        dom.appendChild(document_root);
    }
    else 
    {
        document_root = dom.firstChildElement("sirenml");
        
        if (document_root.isNull())
            throw Siren::corrupted_data( QObject::tr(
                    "Cannot read the XML document as it does not "
                    "contain a <sirenml> tag."), CODELOC );
                    
        sirenml_version = this->getIntAttribute(document_root, "version");
    }
}

/** Raise a corrupted_data error with an error described in the passed
    string, but providing more detail about where in the document the
    error occured */
void XMLStream::Document::corrupted_data(const QString &error_string,
                                         QDomElement element)
{
    throw Siren::corrupted_data( QObject::tr(
            "There was an error while streaming the XML document.\n%1\n"
            "The error occured while streaming tag <%2> at line %3, column %4 "
            "of the XML document.")
                .arg(error_string, element.tagName())
                .arg(element.lineNumber()).arg(element.columnNumber()), CODELOC );
}

/** Assert that the returned attribute 'value' is non-null - this
    attribute came from the tag 'tag', and is attribute 'attribute' */
void XMLStream::Document::assertAttributeExists(QDomElement &tag,
                                                const QString &value, 
                                                const QString &attribute)
{
    if (value.isEmpty())
    {
        this->corrupted_data( QObject::tr(
                "The tag <%1> is missing the required attribute %2.")
                    .arg(tag.tagName(), attribute), tag );
    }
}

/** Push the current and last tag information onto a stack. This
    is necessary when we jump to a different part of the document,
    e.g. when we switch to writing a target, or a string */
void XMLStream::Document::pushStack()
{
    current_element_stack.push( current_element );
}

/** Pop an item off of the stack - this lets us return to our
    last position after we have finished writing a target, string etc. */
void XMLStream::Document::popStack()
{
    if (current_element_stack.isEmpty())
        throw Siren::program_bug( QObject::tr(
                "We are trying to pop the document stack when it is empty!"),
                    CODELOC );
                    
    current_element = current_element_stack.pop();
}

QDomElement XMLStream::Document::findTarget(const QString &targets_tag,
                                            int id, 
                                            const QString &target_tag,
                                            const QString &id_tag)
{
    QDomElement targets = document_root.firstChildElement(targets_tag);
        
    if (targets.isNull())
        this->corrupted_data( QObject::tr(
                "Cannot find <%1 id=\"%2\"> as there is no <%3> "
                "section in this document!")
                    .arg(target_tag).arg(id)
                    .arg(targets_tag), document_root );
                    

    QDomElement target = targets.firstChildElement(target_tag);
    
    while (not target.isNull())
    {
        bool ok;
        int idval = target.attribute(id_tag).toInt(&ok);

        if (ok)
        {
            if (idval == id)
                return target;
        }
        
        target = target.nextSiblingElement(target_tag);
    }
    
    this->corrupted_data( QObject::tr(
            "Cannot find <%1 id=\"%2\"> within the <%3>...</%3> section "
            "of this document.").arg(target_tag).arg(id).arg(targets_tag), targets );
    
    return QDomElement();
}

/** Start writing a new target */
void XMLStream::Document::startTarget(int id, bool is_saving)
{
    if (id == 0)
        throw Siren::program_bug( QObject::tr(
            "Cannot write a target with ID == 0"), CODELOC );

    //save the current position
    this->pushStack();
    
    if (is_saving)
    {
        //create a new target as a child of <targets> (create <targets> if necessary)
        QDomElement targets = document_root.firstChildElement("targets");
        
        if (targets.isNull())
        {
            targets = dom.createElement("targets");
            document_root.appendChild(targets);
        }
        
        QDomElement target = dom.createElement("target");
        target.setAttribute("id", id);
        
        targets.appendChild(target);
        
        current_element = target;
    }
    else
    {
        QDomElement target = this->findTarget("targets", id, "target", "id");

        current_element = target;
    }
}

/** Finish writing a target */
void XMLStream::Document::endTarget(int id)
{
    if (id == 0)
        return;

    //the current element must be the target with ID 'id'
    
    //go back to the previous position in the document
    this->popStack();
}

/** Write the string data 'string' with ID 'id' */
void XMLStream::Document::writeString(int id, const QString &text)
{
    if (id == 0 or text.isEmpty())
        throw Siren::program_bug( QObject::tr(
            "Cannot write an empty string or a string with ID == 0"), CODELOC );

    QDomElement strings = document_root.firstChildElement("strings");
    
    if (strings.isNull())
    {
        strings = dom.createElement("strings");
        document_root.appendChild(strings);
    }
    
    QDomElement string = dom.createElement("strdata");
    string.setAttribute("id", id);
    
    //replace all '<' with '&lt;' and all '>' with &gt;
    QString escaped_text = text;
    escaped_text.replace("<", "&lt;").replace(">", "&gt;");
    
    QDomText t = dom.createTextNode(escaped_text);
    
    string.appendChild(t);
    
    strings.appendChild(string);
}

/** Read and return the string with ID 'id' */
QString XMLStream::Document::readString(int id)
{
    if (id == 0)
        return QString::null;

    QDomElement string = this->findTarget("strings", id, "strdata", "id");
    
    QString text = string.text();
    
    text.replace("&lt;", "<").replace("&gt;", ">");
    
    return text;
}

/** Write the blob 'blob' with ID 'id' */
void XMLStream::Document::writeBlob(int id, const QByteArray &blob)
{
    if (id == 0 or blob.isEmpty())
        throw Siren::program_bug( QObject::tr(
            "Cannot write an empty BLOB or a BLOB with ID == 0"), CODELOC );

    QDomElement blobs = document_root.firstChildElement("binaries");
    
    if (blobs.isNull())
    {
        blobs = dom.createElement("binaries");
        document_root.appendChild(blobs);
    }
    
    //we need to encode the binary data as a base64 string
    //(thanks to chris.venter@gmail.com for providing a 
    // fast public domain base64 encoder).
    
    // The encoded string only uses the characters [A-Za-z0-9+/=]
    // so can safely be placed into an XML document
    
    // base64 encoded data is 1/3 larger than raw blob
    QByteArray encoded_blob( 1 + blob.count() + (blob.count() / 3), '\0' );
        
    base64::encoder encoder;
    
    encoder.encode(blob.constData(), blob.count(), encoded_blob.data());
    
    QDomText text = dom.createTextNode( QLatin1String(encoded_blob.constData()) );
    
    QDomElement b = dom.createElement("bindata");
    b.setAttribute("id", id);
    b.setAttribute("size", blob.count());
    
    b.appendChild(text);
    blobs.appendChild(b);
}

int XMLStream::Document::getIntAttribute(QDomElement &element, const QString &attribute)
{
    QString a = element.attribute(attribute);
    
    bool ok;
    int value = a.toInt(&ok);
    
    if (not ok)
        this->corrupted_data( QObject::tr(
                "Cannot read an integer from the \"%1\" attribute of %2.")
                    .arg(attribute).arg(element.tagName()), element );

    return value;
}

/** Read and return the binary data with ID 'id' */
QByteArray XMLStream::Document::readBlob(int id)
{
    if (id == 0)
        return QByteArray();

    QDomElement binary = this->findTarget("binaries", id, "bindata", "id");
    
    //get the size of the decoded blob
    int decoded_size = this->getIntAttribute(binary, "size"); 
    
    QByteArray encoded_blob = binary.text().toLatin1();
    
    if (encoded_blob.isEmpty())
        this->corrupted_data( QObject::tr(
                "Could not read the encoded binary string for ID %1.").arg(id),
                              binary );

    //the decoded blob cannot be larger than the encoded blob
    if (decoded_size > encoded_blob.size())
        this->corrupted_data( QObject::tr(
                "The size of the decoded blob (%1) is larger than that of "
                "the encoded blob (%2). This implies that the blob has "
                "become corrupted.").arg(decoded_size).arg(encoded_blob.size()),
                binary );

    QByteArray blob;
    
    //ensure that there is enough space in case the buffer overruns
    blob.reserve(encoded_blob.size());
    
    //but we will only return as many bytes as the attribute claims...
    blob.resize(decoded_size);
    
    base64::decoder decoder;
    
    decoder.decode( encoded_blob.constData(), encoded_blob.size(), blob.data() );

    return blob;
}

void XMLStream::Document::clearNextState()
{
    next_name = QString::null;
    next_index = QString::null;
    next_is_base = false;
    next_is_entry = false;
    next_is_key = false;
    next_is_value = false;
}

/** Create a new child as a child of the current node, using the tag
    'tag'. This will automatically add the "name" or "index" 
    attributes if these have been set. This does not change
    the current element. 
*/
QDomElement XMLStream::Document::createChild(const QString &tag)
{
    if (current_element.isNull())
        current_element = document_root;

    QDomElement parent_element = current_element;

    if (next_is_base)
    {
        //this is create just as a <base> - it ignores the tag
        QDomElement base = dom.createElement("base");
        parent_element.appendChild(base);

        this->clearNextState();
        return base;
    }
    else if (next_is_entry)
    {
        QDomElement entry = dom.createElement("entry");
        current_element.appendChild(entry);
        
        parent_element = entry;

        this->clearNextState();
    }
    else if (next_is_key)
    {
        //create the entry tag for this key/value pair
        QDomElement entry = dom.createElement("entry");
        current_element.appendChild(entry);

        entries_awaiting_values.push(entry);
    
        QDomElement key = dom.createElement("key");
        entry.appendChild(key);
        
        parent_element = key;

        this->clearNextState();
    }
    else if (next_is_value)
    {
        if (entries_awaiting_values.isEmpty())
            throw Siren::program_bug( QObject::tr(
                "There are no waiting <entry> tags that are missing a map <value>?"),
                    CODELOC );

        QDomElement entry = entries_awaiting_values.pop();
                    
        QDomElement value = dom.createElement("value");
        entry.appendChild(value);
        
        parent_element = value;

        this->clearNextState();
    }

    QDomElement new_element = dom.createElement(tag);
    
    if (not next_name.isEmpty())
    {
        new_element.setAttribute("name", next_name);
        next_name = QString::null;
    }
    
    if (not next_index.isEmpty())
    {
        new_element.setAttribute("index", next_index);
        next_index = QString::null;
    }
    
    parent_element.appendChild(new_element);
    
    return new_element;
}

void XMLStream::Document::assertCurrentIsContainer(QString container)
{
    if (current_element.tagName() != container)
        this->corrupted_data( QObject::tr(
                "The current item, <%1>, is not a container of type <%2>.")
                    .arg(current_element.tagName(), container),
                        current_element );
}

void XMLStream::Document::missingChildError(const QString &tagname, QString err)
{
    if (not err.isEmpty())
        this->corrupted_data( QObject::tr(
                "Could not find the tag <%1> %2 as a child of the "
                "current tag <%3>.")
                            .arg(tagname, err, current_element.tagName()), 
                                current_element);
    else
        this->corrupted_data( QObject::tr(
                "Could not find the tag <%1> as a child of the "
                "current tag <%4>.")
                            .arg(tagname, current_element.tagName()), 
                                current_element);
}

/** Get the next child of the current element with the tag 'tag'. 
    If the "name" or "index" attributes have been set then this
    will automatically find the child that has the correct values
    of these attributes. This raises an exception if there is
    no matching child. This does not change the current element.
    
    \throw Siren::corrupted_data
*/
QDomElement XMLStream::Document::getChild(const QString &tag_name)
{
    if (current_element.isNull())
        current_element = document_root;

    if (not next_name.isEmpty())
    {
        //the next item is named (so cannot be in a container,
        //and cannot be a base class)
        QDomElement child = current_element.firstChildElement(tag_name);
        
        while (not child.isNull())
        {
            if (child.attribute("name") == next_name)
            {
                this->clearNextState();
                return child;
            }
            else
                child = child.nextSiblingElement(tag_name);
        }
        
        this->missingChildError(tag_name, QObject::tr(
                "with name=\"%1\"").arg(next_name) );
    }
    else if (next_is_base)
    {
        //the next item is a base, not an <object> (so cannot be a container and 
        //cannot be named)
        QDomElement base = current_element.firstChildElement("base");
        
        if (not base.isNull())
        {
            this->clearNextState();
            return base;
        }

        this->missingChildError("base", QObject::tr(
                "as base class of <%1>").arg(current_element.tagName()) );
    }
    else if (not next_index.isEmpty())
    {
        /// Find the next item in an <array>
        if (container_iterator.isNull())
        {
            //the current item must be the 'array' containing the entry
            this->assertCurrentIsContainer("array");
            container_iterator = current_element.firstChildElement(tag_name);
        }
        
        //do a first scan from the current iterator
        while (not container_iterator.isNull())
        {
            if (container_iterator.attribute("index") == next_index)
            {
                this->clearNextState();
                return container_iterator;
            }
            else
                container_iterator = container_iterator.nextSiblingElement(tag_name);
        }
        
        //we didn't find the element on the first scan - do a second scan
        container_iterator = current_element.firstChildElement(tag_name);

        while (not container_iterator.isNull())
        {
            if (container_iterator.attribute("index") == next_index)
            {
                this->clearNextState();
                return container_iterator;
            }
            else
                container_iterator = container_iterator.nextSiblingElement(tag_name);
        }

        this->missingChildError(tag_name, QObject::tr(
                    "with index=\"%1\" in <array>").arg(next_index) );
    }
    else if (next_is_entry)
    {
        if (container_iterator.isNull())
        {
            //the current item must be the 'array' containing the entry
            this->assertCurrentIsContainer("set");
            container_iterator = current_element.firstChildElement("entry");
        }
        else
            container_iterator = container_iterator.nextSiblingElement("entry");

        QDomElement child = container_iterator.firstChildElement(tag_name);
        
        if (not child.isNull())
        {
            this->clearNextState();
            return child;
        }

        this->missingChildError(tag_name, QObject::tr("as <entry> in <set>"));
    }
    else if (next_is_key)
    {
        if (container_iterator.isNull())
        {
            //the current item must be the 'array' containing the entry
            this->assertCurrentIsContainer("map");
            container_iterator = current_element.firstChildElement("entry");
        }
        else
            container_iterator = container_iterator.nextSiblingElement("entry");
            
        //we want the key
        QDomElement key = container_iterator.firstChildElement("key");
        QDomElement child = key.firstChildElement(tag_name);
        
        if (not child.isNull())
        {
            this->clearNextState();
            return child;
        }

        this->missingChildError(tag_name, QObject::tr("as <key> in <map>"));
    }
    else if (next_is_value)
    {
        if (container_iterator.isNull())
        {
            throw Siren::program_bug( QObject::tr(
                    "Streaming of a <value> must follow the streaming of a "
                    "<key>, so the container iterator must be valid...?"), 
                        CODELOC );
        }
        
        //we want the value
        QDomElement value = container_iterator.firstChildElement("value");
        QDomElement child = value.firstChildElement(tag_name);
        
        if (not child.isNull())
        {
            this->clearNextState();
            return child;
        }
    
        this->missingChildError(tag_name, QObject::tr("as <value> in <map>"));
    }
    else 
    {
        //nothing specified, so just get the next object
        QDomElement child = current_element.firstChildElement(tag_name);
        
        if (not child.isNull())
        {
            this->clearNextState();
            return child;
        }
        
        this->missingChildError(tag_name);
    }
     
    //we couldn't find the right child
    if (next_name.isEmpty())
        this->corrupted_data( QObject::tr(
                "Cannot find the required child node <%1>.").arg(tag_name),
                current_element );
    else
        this->corrupted_data( QObject::tr(
            "Cannot find the required child node <%1 name=\"%1\">.")
                    .arg(tag_name, next_name), current_element );
    
    return QDomElement();
}

/** Start working in the child element 'child' - this moves
    focus to this child - all further children will be written
    as children of this child */
void XMLStream::Document::startChild(QDomElement &child)
{
    if (child.ownerDocument() != dom)
        throw Siren::program_bug( QObject::tr(
                "You cannot start a child with a different DOM document!!!"), CODELOC );

    current_element = child;
}

/** Finish working with the child - the focus will move
    back to the parent of this child and all further children
    will be siblings of the current child */
void XMLStream::Document::endChild()
{
    current_element = current_element.parentNode().toElement();
    
    while (current_element.tagName() == "key" or
           current_element.tagName() == "value" or
           current_element.tagName() == "entry")
    {
        current_element = current_element.parentNode().toElement();
    }
    
    if (current_element.isNull())
        throw Siren::program_bug( QObject::tr(
            "Somehow the parent of the child is null!"), CODELOC );
}

/** Start working in the child container 'child' - this moves
    focus to this child - all further children will be written
    as children of this child */
void XMLStream::Document::startContainer(QDomElement &child)
{
    if (child.ownerDocument() != dom)
        throw Siren::program_bug( QObject::tr(
                "You cannot start a child with a different DOM document!!!"), CODELOC );

    current_element = child;
    container_iterator_stack.push(container_iterator);
    container_iterator = QDomElement();
}

/** Finish working with the child - the focus will move
    back to the parent of this child and all further children
    will be siblings of the current child */
void XMLStream::Document::endContainer()
{
    current_element = current_element.parentNode().toElement();
    
    while (current_element.tagName() == "key" or
           current_element.tagName() == "value" or
           current_element.tagName() == "entry")
    {
        current_element = current_element.parentNode().toElement();
    }
    
    if (current_element.isNull())
        throw Siren::program_bug( QObject::tr(
            "Somehow the parent of the child is null!"), CODELOC );

    if (container_iterator_stack.isEmpty())
        throw Siren::program_bug( QObject::tr(
            "Somehow the container iterator stack is empty!"), CODELOC );
    
    container_iterator = container_iterator_stack.pop();
}

void XMLStream::Document::createReference(QString tag_name, int id)
{
    QDomElement reference = this->createChild(tag_name);
    reference.setAttribute("id", id);
}

int XMLStream::Document::readReference(QString tag_name)
{
    QDomElement reference = this->getChild(tag_name);
    return this->getIntAttribute(reference, "id");
}

int XMLStream::Document::readClassID(const QString &class_name, int &version)
{
    QDomElement classes = document_root.firstChildElement("classes");
    
    if (classes.isNull())
        this->corrupted_data( QObject::tr(
                "There is no <classes> section in the document, which "
                "is required to supply the IDs and versions of classes."),
                    document_root );

    QDomElement clas = classes.firstChildElement(class_name);
    
    while (not clas.isNull())
    {
        if (clas.attribute("type") == class_name)
        {
            int id = this->getIntAttribute(clas, "id");
            version = this->getIntAttribute(clas, "version");
            
            return id;
        }
        
        clas = clas.nextSiblingElement("class");
    }
    
    this->corrupted_data( QObject::tr(
            "There is no <class> with type=\"%1\" listed in the "
            "classes section.").arg(class_name), classes );
            
    return 0;
}

void XMLStream::Document::writeClassID(const QString &class_name, int id, int version)
{
    QDomElement classes = document_root.firstChildElement("classes");
    
    if (classes.isNull())
    {
        classes = dom.createElement("classes");
        document_root.appendChild(classes);
    }
    
    QDomElement clas = dom.createElement("class");
    
    clas.setAttribute("type", class_name);
    clas.setAttribute("id", id);
    clas.setAttribute("version", version);
    
    classes.appendChild(clas);
}

/////////
///////// Implementation of XMLStream
/////////

/** Null constructor */
XMLStream::XMLStream() : ImplementsHandle<XMLStream,Stream>()
{
    Stream::saveNeedsDecoration(true);
}

void XMLStream::throwLoadingError(const QString &error, 
                                  int error_line, int error_column) const
{
    throw Siren::io_error( QObject::tr(
            "There was an error reading the XML document at line %1, column %2.\n%3")
                .arg(error_line).arg(error_column).arg(error), CODELOC );
}

/** Construct the stream for the passed QIODevice */
XMLStream::XMLStream(QIODevice *d) : ImplementsHandle<XMLStream,Stream>(true)
{
    if (d == 0)
        XMLStream::operator=( XMLStream() );

    Stream::saveNeedsDecoration(true);
        
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

    xml->initialiseDocument( isSaving() );
}

/** Construct the stream for the passed QString */
XMLStream::XMLStream(QString *s, QIODevice::OpenMode mode)
          : ImplementsHandle<XMLStream,Stream>(true)
{
    if (s == 0)
        XMLStream::operator=( XMLStream() );

    Stream::saveNeedsDecoration(true);
        
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

    xml->initialiseDocument( isSaving() );
}

/** Construct to read from the passed string */
XMLStream::XMLStream(const QString &s) : ImplementsHandle<XMLStream,Stream>(true)
{
    Stream::saveNeedsDecoration(true);

    xml.reset( new Document() );
    
    Stream::saveLoadingMode();
    
    QString err;
    int errline, errcol;
    
    if (not xml->dom.setContent(s, true, &err, &errline, &errcol))
        throwLoadingError(err, errline, errcol);

    xml->initialiseDocument( isSaving() );
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
            
            xml->assertAttributeExists(flag, val, "value");
            
            if (val == QLatin1String("true"))
            {
                b = true;
            }
            else if (val == QLatin1String("false"))
            {
                b = false;
            }
            else
                xml->corrupted_data( QObject::tr(
                    "The \"value\" attribute for a <flag> should be "
                    "\"true\" or \"false\"."), flag );
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
            
            xml->assertAttributeExists(number, read_type, "type");
            xml->assertAttributeExists(number, read_value, "value");
            
            if (read_type != type)
                xml->corrupted_data( QObject::tr(
                        "The \"type\" attribute of this number was expected to be "
                        "\"%1\", but instead it is \"%2\".")
                            .arg(type, read_type), number );
        
            bool ok;
            
            value = ::getInteger<T>(read_value, &ok);
            
            if (not ok)
                xml->corrupted_data( QObject::tr(
                        "The \"value\" attribute of this number was expected to be "
                        "an integer between %1 and %2, but instead it was %3.")
                            .arg(std::numeric_limits<T>::min())
                            .arg(std::numeric_limits<T>::max())
                            .arg(read_value), number );
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
            
            xml->assertAttributeExists(number, read_type, "type");
            xml->assertAttributeExists(number, read_value, "value");
            
            if (read_type != type)
                xml->corrupted_data( QObject::tr(
                        "The \"type\" attribute of this number was expected to be "
                        "\"%1\", but instead it is \"%2\".")
                            .arg(type, read_type), number );
        
            bool ok;
            
            value = ::getFloat<T>(read_value, &ok);
            
            if (not ok)
                xml->corrupted_data( QObject::tr(
                        "The \"value\" attribute of this number was expected to be "
                        "a number between -%1 and %1, but instead it was %2.")
                            .arg(std::numeric_limits<T>::max())
                            .arg(read_value), number );
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

////////////
//////////// Implementation of Stream inherited protected functions
////////////

/** Signifies the start of the next item */
void XMLStream::startItem(const QString &type_name)
{
    if (not isNull())
    {
        if (isSaving())
        {
            QDomElement item = xml->createChild("object");
            item.setAttribute("type", type_name);

            xml->startChild(item);
        }
        else 
        {
            QDomElement item = xml->getChild("object");
            
            if (item.attribute("type") != type_name)
                xml->corrupted_data( QObject::tr(
                    "The type of the object in the document (%1) does not "
                    "match the desired type (%2).")
                        .arg(item.attribute("type"), type_name), item );

            xml->startChild(item);
        }
    }
}

/** Signifies the end of the current item */
void XMLStream::endItem(const QString &type_name)
{
    if (not isNull())
    {
        if ( xml->current_element.tagName() != "base" and
             (xml->current_element.tagName() != "object" or
              xml->current_element.attribute("type") != type_name) )
        {
            xml->corrupted_data( QObject::tr(
                    "The current tag does not match up with the desired "
                    "end tag <object type=\"%1\"></object>.")
                        .arg(type_name), xml->current_element );
        }
        
        xml->endChild();
    }
}

int XMLStream::startContainer(const QString &container_tag,
                              const QString &type_name, int count)
{
    if (not isNull())
    {
        if (isSaving())
        {
            QDomElement container = xml->createChild(container_tag);
            container.setAttribute("type", type_name);
            container.setAttribute("size", count);
            
            xml->startContainer(container);
            
            return count;
        }
        else
        {
            QDomElement container = xml->getChild(container_tag);
            
            if (container.attribute("type") != type_name)
                xml->corrupted_data( QObject::tr(
                    "The type of the %1 in the document (%2) does not "
                    "match the desired type (%3).")
                        .arg(container_tag)
                        .arg(container.attribute("type"), type_name), container );

            int size = xml->getIntAttribute(container, "size");

            xml->startContainer(container);
            
            return size;
        }
    }
    
    return 0;
}

void XMLStream::endContainer(const QString &container_tag, const QString &type_name)
{
    if (not isNull())
    {
        if (xml->current_element.tagName() != container_tag or
            xml->current_element.attribute("type") != type_name)
        {
            xml->corrupted_data( QObject::tr(
                    "The current tag does not match up with the desired "
                    "end tag <%1 type=\"%2\"></%1>.")
                        .arg(container_tag, type_name), xml->current_element );
        }
        
        xml->endContainer();
    }
}

/** Signifies the start of an array with contents of 
    type 'type_name' and size 'count'. This returns the 
    actual number of items in the array */
int XMLStream::startArray(const QString &type_name, int count)
{
    return this->startContainer("array", type_name, count);
}

/** Signifies the end of the array */
void XMLStream::endArray(const QString &type_name)
{
    this->endContainer("array", type_name);
}

/** Signifies the start of a Set of objects of type 'type_name',  
    containing 'count' elements - this returns the number of items
    in the set */
int XMLStream::startSet(const QString &type_name, int count)
{
    return this->startContainer("set", type_name, count);
}

/** Signifies the end of a set */
void XMLStream::endSet(const QString &type_name)
{
    this->endContainer("set", type_name);
}

/** Signifies the start of a map of objects with key type 'key_type',
    value with type 'value_type', containing 'count' elements, and 
    specifying whether or not duplicate keys are permitted - this
    returns the number of items in the map */
int XMLStream::startMap(const QString &key_type, const QString &value_type, 
                        int count, bool allow_duplicates)
{
    if (not isNull())
    {
        if (isSaving())
        {
            QDomElement container = xml->createChild("map");
            container.setAttribute("key_type", key_type);
            container.setAttribute("value_type", value_type);
            container.setAttribute("size", count);
            
            if (allow_duplicates)
                container.setAttribute("allow_duplicates", "true");
            else
                container.setAttribute("allow_duplicates", "false");
            
            xml->startContainer(container);
            
            return count;
        }
        else
        {
            QDomElement container = xml->getChild("map");
            
            if (container.attribute("key_type") != key_type or
                container.attribute("value_type") != value_type)
                xml->corrupted_data( QObject::tr(
                    "The type of the map in the document (key=%1,value=%2) does not "
                    "match the desired type (key=%3,value=%4).")
                        .arg(container.attribute("key_type"),
                             container.attribute("value_type"))
                        .arg(key_type, value_type), container );

            int size = xml->getIntAttribute(container, "size");

            xml->startContainer(container);
            
            return size;
        }
    }
    
    return 0;
}

/** Signifies the end of the map */
void XMLStream::endMap(const QString &key_type, const QString &value_type)
{
    if (not isNull())
    {
        if (xml->current_element.tagName() != "map" or
            xml->current_element.attribute("key_type") != key_type or
            xml->current_element.attribute("value_type") != value_type)
        {
            xml->corrupted_data( QObject::tr(
                    "The current tag does not match up with the desired "
                    "end tag <map key_type=\"%1\" value_type=\"%2\"></map>.")
                        .arg(key_type, value_type), 
                                xml->current_element );
        }
        
        xml->endContainer();
    }
}

/** Set the name of the next value to be written to the stream */
void XMLStream::nextData(const char *data_name)
{
    if (not isNull())
    {
        xml->next_name = data_name;
    }
}

/** Say that the next thing to be streamed is a base class */
void XMLStream::nextBase()
{
    if (not isNull())
    {
        xml->next_is_base = true;
    }
}

/** Say that the next thing to be streamed is an indexed
    item with index 'i' */
void XMLStream::nextIndex(int i)
{
    if (not isNull() and i >= 0)
    {
        xml->next_index = QString::number(i);
    }
}

/** Say that the next thing to be streamed is an entry in a set */
void XMLStream::nextEntry()
{
    if (not isNull())
    {
        xml->next_is_entry = true;
    }
}

/** Say that the next thing to be streamed is a key in a map */
void XMLStream::nextKey()
{
    if (not isNull())
    {
        xml->next_is_key = true;
    }
}

/** Say that the next thing to be streamed is a value in a map */
void XMLStream::nextValue()
{
    if (not isNull())
    {
        xml->next_is_value = true;
    }
}

/** Read and return a reference */
int XMLStream::readReference()
{
    if (not isNull())
    {
        return xml->readReference("reference");
    }
    else
        return 0;
}

/** Create a reference to the object of ID 'id' */
void XMLStream::createReference(int id)
{
    if (not isNull())
    {
        xml->createReference("reference", id);
    }
}

/** Start writing the target for ID 'id' */
void XMLStream::startTarget(int id)
{
    if (not isNull())
    {
        xml->startTarget(id, isSaving());
    }
}

/** End of target */
void XMLStream::endTarget(int id)
{
    if (not isNull())
    {
        xml->endTarget(id);
    }
}

/** Read the string reference ID */
int XMLStream::readStringReference()
{
    if (not isNull())
    {
        return xml->readReference("string");
    }
    else
        return 0;
}

/** Create the string reference ID */
void XMLStream::createStringReference(int id)
{
    if (not isNull())
    {
        xml->createReference("string", id);
    }
}

/** Read and return the string with ID 'id' */
QString XMLStream::readString(int id)
{
    if (not isNull())
    {
        return xml->readString(id);
    }
    else
        return QString::null;
}

/** Write the string data for ID == id */
void XMLStream::writeString(int id, const QString &text)
{
    if (not isNull())
    {
        xml->writeString(id, text);
    }
}

/** Read and return the blob reference */
int XMLStream::readBlobReference()
{
    if (not isNull())
    {
        return xml->readReference("binary");
    }
    else 
        return 0;
}

/** Create a binary reference */
void XMLStream::createBlobReference(int id)
{
    if (not isNull())
    {
        xml->createReference("binary", id);
    }
}

/** Read and return the blob with ID 'id' */
QByteArray XMLStream::readBlob(int id)
{
    if (not isNull())
    {
        return xml->readBlob(id);
    }
    else
        return QByteArray();
}

/** Write the blob data for ID == id */
void XMLStream::writeBlob(int id, const QByteArray &blob)
{
    if (not isNull())
    {
        xml->writeBlob(id, blob);
    }
}

/** Read the ID and version number for a class */
int XMLStream::readClassID(const QString &class_name, int &version)
{
    if (not isNull())
    {
        return xml->readClassID(class_name, version);
    }
    else
    {
        version = 0;
        return 0;
    }
}

/** Write the class ID and version number */
void XMLStream::writeClassID(const QString &class_name, int id, int version)
{
    if (not isNull())
    {
        xml->writeClassID(class_name, id, version);
    }
}

/** Return the type of the next object */
QString XMLStream::peekNextType()
{
    if (isLoading() and not isNull())
    {
        QDomElement next_element;
        
        if (xml->current_element.isNull())
            next_element = xml->document_root.firstChildElement();
        else
            next_element = xml->current_element.nextSiblingElement();

        while (not next_element.isNull())
        {
            if (next_element.tagName() == "reference")
            {
                //is the target of this reference an <object>?
            
                QString id = next_element.attribute("id", id);
                
                if (not id.isEmpty())
                {
                    QDomElement target = xml->document_root.firstChildElement("targets")
                                                           .firstChildElement("target");
                                                           
                    while (not target.isNull())
                    {
                        if (target.attribute("id") == id)
                        {
                            QDomElement obj = target.firstChildElement("object");
                            
                            QString typ = obj.attribute("type");
                            
                            if (not typ.isEmpty())
                                return typ;
                            
                            break;
                        }
                        else
                            target = target.nextSiblingElement("target");
                    }
                }
            }
            else if (next_element.tagName() == "object")
            {
                QString typ = next_element.attribute("type");
                
                if (not typ.isEmpty())
                    return typ;
            }
            else
                next_element = next_element.nextSiblingElement();
        }
    }
    
    return QString::null;
}

/** Decorated streams like XMLStream don't use magic numbers */
int XMLStream::readMagic()
{
    return 0;
}

/** Decorated streams like XMLStream don't use magic numbers */
void XMLStream::writeMagic(int)
{}
