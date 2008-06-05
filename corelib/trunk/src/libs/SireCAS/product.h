#ifndef SIRECAS_PRODUCT_H
#define SIRECAS_PRODUCT_H

#include <QHash>

#include "exbase.h"
#include "expression.h"

SIRE_BEGIN_HEADER

namespace SireCAS
{
class Product;
}

QDataStream& operator<<(QDataStream&, const SireCAS::Product&);
QDataStream& operator>>(QDataStream&, SireCAS::Product&);

namespace SireCAS
{

/**
This class holds a collection of expressions that are to be multiplied (or divided)

@author Christopher Woods
*/
class SIRECAS_EXPORT Product : public ExBase
{

friend QDataStream& ::operator<<(QDataStream&, const Product&);
friend QDataStream& ::operator>>(QDataStream&, Product&);

public:
    Product();
    Product(const Expression &ex0, const Expression &ex1);
    Product(const Expressions &expressions);
    
    Product(const Product &other);
    
    ~Product();

    Expression differentiate(const Symbol &symbol) const;
    Expression integrate(const Symbol &symbol) const;
    
    Expression series(const Symbol &symbol, int n) const;
    
    Expression simplify(int options=0) const;
    
    Expression expand() const;
    Expression collapse() const;
    Expression conjugate() const;
    
    bool isFunction(const Symbol&) const;
    bool isConstant() const;
    bool isComplex() const;
    bool isCompound() const;

    bool operator==(const ExBase &other) const;
    
    uint hash() const;

    const char* what() const
    {
        return "SireCAS::Product";
    }

    QString toString() const;

    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;
    
    Expression substitute(const Identities &identities) const;
    
    Symbols symbols() const;
    Functions functions() const;
    Expressions children() const;

    Expression reduce() const;

    Product numerator() const;
    Product denominator() const;

protected:
    ExBase* clone() const
    {
        return new Product(*this);
    }

private:

    void rebuild();

    Expression take(const Expression &core);
    Expression takeFirst(const Symbol &symbol);

    void multiply(const Expression &ex);

    void multiply(double val);
    void multiply(const Complex &complex);
    void multiply(double val, const Expression &power);
    void multiply(const Complex &complex, const Expression &power);
    void multiply(const Product &product, const Expression &power);
    void multiply(const ExpressionBase &core, const Expression &power);

    void multiplyPvt(const Expression &ex, const Expression &power);

    bool isPureProduct() const;
    
    static Expression productRule(Product product, const Symbol &symbol);
    static Expression quotientRule(const Product &f, const Product &g, const Symbol &symbol);

    /** The collection of powers of each part of the Product, index
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

SIRE_END_HEADER

#endif
