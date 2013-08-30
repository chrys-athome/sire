// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "Helpers/clone_const_reference.hpp"
#include "TwoAtomFunction.pypp.hpp"

namespace bp = boost::python;

#include "SireCAS/symbols.h"

#include "SireError/errors.h"

#include "SireMol/atommatcher.h"

#include "SireMol/atomselection.h"

#include "SireMol/errors.h"

#include "SireMol/moleculeinfodata.h"

#include "SireStream/datastream.h"

#include "SireStream/shareddatastream.h"

#include "sireglobal.h"

#include "twoatomfunctions.h"

#include "twoatomfunctions.h"

SireMM::TwoAtomFunction __copy__(const SireMM::TwoAtomFunction &other){ return SireMM::TwoAtomFunction(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_TwoAtomFunction_class(){

    { //::SireMM::TwoAtomFunction
        typedef bp::class_< SireMM::TwoAtomFunction, bp::bases< SireMM::AtomFunction > > TwoAtomFunction_exposer_t;
        TwoAtomFunction_exposer_t TwoAtomFunction_exposer = TwoAtomFunction_exposer_t( "TwoAtomFunction", bp::init< >() );
        bp::scope TwoAtomFunction_scope( TwoAtomFunction_exposer );
        TwoAtomFunction_exposer.def( bp::init< SireMol::CGAtomIdx const &, SireMol::CGAtomIdx const &, SireCAS::Expression const & >(( bp::arg("atom0"), bp::arg("atom1"), bp::arg("function") )) );
        TwoAtomFunction_exposer.def( bp::init< SireMM::TwoAtomFunction const & >(( bp::arg("other") )) );
        { //::SireMM::TwoAtomFunction::atom0
        
            typedef ::SireMol::CGAtomIdx const & ( ::SireMM::TwoAtomFunction::*atom0_function_type )(  ) const;
            atom0_function_type atom0_function_value( &::SireMM::TwoAtomFunction::atom0 );
            
            TwoAtomFunction_exposer.def( 
                "atom0"
                , atom0_function_value
                , bp::return_value_policy<bp::clone_const_reference>() );
        
        }
        { //::SireMM::TwoAtomFunction::atom1
        
            typedef ::SireMol::CGAtomIdx const & ( ::SireMM::TwoAtomFunction::*atom1_function_type )(  ) const;
            atom1_function_type atom1_function_value( &::SireMM::TwoAtomFunction::atom1 );
            
            TwoAtomFunction_exposer.def( 
                "atom1"
                , atom1_function_value
                , bp::return_value_policy<bp::clone_const_reference>() );
        
        }
        TwoAtomFunction_exposer.def( bp::self != bp::self );
        { //::SireMM::TwoAtomFunction::operator=
        
            typedef ::SireMM::TwoAtomFunction & ( ::SireMM::TwoAtomFunction::*assign_function_type )( ::SireMM::TwoAtomFunction const & ) ;
            assign_function_type assign_function_value( &::SireMM::TwoAtomFunction::operator= );
            
            TwoAtomFunction_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        TwoAtomFunction_exposer.def( bp::self == bp::self );
        { //::SireMM::TwoAtomFunction::toString
        
            typedef ::QString ( ::SireMM::TwoAtomFunction::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireMM::TwoAtomFunction::toString );
            
            TwoAtomFunction_exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        TwoAtomFunction_exposer.def( "__copy__", &__copy__);
        TwoAtomFunction_exposer.def( "__deepcopy__", &__copy__);
        TwoAtomFunction_exposer.def( "clone", &__copy__);
        TwoAtomFunction_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireMM::TwoAtomFunction >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        TwoAtomFunction_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireMM::TwoAtomFunction >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        TwoAtomFunction_exposer.def( "__str__", &__str__< ::SireMM::TwoAtomFunction > );
        TwoAtomFunction_exposer.def( "__repr__", &__str__< ::SireMM::TwoAtomFunction > );
    }

}