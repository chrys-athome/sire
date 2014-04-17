// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "VolumeMap.pypp.hpp"

namespace bp = boost::python;

#include "SireStream/datastream.h"

#include "SireStream/shareddatastream.h"

#include "SireUnits/units.h"

#include "atomcoords.h"

#include "atomelements.h"

#include "molecule.h"

#include "partialmolecule.h"

#include "volumemap.h"

#include <QElapsedTimer>

#include "volumemap.h"

SireMol::VolumeMap __copy__(const SireMol::VolumeMap &other){ return SireMol::VolumeMap(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_VolumeMap_class(){

    { //::SireMol::VolumeMap
        typedef bp::class_< SireMol::VolumeMap, bp::bases< SireBase::Property > > VolumeMap_exposer_t;
        VolumeMap_exposer_t VolumeMap_exposer = VolumeMap_exposer_t( "VolumeMap", bp::init< >() );
        bp::scope VolumeMap_scope( VolumeMap_exposer );
        bp::enum_< SireMol::VolumeMap::FillType>("FillType")
            .value("POINT_ATOMS", SireMol::VolumeMap::POINT_ATOMS)
            .value("VDW_RADIUS", SireMol::VolumeMap::VDW_RADIUS)
            .value("COVALENT_RADIUS", SireMol::VolumeMap::COVALENT_RADIUS)
            .value("BOND_ORDER_RADIUS", SireMol::VolumeMap::BOND_ORDER_RADIUS)
            .export_values()
            ;
        bp::enum_< SireMol::VolumeMap::MapType>("MapType")
            .value("AVERAGE", SireMol::VolumeMap::AVERAGE)
            .value("MAXIMUM", SireMol::VolumeMap::MAXIMUM)
            .value("SUM", SireMol::VolumeMap::SUM)
            .export_values()
            ;
        VolumeMap_exposer.def( bp::init< bool >(( bp::arg("skip_light_atoms") )) );
        VolumeMap_exposer.def( bp::init< SireUnits::Dimension::Length const &, bp::optional< bool > >(( bp::arg("grid_spacing"), bp::arg("skip_light_atoms")=(bool)(false) )) );
        VolumeMap_exposer.def( bp::init< SireMol::VolumeMap::MapType, bp::optional< bool > >(( bp::arg("map_type"), bp::arg("skip_light_atoms")=(bool)(false) )) );
        VolumeMap_exposer.def( bp::init< SireMol::VolumeMap::FillType, bp::optional< bool > >(( bp::arg("fill_type"), bp::arg("skip_light_atoms")=(bool)(false) )) );
        VolumeMap_exposer.def( bp::init< SireUnits::Dimension::Length const &, SireMol::VolumeMap::MapType, bp::optional< bool > >(( bp::arg("grid_spacing"), bp::arg("map_type"), bp::arg("skip_light_atoms")=(bool)(false) )) );
        VolumeMap_exposer.def( bp::init< SireUnits::Dimension::Length const &, SireMol::VolumeMap::FillType, bp::optional< bool > >(( bp::arg("grid_spacing"), bp::arg("fill_type"), bp::arg("skip_light_atoms")=(bool)(false) )) );
        VolumeMap_exposer.def( bp::init< SireMol::VolumeMap::FillType, SireMol::VolumeMap::MapType, bp::optional< bool > >(( bp::arg("fill_type"), bp::arg("map_type"), bp::arg("skip_light_atoms")=(bool)(false) )) );
        VolumeMap_exposer.def( bp::init< SireUnits::Dimension::Length const &, SireMol::VolumeMap::FillType, SireMol::VolumeMap::MapType, bp::optional< bool > >(( bp::arg("grid_spacing"), bp::arg("fill_type"), bp::arg("map_type"), bp::arg("skip_light_atoms")=(bool)(false) )) );
        VolumeMap_exposer.def( bp::init< SireMol::VolumeMap const & >(( bp::arg("other") )) );
        { //::SireMol::VolumeMap::add
        
            typedef void ( ::SireMol::VolumeMap::*add_function_type )( ::SireMol::MoleculeView const &,::SireBase::PropertyMap const & ) ;
            add_function_type add_function_value( &::SireMol::VolumeMap::add );
            
            VolumeMap_exposer.def( 
                "add"
                , add_function_value
                , ( bp::arg("molecule"), bp::arg("map")=SireBase::PropertyMap() ) );
        
        }
        { //::SireMol::VolumeMap::add
        
            typedef void ( ::SireMol::VolumeMap::*add_function_type )( ::SireMol::MoleculeView const &,::SireMol::MoleculeView const &,::SireBase::PropertyMap const & ) ;
            add_function_type add_function_value( &::SireMol::VolumeMap::add );
            
            VolumeMap_exposer.def( 
                "add"
                , add_function_value
                , ( bp::arg("mol0"), bp::arg("mol1"), bp::arg("map")=SireBase::PropertyMap() ) );
        
        }
        { //::SireMol::VolumeMap::add
        
            typedef void ( ::SireMol::VolumeMap::*add_function_type )( ::QList< SireMol::PartialMolecule > const &,::SireBase::PropertyMap const & ) ;
            add_function_type add_function_value( &::SireMol::VolumeMap::add );
            
            VolumeMap_exposer.def( 
                "add"
                , add_function_value
                , ( bp::arg("molecules"), bp::arg("map")=SireBase::PropertyMap() ) );
        
        }
        { //::SireMol::VolumeMap::add
        
            typedef void ( ::SireMol::VolumeMap::*add_function_type )( ::SireMol::Molecules const &,::SireBase::PropertyMap const & ) ;
            add_function_type add_function_value( &::SireMol::VolumeMap::add );
            
            VolumeMap_exposer.def( 
                "add"
                , add_function_value
                , ( bp::arg("molecules"), bp::arg("map")=SireBase::PropertyMap() ) );
        
        }
        { //::SireMol::VolumeMap::add
        
            typedef void ( ::SireMol::VolumeMap::*add_function_type )( ::SireMol::Molecules const &,::SireMol::Molecules const &,::SireBase::PropertyMap const & ) ;
            add_function_type add_function_value( &::SireMol::VolumeMap::add );
            
            VolumeMap_exposer.def( 
                "add"
                , add_function_value
                , ( bp::arg("mols0"), bp::arg("mols1"), bp::arg("map")=SireBase::PropertyMap() ) );
        
        }
        { //::SireMol::VolumeMap::add
        
            typedef void ( ::SireMol::VolumeMap::*add_function_type )( ::QList< SireMol::Molecules > const &,::SireBase::PropertyMap const & ) ;
            add_function_type add_function_value( &::SireMol::VolumeMap::add );
            
            VolumeMap_exposer.def( 
                "add"
                , add_function_value
                , ( bp::arg("molecules"), bp::arg("map")=SireBase::PropertyMap() ) );
        
        }
        { //::SireMol::VolumeMap::add
        
            typedef void ( ::SireMol::VolumeMap::*add_function_type )( ::SireMol::MoleculeGroup const &,::SireBase::PropertyMap const & ) ;
            add_function_type add_function_value( &::SireMol::VolumeMap::add );
            
            VolumeMap_exposer.def( 
                "add"
                , add_function_value
                , ( bp::arg("molecules"), bp::arg("map")=SireBase::PropertyMap() ) );
        
        }
        { //::SireMol::VolumeMap::add
        
            typedef void ( ::SireMol::VolumeMap::*add_function_type )( ::SireMol::MoleculeGroup const &,::SireMol::MoleculeGroup const &,::SireBase::PropertyMap const & ) ;
            add_function_type add_function_value( &::SireMol::VolumeMap::add );
            
            VolumeMap_exposer.def( 
                "add"
                , add_function_value
                , ( bp::arg("mols0"), bp::arg("mols1"), bp::arg("map")=SireBase::PropertyMap() ) );
        
        }
        { //::SireMol::VolumeMap::add
        
            typedef void ( ::SireMol::VolumeMap::*add_function_type )( ::QList< SireMol::MoleculeGroup > const &,::SireBase::PropertyMap const & ) ;
            add_function_type add_function_value( &::SireMol::VolumeMap::add );
            
            VolumeMap_exposer.def( 
                "add"
                , add_function_value
                , ( bp::arg("molecules"), bp::arg("map")=SireBase::PropertyMap() ) );
        
        }
        { //::SireMol::VolumeMap::add
        
            typedef void ( ::SireMol::VolumeMap::*add_function_type )( ::SireMol::VolumeMap const & ) ;
            add_function_type add_function_value( &::SireMol::VolumeMap::add );
            
            VolumeMap_exposer.def( 
                "add"
                , add_function_value
                , ( bp::arg("other") ) );
        
        }
        { //::SireMol::VolumeMap::add
        
            typedef void ( ::SireMol::VolumeMap::*add_function_type )( ::SireVol::GridInfo const &,::QVector< float > const & ) ;
            add_function_type add_function_value( &::SireMol::VolumeMap::add );
            
            VolumeMap_exposer.def( 
                "add"
                , add_function_value
                , ( bp::arg("gridinfo"), bp::arg("values") ) );
        
        }
        { //::SireMol::VolumeMap::clear
        
            typedef void ( ::SireMol::VolumeMap::*clear_function_type )(  ) ;
            clear_function_type clear_function_value( &::SireMol::VolumeMap::clear );
            
            VolumeMap_exposer.def( 
                "clear"
                , clear_function_value );
        
        }
        { //::SireMol::VolumeMap::fillType
        
            typedef ::SireMol::VolumeMap::FillType ( ::SireMol::VolumeMap::*fillType_function_type )(  ) const;
            fillType_function_type fillType_function_value( &::SireMol::VolumeMap::fillType );
            
            VolumeMap_exposer.def( 
                "fillType"
                , fillType_function_value );
        
        }
        { //::SireMol::VolumeMap::gridInfo
        
            typedef ::SireVol::GridInfo const & ( ::SireMol::VolumeMap::*gridInfo_function_type )(  ) const;
            gridInfo_function_type gridInfo_function_value( &::SireMol::VolumeMap::gridInfo );
            
            VolumeMap_exposer.def( 
                "gridInfo"
                , gridInfo_function_value
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMol::VolumeMap::gridSpacing
        
            typedef ::SireUnits::Dimension::Length ( ::SireMol::VolumeMap::*gridSpacing_function_type )(  ) const;
            gridSpacing_function_type gridSpacing_function_value( &::SireMol::VolumeMap::gridSpacing );
            
            VolumeMap_exposer.def( 
                "gridSpacing"
                , gridSpacing_function_value );
        
        }
        { //::SireMol::VolumeMap::isEmpty
        
            typedef bool ( ::SireMol::VolumeMap::*isEmpty_function_type )(  ) const;
            isEmpty_function_type isEmpty_function_value( &::SireMol::VolumeMap::isEmpty );
            
            VolumeMap_exposer.def( 
                "isEmpty"
                , isEmpty_function_value );
        
        }
        { //::SireMol::VolumeMap::mapType
        
            typedef ::SireMol::VolumeMap::MapType ( ::SireMol::VolumeMap::*mapType_function_type )(  ) const;
            mapType_function_type mapType_function_value( &::SireMol::VolumeMap::mapType );
            
            VolumeMap_exposer.def( 
                "mapType"
                , mapType_function_value );
        
        }
        { //::SireMol::VolumeMap::nGridPoints
        
            typedef int ( ::SireMol::VolumeMap::*nGridPoints_function_type )(  ) const;
            nGridPoints_function_type nGridPoints_function_value( &::SireMol::VolumeMap::nGridPoints );
            
            VolumeMap_exposer.def( 
                "nGridPoints"
                , nGridPoints_function_value );
        
        }
        { //::SireMol::VolumeMap::nMaxGridPoints
        
            typedef int ( ::SireMol::VolumeMap::*nMaxGridPoints_function_type )(  ) const;
            nMaxGridPoints_function_type nMaxGridPoints_function_value( &::SireMol::VolumeMap::nMaxGridPoints );
            
            VolumeMap_exposer.def( 
                "nMaxGridPoints"
                , nMaxGridPoints_function_value );
        
        }
        { //::SireMol::VolumeMap::occupancy
        
            typedef ::QVector< float > const & ( ::SireMol::VolumeMap::*occupancy_function_type )(  ) const;
            occupancy_function_type occupancy_function_value( &::SireMol::VolumeMap::occupancy );
            
            VolumeMap_exposer.def( 
                "occupancy"
                , occupancy_function_value
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        VolumeMap_exposer.def( bp::self != bp::self );
        { //::SireMol::VolumeMap::operator=
        
            typedef ::SireMol::VolumeMap & ( ::SireMol::VolumeMap::*assign_function_type )( ::SireMol::VolumeMap const & ) ;
            assign_function_type assign_function_value( &::SireMol::VolumeMap::operator= );
            
            VolumeMap_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        VolumeMap_exposer.def( bp::self == bp::self );
        { //::SireMol::VolumeMap::setFillType
        
            typedef void ( ::SireMol::VolumeMap::*setFillType_function_type )( ::SireMol::VolumeMap::FillType ) ;
            setFillType_function_type setFillType_function_value( &::SireMol::VolumeMap::setFillType );
            
            VolumeMap_exposer.def( 
                "setFillType"
                , setFillType_function_value
                , ( bp::arg("fill_type") ) );
        
        }
        { //::SireMol::VolumeMap::setGridSpacing
        
            typedef void ( ::SireMol::VolumeMap::*setGridSpacing_function_type )( ::SireUnits::Dimension::Length ) ;
            setGridSpacing_function_type setGridSpacing_function_value( &::SireMol::VolumeMap::setGridSpacing );
            
            VolumeMap_exposer.def( 
                "setGridSpacing"
                , setGridSpacing_function_value
                , ( bp::arg("grid_spacing") ) );
        
        }
        { //::SireMol::VolumeMap::setMapType
        
            typedef void ( ::SireMol::VolumeMap::*setMapType_function_type )( ::SireMol::VolumeMap::MapType ) ;
            setMapType_function_type setMapType_function_value( &::SireMol::VolumeMap::setMapType );
            
            VolumeMap_exposer.def( 
                "setMapType"
                , setMapType_function_value
                , ( bp::arg("map_type") ) );
        
        }
        { //::SireMol::VolumeMap::setNMaxGridPoints
        
            typedef void ( ::SireMol::VolumeMap::*setNMaxGridPoints_function_type )( int ) ;
            setNMaxGridPoints_function_type setNMaxGridPoints_function_value( &::SireMol::VolumeMap::setNMaxGridPoints );
            
            VolumeMap_exposer.def( 
                "setNMaxGridPoints"
                , setNMaxGridPoints_function_value
                , ( bp::arg("nmax") ) );
        
        }
        { //::SireMol::VolumeMap::setSkipLightAtoms
        
            typedef void ( ::SireMol::VolumeMap::*setSkipLightAtoms_function_type )( bool ) ;
            setSkipLightAtoms_function_type setSkipLightAtoms_function_value( &::SireMol::VolumeMap::setSkipLightAtoms );
            
            VolumeMap_exposer.def( 
                "setSkipLightAtoms"
                , setSkipLightAtoms_function_value
                , ( bp::arg("on") ) );
        
        }
        { //::SireMol::VolumeMap::skipLightAtoms
        
            typedef bool ( ::SireMol::VolumeMap::*skipLightAtoms_function_type )(  ) const;
            skipLightAtoms_function_type skipLightAtoms_function_value( &::SireMol::VolumeMap::skipLightAtoms );
            
            VolumeMap_exposer.def( 
                "skipLightAtoms"
                , skipLightAtoms_function_value );
        
        }
        { //::SireMol::VolumeMap::toString
        
            typedef ::QString ( ::SireMol::VolumeMap::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireMol::VolumeMap::toString );
            
            VolumeMap_exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        { //::SireMol::VolumeMap::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireMol::VolumeMap::typeName );
            
            VolumeMap_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        { //::SireMol::VolumeMap::what
        
            typedef char const * ( ::SireMol::VolumeMap::*what_function_type )(  ) const;
            what_function_type what_function_value( &::SireMol::VolumeMap::what );
            
            VolumeMap_exposer.def( 
                "what"
                , what_function_value );
        
        }
        VolumeMap_exposer.staticmethod( "typeName" );
        VolumeMap_exposer.def( "__copy__", &__copy__);
        VolumeMap_exposer.def( "__deepcopy__", &__copy__);
        VolumeMap_exposer.def( "clone", &__copy__);
        VolumeMap_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireMol::VolumeMap >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        VolumeMap_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireMol::VolumeMap >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        VolumeMap_exposer.def( "__str__", &__str__< ::SireMol::VolumeMap > );
        VolumeMap_exposer.def( "__repr__", &__str__< ::SireMol::VolumeMap > );
    }

}
