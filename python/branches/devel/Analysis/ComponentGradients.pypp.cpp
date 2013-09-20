// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "ComponentGradients.pypp.hpp"

namespace bp = boost::python;

#include "SireError/errors.h"

#include "SireStream/datastream.h"

#include "SireStream/shareddatastream.h"

#include "ticomponents.h"

#include "tostring.h"

#include "ticomponents.h"

SireAnalysis::ComponentGradients __copy__(const SireAnalysis::ComponentGradients &other){ return SireAnalysis::ComponentGradients(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_ComponentGradients_class(){

    { //::SireAnalysis::ComponentGradients
        typedef bp::class_< SireAnalysis::ComponentGradients, bp::bases< SireBase::Property > > ComponentGradients_exposer_t;
        ComponentGradients_exposer_t ComponentGradients_exposer = ComponentGradients_exposer_t( "ComponentGradients", bp::init< >() );
        bp::scope ComponentGradients_scope( ComponentGradients_exposer );
        ComponentGradients_exposer.def( bp::init< QMap< double, SireSystem::FreeEnergyMonitor > const & >(( bp::arg("gradients") )) );
        ComponentGradients_exposer.def( bp::init< QList< SireSystem::FreeEnergyMonitor > const & >(( bp::arg("gradients") )) );
        ComponentGradients_exposer.def( bp::init< SireAnalysis::ComponentGradients const & >(( bp::arg("other") )) );
        { //::SireAnalysis::ComponentGradients::coulombGradientsAt
        
            typedef ::SireAnalysis::Gradients ( ::SireAnalysis::ComponentGradients::*coulombGradientsAt_function_type )( int ) const;
            coulombGradientsAt_function_type coulombGradientsAt_function_value( &::SireAnalysis::ComponentGradients::coulombGradientsAt );
            
            ComponentGradients_exposer.def( 
                "coulombGradientsAt"
                , coulombGradientsAt_function_value
                , ( bp::arg("i") ) );
        
        }
        { //::SireAnalysis::ComponentGradients::coulombValues
        
            typedef ::QVector< SireAnalysis::DataPoint > ( ::SireAnalysis::ComponentGradients::*coulombValues_function_type )( int ) const;
            coulombValues_function_type coulombValues_function_value( &::SireAnalysis::ComponentGradients::coulombValues );
            
            ComponentGradients_exposer.def( 
                "coulombValues"
                , coulombValues_function_value
                , ( bp::arg("i") ) );
        
        }
        { //::SireAnalysis::ComponentGradients::data
        
            typedef ::QMap< double, SireSystem::FreeEnergyMonitor > ( ::SireAnalysis::ComponentGradients::*data_function_type )( int ) const;
            data_function_type data_function_value( &::SireAnalysis::ComponentGradients::data );
            
            ComponentGradients_exposer.def( 
                "data"
                , data_function_value
                , ( bp::arg("i") ) );
        
        }
        { //::SireAnalysis::ComponentGradients::deltaLambda
        
            typedef double ( ::SireAnalysis::ComponentGradients::*deltaLambda_function_type )(  ) const;
            deltaLambda_function_type deltaLambda_function_value( &::SireAnalysis::ComponentGradients::deltaLambda );
            
            ComponentGradients_exposer.def( 
                "deltaLambda"
                , deltaLambda_function_value );
        
        }
        { //::SireAnalysis::ComponentGradients::gradientsAt
        
            typedef ::SireAnalysis::Gradients ( ::SireAnalysis::ComponentGradients::*gradientsAt_function_type )( int ) const;
            gradientsAt_function_type gradientsAt_function_value( &::SireAnalysis::ComponentGradients::gradientsAt );
            
            ComponentGradients_exposer.def( 
                "gradientsAt"
                , gradientsAt_function_value
                , ( bp::arg("i") ) );
        
        }
        { //::SireAnalysis::ComponentGradients::integrate
        
            typedef ::SireAnalysis::TIPMF ( ::SireAnalysis::ComponentGradients::*integrate_function_type )( int ) const;
            integrate_function_type integrate_function_value( &::SireAnalysis::ComponentGradients::integrate );
            
            ComponentGradients_exposer.def( 
                "integrate"
                , integrate_function_value
                , ( bp::arg("i") ) );
        
        }
        { //::SireAnalysis::ComponentGradients::integrate
        
            typedef ::SireAnalysis::TIPMF ( ::SireAnalysis::ComponentGradients::*integrate_function_type )( int,int ) const;
            integrate_function_type integrate_function_value( &::SireAnalysis::ComponentGradients::integrate );
            
            ComponentGradients_exposer.def( 
                "integrate"
                , integrate_function_value
                , ( bp::arg("i"), bp::arg("order") ) );
        
        }
        { //::SireAnalysis::ComponentGradients::integrate
        
            typedef ::SireAnalysis::TIPMF ( ::SireAnalysis::ComponentGradients::*integrate_function_type )( int,double,double ) const;
            integrate_function_type integrate_function_value( &::SireAnalysis::ComponentGradients::integrate );
            
            ComponentGradients_exposer.def( 
                "integrate"
                , integrate_function_value
                , ( bp::arg("i"), bp::arg("range_min"), bp::arg("range_max") ) );
        
        }
        { //::SireAnalysis::ComponentGradients::integrate
        
            typedef ::SireAnalysis::TIPMF ( ::SireAnalysis::ComponentGradients::*integrate_function_type )( int,double,double,int ) const;
            integrate_function_type integrate_function_value( &::SireAnalysis::ComponentGradients::integrate );
            
            ComponentGradients_exposer.def( 
                "integrate"
                , integrate_function_value
                , ( bp::arg("i"), bp::arg("range_min"), bp::arg("range_max"), bp::arg("order") ) );
        
        }
        { //::SireAnalysis::ComponentGradients::integrateCoulomb
        
            typedef ::SireAnalysis::TIPMF ( ::SireAnalysis::ComponentGradients::*integrateCoulomb_function_type )( int ) const;
            integrateCoulomb_function_type integrateCoulomb_function_value( &::SireAnalysis::ComponentGradients::integrateCoulomb );
            
            ComponentGradients_exposer.def( 
                "integrateCoulomb"
                , integrateCoulomb_function_value
                , ( bp::arg("i") ) );
        
        }
        { //::SireAnalysis::ComponentGradients::integrateCoulomb
        
            typedef ::SireAnalysis::TIPMF ( ::SireAnalysis::ComponentGradients::*integrateCoulomb_function_type )( int,int ) const;
            integrateCoulomb_function_type integrateCoulomb_function_value( &::SireAnalysis::ComponentGradients::integrateCoulomb );
            
            ComponentGradients_exposer.def( 
                "integrateCoulomb"
                , integrateCoulomb_function_value
                , ( bp::arg("i"), bp::arg("order") ) );
        
        }
        { //::SireAnalysis::ComponentGradients::integrateCoulomb
        
            typedef ::SireAnalysis::TIPMF ( ::SireAnalysis::ComponentGradients::*integrateCoulomb_function_type )( int,double,double ) const;
            integrateCoulomb_function_type integrateCoulomb_function_value( &::SireAnalysis::ComponentGradients::integrateCoulomb );
            
            ComponentGradients_exposer.def( 
                "integrateCoulomb"
                , integrateCoulomb_function_value
                , ( bp::arg("i"), bp::arg("range_min"), bp::arg("range_max") ) );
        
        }
        { //::SireAnalysis::ComponentGradients::integrateCoulomb
        
            typedef ::SireAnalysis::TIPMF ( ::SireAnalysis::ComponentGradients::*integrateCoulomb_function_type )( int,double,double,int ) const;
            integrateCoulomb_function_type integrateCoulomb_function_value( &::SireAnalysis::ComponentGradients::integrateCoulomb );
            
            ComponentGradients_exposer.def( 
                "integrateCoulomb"
                , integrateCoulomb_function_value
                , ( bp::arg("i"), bp::arg("range_min"), bp::arg("range_max"), bp::arg("order") ) );
        
        }
        { //::SireAnalysis::ComponentGradients::integrateLJ
        
            typedef ::SireAnalysis::TIPMF ( ::SireAnalysis::ComponentGradients::*integrateLJ_function_type )( int ) const;
            integrateLJ_function_type integrateLJ_function_value( &::SireAnalysis::ComponentGradients::integrateLJ );
            
            ComponentGradients_exposer.def( 
                "integrateLJ"
                , integrateLJ_function_value
                , ( bp::arg("i") ) );
        
        }
        { //::SireAnalysis::ComponentGradients::integrateLJ
        
            typedef ::SireAnalysis::TIPMF ( ::SireAnalysis::ComponentGradients::*integrateLJ_function_type )( int,int ) const;
            integrateLJ_function_type integrateLJ_function_value( &::SireAnalysis::ComponentGradients::integrateLJ );
            
            ComponentGradients_exposer.def( 
                "integrateLJ"
                , integrateLJ_function_value
                , ( bp::arg("i"), bp::arg("order") ) );
        
        }
        { //::SireAnalysis::ComponentGradients::integrateLJ
        
            typedef ::SireAnalysis::TIPMF ( ::SireAnalysis::ComponentGradients::*integrateLJ_function_type )( int,double,double ) const;
            integrateLJ_function_type integrateLJ_function_value( &::SireAnalysis::ComponentGradients::integrateLJ );
            
            ComponentGradients_exposer.def( 
                "integrateLJ"
                , integrateLJ_function_value
                , ( bp::arg("i"), bp::arg("range_min"), bp::arg("range_max") ) );
        
        }
        { //::SireAnalysis::ComponentGradients::integrateLJ
        
            typedef ::SireAnalysis::TIPMF ( ::SireAnalysis::ComponentGradients::*integrateLJ_function_type )( int,double,double,int ) const;
            integrateLJ_function_type integrateLJ_function_value( &::SireAnalysis::ComponentGradients::integrateLJ );
            
            ComponentGradients_exposer.def( 
                "integrateLJ"
                , integrateLJ_function_value
                , ( bp::arg("i"), bp::arg("range_min"), bp::arg("range_max"), bp::arg("order") ) );
        
        }
        { //::SireAnalysis::ComponentGradients::isCompatible
        
            typedef bool ( ::SireAnalysis::ComponentGradients::*isCompatible_function_type )( ::SireAnalysis::ComponentGradients const & ) const;
            isCompatible_function_type isCompatible_function_value( &::SireAnalysis::ComponentGradients::isCompatible );
            
            ComponentGradients_exposer.def( 
                "isCompatible"
                , isCompatible_function_value
                , ( bp::arg("other") ) );
        
        }
        { //::SireAnalysis::ComponentGradients::isEmpty
        
            typedef bool ( ::SireAnalysis::ComponentGradients::*isEmpty_function_type )(  ) const;
            isEmpty_function_type isEmpty_function_value( &::SireAnalysis::ComponentGradients::isEmpty );
            
            ComponentGradients_exposer.def( 
                "isEmpty"
                , isEmpty_function_value );
        
        }
        { //::SireAnalysis::ComponentGradients::lambdaValues
        
            typedef ::QList< double > ( ::SireAnalysis::ComponentGradients::*lambdaValues_function_type )(  ) const;
            lambdaValues_function_type lambdaValues_function_value( &::SireAnalysis::ComponentGradients::lambdaValues );
            
            ComponentGradients_exposer.def( 
                "lambdaValues"
                , lambdaValues_function_value );
        
        }
        { //::SireAnalysis::ComponentGradients::ljGradientsAt
        
            typedef ::SireAnalysis::Gradients ( ::SireAnalysis::ComponentGradients::*ljGradientsAt_function_type )( int ) const;
            ljGradientsAt_function_type ljGradientsAt_function_value( &::SireAnalysis::ComponentGradients::ljGradientsAt );
            
            ComponentGradients_exposer.def( 
                "ljGradientsAt"
                , ljGradientsAt_function_value
                , ( bp::arg("i") ) );
        
        }
        { //::SireAnalysis::ComponentGradients::ljValues
        
            typedef ::QVector< SireAnalysis::DataPoint > ( ::SireAnalysis::ComponentGradients::*ljValues_function_type )( int ) const;
            ljValues_function_type ljValues_function_value( &::SireAnalysis::ComponentGradients::ljValues );
            
            ComponentGradients_exposer.def( 
                "ljValues"
                , ljValues_function_value
                , ( bp::arg("i") ) );
        
        }
        { //::SireAnalysis::ComponentGradients::merge
        
            typedef ::SireAnalysis::ComponentGradients ( *merge_function_type )( ::QList< SireAnalysis::ComponentGradients > const & );
            merge_function_type merge_function_value( &::SireAnalysis::ComponentGradients::merge );
            
            ComponentGradients_exposer.def( 
                "merge"
                , merge_function_value
                , ( bp::arg("gradients") ) );
        
        }
        { //::SireAnalysis::ComponentGradients::nComponents
        
            typedef int ( ::SireAnalysis::ComponentGradients::*nComponents_function_type )(  ) const;
            nComponents_function_type nComponents_function_value( &::SireAnalysis::ComponentGradients::nComponents );
            
            ComponentGradients_exposer.def( 
                "nComponents"
                , nComponents_function_value );
        
        }
        { //::SireAnalysis::ComponentGradients::nLambdaValues
        
            typedef int ( ::SireAnalysis::ComponentGradients::*nLambdaValues_function_type )(  ) const;
            nLambdaValues_function_type nLambdaValues_function_value( &::SireAnalysis::ComponentGradients::nLambdaValues );
            
            ComponentGradients_exposer.def( 
                "nLambdaValues"
                , nLambdaValues_function_value );
        
        }
        { //::SireAnalysis::ComponentGradients::nSamples
        
            typedef ::qint64 ( ::SireAnalysis::ComponentGradients::*nSamples_function_type )(  ) const;
            nSamples_function_type nSamples_function_value( &::SireAnalysis::ComponentGradients::nSamples );
            
            ComponentGradients_exposer.def( 
                "nSamples"
                , nSamples_function_value );
        
        }
        ComponentGradients_exposer.def( bp::self != bp::self );
        ComponentGradients_exposer.def( bp::self + bp::self );
        { //::SireAnalysis::ComponentGradients::operator=
        
            typedef ::SireAnalysis::ComponentGradients & ( ::SireAnalysis::ComponentGradients::*assign_function_type )( ::SireAnalysis::ComponentGradients const & ) ;
            assign_function_type assign_function_value( &::SireAnalysis::ComponentGradients::operator= );
            
            ComponentGradients_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        ComponentGradients_exposer.def( bp::self == bp::self );
        { //::SireAnalysis::ComponentGradients::temperature
        
            typedef ::SireUnits::Dimension::Temperature ( ::SireAnalysis::ComponentGradients::*temperature_function_type )(  ) const;
            temperature_function_type temperature_function_value( &::SireAnalysis::ComponentGradients::temperature );
            
            ComponentGradients_exposer.def( 
                "temperature"
                , temperature_function_value );
        
        }
        { //::SireAnalysis::ComponentGradients::toString
        
            typedef ::QString ( ::SireAnalysis::ComponentGradients::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireAnalysis::ComponentGradients::toString );
            
            ComponentGradients_exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        { //::SireAnalysis::ComponentGradients::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireAnalysis::ComponentGradients::typeName );
            
            ComponentGradients_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        { //::SireAnalysis::ComponentGradients::values
        
            typedef ::QVector< SireAnalysis::DataPoint > ( ::SireAnalysis::ComponentGradients::*values_function_type )( int ) const;
            values_function_type values_function_value( &::SireAnalysis::ComponentGradients::values );
            
            ComponentGradients_exposer.def( 
                "values"
                , values_function_value
                , ( bp::arg("i") ) );
        
        }
        { //::SireAnalysis::ComponentGradients::viewAt
        
            typedef ::SireMol::PartialMolecule ( ::SireAnalysis::ComponentGradients::*viewAt_function_type )( int ) const;
            viewAt_function_type viewAt_function_value( &::SireAnalysis::ComponentGradients::viewAt );
            
            ComponentGradients_exposer.def( 
                "viewAt"
                , viewAt_function_value
                , ( bp::arg("i") ) );
        
        }
        { //::SireAnalysis::ComponentGradients::viewAt
        
            typedef ::SireMol::PartialMolecule ( ::SireAnalysis::ComponentGradients::*viewAt_function_type )( int,double ) const;
            viewAt_function_type viewAt_function_value( &::SireAnalysis::ComponentGradients::viewAt );
            
            ComponentGradients_exposer.def( 
                "viewAt"
                , viewAt_function_value
                , ( bp::arg("i"), bp::arg("lamval") ) );
        
        }
        { //::SireAnalysis::ComponentGradients::what
        
            typedef char const * ( ::SireAnalysis::ComponentGradients::*what_function_type )(  ) const;
            what_function_type what_function_value( &::SireAnalysis::ComponentGradients::what );
            
            ComponentGradients_exposer.def( 
                "what"
                , what_function_value );
        
        }
        ComponentGradients_exposer.staticmethod( "merge" );
        ComponentGradients_exposer.staticmethod( "typeName" );
        ComponentGradients_exposer.def( "__copy__", &__copy__);
        ComponentGradients_exposer.def( "__deepcopy__", &__copy__);
        ComponentGradients_exposer.def( "clone", &__copy__);
        ComponentGradients_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireAnalysis::ComponentGradients >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        ComponentGradients_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireAnalysis::ComponentGradients >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        ComponentGradients_exposer.def( "__str__", &__str__< ::SireAnalysis::ComponentGradients > );
        ComponentGradients_exposer.def( "__repr__", &__str__< ::SireAnalysis::ComponentGradients > );
    }

}
