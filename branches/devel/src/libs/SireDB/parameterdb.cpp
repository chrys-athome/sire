
#include "parameterdb.h"
#include "parametertable.h"
#include "assign_parameters.h"

#include "SireMol/molecule.h"

#include "SireDB/errors.h"
#include "SireError/errors.h"

#include "SqliteDriver/qsql_sqlite.h"
#include "SqliteDriver/sqlite3.h"

#include <QtSql>
#include <QCoreApplication>
#include <QDebug>

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireDB;

static const RegisterMetaType<ParameterDB> r_parameterdb("SireDB::ParameterDB");

/** Serialise to a binary data stream */
QDataStream SIREDB_EXPORT &operator<<(QDataStream &ds, const ParameterDB &db)
{
    writeHeader(ds, r_parameterdb, 1) << const_cast<ParameterDB&>(db).dumpToString();

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREDB_EXPORT &operator>>(QDataStream &ds, ParameterDB &db)
{
    VersionID v = readHeader(ds, r_parameterdb);

    if (v == 1)
    {
        QString rawsql;
        ds >> rawsql;
        db.loadFromString(rawsql);
    }
    else
        throw version_error(v, "1", r_parameterdb, CODELOC);

    return ds;
}

/** Format the passed field so that it can be safely inserted into an SQL line

    \throw SireError::invalid_arg
*/
QString SireDB::formatField(QSqlDatabase &db, const QVariant &field)
{
    //create a QSqlField to represent this type of data
    QSqlField sqlf("tmp",field.type());

    //set the value...
    sqlf.setValue(field);

    //return the formatted value...
    return db.driver()->formatValue(sqlf);
}

/** Constructor */
ParameterDB::ParameterDB() : nrelationships(0), nparams(0)
{}

/** Copy constructor - this is quite slow as it involves making a deep copy
    of the entire database (complete dump and load!) */
ParameterDB::ParameterDB(const ParameterDB &other) : nrelationships(0), nparams(0)
{
    //dump the original database to a raw SQL string
    QString rawsql = const_cast<ParameterDB&>(other).dumpToString();

    //load that SQL into this database
    this->loadFromString(rawsql);
}

/** Destructor */
ParameterDB::~ParameterDB()
{
    //delete all of the database components
    for (hash_type::iterator it = dbparts.begin();
         it != dbparts.end();
         ++it)
    {
        delete it.value();
    }

    //clear the hash
    dbparts.clear();
}

/** Return a string representation of this DB */
QString ParameterDB::toString() const
{
    return QString("ParameterDB( %1 )").arg( types().join(", ") );
}

/** Return whether or not this contains a component of type name 'type_name' */
bool ParameterDB::isA(const QString &type_name) const
{
    return dbparts.contains(type_name);
}

/** Return this database cast as a 'type_name' - note that this will have undefined
    results unless 'isA(type_name)' returns true */
const DBBase& ParameterDB::asA(const QString &type_name) const
{
    return *(dbparts.value(type_name));
}

/** Return this database cast as a 'type_name' - note that this will have undefined
    results unless 'isA(type_name)' returns true */
DBBase& ParameterDB::asA(const QString &type_name)
{
    return *(dbparts[type_name]);
}

/** Return a list of the type names of the types that are contained in
    this database */
QStringList ParameterDB::types() const
{
    QStringList ret(dbparts.keys());
    ret.sort();
    return ret;
}

/** Internal function used to create the database component of type 'type_name'

    \throw SireError::unknown_type
    \throw SireError::program_bug
*/
DBBase* createComponent(const char *type_name)
{
    //create the object using QMetaType
    int id = QMetaType::type( type_name );

    if ( id == 0 or not QMetaType::isRegistered(id) )
        throw SireError::unknown_type(QObject::tr(
              "There is no database component with type \"%1\" available. Ensure that "
              "this type is loaded and that it has been registered with "
              "QMetaData.").arg(type_name), CODELOC);

    std::auto_ptr<DBBase> dbptr( static_cast<DBBase*>(QMetaType::construct(id,0)) );

    if ( dbptr.get() == 0 )
        throw SireError::program_bug(QObject::tr(
              "Could not construct a database component of type \"%1\" despite it having "
              "been registered with QMetaData (ID = %2)")
                  .arg(type_name).arg(id), CODELOC);

    //check that this has the right type name
    if (QLatin1String(type_name) != QLatin1String(dbptr->what()))
        throw SireError::program_bug(QObject::tr(
              "Name mismatch for a database component! typeName() == %1 but "
              "what() == %2").arg(type_name).arg(dbptr->what()), CODELOC);

    return dbptr.release();
}

/** Add the component with type name 'type_name' - note that this does nothing
    if there is already such a component in this database

    \throw SireError::unknown_type
    \throw SireError::program_bug
*/
DBBase& ParameterDB::addComponent(const char *type_name)
{
    std::auto_ptr<DBBase> db_ptr( createComponent(type_name) );

    //initialise this component with this parent
    this->initialise(type_name, *db_ptr);

    return *(db_ptr.release());
}

/** Restore the component with type name 'type_name' - this creates the component
    if it doesn't exist, and then initialises the component from the data already
    present in the SQL database

    \throw SireError::unknown_type
    \throw SireError::program_bug
*/
void ParameterDB::restoreComponent(const char *type_name)
{
    DBBase *dbptr = dbparts.value(type_name, 0);

    if (not dbptr)
    {
        //we need to create the component
        dbptr = createComponent(type_name);

        //save this component with the others
        dbparts.insert( type_name, dbptr );

        //tell the component who its parent is
        dbptr->setParent(this);
    }

    //now post-load the component
    dbptr->postLoad();
}

/** Open the database. This opens a SQLITE, in-memory database, which is initially
    empty.

    \warning If the database was already open, then it will be closed and any data
    held within it will be lost. If you want to save the database, then you must
    ensure that you first call ParameterDB::dump

    \throw SireDB::db_error
*/
void ParameterDB::openDatabase()
{
    //get a unique name for this database, so that multiple MMDBs can use
    //different databases at the same time
    dbname = QString("SireDB::ParameterDB-%1").arg( toUInt(this) );

    //close and remove the database if it is already open
    if (db.isOpen())
    {
        db.close();
        QSqlDatabase::removeDatabase(dbname);
    }

    //create the sqlite database...
    sqlite3 *connection = 0;
    if (sqlite3_open(":memory:", &connection) != SQLITE_OK)
    {
        throw SireDB::db_error( QObject::tr("Error opening the in-memory database: %1")
                                    .arg(sqlite3_errmsg(connection)), CODELOC );
    }

    //create the Qt handle for the database
    QSQLiteDriver *driver = new QSQLiteDriver(connection);

    db = QSqlDatabase::addDatabase(driver, dbname);
}

/** Return a reference to the actual database used to store the database */
QSqlDatabase& ParameterDB::database()
{
    if (not db.isOpen())
        this->initialise();

    return db;
}

/** Assign the parameters for the molecule 'molecule' using the
    instructions held in 'assign_instruct'. Return the parameters in
    a new ParameterTable */
ParameterTable ParameterDB::assign(const Molecule &molecule,
                                   const assign_parameters &assign_instruct,
                                   const MatchMRData &matchmr)
{
    return assign_instruct.assign(molecule, *this, matchmr);
}

/** Assign the parameters for the molecule 'molecule', using the
    parameters already in 'params' together with the parameters
    from this database obtained via 'assign_instruct'. Return the
    parameters in a new ParameterTable. */
ParameterTable ParameterDB::assign(const Molecule &molecule,
                                   const ParameterTable &params,
                                   const assign_parameters &assign_instruct,
                                   const MatchMRData &matchmr)
{
    return assign_instruct.assign(molecule, params, *this, matchmr);
}

/** Check for any errors, and if detected, add them to the database log and
    throw an exception.

    \throw SireError::db_error
*/
void ParameterDB::checkErrors(QSqlQuery &q, const QString throwloc)
{
    if (q.lastError().type() != QSqlError::NoError)
    {
        QString err = QObject::tr("Possible SQL error at %1\n"
                                      "SQL = %2\n"
                                      "DB error = %3\n"
                                      "Driver error = %4\n")
                .arg(throwloc).arg(q.lastQuery())
                .arg(q.lastError().databaseText()).arg(q.lastError().driverText());

        //add this warning to the log
        addLog(err);

        //throw an exception
        throw SireDB::db_error(err,CODELOC);
    }
}

/** Execute some SQL on the database, and if there is an error, then return a string
    describing the error. If QString::null is returned then everything is ok */
QString ParameterDB::executeSQL(const QString &query)
{
    if (not db.isOpen())
        return QObject::tr("Cannot execute \"%1\" as the database isn't open!").arg(query);

    QSqlQuery q(db);
    bool worked = q.exec(query);

    if (not worked)
        return QString("Error executing query \"%1\"\nDatabase error: %2\nDriver error: %3")
                  .arg(query).arg(q.lastError().databaseText()).arg(q.lastError().driverText());
    else
        return QString::null;
}

/** Load the contents of the database from the passed string */
void ParameterDB::loadFromString(QString dbdump)
{
    QTextStream ts(&dbdump, QIODevice::ReadOnly);
    load(ts);
}

/** Load the contents of the database a file containing raw SQL (e.g. from a call
    to MMDB::dump) */
void ParameterDB::load(const QString &filename)
{
    //open the file and read all of the contents into a string
    QFile f(filename);

    if (not f.open(QIODevice::ReadOnly))
        throw SireError::file_error(QObject::tr(
            "Cannot open file '%1' to be loaded into the database.").arg(filename), CODELOC);

    QTextStream ts(&f);

    load(ts);
}

/** Load the contents of the database from the raw SQL in the device attached to
    the QTextStream */
void ParameterDB::load(QTextStream &ts)
{
    //the first step is to clear the SQL database (not anything else)
    this->openDatabase();

    //now read in all of the SQL
    QSqlQuery q(db);

    //read and execute the file one statement at a time. A statement ends with a semicolon
    QString sqlline;
    int linenum = 0;

    while (not ts.atEnd())
    {
        QString line = ts.readLine();
        linenum++;

        if (sqlline.isNull())
            sqlline = line;
        else
            sqlline += "\n" + line;

        sqlline = sqlline.simplified();

        if (sqlline.endsWith(";"))
        {
            //we have a complete statement - execute it!
            if (not q.exec(sqlline))
            {
                const QFile *file = dynamic_cast<const QFile*>( ts.device() );

                if (file)
                {
                    throw SireDB::db_error(QObject::tr("Error reading SQL statement from line "
                                                       "%1 of '%2'.\n"
                                                       "%3\n%4\n%5")
                       .arg(linenum).arg(file->fileName()).arg(q.lastQuery())
                       .arg(q.lastError().databaseText()).arg(q.lastError().driverText()),
                       CODELOC);
                }
                else
                {
                    throw SireDB::db_error(QObject::tr("Error reading SQL statement from line "
                                                       "%1.\n %3\n%4\n%5")
                       .arg(linenum).arg(q.lastQuery())
                       .arg(q.lastError().databaseText()).arg(q.lastError().driverText()),
                       CODELOC);
                }
            }

            sqlline = QString::null;
        }
    }

    //now perform post-loading restoration
    this->postLoad();
}

/** Dump the entire contents of the database to a string */
QString ParameterDB::dumpToString()
{
    QString ret;
    QTextStream ts(&ret, QIODevice::WriteOnly);
    dump(ts);

    return ret;
}

/** Dump the entire contents of this database to a file. This dumps the raw SQL necessary to
    recreate the table. */
void ParameterDB::dump(const QString &filename)
{
    //open the file
    QFile f(filename);

    if (not f.open(QIODevice::WriteOnly))
        throw SireError::file_error(QObject::tr(
            "Could not open file '%1' so that the database could be dumped!")
                .arg(filename), CODELOC);

    QTextStream ts(&f);

    dump(ts);
}

/** Dump the entire contents of the database to the device attached to the
    QTextStream */
void ParameterDB::dump(QTextStream &ts)
{
    //prepare this database for dumping
    this->prepareToDump();

    //the entire dump will occur within a single transaction
    ts << "BEGIN TRANSACTION;\n";

    //The name of all of the tables is held in the 'sqlite_master' table
    QSqlQuery q(db);

    q.exec("select tbl_name,sql from sqlite_master where type='table'");
    checkErrors(q, CODELOC);

    //now collect the complete set of table names and sql
    QStringList tablenames;
    QStringList tablesql;

    for (q.next(); q.isValid(); q.next())
    {
        tablenames.append( q.value(0).toString() );
        tablesql.append( q.value(1).toString() );
    }

    //now we can loop over each table and can dump it
    for (int i=0; i<tablenames.count(); i++)
    {
        //first, write the SQL necessary to create the table
        ts << tablesql[i] << ";\n";

        //now dump the table to the stream
        dumpTable( tablenames[i], ts );
    }

    //commit the transaction
    ts << "COMMIT;\n";

    //all done!
}

/** Dump the contents of the table 'tablename' to the text stream 'ts', in raw SQL */
void ParameterDB::dumpTable(const QString &tablename, QTextStream &ts)
{
    QSqlQuery q(db);

    //get the values using 'select *' - this relies on 'select *' returning same order
    //of the values as the columns in the database.
    q.exec( QString("select * from %1").arg(tablename) );
    checkErrors(q, CODELOC);
    q.next();

    if (not q.isValid())
        return;

    //get the record that corresponds to this query
    QSqlRecord record = q.record();

    //get the number of columns in the table
    int ncols = record.count();

    for( ; q.isValid(); q.next() )
    {
        ts << "INSERT INTO \"" << tablename << "\" VALUES(";

        QSqlRecord rec = q.record();

        //loop over each value and write them to the text stream
        for (int i=0; i<ncols; i++)
        {
            ts << db.driver()->formatValue( rec.field(i) );

            if ( i != ncols-1 )
                ts << ", ";
        }

        ts << ");\n";
    }
}

/** Add 'text' onto the end of the log */
void ParameterDB::addLog(const QString &text)
{
    if (text.isEmpty())
        return;

    //add the text onto the log, together with the current date and time.
    dblog += QString("****** %1 ********\n%2\n")
              .arg(QDateTime::currentDateTime().toString(), text);
}

/** Return the complete log */
QString ParameterDB::getLog() const
{
    return dblog;
}

/** Clear the log */
void ParameterDB::clearLog()
{
    dblog = QString::null;
}

/** Completely clear this database (this clear components - it doesn't delete them) */
void ParameterDB::clear()
{
    //reinitialise the database
    this->initialise();
}

/** Return a new, unique, relationship ID number */
RelateID ParameterDB::getNewRelateID()
{
    nrelationships++;
    return nrelationships;
}

/** Return a new, unique, parameter ID number */
ParamID ParameterDB::getNewParamID()
{
    nparams++;
    return nparams;
}

/** Return the name of the table that holds the global parameters */
QString ParameterDB::parameterTable()
{
    return "SireDB_ParameterDB_Global_Params";
}

/** Return the name of the table that holds global parameters that must
    be represented as binary dumps */
QString ParameterDB::binaryTable()
{
    return "SireDB_ParameterDB_Global_Binary_Params";
}

/** Return the name of the table that holds the type names and versions
    of all of the components of this database */
QString ParameterDB::typeTable()
{
    return "SireDB_ParameterDB_Types";
}

/** Initialise the component 'dbcomp'. */
void ParameterDB::initialise(const char *type_name, DBBase &dbcomp)
{
    //check that the type names match...
    if (QLatin1String(type_name) != QLatin1String(dbcomp.what()))
        throw SireError::program_bug(QObject::tr(
            "The type name of the database component is confused! typeName() == %1 "
            "while what() == %2")
                .arg(type_name).arg(dbcomp.what()), CODELOC);

    //add this type to the list of types in this database
    QSqlQuery q(database());

    QString format_type = formatField(database(),type_name);

    q.exec( QString("delete from '%1' where Type = %2")
                    .arg(typeTable(), format_type) );

    q.exec( QString("insert into '%1' values ( %2 )")
                    .arg(typeTable(), format_type) );

    //add this to the list of components
    dbparts.insert( type_name, &dbcomp );

    //tell the component who it's parent is
    dbcomp.setParent(this);

    //now initialiase the component
    dbcomp.initialise();
}

/** Initialise the database - creates the table that holds global database
    parameters */
void ParameterDB::initialise()
{
    //reopen the database - this clears it
    this->openDatabase();

    //reset the relationship and parameter counts to 0
    nrelationships = 0;
    nparams = 0;

    //clear the log
    this->clearLog();

    //create the parameter table - this is a collection of QVariants
    //indexed by name

    //create the table that holds parameters that can be represented as strings
    QString errors = executeSQL( QString(
          "create table '%1' ( Type TEXT, Name TEXT, Value TEXT, UNIQUE(Type,Name) )")
                                         .arg(parameterTable()) );

    //now create the table that holds parameters that must be represented as
    //binary dumps
    errors += executeSQL( QString(
                          "create table '%1' ( Type TEXT, Name TEXT, Value TEXT, "
                                          "Size INTEGER, CheckSum INTEGER, UNIQUE(Type,Name) )")
                              .arg(binaryTable()) );

    //now create the table that holds the type names of the components in this
    //database
    errors += executeSQL( QString("create table '%1' ( Type TEXT )")
                              .arg(typeTable()) );

    if ( not errors.isNull() )
        throw SireDB::db_error(errors, CODELOC);

    //now initialise all of the database components
    for (hash_type::iterator it = dbparts.begin();
         it != dbparts.end();
         ++it)
    {
        it.value()->initialise();
    }
}

/** Prepare the database for dumping */
void ParameterDB::prepareToDump()
{
    saveParameter<ParameterDB>( "nrelationships", nrelationships );
    saveParameter<ParameterDB>( "nparams", nparams );
    saveParameter<ParameterDB>( "dblog", dblog );

    //now tell each component that a dump is about to be performed
    for (hash_type::iterator it = dbparts.begin();
         it != dbparts.end();
         ++it)
    {
        it.value()->prepareToDump();
    }
}

/** Load the state of this object from the database */
void ParameterDB::postLoad()
{
    nrelationships = loadParameter<ParameterDB>("nrelationships").toUInt();
    nparams = loadParameter<ParameterDB>("nparams").toUInt();
    dblog = loadParameter<ParameterDB>("dblog").toString();

    //now restore all of the components (keeping a record of those that
    //have been restored)
    QSet<QString> restored_dbs;

    QSqlQuery q(database());

    q.exec( QString("select Type from '%1'").arg(typeTable()) );
    checkErrors(q, CODELOC);

    for (q.next(); q.isValid(); q.next())
    {
        QString type_name = q.value(0).toString();

        try
        {
            //restore this component
            this->restoreComponent(type_name.toLatin1().constData());

            restored_dbs.insert(type_name);
        }
        catch(SireError::unknown_type &e)
        {
            this->addLog( QObject::tr("Failed to restore component \"%1\". This part of "
                                      "the database will not be available!\n%2")
                                        .arg(type_name, e.toString()) );
        }
    }

    //now run through the components in this database and re-initialise
    //those components that have not been loaded
    for (hash_type::iterator it = dbparts.begin();
         it != dbparts.end();
         ++it)
    {
        if ( not restored_dbs.contains(it.key()) )
            //this component has not been loaded - initialise it instead
            it.value()->initialise();
    }
}

/** Save the parameter called 'name' for the object with type name
    'type_name' to the global parameter table. This replaces
    any existing parameter with the same name. Note that name-lookups are
    case-sensitive */
void ParameterDB::pvt_saveParameter(const char *type_name,
                                    const QString &name, const QVariant &value)
{
    //format the name so that it can be used in the database
    QString format_name = formatField(database(),name);
    QString format_type = formatField(database(),type_name);

    //remove this parameter from the current tables
    QSqlQuery q(database());

    q.exec( QString("delete from '%1' where Name = %2 and Type = %3")
                              .arg(parameterTable(), format_name, format_type) );
    checkErrors(q, CODELOC);

    q.exec( QString("delete from '%1' where Name = %2 and Type = %3")
                              .arg(binaryTable(), format_name, format_type) );
    checkErrors(q, CODELOC);

    if ( not value.isValid() )
        return;
    else if (value.canConvert<QString>())
    {
        //we can represent the value using a QString - it is safe to
        //use formatField to represent this object in the database directly
        q.exec( QString("insert into '%1' values ( %2, %3, %4 )")
                    .arg(parameterTable(), format_type, format_name,
                         formatField(database(),value)) );
        checkErrors(q,CODELOC);
    }
    else
    {
        //QVariant::toString() won't work, so I will create a binary dump
        //of the QVariant and will save that.
        QByteArray data;
        QDataStream ds( &data, QIODevice::WriteOnly );
        ds << value;

        int sz = data.size();
        quint16 cksum = qChecksum(data, sz);

        q.exec( QString("insert into '%1' values ( %2, %3, %4, %5, %6 )")
                    .arg(binaryTable(), format_type, format_name,
                         formatField(database(),data))
                    .arg(sz).arg(cksum) );
        checkErrors(q, CODELOC);
    }
}

/** Load and return the parameter called 'name' for the object with type name
    'type_name' in the global parameter table,
    or return a null QVariant if there is no such parameter. Note that
    name-lookups are case-sensitive. */
QVariant ParameterDB::pvt_loadParameter(const char *type_name, const QString &name)
{
    QString format_name = formatField(database(), name);
    QString format_type = formatField(database(), type_name);

    //look up the parameter in the text parameter table...
    QSqlQuery q(database());
    q.exec( QString("select Value from '%1' where Name = %2 and Type = %3")
                  .arg(parameterTable(), format_name, format_type) );
    checkErrors(q, CODELOC);

    q.next();
    if (q.isValid())
        return q.value(0);
    else
    {
        //the parameter didn't exist - try to find it in the
        //binary parameter table
        q.exec( QString("select Value, Size, CheckSum from '%1' where "
                                                            "Name = %2 and Type = %3")
                  .arg(binaryTable(), format_name, format_type) );
        checkErrors(q, CODELOC);

        q.next();
        if (q.isValid())
        {
            //we have found the binary data - check that it is ok
            QByteArray data = q.value(0).toByteArray();
            int sz = data.size();
            quint16 cksum = qChecksum(data, sz);

            int oldsz = q.value(1).toInt();
            quint16 oldck = q.value(2).toUInt();

            if (sz != oldsz or cksum != oldck)
            {
                addLog( QString("Possible database corruption in '%1' for parameter '%2:%3'. "
                                "Size and checksum should be '%4' and '%5', but are "
                                "actually '%6' and '%7'")
                          .arg(parameterTable(), type_name, name).arg(oldsz).arg(oldck)
                          .arg(sz).arg(cksum) );
                return QVariant();
            }

            QVariant ret;
            QDataStream ds(data);
            ds >> ret;
            return ret;
        }
        else
            //the parameter was not found at all!
            return QVariant();
    }
}

/** Return the number of parameters that have been loaded into the database */
int ParameterDB::nParameters() const
{
    return nparams;
}

/** Return the number of relationships that have been loaded into the database */
int ParameterDB::nRelationships() const
{
    return nrelationships;
}
