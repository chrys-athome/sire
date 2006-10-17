
#include "SireCAS/qhash_sirecas.h"
#include "SireMol/qhash_siremol.h"
#include "SireMM/qhash_siremm.h"

#include <QDebug>

#include "dihedraldb.h"

#include "SireCAS/expression.h"
#include "SireCAS/values.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireMM;
using namespace SireDB;
using namespace SireCAS;

static const RegisterMetaType<DihedralDB> r_dihedraldb("SireMM::DihedralDB", NO_STREAM);

/** Constructor */
DihedralDB::DihedralDB() : Term14DB(), InternalDB<MolDihedralInfo>(),
                           _phi("phi"), _r("r")
{}

/** Copy constructor */
DihedralDB::DihedralDB(const DihedralDB &other)
           : Term14DB(other), InternalDB<MolDihedralInfo>(other),
             _phi(other._phi), _r(other._r)
{}

/** Destructor */
DihedralDB::~DihedralDB()
{}

/** Dump the version number of this database */
void DihedralDB::prepareToDump()
{
    Term14DB::prepareToDump();

    this->saveParameter<DihedralDB>( "version" , 0 );
}

/** Check the version number */
void DihedralDB::postLoad()
{
    Term14DB::postLoad();

    int v = this->loadParameter<DihedralDB>( "version" ).toInt();

    if (v != 0)
        throw version_error( v, "0", "SireMM::DihedralDB", CODELOC );
}

/** Add the dihedral function 'dihfunc' and associate it with the user identification
    string 'userid'. Note that 'dihfunc' must be a function of the variable
    representing the dihedral size ("phi", obtainable via Symbol("phi") or
    DihedralDB::phi()), or the distance between the 1-4 atoms, ("r", via
    Symbol("r") or DihedralDB::r()) or else it will be treated as a constant. */
void DihedralDB::addDihedral(const QString &userid, const Expression &dihfunc)
{
    if ( not (dihfunc.isFunction(_phi) or dihfunc.isFunction(_r)) )
    {
        //evaluate this constant...
        Expression const_val = dihfunc.evaluate(Values());

        ParamID paramid = addFunc(const_val);
        relateParameter(userid, paramid);
    }
    else
    {
        ParamID paramid = addFunc(dihfunc);
        relateParameter(userid, paramid);
    }
}

/** Return the dihedral function associated with the user identification ID 'userid',
    or a zero function if there is no such function. founddih is set to whether
    a function was found. */
Expression DihedralDB::getDihedral(const QString &userid, bool *founddih)
{
    ParamID paramid = getParameter(userid);

    if (founddih)
        *founddih = (paramid != 0);

    return retrieveFunc(paramid);
}

/** Add the dihedral function 'expression' and associate it with the generic function
    'function'. This will allow other dihedral expressions to contain this function,
    which will be expanded to 'expression' whenever the dihedral is assigned.

    Note that 'expression' must be a function of 'phi' or 'r' or else it will
    be treated as a constant.
*/
void DihedralDB::addDihedral(const Function &function, const Expression &expression)
{
    if ( not (expression.isFunction(_phi) or expression.isFunction(_r)) )
    {
        Expression const_val = expression.evaluate(Values());
        addFunc(function, const_val);
    }
    else
        addFunc(function, expression);
}

/** Return the expression associated with the generic function 'function'. This
    returns an empty expression if the function is not found. 'founddih' is set
    to true if there function is found, false if it isn't */
Expression DihedralDB::getDihedral(const Function &function, bool *founddih)
{
    return getFunc(function, founddih);
}

/** Relate the dihedral function associated with the user identification ID 'userid'
    with the relationship matching a triple of atoms 'matchdih' */
void DihedralDB::relateDihedral(const AssertMatch<4> &matchdih, const QString &userid)
{
    relateFunc(matchdih, userid);
}

/** Relate the dihedral function 'dihfunc' with the relationship matching a quad of atoms.
    Note that the dihfunc must be a function of "phi" or "r" or else the function
    will be treated as a constant */
void DihedralDB::relateDihedral(const AssertMatch<4> &matchdih, const Expression &dihfunc)
{
    if ( not (dihfunc.isFunction(_phi) or dihfunc.isFunction(_r)) )
    {
        Expression const_val = dihfunc.evaluate(Values());
        relateFunc(matchdih, const_val);
    }
    else
        relateFunc(matchdih, dihfunc);
}

/** Return the dihedral that matches the atom-quad matching relationship 'relateid',
    or a zero function if there is no match. founddih is set to whether or
    not a function was found */
Expression DihedralDB::getDihedral(RelateID relateid, bool *founddih)
{
    return getFunc(relateid, founddih);
}

/** Return the dihedral that best matches the atom-quad matching relationships
    in 'relateids', or a zero function if there is no match. founddih is set
    to whether or not a function was found. */
Expression DihedralDB::getDihedral(const RelateIDMap &relateids, bool *founddih)
{
    return getFunc(relateids, founddih);
}

/** Relate the dihedral parameter identified by the user ID string 'userid' to the matching criteria
    identified by the relationship ID 'relateid' */
void DihedralDB::relateDihedral(RelateID relateid, const QString &userid)
{
    relateParameter(relateid, userid);
}

/** Relate the dihedral function 'dihedralfunc' to the matching criteria identified by the relationship
    ID 'relateid' */
void DihedralDB::relateDihedral(RelateID relateid, const Expression &dihedralfunc)
{
    //add the function
    ParamID paramid = addFunc(dihedralfunc);
    //associate this relationship with the parameter
    relateParameter(relateid, paramid);
}

/** Create the DihedralTable in the ParameterTable 'param_table' that will hold the
    parameters in this database, using the internals listed in 'internals'. Note
    that this will remove any existing DihedralTable that exists in 'param_table'.
    I would thus only recommend using this function if that is indeed what you
    intend, or if hasTable(param_table) returns false.
*/
void DihedralDB::createTable(ParameterTable &param_table,
                             const MolDihedralInfo &dihedralinfo) const
{
    param_table.setTable(DihedralTable(dihedralinfo));
}

/** Assign the parameter for the dihedral 'dihedral' using the relationship IDs in
    'relateids', and place the parameter into the table 'param_table'. Return
    whether or not a parameter was found and assigned. */
bool DihedralDB::assignParameter(const Dihedral &dihedral, const RelateIDMap &relateids,
                                 ParameterTable &param_table)
{
/*    BOOST_ASSERT(param_table.isA<DihedralTable>());

    bool found;
    Expression dihedralfunc = this->getFunc(relateids, &found);

    if (found)
        //add the expression to the parameter table
        param_table.asA<DihedralTable>().setParameter(dihedral, dihedralfunc);

    return found;*/

    return false;
}
