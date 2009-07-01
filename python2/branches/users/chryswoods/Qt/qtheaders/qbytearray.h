
#ifndef GCCXML_PARSE_QBYTEARRAY_H
#define GCCXML_PARSE_QBYTEARRAY_H

#ifdef GCCXML_PARSE

#include <string.h>
#include <stdarg.h>

class QBool;
class QDataStream;

template<class T>
class QList;

typedef unsigned int uint;
typedef unsigned short ushort;

class QByteArray
{
public:
    QByteArray();
    QByteArray(const char *);
    QByteArray(const char *, int size);
    QByteArray(int size, char c);
    QByteArray(const QByteArray &);
    ~QByteArray();

    QByteArray& operator=(const QByteArray &other);

    int size() const;
    bool isEmpty() const;
    void resize(int size);

    QByteArray &fill(char c, int size = -1);

    int capacity() const;
    void reserve(int size);
    void squeeze();

    const char *data() const;
    const char *constData() const;
    void detach();
    bool isDetached() const;
    void clear();

    char at(int i) const;
    char operator[](int i) const;
    char operator[](uint i) const;

    int indexOf(char c, int from = 0) const;
    int indexOf(const char *c, int from = 0) const;
    int indexOf(const QByteArray &a, int from = 0) const;
    int lastIndexOf(char c, int from = -1) const;
    int lastIndexOf(const char *c, int from = -1) const;
    int lastIndexOf(const QByteArray &a, int from = -1) const;

    QBool contains(char c) const;
    QBool contains(const char *a) const;
    QBool contains(const QByteArray &a) const;
    int count(char c) const;
    int count(const char *a) const;
    int count(const QByteArray &a) const;

    QByteArray left(int len) const;
    QByteArray right(int len) const;
    QByteArray mid(int index, int len = -1) const;

    bool startsWith(const QByteArray &a) const;
    bool startsWith(char c) const;
    bool startsWith(const char *c) const;

    bool endsWith(const QByteArray &a) const;
    bool endsWith(char c) const;
    bool endsWith(const char *c) const;

    void truncate(int pos);
    void chop(int n);

    QByteArray toLower() const;
    QByteArray toUpper() const;

    QByteArray trimmed() const;
    QByteArray simplified() const;
    QByteArray leftJustified(int width, char fill = ' ', bool truncate = false) const;
    QByteArray rightJustified(int width, char fill = ' ', bool truncate = false) const;

    QByteArray &prepend(char c);
    QByteArray &prepend(const char *s);
    QByteArray &prepend(const QByteArray &a);
    QByteArray &append(char c);
    QByteArray &append(const char *s);
    QByteArray &append(const QByteArray &a);
    QByteArray &insert(int i, char c);
    QByteArray &insert(int i, const char *s);
    QByteArray &insert(int i, const QByteArray &a);
    QByteArray &remove(int index, int len);
    QByteArray &replace(int index, int len, const char *s);
    QByteArray &replace(int index, int len, const QByteArray &s);
    QByteArray &replace(char before, const char *after);
    QByteArray &replace(char before, const QByteArray &after);
    QByteArray &replace(const char *before, const char *after);
    QByteArray &replace(const QByteArray &before, const QByteArray &after);
    QByteArray &replace(const QByteArray &before, const char *after);
    QByteArray &replace(const char *before, const QByteArray &after);
    QByteArray &replace(char before, char after);
    QByteArray &operator+=(char c);
    QByteArray &operator+=(const char *s);
    QByteArray &operator+=(const QByteArray &a);

    QList<QByteArray> split(char sep) const;

    QByteArray &setNum(short, int base = 10);
    QByteArray &setNum(ushort, int base = 10);
    QByteArray &setNum(int, int base = 10);
    QByteArray &setNum(uint, int base = 10);
    QByteArray &setNum(float, char f = 'g', int prec = 6);
    QByteArray &setNum(double, char f = 'g', int prec = 6);

    static QByteArray number(int, int base = 10);
    static QByteArray number(uint, int base = 10);
    static QByteArray number(double, char f = 'g', int prec = 6);
    static QByteArray fromRawData(const char *, int size);
    static QByteArray fromBase64(const QByteArray &base64);

    void push_back(char c);
    void push_back(const char *c);
    void push_back(const QByteArray &a);
    void push_front(char c);
    void push_front(const char *c);
    void push_front(const QByteArray &a);

    int count() const;
    int length() const;
    bool isNull() const;
};

QDataStream& operator<<(QDataStream&, const QByteArray&);
QDataStream& operator>>(QDataStream&, QByteArray&);

#else  // #ifdef GCCXML_PARSE
#include <QByteArray>

#endif // #ifdef GCCXML_PARSE

#endif // GCCXML_PARSE_QBYTEARRAY_H
