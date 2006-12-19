
#include "boost/python.hpp"

#include "SireCAS/qhash_sirecas.h"

#include "SireMM/cljff.h"

#include "SireFF/changedmols.h"

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
            "p_components"
            , &::SireMM::CLJFF::p_components
            , bp::return_value_policy< bp::copy_const_reference, bp::default_call_policies >() )
        .def(
            "COULOMB"
            , &::SireMM::CLJFF::COULOMB
            , bp::default_call_policies() )
        .def(
            "LJ"
            , &::SireMM::CLJFF::LJ
            , bp::default_call_policies() )
        .def(
            "coulomb"
            , &::SireMM::CLJFF::coulomb
            , bp::return_value_policy< bp::copy_const_reference, bp::default_call_policies >() )
        .def(
            "lj"
            , &::SireMM::CLJFF::lj
            , bp::return_value_policy< bp::copy_const_reference, bp::default_call_policies >() )
        .def(
            "space"
            , &::SireMM::CLJFF::space
            , bp::return_value_policy< bp::copy_const_reference, bp::default_call_policies >() )
        .def(
            "switchingFunction"
            , &::SireMM::CLJFF::switchingFunction
            , bp::return_value_policy< bp::copy_const_reference, bp::default_call_policies >() )
        .staticmethod( "COULOMB" )
        .staticmethod( "LJ" )
    ;

    bp::class_<CLJFF::Components, bp::bases<FFBase::Components> >("CLJFF_Components", bp::init<>())

        .def( "coulomb", &CLJFF::Components::coulomb,
                bp::return_value_policy< bp::copy_const_reference, bp::default_call_policies >() )
        .def( "lj", &CLJFF::Components::lj,
                bp::return_value_policy< bp::copy_const_reference, bp::default_call_policies >() )

        .def( "describe_coulomb", &CLJFF::Components::describe_coulomb,
                bp::default_call_policies() ).staticmethod("describe_coulomb")

        .def( "describe_lj", &CLJFF::Components::describe_lj,
                bp::default_call_policies() ).staticmethod("describe_lj")
    ;
}

}
