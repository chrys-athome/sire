// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "DihedralResID.pypp.hpp"
#include "boost/python.hpp"
#include "siremm_headers.h"
#include "SireMol/molecule.h"
#include "SireMol/residue.h"
#include "SireMol/newatom.h"
#include "SireMol/atom.h"

namespace bp = boost::python;

#include "SireQt/qdatastream.hpp"

const char* pvt_get_name(const SireMM::DihedralResID&){ return "SireMM::DihedralResID";}

void register_DihedralResID_class(){

    bp::class_< SireMM::DihedralResID >( "DihedralResID" )    
        .def( bp::init< >() )    
        .def( bp::init< SireMol::ResNum >(( bp::arg("resnum") )) )    
        .def( bp::init< SireMol::ResNum, SireMol::ResNum, SireMol::ResNum, SireMol::ResNum >(( bp::arg("resnum0"), bp::arg("resnum1"), bp::arg("resnum2"), bp::arg("resnum3") )) )    
        .def( bp::init< SireMol::Dihedral const & >(( bp::arg("dihedral") )) )    
        .def( 
            "isInterResidue"
            , (bool ( ::SireMM::DihedralResID::* )(  ) const)( &::SireMM::DihedralResID::isInterResidue ) )    
        .def( 
            "isIntraResidue"
            , (bool ( ::SireMM::DihedralResID::* )(  ) const)( &::SireMM::DihedralResID::isIntraResidue ) )    
        .def( bp::self != bp::self )    
        .def( bp::self == bp::self )    
        .def( 
            "__getitem__"
            , (::SireMol::ResNum ( ::SireMM::DihedralResID::* )( int ) const)( &::SireMM::DihedralResID::operator[] )
            , ( bp::arg("i") ) )    
        .def( 
            "resNum0"
            , (::SireMol::ResNum ( ::SireMM::DihedralResID::* )(  ) const)( &::SireMM::DihedralResID::resNum0 ) )    
        .def( 
            "resNum1"
            , (::SireMol::ResNum ( ::SireMM::DihedralResID::* )(  ) const)( &::SireMM::DihedralResID::resNum1 ) )    
        .def( 
            "resNum2"
            , (::SireMol::ResNum ( ::SireMM::DihedralResID::* )(  ) const)( &::SireMM::DihedralResID::resNum2 ) )    
        .def( 
            "resNum3"
            , (::SireMol::ResNum ( ::SireMM::DihedralResID::* )(  ) const)( &::SireMM::DihedralResID::resNum3 ) )    
        .def( 
            "residueNumbers"
            , (::QSet<SireMol::ResNum> ( ::SireMM::DihedralResID::* )(  ) const)( &::SireMM::DihedralResID::residueNumbers ) )    
        .def( 
            "size"
            , (int ( ::SireMM::DihedralResID::* )(  ) const)( &::SireMM::DihedralResID::size ) )    
        .def( "__rlshift__", &SireQt::__rlshift__QDataStream< ::SireMM::DihedralResID >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__rrshift__", &SireQt::__rrshift__QDataStream< ::SireMM::DihedralResID >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__str__", &pvt_get_name);

}
