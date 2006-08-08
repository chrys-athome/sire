#ifndef SIREDB_ATOMDB_H
#define SIREDB_ATOMDB_H

#include "SireDB/relatedb.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class AtomIndex;
}

namespace SireDB
{

class AtomTable;
class ParameterTable;

using SireMol::AtomIndex;

/**
This is the base class of all databases that store atom parameters. This provides a common interface, thus allowing all atom parameters to be assigned at once (rather than having to run through all atoms each time a new type of parameter is assigned).

@author Christopher Woods
*/
class SIREDB_EXPORT AtomDB : public RelateDB
{
public:
    AtomDB();
    AtomDB(const AtomDB &other);
    
    ~AtomDB();

    static const char* typeName()
    {
        return "SireDB::AtomDB";
    }

    virtual AtomTable& createTable( ParameterTable &param_table ) const=0;

    virtual bool assignParameter( const AtomIndex &atom, 
                                  const RelateIDMap &relateids,
                                  ParameterTable &param_table )=0;

protected:
    void prepareToDump();
    void postLoad();
};

}

SIRE_END_HEADER

#endif
