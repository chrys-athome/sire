
#include "boost/python.hpp"

#include "SireCAS/qhash_sirecas.h"

#include "SireFF/forcefield.h"
#include "SireFF/ffbase.h"

#include "SireMol/molecule.h"
#include "SireMol/residue.h"

namespace bp = boost::python;

namespace SireFF
{

void SIREFF_EXPORT export_ForceField()
{
    bp::class_< SireFF::ForceField >( "ForceField" )
        .def( bp::init< >()[bp::default_call_policies()] )
        .def( bp::init< SireFF::FFBase const & >(( bp::arg("ffbase") )) )
        .def(
            "TOTAL"
            , &::SireFF::ForceField::TOTAL
            , bp::default_call_policies() )
        .def(
            "assertContains"
            , (void ( ::SireFF::ForceField::* )( ::SireCAS::Function const & ) const)( &::SireFF::ForceField::assertContains )
            , ( bp::arg("component") )
            , bp::default_call_policies() )
        .def(
            "assertContains"
            , (void ( ::SireFF::ForceField::* )( ::SireMol::Molecule const & ) const)( &::SireFF::ForceField::assertContains )
            , ( bp::arg("molecule") )
            , bp::default_call_policies() )
        .def(
            "assertContains"
            , (void ( ::SireFF::ForceField::* )( ::SireMol::Residue const & ) const)( &::SireFF::ForceField::assertContains )
            , ( bp::arg("residue") )
            , bp::default_call_policies() )
        .def(
            "assertSameMajorVersion"
            , (void ( ::SireFF::ForceField::* )( ::SireMol::Molecule const & ) const)( &::SireFF::ForceField::assertSameMajorVersion )
            , ( bp::arg("molecule") )
            , bp::default_call_policies() )
        .def(
            "assertSameMajorVersion"
            , (void ( ::SireFF::ForceField::* )( ::SireMol::Residue const & ) const)( &::SireFF::ForceField::assertSameMajorVersion )
            , ( bp::arg("residue") )
            , bp::default_call_policies() )
        .def(
            "assertSameVersion"
            , (void ( ::SireFF::ForceField::* )( ::SireMol::Molecule const & ) const)( &::SireFF::ForceField::assertSameVersion )
            , ( bp::arg("molecule") )
            , bp::default_call_policies() )
        .def(
            "assertSameVersion"
            , (void ( ::SireFF::ForceField::* )( ::SireMol::Residue const & ) const)( &::SireFF::ForceField::assertSameVersion )
            , ( bp::arg("residue") )
            , bp::default_call_policies() )
        .def(
            "component"
            , &::SireFF::ForceField::component
            , ( bp::arg("componentid") )
            , bp::return_value_policy< bp::copy_const_reference, bp::default_call_policies >() )
        .def(
            "components"
            , &::SireFF::ForceField::components
            , bp::default_call_policies() )
        .def(
            "energies"
            , &::SireFF::ForceField::energies
            , bp::default_call_policies() )
        .def(
            "energy"
            , (double ( ::SireFF::ForceField::* )(  ) )( &::SireFF::ForceField::energy )
            , bp::default_call_policies() )
        .def(
            "energy"
            , (double ( ::SireFF::ForceField::* )( ::SireCAS::Function const & ) )( &::SireFF::ForceField::energy )
            , ( bp::arg("component") )
            , bp::default_call_policies() )
        .def(
            "isClean"
            , &::SireFF::ForceField::isClean
            , bp::default_call_policies() )
        .def(
            "isDirty"
            , &::SireFF::ForceField::isDirty
            , bp::default_call_policies() )
        .def(
            "molecule"
            , &::SireFF::ForceField::molecule
            , ( bp::arg("molid") )
            , bp::return_value_policy< bp::copy_const_reference, bp::default_call_policies >() )
        .def(
            "move"
            , (bool ( ::SireFF::ForceField::* )( ::SireMol::Molecule const & ) )( &::SireFF::ForceField::move )
            , ( bp::arg("mol") )
            , bp::default_call_policies() )
        .def(
            "move"
            , (bool ( ::SireFF::ForceField::* )( ::SireMol::Residue const & ) )( &::SireFF::ForceField::move )
            , ( bp::arg("res") )
            , bp::default_call_policies() )
        .def(
            "name"
            , &::SireFF::ForceField::name
            , bp::return_value_policy< bp::copy_const_reference, bp::default_call_policies >() )
        .def(
            "total"
            , &::SireFF::ForceField::total
            , bp::return_value_policy< bp::copy_const_reference, bp::default_call_policies >() )
        .def(
            "what"
            , &::SireFF::ForceField::what
            , bp::default_call_policies() )
        .staticmethod( "TOTAL" );

    bp::implicitly_convertible< SireFF::FFBase const &, SireFF::ForceField >();
}

}
