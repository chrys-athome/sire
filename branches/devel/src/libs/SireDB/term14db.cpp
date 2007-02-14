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

#include "term14db.h"

#include "SireCAS/expression.h"
#include "SireStream/version_error.h"

using namespace SireDB;
using namespace SireStream;

/** Create a Term14DB. */
Term14DB::Term14DB() : FuncDB()
{}

/** Copy constructor */
Term14DB::Term14DB(const Term14DB &other) : FuncDB(other)
{}

/** Destructor */
Term14DB::~Term14DB()
{}
    
/** Dump the version number of this database */
void Term14DB::prepareToDump()
{
    FuncDB::prepareToDump();
    
    this->saveParameter<Term14DB>( "version", 0 );
}

/** Check the version number */
void Term14DB::postLoad()
{
    FuncDB::postLoad();

    int v = this->loadParameter<Term14DB>( "version" ).toInt();
                            
    if (v != 0)
        throw version_error( v, "0", "SireDB::Term14DB", CODELOC );
}

/** Relate the match for a quad of atoms 'matchquad' to the expression identified
    via the user identification string 'userid' */
void Term14DB::relateFunc(const AssertMatch<4> &matchquad, const QString &userid,
                          bool mirrored)
{
    RelateID relateid = matchquad.addTo(parent());
    relateParameter(relateid, userid);
    
    if (mirrored)
    {
        RelateID mirrorid = matchquad.addMirroredTo(parent());
        
        if (relateid != mirrorid)
            relateParameter(mirrorid, userid);
    }
}

/** Relate the match for a quad of atoms 'matchquad' to the expression 'func' */
void Term14DB::relateFunc(const AssertMatch<4> &matchquad, const Expression &func,
                          bool mirrored)
{
    RelateID relateid = matchquad.addTo(parent());
    
    //add the function
    ParamID paramid = addFunc(func);
    
    relateParameter(relateid, paramid);
    
    if (mirrored)
    {
        RelateID mirrorid = matchquad.addMirroredTo(parent());
        
        if (relateid != mirrorid)
            relateParameter(mirrorid, paramid);
    }
}
