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

#include "parametermap.h"

#include "SireStream/datastream.h"

using namespace SireFF;
using namespace SireStream;

////////////
//////////// Implementation of ParameterName
////////////

static const RegisterMetaType<ParameterName> r_paramname;

/** Serialise to a binary datastream */
QDataStream SIREFF_EXPORT &operator<<(QDataStream &ds, const ParameterName &paramname)
{
    writeHeader(ds, r_paramname, 1) << paramname.param_name
                                    << paramname.default_source;

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREFF_EXPORT &operator>>(QDataStream &ds, ParameterName &paramname)
{
    VersionID v = readHeader(ds, r_paramname);

    if (v == 1)
    {
        ds >> paramname.param_name >> paramname.default_source;
    }
    else
        throw version_error(v, "1", r_paramname, CODELOC);

    return ds;
}

/** Construct a ParameterName whose default source is the same as the
    name of the parameter */
ParameterName::ParameterName(const QString &name)
              : param_name(name), default_source(name)
{}

/** Construct a ParameterName with a specified name and default source */
ParameterName::ParameterName(const QString &name, const QString &defaultsource)
              : param_name(name), default_source(defaultsource)
{}

/** Copy constructor */
ParameterName::ParameterName(const ParameterName &other)
              : param_name(other.param_name),
                default_source(other.default_source)
{}

/** Destructor */
ParameterName::~ParameterName()
{}

/** Comparison operator */
bool ParameterName::operator==(const ParameterName &other) const
{
    return param_name == other.param_name and default_source == other.default_source;
}

/** Comparison operator */
bool ParameterName::operator!=(const ParameterName &other) const
{
    return param_name != other.param_name or default_source != other.default_source;
}

/** Set the source of the parameter - return the ParameterSource with
    the required information */
ParameterSource ParameterName::operator==(const QString &source) const
{
    return ParameterSource(*this, source);
}

/** Return whether this is valid */
bool ParameterName::isValid() const
{
    return not (param_name.isEmpty() or default_source.isEmpty());
}

/** Return the name of the parameter */
const QString& ParameterName::name() const
{
    return param_name;
}

/** Return the default source of this parameter */
const QString& ParameterName::defaultSource() const
{
    return default_source;
}

////////////
//////////// Implementation of ParameterSource
////////////

static const RegisterMetaType<ParameterSource> r_paramsource;

/** Serialise to a binary datastream */
QDataStream SIREFF_EXPORT &operator<<(QDataStream &ds, const ParameterSource &paramsource)
{
    writeHeader(ds, r_paramsource, 1)
          << static_cast<const ParameterName&>(paramsource)
          << paramsource.param_source;

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREFF_EXPORT &operator>>(QDataStream &ds, ParameterSource &paramsource)
{
    VersionID v = readHeader(ds, r_paramsource);

    if (v == 1)
    {
        ds >> static_cast<ParameterName&>(paramsource)
           >> paramsource.param_source;
    }
    else
        throw version_error(v, "1", r_paramsource, CODELOC);

    return ds;
}

/** Constructor */
ParameterSource::ParameterSource() : ParameterName()
{}

/** Constructor used by ParameterName to associate a parameter with
    a source */
ParameterSource::ParameterSource(const ParameterName &name,
                                 const QString &source)
                : ParameterName(name), param_source(source)
{}

/** Copy constructor */
ParameterSource::ParameterSource(const ParameterSource &other)
                : ParameterName(other), param_source(other.param_source)
{}

/** Destructor */
ParameterSource::~ParameterSource()
{}

/** Comparison operator */
bool ParameterSource::operator==(const ParameterSource &other) const
{
    return ParameterName::operator==(other) and param_source == other.param_source;
}

/** Comparison operator */
bool ParameterSource::operator!=(const ParameterSource &other) const
{
    return ParameterName::operator!=(other) or param_source != other.param_source;
}

/** Return whether or not this contains any valid data */
bool ParameterSource::isValid() const
{
    return ParameterName::isValid() and not param_source.isEmpty();
}

/** Return whether this is, in fact, equal to the default
    value of the parameter */
bool ParameterSource::isDefault() const
{
    return param_source == this->defaultSource();
}

/** Return the source of the parameter */
const QString& ParameterSource::source() const
{
    return param_source;
}

////////////
//////////// Implementation of ParameterMap
////////////

static const RegisterMetaType<ParameterMap> r_parammap;

/** Serialise to a binary datastream */
QDataStream SIREFF_EXPORT &operator<<(QDataStream &ds, const ParameterMap &parammap)
{
    writeHeader(ds, r_parammap, 1) << parammap.map;
    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREFF_EXPORT &operator>>(QDataStream &ds, ParameterMap &parammap)
{
    VersionID v = readHeader(ds, r_parammap);
    if (v == 1)
    {
        ds >> parammap.map;
    }
    else
        throw version_error(v, "1", r_parammap, CODELOC);

    return ds;
}

/** Constructor */
ParameterMap::ParameterMap()
{}

/** Add the parameter source to the list - this does not
    add invalid or default sources */
void ParameterMap::add(const ParameterSource &source)
{
    if (source.isValid() and not source.isDefault())
        map.insert( source.name(), source.source() );
}

/** Construct from the passed ParameterSource */
ParameterMap::ParameterMap(const ParameterSource &source)
{
    add(source);
}

/** Construct from a list of sources */
ParameterMap::ParameterMap(const QList<ParameterSource> &sources)
{
    for (QList<ParameterSource>::const_iterator it = sources.begin();
         it != sources.end();
         ++it)
    {
        add(*it);
    }
}

/** Copy constructor */
ParameterMap::ParameterMap(const ParameterMap &other) : map(other.map)
{}

/** Destructor */
ParameterMap::~ParameterMap()
{}

/** Assignment operator */
ParameterMap& ParameterMap::operator=(const ParameterMap &other)
{
    map = other.map;
    return *this;
}

/** "and" combine two maps together - this unites the two maps,
    overwriting the parameters in this map with the values of 'other' */
ParameterMap ParameterMap::operator&&(const ParameterMap &other) const
{
    ParameterMap retval;

    if (map.isEmpty())
    {
        retval.map = other.map;
    }
    else if (other.map.isEmpty())
    {
        retval.map = map;
    }
    else
    {
        retval.map = map;

        for (QHash<QString,QString>::const_iterator it = other.map.begin();
             it != other.map.end();
             ++it)
        {
            retval.map.insert( it.key(), it.value() );
        }
    }

    return retval;
}

/** Return the source of the parameter 'param' - this returns the
    default source if none other has been specified */
QString ParameterMap::source(const ParameterName &param) const
{
    return map.value( param.name(), param.defaultSource() );
}
