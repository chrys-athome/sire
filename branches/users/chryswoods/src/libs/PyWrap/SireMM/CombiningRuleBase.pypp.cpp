// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "CombiningRuleBase.pypp.hpp"
#include "boost/python.hpp"
#include "siremm_headers.h"
#include "SireMol/molecule.h"
#include "SireMol/residue.h"
#include "SireMol/newatom.h"
#include "SireMol/atom.h"

namespace bp = boost::python;

void register_CombiningRuleBase_class(){

    bp::class_< SireMM::CombiningRuleBase, boost::noncopyable >( "CombiningRuleBase", bp::no_init )    
        .def( 
            "combine"
            , (void ( ::SireMM::CombiningRuleBase::* )( ::QVector<SireMM::ChargeParameter> const &,::QVector<SireMM::LJParameter> const &,::QVector<SireMM::ChargeParameter> const &,::QVector<SireMM::LJParameter> const &,::SireMM::CLJPairMatrix & ) const)( &::SireMM::CombiningRuleBase::combine )
            , ( bp::arg("chg0"), bp::arg("lj0"), bp::arg("chg1"), bp::arg("lj1"), bp::arg("cljmatrix") ) )    
        .def( 
            "combine"
            , (void ( ::SireMM::CombiningRuleBase::* )( ::QVector<SireMM::ChargeParameter> const &,::QVector<SireMM::LJParameter> const &,::SireMM::CLJPairMatrix & ) const)( &::SireMM::CombiningRuleBase::combine )
            , ( bp::arg("chgs"), bp::arg("ljs"), bp::arg("cljmatrix") ) )    
        .def( 
            "what"
            , (char const * ( ::SireMM::CombiningRuleBase::* )(  ) const)( &::SireMM::CombiningRuleBase::what ) );

}
