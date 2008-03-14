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
using namespace SireFF::detail;

using namespace SireCAS;

static QRegExp uid_regexp( "E_\\{FF:(\\d+)\\}" );
static QRegExp name_regexp( "E_{FF:\\d+}\\^\\{(.+)\\}" );

static QRegExp all_regexp( "E_\\{FF:(\\d+)\\}\\^\\{(._)\\}" );

/** Constructor */
FFComponent::FFComponent(quint64 ffuid, const QLatin1String &name)
            : Symbol( FFComponent::symbolName(ffuid, name) )
{}

/** Construct from a passed symbol 

    \throw SireError::incompatible_error
*/
FFComponent::FFComponent(const Symbol &symbol, const QLatin1String &name)
            : Symbol(symbol)
{
    QRegExp local_copy = all_regexp;
    
    if (local_copy.indexIn( Symbol::toString() ) == -1)
        //we could not interpret the symbol
        throw SireError::incompatible_error( QObject::tr(
            "The symbol \"%1\" is not a valid FFComponent symbol. "
            "FFComponent symbols have the form E_{FF:<UID>}^{<NAME>} "
            "where <UID> is the UID of the potential energy function "
            "and <NAME> is the name of the component on that surface. ")
                .arg(symbol.toString()), CODELOC );
                
    bool ok;
    local_copy.cap(1).toULong(&ok);
    
    if (not ok)
        throw SireError::incompatible_error( QObject::tr(
            "Could not interpret the UID of the potential energy "
            "surface from \"%1\" (from the symbol \"%2\")")
                .arg(local_copy.cap(1), symbol.toString()), CODELOC );
                
    if (name != local_copy.cap(2))
        throw SireError::incompatible_error( QObject::tr(
            "The symbol \"%1\" is for the wrong component! (%2). "
            "The correct component is called \"%3\".")
                .arg(symbol.toString(), local_copy.cap(2), name),
                    CODELOC );
}

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
