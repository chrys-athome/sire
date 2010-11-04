#ifndef GCCXML_PARSE_QFILE_H
#define GCCXML_PARSE_QFILE_H

#ifdef GCCXML_PARSE

#include "qtheaders/qiodevice.h"

class QFile : public QIODevice
{
public:

    enum FileError {
        NoError = 0,
        ReadError = 1,
        WriteError = 2,
        FatalError = 3,
        ResourceError = 4,
        OpenError = 5,
        AbortError = 6,
        TimeOutError = 7,
        UnspecifiedError = 8,
        RemoveError = 9,
        RenameError = 10,
        PositionError = 11,
        ResizeError = 12,
        PermissionsError = 13,
        CopyError = 14
    };

    enum Permission {
        ReadOwner = 0x4000, WriteOwner = 0x2000, ExeOwner = 0x1000,
        ReadUser  = 0x0400, WriteUser  = 0x0200, ExeUser  = 0x0100,
        ReadGroup = 0x0040, WriteGroup = 0x0020, ExeGroup = 0x0010,
        ReadOther = 0x0004, WriteOther = 0x0002, ExeOther = 0x0001
    };
    Q_DECLARE_FLAGS(Permissions, Permission)

    QFile();
    QFile(const QString &name);
    ~QFile();

    FileError error() const;
    void unsetError();

    QString fileName() const;
    void setFileName(const QString &name);

    static QByteArray encodeName(const QString &fileName);
    static QString decodeName(const QByteArray &localFileName);
    
    bool exists() const;
    static bool exists(const QString &fileName);

    QString readLink() const;
    static QString readLink(const QString &fileName);
    QString symLinkTarget() const;
    static QString symLinkTarget(const QString &fileName);

    bool remove();
    static bool remove(const QString &fileName);

    bool rename(const QString &newName);
    static bool rename(const QString &oldName, const QString &newName);

    bool link(const QString &newName);
    static bool link(const QString &oldname, const QString &newName);

    bool copy(const QString &newName);
    static bool copy(const QString &fileName, const QString &newName);

    bool isSequential() const;

    bool open(OpenMode flags);
    virtual void close();

    qint64 size() const;
    qint64 pos() const;
    bool seek(qint64 offset);
    bool atEnd() const;
    bool flush();

    bool resize(qint64 sz);
    static bool resize(const QString &filename, qint64 sz);

    Permissions permissions() const;
    static Permissions permissions(const QString &filename);
    bool setPermissions(Permissions permissionSpec);
    static bool setPermissions(const QString &filename, Permissions permissionSpec);

    int handle() const;
};

template class QFlags<QFile::Permission>;

#else
#include <QFile>

#endif

#endif // QFILE_H
