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

#include "indexinfo.h"

#include "SireError/errors.h"
#include "SireMol/errors.h"

using namespace SireMol;

//////////////////
////////////////// Instantiation of IndexInfo for GroupIndexID
//////////////////

/** Throw an exception stating that 'groupid' is not available 

    \throw SireMol::missing_group
*/
void IndexInfo<GroupIndexID>::invalidGroup(GroupID groupid)
{
    throw SireMol::missing_group( QObject::tr(
                    "There is no group with ID == %1")
                        .arg(groupid.toString()), CODELOC );
}

/** Throw an exception stating that 'id' is not available 

    \throw SireMol::missing_group
*/
void IndexInfo<GroupIndexID>::invalidGroup(const GroupIndexID &id)
{
    throw SireMol::missing_group( QObject::tr(
                    "There is no group with ID == %1 for GroupIndexID(%1,%2)")
                          .arg(id.groupID().toString(),id.index().toString()), CODELOC );
}

/** Throw an exception stating that the index is invalid, as there
    are only 'count' items. 
    
    \throw SireError::invalid_index
*/
void IndexInfo<GroupIndexID>::invalidIndex(Index index, int count)
{
    throw SireError::invalid_index( QObject::tr(
                    "The index \"%1\" is invalid as the number "
                    "of items in the group is %2")
                        .arg(index.toString()).arg(count), CODELOC );
}

/** Throw an exception stating that the index is invalid, as there
    are only 'count' items.
    
    \throw SireError::invalid_index
*/
void IndexInfo<GroupIndexID>::invalidIndex(const GroupIndexID &id, int count)
{
    throw SireError::invalid_index( QObject::tr(
                    "The index in GroupIndexID(%1,%2) is invalid as the "
                    "number of items in the group is only %3")
                        .arg(id.groupID().toString(),id.index().toString()).arg(count), CODELOC );
}

//////////////////
////////////////// Instantiation of IndexInfo for CGAtomID
//////////////////

/** Throw an exception stating that 'cgid' is not available 

    \throw SireMol::missing_cutgroup
*/
void IndexInfo<CGAtomID>::invalidGroup(CutGroupID cgid)
{
    throw SireMol::missing_cutgroup( QObject::tr(
                    "There is no CutGroup with ID == %1 available")
                        .arg(cgid.toString()), CODELOC );
}

/** Throw an exception stating that 'id' is not available 

    \throw SireMol::missing_cutgroup
*/
void IndexInfo<CGAtomID>::invalidGroup(const CGAtomID &cgid)
{
    throw SireMol::missing_cutgroup( QObject::tr(
                    "There is no CutGroup with ID == %1 for CGAtomID(%1,%2)")
                          .arg(cgid.cutGroupID().toString(),
                               cgid.atomID().toString()), CODELOC );
}

/** Throw an exception stating that the index is invalid, as there
    are only 'count' items. 
    
    \throw SireError::invalid_index
*/
void IndexInfo<CGAtomID>::invalidIndex(Index index, int count)
{
    throw SireError::invalid_index( QObject::tr(
                    "The index \"%1\" is invalid as the number "
                    "of atoms in the group is %2")
                        .arg(index.toString()).arg(count), CODELOC );
}

/** Throw an exception stating that the index is invalid, as there
    are only 'count' items.
    
    \throw SireError::invalid_index
*/
void IndexInfo<CGAtomID>::invalidIndex(const CGAtomID &cgid, int count)
{
    throw SireError::invalid_index( QObject::tr(
                    "The index in CGAtomID(%1,%2) is invalid as the "
                    "number of atoms in the group is only %3")
                        .arg(cgid.cutGroupID().toString(),
                             cgid.atomID().toString()).arg(count), CODELOC );
}
