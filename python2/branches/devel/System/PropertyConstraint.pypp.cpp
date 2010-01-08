// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "PropertyConstraint.pypp.hpp"

namespace bp = boost::python;

#include "SireError/errors.h"

#include "SireMaths/maths.h"

#include "SireStream/datastream.h"

#include "SireStream/shareddatastream.h"

#include "SireSystem/errors.h"

#include "constraint.h"

#include "system.h"

#include <QDebug>

#include "constraint.h"

SireSystem::PropertyConstraint __copy__(const SireSystem::PropertyConstraint &other){ return SireSystem::PropertyConstraint(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_PropertyConstraint_class(){

    { //::SireSystem::PropertyConstraint
        typedef bp::class_< SireSystem::PropertyConstraint, bp::bases< SireSystem::Constraint, SireBase::Property > > PropertyConstraint_exposer_t;
        PropertyConstraint_exposer_t PropertyConstraint_exposer = PropertyConstraint_exposer_t( "PropertyConstraint" );
        bp::scope PropertyConstraint_scope( PropertyConstraint_exposer );
        PropertyConstraint_exposer.def( bp::init< >() );
        PropertyConstraint_exposer.def( bp::init< QString const &, SireCAS::Expression const & >(( bp::arg("name"), bp::arg("expression") )) );
        PropertyConstraint_exposer.def( bp::init< QString const &, SireFF::FFID const &, SireCAS::Expression const & >(( bp::arg("name"), bp::arg("ffid"), bp::arg("expression") )) );
        PropertyConstraint_exposer.def( bp::init< SireSystem::PropertyConstraint const & >(( bp::arg("other") )) );
        { //::SireSystem::PropertyConstraint::apply
        
            typedef bool ( ::SireSystem::PropertyConstraint::*apply_function_type )( ::SireSystem::System & ) const;
            apply_function_type apply_function_value( &::SireSystem::PropertyConstraint::apply );
            
            PropertyConstraint_exposer.def( 
                "apply"
                , apply_function_value
                , ( bp::arg("system") ) );
        
        }
        { //::SireSystem::PropertyConstraint::isSatisfied
        
            typedef bool ( ::SireSystem::PropertyConstraint::*isSatisfied_function_type )( ::SireSystem::System const & ) const;
            isSatisfied_function_type isSatisfied_function_value( &::SireSystem::PropertyConstraint::isSatisfied );
            
            PropertyConstraint_exposer.def( 
                "isSatisfied"
                , isSatisfied_function_value
                , ( bp::arg("system") ) );
        
        }
        PropertyConstraint_exposer.def( bp::self != bp::self );
        { //::SireSystem::PropertyConstraint::operator=
        
            typedef ::SireSystem::PropertyConstraint & ( ::SireSystem::PropertyConstraint::*assign_function_type )( ::SireSystem::PropertyConstraint const & ) ;
            assign_function_type assign_function_value( &::SireSystem::PropertyConstraint::operator= );
            
            PropertyConstraint_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        PropertyConstraint_exposer.def( bp::self == bp::self );
        { //::SireSystem::PropertyConstraint::toString
        
            typedef ::QString ( ::SireSystem::PropertyConstraint::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireSystem::PropertyConstraint::toString );
            
            PropertyConstraint_exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        { //::SireSystem::PropertyConstraint::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireSystem::PropertyConstraint::typeName );
            
            PropertyConstraint_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        PropertyConstraint_exposer.staticmethod( "typeName" );
        PropertyConstraint_exposer.def( "__copy__", &__copy__);
        PropertyConstraint_exposer.def( "__deepcopy__", &__copy__);
        PropertyConstraint_exposer.def( "clone", &__copy__);
        PropertyConstraint_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireSystem::PropertyConstraint >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        PropertyConstraint_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireSystem::PropertyConstraint >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        PropertyConstraint_exposer.def( "__str__", &__str__< ::SireSystem::PropertyConstraint > );
        PropertyConstraint_exposer.def( "__repr__", &__str__< ::SireSystem::PropertyConstraint > );
    }

}
