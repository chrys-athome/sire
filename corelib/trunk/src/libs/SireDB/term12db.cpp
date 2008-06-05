
#include "term12db.h"

#include "SireCAS/expression.h"
#include "SireStream/version_error.h"

using namespace SireDB;
using namespace SireStream;

/** Create a Term12DB. */
Term12DB::Term12DB() : FuncDB()
{}

/** Copy constructor */
Term12DB::Term12DB(const Term12DB &other) : FuncDB(other)
{}

/** Destructor */
Term12DB::~Term12DB()
{}
    
/** Dump the version number of this database */
void Term12DB::prepareToDump()
{
    FuncDB::prepareToDump();
    
    this->saveParameter<Term12DB>( "version", 0 );
}

/** Check the version number */
void Term12DB::postLoad()
{
    FuncDB::postLoad();

    int v = this->loadParameter<Term12DB>( "version" ).toInt();
                            
    if (v != 0)
        throw version_error( v, "0", "SireDB::Term12DB", CODELOC );
}

/** Relate the match for a pair of atoms 'matchpair' to the expression identified
    via the user identification string 'userid' */
void Term12DB::relateFunc(const AssertMatch<2> &matchpair, const QString &userid,
                          bool mirror)
{
    RelateID relateid = matchpair.addTo(parent());
    relateParameter(relateid, userid);
    
    if (mirror)
    {
        //relate the reversed relationship
        RelateID mirrorid = matchpair.addMirroredTo(parent());
        
        if (mirrorid != relateid)
            relateParameter(mirrorid, userid);
    }
}

/** Relate the match for a pair of atoms 'matchpair' to the expression 'func' */
void Term12DB::relateFunc(const AssertMatch<2> &matchpair, const Expression &func,
                          bool mirror)
{
    RelateID relateid = matchpair.addTo(parent());
    
    //add the function
    ParamID paramid = addFunc(func);
    
    relateParameter(relateid, paramid);
    
    if (mirror)
    {
        //relate the reversed relationship
        RelateID mirrorid = matchpair.addMirroredTo(parent());
        
        if (mirrorid != relateid)
            relateParameter(mirrorid, paramid);
    }
}
