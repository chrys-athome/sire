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

#ifndef SIRECAS_EXPRESSION_H
#define SIRECAS_EXPRESSION_H

#include <QList>

#include "casnode.h"

SIRE_BEGIN_HEADER

namespace SireMaths
{
class Rational;
class Complex;
}

namespace SireCAS
{

class Identities;
class Factor;

/** An Expression is the base class of all algebraic entities.

    @author Christopher Woods
*/
class SIRECAS_EXPORT Expression : public Siren::Implements<Expression,CASNode>
{
public:
    Expression();

    Expression(int constant);
    Expression(double constant);
    Expression(const SireMaths::Rational &constant);
    Expression(const SireMaths::Complex &constant);

    Expression(const CASNode &node);

    Expression(const Expression &other);

    ~Expression();

    Expression& operator=(const Expression &other);

    bool operator==(const Expression &other) const;
    bool operator!=(const Expression &other) const;

    QString toString() const;
    uint hashCode() const;
    void stream(Siren::Stream &s);

    double factor() const;
    const CASNode& node() const;

    Expression operator-() const;

    Expression differentiate(const Symbol &symbol) const;
    Expression differentiate(const Symbol &symbol, int n) const;
    
    Expression integrate(const Symbol &symbol) const;

    Expression conjugate() const;

    Expression series(const Symbol &symbol, int n) const;

    Expression simplify(int options=0) const;

    bool isFunction(const Symbol&) const;
    bool isConstant() const;
    bool isComplex() const;
    bool isCompound() const;
    bool isZero() const;

    Expression add(const CASNode &node) const;
    Expression add(double val) const;
    Expression add(const SireMaths::Complex &val) const;

    Expression subtract(const CASNode &node) const;
    Expression subtract(double val) const;
    Expression subtract(const SireMaths::Complex &val) const;

    Expression multiply(const CASNode &node) const;
    Expression multiply(double val) const;
    Expression multiply(const SireMaths::Complex &val) const;

    Expression divide(const CASNode &node) const;
    Expression divide(double val) const;
    Expression divide(const SireMaths::Complex &val) const;

    Expression negate() const;
    Expression invert() const;

    Expression pow(int n) const;
    Expression squared() const;
    Expression cubed() const;

    Expression pow(const SireMaths::Rational &n) const;
    Expression pow(double n) const;
    Expression pow(const SireMaths::Complex &n) const;
    Expression pow(const CASNode &n) const;

    Expression sqrt() const;
    Expression cbrt() const;
    Expression root(int n) const;

    double evaluate(const Values &values) const;

    SireMaths::Complex evaluate(const ComplexValues &values) const;

    Expression substitute(const Identities &identities) const;
    
    QSet<Symbol> symbols() const;

    QList<Expression> children() const;
    
    QList<Factor> expand(const Symbol &symbol) const;

    bool test(Siren::Logger &logger) const;

private:
    /** The base of this expression */
    CASNodePtr casnode;

    /** The factor of the expression */
    double fac;
};

#ifndef SIRE_SKIP_INLINE_FUNCTIONS

template<class T>
SIRE_OUTOFLINE_TEMPLATE
QList<T> CASNode::children() const
{
    QList<Expression> exs = this->children();
    
    QList<T> t_exs;
    
    for (typename QList<Expression>::const_iterator it = exs.constBegin();
         it != exs.constEnd();
         ++it)
    {
        if (it->node().isA<T>())
        {
            t_exs.append( it->node().asA<T>() );
        }
    }
    
    return t_exs;
}

#endif // SIRE_SKIP_INLINE_FUNCTIONS

}

Q_DECLARE_METATYPE(SireCAS::Expression)

SIRE_EXPOSE_CLASS( SireCAS::Expression )

SIRE_END_HEADER

#endif
