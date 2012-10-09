// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "RBWorkspaceJM.pypp.hpp"

namespace bp = boost::python;

#include "SireError/errors.h"

#include "SireMaths/axisset.h"

#include "SireMol/atombeading.h"

#include "SireMol/atomcoords.h"

#include "SireMol/atomelements.h"

#include "SireMol/atommasses.h"

#include "SireMol/beadnum.h"

#include "SireMol/molecule.h"

#include "SireMol/moleditor.h"

#include "SireStream/datastream.h"

#include "SireStream/shareddatastream.h"

#include "rbworkspacejm.h"

#include "stdio.h"

#include "rbworkspacejm.h"

SireMove::RBWorkspaceJM __copy__(const SireMove::RBWorkspaceJM &other){ return SireMove::RBWorkspaceJM(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_RBWorkspaceJM_class(){

    { //::SireMove::RBWorkspaceJM
        typedef bp::class_< SireMove::RBWorkspaceJM, bp::bases< SireBase::Property > > RBWorkspaceJM_exposer_t;
        RBWorkspaceJM_exposer_t RBWorkspaceJM_exposer = RBWorkspaceJM_exposer_t( "RBWorkspaceJM", bp::init< bp::optional< SireBase::PropertyMap const & > >(( bp::arg("map")=SireBase::PropertyMap() )) );
        bp::scope RBWorkspaceJM_scope( RBWorkspaceJM_exposer );
        RBWorkspaceJM_exposer.def( bp::init< SireMol::MoleculeGroup const &, bp::optional< SireBase::PropertyMap const & > >(( bp::arg("molgroup"), bp::arg("map")=SireBase::PropertyMap() )) );
        RBWorkspaceJM_exposer.def( bp::init< SireMove::RBWorkspaceJM const & >(( bp::arg("other") )) );
        { //::SireMove::RBWorkspaceJM::beadEnergiesArray
        
            typedef ::QVector< SireMaths::Vector > ( ::SireMove::RBWorkspaceJM::*beadEnergiesArray_function_type )(  ) const;
            beadEnergiesArray_function_type beadEnergiesArray_function_value( &::SireMove::RBWorkspaceJM::beadEnergiesArray );
            
            RBWorkspaceJM_exposer.def( 
                "beadEnergiesArray"
                , beadEnergiesArray_function_value );
        
        }
        { //::SireMove::RBWorkspaceJM::beadForcesArray
        
            typedef ::QVector< SireMaths::Vector > ( ::SireMove::RBWorkspaceJM::*beadForcesArray_function_type )(  ) const;
            beadForcesArray_function_type beadForcesArray_function_value( &::SireMove::RBWorkspaceJM::beadForcesArray );
            
            RBWorkspaceJM_exposer.def( 
                "beadForcesArray"
                , beadForcesArray_function_value );
        
        }
        { //::SireMove::RBWorkspaceJM::beadTorquesArray
        
            typedef ::QVector< SireMaths::Vector > ( ::SireMove::RBWorkspaceJM::*beadTorquesArray_function_type )(  ) const;
            beadTorquesArray_function_type beadTorquesArray_function_value( &::SireMove::RBWorkspaceJM::beadTorquesArray );
            
            RBWorkspaceJM_exposer.def( 
                "beadTorquesArray"
                , beadTorquesArray_function_value );
        
        }
        { //::SireMove::RBWorkspaceJM::beadingProperty
        
            typedef ::SireBase::PropertyName ( ::SireMove::RBWorkspaceJM::*beadingProperty_function_type )(  ) const;
            beadingProperty_function_type beadingProperty_function_value( &::SireMove::RBWorkspaceJM::beadingProperty );
            
            RBWorkspaceJM_exposer.def( 
                "beadingProperty"
                , beadingProperty_function_value );
        
        }
        { //::SireMove::RBWorkspaceJM::calculateForces
        
            typedef bool ( ::SireMove::RBWorkspaceJM::*calculateForces_function_type )( ::SireCAS::Symbol const & ) ;
            calculateForces_function_type calculateForces_function_value( &::SireMove::RBWorkspaceJM::calculateForces );
            
            RBWorkspaceJM_exposer.def( 
                "calculateForces"
                , calculateForces_function_value
                , ( bp::arg("nrg_component") ) );
        
        }
        { //::SireMove::RBWorkspaceJM::commitCoordinates
        
            typedef void ( ::SireMove::RBWorkspaceJM::*commitCoordinates_function_type )(  ) ;
            commitCoordinates_function_type commitCoordinates_function_value( &::SireMove::RBWorkspaceJM::commitCoordinates );
            
            RBWorkspaceJM_exposer.def( 
                "commitCoordinates"
                , commitCoordinates_function_value );
        
        }
        { //::SireMove::RBWorkspaceJM::commitCoordinatesAndVelocities
        
            typedef void ( ::SireMove::RBWorkspaceJM::*commitCoordinatesAndVelocities_function_type )(  ) ;
            commitCoordinatesAndVelocities_function_type commitCoordinatesAndVelocities_function_value( &::SireMove::RBWorkspaceJM::commitCoordinatesAndVelocities );
            
            RBWorkspaceJM_exposer.def( 
                "commitCoordinatesAndVelocities"
                , commitCoordinatesAndVelocities_function_value );
        
        }
        { //::SireMove::RBWorkspaceJM::commitVelocities
        
            typedef void ( ::SireMove::RBWorkspaceJM::*commitVelocities_function_type )(  ) ;
            commitVelocities_function_type commitVelocities_function_value( &::SireMove::RBWorkspaceJM::commitVelocities );
            
            RBWorkspaceJM_exposer.def( 
                "commitVelocities"
                , commitVelocities_function_value );
        
        }
        { //::SireMove::RBWorkspaceJM::kineticEnergy
        
            typedef ::SireUnits::Dimension::MolarEnergy ( ::SireMove::RBWorkspaceJM::*kineticEnergy_function_type )(  ) const;
            kineticEnergy_function_type kineticEnergy_function_value( &::SireMove::RBWorkspaceJM::kineticEnergy );
            
            RBWorkspaceJM_exposer.def( 
                "kineticEnergy"
                , kineticEnergy_function_value );
        
        }
        { //::SireMove::RBWorkspaceJM::kineticEnergy
        
            typedef ::SireUnits::Dimension::MolarEnergy ( ::SireMove::RBWorkspaceJM::*kineticEnergy_function_type )( ::SireMol::MolNum ) const;
            kineticEnergy_function_type kineticEnergy_function_value( &::SireMove::RBWorkspaceJM::kineticEnergy );
            
            RBWorkspaceJM_exposer.def( 
                "kineticEnergy"
                , kineticEnergy_function_value
                , ( bp::arg("molnum") ) );
        
        }
        { //::SireMove::RBWorkspaceJM::kineticEnergy
        
            typedef ::SireUnits::Dimension::MolarEnergy ( ::SireMove::RBWorkspaceJM::*kineticEnergy_function_type )( ::SireMol::MoleculeView const & ) const;
            kineticEnergy_function_type kineticEnergy_function_value( &::SireMove::RBWorkspaceJM::kineticEnergy );
            
            RBWorkspaceJM_exposer.def( 
                "kineticEnergy"
                , kineticEnergy_function_value
                , ( bp::arg("molview") ) );
        
        }
        { //::SireMove::RBWorkspaceJM::nAtoms
        
            typedef int ( ::SireMove::RBWorkspaceJM::*nAtoms_function_type )( int ) const;
            nAtoms_function_type nAtoms_function_value( &::SireMove::RBWorkspaceJM::nAtoms );
            
            RBWorkspaceJM_exposer.def( 
                "nAtoms"
                , nAtoms_function_value
                , ( bp::arg("ibead") ) );
        
        }
        { //::SireMove::RBWorkspaceJM::nBeads
        
            typedef int ( ::SireMove::RBWorkspaceJM::*nBeads_function_type )(  ) const;
            nBeads_function_type nBeads_function_value( &::SireMove::RBWorkspaceJM::nBeads );
            
            RBWorkspaceJM_exposer.def( 
                "nBeads"
                , nBeads_function_value );
        
        }
        RBWorkspaceJM_exposer.def( bp::self != bp::self );
        { //::SireMove::RBWorkspaceJM::operator=
        
            typedef ::SireMove::RBWorkspaceJM & ( ::SireMove::RBWorkspaceJM::*assign_function_type )( ::SireMove::RBWorkspaceJM const & ) ;
            assign_function_type assign_function_value( &::SireMove::RBWorkspaceJM::operator= );
            
            RBWorkspaceJM_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        RBWorkspaceJM_exposer.def( bp::self == bp::self );
        { //::SireMove::RBWorkspaceJM::regenerateVelocities
        
            typedef void ( ::SireMove::RBWorkspaceJM::*regenerateVelocities_function_type )( ::SireMove::VelocityGenerator const & ) ;
            regenerateVelocities_function_type regenerateVelocities_function_value( &::SireMove::RBWorkspaceJM::regenerateVelocities );
            
            RBWorkspaceJM_exposer.def( 
                "regenerateVelocities"
                , regenerateVelocities_function_value
                , ( bp::arg("generator") ) );
        
        }
        { //::SireMove::RBWorkspaceJM::setSystem
        
            typedef bool ( ::SireMove::RBWorkspaceJM::*setSystem_function_type )( ::SireSystem::System const & ) ;
            setSystem_function_type setSystem_function_value( &::SireMove::RBWorkspaceJM::setSystem );
            
            RBWorkspaceJM_exposer.def( 
                "setSystem"
                , setSystem_function_value
                , ( bp::arg("system") ) );
        
        }
        { //::SireMove::RBWorkspaceJM::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireMove::RBWorkspaceJM::typeName );
            
            RBWorkspaceJM_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        RBWorkspaceJM_exposer.staticmethod( "typeName" );
        RBWorkspaceJM_exposer.def( "__copy__", &__copy__);
        RBWorkspaceJM_exposer.def( "__deepcopy__", &__copy__);
        RBWorkspaceJM_exposer.def( "clone", &__copy__);
        RBWorkspaceJM_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireMove::RBWorkspaceJM >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        RBWorkspaceJM_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireMove::RBWorkspaceJM >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        RBWorkspaceJM_exposer.def( "__str__", &__str__< ::SireMove::RBWorkspaceJM > );
        RBWorkspaceJM_exposer.def( "__repr__", &__str__< ::SireMove::RBWorkspaceJM > );
    }

}
