
#include "relationshipdb.h"

#include "SireStream/version_error.h"

using namespace SireDB;
using namespace SireStream;

/** Constructor */
RelationshipDB::RelationshipDB() : DBBase()
{}

/** Copy constructor */
RelationshipDB::RelationshipDB(const RelationshipDB &other) : DBBase(other)
{}

/** Destructor */
RelationshipDB::~RelationshipDB()
{}
    
/** Dump the version number of this database */
void RelationshipDB::prepareToDump()
{
    DBBase::prepareToDump();
    
    this->saveParameter<RelationshipDB>( QString("%1_version").arg(prefix()), 0 );
}

/** Check the version number */
void RelationshipDB::postLoad()
{
    DBBase::postLoad();

    int v = this->loadParameter<RelationshipDB>( 
                            QString("%1_version").arg(prefix()) ).toInt();
                            
    if (v != 0)
        throw version_error( v, "0", "SireDB::RelationshipDB", CODELOC );
}
