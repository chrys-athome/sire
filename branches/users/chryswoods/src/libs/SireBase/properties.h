/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2007  Christopher Woods
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

#ifndef SIREBASE_PROPERTIES_H
#define SIREBASE_PROPERTIES_H

#include "property.h"

#include <QSharedDataPointer>

SIRE_BEGIN_HEADER

namespace SireBase
{
class Properties;
}

QDataStream& operator<<(QDataStream&, const SireBase::Properties&);
QDataStream& operator>>(QDataStream&, SireBase::Properties&);

XMLStream& operator<<(XMLStream&, const SireBase::Properties&);
XMLStream& operator>>(XMLStream&, SireBase::Properties&);

QTextStream& operator<<(QTextStream&, const SireBase::Properties&);

namespace SireBase
{

namespace detail
{
class PropertiesData;
}

/** This class holds a collection of properties, indexed by name.
    Each property comes complete with a set of metadata, which
    also includes a version number for each property within
    this set. The metadata is actually another Properties object,
    and indeed Properties can itself be a Property,
    so allowing Properties to be nested indefinitely.

    @author Christopher Woods
*/
class SIREBASE_EXPORT Properties : public ConcreteProperty<Properties,PropertyBase>
{

friend QDataStream& ::operator<<(QDataStream&, const Properties&);
friend QDataStream& ::operator>>(QDataStream&, Properties&);

friend XMLStream& ::operator<<(XMLStream&, const Properties&);
friend XMLStream& ::operator>>(XMLStream&, Properties&);

friend class detail::PropertiesData; // so can call private constructor

public:
    Properties();

    Properties(const Properties &other);

    ~Properties();

    Properties& operator=(const Properties &other) const;

    bool operator==(const Properties &other) const;
    bool operator!=(const Properties &other) const;

    static const char* typeName()
    {
        return "SireBase::Properties";
    }

    quint32 version() const;
    quint32 version(const QString &key) const;

    const Property& operator[](const QString &key) const;

    const Properties& metadata() const;
    const Properties& metadata(const QString &key) const;

    const Property& value(const QString &key) const;

    QStringList keys() const;

    void addProperty(const QString &key, const Property &value);
    void addProperty(const QString &key, const Property &value,
                     const Properties &metadata);

    void setProperty(const QString &key, const Property &value,
                     bool clear_metadata=false);

    void setMetadata(const QString &key, const QString &metakey,
                     const Property &metavalue);

    void setMetadata(const QString &key, const Properties &metadata);

    void addMetadata(const QString &key, const Properties &metadata);

    void remove(const QString &key);

    void remoteMetaData(const QString &key);
    void removeMetaData(const QString &key, const QString &metakey);

    void clear();

private:
    Properties(bool);

    /** Implicitly shared pointer to the data of this object */
    QSharedDataPointer<detail::PropertiesData> d;
};

}

Q_DECLARE_METATYPE(SireBase::Properties);

SIRE_END_HEADER

#endif
