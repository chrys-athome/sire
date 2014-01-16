// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "TestFF.pypp.hpp"

namespace bp = boost::python;

#include "SireUnits/units.h"

#include "testff.h"

#include <QDebug>

#include <QElapsedTimer>

#include "testff.h"

SireMM::TestFF __copy__(const SireMM::TestFF &other){ return SireMM::TestFF(other); }

const char* pvt_get_name(const SireMM::TestFF&){ return "SireMM::TestFF";}

void register_TestFF_class(){

    { //::SireMM::TestFF
        typedef bp::class_< SireMM::TestFF > TestFF_exposer_t;
        TestFF_exposer_t TestFF_exposer = TestFF_exposer_t( "TestFF", bp::init< >() );
        bp::scope TestFF_scope( TestFF_exposer );
        TestFF_exposer.def( bp::init< SireMM::TestFF const & >(( bp::arg("other") )) );
        { //::SireMM::TestFF::add
        
            typedef void ( ::SireMM::TestFF::*add_function_type )( ::SireMol::Molecules const & ) ;
            add_function_type add_function_value( &::SireMM::TestFF::add );
            
            TestFF_exposer.def( 
                "add"
                , add_function_value
                , ( bp::arg("molecules") ) );
        
        }
        { //::SireMM::TestFF::addFixedAtoms
        
            typedef void ( ::SireMM::TestFF::*addFixedAtoms_function_type )( ::SireMol::Molecules const & ) ;
            addFixedAtoms_function_type addFixedAtoms_function_value( &::SireMM::TestFF::addFixedAtoms );
            
            TestFF_exposer.def( 
                "addFixedAtoms"
                , addFixedAtoms_function_value
                , ( bp::arg("molecules") ) );
        
        }
        { //::SireMM::TestFF::calculateEnergy
        
            typedef void ( ::SireMM::TestFF::*calculateEnergy_function_type )(  ) ;
            calculateEnergy_function_type calculateEnergy_function_value( &::SireMM::TestFF::calculateEnergy );
            
            TestFF_exposer.def( 
                "calculateEnergy"
                , calculateEnergy_function_value );
        
        }
        { //::SireMM::TestFF::operator=
        
            typedef ::SireMM::TestFF & ( ::SireMM::TestFF::*assign_function_type )( ::SireMM::TestFF const & ) ;
            assign_function_type assign_function_value( &::SireMM::TestFF::operator= );
            
            TestFF_exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        TestFF_exposer.def( "__copy__", &__copy__);
        TestFF_exposer.def( "__deepcopy__", &__copy__);
        TestFF_exposer.def( "clone", &__copy__);
        TestFF_exposer.def( "__str__", &pvt_get_name);
        TestFF_exposer.def( "__repr__", &pvt_get_name);
    }

}
