// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "DirValue.pypp.hpp"

namespace bp = boost::python;

#include "SireError/errors.h"

#include "SireSim/values.h"

#include "values.h"

#include <QDomDocument>

#include <QDomElement>

#include "values.h"

SireSim::DirValue __copy__(const SireSim::DirValue &other){ return SireSim::DirValue(other); }

#include "Helpers/str.hpp"

void register_DirValue_class(){

    { //::SireSim::DirValue
        typedef bp::class_< SireSim::DirValue, bp::bases< SireSim::Value > > DirValue_exposer_t;
        DirValue_exposer_t DirValue_exposer = DirValue_exposer_t( "DirValue", bp::init< >() );
        bp::scope DirValue_scope( DirValue_exposer );
        DirValue_exposer.def( bp::init< QString >(( bp::arg("dir") )) );
        DirValue_exposer.def( bp::init< QDomElement >(( bp::arg("elem") )) );
        DirValue_exposer.def( bp::init< SireSim::DirValue const & >(( bp::arg("other") )) );
        DirValue_exposer.def( bp::self != bp::self );
        { //::SireSim::DirValue::operator=
        
            typedef ::SireSim::DirValue & ( ::SireSim::DirValue::*assign_function_type )( ::SireSim::DirValue const & ) ;
            assign_function_type assign_function_value( &::SireSim::DirValue::operator= );
            
            DirValue_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        DirValue_exposer.def( bp::self == bp::self );
        { //::SireSim::DirValue::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireSim::DirValue::typeName );
            
            DirValue_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        { //::SireSim::DirValue::value
        
            typedef ::QString ( ::SireSim::DirValue::*value_function_type )(  ) const;
            value_function_type value_function_value( &::SireSim::DirValue::value );
            
            DirValue_exposer.def( 
                "value"
                , value_function_value );
        
        }
        { //::SireSim::DirValue::what
        
            typedef char const * ( ::SireSim::DirValue::*what_function_type )(  ) const;
            what_function_type what_function_value( &::SireSim::DirValue::what );
            
            DirValue_exposer.def( 
                "what"
                , what_function_value );
        
        }
        DirValue_exposer.staticmethod( "typeName" );
        DirValue_exposer.def( "__copy__", &__copy__);
        DirValue_exposer.def( "__deepcopy__", &__copy__);
        DirValue_exposer.def( "clone", &__copy__);
        DirValue_exposer.def( "__str__", &__str__< ::SireSim::DirValue > );
        DirValue_exposer.def( "__repr__", &__str__< ::SireSim::DirValue > );
    }

}