#ifndef GCCXML_PARSE_QDATASTREAM_H
#define GCCXML_PARSE_QDATASTREAM_H

#ifdef GCCXML_PARSE

class QByteArray;
class QIODevice;

#include "qtheaders/qiodevice.h"

template <typename T> class QList;
template <typename T> class QLinkedList;
template <typename T> class QVector;
template <typename T> class QSet;
template <class Key, class T> class QHash;
template <class Key, class T> class QMap;

class QDataStream
{
public:
    enum Version {
        Qt_1_0 = 1,
        Qt_2_0 = 2,
        Qt_2_1 = 3,
        Qt_3_0 = 4,
        Qt_3_1 = 5,
        Qt_3_3 = 6,
        Qt_4_0 = 7,
        Qt_4_1 = Qt_4_0,
        Qt_4_2 = 8
    };

    enum ByteOrder {
        BigEndian,
        LittleEndian
    };

    enum Status {
        Ok,
        ReadPastEnd,
	ReadCorruptData
    };

    QDataStream();
    QDataStream(QIODevice *);
    QDataStream(QByteArray *, QIODevice::OpenMode flags);
    QDataStream(const QByteArray &);
    ~QDataStream();

    QIODevice *device() const;
    void setDevice(QIODevice *);
    void unsetDevice();

    bool atEnd() const;

    Status status() const;
    void setStatus(Status status);
    void resetStatus();

    ByteOrder byteOrder() const;
    void setByteOrder(ByteOrder);

    int version() const;
    void setVersion(int);

protected:
    QDataStream(const QDataStream&);
    QDataStream& operator=(const QDataStream&);
};

#else // ifdef GCCXML_PARSE
#include <QDataStream>

#endif // ifdef GCCXML_PARSE

#endif // GCCXML_PARSE_QDATASTREAM_H
