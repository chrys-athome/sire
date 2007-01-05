
#include "expression.h"
#include "expressions.h"

using namespace SireCAS;

Expressions::Expressions() : QList<Expression>()
{}

Expressions::Expressions(const Expression &expression) : QList<Expression>()
{
    this->append(expression);
}

Expressions::Expressions(const QList<Expression> &expressions)
            : QList<Expression>(expressions)
{}

Expressions::~Expressions()
{}

Expressions Expressions::differentiate(const Symbol &symbol) const
{
    Expressions diffs;

    int sz = count();

    for (int i=0; i<sz; ++i)
    {
        Expression diff = at(i).differentiate(symbol);

        if (not diff.isZero())
            diffs.append(diff);
    }

    return diffs;
}

Expressions Expressions::integrate(const Symbol &symbol) const
{
    Expressions ints;

    int sz = count();

    for (int i=0; i<sz; ++i)
    {
        Expression integ = at(i).integrate(symbol);

        if (not integ.isZero())
            ints.append(integ);
    }

    return ints;
}
