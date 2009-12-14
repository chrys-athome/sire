/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
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

#ifndef SIRECAS_IDENTITIES_H
#define SIRECAS_IDENTITIES_H

#include <QHash>
#include <QSet>
#include <QList>

#include "symbol.h"

SIRE_BEGIN_HEADER

namespace SireCAS
{

/** This class holds a collection of identities. 
    These are symbols, and the expressions that they are equal to, 
    e.g. x == y.squared() + sin(z). These identities can then be 
    substituted into expressions, with all instances of the variable 
    being replaced by the equivalent expression.

    @author Christopher Woods
*/
class SIRECAS_EXPORT Identities : public Siren::Implements<Identities,Siren::Object>
{
public:
    Identities();
    Identities(const Symbol &symbol, const Expression &expression);
    Identities(const QHash<Symbol,Expression> &expressions);

    Identities(const Identities &other);

    ~Identities();

    Identities& operator=(const Identities &other);
    
    bool operator==(const Identities &other) const;
    bool operator!=(const Identities &other) const;

    Identities operator+(const Identities &other) const;

    QString toString() const;
    uint hashCode() const;
    void stream(Siren::Stream &s);

    bool contains(const Symbol &sym) const;
    Expression expression(const Symbol &sym) const;

    QList<Symbol> keys() const;
    QList<Symbol> symbols() const;

    const QHash<SymbolID,Expression>& values() const;

private:
    /** Hash mapping a symbol to an expression */
    QHash<SymbolID, Expression> idhash;
};

}

Q_DECLARE_METATYPE(SireCAS::Identities)

SIRE_EXPOSE_CLASS( SireCAS::Identities )

SIRE_END_HEADER

#endif
