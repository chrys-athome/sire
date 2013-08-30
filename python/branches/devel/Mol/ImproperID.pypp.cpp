// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "Helpers/clone_const_reference.hpp"
#include "ImproperID.pypp.hpp"

namespace bp = boost::python;

#include "SireBase/property.h"

#include "SireMaths/torsion.h"

#include "SireMaths/vector.h"

#include "SireStream/datastream.h"

#include "SireStream/shareddatastream.h"

#include "atomcoords.h"

#include "improperid.h"

#include "moleculedata.h"

#include "moleculeinfodata.h"

#include "improperid.h"

SireMol::ImproperID __copy__(const SireMol::ImproperID &other){ return SireMol::ImproperID(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_ImproperID_class(){

    { //::SireMol::ImproperID
        typedef bp::class_< SireMol::ImproperID, bp::bases< SireID::ID > > ImproperID_exposer_t;
        ImproperID_exposer_t ImproperID_exposer = ImproperID_exposer_t( "ImproperID", bp::init< >() );
        bp::scope ImproperID_scope( ImproperID_exposer );
        ImproperID_exposer.def( bp::init< SireMol::AtomID const &, SireMol::AtomID const &, SireMol::AtomID const &, SireMol::AtomID const & >(( bp::arg("atom0"), bp::arg("atom1"), bp::arg("atom2"), bp::arg("atom3") )) );
        ImproperID_exposer.def( bp::init< SireMol::ImproperID const & >(( bp::arg("other") )) );
        { //::SireMol::ImproperID::atom0
        
            typedef ::SireMol::AtomID const & ( ::SireMol::ImproperID::*atom0_function_type )(  ) const;
            atom0_function_type atom0_function_value( &::SireMol::ImproperID::atom0 );
            
            ImproperID_exposer.def( 
                "atom0"
                , atom0_function_value
                , bp::return_value_policy<bp::clone_const_reference>() );
        
        }
        { //::SireMol::ImproperID::atom1
        
            typedef ::SireMol::AtomID const & ( ::SireMol::ImproperID::*atom1_function_type )(  ) const;
            atom1_function_type atom1_function_value( &::SireMol::ImproperID::atom1 );
            
            ImproperID_exposer.def( 
                "atom1"
                , atom1_function_value
                , bp::return_value_policy<bp::clone_const_reference>() );
        
        }
        { //::SireMol::ImproperID::atom2
        
            typedef ::SireMol::AtomID const & ( ::SireMol::ImproperID::*atom2_function_type )(  ) const;
            atom2_function_type atom2_function_value( &::SireMol::ImproperID::atom2 );
            
            ImproperID_exposer.def( 
                "atom2"
                , atom2_function_value
                , bp::return_value_policy<bp::clone_const_reference>() );
        
        }
        { //::SireMol::ImproperID::atom3
        
            typedef ::SireMol::AtomID const & ( ::SireMol::ImproperID::*atom3_function_type )(  ) const;
            atom3_function_type atom3_function_value( &::SireMol::ImproperID::atom3 );
            
            ImproperID_exposer.def( 
                "atom3"
                , atom3_function_value
                , bp::return_value_policy<bp::clone_const_reference>() );
        
        }
        { //::SireMol::ImproperID::hash
        
            typedef ::uint ( ::SireMol::ImproperID::*hash_function_type )(  ) const;
            hash_function_type hash_function_value( &::SireMol::ImproperID::hash );
            
            ImproperID_exposer.def( 
                "hash"
                , hash_function_value );
        
        }
        { //::SireMol::ImproperID::isNull
        
            typedef bool ( ::SireMol::ImproperID::*isNull_function_type )(  ) const;
            isNull_function_type isNull_function_value( &::SireMol::ImproperID::isNull );
            
            ImproperID_exposer.def( 
                "isNull"
                , isNull_function_value );
        
        }
        { //::SireMol::ImproperID::map
        
            typedef ::boost::tuples::tuple< SireMol::AtomIdx, SireMol::AtomIdx, SireMol::AtomIdx, SireMol::AtomIdx, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type > ( ::SireMol::ImproperID::*map_function_type )( ::SireMol::MoleculeInfoData const & ) const;
            map_function_type map_function_value( &::SireMol::ImproperID::map );
            
            ImproperID_exposer.def( 
                "map"
                , map_function_value
                , ( bp::arg("molinfo") ) );
        
        }
        { //::SireMol::ImproperID::map
        
            typedef ::boost::tuples::tuple< SireMol::AtomIdx, SireMol::AtomIdx, SireMol::AtomIdx, SireMol::AtomIdx, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type > ( ::SireMol::ImproperID::*map_function_type )( ::SireMol::MoleculeInfoData const &,::SireMol::MoleculeInfoData const &,::SireMol::MoleculeInfoData const &,::SireMol::MoleculeInfoData const & ) const;
            map_function_type map_function_value( &::SireMol::ImproperID::map );
            
            ImproperID_exposer.def( 
                "map"
                , map_function_value
                , ( bp::arg("mol0info"), bp::arg("mol1info"), bp::arg("mol2info"), bp::arg("mol3info") ) );
        
        }
        ImproperID_exposer.def( bp::self != bp::self );
        { //::SireMol::ImproperID::operator=
        
            typedef ::SireMol::ImproperID & ( ::SireMol::ImproperID::*assign_function_type )( ::SireMol::ImproperID const & ) ;
            assign_function_type assign_function_value( &::SireMol::ImproperID::operator= );
            
            ImproperID_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        ImproperID_exposer.def( bp::self == bp::other< SireID::ID >() );
        ImproperID_exposer.def( bp::self == bp::self );
        { //::SireMol::ImproperID::size
        
            typedef ::SireUnits::Dimension::Angle ( ::SireMol::ImproperID::*size_function_type )( ::SireMol::MoleculeData const &,::SireBase::PropertyMap const & ) const;
            size_function_type size_function_value( &::SireMol::ImproperID::size );
            
            ImproperID_exposer.def( 
                "size"
                , size_function_value
                , ( bp::arg("moldata"), bp::arg("map")=SireBase::PropertyMap() ) );
        
        }
        { //::SireMol::ImproperID::size
        
            typedef ::SireUnits::Dimension::Angle ( ::SireMol::ImproperID::*size_function_type )( ::SireMol::MoleculeData const &,::SireMol::MoleculeData const &,::SireMol::MoleculeData const &,::SireMol::MoleculeData const &,::SireBase::PropertyMap const & ) const;
            size_function_type size_function_value( &::SireMol::ImproperID::size );
            
            ImproperID_exposer.def( 
                "size"
                , size_function_value
                , ( bp::arg("mol0data"), bp::arg("mol1data"), bp::arg("mol2data"), bp::arg("mol3data"), bp::arg("map")=SireBase::PropertyMap() ) );
        
        }
        { //::SireMol::ImproperID::size
        
            typedef ::SireUnits::Dimension::Angle ( ::SireMol::ImproperID::*size_function_type )( ::SireMol::MoleculeData const &,::SireBase::PropertyMap const &,::SireMol::MoleculeData const &,::SireBase::PropertyMap const &,::SireMol::MoleculeData const &,::SireBase::PropertyMap const &,::SireMol::MoleculeData const &,::SireBase::PropertyMap const & ) const;
            size_function_type size_function_value( &::SireMol::ImproperID::size );
            
            ImproperID_exposer.def( 
                "size"
                , size_function_value
                , ( bp::arg("mol0data"), bp::arg("map0"), bp::arg("mol1data"), bp::arg("map1"), bp::arg("mol2data"), bp::arg("map2"), bp::arg("mol3data"), bp::arg("map3") ) );
        
        }
        { //::SireMol::ImproperID::toString
        
            typedef ::QString ( ::SireMol::ImproperID::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireMol::ImproperID::toString );
            
            ImproperID_exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        { //::SireMol::ImproperID::torsion
        
            typedef ::SireMaths::Torsion ( ::SireMol::ImproperID::*torsion_function_type )( ::SireMol::MoleculeData const &,::SireBase::PropertyMap const & ) const;
            torsion_function_type torsion_function_value( &::SireMol::ImproperID::torsion );
            
            ImproperID_exposer.def( 
                "torsion"
                , torsion_function_value
                , ( bp::arg("moldata"), bp::arg("map")=SireBase::PropertyMap() ) );
        
        }
        { //::SireMol::ImproperID::torsion
        
            typedef ::SireMaths::Torsion ( ::SireMol::ImproperID::*torsion_function_type )( ::SireMol::MoleculeData const &,::SireMol::MoleculeData const &,::SireMol::MoleculeData const &,::SireMol::MoleculeData const &,::SireBase::PropertyMap const & ) const;
            torsion_function_type torsion_function_value( &::SireMol::ImproperID::torsion );
            
            ImproperID_exposer.def( 
                "torsion"
                , torsion_function_value
                , ( bp::arg("mol0data"), bp::arg("mol1data"), bp::arg("mol2data"), bp::arg("mol3data"), bp::arg("map")=SireBase::PropertyMap() ) );
        
        }
        { //::SireMol::ImproperID::torsion
        
            typedef ::SireMaths::Torsion ( ::SireMol::ImproperID::*torsion_function_type )( ::SireMol::MoleculeData const &,::SireBase::PropertyMap const &,::SireMol::MoleculeData const &,::SireBase::PropertyMap const &,::SireMol::MoleculeData const &,::SireBase::PropertyMap const &,::SireMol::MoleculeData const &,::SireBase::PropertyMap const & ) const;
            torsion_function_type torsion_function_value( &::SireMol::ImproperID::torsion );
            
            ImproperID_exposer.def( 
                "torsion"
                , torsion_function_value
                , ( bp::arg("mol0data"), bp::arg("map0"), bp::arg("mol1data"), bp::arg("map1"), bp::arg("mol2data"), bp::arg("map2"), bp::arg("mol3data"), bp::arg("map3") ) );
        
        }
        { //::SireMol::ImproperID::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireMol::ImproperID::typeName );
            
            ImproperID_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        { //::SireMol::ImproperID::what
        
            typedef char const * ( ::SireMol::ImproperID::*what_function_type )(  ) const;
            what_function_type what_function_value( &::SireMol::ImproperID::what );
            
            ImproperID_exposer.def( 
                "what"
                , what_function_value );
        
        }
        ImproperID_exposer.staticmethod( "typeName" );
        ImproperID_exposer.def( "__copy__", &__copy__);
        ImproperID_exposer.def( "__deepcopy__", &__copy__);
        ImproperID_exposer.def( "clone", &__copy__);
        ImproperID_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireMol::ImproperID >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        ImproperID_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireMol::ImproperID >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        ImproperID_exposer.def( "__str__", &__str__< ::SireMol::ImproperID > );
        ImproperID_exposer.def( "__repr__", &__str__< ::SireMol::ImproperID > );
    }

}