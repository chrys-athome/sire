// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "NullVelocityGenerator.pypp.hpp"

namespace bp = boost::python;

#include "SireCAS/symbol.h"

#include "SireMol/atomelements.h"

#include "SireMol/atommasses.h"

#include "SireMol/atomvelocities.h"

#include "SireMol/moleculedata.h"

#include "SireMol/moleculeinfodata.h"

#include "SireMol/moleculeview.h"

#include "SireStream/datastream.h"

#include "SireStream/shareddatastream.h"

#include "SireUnits/dimensions.h"

#include "SireUnits/temperature.h"

#include "SireUnits/units.h"

#include "velocitygenerator.h"

#include "velocitygenerator.h"

SireMove::NullVelocityGenerator __copy__(const SireMove::NullVelocityGenerator &other){ return SireMove::NullVelocityGenerator(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_NullVelocityGenerator_class(){

    { //::SireMove::NullVelocityGenerator
        typedef bp::class_< SireMove::NullVelocityGenerator, bp::bases< SireMove::VelocityGenerator, SireBase::Property > > NullVelocityGenerator_exposer_t;
        NullVelocityGenerator_exposer_t NullVelocityGenerator_exposer = NullVelocityGenerator_exposer_t( "NullVelocityGenerator", bp::init< >() );
        bp::scope NullVelocityGenerator_scope( NullVelocityGenerator_exposer );
        NullVelocityGenerator_exposer.def( bp::init< SireMove::NullVelocityGenerator const & >(( bp::arg("other") )) );
        { //::SireMove::NullVelocityGenerator::generate
        
            typedef ::SireMol::AtomVelocities ( ::SireMove::NullVelocityGenerator::*generate_function_type )( ::SireMol::MoleculeView const &,::SireBase::PropertyMap const & ) const;
            generate_function_type generate_function_value( &::SireMove::NullVelocityGenerator::generate );
            
            NullVelocityGenerator_exposer.def( 
                "generate"
                , generate_function_value
                , ( bp::arg("molview"), bp::arg("map")=SireBase::PropertyMap() ) );
        
        }
        NullVelocityGenerator_exposer.def( bp::self != bp::self );
        { //::SireMove::NullVelocityGenerator::operator=
        
            typedef ::SireMove::NullVelocityGenerator & ( ::SireMove::NullVelocityGenerator::*assign_function_type )( ::SireMove::NullVelocityGenerator const & ) ;
            assign_function_type assign_function_value( &::SireMove::NullVelocityGenerator::operator= );
            
            NullVelocityGenerator_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        NullVelocityGenerator_exposer.def( bp::self == bp::self );
        { //::SireMove::NullVelocityGenerator::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireMove::NullVelocityGenerator::typeName );
            
            NullVelocityGenerator_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        NullVelocityGenerator_exposer.staticmethod( "typeName" );
        NullVelocityGenerator_exposer.def( "__copy__", &__copy__);
        NullVelocityGenerator_exposer.def( "__deepcopy__", &__copy__);
        NullVelocityGenerator_exposer.def( "clone", &__copy__);
        NullVelocityGenerator_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireMove::NullVelocityGenerator >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        NullVelocityGenerator_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireMove::NullVelocityGenerator >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        NullVelocityGenerator_exposer.def( "__str__", &__str__< ::SireMove::NullVelocityGenerator > );
        NullVelocityGenerator_exposer.def( "__repr__", &__str__< ::SireMove::NullVelocityGenerator > );
    }

}