#ifndef SIRECAS_IDENTITIES_H
#define SIRECAS_IDENTITIES_H

#include <QHash>
#include <QSet>
#include <QList>

#include "symbolexpression.h"
#include "functionsignature.h"

SIRE_BEGIN_HEADER

namespace SireCAS
{
class Identities;
}

class QDataStream;
QDataStream& operator<<(QDataStream&, const SireCAS::Identities&);
QDataStream& operator>>(QDataStream&, SireCAS::Identities&);

namespace SireCAS
{

/**
This class holds a collection of identities. These are symbols, and the expressions that they are equal to, e.g. x == y.squared() + sin(z). These identities can then be substituted into expressions, with all instances of the variable being replaced by the equivalent expression.

@author Christopher Woods
*/
class SIRECAS_EXPORT Identities
{

friend QDataStream& ::operator<<(QDataStream&, const Identities&);
friend QDataStream& ::operator>>(QDataStream&, Identities&);

public:
    Identities();
    Identities(const QList<SymbolExpression> &expressions);
    Identities(const SymbolExpression &symex0);
    Identities(const SymbolExpression &symex0, const SymbolExpression &symex1);
    Identities(const SymbolExpression &symex0, const SymbolExpression &symex1,
               const SymbolExpression &symex2);
    Identities(const SymbolExpression &symex0, const SymbolExpression &symex1,
               const SymbolExpression &symex2, const SymbolExpression &symex3);
    Identities(const SymbolExpression &symex0, const SymbolExpression &symex1,
               const SymbolExpression &symex2, const SymbolExpression &symex3,
               const SymbolExpression &symex4);
    Identities(const SymbolExpression &symex0, const SymbolExpression &symex1,
               const SymbolExpression &symex2, const SymbolExpression &symex3,
               const SymbolExpression &symex4, const SymbolExpression &symex5);
    Identities(const SymbolExpression &symex0, const SymbolExpression &symex1,
               const SymbolExpression &symex2, const SymbolExpression &symex3,
               const SymbolExpression &symex4, const SymbolExpression &symex5,
               const SymbolExpression &symex6);
    Identities(const SymbolExpression &symex0, const SymbolExpression &symex1,
               const SymbolExpression &symex2, const SymbolExpression &symex3,
               const SymbolExpression &symex4, const SymbolExpression &symex5,
               const SymbolExpression &symex6, const SymbolExpression &symex7);
    Identities(const SymbolExpression &symex0, const SymbolExpression &symex1,
               const SymbolExpression &symex2, const SymbolExpression &symex3,
               const SymbolExpression &symex4, const SymbolExpression &symex5,
               const SymbolExpression &symex6, const SymbolExpression &symex7,
               const SymbolExpression &symex8);
    Identities(const SymbolExpression &symex0, const SymbolExpression &symex1,
               const SymbolExpression &symex2, const SymbolExpression &symex3,
               const SymbolExpression &symex4, const SymbolExpression &symex5,
               const SymbolExpression &symex6, const SymbolExpression &symex7,
               const SymbolExpression &symex8, const SymbolExpression &symex9);
    
    Identities(const Identities &other);
    
    ~Identities();

    void add(const SymbolExpression &symex0);
    void add(const SymbolExpression &symex0, const SymbolExpression &symex1);
    void add(const SymbolExpression &symex0, const SymbolExpression &symex1,
             const SymbolExpression &symex2);
    void add(const SymbolExpression &symex0, const SymbolExpression &symex1,
             const SymbolExpression &symex2, const SymbolExpression &symex3);
    void add(const SymbolExpression &symex0, const SymbolExpression &symex1,
             const SymbolExpression &symex2, const SymbolExpression &symex3,
             const SymbolExpression &symex4);
    void add(const SymbolExpression &symex0, const SymbolExpression &symex1,
             const SymbolExpression &symex2, const SymbolExpression &symex3,
             const SymbolExpression &symex4, const SymbolExpression &symex5);
    void add(const SymbolExpression &symex0, const SymbolExpression &symex1,
             const SymbolExpression &symex2, const SymbolExpression &symex3,
             const SymbolExpression &symex4, const SymbolExpression &symex5,
             const SymbolExpression &symex6);
    void add(const SymbolExpression &symex0, const SymbolExpression &symex1,
             const SymbolExpression &symex2, const SymbolExpression &symex3,
             const SymbolExpression &symex4, const SymbolExpression &symex5,
             const SymbolExpression &symex6, const SymbolExpression &symex7);
    void add(const SymbolExpression &symex0, const SymbolExpression &symex1,
             const SymbolExpression &symex2, const SymbolExpression &symex3,
             const SymbolExpression &symex4, const SymbolExpression &symex5,
             const SymbolExpression &symex6, const SymbolExpression &symex7,
             const SymbolExpression &symex8);
    void add(const SymbolExpression &symex0, const SymbolExpression &symex1,
             const SymbolExpression &symex2, const SymbolExpression &symex3,
             const SymbolExpression &symex4, const SymbolExpression &symex5,
             const SymbolExpression &symex6, const SymbolExpression &symex7,
             const SymbolExpression &symex8, const SymbolExpression &symex9);

    bool contains(const Symbol &sym) const;
    Expression expression(const Symbol &sym) const;
    
    bool contains(const Function &func) const;
    Function function(const Function &func) const;

    void set( const Symbol &symbol, const Expression &expression );

private:

    /** Hash mapping a symbol to an expression */
    QHash<SymbolID, Expression> idhash;
    
    /** Hash mapping the signatures of functions to the actual function
        stored in this collection of identities */
    QHash<FunctionSignature, Expression> funchash;
};

/** Add the SymbolExpression to this set of identities */
inline void Identities::add(const SymbolExpression &symex0)
{
    set( symex0.symbol(), symex0.expression() );
}

}

Q_DECLARE_METATYPE(SireCAS::Identities)

SIRE_END_HEADER

#endif
