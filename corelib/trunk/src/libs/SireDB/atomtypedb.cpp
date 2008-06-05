
#include "atomtypedb.h"
#include "atomtypetable.h"
#include "parametertable.h"

#include <QSqlQuery>

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireDB;
using namespace SireStream;

static const RegisterMetaType<AtomTypeDB> r_atomtypedb("SireDB::AtomTypeDB", NO_STREAM);

/** Constructor */
AtomTypeDB::AtomTypeDB() : AtomDB()
{}

/** Copy constructor */
AtomTypeDB::AtomTypeDB(const AtomTypeDB &other) : AtomDB(other)
{}

/** Destructor */
AtomTypeDB::~AtomTypeDB()
{}

/** This function creates all of the tables in this database used to store and 
    find atom types. There is only one table in this component.

    (1) table giving a unique parameter ID to each atom type that has been loaded
*/
void AtomTypeDB::initialise()
{
    //initialise the AtomDB
    AtomDB::initialise();
    
    //create table (1) - table to give a unique ID number to each atom type
    //  columns  id_number(*)   atom_type string,  element integer
    QString error = executeSQL(
        "create table 'AtomTypeDB' ( ParamID INTEGER, AtomType TEXT, Element INTEGER, "
                                    "Primary Key (ParamID), UNIQUE (AtomType,Element) )");

    if (not error.isNull())
        throw SireDB::db_error(error, CODELOC);
}
    
/** Dump the version number of this database */
void AtomTypeDB::prepareToDump()
{
    AtomDB::prepareToDump();
    this->saveParameter<AtomTypeDB>( "version", 1 );
}

/** Check the version number */
void AtomTypeDB::postLoad()
{
    AtomDB::postLoad();

    int v = this->loadParameter<AtomTypeDB>( "version" ).toInt();
                            
    if (v != 1)
        throw version_error( v, "1", "SireDB::AtomTypeDB", CODELOC );
}

/** Add the atom type 'atomtype' to the database, returning the internal ID number of the 
    atom type parameter. This will add a new parameter if this atom type does not exist, or will
    return the id number of the already existing atom type */
ParamID AtomTypeDB::addAtomType(const AtomType &atomtype)
{
    QSqlQuery q(database());
    
    QString format_atomtype = formatField(database(), atomtype.ID());
    
    q.exec( QString("select ParamID from 'AtomTypeDB' where AtomType = %1 and "
                   "Element = %2").arg(format_atomtype).arg(atomtype.element().nProtons()) );
    checkErrors(q,CODELOC);
    q.next();
    
    if (q.isValid())
        //we have found the existing charge parameter - return the ID number
        return q.value(0).toUInt();
    else
    {
        //we need to insert the charge, and then return the ID
        ParamID paramid = getNewParamID();
        q.exec( QString("insert into 'AtomTypeDB' values (%1, %2, %3)")
                      .arg(paramid).arg(format_atomtype).arg(atomtype.element().nProtons()) );
        checkErrors(q,CODELOC);
        return paramid;
    }
}

/** Return the atom type with parameter ID 'id', or record the missing parameter 
    in the log and return a dummy type */
AtomType AtomTypeDB::retrieveAtomType(ParamID id)
{
    if (id == 0)
        return AtomType::dummy();
    else
    {
        QSqlQuery q(database());
        q.exec(QString("select AtomType, Element from 'AtomTypeDB' "
                                                  "where ParamID = %1").arg(id));
        checkErrors(q,CODELOC);
        q.next();
    
        if (q.isValid())
            return AtomType( q.value(0).toString(), Element(q.value(1).toInt()) );
        else
        {
            addLog( QString("**WARNING** Missing atom type parameter with ParamID '%1'")
                          .arg(id) );
            return AtomType::dummy();
        }
    }
}

/** Add an atom type to the database, associated with the user ID string 'userid' */
void AtomTypeDB::addAtomType(const QString &userid, const AtomType &atomtype)
{
    //add the charge...
    ParamID paramid = addAtomType(atomtype);
    
    //associate this parameter ID with the userid
    relateParameter(userid, paramid);
}

