// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "FFCalculator.pypp.hpp"
#include "boost/python.hpp"
#include "sireff_headers.h"
#include "SireMol/molecule.h"
#include "SireMol/residue.h"
#include "SireMol/newatom.h"
#include "SireMol/atom.h"
#include "SireMol/cgatomid.h"
#include "SireMol/cutgroupnum.h"
#include "SireMol/resnumatomid.h"
#include "SireMol/resid.h"
#include "SireMol/moleculeid.h"

namespace bp = boost::python;

const char* pvt_get_name(const SireFF::FFCalculator&){ return "SireFF::FFCalculator";}

void register_FFCalculator_class(){

    bp::class_< SireFF::FFCalculator, bp::bases< SireFF::FFCalculatorBase >, boost::noncopyable >( "FFCalculator" )    
        .def( 
            "add"
            , (bool ( ::SireFF::FFCalculator::* )( ::SireMol::Molecule const &,::SireFF::ParameterMap const & ) )( &::SireFF::FFCalculator::add )
            , ( bp::arg("molecule"), bp::arg("map")=::SireFF::ParameterMap( ) ) )    
        .def( 
            "add"
            , (bool ( ::SireFF::FFCalculator::* )( ::SireMol::Residue const &,::SireFF::ParameterMap const & ) )( &::SireFF::FFCalculator::add )
            , ( bp::arg("residue"), bp::arg("map")=::SireFF::ParameterMap( ) ) )    
        .def( 
            "calculateEnergy"
            , (void ( ::SireFF::FFCalculator::* )(  ) )( &::SireFF::FFCalculator::calculateEnergy ) )    
        .def( 
            "change"
            , (bool ( ::SireFF::FFCalculator::* )( ::SireMol::Molecule const & ) )( &::SireFF::FFCalculator::change )
            , ( bp::arg("molecule") ) )    
        .def( 
            "change"
            , (bool ( ::SireFF::FFCalculator::* )( ::SireMol::Residue const & ) )( &::SireFF::FFCalculator::change )
            , ( bp::arg("residue") ) )    
        .def( 
            "forcefield"
            , (::SireFF::ForceField ( ::SireFF::FFCalculator::* )(  ) const)( &::SireFF::FFCalculator::forcefield ) )    
        .def( 
            "getEnergies"
            , (double ( ::SireFF::FFCalculator::* )( ::SireCAS::Values & ) )( &::SireFF::FFCalculator::getEnergies )
            , ( bp::arg("values") ) )    
        .def( 
            "remove"
            , (bool ( ::SireFF::FFCalculator::* )( ::SireMol::Molecule const & ) )( &::SireFF::FFCalculator::remove )
            , ( bp::arg("molecule") ) )    
        .def( 
            "remove"
            , (bool ( ::SireFF::FFCalculator::* )( ::SireMol::Residue const & ) )( &::SireFF::FFCalculator::remove )
            , ( bp::arg("residue") ) )    
        .def( 
            "replace"
            , (bool ( ::SireFF::FFCalculator::* )( ::SireMol::Molecule const &,::SireMol::Molecule const &,::SireFF::ParameterMap const & ) )( &::SireFF::FFCalculator::replace )
            , ( bp::arg("oldmol"), bp::arg("newmol"), bp::arg("map")=::SireFF::ParameterMap( ) ) )    
        .def( 
            "setForceField"
            , (bool ( ::SireFF::FFCalculator::* )( ::SireFF::ForceField const & ) )( &::SireFF::FFCalculator::setForceField )
            , ( bp::arg("forcefield") ) )    
        .def( "__str__", &pvt_get_name);

}
