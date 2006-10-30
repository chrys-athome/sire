
#include "boost/python.hpp"

#include "SireMM/cljff.h"

#include "SireFF/changedmols.h"

#include "SireMol/molecule.h"
#include "SireMol/residue.h"

#include "SireDB/parametertable.h"

namespace bp = boost::python;

namespace SireMM
{

void export_CLJFF()
{
    bp::class_< CLJFF, bp::bases<SireFF::FFBase>, boost::noncopyable >( "CLJFF", bp::no_init )
        .def(
            "COULOMB"
            , &::SireMM::CLJFF::COULOMB
            , bp::default_call_policies() )
        .def(
            "LJ"
            , &::SireMM::CLJFF::LJ
            , bp::default_call_policies() )
        .def(
            "combiningRules"
            , &::SireMM::CLJFF::combiningRules
            , bp::return_value_policy< bp::copy_const_reference, bp::default_call_policies >() )
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
}

}
