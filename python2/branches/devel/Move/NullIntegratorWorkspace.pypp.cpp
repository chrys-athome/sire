// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "NullIntegratorWorkspace.pypp.hpp"

namespace bp = boost::python;

#include "SireBase/quickcopy.hpp"

#include "SireMol/atomcoords.h"

#include "SireMol/errors.h"

#include "SireMol/molecule.h"

#include "SireMol/moleculeview.h"

#include "SireMol/moleditor.h"

#include "SireMol/partialmolecule.h"

#include "SireStream/datastream.h"

#include "SireStream/shareddatastream.h"

#include "SireSystem/system.h"

#include "integratorworkspace.h"

#include "velocitygenerator.h"

#include "integratorworkspace.h"

SireMove::NullIntegratorWorkspace __copy__(const SireMove::NullIntegratorWorkspace &other){ return SireMove::NullIntegratorWorkspace(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_NullIntegratorWorkspace_class(){

    { //::SireMove::NullIntegratorWorkspace
        typedef bp::class_< SireMove::NullIntegratorWorkspace, bp::bases< SireMove::IntegratorWorkspace, SireBase::Property > > NullIntegratorWorkspace_exposer_t;
        NullIntegratorWorkspace_exposer_t NullIntegratorWorkspace_exposer = NullIntegratorWorkspace_exposer_t( "NullIntegratorWorkspace" );
        bp::scope NullIntegratorWorkspace_scope( NullIntegratorWorkspace_exposer );
        NullIntegratorWorkspace_exposer.def( bp::init< >() );
        NullIntegratorWorkspace_exposer.def( bp::init< SireMove::NullIntegratorWorkspace const & >(( bp::arg("other") )) );
        { //::SireMove::NullIntegratorWorkspace::kineticEnergy
        
            typedef ::SireUnits::Dimension::MolarEnergy ( ::SireMove::NullIntegratorWorkspace::*kineticEnergy_function_type )(  ) const;
            kineticEnergy_function_type kineticEnergy_function_value( &::SireMove::NullIntegratorWorkspace::kineticEnergy );
            
            NullIntegratorWorkspace_exposer.def( 
                "kineticEnergy"
                , kineticEnergy_function_value );
        
        }
        { //::SireMove::NullIntegratorWorkspace::kineticEnergy
        
            typedef ::SireUnits::Dimension::MolarEnergy ( ::SireMove::NullIntegratorWorkspace::*kineticEnergy_function_type )( ::SireMol::MoleculeView const & ) const;
            kineticEnergy_function_type kineticEnergy_function_value( &::SireMove::NullIntegratorWorkspace::kineticEnergy );
            
            NullIntegratorWorkspace_exposer.def( 
                "kineticEnergy"
                , kineticEnergy_function_value
                , ( bp::arg("molview") ) );
        
        }
        NullIntegratorWorkspace_exposer.def( bp::self != bp::self );
        { //::SireMove::NullIntegratorWorkspace::operator=
        
            typedef ::SireMove::NullIntegratorWorkspace & ( ::SireMove::NullIntegratorWorkspace::*assign_function_type )( ::SireMove::NullIntegratorWorkspace const & ) ;
            assign_function_type assign_function_value( &::SireMove::NullIntegratorWorkspace::operator= );
            
            NullIntegratorWorkspace_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        NullIntegratorWorkspace_exposer.def( bp::self == bp::self );
        { //::SireMove::NullIntegratorWorkspace::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireMove::NullIntegratorWorkspace::typeName );
            
            NullIntegratorWorkspace_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        NullIntegratorWorkspace_exposer.staticmethod( "typeName" );
        NullIntegratorWorkspace_exposer.def( "__copy__", &__copy__);
        NullIntegratorWorkspace_exposer.def( "__deepcopy__", &__copy__);
        NullIntegratorWorkspace_exposer.def( "clone", &__copy__);
        NullIntegratorWorkspace_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireMove::NullIntegratorWorkspace >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        NullIntegratorWorkspace_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireMove::NullIntegratorWorkspace >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        NullIntegratorWorkspace_exposer.def( "__str__", &__str__< ::SireMove::NullIntegratorWorkspace > );
        NullIntegratorWorkspace_exposer.def( "__repr__", &__str__< ::SireMove::NullIntegratorWorkspace > );
    }

}
