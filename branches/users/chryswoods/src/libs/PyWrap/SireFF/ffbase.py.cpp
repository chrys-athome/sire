
#include "boost/python.hpp"

#include "SireCAS/qhash_sirecas.h"

#include "SireFF/ffbase.h"
#include "SireFF/changedmols.h"

#include "SireMol/molecule.h"
#include "SireMol/residue.h"

#include "SireDB/parametertable.h"

namespace bp = boost::python;

namespace SireFF
{

class MovedMols
{
public:
    MovedMols()
    {}

    ~MovedMols()
    {}
};

void
SIREFF_EXPORT
export_FFBase()
{
    bp::class_< FFBase, boost::noncopyable >( "FFBase", bp::no_init )
        .def(
            "TOTAL"
            , &::SireFF::FFBase::TOTAL
            , bp::default_call_policies() )
        .def(
            "assertContains"
            , (void ( ::SireFF::FFBase::* )( ::SireCAS::Function const & ) const)( &::SireFF::FFBase::assertContains )
            , ( bp::arg("component") )
            , bp::default_call_policies() )
        .def(
            "assertContains"
            , (void ( ::SireFF::FFBase::* )( ::SireMol::Molecule const & ) const)( &::SireFF::FFBase::assertContains )
            , ( bp::arg("molecule") )
            , bp::default_call_policies() )
        .def(
            "assertContains"
            , (void ( ::SireFF::FFBase::* )( ::SireMol::Residue const & ) const)( &::SireFF::FFBase::assertContains )
            , ( bp::arg("residue") )
            , bp::default_call_policies() )
        .def(
            "assertSameMajorVersion"
            , (void ( ::SireFF::FFBase::* )( ::SireMol::Molecule const & ) const)( &::SireFF::FFBase::assertSameMajorVersion )
            , ( bp::arg("molecule") )
            , bp::default_call_policies() )
        .def(
            "assertSameMajorVersion"
            , (void ( ::SireFF::FFBase::* )( ::SireMol::Residue const & ) const)( &::SireFF::FFBase::assertSameMajorVersion )
            , ( bp::arg("residue") )
            , bp::default_call_policies() )
        .def(
            "assertSameVersion"
            , (void ( ::SireFF::FFBase::* )( ::SireMol::Molecule const & ) const)( &::SireFF::FFBase::assertSameVersion )
            , ( bp::arg("molecule") )
            , bp::default_call_policies() )
        .def(
            "assertSameVersion"
            , (void ( ::SireFF::FFBase::* )( ::SireMol::Residue const & ) const)( &::SireFF::FFBase::assertSameVersion )
            , ( bp::arg("residue") )
            , bp::default_call_policies() )
        .def(
            "component"
            , &::SireFF::FFBase::component
            , ( bp::arg("componentid") )
            , bp::return_value_policy< bp::copy_const_reference, bp::default_call_policies >() )
        .def(
            "components"
            , &::SireFF::FFBase::components
            , bp::default_call_policies() )
        .def(
            "p_components"
            , &::SireFF::FFBase::p_components
            , bp::return_value_policy< bp::copy_const_reference, bp::default_call_policies >() )
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
            "molecule"
            , bp::pure_virtual( &::SireFF::FFBase::molecule )
            , ( bp::arg("molid") )
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
            "total"
            , &::SireFF::FFBase::total
            , bp::return_value_policy< bp::copy_const_reference, bp::default_call_policies >() )
        .def(
            "what"
            , bp::pure_virtual( &::SireFF::FFBase::what )
            , bp::default_call_policies() )
        .staticmethod( "TOTAL" );

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
