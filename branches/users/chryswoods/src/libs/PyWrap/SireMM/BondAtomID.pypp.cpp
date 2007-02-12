// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "BondAtomID.pypp.hpp"
#include "boost/python.hpp"
#include "siremm_headers.h"
#include "SireMol/molecule.h"
#include "SireMol/residue.h"
#include "SireMol/newatom.h"
#include "SireMol/atom.h"

namespace bp = boost::python;

void register_BondAtomID_class(){

    bp::class_< SireMM::BondAtomID >( "BondAtomID" )    
        .def( bp::init< >() )    
        .def( bp::init< QString const &, QString const & >(( bp::arg("atom0"), bp::arg("atom1") )) )    
        .def( bp::init< SireMol::Bond const & >(( bp::arg("bond") )) )    
        .def( 
            "atom0"
            , (::QString const & ( ::SireMM::BondAtomID::* )(  ) const)( &::SireMM::BondAtomID::atom0 )
            , bp::return_value_policy< bp::copy_const_reference >() )    
        .def( 
            "atom1"
            , (::QString const & ( ::SireMM::BondAtomID::* )(  ) const)( &::SireMM::BondAtomID::atom1 )
            , bp::return_value_policy< bp::copy_const_reference >() )    
        .def( bp::self != bp::self )    
        .def( bp::self == bp::self )    
        .def( 
            "__getitem__"
            , (::QString const & ( ::SireMM::BondAtomID::* )( int ) const)( &::SireMM::BondAtomID::operator[] )
            , ( bp::arg("i") )
            , bp::return_value_policy< bp::copy_const_reference >() )    
        .def( 
            "size"
            , (int ( ::SireMM::BondAtomID::* )(  ) const)( &::SireMM::BondAtomID::size ) );

}
