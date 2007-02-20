// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "HarmonicSwitchingFunction.pypp.hpp"
#include "boost/python.hpp"
#include "siremm_headers.h"
#include "SireMol/molecule.h"
#include "SireMol/residue.h"
#include "SireMol/newatom.h"
#include "SireMol/atom.h"

namespace bp = boost::python;

#include "SireQt/qdatastream.hpp"

const char* pvt_get_name(const SireMM::HarmonicSwitchingFunction&){ return "SireMM::HarmonicSwitchingFunction";}

void register_HarmonicSwitchingFunction_class(){

    bp::class_< SireMM::HarmonicSwitchingFunction, bp::bases< SireMM::SwitchFuncBase > >( "HarmonicSwitchingFunction" )    
        .def( bp::init< >() )    
        .def( bp::init< double >(( bp::arg("cutoffdist") )) )    
        .def( bp::init< double, double >(( bp::arg("cutoffdist"), bp::arg("featherdist") )) )    
        .def( bp::init< double, double, double >(( bp::arg("cutoffdist"), bp::arg("elecfeather"), bp::arg("vdwfeather") )) )    
        .def( bp::init< double, double, double, double >(( bp::arg("eleccutoff"), bp::arg("elecfeather"), bp::arg("vdwcutoff"), bp::arg("vdwfeather") )) )    
        .def( 
            "electrostaticScaleFactor"
            , (double ( ::SireMM::HarmonicSwitchingFunction::* )( double ) const)( &::SireMM::HarmonicSwitchingFunction::electrostaticScaleFactor )
            , ( bp::arg("dist") ) )    
        .def( 
            "typeName"
            , (char const * (*)(  ))( &::SireMM::HarmonicSwitchingFunction::typeName ) )    
        .def( 
            "vdwScaleFactor"
            , (double ( ::SireMM::HarmonicSwitchingFunction::* )( double ) const)( &::SireMM::HarmonicSwitchingFunction::vdwScaleFactor )
            , ( bp::arg("dist") ) )    
        .def( 
            "what"
            , (char const * ( ::SireMM::HarmonicSwitchingFunction::* )(  ) const)( &::SireMM::HarmonicSwitchingFunction::what ) )    
        .staticmethod( "typeName" )    
        .def( "__rlshift__", &SireQt::__rlshift__QDataStream< ::SireMM::HarmonicSwitchingFunction >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__rrshift__", &SireQt::__rrshift__QDataStream< ::SireMM::HarmonicSwitchingFunction >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__str__", &pvt_get_name);

}
