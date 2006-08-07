#ifndef SIREDB_PARAMETERDB_H
#define SIREDB_PARAMETERDB_H

#include <QSqlDatabase>

#include <QString>
#include <QLatin1String>
#include <QVariant>
#include <QHash>

#include "SireBase/dynamicsharedptr.hpp"

#include "relateidmap.h"
#include "matchmrdata.h"

class QTextStream;

SIRE_BEGIN_HEADER

namespace SireMol
{
class Molecule;
}

namespace SireDB
{
class ParameterDB;
}

QDataStream& operator<<(QDataStream&, const SireDB::ParameterDB&);
QDataStream& operator>>(QDataStream&, SireDB::ParameterDB&);

namespace SireDB
{

QString formatField(QSqlDatabase &db, const QVariant &field);

class DBBase;

class ParameterTable;
class TableBase;
class assign_parameters;

using SireMol::Molecule;

/** The ParameterDB is the root of the parameter databases that are used to store the parameters
    for a forcefield, and has logic to help their automatic assignment.
     
    The ParameterDB is intended to be multiply inherited with several BaseDB derived classes. It is
    the BaseDB-derived classes that actually hold the code to hold and manipulate forcefield
    parameters.
    
    For example, there are ChargeDB, LJDB and BondDB classes that each inherit from DBBase.
    We can create a parameter database that holds charge, LJ and bond parameters by 
    inheriting a new database class from ParameterDB, ChargeDB, LJDB and BondDB. Each DBBase
    class provides a component, and by multiply inheriting, we can combine all of these
    components into a single, functional parameter database.
    
    @author Christopher Woods
*/
class SIREDB_EXPORT ParameterDB
{

/** DBBase is a friend so that it can provide functions that access the QSqlDatabase,
    and also add to the log */
friend class DBBase;

public:
    ParameterDB();
    ParameterDB(const ParameterDB &other);
    
    ~ParameterDB();

    static const char* typeName()
    {
        return "SireDB::ParameterDB";
    }
    
    const char* what() const
    {
        return ParameterDB::typeName();
    }
    
    QString toString() const;
    
    void addLog(const QString &logtext);
    QString getLog() const;
    void clearLog();

    int nParameters() const;
    int nRelationships() const;

    void clear();
    
    void dump(const QString &filename);
    void load(const QString &filename);
    
    QString dumpToString();
    void loadFromString(QString sqldump);
    
    void dump(QTextStream &ts);
    void load(QTextStream &ts);

    /** Return whether this is a database of type 'T' */
    template<class T>
    bool isA() const;
    
    /** Return whether or not this is a database of type 'type_name' */
    bool isA(const QString &type_name) const;
    
    /** Return this database cast to a 'T' - note that 
        'isA<T>()' must return true or else this will have
        undefined results. */
    template<class T>
    const T& asA() const;

    /** Return a reference to the database component with 
        name 'type_name' - note that isA(type_name) must return
        true or this will have undefined results */
    const DBBase& asA(const QString &type_name) const;

    /** Return this database cast to a 'T' - note that 
        'isA<T>()' must return true or else this will have
        undefined results. */
    template<class T>
    T& asA();

    /** Return a reference to the database component with 
        name 'type_name' - note that isA(type_name) must return
        true or this will have undefined results */
    DBBase& asA(const QString &type_name);

    template<class T>
    T& addComponent();
    
    DBBase& addComponent(const char *type_name);

    QStringList types() const;

    ParameterTable assign(const Molecule &molecule,
                          const assign_parameters &assign_instruct,
                          const MatchMRData &matchmr = MatchMRData());

protected:
    //protected functions may be called by DBBase, which is a friend
    //to ParameterDB
    
    void initialise();
    void prepareToDump();
    void postLoad();
    
    template<class T>
    void saveParameter(const QString &name, const QVariant &value);
    
    template<class T>
    QVariant loadParameter(const QString &name);
    
    QSqlDatabase& database();
    void checkErrors(QSqlQuery &q, const QString throwloc);
    
    void openDatabase();

    QString executeSQL(const QString &query);
    void dumpTable(const QString &tablename, QTextStream &ts);

    RelateID getNewRelateID();
    ParamID getNewParamID();

private:
    static QString parameterTable();
    static QString binaryTable();
    static QString typeTable();
    
    void pvt_saveParameter(const char *type_name, const QString &name, 
                           const QVariant &value);
    QVariant pvt_loadParameter(const char *type_name, const QString &name);
    
    void initialise(const char *type_name, DBBase &dbcomp);
    void restoreComponent(const char *type_name);
    
    /** Connection to the SQL database that stores the parameters */
    QSqlDatabase db;
    
    /** Unique name to identify this database */
    QString dbname;
    
    typedef QHash<QString, DBBase*> hash_type;
    
    /** The components of this database, indexed by their type name */
    hash_type dbparts;
    
    /** A log of all of the database events that occured during its lifetime
        - mainly this provides a record of the 
        decisions that this database had to make, e.g. when it had to replace parameters,
        or when it had select one from a choice etc. */
    QString dblog;

    /** The number of loaded relationships */
    RelateID nrelationships;
    
    /** The number of loaded parameters */
    ParamID nparams;
};

}

#include "dbbase.h"

namespace SireDB
{

/** Save the parameter called 'name' to the global parameter table. This replaces
    any existing parameter with the same name. Note that name-lookups are
    case-sensitive */
template<class T>
inline void ParameterDB::saveParameter(const QString &name, const QVariant &value)
{
    this->pvt_saveParameter( T::typeName(), name, value );
}

/** Load and return the parameter called 'name' in the global parameter table,
    or return a null QVariant if there is no such parameter. Note that 
    name-lookups are case-sensitive. */
template<class T>
inline QVariant ParameterDB::loadParameter(const QString &name)
{
    return this->pvt_loadParameter( T::typeName(), name );
}

/** Return whether or not this database has the component 'T' */
template<class T>
bool ParameterDB::isA() const
{
    hash_type::const_iterator it = dbparts.find( T::typeName() );
    return it != dbparts.end() and it.value()->isA<T>();
}

/** Return this database cast as the component 'T' - note that this 
    will return undefined results unless 'isA<T>()' returns true. */
template<class T>
const T& ParameterDB::asA() const
{
    return dbparts.find( T::typeName() ).value()->asA<T>();
}

/** Return this database cast as the component 'T' - note that this 
    will return undefined results unless 'isA<T>()' returns true. */
template<class T>
T& ParameterDB::asA()
{
    return dbparts.find( T::typeName() ).value()->asA<T>();
}

/** Add the component of type 'T' to this database. This does nothing if this
    component is already part of this database */
template<class T>
T& ParameterDB::addComponent()
{
    //do we already have a component of this type?
    QLatin1String type_name( T::typeName() );

    hash_type::iterator it = dbparts.find(type_name);
    
    if (it != dbparts.end())
    {
        return dynamic_cast<T&>( *(it.value()) );
    }

    //no - so lets create one :-)
    std::auto_ptr<T> dbptr( new T() );
    
    //initialise this database component
    this->initialise(T::typeName(), *dbptr);
    
    return *(dbptr.release());
}

}

Q_DECLARE_METATYPE(SireDB::ParameterDB)

SIRE_END_HEADER

#endif
