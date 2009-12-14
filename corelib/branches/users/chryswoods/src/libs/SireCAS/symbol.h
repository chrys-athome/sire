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

#ifndef SIRECAS_SYMBOL_H
#define SIRECAS_SYMBOL_H

#include <QString>
#include <QPair>

#include "casnode.h"

SIRE_BEGIN_HEADER

namespace SireMaths
{
class Complex;
}

namespace SireCAS
{

class ComplexValues;
class Identities;
class Values;
class Factor;

class Expression;

typedef quint32 SymbolID;

/** This class represents an algebraic symbol in the equation (e.g. 'x' or 'y')

    @author Christopher Woods
*/
class SIRECAS_EXPORT Symbol : public Siren::Implements<Symbol,CASNode>
{
public:
    Symbol();
    Symbol(SymbolID symid);
    Symbol(const QString &rep);

    Symbol(const Symbol &other);

    ~Symbol();

    Symbol& operator=(const Symbol &other);
    Symbol& operator=(SymbolID symid);

    bool operator<(const Symbol &other) const;
    bool operator>(const Symbol &other) const;
    bool operator<=(const Symbol &other) const;
    bool operator>=(const Symbol &other) const;

    bool operator==(const Symbol &other) const;
    bool operator!=(const Symbol &other) const;

    Values operator==(double value) const;
    Values operator==(int value) const;

    ComplexValues operator==(const SireMaths::Complex &value) const;

    Identities operator==(const Expression &expression) const;

    SymbolID ID() const;

    //////////////////////////////
    // Implements Siren::Object //
    //////////////////////////////
    
    QString toString() const;
    uint hashCode() const;
    void stream(Siren::Stream &s);
    
    /////////////////////////////////
    // Implements SireCAS::CASNode //
    /////////////////////////////////

    Expression differentiate(const Symbol &symbol) const;
    Expression integrate(const Symbol &symbol) const;

    bool isFunction(const Symbol&) const;
    bool isConstant() const;

    double evaluate(const Values &values) const;
    SireMaths::Complex evaluate(const ComplexValues &values) const;

    Expression substitute(const Identities &identities) const;

    QSet<Symbol> symbols() const;
    QList<Expression> children() const;

    QList<Factor> expand(const Symbol &symbol) const;

private:
    static SymbolID getNewID(const QString &symbol);
    static QString getName(SymbolID symid);

    /** Unique ID number that is given to every symbol */
    SymbolID id;

    /** String representation of this symbol */
    QString stringrep;
};

}

Q_DECLARE_METATYPE(SireCAS::Symbol)

SIRE_EXPOSE_CLASS( SireCAS::Symbol )

SIRE_END_HEADER

#endif
