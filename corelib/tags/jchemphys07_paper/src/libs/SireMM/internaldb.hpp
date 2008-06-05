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

#ifndef SIREMM_INTERNALDB_HPP
#define SIREMM_INTERNALDB_HPP

#include "SireDB/relateidmap.h"
#include "SireDB/parametertable.h"

#include "internaltablebase.hpp"

SIRE_BEGIN_HEADER


namespace SireMM
{

using SireDB::ParameterTable;
using SireDB::RelateIDMap;

/** This is a small interface class that is used to provide a common
    interface to all DB components that hold internal parameters. 
    
    This class contains no data, just pure virtual functions that
    must be overridden somewhere in the inheritance hierarchy.
    
    @author Christopher Woods
*/
template<class Internals>
class InternalDB
{
public:
    typedef Internals info_type;
    typedef typename Internals::internal_type internal_type;
    typedef internal_type Internal;

    InternalDB()
    {}
    
    virtual ~InternalDB()
    {}

    /** Return whether or not the ParameterTable 'param_table' contains the
        right table to hold the parameters for this database */
//    virtual bool hasTable(const ParameterTable &param_table) const=0;
    
    /** Return the table in the ParameterTable 'param_table' that holds the 
        parameters from this database. This will have undefined results unless
        'hasTable(param_table)' returns true. */
//    virtual InternalTableBase<Internals>& getTable(ParameterTable &param_table) const=0;
    
    /** Create the table in the ParameterTable 'param_table' that will hold the 
        parameters in this database, using the internals listed in 'internals'. Note
        that this will remove any existing parameter table that exists in 'param_table'
        that could hold parameters for this database. I would thus only recommend
        using this function if that is indeed what you intend, or if hasTable(param_table)
        returns false. */
//    virtual InternalTableBase<Internals>& createTable(ParameterTable &param_table, 
//                                                      const Internals &internals) const=0;
    
    /** Assign the parameter for the internal 'internal' into the ParameterTable 
        'param_table' using the relationship IDs in 'relateids'. Note that exceptions
        will be thrown if there is no table for this parameter or if the table doesn't
        contains a space for this internal */
    virtual bool assignParameter(const Internal &internal, 
                                 const RelateIDMap &relateids,
                                 ParameterTable &param_table)=0;
};

}

SIRE_END_HEADER

#endif
