
#include "symbolexpression.h"
#include "symbol.h"
#include "function.h"

using namespace SireCAS;

/** Return the symbol */
const Symbol& SymbolExpression::symbol() const
{
    BOOST_ASSERT( _sym.isA<Symbol>() );
    return _sym.asA<Symbol>();
}
    
/** Return whether or not this is a function */
bool SymbolExpression::isFunction() const
{
    return _sym.isA<Function>();
}

/** Return the symbol as a function - this is only safe
    if 'isFunction()' returns true */
const Function& SymbolExpression::function() const
{
    BOOST_ASSERT( _sym.isA<Function>() );
    return _sym.asA<Function>();
}
