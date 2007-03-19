// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "SymbolComplex.pypp.hpp"
#include "boost/python.hpp"
#include "sirecas_headers.h"

namespace bp = boost::python;

SireCAS::SymbolComplex __copy__(const SireCAS::SymbolComplex &other){ return SireCAS::SymbolComplex(other); }

const char* pvt_get_name(const SireCAS::SymbolComplex&){ return "SireCAS::SymbolComplex";}

void register_SymbolComplex_class(){

    bp::class_< SireCAS::SymbolComplex >( "SymbolComplex", bp::init< SireCAS::SymbolID, SireMaths::Complex const & >(( bp::arg("id"), bp::arg("val") )) )    
        .def( bp::init< SireCAS::SymbolID, double >(( bp::arg("id"), bp::arg("val") )) )    
        .def( bp::init< SireCAS::SymbolValue const & >(( bp::arg("val") )) )    
        .def( 
            "ID"
            , (::SireCAS::SymbolID ( ::SireCAS::SymbolComplex::* )(  ) const)( &::SireCAS::SymbolComplex::ID ) )    
        .def( 
            "value"
            , (::SireMaths::Complex const & ( ::SireCAS::SymbolComplex::* )(  ) const)( &::SireCAS::SymbolComplex::value )
            , bp::return_value_policy< bp::copy_const_reference >() )    
        .def( "__copy__", &__copy__)    
        .def( "__str__", &pvt_get_name);

}
