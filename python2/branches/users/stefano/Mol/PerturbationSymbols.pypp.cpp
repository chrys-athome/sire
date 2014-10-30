// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "Helpers/clone_const_reference.hpp"
#include "PerturbationSymbols.pypp.hpp"

namespace bp = boost::python;

#include "SireCAS/identities.h"

#include "SireCAS/values.h"

#include "SireStream/datastream.h"

#include "SireStream/shareddatastream.h"

#include "geometryperturbation.h"

#include "molecule.h"

#include "moleditor.h"

#include "mover.hpp"

#include "perturbation.h"

#include "perturbation.h"

SireMol::PerturbationSymbols __copy__(const SireMol::PerturbationSymbols &other){ return SireMol::PerturbationSymbols(other); }

const char* pvt_get_name(const SireMol::PerturbationSymbols&){ return "SireMol::PerturbationSymbols";}

void register_PerturbationSymbols_class(){

    { //::SireMol::PerturbationSymbols
        typedef bp::class_< SireMol::PerturbationSymbols > PerturbationSymbols_exposer_t;
        PerturbationSymbols_exposer_t PerturbationSymbols_exposer = PerturbationSymbols_exposer_t( "PerturbationSymbols", bp::init< >() );
        bp::scope PerturbationSymbols_scope( PerturbationSymbols_exposer );
        { //::SireMol::PerturbationSymbols::final
        
            typedef ::SireCAS::Symbol const & ( ::SireMol::PerturbationSymbols::*final_function_type )(  ) const;
            final_function_type final_function_value( &::SireMol::PerturbationSymbols::final );
            
            PerturbationSymbols_exposer.def( 
                "final"
                , final_function_value
                , bp::return_value_policy<bp::clone_const_reference>() );
        
        }
        { //::SireMol::PerturbationSymbols::initial
        
            typedef ::SireCAS::Symbol const & ( ::SireMol::PerturbationSymbols::*initial_function_type )(  ) const;
            initial_function_type initial_function_value( &::SireMol::PerturbationSymbols::initial );
            
            PerturbationSymbols_exposer.def( 
                "initial"
                , initial_function_value
                , bp::return_value_policy<bp::clone_const_reference>() );
        
        }
        { //::SireMol::PerturbationSymbols::lambda
        
            typedef ::SireCAS::Symbol const & ( ::SireMol::PerturbationSymbols::*Lambda_function_type )(  ) const;
            Lambda_function_type Lambda_function_value( &::SireMol::PerturbationSymbols::lambda );
            
            PerturbationSymbols_exposer.def( 
                "Lambda"
                , Lambda_function_value
                , bp::return_value_policy<bp::clone_const_reference>() );
        
        }
        PerturbationSymbols_exposer.def( "__copy__", &__copy__);
        PerturbationSymbols_exposer.def( "__deepcopy__", &__copy__);
        PerturbationSymbols_exposer.def( "clone", &__copy__);
        PerturbationSymbols_exposer.def( "__str__", &pvt_get_name);
        PerturbationSymbols_exposer.def( "__repr__", &pvt_get_name);
    }

}
