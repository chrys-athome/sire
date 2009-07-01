#ifndef GCCXML_PARSE_QDIR_H
#define GCCXML_PARSE_QDIR_H

#ifdef GCCXML_PARSE

#include "qtheaders/qiodevice.h"
#include "qtheaders/qstring.h"

class QStringList;
class QFileInfoList;

class QDir
{
public:
    enum Filter { Dirs        = 0x001,
                  Files       = 0x002,
                  Drives      = 0x004,
                  NoSymLinks  = 0x008,
                  AllEntries  = Dirs | Files | Drives,
                  TypeMask    = 0x00f,
                  Readable    = 0x010,
                  Writable    = 0x020,
                  Executable  = 0x040,
                  PermissionMask    = 0x070,
                  Modified    = 0x080,
                  Hidden      = 0x100,
                  System      = 0x200,
                 
                  AccessMask  = 0x3F0,

                  AllDirs       = 0x400,
                  CaseSensitive = 0x800,
                  NoDotAndDotDot = 0x1000,

                  NoFilter = -1
    };
    Q_DECLARE_FLAGS(Filters, Filter)

    enum SortFlag { Name        = 0x00,
                    Time        = 0x01,
                    Size        = 0x02,
                    Unsorted    = 0x03,
                    SortByMask  = 0x03,

                    DirsFirst   = 0x04,
                    Reversed    = 0x08,
                    IgnoreCase  = 0x10,
                    DirsLast    = 0x20,
                    LocaleAware = 0x40, 
                    Type        = 0x80,
                    NoSort = -1
    };
    Q_DECLARE_FLAGS(SortFlags, SortFlag)

    QDir(const QDir &);
    QDir(const QString &path = QString());
    ~QDir();

    QDir &operator=(const QDir &);
    QDir &operator=(const QString &path);

    void setPath(const QString &path);
    QString path() const;
    QString absolutePath() const;
    QString canonicalPath() const;

    static void addResourceSearchPath(const QString &path);

    QString dirName() const;
    QString filePath(const QString &fileName) const;
    QString absoluteFilePath(const QString &fileName) const;
    QString relativeFilePath(const QString &fileName) const;

    static QString toNativeSeparators(const QString &pathName);
    static QString fromNativeSeparators(const QString &pathName);

    bool cd(const QString &dirName);
    bool cdUp();

    QStringList nameFilters() const;
    void setNameFilters(const QStringList &nameFilters);

    Filters filter() const;
    void setFilter(Filters filter);
    SortFlags sorting() const;
    void setSorting(SortFlags sort);

    uint count() const;
    QString operator[](int) const;

    static QStringList nameFiltersFromString(const QString &nameFilter);

    QStringList entryList(Filters filters, SortFlags sort) const;
    QStringList entryList(const QStringList &nameFilters, Filters filters,
                          SortFlags sort) const;

    QFileInfoList entryInfoList(QDir::Filters filters, QDir::SortFlags sort) const;
    QFileInfoList entryInfoList(const QStringList &nameFilters, QDir::Filters filters,
                                QDir::SortFlags sort) const;

    bool mkdir(const QString &dirName) const;
    bool rmdir(const QString &dirName) const;
    bool mkpath(const QString &dirPath) const;
    bool rmpath(const QString &dirPath) const;

    bool isReadable() const;
    bool exists() const;
    bool isRoot() const;

    static bool isRelativePath(const QString &path);
    static bool isAbsolutePath(const QString &path);
    bool isRelative() const;
    bool isAbsolute() const;
    bool makeAbsolute();

    bool operator==(const QDir &dir) const;
    bool operator!=(const QDir &dir) const;

    bool remove(const QString &fileName);
    bool rename(const QString &oldName, const QString &newName);
    bool exists(const QString &name) const;

    static QFileInfoList drives();

    static QChar separator();

    static bool setCurrent(const QString &path);
    static QDir current();
    static QString currentPath();

    static QDir home();
    static QString homePath();
    static QDir root();
    static QString rootPath();
    static QDir temp();
    static QString tempPath();

    static bool match(const QStringList &filters, const QString &fileName);
    static bool match(const QString &filter, const QString &fileName);

    static QString cleanPath(const QString &path);
    void refresh() const;
};

#else // GCCXML_PARSE

#include <QDir>

#endif // GCCXML_PARSE

#endif // GCCXML_PARSE_QDIR_H
