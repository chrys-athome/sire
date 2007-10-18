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
