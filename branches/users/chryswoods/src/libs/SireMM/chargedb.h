#ifndef SIREMM_CHARGEDB_H
#define SIREMM_CHARGEDB_H

#include "SireDB/atomdb.h"

#include "chargeparameter.h"
#include "chargetable.h"

SIRE_BEGIN_HEADER

namespace SireMM
{

using SireDB::RelateID;
using SireDB::ParamID;
using SireDB::AssertMatch;
using SireDB::RelateIDMap;
using SireDB::ParameterTable;

/**
This forcefield database component is designed to hold the partial charge parameters used by the forcefield to calculate electrostatic interactions.

@author Christopher Woods
*/
class SIREMM_EXPORT ChargeDB : public SireDB::AtomDB
{
public:
    /** The ChargeDB stores ChargeParameter objects */
    typedef ChargeParameter parameter_type;

    /** A ChargeDB will place its parameters into an ChargeTable */
    typedef ChargeTable table_type;

    ChargeDB();
    ChargeDB(const ChargeDB &other);

    ~ChargeDB();

    /** Return the full name of this database component */
    static const char* typeName()
    {
        return "SireMM::ChargeDB";
    }

    const char* what() const
    {
        return ChargeDB::typeName();
    }

    void addCharge(const QString &userid, const ChargeParameter &charge);
    ChargeParameter getCharge(const QString &userid, bool *foundcharge = 0);

    void relateCharge(const AssertMatch<1> &matchatom, const QString &userid);
    void relateCharge(const AssertMatch<1> &matchatom, const ChargeParameter &charge);

    void relateCharge(RelateID relateid, const QString &userid);
    void relateCharge(RelateID relateid, const ChargeParameter &charge);

    ChargeParameter getCharge(RelateID relateid, bool *foundcharge = 0);
    ChargeParameter getCharge(const RelateIDMap &relateids, bool *foundcharge = 0);

    void createTable( ParameterTable &param_table ) const;

    bool assignParameter( const AtomIndex &atom, const RelateIDMap &relateids,
                          ParameterTable &param_table );

protected:
    void initialise();
    void prepareToDump();
    void postLoad();

    ChargeParameter retrieveCharge(ParamID id);
    ParamID addCharge(const ChargeParameter &charge);
};

}

Q_DECLARE_METATYPE(SireMM::ChargeDB)

SIRE_END_HEADER

#endif
