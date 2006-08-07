
#include "qhash_sirecas.h"

#include "functions.h"

using namespace SireCAS;

/** Return the set of all the names of the functions in this set */
QSet<QString> Functions::functionNames() const
{
    QSet<QString> funcnames;
    
    for (Functions::const_iterator it = this->begin();
         it != this->end();
         ++it)
    {
        funcnames.insert( it->name() );
    }
        
    return funcnames;
}
