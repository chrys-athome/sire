// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "ResAtomID.pypp.hpp"

namespace bp = boost::python;

#include "SireMol/errors.h"

#include "SireStream/datastream.h"

#include "atomidentifier.h"

#include "cgidentifier.h"

#include "chainidentifier.h"

#include "groupatomids.h"

#include "residentifier.h"

#include "segidentifier.h"

#include "groupatomids.h"

SireMol::GroupAtomID<SireMol::ResID, SireMol::AtomID> __copy__(const SireMol::GroupAtomID<SireMol::ResID, SireMol::AtomID> &other){ return SireMol::GroupAtomID<SireMol::ResID, SireMol::AtomID>(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_ResAtomID_class(){

    { //::SireMol::GroupAtomID< SireMol::ResID, SireMol::AtomID >
        typedef bp::class_< SireMol::GroupAtomID< SireMol::ResID, SireMol::AtomID >, bp::bases< SireMol::GroupAtomIDBase, SireMol::AtomID, SireID::ID > > ResAtomID_exposer_t;
        ResAtomID_exposer_t ResAtomID_exposer = ResAtomID_exposer_t( "ResAtomID" );
        bp::scope ResAtomID_scope( ResAtomID_exposer );
        ResAtomID_exposer.def( bp::init< >() );
        ResAtomID_exposer.def( bp::init< SireMol::ResID const &, SireMol::AtomID const & >(( bp::arg("group"), bp::arg("atom") )) );
        ResAtomID_exposer.def( bp::init< SireMol::GroupAtomID< SireMol::ResID, SireMol::AtomID > const & >(( bp::arg("other") )) );
        { //::SireMol::GroupAtomID< SireMol::ResID, SireMol::AtomID >::hash
        
            typedef SireMol::GroupAtomID< SireMol::ResID, SireMol::AtomID > exported_class_t;
            typedef ::uint ( ::SireMol::GroupAtomID< SireMol::ResID, SireMol::AtomID >::*hash_function_type )(  ) const;
            hash_function_type hash_function_value( &::SireMol::GroupAtomID< SireMol::ResID, SireMol::AtomID >::hash );
            
            ResAtomID_exposer.def( 
                "hash"
                , hash_function_value );
        
        }
        { //::SireMol::GroupAtomID< SireMol::ResID, SireMol::AtomID >::isNull
        
            typedef SireMol::GroupAtomID< SireMol::ResID, SireMol::AtomID > exported_class_t;
            typedef bool ( ::SireMol::GroupAtomID< SireMol::ResID, SireMol::AtomID >::*isNull_function_type )(  ) const;
            isNull_function_type isNull_function_value( &::SireMol::GroupAtomID< SireMol::ResID, SireMol::AtomID >::isNull );
            
            ResAtomID_exposer.def( 
                "isNull"
                , isNull_function_value );
        
        }
        { //::SireMol::GroupAtomID< SireMol::ResID, SireMol::AtomID >::map
        
            typedef SireMol::GroupAtomID< SireMol::ResID, SireMol::AtomID > exported_class_t;
            typedef ::QList< SireMol::AtomIdx > ( ::SireMol::GroupAtomID< SireMol::ResID, SireMol::AtomID >::*map_function_type )( ::SireMol::MolInfo const & ) const;
            map_function_type map_function_value( &::SireMol::GroupAtomID< SireMol::ResID, SireMol::AtomID >::map );
            
            ResAtomID_exposer.def( 
                "map"
                , map_function_value
                , ( bp::arg("molinfo") ) );
        
        }
        ResAtomID_exposer.def( bp::self != bp::self );
        ResAtomID_exposer.def( bp::self == bp::self );
        ResAtomID_exposer.def( bp::self == bp::other< SireID::ID >() );
        { //::SireMol::GroupAtomID< SireMol::ResID, SireMol::AtomID >::toString
        
            typedef SireMol::GroupAtomID< SireMol::ResID, SireMol::AtomID > exported_class_t;
            typedef ::QString ( ::SireMol::GroupAtomID< SireMol::ResID, SireMol::AtomID >::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireMol::GroupAtomID< SireMol::ResID, SireMol::AtomID >::toString );
            
            ResAtomID_exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        { //::SireMol::GroupAtomID< SireMol::ResID, SireMol::AtomID >::typeName
        
            typedef SireMol::GroupAtomID< SireMol::ResID, SireMol::AtomID > exported_class_t;
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireMol::GroupAtomID< SireMol::ResID, SireMol::AtomID >::typeName );
            
            ResAtomID_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        { //::SireMol::GroupAtomID< SireMol::ResID, SireMol::AtomID >::what
        
            typedef SireMol::GroupAtomID< SireMol::ResID, SireMol::AtomID > exported_class_t;
            typedef char const * ( ::SireMol::GroupAtomID< SireMol::ResID, SireMol::AtomID >::*what_function_type )(  ) const;
            what_function_type what_function_value( &::SireMol::GroupAtomID< SireMol::ResID, SireMol::AtomID >::what );
            
            ResAtomID_exposer.def( 
                "what"
                , what_function_value );
        
        }
        ResAtomID_exposer.staticmethod( "typeName" );
        ResAtomID_exposer.def( "__copy__", &__copy__);
        ResAtomID_exposer.def( "__deepcopy__", &__copy__);
        ResAtomID_exposer.def( "clone", &__copy__);
        ResAtomID_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireMol::GroupAtomID<SireMol::ResID, SireMol::AtomID> >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        ResAtomID_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireMol::GroupAtomID<SireMol::ResID, SireMol::AtomID> >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        ResAtomID_exposer.def( "__str__", &__str__< ::SireMol::GroupAtomID<SireMol::ResID, SireMol::AtomID> > );
        ResAtomID_exposer.def( "__repr__", &__str__< ::SireMol::GroupAtomID<SireMol::ResID, SireMol::AtomID> > );
    }

}
