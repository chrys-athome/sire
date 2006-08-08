#ifndef SIREDB_RELATEDB_H
#define SIREDB_RELATEDB_H

#include "dbbase.h"
#include "matchdata.h"
#include "relateidmap.h"

SIRE_BEGIN_HEADER

namespace SireDB
{

/**
This is the database that is used to relate parameters to user-supplied identifiers, and also to relate RelateID numbers to parameters. This class must form the base class of any parameter database that wants to include complex relationships to parameters.

@author Christopher Woods
*/
class SIREDB_EXPORT RelateDB : public DBBase
{
public:
    RelateDB();
    RelateDB(const RelateDB &other);
    
    ~RelateDB();
    
    static const char* typeName()
    {
        return "SireDB::RelateDB";
    }
    
protected:
    void initialise();

    void prepareToDump();
    void postLoad();

    void relateParameter(const QString &userid, ParamID param);
    ParamID getParameter(const QString &userid);
    
    void relateParameter(RelateID relateid, ParamID param);
    void relateParameter(RelateID relateid, const QString &param);
    ParamID getParameter(RelateID relateid);
    
    ParamID getParameter(const RelateIDMap &relateids);

private:

    QString userIDToParamID() const;
    QString relateIDToParameter() const;

};

}

SIRE_END_HEADER

#endif
