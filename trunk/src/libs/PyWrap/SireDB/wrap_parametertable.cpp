
/**
  * This file contains the boost::python wrapping of ParameterTable
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>

#include "SireMol/qhash_siremol.h"

#include "SireDB/parametertable.h"
#include "SireDB/tablebase.h"
#include "SireDB/atomtypetable.h"

#include "SireDB/parameterdb.h"
#include "SireDB/assign_parameters.h"

#include "SireMol/molecule.h"

#include "SireQt/qdatastream.hpp"

using namespace boost::python;
using namespace SireQt;

namespace SireDB
{

void wrap_assign3(ParameterTable &table, ParameterDB &db, const assign_parameters &assigners,
                  const MatchMRData &matchmr)
{
    table.assign(db, assigners, matchmr);
}

void wrap_assign2(ParameterTable &table, ParameterDB &db, const assign_parameters &assigners)
{
    table.assign(db, assigners);
}

void export_ParameterTable()
{
    
    class_<ParameterTable>( "ParameterTable", init<>() )
        .def( init<const Molecule&>() )
        .def( init<const ParameterTable&>() )
        
        .def( "__rlshift__", &__rlshift__QDataStream<ParameterTable>,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rrshift__", &__rrshift__QDataStream<ParameterTable>,
                    return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        
        .def( "addTable", (TableBase& (ParameterTable::*)(const QString&))
                          &ParameterTable::addTable,
                              return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "addTable", (TableBase& (ParameterTable::*)(const TableBase&))
                          &ParameterTable::addTable,
                              return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        
        .def( "asA", (const TableBase& (ParameterTable::*)(const QString&) const)
                          &ParameterTable::asA,
                              return_value_policy<copy_const_reference>() )
        .def( "asA", (TableBase& (ParameterTable::*)(const QString&))
                          &ParameterTable::asA,
                              return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
  
        .def( "assign", &wrap_assign3 )
        .def( "assign", &wrap_assign2 )
        
        .def( "isA", (bool (ParameterTable::*)(const QString&) const)
                          &ParameterTable::isA )
        
        .def( "isEmpty", &ParameterTable::isEmpty )
    
        .def( "molecule", &ParameterTable::molecule,
                              return_value_policy<copy_const_reference>() )

        .def( "removeTable", (void (ParameterTable::*)(const QString&))
                          &ParameterTable::removeTable )
    
        .def( "setMolecule", &ParameterTable::setMolecule )
        
        .def( "setTable", (TableBase& (ParameterTable::*)(const QString&))
                          &ParameterTable::setTable,
                              return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "setTable", (TableBase& (ParameterTable::*)(const TableBase&))
                          &ParameterTable::setTable,
                              return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
    
        .def( "types", &ParameterTable::types )
    ;
}

}
