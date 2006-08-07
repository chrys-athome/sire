
#include "SireMol/qhash_siremol.h"

#include "parametertable.h"
#include "tablebase.h"
#include "parameterdb.h"
#include "assign_parameters.h"

#include "SireStream/datastream.h"
#include "SireError/errors.h"

using namespace SireStream;
using namespace SireDB;

static const RegisterMetaType<ParameterTable> r_paramtable("SireDB::ParameterTable");

/** Serialise to a binary data stream */
QDataStream SIREDB_EXPORT &operator<<(QDataStream &ds, const ParameterTable &table)
{
    writeHeader(ds, r_paramtable, 1);
        
    //save the molecule...
    ds << table.mol;
        
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
        ds >> table.mol;
        
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

/** Construct a table that holds the parameters for the Molecule 'molecule' */
ParameterTable::ParameterTable(const Molecule &molecule)
               : mol(molecule)
{}               

/** Copy constructor. All of the tables are implicitly shared, so 
    this should be quick. */
ParameterTable::ParameterTable(const ParameterTable &other)
               : mol(other.mol), tables(other.tables)
{}

/** Destructor */
ParameterTable::~ParameterTable()
{}

/** Return whether or not this table is empty (has no molecule) */
bool ParameterTable::isEmpty() const
{
    return mol.isEmpty();
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

/** Return this table as a 'type_name' - note that this is not cast
    to the right type, but is instead a reference to the common base
    class of all tables. This will throw an exception if this 
    is not a table of type 'type_name'
    
    Note that the referenced returned is temporary - it can be 
    made invalid by a call to 'removeTable' or 'setTable'
    
    \throw SireError::invalid_cast
*/
TableBase& ParameterTable::asA(const QString &type_name)
{
    hash_type::iterator it = tables.find(type_name);
    
    if (it == tables.end())
        throw SireError::invalid_cast( QObject::tr(
                "Cannot cast the ParameterTable to type \"%1\". Available "
                "types are [ %2 ]")
                    .arg(type_name, types().join(", ")), CODELOC );

    return *(it.value());
}

/** Internal function used to create a table from a QString type name 

    Throws an exception if there is no type with this type name 
    registered within QMetaData
    
    \throw SireError::unknown_type
*/
DynamicSharedPtr<TableBase> createTable(const QString &type_name)
{
    //use QMetaType to create the table :-)
    int id = QMetaType::type( type_name.toLatin1().constData() );

    if ( id == 0 or not QMetaType::isRegistered(id) )
        throw SireError::unknown_type(QObject::tr(
              "There is no table with type \"%1\" available. Ensure that "
              "this type is loaded and that it has been registered with "
              "QMetaData.").arg(type_name), CODELOC);
              
    DynamicSharedPtr<TableBase> table_ptr = 
                        static_cast<TableBase*>(QMetaType::construct(id,0));
    
    if ( table_ptr.data() == 0 )
        throw SireError::program_bug(QObject::tr(
              "Could not construct a table of type \"%1\" despite it having "
              "been registered with QMetaData (ID = %2)")
                  .arg(type_name).arg(id), CODELOC);

    return table_ptr;
}

/** Create the table of type 'type_name' using the default 
    molecule constructor - Table(molecule)
    
    This does nothing if this table already exists
    
    Throws an exception if there is no type with this type name 
    registered within QMetaData
    
    \throw SireError::unknown_type
*/
TableBase& ParameterTable::addTable(const QString &type_name)
{
    //do nothing if there is already
    //a table with this type_name
    hash_type::iterator it = tables.find(type_name);
    if (it != tables.end())
        return *(it.value());
    else
    {
        DynamicSharedPtr<TableBase> table_ptr = createTable(type_name);
                    
        //If we have a molecule then re-create the table with the desired molecule
        if ( not mol.isNull() )
            table_ptr = table_ptr->create(mol);
                  
        //save the table with the others :-)
        return *(tables.insert( type_name, table_ptr ).value());
    }
}

/** Create the table of type 'type_name' using the default 
    molecule constructor - Table(molecule)
    
    This table then replaces any existing table of this type.
    
    Throws an exception if there is no type with this type name 
    registered within QMetaData
    
    \throw SireError::unknown_type
*/
TableBase& ParameterTable::setTable(const QString &type_name)
{
    DynamicSharedPtr<TableBase> table_ptr = createTable(type_name);
                  
    //If we have a molecule then re-create the table with the desired molecule
    if ( not mol.isNull() )
        table_ptr = table_ptr->create(mol);
                  
    //save the table with the others (overwriting any existing table)
    return *(tables.insert( type_name, table_ptr ).value());
}

/** Remove the table with type 'type_name' - this does nothing if this 
    table is not in the ParameterTable */
void ParameterTable::removeTable(const QString &type_name)
{
    tables.remove(type_name);
}

/** Add the table 'table'. This will check that this table is 
    compatible with the other tables in this object.
    
    This will add a new table if there is no currenly existing table
    with the same type - otherwise it will merge the two tables
    together, with the parameters in the new table overwriting
    the parameters in the existing table.
    
    This will throw an exception if the new table is not compatible
    with the molecule in this object.
    
    \throw SireError::incompatible_error
*/
TableBase& ParameterTable::addTable(const TableBase &table)
{
    //is the table empty?
    if (table.isEmpty())
    {
        //do we already have a table of this type?
        QString type_name = table.what();
        
        hash_type::iterator it = tables.find(type_name);
        
        if (it != tables.end())
            return *(it.value());
        else
            //we don't yet have a table of this type - add a 
            //default molecule-constructed version of the table
            return *(tables.insert( type_name, table.create(mol) ).value());
    }
    else
    {
        //Check that this table is compatible with our molecule.
        //This will throw a SireError::incompatible_error if this
        //is not possible
        if (not table.isCompatibleWith(mol))
            throw SireError::incompatible_error( QObject::tr(
                      "Cannot add the table of type %1 as it has an incompatible "
                      "molcule!").arg(table.what()), CODELOC );
    
        //do we already have an existing table of this type?
        QString type_name = table.what();
    
        hash_type::iterator it = tables.find(type_name);
        
        if (it != tables.end())
            //merge the new table into the old
            tables[type_name]->add(table);
        else
            //save the new table
            it = tables.insert( type_name, table );
            
        return *(it.value());
    }
}

/** Add the table 'table', replacing any existing tables of this type.

    This will throw an exception if the new table is not compatible
    with the molecule in this object. This object will not be changed
    if an exception is thrown.
    
    \throw SireError::incompatible_error
*/
TableBase& ParameterTable::setTable(const TableBase &table)
{    
    if (table.isEmpty())
    {
        //set the table to be a default-molecule constructed table
        return *(tables.insert( table.what(), table.create(mol) ).value());
    }
    else
    {
        //Check that this table is compatible with our molecule.
        if (not table.isCompatibleWith(mol))
            throw SireError::incompatible_error( QObject::tr(
                      "Cannot set the table of type %1 as it holds an incompatible "
                      "molecule!").arg(table.what()), CODELOC );
    
        //save the table with the others (replacing any existing
        //table of the same type)
        return *(tables.insert( table.what(), table ).value());
    }
}

/** Set the molecule for the tables to be 'molecule'. This will clear all of 
    the parameters if the info() for 'molecule' is different to the info()
    for the molecule already present in this table
    
    (e.g. molecule.info() != this->molecule().info())
    
    otherwise the molecule will be changed while the parameters will be 
    retained. 
*/
void ParameterTable::setMolecule( const Molecule &molecule )
{
    if (molecule.info() == mol.info())
        //the molecules are the same - just update the molecule
        mol = molecule;
    else
    {
        //the molecules are different - clear all of the parameters
        //and reconstruct the tables to have space for the parameters
        //for the new molecule
        mol = molecule;
        
        for (hash_type::iterator it = tables.begin();
             it != tables.end();
             ++it)
        {
            it.value() = it.value()->create(mol);
        }
    }
}

/** Assign more parameters into this table from the database 'db'
    using the assignment instructions in 'assign_instruct' */
void ParameterTable::assign( ParameterDB &db, 
                             const assign_parameters &assign_instruct,
                             const MatchMRData &matchmr )
{
    *this = assign_instruct.assign(*this, db, matchmr);
}
