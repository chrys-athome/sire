#ifndef GCCXML_PARSE_QIODEVICE_H
#define GCCXML_PARSE_QIODEVICE_H

#ifdef GCCXML_PARSE

class QByteArray;

typedef long qint64;

#define Q_DECLARE_FLAGS(Flags, Enum)\
typedef QFlags<Enum> Flags;

class QFlag
{
public:
    QFlag(int i);
    operator int() const;
};

template<typename Enum>
class QFlags
{
    typedef void **Zero;

public:
    typedef Enum enum_type;
    QFlags(const QFlags &f);
    QFlags(Enum f);
    QFlags(Zero = 0);
    QFlags(QFlag f);

    QFlags &operator=(const QFlags &f);
    QFlags &operator&=(int mask);
    QFlags &operator&=(uint mask);
    QFlags &operator|=(QFlags f);
    QFlags &operator|=(Enum f);
    QFlags &operator^=(QFlags f);
    QFlags &operator^=(Enum f);

    operator int() const;

    QFlags operator|(QFlags f) const;
    QFlags operator|(Enum f) const;
    QFlags operator^(QFlags f) const;
    QFlags operator^(Enum f) const;
    QFlags operator&(int mask) const;
    QFlags operator&(uint mask) const;
    QFlags operator&(Enum f) const;
    QFlags operator~() const;

    bool operator!() const;

    bool testFlag(Enum f) const;
};

class QIODevice
{
public:
    enum OpenModeFlag {
        NotOpen = 0x0000,
        ReadOnly = 0x0001,
        WriteOnly = 0x0002,
        ReadWrite = ReadOnly | WriteOnly,
        Append = 0x0004,
        Truncate = 0x0008,
        Text = 0x0010,
        Unbuffered = 0x0020
    };

    Q_DECLARE_FLAGS(OpenMode, OpenModeFlag);

    QIODevice();
    virtual ~QIODevice();

    OpenMode openMode() const;

    void setTextModeEnabled(bool enabled);
    bool isTextModeEnabled() const;

    bool isOpen() const;
    bool isReadable() const;
    bool isWritable() const;
    bool isSequential() const;

    bool open(OpenMode mode);
    void close();

    qint64 pos() const;
    qint64 size() const;
    bool seek(qint64 pos);
    bool atEnd() const;
    bool reset();

    qint64 bytesAvailable() const;
    qint64 bytesToWrite() const;

    qint64 read(char *data, qint64 maxlen);
    QByteArray read(qint64 maxlen);
    QByteArray readAll();
    qint64 readLine(char *data, qint64 maxlen);
    QByteArray readLine(qint64 maxlen = 0);
    bool canReadLine() const;

    qint64 write(const char *data, qint64 len);
    qint64 write(const QByteArray &data);

    qint64 peek(char *data, qint64 maxlen);
    QByteArray peek(qint64 maxlen);

    bool waitForReadyRead(int msecs);
    bool waitForBytesWritten(int msecs);

    void ungetChar(char c);
    bool putChar(char c);
    bool getChar(char *c);

    QString errorString() const;

protected:
    QIODevice(const QIODevice&);
    QIODevice& operator=(const QIODevice&);

    virtual qint64 writeData(const char*, qint64)=0;
    virtual qint64 readData(char*, qint64)=0;
};

template class QFlags<QIODevice::OpenModeFlag>;

#else // GCCXML_PARSE
#include <QIODevice>

#endif // GCCXML_PARSE

#endif // GCCXML_PARSE_QIODEVICE_H
