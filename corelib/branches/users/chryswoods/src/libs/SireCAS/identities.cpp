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
#include "function.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireCAS;

static const RegisterMetaType<Identities> r_identities;

/** Serialise to a binary data stream */
QDataStream SIRECAS_EXPORT &operator<<(QDataStream &ds, const Identities &ids)
{
    writeHeader(ds, r_identities, 1) << ids.idhash << ids.funchash;

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIRECAS_EXPORT &operator>>(QDataStream &ds, Identities &ids)
{
    VersionID v = readHeader(ds, r_identities);

    if (v == 1)
    {
        ds >> ids.idhash >> ids.funchash;
    }
    else
        throw version_error(v, "1", r_identities, CODELOC);

    return ds;
}

/** Construct an empty set of identities */
Identities::Identities()
{}

/** Construct from a list of passed expressions */
Identities::Identities(const QList<SymbolExpression> &expressions)
{
    for (QList<SymbolExpression>::const_iterator it = expressions.begin();
         it != expressions.end();
         ++it)
    {
        add(*it);
    }
}

/** Construct with a hash of expressions indexed by symbol */
Identities::Identities(const QHash<Symbol,Expression> &expressions)
{
    for (QHash<Symbol,Expression>::const_iterator it = expressions.begin();
         it != expressions.end();
         ++it)
    {
        set(it.key(), it.value());
    }
}


/** Construct with the passed expressions */
Identities::Identities(const SymbolExpression &symex0)
{
    add(symex0);
}

/** Copy constructor */
Identities::Identities(const Identities &other) : idhash(other.idhash)
{}

/** Destructor */
Identities::~Identities()
{}

/** Set the Symbol 'symbol' equal to 'expression' */
void Identities::set(const Symbol &symbol, const Expression &expression)
{
    if (symbol.isA<Function>())
    {
        //do we already have an expression for this function?
        const Function &func = symbol.asA<Function>();

        //remove any existing function with this signature
        idhash.remove( function(func).ID() );

        //store this function in the funchash
        funchash.insert( func.signature(), func );
    }

    idhash.insert(symbol.ID(), expression);
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

/** Return whether or not this set of identities contains an identity for
    the function 'func' (or one of its relations) */
bool Identities::contains(const Function &func) const
{
    return funchash.contains( func.signature() );
}

/** Return the actual form of the function 'func' stored in this set of
    identities, or the null function if it is not present here */
Function Identities::function(const Function &func) const
{
    if ( funchash.contains(func.signature()) )
        return funchash[func.signature()].base().asA<Function>();
    else
        return Function();
}

/** Add the passed expressions */
void Identities::add(const SymbolExpression &symex0, const SymbolExpression &symex1)
{
    add(symex0);
    add(symex1);
}

/** Add the passed expressions */
void Identities::add(const SymbolExpression &symex0, const SymbolExpression &symex1,
                     const SymbolExpression &symex2)
{
    add(symex0);
    add(symex1);
    add(symex2);
}

/** Add the passed expressions */
void Identities::add(const SymbolExpression &symex0, const SymbolExpression &symex1,
                     const SymbolExpression &symex2, const SymbolExpression &symex3)
{
    add(symex0);
    add(symex1);
    add(symex2);
    add(symex3);
}

/** Add the passed expressions */
void Identities::add(const SymbolExpression &symex0, const SymbolExpression &symex1,
                     const SymbolExpression &symex2, const SymbolExpression &symex3,
                     const SymbolExpression &symex4)
{
    add(symex0);
    add(symex1);
    add(symex2);
    add(symex3);
    add(symex4);
}

/** Add the passed expressions */
void Identities::add(const SymbolExpression &symex0, const SymbolExpression &symex1,
                     const SymbolExpression &symex2, const SymbolExpression &symex3,
                     const SymbolExpression &symex4, const SymbolExpression &symex5)
{
    add(symex0);
    add(symex1);
    add(symex2);
    add(symex3);
    add(symex4);
    add(symex5);
}

/** Add the passed expressions */
void Identities::add(const SymbolExpression &symex0, const SymbolExpression &symex1,
                     const SymbolExpression &symex2, const SymbolExpression &symex3,
                     const SymbolExpression &symex4, const SymbolExpression &symex5,
                     const SymbolExpression &symex6)
{
    add(symex0);
    add(symex1);
    add(symex2);
    add(symex3);
    add(symex4);
    add(symex5);
    add(symex6);
}

/** Add the passed expressions */
void Identities::add(const SymbolExpression &symex0, const SymbolExpression &symex1,
                     const SymbolExpression &symex2, const SymbolExpression &symex3,
                     const SymbolExpression &symex4, const SymbolExpression &symex5,
                     const SymbolExpression &symex6, const SymbolExpression &symex7)
{
    add(symex0);
    add(symex1);
    add(symex2);
    add(symex3);
    add(symex4);
    add(symex5);
    add(symex6);
    add(symex7);
}

/** Add the passed expressions */
void Identities::add(const SymbolExpression &symex0, const SymbolExpression &symex1,
                     const SymbolExpression &symex2, const SymbolExpression &symex3,
                     const SymbolExpression &symex4, const SymbolExpression &symex5,
                     const SymbolExpression &symex6, const SymbolExpression &symex7,
                     const SymbolExpression &symex8)
{
    add(symex0);
    add(symex1);
    add(symex2);
    add(symex3);
    add(symex4);
    add(symex5);
    add(symex6);
    add(symex7);
    add(symex8);
}

/** Add the passed expressions */
void Identities::add(const SymbolExpression &symex0, const SymbolExpression &symex1,
                     const SymbolExpression &symex2, const SymbolExpression &symex3,
                     const SymbolExpression &symex4, const SymbolExpression &symex5,
                     const SymbolExpression &symex6, const SymbolExpression &symex7,
                     const SymbolExpression &symex8, const SymbolExpression &symex9)
{
    add(symex0);
    add(symex1);
    add(symex2);
    add(symex3);
    add(symex4);
    add(symex5);
    add(symex6);
    add(symex7);
    add(symex8);
    add(symex9);
}

/** Construct from the passed values */
Identities::Identities(const SymbolExpression &symex0, const SymbolExpression &symex1)
{
    add(symex0,symex1);
}

/** Construct from the passed values */
Identities::Identities(const SymbolExpression &symex0, const SymbolExpression &symex1,
                       const SymbolExpression &symex2)
{
    add(symex0,symex1,symex2);
}

/** Construct from the passed values */
Identities::Identities(const SymbolExpression &symex0, const SymbolExpression &symex1,
                       const SymbolExpression &symex2, const SymbolExpression &symex3)
{
    add(symex0,symex1,symex2,symex3);
}

/** Construct from the passed values */
Identities::Identities(const SymbolExpression &symex0, const SymbolExpression &symex1,
                       const SymbolExpression &symex2, const SymbolExpression &symex3,
                       const SymbolExpression &symex4)
{
    add(symex0,symex1,symex2,symex3,symex4);
}

/** Construct from the passed values */
Identities::Identities(const SymbolExpression &symex0, const SymbolExpression &symex1,
                       const SymbolExpression &symex2, const SymbolExpression &symex3,
                       const SymbolExpression &symex4, const SymbolExpression &symex5)
{
    add(symex0,symex1,symex2,symex3,symex4,symex5);
}

/** Construct from the passed values */
Identities::Identities(const SymbolExpression &symex0, const SymbolExpression &symex1,
                       const SymbolExpression &symex2, const SymbolExpression &symex3,
                       const SymbolExpression &symex4, const SymbolExpression &symex5,
                       const SymbolExpression &symex6)
{
    add(symex0,symex1,symex2,symex3,symex4,symex5,symex6);
}

/** Construct from the passed values */
Identities::Identities(const SymbolExpression &symex0, const SymbolExpression &symex1,
                       const SymbolExpression &symex2, const SymbolExpression &symex3,
                       const SymbolExpression &symex4, const SymbolExpression &symex5,
                       const SymbolExpression &symex6, const SymbolExpression &symex7)
{
    add(symex0,symex1,symex2,symex3,symex4,symex5,symex6,symex7);
}

/** Construct from the passed values */
Identities::Identities(const SymbolExpression &symex0, const SymbolExpression &symex1,
                       const SymbolExpression &symex2, const SymbolExpression &symex3,
                       const SymbolExpression &symex4, const SymbolExpression &symex5,
                       const SymbolExpression &symex6, const SymbolExpression &symex7,
                       const SymbolExpression &symex8)
{
    add(symex0,symex1,symex2,symex3,symex4,symex5,symex6,symex7,symex8);
}

/** Construct from the passed values */
Identities::Identities(const SymbolExpression &symex0, const SymbolExpression &symex1,
                       const SymbolExpression &symex2, const SymbolExpression &symex3,
                       const SymbolExpression &symex4, const SymbolExpression &symex5,
                       const SymbolExpression &symex6, const SymbolExpression &symex7,
                       const SymbolExpression &symex8, const SymbolExpression &symex9)
{
    add(symex0,symex1,symex2,symex3,symex4,symex5,symex6,symex7,symex8,symex9);
}

const char* Identities::typeName()
{
    return QMetaType::typeName( qMetaTypeId<Identities>() );
}
