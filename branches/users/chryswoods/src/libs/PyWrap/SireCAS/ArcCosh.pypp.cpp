// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "ArcCosh.pypp.hpp"
#include "boost/python.hpp"
#include "sirecas_headers.h"

namespace bp = boost::python;

#include "SireQt/qdatastream.hpp"

#include "SirePy/str.hpp"

void register_ArcCosh_class(){

    bp::class_< SireCAS::ArcCosh, bp::bases< SireCAS::SingleFunc > >( "ArcCosh" )    
        .def( bp::init< >() )    
        .def( bp::init< SireCAS::Expression const & >(( bp::arg("ex") )) )    
        .def( 
            "evaluate"
            , (double ( ::SireCAS::ArcCosh::* )( ::SireCAS::Values const & ) const)( &::SireCAS::ArcCosh::evaluate )
            , ( bp::arg("values") ) )    
        .def( 
            "evaluate"
            , (::SireMaths::Complex ( ::SireCAS::ArcCosh::* )( ::SireCAS::ComplexValues const & ) const)( &::SireCAS::ArcCosh::evaluate )
            , ( bp::arg("values") ) )    
        .def( bp::self == bp::other< SireCAS::ExBase >() )    
        .def( 
            "typeName"
            , (char const * (*)(  ))( &::SireCAS::ArcCosh::typeName ) )    
        .def( 
            "what"
            , (char const * ( ::SireCAS::ArcCosh::* )(  ) const)( &::SireCAS::ArcCosh::what ) )    
        .staticmethod( "typeName" )    
        .def( "__rlshift__", &SireQt::__rlshift__QDataStream< ::SireCAS::ArcCosh >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__rrshift__", &SireQt::__rrshift__QDataStream< ::SireCAS::ArcCosh >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__str__", &SirePy::__str__< ::SireCAS::ArcCosh > );

}
