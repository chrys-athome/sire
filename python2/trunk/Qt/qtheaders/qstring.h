#ifndef GCCXML_PARSE_QSTRING_H
#define GCCXML_PARSE_QSTRING_H

#ifdef GCCXML_PARSE

class QRegExp;
class QStringList;
class QTextCodec;
class QLatin1String;
template <typename T> class QVector;

typedef unsigned int uint;
typedef unsigned long ulong;

#include "qtheaders/qchar.h"

namespace Qt
{
    enum CaseSensitivity {
        CaseInsensitive,
        CaseSensitive
    };
}

class QBool
{
public:
    explicit QBool(bool B);
    operator const void *() const;
};

class QString
{
public:
    QString();
    QString(const QChar *unicode, int size);
    QString(QChar c);
    QString(int size, QChar c);
    QString(const QString &);
    ~QString();
    QString &operator=(const QString &);

    int size() const;
    int count() const;
    int length() const;
    bool isEmpty() const;
    void resize(int size);

    QString &fill(QChar c, int size = -1);
    void truncate(int pos);
    void chop(int n);

    int capacity() const;
    void reserve(int size);
    void squeeze();

    void detach();
    bool isDetached() const;
    void clear();

    const QChar at(int i) const;
    const QChar operator[](int i) const;
    const QChar operator[](uint i) const;

    QString arg(long a, int fieldwidth=0, int base=10,
                const QChar &fillChar = ' ') const;
    QString arg(ulong a, int fieldwidth=0, int base=10,
                const QChar &fillChar = ' ') const;
    QString arg(int a, int fieldWidth = 0, int base = 10,
                const QChar &fillChar = ' ') const;
    QString arg(uint a, int fieldWidth = 0, int base = 10,
                const QChar &fillChar = ' ') const;
    QString arg(double a, int fieldWidth = 0, char fmt = 'g', int prec = -1,
                const QChar &fillChar = ' ') const;
    QString arg(char a, int fieldWidth = 0,
                const QChar &fillChar = ' ') const;
    QString arg(QChar a, int fieldWidth = 0,
                const QChar &fillChar = ' ') const;
    QString arg(const QString &a, int fieldWidth = 0,
                const QChar &fillChar = ' ') const;
    QString arg(const QString &a1, const QString &a2) const;
    QString arg(const QString &a1, const QString &a2, const QString &a3) const;
    QString arg(const QString &a1, const QString &a2, const QString &a3,
                const QString &a4) const;

