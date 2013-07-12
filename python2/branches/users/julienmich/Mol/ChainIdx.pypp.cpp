// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "ChainIdx.pypp.hpp"

namespace bp = boost::python;

#include "chainidx.h"

#include "chainidx.h"

#include "SireMol/errors.h"

#include "atom.h"

#include "chain.h"

#include "chainid.h"

#include "chainidentifier.h"

#include "chainresid.h"

#include "cutgroup.h"

#include "editor.hpp"

#include "groupatomids.h"

#include "groupgroupids.h"

#include "moleculegroup.h"

#include "moleculegroups.h"

#include "molecules.h"

#include "molinfo.h"

#include "mover.hpp"

#include "partialmolecule.h"

#include "residue.h"

#include "segment.h"

#include "selector.hpp"

#include "tostring.h"

#include "chainid.h"

SireMol::ChainIdx __copy__(const SireMol::ChainIdx &other){ return SireMol::ChainIdx(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_ChainIdx_class(){

    { //::SireMol::ChainIdx
        typedef bp::class_< SireMol::ChainIdx, bp::bases< SireMol::ChainID, SireID::ID, SireID::IndexBase > > ChainIdx_exposer_t;
        ChainIdx_exposer_t ChainIdx_exposer = ChainIdx_exposer_t( "ChainIdx" );
        bp::scope ChainIdx_scope( ChainIdx_exposer );
        ChainIdx_exposer.def( bp::init< >() );
        ChainIdx_exposer.def( bp::init< qint32 >(( bp::arg("idx") )) );
        ChainIdx_exposer.def( bp::init< SireMol::ChainIdx const & >(( bp::arg("other") )) );
        { //::SireMol::ChainIdx::hash
        
            typedef ::uint ( ::SireMol::ChainIdx::*hash_function_type )(  ) const;
            hash_function_type hash_function_value( &::SireMol::ChainIdx::hash );
            
            ChainIdx_exposer.def( 
                "hash"
                , hash_function_value );
        
        }
        { //::SireMol::ChainIdx::isNull
        
            typedef bool ( ::SireMol::ChainIdx::*isNull_function_type )(  ) const;
            isNull_function_type isNull_function_value( &::SireMol::ChainIdx::isNull );
            
            ChainIdx_exposer.def( 
                "isNull"
                , isNull_function_value );
        
        }
        { //::SireMol::ChainIdx::map
        
            typedef ::QList< SireMol::ChainIdx > ( ::SireMol::ChainIdx::*map_function_type )( ::SireMol::MolInfo const & ) const;
            map_function_type map_function_value( &::SireMol::ChainIdx::map );
            
            ChainIdx_exposer.def( 
                "map"
                , map_function_value
                , ( bp::arg("molinfo") ) );
        
        }
        { //::SireMol::ChainIdx::null
        
            typedef ::SireMol::ChainIdx ( *null_function_type )(  );
            null_function_type null_function_value( &::SireMol::ChainIdx::null );
            
            ChainIdx_exposer.def( 
                "null"
                , null_function_value );
        
        }
        { //::SireMol::ChainIdx::operator=
        
            typedef ::SireMol::ChainIdx & ( ::SireMol::ChainIdx::*assign_function_type )( ::SireMol::ChainIdx const & ) ;
            assign_function_type assign_function_value( &::SireMol::ChainIdx::operator= );
            
            ChainIdx_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        { //::SireMol::ChainIdx::toString
        
            typedef ::QString ( ::SireMol::ChainIdx::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireMol::ChainIdx::toString );
            
            ChainIdx_exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        { //::SireMol::ChainIdx::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireMol::ChainIdx::typeName );
            
            ChainIdx_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        { //::SireMol::ChainIdx::what
        
            typedef char const * ( ::SireMol::ChainIdx::*what_function_type )(  ) const;
            what_function_type what_function_value( &::SireMol::ChainIdx::what );
            
            ChainIdx_exposer.def( 
                "what"
                , what_function_value );
        
        }
        ChainIdx_exposer.staticmethod( "null" );
        ChainIdx_exposer.staticmethod( "typeName" );
        ChainIdx_exposer.def( "__copy__", &__copy__);
        ChainIdx_exposer.def( "__deepcopy__", &__copy__);
        ChainIdx_exposer.def( "clone", &__copy__);
        ChainIdx_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireMol::ChainIdx >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        ChainIdx_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireMol::ChainIdx >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        ChainIdx_exposer.def( "__str__", &__str__< ::SireMol::ChainIdx > );
        ChainIdx_exposer.def( "__repr__", &__str__< ::SireMol::ChainIdx > );
    }

}
