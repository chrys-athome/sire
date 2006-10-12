
#include "SireCAS/qhash_sirecas.h"
#include "SireMol/qhash_siremol.h"
#include "SireMM/qhash_siremm.h"

#include <QDebug>

#include "bonddb.h"

#include "SireCAS/expression.h"
#include "SireCAS/values.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireMM;
using namespace SireCAS;
using namespace SireDB;

static const RegisterMetaType<BondDB> r_bonddb("SireMM::BondDB", NO_STREAM);

/** Constructor */
BondDB::BondDB() : Term12DB(), InternalDB<MolBondInfo>(), _r("r")
{}

/** Copy constructor */
BondDB::BondDB(const BondDB &other) 
       : Term12DB(other), InternalDB<MolBondInfo>(), _r(other._r)
{}

/** Destructor */
BondDB::~BondDB()
{}
    
/** Dump the version number of this database */
void BondDB::prepareToDump()
{
    Term12DB::prepareToDump();
    
    this->saveParameter<BondDB>( "version" , 0 );
}

/** Check the version number */
void BondDB::postLoad()
{
    Term12DB::postLoad();

    int v = this->loadParameter<BondDB>( "version" ).toInt();
                            
    if (v != 0)
        throw version_error( v, "0", "SireMM::BondDB", CODELOC );
}

/** Add the bond function 'bondfunc' and associate it with the user identification 
    string 'userid'. Note that 'bondfunc' must be a function of the variable
    representing the bond length ("r", obtainable via Symbol("r") or BondDB::r()) 
    or else it will be treated as a constant. */
void BondDB::addBond(const QString &userid, const Expression &bondfunc)
{
    if (not bondfunc.isFunction(_r))
    {
        //evaluate this constant...
        Expression const_val = bondfunc.evaluate(Values());
        
        ParamID paramid = addFunc(const_val);
        relateParameter(userid, paramid);
    }
    else
    {
        ParamID paramid = addFunc(bondfunc);
        relateParameter(userid, paramid);
    }
}

/** Return the bond function associated with the user identification ID 'userid',
    or a zero function if there is no such function. foundbond is set to whether 
    a function was found. */
Expression BondDB::getBond(const QString &userid, bool *foundbond)
{
    ParamID paramid = getParameter(userid);
    
    if (foundbond)
        *foundbond = (paramid != 0);
        
    return retrieveFunc(paramid);
}
    
/** Relate the bond function associated with the user identification ID 'userid'
    with the relationship matching a pair of atoms 'matchbond' */
void BondDB::relateBond(const AssertMatch<2> &matchbond, const QString &userid)
{
    relateFunc(matchbond, userid);
}

/** Relate the bond function 'bondfunc' with the relationship matching a pair of atoms.
    Note that the bondfunc must be a function of "r" (get the symbol via
    Symbol("r") or BondDB::r()) or else the function 
    will be treated as a constant */
void BondDB::relateBond(const AssertMatch<2> &matchbond, const Expression &bondfunc)
{
    if (not bondfunc.isFunction(_r))
    {
        Expression const_val = bondfunc.evaluate(Values());
        relateFunc(matchbond, const_val);
    }
    else
        relateFunc(matchbond, bondfunc);
}
    
/** Return the bond that matches the atom-pair matching relationship 'relateid', 
    or a zero function if there is no match. foundbond is set to whether or 
    not a function was found */
Expression BondDB::getBond(RelateID relateid, bool *foundbond)
{
    return getFunc(relateid, foundbond);
}

/** Return the bond that best matches the atom-pair matching relationships
    in 'relateids', or a zero function if there is no match. foundbond is set
    to whether or not a function was found. */
Expression BondDB::getBond(const RelateIDMap &relateids, bool *foundbond)
{
    return getFunc(relateids, foundbond);
}

/** Relate the bond parameter identified by the user ID string 'userid' to the matching criteria
    identified by the relationship ID 'relateid' */
void BondDB::relateBond(RelateID relateid, const QString &userid)
{
    relateParameter(relateid, userid);
}

/** Relate the bond function 'bondfunc' to the matching criteria identified by the relationship
    ID 'relateid' */
void BondDB::relateBond(RelateID relateid, const Expression &bondfunc)
{
    //add the function
    ParamID paramid = addFunc(bondfunc);
    //associate this relationship with the parameter
    relateParameter(relateid, paramid);
}
    
/** Assign the parameter for the bond 'bond' using the relationship IDs in 
    'relateids', and place the parameter into the table 'param_table'. Return
    whether or not a parameter was found and assigned. */
bool BondDB::assignParameter(const Bond &bond, const RelateIDMap &relateids,
                             ParameterTable &param_table)
{
    BOOST_ASSERT(param_table.isA<BondTable>());

/*    bool found;
    Expression bondfunc = this->getFunc(relateids, &found);
    
    if (found)
        //add the expression to the parameter table
        param_table.asA<BondTable>().setParameter(bond, bondfunc);
        
    return found;*/

    return false;
}

