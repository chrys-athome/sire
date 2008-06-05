
#include "term13db.h"

#include "SireCAS/expression.h"
#include "SireStream/version_error.h"

using namespace SireDB;
using namespace SireStream;

/** Create a Term13DB. */
Term13DB::Term13DB() : FuncDB()
{}

/** Copy constructor */
Term13DB::Term13DB(const Term13DB &other) : FuncDB(other)
{}

/** Destructor */
Term13DB::~Term13DB()
{}
    
/** Dump the version number of this database */
void Term13DB::prepareToDump()
{
    FuncDB::prepareToDump();
    
    this->saveParameter<Term13DB>( "version", 0 );
}

/** Check the version number */
void Term13DB::postLoad()
{
    FuncDB::postLoad();

    int v = this->loadParameter<Term13DB>( "version" ).toInt();
                            
    if (v != 0)
        throw version_error( v, "0", "SireDB::Term13DB", CODELOC );
}

/** Relate the match for a triple of atoms 'matchtriple' to the expression identified
    via the user identification string 'userid' */
void Term13DB::relateFunc(const AssertMatch<3> &matchtriple, const QString &userid,
                          bool mirrored)
{
    RelateID relateid = matchtriple.addTo(parent());
    relateParameter(relateid, userid);
    
    if (mirrored)
    {
        RelateID mirrorid = matchtriple.addMirroredTo(parent());
        
        if (mirrorid != relateid)
            relateParameter(mirrorid, userid);
    }
}

/** Relate the match for a triple of atoms 'matchtriple' to the expression 'func' */
void Term13DB::relateFunc(const AssertMatch<3> &matchtriple, const Expression &func,
                          bool mirrored)
{
    RelateID relateid = matchtriple.addTo(parent());
    
    //add the function
    ParamID paramid = addFunc(func);
    
    relateParameter(relateid, paramid);
    
    if (mirrored)
    {
        RelateID mirrorid = matchtriple.addMirroredTo(parent());
        
        if (mirrorid != relateid)
            relateParameter(mirrorid, paramid);
    }
}
