
#include "chargedb.h"
#include "chargetable.h"

#include "SireDB/errors.h"
#include "SireDB/parametertable.h"

#include "SireMol/atomindex.h"

#include <QSqlQuery>

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireMM;
using namespace SireDB;
using namespace SireMol;

static const RegisterMetaType<ChargeDB> r_chargedb("SireMM::ChargeDB", NO_STREAM);

/** Constructor */
ChargeDB::ChargeDB() : AtomDB()
{}

/** Copy constructor */
ChargeDB::ChargeDB(const ChargeDB &other) : AtomDB(other)
{}

/** Destructor */
ChargeDB::~ChargeDB()
{}

/** This function creates all of the tables in this database used to store and 
    find charge parameters. There is only one table in this component.

    (1) table giving a unique parameter ID to each *CHARGE* that has been loaded
*/
void ChargeDB::initialise()
{
    //create the RelateDB tables...
    AtomDB::initialise();
    
    // (*) indicates column holds the primary key for the table

    //create table (1) - table to give a unique ID number to each charge value
    //  columns  id_number(*)   charge (float)
    QString error = executeSQL(
        "create table 'SireMM_ChargeDB' ( ParamID INTEGER, Charge REAL, "
                                         "Primary Key (ParamID), UNIQUE (Charge) )");

    if (not error.isNull())
        throw SireDB::db_error(error, CODELOC);
}
    
/** Dump the version number of this database */
void ChargeDB::prepareToDump()
{
    AtomDB::prepareToDump();
    
    this->saveParameter<ChargeDB>( "version" , 1 );
}

/** Check the version number */
void ChargeDB::postLoad()
{
    AtomDB::postLoad();

    int v = this->loadParameter<ChargeDB>( "version" ).toInt();
                            
    if (v != 1)
        throw version_error( v, "1", "SireMM::ChargeDB", CODELOC );
}

/** Add the charge 'chg' to the database, returning the internal ID number of the charge
    parameter. This will add a new charge parameter if this charge does not exist, or will
    return the id number of the already existing charge parameter */
ParamID ChargeDB::addCharge(const ChargeParameter &chg)
{
    QSqlQuery q(database());
    
    q.exec(QString("select ParamID from 'SireMM_ChargeDB' where Charge = %1")
                                          .arg(chg.charge()));
    checkErrors(q,CODELOC);
    q.next();
    
    if (q.isValid())
        //we have found the existing charge parameter - return the ID number
        return q.value(0).toUInt();
    else
    {
        //we need to insert the charge, and then return the ID
        ParamID paramid = getNewParamID();
        q.exec(QString("insert into 'SireMM_ChargeDB' values (%1, %2)")
                                              .arg(paramid).arg(chg.charge()));
        checkErrors(q,CODELOC);
        return paramid;
    }
}

/** Return the charge with parameter ID 'chgid', or record the missing parameter 
    in the log and return '0.0' */
ChargeParameter ChargeDB::retrieveCharge(ParamID chgid)
{
    if (chgid == 0)
        return 0.0;
    else
    {
        QSqlQuery q(database());
        q.exec(QString("select Charge from 'SireMM_ChargeDB' where ParamID = %1")
                                                .arg(chgid));
        checkErrors(q,CODELOC);
        q.next();
    
        if (q.isValid())
            return q.value(0).toDouble();
        else
        {
            addLog( QString("**WARNING** Missing charge parameter with ParamID '%1'")
                          .arg(chgid) );
            return 0.0;
        }
    }
}

/** Add a charge to the database, associated with the user ID string 'userid' */
void ChargeDB::addCharge(const QString &userid, const ChargeParameter &charge)
{
    //add the charge...
    ParamID paramid = addCharge(charge);
    
    //associate this parameter ID with the userid
    relateParameter(userid, paramid);
}

/** Return the charge with userid 'userid'. Returns '0.0' if there is no
    such parameter, and if 'foundcharge' is not null, then it will be 
    set to false. */
ChargeParameter ChargeDB::getCharge(const QString &userid, bool *foundcharge)
{
    //get the parameter ID for this userid
    ParamID paramid = getParameter(userid);
    
    //set the flag saying whether or not the parameter exists
    if (foundcharge)
        *foundcharge = (paramid != 0);
        
    return retrieveCharge(paramid);
}
    
/** Relate the charge identified by user ID string 'userid' to the atom matching 
    criteria 'matchatom' */
void ChargeDB::relateCharge(const AssertMatch<1> &matchatom, const QString &userid)
{
    //get the relationship ID of this match
    RelateID relateid = matchatom.addTo(parent());
    
    //associate this relationship with the userID
    relateParameter(relateid, userid);
}

/** Relate the charge 'charge' to the atom matching criteria 'matchatom' */
void ChargeDB::relateCharge(const AssertMatch<1> &matchatom, const ChargeParameter &charge)
{
    //get the relationship ID of this match
    RelateID relateid = matchatom.addTo(parent());
    
    //add the charge
    ParamID paramid = addCharge(charge);
    
    //associate this relationship with the parameter
    relateParameter(relateid, paramid);
}

/** Relate the charge identified by the user ID string 'userid' to the matching criteria
    identified by the relationship ID 'relateid' */
void ChargeDB::relateCharge(RelateID relateid, const QString &userid)
{
    relateParameter(relateid, userid);
}

/** Relate the charge 'charge' to the matching criteria identified by the relationship
    ID 'relateid' */
void ChargeDB::relateCharge(RelateID relateid, const ChargeParameter &charge)
{
    //add the charge
    ParamID paramid = addCharge(charge);
    //associate this relationship with the parameter
    relateParameter(relateid, paramid);
}

/** Return the charge that matches the relationship ID 'relateid'. Returns
    0.0 if there is no such charge, and sets 'foundcharge' to true or false depending
    on whether or not a charge was found. */
ChargeParameter ChargeDB::getCharge(RelateID relateid, bool *foundcharge)
{
    //get the paramID of the relationship
    ParamID paramid = getParameter(relateid);
    
    //set the flag saying whether or not the parameter was found
    if (foundcharge)
        *foundcharge = (paramid != 0);
        
    //return the charge
    return retrieveCharge(paramid);
}

/** Return the best charge matching the relationship IDs in 'relateids'. Returns
    0.0 if there is no such charge. Sets 'foundcharge' to whether or not 
    a charge was found. */
ChargeParameter ChargeDB::getCharge(const RelateIDMap &relateids, bool *foundcharge)
{
    //get the ParamID of the highest scoring and matching relationship
    ParamID paramid = getParameter(relateids);
    
    //set the flag saying whether or not the parameter was found
    if (foundcharge)
        *foundcharge = (paramid != 0);
        
    //return the charge
    return retrieveCharge(paramid);
}

/** Create a table in 'param_table' that is capable of holding the charge 
    parameters - this does nothing if there is already such a table.
     
    This returns a reference to the created table.
*/
ChargeTable& ChargeDB::createTable( ParameterTable &param_table ) const
{
    return param_table.addTable<ChargeTable>();
}

/** Assign the charge parameter for the atom 'atom' using the relationship IDs
    in 'relateids' and place the parameter into the parameter table. This does
    nothing if the parameter is not found. This returns whether or not the
    parameter was found. */
bool ChargeDB::assignParameter( const AtomIndex &atom, const RelateIDMap &relateids,
                                ParameterTable &param_table )
{
    bool found;
    ChargeParameter chg = getCharge(relateids, &found);
    
    if (found)
    {
        ChargeTable &table = param_table.addTable<ChargeTable>();
        table.setCharge(atom, chg);
    }
    
    return found;
}
