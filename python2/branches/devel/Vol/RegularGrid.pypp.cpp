// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "RegularGrid.pypp.hpp"

namespace bp = boost::python;

#include "SireError/errors.h"

#include "SireMaths/rotate.h"

#include "SireStream/datastream.h"

#include "SireStream/shareddatastream.h"

#include "SireUnits/units.h"

#include "grid.h"

#include "grid.h"

SireVol::RegularGrid __copy__(const SireVol::RegularGrid &other){ return SireVol::RegularGrid(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_RegularGrid_class(){

    { //::SireVol::RegularGrid
        typedef bp::class_< SireVol::RegularGrid, bp::bases< SireVol::Grid, SireBase::Property > > RegularGrid_exposer_t;
        RegularGrid_exposer_t RegularGrid_exposer = RegularGrid_exposer_t( "RegularGrid", bp::init< >() );
        bp::scope RegularGrid_scope( RegularGrid_exposer );
        RegularGrid_exposer.def( bp::init< SireMaths::Vector const &, SireMaths::Vector const &, SireUnits::Dimension::Length >(( bp::arg("min"), bp::arg("max"), bp::arg("gridsize") )) );
        RegularGrid_exposer.def( bp::init< SireMaths::Vector const &, SireMaths::Vector const &, SireMaths::Matrix const &, SireUnits::Dimension::Length >(( bp::arg("min"), bp::arg("max"), bp::arg("basis"), bp::arg("gridsize") )) );
        RegularGrid_exposer.def( bp::init< SireMaths::Vector const &, SireMaths::Vector const &, SireMaths::Quaternion const &, SireUnits::Dimension::Length >(( bp::arg("min"), bp::arg("max"), bp::arg("basis"), bp::arg("gridsize") )) );
        RegularGrid_exposer.def( bp::init< SireMaths::Vector const &, int, SireUnits::Dimension::Length >(( bp::arg("center"), bp::arg("npoints"), bp::arg("gridsize") )) );
        RegularGrid_exposer.def( bp::init< SireMaths::Vector const &, SireMaths::Matrix const &, int, SireUnits::Dimension::Length >(( bp::arg("center"), bp::arg("basis"), bp::arg("npoints"), bp::arg("gridsize") )) );
        RegularGrid_exposer.def( bp::init< SireMaths::Vector const &, SireMaths::Quaternion const &, int, SireUnits::Dimension::Length >(( bp::arg("center"), bp::arg("basis"), bp::arg("npoints"), bp::arg("gridsize") )) );
        RegularGrid_exposer.def( bp::init< SireVol::RegularGrid const & >(( bp::arg("other") )) );
        { //::SireVol::RegularGrid::basis
        
            typedef ::SireMaths::Matrix const & ( ::SireVol::RegularGrid::*basis_function_type )(  ) const;
            basis_function_type basis_function_value( &::SireVol::RegularGrid::basis );
            
            RegularGrid_exposer.def( 
                "basis"
                , basis_function_value
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireVol::RegularGrid::dimX
        
            typedef int ( ::SireVol::RegularGrid::*dimX_function_type )(  ) const;
            dimX_function_type dimX_function_value( &::SireVol::RegularGrid::dimX );
            
            RegularGrid_exposer.def( 
                "dimX"
                , dimX_function_value );
        
        }
        { //::SireVol::RegularGrid::dimY
        
            typedef int ( ::SireVol::RegularGrid::*dimY_function_type )(  ) const;
            dimY_function_type dimY_function_value( &::SireVol::RegularGrid::dimY );
            
            RegularGrid_exposer.def( 
                "dimY"
                , dimY_function_value );
        
        }
        { //::SireVol::RegularGrid::dimZ
        
            typedef int ( ::SireVol::RegularGrid::*dimZ_function_type )(  ) const;
            dimZ_function_type dimZ_function_value( &::SireVol::RegularGrid::dimZ );
            
            RegularGrid_exposer.def( 
                "dimZ"
                , dimZ_function_value );
        
        }
        { //::SireVol::RegularGrid::gridSpacing
        
            typedef ::SireUnits::Dimension::Length ( ::SireVol::RegularGrid::*gridSpacing_function_type )(  ) const;
            gridSpacing_function_type gridSpacing_function_value( &::SireVol::RegularGrid::gridSpacing );
            
            RegularGrid_exposer.def( 
                "gridSpacing"
                , gridSpacing_function_value );
        
        }
        RegularGrid_exposer.def( bp::self != bp::self );
        { //::SireVol::RegularGrid::operator=
        
            typedef ::SireVol::RegularGrid & ( ::SireVol::RegularGrid::*assign_function_type )( ::SireVol::RegularGrid const & ) ;
            assign_function_type assign_function_value( &::SireVol::RegularGrid::operator= );
            
            RegularGrid_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        RegularGrid_exposer.def( bp::self == bp::self );
        { //::SireVol::RegularGrid::recenter
        
            typedef ::SireVol::GridPtr ( ::SireVol::RegularGrid::*recenter_function_type )( ::SireMaths::Vector const & ) const;
            recenter_function_type recenter_function_value( &::SireVol::RegularGrid::recenter );
            
            RegularGrid_exposer.def( 
                "recenter"
                , recenter_function_value
                , ( bp::arg("center") ) );
        
        }
        { //::SireVol::RegularGrid::rotate
        
            typedef ::SireVol::GridPtr ( ::SireVol::RegularGrid::*rotate_function_type )( ::SireMaths::Matrix const &,::SireMaths::Vector const & ) const;
            rotate_function_type rotate_function_value( &::SireVol::RegularGrid::rotate );
            
            RegularGrid_exposer.def( 
                "rotate"
                , rotate_function_value
                , ( bp::arg("rotmat"), bp::arg("center")=SireMaths::Vector(0.0) ) );
        
        }
        { //::SireVol::RegularGrid::rotate
        
            typedef ::SireVol::GridPtr ( ::SireVol::RegularGrid::*rotate_function_type )( ::SireMaths::Quaternion const &,::SireMaths::Vector const & ) const;
            rotate_function_type rotate_function_value( &::SireVol::RegularGrid::rotate );
            
            RegularGrid_exposer.def( 
                "rotate"
                , rotate_function_value
                , ( bp::arg("quat"), bp::arg("center")=SireMaths::Vector(0.0) ) );
        
        }
        { //::SireVol::RegularGrid::scale
        
            typedef ::SireVol::GridPtr ( ::SireVol::RegularGrid::*scale_function_type )( double ) const;
            scale_function_type scale_function_value( &::SireVol::RegularGrid::scale );
            
            RegularGrid_exposer.def( 
                "scale"
                , scale_function_value
                , ( bp::arg("scalefactor") ) );
        
        }
        { //::SireVol::RegularGrid::toString
        
            typedef ::QString ( ::SireVol::RegularGrid::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireVol::RegularGrid::toString );
            
            RegularGrid_exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        { //::SireVol::RegularGrid::translate
        
            typedef ::SireVol::GridPtr ( ::SireVol::RegularGrid::*translate_function_type )( ::SireMaths::Vector const & ) const;
            translate_function_type translate_function_value( &::SireVol::RegularGrid::translate );
            
            RegularGrid_exposer.def( 
                "translate"
                , translate_function_value
                , ( bp::arg("delta") ) );
        
        }
        RegularGrid_exposer.def( "__copy__", &__copy__);
        RegularGrid_exposer.def( "__deepcopy__", &__copy__);
        RegularGrid_exposer.def( "clone", &__copy__);
        RegularGrid_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireVol::RegularGrid >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        RegularGrid_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireVol::RegularGrid >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        RegularGrid_exposer.def( "__str__", &__str__< ::SireVol::RegularGrid > );
        RegularGrid_exposer.def( "__repr__", &__str__< ::SireVol::RegularGrid > );
    }

}
