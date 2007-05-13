// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "PrefSampler.pypp.hpp"
#include "boost/python.hpp"
#include "siremove_headers.h"
#include "SireMol/molecule.h"
#include "SireMol/residue.h"
#include "SireMol/newatom.h"
#include "SireMol/moleculegroup.h"
#include "SireSystem/simsystem.h"

namespace bp = boost::python;

SireMove::PrefSampler __copy__(const SireMove::PrefSampler &other){ return SireMove::PrefSampler(other); }

#include "SireQt/qdatastream.hpp"

const char* pvt_get_name(const SireMove::PrefSampler&){ return "SireMove::PrefSampler";}

void register_PrefSampler_class(){

    bp::class_< SireMove::PrefSampler, bp::bases< SireMove::SamplerBase > >( "PrefSampler", bp::init< bp::optional< SireMaths::RanGenerator const & > >(( bp::arg("rangenerator")=::SireMaths::RanGenerator( ) )) )    
        .def( bp::init< SireMol::MoleculeGroup const &, bp::optional< SireMaths::RanGenerator const & > >(( bp::arg("group"), bp::arg("rangenerator")=::SireMaths::RanGenerator( ) )) )    
        .def( bp::init< SireMol::PartialMolecule const &, bp::optional< SireMaths::RanGenerator const & > >(( bp::arg("center"), bp::arg("rangenerator")=::SireMaths::RanGenerator( ) )) )    
        .def( bp::init< SireMol::PartialMolecule const &, double, bp::optional< SireMaths::RanGenerator const & > >(( bp::arg("center"), bp::arg("k"), bp::arg("rangenerator")=::SireMaths::RanGenerator( ) )) )    
        .def( bp::init< SireMol::PartialMolecule const &, SireMol::MoleculeGroup const &, bp::optional< SireMaths::RanGenerator const & > >(( bp::arg("center"), bp::arg("group"), bp::arg("rangenerator")=::SireMaths::RanGenerator( ) )) )    
        .def( bp::init< SireMol::PartialMolecule const &, double, SireMol::MoleculeGroup const &, bp::optional< SireMaths::RanGenerator const & > >(( bp::arg("center"), bp::arg("k"), bp::arg("group"), bp::arg("rangenerator")=::SireMaths::RanGenerator( ) )) )    
        .def( 
            "assertCompatibleWith"
            , (void ( ::SireMove::PrefSampler::* )( ::SireSystem::QuerySystem const & ) const)( &::SireMove::PrefSampler::assertCompatibleWith )
            , ( bp::arg("system") ) )    
        .def( 
            "centerMolecule"
            , (::SireMol::PartialMolecule const & ( ::SireMove::PrefSampler::* )(  ) const)( &::SireMove::PrefSampler::centerMolecule )
            , bp::return_value_policy< bp::copy_const_reference >() )    
        .def( 
            "k"
            , (double ( ::SireMove::PrefSampler::* )(  ) const)( &::SireMove::PrefSampler::k ) )    
        .def( 
            "probabilityOf"
            , (double ( ::SireMove::PrefSampler::* )( ::SireMol::PartialMolecule const &,::SireSystem::QuerySystem const & ) )( &::SireMove::PrefSampler::probabilityOf )
            , ( bp::arg("molecule"), bp::arg("system") ) )    
        .def( 
            "sample"
            , (::boost::tuples::tuple<SireMol::PartialMolecule,double,boost::tuples::null_type,boost::tuples::null_type,boost::tuples::null_type,boost::tuples::null_type,boost::tuples::null_type,boost::tuples::null_type,boost::tuples::null_type,boost::tuples::null_type> ( ::SireMove::PrefSampler::* )( ::SireSystem::QuerySystem const & ) )( &::SireMove::PrefSampler::sample )
            , ( bp::arg("system") ) )    
        .def( 
            "setCenterMolecule"
            , (void ( ::SireMove::PrefSampler::* )( ::SireMol::PartialMolecule const & ) )( &::SireMove::PrefSampler::setCenterMolecule )
            , ( bp::arg("molecule") ) )    
        .def( 
            "setGroup"
            , (void ( ::SireMove::PrefSampler::* )( ::SireMol::MoleculeGroup const & ) )( &::SireMove::PrefSampler::setGroup )
            , ( bp::arg("group") ) )    
        .def( 
            "setK"
            , (void ( ::SireMove::PrefSampler::* )( double ) )( &::SireMove::PrefSampler::setK )
            , ( bp::arg("k") ) )    
        .def( 
            "typeName"
            , (char const * (*)(  ))( &::SireMove::PrefSampler::typeName ) )    
        .def( 
            "what"
            , (char const * ( ::SireMove::PrefSampler::* )(  ) const)( &::SireMove::PrefSampler::what ) )    
        .staticmethod( "typeName" )    
        .def( "__copy__", &__copy__)    
        .def( "__rlshift__", &SireQt::__rlshift__QDataStream< ::SireMove::PrefSampler >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__rrshift__", &SireQt::__rrshift__QDataStream< ::SireMove::PrefSampler >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__str__", &pvt_get_name);

}
