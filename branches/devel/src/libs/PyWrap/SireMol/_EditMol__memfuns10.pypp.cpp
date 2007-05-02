// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "_EditMol__memfuns10.pypp.hpp"
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

void register_EditMol_memfuns10( EditMol_exposer_t& EditMol_exposer ){

    { //::SireMol::EditMol::residueNumbers
    
        typedef ::QVector<SireMol::ResNum> ( ::SireMol::EditMol::*residueNumbers_function_type )( ::SireMol::CutGroupNum ) const;
        
        EditMol_exposer.def( 
            "residueNumbers"
            , residueNumbers_function_type( &::SireMol::EditMol::residueNumbers )
            , ( bp::arg("cgnum") ) );
    
    }

    { //::SireMol::EditMol::residues
    
        typedef ::QHash<SireMol::ResNum,SireMol::EditRes> ( ::SireMol::EditMol::*residues_function_type )(  ) const;
        
        EditMol_exposer.def( 
            "residues"
            , residues_function_type( &::SireMol::EditMol::residues ) );
    
    }

    { //::SireMol::EditMol::residues
    
        typedef ::QHash<SireMol::ResNum,SireMol::EditRes> ( ::SireMol::EditMol::*residues_function_type )( ::QSet<SireMol::ResNum> const & ) const;
        
        EditMol_exposer.def( 
            "residues"
            , residues_function_type( &::SireMol::EditMol::residues )
            , ( bp::arg("resnums") ) );
    
    }

    { //::SireMol::EditMol::residuesBondedTo
    
        typedef ::QHash<SireMol::ResNum,SireMol::EditRes> ( ::SireMol::EditMol::*residuesBondedTo_function_type )( ::SireMol::ResNum ) const;
        
        EditMol_exposer.def( 
            "residuesBondedTo"
            , residuesBondedTo_function_type( &::SireMol::EditMol::residuesBondedTo )
            , ( bp::arg("resnum") ) );
    
    }

    { //::SireMol::EditMol::residuesBondedTo
    
        typedef ::QHash<SireMol::ResNum,SireMol::EditRes> ( ::SireMol::EditMol::*residuesBondedTo_function_type )( ::SireMol::ResID ) const;
        
        EditMol_exposer.def( 
            "residuesBondedTo"
            , residuesBondedTo_function_type( &::SireMol::EditMol::residuesBondedTo )
            , ( bp::arg("resid") ) );
    
    }

    { //::SireMol::EditMol::rotate
    
        typedef void ( ::SireMol::EditMol::*rotate_function_type )( ::SireMaths::Quaternion const &,::SireMaths::Vector const & ) ;
        
        EditMol_exposer.def( 
            "rotate"
            , rotate_function_type( &::SireMol::EditMol::rotate )
            , ( bp::arg("quat"), bp::arg("point") ) );
    
    }

    { //::SireMol::EditMol::rotate
    
        typedef void ( ::SireMol::EditMol::*rotate_function_type )( ::SireMol::AtomIDGroup const &,::SireMaths::Quaternion const &,::SireMaths::Vector const & ) ;
        
        EditMol_exposer.def( 
            "rotate"
            , rotate_function_type( &::SireMol::EditMol::rotate )
            , ( bp::arg("group"), bp::arg("quat"), bp::arg("point") ) );
    
    }

    { //::SireMol::EditMol::rotate
    
        typedef void ( ::SireMol::EditMol::*rotate_function_type )( ::SireMol::AtomIndex const &,::SireMaths::Quaternion const &,::SireMaths::Vector const & ) ;
        
        EditMol_exposer.def( 
            "rotate"
            , rotate_function_type( &::SireMol::EditMol::rotate )
            , ( bp::arg("atom"), bp::arg("quat"), bp::arg("point") ) );
    
    }

    { //::SireMol::EditMol::rotate
    
        typedef void ( ::SireMol::EditMol::*rotate_function_type )( ::QSet<SireMol::AtomIndex> const &,::SireMaths::Quaternion const &,::SireMaths::Vector const & ) ;
        
        EditMol_exposer.def( 
            "rotate"
            , rotate_function_type( &::SireMol::EditMol::rotate )
            , ( bp::arg("atoms"), bp::arg("quat"), bp::arg("point") ) );
    
    }

    { //::SireMol::EditMol::rotate
    
        typedef void ( ::SireMol::EditMol::*rotate_function_type )( ::SireMol::ResNum,::QStringList const &,::SireMaths::Quaternion const &,::SireMaths::Vector const & ) ;
        
        EditMol_exposer.def( 
            "rotate"
            , rotate_function_type( &::SireMol::EditMol::rotate )
            , ( bp::arg("resnum"), bp::arg("atoms"), bp::arg("quat"), bp::arg("point") ) );
    
    }

    { //::SireMol::EditMol::rotate
    
        typedef void ( ::SireMol::EditMol::*rotate_function_type )( ::SireMol::ResNum,::SireMaths::Quaternion const &,::SireMaths::Vector const & ) ;
        
        EditMol_exposer.def( 
            "rotate"
            , rotate_function_type( &::SireMol::EditMol::rotate )
            , ( bp::arg("resnum"), bp::arg("quat"), bp::arg("point") ) );
    
    }

    { //::SireMol::EditMol::rotate
    
        typedef void ( ::SireMol::EditMol::*rotate_function_type )( ::QSet<SireMol::ResNum> const &,::SireMaths::Quaternion const &,::SireMaths::Vector const & ) ;
        
        EditMol_exposer.def( 
            "rotate"
            , rotate_function_type( &::SireMol::EditMol::rotate )
            , ( bp::arg("resnums"), bp::arg("quat"), bp::arg("point") ) );
    
    }

    { //::SireMol::EditMol::rotate
    
        typedef void ( ::SireMol::EditMol::*rotate_function_type )( ::SireMol::ResID,::QStringList const &,::SireMaths::Quaternion const &,::SireMaths::Vector const & ) ;
        
        EditMol_exposer.def( 
            "rotate"
            , rotate_function_type( &::SireMol::EditMol::rotate )
            , ( bp::arg("resid"), bp::arg("atoms"), bp::arg("quat"), bp::arg("point") ) );
    
    }

    { //::SireMol::EditMol::rotate
    
        typedef void ( ::SireMol::EditMol::*rotate_function_type )( ::SireMol::ResID,::SireMaths::Quaternion const &,::SireMaths::Vector const & ) ;
        
        EditMol_exposer.def( 
            "rotate"
            , rotate_function_type( &::SireMol::EditMol::rotate )
            , ( bp::arg("resid"), bp::arg("quat"), bp::arg("point") ) );
    
    }

    { //::SireMol::EditMol::rotate
    
        typedef void ( ::SireMol::EditMol::*rotate_function_type )( ::QSet<SireMol::ResID> const &,::SireMaths::Quaternion const &,::SireMaths::Vector const & ) ;
        
        EditMol_exposer.def( 
            "rotate"
            , rotate_function_type( &::SireMol::EditMol::rotate )
            , ( bp::arg("resids"), bp::arg("quat"), bp::arg("point") ) );
    
    }

    { //::SireMol::EditMol::rotate
    
        typedef void ( ::SireMol::EditMol::*rotate_function_type )( ::SireMol::CutGroupNum,::SireMaths::Quaternion const &,::SireMaths::Vector const & ) ;
        
        EditMol_exposer.def( 
            "rotate"
            , rotate_function_type( &::SireMol::EditMol::rotate )
            , ( bp::arg("cgnum"), bp::arg("quat"), bp::arg("point") ) );
    
    }

    { //::SireMol::EditMol::rotate
    
        typedef void ( ::SireMol::EditMol::*rotate_function_type )( ::QSet<SireMol::CutGroupNum> const &,::SireMaths::Quaternion const &,::SireMaths::Vector const & ) ;
        
        EditMol_exposer.def( 
            "rotate"
            , rotate_function_type( &::SireMol::EditMol::rotate )
            , ( bp::arg("cgnums"), bp::arg("quat"), bp::arg("point") ) );
    
    }

    { //::SireMol::EditMol::rotate
    
        typedef void ( ::SireMol::EditMol::*rotate_function_type )( ::SireMol::CutGroupID,::SireMaths::Quaternion const &,::SireMaths::Vector const & ) ;
        
        EditMol_exposer.def( 
            "rotate"
            , rotate_function_type( &::SireMol::EditMol::rotate )
            , ( bp::arg("cgid"), bp::arg("quat"), bp::arg("point") ) );
    
    }

    { //::SireMol::EditMol::rotate
    
        typedef void ( ::SireMol::EditMol::*rotate_function_type )( ::QSet<SireMol::CutGroupID> const &,::SireMaths::Quaternion const &,::SireMaths::Vector const & ) ;
        
        EditMol_exposer.def( 
            "rotate"
            , rotate_function_type( &::SireMol::EditMol::rotate )
            , ( bp::arg("cgids"), bp::arg("quat"), bp::arg("point") ) );
    
    }

    { //::SireMol::EditMol::rotate
    
        typedef void ( ::SireMol::EditMol::*rotate_function_type )( ::SireMaths::Matrix const &,::SireMaths::Vector const & ) ;
        
        EditMol_exposer.def( 
            "rotate"
            , rotate_function_type( &::SireMol::EditMol::rotate )
            , ( bp::arg("matrix"), bp::arg("point") ) );
    
    }

}
