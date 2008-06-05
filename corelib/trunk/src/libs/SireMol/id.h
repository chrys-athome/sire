#ifndef SIREMOL_ID_H
#define SIREMOL_ID_H

#include <QString>
#include <QDataStream>
#include <QDebug>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class IDBase;

template<class T>
class ID;
}

QDataStream& operator<<(QDataStream&, const SireMol::IDBase&);
QDataStream& operator>>(QDataStream&, SireMol::IDBase&);

uint qHash(const SireMol::IDBase&);

namespace SireMol
{

class IDBase
{

friend QDataStream& ::operator<<(QDataStream&, const IDBase&);
friend QDataStream& ::operator>>(QDataStream&, IDBase&);

friend unsigned int ::qHash(const IDBase&);

public:
    IDBase() : _id(0)
    {}
    
    IDBase(quint32 id) : _id(id)
    {}
    
    IDBase(const IDBase &other) : _id(other._id)
    {}
    
    ~IDBase()
    {}

    quint32 index() const
    {
        return _id;
    }
    
    quint32 value() const
    {
        return _id;
    }
    
    QString toString() const
    {
        return QString::number(_id);
    }
    
protected:
    /** The ID value */
    quint32 _id;
};

template<class T>
class ID : public IDBase
{

public:
    ID() : IDBase()
    {}
    
    ID(quint32 id) : IDBase(id)
    {}
    
    ID(const ID &other) : IDBase(other)
    {}
    
    ~ID()
    {}
    
    T& operator=(const T &other)
    {
        _id = other._id;
        return static_cast<T&>(*this);
    }
    
    T& operator=(quint32 id)
    {
        _id = id;
        return static_cast<T&>(*this);
    }
    
    bool operator==(const T &other) const
    {
        return _id == other._id;
    }
    
    bool operator!=(const T &other) const
    {
        return _id != other._id;
    }
    
    bool operator<(const T &other) const
    {
        return _id < other._id;
    }
    
    bool operator<=(const T &other) const
    {
        return _id <= other._id;
    }
    
    bool operator>(const T &other) const
    {
        return _id > other._id;
    }
    
    bool operator>=(const T &other) const
    {
        return _id >= other._id;
    }
    
    T& operator++()
    {
        ++ _id;
        return static_cast<T&>(*this);
    }
    
    T operator++(int)
    {
        T ret;
        ret._id = _id + 1;
        return ret;
    }
    
    T& operator+=(quint32 val)
    {
        _id += val;
        return static_cast<T&>(*this);
    }
    
    T& operator+=(T val)
    {
        _id += val._id;
        return static_cast<T&>(*this);
    }
    
    T operator+(quint32 val) const
    {
        T ret;
        ret._id = _id + val;
        return ret;
    }
    
    T operator+(T val) const
    {
        T ret;
        ret._id = _id + val;
        return ret;
    }
    
    T& operator--()
    {
        -- _id;
        return static_cast<T&>(*this);
    }
    
    T operator--(int)
    {
        T ret;
        ret._id = _id - 1;
        return ret;
    }
    
    T& operator-=(quint32 val)
    {
        _id -= val;
        return static_cast<T&>(*this);
    }
    
    T& operator-=(T val)
    {
        _id -= val._id;
        return static_cast<T&>(*this);
    }
    
    T operator-(quint32 val) const
    {
        T ret;
        ret._id = _id - val;
        return ret;
    }
    
    T operator-(T val) const
    {
        T ret;
        ret._id = _id - val;
        return ret;
    }
};

}

/** Hash this ID type */
inline uint qHash(const SireMol::IDBase &id)
{
    return id._id;
}

/** Serialise the ID type to a datastream */
inline QDataStream& operator<<(QDataStream &ds, const SireMol::IDBase &id)
{
    ds << id._id;
    
    return ds;
}

/** Deserialise the ID type from a datastream */
inline QDataStream& operator>>(QDataStream &ds, SireMol::IDBase &id)
{
    ds >> id._id;
    return ds;
}

SIRE_END_HEADER

#endif
