// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "AngleGenerator.pypp.hpp"
#include "boost/python.hpp"
#include "siremm_headers.h"
#include "SireMol/molecule.h"
#include "SireMol/residue.h"
#include "SireMol/newatom.h"
#include "SireMol/atom.h"

namespace bp = boost::python;

void register_AngleGenerator_class(){

    bp::class_< SireMM::AngleGenerator >( "AngleGenerator" )    
        .def( bp::init< >() )    
        .def( 
            "generate"
            , (void ( ::SireMM::AngleGenerator::* )( ::SireMol::Molecule const &,::SireMM::MolAngleInfo & ) const)( &::SireMM::AngleGenerator::generate )
            , ( bp::arg("mol"), bp::arg("angleinfo") ) )    
        .def( 
            "typeName"
            , (char const * (*)(  ))( &::SireMM::AngleGenerator::typeName ) )    
        .def( 
            "what"
            , (char const * ( ::SireMM::AngleGenerator::* )(  ) const)( &::SireMM::AngleGenerator::what ) )    
        .staticmethod( "typeName" );

}
