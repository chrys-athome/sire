
#include "SireCAS/qhash_sirecas.h"

#include "funcdb.h"

#include "SireDB/errors.h"

#include "SireCAS/expression.h"
#include "SireCAS/function.h"
#include "SireCAS/functions.h"
#include "SireCAS/identities.h"

#include "SireStream/version_error.h"

#include <QtSql>

using namespace SireDB;
using namespace SireCAS;
using namespace SireStream;

/** Constructer */
FuncDB::FuncDB() : RelateDB()
{}

/** Copy constructer */
FuncDB::FuncDB(const FuncDB &other) : RelateDB(other)
{}

/** Destructor */
FuncDB::~FuncDB()
{}

/** Return the name of the function table */
QString FuncDB::functionTable() const
{
    return QString("%1_SireDB_FuncDB_functions").arg(this->prefix());
}

/** Return the name of the term table (maps generic functions to specific expressions) */
QString FuncDB::termTable() const
{
    return QString("%1_SireDB_FuncDB_terms").arg(this->prefix());
}

/** Create the tables associated with this database. 

    There are two tables in the FuncDB;
    
    (1) Table giving a unique ID to each Expression loaded into the database.
    
    (2) Table mapping SireCAS::Function objects to specific expressions - 
        this allows terms to be added to the database, and then functions
        to be stored as a mathematical combination of these terms
    
    \throw SireDB::db_error
*/
void FuncDB::initialise()
{
    //create the RelateDB tables
    RelateDB::initialise();

    //create table (1) - table to give a unique ID to each MathFunc. As this uses a QByteArray
    //to hold the function, we also store the size of the QByteArray and a checksum so that 
    //we have a chance of detecting if the data has become corrupted.
    QString errors = executeSQL( QString(
          "create table '%1' ( ParamID INTEGER, Function TEXT, "
                              "FunctionDump TEXT, DataSize INTEGER, "
                              "Checksum INTEGER, Primary Key (ParamID) )")
                .arg(functionTable()) );
                         
    //create table (2) - table maps a SireCAS::Function object to a specific
    //expression (held in table 1 and identified via its ParamID). The 
    //function is stored via its SireCAS::SymbolID and text output, and
    //a binary dump is also stored in FunctionDump, DataSize and CheckSum
    errors += executeSQL( QString(
          "create table '%1' ( Function TEXT, "
                              "FunctionDump TEXT, DataSize INTEGER, "
                              "CheckSum INTEGER, ParamID INTEGER, Primary Key (Function) )" )
                .arg(termTable()) );
                         
    if (not errors.isNull())
        throw SireDB::db_error(errors,CODELOC);
}
    
/** Dump the version number of this database */
void FuncDB::prepareToDump()
{
    RelateDB::prepareToDump();
    
    this->saveParameter<FuncDB>( QString("%1_version").arg(prefix()), 1 );
}

/** Check the version number */
void FuncDB::postLoad()
{
    RelateDB::postLoad();

    int v = this->loadParameter<FuncDB>( 
                            QString("%1_version").arg(prefix()) ).toInt();
                            
    if (v != 1)
        throw version_error( v, "1", "SireDB::FuncDB", CODELOC );
}

/** Return a binary array containing the binary representation of function 'func' */
QByteArray FuncDB::dump(const Expression &func)
{
    QByteArray data;
    QDataStream ds(&data, QIODevice::WriteOnly);
    
    ds << func;
    return data;
}

/** Load and return an Expression from binary data. */
Expression FuncDB::load(const QByteArray &data)
{
    QDataStream ds(data);
    
    Expression ex;
    ds >> ex;
    
    return ex;
}

/** Return a binary array containing the binary representation of function 'func' */
QByteArray FuncDB::dumpFunction(const Function &func)
{
    QByteArray data;
    QDataStream ds(&data, QIODevice::WriteOnly);
    
    ds << func;
    return data;
}

/** Load and return a Function from binary data */
Function FuncDB::loadFunction(const QByteArray &data)
{
    QDataStream ds(data);
    
    Function func;
    ds >> func;
    
    return func;
}

/** Add this function to the database and return the internal database ID number for this function */
ParamID FuncDB::addFunc(const Expression &func)
{
    //does this function already exist in the database?
    QString tablename = functionTable();
    QString funcstring = func.toString();
    
    QSqlQuery q(database());
    q.exec( QString("select ParamID from '%1' where Function = %2")
                .arg(tablename, formatField(database(), funcstring)) );
    checkErrors(q, CODELOC);
    q.next();
    
    if (q.isValid())
        //we have found an existing copy of this function - return its ID
        return q.value(0).toUInt();
    else
    {
        //we just add this onto the end of the table - it doesn't matter if there is
        //more than one copy of this function in the database
        ParamID paramid = getNewParamID();
    
        QSqlTableModel table(0, database());
        
        table.setTable(tablename);
        int row = table.rowCount();
        
        //dump the object to a binary array
        QByteArray funcdata = dump(func);
        int sz = funcdata.size();
        quint16 cksum = qChecksum(funcdata, sz);
        
        table.insertRows(row,1);
        table.setData(table.index(row,0), paramid);
        table.setData(table.index(row,1), funcstring);
        table.setData(table.index(row,2), funcdata);
        table.setData(table.index(row,3), sz);
        table.setData(table.index(row,4), cksum);
        
        table.submitAll();
        
        return paramid;
    }
}

