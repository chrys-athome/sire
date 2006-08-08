#ifndef SIREDB_TERM12DB_H
#define SIREDB_TERM12DB_H

#include "funcdb.h"
#include "matchdata.h"

SIRE_BEGIN_HEADER

namespace SireDB
{

/**
A Term12DB is a database of 1-2 interatomic terms (e.g. bond terms).
 
@author Christopher Woods
*/
class SIREDB_EXPORT Term12DB : public FuncDB
{
public:
    Term12DB();
    Term12DB(const Term12DB &other);
    
    ~Term12DB();

    static const char* typeName()
    {
        return "SireDB::Term12DB";
    }

protected:
    void prepareToDump();
    void postLoad();

    void relateFunc(const AssertMatch<2> &matchpair, const QString &userid,
                    bool mirror=true);
                    
    void relateFunc(const AssertMatch<2> &matchpair, const Expression &func,
                    bool mirror=true);
};

}

SIRE_END_HEADER

#endif
