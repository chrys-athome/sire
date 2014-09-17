// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "GeometryPerturbation.pypp.hpp"

namespace bp = boost::python;

#include "SireCAS/identities.h"

#include "SireCAS/values.h"

#include "SireStream/datastream.h"

#include "SireStream/shareddatastream.h"

#include "SireUnits/units.h"

#include "geometryperturbation.h"

#include "molecule.h"

#include "moleditor.h"

#include "mover.hpp"

#include "geometryperturbation.h"

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_GeometryPerturbation_class(){

    { //::SireMol::GeometryPerturbation
        typedef bp::class_< SireMol::GeometryPerturbation, bp::bases< SireMol::Perturbation, SireBase::Property >, boost::noncopyable > GeometryPerturbation_exposer_t;
        GeometryPerturbation_exposer_t GeometryPerturbation_exposer = GeometryPerturbation_exposer_t( "GeometryPerturbation", bp::no_init );
        bp::scope GeometryPerturbation_scope( GeometryPerturbation_exposer );
        { //::SireMol::GeometryPerturbation::null
        
            typedef ::SireMol::NullGeometryPerturbation const & ( *null_function_type )(  );
            null_function_type null_function_value( &::SireMol::GeometryPerturbation::null );
            
            GeometryPerturbation_exposer.def( 
                "null"
                , null_function_value
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMol::GeometryPerturbation::requiredProperties
        
            typedef ::QSet< QString > ( ::SireMol::GeometryPerturbation::*requiredProperties_function_type )(  ) const;
            requiredProperties_function_type requiredProperties_function_value( &::SireMol::GeometryPerturbation::requiredProperties );
            
            GeometryPerturbation_exposer.def( 
                "requiredProperties"
                , requiredProperties_function_value );
        
        }
        { //::SireMol::GeometryPerturbation::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireMol::GeometryPerturbation::typeName );
            
            GeometryPerturbation_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        GeometryPerturbation_exposer.staticmethod( "null" );
        GeometryPerturbation_exposer.staticmethod( "typeName" );
        GeometryPerturbation_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireMol::GeometryPerturbation >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        GeometryPerturbation_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireMol::GeometryPerturbation >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        GeometryPerturbation_exposer.def( "__str__", &__str__< ::SireMol::GeometryPerturbation > );
        GeometryPerturbation_exposer.def( "__repr__", &__str__< ::SireMol::GeometryPerturbation > );
    }

}
