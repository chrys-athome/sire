
#include "atomdb.h"

#include "SireStream/version_error.h"

using namespace SireDB;
using namespace SireStream;

/** Constructor */
AtomDB::AtomDB() : RelateDB()
{}

/** Copy constructor */
AtomDB::AtomDB(const AtomDB &other) : RelateDB(other)
{}

/** Destructor */
AtomDB::~AtomDB()
{}
    
/** Dump the version number of this database */
void AtomDB::prepareToDump()
{
    RelateDB::prepareToDump();
    
    this->saveParameter<AtomDB>( QString("%1_version").arg(prefix()), 0 );
}

/** Check the version number */
void AtomDB::postLoad()
{
    RelateDB::postLoad();

    int v = this->loadParameter<AtomDB>( 
                            QString("%1_version").arg(prefix()) ).toInt();
                            
    if (v != 0)
        throw version_error( v, "0", "SireDB::AtomDB", CODELOC );
}
