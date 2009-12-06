/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2009  Christopher Woods
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

#include "linktoproperty.h"

#include "Siren/stream.h"

using namespace SireID;
using namespace SireBase;
using namespace Siren;

static const RegisterObject<LinkToProperty> r_link;

/** Constructor */
LinkToProperty::LinkToProperty() : Implements<LinkToProperty,Object>()
{}

/** Construct to link to the property 'source' */
LinkToProperty::LinkToProperty(const PropertyName &source)
               : Implements<LinkToProperty,Object>(),
                 target_source(source)
{}

/** Construct to link to the property 'source' in the objects
    identified by 'filter' */
LinkToProperty::LinkToProperty(const PropertyName &source, const ID &filter)
               : Implements<LinkToProperty,Object>(),
                 target_source(source), id_filter(filter)
{}

/** Copy constructor */
LinkToProperty::LinkToProperty(const LinkToProperty &other)
               : Implements<LinkToProperty,Object>(other),
                 target_source(other.target_source), id_filter(other.id_filter)
{}

/** Destructor */
LinkToProperty::~LinkToProperty()
{}

/** Copy assignment operator */
LinkToProperty& LinkToProperty::operator=(const LinkToProperty &other)
{
    if (this != &other)
    {
        target_source = other.target_source;
        id_filter = other.id_filter;
    }
    
    return *this;
}

/** Comparison operator */
bool LinkToProperty::operator==(const LinkToProperty &other) const
{
    return target_source == other.target_source and
           id_filter == other.id_filter;
}

/** Comparison operator */
bool LinkToProperty::operator!=(const LinkToProperty &other) const
{
    return not LinkToProperty::operator==(other);
}

/** Return a string representation of this link */
QString LinkToProperty::toString() const
{
    if (target_source.hasSource())
        return QObject::tr("LinkTo( %1 )").arg(target_source.source());
     
    else if (target_source.hasValue())
        return QObject::tr("LinkTo( %1 )").arg(target_source.value().toString());

    else
        return QObject::tr("LinkTo( NULL )");
}

uint LinkToProperty::hashCode() const
{
    return qHash( LinkToProperty::typeName() ) + qHash(target_source);
}

void LinkToProperty::stream(Stream &s)
{
    s.assertVersion<LinkToProperty>(1);
    
    Schema schema = s.item<LinkToProperty>();
    
    schema.data("target_source") & target_source;
    schema.data("id_filter") & id_filter;
    
    Object::stream( schema.base() );
}

/** Return the target of this link */
const PropertyName& LinkToProperty::target() const
{
    return target_source;
}

/** Return any filter for this link (this is null if there is no filter) */
const SireID::ID& LinkToProperty::filter() const
{
    return id_filter.base();
}

/** Return whether or not this link is filtered */
bool LinkToProperty::isFiltered() const
{
    return not id_filter.isNull();
}
