#ifndef SIREDB_TERM14DB_H
#define SIREDB_TERM14DB_H

#include "funcdb.h"
#include "matchdata.h"

SIRE_BEGIN_HEADER

namespace SireDB
{

/**
The Term14DB is a database of 1-4 interatomic interaction functions (e.g. dihedrals)
 
@author Christopher Woods
*/
class SIREDB_EXPORT Term14DB : public FuncDB
{
public:
    Term14DB();
    Term14DB(const Term14DB &other);
    
    ~Term14DB();

    static const char* typeName()
    {
        return "SireDB::Term14DB";
    }

protected:
    void prepareToDump();
    void postLoad();

    void relateFunc(const AssertMatch<4> &matchtriple, const QString &userid,
                    bool mirrored=true);
    void relateFunc(const AssertMatch<4> &matchtriple, const Expression &func,
                    bool mirrored=true);

};

}

SIRE_END_HEADER

#endif
