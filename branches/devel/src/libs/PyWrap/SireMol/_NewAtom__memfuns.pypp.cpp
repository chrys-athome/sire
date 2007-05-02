// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "_NewAtom__memfuns.pypp.hpp"
#include "boost/python.hpp"
#include "siremol_headers.h"
#include "SireMol/moleculedata.h"
#include "SireMaths/angle.h"
#include "SireMaths/quaternion.h"
#include "SireMaths/matrix.h"
#include "SireMaths/triangle.h"
#include "SireMaths/line.h"
#include "SireMaths/torsion.h"
#include "SireVol/space.h"

namespace bp = boost::python;

void register_NewAtom_memfuns( NewAtom_exposer_t& NewAtom_exposer ){

    { //::SireMol::NewAtom::ID
    
        typedef ::SireMol::MoleculeID ( ::SireMol::NewAtom::*ID_function_type )(  ) const;
        
        NewAtom_exposer.def( 
            "ID"
            , ID_function_type( &::SireMol::NewAtom::ID ) );
    
    }

    { //::SireMol::NewAtom::bondedAtoms
    
        typedef ::QSet<SireMol::NewAtom> ( ::SireMol::NewAtom::*bondedAtoms_function_type )(  ) const;
        
        NewAtom_exposer.def( 
            "bondedAtoms"
            , bondedAtoms_function_type( &::SireMol::NewAtom::bondedAtoms ) );
    
    }

    { //::SireMol::NewAtom::bondedResidues
    
        typedef ::QSet<SireMol::Residue> ( ::SireMol::NewAtom::*bondedResidues_function_type )(  ) const;
        
        NewAtom_exposer.def( 
            "bondedResidues"
            , bondedResidues_function_type( &::SireMol::NewAtom::bondedResidues ) );
    
    }

    { //::SireMol::NewAtom::cgAtomID
    
        typedef ::SireMol::CGAtomID const & ( ::SireMol::NewAtom::*cgAtomID_function_type )(  ) const;
        
        NewAtom_exposer.def( 
            "cgAtomID"
            , cgAtomID_function_type( &::SireMol::NewAtom::cgAtomID )
            , bp::return_value_policy< bp::copy_const_reference >() );
    
    }

    { //::SireMol::NewAtom::coordinates
    
        typedef ::SireMaths::Vector ( ::SireMol::NewAtom::*coordinates_function_type )(  ) const;
        
        NewAtom_exposer.def( 
            "coordinates"
            , coordinates_function_type( &::SireMol::NewAtom::coordinates ) );
    
    }

    { //::SireMol::NewAtom::element
    
        typedef ::SireMol::Element ( ::SireMol::NewAtom::*element_function_type )(  ) const;
        
        NewAtom_exposer.def( 
            "element"
            , element_function_type( &::SireMol::NewAtom::element ) );
    
    }

    { //::SireMol::NewAtom::idString
    
        typedef ::QString ( ::SireMol::NewAtom::*idString_function_type )(  ) const;
        
        NewAtom_exposer.def( 
            "idString"
            , idString_function_type( &::SireMol::NewAtom::idString ) );
    
    }

    { //::SireMol::NewAtom::info
    
        typedef ::SireMol::AtomInfo const & ( ::SireMol::NewAtom::*info_function_type )(  ) const;
        
        NewAtom_exposer.def( 
            "info"
            , info_function_type( &::SireMol::NewAtom::info )
            , bp::return_value_policy< bp::copy_const_reference >() );
    
    }

    { //::SireMol::NewAtom::molecule
    
        typedef ::SireMol::Molecule ( ::SireMol::NewAtom::*molecule_function_type )(  ) const;
        
        NewAtom_exposer.def( 
            "molecule"
            , molecule_function_type( &::SireMol::NewAtom::molecule ) );
    
    }

    { //::SireMol::NewAtom::name
    
        typedef ::QString ( ::SireMol::NewAtom::*name_function_type )(  ) const;
        
        NewAtom_exposer.def( 
            "name"
            , name_function_type( &::SireMol::NewAtom::name ) );
    
    }

    { //::SireMol::NewAtom::property
    
        typedef ::QVariant ( ::SireMol::NewAtom::*property_function_type )( ::QString const & ) const;
        
        NewAtom_exposer.def( 
            "property"
            , property_function_type( &::SireMol::NewAtom::property )
            , ( bp::arg("name") ) );
    
    }

    { //::SireMol::NewAtom::residue
    
        typedef ::SireMol::Residue ( ::SireMol::NewAtom::*residue_function_type )(  ) const;
        
        NewAtom_exposer.def( 
            "residue"
            , residue_function_type( &::SireMol::NewAtom::residue ) );
    
    }

    { //::SireMol::NewAtom::rotate
    
        typedef void ( ::SireMol::NewAtom::*rotate_function_type )( ::SireMaths::Quaternion const &,::SireMaths::Vector const & ) ;
        
        NewAtom_exposer.def( 
            "rotate"
            , rotate_function_type( &::SireMol::NewAtom::rotate )
            , ( bp::arg("quat"), bp::arg("point") ) );
    
    }

    { //::SireMol::NewAtom::rotate
    
        typedef void ( ::SireMol::NewAtom::*rotate_function_type )( ::SireMaths::Matrix const &,::SireMaths::Vector const & ) ;
        
        NewAtom_exposer.def( 
            "rotate"
            , rotate_function_type( &::SireMol::NewAtom::rotate )
            , ( bp::arg("rotmat"), bp::arg("point") ) );
    
    }

    { //::SireMol::NewAtom::setCoordinates
    
        typedef void ( ::SireMol::NewAtom::*setCoordinates_function_type )( double,double,double ) ;
        
        NewAtom_exposer.def( 
            "setCoordinates"
            , setCoordinates_function_type( &::SireMol::NewAtom::setCoordinates )
            , ( bp::arg("x"), bp::arg("y"), bp::arg("z") ) );
    
    }

    { //::SireMol::NewAtom::setCoordinates
    
        typedef void ( ::SireMol::NewAtom::*setCoordinates_function_type )( ::SireMaths::Vector const & ) ;
        
        NewAtom_exposer.def( 
            "setCoordinates"
            , setCoordinates_function_type( &::SireMol::NewAtom::setCoordinates )
            , ( bp::arg("newcoords") ) );
    
    }

    { //::SireMol::NewAtom::toString
    
        typedef ::QString ( ::SireMol::NewAtom::*toString_function_type )(  ) const;
        
        NewAtom_exposer.def( 
            "toString"
            , toString_function_type( &::SireMol::NewAtom::toString ) );
    
    }

    { //::SireMol::NewAtom::translate
    
        typedef void ( ::SireMol::NewAtom::*translate_function_type )( ::SireMaths::Vector const & ) ;
        
        NewAtom_exposer.def( 
            "translate"
            , translate_function_type( &::SireMol::NewAtom::translate )
            , ( bp::arg("delta") ) );
    
    }

    { //::SireMol::NewAtom::version
    
        typedef ::SireMol::MoleculeVersion const & ( ::SireMol::NewAtom::*version_function_type )(  ) const;
        
        NewAtom_exposer.def( 
            "version"
            , version_function_type( &::SireMol::NewAtom::version )
            , bp::return_value_policy< bp::copy_const_reference >() );
    
    }

    { //::SireMol::NewAtom::withinBondRadii
    
        typedef bool ( ::SireMol::NewAtom::*withinBondRadii_function_type )( ::SireMol::NewAtom const &,double ) const;
        
        NewAtom_exposer.def( 
            "withinBondRadii"
            , withinBondRadii_function_type( &::SireMol::NewAtom::withinBondRadii )
            , ( bp::arg("other"), bp::arg("err")=0 ) );
    
    }

}
