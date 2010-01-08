// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "ResName.pypp.hpp"

namespace bp = boost::python;

#include "resname.h"

#include "resname.h"

#include "SireMol/errors.h"

#include "SireStream/datastream.h"

#include "atom.h"

#include "chain.h"

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

#include "resid.h"

#include "residentifier.h"

#include "residue.h"

#include "segment.h"

#include "selector.hpp"

#include "tostring.h"

#include "resid.h"

SireMol::ResName __copy__(const SireMol::ResName &other){ return SireMol::ResName(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_ResName_class(){

    { //::SireMol::ResName
        typedef bp::class_< SireMol::ResName, bp::bases< SireMol::ResID, SireID::ID, SireID::Name > > ResName_exposer_t;
        ResName_exposer_t ResName_exposer = ResName_exposer_t( "ResName" );
        bp::scope ResName_scope( ResName_exposer );
        ResName_exposer.def( bp::init< >() );
        ResName_exposer.def( bp::init< QString const & >(( bp::arg("name") )) );
        ResName_exposer.def( bp::init< QString const &, SireID::CaseSensitivity >(( bp::arg("name"), bp::arg("case_sensitivity") )) );
        ResName_exposer.def( bp::init< SireMol::ResName const & >(( bp::arg("other") )) );
        { //::SireMol::ResName::hash
        
            typedef ::uint ( ::SireMol::ResName::*hash_function_type )(  ) const;
            hash_function_type hash_function_value( &::SireMol::ResName::hash );
            
            ResName_exposer.def( 
                "hash"
                , hash_function_value );
        
        }
        { //::SireMol::ResName::isNull
        
            typedef bool ( ::SireMol::ResName::*isNull_function_type )(  ) const;
            isNull_function_type isNull_function_value( &::SireMol::ResName::isNull );
            
            ResName_exposer.def( 
                "isNull"
                , isNull_function_value );
        
        }
        { //::SireMol::ResName::map
        
            typedef ::QList< SireMol::ResIdx > ( ::SireMol::ResName::*map_function_type )( ::SireMol::MolInfo const & ) const;
            map_function_type map_function_value( &::SireMol::ResName::map );
            
            ResName_exposer.def( 
                "map"
                , map_function_value
                , ( bp::arg("molinfo") ) );
        
        }
        ResName_exposer.def( bp::self != bp::self );
        { //::SireMol::ResName::operator=
        
            typedef ::SireMol::ResName & ( ::SireMol::ResName::*assign_function_type )( ::SireMol::ResName const & ) ;
            assign_function_type assign_function_value( &::SireMol::ResName::operator= );
            
            ResName_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        ResName_exposer.def( bp::self == bp::other< SireID::ID >() );
        ResName_exposer.def( bp::self == bp::self );
        { //::SireMol::ResName::toString
        
            typedef ::QString ( ::SireMol::ResName::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireMol::ResName::toString );
            
            ResName_exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        { //::SireMol::ResName::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireMol::ResName::typeName );
            
            ResName_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        { //::SireMol::ResName::what
        
            typedef char const * ( ::SireMol::ResName::*what_function_type )(  ) const;
            what_function_type what_function_value( &::SireMol::ResName::what );
            
            ResName_exposer.def( 
                "what"
                , what_function_value );
        
        }
        ResName_exposer.staticmethod( "typeName" );
        ResName_exposer.def( "__copy__", &__copy__);
        ResName_exposer.def( "__deepcopy__", &__copy__);
        ResName_exposer.def( "clone", &__copy__);
        ResName_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireMol::ResName >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        ResName_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireMol::ResName >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        ResName_exposer.def( "__str__", &__str__< ::SireMol::ResName > );
        ResName_exposer.def( "__repr__", &__str__< ::SireMol::ResName > );
    }

}
