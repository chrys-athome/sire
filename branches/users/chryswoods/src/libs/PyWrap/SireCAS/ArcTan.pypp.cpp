// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "ArcTan.pypp.hpp"
#include "boost/python.hpp"
#include "sirecas_headers.h"

namespace bp = boost::python;

#include "SireQt/qdatastream.hpp"

#include "SirePy/str.hpp"

void register_ArcTan_class(){

    bp::class_< SireCAS::ArcTan, bp::bases< SireCAS::SingleFunc > >( "ArcTan" )    
        .def( bp::init< >() )    
        .def( bp::init< SireCAS::Expression const & >(( bp::arg("ex") )) )    
        .def( 
            "evaluate"
            , (double ( ::SireCAS::ArcTan::* )( ::SireCAS::Values const & ) const)( &::SireCAS::ArcTan::evaluate )
            , ( bp::arg("values") ) )    
        .def( 
            "evaluate"
            , (::SireMaths::Complex ( ::SireCAS::ArcTan::* )( ::SireCAS::ComplexValues const & ) const)( &::SireCAS::ArcTan::evaluate )
            , ( bp::arg("values") ) )    
        .def( bp::self == bp::other< SireCAS::ExBase >() )    
        .def( 
            "typeName"
            , (char const * (*)(  ))( &::SireCAS::ArcTan::typeName ) )    
        .def( 
            "what"
            , (char const * ( ::SireCAS::ArcTan::* )(  ) const)( &::SireCAS::ArcTan::what ) )    
        .staticmethod( "typeName" )    
        .def( "__rlshift__", &SireQt::__rlshift__QDataStream< ::SireCAS::ArcTan >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__rrshift__", &SireQt::__rrshift__QDataStream< ::SireCAS::ArcTan >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__str__", &SirePy::__str__< ::SireCAS::ArcTan > );

}
