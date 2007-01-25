/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2007  Christopher Woods
  *
  *  This program is free software; you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation; either version 2 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program; if not, write to the Free Software
  *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  *  For full details of the license please see the COPYING file
  *  that should have come with this distribution.
  *
  *  You can contact the authors via the developer's mailing list
  *  at http://siremol.org
  *
\*********************************************/

#include "SireCAS/qhash_sirecas.h"

#include "ureybradleydb.h"

#include "SireCAS/expression.h"
#include "SireCAS/values.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireMM;
using namespace SireDB;
using namespace SireCAS;

static const RegisterMetaType<UreyBradleyDB> r_ubydb(NO_STREAM);

/** Constructor */
UreyBradleyDB::UreyBradleyDB() : Term13DB(), _r("r")
{}

/** Copy constructor */
UreyBradleyDB::UreyBradleyDB(const UreyBradleyDB &other)
              : Term13DB(other), _r(other._r)
{}

/** Destructor */
UreyBradleyDB::~UreyBradleyDB()
{}

/** Dump the version number of this database */
void UreyBradleyDB::prepareToDump()
{
    Term13DB::prepareToDump();

    this->saveParameter<UreyBradleyDB>( "version" , 0 );
}

/** Check the version number */
void UreyBradleyDB::postLoad()
{
    Term13DB::postLoad();

    int v = this->loadParameter<UreyBradleyDB>( "version" ).toInt();

    if (v != 0)
        throw version_error( v, "0", "SireMM::UreyBradleyDB", CODELOC );
}

/** Add the Urey-Bradley function 'ubfunc' and associate it with the user identification
    string 'userid'. Note that 'ubfunc' must be a function of the variable
    representing the distance between the 1-3 atoms, ("r", via
    Symbol("r") or UreyBradleyDB::r()) or else it will be treated as a constant. */
void UreyBradleyDB::addUreyBradley(const QString &userid, const Expression &ubfunc)
{
    if ( not ubfunc.isFunction(_r) )
    {
        //evaluate this constant...
        Expression const_val = ubfunc.evaluate(Values());

        ParamID paramid = addFunc(const_val);
        relateParameter(userid, paramid);
    }
    else
    {
        ParamID paramid = addFunc(ubfunc);
        relateParameter(userid, paramid);
    }
}

/** Return the Urey-Bradley function associated with the user identification ID 'userid',
    or a zero function if there is no such function. foundub is set to whether
    a function was found. */
Expression UreyBradleyDB::getUreyBradley(const QString &userid, bool *foundub)
{
    ParamID paramid = getParameter(userid);

    if (foundub)
        *foundub = (paramid != 0);

    return retrieveFunc(paramid);
}

/** Relate the Urey Bradley function associated with the user identification ID 'userid'
    with the relationship matching a triple of atoms 'matchtriple' */
void UreyBradleyDB::relateUreyBradley(const AssertMatch<3> &matchtriple, const QString &userid)
{
    relateFunc(matchtriple, userid);
}

/** Relate the Urey Bradley function 'ubfunc' with the relationship matching a triple of atoms.
    Note that the ubfunc must be a function of "r" or else the function will be treated as
    a constant */
void UreyBradleyDB::relateUreyBradley(const AssertMatch<3> &matchtriple, const Expression &ubfunc)
{
    if ( not ubfunc.isFunction(_r) )
    {
        Expression const_val = ubfunc.evaluate(Values());
        relateFunc(matchtriple, const_val);
    }
    else
        relateFunc(matchtriple, ubfunc);
}

/** Return the Urey Bradley function that matches the atom-triple matching relationship 'relateid',
    or a zero function if there is no match. foundub is set to whether or
    not a function was found */
Expression UreyBradleyDB::getUreyBradley(RelateID relateid, bool *foundub)
{
    return getFunc(relateid, foundub);
}

/** Return the Urey Bradley function that best matches the atom-triple matching relationships
    in 'relateids', or a zero function if there is no match. foundub is set
    to whether or not a function was found. */
Expression UreyBradleyDB::getUreyBradley(const RelateIDMap &relateids, bool *foundub)
{
    return getFunc(relateids, foundub);
}

/** Relate the Urey Bradley parameter identified by the user ID string 'userid' to the
    matching criteria identified by the relationship ID 'relateid' */
void UreyBradleyDB::relateUreyBradley(RelateID relateid, const QString &userid)
{
    relateParameter(relateid, userid);
}

/** Relate the Urey Bradley function 'ubfunc' to the matching criteria identified by the relationship
    ID 'relateid' */
void UreyBradleyDB::relateUreyBradley(RelateID relateid, const Expression &ubfunc)
{
    //add the function
    ParamID paramid = addFunc(ubfunc);
    //associate this relationship with the parameter
    relateParameter(relateid, paramid);
}
