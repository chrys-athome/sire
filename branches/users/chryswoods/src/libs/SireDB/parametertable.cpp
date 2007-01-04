
#include "SireMol/qhash_siremol.h"

#include <QLatin1String>

#include "parametertable.h"
#include "tablebase.h"
#include "parameterdb.h"
#include "assign_parameters.h"

#include "SireMol/molecule.h"
#include "SireMol/moleculeinfo.h"

#include "SireStream/datastream.h"
#include "SireError/errors.h"

using namespace SireStream;
using namespace SireDB;

static const RegisterMetaType<ParameterTable> r_paramtable;

/** Serialise to a binary data stream */
QDataStream SIREDB_EXPORT &operator<<(QDataStream &ds, const ParameterTable &table)
{
    writeHeader(ds, r_paramtable, 1);

    //save the molecule...
    ds << table.molinfo;

    //save the number of tables that are in the parametertable
    ds << quint32(table.tables.count());

    //run through each table in the parametertable save each table
    //(DynamicSharedPtr can do polymorphic saves!)
    for (ParameterTable::hash_type::const_iterator it = table.tables.begin();
         it != table.tables.end();
         ++it)
    {
        ds << *it;
    }

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREDB_EXPORT &operator>>(QDataStream &ds, ParameterTable &table)
{
    VersionID v = readHeader(ds, r_paramtable);

    if (v == 1)
    {
        //load the molecule
        ds >> table.molinfo;

        //clear any existing tables in the object
        table.tables.clear();

        //how many tables need to be loaded?
        quint32 ntables;
        ds >> ntables;

        for (uint i=0; i<ntables; ++i)
        {
            DynamicSharedPtr<TableBase> tableptr;

            //load the table - DynamicSharedPtr can do polymorphic loads :-)
            ds >> tableptr;

            //the table has been loaded - add it to the parameter table...
            table.tables.insert(tableptr->what(), tableptr);
        }
    }
    else
        throw version_error(v, "1", r_paramtable, CODELOC);

    return ds;
}

/** Construct an empty table */
ParameterTable::ParameterTable()
{}

/** Construct a table that holds the parameters for the
    molecule described by 'info' */
ParameterTable::ParameterTable(const MoleculeInfo &info)
               : molinfo(info)
{}

/** Copy constructor. All of the tables are implicitly shared, so
    this should be quick. */
ParameterTable::ParameterTable(const ParameterTable &other)
               : molinfo(other.molinfo), tables(other.tables)
{}

/** Destructor */
ParameterTable::~ParameterTable()
{}

/** Assert that the table 'table' is compatible with this
    ParameterTable

    \throw SireError::incompatible_error
*/
void ParameterTable::assertTableCompatible(const TableBase &table) const
{
    //ensure that the table is compatible with the molecule
    if ( not table.isCompatibleWith(molinfo) )
        throw SireError::incompatible_error( QObject::tr(
                  "Cannot add the table \"%1\" to this ParameterTable, as it is "
                  "incompatible with this table (this table is for the molecule "
                  "\"%2\" while the new table is for the molecule \"%3\")")
                      .arg(table.what()).arg(table.info().name())
                      .arg(molinfo.name()), CODELOC );
}

/** Assert that this table is compatible with the molecule 'molecule'

    \throw SireError::incompatible_error
*/
void ParameterTable::assertCompatibleWith(const Molecule &molecule) const
{
    if ( molecule.info() != molinfo )
        throw SireError::incompatible_error( QObject::tr(
                "The molecule \"%1\" is not compatible with this ParameterTable. "
                "This parameter table is used to store parameters for the molecule "
                "\"%2\".")
                    .arg(molecule.name(), molinfo.name()), CODELOC );
}

/** Return whether or not this table is empty (has no molecule) */
bool ParameterTable::isEmpty() const
{
    return molinfo.isEmpty();
}

/** Return a list of the type names of all of the table types in this
    table */
QStringList ParameterTable::types() const
{
    QStringList ret(tables.keys());
    ret.sort();
    return ret;
}

/** Return whether or not this is a Parameter table of type 'type_name' */
bool ParameterTable::isA(const QString &type_name) const
{
    return tables.contains(type_name);
}

/** Return this table as a 'type_name' - note that this is not cast
    to the right type, but is instead a reference to the common base
    class of all tables. This will throw an exception if this
    is not a table of type 'type_name'

    Note that the referenced returned is temporary - it can be
    made invalid by a call to 'removeTable' or 'setTable'

    \throw SireError::invalid_cast
*/
const TableBase& ParameterTable::asA(const QString &type_name) const
{
    hash_type::const_iterator it = tables.find(type_name);

    if (it == tables.end())
        throw SireError::invalid_cast( QObject::tr(
                "Cannot cast the ParameterTable to type \"%1\". Available "
                "types are [ %2 ]")
                    .arg(type_name, types().join(", ")), CODELOC );

    return *(it.value());
}

/** Create a table with type 'type_name'

    Throws an exception if there is no type with this type name
    registered within QMetaData

    This is a dangerous function to use, as it will have
    undefined results if 'type_name' does not describe a
    type derived from TableBase.

    \throw SireError::unknown_type
*/
void ParameterTable::createTable(const QString &type_name)
{
    //don't do anything if we already contain this table
    if (tables.contains(type_name))
        return;

    //use QMetaType to create the table :-)
    int id = QMetaType::type( type_name.toLatin1().constData() );

    if ( id == 0 or not QMetaType::isRegistered(id) )
        throw SireError::unknown_type(QObject::tr(
              "There is no table with type \"%1\" available. Ensure that "
              "this type is loaded and that it has been registered with "
              "QMetaData.").arg(type_name), CODELOC);

    DynamicSharedPtr<TableBase> table_ptr =
                        static_cast<TableBase*>(QMetaType::construct(id,0));

    //we need to reconstruct the table so that it can
    //store the parameters for the molecule
    table_ptr = DynamicSharedPtr<TableBase>( table_ptr->create(molinfo) );

    if ( table_ptr.data() == 0 )
        throw SireError::program_bug(QObject::tr(
              "Could not construct a table of type \"%1\" despite it having "
              "been registered with QMetaData (ID = %2)")
                  .arg(type_name).arg(id), CODELOC);

    tables.insert( type_name, table_ptr );
}

/** Add the table of parameters 'table' to this ParameterTable.
    This will add all of the existing parameters from 'table'
    to this ParameterTable.

    \throw SireError::incompatible_error
*/
void ParameterTable::addTable(const TableBase &table)
{
    #warning Need to upgrade to named tables

    this->assertTableCompatible(table);

    QLatin1String tabletype( table.what() );

    if ( tables.contains(tabletype) )
    {
        tables[tabletype]->add(table);
    }
    else
    {
        tables.insert( tabletype, DynamicSharedPtr<TableBase>(table.clone()) );
    }
}

/** Set the table in this ParameterTable to 'table'. This
    will replace any existing table of this type.

    \throw SireError::incompatible_error
*/
void ParameterTable::setTable(const TableBase &table)
{
    this->assertTableCompatible(table);

    tables.insert( table.what(), DynamicSharedPtr<TableBase>(table.clone()) );
}

/** Remove the table with type 'type_name' - this does nothing if this
    table is not in the ParameterTable */
void ParameterTable::removeTable(const QString &type_name)
{
    tables.remove(type_name);
}

/** Assign more parameters into this table from the database 'db'
    using the assignment instructions in 'assign_instruct'

    \throw SireError::incompatible_error
*/
void ParameterTable::assign( const Molecule &molecule,
                             ParameterDB &db,
                             const assign_parameters &assign_instruct,
                             const MatchMRData &matchmr )
{
    *this = assign_instruct.assign(molecule, *this, db, matchmr);
}
