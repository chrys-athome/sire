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

#ifndef SIREN_DATASTREAM_H
#define SIREN_DATASTREAM_H

#include "stream.h"

SIREN_BEGIN_HEADER

namespace Siren
{

class SharedDataRegistry;

/** This is a streaming class that is used to help stream implicitly shared data. 
    This class ensures that only a single copy of the implicitly shared data is actually 
    written to the stream, thus reducing data bloat, and also preserving the shared data
    of the objects when they are read back into the program.

    @author Christopher Woods
*/
class SIREN_EXPORT DataStream : public ImplementsHandle<DataStream,Stream>
{
public:
    DataStream();
    DataStream(QIODevice *d);
    DataStream(QByteArray *a, QIODevice::OpenMode mode);
    DataStream(const QByteArray &a);
    
    DataStream(QDataStream &ds);

    DataStream(const DataStream &other);

    ~DataStream();

    DataStream& operator=(const DataStream &other);
    
    bool operator==(const DataStream &other) const;
    bool operator!=(const DataStream &other) const;

    Stream& operator&(bool &b); 
    
    Stream& operator&(qint8 &i);
    Stream& operator&(quint8 &i);
    Stream& operator&(quint16 &i);
    Stream& operator&(qint16 &i);
    Stream& operator&(qint32 &i);
    Stream& operator&(quint64 &i);
    Stream& operator&(qint64 &i);
    Stream& operator&(quint32 &i);

    Stream& operator&(float &f);
    Stream& operator&(double &f);
    
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
    void registerDataStream();

    /** Shared pointer to a datastream is one had to be created */
    boost::shared_ptr<QDataStream> ds_ptr;

    /** The version number of the DataStream format */
    int ds_version;

    /** Reference to the active datastream */
    QDataStream *ds;
};

} // end of namespace Siren

Q_DECLARE_METATYPE( Siren::DataStream )

SIREN_EXPOSE_CLASS( Siren::DataStream )

SIREN_END_HEADER

#endif
