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

#include "Siren/stream.h"
#include "Siren/streamqt.h"

using namespace Siren;
using namespace SireCAS;

static const RegisterObject<Values> r_values;

/** Construct an empty set of values */
Values::Values() : Implements<Values,Object>()
{}

/** Construct from a list of values */
Values::Values(const QList<SymbolValue> &values)
       : Implements<Values,Object>()
{
    for (QList<SymbolValue>::const_iterator it = values.begin();
         it != values.end();
         ++it)
    {
        add(*it);
    }
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

/** Copy constructor */
Values::Values(const Values &other)
       : Implements<Values,Object>(other), vals(other.vals)
{}

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

/** Construct from the passed values */
Values::Values(const SymbolValue &val0)
       : Implements<Values,Object>()
{
    add(val0);
}

/** Add the passed values */
void Values::add(const SymbolValue &val0, const SymbolValue &val1)
{
    add(val0);
    add(val1);
}

/** Add the passed values */
void Values::add(const SymbolValue &val0, const SymbolValue &val1, const SymbolValue &val2)
{
    add(val0);
    add(val1);
    add(val2);
}

/** Add the passed values */
void Values::add(const SymbolValue &val0, const SymbolValue &val1, const SymbolValue &val2,
                 const SymbolValue &val3)
{
    add(val0);
    add(val1);
    add(val2);
    add(val3);
}

/** Add the passed values */
void Values::add(const SymbolValue &val0, const SymbolValue &val1, const SymbolValue &val2,
                 const SymbolValue &val3, const SymbolValue &val4)
{
    add(val0);
    add(val1);
    add(val2);
    add(val3);
    add(val4);
}

/** Add the passed values */
void Values::add(const SymbolValue &val0, const SymbolValue &val1, const SymbolValue &val2,
                 const SymbolValue &val3, const SymbolValue &val4, const SymbolValue &val5)
{
    add(val0);
    add(val1);
    add(val2);
    add(val3);
    add(val4);
    add(val5);
}

/** Add the passed values */
void Values::add(const SymbolValue &val0, const SymbolValue &val1, const SymbolValue &val2,
                 const SymbolValue &val3, const SymbolValue &val4, const SymbolValue &val5,
                 const SymbolValue &val6)
{
    add(val0);
    add(val1);
    add(val2);
    add(val3);
    add(val4);
    add(val5);
    add(val6);
}

/** Add the passed values */
void Values::add(const SymbolValue &val0, const SymbolValue &val1, const SymbolValue &val2,
                 const SymbolValue &val3, const SymbolValue &val4, const SymbolValue &val5,
                 const SymbolValue &val6, const SymbolValue &val7)
{
    add(val0);
    add(val1);
    add(val2);
    add(val3);
    add(val4);
    add(val5);
    add(val6);
    add(val7);
}

/** Add the passed values */
void Values::add(const SymbolValue &val0, const SymbolValue &val1, const SymbolValue &val2,
                 const SymbolValue &val3, const SymbolValue &val4, const SymbolValue &val5,
                 const SymbolValue &val6, const SymbolValue &val7, const SymbolValue &val8)
{
    add(val0);
    add(val1);
    add(val2);
    add(val3);
    add(val4);
    add(val5);
    add(val6);
    add(val7);
    add(val8);
}

/** Add the passed values */
void Values::add(const SymbolValue &val0, const SymbolValue &val1, const SymbolValue &val2,
                 const SymbolValue &val3, const SymbolValue &val4, const SymbolValue &val5,
                 const SymbolValue &val6, const SymbolValue &val7, const SymbolValue &val8,
                 const SymbolValue &val9)
{
    add(val0);
    add(val1);
    add(val2);
    add(val3);
    add(val4);
    add(val5);
    add(val6);
    add(val7);
    add(val8);
    add(val9);
}

/** Construct from the passed values */
Values::Values(const SymbolValue &val0, const SymbolValue &val1)
       : Implements<Values,Object>()
{
    add(val0,val1);
}

/** Construct from the passed values */
Values::Values(const SymbolValue &val0, const SymbolValue &val1, const SymbolValue &val2)
       : Implements<Values,Object>()
{
    add(val0,val1,val2);
}

/** Construct from the passed values */
Values::Values(const SymbolValue &val0, const SymbolValue &val1, const SymbolValue &val2,
               const SymbolValue &val3)
       : Implements<Values,Object>()
{
    add(val0,val1,val2,val3);
}

/** Construct from the passed values */
Values::Values(const SymbolValue &val0, const SymbolValue &val1, const SymbolValue &val2,
               const SymbolValue &val3, const SymbolValue &val4)
       : Implements<Values,Object>()
{
    add(val0,val1,val2,val3,val4);
}

/** Construct from the passed values */
Values::Values(const SymbolValue &val0, const SymbolValue &val1, const SymbolValue &val2,
               const SymbolValue &val3, const SymbolValue &val4, const SymbolValue &val5)
       : Implements<Values,Object>()
{
    add(val0,val1,val2,val3,val4,val5);
}

/** Construct from the passed values */
Values::Values(const SymbolValue &val0, const SymbolValue &val1, const SymbolValue &val2,
               const SymbolValue &val3, const SymbolValue &val4, const SymbolValue &val5,
               const SymbolValue &val6)
       : Implements<Values,Object>()
{
    add(val0,val1,val2,val3,val4,val5,val6);
}

/** Construct from the passed values */
Values::Values(const SymbolValue &val0, const SymbolValue &val1, const SymbolValue &val2,
               const SymbolValue &val3, const SymbolValue &val4, const SymbolValue &val5,
               const SymbolValue &val6, const SymbolValue &val7)
       : Implements<Values,Object>()
{
    add(val0,val1,val2,val3,val4,val5,val6,val7);
}

/** Construct from the passed values */
Values::Values(const SymbolValue &val0, const SymbolValue &val1, const SymbolValue &val2,
               const SymbolValue &val3, const SymbolValue &val4, const SymbolValue &val5,
               const SymbolValue &val6, const SymbolValue &val7, const SymbolValue &val8)
       : Implements<Values,Object>()
{
    add(val0,val1,val2,val3,val4,val5,val6,val7,val8);
}

/** Construct from the passed values */
Values::Values(const SymbolValue &val0, const SymbolValue &val1, const SymbolValue &val2,
               const SymbolValue &val3, const SymbolValue &val4, const SymbolValue &val5,
               const SymbolValue &val6, const SymbolValue &val7, const SymbolValue &val8,
               const SymbolValue &val9)
       : Implements<Values,Object>()
{
    add(val0,val1,val2,val3,val4,val5,val6,val7,val8,val9);
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

/** Add the value 'val' to this set */
Values& Values::operator+=(const SymbolValue &val)
{
    this->add(val);
    return *this;
}

/** Add the contents of 'other' to this set - this overwrites any
    existing values that are also in 'other' */
Values& Values::operator+=(const Values &other)
{
    if (other.vals.isEmpty())
        return *this;
    else if (vals.isEmpty())
    {
        vals = other.vals;
        return *this;
    }
    else
    {
        vals.reserve( vals.count() + other.vals.count() );

        for (QHash<SymbolID,double>::const_iterator it = other.vals.begin();
             it != other.vals.end();
             ++it)
        {
            vals.insert( it.key(), it.value() );
        }

        return *this;
    }
}

/** Reserve space for at least 'n' items */
void Values::reserve(int n)
{
    vals.reserve(n);
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

/** Add a SymbolValue to the set of values */
void Values::add(const SymbolValue &val0)
{
    vals.insert(val0.first, val0.second);
}

/** Set the Symbol 'symbol' equal to 'value' */
void Values::set(const Symbol &symbol, double value)
{
    vals.insert(symbol.ID(), value);
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

namespace SireCAS
{
    Values SIRECAS_EXPORT operator+(const SymbolValue &val0, const SymbolValue &val1)
    {
        Values vals(val0);
        vals += val1;
        
        return vals;
    }

    Values SIRECAS_EXPORT operator+(const Values &vals, const SymbolValue &val)
    {
        Values new_vals(vals);
        new_vals += val;
        return new_vals;
    }
    
    Values SIRECAS_EXPORT operator+(const SymbolValue &val, const Values &vals)
    {
        Values new_vals(vals);
        new_vals += val;
        return new_vals;
    }

    Values SIRECAS_EXPORT operator+(const Values &vals0, const Values &vals1)
    {
        Values new_vals(vals0);
        new_vals += vals1;
        return new_vals;
    }
}
