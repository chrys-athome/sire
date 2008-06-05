#ifndef SIREDB_TERM13DB_H
#define SIREDB_TERM13DB_H

#include "funcdb.h"
#include "matchdata.h"

SIRE_BEGIN_HEADER

namespace SireDB
{

/**
This class represents a databases of 1-3 interaction functions (e.g. angle functions).
 
@author Christopher Woods
*/
class SIREDB_EXPORT Term13DB : public FuncDB
{
public:
    Term13DB();
    Term13DB(const Term13DB &other);
    
    ~Term13DB();

    static const char* typeName()
    {
        return "SireDB::Term13DB";
    }

protected:
    void prepareToDump();
    void postLoad();

    void relateFunc(const AssertMatch<3> &matchtriple, const QString &userid,
                    bool mirrored=true);
    void relateFunc(const AssertMatch<3> &matchtriple, const Expression &func,
                    bool mirrored=true);

};

}

SIRE_END_HEADER

#endif
