
#include "boost/python.hpp"

#include "SireCAS/qhash_sirecas.h"

#include "SireMM/cljff.h"

#include "SireMol/molecule.h"
#include "SireMol/residue.h"

#include "SireDB/parametertable.h"

namespace bp = boost::python;

using namespace SireFF;

namespace SireMM
{

void
SIREMM_EXPORT
export_CLJFF()
{
    bp::class_< CLJFF, bp::bases<SireFF::FFBase>, boost::noncopyable >( "CLJFF", bp::no_init )

        .def(
            "space"
            , &::SireMM::CLJFF::space
            , bp::return_value_policy< bp::copy_const_reference, bp::default_call_policies >() )
        .def(
            "switchingFunction"
            , &::SireMM::CLJFF::switchingFunction
            , bp::return_value_policy< bp::copy_const_reference, bp::default_call_policies >() )
    ;

    bp::class_<CLJFF::Components, bp::bases<FFBase::Components> >("CLJFF_Components")

        .def( bp::init<>() )

        .def( "coulomb", &CLJFF::Components::coulomb,
                bp::return_value_policy< bp::copy_const_reference, bp::default_call_policies >() )
        .def( "lj", &CLJFF::Components::lj,
                bp::return_value_policy< bp::copy_const_reference, bp::default_call_policies >() )

        .def( "describe_coulomb", &CLJFF::Components::describe_coulomb,
                bp::default_call_policies() ).staticmethod("describe_coulomb")

        .def( "describe_lj", &CLJFF::Components::describe_lj,
                bp::default_call_policies() ).staticmethod("describe_lj")
    ;

    bp::class_<CLJFF::Parameters, bp::bases<FFBase::Parameters> >("CLJFF_Parameters")

        .def( bp::init<>() )
        .def( "coulomb", &CLJFF::Parameters::coulomb,
                            bp::return_value_policy<bp::copy_const_reference>() )
        .def( "lj", &CLJFF::Parameters::lj,
                            bp::return_value_policy<bp::copy_const_reference>() )
    ;

}

}
