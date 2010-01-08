// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "AM1BCC.pypp.hpp"

namespace bp = boost::python;

#include "SireBase/process.h"

#include "SireBase/tempdir.h"

#include "SireIO/pdb.h"

#include "SireMol/atomcoords.h"

#include "SireMol/atomname.h"

#include "SireMol/groupatomids.h"

#include "SireMol/moleculedata.h"

#include "SireMol/moleculeinfodata.h"

#include "SireMol/resname.h"

#include "SireStream/datastream.h"

#include "SireStream/shareddatastream.h"

#include "SireUnits/dimensions.h"

#include "SireUnits/units.h"

#include "am1bcc.h"

#include <cstdlib>

#include "am1bcc.h"

Squire::AM1BCC __copy__(const Squire::AM1BCC &other){ return Squire::AM1BCC(other); }

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_AM1BCC_class(){

    { //::Squire::AM1BCC
        typedef bp::class_< Squire::AM1BCC, bp::bases< Squire::QMChargeCalculator, SireBase::Property > > AM1BCC_exposer_t;
        AM1BCC_exposer_t AM1BCC_exposer = AM1BCC_exposer_t( "AM1BCC" );
        bp::scope AM1BCC_scope( AM1BCC_exposer );
        AM1BCC_exposer.def( bp::init< >() );
        AM1BCC_exposer.def( bp::init< Squire::AM1BCC const & >(( bp::arg("other") )) );
        { //::Squire::AM1BCC::environment
        
            typedef ::QHash< QString, QString > const & ( ::Squire::AM1BCC::*environment_function_type )(  ) const;
            environment_function_type environment_function_value( &::Squire::AM1BCC::environment );
            
            AM1BCC_exposer.def( 
                "environment"
                , environment_function_value
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::Squire::AM1BCC::environment
        
            typedef ::QString ( ::Squire::AM1BCC::*environment_function_type )( ::QString const & ) const;
            environment_function_type environment_function_value( &::Squire::AM1BCC::environment );
            
            AM1BCC_exposer.def( 
                "environment"
                , environment_function_value
                , ( bp::arg("variable") ) );
        
        }
        { //::Squire::AM1BCC::mayChangeCharges
        
            typedef bool ( ::Squire::AM1BCC::*mayChangeCharges_function_type )( ::SireMol::PartialMolecule const &,::SireMol::PartialMolecule const &,::SireBase::PropertyMap const & ) const;
            mayChangeCharges_function_type mayChangeCharges_function_value( &::Squire::AM1BCC::mayChangeCharges );
            
            AM1BCC_exposer.def( 
                "mayChangeCharges"
                , mayChangeCharges_function_value
                , ( bp::arg("oldmol"), bp::arg("newmol"), bp::arg("map")=SireBase::PropertyMap() ) );
        
        }
        AM1BCC_exposer.def( bp::self != bp::self );
        { //::Squire::AM1BCC::operator()
        
            typedef ::SireMol::AtomCharges ( ::Squire::AM1BCC::*__call___function_type )( ::SireMol::PartialMolecule const &,::SireBase::PropertyMap const & ) const;
            __call___function_type __call___function_value( &::Squire::AM1BCC::operator() );
            
            AM1BCC_exposer.def( 
                "__call__"
                , __call___function_value
                , ( bp::arg("molecule"), bp::arg("map")=SireBase::PropertyMap() ) );
        
        }
        { //::Squire::AM1BCC::operator=
        
            typedef ::Squire::AM1BCC & ( ::Squire::AM1BCC::*assign_function_type )( ::Squire::AM1BCC const & ) ;
            assign_function_type assign_function_value( &::Squire::AM1BCC::operator= );
            
            AM1BCC_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        AM1BCC_exposer.def( bp::self == bp::self );
        { //::Squire::AM1BCC::setEnvironment
        
            typedef void ( ::Squire::AM1BCC::*setEnvironment_function_type )( ::QString const &,::QString const & ) ;
            setEnvironment_function_type setEnvironment_function_value( &::Squire::AM1BCC::setEnvironment );
            
            AM1BCC_exposer.def( 
                "setEnvironment"
                , setEnvironment_function_value
                , ( bp::arg("variable"), bp::arg("value") ) );
        
        }
        { //::Squire::AM1BCC::setScaleFactor
        
            typedef void ( ::Squire::AM1BCC::*setScaleFactor_function_type )( double ) ;
            setScaleFactor_function_type setScaleFactor_function_value( &::Squire::AM1BCC::setScaleFactor );
            
            AM1BCC_exposer.def( 
                "setScaleFactor"
                , setScaleFactor_function_value
                , ( bp::arg("sclfac") ) );
        
        }
        { //::Squire::AM1BCC::setTotalCharge
        
            typedef void ( ::Squire::AM1BCC::*setTotalCharge_function_type )( int ) ;
            setTotalCharge_function_type setTotalCharge_function_value( &::Squire::AM1BCC::setTotalCharge );
            
            AM1BCC_exposer.def( 
                "setTotalCharge"
                , setTotalCharge_function_value
                , ( bp::arg("charge") ) );
        
        }
        { //::Squire::AM1BCC::totalCharge
        
            typedef int ( ::Squire::AM1BCC::*totalCharge_function_type )(  ) const;
            totalCharge_function_type totalCharge_function_value( &::Squire::AM1BCC::totalCharge );
            
            AM1BCC_exposer.def( 
                "totalCharge"
                , totalCharge_function_value );
        
        }
        { //::Squire::AM1BCC::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::Squire::AM1BCC::typeName );
            
            AM1BCC_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        AM1BCC_exposer.staticmethod( "typeName" );
        AM1BCC_exposer.def( "__copy__", &__copy__);
        AM1BCC_exposer.def( "__deepcopy__", &__copy__);
        AM1BCC_exposer.def( "clone", &__copy__);
        AM1BCC_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::Squire::AM1BCC >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        AM1BCC_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::Squire::AM1BCC >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        AM1BCC_exposer.def( "__str__", &__str__< ::Squire::AM1BCC > );
        AM1BCC_exposer.def( "__repr__", &__str__< ::Squire::AM1BCC > );
    }

}
