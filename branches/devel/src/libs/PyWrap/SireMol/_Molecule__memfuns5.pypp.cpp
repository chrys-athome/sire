// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "_Molecule__memfuns5.pypp.hpp"
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

void register_Molecule_memfuns5( Molecule_exposer_t& Molecule_exposer ){

    { //::SireMol::Molecule::nIntraBonds
    
        typedef int ( ::SireMol::Molecule::*nIntraBonds_function_type )(  ) const;
        
        Molecule_exposer.def( 
            "nIntraBonds"
            , nIntraBonds_function_type( &::SireMol::Molecule::nIntraBonds ) );
    
    }

    { //::SireMol::Molecule::nIntraBonds
    
        typedef int ( ::SireMol::Molecule::*nIntraBonds_function_type )( ::SireMol::ResNum ) const;
        
        Molecule_exposer.def( 
            "nIntraBonds"
            , nIntraBonds_function_type( &::SireMol::Molecule::nIntraBonds )
            , ( bp::arg("resnum") ) );
    
    }

    { //::SireMol::Molecule::nIntraBonds
    
        typedef int ( ::SireMol::Molecule::*nIntraBonds_function_type )( ::SireMol::ResID ) const;
        
        Molecule_exposer.def( 
            "nIntraBonds"
            , nIntraBonds_function_type( &::SireMol::Molecule::nIntraBonds )
            , ( bp::arg("resid") ) );
    
    }

    { //::SireMol::Molecule::nResidues
    
        typedef int ( ::SireMol::Molecule::*nResidues_function_type )(  ) const;
        
        Molecule_exposer.def( 
            "nResidues"
            , nResidues_function_type( &::SireMol::Molecule::nResidues ) );
    
    }

    { //::SireMol::Molecule::name
    
        typedef ::QString ( ::SireMol::Molecule::*name_function_type )(  ) const;
        
        Molecule_exposer.def( 
            "name"
            , name_function_type( &::SireMol::Molecule::name ) );
    
    }

    { //::SireMol::Molecule::operator[]
    
        typedef ::SireMol::CutGroup ( ::SireMol::Molecule::*__getitem___function_type )( ::SireMol::CutGroupID ) const;
        
        Molecule_exposer.def( 
            "__getitem__"
            , __getitem___function_type( &::SireMol::Molecule::operator[] )
            , ( bp::arg("cgid") ) );
    
    }

    { //::SireMol::Molecule::operator[]
    
        typedef ::SireMol::Residue ( ::SireMol::Molecule::*__getitem___function_type )( ::SireMol::ResID ) const;
        
        Molecule_exposer.def( 
            "__getitem__"
            , __getitem___function_type( &::SireMol::Molecule::operator[] )
            , ( bp::arg("resid") ) );
    
    }

    { //::SireMol::Molecule::operator[]
    
        typedef ::SireMol::Residue ( ::SireMol::Molecule::*__getitem___function_type )( ::SireMol::ResNum ) const;
        
        Molecule_exposer.def( 
            "__getitem__"
            , __getitem___function_type( &::SireMol::Molecule::operator[] )
            , ( bp::arg("resnum") ) );
    
    }

    { //::SireMol::Molecule::operator[]
    
        typedef ::SireMol::Atom ( ::SireMol::Molecule::*__getitem___function_type )( ::SireMol::CGAtomID const & ) const;
        
        Molecule_exposer.def( 
            "__getitem__"
            , __getitem___function_type( &::SireMol::Molecule::operator[] )
            , ( bp::arg("cgatomid") ) );
    
    }

    { //::SireMol::Molecule::operator[]
    
        typedef ::SireMol::Atom ( ::SireMol::Molecule::*__getitem___function_type )( ::SireMol::IDMolAtom const & ) const;
        
        Molecule_exposer.def( 
            "__getitem__"
            , __getitem___function_type( &::SireMol::Molecule::operator[] )
            , ( bp::arg("atomid") ) );
    
    }

    { //::SireMol::Molecule::properties
    
        typedef ::QHash<QString, SireBase::Property> const & ( ::SireMol::Molecule::*properties_function_type )(  ) const;
        
        Molecule_exposer.def( 
            "properties"
            , properties_function_type( &::SireMol::Molecule::properties )
            , bp::return_value_policy< bp::copy_const_reference >() );
    
    }

    { //::SireMol::Molecule::residue
    
        typedef ::SireMol::Residue ( ::SireMol::Molecule::*residue_function_type )( ::SireMol::ResNum ) const;
        
        Molecule_exposer.def( 
            "residue"
            , residue_function_type( &::SireMol::Molecule::residue )
            , ( bp::arg("resnum") ) );
    
    }

    { //::SireMol::Molecule::residue
    
        typedef ::SireMol::Residue ( ::SireMol::Molecule::*residue_function_type )( ::SireMol::ResID ) const;
        
        Molecule_exposer.def( 
            "residue"
            , residue_function_type( &::SireMol::Molecule::residue )
            , ( bp::arg("resid") ) );
    
    }

    { //::SireMol::Molecule::residue
    
        typedef ::SireMol::Residue ( ::SireMol::Molecule::*residue_function_type )( ::QString const & ) const;
        
        Molecule_exposer.def( 
            "residue"
            , residue_function_type( &::SireMol::Molecule::residue )
            , ( bp::arg("resname") ) );
    
    }

    { //::SireMol::Molecule::residueName
    
        typedef ::QString ( ::SireMol::Molecule::*residueName_function_type )( ::SireMol::ResNum ) const;
        
        Molecule_exposer.def( 
            "residueName"
            , residueName_function_type( &::SireMol::Molecule::residueName )
            , ( bp::arg("resnum") ) );
    
    }

    { //::SireMol::Molecule::residueName
    
        typedef ::QString ( ::SireMol::Molecule::*residueName_function_type )( ::SireMol::ResID ) const;
        
        Molecule_exposer.def( 
            "residueName"
            , residueName_function_type( &::SireMol::Molecule::residueName )
            , ( bp::arg("resid") ) );
    
    }

    { //::SireMol::Molecule::residueNames
    
        typedef ::QStringList ( ::SireMol::Molecule::*residueNames_function_type )(  ) const;
        
        Molecule_exposer.def( 
            "residueNames"
            , residueNames_function_type( &::SireMol::Molecule::residueNames ) );
    
    }

    { //::SireMol::Molecule::residueNumber
    
        typedef ::SireMol::ResNum ( ::SireMol::Molecule::*residueNumber_function_type )( ::SireMol::ResID ) const;
        
        Molecule_exposer.def( 
            "residueNumber"
            , residueNumber_function_type( &::SireMol::Molecule::residueNumber )
            , ( bp::arg("resid") ) );
    
    }

    { //::SireMol::Molecule::residueNumber
    
        typedef ::SireMol::ResNum ( ::SireMol::Molecule::*residueNumber_function_type )( ::QString const & ) const;
        
        Molecule_exposer.def( 
            "residueNumber"
            , residueNumber_function_type( &::SireMol::Molecule::residueNumber )
            , ( bp::arg("resname") ) );
    
    }

    { //::SireMol::Molecule::residueNumbers
    
        typedef ::QVector<SireMol::ResNum> ( ::SireMol::Molecule::*residueNumbers_function_type )(  ) const;
        
        Molecule_exposer.def( 
            "residueNumbers"
            , residueNumbers_function_type( &::SireMol::Molecule::residueNumbers ) );
    
    }

}
