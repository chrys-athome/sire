#ifndef SIRECAS_SUM_H
#define SIRECAS_SUM_H

#include <QHash>

#include "exbase.h"
#include "expression.h"

SIRE_BEGIN_HEADER

namespace SireCAS
{
class Sum;
}

QDataStream& operator<<(QDataStream&, const SireCAS::Sum&);
QDataStream& operator>>(QDataStream&, SireCAS::Sum&);

namespace SireCAS
{

/**
This class holds a collection of expressions that are to be added (or subtracted) from one another

@author Christopher Woods
*/
class SIRECAS_EXPORT Sum : public ExBase
{

friend QDataStream& ::operator<<(QDataStream&, const Sum&);
friend QDataStream& ::operator>>(QDataStream&, Sum&);

public:
    Sum();
    Sum(const Expression &ex0, const Expression &ex1);
    Sum(const Expressions &expressions);

    Sum(const Sum &other);

    ~Sum();

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

    static const char* typeName()
    {
        return "SireCAS::Sum";
    }

    const char* what() const
    {
        return Sum::typeName();
    }

    QString toString() const;

    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;

    Expression substitute(const Identities &identities) const;

    Symbols symbols() const;
    Functions functions() const;
    Expressions children() const;

    Expression reduce() const;

    Sum* clone() const
    {
        return new Sum(*this);
    }

private:

    void add(const Expression &ex);

    Expression take(const ExpressionBase &ex);
    void add(double fac, const ExpressionBase &ex);

    /** The positive parts of the sum */
    QHash<ExpressionBase, Expression> posparts;

    /** The negative parts of the sum */
    QHash<ExpressionBase, Expression> negparts;

    /** The start value of the sum */
    double strtval;
};

}

Q_DECLARE_METATYPE(SireCAS::Sum)

SIRE_END_HEADER

#endif
