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

SireMM::CombiningRules __copy__(const SireMM::CombiningRules &other){ return SireMM::CombiningRules(other); }

#include "SireQt/qdatastream.hpp"

const char* pvt_get_name(const SireMM::CombiningRules&){ return "SireMM::CombiningRules";}

void register_CombiningRules_class(){

    bp::class_< SireMM::CombiningRules >( "CombiningRules" )    
        .def( bp::init< >() )    
        .def( bp::init< SireMM::CombiningRuleBase const & >(( bp::arg("rules") )) )    
        .def( bp::init< SireBase::Property const & >(( bp::arg("property") )) )    
        .def( 
            "combine"
            , (void ( ::SireMM::CombiningRules::* )( ::QVector<SireMM::CLJParameter> const &,::QVector<SireMM::CLJParameter> const &,::SireMM::CLJPairMatrix & ) const)( &::SireMM::CombiningRules::combine )
            , ( bp::arg("clj0"), bp::arg("clj1"), bp::arg("cljmatrix") ) )    
        .def( 
            "combine"
            , (void ( ::SireMM::CombiningRules::* )( ::QVector<SireMM::CLJParameter> const &,::SireMM::CLJPairMatrix & ) const)( &::SireMM::CombiningRules::combine )
            , ( bp::arg("cljs"), bp::arg("cljmatrix") ) )    
        .def( 
            "combine"
            , (void ( ::SireMM::CombiningRules::* )( ::QVector<SireMM::ChargeParameter> const &,::QVector<SireMM::LJParameter> const &,::QVector<SireMM::ChargeParameter> const &,::QVector<SireMM::LJParameter> const &,::SireMM::CLJPairMatrix & ) const)( &::SireMM::CombiningRules::combine )
            , ( bp::arg("chg0"), bp::arg("lj0"), bp::arg("chg1"), bp::arg("lj1"), bp::arg("cljmatrix") ) )    
        .def( 
            "combine"
            , (void ( ::SireMM::CombiningRules::* )( ::QVector<SireMM::ChargeParameter> const &,::QVector<SireMM::LJParameter> const &,::SireMM::CLJPairMatrix & ) const)( &::SireMM::CombiningRules::combine )
            , ( bp::arg("chgs"), bp::arg("ljs"), bp::arg("cljmatrix") ) )    
        .def( 
            "combine"
            , (void ( ::SireMM::CombiningRules::* )( ::QVector<SireMM::LJParameter> const &,::QVector<SireMM::LJParameter> const &,::SireMM::LJPairMatrix & ) const)( &::SireMM::CombiningRules::combine )
            , ( bp::arg("lj0"), bp::arg("lj1"), bp::arg("ljmatrix") ) )    
        .def( 
            "combine"
            , (void ( ::SireMM::CombiningRules::* )( ::QVector<SireMM::LJParameter> const &,::SireMM::LJPairMatrix & ) const)( &::SireMM::CombiningRules::combine )
            , ( bp::arg("ljs"), bp::arg("ljmatrix") ) )    
        .def( "as__scope_SireBase_scope_Property", &SireMM::CombiningRules::operator ::SireBase::Property  )    
        .def( 
            "what"
            , (char const * ( ::SireMM::CombiningRules::* )(  ) const)( &::SireMM::CombiningRules::what ) )    
        .def( "__copy__", &__copy__)    
        .def( "__rlshift__", &SireQt::__rlshift__QDataStream< ::SireMM::CombiningRules >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__rrshift__", &SireQt::__rrshift__QDataStream< ::SireMM::CombiningRules >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__str__", &pvt_get_name);

}
