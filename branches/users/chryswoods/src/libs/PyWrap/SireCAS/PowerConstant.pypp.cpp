// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "PowerConstant.pypp.hpp"
#include "boost/python.hpp"
#include "sirecas_headers.h"

namespace bp = boost::python;

#include "SireQt/qdatastream.hpp"

#include "SirePy/str.hpp"

void register_PowerConstant_class(){

    bp::class_< SireCAS::PowerConstant, bp::bases< SireCAS::PowerFunction > >( "PowerConstant" )    
        .def( bp::init< >() )    
        .def( bp::init< double, SireCAS::Expression const & >(( bp::arg("val"), bp::arg("power") )) )    
        .def( 
            "core"
            , (::SireCAS::Expression ( ::SireCAS::PowerConstant::* )(  ) const)( &::SireCAS::PowerConstant::core ) )    
        .def( 
            "evaluate"
            , (double ( ::SireCAS::PowerConstant::* )( ::SireCAS::Values const & ) const)( &::SireCAS::PowerConstant::evaluate )
            , ( bp::arg("values") ) )    
        .def( 
            "evaluate"
            , (::SireMaths::Complex ( ::SireCAS::PowerConstant::* )( ::SireCAS::ComplexValues const & ) const)( &::SireCAS::PowerConstant::evaluate )
            , ( bp::arg("values") ) )    
        .def( 
            "hash"
            , (::uint ( ::SireCAS::PowerConstant::* )(  ) const)( &::SireCAS::PowerConstant::hash ) )    
        .def( bp::self == bp::other< SireCAS::ExBase >() )    
        .def( 
            "power"
            , (::SireCAS::Expression ( ::SireCAS::PowerConstant::* )(  ) const)( &::SireCAS::PowerConstant::power ) )    
        .def( 
            "typeName"
            , (char const * (*)(  ))( &::SireCAS::PowerConstant::typeName ) )    
        .def( 
            "what"
            , (char const * ( ::SireCAS::PowerConstant::* )(  ) const)( &::SireCAS::PowerConstant::what ) )    
        .staticmethod( "typeName" )    
        .def( "__rlshift__", &SireQt::__rlshift__QDataStream< ::SireCAS::PowerConstant >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__rrshift__", &SireQt::__rrshift__QDataStream< ::SireCAS::PowerConstant >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__str__", &SirePy::__str__< ::SireCAS::PowerConstant > );

}
