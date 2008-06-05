/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2008  Christopher Woods
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

#ifndef SIREDB_DISKCACHE_H
#define SIREDB_DISKCACHE_H

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireDB
{
class DiskCache;
}

namespace SireDB
{

/** This class provides a fast cache of items, which can exist in memory
    or on disk. This allows Sire to create and manage data that won't
    fit into memory
    
    This class is *NOT* thread-safe
    
    @author Christopher Woods
*/
class SIREDB_EXPORT DiskCache
{
public:
    DiskCache();
    ~DiskCache();
    
    //CacheToken cache(const QVariant &object);
    //QVariant retrieve(const CacheToken &token);
    
private:
    /** Handle to the SQLite database behind this cache... */
};

}

SIRE_END_HEADER

#endif
