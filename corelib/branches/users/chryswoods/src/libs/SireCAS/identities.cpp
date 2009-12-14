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

#include "identities.h"
#include "symbol.h"
#include "expression.h"

#include "Siren/stream.h"
#include "Siren/streamqt.h"

using namespace Siren;
using namespace SireCAS;

static const RegisterObject<Identities> r_identities;

/** Construct an empty set of identities */
Identities::Identities() : Implements<Identities,Object>()
{}

/** Construct just setting 'symbol' equal to 'value' */
Identities::Identities(const Symbol &symbol, const Expression &expression)
           : Implements<Identities,Object>()
{
    idhash.reserve(1);
    idhash.insert(symbol.ID(), expression);
}

/** Construct with a hash of expressions indexed by symbol */
Identities::Identities(const QHash<Symbol,Expression> &expressions)
           : Implements<Identities,Object>()
{
    for (QHash<Symbol,Expression>::const_iterator it = expressions.begin();
         it != expressions.end();
         ++it)
    {
        idhash.insert(it.key().ID(), it.value());
    }
}

/** Copy constructor */
Identities::Identities(const Identities &other)
           : Implements<Identities,Object>(other), idhash(other.idhash)
{}

/** Destructor */
Identities::~Identities()
{}

Identities& Identities::operator=(const Identities &other)
{
    idhash = other.idhash;
    Object::operator=(other);
    return *this;
}

bool Identities::operator==(const Identities &other) const
{
    return idhash == other.idhash;
}

bool Identities::operator!=(const Identities &other) const
{
    return not Identities::operator==(other);
}

uint Identities::hashCode() const
{
    return qHash( Identities::typeName() ) + qHash( idhash.count() );
}

void Identities::stream(Stream &s)
{
    s.assertVersion<Identities>(1);
    
    Schema schema = s.item<Identities>();
    
    schema.data("identities") & idhash;
    
    Object::stream( schema.base() );
}

/** Return a list of the symbols that are present in this set */
QList<Symbol> Identities::symbols() const
{
    QList<Symbol> s;
    
    for (QHash<SymbolID,Expression>::const_iterator it = idhash.constBegin();
         it != idhash.constEnd();
         ++it)
    {
        s.append( Symbol(it.key()) );
    }

    return s;
}

const QHash<SymbolID,Expression>& Identities::values() const
{
    return idhash;
}

/** Return a list of the symbols that are present in this set */
QList<Symbol> Identities::keys() const
{
    return this->symbols();
}

QString Identities::toString() const
{
    QStringList words;
    QStringList lines;
    
    QList<Symbol> syms = this->symbols();
    
    qSort(syms);
    
    foreach (const Symbol &sym, syms)
    {
        words.append( QString("%1 == %2").arg(sym.toString())
                                         .arg(this->expression(sym).toString()) );

        if (words.count() == 4)
        {
            lines.append( words.join(", ") );
            words.clear();
        }
    }
    
    return QString("{ %1 }").arg( lines.join("\n  ") );
}

/** Return whether or not this set of identities contains an identity for
    the symbol 'symbol' */
bool Identities::contains(const Symbol &symbol) const
{
    return idhash.contains(symbol.ID());
}

/** Return the associated expression for 'symbol', or an expression containing
    this symbol if there is no such expression */
Expression Identities::expression(const Symbol &symbol) const
{
    if ( idhash.contains(symbol.ID()) )
        return idhash[symbol.ID()];
    else
        return symbol;
}

/** Add the contents of 'other' to this set - this overwrites any
    existing values that are also in 'other' */
Identities Identities::operator+(const Identities &other) const
{
    if (other.idhash.isEmpty())
        return *this;
    else if (idhash.isEmpty())
    {
        return other;
    }
    else
    {
        Identities ret;
    
        ret.idhash = idhash;
        ret.idhash.reserve( idhash.count() + other.idhash.count() );

        for (QHash<SymbolID,Expression>::const_iterator it = other.idhash.begin();
             it != other.idhash.end();
             ++it)
        {
            ret.idhash.insert( it.key(), it.value() );
        }

        return ret;
    }
}
