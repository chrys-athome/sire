// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "SamplerBase.pypp.hpp"
#include "boost/python.hpp"
#include "siremove_headers.h"
#include "SireMol/molecule.h"
#include "SireMol/residue.h"
#include "SireMol/newatom.h"
#include "SireMol/moleculegroup.h"
#include "SireSystem/simsystem.h"

namespace bp = boost::python;

#include "SireQt/qdatastream.hpp"

const char* pvt_get_name(const SireMove::SamplerBase&){ return "SireMove::SamplerBase";}

void register_SamplerBase_class(){

    bp::class_< SireMove::SamplerBase, bp::bases< SireBase::PropertyBase >, boost::noncopyable >( "SamplerBase", bp::no_init )    
        .def( 
            "assertCompatibleWith"
            , (void ( ::SireMove::SamplerBase::* )( ::SireSystem::QuerySystem const & ) const)( &::SireMove::SamplerBase::assertCompatibleWith )
            , ( bp::arg("system") ) )    
        .def( 
            "generator"
            , (::SireMaths::RanGenerator const & ( ::SireMove::SamplerBase::* )(  ) const)( &::SireMove::SamplerBase::generator )
            , bp::return_value_policy< bp::copy_const_reference >() )    
        .def( 
            "null_sampler"
            , (::SireBase::SharedPolyPointer<SireMove::SamplerBase> (*)(  ))( &::SireMove::SamplerBase::null_sampler ) )    
        .def( 
            "probabilityOf"
            , (double ( ::SireMove::SamplerBase::* )( ::SireMol::PartialMolecule const &,::SireSystem::QuerySystem const & ) )( &::SireMove::SamplerBase::probabilityOf )
            , ( bp::arg("molecule"), bp::arg("system") ) )    
        .def( 
            "sample"
            , (::boost::tuples::tuple<SireMol::PartialMolecule,double,boost::tuples::null_type,boost::tuples::null_type,boost::tuples::null_type,boost::tuples::null_type,boost::tuples::null_type,boost::tuples::null_type,boost::tuples::null_type,boost::tuples::null_type> ( ::SireMove::SamplerBase::* )( ::SireSystem::QuerySystem const & ) )( &::SireMove::SamplerBase::sample )
            , ( bp::arg("system") ) )    
        .def( 
            "setGenerator"
            , (void ( ::SireMove::SamplerBase::* )( ::SireMaths::RanGenerator const & ) )( &::SireMove::SamplerBase::setGenerator )
            , ( bp::arg("generator") ) )    
        .def( 
            "typeName"
            , (char const * (*)(  ))( &::SireMove::SamplerBase::typeName ) )    
        .staticmethod( "null_sampler" )    
        .staticmethod( "typeName" )    
        .def( "__rlshift__", &SireQt::__rlshift__QDataStream< ::SireMove::SamplerBase >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__rrshift__", &SireQt::__rrshift__QDataStream< ::SireMove::SamplerBase >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__str__", &pvt_get_name);

}
