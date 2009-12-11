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

    bool operator==(const Expression &other) const;
    bool operator!=(const Expression &other) const;

    Expression& operator=(const Expression &other);

    Expression& operator+=(const Expression &other);
    Expression& operator-=(const Expression &other);
    Expression& operator*=(const Expression &other);
    Expression& operator/=(const Expression &other);

    double operator()(const Values &values) const;
    Complex operator()(const ComplexValues &values) const;

    Expression operator-() const;

    QString toString() const;
    uint hashCode() const;
    void stream(Siren::Stream &s);

    Expression add(const Expression &ex) const;
    Expression add(double val) const;
    Expression add(const SireMaths::Complex &val) const;

    Expression subtract(const Expression &ex) const;
    Expression subtract(double val) const;
    Expression subtract(const SireMaths::Complex &val) const;

    Expression multiply(const Expression &ex) const;
    Expression multiply(double val) const;
    Expression multiply(const SireMaths::Complex &val) const;

    Expression divide(const Expression &ex) const;
    Expression divide(double val) const;
    Expression divide(const SireMaths::Complex &val) const;

    Expression negate() const;
    Expression invert() const;
    Expression conjugate() const;

    Expression pow(int n) const;
    Expression squared() const;
    Expression cubed() const;

    Expression pow(const SireMaths::Rational &n) const;
    Expression pow(double n) const;
    Expression pow(const SireMaths::Complex &n) const;
    Expression pow(const Expression &n) const;

    Expression root(int n) const;

    Expression substitute(const Identities &identities) const;
    Expression simplify(int options=0) const;

    QList<Factor> expand(const Symbol &symbol) const;

    double factor() const;
    const CASNode& node() const;

    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;

    Expression differentiate(const Symbol &symbol, int level=1) const;
    Expression integrate(const Symbol &symbol) const;

    Expression diff(const Symbol &symbol, int level=1) const;
    Expression integ(const Symbol &symbol) const;

    Expression series(const Symbol &symbol, int order) const;

    bool isZero() const;
    bool isConstant() const;
    bool isFunction(const Symbol &symbol) const;
    bool isCompound() const;
    bool isComplex() const;

    QSet<Symbol> symbols() const;
    QList<Expression> children() const;

    template<class T>
    QList<T> children() const;

private:
    /** The base of this expression */
    CASNodePtr casnode;

    /** The factor of the expression */
    double fac;
};

#ifndef SIRE_SKIP_INLINE_FUNCTIONS

/** Return a list of all children of type 'T' in this expression */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
QList<T> Expression::children() const
{
    QList<Expression> exs = this->children();

    QList<T> children_t;

    for (typename QList<Expression>::const_iterator it = exs.constBegin();
         it != exs.constEnd();
         ++it)
    {
        const CASNode &n = it->node();

        //gccxml doesn't like this section, so remove it
        //when we are generating the python wrappers
        #ifndef SKIP_BROKEN_GCCXML_PARTS
        if (n.isA<T>())
            children_t.append( n.asA<T>() );
        #endif
    }

    return children_t;
}

#endif //SIRE_SKIP_INLINE_FUNCTIONS

}

Q_DECLARE_METATYPE(SireCAS::Expression)

SIRE_EXPOSE_CLASS( SireCAS::Expression )
SIRE_EXPOSE_FUNCTION( SireCAS::pow )
SIRE_EXPOSE_FUNCTION( SireCAS::sqrt )
SIRE_EXPOSE_FUNCTION( SireCAS::cbrt )

SIRE_END_HEADER

#endif
