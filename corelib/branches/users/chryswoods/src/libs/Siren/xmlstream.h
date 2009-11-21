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

#ifndef SIREN_XMLSTREAM_H
#define SIREN_XMLSTREAM_H

#include <QDomDocument>
#include <QDomElement>

#include "stream.h"

SIREN_BEGIN_HEADER

namespace Siren
{

/** This is a stream that saves/loads data to XML 

    @author Christopher Woods
*/
class SIREN_EXPORT XMLStream : public ImplementsHandle<XMLStream,Stream>
{
public:
    XMLStream();
    
    XMLStream(QIODevice *d);
    XMLStream(QString *s, QIODevice::OpenMode mode);
    XMLStream(const QString &s);
    
    XMLStream(const XMLStream &other);
    
    ~XMLStream();
    
    XMLStream& operator=(const XMLStream &other);
    
    bool operator==(const XMLStream &other) const;
    bool operator!=(const XMLStream &other) const;

    XMLStream& operator&(bool &b); 
    
    XMLStream& operator&(qint8 &i);
    XMLStream& operator&(quint8 &i);
    XMLStream& operator&(quint16 &i);
    XMLStream& operator&(qint16 &i);
    XMLStream& operator&(quint32 &i);
    XMLStream& operator&(qint32 &i);
    XMLStream& operator&(quint64 &i);
    XMLStream& operator&(qint64 &i);

    XMLStream& operator&(float &f);
    XMLStream& operator&(double &f);
    
    uint hashCode() const;
    QString toString() const;
    bool test(Logger &logger) const;
    
protected:
    void startItem(const QString &type_name);
    int startArray(const QString &type_name, int count);
    int startSet(const QString &type_name, int count);
    int startMap(const QString &key_type, const QString &value_type, 
                 int count, bool allow_duplicates);

    void nextData(const char *data_name);
    void nextBase();

    void nextIndex(int i);
    
    void nextEntry();
    
    void nextKey();
    void nextValue();

    void endItem(const QString &type_name);
    void endArray(const QString &type_name);
    void endSet(const QString &type_name);
    void endMap(const QString &key_type, const QString &value_type);

    int readReference();
    void createReference(int id);
    
    void startTarget(int id);
    void endTarget(int id);

    int readStringReference();
    void createStringReference(int id);
    
    QString readString(int id);
    void writeString(int id, const QString &text);

    int readBlobReference();
    void createBlobReference(int id);

    QByteArray readBlob(int id);
    void writeBlob(int id, const QByteArray &blob);

    int readClassID(const QString &class_name, int &version);
    void writeClassID(const QString &class_name, int id, int version);
    
    QString peekNextType();

    int readMagic();
    void writeMagic(int magic);

private:
    void throwLoadingError(const QString &error, 
                           int error_line, int error_column) const;

    template<class T>
    void stream_integer(const QString &type, T &value);
    
    template<class T>
    void stream_float(const QString &type, T &value);

    class Document
    {
    public:
        Document();
        ~Document();
        
        void assertAttributeExists(const QString &value, 
                                   const QString &tag, 
                                   const QString &attribute) const;
        
        void corruptedData(const QString &error_text) const;
        
        void startTarget(int id, bool is_saving);
        void endTarget();
        
        void startString(int id, bool is_saving);
        void endString();
        
        void startBlob(int id, bool is_saving);
        void endBlob();
        
        void pushStack();
        void popStack();
        
        QDomElement createChild(const QString &tag_name);
        QDomElement getChild(const QString &tag_name);
        
        QDomDocument dom;

        QDomElement current_element;
        QDomElement last_element;

        QStack<QDomElement> current_element_stack;
        QStack<QDomElement> last_element_stack; 
        
        QIODevice *device;
        QString *string;
    };

    /** The document containing the XML */
    boost::shared_ptr<Document> xml;
};

}

Q_DECLARE_METATYPE( Siren::XMLStream )

SIREN_EXPOSE_CLASS( Siren::XMLStream )

SIREN_END_HEADER

#endif
