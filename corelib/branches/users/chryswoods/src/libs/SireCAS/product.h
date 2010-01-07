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

#ifndef SIRECAS_PRODUCT_H
#define SIRECAS_PRODUCT_H

#include <QHash>

#include "expression.h"

SIRE_BEGIN_HEADER

namespace SireCAS
{

/** This class holds a collection of expressions that are to be multiplied (or divided)

    @author Christopher Woods
*/
class SIRECAS_EXPORT Product : public Siren::Implements<Product,CASNode>
{
public:
    Product();
    Product(const Expression &ex0, const Expression &ex1);
    Product(const QList<Expression> &expressions);

    Product(const Product &other);

    ~Product();

    Product& operator=(const Product &other);
    
    bool operator==(const Product &other) const;
    bool operator!=(const Product &other) const;
    
    uint hashCode() const;
    QString toString() const;
    void stream(Siren::Stream &s);

    Expression differentiate(const Symbol &symbol) const;
    Expression integrate(const Symbol &symbol) const;

    Expression series(const Symbol &symbol, int n) const;

    Expression simplify(int options=0) const;

    Expression conjugate() const;

    bool isFunction(const Symbol&) const;
    bool isConstant() const;
    bool isComplex() const;
    bool isCompound() const;

    double evaluate(const Values &values) const;
    SireMaths::Complex evaluate(const ComplexValues &values) const;

    Expression substitute(const Identities &identities) const;

    QSet<Symbol> symbols() const;
    QList<Expression> children() const;

    Expression reduce() const;

    Product numerator() const;
    Product denominator() const;

    QList<Factor> expand(const Symbol &symbol) const;

    bool test(Siren::Logger &logger) const;

private:
    void rebuild();

    Expression take(const Expression &core);
    Expression takeFirst(const Symbol &symbol);

    void pvt_multiply(const Expression &ex);

    void pvt_multiply(double val);
    void pvt_multiply(const SireMaths::Complex &complex);
    void pvt_multiply(double val, const Expression &power);
    void pvt_multiply(const SireMaths::Complex &complex, const Expression &power);
    void pvt_multiply(const Product &product, const Expression &power);
    void pvt_multiply(const CASNode &core, const Expression &power);

    void pvt_multiply(const Expression &ex, const Expression &power);

    bool isPureProduct() const;

    static Expression productRule(Product product, const Symbol &symbol);
    static Expression quotientRule(const Product &f, const Product &g, 
                                   const Symbol &symbol);

    /** The collection of powers of each part of the Product, indexed
        by their core */
    QHash<Expression, Expression> powers;

    /** The expressions on the numerator, indexed by their core */
    QHash<Expression, Expression> numparts;

    /** The expressions on the denominator, indexed by their core */
    QHash<Expression, Expression> denomparts;

    /** Base factor */
    double strtval;
};

}

Q_DECLARE_METATYPE(SireCAS::Product)

SIRE_EXPOSE_CLASS( SireCAS::Product )

SIRE_END_HEADER

#endif
