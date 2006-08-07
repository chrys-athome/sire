
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

void export_AtomTypeDB()
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
