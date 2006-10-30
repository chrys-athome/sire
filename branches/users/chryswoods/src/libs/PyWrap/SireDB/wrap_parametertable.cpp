
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

void export_ParameterTable()
{

    class_<ParameterTable>( "ParameterTable", init<>() )
        .def( init<const ParameterTable&>() )

        .def( "addTable", (void (ParameterTable::*)(const TableBase&))
                          &ParameterTable::addTable )

        .def( "types", &ParameterTable::types )
    ;
}

}
