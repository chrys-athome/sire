// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "NullGeometryPerturbation.pypp.hpp"

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

SireMol::NullGeometryPerturbation __copy__(const SireMol::NullGeometryPerturbation &other){ return SireMol::NullGeometryPerturbation(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_NullGeometryPerturbation_class(){

    { //::SireMol::NullGeometryPerturbation
        typedef bp::class_< SireMol::NullGeometryPerturbation, bp::bases< SireMol::GeometryPerturbation, SireMol::Perturbation, SireBase::Property > > NullGeometryPerturbation_exposer_t;
        NullGeometryPerturbation_exposer_t NullGeometryPerturbation_exposer = NullGeometryPerturbation_exposer_t( "NullGeometryPerturbation" );
        bp::scope NullGeometryPerturbation_scope( NullGeometryPerturbation_exposer );
        NullGeometryPerturbation_exposer.def( bp::init< >() );
        NullGeometryPerturbation_exposer.def( bp::init< SireMol::NullGeometryPerturbation const & >(( bp::arg("other") )) );
        NullGeometryPerturbation_exposer.def( bp::self != bp::self );
        { //::SireMol::NullGeometryPerturbation::operator=
        
            typedef ::SireMol::NullGeometryPerturbation & ( ::SireMol::NullGeometryPerturbation::*assign_function_type )( ::SireMol::NullGeometryPerturbation const & ) ;
            assign_function_type assign_function_value( &::SireMol::NullGeometryPerturbation::operator= );
            
            NullGeometryPerturbation_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        NullGeometryPerturbation_exposer.def( bp::self == bp::self );
        { //::SireMol::NullGeometryPerturbation::perturbMolecule
        
            typedef void ( ::SireMol::NullGeometryPerturbation::*perturbMolecule_function_type )( ::SireMol::MolEditor &,::SireCAS::Values const & ) const;
            perturbMolecule_function_type perturbMolecule_function_value( &::SireMol::NullGeometryPerturbation::perturbMolecule );
            
            NullGeometryPerturbation_exposer.def( 
                "perturbMolecule"
                , perturbMolecule_function_value
                , ( bp::arg("arg0"), bp::arg("arg1") ) );
        
        }
        { //::SireMol::NullGeometryPerturbation::requiredProperties
        
            typedef ::QSet< QString > ( ::SireMol::NullGeometryPerturbation::*requiredProperties_function_type )(  ) const;
            requiredProperties_function_type requiredProperties_function_value( &::SireMol::NullGeometryPerturbation::requiredProperties );
            
            NullGeometryPerturbation_exposer.def( 
                "requiredProperties"
                , requiredProperties_function_value );
        
        }
        { //::SireMol::NullGeometryPerturbation::requiredSymbols
        
            typedef ::QSet< SireCAS::Symbol > ( ::SireMol::NullGeometryPerturbation::*requiredSymbols_function_type )(  ) const;
            requiredSymbols_function_type requiredSymbols_function_value( &::SireMol::NullGeometryPerturbation::requiredSymbols );
            
            NullGeometryPerturbation_exposer.def( 
                "requiredSymbols"
                , requiredSymbols_function_value );
        
        }
        { //::SireMol::NullGeometryPerturbation::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireMol::NullGeometryPerturbation::typeName );
            
            NullGeometryPerturbation_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        { //::SireMol::NullGeometryPerturbation::wouldChange
        
            typedef bool ( ::SireMol::NullGeometryPerturbation::*wouldChange_function_type )( ::SireMol::Molecule const &,::SireCAS::Values const & ) const;
            wouldChange_function_type wouldChange_function_value( &::SireMol::NullGeometryPerturbation::wouldChange );
            
            NullGeometryPerturbation_exposer.def( 
                "wouldChange"
                , wouldChange_function_value
                , ( bp::arg("arg0"), bp::arg("arg1") ) );
        
        }
        NullGeometryPerturbation_exposer.staticmethod( "typeName" );
        NullGeometryPerturbation_exposer.def( "__copy__", &__copy__);
        NullGeometryPerturbation_exposer.def( "__deepcopy__", &__copy__);
        NullGeometryPerturbation_exposer.def( "clone", &__copy__);
        NullGeometryPerturbation_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireMol::NullGeometryPerturbation >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        NullGeometryPerturbation_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireMol::NullGeometryPerturbation >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        NullGeometryPerturbation_exposer.def( "__str__", &__str__< ::SireMol::NullGeometryPerturbation > );
        NullGeometryPerturbation_exposer.def( "__repr__", &__str__< ::SireMol::NullGeometryPerturbation > );
    }

}
