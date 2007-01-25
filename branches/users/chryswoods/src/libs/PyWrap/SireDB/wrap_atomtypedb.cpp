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

#include <Python.h>
#include <boost/python.hpp>

#include "SireDB/atomtypedb.h"

using namespace boost::python;

namespace SireDB
{
    
AtomType wrap_getAtomType_QString2(AtomTypeDB &db, const QString &userid, bool &foundtype)
{
    return db.getAtomType(userid, &foundtype);
}

AtomType wrap_getAtomType_QString1(AtomTypeDB &db, const QString &userid)
{
    return db.getAtomType(userid);
}

AtomType wrap_getAtomType_RelateID2(AtomTypeDB &db, RelateID relateid, bool &foundtype)
{
    return db.getAtomType(relateid, &foundtype);
}

AtomType wrap_getAtomType_RelateID1(AtomTypeDB &db, RelateID relateid)
{
    return db.getAtomType(relateid);
}

AtomType wrap_getAtomType_RelateIDMap2(AtomTypeDB &db, const RelateIDMap &relateids, 
                                       bool &foundtype)
{
    return db.getAtomType(relateids, &foundtype);
}

AtomType wrap_getAtomType_RelateIDMap1(AtomTypeDB &db, const RelateIDMap &relateids)
{
    return db.getAtomType(relateids);
}

void  
SIREDB_EXPORT
export_AtomTypeDB()
{
    class_<AtomTypeDB, bases<AtomDB> >( "AtomTypeDB", init<>() )
    
        .def( "addAtomType", (void (AtomTypeDB::*)(const QString&, const AtomType&))
                        &AtomTypeDB::addAtomType )
    
        .def( "getAtomType", &wrap_getAtomType_QString1 )
        .def( "getAtomType", &wrap_getAtomType_QString2 )
        
        .def( "getAtomType", &wrap_getAtomType_RelateID1 )
        .def( "getAtomType", &wrap_getAtomType_RelateID2 )
        
        .def( "getAtomType", &wrap_getAtomType_RelateIDMap1 )
        .def( "getAtomType", &wrap_getAtomType_RelateIDMap2 )
        
        .def( "relateAtomType", (void (AtomTypeDB::*)(const AssertMatch<1>&, const QString&))
                        &AtomTypeDB::relateAtomType )
        .def( "relateAtomType", (void (AtomTypeDB::*)(const AssertMatch<1>&matchatom, 
                                                      const AtomType&))
                        &AtomTypeDB::relateAtomType )
        .def( "relateAtomType", (void (AtomTypeDB::*)(RelateID, const QString&))
                        &AtomTypeDB::relateAtomType )
        .def( "relateAtomType", (void (AtomTypeDB::*)(RelateID, const AtomType&))
                        &AtomTypeDB::relateAtomType )
    
        .def( "typeName", &AtomTypeDB::typeName ).staticmethod("typeName")
    ;
}


}
