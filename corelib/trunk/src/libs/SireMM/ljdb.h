#ifndef SIREMM_LJDB_H
#define SIREMM_LJDB_H

#include "SireDB/atomdb.h"

#include "ljparameter.h"
#include "ljtable.h"

SIRE_BEGIN_HEADER

namespace SireMM
{

using SireDB::ParamID;
using SireDB::RelateID;
using SireDB::AssertMatch;
using SireDB::RelateIDMap;
using SireDB::ParameterTable;

/**
This forcefield database component is used to store Lennard Jones parameters, used to calculate the vdw interactions.
 
@author Christopher Woods
*/
class SIREMM_EXPORT LJDB : public SireDB::AtomDB
{
public:
    /** The LJDB stores LJParameter objects */
    typedef LJParameter parameter_type;
    
    /** A LJDB will place its parameters into an LJTable */
    typedef LJTable table_type;
    
    LJDB();
    LJDB(const LJDB &other);
    
    ~LJDB();

    /** Return the full name of this database component */
    static const char* typeName()
    {
        return "SireMM::LJDB";
    }

    const char* what() const
    {
        return LJDB::typeName();
    }

    void addLJ(const QString &userid, const LJParameter &ljparam);
    LJParameter getLJ(const QString &userid, bool *foundlj = 0);
    
    void relateLJ(const AssertMatch<1> &matchatom, const QString &userid);
    void relateLJ(const AssertMatch<1> &matchatom, const LJParameter &ljparam);
  
    void relateLJ(RelateID relateid, const QString &userid);
    void relateLJ(RelateID relateid, const LJParameter &ljparam);

    LJParameter getLJ(RelateID relateid, bool *foundlj = 0);
    LJParameter getLJ(const RelateIDMap &relateids, bool *foundlj = 0);

    LJTable& createTable( ParameterTable &param_table ) const;

    bool assignParameter( const AtomIndex &atom, const RelateIDMap &relateids,
                          ParameterTable &param_table );

protected:
    void initialise();
    void prepareToDump();
    void postLoad();
    
    ParamID addLJ(const LJParameter &ljparam);
    LJParameter retrieveLJ(ParamID id);
};

}

Q_DECLARE_METATYPE(SireMM::LJDB)

SIRE_END_HEADER

#endif
