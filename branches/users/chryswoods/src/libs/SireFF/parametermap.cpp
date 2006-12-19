
#include "parametermap.h"

using namespace SireFF;

////////////
//////////// Implementation of ParameterName
////////////

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
