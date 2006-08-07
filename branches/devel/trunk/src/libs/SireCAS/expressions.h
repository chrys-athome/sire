#ifndef SIRECAS_EXPRESSIONS_H
#define SIRECAS_EXPRESSIONS_H

#include  <QList>

#include "expression.h"

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireCAS
{

/**
Expressions provides a list of expressions.

@author Christopher Woods
*/
class SIRECAS_EXPORT Expressions : public QList<Expression>
{
public:
    Expressions() : QList<Expression>()
    {}
    
    Expressions(const Expression &expression) : QList<Expression>()
    {
        this->append(expression);
    }
    
    Expressions(const QList<Expression> &expressions)
             : QList<Expression>(expressions)
    {}
    
    ~Expressions()
    {}

    Expressions differentiate(const Symbol &symbol) const
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

};

}

SIRE_END_HEADER

#endif
