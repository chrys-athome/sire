// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "_EditRes__memfuns6.pypp.hpp"
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

void register_EditRes_memfuns6( EditRes_exposer_t& EditRes_exposer ){

    { //::SireMol::EditRes::setAll
    
        typedef void ( ::SireMol::EditRes::*setAll_function_type )( ::SireMol::Dihedral const &,::SireMaths::Angle const &,::QSet<SireMol::AtomIndex> const & ) ;
        
        EditRes_exposer.def( 
            "setAll"
            , setAll_function_type( &::SireMol::EditRes::setAll )
            , ( bp::arg("dihedral"), bp::arg("ang"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) ) );
    
    }

    { //::SireMol::EditRes::setAll
    
        typedef void ( ::SireMol::EditRes::*setAll_function_type )( ::SireMol::Dihedral const &,::SireMaths::Angle const &,::SireMol::WeightFunction const &,::QSet<SireMol::AtomIndex> const & ) ;
        
        EditRes_exposer.def( 
            "setAll"
            , setAll_function_type( &::SireMol::EditRes::setAll )
            , ( bp::arg("dihedral"), bp::arg("ang"), bp::arg("func"), bp::arg("anchors")=::QSet<SireMol::AtomIndex>( ) ) );
    
    }

    { //::SireMol::EditRes::setCoordinates
    
        typedef void ( ::SireMol::EditRes::*setCoordinates_function_type )( ::QVector<SireMaths::Vector> const & ) ;
        
        EditRes_exposer.def( 
            "setCoordinates"
            , setCoordinates_function_type( &::SireMol::EditRes::setCoordinates )
            , ( bp::arg("newcoords") ) );
    
    }

    { //::SireMol::EditRes::setCoordinates
    
        typedef void ( ::SireMol::EditRes::*setCoordinates_function_type )( ::SireMol::AtomID,::SireMaths::Vector const & ) ;
        
        EditRes_exposer.def( 
            "setCoordinates"
            , setCoordinates_function_type( &::SireMol::EditRes::setCoordinates )
            , ( bp::arg("atomid"), bp::arg("newcoords") ) );
    
    }

    { //::SireMol::EditRes::setCoordinates
    
        typedef void ( ::SireMol::EditRes::*setCoordinates_function_type )( ::QHash<SireMol::AtomID,SireMaths::Vector> const & ) ;
        
        EditRes_exposer.def( 
            "setCoordinates"
            , setCoordinates_function_type( &::SireMol::EditRes::setCoordinates )
            , ( bp::arg("newcoords") ) );
    
    }

    { //::SireMol::EditRes::setCoordinates
    
        typedef void ( ::SireMol::EditRes::*setCoordinates_function_type )( ::QString const &,::SireMaths::Vector const & ) ;
        
        EditRes_exposer.def( 
            "setCoordinates"
            , setCoordinates_function_type( &::SireMol::EditRes::setCoordinates )
            , ( bp::arg("atomname"), bp::arg("newcoords") ) );
    
    }

    { //::SireMol::EditRes::setCoordinates
    
        typedef void ( ::SireMol::EditRes::*setCoordinates_function_type )( ::QHash<QString,SireMaths::Vector> const & ) ;
        
        EditRes_exposer.def( 
            "setCoordinates"
            , setCoordinates_function_type( &::SireMol::EditRes::setCoordinates )
            , ( bp::arg("newcoords") ) );
    
    }

    { //::SireMol::EditRes::setCoordinates
    
        typedef void ( ::SireMol::EditRes::*setCoordinates_function_type )( ::SireMol::AtomIndex const &,::SireMaths::Vector const & ) ;
        
        EditRes_exposer.def( 
            "setCoordinates"
            , setCoordinates_function_type( &::SireMol::EditRes::setCoordinates )
            , ( bp::arg("atom"), bp::arg("newcoords") ) );
    
    }

    { //::SireMol::EditRes::setCoordinates
    
        typedef void ( ::SireMol::EditRes::*setCoordinates_function_type )( ::QHash<SireMol::AtomIndex,SireMaths::Vector> const & ) ;
        
        EditRes_exposer.def( 
            "setCoordinates"
            , setCoordinates_function_type( &::SireMol::EditRes::setCoordinates )
            , ( bp::arg("newcoords") ) );
    
    }

    { //::SireMol::EditRes::setName
    
        typedef void ( ::SireMol::EditRes::*setName_function_type )( ::QString ) ;
        
        EditRes_exposer.def( 
            "setName"
            , setName_function_type( &::SireMol::EditRes::setName )
            , ( bp::arg("name") ) );
    
    }

    { //::SireMol::EditRes::setNumber
    
        typedef void ( ::SireMol::EditRes::*setNumber_function_type )( ::SireMol::ResNum ) ;
        
        EditRes_exposer.def( 
            "setNumber"
            , setNumber_function_type( &::SireMol::EditRes::setNumber )
            , ( bp::arg("newnum") ) );
    
    }

    { //::SireMol::EditRes::translate
    
        typedef void ( ::SireMol::EditRes::*translate_function_type )( ::SireMaths::Vector const & ) ;
        
        EditRes_exposer.def( 
            "translate"
            , translate_function_type( &::SireMol::EditRes::translate )
            , ( bp::arg("delta") ) );
    
    }

    { //::SireMol::EditRes::translate
    
        typedef void ( ::SireMol::EditRes::*translate_function_type )( ::SireMol::AtomID,::SireMaths::Vector const & ) ;
        
        EditRes_exposer.def( 
            "translate"
            , translate_function_type( &::SireMol::EditRes::translate )
            , ( bp::arg("atomid"), bp::arg("delta") ) );
    
    }

    { //::SireMol::EditRes::translate
    
        typedef void ( ::SireMol::EditRes::*translate_function_type )( ::QSet<SireMol::AtomID> const &,::SireMaths::Vector const & ) ;
        
        EditRes_exposer.def( 
            "translate"
            , translate_function_type( &::SireMol::EditRes::translate )
            , ( bp::arg("atomids"), bp::arg("delta") ) );
    
    }

    { //::SireMol::EditRes::translate
    
        typedef void ( ::SireMol::EditRes::*translate_function_type )( ::QString const &,::SireMaths::Vector const & ) ;
        
        EditRes_exposer.def( 
            "translate"
            , translate_function_type( &::SireMol::EditRes::translate )
            , ( bp::arg("atom"), bp::arg("delta") ) );
    
    }

    { //::SireMol::EditRes::translate
    
        typedef void ( ::SireMol::EditRes::*translate_function_type )( ::QStringList const &,::SireMaths::Vector const & ) ;
        
        EditRes_exposer.def( 
            "translate"
            , translate_function_type( &::SireMol::EditRes::translate )
            , ( bp::arg("atoms"), bp::arg("delta") ) );
    
    }

    { //::SireMol::EditRes::translate
    
        typedef void ( ::SireMol::EditRes::*translate_function_type )( ::SireMol::AtomIndex const &,::SireMaths::Vector const & ) ;
        
        EditRes_exposer.def( 
            "translate"
            , translate_function_type( &::SireMol::EditRes::translate )
            , ( bp::arg("atom"), bp::arg("delta") ) );
    
    }

    { //::SireMol::EditRes::translate
    
        typedef void ( ::SireMol::EditRes::*translate_function_type )( ::QSet<SireMol::AtomIndex> const &,::SireMaths::Vector const & ) ;
        
        EditRes_exposer.def( 
            "translate"
            , translate_function_type( &::SireMol::EditRes::translate )
            , ( bp::arg("atoms"), bp::arg("delta") ) );
    
    }

}
