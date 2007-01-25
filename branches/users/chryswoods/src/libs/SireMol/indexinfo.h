/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
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

#ifndef SIREMOL_INDEXINFO_H
#define SIREMOL_INDEXINFO_H

#include "groupindexid.h"
#include "cgatomid.h"
#include "resnumatomid.h"
#include "resnumindexid.h"

SIRE_BEGIN_HEADER

namespace SireError
{
class invalid_index;
class invalid_key;
}

namespace SireMol
{

class missing_group;
class missing_residue;
class missing_cutgroup;

template<class T>
class IndexInfo;

/** The IndexInfo object used to allow a GroupedVector to be 
    constructed using GroupIndexID as the index */
template<>
class SIREMOL_EXPORT IndexInfo<GroupIndexID>
{
public:
    typedef GroupID groupid_type;
    typedef GroupIndexID index_type;

    typedef SireMol::missing_group invalid_group_error;
    typedef SireError::invalid_index invalid_index_error;
    
    static GroupID groupID(const GroupIndexID &id)
    {
        return id.groupID();
    }
    
    static Index index(const GroupIndexID &id)
    {
        return id.index();
    }
    
    static void invalidGroup(GroupID groupid);
    static void invalidGroup(const GroupIndexID &id);
    static void invalidIndex(Index index, int count);
    static void invalidIndex(const GroupIndexID &id, int count);
};

/** The IndexInfo object used to allow a GroupedVector to be 
    constructed using CGAtomID as the index */
template<>
class SIREMOL_EXPORT IndexInfo<CGAtomID>
{
public:
    typedef CutGroupID groupid_type;
    
    typedef SireMol::missing_cutgroup invalid_group_error;
    typedef SireError::invalid_index invalid_index_error;
    
    static CutGroupID groupID(const CGAtomID &id)
    {
        return id.cutGroupID();
    }
    
    static AtomID index(const CGAtomID &id)
    {
        return id.atomID();
    }

    static void invalidGroup(CutGroupID cgid);
    static void invalidGroup(const CGAtomID &id);
    static void invalidIndex(Index index, int count);
    static void invalidIndex(const CGAtomID &id, int count);
};

/** The IndexInfo object used to allow a GroupedVector to
    be constructed using ResIndexID as the index */
template<>
class SIREMOL_EXPORT IndexInfo<ResNumIndexID>
{
public:
    typedef ResNum groupid_type;
    
    typedef SireMol::missing_residue invalid_group_error;
    typedef SireError::invalid_index invalid_index_error;
    
    static ResNum groupID(const ResNumIndexID &id)
    {
        return id.resNum();
    }
    
    static Index index(const ResNumIndexID &id)
    {
        return id.index();
    }

    static void invalidGroup(ResNum resnum);
    static void invalidGroup(const ResNumIndexID &id);
    static void invalidIndex(Index index, int count);
    static void invalidIndex(const ResNumIndexID &id, int count);
};

/** The IndexInfo object used to allow a GroupedVector to
    be constructed using ResAtomID as the index */
template<>
class SIREMOL_EXPORT IndexInfo<ResNumAtomID>
{
public:
    typedef ResNum groupid_type;
    
    typedef SireMol::missing_residue invalid_group_error;
    typedef SireError::invalid_index invalid_index_error;
    
    static ResNum groupID(const ResNumAtomID &id)
    {
        return id.resNum();
    }
    
    static AtomID index(const ResNumAtomID &id)
    {
        return id.atomID();
    }

    static void invalidGroup(ResNum resnum);
    static void invalidGroup(const ResNumAtomID &id);
    static void invalidIndex(Index index, int count);
    static void invalidIndex(const ResNumAtomID &id, int count);
};

}

SIRE_END_HEADER

#endif
