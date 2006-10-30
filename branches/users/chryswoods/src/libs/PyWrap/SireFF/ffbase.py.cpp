
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

void export_FFBase()
{
    bp::class_< FFBase, boost::noncopyable >( "FFBase", bp::no_init )
        .def(
            "TOTAL"
            , &::SireFF::FFBase::TOTAL
            , bp::default_call_policies() )
        .def(
            "add"
            , bp::pure_virtual( (void ( ::SireFF::FFBase::* )( ::SireMol::Molecule const &,::SireDB::ParameterTable const &,int ) )(&::SireFF::FFBase::add) )
            , ( bp::arg("mol"), bp::arg("params"), bp::arg("groupid") )
            , bp::default_call_policies() )
        .def(
            "add"
            , bp::pure_virtual( (void ( ::SireFF::FFBase::* )( ::SireMol::Residue const &,::SireDB::ParameterTable const &,int ) )(&::SireFF::FFBase::add) )
            , ( bp::arg("res"), bp::arg("params"), bp::arg("groupid") )
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
            "change"
            , bp::pure_virtual( (void ( ::SireFF::FFBase::* )( ::SireFF::ChangedMols const & ) )(&::SireFF::FFBase::change) )
            , ( bp::arg("changemols") )
            , bp::default_call_policies() )
        .def(
            "change"
            , bp::pure_virtual( (void ( ::SireFF::FFBase::* )( ::SireMol::Molecule const &,::SireDB::ParameterTable const & ) )(&::SireFF::FFBase::change) )
            , ( bp::arg("mol"), bp::arg("params") )
            , bp::default_call_policies() )
        .def(
            "change"
            , bp::pure_virtual( (void ( ::SireFF::FFBase::* )( ::SireMol::Residue const &,::SireDB::ParameterTable const & ) )(&::SireFF::FFBase::change) )
            , ( bp::arg("res"), bp::arg("params") )
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
            , bp::pure_virtual( (void ( ::SireFF::FFBase::* )( ::SireFF::MovedMols const & ) )(&::SireFF::FFBase::move) )
            , ( bp::arg("movemols") )
            , bp::default_call_policies() )
        .def(
            "move"
            , bp::pure_virtual( (void ( ::SireFF::FFBase::* )( ::SireMol::Molecule const & ) )(&::SireFF::FFBase::move) )
            , ( bp::arg("mol") )
            , bp::default_call_policies() )
        .def(
            "move"
            , bp::pure_virtual( (void ( ::SireFF::FFBase::* )( ::SireMol::Residue const & ) )(&::SireFF::FFBase::move) )
            , ( bp::arg("res") )
            , bp::default_call_policies() )
        .def(
            "name"
            , &::SireFF::FFBase::name
            , bp::return_value_policy< bp::copy_const_reference, bp::default_call_policies >() )
        .def(
            "remove"
            , bp::pure_virtual( (void ( ::SireFF::FFBase::* )( ::SireMol::Molecule const & ) )(&::SireFF::FFBase::remove) )
            , ( bp::arg("mol") )
            , bp::default_call_policies() )
        .def(
            "remove"
            , bp::pure_virtual( (void ( ::SireFF::FFBase::* )( ::SireMol::Residue const & ) )(&::SireFF::FFBase::remove) )
            , ( bp::arg("res") )
            , bp::default_call_policies() )
        .def(
            "total"
            , &::SireFF::FFBase::total
            , bp::return_value_policy< bp::copy_const_reference, bp::default_call_policies >() )
        .def(
            "what"
            , bp::pure_virtual( &::SireFF::FFBase::what )
            , bp::default_call_policies() )
        .staticmethod( "TOTAL" );
}

}
