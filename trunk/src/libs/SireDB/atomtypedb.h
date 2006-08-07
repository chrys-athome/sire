#ifndef SIREDB_ATOMTYPEDB_H
#define SIREDB_ATOMTYPEDB_H

#include "atomdb.h"
#include "atomtype.h"
#include "atomtypetable.h"

SIRE_BEGIN_HEADER

namespace SireDB
{

/**
This database is used to store and retrieve atom types.

@author Christopher Woods
*/
class SIREDB_EXPORT AtomTypeDB : public AtomDB
{
public:
    /** An AtomTypeDB stores AtomType objects */
    typedef AtomType parameter_type;
    
    /** An AtomTypeDB will place its parameters into an AtomTypeTable */
    typedef AtomTypeTable table_type;

    AtomTypeDB();
    AtomTypeDB(const AtomTypeDB &other);
    
    ~AtomTypeDB();

    /** Return the full name of this database component */
    static const char* typeName()
    {
        return "SireDB::AtomTypeDB";
    }

    const char* what() const
    {
        return AtomTypeDB::typeName();
    }

    void addAtomType(const QString &userid, const AtomType &atomtype);
    AtomType getAtomType(const QString &userid, bool *foundtype = 0);
    
    void relateAtomType(const AssertMatch<1> &matchatom, const QString &userid);
    void relateAtomType(const AssertMatch<1> &matchatom, const AtomType &atomtype);

    void relateAtomType(RelateID relateid, const QString &userid);
    void relateAtomType(RelateID relateid, const AtomType &atomtype);

    AtomType getAtomType(RelateID relateid, bool *foundtype = 0);
    AtomType getAtomType(const RelateIDMap &relateids, bool *foundtype = 0);

    AtomTypeTable& createTable( ParameterTable &param_table ) const;

    bool assignParameter( const AtomIndex &atom, const RelateIDMap &relateids,
                          ParameterTable &param_table );

protected:
    void initialise();
    void prepareToDump();
    void postLoad();
    
    AtomType retrieveAtomType(ParamID id);
    ParamID addAtomType(const AtomType &atomtype);
    
};

}

Q_DECLARE_METATYPE(SireDB::AtomTypeDB)

SIRE_END_HEADER

#endif
