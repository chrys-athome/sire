
#include "relatedb.h"

#include "SireStream/version_error.h"

#include <boost/assert.hpp>
#include <QSqlQuery>
#include <QDebug>

using namespace SireDB;
using namespace SireStream;

/** Constructor */
RelateDB::RelateDB() : DBBase()
{}

/** Copy constructor */
RelateDB::RelateDB(const RelateDB &other) : DBBase(other)
{}

/** Destructor */
RelateDB::~RelateDB()
{}

/** Return the name of the table used to map user ID strings to parameter IDs */
QString RelateDB::userIDToParamID() const
{
    return QString("%1_SireDB_RelateDB_UserID_to_ParamID").arg(prefix());
}

/** Return the name of the table used to map relationship IDs to user IDs */
QString RelateDB::relateIDToParameter() const
{
    return QString("%1_SireDB_RelateDB_RelateID_to_Parameter").arg(prefix());
}

/** Create the tables used by this database. There are two tables;

    (1) userIDToParamID : Maps user-supplied parameter identification 
                          strings (userids) to specific parameter ID 
                          numbers.
                          
    (2) relateIDToParameter : Maps a specific relationship ID to a user-supplied
                              parameter identification string or a specific parameter ID
                            
*/
void RelateDB::initialise()
{
    //initialise the base database
    DBBase::initialise();
    
    //create table (1)
    QString error = executeSQL( QString(
          "create table '%1' ( UserID TEXT, ParamID INTEGER, UNIQUE(UserID) )")
              .arg( userIDToParamID() ) );
              
    //create table (2)
    error += executeSQL( QString(
          "create table '%1' ( RelateID INTEGER, ParamID INTEGER, UserID TEXT, UNIQUE(RelateID) )")
              .arg( relateIDToParameter() ) );
              
    if (not error.isNull())
        throw SireDB::db_error(error,CODELOC);
}
    
/** Dump the version number of this database */
void RelateDB::prepareToDump()
{
    DBBase::prepareToDump();
    
    this->saveParameter<RelateDB>( QString("%1_version").arg(prefix()), 1 );
}

/** Check the version number */
void RelateDB::postLoad()
{
    DBBase::postLoad();

    int v = this->loadParameter<RelateDB>( 
                            QString("%1_version").arg(prefix()) ).toInt();
                            
    if (v != 1)
        throw version_error( v, "1", "SireDB::RelateDB", CODELOC );
}

/** Relate the user ID string 'userid' to the parameter 'param' */
void RelateDB::relateParameter(const QString &userid, ParamID param)
{
    //format the userid for use in the database
    QString format_userid = formatField(database(), userid);

    //cannot relate an empty string
    if (format_userid.isEmpty())
        return;

    //get the table name
    QString tablename = userIDToParamID();

    //is there already such a relationship?
    QSqlQuery q(database());
    
    q.exec( QString("select ParamID from '%1' where UserID = %2")
                .arg(tablename, format_userid) );
    checkErrors(q,CODELOC);
    
    q.next();
    if (q.isValid())
    {
        //there is an existing relationship!
        if (q.value(0).toUInt() == param)
            //it is mapping the userid to the same parameter - nothing further to do
            return;
        else
        {
            //changing the mapping! Add a warning to the log
            addLog( QString("Overwriting parameter in \"%1\" - replacing %2 == %3 "
                            "with %2 == %4")
                  .arg(tablename, userid).arg(q.value(0).toUInt()).arg(param) );
                  
            //remove the existing parameter
            q.exec( QString("delete from '%1' where UserID = %2")
                        .arg(tablename, format_userid) );
            checkErrors(q, CODELOC);
        }
    }
    
    //now add the relationship to the database
    q.exec( QString("insert into %1 values ( %2, %3 )")
                .arg(tablename, format_userid).arg(param) );
    checkErrors(q, CODELOC);
}

/** Return the parameter with userid 'userid' - this will return 0 if there is 
    no such parameter, and will record that fact in the log. */
