/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2008  Christopher Woods
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

#include "ffcomponent.h"

#include "SireError/errors.h"

#include <QRegExp>

using namespace SireFF;
using namespace SireCAS;

static QRegExp uid_regexp( "E_{FF:(\\d+)}" );
static QRegExp name_regexp( "E_{FF:\\d+}^{(*)}" );

/** Constructor */
FFComponent::FFComponent(quint64 ffuid, const QLatin1String &name)
            : Symbol( FFComponent::symbolName(ffuid, name) )
{}

/** Copy constructor */
FFComponent::FFComponent(const FFComponent &other)
            : Symbol(other)
{}

/** Destructor */
FFComponent::~FFComponent()
{}

/** Return the unique ID of the potential energy surface
    to which this component belongs */
quint64 FFComponent::UID() const
{
    QRegExp local_copy = uid_regexp;
    
    if (local_copy.indexIn( Symbol::toString() ) == -1)
        //we could not interpret the name!
        throw SireError::program_bug( QObject::tr(
            "Could not interpret this symbol (%1) as an FFComponent.")
                .arg(Symbol::toString()), CODELOC );
                
    return local_copy.cap(1).toULong();
}

/** Return the name of the component of the potential energy
    surface that this symbol represents */
QString FFComponent::name() const
{
    QRegExp local_copy = name_regexp;
    
    if (name_regexp.indexIn( Symbol::toString() ) == -1)
        //we could not interpret the name!
        throw SireError::program_bug( QObject::tr(
            "Could not interpret this symbol (%1) as an FFComponent.")
                .arg(Symbol::toString()), CODELOC );

    return local_copy.cap(1);
}
