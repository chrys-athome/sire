
#include <QSet>
#include <QDebug>
#include <QLatin1String>

#include "assignbase.h"
#include "using_relationships.hpp"
#include "using_parameters.hpp"

#include "SireError/errors.h"
#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireDB;

static const RegisterMetaType<AssignBase> r_assignbase(MAGIC_ONLY, "SireDB::AssignBase");

/** Serialise to a binary data stream */
QDataStream SIREDB_EXPORT &operator<<(QDataStream &ds, const SireDB::AssignBase &assign)
{
    writeHeader(ds, r_assignbase, 1)
            << assign.paramdbs << assign.relatedbs << quint32(assign._overwrite);

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREDB_EXPORT &operator>>(QDataStream &ds, SireDB::AssignBase &assign)
{
    VersionID v = readHeader(ds, r_assignbase);

    if (v == 1)
    {
        quint32 overwrite;
        ds >> assign.paramdbs >> assign.relatedbs >> overwrite;

        assign._overwrite = overwrite;
    }
    else
        throw version_error(v, "1", r_assignbase, CODELOC);

    return ds;
}

/** Construct an assigner that assigns parameters from the parameter database
    classes listed in parameter_databases, using the relationships stored in
    the relationship database classes listed in relationship_databases. */
AssignBase::AssignBase() : AssignInstruction(), _overwrite(false)
{}

/** Copy constructor */
AssignBase::AssignBase(const AssignBase &other)
           : AssignInstruction(other),
             paramdbs(other.paramdbs), relatedbs(other.relatedbs),
             _overwrite(other._overwrite)
{}

/** Destructor */
AssignBase::~AssignBase()
{}

/** Add a collection of relationship database types to use */
void AssignBase::addDataBase(const using_relationships_base &relationshipdbs)
{
    foreach( QString type_name, relationshipdbs )
    {
        this->addRelationshipDataBase( type_name );
    }
}

/** Add a collection of parameter database types to use */
void AssignBase::addDataBase(const using_parameters_base &parameterdbs)
{
    foreach( QString type_name, parameterdbs )
    {
        this->addParameterDataBase( type_name );
    }
}

/** Add a set of database requirements to the list */
void AssignBase::addDataBase(const using_database &dbs)
{
    //is this a set of parameter databases?
    const using_parameters_base *parameterdbs =
                      dynamic_cast<const using_parameters_base*>(&dbs);

    if (parameterdbs)
    {
        addDataBase( *parameterdbs );
        return;
    }

    //is this a set of relationship databases?
    const using_relationships_base *relatedbs =
                      dynamic_cast<const using_relationships_base*>(&dbs);

    if (relatedbs)
    {
        addDataBase( *relatedbs );
        return;
    }

    //we should never get here! (though we do get here if the
    //class is not exported (e.g. SIREDB_EXPORT is required for SireDB
    //classes so that they can be dynamic-casted and be visible
    //outside of libSireDB

    throw SireError::program_bug(QObject::tr(
              "Cannot work out the type of the using_database "
              "requirements (typeid == \"%1\")")
                  .arg( typeid(dbs).name() ), CODELOC);
}

/** Append a parameter database to the list - this does nothing if the
    database is already in the list. */
void AssignBase::addParameterDataBase(QString db_name)
{
    if (not paramdbs.contains(db_name))
        paramdbs.append(db_name);
}

/** Append a relationship database to the list - this does nothing if the
    database is already in the list. */
void AssignBase::addRelationshipDataBase(QString db_name)
{
    if (not relatedbs.contains(db_name))
        relatedbs.append(db_name);
}

/** Return the list of parameter databases used to assign parameters */
QStringList AssignBase::parameterDataBases() const
{
    return paramdbs;
}

/** Return the list of relationship databases used to assign parameters */
QStringList AssignBase::relationshipDataBases() const
{
    return relatedbs;
}

/** Set whether or not the parameters listed in this assignment are
    overwritten on assignment */
void AssignBase::setOverwriteParameters(bool flag)
{
    _overwrite = flag;
}

/** Return whether or not this instructs that the parameters
    will be overwritten on assignment. By default, they are not. */
bool AssignBase::overwriteParameters() const
{
    return _overwrite;
}
