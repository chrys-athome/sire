#ifndef SIREDB_ATOMDBWrapper_H
#define SIREDB_ATOMDBWrapper_H

#include "dbwrapper.h"
#include "relateidmap.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class AtomIndex;
}

namespace SireDB
{

class ParameterTable;

using SireMol::AtomIndex;

/** This is a specialisation of DBPtr that exposes the functions that 
    access the atom-parameter functionality of the database. This provides
    the interface that is used by assign_atoms to assign atom parameters
    from this database component
    
    @author Christopher Woods
*/
class SIREDB_EXPORT AtomDBWrapper : public DBWrapper
{
public:
    AtomDBWrapper() : DBWrapper()
    {}
    
    ~AtomDBWrapper()
    {}
    
    virtual AtomTable& createTable(ParameterTable &param_table) const=0;
    
    virtual bool assignedParameter(const AtomIndex &atom,
                                   const AtomTable &atomtable) const=0;
    
    virtual bool assignParameter(const AtomIndex &atom,
                                 const RelateIDMap &relateids,
                                 AtomTable &atomtable)=0;
};

}

SIRE_END_HEADER

#endif