    int indexOf(QChar c, int from = 0, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;
    int indexOf(const QString &s, int from = 0, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;
    int lastIndexOf(QChar c, int from = -1, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;
    int lastIndexOf(const QString &s, int from = -1, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;

    QBool contains(QChar c, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;
    QBool contains(const QString &s, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;
    int count(QChar c, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;
    int count(const QString &s, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;

    int indexOf(const QRegExp &, int from = 0) const;
    int lastIndexOf(const QRegExp &, int from = -1) const;
    inline QBool contains(const QRegExp &rx) const { return QBool(indexOf(rx) != -1); }
    int count(const QRegExp &) const;

    enum SectionFlag {
        SectionDefault             = 0x00,
        SectionSkipEmpty           = 0x01,
        SectionIncludeLeadingSep   = 0x02,
        SectionIncludeTrailingSep  = 0x04,
        SectionCaseInsensitiveSeps = 0x08
    };
    Q_DECLARE_FLAGS(SectionFlags, SectionFlag)

    QString section(QChar sep, int start, int end, SectionFlags flags) const;
    QString section(const QString &in_sep, int start, int end, SectionFlags flags) const;
    QString section(const QRegExp &reg, int start, int end, SectionFlags flags) const;

    QString left(int len) const;
    QString right(int len) const;
    QString mid(int i, int len = -1) const;

    bool startsWith(const QString &s, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;
    bool startsWith(const QChar &c, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;
    bool endsWith(const QString &s, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;
    bool endsWith(const QChar &c, Qt::CaseSensitivity cs = Qt::CaseSensitive) const;

    QString leftJustified(int width, QChar fill = ' ', bool trunc = false) const;
    QString rightJustified(int width, QChar fill = ' ', bool trunc = false) const;

    QString toLower() const;
    QString toUpper() const;

    QString trimmed() const;
    QString simplified() const;

    QString &insert(int i, QChar c);
    QString &insert(int i, const QChar *uc, int len);
    QString &insert(int i, const QString &s);
    QString &insert(int i, const QLatin1String &s);
    QString &append(QChar c);
    QString &append(const QString &s);
    QString &prepend(QChar c);
    QString &prepend(const QString &s);
    QString &operator+=(QChar c);
    QString &operator+=(const QString &s);

    QString &remove(int i, int len);
    QString &remove(QChar c, Qt::CaseSensitivity cs = Qt::CaseSensitive);
    QString &remove(const QString &s, Qt::CaseSensitivity cs = Qt::CaseSensitive);
    QString &replace(int i, int len, QChar after);
    QString &replace(int i, int len, const QChar *s, int slen);
    QString &replace(int i, int len, const QString &after);
    QString &replace(QChar before, QChar after, Qt::CaseSensitivity cs = Qt::CaseSensitive);
    QString &replace(QChar c, const QString &after, Qt::CaseSensitivity cs = Qt::CaseSensitive);
    QString &replace(const QString &before, const QString &after,
                     Qt::CaseSensitivity cs = Qt::CaseSensitive);
    QString &replace(const QRegExp &rx, const QString &after);
    QString &remove(const QRegExp &rx);

    enum SplitBehavior { KeepEmptyParts, SkipEmptyParts };

    QStringList split(const QString &sep, SplitBehavior behavior,
                      Qt::CaseSensitivity cs) const;
    QStringList split(const QChar &sep, SplitBehavior behavior,
                      Qt::CaseSensitivity cs) const;
    QStringList split(const QRegExp &sep, SplitBehavior behavior) const;

    enum NormalizationForm {
        NormalizationForm_D,
        NormalizationForm_C,
        NormalizationForm_KD,
        NormalizationForm_KC
    };
    QString normalized(NormalizationForm mode) const;
    QString normalized(NormalizationForm mode, QChar::UnicodeVersion version) const;

    QByteArray toAscii() const;
    QByteArray toLatin1() const;
    QByteArray toUtf8() const;
    QByteArray toLocal8Bit() const;

    static QString fromAscii(const char *, int size = -1);
    static QString fromLatin1(const char *, int size = -1);
    static QString fromUtf8(const char *, int size = -1);
    static QString fromLocal8Bit(const char *, int size = -1);
    static QString fromUtf16(const ushort *, int size = -1);
    static QString fromUcs4(const uint *, int size = -1);
    static QString fromRawData(const QChar *, int size);

    QString &setUnicode(const QChar *unicode, int size);

    int compare(const QString &s) const;
    int compare(const QString &s, Qt::CaseSensitivity cs) const;

    static int compare(const QString &s1, const QString &s2);
    static int compare(const QString &s1, const QString &s2, Qt::CaseSensitivity cs);

    int localeAwareCompare(const QString& s) const;
    static int localeAwareCompare(const QString& s1, const QString& s2);

    short  toShort(bool *ok=0, int base=10) const;
    ushort toUShort(bool *ok=0, int base=10) const;
    int toInt(bool *ok=0, int base=10) const;
    uint toUInt(bool *ok=0, int base=10) const;
    long toLong(bool *ok=0, int base=10) const;
    ulong toULong(bool *ok=0, int base=10) const;
    float toFloat(bool *ok=0) const;
    double toDouble(bool *ok=0) const;

    QString &setNum(short, int base=10);
    QString &setNum(ushort, int base=10);
    QString &setNum(int, int base=10);
    QString &setNum(uint, int base=10);
    QString &setNum(long, int base=10);
    QString &setNum(ulong, int base=10);
    QString &setNum(float, char f='g', int prec=6);
    QString &setNum(double, char f='g', int prec=6);

    static QString number(int, int base=10);
    static QString number(uint, int base=10);
    static QString number(long, int base=10);
    static QString number(ulong, int base=10);
    static QString number(double, char f='g', int prec=6);

    bool operator==(const QString &s) const;
    bool operator<(const QString &s) const;
    bool operator>(const QString &s) const;
    bool operator!=(const QString &s) const;
    bool operator<=(const QString &s) const;
    bool operator>=(const QString &s) const;

    void push_back(QChar c);
    void push_back(const QString &s);
    void push_front(QChar c);
    void push_front(const QString &s);

    bool isNull() const;
};

#else

#include <QString>

#endif

#endif // QSTRING_H
