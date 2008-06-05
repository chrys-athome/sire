/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
  *
  *  This program is free software; you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation; either version 2 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program; if not, write to the Free Software
  *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  *  For full details of the license please see the COPYING file
  *  that should have come with this distribution.
  *
  *  You can contact the authors via the developer's mailing list
  *  at http://siremol.org
  *
\*********************************************/

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

    explicit IDBase(quint32 id) : _id(id)
    {}

    IDBase(const IDBase &other) : _id(other._id)
    {}

    ~IDBase()
    {}

    IDBase& operator=(quint32 val)
    {
        _id = val;
        return *this;
    }

    IDBase& operator++()
    {
        ++ _id;
        return *this;
    }

    IDBase& operator+=(quint32 val)
    {
        _id += val;
        return *this;
    }

    IDBase& operator--()
    {
        -- _id;
        return *this;
    }

    IDBase& operator-=(quint32 val)
    {
        _id -= val;
        return *this;
    }

    quint32 toInt() const
    {
        return _id;
    }

    QString toString() const
    {
        return QString::number(_id);
    }

    operator quint32() const
    {
        return _id;
    }

protected:
    /** The ID value */
    quint32 _id;
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
