// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "CLJBox.pypp.hpp"

namespace bp = boost::python;

#include "SireError/errors.h"

#include "SireMM/cljboxes.h"

#include "SireStream/datastream.h"

#include "SireStream/shareddatastream.h"

#include "SireVol/aabox.h"

#include "SireVol/space.h"

#include "cljboxes.h"

#include <QDebug>

#include <QElapsedTimer>

#include "cljboxes.h"

SireMM::CLJBox __copy__(const SireMM::CLJBox &other){ return SireMM::CLJBox(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_CLJBox_class(){

    { //::SireMM::CLJBox
        typedef bp::class_< SireMM::CLJBox > CLJBox_exposer_t;
        CLJBox_exposer_t CLJBox_exposer = CLJBox_exposer_t( "CLJBox", bp::init< >() );
        bp::scope CLJBox_scope( CLJBox_exposer );
        CLJBox_exposer.def( bp::init< SireMM::CLJAtoms const & >(( bp::arg("atoms") )) );
        CLJBox_exposer.def( bp::init< SireMM::CLJBox const & >(( bp::arg("other") )) );
        { //::SireMM::CLJBox::atoms
        
            typedef ::SireMM::CLJAtoms const & ( ::SireMM::CLJBox::*atoms_function_type )(  ) const;
            atoms_function_type atoms_function_value( &::SireMM::CLJBox::atoms );
            
            CLJBox_exposer.def( 
                "atoms"
                , atoms_function_value
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMM::CLJBox::nAtoms
        
            typedef int ( ::SireMM::CLJBox::*nAtoms_function_type )(  ) const;
            nAtoms_function_type nAtoms_function_value( &::SireMM::CLJBox::nAtoms );
            
            CLJBox_exposer.def( 
                "nAtoms"
                , nAtoms_function_value );
        
        }
        CLJBox_exposer.def( bp::self != bp::self );
        { //::SireMM::CLJBox::operator=
        
            typedef ::SireMM::CLJBox & ( ::SireMM::CLJBox::*assign_function_type )( ::SireMM::CLJBox const & ) ;
            assign_function_type assign_function_value( &::SireMM::CLJBox::operator= );
            
            CLJBox_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        CLJBox_exposer.def( bp::self == bp::self );
        { //::SireMM::CLJBox::toString
        
            typedef ::QString ( ::SireMM::CLJBox::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireMM::CLJBox::toString );
            
            CLJBox_exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        { //::SireMM::CLJBox::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireMM::CLJBox::typeName );
            
            CLJBox_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        { //::SireMM::CLJBox::what
        
            typedef char const * ( ::SireMM::CLJBox::*what_function_type )(  ) const;
            what_function_type what_function_value( &::SireMM::CLJBox::what );
            
            CLJBox_exposer.def( 
                "what"
                , what_function_value );
        
        }
        CLJBox_exposer.staticmethod( "typeName" );
        CLJBox_exposer.def( "__copy__", &__copy__);
        CLJBox_exposer.def( "__deepcopy__", &__copy__);
        CLJBox_exposer.def( "clone", &__copy__);
        CLJBox_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireMM::CLJBox >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        CLJBox_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireMM::CLJBox >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        CLJBox_exposer.def( "__str__", &__str__< ::SireMM::CLJBox > );
        CLJBox_exposer.def( "__repr__", &__str__< ::SireMM::CLJBox > );
    }

}
