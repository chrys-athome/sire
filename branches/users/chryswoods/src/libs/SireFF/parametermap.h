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

#ifndef SIREFF_PARAMETERMAP_H
#define SIREFF_PARAMETERMAP_H

#include <QHash>
#include <QString>
#include <QList>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireFF
{
class ParameterSource;
class ParameterName;
class ParameterMap;
}

QDataStream& operator<<(QDataStream&, const SireFF::ParameterName&);
QDataStream& operator>>(QDataStream&, SireFF::ParameterName&);

QDataStream& operator<<(QDataStream&, const SireFF::ParameterSource&);
QDataStream& operator>>(QDataStream&, SireFF::ParameterSource&);

QDataStream& operator<<(QDataStream&, const SireFF::ParameterMap&);
QDataStream& operator>>(QDataStream&, SireFF::ParameterMap&);

uint qHash(const SireFF::ParameterName&);
uint qHash(const SireFF::ParameterSource&);

namespace SireFF
{

class ParameterSource;

/** This is a very simple class that holds the name of a type
    of parameter that is used by a forcefield, together with
    a default source property from which that parameter may
    be obtained.

    @author Christopher Woods
*/
class SIREFF_EXPORT ParameterName
{

friend QDataStream& ::operator<<(QDataStream&, const ParameterName&);
friend QDataStream& ::operator>>(QDataStream&, ParameterName&);

public:
    explicit ParameterName(const QString &name = QString::null);
    ParameterName(const QString &name, const QString &defaultsource);

    ParameterName(const ParameterName &other);

    ~ParameterName();

    bool operator==(const ParameterName &other) const;
    bool operator!=(const ParameterName &other) const;

    ParameterSource operator==(const QString &source) const;

    bool isValid() const;

    const QString& name() const;
    const QString& defaultSource() const;

private:
    /** The name of the parameter */
    QString param_name;

    /** The default source of the parameter */
    QString default_source;
};

/** This is a simple extension to ParameterName that allows the
    user to override the source property of the the parameter.

    ParameterName has an overloaded property that provides
    the only public way of creating valid ParameterSource objects,
    e.g

    \code
    ParameterSource src = ParameterName("coulomb") == "charges";

    //or more cleanly,
    cljff.add( mol, cljff.parameters().coulomb() == "charges" );

    \endcode

    \author Christopher Woods
*/
class SIREFF_EXPORT ParameterSource : public ParameterName
{

friend class ParameterName;

friend QDataStream& ::operator<<(QDataStream&, const ParameterSource&);
friend QDataStream& ::operator>>(QDataStream&, ParameterSource&);

public:
    ParameterSource();

    ParameterSource(const ParameterSource &other);

    ~ParameterSource();

    bool operator==(const ParameterSource &other) const;
    bool operator!=(const ParameterSource &other) const;

    bool isValid() const;
    bool isDefault() const;

    const QString& source() const;

protected:
    ParameterSource(const ParameterName &name, const QString &value);

private:
    /** The source of the parameter (property name) */
    QString param_source;
};

/** This is a simple class that holds all of the mappings from
    parameters to sources. This holds only non-default source
    locations. This object is designed to be created implicitly
    by combining together ParameterSource objects, e.g.

    \code
    cljff.add( mol, cljff.parameters().coulomb() == "charges" and
                    cljff.parameters().lj() == "ljs" );
    \endcode

    @author Christopher Woods
*/
class SIREFF_EXPORT ParameterMap
{

friend QDataStream& ::operator<<(QDataStream&, const ParameterMap&);
friend QDataStream& ::operator>>(QDataStream&, ParameterMap&);

public:
    ParameterMap();

    ParameterMap(const ParameterSource &source);
    ParameterMap(const QList<ParameterSource> &sources);

    ParameterMap(const QHash<ParameterName,QString> &mapping);

    ParameterMap(const ParameterMap &other);

    ~ParameterMap();

    ParameterMap& operator=(const ParameterMap &other);

    ParameterMap operator&&(const ParameterMap &other) const;

    bool operator==(const ParameterMap &other) const;
    bool operator!=(const ParameterMap &other) const;

    QString source(const ParameterName &param) const;

private:
    void add(const ParameterSource &source);

    /** Hash mapping the name of one type of
        parameters to the property containing
        those parameters */
    QHash<QString,QString> map;
};

/** Comparison operator */
inline bool ParameterMap::operator==(const ParameterMap &other) const
{
    return map == other.map;
}

/** Comparison operator */
inline bool ParameterMap::operator!=(const ParameterMap &other) const
{
    return map != other.map;
}

}

inline uint qHash(const SireFF::ParameterSource &source)
{
    return qHash(source.name());
}

inline uint qHash(const SireFF::ParameterName &source)
{
    return qHash(source.name());
}

Q_DECLARE_METATYPE(SireFF::ParameterName);
Q_DECLARE_METATYPE(SireFF::ParameterSource);
Q_DECLARE_METATYPE(SireFF::ParameterMap);

SIRE_END_HEADER

#endif
