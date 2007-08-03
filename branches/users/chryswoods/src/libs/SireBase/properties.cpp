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

#include "properties.h"

using namespace SireBase;

namespace SireBase
{

namespace detail
{

/** This is the data for a Properties object */
class PropertiesData : public QSharedData
{
public:
    PropertiesData();
    PropertiesData(const PropertiesData &other);

    ~PropertiesData();

    PropertiesData& operator=(const PropertiesData &other);

    void incrementVersion();

    /** The version number of this metadata */
    quint32 version;

    /** Shared pointer to the incremint used to increment the version
        of this set of properties */
    boost::shared_ptr<Incremint> incremint;

    /** The metadata for this set of properties itself */
    Properties metadata;

    /** The collection of properties, indexed by name */
    QHash<QString, Property> properties;

    /** The metadata for each property, indexed by name */
    QHash<QString, Properties> props_metadata;

    static const QSharedDataPointer<PropertiesData>& getNullData();

private:
    /** Constructor used only once to create the global-null PropertiesData */
    PropertiesData(bool) : version(0), incremint(new Incremint()),
                           metadata(false)
    {}
};

} // namespace detail

} // namespace SireBase

using namespace SireBase::detail;

static QSharedDataPointer<PropertiesData> nulldata_ptr;

/** Return a shared pointer to the null object - this is not
    as trivial as it sounds, as there is a circular reference! */
const QSharedDataPointer<PropertiesData>& PropertiesData::getNullData()
{
    if (nulldata_ptr.isNull())
    {
        //use a constructor where the circular reference is broken
        nulldata_ptr.reset( new PropertiesData(false) );

        //fix the reference
        nulldata_ptr->properties = Properties();
    }

    return nulldata_ptr;
}

/** Null constructor */
PropertiesData::PropertiesData()
               : version(0), incremint(new Incremint(0))
{}

/** Copy constructor */
PropertiesData::PropertiesData(const PropertiesData &other)
               : version(other.version), incremint(other.incremint),
                 metadata(other.metadata), properties(other.properties),
                 props_metadata(other.props_metadata)
{}

/** Destructor */
PropertiesData::~PropertiesData()
{}

/** Assignment operator */
PropertiesData& PropertiesData::operator=(const PropertiesData &other)
{
    if (this != &other)
    {
        version = other.version;
        incremint = other.incremint;
        metadata = other.metadata;
        properties = other.properties;
        props_metadata = other.props_metadata;
    }

    return *this;
}

/** Increment the version number of these properties */
void PropertiesData::incrementVersion()
{
    version = incremint->increment();
}

/////////////
///////////// Implementation of Properties
/////////////

/** Private constructor used to avoid the problem of the circular reference! */
Properties::Properties(bool) : d(0)
{}

/** Null constructor - construct an empty set of properties */
Properties::Properties() : d( PropertiesData::getNullData() )
{}

/** Copy constructor */
Properties::Properties(const Properties &other) : d(other.d)
{}

/** Destructor */
Properties::~Properties()
{}

/** Comparison operator */
bool Properties::operator==(const Properties &other) const
{
    return d == other.d or *d == *(other.d);
}

/** Comparison operator */
bool Properties::operator!=(const Properties &other) const
{
    return d != other.d and *d != *(other.d);
}

/** Return the version number of these properties */
quint32 Properties::version() const
{
    return d->version;
}

/** Return the keys for all of the properties in this set */
QStringList Properties::keys() const
{
    return d->properties.keys();
}

/** Return the property with name 'name'

    \throw SireBase::missing_property
*/
const Property& Properties::operator[](const QString &name) const
{
    QHash<QString,Property>::const_iterator it = d->properties.constFind(name);

    if (it == d->properties.constEnd())
    {
        throw SireBase::missing_property( QObject::tr(
            "There is no property with name \"%1\". Available properties are [ %2 ].")
                .arg(name, keys().join(", ")), CODELOC );
    }

    return *it;
}

/** Return the metadata associated with these properties */
const Properties& Properties::metadata() const
{
    return d->metadata;
}

/** Return the metadata for the property with key 'name'

    \throw SireBase::missing_property
*/
const Properties& Properties::metadata(const QString &name) const
{
    QHash<QString,Properties>::const_iterator it = d->props_metadata.constFind(name);

    if (it == d->props_metadata.constEnd())
    {
        throw SireBase::missing_property( QObject::tr(
            "There is no property with name \"%1\". Available properties are [ %2 ].")
                .arg(name, keys().join(", ")), CODELOC );
    }

    return *it;
}

/** Return the version number of the property with key 'key'

    \throw SireBase::missing_property
*/
quint32 Properties::version(const QString &key) const
{
    return metadata(key).version();
}

/** Return the property with name 'name'

    \throw SireBase::missing_property
*/
const Property& Properties::value(const QString &name) const
{
    return this->operator[](name);
}
