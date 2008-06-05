
#include "dbbase.h"

#include <QDebug>

#include "SireStream/version_error.h"
#include "SireError/errors.h"

using namespace SireDB;
using namespace SireStream;

/** Constructor */
DBBase::DBBase() : prnt(0)
{}

/** Copy constructor - note that you can only copy a null database! You 
    can only copy a database by copying the parent ParameterDB

    \throw SireError::program_bug
*/
DBBase::DBBase(const DBBase &other) : prnt(0)
{
    if (other.prnt)
        throw SireError::program_bug(QObject::tr(
                          "You cannot copy a non-null DBBase!"), CODELOC);
}

/** Destructor */
DBBase::~DBBase()
{}

/** Return a string representation of this database */
QString DBBase::toString() const
{
    return this->what();
}

/** Set the parent of this database component - note that this is necessary 
    to be able to use the component. Note that you can't change the parent 
    once it has been set 
    
    \throw SireError::program_bug
*/
void DBBase::setParent(ParameterDB *parent)
{
    if (parent and prnt)
        throw SireError::program_bug(QObject::tr(
                          "Cannot change the parent of a parameter database component!"),
                              CODELOC);
                              
    prnt = parent;
    
    //get the table prefix - this can be used to ensure that common
    //base classes all work in their own namespace in the database
    table_prefix = QString( this->what() ).replace("::","_");
}

/** Empty initialisation */
void DBBase::initialise()
{
    BOOST_ASSERT( prnt != 0 );
}
    
/** Dump the version number of this database */
void DBBase::prepareToDump()
{
    this->saveParameter<DBBase>( QString("%1_version").arg(prefix()), 0 );
}

/** Check the version number */
void DBBase::postLoad()
{
    int v = this->loadParameter<DBBase>( 
                            QString("%1_version").arg(prefix()) ).toInt();
                            
    if (v != 0)
        throw version_error( v, "0", "SireDB::FuncDB", CODELOC );
}
