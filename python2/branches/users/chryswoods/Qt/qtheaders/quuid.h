#ifndef GCCXML_PARSE_QUUID_H
#define GCCXML_PARSE_QUUID_H

#ifdef GCCXML_PARSE

struct QUuid
{
    enum Variant {
        VarUnknown        =-1,
        NCS                = 0, // 0 - -
        DCE                = 2, // 1 0 -
        Microsoft        = 6, // 1 1 0
        Reserved        = 7  // 1 1 1
    };

    enum Version {
        VerUnknown        =-1,
        Time                = 1, // 0 0 0 1
        EmbeddedPOSIX        = 2, // 0 0 1 0
        Name                = 3, // 0 0 1 1
        Random                = 4  // 0 1 0 0
    };

    QUuid();
    QUuid(uint l, ushort w1, ushort w2, uchar b1, uchar b2, uchar b3, uchar b4, uchar b5, uchar b6, uchar b7, uchar b8);

    QUuid(const QString &);
    QUuid(const char *);
    QString toString() const;
    bool isNull() const;

    bool operator==(const QUuid &orig) const;
    bool operator!=(const QUuid &orig) const;

    bool operator<(const QUuid &other) const;
    bool operator>(const QUuid &other) const;

    static QUuid createUuid();
    QUuid::Variant variant() const;
    QUuid::Version version() const;
};

QDataStream &operator<<(QDataStream &, const QUuid &);
QDataStream &operator>>(QDataStream &, QUuid &);

#else // ifdef GCCXML_PARSE
#include <QUuid>

#endif // ifdef GCCXML_PARSE

#endif // GCCXML_PARSE_QUUID_H
