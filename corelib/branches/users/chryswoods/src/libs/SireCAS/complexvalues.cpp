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

#include "complexvalues.h"
#include "symbol.h"

#include "SireMaths/complex.h"

#include "Siren/tostring.h"
#include "Siren/stream.h"
#include "Siren/streamqt.h"

using namespace Siren;
using namespace SireMaths;
using namespace SireCAS;

static const RegisterObject<ComplexValues> r_complexvals;

/** Construct an empty set of values */
ComplexValues::ComplexValues() : Implements<ComplexValues,Object>()
{}

/** Construct just setting 'symbol' equal to 'value' */
ComplexValues::ComplexValues(const Symbol &symbol, const Complex &value)
              : Implements<ComplexValues,Object>()
{
    vals.reserve(1);
    vals.insert(symbol.ID(), value);
}

/** Copy constructor */
ComplexValues::ComplexValues(const ComplexValues &other) 
              : Implements<ComplexValues,Object>(other), vals(other.vals)
{}

/** Construct from a hash of values indexed by symbol */
ComplexValues::ComplexValues(const QHash<Symbol,Complex> &values)
              : Implements<ComplexValues,Object>()
{
    for (QHash<Symbol,Complex>::const_iterator it = values.begin();
         it != values.end();
         ++it)
    {
        vals.insert(it.key().ID(), it.value());
    }
}

/** Construct from Values */
ComplexValues::ComplexValues(const Values &other)
              : Implements<ComplexValues,Object>()
{
    for (QHash<SymbolID,double>::const_iterator it = other.values().begin();
         it != other.values().end();
         ++it)
    {
        vals.insert(it.key(), it.value());
    }
}

/** Destructor */
ComplexValues::~ComplexValues()
{}

ComplexValues& ComplexValues::operator=(const ComplexValues &other)
{
    vals = other.vals;
    return *this;
}

bool ComplexValues::operator==(const ComplexValues &other) const
{
    return vals == other.vals;
}

bool ComplexValues::operator!=(const ComplexValues &other) const
{
    return vals != other.vals;
}

uint ComplexValues::hashCode() const
{
    return qHash(ComplexValues::typeName()) + vals.count();
}

void ComplexValues::stream(Siren::Stream &s)
{
    s.assertVersion<ComplexValues>(1);
    
    Schema schema = s.item<ComplexValues>();
    
    schema.data("values") & vals;
    
    Object::stream( schema.base() );
}

/** Return a list of the symbols that are present in this set */
QList<Symbol> ComplexValues::symbols() const
{
    QList<Symbol> s;
    
    for (QHash<SymbolID,Complex>::const_iterator it = vals.constBegin();
         it != vals.constEnd();
         ++it)
    {
        s.append( Symbol(it.key()) );
    }

    return s;
}

/** Return a list of the symbols that are present in this set */
QList<Symbol> ComplexValues::keys() const
{
    return this->symbols();
}

/** Return a string representation of these values */
QString ComplexValues::toString() const
{
    QStringList words;
    QStringList lines;
    
    QList<Symbol> syms = this->symbols();
    
    qSort(syms);
    
    foreach (const Symbol &sym, syms)
    {
        words.append( QString("%1 == %2").arg(sym.toString())
                                         .arg(this->value(sym).toString()) );

        if (words.count() == 4)
        {
            lines.append( words.join(", ") );
            words.clear();
        }
    }
    
    return QString("{ %1 }").arg( lines.join("\n  ") );
}

/** Return the value of the Symbol with ID 'id', or 0.0 if there is no such symbol */
Complex ComplexValues::value(const Symbol &sym) const
{
    return vals.value(sym.ID(),Complex(0));
}

/** Add the contents of 'other' to this set - this overwrites any
    existing values that are also in 'other' */
ComplexValues ComplexValues::operator+(const ComplexValues &other) const
{
    if (other.vals.isEmpty())
        return *this;
    else if (vals.isEmpty())
    {
        return other;
    }
    else
    {
        ComplexValues ret;
    
        ret.vals = vals;
        ret.vals.reserve( vals.count() + other.vals.count() );

        for (QHash<SymbolID,Complex>::const_iterator it = other.vals.begin();
             it != other.vals.end();
             ++it)
        {
            ret.vals.insert( it.key(), it.value() );
        }

        return ret;
    }
}

/** Return the hash mapping Symbol IDs to complex values */
const QHash<SymbolID,Complex>& ComplexValues::values() const
{
    return vals;
}
