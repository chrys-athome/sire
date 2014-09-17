// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "Helpers/clone_const_reference.hpp"
#include "LinkToProperty.pypp.hpp"

namespace bp = boost::python;

#include "SireStream/datastream.h"

#include "SireStream/shareddatastream.h"

#include "linktoproperty.h"

#include "linktoproperty.h"

SireBase::LinkToProperty __copy__(const SireBase::LinkToProperty &other){ return SireBase::LinkToProperty(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_LinkToProperty_class(){

    { //::SireBase::LinkToProperty
        typedef bp::class_< SireBase::LinkToProperty, bp::bases< SireBase::Property > > LinkToProperty_exposer_t;
        LinkToProperty_exposer_t LinkToProperty_exposer = LinkToProperty_exposer_t( "LinkToProperty", bp::init< >() );
        bp::scope LinkToProperty_scope( LinkToProperty_exposer );
        LinkToProperty_exposer.def( bp::init< SireBase::PropertyName const & >(( bp::arg("source") )) );
        LinkToProperty_exposer.def( bp::init< SireBase::PropertyName const &, SireID::ID const & >(( bp::arg("source"), bp::arg("filter") )) );
        LinkToProperty_exposer.def( bp::init< SireBase::LinkToProperty const & >(( bp::arg("other") )) );
        { //::SireBase::LinkToProperty::filter
        
            typedef ::SireID::ID const & ( ::SireBase::LinkToProperty::*filter_function_type )(  ) const;
            filter_function_type filter_function_value( &::SireBase::LinkToProperty::filter );
            
            LinkToProperty_exposer.def( 
                "filter"
                , filter_function_value
                , bp::return_value_policy<bp::clone_const_reference>() );
        
        }
        { //::SireBase::LinkToProperty::isFiltered
        
            typedef bool ( ::SireBase::LinkToProperty::*isFiltered_function_type )(  ) const;
            isFiltered_function_type isFiltered_function_value( &::SireBase::LinkToProperty::isFiltered );
            
            LinkToProperty_exposer.def( 
                "isFiltered"
                , isFiltered_function_value );
        
        }
        LinkToProperty_exposer.def( bp::self != bp::self );
        { //::SireBase::LinkToProperty::operator=
        
            typedef ::SireBase::LinkToProperty & ( ::SireBase::LinkToProperty::*assign_function_type )( ::SireBase::LinkToProperty const & ) ;
            assign_function_type assign_function_value( &::SireBase::LinkToProperty::operator= );
            
            LinkToProperty_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        LinkToProperty_exposer.def( bp::self == bp::self );
        { //::SireBase::LinkToProperty::target
        
            typedef ::SireBase::PropertyName const & ( ::SireBase::LinkToProperty::*target_function_type )(  ) const;
            target_function_type target_function_value( &::SireBase::LinkToProperty::target );
            
            LinkToProperty_exposer.def( 
                "target"
                , target_function_value
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireBase::LinkToProperty::toString
        
            typedef ::QString ( ::SireBase::LinkToProperty::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireBase::LinkToProperty::toString );
            
            LinkToProperty_exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        { //::SireBase::LinkToProperty::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireBase::LinkToProperty::typeName );
            
            LinkToProperty_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        LinkToProperty_exposer.staticmethod( "typeName" );
        LinkToProperty_exposer.def( "__copy__", &__copy__);
        LinkToProperty_exposer.def( "__deepcopy__", &__copy__);
        LinkToProperty_exposer.def( "clone", &__copy__);
        LinkToProperty_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireBase::LinkToProperty >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        LinkToProperty_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireBase::LinkToProperty >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        LinkToProperty_exposer.def( "__str__", &__str__< ::SireBase::LinkToProperty > );
        LinkToProperty_exposer.def( "__repr__", &__str__< ::SireBase::LinkToProperty > );
    }

}
