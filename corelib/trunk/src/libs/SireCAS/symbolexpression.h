#ifndef SIRECAS_SYMBOLEXPRESSION_H
#define SIRECAS_SYMBOLEXPRESSION_H

#include <qglobal.h>

#include "symbolvalue.h"
#include "symbolcomplex.h"
#include "expression.h"

SIRE_BEGIN_HEADER

namespace SireCAS
{

class Symbol;
class Function;

/** Small class that holds a SymbolID number and an associated expression */
class SIRECAS_EXPORT SymbolExpression
{
public:
    SymbolExpression(const ExpressionBase &symbol, const Expression &expression) 
                : _ex(expression), _sym(symbol)
    {}
    
    ~SymbolExpression()
    {}
    
    const Symbol& symbol() const;
    
    bool isFunction() const;
    const Function& function() const;
    
    const Expression& expression() const
    {
        return _ex;
    }
private:

    Expression _ex;
    ExpressionBase _sym;
};

}

SIRE_END_HEADER

#endif
