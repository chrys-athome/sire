
#include "ljdb.h"
//#include "ljtable.h"

#include "SireDB/errors.h"
#include "SireDB/parametertable.h"

#include "SireMol/atomindex.h"

#include <QSqlQuery>

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireMM;
using namespace SireDB;
using namespace SireMol;

static const RegisterMetaType<LJDB> r_ljdb(NO_STREAM);

/** Constructor */
LJDB::LJDB() : AtomDB()
{}

/** Copy constructor */
LJDB::LJDB(const LJDB &other) : AtomDB(other)
{}

/** Destructor */
LJDB::~LJDB()
{}

/** This function creates all of the tables in this database used to store and
    find LJ parameters.

    The table used to do this is;

    table giving a unique index to each *LJ_PARAMETER* that has been loaded
*/
void LJDB::initialise()
{
    //create the RelateDB tables
    AtomDB::initialise();

    // (*) indicates column holds the primary key for the table

    //create table - table to give a unique ID number to each LJ parameter value
    //  columns  id_number(*)   sigma(float)  epsilon(float)
    QString error = executeSQL(
        "create table 'SireMM_LJDB' ( ParamID INTEGER, Sigma REAL, Epsilon REAL, "
                                     "Primary Key (ParamID), UNIQUE( Sigma, Epsilon) )");

    if (not error.isNull())
        throw SireDB::db_error(error, CODELOC);
}

/** Dump the version number of this database */
void LJDB::prepareToDump()
{
    AtomDB::prepareToDump();

    this->saveParameter<LJDB>( "version" , 1 );
}

/** Check the version number */
void LJDB::postLoad()
{
    AtomDB::postLoad();

    int v = this->loadParameter<LJDB>( "version" ).toInt();

    if (v != 1)
        throw version_error( v, "1", "SireMM::LJDB", CODELOC );
}

/** Add the LJParameter 'ljparam' to the database, returning the internal ID number
    of the parameter. This will add a new LJ parameter if it is not already added, or will
    return the id number of the already existing LJ parameter */
ParamID LJDB::addLJ(const LJParameter &ljparam)
{
    QSqlQuery q(database());

    //get the sigma and epsilon parameters
    double sigma = ljparam.sigma();
    double epsilon = ljparam.epsilon();

    q.exec( QString("select ParamID from SireMM_LJDB where Sigma = %1 and Epsilon = %2")
                  .arg(sigma).arg(epsilon) );
    checkErrors(q,CODELOC);
    q.next();

    if (q.isValid())
        //we have found the existing parameter - return the ID number
        return q.value(0).toUInt();
    else
    {
        //we need to insert the sigma and epsilon values, and then return the ID
        ParamID paramid = getNewParamID();

        q.exec( QString("insert into SireMM_LJDB values(%1, %2, %3)")
                    .arg(paramid).arg(sigma).arg(epsilon) );
        checkErrors(q,CODELOC);
        return paramid;
    }
}

/** Return the LJ parameter with ID 'ljid', or return a zero LJ parameter
    if it doesn't exist. Records in the log if there is a missing LJ parameter. */
LJParameter LJDB::retrieveLJ(ParamID ljid)
{
    if (ljid == 0)
        return LJParameter();
    else
    {
        QSqlQuery q(database());
        q.exec(QString("select Sigma, Epsilon from SireMM_LJDB where ParamID = %1")
                                                .arg(ljid));
        checkErrors(q,CODELOC);
        q.next();

        if (q.isValid())
            return LJParameter( q.value(0).toDouble(), q.value(1).toDouble() );
        else
        {
            addLog( QString("**WARNING** Missing LJ parameter with ParamID '%1'")
                          .arg(ljid) );
            return LJParameter();
        }
    }
}

/** Add the passed LJ parameter to the database and associate it with the
    passed user identification string 'userid' */
void LJDB::addLJ(const QString &userid, const LJParameter &ljparam)
{
    ParamID paramid = addLJ(ljparam);
    relateParameter(userid, paramid);
}

/** return the LJ parameter corresponding to the user ID string 'userid', or
    a zero parameter if there is no such parameter. 'foundlj' is set to whether
    or not a parameter was found. */
LJParameter LJDB::getLJ(const QString &userid, bool *foundlj)
{
    ParamID paramid = getParameter(userid);

    if (foundlj)
        *foundlj = (paramid != 0);

    return retrieveLJ(paramid);
}

/** Relate the LJ parameter associated with the user ID string 'userid' with
    the atom matching criteria in 'matchatom' */
void LJDB::relateLJ(const AssertMatch<1> &matchatom, const QString &userid)
{
    RelateID relateid = matchatom.addTo(parent());

    relateParameter(relateid, userid);
}

/** Relate the LJ parameter 'ljparam' to the atom matching criteria in 'matchatom' */
void LJDB::relateLJ(const AssertMatch<1> &matchatom, const LJParameter &ljparam)
{
    RelateID relateid = matchatom.addTo(parent());

    //add the parameter
    ParamID paramid = addLJ(ljparam);

    relateParameter(relateid, paramid);
}

/** Relate the LJ parameter identified by the user ID string 'userid' to the matching criteria
    identified by the relationship ID 'relateid' */
void LJDB::relateLJ(RelateID relateid, const QString &userid)
{
    relateParameter(relateid, userid);
}

/** Relate the LJ parameter 'ljparam' to the matching criteria identified by the relationship
    ID 'relateid' */
void LJDB::relateLJ(RelateID relateid, const LJParameter &ljparam)
{
    //add the parameter
    ParamID paramid = addLJ(ljparam);
    //associate this relationship with the parameter
    relateParameter(relateid, paramid);
}

/** Return the LJ parameter associated with the relationship ID 'relateid', or a
    zero LJParameter if there is no such parameter. Set 'foundlj' to whether or
    not a parameter was found. */
LJParameter LJDB::getLJ(RelateID relateid, bool *foundlj)
{
    ParamID paramid = getParameter(relateid);

    if (foundlj)
        *foundlj = (paramid != 0);

    return retrieveLJ(paramid);
}

/** Return the LJ parameter associated with the relationship IDs in 'relateids',
    or a zero LJParameter if there is no such parameter. Set 'foundlj' to whether
    or not a parameter was found. */
LJParameter LJDB::getLJ(const RelateIDMap &relateids, bool *foundlj)
{
    ParamID paramid = getParameter(relateids);

    if (foundlj)
        *foundlj = (paramid != 0);

    return retrieveLJ(paramid);
}

/** Create a table in 'param_table' that is capable of holding the LJ
    parameters - this does nothing if there is already such a table.

    This returns a reference to the created table.
*/
void LJDB::createTable( ParameterTable &param_table ) const
{
    param_table.createTable<LJTable>();
}

/** Assign the LJ parameter for the atom 'atom' using the relationship IDs
    in 'relateids' and place the parameter into the parameter table. This does
    nothing if the parameter is not found. This returns whether or not the
    parameter was found. */
bool LJDB::assignParameter( const AtomIndex &atom, const RelateIDMap &relateids,
                            ParameterTable &param_table )
{
/*    bool found;
    LJParameter lj = getLJ(relateids, &found);

    if (found)
    {
        LJTable &table = param_table.addTable<LJTable>();
        table.setLJ(atom, lj);
    }

    return found;*/

    return false;
}
