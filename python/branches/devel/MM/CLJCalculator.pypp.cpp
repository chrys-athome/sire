// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "CLJCalculator.pypp.hpp"

namespace bp = boost::python;

#include "cljatoms.h"

#include "cljboxes.h"

#include "cljcalculator.h"

#include "cljdelta.h"

#include "cljfunction.h"

#include "tbb/blocked_range.h"

#include "tbb/parallel_for.h"

#include "cljcalculator.h"

SireMM::CLJCalculator __copy__(const SireMM::CLJCalculator &other){ return SireMM::CLJCalculator(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_CLJCalculator_class(){

    { //::SireMM::CLJCalculator
        typedef bp::class_< SireMM::CLJCalculator > CLJCalculator_exposer_t;
        CLJCalculator_exposer_t CLJCalculator_exposer = CLJCalculator_exposer_t( "CLJCalculator", bp::init< bp::optional< bool > >(( bp::arg("reproducible_sum")=(bool)(false) )) );
        bp::scope CLJCalculator_scope( CLJCalculator_exposer );
        CLJCalculator_exposer.def( bp::init< SireMM::CLJCalculator const & >(( bp::arg("other") )) );
        { //::SireMM::CLJCalculator::calculate
        
            typedef ::boost::tuples::tuple< double, double, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type > ( ::SireMM::CLJCalculator::*calculate_function_type )( ::SireMM::CLJFunction const &,::SireMM::CLJBoxes const & ) const;
            calculate_function_type calculate_function_value( &::SireMM::CLJCalculator::calculate );
            
            CLJCalculator_exposer.def( 
                "calculate"
                , calculate_function_value
                , ( bp::arg("func"), bp::arg("boxes") ) );
        
        }
        { //::SireMM::CLJCalculator::calculate
        
            typedef ::boost::tuples::tuple< QVector< double >, QVector< double >, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type > ( ::SireMM::CLJCalculator::*calculate_function_type )( ::QVector< SireBase::PropPtr< SireMM::CLJFunction > > const &,::SireMM::CLJBoxes const & ) const;
            calculate_function_type calculate_function_value( &::SireMM::CLJCalculator::calculate );
            
            CLJCalculator_exposer.def( 
                "calculate"
                , calculate_function_value
                , ( bp::arg("funcs"), bp::arg("boxes") ) );
        
        }
        { //::SireMM::CLJCalculator::calculate
        
            typedef ::boost::tuples::tuple< double, double, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type > ( ::SireMM::CLJCalculator::*calculate_function_type )( ::SireMM::CLJFunction const &,::SireMM::CLJBoxes const &,::SireMM::CLJBoxes const & ) const;
            calculate_function_type calculate_function_value( &::SireMM::CLJCalculator::calculate );
            
            CLJCalculator_exposer.def( 
                "calculate"
                , calculate_function_value
                , ( bp::arg("func"), bp::arg("boxes0"), bp::arg("boxes1") ) );
        
        }
        { //::SireMM::CLJCalculator::calculate
        
            typedef ::boost::tuples::tuple< double, double, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type > ( ::SireMM::CLJCalculator::*calculate_function_type )( ::SireMM::CLJFunction const &,::SireMM::CLJAtoms const &,::SireMM::CLJBoxes const & ) const;
            calculate_function_type calculate_function_value( &::SireMM::CLJCalculator::calculate );
            
            CLJCalculator_exposer.def( 
                "calculate"
                , calculate_function_value
                , ( bp::arg("func"), bp::arg("atoms0"), bp::arg("boxes1") ) );
        
        }
        { //::SireMM::CLJCalculator::calculate
        
            typedef ::boost::tuples::tuple< QVector< double >, QVector< double >, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type > ( ::SireMM::CLJCalculator::*calculate_function_type )( ::QVector< SireBase::PropPtr< SireMM::CLJFunction > > const &,::SireMM::CLJBoxes const &,::SireMM::CLJBoxes const & ) const;
            calculate_function_type calculate_function_value( &::SireMM::CLJCalculator::calculate );
            
            CLJCalculator_exposer.def( 
                "calculate"
                , calculate_function_value
                , ( bp::arg("funcs"), bp::arg("boxes0"), bp::arg("boxes1") ) );
        
        }
        { //::SireMM::CLJCalculator::calculate
        
            typedef ::boost::tuples::tuple< QVector< double >, QVector< double >, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type > ( ::SireMM::CLJCalculator::*calculate_function_type )( ::QVector< SireBase::PropPtr< SireMM::CLJFunction > > const &,::SireMM::CLJAtoms const &,::SireMM::CLJBoxes const & ) const;
            calculate_function_type calculate_function_value( &::SireMM::CLJCalculator::calculate );
            
            CLJCalculator_exposer.def( 
                "calculate"
                , calculate_function_value
                , ( bp::arg("funcs"), bp::arg("atoms0"), bp::arg("boxes1") ) );
        
        }
        CLJCalculator_exposer.def( bp::self != bp::self );
        { //::SireMM::CLJCalculator::operator=
        
            typedef ::SireMM::CLJCalculator & ( ::SireMM::CLJCalculator::*assign_function_type )( ::SireMM::CLJCalculator const & ) ;
            assign_function_type assign_function_value( &::SireMM::CLJCalculator::operator= );
            
            CLJCalculator_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        CLJCalculator_exposer.def( bp::self == bp::self );
        { //::SireMM::CLJCalculator::toString
        
            typedef ::QString ( ::SireMM::CLJCalculator::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireMM::CLJCalculator::toString );
            
            CLJCalculator_exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        { //::SireMM::CLJCalculator::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireMM::CLJCalculator::typeName );
            
            CLJCalculator_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        { //::SireMM::CLJCalculator::what
        
            typedef char const * ( ::SireMM::CLJCalculator::*what_function_type )(  ) const;
            what_function_type what_function_value( &::SireMM::CLJCalculator::what );
            
            CLJCalculator_exposer.def( 
                "what"
                , what_function_value );
        
        }
        CLJCalculator_exposer.staticmethod( "typeName" );
        CLJCalculator_exposer.def( "__copy__", &__copy__);
        CLJCalculator_exposer.def( "__deepcopy__", &__copy__);
        CLJCalculator_exposer.def( "clone", &__copy__);
        CLJCalculator_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireMM::CLJCalculator >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        CLJCalculator_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireMM::CLJCalculator >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        CLJCalculator_exposer.def( "__str__", &__str__< ::SireMM::CLJCalculator > );
        CLJCalculator_exposer.def( "__repr__", &__str__< ::SireMM::CLJCalculator > );
    }

}