/** Retreive the function with the internal ID number 'funcid' or return an empty
    expression if it could not be found, and record the error in the log.
    
    \throw SireDB::db_error
*/
Expression FuncDB::retrieveFunc(ParamID funcid)
{
    if (funcid == 0)
        return Expression();
    else
    {
        //search for this function in the database...
        QSqlQuery q(database());
        q.exec( QString("select FunctionDump, DataSize, CheckSum "
                        "from '%1' where ParamID == %2")
                    .arg(functionTable()).arg(funcid) );
        checkErrors(q,CODELOC);
        
        q.next();
        if (not q.isValid())
        {
            addLog( QString("**WARNING** Missing expression parameter with ParamID '%1'")
                              .arg(funcid) );
            return Expression();
        }
    
        //extract a QByteArray from the database
        QByteArray funcdata = q.value(0).toByteArray();
    
        //ensure that the size of the bytearray and the checksum are correct
        int sz = funcdata.size();
        quint16 check = qChecksum(funcdata, sz);
        
        int oldsz = q.value(1).toInt();
        quint16 oldcheck= q.value(2).toUInt();
        
        if (sz != oldsz or check != oldcheck)
        {
            //there has been some database corruption!
            QString err = QObject::tr(
                "Detected possible database corruption! Function %1 should have data size "
                "%2 and checksum %3, but retreived data has size %4 and checksum %5.")
                    .arg(funcid).arg(oldsz).arg(oldcheck).arg(sz).arg(check);
           
            addLog(err);
            return Expression();
        }
        
        //now return the function constructed from this data
        return load(funcdata);
    }
}   
                     
/** Add the function 'func' and relate it to the user ID string 'userid' */
void FuncDB::addFunc(const QString &userid, const Expression &func)
{
    //first add the function to the database
    ParamID paramid = this->addFunc(func);
    
    //relate the paramid to the userid
    relateParameter( userid, paramid );
}

/** Return the function identified by the user ID string 'userid', or return a zero
    function if there is no such userid in the database (and record this fact in the log) */
Expression FuncDB::getFunc(const QString &userid, bool *foundfunc)
{
    ParamID paramid = getParameter(userid);
    
    if (foundfunc)
        *foundfunc = (paramid != 0);
        
    return retrieveFunc(paramid);
}

/** Return the function that matches the relationship ID 'relateid', or return a 
    zero expression if there is no such expression. 'foundfunc' is set to whether
    or not an expression was found. */
Expression FuncDB::getFunc(RelateID relateid, bool *foundfunc)
{
    //get the parameter ID number
    ParamID paramid = getParameter(relateid);
    
    if (foundfunc)
        *foundfunc = (paramid != 0);
        
    return retrieveFunc(paramid);
}

/** Return the function that matches the best relationship ID in 'relateids', or
    return a zero expression if one does not exist. 'foundfunc' is set to whether
    or not an expression was found. */
Expression FuncDB::getFunc(const RelateIDMap &relateids, bool *foundfunc)
{
    ParamID paramid = getParameter(relateids);
    
    if (foundfunc)
        *foundfunc = (paramid != 0);
        
    return retrieveFunc(paramid);
}

/** Add the relationship of a function (term) to a specific expression, 
    e.g. func == equals */
void FuncDB::addFunc(const Function &func, const Expression &equals)
{
    //add the expression and get its unique ID number
    ParamID paramid = addFunc(equals);
    
    //does the function already exist in the database?
    QString funcname = func.name();
    
    //get the name of the table holding function terms
    QString tablename = termTable();
    
    QSqlQuery q(database());
    q.exec( QString("select ParamID from '%1' where Function = %2")
                .arg(tablename, formatField(database(),funcname) ) );
    checkErrors(q, CODELOC);
    
    q.next();
    if (q.isValid())
    {
        //this term is already in the database...
        if (q.value(0).toUInt() == paramid)
            //it is already mapped to the correct expression :-)
            //nothing further to do
            return;
        else
        {
            //we need to remove the existing entry
            q.exec( QString("delete from '%1' where Function = %2")
                  .arg(tablename,funcname) );
            checkErrors(q, CODELOC);
        }
    }
    
    QSqlTableModel table(0, database());
        
    table.setTable(tablename);
    int row = table.rowCount();
        
    //dump the object to a binary array
    QByteArray funcdata = dump(func);
    int sz = funcdata.size();
    quint16 cksum = qChecksum(funcdata, sz);
        
    table.insertRows(row,1);
    table.setData(table.index(row,0), funcname);
    table.setData(table.index(row,1), funcdata);
    table.setData(table.index(row,2), sz);
    table.setData(table.index(row,3), cksum);
    table.setData(table.index(row,4), paramid);
        
    table.submitAll();
}

