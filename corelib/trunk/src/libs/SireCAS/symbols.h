#ifndef SIRECAS_SYMBOLS_H
#define SIRECAS_SYMBOLS_H

#include <QSet>

#include "symbol.h"

SIRE_BEGIN_HEADER

namespace SireCAS
{

/** Trival derivation of QSet<Symbol> that adds a constructor that
    automatically adds the passed Symbol 
    
    @author Christopher Woods
*/
class SIRECAS_EXPORT Symbols : public QSet<Symbol>
{
public:
    Symbols() : QSet<Symbol>()
    {}
    
    Symbols(const Symbol &sym) : QSet<Symbol>()
    {
        this->insert(sym);
    }
    
    Symbols(const QSet<Symbol> &other) : QSet<Symbol>(other)
    {}
    
    Symbols(const QList<Symbol> &other) : QSet<Symbol>()
    {
        int n = other.count();
        for (int i=0; i<n; ++i)
            insert( other.at(i) );
    }
    
    ~Symbols()
    {}
    
    void insert(const Symbol &symbol)
    {
        QSet<Symbol>::insert(symbol);
    }
    
    void insert(const Symbols &symbols)
    {
        for (Symbols::const_iterator it = symbols.begin();
             it != symbols.end();
             ++it)
        {
            this->insert(*it);
        }
    }
};

}

SIRE_END_HEADER

#endif
