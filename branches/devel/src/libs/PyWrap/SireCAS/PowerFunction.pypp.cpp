// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "PowerFunction.pypp.hpp"
#include "boost/python.hpp"
#include "sirecas_headers.h"

namespace bp = boost::python;

#include "SireQt/qdatastream.hpp"

#include "SirePy/str.hpp"

void register_PowerFunction_class(){

    bp::class_< SireCAS::PowerFunction, bp::bases< SireCAS::ExBase >, boost::noncopyable >( "PowerFunction", bp::no_init )    
        .def( 
            "children"
            , (::SireCAS::Expressions ( ::SireCAS::PowerFunction::* )(  ) const)( &::SireCAS::PowerFunction::children ) )    
        .def( 
            "core"
            , (::SireCAS::Expression ( ::SireCAS::PowerFunction::* )(  ) const)( &::SireCAS::PowerFunction::core ) )    
        .def( 
            "differentiate"
            , (::SireCAS::Expression ( ::SireCAS::PowerFunction::* )( ::SireCAS::Symbol const & ) const)( &::SireCAS::PowerFunction::differentiate )
            , ( bp::arg("symbol") ) )    
        .def( 
            "functions"
            , (::SireCAS::Functions ( ::SireCAS::PowerFunction::* )(  ) const)( &::SireCAS::PowerFunction::functions ) )    
        .def( 
            "integrate"
            , (::SireCAS::Expression ( ::SireCAS::PowerFunction::* )( ::SireCAS::Symbol const & ) const)( &::SireCAS::PowerFunction::integrate )
            , ( bp::arg("symbol") ) )    
        .def( 
            "isCompound"
            , (bool ( ::SireCAS::PowerFunction::* )(  ) const)( &::SireCAS::PowerFunction::isCompound ) )    
        .def( 
            "isConstant"
            , (bool ( ::SireCAS::PowerFunction::* )(  ) const)( &::SireCAS::PowerFunction::isConstant ) )    
        .def( 
            "isFunction"
            , (bool ( ::SireCAS::PowerFunction::* )( ::SireCAS::Symbol const & ) const)( &::SireCAS::PowerFunction::isFunction )
            , ( bp::arg("symbol") ) )    
        .def( 
            "power"
            , (::SireCAS::Expression ( ::SireCAS::PowerFunction::* )(  ) const)( &::SireCAS::PowerFunction::power ) )    
        .def( 
            "reduce"
            , (::SireCAS::Expression ( ::SireCAS::PowerFunction::* )(  ) const)( &::SireCAS::PowerFunction::reduce ) )    
        .def( 
            "substitute"
            , (::SireCAS::Expression ( ::SireCAS::PowerFunction::* )( ::SireCAS::Identities const & ) const)( &::SireCAS::PowerFunction::substitute )
            , ( bp::arg("identities") ) )    
        .def( 
            "symbols"
            , (::SireCAS::Symbols ( ::SireCAS::PowerFunction::* )(  ) const)( &::SireCAS::PowerFunction::symbols ) )    
        .def( 
            "toString"
            , (::QString ( ::SireCAS::PowerFunction::* )(  ) const)( &::SireCAS::PowerFunction::toString ) )    
        .def( 
            "typeName"
            , (char const * (*)(  ))( &::SireCAS::PowerFunction::typeName ) )    
        .staticmethod( "typeName" )    
        .def( "__rlshift__", &SireQt::__rlshift__QDataStream< ::SireCAS::PowerFunction >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__rrshift__", &SireQt::__rrshift__QDataStream< ::SireCAS::PowerFunction >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__str__", &SirePy::__str__< ::SireCAS::PowerFunction > );

}
