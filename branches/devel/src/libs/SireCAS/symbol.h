#ifndef SIRECAS_SYMBOL_H
#define SIRECAS_SYMBOL_H

#include <QString>

#include "exbase.h"
#include "symbolvalue.h"
#include "symbolcomplex.h"
#include "symbolexpression.h"

SIRE_BEGIN_HEADER

namespace SireCAS
{
class Symbol;
}

QDataStream& operator<<(QDataStream&, const SireCAS::Symbol&);
QDataStream& operator>>(QDataStream&, SireCAS::Symbol&);

namespace SireCAS
{

class Symbols;
class Values;

/**
This class represents an algebraic symbol in the equation (e.g. 'x' or 'y')

@author Christopher Woods
*/
class SIRECAS_EXPORT Symbol : public ExBase
{

friend QDataStream& ::operator<<(QDataStream&, const Symbol&);
friend QDataStream& ::operator>>(QDataStream&, Symbol&);

public:
    Symbol();
    Symbol(const QString &rep);
    
    Symbol(const Symbol &other);
    
    ~Symbol();

    Symbol& operator=(const Symbol &other);

    /** Return the unique ID number of the symbol */
    SymbolID ID() const
    {
        return id;
    }
    
    /** Convienient operator used to combine a symbol with a value */
    SymbolValue operator==(double val) const
    {
        return SymbolValue(ID(), val);
    }
    
    /** Convienient operator used to combine a symbol with a value */
    SymbolValue operator==(int val) const
    {
        return SymbolValue(ID(), val);
    }
    
    /** Convienient operator used to combine a symbol with a Complex */
    SymbolComplex operator==(const Complex &val) const
    {
        return SymbolComplex(ID(), val);
    }
    
    /** Convieient operator used to combine a symbol with an equivalent 
        expression */
    SymbolExpression operator==(const Expression &ex) const
    {
        return SymbolExpression(*this, ex);
    }
    
    Expression differentiate(const Symbol &symbol) const;
    Expression integrate(const Symbol &symbol) const;
    
    bool isFunction(const Symbol&) const;
    bool isConstant() const;

    bool operator==(const ExBase &other) const;
    
    uint hash() const;

    const char* what() const
    {
        return "SireCAS::Symbol";
    }

    QString toString() const;

    double evaluate(const Values &values) const;
    Complex evaluate(const ComplexValues &values) const;
    
    Expression substitute(const Identities &identities) const;
    
    Symbols symbols() const;
    Functions functions() const;
    Expressions children() const;

protected:

    static SymbolID getNewID(const QString &symbol);
    
    ExBase* clone() const
    {
        return new Symbol(*this);
    }

    /** Unique ID number that is given to every symbol */
    SymbolID id;

    /** String representation of this symbol */
    QString stringrep;
};

}

inline uint qHash(const SireCAS::Symbol &symbol)
{
    return symbol.hash();
}

Q_DECLARE_METATYPE(SireCAS::Symbol)

SIRE_END_HEADER

#endif
