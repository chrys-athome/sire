// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "ArcCsc.pypp.hpp"
#include "boost/python.hpp"
#include "sirecas_headers.h"

namespace bp = boost::python;

#include "SireQt/qdatastream.hpp"

#include "SirePy/str.hpp"

void register_ArcCsc_class(){

    bp::class_< SireCAS::ArcCsc, bp::bases< SireCAS::SingleFunc > >( "ArcCsc" )    
        .def( bp::init< >() )    
        .def( bp::init< SireCAS::Expression const & >(( bp::arg("ex") )) )    
        .def( 
            "evaluate"
            , (double ( ::SireCAS::ArcCsc::* )( ::SireCAS::Values const & ) const)( &::SireCAS::ArcCsc::evaluate )
            , ( bp::arg("values") ) )    
        .def( 
            "evaluate"
            , (::SireMaths::Complex ( ::SireCAS::ArcCsc::* )( ::SireCAS::ComplexValues const & ) const)( &::SireCAS::ArcCsc::evaluate )
            , ( bp::arg("values") ) )    
        .def( bp::self == bp::other< SireCAS::ExBase >() )    
        .def( 
            "typeName"
            , (char const * (*)(  ))( &::SireCAS::ArcCsc::typeName ) )    
        .def( 
            "what"
            , (char const * ( ::SireCAS::ArcCsc::* )(  ) const)( &::SireCAS::ArcCsc::what ) )    
        .staticmethod( "typeName" )    
        .def( "__rlshift__", &SireQt::__rlshift__QDataStream< ::SireCAS::ArcCsc >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__rrshift__", &SireQt::__rrshift__QDataStream< ::SireCAS::ArcCsc >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__str__", &SirePy::__str__< ::SireCAS::ArcCsc > );

}
