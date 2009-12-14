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

#include <QMutex>
#include <QHash>

#include "symbol.h"
#include "factor.h"
#include "values.h"
#include "complexvalues.h"
#include "identities.h"

#include "SireMaths/complex.h"

#include "SireCAS/errors.h"

#include "Siren/errors.h"
#include "Siren/stream.h"

#include <QDebug>

using namespace Siren;
using namespace SireMaths;
using namespace SireCAS;

typedef QHash<QString,SymbolID> SymbolRegistry;

Q_GLOBAL_STATIC( SymbolRegistry, symbolRegistry )
Q_GLOBAL_STATIC( QMutex, registryMutex )

/** Return an ID for the symbol with representation 'rep'. This
    creates a new ID if there is no symbol currently registered with
    this ID. */
SymbolID Symbol::getNewID(const QString &rep)
{
    if (rep.isNull() or rep.isEmpty())
        return 0;

    QMutexLocker lkr( registryMutex() );

    SymbolRegistry *registry = symbolRegistry();

    if (registry->contains(rep))
        return registry->value(rep);
    else
    {
        SymbolID id = registry->count() + 1;
        registry->insert( rep, id );
        return id;
    }
}

/** Return the name of the symbol with ID == symid

    \throw SireCAS::invalid_symbol
*/
QString Symbol::getName(SymbolID symid)
{
    QMutexLocker lkr( registryMutex() );

    SymbolRegistry *registry = symbolRegistry();

    for (QHash<QString,SymbolID>::const_iterator it = registry->constBegin();
         it != registry->constEnd();
         ++it)
    {
        if (*it == symid)
            return it.key();
    }

    throw SireCAS::invalid_symbol( QObject::tr(
              "There is no symbol with ID == %1.")
                  .arg(symid), CODELOC );

    return QString::null;
}

static const RegisterObject<Symbol> r_symbol;

/** Null constructor */
Symbol::Symbol() : Implements<Symbol,CASNode>(), id(0), stringrep(QString::null)
{}

/** Construct a symbol from the passed ID number */
Symbol::Symbol(SymbolID symid)
       : Implements<Symbol,CASNode>(), id(symid), stringrep( Symbol::getName(symid) )
{}

/** Construct a new symbol, with string representation 'rep' */
Symbol::Symbol(const QString &rep)
       : Implements<Symbol,CASNode>(), id( Symbol::getNewID(rep) ), stringrep(rep)
{}

/** Copy constructor */
Symbol::Symbol(const Symbol &other)
       : Implements<Symbol,CASNode>(other), id(other.id), stringrep(other.stringrep)
{}

/** Destructor */
Symbol::~Symbol()
{}

/** Assignment operator */
Symbol& Symbol::operator=(const Symbol &other)
{
    if (this != &other)
    {
        id = other.id;
        stringrep = other.stringrep;
        CASNode::operator=(other);
    }
    
    return *this;
}

bool Symbol::operator==(const Symbol &other) const
{
    return id == other.id;
}

bool Symbol::operator!=(const Symbol &other) const
{
    return id != other.id;
}

/** Assignment operator */
Symbol& Symbol::operator=(SymbolID symid)
{
    return this->operator=( Symbol(symid) );
}

/** Comparison operator - a Symbol is greater than another
    symbol if it's string representation is greater - this
    allows lists of symbols to be sorted alphabetically */
bool Symbol::operator<(const Symbol &other) const
{
    return stringrep < other.stringrep;
}

/** Comparison operator - a Symbol is greater than another
    symbol if it's string representation is greater - this
    allows lists of symbols to be sorted alphabetically */
bool Symbol::operator>(const Symbol &other) const
{
    return stringrep > other.stringrep;
}

/** Comparison operator - a Symbol is greater than another
    symbol if it's string representation is greater - this
    allows lists of symbols to be sorted alphabetically */
bool Symbol::operator<=(const Symbol &other) const
{
    return stringrep <= other.stringrep;
}

/** Comparison operator - a Symbol is greater than another
    symbol if it's string representation is greater - this
    allows lists of symbols to be sorted alphabetically */
bool Symbol::operator>=(const Symbol &other) const
{
    return stringrep >= other.stringrep;
}

/** Return a string representation of this symbol */
QString Symbol::toString() const
{
    return stringrep;
}

uint Symbol::hashCode() const
{
    return id;
}

/** Return the ID number of this symbol - this ID number
    is just for this session - if this symbol is streamed
    and loaded into another process then the ID will change */
SymbolID Symbol::ID() const
{
    return id;
}

void Symbol::stream(Stream &s)
{
    s.assertVersion<Symbol>(1);
    
    Schema schema = s.item<Symbol>();
    
    schema.data("representation") & stringrep;
    
    if (s.isLoading())
    {
        id = getNewID(stringrep);
    }
    
    CASNode::stream( schema.base() );
}

/** There are no child expressions in a symbol */
QList<Expression> Symbol::children() const
{
    return QList<Expression>();
}

Values Symbol::operator==(double value) const
{
    return Values(*this, value);
}

Values Symbol::operator==(int value) const
{
    return Values(*this, value);
}

ComplexValues Symbol::operator==(const SireMaths::Complex &value) const
{
    return ComplexValues(*this, value);
}

Identities Symbol::operator==(const Expression &expression) const
{
    return Identities(*this, expression);
}

/** Evaluate this symbol - returns the value of the symbol in 'values' if
    it is present, else it returns 0.0 */
double Symbol::evaluate(const Values &values) const
{
    return values.value(*this);
}

/** Evaluate this symbol - returns the value of the symbol in 'values' if
    it is present, else it returns 0 */
Complex Symbol::evaluate(const ComplexValues &values) const
{
    return values.value(*this);
}

/** Differentiate this symbol with respect to 'sym'. This returns 1.0 if this
    is 'sym', else it returns 0.0 */
Expression Symbol::differentiate(const Symbol &sym) const
{
    //assume int(true) == 1 and int(false) == 0
    return Expression( int( sym.ID() == ID() ) );
}

/** Integrate this symbol with respect to 'sym'. If 'sym' == this, then
    return 0.5 sym^2, else return *this * sym */
Expression Symbol::integrate(const Symbol &sym) const
{
    if (sym.ID() == ID())
        return 0.5 * SireCAS::pow(sym,2);
    else
        return *this * sym;
}

/** Return the expression that matches this symbol in 'identities' - or return
    an expression holding only this symbol if it does no exist in 'identities' */
Expression Symbol::substitute(const Identities &identities) const
{
    return identities.expression(*this);
}

/** Return this symbol */
QSet<Symbol> Symbol::symbols() const
{
    QSet<Symbol> s;
    s.insert(*this);

    return s;
}

/** Is this a function of 'symbol' */
bool Symbol::isFunction(const Symbol &sym) const
{
    return (id != 0) and (sym.ID() == ID());
}

/** A symbol is by definition not constant */
bool Symbol::isConstant() const
{
    return false;
}

QList<Factor> Symbol::expand(const Symbol &symbol) const
{
    QList<Factor> factors;

    if ( *this == symbol )
    {
        factors.append( Factor(symbol, 1,1) );
    }
    else
    {
        factors.append( Factor(symbol, *this,0) );
    }
    
    return factors;
}