ParamID RelateDB::getParameter(const QString &userid)
{
    //get the table name
    QString tablename = userIDToParamID();
    
    //format the userid for use in the database
    QString format_userid = formatField(database(), userid);

    if (format_userid.isEmpty())
        return 0;

    QSqlQuery q(database());
    
    q.exec( QString("select ParamID from '%1' where UserID = %2")
              .arg(tablename, format_userid) );
    checkErrors(q, CODELOC);
    
    q.next();
    if (q.isValid())
        return q.value(0).toUInt();
    else
    {
        //the parameter is not in the database - report this fact in the log
        //and return 0
        addLog( QString("There is no parameter with user ID \"%1\" in database \"%2\"")
                  .arg(userid, prefix()) );
                  
        return 0;
    }
}
    
/** Relate the parameter with ID 'param' to the relationship with ID 'relatid' */
void RelateDB::relateParameter(RelateID relateid, ParamID param)
{
    //get the table name
    QString tablename = relateIDToParameter();
    
    QSqlQuery q(database());
    
    //remove any existing parameter for this relationship ID
    q.exec( QString("delete from '%1' where RelateID = %2")
                .arg(tablename).arg(relateid) );
    checkErrors(q, CODELOC);
    
    //add the relationship to the database
    q.exec( QString("insert into '%1' values ( %2, %3, NULL )")
                .arg(tablename).arg(relateid).arg(param) );
    checkErrors(q, CODELOC);
}

/** Relate the parameter with user ID string 'param' to the relationship 
    with ID 'relateid' */
void RelateDB::relateParameter(RelateID relateid, const QString &param)
{
    //format the userid for use in the database
    QString format_param = formatField(database(), param);
    
    //cannot relate to an empty user ID
    if (format_param.isEmpty())
        return;

    //get the table name
    QString tablename = relateIDToParameter();
    
    QSqlQuery q(database());
    
    //remove any existing parameter for this relationship ID
    q.exec( QString("delete from '%1' where RelateID = %2")
              .arg(tablename).arg(relateid) );
    checkErrors(q, CODELOC);
    
    //add the relationship to the database
    q.exec( QString("insert into '%1' values ( %2, 0, %3 )")
                .arg(tablename).arg(relateid).arg(format_param) );
    checkErrors(q, CODELOC);
}

/** Return the parameter for relationship ID 'relateid'. Returns '0' if there
    is no such parameter. */
ParamID RelateDB::getParameter(RelateID relateid)
{
    //get the table name
    QString tablename = relateIDToParameter();

    QSqlQuery q(database());

    q.exec( QString("select ParamID, UserID from '%1' where RelateID = %2")
              .arg(tablename).arg(relateid) );
    checkErrors(q, CODELOC);
    
    q.next();
    if (q.isValid())
    {
        //is this a paramID match or a userID match?
        ParamID paramid = q.value(0).toUInt();
        if (paramid != 0)
            //paramID match :-)
            return paramid;
        else
        {
            //must be a userID match
            return getParameter( q.value(1).toString() );
        }
    }
    else
        return 0;
}

/** Return the first parameter that matches any of the relationship IDs in 'relateids'.
    These are searched from the highest scoring match down to the lowest scoring.
    This returns '0' if there are no matches. */
ParamID RelateDB::getParameter(const RelateIDMap &relateids)
{
    if (relateids.isEmpty())
        return 0;
        
    QString tablename = relateIDToParameter();
    
    QSqlQuery q(database());
        
    q.exec( QString("select COUNT(*) from '%1'").arg(tablename) );
    checkErrors(q, CODELOC);
    q.next();
    BOOST_ASSERT(q.isValid());
    if (q.value(0).toInt() == 0)
        //there is no relationship data in this database - there can be 
        //no matches, so return 0
        return 0;
        
    //iterate backwards over the relateids (e.g. from the highest score
    //down to the lowest score)
    RelateIDMapIterator it(relateids);
    it.toBack();
        
    while( it.hasPrevious() )
    {
        it.previous();
        
        //is there a parameter for this relationship?
        q.exec( QString("select ParamID, UserID from '%1' where RelateID = %2")
                  .arg(tablename).arg(it.value()) );
        checkErrors(q, CODELOC);
        
        for (q.next(); q.isValid(); q.next())
        {
            //is this a paramID match?
            ParamID paramid = q.value(0).toUInt();
            
            if (paramid != 0)
                return paramid;
            else
            {
                //this should be a name match
                paramid = getParameter( q.value(1).toString() );
                if (paramid != 0)
                    return paramid;
            }
        }
    }
    
    //there have been no matches at all!
    return 0;
}
