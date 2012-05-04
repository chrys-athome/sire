// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "Helpers/clone_const_reference.hpp"
#include "StretchBendTorsionSymbols.pypp.hpp"

namespace bp = boost::python;

#include "SireError/errors.h"

#include "SireFF/errors.h"

#include "SireMol/cgidx.h"

#include "SireMol/molecule.h"

#include "SireMol/mover.hpp"

#include "SireMol/partialmolecule.h"

#include "SireVol/coordgroup.h"

#include "internalparameters.h"

#include "sireglobal.h"

#include "tostring.h"

#include <algorithm>

#include "internalparameters.h"

SireMM::StretchBendTorsionSymbols __copy__(const SireMM::StretchBendTorsionSymbols &other){ return SireMM::StretchBendTorsionSymbols(other); }

const char* pvt_get_name(const SireMM::StretchBendTorsionSymbols&){ return "SireMM::StretchBendTorsionSymbols";}

void register_StretchBendTorsionSymbols_class(){

    { //::SireMM::StretchBendTorsionSymbols
        typedef bp::class_< SireMM::StretchBendTorsionSymbols, bp::bases< SireMM::InternalSymbolsBase > > StretchBendTorsionSymbols_exposer_t;
        StretchBendTorsionSymbols_exposer_t StretchBendTorsionSymbols_exposer = StretchBendTorsionSymbols_exposer_t( "StretchBendTorsionSymbols" );
        bp::scope StretchBendTorsionSymbols_scope( StretchBendTorsionSymbols_exposer );
        StretchBendTorsionSymbols_exposer.def( bp::init< >() );
        { //::SireMM::StretchBendTorsionSymbols::phi
        
            typedef ::SireCAS::Symbol const & ( ::SireMM::StretchBendTorsionSymbols::*phi_function_type )(  ) const;
            phi_function_type phi_function_value( &::SireMM::StretchBendTorsionSymbols::phi );
            
            StretchBendTorsionSymbols_exposer.def( 
                "phi"
                , phi_function_value
                , bp::return_value_policy<bp::clone_const_reference>() );
        
        }
        { //::SireMM::StretchBendTorsionSymbols::r01
        
            typedef ::SireCAS::Symbol const & ( ::SireMM::StretchBendTorsionSymbols::*r01_function_type )(  ) const;
            r01_function_type r01_function_value( &::SireMM::StretchBendTorsionSymbols::r01 );
            
            StretchBendTorsionSymbols_exposer.def( 
                "r01"
                , r01_function_value
                , bp::return_value_policy<bp::clone_const_reference>() );
        
        }
        { //::SireMM::StretchBendTorsionSymbols::r03
        
            typedef ::SireCAS::Symbol const & ( ::SireMM::StretchBendTorsionSymbols::*r03_function_type )(  ) const;
            r03_function_type r03_function_value( &::SireMM::StretchBendTorsionSymbols::r03 );
            
            StretchBendTorsionSymbols_exposer.def( 
                "r03"
                , r03_function_value
                , bp::return_value_policy<bp::clone_const_reference>() );
        
        }
        { //::SireMM::StretchBendTorsionSymbols::r12
        
            typedef ::SireCAS::Symbol const & ( ::SireMM::StretchBendTorsionSymbols::*r12_function_type )(  ) const;
            r12_function_type r12_function_value( &::SireMM::StretchBendTorsionSymbols::r12 );
            
            StretchBendTorsionSymbols_exposer.def( 
                "r12"
                , r12_function_value
                , bp::return_value_policy<bp::clone_const_reference>() );
        
        }
        { //::SireMM::StretchBendTorsionSymbols::r32
        
            typedef ::SireCAS::Symbol const & ( ::SireMM::StretchBendTorsionSymbols::*r32_function_type )(  ) const;
            r32_function_type r32_function_value( &::SireMM::StretchBendTorsionSymbols::r32 );
            
            StretchBendTorsionSymbols_exposer.def( 
                "r32"
                , r32_function_value
                , bp::return_value_policy<bp::clone_const_reference>() );
        
        }
        { //::SireMM::StretchBendTorsionSymbols::theta012
        
            typedef ::SireCAS::Symbol const & ( ::SireMM::StretchBendTorsionSymbols::*theta012_function_type )(  ) const;
            theta012_function_type theta012_function_value( &::SireMM::StretchBendTorsionSymbols::theta012 );
            
            StretchBendTorsionSymbols_exposer.def( 
                "theta012"
                , theta012_function_value
                , bp::return_value_policy<bp::clone_const_reference>() );
        
        }
        { //::SireMM::StretchBendTorsionSymbols::theta321
        
            typedef ::SireCAS::Symbol const & ( ::SireMM::StretchBendTorsionSymbols::*theta321_function_type )(  ) const;
            theta321_function_type theta321_function_value( &::SireMM::StretchBendTorsionSymbols::theta321 );
            
            StretchBendTorsionSymbols_exposer.def( 
                "theta321"
                , theta321_function_value
                , bp::return_value_policy<bp::clone_const_reference>() );
        
        }
        StretchBendTorsionSymbols_exposer.def( "__copy__", &__copy__);
        StretchBendTorsionSymbols_exposer.def( "__deepcopy__", &__copy__);
        StretchBendTorsionSymbols_exposer.def( "clone", &__copy__);
        StretchBendTorsionSymbols_exposer.def( "__str__", &pvt_get_name);
        StretchBendTorsionSymbols_exposer.def( "__repr__", &pvt_get_name);
    }

}
