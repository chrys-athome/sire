// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "CombiningRules.pypp.hpp"
#include "boost/python.hpp"
#include "siremm_headers.h"
#include "SireMol/molecule.h"
#include "SireMol/residue.h"
#include "SireMol/newatom.h"
#include "SireMol/atom.h"

namespace bp = boost::python;

void register_CombiningRules_class(){

    bp::class_< SireMM::CombiningRules >( "CombiningRules" )    
        .def( bp::init< >() )    
        .def( bp::init< SireMM::CombiningRuleBase const & >(( bp::arg("rules") )) )    
        .def( 
            "combine"
            , (void ( ::SireMM::CombiningRules::* )( ::QVector<SireMM::ChargeParameter> const &,::QVector<SireMM::LJParameter> const &,::QVector<SireMM::ChargeParameter> const &,::QVector<SireMM::LJParameter> const &,::SireMM::CLJPairMatrix & ) const)( &::SireMM::CombiningRules::combine )
            , ( bp::arg("chg0"), bp::arg("lj0"), bp::arg("chg1"), bp::arg("lj1"), bp::arg("cljmatrix") ) )    
        .def( 
            "combine"
            , (void ( ::SireMM::CombiningRules::* )( ::QVector<SireMM::ChargeParameter> const &,::QVector<SireMM::LJParameter> const &,::SireMM::CLJPairMatrix & ) const)( &::SireMM::CombiningRules::combine )
            , ( bp::arg("chgs"), bp::arg("ljs"), bp::arg("cljmatrix") ) )    
        .def( 
            "what"
            , (char const * ( ::SireMM::CombiningRules::* )(  ) const)( &::SireMM::CombiningRules::what ) );

}
