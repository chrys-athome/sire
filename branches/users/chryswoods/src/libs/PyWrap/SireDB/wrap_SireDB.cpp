
#include <Python.h>
#include <boost/python.hpp>

#include "sireglobal.h"

#include "wrap_SireDB.h"

namespace SireDB
{

void export_AtomType();
void export_Match();
void export_MatchHelpers();
void export_DB();
void export_ParameterDB();
void export_AtomTypeDB();
void export_RelateDB();
void export_ParameterTable();
void export_AtomTable();
void export_AtomTypeTable();

void export_using_database();
void export_assign_atoms();
void export_assign_parameters();

void SIREDB_EXPORT export_SireDB()
{
    export_AtomType();
    export_Match();
    export_MatchHelpers();
    export_DB();
    export_ParameterDB();
    export_AtomTypeDB();
    export_RelateDB();
    export_ParameterTable();
    export_AtomTable();
    export_AtomTypeTable();

    export_using_database();
    export_assign_atoms();
    export_assign_parameters();
}

}
