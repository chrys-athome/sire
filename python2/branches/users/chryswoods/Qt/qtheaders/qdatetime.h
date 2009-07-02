#ifndef GCCXML_PARSE_QDATETIME_H
#define GCCXML_PARSE_QDATETIME_H

#ifdef GCCXML_PARSE

typedef long qint64;

typedef unsigned int uint;
typedef unsigned short ushort;

class QString;
class QDataStream;

class QDate
{
public:
    QDate();
    QDate(int y, int m, int d);

    bool isNull() const;
    bool isValid() const;

    int year() const;
    int month() const;
    int day() const;
    int dayOfWeek() const;
    int dayOfYear() const;
    int daysInMonth() const;
    int daysInYear() const;
    int weekNumber(int *yearNum = 0) const;

    static QString shortMonthName(int month);
    static QString shortDayName(int weekday);
    static QString longMonthName(int month);
    static QString longDayName(int weekday);

    QString toString(const QString &format) const;

    bool setYMD(int y, int m, int d);
    bool setDate(int year, int month, int date);

    QDate addDays(int days) const;
    QDate addMonths(int months) const;
    QDate addYears(int years) const;
    int daysTo(const QDate &) const;

    bool operator==(const QDate &other) const;
    bool operator!=(const QDate &other) const;
    bool operator<(const QDate &other) const;
    bool operator<=(const QDate &other) const;
    bool operator>(const QDate &other) const;
    bool operator>=(const QDate &other) const;

    static QDate currentDate();
    static QDate fromString(const QString &s, const QString &format);

    static bool isValid(int y, int m, int d);
    static bool isLeapYear(int year);

    static QDate fromJulianDay(int jd);
    inline int toJulianDay() const;
};

class QTime
{
public:
    QTime();
    QTime(int h, int m, int s = 0, int ms = 0);

    bool isNull() const;
    bool isValid() const;

    int hour() const;
    int minute() const;
    int second() const;
    int msec() const;

    QString toString(const QString &format) const;

    bool setHMS(int h, int m, int s, int ms = 0);

    QTime addSecs(int secs) const;
    int secsTo(const QTime &) const;
    QTime addMSecs(int ms) const;
    int msecsTo(const QTime &) const;

    bool operator==(const QTime &other) const;
    bool operator!=(const QTime &other) const;
    bool operator<(const QTime &other) const;
    bool operator<=(const QTime &other) const;
    bool operator>(const QTime &other) const;
    bool operator>=(const QTime &other) const;

    static QTime currentTime();

    static QTime fromString(const QString &s, const QString &format);

    static bool isValid(int h, int m, int s, int ms = 0);

    void start();
    int restart();
    int elapsed() const;
};

class QDateTime
{
public:
    QDateTime();
    QDateTime(const QDateTime &other);
    ~QDateTime();

    QDateTime &operator=(const QDateTime &other);

    bool isNull() const;
    bool isValid() const;

    QDate date() const;
    QTime time() const;
    uint toTime_t() const;
    void setDate(const QDate &date);
    void setTime(const QTime &time);
    void setTime_t(uint secsSince1Jan1970UTC);
    QString toString(const QString &format) const;
    QDateTime addDays(int days) const;
    QDateTime addMonths(int months) const;
    QDateTime addYears(int years) const;
    QDateTime addSecs(int secs) const;
    QDateTime addMSecs(qint64 msecs) const;
    QDateTime toLocalTime() const;
    QDateTime toUTC() const;
    int daysTo(const QDateTime &) const;
    int secsTo(const QDateTime &) const;

    bool operator==(const QDateTime &other) const;
    bool operator!=(const QDateTime &other) const;
    bool operator<(const QDateTime &other) const;
    bool operator<=(const QDateTime &other) const;
    bool operator>(const QDateTime &other) const;
    bool operator>=(const QDateTime &other) const;

    static QDateTime currentDateTime();
    static QDateTime fromString(const QString &s, const QString &format);
    static QDateTime fromTime_t(uint secsSince1Jan1970UTC);
};

QDataStream &operator<<(QDataStream &, const QDate &);
QDataStream &operator>>(QDataStream &, QDate &);
QDataStream &operator<<(QDataStream &, const QTime &);
QDataStream &operator>>(QDataStream &, QTime &);
QDataStream &operator<<(QDataStream &, const QDateTime &);
QDataStream &operator>>(QDataStream &, QDateTime &);

#else // ifdef GCCXML_PARSE
#include <QDateTime>

#endif // ifdef GCCXML_PARSE

#endif // GCCXML_PARSE_QDATETIME_H
