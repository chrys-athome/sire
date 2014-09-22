// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "BeadNum.pypp.hpp"

namespace bp = boost::python;

#include "beadnum.h"

#include "beadnum.h"

#include "SireStream/datastream.h"

#include "beadid.h"

#include "beadidx.h"

#include "beadnum.h"

#include "beadid.h"

SireMol::BeadNum __copy__(const SireMol::BeadNum &other){ return SireMol::BeadNum(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_BeadNum_class(){

    { //::SireMol::BeadNum
        typedef bp::class_< SireMol::BeadNum, bp::bases< SireMol::BeadID, SireID::ID, SireID::Number > > BeadNum_exposer_t;
        BeadNum_exposer_t BeadNum_exposer = BeadNum_exposer_t( "BeadNum", bp::init< >() );
        bp::scope BeadNum_scope( BeadNum_exposer );
        BeadNum_exposer.def( bp::init< quint32 >(( bp::arg("num") )) );
        BeadNum_exposer.def( bp::init< SireMol::BeadNum const & >(( bp::arg("other") )) );
        { //::SireMol::BeadNum::hash
        
            typedef ::uint ( ::SireMol::BeadNum::*hash_function_type )(  ) const;
            hash_function_type hash_function_value( &::SireMol::BeadNum::hash );
            
            BeadNum_exposer.def( 
                "hash"
                , hash_function_value );
        
        }
        { //::SireMol::BeadNum::isNull
        
            typedef bool ( ::SireMol::BeadNum::*isNull_function_type )(  ) const;
            isNull_function_type isNull_function_value( &::SireMol::BeadNum::isNull );
            
            BeadNum_exposer.def( 
                "isNull"
                , isNull_function_value );
        
        }
        BeadNum_exposer.def( bp::self != bp::self );
        BeadNum_exposer.def( bp::self < bp::self );
        BeadNum_exposer.def( bp::self <= bp::self );
        { //::SireMol::BeadNum::operator=
        
            typedef ::SireMol::BeadNum & ( ::SireMol::BeadNum::*assign_function_type )( ::SireMol::BeadNum const & ) ;
            assign_function_type assign_function_value( &::SireMol::BeadNum::operator= );
            
            BeadNum_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        BeadNum_exposer.def( bp::self == bp::other< SireID::ID >() );
        BeadNum_exposer.def( bp::self == bp::self );
        BeadNum_exposer.def( bp::self > bp::self );
        BeadNum_exposer.def( bp::self >= bp::self );
        { //::SireMol::BeadNum::toString
        
            typedef ::QString ( ::SireMol::BeadNum::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireMol::BeadNum::toString );
            
            BeadNum_exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        { //::SireMol::BeadNum::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireMol::BeadNum::typeName );
            
            BeadNum_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        { //::SireMol::BeadNum::what
        
            typedef char const * ( ::SireMol::BeadNum::*what_function_type )(  ) const;
            what_function_type what_function_value( &::SireMol::BeadNum::what );
            
            BeadNum_exposer.def( 
                "what"
                , what_function_value );
        
        }
        BeadNum_exposer.staticmethod( "typeName" );
        BeadNum_exposer.def( "__copy__", &__copy__);
        BeadNum_exposer.def( "__deepcopy__", &__copy__);
        BeadNum_exposer.def( "clone", &__copy__);
        BeadNum_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireMol::BeadNum >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        BeadNum_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireMol::BeadNum >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        BeadNum_exposer.def( "__str__", &__str__< ::SireMol::BeadNum > );
        BeadNum_exposer.def( "__repr__", &__str__< ::SireMol::BeadNum > );
    }

}
