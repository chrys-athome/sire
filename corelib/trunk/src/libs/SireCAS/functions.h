#ifndef SIRECAS_FUNCTIONS_H
#define SIRECAS_FUNCTIONS_H

#include <QSet>

#include "function.h"

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireCAS
{

/** Trival derivation of QSet<Function> that adds a constructor that
    automatically adds the passed Function. There are additional functions
    that retrieve all of the function names. 
    
    @author Christopher Woods
*/
class SIRECAS_EXPORT Functions : public QSet<Function>
{
public:
    Functions() : QSet<Function>()
    {}
    
    Functions(const Function &func) : QSet<Function>()
    {
        this->insert(func);
    }
    
    Functions(const QSet<Function> &other) : QSet<Function>(other)
    {}
    
    Functions(const QList<Function> &other) : QSet<Function>()
    {
        int n = other.count();
        for (int i=0; i<n; ++i)
            insert( other.at(i) );
    }
    
    ~Functions()
    {}
    
    void insert(const Function &func)
    {
        QSet<Function>::insert(func);
    }
    
    void insert(const Functions &funcs)
    {
        for (Functions::const_iterator it = funcs.begin();
             it != funcs.end();
             ++it)
        {
            this->insert(*it);
        }
    }
    
    QSet<QString> functionNames() const;
};

}

SIRE_END_HEADER

#endif
