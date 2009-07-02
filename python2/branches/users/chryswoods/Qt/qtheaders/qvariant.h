#ifndef GCCXML_PARSE_QVARIANT_H
#define GCCXML_PARSE_QVARIANT_H

#ifdef GCCXML_PARSE

class QBitArray;
class QChar;
class QDataStream;
class QDate;
class QDateTime;
class QLine;
class QLineF;
class QLocale;
class QMatrix;
class QStringList;
class QTime;
class QPoint;
class QPointF;
class QSize;
class QSizeF;
class QRect;
class QRectF;
class QRegExp;
class QTextFormat;
class QTextLength;
class QUrl;
class QVariant;
class QVariantComparisonHelper;

template<class Key, class T>
class QMap;

template<class T>
class QList;

class QVariant
{
 public:
    enum Type {
        Invalid = 0,

        Bool = 1,
        Int = 2,
        UInt = 3,
        LongLong = 4,
        ULongLong = 5,
        Double = 6,
        Char = 7,
        Map = 8,
        List = 9,
        String = 10,
        StringList = 11,
        ByteArray = 12,
        BitArray = 13,
        Date = 14,
        Time = 15,
        DateTime = 16,
        Url = 17,
        Locale = 18,
        Rect = 19,
        RectF = 20,
        Size = 21,
        SizeF = 22,
        Line = 23,
        LineF = 24,
        Point = 25,
        PointF = 26,
	RegExp = 27,
        LastCoreType = RegExp,

        Font = 64,
        Pixmap = 65,
        Brush = 66,
        Color = 67,
        Palette = 68,
        Icon = 69,
        Image = 70,
        Polygon = 71,
        Region = 72,
        Bitmap = 73,
        Cursor = 74,
        SizePolicy = 75,
        KeySequence = 76,
        Pen = 77,
        TextLength = 78,
        TextFormat = 79,
        Matrix = 80,
        LastGuiType = Matrix,

        UserType = 127,
        LastType = 0xffffffff // need this so that gcc >= 3.4 allocates 32 bits for Type
    };

    QVariant();
    ~QVariant();
    QVariant(Type type);
    QVariant(const QVariant &other);

    QVariant(QDataStream &s);

    QVariant(int i);
    QVariant(uint ui);
    QVariant(bool b);
    QVariant(double d);

    QVariant(const QByteArray &bytearray);
    QVariant(const QBitArray &bitarray);
    QVariant(const QString &string);
    QVariant(const QStringList &stringlist);
    QVariant(const QChar &qchar);
    QVariant(const QDate &date);
    QVariant(const QTime &time);
    QVariant(const QDateTime &datetime);
    QVariant(const QList<QVariant> &list);
    QVariant(const QMap<QString,QVariant> &map);
    QVariant(const QUrl &url);
    QVariant(const QLocale &locale);
    QVariant(const QRegExp &regExp);

    QVariant& operator=(const QVariant &other);

    Type type() const;
    int userType() const;
    const char *typeName() const;

    bool canConvert(Type t) const;
    bool convert(Type t);

    bool isValid() const;
    bool isNull() const;

    void clear();

    void detach();
    bool isDetached() const;

    int toInt(bool *ok = 0) const;
    uint toUInt(bool *ok = 0) const;
    bool toBool() const;
    double toDouble(bool *ok = 0) const;
    QByteArray toByteArray() const;
    QBitArray toBitArray() const;
    QString toString() const;
    QStringList toStringList() const;
    QChar toChar() const;
    QDate toDate() const;
    QTime toTime() const;
    QDateTime toDateTime() const;
    QList<QVariant> toList() const;
    QMap<QString, QVariant> toMap() const;

    QUrl toUrl() const;
    QLocale toLocale() const;
    QRegExp toRegExp() const;

    void load(QDataStream &ds);
    void save(QDataStream &ds) const;

    static const char *typeToName(Type type);
    static Type nameToType(const char *name);
};

QDataStream& operator>> (QDataStream& s, QVariant& p);
QDataStream& operator<< (QDataStream& s, const QVariant& p);
QDataStream& operator>> (QDataStream& s, QVariant::Type& p);
QDataStream& operator<< (QDataStream& s, const QVariant::Type p);

#else
#include <QVariant>

#endif

#endif // QVARIANT_H
