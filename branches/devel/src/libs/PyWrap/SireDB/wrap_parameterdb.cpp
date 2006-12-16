
#include <Python.h>
#include <boost/python.hpp>

#include "SirePy/str.hpp"
#include "SireQt/qdatastream.hpp"

#include "SireDB/parameterdb.h"
#include "SireDB/tablebase.h"
#include "SireDB/parametertable.h"
#include "SireDB/assign_parameters.h"

#include "SireMol/molecule.h"

using namespace boost::python;
using namespace SirePy;
using namespace SireQt;
using namespace SireMol;

namespace SireDB
{

ParameterTable wrap_assign3(ParameterDB &db, const Molecule &molecule,
                            const assign_parameters &assign_instruct,
                            const MatchMRData &matchmr)
{
    return db.assign(molecule, assign_instruct, matchmr);
}

ParameterTable wrap_assign2(ParameterDB &db, const Molecule &molecule,
                            const assign_parameters &assign_instruct)
{
    return db.assign(molecule, assign_instruct);
}

void  
SIREDB_EXPORT
export_ParameterDB()
{
    
    class_<ParameterDB>( "ParameterDB", init<>() )
    
        .def( init<const ParameterDB&>() )
    
        .def( "__str__",  &__str__<ParameterDB> )
        
        .def( "__rlshift__", &__rlshift__QDataStream<ParameterDB>,
                  return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rrshift__", &__rrshift__QDataStream<ParameterDB>,
                  return_internal_reference< 1, with_custodian_and_ward<1,2> >() )

        .def( "addComponent", (DBBase& (ParameterDB::*)(const char*))
                    &ParameterDB::addComponent,
                        return_internal_reference<1, with_custodian_and_ward<1,2> >() )
    
        .def( "addLog", &ParameterDB::addLog )
        
        .def( "asA", (DBBase& (ParameterDB::*)(const QString&))
                    &ParameterDB::asA,
                        return_internal_reference<1, with_custodian_and_ward<1,2> >() )

        .def( "assign", &wrap_assign3 )
        .def( "assign", &wrap_assign2 )

        .def( "clear", &ParameterDB::clear )
                  
        .def( "clearLog", &ParameterDB::clearLog )
        
        .def( "dump", (void (ParameterDB::*)(const QString&))
                    &ParameterDB::dump )
        .def( "dump", (void (ParameterDB::*)(QTextStream&))
                    &ParameterDB::dump )
    
        .def( "dumpToString", &ParameterDB::dumpToString )

        .def( "isA", (bool (ParameterDB::*)(const QString&) const)
                    &ParameterDB::isA )
        
        .def( "getLog", &ParameterDB::getLog )
    
        .def( "load", (void (ParameterDB::*)(QTextStream&))
                    &ParameterDB::load )

        .def( "load", (void (ParameterDB::*)(const QString&))
                    &ParameterDB::load )
        .def( "loadFromString", &ParameterDB::loadFromString )

        .def( "nParameters", &ParameterDB::nParameters )
        .def( "nRelationships", &ParameterDB::nRelationships )
        
        .def( "toString", &ParameterDB::toString )
        
        .def( "typeName", &ParameterDB::typeName ).staticmethod("typeName")

        .def( "types", &ParameterDB::types )
    
        .def( "what", &ParameterDB::what )
    ;

}

}
