#ifndef GCCXML_PARSE_QFILEINFO_H
#define GCCXML_PARSE_QFILEINFO_H

#ifdef GCCXML_PARSE

class QDir;
class QDateTime;

#include "qtheaders/qfile.h"

class QFileInfo
{
public:
    QFileInfo();
    QFileInfo(const QString &file);
    QFileInfo(const QFile &file);
    QFileInfo(const QDir &dir, const QString &file);
    QFileInfo(const QFileInfo &fileinfo);
    ~QFileInfo();

    QFileInfo &operator=(const QFileInfo &fileinfo);
    bool operator==(const QFileInfo &fileinfo) const;
    bool operator!=(const QFileInfo &fileinfo) const;

    void setFile(const QString &file);
    void setFile(const QFile &file);
    void setFile(const QDir &dir, const QString &file);
    bool exists() const;
    void refresh();

    QString filePath() const;
    QString absoluteFilePath() const;
    QString canonicalFilePath() const;
    QString fileName() const;
    QString baseName() const;
    QString completeBaseName() const;
    QString suffix() const;
    QString completeSuffix() const;

    QString path() const;
    QString absolutePath() const;
    QString canonicalPath() const;
    QDir dir() const;
    QDir absoluteDir() const;

    bool isReadable() const;
    bool isWritable() const;
    bool isExecutable() const;
    bool isHidden() const;

    bool isRelative() const;
    bool isAbsolute() const;
    bool makeAbsolute();

    bool isFile() const;
    bool isDir() const;
    bool isSymLink() const;
    bool isRoot() const;

    QString readLink() const;
    QString symLinkTarget() const;

    QString owner() const;
    uint ownerId() const;
    QString group() const;
    uint groupId() const;

    bool permission(QFile::Permissions permissions) const;
    QFile::Permissions permissions() const;

    qint64 size() const;

    QDateTime created() const;
    QDateTime lastModified() const;
    QDateTime lastRead() const;

    void detach();

    bool caching() const;
    void setCaching(bool on);
};

#else
#include <QFileInfo>

#endif

#endif // QFILEINFO_H
