// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "VectorPoint.pypp.hpp"

namespace bp = boost::python;

#include "SireMol/evaluator.h"

#include "SireMol/mgidx.h"

#include "SireMol/molecule.h"

#include "SireMol/moleculegroup.h"

#include "SireMol/moleculegroups.h"

#include "SireStream/datastream.h"

#include "SireStream/shareddatastream.h"

#include "SireVol/aabox.h"

#include "SireVol/errors.h"

#include "forcetable.h"

#include "point.h"

#include <boost/tuple/tuple.hpp>

#include "point.h"

SireFF::VectorPoint __copy__(const SireFF::VectorPoint &other){ return SireFF::VectorPoint(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_VectorPoint_class(){

    { //::SireFF::VectorPoint
        typedef bp::class_< SireFF::VectorPoint, bp::bases< SireFF::Point, SireBase::Property > > VectorPoint_exposer_t;
        VectorPoint_exposer_t VectorPoint_exposer = VectorPoint_exposer_t( "VectorPoint" );
        bp::scope VectorPoint_scope( VectorPoint_exposer );
        VectorPoint_exposer.def( bp::init< >() );
        VectorPoint_exposer.def( bp::init< SireMaths::Vector const & >(( bp::arg("point") )) );
        VectorPoint_exposer.def( bp::init< SireFF::VectorPoint const & >(( bp::arg("other") )) );
        { //::SireFF::VectorPoint::addForce
        
            typedef bool ( ::SireFF::VectorPoint::*addForce_function_type )( ::SireFF::MolForceTable &,::SireMaths::Vector const & ) const;
            addForce_function_type addForce_function_value( &::SireFF::VectorPoint::addForce );
            
            VectorPoint_exposer.def( 
                "addForce"
                , addForce_function_value
                , ( bp::arg("molforces"), bp::arg("force") ) );
        
        }
        { //::SireFF::VectorPoint::addForce
        
            typedef bool ( ::SireFF::VectorPoint::*addForce_function_type )( ::SireFF::ForceTable &,::SireMaths::Vector const & ) const;
            addForce_function_type addForce_function_value( &::SireFF::VectorPoint::addForce );
            
            VectorPoint_exposer.def( 
                "addForce"
                , addForce_function_value
                , ( bp::arg("forces"), bp::arg("force") ) );
        
        }
        { //::SireFF::VectorPoint::contains
        
            typedef bool ( ::SireFF::VectorPoint::*contains_function_type )( ::SireMol::MolNum ) const;
            contains_function_type contains_function_value( &::SireFF::VectorPoint::contains );
            
            VectorPoint_exposer.def( 
                "contains"
                , contains_function_value
                , ( bp::arg("molnum") ) );
        
        }
        { //::SireFF::VectorPoint::contains
        
            typedef bool ( ::SireFF::VectorPoint::*contains_function_type )( ::SireMol::MolID const & ) const;
            contains_function_type contains_function_value( &::SireFF::VectorPoint::contains );
            
            VectorPoint_exposer.def( 
                "contains"
                , contains_function_value
                , ( bp::arg("molid") ) );
        
        }
        { //::SireFF::VectorPoint::molecules
        
            typedef ::SireMol::Molecules ( ::SireFF::VectorPoint::*molecules_function_type )(  ) const;
            molecules_function_type molecules_function_value( &::SireFF::VectorPoint::molecules );
            
            VectorPoint_exposer.def( 
                "molecules"
                , molecules_function_value );
        
        }
        { //::SireFF::VectorPoint::nMolecules
        
            typedef int ( ::SireFF::VectorPoint::*nMolecules_function_type )(  ) const;
            nMolecules_function_type nMolecules_function_value( &::SireFF::VectorPoint::nMolecules );
            
            VectorPoint_exposer.def( 
                "nMolecules"
                , nMolecules_function_value );
        
        }
        VectorPoint_exposer.def( bp::self != bp::self );
        { //::SireFF::VectorPoint::operator=
        
            typedef ::SireFF::VectorPoint & ( ::SireFF::VectorPoint::*assign_function_type )( ::SireFF::VectorPoint const & ) ;
            assign_function_type assign_function_value( &::SireFF::VectorPoint::operator= );
            
            VectorPoint_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        VectorPoint_exposer.def( bp::self == bp::self );
        { //::SireFF::VectorPoint::toString
        
            typedef ::QString ( ::SireFF::VectorPoint::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireFF::VectorPoint::toString );
            
            VectorPoint_exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        { //::SireFF::VectorPoint::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireFF::VectorPoint::typeName );
            
            VectorPoint_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        { //::SireFF::VectorPoint::update
        
            typedef bool ( ::SireFF::VectorPoint::*update_function_type )( ::SireMol::MoleculeData const & ) ;
            update_function_type update_function_value( &::SireFF::VectorPoint::update );
            
            VectorPoint_exposer.def( 
                "update"
                , update_function_value
                , ( bp::arg("moldata") ) );
        
        }
        { //::SireFF::VectorPoint::update
        
            typedef bool ( ::SireFF::VectorPoint::*update_function_type )( ::SireMol::Molecules const & ) ;
            update_function_type update_function_value( &::SireFF::VectorPoint::update );
            
            VectorPoint_exposer.def( 
                "update"
                , update_function_value
                , ( bp::arg("molecules") ) );
        
        }
        { //::SireFF::VectorPoint::update
        
            typedef bool ( ::SireFF::VectorPoint::*update_function_type )( ::SireMol::MoleculeGroup const & ) ;
            update_function_type update_function_value( &::SireFF::VectorPoint::update );
            
            VectorPoint_exposer.def( 
                "update"
                , update_function_value
                , ( bp::arg("molgroup") ) );
        
        }
        { //::SireFF::VectorPoint::update
        
            typedef bool ( ::SireFF::VectorPoint::*update_function_type )( ::SireMol::MolGroupsBase const & ) ;
            update_function_type update_function_value( &::SireFF::VectorPoint::update );
            
            VectorPoint_exposer.def( 
                "update"
                , update_function_value
                , ( bp::arg("molgroups") ) );
        
        }
        { //::SireFF::VectorPoint::usesMoleculesIn
        
            typedef bool ( ::SireFF::VectorPoint::*usesMoleculesIn_function_type )( ::SireFF::ForceTable const & ) const;
            usesMoleculesIn_function_type usesMoleculesIn_function_value( &::SireFF::VectorPoint::usesMoleculesIn );
            
            VectorPoint_exposer.def( 
                "usesMoleculesIn"
                , usesMoleculesIn_function_value
                , ( bp::arg("forcetable") ) );
        
        }
        { //::SireFF::VectorPoint::usesMoleculesIn
        
            typedef bool ( ::SireFF::VectorPoint::*usesMoleculesIn_function_type )( ::SireMol::Molecules const & ) const;
            usesMoleculesIn_function_type usesMoleculesIn_function_value( &::SireFF::VectorPoint::usesMoleculesIn );
            
            VectorPoint_exposer.def( 
                "usesMoleculesIn"
                , usesMoleculesIn_function_value
                , ( bp::arg("molecules") ) );
        
        }
        { //::SireFF::VectorPoint::usesMoleculesIn
        
            typedef bool ( ::SireFF::VectorPoint::*usesMoleculesIn_function_type )( ::SireMol::MoleculeGroup const & ) const;
            usesMoleculesIn_function_type usesMoleculesIn_function_value( &::SireFF::VectorPoint::usesMoleculesIn );
            
            VectorPoint_exposer.def( 
                "usesMoleculesIn"
                , usesMoleculesIn_function_value
                , ( bp::arg("molgroup") ) );
        
        }
        { //::SireFF::VectorPoint::usesMoleculesIn
        
            typedef bool ( ::SireFF::VectorPoint::*usesMoleculesIn_function_type )( ::SireMol::MolGroupsBase const & ) const;
            usesMoleculesIn_function_type usesMoleculesIn_function_value( &::SireFF::VectorPoint::usesMoleculesIn );
            
            VectorPoint_exposer.def( 
                "usesMoleculesIn"
                , usesMoleculesIn_function_value
                , ( bp::arg("molgroups") ) );
        
        }
        VectorPoint_exposer.staticmethod( "typeName" );
        VectorPoint_exposer.def( "__copy__", &__copy__);
        VectorPoint_exposer.def( "__deepcopy__", &__copy__);
        VectorPoint_exposer.def( "clone", &__copy__);
        VectorPoint_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireFF::VectorPoint >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        VectorPoint_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireFF::VectorPoint >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        VectorPoint_exposer.def( "__str__", &__str__< ::SireFF::VectorPoint > );
        VectorPoint_exposer.def( "__repr__", &__str__< ::SireFF::VectorPoint > );
    }

}
