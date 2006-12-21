
#include "boost/python.hpp"

#include "SireCAS/qhash_sirecas.h"

#include "SireFF/ffbase.h"

#include "SireMol/molecule.h"
#include "SireMol/residue.h"

namespace bp = boost::python;

namespace SireFF
{

void
SIREFF_EXPORT
export_FFBase()
{
    bp::class_< FFBase, boost::noncopyable >( "FFBase", bp::no_init )
        .def(
            "components"
            , &::SireFF::FFBase::components
            , bp::return_internal_reference<>() )
        .def(
            "energies"
            , &::SireFF::FFBase::energies
            , bp::default_call_policies() )
        .def(
            "energy"
            , (double ( ::SireFF::FFBase::* )(  ) )( &::SireFF::FFBase::energy )
            , bp::default_call_policies() )
        .def(
            "energy"
            , (double ( ::SireFF::FFBase::* )( ::SireCAS::Function const & ) )( &::SireFF::FFBase::energy )
            , ( bp::arg("component") )
            , bp::default_call_policies() )
        .def(
            "ID"
            , &::SireFF::FFBase::ID
            , bp::default_call_policies() )
        .def(
            "version"
            , &::SireFF::FFBase::version
            , bp::return_value_policy< bp::copy_const_reference, bp::default_call_policies >() )
        .def(
            "move"
            , (bool ( ::SireFF::FFBase::* )( ::SireMol::Molecule const & ) )(&::SireFF::FFBase::move)
            , ( bp::arg("mol") )
            , bp::default_call_policies() )
        .def(
            "move"
            , (bool ( ::SireFF::FFBase::* )( ::SireMol::Residue const & ) )(&::SireFF::FFBase::move)
            , ( bp::arg("res") )
            , bp::default_call_policies() )
        .def(
            "name"
            , &::SireFF::FFBase::name
            , bp::return_value_policy< bp::copy_const_reference, bp::default_call_policies >() )
        .def(
            "parameters"
            , &::SireFF::FFBase::parameters
            , bp::return_internal_reference<>() )
        .def(
            "setName"
            , &::SireFF::FFBase::setName
            , ( bp::arg("name") )
            , bp::default_call_policies() )
        .def(
            "what"
            , bp::pure_virtual( &::SireFF::FFBase::what )
            , bp::default_call_policies() )
    ;

    bp::class_<FFBase::Components, boost::noncopyable>( "FFBase_Components", bp::no_init )

        .def( "total", &FFBase::Components::total,
                       bp::return_value_policy< bp::copy_const_reference,
                       bp::default_call_policies >() )

        .def( "describe_total", &FFBase::Components::describe_total,
                       bp::default_call_policies() ).staticmethod("describe_total")
    ;

    bp::class_<FFBase::Parameters, boost::noncopyable>( "FFBase_Parameters", bp::no_init )
    ;
}

}