/** Return the atom type with userid 'userid'. Returns a dummy type if there is no
    such parameter. foundtype will be set to whether or not a parameter was found */
AtomType AtomTypeDB::getAtomType(const QString &userid, bool *foundtype)
{
    //get the parameter ID for this userid
    ParamID paramid = getParameter(userid);
    
    //set the flag saying whether or not the parameter exists
    if (foundtype)
        *foundtype = (paramid != 0);
        
    return retrieveAtomType(paramid);
}
    
/** Relate the atom type identified by user ID string 'userid' to the atom matching 
    criteria 'matchatom' */
void AtomTypeDB::relateAtomType(const AssertMatch<1> &matchatom, const QString &userid)
{
    //get the relationship ID of this match
    RelateID relateid = matchatom.addTo(parent());
    
    //associate this relationship with the userID
    relateParameter(relateid, userid);
}

/** Relate the atom type to the atom matching criteria 'matchatom' */
void AtomTypeDB::relateAtomType(const AssertMatch<1> &matchatom, const AtomType &atomtype)
{
    //get the relationship ID of this match
    RelateID relateid = matchatom.addTo(parent());
    
    //add the atom type
    ParamID paramid = addAtomType(atomtype);
    
    //associate this relationship with the parameter
    relateParameter(relateid, paramid);
}

/** Relate the atom type identified by the user ID string 'userid' to the matching criteria
    identified by the relationship ID 'relateid' */
void AtomTypeDB::relateAtomType(RelateID relateid, const QString &userid)
{
    relateParameter(relateid, userid);
}

/** Relate the atom type to the matching criteria identified by the relationship
    ID 'relateid' */
void AtomTypeDB::relateAtomType(RelateID relateid, const AtomType &atomtype)
{
    //add the atom type
    ParamID paramid = addAtomType(atomtype);
    //associate this relationship with the parameter
    relateParameter(relateid, paramid);
}

/** Return the atom type that matches the relationship ID 'relateid'. Returns
    a dummy atom type if there is no such type, and sets 'foundtype' to true 
    or false depending on whether or not a parameter was found. */
AtomType AtomTypeDB::getAtomType(RelateID relateid, bool *foundtype)
{
    //get the paramID of the relationship
    ParamID paramid = getParameter(relateid);
    
    //set the flag saying whether or not the parameter was found
    if (foundtype)
        *foundtype = (paramid != 0);
        
    //return the atom type
    return retrieveAtomType(paramid);
}

/** Return the best atom type matching the relationship IDs in 'relateids'. Returns
    a dummy type if there is no such parameter. Sets 'foundtype' to whether or not 
    a parameter was found. */
AtomType AtomTypeDB::getAtomType(const RelateIDMap &relateids, bool *foundtype)
{
    //get the ParamID of the highest scoring and matching relationship
    ParamID paramid = getParameter(relateids);
    
    //set the flag saying whether or not the parameter was found
    if (foundtype)
        *foundtype = (paramid != 0);
        
    //return the atom type
    return retrieveAtomType(paramid);
}

/** Create a table in 'param_table' that is capable of holding the atom type 
    parameters - this does nothing if there is already such a table.
     
    This returns a reference to the created table.
*/
AtomTypeTable& AtomTypeDB::createTable( ParameterTable &param_table ) const
{
    return param_table.addTable<AtomTypeTable>();
}

/** Assign the atom type parameter for the atom 'atom' using the relationship IDs
    in 'relateids' and place the parameter into the parameter table. This does
    nothing if the parameter is not found. This returns whether or not the
    parameter was found. */
bool AtomTypeDB::assignParameter( const AtomIndex &atom, const RelateIDMap &relateids,
                                  ParameterTable &param_table )
{
    bool found;
    AtomType atomtype = getAtomType(relateids, &found);
    
    if (found)
    {
        AtomTypeTable &table = param_table.addTable<AtomTypeTable>();
        table.setAtomType(atom, atomtype);
    }
    
    return found;
}
