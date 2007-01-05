#ifndef SIRECAS_EXPRESSIONS_H
#define SIRECAS_EXPRESSIONS_H

#include  <QList>

#include "expression.h"

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireCAS
{

class Expression;

/**
Expressions provides a list of expressions.

@author Christopher Woods
*/
class SIRECAS_EXPORT Expressions : public QList<Expression>
{
public:
    Expressions();

    Expressions(const Expression &expression);

    Expressions(const QList<Expression> &expressions);

    ~Expressions();

    Expressions differentiate(const Symbol &symbol) const;
    Expressions integrate(const Symbol &symbol) const;

};

}

SIRE_END_HEADER

#endif
