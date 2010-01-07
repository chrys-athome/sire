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

#include "values.h"
#include "symbol.h"
#include "complexvalues.h"

#include "SireMaths/complex.h"
#include "SireMaths/errors.h"

#include "Siren/stream.h"
#include "Siren/streamqt.h"

using namespace Siren;
using namespace SireMaths;
using namespace SireCAS;

static const RegisterObject<Values> r_values;

/** Construct an empty set of values */
Values::Values() : Implements<Values,Object>()
{}

/** Construct just setting 'symbol' equal to 'value' */
Values::Values(const Symbol &symbol, double value)
       : Implements<Values,Object>()
{
    vals.reserve(1);
    vals.insert(symbol.ID(), value);
}

/** Construct from a hash of values indexed by symbols */
Values::Values(const QHash<Symbol,double> &values)
       : Implements<Values,Object>()
{
    for (QHash<Symbol,double>::const_iterator it = values.begin();
         it != values.end();
         ++it)
    {
        vals.insert( it.key().ID(), it.value() );
    }
}

/** Construct from the passed complex values

    \throw SireMaths::domain_error
*/
Values::Values(const ComplexValues &other)
       : Implements<Values,Object>()
{
    vals.reserve(other.values().count());

    for (QHash<SymbolID,Complex>::const_iterator it = other.values().constBegin();
         it != other.values().constEnd();
         ++it)
    {
        if (not it->isReal())
            throw SireMaths::domain_error( QObject::tr(
                "Cannot construct a 'Values' object from a 'ComplexValues' "
                "object as some of the values are not real (e.g. %1).\n"
                "%2")
                    .arg(it->toString())
                    .arg(other.toString()), CODELOC );
    
        vals.insert( it.key(), it->real() );
    }
}

/** Copy constructor */
Values::Values(const Values &other)
       : Implements<Values,Object>(other), vals(other.vals)
{}

Values& Values::operator=(const Values &other)
{
    vals = other.vals;
    Object::operator=(other);
    return *this;
}

/** Comparison operator */
bool Values::operator==(const Values &other) const
{
    return vals == other.vals;
}

/** Comparison operator */
bool Values::operator!=(const Values &other) const
{
    return vals != other.vals;
}

/** Return a string representation of these values */
QString Values::toString() const
{
    QStringList words;
    QStringList lines;
    
    QList<Symbol> syms = this->symbols();
    
    qSort(syms);
    
    foreach (const Symbol &sym, syms)
    {
        words.append( QString("%1 == %2").arg(sym.toString())
                                         .arg(this->value(sym)) );

        if (words.count() == 4)
        {
            lines.append( words.join(", ") );
            words.clear();
        }
    }
    
    if (not words.isEmpty())
    {
        lines.append( words.join(", ") );
    }
    
    return QString("{ %1 }").arg( lines.join("\n  ") );
}

/** Return a list of the symbols that are present in this set */
QList<Symbol> Values::symbols() const
{
    QList<Symbol> s;
    
    for (QHash<SymbolID,double>::const_iterator it = vals.constBegin();
         it != vals.constEnd();
         ++it)
    {
        s.append( Symbol(it.key()) );
    }

    return s;
}

/** Return a list of the symbols that are present in this set */
QList<Symbol> Values::keys() const
{
    return this->symbols();
}

/** Destructor */
Values::~Values()
{}

uint Values::hashCode() const
{
    return qHash( Values::typeName() ) + qHash( vals.count() );
}

void Values::stream(Stream &s)
{
    s.assertVersion<Values>(1);
    
    Schema schema = s.item<Values>();
    
    schema.data("values") & vals;
    
    Object::stream( schema.base() );
}

/** Return the value of the Symbol with ID 'id', or 0.0 if there is no such symbol */
double Values::value(const Symbol &sym) const
{
    return vals.value(sym.ID(),0.0);
}

/** Return the value of the Symbol with ID 'id', or 0.0 if there is no such symbol */
double Values::operator[](const Symbol &sym) const
{
    return this->value(sym);
}

/** Return the value of the Symbol with ID 'id', or 0.0 if there is no such symbol */
double Values::operator()(const Symbol &sym) const
{
    return this->value(sym);
}

/** Add the contents of 'other' to this set - this overwrites any
    existing values that are also in 'other' */
Values Values::operator+(const Values &other) const
{
    if (other.vals.isEmpty())
        return *this;
    else if (vals.isEmpty())
    {
        return other;
    }
    else
    {
        Values ret;
    
        ret.vals = vals;
        ret.vals.reserve( vals.count() + other.vals.count() );

        for (QHash<SymbolID,double>::const_iterator it = other.vals.begin();
             it != other.vals.end();
             ++it)
        {
            ret.vals.insert( it.key(), it.value() );
        }

        return ret;
    }
}

/** Return whether or not this set of values is empty */
bool Values::isEmpty() const
{
    return vals.isEmpty();
}

/** Return the number of specified values in this set */
int Values::count() const
{
    return vals.count();
}

/** Return the hash mapping the symbol ID to a value */
const QHash<SymbolID,double>& Values::values() const
{
    return vals;
}

/** Return whether or not a value for the symbol 'symbol' has been set */
bool Values::contains(const Symbol &symbol) const
{
    return vals.contains(symbol.ID());
}
