
#include <Python.h>

#include <boost/python.hpp>

#include "SireDB/assign_atoms.h"
#include "SireDB/parametertable.h"
#include "SireDB/parameterdb.h"
#include "SireDB/using_database.h"

#include "SireMol/molecule.h"

using namespace boost::python;

using namespace SireMol;

namespace SireDB
{

void wrap_assignParameters3( const AssignBase &assignbase,
                             const Molecule &molecule,
                             ParameterTable &param_table,
                             ParameterDB &database,
                             const MatchMRData &matchmr )
{
    return assignbase.assignParameters(molecule, param_table, database, matchmr);
}

void wrap_assignParameters2( const AssignBase &assignbase,
                             const Molecule &molecule,
                             ParameterTable &param_table,
                             ParameterDB &database )
{
    return assignbase.assignParameters(molecule, param_table, database);
}

void export_assign_atoms()
{

    class_<AssignBase, bases<AssignInstruction>, boost::noncopyable>
                                      ("AssignBase", no_init)

        .def( "addDataBase", (void (AssignBase::*)(const using_database&))
                        &AssignBase::addDataBase )

        .def( "parameterDataBases", &AssignBase::parameterDataBases )
        .def( "relationshipDataBases", &AssignBase::relationshipDataBases )

        .def( "assignParameters", &wrap_assignParameters3 )
        .def( "assignParameters", &wrap_assignParameters2 )

    ;

    class_< assign_atoms, bases<AssignBase> >( "assign_atoms", init<>() )

        .def( init< optional<const using_database&, const using_database&,
                             const using_database&, const using_database&,
                             const using_database&, const using_database&,
                             const using_database&, const using_database&,
                             const using_database&, const using_database&> >() )

        .def( init< const QSet<AtomIndex>&,
                    optional<const using_database&, const using_database&,
                             const using_database&, const using_database&,
                             const using_database&, const using_database&,
                             const using_database&, const using_database&,
                             const using_database&, const using_database&> >() )

        .def( "typeName", &assign_atoms::typeName )
    ;

}

}
