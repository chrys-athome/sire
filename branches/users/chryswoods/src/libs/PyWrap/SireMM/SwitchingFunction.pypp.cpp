// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "SwitchingFunction.pypp.hpp"
#include "boost/python.hpp"
#include "siremm_headers.h"
#include "SireMol/molecule.h"
#include "SireMol/residue.h"
#include "SireMol/newatom.h"
#include "SireMol/atom.h"

namespace bp = boost::python;

void register_SwitchingFunction_class(){

    bp::class_< SireMM::SwitchingFunction >( "SwitchingFunction" )    
        .def( bp::init< >() )    
        .def( bp::init< SireMM::SwitchFuncBase const & >(( bp::arg("switchingfunction") )) )    
        .def( 
            "cutoffDistance"
            , (double ( ::SireMM::SwitchingFunction::* )(  ) const)( &::SireMM::SwitchingFunction::cutoffDistance ) )    
        .def( 
            "electrostaticScaleFactor"
            , (double ( ::SireMM::SwitchingFunction::* )( double ) const)( &::SireMM::SwitchingFunction::electrostaticScaleFactor )
            , ( bp::arg("dist") ) )    
        .def( 
            "vdwScaleFactor"
            , (double ( ::SireMM::SwitchingFunction::* )( double ) const)( &::SireMM::SwitchingFunction::vdwScaleFactor )
            , ( bp::arg("dist") ) )    
        .def( 
            "what"
            , (char const * ( ::SireMM::SwitchingFunction::* )(  ) const)( &::SireMM::SwitchingFunction::what ) );

}
