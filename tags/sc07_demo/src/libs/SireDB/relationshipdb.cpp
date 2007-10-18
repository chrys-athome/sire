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
