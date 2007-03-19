// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "ChargeParameter.pypp.hpp"
#include "boost/python.hpp"
#include "siremm_headers.h"
#include "SireMol/molecule.h"
#include "SireMol/residue.h"
#include "SireMol/newatom.h"
#include "SireMol/atom.h"

namespace bp = boost::python;

SireMM::ChargeParameter __copy__(const SireMM::ChargeParameter &other){ return SireMM::ChargeParameter(other); }

#include "SireQt/qdatastream.hpp"

#include "SirePy/str.hpp"

void register_ChargeParameter_class(){

    bp::class_< SireMM::ChargeParameter >( "ChargeParameter" )    
        .def( bp::init< >() )    
        .def( bp::init< double >(( bp::arg("charge") )) )    
        .def( 
            "charge"
            , (double ( ::SireMM::ChargeParameter::* )(  ) const)( &::SireMM::ChargeParameter::charge ) )    
        .def( 
            "dummy"
            , (::SireMM::ChargeParameter (*)(  ))( &::SireMM::ChargeParameter::dummy ) )    
        .def( 
            "isDummy"
            , (bool ( ::SireMM::ChargeParameter::* )(  ) const)( &::SireMM::ChargeParameter::isDummy ) )    
        .def( bp::self != bp::self )    
        .def( bp::self == bp::self )    
        .def( 
            "toString"
            , (::QString ( ::SireMM::ChargeParameter::* )(  ) const)( &::SireMM::ChargeParameter::toString ) )    
        .def( 
            "zeroCharge"
            , (bool ( ::SireMM::ChargeParameter::* )(  ) const)( &::SireMM::ChargeParameter::zeroCharge ) )    
        .staticmethod( "dummy" )    
        .def( "__copy__", &__copy__)    
        .def( "__rlshift__", &SireQt::__rlshift__QDataStream< ::SireMM::ChargeParameter >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__rrshift__", &SireQt::__rrshift__QDataStream< ::SireMM::ChargeParameter >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__str__", &SirePy::__str__< ::SireMM::ChargeParameter > );

}
