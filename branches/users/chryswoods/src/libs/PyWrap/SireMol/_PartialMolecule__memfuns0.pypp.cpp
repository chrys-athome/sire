// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "_PartialMolecule__memfuns0.pypp.hpp"
#include "boost/python.hpp"
#include "siremol_headers.h"
#include "SireMaths/angle.h"
#include "SireMaths/quaternion.h"
#include "SireMaths/matrix.h"
#include "SireMaths/triangle.h"
#include "SireMaths/line.h"
#include "SireMaths/torsion.h"

namespace bp = boost::python;

void register_PartialMolecule_memfuns0( PartialMolecule_exposer_t& PartialMolecule_exposer ){

    { //::SireMol::PartialMolecule::ID
    
        typedef ::SireMol::MoleculeID ( ::SireMol::PartialMolecule::*ID_function_type )(  ) const;
        
        PartialMolecule_exposer.def( 
            "ID"
            , ID_function_type( &::SireMol::PartialMolecule::ID ) );
    
    }

    { //::SireMol::PartialMolecule::add
    
        typedef bool ( ::SireMol::PartialMolecule::*add_function_type )( ::SireMol::AtomSelection const & ) ;
        
        PartialMolecule_exposer.def( 
            "add"
            , add_function_type( &::SireMol::PartialMolecule::add )
            , ( bp::arg("atoms") ) );
    
    }

    { //::SireMol::PartialMolecule::applyMask
    
        typedef void ( ::SireMol::PartialMolecule::*applyMask_function_type )( ::QSet<SireMol::CutGroupID> const & ) ;
        
        PartialMolecule_exposer.def( 
            "applyMask"
            , applyMask_function_type( &::SireMol::PartialMolecule::applyMask )
            , ( bp::arg("cgids") ) );
    
    }

    { //::SireMol::PartialMolecule::applyMask
    
        typedef void ( ::SireMol::PartialMolecule::*applyMask_function_type )( ::QSet<SireMol::ResNum> const & ) ;
        
        PartialMolecule_exposer.def( 
            "applyMask"
            , applyMask_function_type( &::SireMol::PartialMolecule::applyMask )
            , ( bp::arg("resnums") ) );
    
    }

    { //::SireMol::PartialMolecule::applyMask
    
        typedef void ( ::SireMol::PartialMolecule::*applyMask_function_type )( ::SireMol::AtomSelection const & ) ;
        
        PartialMolecule_exposer.def( 
            "applyMask"
            , applyMask_function_type( &::SireMol::PartialMolecule::applyMask )
            , ( bp::arg("other") ) );
    
    }

    { //::SireMol::PartialMolecule::atom
    
        typedef ::SireMol::NewAtom ( ::SireMol::PartialMolecule::*atom_function_type )( ::SireMol::IDMolAtom const & ) const;
        
        PartialMolecule_exposer.def( 
            "atom"
            , atom_function_type( &::SireMol::PartialMolecule::atom )
            , ( bp::arg("atomid") ) );
    
    }

    { //::SireMol::PartialMolecule::change
    
        typedef bool ( ::SireMol::PartialMolecule::*change_function_type )( ::SireMol::PartialMolecule const & ) ;
        
        PartialMolecule_exposer.def( 
            "change"
            , change_function_type( &::SireMol::PartialMolecule::change )
            , ( bp::arg("molecule") ) );
    
    }

    { //::SireMol::PartialMolecule::contains
    
        typedef bool ( ::SireMol::PartialMolecule::*contains_function_type )( ::SireMol::AtomSelection const & ) const;
        
        PartialMolecule_exposer.def( 
            "contains"
            , contains_function_type( &::SireMol::PartialMolecule::contains )
            , ( bp::arg("other") ) );
    
    }

    { //::SireMol::PartialMolecule::deselect
    
        typedef void ( ::SireMol::PartialMolecule::*deselect_function_type )( ::SireMol::CGAtomID const & ) ;
        
        PartialMolecule_exposer.def( 
            "deselect"
            , deselect_function_type( &::SireMol::PartialMolecule::deselect )
            , ( bp::arg("cgatomid") ) );
    
    }

    { //::SireMol::PartialMolecule::deselect
    
        typedef void ( ::SireMol::PartialMolecule::*deselect_function_type )( ::SireMol::IDMolAtom const & ) ;
        
        PartialMolecule_exposer.def( 
            "deselect"
            , deselect_function_type( &::SireMol::PartialMolecule::deselect )
            , ( bp::arg("atomid") ) );
    
    }

    { //::SireMol::PartialMolecule::deselectAll
    
        typedef void ( ::SireMol::PartialMolecule::*deselectAll_function_type )(  ) ;
        
        PartialMolecule_exposer.def( 
            "deselectAll"
            , deselectAll_function_type( &::SireMol::PartialMolecule::deselectAll ) );
    
    }

    { //::SireMol::PartialMolecule::deselectAll
    
        typedef void ( ::SireMol::PartialMolecule::*deselectAll_function_type )( ::SireMol::CutGroupID ) ;
        
        PartialMolecule_exposer.def( 
            "deselectAll"
            , deselectAll_function_type( &::SireMol::PartialMolecule::deselectAll )
            , ( bp::arg("cgid") ) );
    
    }

    { //::SireMol::PartialMolecule::deselectAll
    
        typedef void ( ::SireMol::PartialMolecule::*deselectAll_function_type )( ::SireMol::ResNum ) ;
        
        PartialMolecule_exposer.def( 
            "deselectAll"
            , deselectAll_function_type( &::SireMol::PartialMolecule::deselectAll )
            , ( bp::arg("resnum") ) );
    
    }

    { //::SireMol::PartialMolecule::deselectAll
    
        typedef void ( ::SireMol::PartialMolecule::*deselectAll_function_type )( ::SireMol::AtomSelection const & ) ;
        
        PartialMolecule_exposer.def( 
            "deselectAll"
            , deselectAll_function_type( &::SireMol::PartialMolecule::deselectAll )
            , ( bp::arg("selection") ) );
    
    }

    { //::SireMol::PartialMolecule::info
    
        typedef ::SireMol::MoleculeInfo const & ( ::SireMol::PartialMolecule::*info_function_type )(  ) const;
        
        PartialMolecule_exposer.def( 
            "info"
            , info_function_type( &::SireMol::PartialMolecule::info )
            , bp::return_value_policy< bp::copy_const_reference >() );
    
    }

    { //::SireMol::PartialMolecule::info
    
        typedef ::SireMol::ResidueInfo const & ( ::SireMol::PartialMolecule::*info_function_type )( ::SireMol::ResNum ) const;
        
        PartialMolecule_exposer.def( 
            "info"
            , info_function_type( &::SireMol::PartialMolecule::info )
            , ( bp::arg("resnum") )
            , bp::return_value_policy< bp::copy_const_reference >() );
    
    }

    { //::SireMol::PartialMolecule::info
    
        typedef ::SireMol::ResidueInfo const & ( ::SireMol::PartialMolecule::*info_function_type )( ::SireMol::ResID ) const;
        
        PartialMolecule_exposer.def( 
            "info"
            , info_function_type( &::SireMol::PartialMolecule::info )
            , ( bp::arg("resid") )
            , bp::return_value_policy< bp::copy_const_reference >() );
    
    }

    { //::SireMol::PartialMolecule::intersect
    
        typedef ::SireMol::PartialMolecule ( ::SireMol::PartialMolecule::*intersect_function_type )( ::SireMol::AtomSelection const & ) const;
        
        PartialMolecule_exposer.def( 
            "intersect"
            , intersect_function_type( &::SireMol::PartialMolecule::intersect )
            , ( bp::arg("other") ) );
    
    }

    { //::SireMol::PartialMolecule::intersects
    
        typedef bool ( ::SireMol::PartialMolecule::*intersects_function_type )( ::SireMol::AtomSelection const & ) const;
        
        PartialMolecule_exposer.def( 
            "intersects"
            , intersects_function_type( &::SireMol::PartialMolecule::intersects )
            , ( bp::arg("other") ) );
    
    }

    { //::SireMol::PartialMolecule::invert
    
        typedef void ( ::SireMol::PartialMolecule::*invert_function_type )(  ) ;
        
        PartialMolecule_exposer.def( 
            "invert"
            , invert_function_type( &::SireMol::PartialMolecule::invert ) );
    
    }

}
