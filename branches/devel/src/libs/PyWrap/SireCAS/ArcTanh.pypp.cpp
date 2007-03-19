// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "ArcTanh.pypp.hpp"
#include "boost/python.hpp"
#include "sirecas_headers.h"

namespace bp = boost::python;

SireCAS::ArcTanh __copy__(const SireCAS::ArcTanh &other){ return SireCAS::ArcTanh(other); }

#include "SireQt/qdatastream.hpp"

#include "SirePy/str.hpp"

void register_ArcTanh_class(){

    bp::class_< SireCAS::ArcTanh, bp::bases< SireCAS::SingleFunc > >( "ArcTanh" )    
        .def( bp::init< >() )    
        .def( bp::init< SireCAS::Expression const & >(( bp::arg("ex") )) )    
        .def( 
            "evaluate"
            , (double ( ::SireCAS::ArcTanh::* )( ::SireCAS::Values const & ) const)( &::SireCAS::ArcTanh::evaluate )
            , ( bp::arg("values") ) )    
        .def( 
            "evaluate"
            , (::SireMaths::Complex ( ::SireCAS::ArcTanh::* )( ::SireCAS::ComplexValues const & ) const)( &::SireCAS::ArcTanh::evaluate )
            , ( bp::arg("values") ) )    
        .def( bp::self == bp::other< SireCAS::ExBase >() )    
        .def( 
            "typeName"
            , (char const * (*)(  ))( &::SireCAS::ArcTanh::typeName ) )    
        .def( 
            "what"
            , (char const * ( ::SireCAS::ArcTanh::* )(  ) const)( &::SireCAS::ArcTanh::what ) )    
        .staticmethod( "typeName" )    
        .def( "__copy__", &__copy__)    
        .def( "__rlshift__", &SireQt::__rlshift__QDataStream< ::SireCAS::ArcTanh >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__rrshift__", &SireQt::__rrshift__QDataStream< ::SireCAS::ArcTanh >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__str__", &SirePy::__str__< ::SireCAS::ArcTanh > );

}
