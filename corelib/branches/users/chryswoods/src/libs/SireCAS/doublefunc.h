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

#ifndef SIRECAS_DOUBLEFUNC_H
#define SIRECAS_DOUBLEFUNC_H

#include "symbol.h"
#include "expression.h"

SIRE_BEGIN_HEADER

namespace SireCAS
{

/** Base class of all double-expression functions ( e.g. f(x(), y()) )

    @author Christopher Woods
*/
class SIRECAS_EXPORT DoubleFunc : public Siren::Extends<DoubleFunc,CASNode>
{
public:
    DoubleFunc();
    DoubleFunc(const Expression &x, const Expression &y);

    DoubleFunc(const DoubleFunc &other);

    ~DoubleFunc();

    uint hashCode() const;
    QString toString() const;
    void stream(Siren::Stream &s);

    static QString typeName();

    const Expression& x() const;
    const Expression& y() const;

    Expression conjugate() const;

    bool isFunction(const Symbol &symbol) const;
    bool isConstant() const;
    bool isComplex() const;
    bool isCompound() const;

    Expression substitute(const Identities &identities) const;
    QSet<Symbol> symbols() const;
    QList<Expression> children() const;

    Expression differentiate(const Symbol &symbol) const;
    Expression integrate(const Symbol &symbol) const;

    QList<Factor> expand(const Symbol &symbol) const;

protected:
    DoubleFunc& operator=(const DoubleFunc &other);
    bool operator==(const DoubleFunc &other) const;
    bool operator!=(const DoubleFunc &other) const;

    virtual Expression functionOf(const Expression &x, 
                                  const Expression &y) const=0;

    virtual QString stringRep() const=0;

    /** The two expressions that this function operates on */
    Expression ex0, ex1;
};

/** To declare a new function copy Min or Max (in minmax.h) */

}

SIRE_EXPOSE_CLASS( SireCAS::DoubleFunc )

SIRE_END_HEADER

#endif
