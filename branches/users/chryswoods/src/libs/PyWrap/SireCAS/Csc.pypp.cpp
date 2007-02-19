// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "Csc.pypp.hpp"
#include "boost/python.hpp"
#include "sirecas_headers.h"

namespace bp = boost::python;

#include "SireQt/qdatastream.hpp"

#include "SirePy/str.hpp"

void register_Csc_class(){

    bp::class_< SireCAS::Csc, bp::bases< SireCAS::SingleFunc > >( "Csc" )    
        .def( bp::init< >() )    
        .def( bp::init< SireCAS::Expression const & >(( bp::arg("ex") )) )    
        .def( 
            "evaluate"
            , (double ( ::SireCAS::Csc::* )( ::SireCAS::Values const & ) const)( &::SireCAS::Csc::evaluate )
            , ( bp::arg("values") ) )    
        .def( 
            "evaluate"
            , (::SireMaths::Complex ( ::SireCAS::Csc::* )( ::SireCAS::ComplexValues const & ) const)( &::SireCAS::Csc::evaluate )
            , ( bp::arg("values") ) )    
        .def( bp::self == bp::other< SireCAS::ExBase >() )    
        .def( 
            "typeName"
            , (char const * (*)(  ))( &::SireCAS::Csc::typeName ) )    
        .def( 
            "what"
            , (char const * ( ::SireCAS::Csc::* )(  ) const)( &::SireCAS::Csc::what ) )    
        .staticmethod( "typeName" )    
        .def( "__rlshift__", &SireQt::__rlshift__QDataStream< ::SireCAS::Csc >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__rrshift__", &SireQt::__rrshift__QDataStream< ::SireCAS::Csc >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__str__", &SirePy::__str__< ::SireCAS::Csc > );

}
