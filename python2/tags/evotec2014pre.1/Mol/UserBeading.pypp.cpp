// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "Helpers/clone_const_reference.hpp"
#include "UserBeading.pypp.hpp"

namespace bp = boost::python;

#include "SireBase/errors.h"

#include "SireError/errors.h"

#include "SireStream/datastream.h"

#include "SireStream/shareddatastream.h"

#include "atombeads.h"

#include "atomidx.h"

#include "atomselection.h"

#include "beadidx.h"

#include "beading.h"

#include "moleculedata.h"

#include "moleculeinfodata.h"

#include <boost/noncopyable.hpp>

#include "beading.h"

SireMol::UserBeading __copy__(const SireMol::UserBeading &other){ return SireMol::UserBeading(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_UserBeading_class(){

    { //::SireMol::UserBeading
        typedef bp::class_< SireMol::UserBeading, bp::bases< SireMol::Beading, SireMol::MolViewProperty, SireBase::Property > > UserBeading_exposer_t;
        UserBeading_exposer_t UserBeading_exposer = UserBeading_exposer_t( "UserBeading", bp::init< >() );
        bp::scope UserBeading_scope( UserBeading_exposer );
        UserBeading_exposer.def( bp::init< SireMol::AtomBeads const & >(( bp::arg("beads") )) );
        UserBeading_exposer.def( bp::init< SireMol::UserBeading const & >(( bp::arg("other") )) );
        { //::SireMol::UserBeading::atomBeads
        
            typedef ::SireMol::AtomBeads const & ( ::SireMol::UserBeading::*atomBeads_function_type )(  ) const;
            atomBeads_function_type atomBeads_function_value( &::SireMol::UserBeading::atomBeads );
            
            UserBeading_exposer.def( 
                "atomBeads"
                , atomBeads_function_value
                , bp::return_value_policy<bp::clone_const_reference>() );
        
        }
        { //::SireMol::UserBeading::isCompatibleWith
        
            typedef bool ( ::SireMol::UserBeading::*isCompatibleWith_function_type )( ::SireMol::MoleculeInfoData const & ) const;
            isCompatibleWith_function_type isCompatibleWith_function_value( &::SireMol::UserBeading::isCompatibleWith );
            
            UserBeading_exposer.def( 
                "isCompatibleWith"
                , isCompatibleWith_function_value
                , ( bp::arg("molinfo") ) );
        
        }
        UserBeading_exposer.def( bp::self != bp::self );
        { //::SireMol::UserBeading::operator=
        
            typedef ::SireMol::UserBeading & ( ::SireMol::UserBeading::*assign_function_type )( ::SireMol::UserBeading const & ) ;
            assign_function_type assign_function_value( &::SireMol::UserBeading::operator= );
            
            UserBeading_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        UserBeading_exposer.def( bp::self == bp::self );
        { //::SireMol::UserBeading::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireMol::UserBeading::typeName );
            
            UserBeading_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        UserBeading_exposer.staticmethod( "typeName" );
        UserBeading_exposer.def( "__copy__", &__copy__);
        UserBeading_exposer.def( "__deepcopy__", &__copy__);
        UserBeading_exposer.def( "clone", &__copy__);
        UserBeading_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireMol::UserBeading >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        UserBeading_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireMol::UserBeading >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        UserBeading_exposer.def( "__str__", &__str__< ::SireMol::UserBeading > );
        UserBeading_exposer.def( "__repr__", &__str__< ::SireMol::UserBeading > );
    }

}