/** Return the expression matching the function 'func', or return an empty
    expression if there is no such function in the database. 'foundfunc' is
    set to true if the function is found, false if it isn't */
Expression FuncDB::getFunc(const Function &func, bool *foundfunc)
{
    //does the function already exist in the database?
    QString funcname = func.name();
    
    //get the name of the table holding function terms
    QString tablename = termTable();
    
    QSqlQuery q(database());
    q.exec( QString("select FunctionDump, DataSize, CheckSum, ParamID from '%1' "
                    "where Function = %2")
                .arg(tablename, formatField(database(),funcname) ) );
    checkErrors(q, CODELOC);
    
    for (q.next(); q.isValid(); q.next())
    {
        //extract the function stored in the database...
        QByteArray funcdata = q.value(0).toByteArray();
        
        //verify that the data has not become corrupted
        int sz = funcdata.size();
        quint16 check = qChecksum(funcdata, sz);
        
        int oldsz = q.value(1).toInt();
        quint16 oldcheck = q.value(2).toUInt();
        
        if (sz != oldsz or check != oldcheck)
        {
            QString err = QObject::tr(
                "Detected possible database corruption! Function \"%1\" should have data size "
                "%2 and checksum %3, but retreived data has size %4 and checksum %5.")
                    .arg(func.name()).arg(oldsz).arg(oldcheck).arg(sz).arg(check);
            
            addLog(err);
            continue;
        }
        
        //extract the function from the binary data
        Function oldfunc = loadFunction(funcdata);
        
        //is this the right function?
        if (func == oldfunc)
        {
            if (foundfunc)
                *foundfunc = true;
        
            //it is! Get the expression and return it
            return retrieveFunc( q.value(3).toUInt() );
        }
    }
    
    //the function was not found
    if (foundfunc)
        *foundfunc = false;
        
    return Expression();
}

/** Retrieve the expression ID associated with the function 'func', or else 
    return 0 if there is no such mapping in this database */
tuple<Function,Expression> FuncDB::retrieveFunc(const QString &funcname)
{
    QSqlQuery q(database());
    q.exec( QString("select FunctionDump, DataSize, CheckSum, ParamID from '%1' "
                    "where Function = %2")
                  .arg(termTable(), formatField(database(),funcname) ) );
    checkErrors(q, CODELOC);
    
    q.next();
    if (q.isValid())
    {
        //extract a QByteArray from the database
        QByteArray funcdata = q.value(0).toByteArray();
    
        //ensure that the size of the bytearray and the checksum are correct
        int sz = funcdata.size();
        quint16 check = qChecksum(funcdata, sz);
        
        int oldsz = q.value(1).toInt();
        quint16 oldcheck= q.value(2).toUInt();
        
        if (sz != oldsz or check != oldcheck)
        {
            //there has been some database corruption!
            QString err = QObject::tr(
                "Detected possible database corruption! Function %1 should have data size "
                "%2 and checksum %3, but retreived data has size %4 and checksum %5.")
                    .arg(funcname).arg(oldsz).arg(oldcheck).arg(sz).arg(check);
           
            addLog(err);
            return tuple<Function,Expression>();
        }
        
        //now return the function constructed from this data
        return tuple<Function,Expression>( loadFunction(funcdata), 
                                           retrieveFunc( ParamID(q.value(3).toUInt()) )
                                         );
    }
    else
        return tuple<Function,Expression>();
}

/** Resolve the terms in 'function' - this replaces any terms in the function by
    the associated mappings that are stored in the database. This eliminates any
    terms that are not in this database (recording the fact in the log) */
Expression FuncDB::resolveTerms(const Expression &expression)
{
    //get the list of SireCAS::Function objects in this expression
    Functions funcs = expression.functions();
    
    if (funcs.isEmpty())
        //the expression doesn't contain any functions - just return it
        return expression;
    else
    {    
        Expression ret = expression;
    
        QSet<QString> funcnames = funcs.functionNames();
    
        for (QSet<QString>::const_iterator it = funcnames.begin();
             it != funcnames.end();
             ++it)
        {
            //get the ParamID of the expression that is mapped to this function...
            tuple<Function,Expression> funcex = retrieveFunc( *it );
        
            const Function &func = funcex.get<0>();
            const Expression &ex = funcex.get<1>();
        
            if (func.name().isEmpty())
            {
                //record the missing term in the log
                addLog( QObject::tr("WARNING: There is no recorded expression matching the "
                                    "term \"%1\" in the function \"%2\"")
                                        .arg(*it, expression.toString()) );
                                    
                ret = ret.substitute( func == Expression(0) );
            }
            else
            {
                ret = ret.substitute( func == ex );
            }
        }
        
        //substituting in the terms may have introduced more terms - recurse
        //until all terms have been substituted...
        //return resolveTerms(ret);
        
        //cannot recurse yet as f(x) will not sub for f(x,y), and thus we 
        //would loop forever... For the moment I will just return and not
        //deal with function of a function (especially as function of a function
        //needs sorting out in SireCAS anyway!)
        return ret;
    }
}
